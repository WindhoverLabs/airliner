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

#include "aspd4525_cmds_test.h"
#include "aspd4525_test_utils.h"
#include "aspd4525_math.h"
#include "aspd4525_version.h"

#include "aspd4525_msg.h"

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

#include <float.h>


/**************************************************************************
 * Tests for ASPD4525_ProcessNewCmds()
 **************************************************************************/
/**
 * Test ASPD4525_ProcessNewCmds, InvalidCmd
 */
void Test_ASPD4525_ProcessNewCmds_InvalidCmd(void)
{
    int32                CmdPipe;
    ASPD4525_NoArgCmd_t  CmdMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(ASPD4525_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, 0, sizeof(CmdMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    ASPD4525_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, ASPD4525_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    ASPD4525_AppMain();

    sprintf(expectedEvent, "Recvd invalid CMD msgId (0x%04X)", 0);

    /* Verify results */
    UtAssert_EventSent(ASPD4525_MSGID_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "ProcessNewCmds, InvalidCmd");
}


/**
 * Test ASPD4525_ProcessNewCmds, InvalidCmdCode
 */
void Test_ASPD4525_ProcessNewCmds_InvalidCmdCode(void)
{
    int32                CmdPipe;
    ASPD4525_NoArgCmd_t  CmdMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(ASPD4525_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, ASPD4525_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)100);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    ASPD4525_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, ASPD4525_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    ASPD4525_AppMain();

    sprintf(expectedEvent, "Recvd invalid cmdId (%u)", 100);

    /* Verify results */
    UtAssert_EventSent(ASPD4525_MSGID_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "ProcessNewCmds, InvalidCmd");
}


/**
 * Test ASPD4525_ProcessNewCmds, PipeError
 */
void Test_ASPD4525_ProcessNewCmds_PipeError(void)
{
    int32                SchPipe;
    int32                expected = CFE_SB_PIPE_RD_ERR;
    ASPD4525_NoArgCmd_t  InMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    SchPipe = Ut_CFE_SB_CreatePipe(ASPD4525_SCH_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, ASPD4525_WAKEUP_MID, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)SchPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, expected, 2);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    ASPD4525_AppMain();

    sprintf(expectedEvent, "CMD pipe read error (0x%08X)",
            (unsigned int)CFE_SB_PIPE_RD_ERR);

    /* Verify results */
    UtAssert_EventSent(ASPD4525_PIPE_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "ProcessNewCmds, PipeError");
}


/**
 * Test ASPD4525_ProcessNewCmds, Noop
 */
void Test_ASPD4525_ProcessNewCmds_Noop(void)
{
    int32                CmdPipe;
    ASPD4525_NoArgCmd_t  CmdMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CmdPipe = Ut_CFE_SB_CreatePipe(ASPD4525_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, ASPD4525_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)ASPD4525_NOOP_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    ASPD4525_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, ASPD4525_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    ASPD4525_AppMain();

    sprintf(expectedEvent, "Recvd NOOP cmd (%u), Version %d.%d.%d.%d",
                           ASPD4525_NOOP_CC, ASPD4525_MAJOR_VERSION,
                           ASPD4525_MINOR_VERSION, ASPD4525_REVISION,
                           ASPD4525_MISSION_REV);

    /* Verify results */
    UtAssert_EventSent(ASPD4525_CMD_INF_EID, CFE_EVS_INFORMATION,
                       expectedEvent, "ProcessNewCmds, Noop");
}


/**
 * Test ASPD4525_ProcessNewCmds, Reset
 */
void Test_ASPD4525_ProcessNewCmds_Reset(void)
{
    int32                CmdPipe;
    ASPD4525_NoArgCmd_t  CmdMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CmdPipe = Ut_CFE_SB_CreatePipe(ASPD4525_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, ASPD4525_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)ASPD4525_RESET_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    ASPD4525_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, ASPD4525_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    ASPD4525_AppMain();

    sprintf(expectedEvent, "Recvd RESET cmd (%u)", ASPD4525_RESET_CC);

    /* Verify results */
    UtAssert_EventSent(ASPD4525_CMD_INF_EID, CFE_EVS_INFORMATION,
                       expectedEvent, "ProcessNewCmds, Reset");
}


/**
 * Test ASPD4525_ProcessNewCmds, ASPD4525_MAN_CALIB_CC
 */
void Test_ASPD4525_ProcessNewCmds_ASPD4525_MAN_CALIB_CC(void)
{
    int32                      iStatus = CFE_SUCCESS;
    int32                      CmdPipe;
    ASPD4525_ManCalibArgCmd_t  CmdMsg;
    ASPD4525_ConfigTblEntry_t  *pConfig = NULL;

    CmdPipe = Ut_CFE_SB_CreatePipe(ASPD4525_CMD_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, ASPD4525_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                       (uint16)ASPD4525_MAN_CALIB_CC);

    /* Execute the function being tested */
    iStatus = ASPD4525_InitApp();

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE,
                      "ProcessNewCmds, ASPD4525_MAN_CALIB_CC, InitApp");

        return;
    }

    pConfig = &ASPD4525_AppData.ConfigTblPtr[0];
    CmdMsg.fAirGasConstantR_SI = pConfig->fAirGasConstantR_SI + 1.0f;
    CmdMsg.fAirMolarMass_SI = pConfig->fAirMolarMass_SI + 1.0f;
    CmdMsg.fPressureMaximum_PSI = pConfig->fPressureMaximum_PSI + 1.0f;
    CmdMsg.fPressureMinimum_PSI = pConfig->fPressureMinimum_PSI + 1.0f;
    CmdMsg.fTemperatureMaximum_Celcius =
                           pConfig->fTemperatureMaximum_Celcius + 1.0f;
    CmdMsg.fTemperatureMinimum_Celcius =
                           pConfig->fTemperatureMinimum_Celcius + 1.0f;
    CmdMsg.fGravitationalAccereleration_SI =
                       pConfig->fGravitationalAccereleration_SI + 1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    ASPD4525_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    ASPD4525_ProcessNewCmds();

    /* Verify results */
    UtAssert_DoubleCmpAbs(pConfig->fAirGasConstantR_SI,
            CmdMsg.fAirGasConstantR_SI, FLT_EPSILON,
            "ProcessNewCmds, ASPD4525_MAN_CALIB_CC, fPressureMinimum_PSI");
    UtAssert_DoubleCmpAbs(pConfig->fAirMolarMass_SI,
            CmdMsg.fAirMolarMass_SI, FLT_EPSILON,
            "ProcessNewCmds, ASPD4525_MAN_CALIB_CC, fAirMolarMass_SI");
    UtAssert_DoubleCmpAbs(pConfig->fPressureMaximum_PSI,
            CmdMsg.fPressureMaximum_PSI, FLT_EPSILON,
            "ProcessNewCmds, ASPD4525_MAN_CALIB_CC, fPressureMaximum_PSI");
    UtAssert_DoubleCmpAbs(pConfig->fPressureMinimum_PSI,
            CmdMsg.fPressureMinimum_PSI, FLT_EPSILON,
            "ProcessNewCmds, ASPD4525_MAN_CALIB_CC, fPressureMinimum_PSI");
    UtAssert_DoubleCmpAbs(pConfig->fTemperatureMaximum_Celcius,
            CmdMsg.fTemperatureMaximum_Celcius, FLT_EPSILON,
            "ProcessNewCmds, ASPD4525_MAN_CALIB_CC,"
            " fTemperatureMaximum_Celcius");
    UtAssert_DoubleCmpAbs(pConfig->fTemperatureMinimum_Celcius,
            CmdMsg.fTemperatureMinimum_Celcius, FLT_EPSILON,
            "ProcessNewCmds, ASPD4525_MAN_CALIB_CC,"
            " fTemperatureMinimum_Celcius");
    UtAssert_DoubleCmpAbs(pConfig->fGravitationalAccereleration_SI,
            CmdMsg.fGravitationalAccereleration_SI, FLT_EPSILON,
            "ProcessNewCmds, ASPD4525_MAN_CALIB_CC,"
            " fGravitationalAccereleration_SI");
    UtAssert_True(ASPD4525_AppData.HkTlm.usCmdCnt == 1,
            "ProcessNewCmds, ASPD4525_MAN_CALIB_CC, usCmdCnt");
}


/**
 * Test ASPD4525_ProcessNewCmds, ASPD4525_LAB_CALIB_CC
 */
void Test_ASPD4525_ProcessNewCmds_ASPD4525_LAB_CALIB_CC(void)
{
    int32                      iStatus = CFE_SUCCESS;
    int32                      CmdPipe;
    ASPD4525_LabCalibArgCmd_t  CmdMsg;
    ASPD4525_ConfigTblEntry_t  Config[ASPD4525_CONFIG_TABLE_MAX_ENTRIES];

    CmdPipe = Ut_CFE_SB_CreatePipe(ASPD4525_CMD_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, ASPD4525_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)ASPD4525_LAB_CALIB_CC);

    /* Execute the function being tested */
    iStatus = ASPD4525_InitApp();

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE,
                      "ProcessNewCmds, ASPD4525_LAB_CALIB_CC, InitApp");

        return;
    }

    CmdMsg.uPCountLow = 0x2000;
    CmdMsg.fVelocityLow_SI = 5.0f;
    CmdMsg.uPCountHigh = 0x3000;
    CmdMsg.fVelocityHigh_SI = 10.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    ASPD4525_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    ASPD4525_ProcessNewCmds();

    ASPD4525_MATH_CalibrateAirSpeedPressures(&Config[0], &CmdMsg);

    /* Verify results */
    UtAssert_DoubleCmpAbs(Config[0].fPressureMaximum_PSI,
            ASPD4525_AppData.HkTlm.fPressureMaximum_PSI, FLT_EPSILON,
            "ProcessNewCmds, ASPD4525_LAB_CALIB_CC, fPressureMaximum_PSI");
    UtAssert_DoubleCmpAbs(Config[0].fPressureMinimum_PSI,
            ASPD4525_AppData.HkTlm.fPressureMinimum_PSI, FLT_EPSILON,
            "ProcessNewCmds, ASPD4525_LAB_CALIB_CC, fPressureMinimum_PSI");
    UtAssert_True(ASPD4525_AppData.HkTlm.usCmdCnt == 1,
            "ProcessNewCmds, ASPD4525_LAB_CALIB_CC, usCmdCnt");
}


/**
 * Test ASPD4525_ProcessNewCmds, ASPD4525_TEMP_CALIB_CC
 */
void Test_ASPD4525_ProcessNewCmds_ASPD4525_TEMP_CALIB_CC(void)
{
    int32                      iStatus = CFE_SUCCESS;
    int32                      CmdPipe;
    ASPD4525_TempCalibArgCmd_t CmdMsg;
    ASPD4525_ConfigTblEntry_t  Config[ASPD4525_CONFIG_TABLE_MAX_ENTRIES];

    CmdPipe = Ut_CFE_SB_CreatePipe(ASPD4525_CMD_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, ASPD4525_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)ASPD4525_TEMP_CALIB_CC);

    /* Execute the function being tested */
    iStatus = ASPD4525_InitApp();

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE,
                      "ProcessNewCmds, ASPD4525_TEMP_CALIB_CC, InitApp");

        return;
    }

    CmdMsg.uTCountLow = 0x0050;
    CmdMsg.fTemperatureLow_Celcius = 10.0;
    CmdMsg.uTCountHigh = 0x00ff;
    CmdMsg.fTemperatureHigh_Celcius = 15.0;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    ASPD4525_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    ASPD4525_ProcessNewCmds();

    ASPD4525_MATH_CalibrateTemperature(&Config[0], &CmdMsg);

    /* Verify results */
    UtAssert_DoubleCmpAbs(Config[0].fTemperatureMaximum_Celcius,
            ASPD4525_AppData.HkTlm.fTemperatureMaximum_Celcius, FLT_EPSILON,
            "ProcessNewCmds, ASPD4525_TEMP_CALIB_CC,"
            " fTemperatureMaximum_Celcius");
    UtAssert_DoubleCmpAbs(Config[0].fTemperatureMinimum_Celcius,
            ASPD4525_AppData.HkTlm.fTemperatureMinimum_Celcius, FLT_EPSILON,
            "ProcessNewCmds, ASPD4525_TEMP_CALIB_CC,"
            " fTemperatureMinimum_Celcius");
    UtAssert_True(ASPD4525_AppData.HkTlm.usCmdCnt == 1,
            "ProcessNewCmds, ASPD4525_TEMP_CALIB_CC, usCmdCnt");
}


/**
 * Test ASPD4525_ProcessNewCmds, ASPD4525_PHYSICS_CALIB_CC
 */
void Test_ASPD4525_ProcessNewCmds_ASPD4525_PHYSICS_CALIB_CC(void)
{
    int32                         iStatus = CFE_SUCCESS;
    int32                         CmdPipe;
    ASPD4525_PhysicsCalibArgCmd_t CmdMsg;
    ASPD4525_ConfigTblEntry_t     *pConfig = NULL;

    CmdPipe = Ut_CFE_SB_CreatePipe(ASPD4525_CMD_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, ASPD4525_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)ASPD4525_PHYSICS_CALIB_CC);

    /* Execute the function being tested */
    iStatus = ASPD4525_InitApp();

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE,
                      "ProcessNewCmds, ASPD4525_PHYSICS_CALIB_CC, InitApp");

        return;
    }

    pConfig = &ASPD4525_AppData.ConfigTblPtr[0];
    CmdMsg.fAirGasConstantR_SI = pConfig->fAirGasConstantR_SI + 1.0f;
    CmdMsg.fAirMolarMass_SI = pConfig->fAirMolarMass_SI + 1.0f;
    CmdMsg.fGravitationalAccereleration_SI =
            pConfig->fGravitationalAccereleration_SI + 1.0f;
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    ASPD4525_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    ASPD4525_ProcessNewCmds();

    /* Verify results */
    UtAssert_DoubleCmpAbs(pConfig->fAirGasConstantR_SI,
            CmdMsg.fAirGasConstantR_SI, FLT_EPSILON,
            "ProcessNewCmds, ASPD4525_PHYSICS_CALIB_CC,"
            " fAirGasConstantR_SI");
    UtAssert_DoubleCmpAbs(pConfig->fAirMolarMass_SI,
            CmdMsg.fAirMolarMass_SI, FLT_EPSILON,
            "ProcessNewCmds, ASPD4525_PHYSICS_CALIB_CC,"
            " fAirMolarMass_SI");
    UtAssert_DoubleCmpAbs(pConfig->fGravitationalAccereleration_SI,
            CmdMsg.fGravitationalAccereleration_SI, FLT_EPSILON,
            "ProcessNewCmds, ASPD4525_PHYSICS_CALIB_CC,"
            " fGravitationalAccereleration_SI");
    UtAssert_True(ASPD4525_AppData.HkTlm.usCmdCnt == 1,
            "ProcessNewCmds, ASPD4525_PHYSICS_CALIB_CC, usCmdCnt");
}


/**
 * Test ASPD4525_ProcessNewCmds, ASPD4525_AIR_COL_CALIB_CC
 */
void Test_ASPD4525_ProcessNewCmds_ASPD4525_AIR_COL_CALIB_CC(void)
{
    boolean                       bResult = TRUE;
    int32                         iStatus = CFE_SUCCESS;
    int32                         CmdPipe;
    int                           i;
    int                           floatCnt = 0;
    float                         *pFloat = NULL;
    float                         *pCmd = NULL;
    ASPD4525_AirColCalibArgCmd_t  CmdMsg;
    ASPD4525_ConfigTblEntry_t     *pConfig = NULL;

    CmdPipe = Ut_CFE_SB_CreatePipe(ASPD4525_CMD_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, ASPD4525_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)ASPD4525_AIR_COL_CALIB_CC);

    /* Execute the function being tested */
    iStatus = ASPD4525_InitApp();

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE,
                      "ProcessNewCmds, ASPD4525_AIR_COL_CALIB_CC, InitApp");

        return;
    }

    pConfig = &ASPD4525_AppData.ConfigTblPtr[0];
    floatCnt = ASPD4525_NUM_ATM_LAYERS * 4;
    pFloat = pConfig->fAltitudeMeters_bs;
    pCmd = &CmdMsg.fh_b0;
    for (i = 0; i < floatCnt; i++)
    {
        *pCmd = *pFloat + 1.0f;
        pCmd ++;
        pFloat ++;
    }

    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    ASPD4525_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    ASPD4525_ProcessNewCmds();

    /* Verify results */
    pFloat = pConfig->fAltitudeMeters_bs;
    pCmd = &CmdMsg.fh_b0;
    for (i = 0; i < floatCnt; i++)
    {
        if (*pFloat != *pCmd)
        {
            bResult = FALSE;
            break;
        }
        pFloat ++;
        pCmd ++;
    }

    UtAssert_True(bResult,
             "ProcessNewCmds, ASPD4525_AIR_COL_CALIB_CC, params");
    UtAssert_True(ASPD4525_AppData.HkTlm.usCmdCnt == 1,
             "ProcessNewCmds, ASPD4525_AIR_COL_CALIB_CC, usCmdCnt");
}


/**
 * Test ASPD4525_ProcessNewCmds, ASPD4525_SET_AIR_DENSITY_MODE_CC
 */
void Test_ASPD4525_ProcessNewCmds_ASPD4525_SET_AIR_DENSITY_MODE_CC(void)
{
    int32                            iStatus = CFE_SUCCESS;
    int32                            CmdPipe;
    ASPD4525_AirDensityModeArgCmd_t  CmdMsg;
    ASPD4525_ConfigTblEntry_t        *pConfig = NULL;

    CmdPipe = Ut_CFE_SB_CreatePipe(ASPD4525_CMD_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, ASPD4525_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg,
                      (uint16)ASPD4525_SET_AIR_DENSITY_MODE_CC);

    /* Execute the function being tested */
    iStatus = ASPD4525_InitApp();

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE,
                 "ProcessNewCmds, ASPD4525_SET_AIR_DENSITY_MODE_CC, InitApp");

        return;
    }

    pConfig = &ASPD4525_AppData.ConfigTblPtr[0];
    CmdMsg.uAirDensityCalculationMode =
                 ASPD4525_CONFIG_AIRDENSITY_ALTITUDE_TEMPERATURE_MODE;

    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    ASPD4525_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    ASPD4525_ProcessNewCmds();

    UtAssert_True(pConfig->uAirDensityCalculationMode ==
                  ASPD4525_CONFIG_AIRDENSITY_ALTITUDE_TEMPERATURE_MODE,
                  "ProcessNewCmds, ASPD4525_SET_AIR_DENSITY_MODE_CC, param");
    UtAssert_True(ASPD4525_AppData.HkTlm.usCmdCnt == 1,
               "ProcessNewCmds, ASPD4525_SET_AIR_DENSITY_MODE_CC, usCmdCnt");
}



void ASPD4525_Cmds_Test_AddTestCases(void)
{
    UtTest_Add(Test_ASPD4525_ProcessNewCmds_InvalidCmd,
            ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
            "Test_ASPD4525_ProcessNewCmds_InvalidCmd");
    UtTest_Add(Test_ASPD4525_ProcessNewCmds_InvalidCmdCode,
            ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
            "Test_ASPD4525_ProcessNewCmds_InvalidCmdCode");
    UtTest_Add(Test_ASPD4525_ProcessNewCmds_PipeError,
            ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
            "Test_ASPD4525_ProcessNewCmds_PipeError");
    UtTest_Add(Test_ASPD4525_ProcessNewCmds_Noop,
            ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
            "Test_ASPD4525_ProcessNewCmds_Noop");
    UtTest_Add(Test_ASPD4525_ProcessNewCmds_Reset,
            ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
            "Test_ASPD4525_ProcessNewCmds_Reset");
    UtTest_Add(Test_ASPD4525_ProcessNewCmds_ASPD4525_MAN_CALIB_CC,
            ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
            "Test_ASPD4525_ProcessNewCmds_ASPD4525_MAN_CALIB_CC");
    UtTest_Add(Test_ASPD4525_ProcessNewCmds_ASPD4525_LAB_CALIB_CC,
            ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
            "Test_ASPD4525_ProcessNewCmds_ASPD4525_LAB_CALIB_CC");
    UtTest_Add(Test_ASPD4525_ProcessNewCmds_ASPD4525_TEMP_CALIB_CC,
            ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
            "Test_ASPD4525_ProcessNewCmds_ASPD4525_TEMP_CALIB_CC");
    UtTest_Add(Test_ASPD4525_ProcessNewCmds_ASPD4525_PHYSICS_CALIB_CC,
            ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
            "Test_ASPD4525_ProcessNewCmds_ASPD4525_PHYSICS_CALIB_CC");
    UtTest_Add(Test_ASPD4525_ProcessNewCmds_ASPD4525_AIR_COL_CALIB_CC,
            ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
            "Test_ASPD4525_ProcessNewCmds_ASPD4525_AIR_COL_CALIB_CC");
    UtTest_Add(Test_ASPD4525_ProcessNewCmds_ASPD4525_SET_AIR_DENSITY_MODE_CC,
            ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
            "Test_ASPD4525_ProcessNewCmds_ASPD4525_SET_AIR_DENSITY_MODE_CC");
} /* end ASPD4525_Cmds_Test_AddTestCases */
