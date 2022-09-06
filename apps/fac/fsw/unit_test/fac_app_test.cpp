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

#include "fac_app_test.hpp"
#include "fac_app.hpp"
#include "fac_test_utils.hpp"

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
#include <iostream>

int32   WriteToSysLog_HookCalledCnt = 0;
int32   SendEvent_HookCalledCnt = 0;
uint32  UpdateParams_ValidateStatus = 0x0;

double  UpdateParams_ParamChecksum = 0.0;


/**************************************************************************
 * Tests for FAC InitEvent()
 **************************************************************************/
/**
 * Test FAC InitEvent() with failed CFE_EVS_Register
 */
void Test_FAC_InitEvent_Fail_Register(void)
{
    /* Set a fail result for EVS */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFAC.InitEvent();

    /* Verify results */
    UtAssert_True (result == expected, "InitEvent, failed EVS Register");
}


/**************************************************************************
 * Tests for FAC InitPipe()
 **************************************************************************/
/**
 * Test FAC InitPipe(), fail SCH CFE_SB_CreatePipe
 */
void Test_FAC_InitPipe_Fail_CreateSCHPipe(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create SCH pipe");
}


/**
 * Test FAC InitPipe(), fail CFE_SB_SubscribeEx for SendHK
 */
void Test_FAC_InitPipe_Fail_SubscribeSendHK(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for FAC_SEND_HK");
}


/**
 * Test FAC InitPipe(), fail CFE_SB_SubscribeEx for RunController
 */
void Test_FAC_InitPipe_Fail_SubscribeRunController(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for FAC_RUN_CONTROLLER");
}


/**
 * Test FAC InitPipe(), fail CMD CFE_SB_CreatePipe
 */
void Test_FAC_InitPipe_Fail_CreateCMDPipe(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create CMD pipe");
}


/**
 * Test FAC InitPipe(), fail CFE_SB_Subscribe for CMD msg
 */
void Test_FAC_InitPipe_Fail_SubscribeCMD(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_Subscribe for CMD");
}


/**
 * Test FAC InitPipe(), fail DATA CFE_SB_CreatePipe
 */
void Test_FAC_InitPipe_Fail_CreateDATAPipe(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 3);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create DATA pipe");
}


/**
 * Test FAC InitPipe(), fail CFE_SB_SubscribeEx for BatteryStatus
 */
void Test_FAC_InitPipe_Fail_SubscribeBatteryStatus(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 3);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for PX4_BATTERY_STATUS_MID");
}


/**
 * Test FAC InitPipe(), fail CFE_SB_SubscribeEx for ManualControlSp
 */
void Test_FAC_InitPipe_Fail_SubscribeManualControlSp(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 4);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for PX4_MANUAL_CONTROL_SETPOINT_MID");
}


/**
 * Test FAC InitPipe(), fail CFE_SB_SubscribeEx for VAttSp
 */
void Test_FAC_InitPipe_Fail_SubscribeVAttSp(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 5);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for PX4_VEHICLE_ATTITUDE_SETPOINT_MID");
}


/**
 * Test FAC InitPipe(), fail CFE_SB_SubscribeEx for VAtt
 */
void Test_FAC_InitPipe_Fail_SubscribeVAtt(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 6);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for PX4_VEHICLE_ATTITUDE_MID");
}


/**
 * Test FAC InitPipe(), fail CFE_SB_SubscribeEx for VControlMode
 */
void Test_FAC_InitPipe_Fail_SubscribeVControlMode(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 7);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for PX4_VEHICLE_CONTROL_MODE_MID");
}


/**
 * Test FAC InitPipe(), fail CFE_SB_SubscribeEx for VehicleStatus
 */
void Test_FAC_InitPipe_Fail_SubscribeVehicleStatus(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 8);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for PX4_VEHICLE_STATUS_MID");
}


/**
 * Test FAC InitPipe(), fail CFE_SB_SubscribeEx for VGlobalPosition
 */
void Test_FAC_InitPipe_Fail_SubscribeVGlobalPosition(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 9);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for PX4_VEHICLE_GLOBAL_POSITION_MID");
}


/**
 * Test FAC InitPipe(), fail CFE_SB_SubscribeEx for VLandDetected
 */
void Test_FAC_InitPipe_Fail_SubscribeVLandDetected(void)
{
    /* Set a fail result for SB */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 10);

    /* Execute the function being tested */
    result = oFAC.InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for PX4_VEHICLE_LAND_DETECTED_MID");
}


/**************************************************************************
 * Tests for FAC InitData()
 **************************************************************************/
/**
 * Test FAC InitData()
 */
void Test_FAC_InitDataNominal(void)
{
    /* Set a fail result */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 conf_result = -1;
    int32 expected = CFE_SUCCESS;

    /* Initialize the table so the table pointer is set. */
    conf_result = oFAC.InitConfigTbl();

    /* Execute the function being tested */
    if (conf_result == CFE_SUCCESS)
    {
        result = oFAC.InitData();
    }

    /* Verify results */
    UtAssert_True (result == expected, "InitData");
}


/**************************************************************************
 * Tests for FAC InitApp()
 **************************************************************************/
/**
 * Test FAC InitApp(), fail init event
 */
void Test_FAC_InitApp_Fail_InitEvent(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFAC.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init event");
}


/**
 * Test FAC InitApp(), fail init pipe
 */
void Test_FAC_InitApp_Fail_InitPipe(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFAC.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init pipe");
}


/**
 * Test FAC InitApp(), fail init data.
 * NOTE: no current way to fail FAC_InitData() in default
 */
void Test_FAC_InitApp_Fail_InitData(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oFAC.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init data");
}


/**
 * Test FAC InitApp(), fail init config table
 */
void Test_FAC_InitApp_Fail_InitConfigTbl(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_NAME;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFAC.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init config table");
}


/**
 * Test FAC InitApp(), Nominal
 */
void Test_FAC_InitApp_Nominal(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oFAC.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, nominal");
}


/**************************************************************************
 * Tests for extern FAC_AppMain()
 **************************************************************************/
/**
 * Test FAC extern FAC_AppMain(), Nominal
 */
void Test_FAC_FAC_AppMain_Nominal(void)
{
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    FAC_AppMain();
}


/**************************************************************************
 * Tests for oFAC::AppMain()
 **************************************************************************/
/**
 * Test FAC AppMain(), WriteToSysLogHook
 */
int32 Test_FAC_AppMain_WriteToSysLogHook(const char *StringPtr, ...)
{
    va_list   Ptr;
    char      Buf[256];

    WriteToSysLog_HookCalledCnt++;

    va_start(Ptr, StringPtr);
    vsnprintf(Buf, (size_t)CFE_EVS_MAX_MESSAGE_LENGTH, StringPtr, Ptr);
    va_end(Ptr);

    printf("###AppMain_WriteToSysLogHook:\n");
    printf("%s", Buf);

    return WriteToSysLog_HookCalledCnt;
}


/**
 * Test FAC AppMain(), SendEventHook
 */
int32 Test_FAC_AppMain_SendEventHook(uint16 EventID, uint16 EventType, const char *EventText, ...)
{
    va_list  Ptr;
    char     Buf[256];

    SendEvent_HookCalledCnt++;

    va_start(Ptr, EventText);
    vsnprintf(Buf, (size_t)CFE_EVS_MAX_MESSAGE_LENGTH, EventText, Ptr);
    va_end(Ptr);

    printf("###AppMain_SendEventHook:\n");
    printf("%s\n", Buf);

    return SendEvent_HookCalledCnt;
}


/**
 * Test FAC AppMain(), Fail RegisterApp
 */
void Test_FAC_AppMain_Fail_RegisterApp(void)
{
    int32 expected = CFE_ES_ERR_APP_REGISTER;

    /* fail the register app */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX, expected, 1);

    /* Execute the function being tested */
    oFAC.AppMain();
}


/**
 * Test FAC AppMain(), Fail InitApp
 */
void Test_FAC_AppMain_Fail_InitApp(void)
{
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    /* fail the register app */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    WriteToSysLog_HookCalledCnt = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_WRITETOSYSLOG_INDEX,
               (void*)&Test_FAC_AppMain_WriteToSysLogHook);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True (WriteToSysLog_HookCalledCnt == 3, "AppMain, Fail_InitApp");
}


/**
 * Test FAC AppMain(), Fail AcquireConfigPtrs
 */
void Test_FAC_AppMain_Fail_AcquireConfigPtrs(void)
{
    int32 expected = CFE_TBL_ERR_INVALID_HANDLE;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, expected, 2);

    SendEvent_HookCalledCnt = 0;
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
                (void*)Test_FAC_AppMain_SendEventHook);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True (SendEvent_HookCalledCnt == 3, "AppMain, Fail_AcquireConfigPtrs");
}


/**
 * Test FAC AppMain(), SchPipeError
 */
void Test_FAC_AppMain_SchPipeError(void)
{
    int32 expected = CFE_SB_PIPE_RD_ERR;

    /* The following will emulate the behavior of SCH pipe reading error */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, expected, 1);

    oFAC.AppMain();
}


/**
 * Test FAC AppMain(), SchPipeNoMessage
 */
void Test_FAC_AppMain_SchPipeNoMessage(void)
{
    int32 expected = CFE_SB_NO_MESSAGE;

    /* The following will emulate the behavior of SCH pipe: No message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, expected, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();
}


/**
 * Test FAC AppMain(), Invalid Schedule Message
 */
void Test_FAC_AppMain_InvalidSchMessage(void)
{
    /* The following will emulate the behavior of receiving an invalid SCH message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, PX4_BATTERY_STATUS_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True (oFAC.HkTlm.usSchErrCnt == 1, "AppMain, InvalidSchMessage");
}


/**
 * Hook to support: FAC AppMain(), Nominal - SendHK
 */
int32 Test_FAC_AppMain_Nominal_SendHK_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    /* TODO:  Test the contents of your HK message here. */
    unsigned char*     pBuff = NULL;
    uint16             msgLen = 0;
    int                i = 0;
    CFE_SB_MsgId_t     MsgId;
    time_t             localTime;
    struct tm          *loc_time;
    CFE_TIME_SysTime_t TimeFromMsg;
    FAC_HkTlm_t        HkMsg;

    pBuff = (unsigned char*)MsgPtr;

    msgLen = CFE_SB_GetTotalMsgLength(MsgPtr);                /* DataLenth + 7 */
    printf("###AppMain_SendHK_SendMsgHook: MsgLen(%u)\n", msgLen);
    for (i = 0; i < msgLen; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff++;
    }
    printf("\n");

    TimeFromMsg = CFE_SB_GetMsgTime(MsgPtr);
    localTime = FAC_Test_GetTimeFromMsg(TimeFromMsg);
    loc_time = localtime(&localTime);
    printf("TimeFromMessage: %s", asctime(loc_time));

    MsgId = CFE_SB_GetMsgId(MsgPtr);
    switch (MsgId)
    {
        case FAC_HK_TLM_MID:
        {
            CFE_PSP_MemCpy((void*)&HkMsg, (void*)MsgPtr, sizeof(FAC_HkTlm_t));

            printf("Sent FAC_HK_TLM_MID:\n");
            printf("CmdCnt: %d\n", HkMsg.usCmdCnt);
            printf("CmdErrCnt: %d\n", HkMsg.usCmdErrCnt);
            printf("SchErrCnt: %d\n", HkMsg.usSchErrCnt);
            printf("DataErrCnt: %d\n", HkMsg.usDataErrCnt);
            printf("SendHkMsgRcvCnt: %ld\n", HkMsg.SendHkMsgRcvCnt);
            printf("RunControllerMsgRcvCnt: %ld\n", HkMsg.RunControllerMsgRcvCnt);
            printf("AirSpeedMsgRcvCnt: %ld\n", HkMsg.AirSpeedMsgRcvCnt);
            printf("BatteryStatusMsgRcvCnt: %ld\n", HkMsg.BatteryStatusMsgRcvCnt);
            printf("ManualControlSpMsgRcvCnt: %ld\n", HkMsg.ManualControlSpMsgRcvCnt);
            printf("VAttMsgRcvCnt: %ld\n", HkMsg.VAttMsgRcvCnt);
            printf("VAttSpMsgRcvCnt: %ld\n", HkMsg.VAttSpMsgRcvCnt);
            printf("VControlModeMsgRcvCnt: %ld\n", HkMsg.VControlModeMsgRcvCnt);
            printf("VGlobalPositionMsgRcvCnt: %ld\n", HkMsg.VGlobalPositionMsgRcvCnt);
            printf("VLandDetectedMsgRcvCnt: %ld\n", HkMsg.VLandDetectedMsgRcvCnt);
            printf("VehicleStatusMsgRcvCnt: %ld\n", HkMsg.VehicleStatusMsgRcvCnt);
            printf("ActuatorControls0MsgSndCnt: %ld\n", HkMsg.ActuatorControls0MsgSndCnt);
            printf("ActuatorControls2MsgSndCnt: %ld\n", HkMsg.ActuatorControls2MsgSndCnt);
            printf("VehicleRatesSetpointMsgSndCnt: %ld\n", HkMsg.VehicleRatesSetpointMsgSndCnt);
            printf("HkMsgSndCnt: %ld\n", HkMsg.HkMsgSndCnt);

            break;
        }
        default:
        {
            printf("Sent Invalid Message\n");
            break;
        }
    }

    return CFE_SUCCESS;
}


/**
 * Test FAC AppMain(), Nominal - SendHK
 */
void Test_FAC_AppMain_Nominal_SendHK(void)
{
    /* The following will emulate the behavior of receiving FAC_SEND_HK_MID message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Used to verify HK was transmitted correctly. */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
               (void*)&Test_FAC_AppMain_Nominal_SendHK_SendMsgHook);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True (((oFAC.HkTlm.SendHkMsgRcvCnt == 1) && (oFAC.HkTlm.HkMsgSndCnt == 1)),
                    "AppMain_Nominal_SendHK");
}


/**
 * Test FAC AppMain(), Nominal - RunController
 */
void Test_FAC_AppMain_Nominal_RunController(void)
{
    /* The following will emulate the behavior of receiving FAC_RUN_CONTROLLER_MID message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

}


/**
 * Test FAC AppMain(), ProcessIncomingDataHook
 */
int32 Test_FAC_AppMain_ProcessIncomingDataHook(void *dst, void *src, uint32 size)
{
    unsigned char *pBuff = NULL;
    int i = 0;

    pBuff = (unsigned char*)src;
    printf("###IncomingDataHook: ");
    for (i = 0; i < size; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff ++;
    }
    printf("\n");

    if (oFAC.HkTlm.usDataErrCnt > 0)
    {
        return oFAC.HkTlm.usDataErrCnt;
    }
    else
    {
        return 0;
    }
}


/**
 * Test FAC AppMain(), ProcessIncomingData - DataPipeError
 */
void Test_FAC_AppMain_ProcessIncomingData_DataPipeError(void)
{
    int32              expected = CFE_SB_PIPE_RD_ERR;
    int32              SchPipe;
    FAC_NoArgCmd_t     InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    SchPipe = Ut_CFE_SB_CreatePipe("FAC_SCH_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, FAC_RUN_CONTROLLER_MID, sizeof(FAC_NoArgCmd_t), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)0);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)SchPipe);

    /* The following will emulate the behavior of SCH pipe reading error */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, expected, 2);

    /* Execute the function being tested */
    oFAC.AppMain();
}


/**
 * Test FAC AppMain(), ProcessIncomingData - InvalidMsgID
 */
void Test_FAC_AppMain_ProcessIncomingData_InvalidMsgID(void)
{
    int32 DataPipe;
    PX4_ActuatorArmedMsg_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_ACTUATOR_ARMED_MID, sizeof(PX4_ActuatorArmedMsg_t), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
#if 1
    UtAssert_True(oFAC.HkTlm.usDataErrCnt == 1, "AppMain_ProcessIncomingData_InvalidMsgID");
#else
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
    UtAssert_EventSent(FAC_MSGID_ERR_EID, CFE_EVS_ERROR,
                      "Recvd invalid DATA msgId (0x0000)", "Error Event Sent");
#endif
}


/**
 * Test FAC AppMain(), ProcessIncomingData - Airspeed
 */
void Test_FAC_AppMain_ProcessIncomingData_Airspeed(void)
{
    int32              DataPipe;
    PX4_AirspeedMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_AIRSPEED_MID, sizeof(PX4_AirspeedMsg_t), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FAC_AppMain_ProcessIncomingDataHook);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True(oFAC.HkTlm.AirSpeedMsgRcvCnt == 1, "AppMain_ProcessIncomingData_Airspeed");
}


/**
 * Test FAC AppMain(), ProcessIncomingData - BatteryStatus
 */
void Test_FAC_AppMain_ProcessIncomingData_BatteryStatus(void)
{
    int32                   DataPipe;
    PX4_BatteryStatusMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_BATTERY_STATUS_MID,
                     sizeof(PX4_BatteryStatusMsg_t), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FAC_AppMain_ProcessIncomingDataHook);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True(oFAC.HkTlm.BatteryStatusMsgRcvCnt == 1,
                  "AppMain_ProcessIncomingData_BatteryStatus");
}


/**
 * Test FAC AppMain(), ProcessIncomingData - ManualControlSp
 */
void Test_FAC_AppMain_ProcessIncomingData_ManualControlSp(void)
{
    int32              DataPipe;
    PX4_ManualControlSetpointMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_MANUAL_CONTROL_SETPOINT_MID,
                     sizeof(PX4_ManualControlSetpointMsg_t), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FAC_AppMain_ProcessIncomingDataHook);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True(oFAC.HkTlm.ManualControlSpMsgRcvCnt == 1,
                   "AppMain_ProcessIncomingData_ManualControlSp");
}


/**
 * Test FAC AppMain(), ProcessIncomingData - VAtt
 */
void Test_FAC_AppMain_ProcessIncomingData_VAtt(void)
{
    int32              DataPipe;
    PX4_VehicleAttitudeMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_VEHICLE_ATTITUDE_MID,
                     sizeof(PX4_VehicleAttitudeMsg_t), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FAC_AppMain_ProcessIncomingDataHook);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True(oFAC.HkTlm.VAttMsgRcvCnt == 1, "AppMain_ProcessIncomingData_VAtt");
}


/**
 * Test FAC AppMain(), ProcessIncomingData - VAttSp
 */
void Test_FAC_AppMain_ProcessIncomingData_VAttSp(void)
{
    int32              DataPipe;
    PX4_VehicleAttitudeSetpointMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_VEHICLE_ATTITUDE_SETPOINT_MID,
                     sizeof(PX4_VehicleAttitudeSetpointMsg_t), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FAC_AppMain_ProcessIncomingDataHook);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True(oFAC.HkTlm.VAttSpMsgRcvCnt == 1, "AppMain_ProcessIncomingData_VAttSp");
}


/**
 * Test FAC AppMain(), ProcessIncomingData - VControlMode
 */
void Test_FAC_AppMain_ProcessIncomingData_VControlMode(void)
{
    int32              DataPipe;
    PX4_VehicleControlModeMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_VEHICLE_CONTROL_MODE_MID,
                     sizeof(PX4_VehicleControlModeMsg_t), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FAC_AppMain_ProcessIncomingDataHook);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True(oFAC.HkTlm.VControlModeMsgRcvCnt == 1,
                   "AppMain_ProcessIncomingData_VControlMode");
}


/**
 * Test FAC AppMain(), ProcessIncomingData - VGlobalPosition
 */
void Test_FAC_AppMain_ProcessIncomingData_VGlobalPosition(void)
{
    int32              DataPipe;
    PX4_VehicleGlobalPositionMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_VEHICLE_GLOBAL_POSITION_MID,
                     sizeof(PX4_VehicleGlobalPositionMsg_t), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FAC_AppMain_ProcessIncomingDataHook);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True(oFAC.HkTlm.VGlobalPositionMsgRcvCnt == 1,
                  "AppMain_ProcessIncomingData_VGlobalPosition");
}


/**
 * Test FAC AppMain(), ProcessIncomingData - VLandDetected
 */
void Test_FAC_AppMain_ProcessIncomingData_VLandDetected(void)
{
    int32              DataPipe;
    PX4_VehicleLandDetectedMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_VEHICLE_LAND_DETECTED_MID,
                     sizeof(PX4_VehicleLandDetectedMsg_t), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FAC_AppMain_ProcessIncomingDataHook);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True(oFAC.HkTlm.VLandDetectedMsgRcvCnt == 1,
                   "AppMain_ProcessIncomingData_VLandDetected");
}


/**
 * Test FAC AppMain(), ProcessIncomingData - VehicleStatus
 */
void Test_FAC_AppMain_ProcessIncomingData_VehicleStatus(void)
{
    int32              DataPipe;
    PX4_VehicleStatusMsg_t  InMsg;

    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_VEHICLE_STATUS_MID, sizeof(PX4_VehicleStatusMsg_t), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    Ut_CFE_PSP_MEMUTILS_SetFunctionHook(UT_CFE_PSP_MEMUTILS_MEMCPY_INDEX,
                         (void*)&Test_FAC_AppMain_ProcessIncomingDataHook);

    /* Execute the function being tested */
    oFAC.AppMain();

    /* Verify results */
    UtAssert_True(oFAC.HkTlm.VehicleStatusMsgRcvCnt == 1,
                  "AppMain_ProcessIncomingData_VehicleStatus");
}


/**************************************************************************
 * Tests for FAC RunController()
 **************************************************************************/
/**
 * Test FAC GetPSPTimeHook
 */
void Test_FAC_GetPSPTimeHook(OS_time_t *LocalTime)
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


/**
 * Test FAC RunController(), SendEventHook
 */
int32 Test_FAC_RunController_SendEventHook
            (uint16 EventID, uint16 EventType, const char *EventText, ...)
{
    va_list  Ptr;
    char     Buf[256];

    SendEvent_HookCalledCnt++;

    va_start(Ptr, EventText);
    vsnprintf(Buf, (size_t)CFE_EVS_MAX_MESSAGE_LENGTH, EventText, Ptr);
    va_end(Ptr);

    printf("###RunController_SendEventHook:\n");
    printf("%s\n", Buf);

    return SendEvent_HookCalledCnt;
}


/**
 * Test FAC RunController, SendMsgtHook
 */
int32 Test_FAC_RunController_SendMsgHook(CFE_SB_Msg_t   *MsgPtr)
{
    unsigned char      *pBuff = NULL;
    uint16             msgLen = 0;
    int                i = 0;
    CFE_SB_MsgId_t     MsgId;
    time_t             localTime;
    struct tm          *loc_time;
    CFE_TIME_SysTime_t TimeFromMsg;

    pBuff = (unsigned char*)MsgPtr;
    msgLen = CFE_SB_GetTotalMsgLength(MsgPtr);             /* DataLenth + 7 */
    printf("###RunController_SendMsgHook: MsgLen(%u)\n", msgLen);
    for (i = 0; i < msgLen; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff++;
    }
    printf("\n");

    TimeFromMsg = CFE_SB_GetMsgTime(MsgPtr);
    localTime = FAC_Test_GetTimeFromMsg(TimeFromMsg);
    loc_time = localtime(&localTime);
    printf("TimeFromMessage: %s", asctime(loc_time));

    MsgId = CFE_SB_GetMsgId(MsgPtr);
    switch (MsgId)
    {
        case PX4_VEHICLE_RATES_SETPOINT_MID:
        {
            PX4_VehicleRatesSetpointMsg_t  VRatesSp;
            CFE_PSP_MemCpy((void*)&VRatesSp, (void*)MsgPtr, sizeof(VRatesSp));

            printf("Sent PX4_VEHICLE_RATES_SETPOINT_MID:\n");
            localTime = FAC_Test_GetTimeFromTimestamp(VRatesSp.Timestamp);
            loc_time = localtime(&localTime);
            printf("Timestamp: %s", asctime(loc_time));
            printf("Roll: %f\n", VRatesSp.Roll);
            printf("Pitch: %f\n", VRatesSp.Pitch);
            printf("Yaw: %f\n", VRatesSp.Yaw);
            printf("Thrust: %f\n\n", VRatesSp.Thrust);
            break;
        }

        case PX4_ACTUATOR_CONTROLS_0_MID:
        {
            PX4_ActuatorControlsMsg_t  AControls0;
            CFE_PSP_MemCpy((void*)&AControls0, (void*)MsgPtr, sizeof(AControls0));

            printf("Sent PX4_ACTUATOR_CONTROLS_0_MID\n");
            localTime = FAC_Test_GetTimeFromTimestamp(AControls0.Timestamp);
            loc_time = localtime(&localTime);
            printf("Timestamp: %s", asctime(loc_time));
            printf("Control[Roll]: %f\n", AControls0.Control[0]);
            printf("Control[Pitch]: %f\n", AControls0.Control[1]);
            printf("Control[Yaw]: %f\n", AControls0.Control[2]);
            printf("Control[Throttle]: %f\n", AControls0.Control[3]);
            printf("Control[Flaps]: %f\n", AControls0.Control[4]);
            printf("Control[Spoilers]: %f\n", AControls0.Control[5]);
            printf("Control[Airbrakes]: %f\n", AControls0.Control[6]);
            printf("Control[LandingGear]: %f\n\n", AControls0.Control[7]);
            break;
        }

        case PX4_ACTUATOR_CONTROLS_2_MID:
        {
            PX4_ActuatorControlsMsg_t  AControls2;
            CFE_PSP_MemCpy((void*)&AControls2, (void*)MsgPtr, sizeof(AControls2));

            printf("Sent PX4_ACTUATOR_CONTROLS_2_MID\n");
            localTime = FAC_Test_GetTimeFromTimestamp(AControls2.Timestamp);
            loc_time = localtime(&localTime);
            printf("Timestamp: %s", asctime(loc_time));
            printf("Control[Roll]: %f\n", AControls2.Control[0]);
            printf("Control[Pitch]: %f\n", AControls2.Control[1]);
            printf("Control[Yaw]: %f\n", AControls2.Control[2]);
            printf("Control[Throttle]: %f\n", AControls2.Control[3]);
            printf("Control[Flaps]: %f\n", AControls2.Control[4]);
            printf("Control[Spoilers]: %f\n", AControls2.Control[5]);
            printf("Control[Airbrakes]: %f\n", AControls2.Control[6]);
            printf("Control[LandingGear]: %f\n\n", AControls2.Control[7]);
            break;
        }

        default:
        {
            printf("Sent Invalid Message ID\n");
            break;
        }
    }

    return 0;
}


/**
 * Test FAC RunController, TailSitter
 */
void Test_FAC_RunController_TailSitter(void)
{
    int32                             DataPipe;
    int32                             SchPipe;
    FAC_NoArgCmd_t                    RController;
    PX4_VehicleStatusMsg_t            VStatus;
    PX4_BatteryStatusMsg_t            BatStat;
    PX4_VehicleLandDetectedMsg_t      VLDetect;
    PX4_VehicleGlobalPositionMsg_t    VGlobalPos;
    PX4_VehicleAttitudeMsg_t          VAtt;
    PX4_VehicleAttitudeSetpointMsg_t  VAttSp;
    PX4_ManualControlSetpointMsg_t    MCSp;
    PX4_AirspeedMsg_t                 ASpeed;
    PX4_VehicleControlModeMsg_t       VCMode;

    /* Set inputs */
    SchPipe = Ut_CFE_SB_CreatePipe("FAC_SCH_PIPE");

    CFE_SB_InitMsg ((void*)&RController, FAC_RUN_CONTROLLER_MID, sizeof(RController), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&RController, (uint16)0);
    Ut_CFE_SB_AddMsgToPipe((void*)&RController, (CFE_SB_PipeId_t)SchPipe);

    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");

    CFE_SB_InitMsg ((void*)&VStatus, PX4_VEHICLE_STATUS_MID, sizeof(VStatus), TRUE);
    VStatus.Timestamp = FAC_Test_GetTimeUs();
    VStatus.IsVtol = TRUE;
    VStatus.IsRotaryWing = FALSE;
    VStatus.InTransitionMode = FALSE;                       // fix this
    VStatus.EngineFailure = FALSE;
    VStatus.EngineFailureCmd = FALSE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VStatus);
    Ut_CFE_SB_AddMsgToPipe((void*)&VStatus, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&BatStat, PX4_BATTERY_STATUS_MID, sizeof(BatStat), TRUE);
    BatStat.Timestamp = FAC_Test_GetTimeUs();
    BatStat.Voltage = 0.0f;                                 // fix this
    BatStat.VoltageFiltered = 0.0f;                         // fix this
    BatStat.Current = 0.0f;                                 // fix this
    BatStat.CurrentFiltered = 0.0f;                         // fix this
    BatStat.Discharged = 0.0f;                              // fix this
    BatStat.Remaining = 0.0f;                               // fix this
    BatStat.Scale = 0.0f;                                   // fix this
    BatStat.CellCount = 0;                                  // fix this
    BatStat.Connected = FALSE;                              // fix this
    BatStat.Warning = PX4_BATTERY_WARNING_NONE;             // fix this
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&BatStat);
    Ut_CFE_SB_AddMsgToPipe((void*)&BatStat, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VLDetect, PX4_VEHICLE_LAND_DETECTED_MID, sizeof(VLDetect), TRUE);
    VLDetect.Timestamp = FAC_Test_GetTimeUs();
    VLDetect.AltMax = 0.0f;                                 // fix this
    VLDetect.Landed = FALSE;
    VLDetect.Freefall = FALSE;
    VLDetect.GroundContact = FALSE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VLDetect);
    Ut_CFE_SB_AddMsgToPipe((void*)&VLDetect, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VGlobalPos, PX4_VEHICLE_GLOBAL_POSITION_MID,
                    sizeof(VGlobalPos), TRUE);
    VGlobalPos.Timestamp = FAC_Test_GetTimeUs();
    VGlobalPos.TimeUtcUsec = FAC_Test_GetTimeUs();
    VGlobalPos.Lat = 9.0;
    VGlobalPos.Lon = 4.0;
    VGlobalPos.Alt = 16.0f;
    VGlobalPos.DeltaLatLon[0] = 6.0;
    VGlobalPos.DeltaLatLon[1] = 0.0;
    VGlobalPos.DeltaAlt = 11.0f;
    VGlobalPos.LatLonResetCounter = 19;
    VGlobalPos.AltResetCounter = 10;
    VGlobalPos.VelN = 1.0f;
    VGlobalPos.VelE = 14.0f;
    VGlobalPos.VelD = 15.0f;
    VGlobalPos.Yaw = 3.0f;
    VGlobalPos.EpH = 20.0f;
    VGlobalPos.EpV = 18.0f;
    VGlobalPos.EvH = 17.0f;
    VGlobalPos.EvV = 22.0f;
    VGlobalPos.TerrainAlt = 21.0f;
    VGlobalPos.PressureAlt = 13.0f;
    VGlobalPos.TerrainAltValid = TRUE;
    VGlobalPos.DeadReckoning = TRUE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VGlobalPos);
    Ut_CFE_SB_AddMsgToPipe((void*)&VGlobalPos, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VAtt, PX4_VEHICLE_ATTITUDE_MID, sizeof(VAtt), TRUE);
    VAtt.Timestamp = FAC_Test_GetTimeUs();
    VAtt.RollSpeed = 0.0f;                                 // fix this
    VAtt.PitchSpeed = 0.0f;                                // fix this
    VAtt.YawSpeed = 0.0f;                                  // fix this
    VAtt.Q[0] = 0.7083791494f;
    VAtt.Q[1] = -0.0311437733f;
    VAtt.Q[2] = -0.0508509092f;
    VAtt.Q[3] = 0.7033087611f;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VAtt);
    Ut_CFE_SB_AddMsgToPipe((void*)&VAtt, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VAttSp, PX4_VEHICLE_ATTITUDE_SETPOINT_MID,
                    sizeof(VAttSp), TRUE);
    VAttSp.Timestamp = FAC_Test_GetTimeUs();
    VAttSp.RollBody = 0.0f;                                // fix this
    VAttSp.PitchBody = 0.0f;                               // fix this
    VAttSp.YawBody = 0.0f;                                 // fix this
    VAttSp.YawSpMoveRate = 0.0000000000f;
    VAttSp.Q_D[0] = 0.7084835768f;
    VAttSp.Q_D[1] = -0.0144501235f;
    VAttSp.Q_D[2] = -0.0355133303f;
    VAttSp.Q_D[3] = 0.7046850324f;
    VAttSp.Q_D_Valid = TRUE;                               // fix this
    VAttSp.Thrust = 0.3333882987f;
    VAttSp.RollResetIntegral = TRUE;
    VAttSp.PitchResetIntegral = TRUE;
    VAttSp.YawResetIntegral = TRUE;
    VAttSp.FwControlYaw = FALSE;
    VAttSp.DisableMcYawControl = TRUE;
    VAttSp.ApplyFlaps = TRUE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VAttSp);
    Ut_CFE_SB_AddMsgToPipe((void*)&VAttSp, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&MCSp, PX4_MANUAL_CONTROL_SETPOINT_MID, sizeof(MCSp), TRUE);
    MCSp.Timestamp = FAC_Test_GetTimeUs();
    MCSp.X = 0.0f;                                        // fix this
    MCSp.Y = 0.0f;                                        // fix this
    MCSp.Z = 0.0f;                                        // fix this
    MCSp.R = 0.0f;                                        // fix this
    MCSp.Flaps = 0.0f;                                    // fix this
    MCSp.Aux1 = 0.0f;                                     // fix this
    MCSp.Aux2 = 0.0f;                                     // fix this
    MCSp.Aux3 = 0.0f;                                     // fix this
    MCSp.Aux4 = 0.0f;                                     // fix this
    MCSp.Aux5 = 0.0f;                                     // fix this
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&MCSp);
    Ut_CFE_SB_AddMsgToPipe((void*)&MCSp, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&ASpeed, PX4_AIRSPEED_MID, sizeof(ASpeed), TRUE);
    ASpeed.Timestamp = FAC_Test_GetTimeUs();
    ASpeed.IndicatedAirspeed = 1.0f;                      // fix this
    ASpeed.TrueAirspeed = 0.0f;                           // fix this
    ASpeed.TrueAirspeedUnfiltered = 0.0f;                 // fix this
    ASpeed.AirTemperature = 0.0f;                         // fix this
    ASpeed.Confidence = 0.0f;                             // fix this
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&ASpeed);
    Ut_CFE_SB_AddMsgToPipe((void*)&ASpeed, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VCMode, PX4_VEHICLE_CONTROL_MODE_MID, sizeof(VCMode), TRUE);
    VCMode.Timestamp = FAC_Test_GetTimeUs();
    VCMode.ExternalManualOverrideOk = FALSE;
    VCMode.SystemHilEnabled = FALSE;
    VCMode.ControlManualEnabled = TRUE;
    VCMode.ControlAutoEnabled = FALSE;
    VCMode.ControlOffboardEnabled = FALSE;
    VCMode.ControlRatesEnabled = TRUE;
    VCMode.ControlAttitudeEnabled = TRUE;
    VCMode.ControlRattitudeEnabled = TRUE;
    VCMode.ControlForceEnabled = FALSE;
    VCMode.ControlAccelerationEnabled = FALSE;
    VCMode.ControlVelocityEnabled = TRUE;
    VCMode.ControlPositionEnabled = FALSE;
    VCMode.ControlAltitudeEnabled = TRUE;
    VCMode.ControlClimbRateEnabled = FALSE;
    VCMode.ControlTerminationEnabled = FALSE;
    VCMode.ControlFixedHdgEnabled = FALSE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VCMode);
    Ut_CFE_SB_AddMsgToPipe((void*)&VCMode, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 9);

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
               (void*)&Test_FAC_GetPSPTimeHook);
    SendEvent_HookCalledCnt = 0;
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FAC_RunController_SendEventHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
               (void*)&Test_FAC_RunController_SendMsgHook);

    /* Execute the function being tested */
    oFAC.AppMain();
}


void Test_FAC_RunController_RotaryWing(void)
{
    int32                             DataPipe;
    int32                             SchPipe;
    FAC_NoArgCmd_t                    RController;
    PX4_VehicleStatusMsg_t            VStatus;
    PX4_BatteryStatusMsg_t            BatStat;
    PX4_VehicleLandDetectedMsg_t      VLDetect;
    PX4_VehicleGlobalPositionMsg_t    VGlobalPos;
    PX4_VehicleAttitudeMsg_t          VAtt;
    PX4_VehicleAttitudeSetpointMsg_t  VAttSp;
    PX4_ManualControlSetpointMsg_t    MCSp;
    PX4_AirspeedMsg_t                 ASpeed;
    PX4_VehicleControlModeMsg_t       VCMode;

    /* Set inputs */
    SchPipe = Ut_CFE_SB_CreatePipe("FAC_SCH_PIPE");

    CFE_SB_InitMsg ((void*)&RController, FAC_RUN_CONTROLLER_MID, sizeof(RController), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&RController, (uint16)0);
    Ut_CFE_SB_AddMsgToPipe((void*)&RController, (CFE_SB_PipeId_t)SchPipe);

    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");

    CFE_SB_InitMsg ((void*)&VStatus, PX4_VEHICLE_STATUS_MID, sizeof(VStatus), TRUE);
    VStatus.Timestamp = FAC_Test_GetTimeUs();
    VStatus.IsVtol = FALSE;
    VStatus.IsRotaryWing = TRUE;
    VStatus.InTransitionMode = FALSE;                       // fix this
    VStatus.EngineFailure = FALSE;
    VStatus.EngineFailureCmd = FALSE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VStatus);
    Ut_CFE_SB_AddMsgToPipe((void*)&VStatus, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&BatStat, PX4_BATTERY_STATUS_MID, sizeof(BatStat), TRUE);
    BatStat.Timestamp = FAC_Test_GetTimeUs();
    BatStat.Voltage = 0.0f;                                 // fix this
    BatStat.VoltageFiltered = 0.0f;                         // fix this
    BatStat.Current = 0.0f;                                 // fix this
    BatStat.CurrentFiltered = 0.0f;                         // fix this
    BatStat.Discharged = 0.0f;                              // fix this
    BatStat.Remaining = 0.0f;                               // fix this
    BatStat.Scale = 0.1f;                                   // fix this
    BatStat.CellCount = 0;                                  // fix this
    BatStat.Connected = FALSE;                              // fix this
    BatStat.Warning = PX4_BATTERY_WARNING_NONE;             // fix this
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&BatStat);
    Ut_CFE_SB_AddMsgToPipe((void*)&BatStat, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VLDetect, PX4_VEHICLE_LAND_DETECTED_MID, sizeof(VLDetect), TRUE);
    VLDetect.Timestamp = FAC_Test_GetTimeUs();
    VLDetect.AltMax = 0.0f;                                 // fix this
    VLDetect.Landed = FALSE;
    VLDetect.Freefall = FALSE;
    VLDetect.GroundContact = FALSE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VLDetect);
    Ut_CFE_SB_AddMsgToPipe((void*)&VLDetect, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VGlobalPos, PX4_VEHICLE_GLOBAL_POSITION_MID,
                    sizeof(VGlobalPos), TRUE);
    VGlobalPos.Timestamp = FAC_Test_GetTimeUs();
    VGlobalPos.TimeUtcUsec = FAC_Test_GetTimeUs();
    VGlobalPos.Lat = 9.0;
    VGlobalPos.Lon = 4.0;
    VGlobalPos.Alt = 16.0f;
    VGlobalPos.DeltaLatLon[0] = 6.0;
    VGlobalPos.DeltaLatLon[1] = 0.0;
    VGlobalPos.DeltaAlt = 11.0f;
    VGlobalPos.LatLonResetCounter = 19;
    VGlobalPos.AltResetCounter = 10;
    VGlobalPos.VelN = 1.0f;
    VGlobalPos.VelE = 14.0f;
    VGlobalPos.VelD = 15.0f;
    VGlobalPos.Yaw = 3.0f;
    VGlobalPos.EpH = 20.0f;
    VGlobalPos.EpV = 18.0f;
    VGlobalPos.EvH = 17.0f;
    VGlobalPos.EvV = 22.0f;
    VGlobalPos.TerrainAlt = 21.0f;
    VGlobalPos.PressureAlt = 13.0f;
    VGlobalPos.TerrainAltValid = TRUE;
    VGlobalPos.DeadReckoning = TRUE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VGlobalPos);
    Ut_CFE_SB_AddMsgToPipe((void*)&VGlobalPos, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VAtt, PX4_VEHICLE_ATTITUDE_MID, sizeof(VAtt), TRUE);
    VAtt.Timestamp = FAC_Test_GetTimeUs();
    VAtt.RollSpeed = 0.0f;                              // fix this
    VAtt.PitchSpeed = 0.0f;                             // fix this
    VAtt.YawSpeed = 0.0f;                               // fix this
    VAtt.Q[0] = 0.7083791494f;
    VAtt.Q[1] = -0.0311437733f;
    VAtt.Q[2] = -0.0508509092f;
    VAtt.Q[3] = 0.7033087611f;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VAtt);
    Ut_CFE_SB_AddMsgToPipe((void*)&VAtt, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VAttSp, PX4_VEHICLE_ATTITUDE_SETPOINT_MID,
                    sizeof(VAttSp), TRUE);
    VAttSp.Timestamp = FAC_Test_GetTimeUs();
    VAttSp.RollBody = 0.0f;                            // fix this
    VAttSp.PitchBody = 0.0f;                           // fix this
    VAttSp.YawBody = 0.0f;                             // fix this
    VAttSp.YawSpMoveRate = 0.0000000000f;
    VAttSp.Q_D[0] = 0.7084835768f;
    VAttSp.Q_D[1] = -0.0144501235f;
    VAttSp.Q_D[2] = -0.0355133303f;
    VAttSp.Q_D[3] = 0.7046850324f;
    VAttSp.Q_D_Valid = TRUE;                           // fix this
    VAttSp.Thrust = 0.3333882987f;
    VAttSp.RollResetIntegral = TRUE;
    VAttSp.PitchResetIntegral = TRUE;
    VAttSp.YawResetIntegral = TRUE;
    VAttSp.FwControlYaw = FALSE;
    VAttSp.DisableMcYawControl = TRUE;
    VAttSp.ApplyFlaps = TRUE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VAttSp);
    Ut_CFE_SB_AddMsgToPipe((void*)&VAttSp, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&MCSp, PX4_MANUAL_CONTROL_SETPOINT_MID, sizeof(MCSp), TRUE);
    MCSp.Timestamp = FAC_Test_GetTimeUs();
    MCSp.X = 0.2f;                                     // fix this
    MCSp.Y = 0.3f;                                     // fix this
    MCSp.Z = 0.0f;                                     // fix this
    MCSp.R = 0.0f;                                     // fix this
    MCSp.Flaps = 0.0f;                                 // fix this
    MCSp.Aux1 = 0.0f;                                  // fix this
    MCSp.Aux2 = 0.0f;                                  // fix this
    MCSp.Aux3 = 0.0f;                                  // fix this
    MCSp.Aux4 = 0.0f;                                  // fix this
    MCSp.Aux5 = 0.0f;                                  // fix this
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&MCSp);
    Ut_CFE_SB_AddMsgToPipe((void*)&MCSp, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&ASpeed, PX4_AIRSPEED_MID, sizeof(ASpeed), TRUE);
    ASpeed.Timestamp = FAC_Test_GetTimeUs();
    ASpeed.IndicatedAirspeed = 1.0f;                   // fix this
    ASpeed.TrueAirspeed = 0.0f;                        // fix this
    ASpeed.TrueAirspeedUnfiltered = 0.0f;              // fix this
    ASpeed.AirTemperature = 0.0f;                      // fix this
    ASpeed.Confidence = 0.0f;                          // fix this
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&ASpeed);
    Ut_CFE_SB_AddMsgToPipe((void*)&ASpeed, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VCMode, PX4_VEHICLE_CONTROL_MODE_MID, sizeof(VCMode), TRUE);
    VCMode.Timestamp = FAC_Test_GetTimeUs();
    VCMode.ExternalManualOverrideOk = FALSE;
    VCMode.SystemHilEnabled = FALSE;
    VCMode.ControlManualEnabled = TRUE;
    VCMode.ControlAutoEnabled = FALSE;
    VCMode.ControlOffboardEnabled = FALSE;
    VCMode.ControlRatesEnabled = TRUE;
    VCMode.ControlAttitudeEnabled = TRUE;
    VCMode.ControlRattitudeEnabled = TRUE;
    VCMode.ControlForceEnabled = FALSE;
    VCMode.ControlAccelerationEnabled = FALSE;
    VCMode.ControlVelocityEnabled = TRUE;
    VCMode.ControlPositionEnabled = FALSE;
    VCMode.ControlAltitudeEnabled = TRUE;
    VCMode.ControlClimbRateEnabled = FALSE;
    VCMode.ControlTerminationEnabled = FALSE;
    VCMode.ControlFixedHdgEnabled = FALSE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VCMode);
    Ut_CFE_SB_AddMsgToPipe((void*)&VCMode, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 9);

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
               (void*)&Test_FAC_GetPSPTimeHook);
    SendEvent_HookCalledCnt = 0;
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FAC_RunController_SendEventHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
               (void*)&Test_FAC_RunController_SendMsgHook);

    /* Execute the function being tested */
    oFAC.AppMain();
}


/**
 * Test FAC RunController, AttitudeManual
 */
void Test_FAC_RunController_AttitudeManual(void)
{
    int32                             DataPipe;
    int32                             SchPipe;
    FAC_NoArgCmd_t                    RController;
    PX4_VehicleStatusMsg_t            VStatus;
    PX4_BatteryStatusMsg_t            BatStat;
    PX4_VehicleLandDetectedMsg_t      VLDetect;
    PX4_VehicleGlobalPositionMsg_t    VGlobalPos;
    PX4_VehicleAttitudeMsg_t          VAtt;
    PX4_VehicleAttitudeSetpointMsg_t  VAttSp;
    PX4_ManualControlSetpointMsg_t    MCSp;
    PX4_AirspeedMsg_t                 ASpeed;
    PX4_VehicleControlModeMsg_t       VCMode;

    /* Set inputs */
    SchPipe = Ut_CFE_SB_CreatePipe("FAC_SCH_PIPE");

    CFE_SB_InitMsg ((void*)&RController, FAC_RUN_CONTROLLER_MID, sizeof(RController), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&RController, (uint16)0);
    Ut_CFE_SB_AddMsgToPipe((void*)&RController, (CFE_SB_PipeId_t)SchPipe);

    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");

    CFE_SB_InitMsg ((void*)&VStatus, PX4_VEHICLE_STATUS_MID, sizeof(VStatus), TRUE);
    VStatus.Timestamp = FAC_Test_GetTimeUs();
    VStatus.IsVtol = FALSE;
    VStatus.IsRotaryWing = FALSE;
    VStatus.InTransitionMode = FALSE;                       // fix this
    VStatus.EngineFailure = FALSE;
    VStatus.EngineFailureCmd = FALSE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VStatus);
    Ut_CFE_SB_AddMsgToPipe((void*)&VStatus, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&BatStat, PX4_BATTERY_STATUS_MID, sizeof(BatStat), TRUE);
    BatStat.Timestamp = FAC_Test_GetTimeUs();
    BatStat.Voltage = 0.0f;                                 // fix this
    BatStat.VoltageFiltered = 0.0f;                         // fix this
    BatStat.Current = 0.0f;                                 // fix this
    BatStat.CurrentFiltered = 0.0f;                         // fix this
    BatStat.Discharged = 0.0f;                              // fix this
    BatStat.Remaining = 0.0f;                               // fix this
    BatStat.Scale = 0.1f;                                   // fix this
    BatStat.CellCount = 0;                                  // fix this
    BatStat.Connected = FALSE;                              // fix this
    BatStat.Warning = PX4_BATTERY_WARNING_NONE;             // fix this
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&BatStat);
    Ut_CFE_SB_AddMsgToPipe((void*)&BatStat, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VLDetect, PX4_VEHICLE_LAND_DETECTED_MID, sizeof(VLDetect), TRUE);
    VLDetect.Timestamp = FAC_Test_GetTimeUs();
    VLDetect.AltMax = 0.0f;                                 // fix this
    VLDetect.Landed = FALSE;
    VLDetect.Freefall = FALSE;
    VLDetect.GroundContact = FALSE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VLDetect);
    Ut_CFE_SB_AddMsgToPipe((void*)&VLDetect, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VGlobalPos, PX4_VEHICLE_GLOBAL_POSITION_MID,
                    sizeof(VGlobalPos), TRUE);
    VGlobalPos.Timestamp = FAC_Test_GetTimeUs();
    VGlobalPos.TimeUtcUsec = FAC_Test_GetTimeUs();
    VGlobalPos.Lat = 9.0;
    VGlobalPos.Lon = 4.0;
    VGlobalPos.Alt = 16.0f;
    VGlobalPos.DeltaLatLon[0] = 6.0;
    VGlobalPos.DeltaLatLon[1] = 0.0;
    VGlobalPos.DeltaAlt = 11.0f;
    VGlobalPos.LatLonResetCounter = 19;
    VGlobalPos.AltResetCounter = 10;
    VGlobalPos.VelN = 1.0f;
    VGlobalPos.VelE = 14.0f;
    VGlobalPos.VelD = 15.0f;
    VGlobalPos.Yaw = 3.0f;
    VGlobalPos.EpH = 20.0f;
    VGlobalPos.EpV = 18.0f;
    VGlobalPos.EvH = 17.0f;
    VGlobalPos.EvV = 22.0f;
    VGlobalPos.TerrainAlt = 21.0f;
    VGlobalPos.PressureAlt = 13.0f;
    VGlobalPos.TerrainAltValid = TRUE;
    VGlobalPos.DeadReckoning = TRUE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VGlobalPos);
    Ut_CFE_SB_AddMsgToPipe((void*)&VGlobalPos, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VAtt, PX4_VEHICLE_ATTITUDE_MID, sizeof(VAtt), TRUE);
    VAtt.Timestamp = FAC_Test_GetTimeUs();
    VAtt.RollSpeed = 0.0f;                              // fix this
    VAtt.PitchSpeed = 0.0f;                             // fix this
    VAtt.YawSpeed = 0.0f;                               // fix this
    VAtt.Q[0] = 0.7083791494f;
    VAtt.Q[1] = -0.0311437733f;
    VAtt.Q[2] = -0.0508509092f;
    VAtt.Q[3] = 0.7033087611f;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VAtt);
    Ut_CFE_SB_AddMsgToPipe((void*)&VAtt, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VAttSp, PX4_VEHICLE_ATTITUDE_SETPOINT_MID,
                    sizeof(VAttSp), TRUE);
    VAttSp.Timestamp = FAC_Test_GetTimeUs();
    VAttSp.RollBody = 0.0f;                            // fix this
    VAttSp.PitchBody = 0.0f;                           // fix this
    VAttSp.YawBody = 0.0f;                             // fix this
    VAttSp.YawSpMoveRate = 0.0000000000f;
    VAttSp.Q_D[0] = 0.7084835768f;
    VAttSp.Q_D[1] = -0.0144501235f;
    VAttSp.Q_D[2] = -0.0355133303f;
    VAttSp.Q_D[3] = 0.7046850324f;
    VAttSp.Q_D_Valid = TRUE;                           // fix this
    VAttSp.Thrust = 0.3333882987f;
    VAttSp.RollResetIntegral = TRUE;
    VAttSp.PitchResetIntegral = TRUE;
    VAttSp.YawResetIntegral = TRUE;
    VAttSp.FwControlYaw = FALSE;
    VAttSp.DisableMcYawControl = TRUE;
    VAttSp.ApplyFlaps = TRUE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VAttSp);
    Ut_CFE_SB_AddMsgToPipe((void*)&VAttSp, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&MCSp, PX4_MANUAL_CONTROL_SETPOINT_MID, sizeof(MCSp), TRUE);
    MCSp.Timestamp = FAC_Test_GetTimeUs();
    MCSp.X = 0.2f;                                     // fix this
    MCSp.Y = 0.3f;                                     // fix this
    MCSp.Z = 0.0f;                                     // fix this
    MCSp.R = 0.0f;                                     // fix this
    MCSp.Flaps = 0.0f;                                 // fix this
    MCSp.Aux1 = 0.0f;                                  // fix this
    MCSp.Aux2 = 0.0f;                                  // fix this
    MCSp.Aux3 = 0.0f;                                  // fix this
    MCSp.Aux4 = 0.0f;                                  // fix this
    MCSp.Aux5 = 0.0f;                                  // fix this
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&MCSp);
    Ut_CFE_SB_AddMsgToPipe((void*)&MCSp, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&ASpeed, PX4_AIRSPEED_MID, sizeof(ASpeed), TRUE);
    ASpeed.Timestamp = FAC_Test_GetTimeUs();
    ASpeed.IndicatedAirspeed = 1.0f;                   // fix this
    ASpeed.TrueAirspeed = 0.0f;                        // fix this
    ASpeed.TrueAirspeedUnfiltered = 0.0f;              // fix this
    ASpeed.AirTemperature = 0.0f;                      // fix this
    ASpeed.Confidence = 0.0f;                          // fix this
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&ASpeed);
    Ut_CFE_SB_AddMsgToPipe((void*)&ASpeed, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VCMode, PX4_VEHICLE_CONTROL_MODE_MID, sizeof(VCMode), TRUE);
    VCMode.Timestamp = FAC_Test_GetTimeUs();
    VCMode.ExternalManualOverrideOk = FALSE;
    VCMode.SystemHilEnabled = FALSE;
    VCMode.ControlManualEnabled = TRUE;
    VCMode.ControlAutoEnabled = FALSE;
    VCMode.ControlOffboardEnabled = FALSE;
    VCMode.ControlRatesEnabled = TRUE;
    VCMode.ControlAttitudeEnabled = TRUE;
    VCMode.ControlRattitudeEnabled = TRUE;
    VCMode.ControlForceEnabled = FALSE;
    VCMode.ControlAccelerationEnabled = FALSE;
    VCMode.ControlVelocityEnabled = TRUE;
    VCMode.ControlPositionEnabled = FALSE;
    VCMode.ControlAltitudeEnabled = TRUE;
    VCMode.ControlClimbRateEnabled = FALSE;
    VCMode.ControlTerminationEnabled = FALSE;
    VCMode.ControlFixedHdgEnabled = FALSE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VCMode);
    Ut_CFE_SB_AddMsgToPipe((void*)&VCMode, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 9);

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
               (void*)&Test_FAC_GetPSPTimeHook);
    SendEvent_HookCalledCnt = 0;
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FAC_RunController_SendEventHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
               (void*)&Test_FAC_RunController_SendMsgHook);

    /* Execute the function being tested */
    oFAC.AppMain();

}


/**
 * Test FAC RunController, PureRateControl
 */
void Test_FAC_RunController_PureRateControl(void)
{
    int32                             DataPipe;
    int32                             SchPipe;
    FAC_NoArgCmd_t                    RController;
    PX4_VehicleStatusMsg_t            VStatus;
    PX4_BatteryStatusMsg_t            BatStat;
    PX4_VehicleLandDetectedMsg_t      VLDetect;
    PX4_VehicleGlobalPositionMsg_t    VGlobalPos;
    PX4_VehicleAttitudeMsg_t          VAtt;
    PX4_VehicleAttitudeSetpointMsg_t  VAttSp;
    PX4_ManualControlSetpointMsg_t    MCSp;
    PX4_AirspeedMsg_t                 ASpeed;
    PX4_VehicleControlModeMsg_t       VCMode;

    /* Set inputs */
    SchPipe = Ut_CFE_SB_CreatePipe("FAC_SCH_PIPE");

    CFE_SB_InitMsg ((void*)&RController, FAC_RUN_CONTROLLER_MID, sizeof(RController), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&RController, (uint16)0);
    Ut_CFE_SB_AddMsgToPipe((void*)&RController, (CFE_SB_PipeId_t)SchPipe);

    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");

    CFE_SB_InitMsg ((void*)&VStatus, PX4_VEHICLE_STATUS_MID, sizeof(VStatus), TRUE);
    VStatus.Timestamp = FAC_Test_GetTimeUs();
    VStatus.IsVtol = FALSE;
    VStatus.IsRotaryWing = FALSE;
    VStatus.InTransitionMode = FALSE;                       // fix this
    VStatus.EngineFailure = FALSE;
    VStatus.EngineFailureCmd = FALSE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VStatus);
    Ut_CFE_SB_AddMsgToPipe((void*)&VStatus, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&BatStat, PX4_BATTERY_STATUS_MID, sizeof(BatStat), TRUE);
    BatStat.Timestamp = FAC_Test_GetTimeUs();
    BatStat.Voltage = 0.0f;                                 // fix this
    BatStat.VoltageFiltered = 0.0f;                         // fix this
    BatStat.Current = 0.0f;                                 // fix this
    BatStat.CurrentFiltered = 0.0f;                         // fix this
    BatStat.Discharged = 0.0f;                              // fix this
    BatStat.Remaining = 0.0f;                               // fix this
    BatStat.Scale = 0.1f;                                   // fix this
    BatStat.CellCount = 0;                                  // fix this
    BatStat.Connected = FALSE;                              // fix this
    BatStat.Warning = PX4_BATTERY_WARNING_NONE;             // fix this
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&BatStat);
    Ut_CFE_SB_AddMsgToPipe((void*)&BatStat, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VLDetect, PX4_VEHICLE_LAND_DETECTED_MID, sizeof(VLDetect), TRUE);
    VLDetect.Timestamp = FAC_Test_GetTimeUs();
    VLDetect.AltMax = 0.0f;                                 // fix this
    VLDetect.Landed = FALSE;
    VLDetect.Freefall = FALSE;
    VLDetect.GroundContact = FALSE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VLDetect);
    Ut_CFE_SB_AddMsgToPipe((void*)&VLDetect, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VGlobalPos, PX4_VEHICLE_GLOBAL_POSITION_MID,
                    sizeof(VGlobalPos), TRUE);
    VGlobalPos.Timestamp = FAC_Test_GetTimeUs();
    VGlobalPos.TimeUtcUsec = FAC_Test_GetTimeUs();
    VGlobalPos.Lat = 9.0;
    VGlobalPos.Lon = 4.0;
    VGlobalPos.Alt = 16.0f;
    VGlobalPos.DeltaLatLon[0] = 6.0;
    VGlobalPos.DeltaLatLon[1] = 0.0;
    VGlobalPos.DeltaAlt = 11.0f;
    VGlobalPos.LatLonResetCounter = 19;
    VGlobalPos.AltResetCounter = 10;
    VGlobalPos.VelN = 1.0f;
    VGlobalPos.VelE = 14.0f;
    VGlobalPos.VelD = 15.0f;
    VGlobalPos.Yaw = 3.0f;
    VGlobalPos.EpH = 20.0f;
    VGlobalPos.EpV = 18.0f;
    VGlobalPos.EvH = 17.0f;
    VGlobalPos.EvV = 22.0f;
    VGlobalPos.TerrainAlt = 21.0f;
    VGlobalPos.PressureAlt = 13.0f;
    VGlobalPos.TerrainAltValid = TRUE;
    VGlobalPos.DeadReckoning = TRUE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VGlobalPos);
    Ut_CFE_SB_AddMsgToPipe((void*)&VGlobalPos, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VAtt, PX4_VEHICLE_ATTITUDE_MID, sizeof(VAtt), TRUE);
    VAtt.Timestamp = FAC_Test_GetTimeUs();
    VAtt.RollSpeed = 0.0f;                              // fix this
    VAtt.PitchSpeed = 0.0f;                             // fix this
    VAtt.YawSpeed = 0.0f;                               // fix this
    VAtt.Q[0] = 0.7083791494f;
    VAtt.Q[1] = -0.0311437733f;
    VAtt.Q[2] = -0.0508509092f;
    VAtt.Q[3] = 0.7033087611f;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VAtt);
    Ut_CFE_SB_AddMsgToPipe((void*)&VAtt, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VAttSp, PX4_VEHICLE_ATTITUDE_SETPOINT_MID,
                    sizeof(VAttSp), TRUE);
    VAttSp.Timestamp = FAC_Test_GetTimeUs();
    VAttSp.RollBody = 0.0f;                             // fix this
    VAttSp.PitchBody = 0.0f;                            // fix this
    VAttSp.YawBody = 0.0f;                              // fix this
    VAttSp.YawSpMoveRate = 0.0000000000f;
    VAttSp.Q_D[0] = 0.7084835768f;
    VAttSp.Q_D[1] = -0.0144501235f;
    VAttSp.Q_D[2] = -0.0355133303f;
    VAttSp.Q_D[3] = 0.7046850324f;
    VAttSp.Q_D_Valid = TRUE;                            // fix this
    VAttSp.Thrust = 0.3333882987f;
    VAttSp.RollResetIntegral = TRUE;
    VAttSp.PitchResetIntegral = TRUE;
    VAttSp.YawResetIntegral = TRUE;
    VAttSp.FwControlYaw = FALSE;
    VAttSp.DisableMcYawControl = TRUE;
    VAttSp.ApplyFlaps = TRUE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VAttSp);
    Ut_CFE_SB_AddMsgToPipe((void*)&VAttSp, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&MCSp, PX4_MANUAL_CONTROL_SETPOINT_MID, sizeof(MCSp), TRUE);
    MCSp.Timestamp = FAC_Test_GetTimeUs();
    MCSp.X = 0.2f;                                      // fix this
    MCSp.Y = 0.9f;                                      // fix this
    MCSp.Z = 0.0f;                                      // fix this
    MCSp.R = 0.0f;                                      // fix this
    MCSp.Flaps = 0.0f;                                  // fix this
    MCSp.Aux1 = 0.0f;                                   // fix this
    MCSp.Aux2 = 0.0f;                                   // fix this
    MCSp.Aux3 = 0.0f;                                   // fix this
    MCSp.Aux4 = 0.0f;                                   // fix this
    MCSp.Aux5 = 0.0f;                                   // fix this
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&MCSp);
    Ut_CFE_SB_AddMsgToPipe((void*)&MCSp, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&ASpeed, PX4_AIRSPEED_MID, sizeof(ASpeed), TRUE);
    ASpeed.Timestamp = FAC_Test_GetTimeUs();
    ASpeed.IndicatedAirspeed = 1.0f;                    // fix this
    ASpeed.TrueAirspeed = 0.0f;                         // fix this
    ASpeed.TrueAirspeedUnfiltered = 0.0f;               // fix this
    ASpeed.AirTemperature = 0.0f;                       // fix this
    ASpeed.Confidence = 0.0f;                           // fix this
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&ASpeed);
    Ut_CFE_SB_AddMsgToPipe((void*)&ASpeed, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VCMode, PX4_VEHICLE_CONTROL_MODE_MID, sizeof(VCMode), TRUE);
    VCMode.Timestamp = FAC_Test_GetTimeUs();
    VCMode.ExternalManualOverrideOk = FALSE;
    VCMode.SystemHilEnabled = FALSE;
    VCMode.ControlManualEnabled = TRUE;
    VCMode.ControlAutoEnabled = FALSE;
    VCMode.ControlOffboardEnabled = FALSE;
    VCMode.ControlRatesEnabled = TRUE;
    VCMode.ControlAttitudeEnabled = TRUE;
    VCMode.ControlRattitudeEnabled = TRUE;
    VCMode.ControlForceEnabled = FALSE;
    VCMode.ControlAccelerationEnabled = FALSE;
    VCMode.ControlVelocityEnabled = TRUE;
    VCMode.ControlPositionEnabled = FALSE;
    VCMode.ControlAltitudeEnabled = TRUE;
    VCMode.ControlClimbRateEnabled = FALSE;
    VCMode.ControlTerminationEnabled = FALSE;
    VCMode.ControlFixedHdgEnabled = FALSE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VCMode);
    Ut_CFE_SB_AddMsgToPipe((void*)&VCMode, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 9);

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
               (void*)&Test_FAC_GetPSPTimeHook);
    SendEvent_HookCalledCnt = 0;
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FAC_RunController_SendEventHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
               (void*)&Test_FAC_RunController_SendMsgHook);

    /* Execute the function being tested */
    oFAC.AppMain();
}


/**
 * Test FAC RunController(), ManualDirect
 */
void Test_FAC_RunController_ManualDirect(void)
{
    int32                             DataPipe;
    int32                             SchPipe;
    FAC_NoArgCmd_t                    RController;
    PX4_VehicleStatusMsg_t            VStatus;
    PX4_BatteryStatusMsg_t            BatStat;
    PX4_VehicleLandDetectedMsg_t      VLDetect;
    PX4_VehicleGlobalPositionMsg_t    VGlobalPos;
    PX4_VehicleAttitudeMsg_t          VAtt;
    PX4_VehicleAttitudeSetpointMsg_t  VAttSp;
    PX4_ManualControlSetpointMsg_t    MCSp;
    PX4_AirspeedMsg_t                 ASpeed;
    PX4_VehicleControlModeMsg_t       VCMode;

    /* Set inputs */
    SchPipe = Ut_CFE_SB_CreatePipe("FAC_SCH_PIPE");

    CFE_SB_InitMsg ((void*)&RController, FAC_RUN_CONTROLLER_MID, sizeof(RController), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&RController, (uint16)0);
    Ut_CFE_SB_AddMsgToPipe((void*)&RController, (CFE_SB_PipeId_t)SchPipe);

    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");

    CFE_SB_InitMsg ((void*)&VStatus, PX4_VEHICLE_STATUS_MID, sizeof(VStatus), TRUE);
    VStatus.Timestamp = FAC_Test_GetTimeUs();
    VStatus.IsVtol = FALSE;
    VStatus.IsRotaryWing = FALSE;
    VStatus.InTransitionMode = FALSE;                      // fix this
    VStatus.EngineFailure = FALSE;
    VStatus.EngineFailureCmd = FALSE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VStatus);
    Ut_CFE_SB_AddMsgToPipe((void*)&VStatus, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&BatStat, PX4_BATTERY_STATUS_MID, sizeof(BatStat), TRUE);
    BatStat.Timestamp = FAC_Test_GetTimeUs();
    BatStat.Voltage = 0.0f;                                 // fix this
    BatStat.VoltageFiltered = 0.0f;                         // fix this
    BatStat.Current = 0.0f;                                 // fix this
    BatStat.CurrentFiltered = 0.0f;                         // fix this
    BatStat.Discharged = 0.0f;                              // fix this
    BatStat.Remaining = 0.0f;                               // fix this
    BatStat.Scale = 0.1f;                                   // fix this
    BatStat.CellCount = 0;                                  // fix this
    BatStat.Connected = FALSE;                              // fix this
    BatStat.Warning = PX4_BATTERY_WARNING_NONE;             // fix this
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&BatStat);
    Ut_CFE_SB_AddMsgToPipe((void*)&BatStat, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VLDetect, PX4_VEHICLE_LAND_DETECTED_MID, sizeof(VLDetect), TRUE);
    VLDetect.Timestamp = FAC_Test_GetTimeUs();
    VLDetect.AltMax = 0.0f;                                 // fix this
    VLDetect.Landed = FALSE;
    VLDetect.Freefall = FALSE;
    VLDetect.GroundContact = FALSE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VLDetect);
    Ut_CFE_SB_AddMsgToPipe((void*)&VLDetect, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VGlobalPos, PX4_VEHICLE_GLOBAL_POSITION_MID,
                    sizeof(VGlobalPos), TRUE);
    VGlobalPos.Timestamp = FAC_Test_GetTimeUs();
    VGlobalPos.TimeUtcUsec = FAC_Test_GetTimeUs();
    VGlobalPos.Lat = 9.0;
    VGlobalPos.Lon = 4.0;
    VGlobalPos.Alt = 16.0f;
    VGlobalPos.DeltaLatLon[0] = 6.0;
    VGlobalPos.DeltaLatLon[1] = 0.0;
    VGlobalPos.DeltaAlt = 11.0f;
    VGlobalPos.LatLonResetCounter = 19;
    VGlobalPos.AltResetCounter = 10;
    VGlobalPos.VelN = 1.0f;
    VGlobalPos.VelE = 14.0f;
    VGlobalPos.VelD = 15.0f;
    VGlobalPos.Yaw = 3.0f;
    VGlobalPos.EpH = 20.0f;
    VGlobalPos.EpV = 18.0f;
    VGlobalPos.EvH = 17.0f;
    VGlobalPos.EvV = 22.0f;
    VGlobalPos.TerrainAlt = 21.0f;
    VGlobalPos.PressureAlt = 13.0f;
    VGlobalPos.TerrainAltValid = TRUE;
    VGlobalPos.DeadReckoning = TRUE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VGlobalPos);
    Ut_CFE_SB_AddMsgToPipe((void*)&VGlobalPos, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VAtt, PX4_VEHICLE_ATTITUDE_MID, sizeof(VAtt), TRUE);
    VAtt.Timestamp = FAC_Test_GetTimeUs();
    VAtt.RollSpeed = 0.0f;                                // fix this
    VAtt.PitchSpeed = 0.0f;                               // fix this
    VAtt.YawSpeed = 0.0f;                                 // fix this
    VAtt.Q[0] = 0.7083791494f;
    VAtt.Q[1] = -0.0311437733f;
    VAtt.Q[2] = -0.0508509092f;
    VAtt.Q[3] = 0.7033087611f;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VAtt);
    Ut_CFE_SB_AddMsgToPipe((void*)&VAtt, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VAttSp, PX4_VEHICLE_ATTITUDE_SETPOINT_MID, sizeof(VAttSp), TRUE);
    VAttSp.Timestamp = FAC_Test_GetTimeUs();
    VAttSp.RollBody = 0.0f;                               // fix this
    VAttSp.PitchBody = 0.0f;                              // fix this
    VAttSp.YawBody = 0.0f;                                // fix this
    VAttSp.YawSpMoveRate = 0.0000000000f;
    VAttSp.Q_D[0] = 0.7084835768f;
    VAttSp.Q_D[1] = -0.0144501235f;
    VAttSp.Q_D[2] = -0.0355133303f;
    VAttSp.Q_D[3] = 0.7046850324f;
    VAttSp.Q_D_Valid = TRUE;                              // fix this
    VAttSp.Thrust = 0.3333882987f;
    VAttSp.RollResetIntegral = TRUE;
    VAttSp.PitchResetIntegral = TRUE;
    VAttSp.YawResetIntegral = TRUE;
    VAttSp.FwControlYaw = FALSE;
    VAttSp.DisableMcYawControl = TRUE;
    VAttSp.ApplyFlaps = TRUE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VAttSp);
    Ut_CFE_SB_AddMsgToPipe((void*)&VAttSp, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&MCSp, PX4_MANUAL_CONTROL_SETPOINT_MID, sizeof(MCSp), TRUE);
    MCSp.Timestamp = FAC_Test_GetTimeUs();
    MCSp.X = 0.0f;                                       // fix this
    MCSp.Y = 0.0f;                                       // fix this
    MCSp.Z = 0.0f;                                       // fix this
    MCSp.R = 0.0f;                                       // fix this
    MCSp.Flaps = 0.0f;                                   // fix this
    MCSp.Aux1 = 0.0f;                                    // fix this
    MCSp.Aux2 = 0.0f;                                    // fix this
    MCSp.Aux3 = 0.0f;                                    // fix this
    MCSp.Aux4 = 0.0f;                                    // fix this
    MCSp.Aux5 = 0.0f;                                    // fix this
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&MCSp);
    Ut_CFE_SB_AddMsgToPipe((void*)&MCSp, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&ASpeed, PX4_AIRSPEED_MID, sizeof(ASpeed), TRUE);
    ASpeed.Timestamp = FAC_Test_GetTimeUs();
    ASpeed.IndicatedAirspeed = 1.0f;                     // fix this
    ASpeed.TrueAirspeed = 0.0f;                          // fix this
    ASpeed.TrueAirspeedUnfiltered = 0.0f;                // fix this
    ASpeed.AirTemperature = 0.0f;                        // fix this
    ASpeed.Confidence = 0.0f;                            // fix this
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&ASpeed);
    Ut_CFE_SB_AddMsgToPipe((void*)&ASpeed, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VCMode, PX4_VEHICLE_CONTROL_MODE_MID, sizeof(VCMode), TRUE);
    VCMode.Timestamp = FAC_Test_GetTimeUs();
    VCMode.ExternalManualOverrideOk = FALSE;
    VCMode.SystemHilEnabled = FALSE;
    VCMode.ControlManualEnabled = TRUE;
    VCMode.ControlAutoEnabled = FALSE;                   // fix this
    VCMode.ControlOffboardEnabled = FALSE;               // fix this
    VCMode.ControlRatesEnabled = FALSE;
    VCMode.ControlAttitudeEnabled = FALSE;
    VCMode.ControlRattitudeEnabled = FALSE;
    VCMode.ControlForceEnabled = FALSE;
    VCMode.ControlAccelerationEnabled = FALSE;
    VCMode.ControlVelocityEnabled = FALSE;
    VCMode.ControlPositionEnabled = FALSE;
    VCMode.ControlAltitudeEnabled = FALSE;
    VCMode.ControlClimbRateEnabled = FALSE;
    VCMode.ControlTerminationEnabled = FALSE;
    VCMode.ControlFixedHdgEnabled = FALSE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VCMode);
    Ut_CFE_SB_AddMsgToPipe((void*)&VCMode, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 9);

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
               (void*)&Test_FAC_GetPSPTimeHook);
    SendEvent_HookCalledCnt = 0;
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FAC_RunController_SendEventHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
               (void*)&Test_FAC_RunController_SendMsgHook);

    /* Execute the function being tested */
    oFAC.AppMain();

}


/**
 * Test FAC RunController(), Auto
 */
void Test_FAC_RunController_Auto(void)
{
    int32                             DataPipe;
    int32                             SchPipe;
    FAC_NoArgCmd_t                    RController;
    PX4_VehicleStatusMsg_t            VStatus;
    PX4_BatteryStatusMsg_t            BatStat;
    PX4_VehicleLandDetectedMsg_t      VLDetect;
    PX4_VehicleGlobalPositionMsg_t    VGlobalPos;
    PX4_VehicleAttitudeMsg_t          VAtt;
    PX4_VehicleAttitudeSetpointMsg_t  VAttSp;
    PX4_ManualControlSetpointMsg_t    MCSp;
    PX4_AirspeedMsg_t                 ASpeed;
    PX4_VehicleControlModeMsg_t       VCMode;

    /* Set inputs */
    SchPipe = Ut_CFE_SB_CreatePipe("FAC_SCH_PIPE");

    CFE_SB_InitMsg ((void*)&RController, FAC_RUN_CONTROLLER_MID, sizeof(RController), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&RController, (uint16)0);
    Ut_CFE_SB_AddMsgToPipe((void*)&RController, (CFE_SB_PipeId_t)SchPipe);

    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");

    CFE_SB_InitMsg ((void*)&VStatus, PX4_VEHICLE_STATUS_MID, sizeof(VStatus), TRUE);
    VStatus.Timestamp = FAC_Test_GetTimeUs();
    VStatus.IsVtol = FALSE;
    VStatus.IsRotaryWing = FALSE;
    VStatus.InTransitionMode = FALSE;                      // fix this
    VStatus.EngineFailure = FALSE;
    VStatus.EngineFailureCmd = FALSE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VStatus);
    Ut_CFE_SB_AddMsgToPipe((void*)&VStatus, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&BatStat, PX4_BATTERY_STATUS_MID, sizeof(BatStat), TRUE);
    BatStat.Timestamp = FAC_Test_GetTimeUs();
    BatStat.Voltage = 0.0f;                                 // fix this
    BatStat.VoltageFiltered = 0.0f;                         // fix this
    BatStat.Current = 0.0f;                                 // fix this
    BatStat.CurrentFiltered = 0.0f;                         // fix this
    BatStat.Discharged = 0.0f;                              // fix this
    BatStat.Remaining = 0.0f;                               // fix this
    BatStat.Scale = 0.1f;                                   // fix this
    BatStat.CellCount = 0;                                  // fix this
    BatStat.Connected = FALSE;                              // fix this
    BatStat.Warning = PX4_BATTERY_WARNING_NONE;             // fix this
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&BatStat);
    Ut_CFE_SB_AddMsgToPipe((void*)&BatStat, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VLDetect, PX4_VEHICLE_LAND_DETECTED_MID, sizeof(VLDetect), TRUE);
    VLDetect.Timestamp = FAC_Test_GetTimeUs();
    VLDetect.AltMax = 0.0f;                                 // fix this
    VLDetect.Landed = FALSE;
    VLDetect.Freefall = FALSE;
    VLDetect.GroundContact = FALSE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VLDetect);
    Ut_CFE_SB_AddMsgToPipe((void*)&VLDetect, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VGlobalPos, PX4_VEHICLE_GLOBAL_POSITION_MID,
                    sizeof(VGlobalPos), TRUE);
    VGlobalPos.Timestamp = FAC_Test_GetTimeUs();
    VGlobalPos.TimeUtcUsec = FAC_Test_GetTimeUs();
    VGlobalPos.Lat = 9.0;
    VGlobalPos.Lon = 4.0;
    VGlobalPos.Alt = 16.0f;
    VGlobalPos.DeltaLatLon[0] = 6.0;
    VGlobalPos.DeltaLatLon[1] = 0.0;
    VGlobalPos.DeltaAlt = 11.0f;
    VGlobalPos.LatLonResetCounter = 19;
    VGlobalPos.AltResetCounter = 10;
    VGlobalPos.VelN = 1.0f;
    VGlobalPos.VelE = 14.0f;
    VGlobalPos.VelD = 15.0f;
    VGlobalPos.Yaw = 3.0f;
    VGlobalPos.EpH = 20.0f;
    VGlobalPos.EpV = 18.0f;
    VGlobalPos.EvH = 17.0f;
    VGlobalPos.EvV = 22.0f;
    VGlobalPos.TerrainAlt = 21.0f;
    VGlobalPos.PressureAlt = 13.0f;
    VGlobalPos.TerrainAltValid = TRUE;
    VGlobalPos.DeadReckoning = TRUE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VGlobalPos);
    Ut_CFE_SB_AddMsgToPipe((void*)&VGlobalPos, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VAtt, PX4_VEHICLE_ATTITUDE_MID, sizeof(VAtt), TRUE);
    VAtt.Timestamp = FAC_Test_GetTimeUs();
    VAtt.RollSpeed = 0.0f;                                 // fix this
    VAtt.PitchSpeed = 0.0f;                                // fix this
    VAtt.YawSpeed = 0.0f;                                  // fix this
    VAtt.Q[0] = 0.7083791494f;
    VAtt.Q[1] = -0.0311437733f;
    VAtt.Q[2] = -0.0508509092f;
    VAtt.Q[3] = 0.7033087611f;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VAtt);
    Ut_CFE_SB_AddMsgToPipe((void*)&VAtt, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VAttSp, PX4_VEHICLE_ATTITUDE_SETPOINT_MID, sizeof(VAttSp), TRUE);
    VAttSp.Timestamp = FAC_Test_GetTimeUs();
    VAttSp.RollBody = 0.0f;                               // fix this
    VAttSp.PitchBody = 0.0f;                              // fix this
    VAttSp.YawBody = 0.0f;                                // fix this
    VAttSp.YawSpMoveRate = 0.0000000000f;
    VAttSp.Q_D[0] = 0.7084835768f;
    VAttSp.Q_D[1] = -0.0144501235f;
    VAttSp.Q_D[2] = -0.0355133303f;
    VAttSp.Q_D[3] = 0.7046850324f;
    VAttSp.Q_D_Valid = TRUE;                              // fix this
    VAttSp.Thrust = 0.3333882987f;
    VAttSp.RollResetIntegral = TRUE;
    VAttSp.PitchResetIntegral = TRUE;
    VAttSp.YawResetIntegral = TRUE;
    VAttSp.FwControlYaw = TRUE;
    VAttSp.DisableMcYawControl = TRUE;
    VAttSp.ApplyFlaps = TRUE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VAttSp);
    Ut_CFE_SB_AddMsgToPipe((void*)&VAttSp, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&MCSp, PX4_MANUAL_CONTROL_SETPOINT_MID, sizeof(MCSp), TRUE);
    MCSp.Timestamp = FAC_Test_GetTimeUs();
    MCSp.X = 0.0f;                                        // fix this
    MCSp.Y = 0.0f;                                        // fix this
    MCSp.Z = 0.0f;                                        // fix this
    MCSp.R = 0.0f;                                        // fix this
    MCSp.Flaps = 0.0f;                                    // fix this
    MCSp.Aux1 = 0.0f;                                     // fix this
    MCSp.Aux2 = 0.0f;                                     // fix this
    MCSp.Aux3 = 0.0f;                                     // fix this
    MCSp.Aux4 = 0.0f;                                     // fix this
    MCSp.Aux5 = 0.0f;                                     // fix this
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&MCSp);
    Ut_CFE_SB_AddMsgToPipe((void*)&MCSp, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&ASpeed, PX4_AIRSPEED_MID, sizeof(ASpeed), TRUE);
    ASpeed.Timestamp = FAC_Test_GetTimeUs();
    ASpeed.IndicatedAirspeed = 1.0f;                     // fix this
    ASpeed.TrueAirspeed = 0.0f;                          // fix this
    ASpeed.TrueAirspeedUnfiltered = 0.0f;                // fix this
    ASpeed.AirTemperature = 0.0f;                        // fix this
    ASpeed.Confidence = 0.0f;                            // fix this
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&ASpeed);
    Ut_CFE_SB_AddMsgToPipe((void*)&ASpeed, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VCMode, PX4_VEHICLE_CONTROL_MODE_MID, sizeof(VCMode), TRUE);
    VCMode.Timestamp = FAC_Test_GetTimeUs();
    VCMode.ExternalManualOverrideOk = FALSE;
    VCMode.SystemHilEnabled = FALSE;
    VCMode.ControlManualEnabled = FALSE;
    VCMode.ControlAutoEnabled = TRUE;                    // fix this
    VCMode.ControlOffboardEnabled = FALSE;               // fix this
    VCMode.ControlRatesEnabled = TRUE;
    VCMode.ControlAttitudeEnabled = TRUE;
    VCMode.ControlRattitudeEnabled = TRUE;
    VCMode.ControlForceEnabled = FALSE;
    VCMode.ControlAccelerationEnabled = FALSE;
    VCMode.ControlVelocityEnabled = TRUE;
    VCMode.ControlPositionEnabled = FALSE;
    VCMode.ControlAltitudeEnabled = TRUE;
    VCMode.ControlClimbRateEnabled = FALSE;
    VCMode.ControlTerminationEnabled = FALSE;
    VCMode.ControlFixedHdgEnabled = FALSE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VCMode);
    Ut_CFE_SB_AddMsgToPipe((void*)&VCMode, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 9);

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
               (void*)&Test_FAC_GetPSPTimeHook);
    SendEvent_HookCalledCnt = 0;
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FAC_RunController_SendEventHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
               (void*)&Test_FAC_RunController_SendMsgHook);

    /* Execute the function being tested */
    oFAC.AppMain();
}


/**
 * Test FAC RunController(), Landed
 */
void Test_FAC_RunController_Landed(void)
{
    int32                             DataPipe;
    int32                             SchPipe;
    FAC_NoArgCmd_t                    RController;
    PX4_VehicleStatusMsg_t            VStatus;
    PX4_BatteryStatusMsg_t            BatStat;
    PX4_VehicleLandDetectedMsg_t      VLDetect;
    PX4_VehicleGlobalPositionMsg_t    VGlobalPos;
    PX4_VehicleAttitudeMsg_t          VAtt;
    PX4_VehicleAttitudeSetpointMsg_t  VAttSp;
    PX4_ManualControlSetpointMsg_t    MCSp;
    PX4_AirspeedMsg_t                 ASpeed;
    PX4_VehicleControlModeMsg_t       VCMode;

    /* Set inputs */
    SchPipe = Ut_CFE_SB_CreatePipe("FAC_SCH_PIPE");

    CFE_SB_InitMsg ((void*)&RController, FAC_RUN_CONTROLLER_MID, sizeof(RController), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&RController, (uint16)0);
    Ut_CFE_SB_AddMsgToPipe((void*)&RController, (CFE_SB_PipeId_t)SchPipe);

    DataPipe = Ut_CFE_SB_CreatePipe("FAC_DATA_PIPE");

    CFE_SB_InitMsg ((void*)&VStatus, PX4_VEHICLE_STATUS_MID, sizeof(VStatus), TRUE);
    VStatus.Timestamp = FAC_Test_GetTimeUs();
    VStatus.IsVtol = FALSE;
    VStatus.IsRotaryWing = FALSE;
    VStatus.InTransitionMode = FALSE;                      // fix this
    VStatus.EngineFailure = FALSE;
    VStatus.EngineFailureCmd = FALSE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VStatus);
    Ut_CFE_SB_AddMsgToPipe((void*)&VStatus, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&BatStat, PX4_BATTERY_STATUS_MID, sizeof(BatStat), TRUE);
    BatStat.Timestamp = FAC_Test_GetTimeUs();
    BatStat.Voltage = 0.0f;                                 // fix this
    BatStat.VoltageFiltered = 0.0f;                         // fix this
    BatStat.Current = 0.0f;                                 // fix this
    BatStat.CurrentFiltered = 0.0f;                         // fix this
    BatStat.Discharged = 0.0f;                              // fix this
    BatStat.Remaining = 0.0f;                               // fix this
    BatStat.Scale = 0.1f;                                   // fix this
    BatStat.CellCount = 0;                                  // fix this
    BatStat.Connected = FALSE;                              // fix this
    BatStat.Warning = PX4_BATTERY_WARNING_NONE;             // fix this
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&BatStat);
    Ut_CFE_SB_AddMsgToPipe((void*)&BatStat, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VLDetect, PX4_VEHICLE_LAND_DETECTED_MID, sizeof(VLDetect), TRUE);
    VLDetect.Timestamp = FAC_Test_GetTimeUs();
    VLDetect.AltMax = 0.0f;                                 // fix this
    VLDetect.Landed = TRUE;
    VLDetect.Freefall = FALSE;
    VLDetect.GroundContact = FALSE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VLDetect);
    Ut_CFE_SB_AddMsgToPipe((void*)&VLDetect, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VGlobalPos, PX4_VEHICLE_GLOBAL_POSITION_MID,
                    sizeof(VGlobalPos), TRUE);
    VGlobalPos.Timestamp = FAC_Test_GetTimeUs();
    VGlobalPos.TimeUtcUsec = FAC_Test_GetTimeUs();
    VGlobalPos.Lat = 9.0;
    VGlobalPos.Lon = 4.0;
    VGlobalPos.Alt = 16.0f;
    VGlobalPos.DeltaLatLon[0] = 6.0;
    VGlobalPos.DeltaLatLon[1] = 0.0;
    VGlobalPos.DeltaAlt = 11.0f;
    VGlobalPos.LatLonResetCounter = 19;
    VGlobalPos.AltResetCounter = 10;
    VGlobalPos.VelN = 1.0f;
    VGlobalPos.VelE = 14.0f;
    VGlobalPos.VelD = 15.0f;
    VGlobalPos.Yaw = 3.0f;
    VGlobalPos.EpH = 20.0f;
    VGlobalPos.EpV = 18.0f;
    VGlobalPos.EvH = 17.0f;
    VGlobalPos.EvV = 22.0f;
    VGlobalPos.TerrainAlt = 21.0f;
    VGlobalPos.PressureAlt = 13.0f;
    VGlobalPos.TerrainAltValid = TRUE;
    VGlobalPos.DeadReckoning = TRUE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VGlobalPos);
    Ut_CFE_SB_AddMsgToPipe((void*)&VGlobalPos, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VAtt, PX4_VEHICLE_ATTITUDE_MID, sizeof(VAtt), TRUE);
    VAtt.Timestamp = FAC_Test_GetTimeUs();
    VAtt.RollSpeed = 0.0f;                                 // fix this
    VAtt.PitchSpeed = 0.0f;                                // fix this
    VAtt.YawSpeed = 0.0f;                                  // fix this
    VAtt.Q[0] = 0.7083791494f;
    VAtt.Q[1] = -0.0311437733f;
    VAtt.Q[2] = -0.0508509092f;
    VAtt.Q[3] = 0.7033087611f;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VAtt);
    Ut_CFE_SB_AddMsgToPipe((void*)&VAtt, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VAttSp, PX4_VEHICLE_ATTITUDE_SETPOINT_MID, sizeof(VAttSp), TRUE);
    VAttSp.Timestamp = FAC_Test_GetTimeUs();
    VAttSp.RollBody = 0.0f;                               // fix this
    VAttSp.PitchBody = 0.0f;                              // fix this
    VAttSp.YawBody = 0.0f;                                // fix this
    VAttSp.YawSpMoveRate = 0.0000000000f;
    VAttSp.Q_D[0] = 0.7084835768f;
    VAttSp.Q_D[1] = -0.0144501235f;
    VAttSp.Q_D[2] = -0.0355133303f;
    VAttSp.Q_D[3] = 0.7046850324f;
    VAttSp.Q_D_Valid = TRUE;                              // fix this
    VAttSp.Thrust = 0.3333882987f;
    VAttSp.RollResetIntegral = TRUE;
    VAttSp.PitchResetIntegral = TRUE;
    VAttSp.YawResetIntegral = TRUE;
    VAttSp.FwControlYaw = FALSE;
    VAttSp.DisableMcYawControl = TRUE;
    VAttSp.ApplyFlaps = TRUE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VAttSp);
    Ut_CFE_SB_AddMsgToPipe((void*)&VAttSp, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&MCSp, PX4_MANUAL_CONTROL_SETPOINT_MID, sizeof(MCSp), TRUE);
    MCSp.Timestamp = FAC_Test_GetTimeUs();
    MCSp.X = 0.0f;                                        // fix this
    MCSp.Y = 0.0f;                                        // fix this
    MCSp.Z = 0.0f;                                        // fix this
    MCSp.R = 0.0f;                                        // fix this
    MCSp.Flaps = 0.0f;                                    // fix this
    MCSp.Aux1 = 0.0f;                                     // fix this
    MCSp.Aux2 = 0.0f;                                     // fix this
    MCSp.Aux3 = 0.0f;                                     // fix this
    MCSp.Aux4 = 0.0f;                                     // fix this
    MCSp.Aux5 = 0.0f;                                     // fix this
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&MCSp);
    Ut_CFE_SB_AddMsgToPipe((void*)&MCSp, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&ASpeed, PX4_AIRSPEED_MID, sizeof(ASpeed), TRUE);
    ASpeed.Timestamp = FAC_Test_GetTimeUs();
    ASpeed.IndicatedAirspeed = 1.0f;                      // fix this
    ASpeed.TrueAirspeed = 0.0f;                           // fix this
    ASpeed.TrueAirspeedUnfiltered = 0.0f;                 // fix this
    ASpeed.AirTemperature = 0.0f;                         // fix this
    ASpeed.Confidence = 0.0f;                             // fix this
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&ASpeed);
    Ut_CFE_SB_AddMsgToPipe((void*)&ASpeed, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&VCMode, PX4_VEHICLE_CONTROL_MODE_MID, sizeof(VCMode), TRUE);
    VCMode.Timestamp = FAC_Test_GetTimeUs();
    VCMode.ExternalManualOverrideOk = FALSE;
    VCMode.SystemHilEnabled = FALSE;
    VCMode.ControlManualEnabled = TRUE;
    VCMode.ControlAutoEnabled = FALSE;                    // fix this
    VCMode.ControlOffboardEnabled = FALSE;                // fix this
    VCMode.ControlRatesEnabled = TRUE;
    VCMode.ControlAttitudeEnabled = TRUE;
    VCMode.ControlRattitudeEnabled = TRUE;
    VCMode.ControlForceEnabled = FALSE;
    VCMode.ControlAccelerationEnabled = FALSE;
    VCMode.ControlVelocityEnabled = TRUE;
    VCMode.ControlPositionEnabled = FALSE;
    VCMode.ControlAltitudeEnabled = TRUE;
    VCMode.ControlClimbRateEnabled = FALSE;
    VCMode.ControlTerminationEnabled = FALSE;
    VCMode.ControlFixedHdgEnabled = FALSE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VCMode);
    Ut_CFE_SB_AddMsgToPipe((void*)&VCMode, (CFE_SB_PipeId_t)DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FAC_RUN_CONTROLLER_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 9);

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
               (void*)&Test_FAC_GetPSPTimeHook);
    SendEvent_HookCalledCnt = 0;
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FAC_RunController_SendEventHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
               (void*)&Test_FAC_RunController_SendMsgHook);

    /* Execute the function being tested */
    oFAC.AppMain();
}


/**
 * Test FAC RunController(), TerminationEnabled
 */
void Test_FAC_RunController_TerminationEnabled(void)
{
    int32 iStatus = CFE_SUCCESS;

    iStatus = oFAC.InitConfigTbl();
    if (iStatus == CFE_SUCCESS)
    {
        oFAC.InitData();

        oFAC.CVT.VehicleStatus.IsVtol = false;
        oFAC.CVT.VControlMode.ControlTerminationEnabled = true;

        oFAC.RunController();
    }
}


/**
 * Test FAC RunController(), InvalidValue
 */
void Test_FAC_RunController_InvalidValue(void)
{
    int32 iStatus = CFE_SUCCESS;

    iStatus = oFAC.InitConfigTbl();
    if (iStatus == CFE_SUCCESS)
    {
        oFAC.InitData();

        oFAC.CVT.VehicleStatus.IsVtol = false;
        oFAC.CVT.VControlMode.ControlTerminationEnabled = false;
        oFAC.CVT.VehicleStatus.IsRotaryWing = false;
        oFAC.CVT.VControlMode.ControlRattitudeEnabled = false;
        oFAC.CVT.VControlMode.ControlRatesEnabled = true;
        oFAC.CVT.Airspeed.IndicatedAirspeed = NAN;

        oFAC.RunController();
    }
}


/**************************************************************************
 * Tests for FAC UpdateParams()
 **************************************************************************/
/**
 * Test FAC UpdateParams(), SendEventHook
 */
int32 Test_FAC_UpdateParams_SendEventHook(uint16 EventID, uint16 EventType, const char *EventText, ...)
{
    va_list  Ptr;
    char     Buf[256];
    FAC_ParamTbl_t *pTbl;

    va_start(Ptr, EventText);
    vsnprintf(Buf, (size_t)CFE_EVS_MAX_MESSAGE_LENGTH, EventText, Ptr);
    va_end(Ptr);

    printf("###UpdateParams_SendEventHook:\n");
    printf("%s\n", Buf);

    pTbl = oFAC.ParamTblPtr;

    UpdateParams_ParamChecksum = (
          (double)pTbl->FW_R_TC + (double)pTbl->FW_P_TC +
          (double)pTbl->FW_PR_P + (double)pTbl->FW_PR_I +
          (double)pTbl->FW_P_RMAX_POS + (double)pTbl->FW_P_RMAX_NEG +
          (double)pTbl->FW_PR_IMAX +
          (double)pTbl->FW_RR_P + (double)pTbl->FW_RR_I +
          (double)pTbl->FW_RR_IMAX + (double)pTbl->FW_R_RMAX +
          (double)pTbl->FW_YR_P + (double)pTbl->FW_YR_I +
          (double)pTbl->FW_YR_IMAX + (double)pTbl->FW_Y_RMAX +
          (double)pTbl->FW_RLL_TO_YAW_FF +
          (double)pTbl->FW_W_EN + (double)pTbl->FW_WR_P +
          (double)pTbl->FW_WR_I + (double)pTbl->FW_WR_IMAX + (double)pTbl->FW_W_RMAX +
          (double)pTbl->FW_RR_FF + (double)pTbl->FW_PR_FF +
          (double)pTbl->FW_YR_FF + (double)pTbl->FW_WR_FF +
          (double)pTbl->FW_YCO_VMIN + (double)pTbl->FW_YCO_METHOD +
          (double)pTbl->FW_RSP_OFF + (double)pTbl->FW_PSP_OFF +
          (double)pTbl->FW_MAN_R_MAX + (double)pTbl->FW_MAN_P_MAX +
          (double)pTbl->FW_FLAPS_SCL + (double)pTbl->FW_FLAPERON_SCL +
          (double)pTbl->FW_ARSP_MODE +
          (double)pTbl->FW_MAN_R_SC + (double)pTbl->FW_MAN_P_SC + (double)pTbl->FW_MAN_Y_SC +
          (double)pTbl->FW_BAT_SCALE_EN +
          (double)pTbl->FW_ACRO_X_MAX + (double)pTbl->FW_ACRO_Y_MAX + (double)pTbl->FW_ACRO_Z_MAX +
          (double)pTbl->FW_RATT_TH +
          (double)pTbl->FW_AIRSPD_MIN + (double)pTbl->FW_AIRSPD_MAX + (double)pTbl->FW_AIRSPD_TRIM +
          (double)pTbl->TRIM_ROLL + (double)pTbl->TRIM_PITCH + (double)pTbl->TRIM_YAW +
          (double)pTbl->VT_TYPE);
    std::cout.precision(17);
    std::cout << std::endl << "Params Checksum(with Max precision): "
              << UpdateParams_ParamChecksum << std::endl;

    std::cout.precision(7);
    std::cout << std::endl << "Param Table Values:" << std::endl;
    std::cout << "FW_R_TC: " << pTbl->FW_R_TC << ", FW_P_TC: " << pTbl->FW_P_TC << std::endl;
    std::cout << "FW_PR_P: " << pTbl->FW_PR_P << ", FW_PR_I: " << pTbl->FW_PR_I << std::endl;
    std::cout << "FW_P_RMAX_POS: " << pTbl->FW_P_RMAX_POS << ", FW_P_RMAX_NEG: "
              << pTbl->FW_P_RMAX_NEG << std::endl;
    std::cout << "FW_PR_IMAX: " << pTbl->FW_PR_IMAX << std::endl;
    std::cout << "FW_RR_P: " << pTbl->FW_RR_P << ", FW_RR_I: " << pTbl->FW_RR_I << std::endl;
    std::cout << "FW_RR_IMAX: " << pTbl->FW_RR_IMAX << ", FW_R_RMAX: " << pTbl->FW_R_RMAX
              << std::endl;
    std::cout << "FW_YR_P: " << pTbl->FW_YR_P << ", FW_YR_I: " << pTbl->FW_YR_I << std::endl;
    std::cout << "FW_YR_IMAX: " << pTbl->FW_YR_IMAX << ", FW_Y_RMAX: " << pTbl->FW_Y_RMAX
              << std::endl;
    std::cout << "FW_RLL_TO_YAW_FF: " << pTbl->FW_RLL_TO_YAW_FF << std::endl;
    std::cout << "FW_W_EN: " << pTbl->FW_W_EN << ", FW_WR_P: " << pTbl->FW_WR_P << std::endl;
    std::cout << "FW_WR_I: " << pTbl->FW_WR_I << ", FW_WR_IMAX: " << pTbl->FW_WR_IMAX
              << ", FW_W_RMAX: " << pTbl->FW_W_RMAX << std::endl;
    std::cout << "FW_RR_FF: " << pTbl->FW_RR_FF << ", FW_PR_FF: " << pTbl->FW_PR_FF
              << std::endl;
    std::cout << "FW_YR_FF: " << pTbl->FW_YR_FF << ", FW_WR_FF: " << pTbl->FW_WR_FF
              << std::endl;
    std::cout << "FW_YCO_VMIN: " << pTbl->FW_YCO_VMIN << ", FW_YCO_METHOD: "
              << pTbl->FW_YCO_METHOD << std::endl;
    std::cout << "FW_RSP_OFF: " << pTbl->FW_RSP_OFF << ", FW_PSP_OFF: " << pTbl->FW_PSP_OFF
              << std::endl;
    std::cout << "FW_MAN_R_MAX: " << pTbl->FW_MAN_R_MAX << ", FW_MAN_P_MAX: "
              << pTbl->FW_MAN_P_MAX << std::endl;
    std::cout << "FW_FLAPS_SCL: " << pTbl->FW_FLAPS_SCL << ", FW_FLAPERON_SCL: "
              << pTbl->FW_FLAPERON_SCL << std::endl;
    std::cout << "FW_ARSP_MODE: " << pTbl->FW_ARSP_MODE << std::endl;
    std::cout << "FW_MAN_R_SC: " << pTbl->FW_MAN_R_SC << ", FW_MAN_P_SC: "
              << pTbl->FW_MAN_P_SC << ", FW_MAN_Y_SC: " << pTbl->FW_MAN_Y_SC << std::endl;
    std::cout << "FW_BAT_SCALE_EN: " << pTbl->FW_BAT_SCALE_EN << std::endl;
    std::cout << "FW_ACRO_X_MAX: " << pTbl->FW_ACRO_X_MAX << ", FW_ACRO_Y_MAX: "
              << pTbl->FW_ACRO_Y_MAX << ", FW_ACRO_Z_MAX: " << pTbl->FW_ACRO_Z_MAX
              << std::endl;
    std::cout << "FW_RATT_TH: " << pTbl->FW_RATT_TH << std::endl;
    std::cout << "FW_AIRSPD_MIN: " << pTbl->FW_AIRSPD_MIN << ", FW_AIRSPD_MAX: "
              << pTbl->FW_AIRSPD_MAX << ", FW_AIRSPD_TRIM: " << pTbl->FW_AIRSPD_TRIM
              << std::endl;
    std::cout << "TRIM_ROLL: " << pTbl->TRIM_ROLL << ", TRIM_PITCH: " << pTbl->TRIM_PITCH
              << ", TRIM_YAW: " << pTbl->TRIM_YAW << std::endl;
    std::cout << "VT_TYPE: " << pTbl->VT_TYPE << std::endl;

    printf("\n###Updated ECL Values:\n");
    FAC_Test_PrintEclValues();

    UpdateParams_ValidateStatus = FAC_Test_ValidateEclValues();
}


/**
 * Test FAC UpdateParams, Standard
 */
void Test_FAC_UpdateParams_Standard(void)
{
    int32  iStatus = CFE_SUCCESS;
    double expected_checksum = 1563.91;

    UpdateParams_ParamChecksum = 0.0;
    UpdateParams_ValidateStatus = 0x0;

    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
                (void*)Test_FAC_UpdateParams_SendEventHook);

    printf("###Initial ECL Values:\n");
    FAC_Test_PrintEclValues();

    /* Execute the function being tested */
    iStatus = oFAC.InitApp();

    /* Verify results */
    if ((iStatus == CFE_SUCCESS) &&
        (UpdateParams_ValidateStatus == 0x0) &&
        (fabs(UpdateParams_ParamChecksum - expected_checksum) <= FLT_EPSILON)) // Fail with DBL_EPSILON
    {
        UtAssert_True(TRUE, "FAC UpdateParams_Standard");
    }
    else
    {
        UtAssert_True(FALSE, "FAC UpdateParams_Standard");
    }
}


/**
 * Test FAC UpdateParams, TailSitter()
 */
void Test_FAC_UpdateParams_TailSitter(void)
{
    int32  iStatus = CFE_SUCCESS;
    double expected_checksum = 1379.37;

    UpdateParams_ParamChecksum = 0.0;
    UpdateParams_ValidateStatus = 0x0;

    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
                (void*)Test_FAC_UpdateParams_SendEventHook);

    printf("###Initial ECL Values:\n");
    FAC_Test_PrintEclValues();

    /* Execute the function being tested */
    oFAC.InitApp();

    /* Verify results */
    if ((iStatus == CFE_SUCCESS) &&
        (UpdateParams_ValidateStatus == 0x0) &&
        (fabs(UpdateParams_ParamChecksum - expected_checksum) <= FLT_EPSILON)) // Fail with DBL_EPSILON
    {
        UtAssert_True(TRUE, "FAC UpdateParams_TailSitter");
    }
    else
    {
        UtAssert_True(FALSE, "FAC UpdateParams_TailSitter");
    }
}


/**************************************************************************
 * Tests for FAC ECL Controllers()
 **************************************************************************/
/**
 * Test FAC ECL_PitchController(), control_attitude_bad_inputs
 */
void Test_FAC_ECL_PitchController_control_attitude_bad_inputs(void)
{
    ECL_PitchController pitchController;
    ECL_ControlData ctlData;
    float rateSetpoint = 0.0f;

    /* Get nominal baseline #1 */
    pitchController.init();
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.pitch_setpoint = 1.0f;
    ctlData.roll = 2.0f;
    ctlData.pitch = 3.0f;
    ctlData.airspeed = 4.0f;
    pitchController.set_max_rate(10.0);
    pitchController.set_max_rate_neg(10.0);

    rateSetpoint = pitchController.control_attitude(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, -10.0, FLT_EPSILON,
                "FAC ECL_PitchController(), control_attitude_bad_inputs(nominal baseline #1)");

    /* Get nominal baseline #2 */
    pitchController.init();
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.pitch_setpoint = 4.0f;
    ctlData.roll = 2.0f;
    ctlData.pitch = 3.0f;
    ctlData.airspeed = 4.0f;
    pitchController.set_max_rate(10.0);
    pitchController.set_max_rate_neg(10.0);

    rateSetpoint = pitchController.control_attitude(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 10.0, FLT_EPSILON,
                "FAC ECL_PitchController(), control_attitude_bad_inputs(nominal baseline #2)");

    /* Invalid input #1 */
    pitchController.init();
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.pitch_setpoint = NAN;
    ctlData.roll = 2.0f;
    ctlData.pitch = 3.0f;
    ctlData.airspeed = 4.0f;
    pitchController.set_max_rate(10.0);
    pitchController.set_max_rate_neg(10.0);

    rateSetpoint = pitchController.control_attitude(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
                "FAC ECL_PitchController(), control_attitude_bad_inputs(Invalid input #1)");

    /* Invalid input #2 */
    pitchController.init();
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.pitch_setpoint = 1.0f;
    ctlData.roll = NAN;
    ctlData.pitch = 3.0f;
    ctlData.airspeed = 4.0f;
    pitchController.set_max_rate(10.0);
    pitchController.set_max_rate_neg(10.0);

    rateSetpoint = pitchController.control_attitude(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
                "FAC ECL_PitchController(), control_attitude_bad_inputs(Invalid input #2)");

    /* Invalid input #3 */
    pitchController.init();
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.pitch_setpoint = 1.0f;
    ctlData.roll = 2.0f;
    ctlData.pitch = NAN;
    ctlData.airspeed = 4.0f;
    pitchController.set_max_rate(10.0);
    pitchController.set_max_rate_neg(10.0);

    rateSetpoint = pitchController.control_attitude(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
                "FAC ECL_PitchController(), control_attitude_bad_inputs(Invalid input #3)");

    /* Invalid input #4 */
    pitchController.init();
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.pitch_setpoint = 1.0f;
    ctlData.roll = 2.0f;
    ctlData.pitch = 3.0f;
    ctlData.airspeed = NAN;
    pitchController.set_max_rate(10.0);
    pitchController.set_max_rate_neg(10.0);

    rateSetpoint = pitchController.control_attitude(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
                "FAC ECL_PitchController(), control_attitude_bad_inputs(Invalid input #4)");
}


/**
 * Test FAC ECL_PitchController(), control_bodyrate_bad_inputs
 */
void Test_FAC_ECL_PitchController_control_bodyrate_bad_inputs(void)
{
    ECL_PitchController pitchController;
    ECL_ControlData ctlData;
    float rateSetpoint = 0.0f;

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
               (void*)&Test_FAC_GetPSPTimeHook);

    /* Get nominal baseline */
    pitchController.init();
    pitchController.set_k_i(1.0f);
    pitchController.set_k_ff(0.5f);
    pitchController.set_bodyrate_setpoint(0.5f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.roll = 2.0f;
    ctlData.pitch = 3.0f;
    ctlData.body_y_rate = 6.0f;
    ctlData.body_z_rate = 7.0f;
    ctlData.yaw_rate_setpoint = 8.0f;
    ctlData.airspeed = 4.0f;
    ctlData.airspeed_min = 1.0f;
    ctlData.airspeed_max = 100.0f;
    ctlData.scaler = 1.0f;
    ctlData.lock_integrator = false;
    pitchController.set_max_rate(10.0);
    pitchController.set_max_rate_neg(10.0);

    rateSetpoint = pitchController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.25, FLT_EPSILON,
             "FAC ECL_PitchController(), control_bodyrate_bad_inputs(nominal baseline)");

    /* Invalid input #1 */
    pitchController.init();
    pitchController.set_k_i(1.0f);
    pitchController.set_k_ff(0.5f);
    pitchController.set_bodyrate_setpoint(0.5f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.roll = NAN;
    ctlData.pitch = 3.0f;
    ctlData.body_y_rate = 6.0f;
    ctlData.body_z_rate = 7.0f;
    ctlData.yaw_rate_setpoint = 8.0f;
    ctlData.airspeed = 4.0f;
    ctlData.airspeed_min = 1.0f;
    ctlData.airspeed_max = 100.0f;
    ctlData.scaler = 1.0f;
    ctlData.lock_integrator = false;
    pitchController.set_max_rate(10.0);
    pitchController.set_max_rate_neg(10.0);

    rateSetpoint = pitchController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
             "FAC ECL_PitchController(), control_bodyrate_bad_inputs(Invalid input #1)");

    /* Invalid input #2 */
    pitchController.init();
    pitchController.set_k_i(1.0f);
    pitchController.set_k_ff(0.5f);
    pitchController.set_bodyrate_setpoint(0.5f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.roll = 2.0f;
    ctlData.pitch = NAN;
    ctlData.body_y_rate = 6.0f;
    ctlData.body_z_rate = 7.0f;
    ctlData.yaw_rate_setpoint = 8.0f;
    ctlData.airspeed = 4.0f;
    ctlData.airspeed_min = 1.0f;
    ctlData.airspeed_max = 100.0f;
    ctlData.scaler = 1.0f;
    ctlData.lock_integrator = false;
    pitchController.set_max_rate(10.0);
    pitchController.set_max_rate_neg(10.0);

    rateSetpoint = pitchController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
             "FAC ECL_PitchController(), control_bodyrate_bad_inputs(Invalid input #2)");

    /* Invalid input #3 */
    pitchController.init();
    pitchController.set_k_i(1.0f);
    pitchController.set_k_ff(0.5f);
    pitchController.set_bodyrate_setpoint(0.5f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.roll = 2.0f;
    ctlData.pitch = 3.0f;
    ctlData.body_y_rate = NAN;
    ctlData.body_z_rate = 7.0f;
    ctlData.yaw_rate_setpoint = 8.0f;
    ctlData.airspeed = 4.0f;
    ctlData.airspeed_min = 1.0f;
    ctlData.airspeed_max = 100.0f;
    ctlData.scaler = 1.0f;
    ctlData.lock_integrator = false;
    pitchController.set_max_rate(10.0);
    pitchController.set_max_rate_neg(10.0);

    rateSetpoint = pitchController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
             "FAC ECL_PitchController(), control_bodyrate_bad_inputs(Invalid input #3)");

    /* Invalid input #4 */
    pitchController.init();
    pitchController.set_k_i(1.0f);
    pitchController.set_k_ff(0.5f);
    pitchController.set_bodyrate_setpoint(0.5f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.roll = 2.0f;
    ctlData.pitch = 3.0f;
    ctlData.body_y_rate = 6.0f;
    ctlData.body_z_rate = NAN;
    ctlData.yaw_rate_setpoint = 8.0f;
    ctlData.airspeed = 4.0f;
    ctlData.airspeed_min = 1.0f;
    ctlData.airspeed_max = 100.0f;
    ctlData.scaler = 1.0f;
    ctlData.lock_integrator = false;
    pitchController.set_max_rate(10.0);
    pitchController.set_max_rate_neg(10.0);

    rateSetpoint = pitchController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
             "FAC ECL_PitchController(), control_bodyrate_bad_inputs(Invalid input #4)");

    /* Invalid input #5 */
    pitchController.init();
    pitchController.set_k_i(1.0f);
    pitchController.set_k_ff(0.5f);
    pitchController.set_bodyrate_setpoint(0.5f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.roll = 2.0f;
    ctlData.pitch = 3.0f;
    ctlData.body_y_rate = 6.0f;
    ctlData.body_z_rate = 7.0f;
    ctlData.yaw_rate_setpoint = NAN;
    ctlData.airspeed = 4.0f;
    ctlData.airspeed_min = 1.0f;
    ctlData.airspeed_max = 100.0f;
    ctlData.scaler = 1.0f;
    ctlData.lock_integrator = false;
    pitchController.set_max_rate(10.0);
    pitchController.set_max_rate_neg(10.0);

    rateSetpoint = pitchController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
             "FAC ECL_PitchController(), control_bodyrate_bad_inputs(Invalid input #5)");

    /* Invalid input #6 */
    pitchController.init();
    pitchController.set_k_i(1.0f);
    pitchController.set_k_ff(0.5f);
    pitchController.set_bodyrate_setpoint(0.5f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.roll = 2.0f;
    ctlData.pitch = 3.0f;
    ctlData.body_y_rate = 6.0f;
    ctlData.body_z_rate = 7.0f;
    ctlData.yaw_rate_setpoint = 8.0f;
    ctlData.airspeed = 4.0f;
    ctlData.airspeed_min = NAN;
    ctlData.airspeed_max = 100.0f;
    ctlData.scaler = 1.0f;
    ctlData.lock_integrator = false;
    pitchController.set_max_rate(10.0);
    pitchController.set_max_rate_neg(10.0);

    rateSetpoint = pitchController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
             "FAC ECL_PitchController(), control_bodyrate_bad_inputs(Invalid input #6)");

    /* Invalid input #7 */
    pitchController.init();
    pitchController.set_k_i(1.0f);
    pitchController.set_k_ff(0.5f);
    pitchController.set_bodyrate_setpoint(0.5f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.roll = 2.0f;
    ctlData.pitch = 3.0f;
    ctlData.body_y_rate = 6.0f;
    ctlData.body_z_rate = 7.0f;
    ctlData.yaw_rate_setpoint = 8.0f;
    ctlData.airspeed = 4.0f;
    ctlData.airspeed_min = 1.0f;
    ctlData.airspeed_max = NAN;
    ctlData.scaler = 1.0f;
    ctlData.lock_integrator = false;
    pitchController.set_max_rate(10.0);
    pitchController.set_max_rate_neg(10.0);

    rateSetpoint = pitchController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
             "FAC ECL_PitchController(), control_bodyrate_bad_inputs(Invalid input #7)");

    /* Invalid input #8 */
    pitchController.init();
    pitchController.set_k_i(1.0f);
    pitchController.set_k_ff(0.5f);
    pitchController.set_bodyrate_setpoint(0.5f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.roll = 2.0f;
    ctlData.pitch = 3.0f;
    ctlData.body_y_rate = 6.0f;
    ctlData.body_z_rate = 7.0f;
    ctlData.yaw_rate_setpoint = 8.0f;
    ctlData.airspeed = 4.0f;
    ctlData.airspeed_min = 1.0f;
    ctlData.airspeed_max = 100.0f;
    ctlData.scaler = NAN;
    ctlData.lock_integrator = false;
    pitchController.set_max_rate(10.0);
    pitchController.set_max_rate_neg(10.0);

    rateSetpoint = pitchController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
             "FAC ECL_PitchController(), control_bodyrate_bad_inputs(Invalid input #8)");
}


/**
 * Test FAC ECL_PitchController(), control_bodyrate_constrain
 */
void Test_FAC_ECL_PitchController_control_bodyrate_constrain(void)
{
    ECL_PitchController pitchController;
    ECL_ControlData ctlData;
    float rateSetpoint = 0.0f;

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
               (void*)&Test_FAC_GetPSPTimeHook);

    /* Get nominal baseline #1 */
    pitchController.init();
    pitchController.set_k_i(1.0f);
    pitchController.set_k_ff(1.0f);
    pitchController.set_bodyrate_setpoint(1.0f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.roll = 2.0f;
    ctlData.pitch = 3.0f;
    ctlData.body_y_rate = 6.0f;
    ctlData.body_z_rate = 7.0f;
    ctlData.yaw_rate_setpoint = 8.0f;
    ctlData.airspeed = 4.0f;
    ctlData.airspeed_min = 1.0f;
    ctlData.airspeed_max = 100.0f;
    ctlData.scaler = 2.0f;
    ctlData.lock_integrator = false;
    pitchController.set_max_rate(10.0);
    pitchController.set_max_rate_neg(10.0);

    rateSetpoint = pitchController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 1.0, FLT_EPSILON,
             "FAC ECL_PitchController(), control_bodyrate_constrain(baseline #1, First run)");

    rateSetpoint = pitchController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 1.0, FLT_EPSILON,
             "FAC ECL_PitchController(), control_bodyrate_constrain(baseline #1, Second run)");

    /* Get nominal baseline #2 */
    pitchController.init();
    pitchController.set_k_i(1.0f);
    pitchController.set_k_ff(1.0f);
    pitchController.set_bodyrate_setpoint(1.0f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.roll = 2.0f;
    ctlData.pitch = 3.0f;
    ctlData.body_y_rate = 6.0f;
    ctlData.body_z_rate = 7.0f;
    ctlData.yaw_rate_setpoint = 8.0f;
    ctlData.airspeed = 4.0f;
    ctlData.airspeed_min = 1.0f;
    ctlData.airspeed_max = 100.0f;
    ctlData.scaler = -2.0f;
    ctlData.lock_integrator = false;
    pitchController.set_max_rate(10.0);
    pitchController.set_max_rate_neg(10.0);

    rateSetpoint = pitchController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, -1.0, FLT_EPSILON,
             "FAC ECL_PitchController(), control_bodyrate_constrain(baseline #2, First run)");

    rateSetpoint = pitchController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, -1.0, FLT_EPSILON,
             "FAC ECL_PitchController(), control_bodyrate_constrain(baseline #2, Second run)");
}


/**
 * Test FAC ECL_PitchController(), control_euler_rate
 */
void Test_FAC_ECL_PitchController_control_euler_rate(void)
{
    ECL_PitchController pitchController;
    ECL_ControlData ctlData;
    float result = 0.0f;

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
               (void*)&Test_FAC_GetPSPTimeHook);

    /* Get nominal baseline */
    pitchController.init();
    pitchController.set_k_i(1.0f);
    pitchController.set_k_ff(0.5f);
    pitchController.set_bodyrate_setpoint(0.5f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.roll = 2.0f;
    ctlData.pitch = 3.0f;
    ctlData.body_y_rate = 6.0f;
    ctlData.body_z_rate = 7.0f;
    ctlData.yaw_rate_setpoint = 8.0f;
    ctlData.airspeed = 4.0f;
    ctlData.airspeed_min = 1.0f;
    ctlData.airspeed_max = 100.0f;
    ctlData.scaler = 1.0f;
    ctlData.lock_integrator = false;
    pitchController.set_max_rate(10.0);
    pitchController.set_max_rate_neg(10.0);

    result = pitchController.control_euler_rate(ctlData);
}


/**
 * Test FAC ECL_RollController(), control_attitude_bad_inputs
 */
void Test_FAC_ECL_RollController_control_attitude_bad_inputs(void)
{
    ECL_RollController rollController;
    ECL_ControlData ctlData;
    float rateSetpoint = 0.0f;

    /* Get nominal baseline #1*/
    rollController.init();
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.roll_setpoint = 2.0f;
    ctlData.roll = 3.0f;

    rateSetpoint = rollController.control_attitude(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, -10.0, FLT_EPSILON,
             "FAC ECL_RollController(), control_attitude_bad_inputs(nominal baseline #1)");

    /* Get nominal baseline #2*/
    rollController.init();
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.roll_setpoint = 2.0f;
    ctlData.roll = 3.0f;
    rollController.set_max_rate(15);

    rateSetpoint = rollController.control_attitude(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, -10.0, FLT_EPSILON,
             "FAC ECL_RollController(), control_attitude_bad_inputs(nominal baseline #2)");

    /* Invalid Input #1 */
    rollController.init();
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.roll_setpoint = NAN;
    ctlData.roll = 3.0f;

    rateSetpoint = rollController.control_attitude(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
             "FAC ECL_RollController(), control_attitude_bad_inputs(Invalid Input #1)");

    /* Invalid Input #2 */
    rollController.init();
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.roll_setpoint = 2.0f;
    ctlData.roll = NAN;

    rateSetpoint = rollController.control_attitude(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
             "FAC ECL_RollController(), control_attitude_bad_inputs(Invalid Input #2)");
}


/**
 * Test FAC ECL_RollController(), control_bodyrate_bad_inputs
 */
void Test_FAC_ECL_RollController_control_bodyrate_bad_inputs(void)
{
    ECL_RollController rollController;
    ECL_ControlData ctlData;
    float rateSetpoint = 0.0f;

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
               (void*)&Test_FAC_GetPSPTimeHook);

    /* Get nominal baseline */
    rollController.init();
    rollController.set_k_i(1.0f);
    rollController.set_k_ff(1.0f);
    rollController.set_bodyrate_setpoint(1.0f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.pitch = 1.0f;
    ctlData.body_x_rate = 2.0f;
    ctlData.body_z_rate = 3.0f;
    ctlData.yaw_rate_setpoint = 4.0f;
    ctlData.airspeed_min = 5.0f;
    ctlData.airspeed_max = 6.0f;
    ctlData.scaler = 7.0f;
    ctlData.lock_integrator = false;

    rateSetpoint = rollController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 1.0, FLT_EPSILON,
             "FAC ECL_RollController(), control_bodyrate_bad_inputs(nominal baseline)");

    /* Invalid input #1 */
    rollController.init();
    rollController.set_k_i(1.0f);
    rollController.set_k_ff(1.0f);
    rollController.set_bodyrate_setpoint(1.0f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.pitch = NAN;
    ctlData.body_x_rate = 2.0f;
    ctlData.body_z_rate = 3.0f;
    ctlData.yaw_rate_setpoint = 4.0f;
    ctlData.airspeed_min = 5.0f;
    ctlData.airspeed_max = 6.0f;
    ctlData.scaler = 7.0f;
    ctlData.lock_integrator = false;

    rateSetpoint = rollController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
             "FAC ECL_RollController(), control_bodyrate_bad_inputs(Invalid input #1)");

    /* Invalid input #2 */
    rollController.init();
    rollController.set_k_i(1.0f);
    rollController.set_k_ff(1.0f);
    rollController.set_bodyrate_setpoint(1.0f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.pitch = 1.0f;
    ctlData.body_x_rate = NAN;
    ctlData.body_z_rate = 3.0f;
    ctlData.yaw_rate_setpoint = 4.0f;
    ctlData.airspeed_min = 5.0f;
    ctlData.airspeed_max = 6.0f;
    ctlData.scaler = 7.0f;
    ctlData.lock_integrator = false;

    rateSetpoint = rollController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
             "FAC ECL_RollController(), control_bodyrate_bad_inputs(Invalid input #2)");

    /* Invalid input #3 */
    rollController.init();
    rollController.set_k_i(1.0f);
    rollController.set_k_ff(1.0f);
    rollController.set_bodyrate_setpoint(1.0f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.pitch = 1.0f;
    ctlData.body_x_rate = 2.0f;
    ctlData.body_z_rate = NAN;
    ctlData.yaw_rate_setpoint = 4.0f;
    ctlData.airspeed_min = 5.0f;
    ctlData.airspeed_max = 6.0f;
    ctlData.scaler = 7.0f;
    ctlData.lock_integrator = false;

    rateSetpoint = rollController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
             "FAC ECL_RollController(), control_bodyrate_bad_inputs(Invalid input #3)");

    /* Invalid input #4 */
    rollController.init();
    rollController.set_k_i(1.0f);
    rollController.set_k_ff(1.0f);
    rollController.set_bodyrate_setpoint(1.0f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.pitch = 1.0f;
    ctlData.body_x_rate = 2.0f;
    ctlData.body_z_rate = 3.0f;
    ctlData.yaw_rate_setpoint = NAN;
    ctlData.airspeed_min = 5.0f;
    ctlData.airspeed_max = 6.0f;
    ctlData.scaler = 7.0f;
    ctlData.lock_integrator = false;

    rateSetpoint = rollController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
             "FAC ECL_RollController(), control_bodyrate_bad_inputs(Invalid input #4)");

    /* Invalid input #5 */
    rollController.init();
    rollController.set_k_i(1.0f);
    rollController.set_k_ff(1.0f);
    rollController.set_bodyrate_setpoint(1.0f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.pitch = 1.0f;
    ctlData.body_x_rate = 2.0f;
    ctlData.body_z_rate = 3.0f;
    ctlData.yaw_rate_setpoint = 4.0f;
    ctlData.airspeed_min = NAN;
    ctlData.airspeed_max = 6.0f;
    ctlData.scaler = 7.0f;
    ctlData.lock_integrator = false;

    rateSetpoint = rollController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
             "FAC ECL_RollController(), control_bodyrate_bad_inputs(Invalid input #5)");

    /* Invalid input #6 */
    rollController.init();
    rollController.set_k_i(1.0f);
    rollController.set_k_ff(1.0f);
    rollController.set_bodyrate_setpoint(1.0f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.pitch = 1.0f;
    ctlData.body_x_rate = 2.0f;
    ctlData.body_z_rate = 3.0f;
    ctlData.yaw_rate_setpoint = 4.0f;
    ctlData.airspeed_min = 5.0f;
    ctlData.airspeed_max = NAN;
    ctlData.scaler = 7.0f;
    rateSetpoint = rollController.control_bodyrate(ctlData);

    ctlData.lock_integrator = false;
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
             "FAC ECL_RollController(), control_bodyrate_bad_inputs(Invalid input #6)");

    /* Invalid input #7 */
    rollController.init();
    rollController.set_k_i(1.0f);
    rollController.set_k_ff(1.0f);
    rollController.set_bodyrate_setpoint(1.0f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.pitch = 1.0f;
    ctlData.body_x_rate = 2.0f;
    ctlData.body_z_rate = 3.0f;
    ctlData.yaw_rate_setpoint = 4.0f;
    ctlData.airspeed_min = 5.0f;
    ctlData.airspeed_max = 6.0f;
    ctlData.scaler = NAN;
    ctlData.lock_integrator = false;

    rateSetpoint = rollController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
             "FAC ECL_RollController(), control_bodyrate_bad_inputs(Invalid input #7)");
}


/**
 * Test FAC ECL_RollController(), control_bodyrate_constrain
 */
void Test_FAC_ECL_RollController_control_bodyrate_constrain(void)
{
    ECL_RollController rollController;
    ECL_ControlData ctlData;
    float rateSetpoint = 0.0f;

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
               (void*)&Test_FAC_GetPSPTimeHook);

    /* Get nominal baseline #1 */
    rollController.init();
    rollController.set_k_i(1.0f);
    rollController.set_k_ff(1.0f);
    rollController.set_bodyrate_setpoint(1.0f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.pitch = 1.0f;
    ctlData.body_x_rate = 2.0f;
    ctlData.body_z_rate = 3.0f;
    ctlData.yaw_rate_setpoint = 4.0f;
    ctlData.airspeed_min = 5.0f;
    ctlData.airspeed_max = 6.0f;
    ctlData.scaler = 7.0f;
    ctlData.lock_integrator = false;

    rateSetpoint = rollController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 1.0, FLT_EPSILON,
             "FAC ECL_RollController(), control_bodyrate_constrain(nominal baseline #1, First run)");

    rateSetpoint = rollController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 1.0, FLT_EPSILON,
             "FAC ECL_RollController(), control_bodyrate_constrain(nominal baseline #1, Second run)");

    /* Get nominal baseline #2 */
    rollController.init();
    rollController.set_k_i(1.0f);
    rollController.set_k_ff(1.0f);
    rollController.set_bodyrate_setpoint(1.0f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.pitch = 1.0f;
    ctlData.body_x_rate = 2.0f;
    ctlData.body_z_rate = 3.0f;
    ctlData.yaw_rate_setpoint = 4.0f;
    ctlData.airspeed_min = 5.0f;
    ctlData.airspeed_max = 6.0f;
    ctlData.scaler = -7.0f;
    ctlData.lock_integrator = false;

    rateSetpoint = rollController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, -1.0, FLT_EPSILON,
          "FAC ECL_RollController(), control_bodyrate_constrain(nominal baseline #2, First run)");

    rateSetpoint = rollController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, -1.0, FLT_EPSILON,
          "FAC ECL_RollController(), control_bodyrate_constrain(nominal baseline #2, Second run)");
}


/**
 * Test FAC ECL_RollController(), control_euler_rate
 */
void Test_FAC_ECL_RollController_control_euler_rate(void)
{
    ECL_RollController rollController;
    ECL_ControlData ctlData;
    float result = 0.0f;

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
               (void*)&Test_FAC_GetPSPTimeHook);

    /* Get nominal baseline */
    rollController.init();
    rollController.set_k_i(1.0f);
    rollController.set_k_ff(1.0f);
    rollController.set_bodyrate_setpoint(1.0f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.pitch = 1.0f;
    ctlData.body_x_rate = 2.0f;
    ctlData.body_z_rate = 3.0f;
    ctlData.yaw_rate_setpoint = 4.0f;
    ctlData.roll_rate_setpoint = 2.0f;
    ctlData.airspeed_min = 5.0f;
    ctlData.airspeed_max = 6.0f;
    ctlData.scaler = 7.0f;
    ctlData.lock_integrator = false;

    result = rollController.control_euler_rate(ctlData);
}


/**
 * Test FAC ECL_WheelController(), get_desired_rate
 */
void Test_FAC_ECL_WheelController_get_desired_rate(void)
{
    ECL_WheelController wheelController;
    ECL_ControlData ctlData;
    float rateSetpoint = 0.0f;

    wheelController.init();
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.yaw_setpoint = 1.0f;
    ctlData.yaw = 2.0f;

    rateSetpoint = wheelController.control_attitude(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, -10.0, FLT_EPSILON,
             "FAC ECL_WheelController(), get_desired_rate(control_attitude)");

    rateSetpoint = wheelController.get_desired_rate();
    UtAssert_DoubleCmpAbs(rateSetpoint, -10.0, FLT_EPSILON,
             "FAC ECL_WheelController(), get_desired_rate(get_desired_rate)");
}


/**
 * Test FAC ECL_WheelController(), control_attitude_bad_inputs
 */
void Test_FAC_ECL_WheelController_control_attitude_bad_inputs(void)
{
    ECL_WheelController wheelController;
    ECL_ControlData ctlData;
    float rateSetpoint = 0.0f;

    /* Get nominal baseline */
    wheelController.init();
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.yaw_setpoint = 2.0f;
    ctlData.yaw = 3.0f;

    rateSetpoint = wheelController.control_attitude(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, -10.0, FLT_EPSILON,
             "FAC ECL_WheelController(), control_attitude_bad_inputs(nominal baseline)");

    /* Invalid input #1 */
    wheelController.init();
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.yaw_setpoint = NAN;
    ctlData.yaw = 3.0f;

    rateSetpoint = wheelController.control_attitude(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
             "FAC ECL_WheelController(), control_attitude_bad_inputs(Invalid input #1)");

    /* Invalid input #2 */
    wheelController.init();
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.yaw_setpoint = 2.0f;
    ctlData.yaw = NAN;

    rateSetpoint = wheelController.control_attitude(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
             "FAC ECL_WheelController(), control_attitude_bad_inputs(Invalid input #2)");
}


/**
 * Test FAC ECL_WheelController(), control_attitude_constrain
 */
void Test_FAC_ECL_WheelController_control_attitude_constrain(void)
{
    ECL_WheelController wheelController;
    ECL_ControlData ctlData;
    float rateSetpoint = 0.0f;

    /* Get nominal baseline #1 */
    wheelController.init();
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.yaw_setpoint = 1.0f;
    ctlData.yaw = 2.0f;
    wheelController.set_max_rate(10.0);

    rateSetpoint = wheelController.control_attitude(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, -10.0, FLT_EPSILON,
             "FAC ECL_WheelController(), control_attitude_constrain(nominal baseline #1)");

    /* Get nominal baseline #2 */
    wheelController.init();
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.yaw_setpoint = 2.0f;
    ctlData.yaw = 1.0f;
    wheelController.set_max_rate(10.0);

    rateSetpoint = wheelController.control_attitude(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 10.0, FLT_EPSILON,
             "FAC ECL_WheelController(), control_attitude_constrain(nominal baseline #2)");
}


/**
 * Test FAC ECL_WheelController(), control_bodyrate_bad_inputs
 */
void Test_FAC_ECL_WheelController_control_bodyrate_bad_inputs(void)
{
    ECL_WheelController wheelController;
    ECL_ControlData ctlData;
    float rateSetpoint = 0.0f;

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
               (void*)&Test_FAC_GetPSPTimeHook);

    /* Get nominal baseline */
    wheelController.init();
    wheelController.set_k_i(1.0f);
    wheelController.set_k_p(1.0f);
    wheelController.set_k_ff(1.0f);
    wheelController.set_bodyrate_setpoint(1.0f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.body_z_rate = 1.0f;
    ctlData.groundspeed = 2.0f;
    ctlData.groundspeed_scaler = 3.0f;
    ctlData.lock_integrator = 3.0f;

    rateSetpoint = wheelController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, -1.0, FLT_EPSILON,
             "FAC ECL_WheelController(), control_bodyrate_bad_inputs(nominal baseline)");

    /* Invalid input #1 */
    wheelController.init();
    wheelController.set_k_i(1.0f);
    wheelController.set_k_p(1.0f);
    wheelController.set_k_ff(1.0f);
    wheelController.set_bodyrate_setpoint(1.0f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.body_z_rate = NAN;
    ctlData.groundspeed = 2.0f;
    ctlData.groundspeed_scaler = 3.0f;
    ctlData.lock_integrator = 3.0f;

    rateSetpoint = wheelController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
             "FAC ECL_WheelController(), control_bodyrate_bad_inputs(Invalid input #1)");

    /* Invalid input #2 */
    wheelController.init();
    wheelController.set_k_i(1.0f);
    wheelController.set_k_p(1.0f);
    wheelController.set_k_ff(1.0f);
    wheelController.set_bodyrate_setpoint(1.0f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.body_z_rate = 1.0f;
    ctlData.groundspeed = NAN;
    ctlData.groundspeed_scaler = 3.0f;
    ctlData.lock_integrator = 3.0f;

    rateSetpoint = wheelController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
             "FAC ECL_WheelController(), control_bodyrate_bad_inputs(Invalid input #2)");

    /* Invalid input #3 */
    wheelController.init();
    wheelController.set_k_i(1.0f);
    wheelController.set_k_p(1.0f);
    wheelController.set_k_ff(1.0f);
    wheelController.set_bodyrate_setpoint(1.0f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.body_z_rate = 1.0f;
    ctlData.groundspeed = 2.0f;
    ctlData.groundspeed_scaler = NAN;
    ctlData.lock_integrator = 3.0f;

    rateSetpoint = wheelController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
             "FAC ECL_WheelController(), control_bodyrate_bad_inputs(Invalid input #3)");
}


/**
 * Test FAC ECL_WheelController(), control_bodyrate_constrain
 */
void Test_FAC_ECL_WheelController_control_bodyrate_constrain(void)
{
    ECL_WheelController wheelController;
    ECL_ControlData ctlData;
    float rateSetpoint = 0.0f;

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
               (void*)&Test_FAC_GetPSPTimeHook);

    /* Get nominal baseline #1 */
    wheelController.init();
    wheelController.set_k_i(1.0f);
    wheelController.set_k_p(1.0f);
    wheelController.set_k_ff(1.0f);
    wheelController.set_bodyrate_setpoint(1.0f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.body_z_rate = 1.0f;
    ctlData.groundspeed = 2.0f;
    ctlData.groundspeed_scaler = 3.0f;

    rateSetpoint = wheelController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, -1.0, FLT_EPSILON,
          "FAC ECL_WheelController(), control_bodyrate_constrain(nominal baseline #1, First run)");

    rateSetpoint = wheelController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, -1.0, FLT_EPSILON,
          "FAC ECL_WheelController(), control_bodyrate_constrain(nominal baseline #1, Second run)");

    /* Get nominal baseline #2 */
    wheelController.init();
    wheelController.set_k_i(1.0f);
    wheelController.set_k_p(1.0f);
    wheelController.set_k_ff(1.0f);
    wheelController.set_bodyrate_setpoint(1.0f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.body_z_rate = -1.0f;
    ctlData.groundspeed = 2.0f;
    ctlData.groundspeed_scaler = 3.0f;

    rateSetpoint = wheelController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 1.0, FLT_EPSILON,
          "FAC ECL_WheelController(), control_bodyrate_constrain(nominal baseline #2, First run)");

    rateSetpoint = wheelController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 1.0, FLT_EPSILON,
          "FAC ECL_WheelController(), control_bodyrate_constrain(nominal baseline #2, Second run)");
}


/**
 * Test FAC ECL_WheelController(), control_euler_rate
 */
void Test_FAC_ECL_WheelController_control_euler_rate(void)
{
    ECL_WheelController wheelController;
    ECL_ControlData ctlData;
    float result = 0.0f;

    /* Just call the function for coverage. */
    result = wheelController.control_euler_rate(ctlData);
}


/**
 * Test FAC ECL_YawController(), control_attitude_bad_inputs
 */
void Test_FAC_ECL_YawController_control_attitude_bad_inputs(void)
{
    ECL_YawController yawController;
    ECL_ControlData ctlData;
    float rateSetpoint = 0.0f;

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
               (void*)&Test_FAC_GetPSPTimeHook);

    /* Get nominal baseline #1: Not inverted */
    yawController.init();
    yawController.set_coordinated_method(ECL_YawController::COORD_METHOD_OPEN);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.roll = 1.5f;
    ctlData.pitch = 1.0f;
    ctlData.roll_setpoint = 1.0f;
    ctlData.airspeed_min = 1.0f;
    ctlData.airspeed = 4.0f;
    yawController.set_max_rate(10.0);

    rateSetpoint = yawController.control_attitude(ctlData);
    UtAssert_DoubleCmpAbs((double)rateSetpoint, 2.063708, 1e-6,     // Fail with FLT_EPSILON
                "FAC ECL_YawController(), control_attitude_bad_inputs(nominal baseline #1)");

    /* Get nominal baseline #2: Inverted */
    yawController.init();
    yawController.set_coordinated_method(ECL_YawController::COORD_METHOD_OPEN);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.roll = 2.0f;
    ctlData.pitch = 3.0f;
    ctlData.roll_setpoint = 1.0f;
    ctlData.airspeed_min = 1.0f;
    ctlData.airspeed = 4.0f;
    yawController.set_max_rate(10.0);

    rateSetpoint = yawController.control_attitude(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
                "FAC ECL_YawController(), control_attitude_bad_inputs(nominal baseline #2)");

    /* Get nominal baseline #3: left hemisphere */
    yawController.init();
    yawController.set_coordinated_method(ECL_YawController::COORD_METHOD_OPEN);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.roll = -2.0f;
    ctlData.pitch = 3.0f;
    ctlData.roll_setpoint = -1.0f;
    ctlData.airspeed_min = 1.0f;
    ctlData.airspeed = 4.0f;
    yawController.set_max_rate(10.0);

    rateSetpoint = yawController.control_attitude(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
                "FAC ECL_YawController(), control_attitude_bad_inputs(nominal baseline #3)");

    /* Invalid input #1 */
    yawController.init();
    yawController.set_coordinated_method(ECL_YawController::COORD_METHOD_OPEN);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.roll = NAN;
    ctlData.pitch = 3.0f;
    ctlData.roll_setpoint = 1.0f;
    ctlData.roll_rate_setpoint = NAN;
    ctlData.pitch_rate_setpoint = NAN;
    ctlData.airspeed_min = 1.0f;
    ctlData.airspeed = 1.0f;
    yawController.set_max_rate(10.0);

    rateSetpoint = yawController.control_attitude(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
                "FAC ECL_YawController(), control_attitude_bad_inputs(Invalid input #1)");

    /* Invalid input #2 */
    yawController.init();
    yawController.set_coordinated_method(ECL_YawController::COORD_METHOD_CLOSEACC);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.roll = -2.0f;
    ctlData.pitch = 3.0f;
    ctlData.roll_setpoint = -1.0f;
    ctlData.roll_rate_setpoint = 1.0f;
    ctlData.pitch_rate_setpoint = 1.0f;
    ctlData.airspeed_min = 1.0f;
    ctlData.airspeed = 1.0f;
    yawController.set_max_rate(10.0);

    rateSetpoint = yawController.control_attitude(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
                "FAC ECL_YawController(), control_attitude_bad_inputs(Invalid input #2)");

    /* Invalid input #3 */
    yawController.init();
    yawController.set_coordinated_method(2);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.roll = -2.0f;
    ctlData.pitch = 3.0f;
    ctlData.roll_setpoint = -1.0f;
    ctlData.roll_rate_setpoint = 1.0f;
    ctlData.pitch_rate_setpoint = 1.0f;
    ctlData.airspeed_min = 1.0f;
    ctlData.airspeed = 1.0f;
    yawController.set_max_rate(10.0);

    rateSetpoint = yawController.control_attitude(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
                "FAC ECL_YawController(), control_attitude_bad_inputs(Invalid input #3)");
}


/**
 * Test FAC ECL_YawController(), control_bodyrate_bad_inputs
 */
void Test_FAC_ECL_YawController_control_bodyrate_bad_inputs(void)
{
    ECL_YawController yawController;
    ECL_ControlData ctlData;
    float rateSetpoint = 0.0f;

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
               (void*)&Test_FAC_GetPSPTimeHook);

    /* Get nominal baseline #1 */
    yawController.init();
    yawController.set_coordinated_method(ECL_YawController::COORD_METHOD_OPEN);
    yawController.set_k_p(1.0f);
    yawController.set_k_i(1.0f);
    yawController.set_k_ff(0.5f);
    yawController.set_bodyrate_setpoint(0.5f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.body_z_rate = 7.0f;
    ctlData.airspeed_min = 1.0f;
    ctlData.airspeed_max = 100.0f;
    ctlData.airspeed = 4.0f;
    ctlData.scaler = 1.0f;
    ctlData.lock_integrator = false;
    yawController.set_max_rate(10.0);

    rateSetpoint = yawController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, -1.0, FLT_EPSILON,
          "FAC ECL_YawController(), control_bodyrate_bad_inputs(nominal baseline #1, First run)");

    rateSetpoint = yawController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, -1.0, FLT_EPSILON,
          "FAC ECL_YawController(), control_bodyrate_bad_inputs(nominal baseline #1, Second run)");

    /* Get nominal baseline #2 */
    yawController.init();
    yawController.set_coordinated_method(ECL_YawController::COORD_METHOD_OPEN);
    yawController.set_k_p(1.0f);
    yawController.set_k_i(1.0f);
    yawController.set_k_ff(0.5f);
    yawController.set_bodyrate_setpoint(2.5f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.body_z_rate = 2.0f;
    ctlData.airspeed_min = 1.0f;
    ctlData.airspeed_max = 100.0f;
    ctlData.airspeed = 0.8f;
    ctlData.scaler = 1.0f;
    ctlData.lock_integrator = false;
    yawController.set_max_rate(10.0);

    rateSetpoint = yawController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 1.0, FLT_EPSILON,
          "FAC ECL_YawController(), control_bodyrate_bad_inputs(nominal baseline #2, First run)");

    rateSetpoint = yawController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 1.0, FLT_EPSILON,
          "FAC ECL_YawController(), control_bodyrate_bad_inputs(nominal baseline #2, Second run)");

    /* Invalid input #1 */
    yawController.init();
    yawController.set_coordinated_method(ECL_YawController::COORD_METHOD_OPEN);
    yawController.set_k_p(1.0f);
    yawController.set_k_i(1.0f);
    yawController.set_k_ff(0.5f);
    yawController.set_bodyrate_setpoint(2.5f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.roll = NAN;
    ctlData.pitch = NAN;
    ctlData.body_z_rate = NAN;
    ctlData.body_y_rate = NAN;
    ctlData.pitch_rate_setpoint = NAN;
    ctlData.airspeed_min = 1.0f;
    ctlData.airspeed_max = 100.0f;
    ctlData.airspeed = 4.0f;
    ctlData.scaler = 1.0f;
    ctlData.lock_integrator = false;
    yawController.set_max_rate(10.0);

    rateSetpoint = yawController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, 0.0, FLT_EPSILON,
             "FAC ECL_YawController(), control_bodyrate_bad_inputs(Invalid input #1)");

    /* Invalid input #2 */
    yawController.init();
    yawController.set_coordinated_method(ECL_YawController::COORD_METHOD_OPEN);
    yawController.set_k_p(1.0f);
    yawController.set_k_i(1.0f);
    yawController.set_k_ff(0.5f);
    yawController.set_bodyrate_setpoint(2.5f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.body_z_rate = 7.0f;
    ctlData.airspeed_min = 1.0f;
    ctlData.airspeed_max = 100.0f;
    ctlData.airspeed = NAN;
    ctlData.scaler = 1.0f;
    ctlData.lock_integrator = false;
    yawController.set_max_rate(10.0);

    rateSetpoint = yawController.control_bodyrate(ctlData);
    UtAssert_DoubleCmpAbs(rateSetpoint, -1.0, FLT_EPSILON,
             "FAC ECL_YawController(), control_bodyrate_bad_inputs(Invalid input #2)");
}


/**
 * Test FAC ECL_YawController(), control_euler_rate
 */
void Test_FAC_ECL_YawController_control_euler_rate(void)
{
    ECL_YawController yawController;
    ECL_ControlData ctlData;
    float result = 0.0f;

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
               (void*)&Test_FAC_GetPSPTimeHook);

    /* Get nominal baseline #1 */
    yawController.init();
    yawController.set_coordinated_method(ECL_YawController::COORD_METHOD_OPEN);
    yawController.set_k_p(1.0f);
    yawController.set_k_i(1.0f);
    yawController.set_k_ff(0.5f);
    yawController.set_bodyrate_setpoint(0.5f);
    memset((void*)&ctlData, 0x00, sizeof(ctlData));
    ctlData.roll = 2.0f;
    ctlData.pitch = 3.0f;
    ctlData.body_z_rate = 7.0f;
    ctlData.pitch_rate_setpoint = 4.0f;
    ctlData.airspeed_min = 1.0f;
    ctlData.airspeed_max = 100.0f;
    ctlData.airspeed = 4.0f;
    ctlData.scaler = 1.0f;
    ctlData.lock_integrator = false;
    yawController.set_max_rate(10.0);

    result = yawController.control_euler_rate(ctlData);
}


/**
 * Test FAC ECL_YawController(), get_coordinated_min_speed
 */
void Test_FAC_ECL_YawController_get_coordinated_min_speed(void)
{
    ECL_YawController yawController;
    float result = 0.0f;

    result = yawController.get_coordinated_min_speed();
}


/**
 * Test FAC ECL_YawController(), get_coordinated_method
 */
void Test_FAC_ECL_YawController_get_coordinated_method(void)
{
    ECL_YawController yawController;
    int32_t result = 0;

    result = yawController.get_coordinated_method();
}



/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void FAC_App_Test_AddTestCases(void)
{

    UtTest_Add(Test_FAC_InitEvent_Fail_Register, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitEvent_Fail_Register");

    UtTest_Add(Test_FAC_InitPipe_Fail_CreateSCHPipe, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_CreateSCHPipe");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeSendHK, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeSendHK");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeRunController, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeRunController");
    UtTest_Add(Test_FAC_InitPipe_Fail_CreateCMDPipe, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_CreateCMDPipe");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeCMD, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeCMD");
    UtTest_Add(Test_FAC_InitPipe_Fail_CreateDATAPipe, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_CreateDATAPipe");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeBatteryStatus, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeBatteryStatus");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeManualControlSp, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeManualControlSp");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeVAttSp, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeVAttSp");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeVAtt, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeVAtt");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeVControlMode, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeVControlMode");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeVehicleStatus, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeVehicleStatus");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeVGlobalPosition, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeVGlobalPosition");
    UtTest_Add(Test_FAC_InitPipe_Fail_SubscribeVLandDetected, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitPipe_Fail_SubscribeVLandDetected");

    UtTest_Add(Test_FAC_InitDataNominal, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitDataNominal");

    UtTest_Add(Test_FAC_InitApp_Fail_InitEvent, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitApp_Fail_InitEvent");
    UtTest_Add(Test_FAC_InitApp_Fail_InitPipe, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitApp_Fail_InitPipe");
    UtTest_Add(Test_FAC_InitApp_Fail_InitData, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitApp_Fail_InitData");
    UtTest_Add(Test_FAC_InitApp_Fail_InitConfigTbl, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitApp_Fail_InitConfigTbl");
    UtTest_Add(Test_FAC_InitApp_Nominal, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitApp_Nominal");

    UtTest_Add(Test_FAC_FAC_AppMain_Nominal, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_FAC_AppMain_Nominal");

    UtTest_Add(Test_FAC_AppMain_Fail_RegisterApp, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_AppMain_Fail_RegisterApp");
    UtTest_Add(Test_FAC_AppMain_Fail_InitApp, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_AppMain_Fail_InitApp");
    UtTest_Add(Test_FAC_AppMain_Fail_AcquireConfigPtrs, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_AppMain_Fail_AcquireConfigPtrs");
    UtTest_Add(Test_FAC_AppMain_SchPipeError, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_AppMain_SchPipeError");
    UtTest_Add(Test_FAC_AppMain_SchPipeNoMessage, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_AppMain_SchPipeNoMessage");
    UtTest_Add(Test_FAC_AppMain_InvalidSchMessage, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_AppMain_InvalidSchMessage");
    UtTest_Add(Test_FAC_AppMain_Nominal_SendHK, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_AppMain_Nominal_SendHK");
    UtTest_Add(Test_FAC_AppMain_Nominal_RunController, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_AppMain_Nominal_RunController");

    UtTest_Add(Test_FAC_AppMain_ProcessIncomingData_DataPipeError, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_AppMain_ProcessIncomingData_DataPipeError");
    UtTest_Add(Test_FAC_AppMain_ProcessIncomingData_InvalidMsgID, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_AppMain_ProcessIncomingData_InvalidMsgID");
    UtTest_Add(Test_FAC_AppMain_ProcessIncomingData_Airspeed, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_AppMain_ProcessIncomingData_Airspeed");
    UtTest_Add(Test_FAC_AppMain_ProcessIncomingData_BatteryStatus, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_AppMain_ProcessIncomingData_BatteryStatus");
    UtTest_Add(Test_FAC_AppMain_ProcessIncomingData_ManualControlSp, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_AppMain_ProcessIncomingData_ManualControlSp");
    UtTest_Add(Test_FAC_AppMain_ProcessIncomingData_VAtt, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_AppMain_ProcessIncomingData_VAtt");
    UtTest_Add(Test_FAC_AppMain_ProcessIncomingData_VAttSp, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_AppMain_ProcessIncomingData_VAttSp");
    UtTest_Add(Test_FAC_AppMain_ProcessIncomingData_VControlMode, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_AppMain_ProcessIncomingData_VControlMode");
    UtTest_Add(Test_FAC_AppMain_ProcessIncomingData_VGlobalPosition, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_AppMain_ProcessIncomingData_VGlobalPosition");
    UtTest_Add(Test_FAC_AppMain_ProcessIncomingData_VLandDetected, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_AppMain_ProcessIncomingData_VLandDetected");
    UtTest_Add(Test_FAC_AppMain_ProcessIncomingData_VehicleStatus, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_AppMain_ProcessIncomingData_VehicleStatus");

    UtTest_Add(Test_FAC_RunController_TailSitter, FAC_Test_Setup_TailSitter,
               FAC_Test_TearDown, "Test_FAC_RunController_TailSitter");
    UtTest_Add(Test_FAC_RunController_RotaryWing, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_RunController_RotaryWing");
    UtTest_Add(Test_FAC_RunController_AttitudeManual, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_RunController_AttitudeManual");
    UtTest_Add(Test_FAC_RunController_PureRateControl, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_RunController_PureRateControl");
    UtTest_Add(Test_FAC_RunController_ManualDirect, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_RunController_ManualDirect");
    UtTest_Add(Test_FAC_RunController_Auto, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_RunController_Auto");
    UtTest_Add(Test_FAC_RunController_Landed, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_RunController_Landed");
    UtTest_Add(Test_FAC_RunController_TerminationEnabled, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_RunController_TerminationEnabled");
    UtTest_Add(Test_FAC_RunController_InvalidValue, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_RunController_InvalidValue");

    UtTest_Add(Test_FAC_UpdateParams_Standard, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_UpdateParams_Standard");
    UtTest_Add(Test_FAC_UpdateParams_TailSitter, FAC_Test_Setup_TailSitter, FAC_Test_TearDown,
               "Test_FAC_UpdateParams_TailSitter");

    UtTest_Add(Test_FAC_ECL_PitchController_control_attitude_bad_inputs, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_ECL_PitchController_control_attitude_bad_inputs");
    UtTest_Add(Test_FAC_ECL_PitchController_control_bodyrate_bad_inputs, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_ECL_PitchController_control_bodyrate_bad_inputs");
    UtTest_Add(Test_FAC_ECL_PitchController_control_bodyrate_constrain, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_ECL_PitchController_control_bodyrate_constrain");
    UtTest_Add(Test_FAC_ECL_PitchController_control_euler_rate, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_ECL_PitchController_control_euler_rate");
    UtTest_Add(Test_FAC_ECL_RollController_control_attitude_bad_inputs, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_ECL_RollController_control_attitude_bad_inputs");
    UtTest_Add(Test_FAC_ECL_RollController_control_bodyrate_bad_inputs, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_ECL_RollController_control_bodyrate_bad_inputs");
    UtTest_Add(Test_FAC_ECL_RollController_control_bodyrate_constrain, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_ECL_RollController_control_bodyrate_constrain");
    UtTest_Add(Test_FAC_ECL_RollController_control_euler_rate, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_ECL_RollController_control_euler_rate");
    UtTest_Add(Test_FAC_ECL_WheelController_get_desired_rate, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_ECL_WheelController_get_desired_rate");
    UtTest_Add(Test_FAC_ECL_WheelController_control_attitude_bad_inputs, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_ECL_WheelController_control_attitude_bad_inputs");
    UtTest_Add(Test_FAC_ECL_WheelController_control_attitude_constrain, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_ECL_WheelController_control_attitude_constrain");
    UtTest_Add(Test_FAC_ECL_WheelController_control_bodyrate_bad_inputs, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_ECL_WheelController_control_bodyrate_bad_inputs");
    UtTest_Add(Test_FAC_ECL_WheelController_control_bodyrate_constrain, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_ECL_WheelController_control_bodyrate_constrain");
    UtTest_Add(Test_FAC_ECL_WheelController_control_euler_rate, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_ECL_WheelController_control_euler_rate");
    UtTest_Add(Test_FAC_ECL_YawController_control_attitude_bad_inputs, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_ECL_YawController_control_attitude_bad_inputs");
    UtTest_Add(Test_FAC_ECL_YawController_control_bodyrate_bad_inputs, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_ECL_YawController_control_bodyrate_bad_inputs");
    UtTest_Add(Test_FAC_ECL_YawController_control_euler_rate, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_ECL_YawController_control_euler_rate");
    UtTest_Add(Test_FAC_ECL_YawController_get_coordinated_min_speed, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_ECL_YawController_get_coordinated_min_speed");
    UtTest_Add(Test_FAC_ECL_YawController_get_coordinated_method, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_ECL_YawController_get_coordinated_method");
}
