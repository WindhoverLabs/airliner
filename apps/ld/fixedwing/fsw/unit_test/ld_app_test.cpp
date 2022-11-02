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
#include "ld_app.h"
#include "ld_test_utils.hpp"
#include "ld_app_test.hpp"
#include "uttest.h"
#include "ut_osapi_stubs.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_sb_hooks.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_es_hooks.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_psp_timer_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_cfe_time_stubs.h"

#include <float.h>
#include <time.h>


uint16   RcvDataPipeMsgHook_MsgId = 0;
uint16   SendHKSendMsgHook_MsgId = 0;
uint16   DiagTlmSendMsgHook_MsgId = 0;
int32    WriteToSysLog_HookCalledCnt = 0;

double   ConfigData_Checksum = 0.0;


/**
 * Test LD GetPSPTimeHook
 */
void Test_LD_GetPSPTimeHook(OS_time_t *LocalTime)
{
    int              iStatus;
    struct timespec  time;

    iStatus = clock_gettime(CLOCK_REALTIME, &time);
    if (iStatus == 0)
    {
        LocalTime->seconds = time.tv_sec;
        LocalTime->microsecs = time.tv_nsec / 1000;
    }

    return;
}


/**************************************************************************
 * Tests for LD_InitEvent()
 **************************************************************************/
/**
 * Test LD_InitEvent() with failed CFE_EVS_Register
 */
void Test_LD_InitEvent_Fail_Register(void)
{
    LD oLD;

    /* Set a fail result for EVS */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EVENTS_SERVICE | CFE_EVS_NOT_IMPLEMENTED;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oLD.InitEvent();

    /* Verify results */
    UtAssert_True (result == expected, "InitEvent, failed EVS Register");
}


/**************************************************************************
 * Tests for LD_InitPipe()
 **************************************************************************/
/**
 * Test LD_InitPipe(), fail SCH CFE_SB_CreatePipe
 */
void Test_LD_InitPipe_Fail_CreateSCHPipe(void)
{
    LD oLD;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oLD.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create SCH pipe");
}


/**
 * Test LD_InitPipe(), fail CFE_SB_SubscribeEx for wakeup
 */
void Test_LD_InitPipe_Fail_SubscribeWakeup(void)
{
    LD oLD;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oLD.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for wakeup");
}


/**
 * Test LD_InitPipe(), fail CFE_SB_SubscribeEx for sendhk
 */
void Test_LD_InitPipe_Fail_SubscribeSendHK(void)
{
    LD oLD;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oLD.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for sendhk");
}


/**
 * Test LD_InitPipe(), fail CMD CFE_SB_CreatePipe
 */
void Test_LD_InitPipe_Fail_CreateCMDPipe(void)
{
    LD oLD;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oLD.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create CMD pipe");
}


/**
 * Test LD_InitPipe(), fail CFE_SB_Subscribe for CMD msg
 */
void Test_LD_InitPipe_Fail_SubscribeCMD(void)
{
    LD oLD;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oLD.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_Subscribe for CMD");
}


/**
 * Test LD_InitPipe(), fail DATA CFE_SB_CreatePipe
 */
void Test_LD_InitPipe_Fail_CreateDATAPipe(void)
{
    LD oLD;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 3);

    /* Execute the function being tested */
    result = oLD.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create DATA pipe");
}


/**
 * Test LD_InitPipe(), fail CFE_SB_SubscribeEx for ActuatorArmed
 */
void Test_LD_InitPipe_Fail_SubscribeExActuatorArmed(void)
{
    LD oLD;

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 3);

    /* Execute the function being tested */
    result = oLD.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for ActuatorArmed");
}


/**
 * Test LD_InitPipe(), fail CFE_SB_SubscribeEx for Airspeed
 */
void Test_LD_InitPipe_Fail_SubscribeExAirspeed(void)
{
    LD oLD;

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 4);

    /* Execute the function being tested */
    result = oLD.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for Airspeed");
}


/**
 * Test LD_InitPipe(), fail CFE_SB_SubscribeEx for ActuatorControls0
 */
void Test_LD_InitPipe_Fail_SubscribeExActuatorControls0(void)
{
    LD oLD;

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 5);

    /* Execute the function being tested */
    result = oLD.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                 "InitPipe, fail CFE_SB_SubscribeEx for ActuatorControls0");
}


/**
 * Test LD_InitPipe(), fail CFE_SB_SubscribeEx for ControlState
 */
void Test_LD_InitPipe_Fail_SubscribeExControlState(void)
{
    LD oLD;

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 6);

    /* Execute the function being tested */
    result = oLD.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for ControlState");
}


/**
 * Test LD_InitPipe(), fail CFE_SB_SubscribeEx for BatteryStatus
 */
void Test_LD_InitPipe_Fail_SubscribeExBatteryStatus(void)
{
    LD oLD;

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 7);

    /* Execute the function being tested */
    result = oLD.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for BatteryStatus");
}


/**
 * Test LD_InitPipe(), fail CFE_SB_SubscribeEx for VehicleAttitude
 */
void Test_LD_InitPipe_Fail_SubscribeExVehicleAttitude(void)
{
    LD oLD;

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 8);

    /* Execute the function being tested */
    result = oLD.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for VehicleAttitude");
}


/**
 * Test LD_InitPipe(), fail CFE_SB_SubscribeEx for ManualControlSetpoint
 */
void Test_LD_InitPipe_Fail_SubscribeExManualControlSetpoint(void)
{
    LD oLD;

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 9);

    /* Execute the function being tested */
    result = oLD.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
            "InitPipe, fail CFE_SB_SubscribeEx for ManualControlSetpoint");
}


/**
 * Test LD_InitPipe(), fail CFE_SB_SubscribeEx for VehicleLocalPosition
 */
void Test_LD_InitPipe_Fail_SubscribeExVehicleLocalPosition(void)
{
    LD oLD;

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 10);

    /* Execute the function being tested */
    result = oLD.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
             "InitPipe, fail CFE_SB_SubscribeEx for VehicleLocalPosition");
}


/**
 * Test LD_InitPipe(), fail CFE_SB_SubscribeEx for VehicleSensorCombined
 */
void Test_LD_InitPipe_Fail_SubscribeExVehicleSensorCombined(void)
{
    LD oLD;

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 11);

    /* Execute the function being tested */
    result = oLD.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
           "InitPipe, fail CFE_SB_SubscribeEx for VehicleSensorCombined");
}


/**
 * Test LD_InitPipe(), fail CFE_SB_SubscribeEx for VehicleControlMode
 */
void Test_LD_InitPipe_Fail_SubscribeExVehicleControlMode(void)
{
    LD oLD;

    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 12);

    /* Execute the function being tested */
    result = oLD.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
              "InitPipe, fail CFE_SB_SubscribeEx for VehicleControlMode");
}



/**************************************************************************
 * Tests for LD_InitData()
 **************************************************************************/
/**
 * Test LD_InitData()
 */
void Test_LD_InitData(void)
{
    LD oLD;

    /* Set a fail result */
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    oLD.InitData();

    /* Verify results */
    //UtAssert_True (result == expected, "InitData");
}


/**************************************************************************
 * Tests for LD_InitApp()
 **************************************************************************/
/**
 * Test LD_InitApp(), fail init event
 */
void Test_LD_InitApp_Fail_InitEvent(void)
{
    LD oLD;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oLD.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init event");
}


/**
 * Test LD_InitApp(), fail init pipe
 */
void Test_LD_InitApp_Fail_InitPipe(void)
{
    LD oLD;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oLD.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init pipe");
}


/**
 * Test LD_InitApp(), fail init data.
 * NOTE: no current way to fail LD_InitData() in default
 */
void Test_LD_InitApp_Fail_InitData(void)
{
    LD oLD;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oLD.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init data");
}


/**
 * Test LD_InitApp(), fail init config table
 */
void Test_LD_InitApp_Fail_InitConfigTbl(void)
{
    LD oLD;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_NAME;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oLD.InitApp();

    /* Verify results */
    UtAssert_True(result == expected, "InitApp, fail init config table");
}


/**
 * Test LD_InitApp(), Nominal
 */
void Test_LD_InitApp_Nominal(void)
{
    LD oLD;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oLD.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, nominal");
}


/**************************************************************************
 * Tests for extern LD_AppMain()
 **************************************************************************/
/**
 * Test LD extern LD_AppMain(), Nominal
 */
void Test_LD_LD_AppMain_Nominal(void)
{
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    LD_AppMain();
}

/**************************************************************************
 * Tests for LD_AppMain()
 **************************************************************************/
/**
 * Test LD AppMain(), WriteToSysLogHook
 */
int32 Test_LD_AppMain_WriteToSysLogHook(const char *StringPtr, ...)
{
    va_list   Ptr;
    char      Buff[256];

    WriteToSysLog_HookCalledCnt++;

    va_start(Ptr, StringPtr);
    vsnprintf(Buff, (size_t)CFE_EVS_MAX_MESSAGE_LENGTH, StringPtr, Ptr);
    va_end(Ptr);

    printf("###AppMain_WriteToSysLogHook:\n");
    printf("%s", Buff);

    return CFE_SUCCESS;
}


/**
 * Test LD_AppMain(), Fail RegisterApp
 */
void Test_LD_AppMain_Fail_RegisterApp(void)
{
    LD oLD;

    /* fail the register app */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX,
                            CFE_ES_ERR_APP_REGISTER, 1);

    /* Execute the function being tested */
    oLD.AppMain();
}


/**
 * Test LD_AppMain(), Fail InitApp
 */
void Test_LD_AppMain_Fail_InitApp(void)
{
    LD oLD;

    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    /* fail the register app */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    WriteToSysLog_HookCalledCnt = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_WRITETOSYSLOG_INDEX,
               (void*)&Test_LD_AppMain_WriteToSysLogHook);

    /* Execute the function being tested */
    oLD.AppMain();

    /* Verify results */
    UtAssert_True(WriteToSysLog_HookCalledCnt == 3,
                  "AppMain, Fail_InitApp");
}


/**
 * Test LD_AppMain(), Fail AcquireConfigPtrs
 */
void Test_LD_AppMain_Fail_AcquireConfigPtrs(void)
{
    LD oLD;

    int32 expected = CFE_TBL_ERR_INVALID_HANDLE;
    char  expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, expected, 2);

    /* Execute the function being tested */
    oLD.AppMain();

    sprintf(expectedEvent,
            "Failed to get Config table's address (0x%08lX)",
            CFE_TBL_ERR_INVALID_HANDLE);
    /* Verify results */
    UtAssert_EventSent(LD_CFGTBL_GETADDR_ERR_EID, CFE_EVS_ERROR,
                  expectedEvent, "LD_AppMain(), Fail AcquireConfigPtrs");
}


/**
 * Test LD_AppMain(), Invalid Schedule Message
 */
void Test_LD_AppMain_InvalidSchMessage(void)
{
    LD oLD;

    char expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving an
       invalid SCH message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, 0, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLD.AppMain();

    sprintf(expectedEvent, "Recvd invalid SCH msgId (0x%04X)", 0);

    /* Verify results */
    UtAssert_EventSent(LD_MSGID_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "LD_AppMain(), Invalid Schedule Message");
}


/**
 * Test LD_AppMain(), SchPipeError
 */
void Test_LD_AppMain_SchPipeError(void)
{
    LD oLD;

    char expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_PIPE_RD_ERR, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLD.AppMain();

    sprintf(expectedEvent, "SCH pipe read error (0x%08lX).",
                            CFE_SB_PIPE_RD_ERR);

    /* Verify results */
    UtAssert_EventSent(LD_RCVMSG_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "LD_AppMain(), SchPipeError");
}


/**
 * Hook to support: LD_AppMain(), Nominal_SendHK - SendMsg
 */
int32 Test_LD_AppMain_Nominal_SendHK_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    unsigned char      *pBuff = NULL;
    uint16             msgLen = 0;
    int                i = 0;
    CFE_SB_MsgId_t     MsgId;
    time_t             localTime;
    struct tm          *loc_time;
    CFE_TIME_SysTime_t TimeFromMsg;
    LD_HkTlm_t         HkMsg;

    pBuff = (unsigned char*)MsgPtr;

    msgLen = CFE_SB_GetTotalMsgLength(MsgPtr);    /* DataLenth + 7 */
    printf("###AppMain_SendHK_SendMsgHook: MsgLen(%u)\n", msgLen);
    for (i = 0; i < msgLen; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff++;
    }
    printf("\n");

    TimeFromMsg = CFE_SB_GetMsgTime(MsgPtr);
    localTime = LD_Test_GetTimeFromMsg(TimeFromMsg);
    loc_time = localtime(&localTime);
    printf("TimeFromMessage: %s", asctime(loc_time));

    MsgId = (((MsgPtr->Hdr).StreamId[0] << 8) +
             ((MsgPtr->Hdr).StreamId[1]));
    switch (MsgId)
    {
        case LD_HK_TLM_MID:
        {
            SendHKSendMsgHook_MsgId = LD_HK_TLM_MID;
            CFE_PSP_MemCpy((void*)&HkMsg, (void*)MsgPtr,
                           sizeof(HkMsg));

            printf("Sent LD_HK_TLM_MID:\n");
            printf("CmdCnt: %d\n", HkMsg.usCmdCnt);
            printf("CmdErrCnt: %d\n", HkMsg.usCmdErrCnt);
            printf("state: %d\n", HkMsg.state);
            printf("velocity_xy_filtered_f: %f\n",
                                     HkMsg.velocity_xy_filtered_f);
            printf("velocity_xy_unfiltered_f: %f\n",
                                   HkMsg.velocity_xy_unfiltered_f);
            printf("velocity_z_filtered_f: %f\n",
                                      HkMsg.velocity_z_filtered_f);
            printf("velocity_z_unfiltered_f: %f\n",
                                    HkMsg.velocity_z_unfiltered_f);
            printf("airspeed_filtered_f: %f\n",
                                        HkMsg.airspeed_filtered_f);
            printf("airspeed_unfiltered_f: %f\n",
                                      HkMsg.airspeed_unfiltered_f);
            printf("accel_xy_filtered_f: %f\n",
                                        HkMsg.accel_xy_filtered_f);
            printf("accel_xy_unfiltered_f: %f\n",
                                      HkMsg.accel_xy_unfiltered_f);
            printf("static_xy_b: %d\n", HkMsg.static_xy_b);
            printf("static_z_b: %d\n", HkMsg.static_z_b);
            printf("static_air_b: %d\n", HkMsg.static_air_b);
            printf("landDetected_b: %d\n", HkMsg.landDetected_b);
            break;
        }
        default:
        {
            printf("Sent MID(0x%04X)\n", MsgId);
            break;
        }
    }

    return CFE_SUCCESS;
}

/**
 * Test LD_AppMain(), Nominal - SendHK
 */
void Test_LD_AppMain_Nominal_SendHK(void)
{
    LD oLD;

    int32            SchPipe;
    LD_NoArgCmd_t    InMsg;

    SchPipe = Ut_CFE_SB_CreatePipe(LD_SCH_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, LD_SEND_HK_MID, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)SchPipe);

    /* The following will emulate the behavior of receiving
       LD_SEND_HK_MID message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, LD_WAKEUP_MID, 1);

    /* Used to verify HK was transmitted correctly. */
    SendHKSendMsgHook_MsgId = 0;
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                 (void*)&Test_LD_AppMain_Nominal_SendHK_SendMsgHook);
    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
                                     (void*)&Test_LD_GetPSPTimeHook);

    /* Execute the function being tested */
    oLD.InitApp();

    /* Fix LocalPosition data */
    oLD.CVT.VehicleLocalPositionMsg.Timestamp = LD_Test_GetTimeUs();
    oLD.CVT.VehicleLocalPositionMsg.RefTimestamp = LD_Test_GetTimeUs();
    oLD.CVT.VehicleLocalPositionMsg.RefLat = 47.397741928975;
    oLD.CVT.VehicleLocalPositionMsg.RefLon = 8.545593979817;
    oLD.CVT.VehicleLocalPositionMsg.SurfaceBottomTimestamp =
                                             LD_Test_GetTimeUs();
    oLD.CVT.VehicleLocalPositionMsg.X = -1.995731f;
    oLD.CVT.VehicleLocalPositionMsg.Y = 1.565559f;
    oLD.CVT.VehicleLocalPositionMsg.Z = -0.826584f;
    oLD.CVT.VehicleLocalPositionMsg.Delta_XY[0] = 0.0f;
    oLD.CVT.VehicleLocalPositionMsg.Delta_XY[1] = 0.0f;
    oLD.CVT.VehicleLocalPositionMsg.Delta_Z = 0.0f;
    oLD.CVT.VehicleLocalPositionMsg.VX = -0.027511f;
    oLD.CVT.VehicleLocalPositionMsg.VY = 0.006788f;
    oLD.CVT.VehicleLocalPositionMsg.VZ = -0.051438f;
    oLD.CVT.VehicleLocalPositionMsg.Delta_VXY[0] = 0.0f;
    oLD.CVT.VehicleLocalPositionMsg.Delta_VXY[1] = 0.0f;
    oLD.CVT.VehicleLocalPositionMsg.Delta_VZ = 0.0f;
    oLD.CVT.VehicleLocalPositionMsg.AX = 0.0f;
    oLD.CVT.VehicleLocalPositionMsg.AY = 0.0f;
    oLD.CVT.VehicleLocalPositionMsg.AZ = 0.0f;
    oLD.CVT.VehicleLocalPositionMsg.Yaw = 1.547718f;
    oLD.CVT.VehicleLocalPositionMsg.RefAlt = 490.7512f;
    oLD.CVT.VehicleLocalPositionMsg.DistBottom = 1.155246f;
    oLD.CVT.VehicleLocalPositionMsg.DistBottomRate = 0.051438f;
    oLD.CVT.VehicleLocalPositionMsg.EpH = 0.369742f;
    oLD.CVT.VehicleLocalPositionMsg.EpV = 0.216528f;
    oLD.CVT.VehicleLocalPositionMsg.EvH = 0.0f;
    oLD.CVT.VehicleLocalPositionMsg.EvV = 0.0f;
    oLD.CVT.VehicleLocalPositionMsg.EstimatorType = 0;
    oLD.CVT.VehicleLocalPositionMsg.XY_Valid = TRUE;
    oLD.CVT.VehicleLocalPositionMsg.Z_Valid = TRUE;
    oLD.CVT.VehicleLocalPositionMsg.V_XY_Valid = TRUE;
    oLD.CVT.VehicleLocalPositionMsg.V_Z_Valid = TRUE;
    oLD.CVT.VehicleLocalPositionMsg.XY_ResetCounter = 0;
    oLD.CVT.VehicleLocalPositionMsg.Z_ResetCounter = 0;
    oLD.CVT.VehicleLocalPositionMsg.VXY_ResetCounter = 0;
    oLD.CVT.VehicleLocalPositionMsg.VZ_ResetCounter = 0;
    oLD.CVT.VehicleLocalPositionMsg.XY_Global = TRUE;
    oLD.CVT.VehicleLocalPositionMsg.Z_Global = TRUE;
    oLD.CVT.VehicleLocalPositionMsg.DistBottomValid = TRUE;

    /* Fix Airspeed data */
    oLD.CVT.AirspeedMsg.Timestamp = LD_Test_GetTimeUs();
    oLD.CVT.AirspeedMsg.IndicatedAirspeed = 1.0f;
    oLD.CVT.AirspeedMsg.TrueAirspeed = 1.5f;
    oLD.CVT.AirspeedMsg.TrueAirspeedUnfiltered = 0.0f;
    oLD.CVT.AirspeedMsg.AirTemperature = 0.0f;
    oLD.CVT.AirspeedMsg.Confidence = 0.0f;

    oLD.RcvSchPipeMsg(LD_SCH_PIPE_PEND_TIME);
    oLD.RcvSchPipeMsg(LD_SCH_PIPE_PEND_TIME);

    /* Verify results */
    UtAssert_True (SendHKSendMsgHook_MsgId == LD_HK_TLM_MID,
                   "AppMain_Nominal_SendHK");

}


/**
 * Hook to support: LD_AppMain(), Nominal_DiagTlm SendMsg
 */
int32 Test_LD_AppMain_Nominal_DiagTlm_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    unsigned char      *pBuff = NULL;
    uint16             msgLen = 0;
    int                i = 0;
    CFE_SB_MsgId_t     MsgId;
    time_t             localTime;
    struct tm          *loc_time;
    CFE_TIME_SysTime_t TimeFromMsg;
    LD_Diag_t          DiagMsg;

    pBuff = (unsigned char*)MsgPtr;

    msgLen = CFE_SB_GetTotalMsgLength(MsgPtr);   /* DataLenth + 7 */
    printf("###AppMain_DiagTlm_SendMsgHook: MsgLen(%u)\n", msgLen);
    for (i = 0; i < msgLen; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff++;
    }
    printf("\n");

    TimeFromMsg = CFE_SB_GetMsgTime(MsgPtr);
    localTime = LD_Test_GetTimeFromMsg(TimeFromMsg);
    loc_time = localtime(&localTime);
    printf("TimeFromMessage: %s", asctime(loc_time));

    MsgId = (((MsgPtr->Hdr).StreamId[0] << 8) +
              ((MsgPtr->Hdr).StreamId[1]));
    switch (MsgId)
    {
        case LD_DIAG_TLM_MID:
        {
            DiagTlmSendMsgHook_MsgId = LD_DIAG_TLM_MID;
            CFE_PSP_MemCpy((void*)&DiagMsg, (void*)MsgPtr,
                           sizeof(DiagMsg));

            printf("Sent LD_DIAG_TLM_MID:\n");
            printf("GC_MinThrust: %d\n", DiagMsg.GC_MinThrust);
            printf("GC_AltitudeLock: %d\n", DiagMsg.GC_AltitudeLock);
            printf("GC_PositionLock: %d\n", DiagMsg.GC_PositionLock);
            printf("GC_InDescent: %d\n", DiagMsg.GC_InDescent);
            printf("GC_HitGround: %d\n", DiagMsg.GC_HitGround);
            printf("GC_HorMovement: %d\n", DiagMsg.GC_HorMovement);
            printf("GC_VertMovement: %d\n", DiagMsg.GC_VertMovement);
            printf("GC_ManualControlIdlingOrAutomatic: %d\n",
                     DiagMsg.GC_ManualControlIdlingOrAutomatic);
            printf("GroundContact: %d\n", DiagMsg.GroundContact);
            printf("LD_GC_history_state: %d\n",
                    DiagMsg.LD_GC_history_state);
            printf("LD_MinThrust: %d\n", DiagMsg.LD_MinThrust);
            printf("LD_Rotation: %d\n", DiagMsg.LD_Rotation);
            printf("LD_HorMovement: %d\n", DiagMsg.LD_HorMovement);
            printf("LD_PositionLock: %d\n", DiagMsg.LD_PositionLock);
            printf("Landed: %d\n", DiagMsg.Landed);
            break;
        }
        default:
        {
            printf("Sent MID(0x%04X)\n", MsgId);
            break;
        }
    }

    return CFE_SUCCESS;
}
/**
 * Test LD_AppMain(), Nominal - DiagTlm
 */
void Test_LD_AppMain_Nominal_DiagTlm(void)
{
    LD oLD;

    /* The following will emulate the behavior of receiving
       LD_WAKEUP_MID message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX,
                            LD_WAKEUP_MID, 1);

    /* Used to verify Diag Tlm was transmitted correctly. */
    DiagTlmSendMsgHook_MsgId = 0;
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
               (void*)&Test_LD_AppMain_Nominal_DiagTlm_SendMsgHook);
    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
                                     (void*)&Test_LD_GetPSPTimeHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLD.AppMain();

    /* Verify results */
    UtAssert_True (DiagTlmSendMsgHook_MsgId == LD_DIAG_TLM_MID,
                   "AppMain_Nominal_DiagTlm");
}


/**
 * Hook to support: LD_AppMain(), Nominal_Wakeup SendMsg
 */
int32 Test_LD_AppMain_Nominal_Wakeup_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    unsigned char      *pBuff = NULL;
    uint16             msgLen = 0;
    int                i = 0;
    CFE_SB_MsgId_t     MsgId;
    time_t             localTime;
    struct tm          *loc_time;
    CFE_TIME_SysTime_t TimeFromMsg;

    pBuff = (unsigned char*)MsgPtr;
    msgLen = CFE_SB_GetTotalMsgLength(MsgPtr);  /* DataLenth + 7 */
    printf("###Wakeup_SendMsgHook: MsgLen(%u)\n", msgLen);
    for (i = 0; i < msgLen; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff++;
    }
    printf("\n");

    TimeFromMsg = CFE_SB_GetMsgTime(MsgPtr);
    localTime = LD_Test_GetTimeFromMsg(TimeFromMsg);
    loc_time = localtime(&localTime);
    printf("TimeFromMessage: %s", asctime(loc_time));

    MsgId = CFE_SB_GetMsgId(MsgPtr);
    switch (MsgId)
    {
        case PX4_VEHICLE_LAND_DETECTED_MID:
        {
            PX4_VehicleLandDetectedMsg_t   VehicleLandDetected;
            CFE_PSP_MemCpy((void*)&VehicleLandDetected,
                         (void*)MsgPtr, sizeof(VehicleLandDetected));

            printf("Sent PX4_VEHICLE_LAND_DETECTED_MID:\n");
            localTime = LD_Test_GetTimeFromTimestamp(
                                      VehicleLandDetected.Timestamp);
            loc_time = localtime(&localTime);
            printf("Timestamp: %s", asctime(loc_time));
            printf("AltMax: %f\n", VehicleLandDetected.AltMax);
            printf("Landed: %d\n", VehicleLandDetected.Landed);
            printf("Freefall: %d\n", VehicleLandDetected.Freefall);
            printf("GroundContact: %d\n",
                    VehicleLandDetected.GroundContact);
            break;
        }
        case LD_DIAG_TLM_MID:
        {
            LD_Diag_t   Diag;
            CFE_PSP_MemCpy((void*)&Diag, (void*)MsgPtr,
                           sizeof(Diag));

            printf("Sent LD_DIAG_TLM_MID:\n");
            printf("GC_MinThrust: %d\n", Diag.GC_MinThrust);
            printf("GC_AltitudeLock: %d\n", Diag.GC_AltitudeLock);
            printf("GC_PositionLock: %d\n", Diag.GC_PositionLock);
            printf("GC_InDescent: %d\n", Diag.GC_InDescent);
            printf("GC_HitGround: %d\n", Diag.GC_HitGround);
            printf("GC_HorMovement: %d\n", Diag.GC_HorMovement);
            printf("GC_VertMovement: %d\n", Diag.GC_VertMovement);
            printf("GC_ManualControlIdlingOrAutomatic: %d\n",
                     Diag.GC_ManualControlIdlingOrAutomatic);
            printf("GroundContact: %d\n", Diag.GroundContact);
            printf("LD_GC_history_state: %d\n",
                    Diag.LD_GC_history_state);
            printf("LD_MinThrust: %d\n", Diag.LD_MinThrust);
            printf("LD_Rotation: %d\n", Diag.LD_Rotation);
            printf("LD_HorMovement: %d\n", Diag.LD_HorMovement);
            printf("LD_PositionLock: %d\n", Diag.LD_PositionLock);
            printf("Landed: %d\n", Diag.Landed);
            break;
        }
        default:
        {
            printf("Sent MID(0x%04X)\n", MsgId);
            break;
        }
    }

    return CFE_SUCCESS;
}
/**
 * Test LD_AppMain(), Nominal - Wakeup
 */
void Test_LD_AppMain_Nominal_Wakeup(void)
{
    LD oLD;

    /* The following will emulate behavior of receiving a
       SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX,
                            LD_WAKEUP_MID, 1);

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
               (void*)&Test_LD_AppMain_Nominal_Wakeup_SendMsgHook);
    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
                                   (void*)&Test_LD_GetPSPTimeHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLD.AppMain();
}


/**
 * Test LD AppMain(), RcvDataPipeMsgHook
 */
int32 Test_LD_AppMain_RcvDataPipeMsgHook
                    (void *dst, void *src, uint32 size)
{
    unsigned char   *pBuff = NULL;
    int             i = 0;
    CFE_SB_Msg_t*   dataMsgPtr=NULL;
    CFE_SB_MsgId_t  DataMsgId;

    pBuff = (unsigned char*)src;
    printf("###RcvDataPipeMsgHook: ");
    for (i = 0; i < size; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff ++;
    }
    printf("\n");

    dataMsgPtr = (CFE_SB_Msg_t*)src;
    DataMsgId = CFE_SB_GetMsgId(dataMsgPtr);
    switch(DataMsgId)
    {
        case PX4_ACTUATOR_ARMED_MID:
            RcvDataPipeMsgHook_MsgId = PX4_ACTUATOR_ARMED_MID;
            break;
        case PX4_AIRSPEED_MID:
            RcvDataPipeMsgHook_MsgId = PX4_AIRSPEED_MID;
            break;
        case PX4_ACTUATOR_CONTROLS_0_MID:
            RcvDataPipeMsgHook_MsgId = PX4_ACTUATOR_CONTROLS_0_MID;
            break;
        case PX4_CONTROL_STATE_MID:
            RcvDataPipeMsgHook_MsgId = PX4_CONTROL_STATE_MID;
            break;
        case PX4_BATTERY_STATUS_MID:
            RcvDataPipeMsgHook_MsgId = PX4_BATTERY_STATUS_MID;
            break;
        case PX4_VEHICLE_ATTITUDE_MID:
            RcvDataPipeMsgHook_MsgId = PX4_VEHICLE_ATTITUDE_MID;
            break;
        case PX4_MANUAL_CONTROL_SETPOINT_MID:
            RcvDataPipeMsgHook_MsgId = PX4_MANUAL_CONTROL_SETPOINT_MID;
            break;
        case PX4_VEHICLE_LOCAL_POSITION_MID:
            RcvDataPipeMsgHook_MsgId = PX4_VEHICLE_LOCAL_POSITION_MID;
            break;
        case PX4_SENSOR_COMBINED_MID:
            RcvDataPipeMsgHook_MsgId = PX4_SENSOR_COMBINED_MID;
            break;
        case PX4_VEHICLE_CONTROL_MODE_MID:
            RcvDataPipeMsgHook_MsgId = PX4_VEHICLE_CONTROL_MODE_MID;
            break;
        default:
            RcvDataPipeMsgHook_MsgId = 0;
            break;
    }

    return CFE_SUCCESS;
}


/**
 * Test LD AppMain(), RcvDataPipeMsg - InvalidMsgID
 */
void Test_LD_AppMain_RcvDataPipeMsg_InvalidMsgID(void)
{
    LD oLD;

    int32                          DataPipe;
    PX4_DifferentialPressureMsg_t  InMsg;
    char   expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe(LD_DATA_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, PX4_DIFFERENTIAL_PRESSURE_MID,
                    sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLD.AppMain();

    sprintf(expectedEvent, "Recvd invalid Data msgId (0x%04X)",
                           PX4_DIFFERENTIAL_PRESSURE_MID);

    /* Verify results */
    UtAssert_EventSent(LD_MSGID_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "AppMain(), RcvDataPipeMsg - InvalidMsgID");
}


/**
 * Test LD AppMain(), RcvDataPipeMsg - DataPipeError
 */
void Test_LD_AppMain_RcvDataPipeMsg_DataPipeError(void)
{
    LD oLD;

    char   expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_PIPE_RD_ERR, 1);

    /* Execute the function being tested */
    oLD.InitApp();
    oLD.RcvDataPipeMsg();

    sprintf(expectedEvent, "Data pipe read error (0x%08lX).",
                            CFE_SB_PIPE_RD_ERR);

    /* Verify results */
    UtAssert_EventSent(LD_RCVMSG_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "LD_AppMain(), RcvDataPipeMsg - DataPipeError");
}


/**
 * Test LD AppMain(), RcvDataPipeMsg - ActuatorArmed
 */
void Test_LD_AppMain_RcvDataPipeMsg_ActuatorArmed(void)
{
    LD oLD;

    int32                   DataPipe;
    PX4_ActuatorArmedMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe(LD_DATA_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, PX4_ACTUATOR_ARMED_MID,
                    sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    RcvDataPipeMsgHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(
                         UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_LD_AppMain_RcvDataPipeMsgHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLD.AppMain();

    /* Verify results */
    UtAssert_True(RcvDataPipeMsgHook_MsgId == PX4_ACTUATOR_ARMED_MID,
                  "AppMain_RcvDataPipeMsg_ActuatorArmed");
}


/**
 * Test LD AppMain(), RcvDataPipeMsg - Airspeed
 */
void Test_LD_AppMain_RcvDataPipeMsg_Airspeed(void)
{
    LD oLD;

    int32              DataPipe;
    PX4_AirspeedMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe(LD_DATA_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, PX4_AIRSPEED_MID,
                     sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    RcvDataPipeMsgHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(
                          UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                          (void*)&Test_LD_AppMain_RcvDataPipeMsgHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLD.AppMain();

    /* Verify results */
    UtAssert_True(RcvDataPipeMsgHook_MsgId == PX4_AIRSPEED_MID,
                  "AppMain_RcvDataPipeMsg_Airspeed");
}


/**
 * Test LD AppMain(), RcvDataPipeMsg - ActuatorControls0
 */
void Test_LD_AppMain_RcvDataPipeMsg_ActuatorControls0(void)
{
    LD oLD;

    int32                      DataPipe;
    PX4_ActuatorControlsMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe(LD_DATA_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, PX4_ACTUATOR_CONTROLS_0_MID,
                    sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    RcvDataPipeMsgHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(
                                    UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_LD_AppMain_RcvDataPipeMsgHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLD.AppMain();

    /* Verify results */
    UtAssert_True(
             RcvDataPipeMsgHook_MsgId == PX4_ACTUATOR_CONTROLS_0_MID,
             "AppMain_RcvDataPipeMsg_ActuatorControls0");
}


/**
 * Test LD AppMain(), RcvDataPipeMsg - ControlState
 */
void Test_LD_AppMain_RcvDataPipeMsg_ControlState(void)
{
    LD oLD;

    int32                  DataPipe;
    PX4_ControlStateMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe(LD_DATA_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, PX4_CONTROL_STATE_MID,
                    sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    RcvDataPipeMsgHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(
                                    UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_LD_AppMain_RcvDataPipeMsgHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLD.AppMain();

    /* Verify results */
    UtAssert_True(RcvDataPipeMsgHook_MsgId == PX4_CONTROL_STATE_MID,
                  "AppMain_RcvDataPipeMsg_ControlState");
}


/**
 * Test LD AppMain(), RcvDataPipeMsg - BatteryStatus
 */
void Test_LD_AppMain_RcvDataPipeMsg_BatteryStatus(void)
{
    LD oLD;

    int32                   DataPipe;
    PX4_BatteryStatusMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe(LD_DATA_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, PX4_BATTERY_STATUS_MID,
                    sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    RcvDataPipeMsgHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(
                                    UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_LD_AppMain_RcvDataPipeMsgHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLD.AppMain();

    /* Verify results */
    UtAssert_True(RcvDataPipeMsgHook_MsgId == PX4_BATTERY_STATUS_MID,
                  "AppMain_RcvDataPipeMsg_BatteryStatus");
}


/**
 * Test LD AppMain(), RcvDataPipeMsg - VehicleAttitude
 */
void Test_LD_AppMain_RcvDataPipeMsg_VehicleAttitude(void)
{
    LD oLD;

    int32                     DataPipe;
    PX4_VehicleAttitudeMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe(LD_DATA_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, PX4_VEHICLE_ATTITUDE_MID,
                    sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    RcvDataPipeMsgHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(
                                    UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_LD_AppMain_RcvDataPipeMsgHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLD.AppMain();

    /* Verify results */
    UtAssert_True(
               RcvDataPipeMsgHook_MsgId == PX4_VEHICLE_ATTITUDE_MID,
               "AppMain_RcvDataPipeMsg_VehicleAttitude");
}


/**
 * Test LD AppMain(), RcvDataPipeMsg - ManualControlSetpoint
 */
void Test_LD_AppMain_RcvDataPipeMsg_ManualControlSetpoint(void)
{
    LD oLD;

    int32                           DataPipe;
    PX4_ManualControlSetpointMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe(LD_DATA_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, PX4_MANUAL_CONTROL_SETPOINT_MID,
                    sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    RcvDataPipeMsgHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(
                                    UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_LD_AppMain_RcvDataPipeMsgHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLD.AppMain();

    /* Verify results */
    UtAssert_True(
           RcvDataPipeMsgHook_MsgId == PX4_MANUAL_CONTROL_SETPOINT_MID,
           "AppMain_RcvDataPipeMsg_ManualControlSetpoint");
}


/**
 * Test LD AppMain(), RcvDataPipeMsg - VehicleLocalPosition
 */
void Test_LD_AppMain_RcvDataPipeMsg_VehicleLocalPosition(void)
{
    LD oLD;

    int32                          DataPipe;
    PX4_VehicleLocalPositionMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe(LD_DATA_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, PX4_VEHICLE_LOCAL_POSITION_MID,
                    sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    RcvDataPipeMsgHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(
                                    UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_LD_AppMain_RcvDataPipeMsgHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLD.AppMain();

    /* Verify results */
    UtAssert_True(
           RcvDataPipeMsgHook_MsgId == PX4_VEHICLE_LOCAL_POSITION_MID,
           "AppMain_RcvDataPipeMsg_VehicleLocalPosition");
}


/**
 * Test LD AppMain(), RcvDataPipeMsg - VehicleSensorCombined
 */
void Test_LD_AppMain_RcvDataPipeMsg_VehicleSensorCombined(void)
{
    LD oLD;

    int32 DataPipe;
    PX4_SensorCombinedMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe(LD_DATA_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, PX4_SENSOR_COMBINED_MID,
                    sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    RcvDataPipeMsgHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(
                                    UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_LD_AppMain_RcvDataPipeMsgHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLD.AppMain();

    /* Verify results */
    UtAssert_True(RcvDataPipeMsgHook_MsgId == PX4_SENSOR_COMBINED_MID,
                  "AppMain_RcvDataPipeMsg_VehicleSensorCombined");
}


/**
 * Test LD AppMain(), RcvDataPipeMsg - VehicleControlMode
 */
void Test_LD_AppMain_RcvDataPipeMsg_VehicleControlMode(void)
{
    LD oLD;

    int32                        DataPipe;
    PX4_VehicleControlModeMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe(LD_DATA_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, PX4_VEHICLE_CONTROL_MODE_MID,
                    sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    RcvDataPipeMsgHook_MsgId = 0;
    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(
                                    UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_LD_AppMain_RcvDataPipeMsgHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLD.AppMain();

    /* Verify results */
    UtAssert_True(
             RcvDataPipeMsgHook_MsgId == PX4_VEHICLE_CONTROL_MODE_MID,
             "AppMain_RcvDataPipeMsg_VehicleControlMode");
}


/**************************************************************************
 * Tests for LD Execute()
 **************************************************************************/
/**
 * Test LD Execute, SendMsgtHook
 */
int32 Test_LD_Execute_SendMsgHook(CFE_SB_Msg_t   *MsgPtr)
{
    unsigned char      *pBuff = NULL;
    uint16             msgLen = 0;
    int                i = 0;
    CFE_SB_MsgId_t     MsgId;
    time_t             localTime;
    struct tm          *loc_time;
    CFE_TIME_SysTime_t TimeFromMsg;

    pBuff = (unsigned char*)MsgPtr;
    msgLen = CFE_SB_GetTotalMsgLength(MsgPtr);  /* DataLenth + 7 */
    printf("###Execute_SendMsgHook: MsgLen(%u)\n", msgLen);
    for (i = 0; i < msgLen; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff++;
    }
    printf("\n");

    TimeFromMsg = CFE_SB_GetMsgTime(MsgPtr);
    localTime = LD_Test_GetTimeFromMsg(TimeFromMsg);
    loc_time = localtime(&localTime);
    printf("TimeFromMessage: %s", asctime(loc_time));

    MsgId = CFE_SB_GetMsgId(MsgPtr);
    switch (MsgId)
    {
        case PX4_VEHICLE_LAND_DETECTED_MID:
        {
            PX4_VehicleLandDetectedMsg_t   VehicleLandDetected;
            CFE_PSP_MemCpy((void*)&VehicleLandDetected,
                         (void*)MsgPtr, sizeof(VehicleLandDetected));

            printf("Sent PX4_VEHICLE_LAND_DETECTED_MID:\n");
            localTime = LD_Test_GetTimeFromTimestamp(
                                      VehicleLandDetected.Timestamp);
            loc_time = localtime(&localTime);
            printf("Timestamp: %s", asctime(loc_time));
            printf("AltMax: %f\n", VehicleLandDetected.AltMax);
            printf("Landed: %d\n", VehicleLandDetected.Landed);
            printf("Freefall: %d\n", VehicleLandDetected.Freefall);
            printf("GroundContact: %d\n",
                    VehicleLandDetected.GroundContact);
            break;
        }
        case LD_DIAG_TLM_MID:
        {
            LD_Diag_t   Diag;
            CFE_PSP_MemCpy((void*)&Diag, (void*)MsgPtr,
                           sizeof(Diag));

            printf("Sent LD_DIAG_TLM_MID:\n");
            printf("GC_MinThrust: %d\n", Diag.GC_MinThrust);
            printf("GC_AltitudeLock: %d\n", Diag.GC_AltitudeLock);
            printf("GC_PositionLock: %d\n", Diag.GC_PositionLock);
            printf("GC_InDescent: %d\n", Diag.GC_InDescent);
            printf("GC_HitGround: %d\n", Diag.GC_HitGround);
            printf("GC_HorMovement: %d\n", Diag.GC_HorMovement);
            printf("GC_VertMovement: %d\n", Diag.GC_VertMovement);
            printf("GC_ManualControlIdlingOrAutomatic: %d\n",
                     Diag.GC_ManualControlIdlingOrAutomatic);
            printf("GroundContact: %d\n", Diag.GroundContact);
            printf("LD_GC_history_state: %d\n",
                    Diag.LD_GC_history_state);
            printf("LD_MinThrust: %d\n", Diag.LD_MinThrust);
            printf("LD_Rotation: %d\n", Diag.LD_Rotation);
            printf("LD_HorMovement: %d\n", Diag.LD_HorMovement);
            printf("LD_PositionLock: %d\n", Diag.LD_PositionLock);
            printf("Landed: %d\n", Diag.Landed);
            break;
        }
        default:
        {
            printf("Sent MID(0x%04X)\n", MsgId);
            break;
        }
    }

    return CFE_SUCCESS;
}


/**
 * Test LD Execute()
 */
void Test_LD_Execute(void)
{
    LD oLD;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX,
                            LD_WAKEUP_MID, 1);


    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
               (void*)&Test_LD_Execute_SendMsgHook);
    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
                                    (void*)&Test_LD_GetPSPTimeHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oLD.AppMain();
}


/*********************************************************************
 * Tests for LD ConfigData()
 *********************************************************************/
/**
 * Test LD ConfigData()
 */
void Test_LD_ConfigData(void)
{
    LD oLD;

    double expected_checksum = 8010021.73;

    /* Execute the function being tested */
    oLD.InitApp();

    ConfigData_Checksum = 0.0;
    ConfigData_Checksum = LD_Test_GetConfigDataChecksum(
                                        oLD.ConfigTblPtr);

    /* Verify results */
    UtAssert_True(
        fabs(ConfigData_Checksum - expected_checksum) <= FLT_EPSILON,
        "LD ConfigData");
}



/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void LD_App_Test_AddTestCases(void)
{
    UtTest_Add(Test_LD_InitEvent_Fail_Register,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitEvent_Fail_Register");

    UtTest_Add(Test_LD_InitPipe_Fail_CreateSCHPipe,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_CreateSCHPipe");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeWakeup,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_SubscribeWakeup");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeSendHK,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_SubscribeSendHK");
    UtTest_Add(Test_LD_InitPipe_Fail_CreateCMDPipe,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_CreateCMDPipe");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeCMD,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_SubscribeCMD");
    UtTest_Add(Test_LD_InitPipe_Fail_CreateDATAPipe,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_CreateDATAPipe");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeExActuatorArmed,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_SubscribeExActuatorArmed");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeExAirspeed,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_SubscribeExAirspeed");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeExActuatorControls0,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_SubscribeExActuatorControls0");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeExControlState,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_SubscribeExControlState");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeExBatteryStatus,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_SubscribeExBatteryStatus");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeExVehicleAttitude,
           LD_Test_Setup, LD_Test_TearDown,
           "Test_LD_InitPipe_Fail_SubscribeExVehicleAttitude");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeExManualControlSetpoint,
           LD_Test_Setup, LD_Test_TearDown,
           "Test_LD_InitPipe_Fail_SubscribeExManualControlSetpoint");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeExVehicleLocalPosition,
           LD_Test_Setup, LD_Test_TearDown,
           "Test_LD_InitPipe_Fail_SubscribeExVehicleLocalPosition");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeExVehicleSensorCombined,
           LD_Test_Setup, LD_Test_TearDown,
           "Test_LD_InitPipe_Fail_SubscribeExVehicleSensorCombined");
    UtTest_Add(Test_LD_InitPipe_Fail_SubscribeExVehicleControlMode,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitPipe_Fail_SubscribeExVehicleControlMode");

    UtTest_Add(Test_LD_InitData,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitData");

    UtTest_Add(Test_LD_InitApp_Fail_InitEvent,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitApp_Fail_InitEvent");
    UtTest_Add(Test_LD_InitApp_Fail_InitPipe,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitApp_Fail_InitPipe");
    UtTest_Add(Test_LD_InitApp_Fail_InitData,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitApp_Fail_InitData");
    UtTest_Add(Test_LD_InitApp_Fail_InitConfigTbl,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitApp_Fail_InitConfigTbl");
    UtTest_Add(Test_LD_InitApp_Nominal,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_InitApp_Nominal");

    UtTest_Add(Test_LD_LD_AppMain_Nominal,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_LD_AppMain_Nominal");

    UtTest_Add(Test_LD_AppMain_Fail_RegisterApp,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_Fail_RegisterApp");
    UtTest_Add(Test_LD_AppMain_Fail_InitApp,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_Fail_InitApp");
    UtTest_Add(Test_LD_AppMain_Fail_AcquireConfigPtrs,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_Fail_AcquireConfigPtrs");
    UtTest_Add(Test_LD_AppMain_InvalidSchMessage,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_InvalidSchMessage");
    UtTest_Add(Test_LD_AppMain_SchPipeError,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_SchPipeError");
    UtTest_Add(Test_LD_AppMain_Nominal_SendHK,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_Nominal_SendHK");
    UtTest_Add(Test_LD_AppMain_Nominal_DiagTlm,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_Nominal_DiagTlm");
    UtTest_Add(Test_LD_AppMain_Nominal_Wakeup,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_Nominal_Wakeup");
    UtTest_Add(Test_LD_AppMain_RcvDataPipeMsg_InvalidMsgID,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_RcvDataPipeMsg_InvalidMsgID");
    UtTest_Add(Test_LD_AppMain_RcvDataPipeMsg_DataPipeError,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_RcvDataPipeMsg_DataPipeError");
    UtTest_Add(Test_LD_AppMain_RcvDataPipeMsg_ActuatorArmed,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_RcvDataPipeMsg_ActuatorArmed");
    UtTest_Add(Test_LD_AppMain_RcvDataPipeMsg_Airspeed,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_RcvDataPipeMsg_Airspeed");
    UtTest_Add(Test_LD_AppMain_RcvDataPipeMsg_ActuatorControls0,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_RcvDataPipeMsg_ActuatorControls0");
    UtTest_Add(Test_LD_AppMain_RcvDataPipeMsg_ControlState,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_RcvDataPipeMsg_ControlState");
    UtTest_Add(Test_LD_AppMain_RcvDataPipeMsg_BatteryStatus,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_RcvDataPipeMsg_BatteryStatus");
    UtTest_Add(Test_LD_AppMain_RcvDataPipeMsg_VehicleAttitude,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_RcvDataPipeMsg_VehicleAttitude");
    UtTest_Add(Test_LD_AppMain_RcvDataPipeMsg_ManualControlSetpoint,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_RcvDataPipeMsg_ManualControlSetpoint");
    UtTest_Add(Test_LD_AppMain_RcvDataPipeMsg_VehicleLocalPosition,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_RcvDataPipeMsg_VehicleLocalPosition");
    UtTest_Add(Test_LD_AppMain_RcvDataPipeMsg_VehicleSensorCombined,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_RcvDataPipeMsg_VehicleSensorCombined");
    UtTest_Add(Test_LD_AppMain_RcvDataPipeMsg_VehicleControlMode,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_AppMain_RcvDataPipeMsg_VehicleControlMode");

    UtTest_Add(Test_LD_Execute,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_Execute");
    UtTest_Add(Test_LD_ConfigData,
               LD_Test_Setup, LD_Test_TearDown,
               "Test_LD_ConfigData");
}
