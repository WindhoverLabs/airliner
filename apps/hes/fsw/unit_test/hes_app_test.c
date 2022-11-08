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

#include "hes_app_test.h"
#include "hes_app.h"
#include "hes_test_utils.h"

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

#include <time.h>


CFE_SB_MsgId_t  SendHK_SendMsgHook_MsgId = 0;
int32           WriteToSysLog_HookCalledCnt = 0;

/**************************************************************************
 * Tests for HES_InitEvent()
 **************************************************************************/

/**
 * Test HES_InitEvent() with failed CFE_EVS_Register
 */
void Test_HES_InitEvent_Fail_Register(void)
{
    /* Set a fail result for EVS */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EVENTS_SERVICE | CFE_EVS_NOT_IMPLEMENTED;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = HES_InitEvent();

    /* Verify results */
    UtAssert_True (result == expected, "InitEvent, failed EVS Register");
}


/**************************************************************************
 * Tests for HES_InitPipe()
 **************************************************************************/
/**
 * Test HES_InitPipe(), fail SCH CFE_SB_CreatePipe
 */
void Test_HES_InitPipe_Fail_CreateSCHPipe(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = HES_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create SCH pipe");
}


/**
 * Test HES_InitPipe(), fail CFE_SB_SubscribeEx for wakeup
 */
void Test_HES_InitPipe_Fail_SubscribeWakeup(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    result = HES_InitPipe();

    /* Verify results */
    UtAssert_True(result == expected,
                  "InitPipe, fail CFE_SB_SubscribeEx for wakeup");
}


/**
 * Test HES_InitPipe(), fail CFE_SB_SubscribeEx for sendhk
 */
void Test_HES_InitPipe_Fail_SubscribeSendHK(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 2);

    /* Execute the function being tested */
    result = HES_InitPipe();

    /* Verify results */
    UtAssert_True(result == expected,
                  "InitPipe, fail CFE_SB_SubscribeEx for sendhk");
}


/**
 * Test HES_InitPipe(), fail CMD CFE_SB_CreatePipe
 */
void Test_HES_InitPipe_Fail_CreateCMDPipe(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = HES_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create CMD pipe");
}


/**
 * Test HES_InitPipe(), fail CFE_SB_Subscribe for CMD msg
 */
void Test_HES_InitPipe_Fail_SubscribeCMD(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = HES_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_Subscribe for CMD");
}


/**
 * Test HES_InitPipe(), fail DATA CFE_SB_CreatePipe
 */
void Test_HES_InitPipe_Fail_CreateDATAPipe(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 3);

    /* Execute the function being tested */
    result = HES_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create DATA pipe");
}


/**
 * Test HES_InitPipe(), fail SubscribeEx for VGlobalPosition
 */
void Test_HES_InitPipe_Fail_SubscribeVGlobalPosition(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 3);

    /* Execute the function being tested */
    result = HES_InitPipe();

    /* Verify results */
    UtAssert_True(result == expected,
                  "InitPipe, fail CFE_SB_SubscribeEx for VGlobalPosition");
}


/**
 * Test HES_InitPipe(), fail SubscribeEx for VAtt
 */
void Test_HES_InitPipe_Fail_SubscribeVAtt(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 4);

    /* Execute the function being tested */
    result = HES_InitPipe();

    /* Verify results */
    UtAssert_True(result == expected,
                  "InitPipe, fail CFE_SB_SubscribeEx for VAtt");
}


/**
 * Test HES_InitPipe(), fail SubscribeEx for Airspeed
 */
void Test_HES_InitPipe_Fail_SubscribeAirspeed(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 5);

    /* Execute the function being tested */
    result = HES_InitPipe();

    /* Verify results */
    UtAssert_True(result == expected,
                  "InitPipe, fail CFE_SB_SubscribeEx for Airspeed");
}


/**
 * Test HES_InitPipe(), fail SubscribeEx for BatteryStatus
 */
void Test_HES_InitPipe_Fail_SubscribeBatteryStatus(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 6);

    /* Execute the function being tested */
    result = HES_InitPipe();

    /* Verify results */
    UtAssert_True(result == expected,
                  "InitPipe, fail CFE_SB_SubscribeEx for BatteryStatus");
}


/**
 * Test HES_InitPipe(), fail SubscribeEx for VLandDetected
 */
void Test_HES_InitPipe_Fail_SubscribeVLandDetected(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 7);

    /* Execute the function being tested */
    result = HES_InitPipe();

    /* Verify results */
    UtAssert_True(result == expected,
                  "InitPipe, fail CFE_SB_SubscribeEx for VLandDetected");
}


/**
 * Test HES_InitPipe(), fail SubscribeEx for VehicleStatus
 */
void Test_HES_InitPipe_Fail_SubscribeVehicleStatus(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 8);

    /* Execute the function being tested */
    result = HES_InitPipe();

    /* Verify results */
    UtAssert_True(result == expected,
                  "InitPipe, fail CFE_SB_SubscribeEx for VehicleStatus");
}


/**************************************************************************
 * Tests for HES_InitData()
 **************************************************************************/
/**
 * Test HES_InitData()
 */
void Test_HES_InitData(void)
{
    /* Set a fail result */
    int32 result = -1;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = HES_InitData();

    /* Verify results */
    UtAssert_True (result == expected, "InitData");
}


/**************************************************************************
 * Tests for HES_InitApp()
 **************************************************************************/
/**
 * Test HES_InitApp(), fail init event
 */
void Test_HES_InitApp_Fail_InitEvent(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = HES_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init event");
}


/**
 * Test HES_InitApp(), fail init pipe
 */
void Test_HES_InitApp_Fail_InitPipe(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = HES_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init pipe");
}


/**
 * Test HES_InitApp(), fail init data.
 * NOTE: no current way to fail HES_InitData() in default
 */
void Test_HES_InitApp_Fail_InitData(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = HES_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init data");
}


/**
 * Test HES_InitApp(), fail init config table
 */
void Test_HES_InitApp_Fail_InitConfigTbl(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_NAME;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = HES_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init config table");
}


/**
 * Test HES_InitApp(), Nominal
 */
void Test_HES_InitApp_Nominal(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = HES_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, nominal");
}


/**************************************************************************
 * Tests for HES_AppMain()
 **************************************************************************/
/**
 * Test HES AppMain(), WriteToSysLogHook
 */
int32 Test_HES_AppMain_WriteToSysLogHook(const char *StringPtr, ...)
{
    va_list   Ptr;
    char      Buf[256];

    WriteToSysLog_HookCalledCnt++;

    va_start(Ptr, StringPtr);
    vsnprintf(Buf, (size_t)CFE_EVS_MAX_MESSAGE_LENGTH, StringPtr, Ptr);
    va_end(Ptr);

    printf("###AppMain_WriteToSysLogHook:\n");
    printf("%s", Buf);

    return CFE_SUCCESS;
}


/**
 * Test HES_AppMain(), Fail RegisterApp
 */
void Test_HES_AppMain_Fail_RegisterApp(void)
{
    /* fail the register app */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX,
                            CFE_ES_ERR_APP_REGISTER, 1);

    /* Execute the function being tested */
    HES_AppMain();
}


/**
 * Test HES_AppMain(), Fail InitApp
 */
void Test_HES_AppMain_Fail_InitApp(void)
{
    /* fail the register app */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX,
                             CFE_EVS_APP_NOT_REGISTERED, 1);

    WriteToSysLog_HookCalledCnt = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_WRITETOSYSLOG_INDEX,
               (void*)&Test_HES_AppMain_WriteToSysLogHook);

    /* Execute the function being tested */
    HES_AppMain();

    /* Verify results */
    UtAssert_True (WriteToSysLog_HookCalledCnt == 3, "AppMain, Fail_InitApp");
}


/**
 * Test HES_AppMain(), Fail AcquireConfigPtrs
 */
void Test_HES_AppMain_Fail_AcquireConfigPtrs(void)
{
    char  expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX,
                             CFE_TBL_ERR_INVALID_HANDLE, 2);

    /* Execute the function being tested */
    HES_AppMain();

    sprintf(expectedEvent, "Failed to get Config table's address (0x%08X)",
                           (unsigned int)CFE_TBL_ERR_INVALID_HANDLE);

    /* Verify results */
    UtAssert_EventSent(HES_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
                       expectedEvent, "AppMain(), Fail AcquireConfigPtrs");
}


/**
 * Test HES_AppMain(), Invalid Schedule Message
 */
void Test_HES_AppMain_InvalidSchMessage(void)
{
    char  expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a SCH message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, 0, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    HES_AppMain();

    sprintf(expectedEvent, "Recvd invalid SCH msgId (0x%04X)", 0);

    /* Verify results */
    UtAssert_EventSent(HES_MSGID_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "AppMain(), Invalid Schedule Message");
}


/**
 * Hook to support: HES_AppMain(), Nominal - SendHK
 */
int32 Test_HES_AppMain_Nominal_SendHK_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    unsigned char*     pBuff = NULL;
    uint16             msgLen = 0;
    int                i = 0;
    CFE_SB_MsgId_t     MsgId;
    time_t             localTime;
    struct tm          *loc_time;
    CFE_TIME_SysTime_t TimeFromMsg;
    HES_HkTlm_t        HkMsg;

    pBuff = (unsigned char*)MsgPtr;

    msgLen = CFE_SB_GetTotalMsgLength(MsgPtr);
    printf("###AppMain_SendHK_SendMsgHook: MsgLen(%u)\n", msgLen);
    for (i = 0; i < msgLen; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff++;
    }
    printf("\n");

    TimeFromMsg = CFE_SB_GetMsgTime(MsgPtr);
    localTime = HES_Test_GetTimeFromMsg(TimeFromMsg);
    loc_time = localtime(&localTime);
    printf("TimeFromMessage: %s", asctime(loc_time));

    MsgId = CFE_SB_GetMsgId(MsgPtr);
    switch (MsgId)
    {
        case HES_HK_TLM_MID:
        {
            SendHK_SendMsgHook_MsgId = HES_HK_TLM_MID;
            CFE_PSP_MemCpy((void*)&HkMsg, (void*)MsgPtr, sizeof(HkMsg));
            printf("Sent HES_HK_TLM_MID:\n");
            break;
        }
        default:
        {
            printf("Sent MID(0x%04x)\n", MsgId);
            break;
        }
    }

    return CFE_SUCCESS;
}

/**
 * Test HES_AppMain(), Nominal - SendHK
 */
void Test_HES_AppMain_Nominal_SendHK(void)
{
    /* The following will emulate behavior of receiving a SCH message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, HES_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Used to verify HK was transmitted correctly. */
    SendHK_SendMsgHook_MsgId = 0;
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                    (void *)&Test_HES_AppMain_Nominal_SendHK_SendMsgHook);

    /* Execute the function being tested */
    HES_AppMain();

    /* Verify results */
    UtAssert_True(SendHK_SendMsgHook_MsgId == HES_HK_TLM_MID,
                  "AppMain_Nominal_SendHK");
}


/**
 * Test HES_AppMain(), Nominal - Wakeup
 */
void Test_HES_AppMain_Nominal_Wakeup(void)
{
    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, HES_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    HES_AppMain();

}


/**
 * Test HES_AppMain(), ProcessNewData - InvalidMsgID
 */
void Test_HES_AppMain_ProcessNewData_InvalidMsgID(void)
{
    PX4_AirspeedMsg_t  InMsg;
    int32              DataPipe;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it data to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("HES_DATA_PIPE");
    CFE_SB_InitMsg (&InMsg, 0x0000, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InMsg, DataPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, HES_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    HES_AppMain();

    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==3,"Event Count = 3");
    UtAssert_EventSent(HES_MSGID_ERR_EID, CFE_EVS_ERROR, "", "Error Event Sent");
}



/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void HES_App_Test_AddTestCases(void)
{
    UtTest_Add(Test_HES_InitEvent_Fail_Register,
               HES_Test_Setup, HES_Test_TearDown,
               "Test_HES_InitEvent_Fail_Register");

    UtTest_Add(Test_HES_InitPipe_Fail_CreateSCHPipe,
               HES_Test_Setup, HES_Test_TearDown,
               "Test_HES_InitPipe_Fail_CreateSCHPipe");
    UtTest_Add(Test_HES_InitPipe_Fail_SubscribeWakeup,
               HES_Test_Setup, HES_Test_TearDown,
               "Test_HES_InitPipe_Fail_SubscribeWakeup");
    UtTest_Add(Test_HES_InitPipe_Fail_SubscribeSendHK,
               HES_Test_Setup, HES_Test_TearDown,
               "Test_HES_InitPipe_Fail_SubscribeSendHK");
    UtTest_Add(Test_HES_InitPipe_Fail_CreateCMDPipe,
               HES_Test_Setup, HES_Test_TearDown,
               "Test_HES_InitPipe_Fail_CreateCMDPipe");
    UtTest_Add(Test_HES_InitPipe_Fail_SubscribeCMD,
               HES_Test_Setup, HES_Test_TearDown,
               "Test_HES_InitPipe_Fail_SubscribeCMD");
    UtTest_Add(Test_HES_InitPipe_Fail_CreateDATAPipe,
               HES_Test_Setup, HES_Test_TearDown,
               "Test_HES_InitPipe_Fail_CreateDATAPipe");
    UtTest_Add(Test_HES_InitPipe_Fail_SubscribeVGlobalPosition,
               HES_Test_Setup, HES_Test_TearDown,
               "Test_HES_InitPipe_Fail_SubscribeVGlobalPosition");
    UtTest_Add(Test_HES_InitPipe_Fail_SubscribeVAtt,
               HES_Test_Setup, HES_Test_TearDown,
               "Test_HES_InitPipe_Fail_SubscribeVAtt");
    UtTest_Add(Test_HES_InitPipe_Fail_SubscribeAirspeed,
               HES_Test_Setup, HES_Test_TearDown,
               "Test_HES_InitPipe_Fail_SubscribeAirspeed");
    UtTest_Add(Test_HES_InitPipe_Fail_SubscribeBatteryStatus,
               HES_Test_Setup, HES_Test_TearDown,
               "Test_HES_InitPipe_Fail_SubscribeBatteryStatus");
    UtTest_Add(Test_HES_InitPipe_Fail_SubscribeVLandDetected,
               HES_Test_Setup, HES_Test_TearDown,
               "Test_HES_InitPipe_Fail_SubscribeVLandDetected");
    UtTest_Add(Test_HES_InitPipe_Fail_SubscribeVehicleStatus,
               HES_Test_Setup, HES_Test_TearDown,
               "Test_HES_InitPipe_Fail_SubscribeVehicleStatus");

    UtTest_Add(Test_HES_InitData,
               HES_Test_Setup, HES_Test_TearDown,
               "Test_HES_InitData");

    UtTest_Add(Test_HES_InitApp_Fail_InitEvent,
               HES_Test_Setup, HES_Test_TearDown,
               "Test_HES_InitApp_Fail_InitEvent");
    UtTest_Add(Test_HES_InitApp_Fail_InitPipe,
               HES_Test_Setup, HES_Test_TearDown,
               "Test_HES_InitApp_Fail_InitPipe");
    UtTest_Add(Test_HES_InitApp_Fail_InitData,
               HES_Test_Setup, HES_Test_TearDown,
               "Test_HES_InitApp_Fail_InitData");
    UtTest_Add(Test_HES_InitApp_Fail_InitConfigTbl,
               HES_Test_Setup, HES_Test_TearDown,
               "Test_HES_InitApp_Fail_InitConfigTbl");
    UtTest_Add(Test_HES_InitApp_Nominal,
               HES_Test_Setup, HES_Test_TearDown,
               "Test_HES_InitApp_Nominal");

    UtTest_Add(Test_HES_AppMain_Fail_RegisterApp,
               HES_Test_Setup, HES_Test_TearDown,
               "Test_HES_AppMain_Fail_RegisterApp");
    UtTest_Add(Test_HES_AppMain_Fail_InitApp,
               HES_Test_Setup, HES_Test_TearDown,
               "Test_HES_AppMain_Fail_InitApp");
    UtTest_Add(Test_HES_AppMain_Fail_AcquireConfigPtrs,
               HES_Test_Setup, HES_Test_TearDown,
               "Test_HES_AppMain_Fail_AcquireConfigPtrs");
    UtTest_Add(Test_HES_AppMain_InvalidSchMessage,
               HES_Test_Setup, HES_Test_TearDown,
               "Test_HES_AppMain_InvalidSchMessage");
    UtTest_Add(Test_HES_AppMain_Nominal_SendHK,
               HES_Test_Setup, HES_Test_TearDown,
               "Test_HES_AppMain_Nominal_SendHK");
    UtTest_Add(Test_HES_AppMain_Nominal_Wakeup,
               HES_Test_Setup, HES_Test_TearDown,
               "Test_HES_AppMain_Nominal_Wakeup");
    UtTest_Add(Test_HES_AppMain_ProcessNewData_InvalidMsgID,
               HES_Test_Setup, HES_Test_TearDown,
               "Test_HES_AppMain_ProcessNewData_InvalidMsgID");
}
