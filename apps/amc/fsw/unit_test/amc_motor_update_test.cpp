/****************************************************************************
*
*   Copyright (c) 2017 Windhover Labs, L.L.C. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
* 3. Neither the name Windhover Labs nor the names of its 
*    contributors may be used to endorse or promote products derived 
*    from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/

#include "cfe.h"
#include "amc_msg.h"
#include "uttest.h"
#include "ut_osapi_stubs.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_sb_hooks.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_es_hooks.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_cfe_time_stubs.h"
#include "ut_amc_custom_stubs.h"
#include "ut_amc_custom_hooks.h"

#include "amc_motor_update_test.hpp"
#include "amc_test_utils.hpp"

#include <time.h>
#include <unistd.h>


uint32  ActuatorOutputs_SendCnt = 0;


/**************************************************************************
 * Tests for AMC UpdateMotors()
 **************************************************************************/
/**
 * Test AMC UpdateMotors(), SendMsgHook
 */
int32 Test_AMC_UpdateMotors_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    unsigned char*            pBuff = NULL;
    uint16                    msgLen = 0;
    int                       i = 0;
    CFE_SB_MsgId_t            MsgId;
    time_t                    localTime;
    struct tm                 *loc_time;
    CFE_TIME_SysTime_t        TimeFromMsg;
    PX4_ActuatorOutputsMsg_t  ActOuts;

    pBuff = (unsigned char*)MsgPtr;

    msgLen = CFE_SB_GetTotalMsgLength(MsgPtr);
    printf("###UpdateMotors_SendMsgHook: MsgLen(%u)\n", msgLen);
    for (i = 0; i < msgLen; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff++;
    }
    printf("\n");

    TimeFromMsg = CFE_SB_GetMsgTime(MsgPtr);
    localTime = AMC_Test_GetTimeFromMsg(TimeFromMsg);
    loc_time = localtime(&localTime);
    printf("TimeFromMessage: %s", asctime(loc_time));

    MsgId = CFE_SB_GetMsgId(MsgPtr);
    switch (MsgId)
    {
        case PX4_ACTUATOR_OUTPUTS_MID:
        {
            ActuatorOutputs_SendCnt ++;
            CFE_PSP_MemCpy((void*)&ActOuts, (void*)MsgPtr, sizeof(ActOuts));

            printf("Sent PX4_ACTUATOR_OUTPUTS_MID:\n");
            localTime = AMC_Test_GetTimeFromTimestamp(ActOuts.Timestamp);
            loc_time = localtime(&localTime);
            printf("Timestamp: %s", asctime(loc_time));
            printf("Count: %lu\n", ActOuts.Count);
            for (i = 0; i < PX4_ACTUATOR_OUTPUTS_MAX; i++)
            {
                printf("Output[%d]: %f\n", i, ActOuts.Output[i]);
            }
            break;
        }
        case AMC_HK_TLM_MID:
        {
            printf("Sent AMC_HK_TLM_MID\n");
            break;
        }
        default:
        {
            printf("Sent MID(0x%04X)\n", MsgId);
            break;
        }
    }
}


/**
 * Test AMC UpdateMotors(), Nominal_UnArmed
 */
void AMC_UpdateMotors_Nominal_UnArmed(void)
{
    AMC  oAMC;

    int32                   DataPipe;
    PX4_ActuatorArmedMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe(AMC_DATA_PIPE_NAME);
    CFE_SB_InitMsg((void*)&InMsg, PX4_ACTUATOR_ARMED_MID, sizeof(InMsg), TRUE);
    InMsg.Timestamp = PX4LIB_GetPX4TimeUs();
    InMsg.Armed = FALSE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&InMsg);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                              (void*)&Test_AMC_UpdateMotors_SendMsgHook);

    /* Execute the function being tested */
    oAMC.InitApp();

    SetMotorOutputs_CalledCnt = 0;
    ActuatorOutputs_SendCnt = 0;
    oAMC.ProcessDataPipe();
    UtAssert_True(ActuatorOutputs_SendCnt == 2,
                  "UpdateMotors, Nominal_UnArmed: Sent Actuator Output #1");

    usleep(RAMP_TIME_US);
    InMsg.Timestamp = PX4LIB_GetPX4TimeUs();
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    ActuatorOutputs_SendCnt = 0;
    oAMC.ProcessDataPipe();
    UtAssert_True(ActuatorOutputs_SendCnt == 2,
                  "UpdateMotors, Nominal_UnArmed: Sent Actuator Output #2");

    UtAssert_True(SetMotorOutputs_CalledCnt == 4,
             "UpdateMotors, Nominal_UnArmed: SetMotorOutputs called count");
}


/**
 * Test AMC UpdateMotors(), Nominal_Armed
 */
void AMC_UpdateMotors_Nominal_Armed(void)
{
    AMC  oAMC;

    int32                   DataPipe;
    PX4_ActuatorArmedMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe(AMC_DATA_PIPE_NAME);
    CFE_SB_InitMsg((void*)&InMsg, PX4_ACTUATOR_ARMED_MID, sizeof(InMsg), TRUE);
    InMsg.Timestamp = PX4LIB_GetPX4TimeUs();
    InMsg.Armed = TRUE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&InMsg);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                              (void*)&Test_AMC_UpdateMotors_SendMsgHook);

    /* Execute the function being tested */
    oAMC.InitApp();

    SetMotorOutputs_CalledCnt = 0;
    ActuatorOutputs_SendCnt = 0;
    oAMC.ProcessDataPipe();
    UtAssert_True(ActuatorOutputs_SendCnt == 2,
                  "UpdateMotors, Nominal_Armed: Sent Actuator Output #1");

    usleep(RAMP_TIME_US);
    InMsg.Timestamp = PX4LIB_GetPX4TimeUs();
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    ActuatorOutputs_SendCnt = 0;
    oAMC.ProcessDataPipe();
    UtAssert_True(ActuatorOutputs_SendCnt == 2,
                  "UpdateMotors, Nominal_Armed: Sent Actuator Output #2");

    UtAssert_True(SetMotorOutputs_CalledCnt == 4,
             "UpdateMotors, Nominal_Armed: SetMotorOutputs called count");
}


/**
 * Test AMC UpdateMotors(), Lockdown_UnArmed
 */
void AMC_UpdateMotors_Lockdown_UnArmed(void)
{
    AMC  oAMC;

    int32                   DataPipe;
    PX4_ActuatorArmedMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe(AMC_DATA_PIPE_NAME);
    CFE_SB_InitMsg((void*)&InMsg, PX4_ACTUATOR_ARMED_MID, sizeof(InMsg), TRUE);
    InMsg.Timestamp = PX4LIB_GetPX4TimeUs();
    InMsg.Armed = FALSE;
    InMsg.Lockdown = TRUE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&InMsg);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                              (void*)&Test_AMC_UpdateMotors_SendMsgHook);

    /* Execute the function being tested */
    oAMC.InitApp();

    SetMotorOutputs_CalledCnt = 0;
    ActuatorOutputs_SendCnt = 0;
    oAMC.ProcessDataPipe();
    UtAssert_True(ActuatorOutputs_SendCnt == 2,
                  "UpdateMotors, Lockdown_UnArmed: Sent Actuator Output #1");

    usleep(RAMP_TIME_US);
    InMsg.Timestamp = PX4LIB_GetPX4TimeUs();
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    ActuatorOutputs_SendCnt = 0;
    oAMC.ProcessDataPipe();
    UtAssert_True(ActuatorOutputs_SendCnt == 2,
                  "UpdateMotors, Lockdown_UnArmed: Sent Actuator Output #2");

    UtAssert_True(SetMotorOutputs_CalledCnt == 8,
             "UpdateMotors, Lockdown_UnArmed: SetMotorOutputs called count");
}


/**
 * Test AMC UpdateMotors(), Lockdown_Armed
 */
void AMC_UpdateMotors_Lockdown_Armed(void)
{
    AMC  oAMC;

    int32                   DataPipe;
    PX4_ActuatorArmedMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe(AMC_DATA_PIPE_NAME);
    CFE_SB_InitMsg((void*)&InMsg, PX4_ACTUATOR_ARMED_MID, sizeof(InMsg), TRUE);
    InMsg.Timestamp = PX4LIB_GetPX4TimeUs();
    InMsg.Armed = TRUE;
    InMsg.Lockdown = TRUE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&InMsg);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                              (void*)&Test_AMC_UpdateMotors_SendMsgHook);

    /* Execute the function being tested */
    oAMC.InitApp();

    SetMotorOutputs_CalledCnt = 0;
    ActuatorOutputs_SendCnt = 0;
    oAMC.ProcessDataPipe();
    UtAssert_True(ActuatorOutputs_SendCnt == 2,
                  "UpdateMotors, Lockdown_Armed: Sent Actuator Output #1");

    usleep(RAMP_TIME_US);
    InMsg.Timestamp = PX4LIB_GetPX4TimeUs();
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    ActuatorOutputs_SendCnt = 0;
    oAMC.ProcessDataPipe();
    UtAssert_True(ActuatorOutputs_SendCnt == 2,
                  "UpdateMotors, Lockdown_Armed: Sent Actuator Output #2");

    UtAssert_True(SetMotorOutputs_CalledCnt == 8,
             "UpdateMotors, Lockdown_Armed: SetMotorOutputs called count");
}

/**
 * Test AMC UpdateMotors(), InEscCalibrationMode_UnArmed
 */
void AMC_UpdateMotors_InEscCalibrationMode_UnArmed(void)
{
    AMC  oAMC;

    int32                      DataPipe;
    PX4_ActuatorArmedMsg_t     InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe(AMC_DATA_PIPE_NAME);
    CFE_SB_InitMsg((void*)&InMsg, PX4_ACTUATOR_ARMED_MID, sizeof(InMsg), TRUE);
    InMsg.Timestamp = PX4LIB_GetPX4TimeUs();
    InMsg.Armed = FALSE;
    InMsg.InEscCalibrationMode = TRUE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&InMsg);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                              (void*)&Test_AMC_UpdateMotors_SendMsgHook);

    /* Execute the function being tested */
    oAMC.InitApp();

    SetMotorOutputs_CalledCnt = 0;
    ActuatorOutputs_SendCnt = 0;
    oAMC.ProcessDataPipe();
    UtAssert_True(ActuatorOutputs_SendCnt == 2,
      "UpdateMotors, InEscCalibrationMode_UnArmed: Sent Actuator Output #1");

    usleep(RAMP_TIME_US);
    InMsg.Timestamp = PX4LIB_GetPX4TimeUs();
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    ActuatorOutputs_SendCnt = 0;
    oAMC.ProcessDataPipe();
    UtAssert_True(ActuatorOutputs_SendCnt == 2,
      "UpdateMotors, InEscCalibrationMode_UnArmed: Sent Actuator Output #2");

    UtAssert_True(SetMotorOutputs_CalledCnt == 0,
     "UpdateMotors, InEscCalibrationMode_UnArmed: SetMotorOutputs called count");
}


/**
 * Test AMC UpdateMotors(), InEscCalibrationMode_Armed
 */
void AMC_UpdateMotors_InEscCalibrationMode_Armed(void)
{
    AMC  oAMC;

    int32                      DataPipe;
    PX4_ActuatorArmedMsg_t     InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe(AMC_DATA_PIPE_NAME);
    CFE_SB_InitMsg((void*)&InMsg, PX4_ACTUATOR_ARMED_MID, sizeof(InMsg), TRUE);
    InMsg.Timestamp = PX4LIB_GetPX4TimeUs();
    InMsg.Armed = TRUE;
    InMsg.InEscCalibrationMode = TRUE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&InMsg);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                              (void*)&Test_AMC_UpdateMotors_SendMsgHook);

    /* Execute the function being tested */
    oAMC.InitApp();

    SetMotorOutputs_CalledCnt = 0;
    ActuatorOutputs_SendCnt = 0;
    oAMC.ProcessDataPipe();
    UtAssert_True(ActuatorOutputs_SendCnt == 2,
      "UpdateMotors, InEscCalibrationMode_Armed: Sent Actuator Output #1");

    usleep(RAMP_TIME_US);
    InMsg.Timestamp = PX4LIB_GetPX4TimeUs();
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    ActuatorOutputs_SendCnt = 0;
    oAMC.ProcessDataPipe();
    UtAssert_True(ActuatorOutputs_SendCnt == 2,
      "UpdateMotors, InEscCalibrationMode_Armed: Sent Actuator Output #2");

    UtAssert_True(SetMotorOutputs_CalledCnt == 0,
     "UpdateMotors, InEscCalibrationMode_Armed: SetMotorOutputs called count");
}


void AMC_Motor_Update_Test_AddTestCases(void)
{
    UtTest_Add(AMC_UpdateMotors_Nominal_UnArmed,
               AMC_Test_Setup, AMC_Test_TearDown,
               "AMC_UpdateMotors_Nominal_UnArmed");
    UtTest_Add(AMC_UpdateMotors_Nominal_Armed,
               AMC_Test_Setup, AMC_Test_TearDown,
               "AMC_UpdateMotors_Nominal_Armed");
    UtTest_Add(AMC_UpdateMotors_Lockdown_UnArmed,
               AMC_Test_Setup, AMC_Test_TearDown,
               "AMC_UpdateMotors_Lockdown_UnArmed");
    UtTest_Add(AMC_UpdateMotors_Lockdown_Armed,
               AMC_Test_Setup, AMC_Test_TearDown,
               "AMC_UpdateMotors_Lockdown_Armed");
    UtTest_Add(AMC_UpdateMotors_InEscCalibrationMode_UnArmed,
               AMC_Test_Setup, AMC_Test_TearDown,
               "AMC_UpdateMotors_InEscCalibrationMode_UnArmed");
    UtTest_Add(AMC_UpdateMotors_InEscCalibrationMode_Armed,
               AMC_Test_Setup, AMC_Test_TearDown,
               "AMC_UpdateMotors_InEscCalibrationMode_Armed");
}
