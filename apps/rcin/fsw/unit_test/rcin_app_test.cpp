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
#include "rcin_app.h"
#include "rcin_version.h"

#include "rcin_test_utils.h"
#include "rcin_custom_stubs.h"
#include "rcin_custom_hooks.h"
#include "rcin_app_test.h"

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
#include "ut_cfe_psp_timer_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_cfe_time_stubs.h"

#include <time.h>
#include <inttypes.h>


CFE_SB_MsgId_t SendHK_SendMsgHook_MsgId = 0;
RCIN_HkTlm_t   HkHookMsg;

uint32            InputRcMsgHook_CalledCnt = 0;
CFE_SB_MsgId_t    InputRcMsgHook_MsgId = 0;
PX4_InputRcMsg_t  HookInRcMsg;



/**************************************************************************
 * Tests for RCIN InitEvent()
 **************************************************************************/
/**
 * Test RCIN InitEvent() with failed CFE_EVS_Register
 */
void Test_RCIN_InitEvent_Fail_Register(void)
{
    RCIN oRCINut;

    /* Set a fail result for EVS */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EVENTS_SERVICE | CFE_EVS_NOT_IMPLEMENTED;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;
    char  expSysLog[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oRCINut.InitEvent();

    sprintf(expSysLog, "RCIN - Failed to register with EVS (0x%08lX)\n",
                       (long unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected, "InitEvent, failed CFE_EVS_Register");

    UtAssert_True(Ut_CFE_ES_SysLogWritten(expSysLog),
                  "InitEvent, failed CFE_EVS_Register: SysLog Written");
}


/**************************************************************************
 * Tests for RCIN InitPipe()
 **************************************************************************/
/**
 * Test RCIN InitPipe(), fail SCH CFE_SB_CreatePipe
 */
void Test_RCIN_InitPipe_Fail_CreateSCHPipe(void)
{
    RCIN oRCINut;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oRCINut.InitPipe();

    sprintf(expEvent, "Failed to create SCH pipe (0x%08lX)",
                      (long unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected, "InitPipe, fail SCH CFE_SB_CreatePipe");

    UtAssert_EventSent(RCIN_PIPE_INIT_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "InitPipe, fail SCH CFE_SB_CreatePipe: Event Sent");
}


/**
 * Test RCIN InitPipe(), fail CFE_SB_SubscribeEx for wakeup
 */
void Test_RCIN_InitPipe_Fail_SubscribeWakeup(void)
{
    RCIN oRCINut;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oRCINut.InitPipe();

    sprintf(expEvent,
            "Sch Pipe failed to subscribe to RCIN_WAKEUP_MID. (0x%08lX)",
            (long unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected,
                  "InitPipe, fail CFE_SB_SubscribeEx for wakeup");

    UtAssert_EventSent(RCIN_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR, expEvent,
                "InitPipe, fail CFE_SB_SubscribeEx for wakeup: Event Sent");
}


/**
 * Test RCIN InitPipe(), fail CFE_SB_SubscribeEx for sendhk
 */
void Test_RCIN_InitPipe_Fail_SubscribeSendHK(void)
{
    RCIN oRCINut;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oRCINut.InitPipe();

    sprintf(expEvent,
            "Sch Pipe failed to subscribe to RCIN_SEND_HK_MID. (0x%08X)",
            (unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected,
                  "InitPipe, fail CFE_SB_SubscribeEx for sendhk");

    UtAssert_EventSent(RCIN_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR, expEvent,
                "InitPipe, fail CFE_SB_SubscribeEx for sendhk: Event Sent");
}


/**
 * Test RCIN InitPipe(), fail CMD CFE_SB_CreatePipe
 */
void Test_RCIN_InitPipe_Fail_CreateCMDPipe(void)
{
    RCIN oRCINut;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oRCINut.InitPipe();

    sprintf(expEvent, "Failed to create CMD pipe (0x%08lX)",
                      (long unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected, "InitPipe, fail CMD CFE_SB_CreatePipe");

    UtAssert_EventSent(RCIN_PIPE_INIT_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "InitPipe, fail CMD CFE_SB_CreatePipe: Event Sent");
}


/**
 * Test RCIN InitPipe(), fail CFE_SB_Subscribe for CMD msg
 */
void Test_RCIN_InitPipe_Fail_SubscribeCMD(void)
{
    RCIN oRCINut;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oRCINut.InitPipe();

    sprintf(expEvent,
            "CMD Pipe failed to subscribe to RCIN_CMD_MID. (0x%08lX)",
            (long unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected,
                  "InitPipe, fail CFE_SB_Subscribe for CMD");

    UtAssert_EventSent(RCIN_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR, expEvent,
                     "InitPipe, fail CFE_SB_Subscribe for CMD: Event Sent");
}


/**************************************************************************
 * Tests for RCIN InitData()
 **************************************************************************/
/**
 * Test RCIN InitData()
 */
void Test_RCIN_InitData(void)
{
    RCIN oRCINut;

    /* Set a fail result */
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    oRCINut.InitData();

    /* Verify results */
    //UtAssert_True (result == expected, "InitData");
}


/**************************************************************************
 * Tests for RCIN InitApp()
 **************************************************************************/
/**
 * Test RCIN InitApp(), fail init event
 */
void Test_RCIN_InitApp_Fail_InitEvent(void)
{
    RCIN oRCINut;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;
    char  expSysLog[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oRCINut.InitApp();

    sprintf(expSysLog, "RCIN - Failed to init events (0x%08lX)\n",
                       (long unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected, "InitApp, fail init event");

    UtAssert_True(Ut_CFE_ES_GetSysLogQueueDepth() == 3,
                  "InitApp, fail init event: Sys Log QueueDepth");

    UtAssert_True(Ut_CFE_ES_SysLogWritten(expSysLog),
                  "InitApp, fail init event: Sys Log Written");
}


/**
 * Test RCIN InitApp(), fail init pipe
 */
void Test_RCIN_InitApp_Fail_InitPipe(void)
{
    RCIN oRCINut;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oRCINut.InitApp();

    sprintf(expEvent, "Failed to create SCH pipe (0x%08lX)",
            CFE_SB_BAD_ARGUMENT);

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init pipe");

    UtAssert_EventSent(RCIN_PIPE_INIT_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "InitApp, fail init pipe: Event Sent");
}


/**
 * Test RCIN InitApp(), fail init data.
 * NOTE: no current way to fail RCIN_InitData() in default
 */
void Test_RCIN_InitApp_Fail_InitData(void)
{
    RCIN oRCINut;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oRCINut.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init data");
}


/**
 * Test RCIN InitApp(), fail Custom_Init
 */
void Test_RCIN_InitApp_Fail_Custom_Init(void)
{
    RCIN oRCINut;

    int32 result = CFE_SUCCESS;
    int32 expected = RCIN_ERROR;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_RCIN_Custom_SetReturnCode(UT_RCIN_CUSTOM_SEDLIB_GETPIPE_INDEX,
                                 SEDLIB_PIPE_NOT_FOUND, 1);

    /* Execute the function being tested */
    result = oRCINut.InitApp();

    sprintf(expEvent, "Failed to initialize UART MsgPort. (0x%08lX)",
                      (long unsigned int)SEDLIB_PIPE_NOT_FOUND);

    /* Verify results */
    UtAssert_True(result == expected, "InitApp, fail Custom_Init");

    UtAssert_EventSent(RCIN_INIT_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "InitApp, fail Custom_Init: Event Sent");
}


/**
 * Test RCIN InitApp(), fail OS_TaskInstallDeleteHandler
 */
void Test_RCIN_InitApp_Fail_OS_TaskInstallDeleteHandler(void)
{
    RCIN  oRCINut;

    int32 result = CFE_SUCCESS;
    int32 expected = OS_ERROR;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_OSAPI_SetReturnCode(UT_OSAPI_TASKINSTALLDELETEHANDLER_INDEX,
                           expected, 1);

    /* Execute the function being tested */
    result = oRCINut.InitApp();

    sprintf(expEvent, "Failed to init register cleanup callback (0x%08X)",
                      (unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected,
                  "InitApp, fail OS_TaskInstallDeleteHandler");

    UtAssert_EventSent(RCIN_INIT_ERR_EID, CFE_EVS_ERROR, expEvent,
                 "InitApp, fail OS_TaskInstallDeleteHandler: Event Sent");
}


/**
 * Test RCIN InitApp(), Nominal
 */
void Test_RCIN_InitApp_Nominal(void)
{
    RCIN  oRCINut;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_RCIN_Custom_SetReturnCode(UT_RCIN_CUSTOM_SEDLIB_GETPIPE_INDEX,
                                 SEDLIB_OK, 1);

    /* Execute the function being tested */
    result = oRCINut.InitApp();

    sprintf(expEvent, "Initialized.  Version %d.%d.%d.%d",
            RCIN_MAJOR_VERSION, RCIN_MINOR_VERSION,
            RCIN_REVISION, RCIN_MISSION_REV);

    /* Verify results */
    UtAssert_True(result == expected, "InitApp, Nominal");

    UtAssert_EventSent(RCIN_INIT_INF_EID, CFE_EVS_INFORMATION, expEvent,
                       "InitApp, Nominal: Event Sent");
}


/**************************************************************************
 * Tests for extern RCIN_AppMain()
 **************************************************************************/
/**
 * Test extern RCIN_AppMain()
 */
void Test_Extern_RCIN_AppMain(void)
{
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_RCIN_Custom_SetReturnCode(UT_RCIN_CUSTOM_SEDLIB_GETPIPE_INDEX,
                                 SEDLIB_OK, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    RCIN_AppMain();

    sprintf(expEvent, "Initialized.  Version %d.%d.%d.%d",
            RCIN_MAJOR_VERSION, RCIN_MINOR_VERSION,
            RCIN_REVISION, RCIN_MISSION_REV);

    /* Verify results */
    UtAssert_EventSent(RCIN_INIT_INF_EID, CFE_EVS_INFORMATION, expEvent,
                       "RCIN_AppMain: Event Sent");
}


/**************************************************************************
 * Tests for RCIN AppMain()
 **************************************************************************/
/**
 * Test RCIN AppMain(), Fail RegisterApp
 */
void Test_RCIN_AppMain_Fail_RegisterApp(void)
{
    RCIN oRCINut;

    int32 expected = CFE_ES_ERR_APP_REGISTER;
    char  expSysLog[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* fail the register app */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX, expected, 1);

    /* Execute the function being tested */
    oRCINut.AppMain();

    sprintf(expSysLog, "RCIN - Failed to register the app (0x%08lX)\n",
                       (long unsigned int)expected);

    /* Verify results */
    UtAssert_True(Ut_CFE_ES_SysLogWritten(expSysLog),
                  "AppMain, Fail RegisterApp: SysLog Written");
}


/**
 * Test RCIN AppMain(), Fail InitApp
 */
void Test_RCIN_AppMain_Fail_InitApp(void)
{
    RCIN oRCINut;

    int32 expected = CFE_EVS_APP_NOT_REGISTERED;
    char  expSysLog[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* fail the register app */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    oRCINut.AppMain();

    sprintf(expSysLog, "RCIN - Application failed to initialize\n");

    /* Verify results */
    UtAssert_True(Ut_CFE_ES_GetSysLogQueueDepth() == 3,
                  "AppMain, Fail InitApp: SysLog QueueDepth");

    UtAssert_True(Ut_CFE_ES_SysLogWritten(expSysLog),
                  "AppMain, Fail InitApp: SysLog Written");
}


/**
 * Test RCIN AppMain(), Invalid Schedule Message
 */
void Test_RCIN_AppMain_InvalidSchMessage(void)
{
    RCIN  oRCINut;

    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will the emulate behavior of receiving a SCH message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, 0, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oRCINut.AppMain();

    sprintf(expEvent, "Recvd invalid SCH msgId (0x%04X)", 0);

    /* Verify results */
    UtAssert_EventSent(RCIN_MSGID_ERR_EID, CFE_EVS_ERROR, expEvent,
                  "AppMain, Invalid Schedule Message: Event Sent");
}


/**
 * Test RCIN AppMain(), NoSchMessage
 */
void Test_RCIN_AppMain_NoSchMessage(void)
{
    RCIN  oRCINut;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_NO_MESSAGE, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oRCINut.AppMain();

    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                  "AppMain, NoSchMessage: Event QueueDepth");
}


/**
 * Test RCIN AppMain(), SB_Timeout
 */
void Test_RCIN_AppMain_SB_Timeout(void)
{
    RCIN  oRCINut;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_TIME_OUT, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oRCINut.AppMain();

    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1,
                  "AppMain, SB_Timeout: Event QueueDepth");
}


/**
 * Test RCIN AppMain(), SchPipeError
 */
void Test_RCIN_AppMain_SchPipeError(void)
{
    RCIN  oRCINut;

    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_PIPE_RD_ERR, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oRCINut.AppMain();

    sprintf(expEvent, "SCH pipe read error (0x%08lX).",
                      (long unsigned int)CFE_SB_PIPE_RD_ERR);

    /* Verify results */
    UtAssert_EventSent(RCIN_RCVMSG_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "AppMain, SchPipeError: Event Sent");
}


/**
 * Hook to support: RCIN AppMain(), Nominal - SendHK
 */
int32 Test_RCIN_AppMain_Nominal_SendHK_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    unsigned char*     pBuff = NULL;
    uint16             msgLen = 0;
    int                i = 0;
    CFE_SB_MsgId_t     MsgId;
    time_t             localTime;
    struct tm          *loc_time;
    CFE_TIME_SysTime_t TimeFromMsg;

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
    localTime = RCIN_Test_GetTimeFromMsg(TimeFromMsg);
    loc_time = localtime(&localTime);
    printf("TimeFromMessage: %s", asctime(loc_time));

    MsgId = CFE_SB_GetMsgId(MsgPtr);
    switch (MsgId)
    {
        case RCIN_HK_TLM_MID:
        {
            SendHK_SendMsgHook_MsgId = RCIN_HK_TLM_MID;
            CFE_PSP_MemCpy((void*)&HkHookMsg, (void*)MsgPtr,
                           sizeof(HkHookMsg));

            printf("Sent RCIN_HK_TLM_MID:\n");
            printf("usCmdCnt: %u\n", HkHookMsg.usCmdCnt);
            printf("usCmdErrCnt: %u\n", HkHookMsg.usCmdErrCnt);
            printf("State: %u\n", HkHookMsg.State);
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
 * Test RCIN AppMain(), Nominal - SendHK
 */
void Test_RCIN_AppMain_Nominal_SendHK(void)
{
    RCIN  oRCINut;

    /* The following will emulate the behavior of receiving a SCH message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, RCIN_SEND_HK_MID, 1);

    /* To give the unit test system time for SB Msg */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void *)&CFE_SB_TimeStampMsgHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void *)&CFE_TIME_GetTimeHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Used to verify HK was transmitted correctly. */
    SendHK_SendMsgHook_MsgId = 0;
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                     (void*)&Test_RCIN_AppMain_Nominal_SendHK_SendMsgHook);

    /* Execute the function being tested */
    oRCINut.AppMain();

    /* Verify results */
    UtAssert_True(SendHK_SendMsgHook_MsgId == RCIN_HK_TLM_MID,
                  "AppMain, Nominal_SendHK: Sent RCIN_HK_TLM_MID");
}


/**
 * Hook to support: RCIN InputRcMsgHook
 */
int32 Test_RCIN_InputRcMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    unsigned char*     pBuff = NULL;
    uint16             msgLen = 0;
    int                i = 0;
    CFE_SB_MsgId_t     MsgId;
    time_t             localTime;
    struct tm          *loc_time;
    CFE_TIME_SysTime_t TimeFromMsg;

    pBuff = (unsigned char*)MsgPtr;

    msgLen = CFE_SB_GetTotalMsgLength(MsgPtr);
    printf("###InputRcMsgHook: MsgLen(%u)\n", msgLen);
    for (i = 0; i < msgLen; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff++;
    }
    printf("\n");

    TimeFromMsg = CFE_SB_GetMsgTime(MsgPtr);
    localTime = RCIN_Test_GetTimeFromMsg(TimeFromMsg);
    loc_time = localtime(&localTime);
    printf("TimeFromMessage: %s", asctime(loc_time));

    MsgId = CFE_SB_GetMsgId(MsgPtr);
    switch (MsgId)
    {
        case PX4_INPUT_RC_MID:
        {
            InputRcMsgHook_CalledCnt ++;
            InputRcMsgHook_MsgId = PX4_INPUT_RC_MID;
            CFE_PSP_MemCpy((void*)&HookInRcMsg, (void*)MsgPtr,
                           sizeof(HookInRcMsg));

            printf("Sent PX4_INPUT_RC_MID:\n");
            localTime = RCIN_Test_GetTimeFromTimestamp(
                                           HookInRcMsg.Timestamp);
            loc_time = localtime(&localTime);
            printf("Timestamp: %s", asctime(loc_time));
            printf("LastSignal: %" PRIu64"\n", HookInRcMsg.LastSignal);
            printf("ChannelCount: %lu\n", HookInRcMsg.ChannelCount);
            printf("RSSI: %ld\n", HookInRcMsg.RSSI);
            printf("RcLostFrameCount: %u\n", HookInRcMsg.RcLostFrameCount);
            printf("RcTotalFrameCount: %u\n", HookInRcMsg.RcTotalFrameCount);
            printf("RcPpmFrameLength: %u\n", HookInRcMsg.RcPpmFrameLength);
            printf("Values: ");
            for (i = 0; i < PX4_RC_INPUT_MAX_CHANNELS; i++)
            {
                printf("%04x ", HookInRcMsg.Values[i]);
            }
            printf("\n");
            printf("RcFailsafe: %u\n", HookInRcMsg.RcFailsafe);
            printf("RcLost: %u\n", HookInRcMsg.RcLost);
            printf("InputSource: %d\n", HookInRcMsg.InputSource);
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
 * Test RCIN AppMain(), Nominal - Wakeup
 */
void Test_RCIN_AppMain_Nominal_Wakeup(void)
{
    /* The following will emulate the behavior of receiving a SCH message
       to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, RCIN_WAKEUP_MID, 1);

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
                                     (void*)&CFE_PSP_GetTimeHook);

    /* To give the unit test system time for SB Msg */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void *)&CFE_SB_TimeStampMsgHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void *)&CFE_TIME_GetTimeHook);

    InputRcMsgHook_CalledCnt = 0;
    InputRcMsgHook_MsgId = 0;
    memset(&HookInRcMsg, 0x00, sizeof(HookInRcMsg));
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                              (void*)&Test_RCIN_InputRcMsgHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oRCIN.AppMain();

    /* Verify results */
    UtAssert_True(InputRcMsgHook_MsgId == PX4_INPUT_RC_MID,
                  "AppMain, Nominal - Wakeup: Sent PX4_INPUT_RC_MID");
}


/**************************************************************************
 * Tests for RCIN ReadDevice()
 **************************************************************************/
/**
 * Test RCIN ReadDevice(), 1Msg_Nominal
 */
void Test_RCIN_ReadDevice_1Msg_Nominal(void)
{
    int     i;
    uint32  total_error_count;
    uint32  hookmsg_checksum;
    uint32  checksum;
    char    expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_RCIN_Custom_SetReturnCode(UT_RCIN_CUSTOM_SEDLIB_GETPIPE_INDEX,
                                 SEDLIB_OK, 1);

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_TASKDELAY_INDEX,
                             (void *)&OS_TaskDelayHook);

    /* To make the custom parser find the footer */
    SEDLIB_ReadMsg_Cnt = 2;
    Ut_RCIN_Custom_SetFunctionHook(UT_RCIN_CUSTOM_SEDLIB_READMSG_INDEX,
                                   (void*)&SEDLIB_ReadMsgHook_1Msg_Nominal);

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
                                     (void*)&CFE_PSP_GetTimeHook);

    /* To give the unit test system time for SB Msg */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void *)&CFE_SB_TimeStampMsgHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void *)&CFE_TIME_GetTimeHook);

    InputRcMsgHook_CalledCnt = 0;
    InputRcMsgHook_MsgId = 0;
    memset(&HookInRcMsg, 0x00, sizeof(HookInRcMsg));
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                              (void*)&Test_RCIN_InputRcMsgHook);

    /* Execute the function being tested */
    oRCIN.InitApp();

    total_error_count = 0;
    if (RCIN_Util_Stream_Emulator() == RCIN_ERROR)
    {
        total_error_count ++;
    }

    oRCIN.ReadDevice();
    oRCIN.SendInputRcMsg();

    RCIN_CleanupCallback();

    hookmsg_checksum = 0;
    for (i = 0; i < TEST_RCIN_BUS_CHANNEL_CNT; i++)
    {
        hookmsg_checksum += HookInRcMsg.Values[i];
    }

    checksum = GetChecksum_1Msg_Nominal();

    sprintf(expEvent, "%s", "RCIN is publishing fresh data");

    /* Verify results */
    UtAssert_True(InputRcMsgHook_MsgId == PX4_INPUT_RC_MID,
                  "ReadDevice, 1Msg_Nominal: Sent PX4_INPUT_RC_MID");

    UtAssert_True(total_error_count == 0,
                  "ReadDevice, 1Msg_Nominal: total_error_count");

    UtAssert_True(oRCIN.StrikeCount == 0,
                  "ReadDevice, 1Msg_Nominal: StrikeCount");

    UtAssert_True(InputRcMsgHook_CalledCnt == 1,
                  "ReadDevice, 1Msg_Nominal: InputRcMsgHook_CalledCnt");

    UtAssert_True((HookInRcMsg.ChannelCount == TEST_RCIN_BUS_CHANNEL_CNT) &&
                  (HookInRcMsg.RSSI == 100) &&
                  (HookInRcMsg.RcLostFrameCount == 0) &&
                  (HookInRcMsg.RcTotalFrameCount == 1) &&
                  (HookInRcMsg.RcPpmFrameLength == 100) &&
                  (HookInRcMsg.RcFailsafe == TRUE) &&
                  (HookInRcMsg.RcLost == FALSE) &&
                  (hookmsg_checksum == checksum),
                  "ReadDevice, 1Msg_Nominal: InputRc Data");

    UtAssert_EventSent(RCIN_PUBLISHING_INF_EID, CFE_EVS_INFORMATION,
                       expEvent, "ReadDevice, 1Msg_Nominal: Event Sent");
}


/**
 * Test RCIN ReadDevice(), 2Msg_Nominal
 */
void Test_RCIN_ReadDevice_2Msg_Nominal(void)
{
    int     i;
    uint32  total_error_count;
    uint32  hookmsg_checksum;
    uint32  checksum;
    char    expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_RCIN_Custom_SetReturnCode(UT_RCIN_CUSTOM_SEDLIB_GETPIPE_INDEX,
                                 SEDLIB_OK, 1);

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_TASKDELAY_INDEX,
                             (void *)&OS_TaskDelayHook);

    SEDLIB_ReadMsg_Cnt = 1;
    Ut_RCIN_Custom_SetFunctionHook(UT_RCIN_CUSTOM_SEDLIB_READMSG_INDEX,
                                   (void*)&SEDLIB_ReadMsgHook_2Msg_Nominal);

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
                                     (void*)&CFE_PSP_GetTimeHook);

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void *)&CFE_SB_TimeStampMsgHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void *)&CFE_TIME_GetTimeHook);

    InputRcMsgHook_CalledCnt = 0;
    InputRcMsgHook_MsgId = 0;
    memset(&HookInRcMsg, 0x00, sizeof(HookInRcMsg));
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                              (void*)&Test_RCIN_InputRcMsgHook);

    /* Execute the function being tested */
    oRCIN.InitApp();

    total_error_count = 0;
    if (RCIN_Util_Stream_Emulator() == RCIN_ERROR)
    {
        total_error_count ++;
    }

    oRCIN.ReadDevice();
    oRCIN.SendInputRcMsg();

    RCIN_CleanupCallback();

    hookmsg_checksum = 0;
    for (i = 0; i < TEST_RCIN_BUS_CHANNEL_CNT; i++)
    {
        hookmsg_checksum += HookInRcMsg.Values[i];
    }

    checksum = GetChecksum_2Msg_Nominal();

    sprintf(expEvent, "%s", "RCIN is publishing fresh data");

    /* Verify results */
    UtAssert_True(InputRcMsgHook_MsgId == PX4_INPUT_RC_MID,
                  "ReadDevice, 2Msg_Nominal: Sent PX4_INPUT_RC_MID");

    UtAssert_True(total_error_count == 0,
                  "ReadDevice, 2Msg_Nominal: total_error_count");

    UtAssert_True(oRCIN.StrikeCount == 0,
                  "ReadDevice, 2Msg_Nominal: StrikeCount");

    UtAssert_True(InputRcMsgHook_CalledCnt == 1,
                  "ReadDevice, 2Msg_Nominal: InputRcMsgHook_CalledCnt");

    UtAssert_True((HookInRcMsg.ChannelCount == TEST_RCIN_BUS_CHANNEL_CNT) &&
                  (HookInRcMsg.RSSI == 100) &&
                  (HookInRcMsg.RcLostFrameCount == 0) &&
                  (HookInRcMsg.RcTotalFrameCount == 1) &&
                  (HookInRcMsg.RcPpmFrameLength == 100) &&
                  (HookInRcMsg.RcFailsafe == FALSE) &&
                  (HookInRcMsg.RcLost == FALSE) &&
                  (hookmsg_checksum == checksum),
                  "ReadDevice, 2Msg_Nominal: InputRc Data");

    UtAssert_EventSent(RCIN_PUBLISHING_INF_EID, CFE_EVS_INFORMATION,
                       expEvent, "ReadDevice, 2Msg_Nominal: Event Sent");
}


/**
 * Test RCIN ReadDevice(), 2Msg_RcLost
 */
void Test_RCIN_ReadDevice_2Msg_RcLost(void)
{
    int     i;
    uint32  total_error_count;
    uint32  hookmsg_checksum;
    uint32  checksum;

    Ut_RCIN_Custom_SetReturnCode(UT_RCIN_CUSTOM_SEDLIB_GETPIPE_INDEX,
                                 SEDLIB_OK, 1);

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_TASKDELAY_INDEX,
                             (void *)&OS_TaskDelayHook);

    SEDLIB_ReadMsg_Cnt = 1;
    Ut_RCIN_Custom_SetFunctionHook(UT_RCIN_CUSTOM_SEDLIB_READMSG_INDEX,
                                   (void*)&SEDLIB_ReadMsgHook_2Msg_RcLost);

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
                                     (void*)&CFE_PSP_GetTimeHook);

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void *)&CFE_SB_TimeStampMsgHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void *)&CFE_TIME_GetTimeHook);

    InputRcMsgHook_CalledCnt = 0;
    InputRcMsgHook_MsgId = 0;
    memset(&HookInRcMsg, 0x00, sizeof(HookInRcMsg));
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                              (void*)&Test_RCIN_InputRcMsgHook);

    /* Execute the function being tested */
    oRCIN.InitApp();

    total_error_count = 0;
    if (RCIN_Util_Stream_Emulator() == RCIN_ERROR)
    {
        total_error_count ++;
    }

    oRCIN.ReadDevice();
    oRCIN.SendInputRcMsg();

    RCIN_CleanupCallback();

    hookmsg_checksum = 0;
    for (i = 0; i < TEST_RCIN_BUS_CHANNEL_CNT; i++)
    {
        hookmsg_checksum += HookInRcMsg.Values[i];
    }

    checksum = GetChecksum_2Msg_RcLost();

    /* Verify results */
    UtAssert_True(InputRcMsgHook_MsgId == PX4_INPUT_RC_MID,
                  "ReadDevice, 2Msg_RcLost: Sent PX4_INPUT_RC_MID");

    UtAssert_True(total_error_count == 1,
                  "ReadDevice, 2Msg_RcLost: total_error_count");

    UtAssert_True(oRCIN.StrikeCount == 1,
                  "ReadDevice, 2Msg_RcLost: StrikeCount");

    UtAssert_True(InputRcMsgHook_CalledCnt == 1,
                  "ReadDevice, 2Msg_RcLost: InputRcMsgHook_CalledCnt");

    UtAssert_True((HookInRcMsg.ChannelCount == TEST_RCIN_BUS_CHANNEL_CNT) &&
                  (HookInRcMsg.RSSI == 100) &&
                  (HookInRcMsg.RcLostFrameCount == 0) &&
                  (HookInRcMsg.RcTotalFrameCount == 1) &&
                  (HookInRcMsg.RcPpmFrameLength == 100) &&
                  (HookInRcMsg.RcFailsafe == TRUE) &&
                  (HookInRcMsg.RcLost == TRUE) &&
                  (hookmsg_checksum == checksum),
                  "ReadDevice, 2Msg_RcLost: InputRc Data");
}


/**
 * Test RCIN ReadDevice(), 2Msg_1NoFooter
 */
void Test_RCIN_ReadDevice_2Msg_1NoFooter(void)
{
    uint32  total_error_count;

    Ut_RCIN_Custom_SetReturnCode(UT_RCIN_CUSTOM_SEDLIB_GETPIPE_INDEX,
                                 SEDLIB_OK, 1);

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_TASKDELAY_INDEX,
                             (void *)&OS_TaskDelayHook);

    SEDLIB_ReadMsg_Cnt = 1;
    Ut_RCIN_Custom_SetFunctionHook(UT_RCIN_CUSTOM_SEDLIB_READMSG_INDEX,
                                  (void*)&SEDLIB_ReadMsgHook_2Msg_1NoFooter);

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
                                     (void*)&CFE_PSP_GetTimeHook);

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void *)&CFE_SB_TimeStampMsgHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void *)&CFE_TIME_GetTimeHook);

    InputRcMsgHook_CalledCnt = 0;
    InputRcMsgHook_MsgId = 0;
    memset(&HookInRcMsg, 0x00, sizeof(HookInRcMsg));
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                              (void*)&Test_RCIN_InputRcMsgHook);

    /* Execute the function being tested */
    oRCIN.InitApp();

    total_error_count = 0;
    if (RCIN_Util_Stream_Emulator() == RCIN_ERROR)
    {
        total_error_count ++;
    }

    oRCIN.ReadDevice();
    oRCIN.SendInputRcMsg();

    RCIN_CleanupCallback();

    /* Verify results */
    UtAssert_True(InputRcMsgHook_MsgId == PX4_INPUT_RC_MID,
                  "ReadDevice, 2Msg_1NoFooter: Sent PX4_INPUT_RC_MID");

    UtAssert_True(total_error_count == 1,
                  "ReadDevice, 2Msg_1NoFooter: total_error_count");

    UtAssert_True(oRCIN.StrikeCount == 1,
                  "ReadDevice, 2Msg_1NoFooter: StrikeCount");

    UtAssert_True(InputRcMsgHook_CalledCnt == 1,
                  "ReadDevice, 2Msg_1NoFooter: InputRcMsgHook_CalledCnt");

    UtAssert_True((HookInRcMsg.RcFailsafe == TRUE) &&
                  (HookInRcMsg.RcLost == TRUE),
             "ReadDevice, 2Msg_1NoFooter: InputRc RcFailsafe/RcLost Data");
}


/**
 * Test RCIN ReadDevice(), 2Msg_1NoHdr
 */
void Test_RCIN_ReadDevice_2Msg_1NoHdr(void)
{
    uint32  total_error_count;

    Ut_RCIN_Custom_SetReturnCode(UT_RCIN_CUSTOM_SEDLIB_GETPIPE_INDEX,
                                 SEDLIB_OK, 1);

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_TASKDELAY_INDEX,
                             (void *)&OS_TaskDelayHook);

    SEDLIB_ReadMsg_Cnt = 1;
    Ut_RCIN_Custom_SetFunctionHook(UT_RCIN_CUSTOM_SEDLIB_READMSG_INDEX,
                                   (void*)&SEDLIB_ReadMsgHook_2Msg_1NoHdr);

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
                                     (void*)&CFE_PSP_GetTimeHook);

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void *)&CFE_SB_TimeStampMsgHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void *)&CFE_TIME_GetTimeHook);

    InputRcMsgHook_CalledCnt = 0;
    InputRcMsgHook_MsgId = 0;
    memset(&HookInRcMsg, 0x00, sizeof(HookInRcMsg));
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                              (void*)&Test_RCIN_InputRcMsgHook);

    /* Execute the function being tested */
    oRCIN.InitApp();

    total_error_count = 0;
    if (RCIN_Util_Stream_Emulator() == RCIN_ERROR)
    {
        total_error_count ++;
    }

    oRCIN.ReadDevice();
    oRCIN.SendInputRcMsg();

    RCIN_CleanupCallback();

    /* Verify results */
    UtAssert_True(InputRcMsgHook_MsgId == PX4_INPUT_RC_MID,
                  "ReadDevice, 2Msg_1NoHdr: Sent PX4_INPUT_RC_MID");

    /* This depends on the previous streaming status */
    UtAssert_True(total_error_count == 1,
                  "ReadDevice, 2Msg_1NoHdr: total_error_count");

    /* This depends on the previous streaming status */
    UtAssert_True(oRCIN.StrikeCount == 1,
                  "ReadDevice, 2Msg_1NoHdr: StrikeCount");

    UtAssert_True(InputRcMsgHook_CalledCnt == 1,
                  "ReadDevice, 2Msg_1NoHdr: InputRcMsgHook_CalledCnt");

    /* This depends on the previous streaming status */
    UtAssert_True((HookInRcMsg.RcFailsafe == TRUE) &&
                  (HookInRcMsg.RcLost == TRUE),
                  "ReadDevice, 2Msg_1NoHdr: InputRc RcFailsafe/RcLost Data");
}


/**
 * Test RCIN ReadDevice(), 10Msg_1NoHdr1NoFooter
 */
void Test_RCIN_ReadDevice_10Msg_1NoHdr1NoFooter(void)
{
    uint32  total_error_count;
    char    expEventSync[CFE_EVS_MAX_MESSAGE_LENGTH];
    char    expEventOutSync[CFE_EVS_MAX_MESSAGE_LENGTH];
    char    expEventPublish[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_RCIN_Custom_SetReturnCode(UT_RCIN_CUSTOM_SEDLIB_GETPIPE_INDEX,
                                 SEDLIB_OK, 1);

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_TASKDELAY_INDEX,
                             (void *)&OS_TaskDelayHook);

    SEDLIB_ReadMsg_Cnt = 1;
    Ut_RCIN_Custom_SetFunctionHook(UT_RCIN_CUSTOM_SEDLIB_READMSG_INDEX,
                         (void*)&SEDLIB_ReadMsgHook_10Msg_1NoHdr1NoFooter);

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
                                     (void*)&CFE_PSP_GetTimeHook);

    /* To give the unit test system time for SB Msg */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void *)&CFE_SB_TimeStampMsgHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void *)&CFE_TIME_GetTimeHook);

    InputRcMsgHook_CalledCnt = 0;
    InputRcMsgHook_MsgId = 0;
    memset(&HookInRcMsg, 0x00, sizeof(HookInRcMsg));
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                              (void*)&Test_RCIN_InputRcMsgHook);

    /* Execute the function being tested */
    oRCIN.InitApp();

    total_error_count = 0;
    if (RCIN_Util_Stream_Emulator() == RCIN_ERROR)
    {
        total_error_count ++;
    }

    oRCIN.ReadDevice();
    oRCIN.SendInputRcMsg();

    RCIN_CleanupCallback();

    sprintf(expEventSync, "%s", "RCIN in sync.");
    sprintf(expEventOutSync, "%s", "RCIN out of sync.");
    sprintf(expEventPublish, "%s", "RCIN is publishing fresh data");

    /* Verify results */
    UtAssert_True(InputRcMsgHook_MsgId == PX4_INPUT_RC_MID,
               "ReadDevice, 10Msg_1NoHdr1NoFooter: Sent PX4_INPUT_RC_MID");

    UtAssert_True(total_error_count == 0,
               "ReadDevice, 10Msg_1NoHdr1NoFooter: total_error_count");

    UtAssert_True(oRCIN.StrikeCount == 0,
               "ReadDevice, 10Msg_1NoHdr1NoFooter: StrikeCount");

    UtAssert_EventSent(TEST_RCIN_IN_SYNC_INF_EID, CFE_EVS_INFORMATION,
                  expEventSync,
                  "ReadDevice, 10Msg_1NoHdr1NoFooter: In Sync Event Sent");

    UtAssert_EventSent(TEST_RCIN_OUT_SYNC_ERR_EID, CFE_EVS_ERROR,
                  expEventOutSync,
                  "ReadDevice, 10Msg_1NoHdr1NoFooter: Out Sync Event Sent");

    UtAssert_EventSent(RCIN_PUBLISHING_INF_EID, CFE_EVS_INFORMATION,
          expEventPublish,
          "ReadDevice, 10Msg_1NoHdr1NoFooter: Fresh Publishing Event Sent");
}


/**
 * Test RCIN ReadDevice(), Multiple_1NoHdr1NoFooter
 */
void Test_RCIN_ReadDevice_Multiple_1NoHdr1NoFooter(void)
{
    int     i;
    uint32  emulator_count;
    uint32  total_error_count;
    char    expEventSync[CFE_EVS_MAX_MESSAGE_LENGTH];
    char    expEventOutSync[CFE_EVS_MAX_MESSAGE_LENGTH];
    char    expEventPublish[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_RCIN_Custom_SetReturnCode(UT_RCIN_CUSTOM_SEDLIB_GETPIPE_INDEX,
                                 SEDLIB_OK, 1);

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_TASKDELAY_INDEX,
                             (void *)&OS_TaskDelayHook);

    SEDLIB_ReadMsg_Cnt = 1;
    Ut_RCIN_Custom_SetFunctionHook(UT_RCIN_CUSTOM_SEDLIB_READMSG_INDEX,
                      (void*)&SEDLIB_ReadMsgHook_Multiple_1NoHdr1NoFooter);

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
                                     (void*)&CFE_PSP_GetTimeHook);

    /* To give the unit test system time for SB Msg */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void *)&CFE_SB_TimeStampMsgHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void *)&CFE_TIME_GetTimeHook);

    InputRcMsgHook_CalledCnt = 0;
    InputRcMsgHook_MsgId = 0;
    memset(&HookInRcMsg, 0x00, sizeof(HookInRcMsg));
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                              (void*)&Test_RCIN_InputRcMsgHook);

    /* Execute the function being tested */
    oRCIN.InitApp();

    emulator_count = 30;
    total_error_count = 0;
    for (i = 0; i < emulator_count; i++)
    {
        SEDLIB_ReadMsg_Cnt = 1;
        if (RCIN_Util_Stream_Emulator() == RCIN_ERROR)
        {
            total_error_count ++;
        }

        oRCIN.ReadDevice();
        oRCIN.SendInputRcMsg();
    }

    RCIN_CleanupCallback();

    sprintf(expEventSync, "%s", "RCIN in sync.");
    sprintf(expEventOutSync, "%s", "RCIN out of sync.");
    sprintf(expEventPublish, "%s", "RCIN is publishing fresh data");

    /* Verify results */
    UtAssert_True(InputRcMsgHook_MsgId == PX4_INPUT_RC_MID,
             "ReadDevice, Multiple_1NoHdr1NoFooter: Sent PX4_INPUT_RC_MID");

    UtAssert_True(total_error_count == 11,
                  "ReadDevice, Multiple_1NoHdr1NoFooter: total_error_count");

    UtAssert_True(oRCIN.StrikeCount == 0,
                  "ReadDevice, Multiple_1NoHdr1NoFooter: StrikeCount");

    UtAssert_True(InputRcMsgHook_CalledCnt == emulator_count,
          "ReadDevice, Multiple_1NoHdr1NoFooter: InputRcMsgHook_CalledCnt");

    UtAssert_EventSent(TEST_RCIN_IN_SYNC_INF_EID, CFE_EVS_INFORMATION,
                expEventSync,
                "ReadDevice, Multiple_1NoHdr1NoFooter: In Sync Event Sent");

    UtAssert_EventSent(TEST_RCIN_OUT_SYNC_ERR_EID, CFE_EVS_ERROR,
                expEventOutSync,
                "ReadDevice, Multiple_1NoHdr1NoFooter: Out Sync Event Sent");

    UtAssert_EventSent(RCIN_PUBLISHING_INF_EID, CFE_EVS_INFORMATION,
        expEventPublish,
        "ReadDevice, Multiple_1NoHdr1NoFooter: Fresh Publishing Event Sent");
}


/**
 * Test RCIN ReadDevice(), Multiple_25NoHdr
 */
void Test_RCIN_ReadDevice_Multiple_25NoHdr(void)
{
    int     i;
    uint32  emulator_count;
    uint32  total_error_count;
    char    expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_RCIN_Custom_SetReturnCode(UT_RCIN_CUSTOM_SEDLIB_GETPIPE_INDEX,
                                 SEDLIB_OK, 1);

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_TASKDELAY_INDEX,
                             (void *)&OS_TaskDelayHook);

    SEDLIB_ReadMsg_Cnt = 1;
    Ut_RCIN_Custom_SetFunctionHook(UT_RCIN_CUSTOM_SEDLIB_READMSG_INDEX,
                              (void*)&SEDLIB_ReadMsgHook_Multiple_25NoHdr);

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
                                     (void*)&CFE_PSP_GetTimeHook);

    /* To give the unit test system time for SB Msg */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void *)&CFE_SB_TimeStampMsgHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void *)&CFE_TIME_GetTimeHook);

    InputRcMsgHook_CalledCnt = 0;
    InputRcMsgHook_MsgId = 0;
    memset(&HookInRcMsg, 0x00, sizeof(HookInRcMsg));
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                              (void*)&Test_RCIN_InputRcMsgHook);

    /* Execute the function being tested */
    oRCIN.InitApp();

    emulator_count = 30;
    total_error_count = 0;
    for (i = 0; i < emulator_count; i++)
    {
        SEDLIB_ReadMsg_Cnt = 1;
        if (RCIN_Util_Stream_Emulator() == RCIN_ERROR)
        {
            total_error_count ++;
        }

        oRCIN.ReadDevice();
        oRCIN.SendInputRcMsg();
    }

    RCIN_CleanupCallback();

    printf("Test_RCIN_ReadDevice_Multiple_25NoHdr: total_error_count(%lu)\n",
            total_error_count);

    sprintf(expEvent, "%s", "RCIN is NOT publishing fresh data");

    /* Verify results */
    UtAssert_True(InputRcMsgHook_MsgId == PX4_INPUT_RC_MID,
             "ReadDevice, Multiple_Multiple_25NoHdr: Sent PX4_INPUT_RC_MID");

    UtAssert_True(InputRcMsgHook_CalledCnt == emulator_count,
                  "ReadDevice, Multiple_25NoHdr: InputRcMsgHook_CalledCnt");

    UtAssert_EventSent(RCIN_NOT_PUBLISHING_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "ReadDevice, Multiple_25NoHdr: Not publishing fresh "
                       "data Event Sent");
}


/**
 * Test RCIN ReadDevice(), Multiple_10Bytes
 */
void Test_RCIN_ReadDevice_Multiple_10Bytes(void)
{
    int     i;
    uint32  emulator_count;
    uint32  total_error_count;

    Ut_RCIN_Custom_SetReturnCode(UT_RCIN_CUSTOM_SEDLIB_GETPIPE_INDEX,
                                 SEDLIB_OK, 1);

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_TASKDELAY_INDEX,
                             (void *)&OS_TaskDelayHook);

    SEDLIB_ReadMsg_Cnt = 1;
    Ut_RCIN_Custom_SetFunctionHook(UT_RCIN_CUSTOM_SEDLIB_READMSG_INDEX,
                      (void*)&SEDLIB_ReadMsgHook_Multiple_10Bytes);

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
                                     (void*)&CFE_PSP_GetTimeHook);

    /* To give the unit test system time for SB Msg */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void *)&CFE_SB_TimeStampMsgHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void *)&CFE_TIME_GetTimeHook);

    InputRcMsgHook_CalledCnt = 0;
    InputRcMsgHook_MsgId = 0;
    memset(&HookInRcMsg, 0x00, sizeof(HookInRcMsg));
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                              (void*)&Test_RCIN_InputRcMsgHook);

    /* Execute the function being tested */
    oRCIN.InitApp();

    emulator_count = 30;
    total_error_count = 0;
    for (i = 0; i < emulator_count; i++)
    {
        SEDLIB_ReadMsg_Cnt = 1;
        if (RCIN_Util_Stream_Emulator() == RCIN_ERROR)
        {
            total_error_count ++;
        }

        oRCIN.ReadDevice();
        oRCIN.SendInputRcMsg();
    }

    RCIN_CleanupCallback();

    /* Verify results */
    UtAssert_True(InputRcMsgHook_MsgId == PX4_INPUT_RC_MID,
             "ReadDevice, Multiple_10Bytes: Sent PX4_INPUT_RC_MID");
}


/**
 * Test RCIN ReadDevice(), NoData
 */
void Test_RCIN_ReadDevice_NoData(void)
{
    int   i;
    int   emulator_count;
    char  expEventLost[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_RCIN_Custom_SetReturnCode(UT_RCIN_CUSTOM_SEDLIB_GETPIPE_INDEX,
                                 SEDLIB_OK, 1);

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_TASKDELAY_INDEX,
                             (void *)&OS_TaskDelayHook);

    SEDLIB_ReadMsg_Cnt = 1;
    Ut_RCIN_Custom_SetFunctionHook(UT_RCIN_CUSTOM_SEDLIB_READMSG_INDEX,
                                   (void*)&SEDLIB_ReadMsgHook_NoData);

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
                                     (void*)&CFE_PSP_GetTimeHook);

    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void *)&CFE_SB_TimeStampMsgHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void *)&CFE_TIME_GetTimeHook);

    InputRcMsgHook_CalledCnt = 0;
    InputRcMsgHook_MsgId = 0;
    memset(&HookInRcMsg, 0x00, sizeof(HookInRcMsg));
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                              (void*)&Test_RCIN_InputRcMsgHook);

    /* Execute the function being tested */
    oRCIN.InitApp();

    emulator_count = TEST_RCIN_MAX_ERROR_CNT + 1;
    for (i = 0; i < emulator_count; i++)
    {
        SEDLIB_ReadMsg_Cnt = 1;
        RCIN_Util_Stream_Emulator();

        oRCIN.ReadDevice();
        oRCIN.SendInputRcMsg();
    }

    RCIN_CleanupCallback();

    sprintf(expEventLost, "%s",
            "RCIN RC LOST, setting failsafe and lost flags");

    sprintf(expEvent, "%s", "RCIN is NOT publishing fresh data");

    /* Verify results */
    UtAssert_True(InputRcMsgHook_MsgId == PX4_INPUT_RC_MID,
                  "ReadDevice, NoData: Sent PX4_INPUT_RC_MID");

    UtAssert_True(oRCIN.StrikeCount == emulator_count,
                  "ReadDevice, NoData: StrikeCount");

    UtAssert_True((HookInRcMsg.RcFailsafe == TRUE) &&
                  (HookInRcMsg.RcLost == TRUE),
                  "ReadDevice, NoData: InputRc RcFailsafe/RcLost Data");

    UtAssert_EventSent(TEST_RCIN_DEV_ERR_EID, CFE_EVS_ERROR, expEventLost,
                   "ReadDevice, NoData: RcFailsafe/RcLost flag Event Sent");

    UtAssert_EventSent(RCIN_NOT_PUBLISHING_ERR_EID, CFE_EVS_ERROR, expEvent,
                     "ReadDevice, NoData: Not publishing fresh Event Sent");
}



/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void RCIN_App_Test_AddTestCases(void)
{
    UtTest_Add(Test_RCIN_InitEvent_Fail_Register,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_InitEvent_Fail_Register");

    UtTest_Add(Test_RCIN_InitPipe_Fail_CreateSCHPipe,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_InitPipe_Fail_CreateSCHPipe");
    UtTest_Add(Test_RCIN_InitPipe_Fail_SubscribeWakeup,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_InitPipe_Fail_SubscribeWakeup");
    UtTest_Add(Test_RCIN_InitPipe_Fail_SubscribeSendHK,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_InitPipe_Fail_SubscribeSendHK");
    UtTest_Add(Test_RCIN_InitPipe_Fail_CreateCMDPipe,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_InitPipe_Fail_CreateCMDPipe");
    UtTest_Add(Test_RCIN_InitPipe_Fail_SubscribeCMD,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_InitPipe_Fail_SubscribeCMD");

    UtTest_Add(Test_RCIN_InitData,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_InitData");

    UtTest_Add(Test_RCIN_InitApp_Fail_InitEvent,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_InitApp_Fail_InitEvent");
    UtTest_Add(Test_RCIN_InitApp_Fail_InitPipe,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_InitApp_Fail_InitPipe");
    UtTest_Add(Test_RCIN_InitApp_Fail_InitData,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_InitApp_Fail_InitData");
    UtTest_Add(Test_RCIN_InitApp_Fail_Custom_Init,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_InitApp_Fail_Custom_Init");
    UtTest_Add(Test_RCIN_InitApp_Fail_OS_TaskInstallDeleteHandler,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_InitApp_Fail_OS_TaskInstallDeleteHandler");
    UtTest_Add(Test_RCIN_InitApp_Nominal,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_InitApp_Nominal");

    UtTest_Add(Test_Extern_RCIN_AppMain,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_Extern_RCIN_AppMain");

    UtTest_Add(Test_RCIN_AppMain_Fail_RegisterApp,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_AppMain_Fail_RegisterApp");
    UtTest_Add(Test_RCIN_AppMain_Fail_InitApp,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_AppMain_Fail_InitApp");
    UtTest_Add(Test_RCIN_AppMain_InvalidSchMessage,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_AppMain_InvalidSchMessage");
    UtTest_Add(Test_RCIN_AppMain_NoSchMessage,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_AppMain_NoSchMessage");
    UtTest_Add(Test_RCIN_AppMain_SB_Timeout,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_AppMain_SB_Timeout");
    UtTest_Add(Test_RCIN_AppMain_SchPipeError,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_AppMain_SchPipeError");
    UtTest_Add(Test_RCIN_AppMain_Nominal_SendHK,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_AppMain_Nominal_SendHK");
    UtTest_Add(Test_RCIN_AppMain_Nominal_Wakeup,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_AppMain_Nominal_Wakeup");

    UtTest_Add(Test_RCIN_ReadDevice_1Msg_Nominal,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_ReadDevice_1Msg_Nominal");
    UtTest_Add(Test_RCIN_ReadDevice_2Msg_Nominal,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_ReadDevice_2Msg_Nominal");
    UtTest_Add(Test_RCIN_ReadDevice_2Msg_RcLost,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_ReadDevice_2Msg_RcLost");
    UtTest_Add(Test_RCIN_ReadDevice_2Msg_1NoFooter,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_ReadDevice_2Msg_1NoFooter");
    UtTest_Add(Test_RCIN_ReadDevice_2Msg_1NoHdr,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_ReadDevice_2Msg_1NoHdr");
    UtTest_Add(Test_RCIN_ReadDevice_10Msg_1NoHdr1NoFooter,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_ReadDevice_10Msg_1NoHdr1NoFooter");
    UtTest_Add(Test_RCIN_ReadDevice_Multiple_1NoHdr1NoFooter,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_ReadDevice_Multiple_1NoHdr1NoFooter");
    UtTest_Add(Test_RCIN_ReadDevice_Multiple_25NoHdr,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_ReadDevice_Multiple_25NoHdr");
    UtTest_Add(Test_RCIN_ReadDevice_Multiple_10Bytes,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_ReadDevice_Multiple_10Bytes");
    UtTest_Add(Test_RCIN_ReadDevice_NoData,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_ReadDevice_NoData");
}
