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
CFE_SB_MsgId_t Wakeup_SendMsgHook_MsgId = 0;


/**
 * Test RCIN GetPSPTimeHook
 */
void Test_RCIN_GetPSPTimeHook(OS_time_t *LocalTime)
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
 * Tests for RCIN_InitEvent()
 **************************************************************************/
/**
 * Test RCIN_InitEvent() with failed CFE_EVS_Register
 */
void Test_RCIN_InitEvent_Fail_Register(void)
{
    RCIN oRCIN;

    /* Set a fail result for EVS */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EVENTS_SERVICE | CFE_EVS_NOT_IMPLEMENTED;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;
    char  expSysLog[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oRCIN.InitEvent();

    sprintf(expSysLog, "RCIN - Failed to register with EVS (0x%08lX)\n",
                       (long unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected, "InitEvent, failed EVS Register");

    UtAssert_True(Ut_CFE_ES_SysLogWritten(expSysLog),
                  "InitEvent, failed EVS Register SysLog Written");
}


/**************************************************************************
 * Tests for RCIN_InitPipe()
 **************************************************************************/
/**
 * Test RCIN_InitPipe(), fail SCH CFE_SB_CreatePipe
 */
void Test_RCIN_InitPipe_Fail_CreateSCHPipe(void)
{
    RCIN oRCIN;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oRCIN.InitPipe();

    sprintf(expEvent, "Failed to create SCH pipe (0x%08lX)",
                      (long unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected, "InitPipe, fail SB create SCH pipe");

    UtAssert_EventSent(RCIN_PIPE_INIT_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "InitPipe, fail SB create SCH pipe Event Sent");
}


/**
 * Test RCIN_InitPipe(), fail CFE_SB_SubscribeEx for wakeup
 */
void Test_RCIN_InitPipe_Fail_SubscribeWakeup(void)
{
    RCIN oRCIN;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oRCIN.InitPipe();

    sprintf(expEvent,
            "Sch Pipe failed to subscribe to RCIN_WAKEUP_MID. (0x%08lX)",
            (long unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected,
                  "InitPipe, fail CFE_SB_SubscribeEx for wakeup");

    UtAssert_EventSent(RCIN_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR, expEvent,
                "InitPipe, fail CFE_SB_SubscribeEx for wakeup Event Sent");
}


/**
 * Test RCIN_InitPipe(), fail CFE_SB_SubscribeEx for sendhk
 */
void Test_RCIN_InitPipe_Fail_SubscribeSendHK(void)
{
    RCIN oRCIN;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oRCIN.InitPipe();

    sprintf(expEvent,
            "Sch Pipe failed to subscribe to RCIN_SEND_HK_MID. (0x%08X)",
            (unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected,
                  "InitPipe, fail CFE_SB_SubscribeEx for sendhk");

    UtAssert_EventSent(RCIN_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR, expEvent,
                "InitPipe, fail CFE_SB_SubscribeEx for sendhk Event Sent");
}


/**
 * Test RCIN_InitPipe(), fail CMD CFE_SB_CreatePipe
 */
void Test_RCIN_InitPipe_Fail_CreateCMDPipe(void)
{
    RCIN oRCIN;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = oRCIN.InitPipe();

    sprintf(expEvent, "Failed to create CMD pipe (0x%08lX)",
                      (long unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected, "InitPipe, fail SB create CMD pipe");

    UtAssert_EventSent(RCIN_PIPE_INIT_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "InitPipe, fail SB create CMD pipe Event Sent");
}


/**
 * Test RCIN_InitPipe(), fail CFE_SB_Subscribe for CMD msg
 */
void Test_RCIN_InitPipe_Fail_SubscribeCMD(void)
{
    RCIN oRCIN;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oRCIN.InitPipe();

    sprintf(expEvent,
            "CMD Pipe failed to subscribe to RCIN_CMD_MID. (0x%08lX)",
            (long unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected,
                  "InitPipe, fail CFE_SB_Subscribe for CMD");

    UtAssert_EventSent(RCIN_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR, expEvent,
                     "InitPipe, fail CFE_SB_Subscribe for CMD Event Sent");
}


/**************************************************************************
 * Tests for RCIN_InitData()
 **************************************************************************/
/**
 * Test RCIN_InitData()
 */
void Test_RCIN_InitData(void)
{
    RCIN oRCIN;

    /* Set a fail result */
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    oRCIN.InitData();

    /* Verify results */
    //UtAssert_True (result == expected, "InitData");
}


/**************************************************************************
 * Tests for RCIN_InitApp()
 **************************************************************************/
/**
 * Test RCIN_InitApp(), fail init event
 */
void Test_RCIN_InitApp_Fail_InitEvent(void)
{
    RCIN oRCIN;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;
    char  expSysLog[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oRCIN.InitApp();

    sprintf(expSysLog, "RCIN - Failed to init events (0x%08lX)\n",
                       (long unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected, "InitApp, fail init event");

    UtAssert_True(Ut_CFE_ES_GetSysLogQueueDepth() == 3,
                  "InitApp, fail init event Sys Log QueueDepth");

    UtAssert_True(Ut_CFE_ES_SysLogWritten(expSysLog),
                  "InitApp, fail init event Sys Log Written");
}


/**
 * Test RCIN_InitApp(), fail init pipe
 */
void Test_RCIN_InitApp_Fail_InitPipe(void)
{
    RCIN oRCIN;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oRCIN.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init pipe");
}


/**
 * Test RCIN_InitApp(), fail init data.
 * NOTE: no current way to fail RCIN_InitData() in default
 */
void Test_RCIN_InitApp_Fail_InitData(void)
{
    RCIN oRCIN;

    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oRCIN.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init data");
}


/**
 * Test RCIN_InitApp(), Nominal
 */
void Test_RCIN_InitApp_Nominal(void)
{
    RCIN oRCIN;

    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oRCIN.InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, nominal");
}


/**************************************************************************
 * Tests for RCIN_AppMain()
 **************************************************************************/
/**
 * Test RCIN_AppMain(), Fail RegisterApp
 */
void Test_RCIN_AppMain_Fail_RegisterApp(void)
{
    RCIN oRCIN;

    int32 expected = CFE_ES_ERR_APP_REGISTER;
    char  expSysLog[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* fail the register app */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX, expected, 1);

    /* Execute the function being tested */
    oRCIN.AppMain();

    sprintf(expSysLog, "RCIN - Failed to register the app (0x%08lX)\n",
                       (long unsigned int)expected);

    /* Verify results */
    UtAssert_True(Ut_CFE_ES_SysLogWritten(expSysLog),
                  "RCIN_AppMain, Fail RegisterApp SysLog Written");
}


/**
 * Test RCIN_AppMain(), Fail InitApp
 */
void Test_RCIN_AppMain_Fail_InitApp(void)
{
    RCIN oRCIN;

    int32 expected = CFE_EVS_APP_NOT_REGISTERED;
    char  expSysLog[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* fail the register app */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    oRCIN.AppMain();

    sprintf(expSysLog, "RCIN - Application failed to initialize\n");

    /* Verify results */
    UtAssert_True(Ut_CFE_ES_GetSysLogQueueDepth() == 3,
                  "RCIN_AppMain, Fail InitApp SysLog Queue Depth");

    UtAssert_True(Ut_CFE_ES_SysLogWritten(expSysLog),
                  "RCIN_AppMain, Fail InitApp SysLog Written");
}


/**
 * Test RCIN_AppMain(), Invalid Schedule Message
 */
void Test_RCIN_AppMain_InvalidSchMessage(void)
{
    RCIN oRCIN;

    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will the emulate behavior of receiving a SCH message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, 0, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oRCIN.AppMain();

    sprintf(expEvent, "Recvd invalid SCH msgId (0x%04X)", 0);

    /* Verify results */
    UtAssert_EventSent(RCIN_MSGID_ERR_EID, CFE_EVS_ERROR, expEvent,
                  "RCIN_AppMain, Invalid Schedule Message Event Sent");
}


/**
 * Test RCIN_AppMain(), NoSchMessage
 */
void Test_RCIN_AppMain_NoSchMessage(void)
{
    RCIN oRCIN;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_NO_MESSAGE, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oRCIN.AppMain();
}


/**
 * Test RCIN_AppMain(), SB_Timeout
 */
void Test_RCIN_AppMain_SB_Timeout(void)
{
    RCIN oRCIN;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_TIME_OUT, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oRCIN.AppMain();
}


/**
 * Test RCIN_AppMain(), SchPipeError
 */
void Test_RCIN_AppMain_SchPipeError(void)
{
    RCIN oRCIN;

    char expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_PIPE_RD_ERR, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oRCIN.AppMain();

    sprintf(expEvent, "SCH pipe read error (0x%08lX).",
                      (long unsigned int)CFE_SB_PIPE_RD_ERR);

    /* Verify results */
    UtAssert_EventSent(RCIN_RCVMSG_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "RCIN_AppMain, SchPipeError Event Sent");
}


/**
 * Hook to support: RCIN_AppMain(), Nominal - SendHK
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
    RCIN_HkTlm_t       HkMsg;

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
            CFE_PSP_MemCpy((void*)&HkMsg, (void*)MsgPtr, sizeof(HkMsg));

            printf("Sent RCIN_HK_TLM_MID:\n");
            printf("usCmdCnt: %u\n", HkMsg.usCmdCnt);
            printf("usCmdErrCnt: %u\n", HkMsg.usCmdErrCnt);
            printf("State: %u\n", HkMsg.State);
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
 * Test RCIN_AppMain(), Nominal - SendHK
 */
void Test_RCIN_AppMain_Nominal_SendHK(void)
{
    RCIN oRCIN;

    /* The following will emulate the behavior of receiving a SCH message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, RCIN_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Used to verify HK was transmitted correctly. */
    SendHK_SendMsgHook_MsgId = 0;
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                     (void*)&Test_RCIN_AppMain_Nominal_SendHK_SendMsgHook);

    /* Execute the function being tested */
    oRCIN.AppMain();

    /* Verify results */
    UtAssert_True(SendHK_SendMsgHook_MsgId == RCIN_HK_TLM_MID,
                  "AppMain_Nominal_SendHK");
}


/**
 * Hook to support: RCIN_AppMain(), Nominal - Wakeup
 */
int32 Test_RCIN_AppMain_Nominal_Wakeup_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    unsigned char*     pBuff = NULL;
    uint16             msgLen = 0;
    int                i = 0;
    CFE_SB_MsgId_t     MsgId;
    time_t             localTime;
    struct tm          *loc_time;
    CFE_TIME_SysTime_t TimeFromMsg;
    PX4_InputRcMsg_t   InputRcMsg;

    pBuff = (unsigned char*)MsgPtr;

    msgLen = CFE_SB_GetTotalMsgLength(MsgPtr);
    printf("###AppMain_Wakeup_SendMsgHook: MsgLen(%u)\n", msgLen);
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
            Wakeup_SendMsgHook_MsgId = PX4_INPUT_RC_MID;
            CFE_PSP_MemCpy((void*)&InputRcMsg, (void*)MsgPtr,
                           sizeof(InputRcMsg));

            printf("Sent PX4_INPUT_RC_MID:\n");
            localTime = RCIN_Test_GetTimeFromTimestamp(InputRcMsg.Timestamp);
            loc_time = localtime(&localTime);
            printf("Timestamp: %s", asctime(loc_time));
            printf("LastSignal: %" PRIu64"\n", InputRcMsg.LastSignal);
            printf("ChannelCount: %lu\n", InputRcMsg.ChannelCount);
            printf("RSSI: %ld\n", InputRcMsg.RSSI);
            printf("RcLostFrameCount: %u\n", InputRcMsg.RcLostFrameCount);
            printf("RcTotalFrameCount: %u\n", InputRcMsg.RcTotalFrameCount);
            printf("RcPpmFrameLength: %u\n", InputRcMsg.RcPpmFrameLength);
            printf("Values: ");
            for (i = 0; i < PX4_RC_INPUT_MAX_CHANNELS; i++)
            {
                printf("%02x ", InputRcMsg.Values[i]);
            }
            printf("\n");
            printf("RcFailsafe: %u\n", InputRcMsg.RcFailsafe);
            printf("RcLost: %u\n", InputRcMsg.RcLost);
            printf("InputSource: %d\n", InputRcMsg.InputSource);
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
 * Test RCIN_AppMain(), Nominal - Wakeup
 */
void Test_RCIN_AppMain_Nominal_Wakeup(void)
{
    RCIN oRCIN;

    /* The following will emulate the behavior of receiving a SCH message
       to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, RCIN_WAKEUP_MID, 1);

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
                                     (void*)&Test_RCIN_GetPSPTimeHook);
    Wakeup_SendMsgHook_MsgId = 0;
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                     (void*)&Test_RCIN_AppMain_Nominal_Wakeup_SendMsgHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oRCIN.AppMain();

    /* Verify results */
    UtAssert_True(Wakeup_SendMsgHook_MsgId == PX4_INPUT_RC_MID,
                  "RCIN_AppMain(), Nominal - Wakeup");
}


/**
 * Test RCIN_ReadDevice()
 */
void Test_RCIN_ReadDevice(void)
{
    RCIN oRCIN;

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERCHILDTASK_INDEX,
                            CFE_SUCCESS, 1);
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_EXITCHILDTASK_INDEX, CFE_SUCCESS, 1);

    Ut_OSAPI_SetReturnCode(UT_OSAPI_MUTSEMTAKE_INDEX, CFE_SUCCESS, 1);
    Ut_OSAPI_SetReturnCode(UT_OSAPI_MUTSEMGIVE_INDEX, CFE_SUCCESS, 1);

    Ut_RCIN_Custom_SetReturnCode(UT_RCIN_CUSTOM_SEDLIB_GETPIPE_INDEX,
                                 SEDLIB_OK, 1);
    Ut_RCIN_Custom_SetFunctionHook(UT_RCIN_CUSTOM_SEDLIB_READMSG_INDEX,
                                   (void*)&SEDLIB_ReadMsgHook);

    /* Execute the function being tested */
    oRCIN.InitApp();

    RCIN_Stream_Task();

    RCIN_CleanupCallback();
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
    UtTest_Add(Test_RCIN_InitApp_Nominal,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_InitApp_Nominal");

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

    UtTest_Add(Test_RCIN_ReadDevice,
               RCIN_Test_Setup, RCIN_Test_TearDown,
               "Test_RCIN_ReadDevice");
}
