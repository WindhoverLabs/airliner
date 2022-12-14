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

#include "cf_app_test.h"
#include "cf_test_utils.h"

#include "cf_events.h"
#include "cf_version.h"

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

#include <time.h>


/**
 * Test CF GetPSPTimeHook
 */
void Test_CF_GetPSPTimeHook(OS_time_t *LocalTime)
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
 * Tests for CF_ChannelInit()
 **************************************************************************/
/**
 * Test CF_ChannelInit, fail PoolCreate
 */
void Test_CF_ChannelInit_Fail_PoolCreate(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_ES_BAD_ARGUMENT;
    char  expSysLog[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_POOLCREATEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    result = CF_ChannelInit();

    sprintf(expSysLog,
            "PoolCreate failed for CF, gave adr 0x%x,size %d,stat=0x%x\n",
            (unsigned int)CF_AppData.Mem.Partition, CF_MEMORY_POOL_BYTES,
            (unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected, "CF_ChannelInit, fail PoolCreate");

    UtAssert_True(Ut_CFE_ES_SysLogWritten(expSysLog),
                  "CF_ChannelInit, fail PoolCreate: SysLog Written");
}


/**
 * Test CF_ChannelInit, Nominal
 */
void Test_CF_ChannelInit_Nominal(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SUCCESS;

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_POOLCREATEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    CF_TableInit();
    result = CF_ChannelInit();

    /* Verify results */
    UtAssert_True(result == expected, "CF_ChannelInit, Nominal");
}


/**************************************************************************
 * Tests for CF_AppInit()
 **************************************************************************/
/**
 * Test CF_AppInit, fail EventInit
 */
void Test_CF_AppInit_Fail_EventInit(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;
    char  expSysLog[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = CF_AppInit();

    sprintf(expSysLog, "CF App: Error Registering Events,RC=0x%08X",
                       (unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected, "AppInit, fail EventInit");

    UtAssert_True(Ut_CFE_ES_GetSysLogQueueDepth() == 1,
                  "AppInit, fail EventInit: SysLog QueueDepth");

    UtAssert_True(Ut_CFE_ES_SysLogWritten(expSysLog),
                  "AppInit, fail EventInit: SysLog Written");
}


/**
 * Test CF_AppInit, fail CreateCMDPipe
 */
void Test_CF_AppInit_Fail_CreateCMDPipe(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = CF_AppInit();

    sprintf(expEvent, "Error Creating SB Pipe,RC=0x%08X",
                      (unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected, "AppInit, fail PipeInit");

    UtAssert_EventSent(CF_CR_PIPE_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "AppInit, fail PipeInit: Event Sent");
}


/**
 * Test CF_AppInit, fail SubscribeSendHK
 */
void Test_CF_AppInit_Fail_SubscribeSendHK(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = CF_AppInit();

    sprintf(expEvent, "Error Subscribing to HK Request,RC=0x%08X",
                      (unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected, "CF_AppInit, fail SubscribeSendHK");

    UtAssert_EventSent(CF_SUB_REQ_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppInit, fail SubscribeSendHK: Event Sent");
}


/**
 * Test CF_AppInit, fail SubscribeCmd
 */
void Test_CF_AppInit_Fail_SubscribeCmd(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = CF_AppInit();

    sprintf(expEvent, "Error Subscribing to CF Gnd Cmds,RC=0x%08X",
                      (unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected, "CF_AppInit, fail SubscribeCmd");

    UtAssert_EventSent(CF_SUB_CMD_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppInit, fail SubscribeCmd: Event Sent");
}


/**
 * Test CF_AppInit, fail SubscribeWakeupReqCmd
 */
void Test_CF_AppInit_Fail_SubscribeWakeupReqCmd(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 3);

    /* Execute the function being tested */
    result = CF_AppInit();

    sprintf(expEvent, "Error Subscribing to Wakeup Cmd,RC=0x%08X",
                      (unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected,
                  "CF_AppInit, fail SubscribeWakeupReqCmd");

    UtAssert_EventSent(CF_SUB_WAKE_ERR_EID, CFE_EVS_ERROR, expEvent,
                  "CF_AppInit, fail SubscribeWakeupReqCmd: Event Sent");
}


/**
 * Test CF_AppInit, fail SubscribeInCh0PduMid
 */
void Test_CF_AppInit_Fail_SubscribeInCh0PduMid(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  strTestResult[128];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 4);

    /* Execute the function being tested */
    result = CF_AppInit();

    sprintf(expEvent, "Error Subscribing to Incoming PDUs,RC=0x%08X",
                      (unsigned int)expected);
    sprintf(strTestResult,
            "CF_AppInit, fail SubscribeInCh0PduMid(%04X): Event Sent",
            CF_AppData.Tbl->InCh[0].IncomingPDUMsgId);

    /* Verify results */
    UtAssert_True(result == expected,
                  "CF_AppInit, fail SubscribeInCh0PduMid");

    UtAssert_EventSent(CF_SUB_PDUS_ERR_EID, CFE_EVS_ERROR, expEvent,
                       strTestResult);
}


/**
 * Test CF_AppInit, fail SubscribeInCh1PduMid
 */
void Test_CF_AppInit_Fail_SubscribeInCh1PduMid(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  strTestResult[128];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 5);

    /* Execute the function being tested */
    result = CF_AppInit();

    sprintf(expEvent, "Error Subscribing to Incoming PDUs,RC=0x%08X",
                      (unsigned int)expected);
    sprintf(strTestResult,
            "CF_AppInit, fail SubscribeInCh1PduMid(%04X): Event Sent",
            CF_AppData.Tbl->InCh[1].IncomingPDUMsgId);

    /* Verify results */
    UtAssert_True(result == expected,
                  "CF_AppInit, fail SubscribeInCh1PduMid");

    UtAssert_EventSent(CF_SUB_PDUS_ERR_EID, CFE_EVS_ERROR, expEvent,
                       strTestResult);
}


/**
 * Test CF_AppInit, fail TableInit
 */
void Test_CF_AppInit_Fail_TableInit(void)
{
    /* Set a fail result */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_TBL_ERR_INVALID_NAME;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = CF_AppInit();

    sprintf(expEvent, "Error Registering Config Table,RC=0x%08X",
                      (unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected, "CF_AppInit, fail TableInit");

    UtAssert_EventSent(CF_CFGTBL_REG_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppInit, fail TableInit: Event Sent");
}


/**
 * Test CF_AppInit, fail ChannelInit
 */
void Test_CF_AppInit_Fail_ChannelInit(void)
{
    /* Set a fail result */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_ES_BAD_ARGUMENT;
    char  expSysLog[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_POOLCREATEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    result = CF_AppInit();

    sprintf(expSysLog,
            "PoolCreate failed for CF, gave adr 0x%x,size %d,stat=0x%x\n",
            (unsigned int)CF_AppData.Mem.Partition, CF_MEMORY_POOL_BYTES,
            (unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected, "CF_AppInit, fail ChannelInit");

    UtAssert_True(Ut_CFE_ES_SysLogWritten(expSysLog),
                  "CF_AppInit, fail ChannelInit: SysLog Written");
}


/**
 * Test CF_AppInit, Nominal
 */
void Test_CF_AppInit_Nominal(void)
{
    /* Set a fail result */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SUCCESS;
    char  expEventInit[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventAckTimeOut[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventAckLimit[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventNakTimeout[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventNakLimit[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventInactTimeout[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventOFS[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventSaveIF[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventMyId[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Execute the function being tested */
    result = CF_AppInit();

    sprintf(expEventInit, "CF Initialized.  Version %d.%d.%d.%d",
         CF_MAJOR_VERSION, CF_MINOR_VERSION, CF_REVISION, CF_MISSION_REV);
    sprintf(expEventAckTimeOut, "cfdp_engine: 'ack_timeout' set to '%s'.",
                                CF_AppData.Tbl->AckTimeout);
    sprintf(expEventAckLimit, "cfdp_engine: 'ack_limit' set to '%s'.",
                                CF_AppData.Tbl->AckLimit);
    sprintf(expEventNakTimeout, "cfdp_engine: 'nak_timeout' set to '%s'.",
                                CF_AppData.Tbl->NakTimeout);
    sprintf(expEventNakLimit, "cfdp_engine: 'nak_limit' set to '%s'.",
                                CF_AppData.Tbl->NakLimit);
    sprintf(expEventInactTimeout,
            "cfdp_engine: 'inactivity_timeout' set to '%s'.",
            CF_AppData.Tbl->InactivityTimeout);
    sprintf(expEventOFS,
            "cfdp_engine: 'outgoing_file_chunk_size' set to '%s'.",
            CF_AppData.Tbl->OutgoingFileChunkSize);
    sprintf(expEventSaveIF,
            "cfdp_engine: 'save_incomplete_files' set to '%s'.",
            CF_AppData.Tbl->SaveIncompleteFiles);
    sprintf(expEventMyId, "cfdp_engine: entity-id set to '%s'.",
                          CF_AppData.Tbl->FlightEntityId);

    /* Verify results */
    UtAssert_True(result == expected, "CF_AppInit, Nominal");

    UtAssert_EventSent(CF_INIT_EID, CFE_EVS_INFORMATION, expEventInit,
                       "CF_AppInit, Nominal: Init Event Sent");

    UtAssert_EventSent(CF_CFDP_ENGINE_INFO_EID, CFE_EVS_DEBUG,
                       expEventAckTimeOut,
                       "CF_AppInit, Nominal: AckTimeOut Event Sent");
    UtAssert_EventSent(CF_CFDP_ENGINE_INFO_EID, CFE_EVS_DEBUG,
                       expEventAckLimit,
                       "CF_AppInit, Nominal: AckLimit Event Sent");
    UtAssert_EventSent(CF_CFDP_ENGINE_INFO_EID, CFE_EVS_DEBUG,
                       expEventNakTimeout,
                       "CF_AppInit, Nominal: NakTimeout Event Sent");
    UtAssert_EventSent(CF_CFDP_ENGINE_INFO_EID, CFE_EVS_DEBUG,
                       expEventNakLimit,
                       "CF_AppInit, Nominal: NakLimit Event Sent");
    UtAssert_EventSent(CF_CFDP_ENGINE_INFO_EID, CFE_EVS_DEBUG,
                       expEventInactTimeout,
                       "CF_AppInit, Nominal: InactTimeout Event Sent");
    UtAssert_EventSent(CF_CFDP_ENGINE_INFO_EID, CFE_EVS_DEBUG,
             expEventOFS,
             "CF_AppInit, Nominal: OutgoingFileChunkSize Event Sent");
    UtAssert_EventSent(CF_CFDP_ENGINE_INFO_EID, CFE_EVS_DEBUG,
             expEventSaveIF,
             "CF_AppInit, Nominal: SaveIncompleteFiles Event Sent");
    UtAssert_EventSent(CF_CFDP_ENGINE_INFO_EID, CFE_EVS_DEBUG,
             expEventMyId,
             "CF_AppInit, Nominal: FlightEntityId Event Sent");

    UtAssert_True(Ut_CFE_ES_GetSysLogQueueDepth() == 0,
                  "CF_AppInit, Nominal: No SysLogWritten");
}


/**************************************************************************
 * Tests for CF_AppMain()
 **************************************************************************/
/**
 * Test CF_AppMain, fail RegisterApp
 */
void Test_CF_AppMain_Fail_RegisterApp(void)
{
    char  expSysLog[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* fail the register app */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX,
                            CFE_ES_ERR_APP_REGISTER, 1);

    /* Execute the function being tested */
    CF_AppMain();

    sprintf(expSysLog, "%s", "");

    /* Verify results */
    UtAssert_True(Ut_CFE_ES_SysLogWritten(expSysLog),
                  "CF_AppMain, fail RegisterApp: SysLog Written");
}


/**
 * Test CF_AppMain, fail AppInit
 */
void Test_CF_AppMain_Fail_AppInit(void)
{
    char  expSysLog[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* fail the register app */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX,
                             CFE_EVS_APP_NOT_REGISTERED, 1);

    /* Execute the function being tested */
    CF_AppMain();

    sprintf(expSysLog, "CF App: Error Registering Events,RC=0x%08X",
                       (unsigned int)CFE_EVS_APP_NOT_REGISTERED);

    /* Verify results */
    UtAssert_True(Ut_CFE_ES_SysLogWritten(expSysLog),
                  "CF_AppMain, fail AppInit: SysLog Written");
}


/**
 * Test CF_AppMain, fail AcquireConfigPtrs
 */
void Test_CF_AppMain_Fail_AcquireConfigPtrs(void)
{
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX,
                             CFE_TBL_ERR_INVALID_HANDLE, 2);

    /* Execute the function being tested */
    CF_AppMain();

    sprintf(expEvent, "%s", "");

    /* Verify results */
}


/**
 * Test CF_AppMain, fail InvalidCmdMessage
 */
void Test_CF_AppMain_Fail_InvalidCmdMessage(void)
{
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, 0, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    CF_AppMain();

    sprintf(expEvent, "Unexpected Msg Received MsgId -- ID = 0x%04X", 0);

    /* Verify results */
    UtAssert_EventSent(CF_MID_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppMain, fail InvalidCmdMessage: Event Sent");
}


/**
 * Test CF_AppMain, fail PipeError
 */
void Test_CF_AppMain_Fail_PipeError(void)
{
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_PIPE_RD_ERR, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    CF_AppMain();

    sprintf(expEvent, "CF_APP Exiting due to CFE_SB_RcvMsg error 0x%08X",
                      (unsigned int)CFE_SB_PIPE_RD_ERR);

    /* Verify results */
    UtAssert_EventSent(CF_RCV_MSG_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppMain, fail PipeError: Event Sent");
}


/**
 * Test CF_AppMain, Nominal
 */
void Test_CF_AppMain_Nominal(void)
{
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    CF_AppMain();
}




/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void CF_App_Test_AddTestCases(void)
{
    UtTest_Add(Test_CF_ChannelInit_Fail_PoolCreate,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_ChannelInit_Fail_PoolCreate");
    UtTest_Add(Test_CF_ChannelInit_Nominal,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_ChannelInit_Nominal");

    UtTest_Add(Test_CF_AppInit_Fail_EventInit,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppInit_Fail_EventInit");
    UtTest_Add(Test_CF_AppInit_Fail_CreateCMDPipe,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppInit_Fail_CreateCMDPipe");
    UtTest_Add(Test_CF_AppInit_Fail_SubscribeSendHK,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppInit_Fail_SubscribeSendHK");
    UtTest_Add(Test_CF_AppInit_Fail_SubscribeCmd,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppInit_Fail_SubscribeCmd");
    UtTest_Add(Test_CF_AppInit_Fail_SubscribeWakeupReqCmd,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppInit_Fail_SubscribeWakeupReqCmd");
    UtTest_Add(Test_CF_AppInit_Fail_SubscribeInCh0PduMid,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppInit_Fail_SubscribeInCh0PduMid");
    UtTest_Add(Test_CF_AppInit_Fail_SubscribeInCh1PduMid,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppInit_Fail_SubscribeInCh1PduMid");
    UtTest_Add(Test_CF_AppInit_Fail_TableInit,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppInit_Fail_TableInit");
    UtTest_Add(Test_CF_AppInit_Fail_ChannelInit,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppInit_Fail_ChannelInit");
    UtTest_Add(Test_CF_AppInit_Nominal,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppInit_Nominal");

    UtTest_Add(Test_CF_AppMain_Fail_RegisterApp,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppMain_Fail_RegisterApp");
#if 0
    UtTest_Add(Test_CF_AppMain_Fail_AppInit,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppMain_Fail_AppInit");
#endif
    UtTest_Add(Test_CF_AppMain_Fail_AcquireConfigPtrs,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppMain_Fail_AcquireConfigPtrs");
    UtTest_Add(Test_CF_AppMain_Fail_InvalidCmdMessage,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppMain_Fail_InvalidCmdMessage");
    UtTest_Add(Test_CF_AppMain_Fail_PipeError,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppMain_Fail_PipeError");

    UtTest_Add(Test_CF_AppMain_Nominal,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppMain_Nominal");
}
