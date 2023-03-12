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
#include "cf_custom_hooks.h"

#include "cf_events.h"
#include "cf_version.h"

#include "cfdp_provides.h"

#include "uttest.h"
#include "ut_osapi_stubs.h"
#include "ut_osfileapi_stubs.h"
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
 * Test CF_AppInit, fail SendEvent
 */
void Test_CF_AppInit_Fail_SendEvent(void)
{
    /* Set a fail result */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;
    int32 NumEventsExp;
    char  expSysLog[CFE_EVS_MAX_MESSAGE_LENGTH];

    NumEventsExp = 8;/* cfdp debug events for set mib calls */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_SENDEVENT_INDEX, expected,
                             NumEventsExp + 1);

    /* Execute the function being tested */
    result = CF_AppInit();

    sprintf(expSysLog,
            "CF App:Error Sending Initialization Event,RC=0x%08X\n",
            (unsigned int)expected);

    /* Verify results */
    UtAssert_True(result == expected, "CF_AppInit, fail SendEvent");

    UtAssert_True(Ut_CFE_ES_SysLogWritten(expSysLog),
                  "CF_AppInit, fail SendEvent: SysLog Written");
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

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX,
                             CFE_EVS_APP_NOT_REGISTERED, 1);

    /* Execute the function being tested */
    CF_AppMain();

    sprintf(expSysLog, "CF App: Error Registering Events,RC=0x%08X",
                       (unsigned int)CFE_EVS_APP_NOT_REGISTERED);

    /* Verify results */
    UtAssert_True(CF_AppData.RunStatus == CFE_ES_APP_ERROR,
                  "CF_AppMain, fail AppInit");

    UtAssert_True(Ut_CFE_ES_SysLogWritten(expSysLog),
                  "CF_AppMain, fail AppInit: SysLog Written");
}


/**
 * Test CF_AppMain, fail SemGetIdByName
 */
void Test_CF_AppMain_Fail_SemGetIdByName(void)
{
    char  expEventCh0[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventCh1[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_OSAPI_SetReturnCode(UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX,
                           OS_INVALID_POINTER, 1);
    Ut_OSAPI_ContinueReturnCodeAfterCountZero(
                           UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    CF_AppMain();

    sprintf(expEventCh0,
         "SemGetId Err:Chan %d downlink PDUs cannot be throttled.0x%08X",
         0, (unsigned int)OS_INVALID_POINTER);
    sprintf(expEventCh1,
         "SemGetId Err:Chan %d downlink PDUs cannot be throttled.0x%08X",
         1, (unsigned int)OS_INVALID_POINTER);

    /* Verify results */
    UtAssert_EventSent(CF_HANDSHAKE_ERR1_EID, CFE_EVS_ERROR, expEventCh0,
                       "CF_AppMain, fail SemGetIdByName: Ch0 Event Sent");

    UtAssert_EventSent(CF_HANDSHAKE_ERR1_EID, CFE_EVS_ERROR, expEventCh1,
                       "CF_AppMain, fail SemGetIdByName: Ch1 Event Sent");
}


/**
 * Test CF_AppMain, fail AcquireConfigPtrs
 */
void Test_CF_AppMain_Fail_AcquireConfigPtrs(void)
{
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX,
                             CFE_TBL_ERR_INVALID_HANDLE, 2);

    /* Execute the function being tested */
    CF_AppMain();

    sprintf(expEvent, "%s", "");

    /* Verify results */
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
    int32           CmdPipe;
    CF_NoArgsCmd_t  CmdMsg;
    char  expEventInit[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventNoop[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(CF_PIPE_NAME);
    CFE_SB_InitMsg((void*)&CmdMsg, CF_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)CF_NOOP_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    CF_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    CF_AppMain();

    sprintf(expEventInit, "CF Initialized.  Version %d.%d.%d.%d",
            CF_MAJOR_VERSION, CF_MINOR_VERSION, CF_REVISION, CF_MISSION_REV);

    sprintf(expEventNoop, "CF No-op command, Version %d.%d.%d.%d",
            CF_MAJOR_VERSION, CF_MINOR_VERSION, CF_REVISION, CF_MISSION_REV);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 1) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppMain, Nominal");

    UtAssert_EventSent(CF_INIT_EID, CFE_EVS_INFORMATION, expEventInit,
                       "CF_AppMain, Nominal: Init Event Sent");

    UtAssert_EventSent(CF_NOOP_CMD_EID, CFE_EVS_INFORMATION, expEventNoop,
                       "CF_AppMain, Nominal: Noop Event Sent");
}


/**
 * Test CF_AppMain, IncomingMsgClass1
 */
void Test_CF_AppMain_IncomingMsgClass1(void)
{
    uint16              PDataLen;
    uint16              hdr_len;
    uint32              UpActQEntryCnt;
    uint32              UpHistQEntryCnt;
    int32               CmdPipe;
    CF_Test_InPDUInfo_t InPDUInfo1;
    CF_Test_InPDUInfo_t InPDUInfo2;
    CF_Test_InPDUMsg_t  InMdPDUMsg1;
    CF_Test_InPDUMsg_t  InFdPDUMsg1;
    CF_Test_InPDUMsg_t  InEofPDUMsg1;
    CF_Test_InPDUMsg_t  InMdPDUMsg2;
    CF_Test_InPDUMsg_t  InFdPDUMsg2;
    CF_Test_InPDUMsg_t  InEofPDUMsg2;
    char   expEventInSt1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char   expEventInFin1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char   expEventInSt2[CFE_EVS_MAX_MESSAGE_LENGTH];
    char   expEventInFin2[CFE_EVS_MAX_MESSAGE_LENGTH];

    CmdPipe = Ut_CFE_SB_CreatePipe(CF_PIPE_NAME);

    /**** File 1 ****/
    /* Send CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg1, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg1), TRUE);
    InPDUInfo1.pdu_type = FILE_DIR_PDU;
    InPDUInfo1.direction = TEST_TO_RECEIVER;
    InPDUInfo1.mode = TEST_UNACK_MODE;
    InPDUInfo1.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo1.trans.source_id);
    InPDUInfo1.trans.number = TEST_IN_TRANS_NUMBER;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo1.dest_id);
    InPDUInfo1.file_transfer = YES;
    InPDUInfo1.segmentation_control = NO;
    InPDUInfo1.file_size = TEST_FILE_SIZE1;   /* Total file size */
    sprintf(InPDUInfo1.src_filename, "%s%s", TestInDir0, TestInFile1);
    sprintf(InPDUInfo1.dst_filename, "%s%s", TestInDir0, TestInFile1);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo1.src_filename) + 1;
    PDataLen += strlen(InPDUInfo1.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg1, &InPDUInfo1, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg1, &InPDUInfo1, hdr_len);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMdPDUMsg1, (CFE_SB_PipeId_t)CmdPipe);

    /* Send CF_PPD_TO_CPD_PDU_MID (FD PDU) */
    CFE_SB_InitMsg((void*)&InFdPDUMsg1, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InFdPDUMsg1), TRUE);
    InPDUInfo1.pdu_type = FILE_DATA_PDU;
    InPDUInfo1.offset = 0;
    InPDUInfo1.file_size = TEST_FILE_SIZE1;   /* File size for this FD PDU */

    PDataLen = InPDUInfo1.file_size + 4;
    hdr_len = CF_TstUtil_GenPDUHeader(&InFdPDUMsg1, &InPDUInfo1, PDataLen);
    CF_TstUtil_BuildFDPdu(&InFdPDUMsg1, &InPDUInfo1, hdr_len);
    Ut_CFE_SB_AddMsgToPipe((void*)&InFdPDUMsg1, (CFE_SB_PipeId_t)CmdPipe);

    /* Send CF_PPD_TO_CPD_PDU_MID (EOF PDU) */
    CFE_SB_InitMsg((void*)&InEofPDUMsg1, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InEofPDUMsg1), TRUE);
    InPDUInfo1.pdu_type = FILE_DIR_PDU;
    InPDUInfo1.cond_code = NO_ERROR;
    InPDUInfo1.file_size = TEST_FILE_SIZE1;   /* Total file size */
    InPDUInfo1.checksum = 0;                 /* Not supported */

    PDataLen = 10;
    hdr_len = CF_TstUtil_GenPDUHeader(&InEofPDUMsg1, &InPDUInfo1, PDataLen);
    CF_TstUtil_BuildEOFPdu(&InEofPDUMsg1, &InPDUInfo1, hdr_len);
    Ut_CFE_SB_AddMsgToPipe((void*)&InEofPDUMsg1, (CFE_SB_PipeId_t)CmdPipe);

    /**** File 2 ****/
    /* Send CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg2, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg2), TRUE);
    InPDUInfo2.pdu_type = FILE_DIR_PDU;
    InPDUInfo2.direction = TEST_TO_RECEIVER;
    InPDUInfo2.mode = TEST_UNACK_MODE;
    InPDUInfo2.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo2.trans.source_id);
    InPDUInfo2.trans.number = TEST_IN_TRANS_NUMBER + 1;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo2.dest_id);
    InPDUInfo2.file_transfer = YES;
    InPDUInfo2.segmentation_control = NO;
    InPDUInfo2.file_size = TEST_FILE_SIZE1;   /* Total file size */
    sprintf(InPDUInfo2.src_filename, "%s%s", TestInDir0, TestInFile2);
    sprintf(InPDUInfo2.dst_filename, "%s%s", TestInDir0, TestInFile2);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo2.src_filename) + 1;
    PDataLen += strlen(InPDUInfo2.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg2, &InPDUInfo2, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg2, &InPDUInfo2, hdr_len);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMdPDUMsg2, (CFE_SB_PipeId_t)CmdPipe);

    /* Send CF_PPD_TO_CPD_PDU_MID (FD PDU) */
    CFE_SB_InitMsg((void*)&InFdPDUMsg2, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InFdPDUMsg2), TRUE);
    InPDUInfo2.pdu_type = FILE_DATA_PDU;
    InPDUInfo2.offset = 0;
    InPDUInfo2.file_size = TEST_FILE_SIZE1;   /* File size for this FD PDU */

    PDataLen = InPDUInfo2.file_size + 4;
    hdr_len = CF_TstUtil_GenPDUHeader(&InFdPDUMsg2, &InPDUInfo2, PDataLen);
    CF_TstUtil_BuildFDPdu(&InFdPDUMsg2, &InPDUInfo2, hdr_len);
    Ut_CFE_SB_AddMsgToPipe((void*)&InFdPDUMsg2, (CFE_SB_PipeId_t)CmdPipe);

    /* Send CF_PPD_TO_CPD_PDU_MID (EOF PDU) */
    CFE_SB_InitMsg((void*)&InEofPDUMsg2, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InEofPDUMsg2), TRUE);
    InPDUInfo2.pdu_type = FILE_DIR_PDU;
    InPDUInfo2.cond_code = NO_ERROR;
    InPDUInfo2.file_size = TEST_FILE_SIZE1;   /* Total file size */
    InPDUInfo2.checksum = 0;                 /* Not supported */

    PDataLen = 10;
    hdr_len = CF_TstUtil_GenPDUHeader(&InEofPDUMsg2, &InPDUInfo2, PDataLen);
    CF_TstUtil_BuildEOFPdu(&InEofPDUMsg2, &InPDUInfo2, hdr_len);
    Ut_CFE_SB_AddMsgToPipe((void*)&InEofPDUMsg2, (CFE_SB_PipeId_t)CmdPipe);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 7);

    /* Execute the function being tested */
    CF_AppMain();

    UpActQEntryCnt = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt;
    UpHistQEntryCnt = CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt;

    CF_ShowQs();

    sprintf(expEventInSt1, "Incoming trans started %d.%d_%d,dest %s",
            InPDUInfo1.trans.source_id.value[0],
            InPDUInfo1.trans.source_id.value[1],
            (int)InPDUInfo1.trans.number, InPDUInfo1.dst_filename);

    sprintf(expEventInFin1, "Incoming trans success %d.%d_%d,dest %s",
            InPDUInfo1.trans.source_id.value[0],
            InPDUInfo1.trans.source_id.value[1],
            (int)InPDUInfo1.trans.number, InPDUInfo1.dst_filename);

    sprintf(expEventInSt2, "Incoming trans started %d.%d_%d,dest %s",
            InPDUInfo2.trans.source_id.value[0],
            InPDUInfo2.trans.source_id.value[1],
            (int)InPDUInfo2.trans.number, InPDUInfo2.dst_filename);

    sprintf(expEventInFin2, "Incoming trans success %d.%d_%d,dest %s",
            InPDUInfo2.trans.source_id.value[0],
            InPDUInfo2.trans.source_id.value[1],
            (int)InPDUInfo2.trans.number, InPDUInfo2.dst_filename);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppMain, IncomingMsgClass1: No ErrCounter");

    UtAssert_True((CF_AppData.Hk.Up.MetaCount == 2) &&
                  (CF_AppData.Hk.Up.SuccessCounter == 2) &&
                  (CF_AppData.Hk.Up.FailedCounter == 0),
                  "CF_AppMain, IncomingMsgClass1: Up Telemetry data");

    UtAssert_True((UpActQEntryCnt == 0) && (UpHistQEntryCnt == 2),
                  "CF_AppMain, IncomingMsgClass1: UpQueueEntry cnt");

    UtAssert_EventSent(CF_IN_TRANS_START_EID, CFE_EVS_INFORMATION,
                  expEventInSt1,
                  "CF_AppMain, IncomingMsgClass1: InPDU1 Start Event Sent");

    UtAssert_EventSent(CF_IN_TRANS_OK_EID, CFE_EVS_INFORMATION,
                  expEventInFin1,
                  "CF_AppMain, IncomingMsgClass1: InPDU1 Finish Event Sent");

    UtAssert_EventSent(CF_IN_TRANS_START_EID, CFE_EVS_INFORMATION,
                  expEventInSt2,
                  "CF_AppMain, IncomingMsgClass1: InPDU2 Start Event Sent");

    UtAssert_EventSent(CF_IN_TRANS_OK_EID, CFE_EVS_INFORMATION,
                  expEventInFin2,
                  "CF_AppMain, IncomingMsgClass1: InPDU2 Finish Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF, IncomingMsgClass2Nominal
 */
void Test_CF_IncomingMsgClass2Nominal(void)
{
    uint16              PDataLen;
    uint16              hdr_len;
    int                 i;
    uint32              offset;
    uint32              size_left;
    uint32              UpActQEntryCntBefore;
    uint32              UpHistQEntryCntBefore;
    uint32              UpActQEntryCntAfter;
    uint32              UpHistQEntryCntAfter;
    CF_Test_InPDUInfo_t InMdPDUInfo;
    CF_Test_InPDUInfo_t InFdPDUInfo;
    CF_Test_InPDUInfo_t InEofPDUInfo;
    CF_Test_InPDUMsg_t  InMdPDUMsg;
    CF_Test_InPDUMsg_t  InFdPDUMsg;
    CF_Test_InPDUMsg_t  InEofPDUMsg;
    CF_Test_InPDUMsg_t  InAckFinMsg;
    char   expEventInSt[CFE_EVS_MAX_MESSAGE_LENGTH];
    char   expEventInFin[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg), TRUE);
    InMdPDUInfo.pdu_type = FILE_DIR_PDU;
    InMdPDUInfo.direction = TEST_TO_RECEIVER;
    InMdPDUInfo.mode = TEST_ACK_MODE;
    InMdPDUInfo.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InMdPDUInfo.trans.source_id);
    InMdPDUInfo.trans.number = TEST_IN_TRANS_NUMBER;
    cfdp_id_from_string(TestFlightEntityId, &InMdPDUInfo.dest_id);
    InMdPDUInfo.file_transfer = YES;
    InMdPDUInfo.segmentation_control = NO;
    InMdPDUInfo.file_size = TEST_FILE_SIZE2;   /* Total file size */
    sprintf(InMdPDUInfo.src_filename, "%s%s", TestInDir1, TestInFileNominal);
    sprintf(InMdPDUInfo.dst_filename, "%s%s", TestInDir1, TestInFileNominal);

    PDataLen = 6;
    PDataLen += strlen(InMdPDUInfo.src_filename) + 1;
    PDataLen += strlen(InMdPDUInfo.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg, &InMdPDUInfo, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg, &InMdPDUInfo, hdr_len);

    InFdPDUInfo = InMdPDUInfo;
    InEofPDUInfo = InMdPDUInfo;

    /* Build CF_PPD_TO_CPD_PDU_MID (EOF PDU) */
    CFE_SB_InitMsg((void*)&InEofPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InEofPDUMsg), TRUE);
    InEofPDUInfo.pdu_type = FILE_DIR_PDU;
    InEofPDUInfo.cond_code = NO_ERROR;
    InEofPDUInfo.file_size = TEST_FILE_SIZE2;
    InEofPDUInfo.checksum = 0x94920cb3;

    PDataLen = 10;
    hdr_len = CF_TstUtil_GenPDUHeader(&InEofPDUMsg, &InEofPDUInfo, PDataLen);
    CF_TstUtil_BuildEOFPdu(&InEofPDUMsg, &InEofPDUInfo, hdr_len);

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX,
                             (void *)&OS_CountSemGetIdByNameHook);

    /* To give the unit test system time for SB Msg */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void *)&CFE_SB_TimeStampMsgHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void *)&CFE_TIME_GetTimeHook);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_CREAT_INDEX,
                                 (void *)&OS_creatHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_OPEN_INDEX,
                                 (void *)&OS_openHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_READ_INDEX,
                                 (void *)&OS_readHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_WRITE_INDEX,
                                 (void *)&OS_writeHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_CLOSE_INDEX,
                                 (void *)&OS_closeHook);

    /* Return a buf pointer to send a PDU through SB */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_ZEROCOPYGETPTR_INDEX,
                              (void *)&CFE_SB_ZeroCopyGetPtrHook);

    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX,
                              (void*)&CFE_ES_PutPoolBufHook);

    ZeroCopySendHook_AckEofNoErr_Rcvd = NO;
    ZeroCopySendHook_FinNoErr_Rcvd = NO;
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_ZEROCOPYSEND_INDEX,
                              (void *)&CFE_SB_ZeroCopySendHook);

    /* Execute the function being tested */
    CF_AppInit();
    CF_GetHandshakeSemIds();

    /* Incoming MD PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Incoming FD PDU */
    offset = 0;
    size_left = TEST_FILE_SIZE2;
    while (size_left > 0)
    {
        CFE_SB_InitMsg((void*)&InFdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                       sizeof(InFdPDUMsg), TRUE);
        InFdPDUInfo.pdu_type = FILE_DATA_PDU;
        InFdPDUInfo.offset = offset;
        if (size_left <= TEST_FILE_CHUNK_SIZE)
        {
            InFdPDUInfo.file_size = size_left;
            size_left = 0;
        }
        else
        {
            InFdPDUInfo.file_size = TEST_FILE_CHUNK_SIZE;
            size_left -= TEST_FILE_CHUNK_SIZE;
            offset += TEST_FILE_CHUNK_SIZE;
        }

        PDataLen = InFdPDUInfo.file_size + 4;
        hdr_len = CF_TstUtil_GenPDUHeader(&InFdPDUMsg, &InFdPDUInfo, PDataLen);
        CF_TstUtil_BuildFDPdu(&InFdPDUMsg, &InFdPDUInfo, hdr_len);

        CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InFdPDUMsg;
        CF_AppPipe(CF_AppData.MsgPtr);
    }

    /* Incoming EOF PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InEofPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    UpActQEntryCntBefore = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt;
    UpHistQEntryCntBefore = CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt;

    CF_ShowQs();

    cfdp_cycle_each_transaction();
    cfdp_cycle_each_transaction();

    if (ZeroCopySendHook_FinNoErr_Rcvd == YES)
    {
        CFE_SB_InitMsg((void*)&InAckFinMsg, CF_PPD_TO_CPD_PDU_MID,
                       sizeof(InAckFinMsg), TRUE);
        memcpy(InAckFinMsg.PduContent.Content, HookOutPDU.content,
               HookOutPDU.length);
        CF_Test_PrintCmdMsg(&InAckFinMsg,
                            CFE_SB_CMD_HDR_SIZE + HookOutPDU.length);

        CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InAckFinMsg;
        CF_AppPipe(CF_AppData.MsgPtr);
    }

    UpActQEntryCntAfter = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt;
    UpHistQEntryCntAfter = CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt;

    CF_ShowQs();

    sprintf(expEventInSt, "Incoming trans started %d.%d_%d,dest %s",
            InMdPDUInfo.trans.source_id.value[0],
            InMdPDUInfo.trans.source_id.value[1],
            (int)InMdPDUInfo.trans.number, InMdPDUInfo.dst_filename);

    sprintf(expEventInFin, "Incoming trans success %d.%d_%d,dest %s",
            InMdPDUInfo.trans.source_id.value[0],
            InMdPDUInfo.trans.source_id.value[1],
            (int)InMdPDUInfo.trans.number, InMdPDUInfo.dst_filename);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF, IncomingMsgClass2Nominal: No ErrCounter");

    UtAssert_True((CF_AppData.Hk.Up.MetaCount == 1) &&
                  (CF_AppData.Hk.Up.SuccessCounter == 1) &&
                  (CF_AppData.Hk.Up.FailedCounter == 0),
                  "CF, IncomingMsgClass2Nominal: Up Telemetry data");

    UtAssert_True(CF_AppData.Hk.Chan[0].PDUsSent == 2,
                  "CF, IncomingMsgClass2Nominal: Pb Telemetry data");

    UtAssert_True((UpActQEntryCntBefore == 1) && (UpActQEntryCntAfter == 0)
              && (UpHistQEntryCntBefore == 0) && (UpHistQEntryCntAfter == 1),
              "CF, IncomingMsgClass2Nominal: UpQueueEntry cnt");

    UtAssert_True(ZeroCopySendHook_AckEofNoErr_Rcvd == YES,
                  "CF, IncomingMsgClass2Nominal: ACK-EOF NoErr received");

    UtAssert_True(ZeroCopySendHook_FinNoErr_Rcvd == YES,
                  "CF, IncomingMsgClass2Nominal: FIN COMPLETE received");

    UtAssert_EventSent(CF_IN_TRANS_START_EID, CFE_EVS_INFORMATION, expEventInSt,
                     "CF, IncomingMsgClass2Nominal: Incoming Start Event Sent");

    UtAssert_EventSent(CF_IN_TRANS_OK_EID, CFE_EVS_INFORMATION, expEventInFin,
                    "CF, IncomingMsgClass2Nominal: Incoming Finish Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF, IncomingMsgClass2InactTimerExpired
 */
void Test_CF_IncomingMsgClass2InactTimerExpired(void)
{
    uint16              PDataLen;
    uint16              hdr_len;
    int                 i;
    uint32              offset;
    uint32              size_left;
    uint32              UpActQEntryCntBefore;
    uint32              UpHistQEntryCntBefore;
    uint32              UpActQEntryCntAfter;
    uint32              UpHistQEntryCntAfter;
    uint32              inactivity_timeout;
    time_t              start_time, elapsed_time;
    CF_Test_InPDUInfo_t InMdPDUInfo;
    CF_Test_InPDUInfo_t InFdPDUInfo;
    CF_Test_InPDUInfo_t InEofPDUInfo;
    CF_Test_InPDUMsg_t  InMdPDUMsg;
    CF_Test_InPDUMsg_t  InFdPDUMsg;
    CF_Test_InPDUMsg_t  InEofPDUMsg;
    CF_Test_InPDUMsg_t  InAckFinPDUMsg;
    char  FullDstFilename[MAX_FILE_NAME_LENGTH];
    char  expEventInSt[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventInact[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventInFail[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg), TRUE);
    InMdPDUInfo.pdu_type = FILE_DIR_PDU;
    InMdPDUInfo.direction = TEST_TO_RECEIVER;
    InMdPDUInfo.mode = TEST_ACK_MODE;
    InMdPDUInfo.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InMdPDUInfo.trans.source_id);
    InMdPDUInfo.trans.number = TEST_IN_TRANS_NUMBER;
    cfdp_id_from_string(TestFlightEntityId, &InMdPDUInfo.dest_id);
    InMdPDUInfo.file_transfer = YES;
    InMdPDUInfo.segmentation_control = NO;
    InMdPDUInfo.file_size = TEST_FILE_SIZE2;
    sprintf(InMdPDUInfo.src_filename, "%s%s",
            TestInDir1, TestInFileTimerExpired);
    sprintf(InMdPDUInfo.dst_filename, "%s%s",
            TestInDir1, TestInFileTimerExpired);

    PDataLen = 6;
    PDataLen += strlen(InMdPDUInfo.src_filename) + 1;
    PDataLen += strlen(InMdPDUInfo.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg, &InMdPDUInfo, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg, &InMdPDUInfo, hdr_len);

    InFdPDUInfo = InMdPDUInfo;
    InEofPDUInfo = InMdPDUInfo;

    /* Build CF_PPD_TO_CPD_PDU_MID (EOF PDU) */
    CFE_SB_InitMsg((void*)&InEofPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InEofPDUMsg), TRUE);
    InEofPDUInfo.pdu_type = FILE_DIR_PDU;
    InEofPDUInfo.cond_code = NO_ERROR;
    InEofPDUInfo.file_size = TEST_FILE_SIZE2;
    InEofPDUInfo.checksum = 0;

    PDataLen = 10;
    hdr_len = CF_TstUtil_GenPDUHeader(&InEofPDUMsg, &InEofPDUInfo, PDataLen);
    CF_TstUtil_BuildEOFPdu(&InEofPDUMsg, &InEofPDUInfo, hdr_len);

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX,
                             (void *)&OS_CountSemGetIdByNameHook);

    /* To give the unit test system time for SB Msg */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void *)&CFE_SB_TimeStampMsgHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void *)&CFE_TIME_GetTimeHook);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_CREAT_INDEX,
                                 (void *)&OS_creatHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_OPEN_INDEX,
                                 (void *)&OS_openHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_READ_INDEX,
                                 (void *)&OS_readHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_WRITE_INDEX,
                                 (void *)&OS_writeHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_CLOSE_INDEX,
                                 (void *)&OS_closeHook);

    /* Return a buf pointer to send a PDU through SB */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_ZEROCOPYGETPTR_INDEX,
                              (void *)&CFE_SB_ZeroCopyGetPtrHook);

    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX,
                              (void*)&CFE_ES_PutPoolBufHook);

    ZeroCopySendHook_FinInactTimeout_Rcvd = NO;
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_ZEROCOPYSEND_INDEX,
                              (void *)&CFE_SB_ZeroCopySendHook);

    /* Execute the function being tested */
    CF_AppInit();
    CF_GetHandshakeSemIds();

    /* Incoming MD PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Incoming FD PDU */
    offset = 0;
    size_left = TEST_FILE_SIZE2;
    while (size_left > 0)
    {
        CFE_SB_InitMsg((void*)&InFdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                       sizeof(InFdPDUMsg), TRUE);
        InFdPDUInfo.pdu_type = FILE_DATA_PDU;
        InFdPDUInfo.offset = offset;
        if (size_left <= TEST_FILE_CHUNK_SIZE)
        {
            InFdPDUInfo.file_size = size_left;
            size_left = 0;
        }
        else
        {
            InFdPDUInfo.file_size = TEST_FILE_CHUNK_SIZE;
            size_left -= TEST_FILE_CHUNK_SIZE;
            offset += TEST_FILE_CHUNK_SIZE;
        }

        PDataLen = InFdPDUInfo.file_size + 4;
        hdr_len = CF_TstUtil_GenPDUHeader(&InFdPDUMsg, &InFdPDUInfo, PDataLen);
        CF_TstUtil_BuildFDPdu(&InFdPDUMsg, &InFdPDUInfo, hdr_len);

        CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InFdPDUMsg;
        CF_AppPipe(CF_AppData.MsgPtr);
    }

    /* Do not send EOF PDU */
    printf("Waiting for the timer expired..................\n");

    inactivity_timeout = (uint32)atoi(CF_AppData.Tbl->InactivityTimeout);
    time(&start_time);
    do
    {
        time(&elapsed_time);
    } while((elapsed_time - start_time) < (inactivity_timeout + 5));

    UpActQEntryCntBefore = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt;
    UpHistQEntryCntBefore = CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt;
    CF_ShowQs();

    cfdp_cycle_each_transaction();
    cfdp_cycle_each_transaction();

    if (ZeroCopySendHook_FinInactTimeout_Rcvd == YES)
    {
        CFE_SB_InitMsg((void*)&InAckFinPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                       sizeof(InAckFinPDUMsg), TRUE);
        memcpy(InAckFinPDUMsg.PduContent.Content, HookOutPDU.content,
               HookOutPDU.length);
        CF_Test_PrintCmdMsg(&InAckFinPDUMsg,
                            CFE_SB_CMD_HDR_SIZE + HookOutPDU.length);

        CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InAckFinPDUMsg;
        CF_AppPipe(CF_AppData.MsgPtr);
    }

    UpActQEntryCntAfter = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt;
    UpHistQEntryCntAfter = CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt;
    CF_ShowQs();

    sprintf(expEventInSt, "Incoming trans started %d.%d_%d,dest %s",
            InMdPDUInfo.trans.source_id.value[0],
            InMdPDUInfo.trans.source_id.value[1],
            (int)InMdPDUInfo.trans.number, InMdPDUInfo.dst_filename);

    sprintf(expEventInact, "Flight Inactivity Timer Expired %d.%d_%d,%s",
            InMdPDUInfo.trans.source_id.value[0],
            InMdPDUInfo.trans.source_id.value[1],
            (int)InMdPDUInfo.trans.number, InMdPDUInfo.src_filename);

    strcpy(FullDstFilename, InMdPDUInfo.dst_filename);
    sprintf(expEventInFail, "Incoming trans %d.%d_%d %s,CondCode %s,dest %s",
            InMdPDUInfo.trans.source_id.value[0],
            InMdPDUInfo.trans.source_id.value[1],
            (int)InMdPDUInfo.trans.number, "CANCELLED", "INACTIVITY_DETECTED",
            FullDstFilename);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF, IncomingMsgClass2InactTimerExpired: No ErrCounter");

    UtAssert_True((CF_AppData.Hk.Up.MetaCount == 1) &&
                  (CF_AppData.Hk.Up.SuccessCounter == 0) &&
                  (CF_AppData.Hk.Up.FailedCounter == 1),
                 "CF, IncomingMsgClass2InactTimerExpired: Up Telemetry data");

    UtAssert_True(CF_AppData.Hk.Chan[0].PDUsSent == 1,
                 "CF, IncomingMsgClass2InactTimerExpired: Pb Telemetry data");

    UtAssert_True((UpActQEntryCntBefore == 1) && (UpActQEntryCntAfter == 0) &&
                  (UpHistQEntryCntBefore == 0) && (UpHistQEntryCntAfter == 1),
                  "CF, IncomingMsgClass2InactTimerExpired: UpQueueEntry cnt");

    UtAssert_True(ZeroCopySendHook_FinInactTimeout_Rcvd == YES,
         "CF, IncomingMsgClass2InactTimerExpired: FIN-InactTimeout received");

    UtAssert_EventSent(CF_IN_TRANS_START_EID, CFE_EVS_INFORMATION,
                       expEventInSt, "CF, IncomingMsgClass2InactTimerExpired: "
                       "Incoming Start Event Sent");

    UtAssert_EventSent(CF_IND_INA_TIM_EXP_EID, CFE_EVS_INFORMATION,
                       expEventInact, "CF, IncomingMsgClass2InactTimerExpired: "
                       "Inactivity Event Sent");

    UtAssert_EventSent(CF_IN_TRANS_FAILED_EID, CFE_EVS_ERROR, expEventInFail,
            "CF, IncomingMsgClass2InactTimerExpired: Incoming Fail Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF, IncomingMsgClass2DataLoss
 */
void Test_CF_IncomingMsgClass2DataLoss(void)
{
    uint16              PDataLen;
    uint16              hdr_len;
    int                 i;
    uint32              offset;
    uint32              size_left;
    uint32              UpActQEntryCntBefore;
    uint32              UpHistQEntryCntBefore;
    uint32              UpActQEntryCntAfter;
    uint32              UpHistQEntryCntAfter;
    CF_Test_InPDUInfo_t InMdPDUInfo;
    CF_Test_InPDUInfo_t InFdPDUInfo;
    CF_Test_InPDUInfo_t InEofPDUInfo;
    CF_Test_InPDUMsg_t  InMdPDUMsg;
    CF_Test_InPDUMsg_t  InFdPDUMsg;
    CF_Test_InPDUMsg_t  InEofPDUMsg;
    CF_Test_InPDUMsg_t  InEofCancelPDUMsg;
    char  expEventInSt[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventInFail[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg), TRUE);
    InMdPDUInfo.pdu_type = FILE_DIR_PDU;
    InMdPDUInfo.direction = TEST_TO_RECEIVER;
    InMdPDUInfo.mode = TEST_ACK_MODE;
    InMdPDUInfo.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InMdPDUInfo.trans.source_id);
    InMdPDUInfo.trans.number = TEST_IN_TRANS_NUMBER;
    cfdp_id_from_string(TestFlightEntityId, &InMdPDUInfo.dest_id);
    InMdPDUInfo.file_transfer = YES;
    InMdPDUInfo.segmentation_control = NO;
    InMdPDUInfo.file_size = TEST_FILE_SIZE2;   /* Total file size */
    sprintf(InMdPDUInfo.src_filename, "%s%s", TestInDir1, TestInFileDataLoss);
    sprintf(InMdPDUInfo.dst_filename, "%s%s", TestInDir1, TestInFileDataLoss);

    PDataLen = 6;
    PDataLen += strlen(InMdPDUInfo.src_filename) + 1;
    PDataLen += strlen(InMdPDUInfo.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg, &InMdPDUInfo, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg, &InMdPDUInfo, hdr_len);

    InFdPDUInfo = InMdPDUInfo;
    InEofPDUInfo = InMdPDUInfo;

    /* Build CF_PPD_TO_CPD_PDU_MID (EOF PDU) */
    CFE_SB_InitMsg((void*)&InEofPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InEofPDUMsg), TRUE);
    InEofPDUInfo.pdu_type = FILE_DIR_PDU;
    InEofPDUInfo.cond_code = NO_ERROR;
    InEofPDUInfo.file_size = TEST_FILE_SIZE2;
    InEofPDUInfo.checksum = 0;

    PDataLen = 10;
    hdr_len = CF_TstUtil_GenPDUHeader(&InEofPDUMsg, &InEofPDUInfo, PDataLen);
    CF_TstUtil_BuildEOFPdu(&InEofPDUMsg, &InEofPDUInfo, hdr_len);

    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX,
                             (void *)&OS_CountSemGetIdByNameHook);

    /* To give the unit test system time for SB Msg */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void *)&CFE_SB_TimeStampMsgHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void *)&CFE_TIME_GetTimeHook);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_CREAT_INDEX,
                                 (void *)&OS_creatHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_OPEN_INDEX,
                                 (void *)&OS_openHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_READ_INDEX,
                                 (void *)&OS_readHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_WRITE_INDEX,
                                 (void *)&OS_writeHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_CLOSE_INDEX,
                                 (void *)&OS_closeHook);

    /* Return a buf pointer to send a PDU through SB */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_ZEROCOPYGETPTR_INDEX,
                              (void *)&CFE_SB_ZeroCopyGetPtrHook);

    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX,
                              (void*)&CFE_ES_PutPoolBufHook);

    ZeroCopySendHook_AckEofNoErr_Rcvd = NO;
    ZeroCopySendHook_Nak_Rcvd = NO;
    ZeroCopySendHook_AckEofCancel_Rcvd = NO;
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_ZEROCOPYSEND_INDEX,
                              (void *)&CFE_SB_ZeroCopySendHook);

    /* Execute the function being tested */
    CF_AppInit();
    CF_GetHandshakeSemIds();

    /* Incoming MD PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Incoming FD PDU */
    offset = 0;
    size_left = TEST_FILE_SIZE2;
    while (size_left > 0)
    {
        CFE_SB_InitMsg((void*)&InFdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                       sizeof(InFdPDUMsg), TRUE);
        InFdPDUInfo.pdu_type = FILE_DATA_PDU;
        InFdPDUInfo.offset = offset;
        if (size_left <= TEST_FILE_CHUNK_SIZE)
        {
            InFdPDUInfo.file_size = size_left;
            size_left = 0;
        }
        else
        {
            InFdPDUInfo.file_size = TEST_FILE_CHUNK_SIZE;
            /* Drop one file chunk for this TEST_FILE_SIZE2 */
            size_left -= (TEST_FILE_CHUNK_SIZE * 2);
            offset += (TEST_FILE_CHUNK_SIZE * 2);
        }

        PDataLen = InFdPDUInfo.file_size + 4;
        hdr_len = CF_TstUtil_GenPDUHeader(&InFdPDUMsg, &InFdPDUInfo, PDataLen);
        CF_TstUtil_BuildFDPdu(&InFdPDUMsg, &InFdPDUInfo, hdr_len);

        CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InFdPDUMsg;
        CF_AppPipe(CF_AppData.MsgPtr);
    }

    /* Incoming EOF PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InEofPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    UpActQEntryCntBefore = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt;
    UpHistQEntryCntBefore = CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt;
    CF_ShowQs();

    cfdp_cycle_each_transaction();
    cfdp_cycle_each_transaction();

    if (ZeroCopySendHook_Nak_Rcvd == YES)
    {
        CFE_SB_InitMsg((void*)&InEofCancelPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                       sizeof(InEofCancelPDUMsg), TRUE);
        memcpy(InEofCancelPDUMsg.PduContent.Content, HookOutPDU.content,
               HookOutPDU.length);
        CF_Test_PrintCmdMsg(&InEofCancelPDUMsg,
                            CFE_SB_CMD_HDR_SIZE + HookOutPDU.length);

        CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InEofCancelPDUMsg;
        CF_AppPipe(CF_AppData.MsgPtr);
    }

    cfdp_cycle_each_transaction();
    cfdp_cycle_each_transaction();

    UpActQEntryCntAfter = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt;
    UpHistQEntryCntAfter = CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt;
    CF_ShowQs();

    sprintf(expEventInSt, "Incoming trans started %d.%d_%d,dest %s",
            InMdPDUInfo.trans.source_id.value[0],
            InMdPDUInfo.trans.source_id.value[1],
            (int)InMdPDUInfo.trans.number, InMdPDUInfo.dst_filename);

    sprintf(expEventInFail, "Incoming trans %d.%d_%d %s,CondCode %s,dest %s",
            InMdPDUInfo.trans.source_id.value[0],
            InMdPDUInfo.trans.source_id.value[1],
            (int)InMdPDUInfo.trans.number, "CANCELLED", "FILESIZE_ERR",
            InMdPDUInfo.dst_filename);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF, IncomingMsgClass2DataLoss: No ErrCounter");

    UtAssert_True((CF_AppData.Hk.Up.MetaCount == 1) &&
                  (CF_AppData.Hk.Up.SuccessCounter == 0) &&
                  (CF_AppData.Hk.Up.FailedCounter == 1),
                  "CF, IncomingMsgClass2DataLoss: Up Telemetry data");

    UtAssert_True(CF_AppData.Hk.Chan[0].PDUsSent == 3,
                  "CF, IncomingMsgClass2DataLoss: Pb Telemetry data");

    UtAssert_True((UpActQEntryCntBefore == 1) && (UpActQEntryCntAfter == 0) &&
                  (UpHistQEntryCntBefore == 0) &&(UpHistQEntryCntAfter == 1),
                  "CF, IncomingMsgClass2DataLoss: UpQueueEntry cnt");

    UtAssert_True(ZeroCopySendHook_AckEofNoErr_Rcvd == YES,
                  "CF, IncomingMsgClass2DataLoss: ACK-EOF NoErr received");

    UtAssert_True(ZeroCopySendHook_Nak_Rcvd == YES,
                  "CF, IncomingMsgClass2DataLoss: NAK received");

    UtAssert_True(ZeroCopySendHook_AckEofCancel_Rcvd == YES,
                  "CF, IncomingMsgClass2DataLoss: ACK-EOF Cancel received");

    UtAssert_EventSent(CF_IN_TRANS_START_EID, CFE_EVS_INFORMATION, expEventInSt,
                    "CF, IncomingMsgClass2DataLoss: Incoming Start Event Sent");

    UtAssert_EventSent(CF_IN_TRANS_FAILED_EID, CFE_EVS_ERROR, expEventInFail,
                     "CF, IncomingMsgClass2DataLoss: Incoming Fail Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_AppMain, WakeupReqCmd
 */
void Test_CF_AppMain_WakeupReqCmd(void)
{
    uint32              PbPendQ_0_EntryCnt;
    uint32              PbActQ_0_EntryCnt;
    uint32              PbHistQ_0_EntryCnt;
    uint32              PbPendQ_1_EntryCnt;
    uint32              PbActQ_1_EntryCnt;
    uint32              PbHistQ_1_EntryCnt;
    int32               CmdPipe;
    CF_NoArgsCmd_t      WakeupCmdMsg;
    TRANSACTION         trans;
    char   TransStr1[MAX_FILE_NAME_LENGTH], TransStr2[MAX_FILE_NAME_LENGTH];
    char   TransStr3[MAX_FILE_NAME_LENGTH], TransStr4[MAX_FILE_NAME_LENGTH];
    char   TransStr5[MAX_FILE_NAME_LENGTH], TransStr6[MAX_FILE_NAME_LENGTH];
    char   SrcFilename1[MAX_FILE_NAME_LENGTH];
    char   SrcFilename2[MAX_FILE_NAME_LENGTH];
    char   SrcFilename3[MAX_FILE_NAME_LENGTH];
    char   SrcFilename4[MAX_FILE_NAME_LENGTH];
    char   SrcFilename5[MAX_FILE_NAME_LENGTH];
    char   SrcFilename6[MAX_FILE_NAME_LENGTH];
    char   expEventPbSt1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char   expEventPbFin1[CFE_EVS_MAX_MESSAGE_LENGTH];
    char   expEventPbSt2[CFE_EVS_MAX_MESSAGE_LENGTH];
    char   expEventPbFin2[CFE_EVS_MAX_MESSAGE_LENGTH];
    char   expEventPbSt3[CFE_EVS_MAX_MESSAGE_LENGTH];
    char   expEventPbFin3[CFE_EVS_MAX_MESSAGE_LENGTH];
    char   expEventPbSt4[CFE_EVS_MAX_MESSAGE_LENGTH];
    char   expEventPbFin4[CFE_EVS_MAX_MESSAGE_LENGTH];
    char   expEventPbSt5[CFE_EVS_MAX_MESSAGE_LENGTH];
    char   expEventPbFin5[CFE_EVS_MAX_MESSAGE_LENGTH];
    char   expEventPbSt6[CFE_EVS_MAX_MESSAGE_LENGTH];
    char   expEventPbFin6[CFE_EVS_MAX_MESSAGE_LENGTH];

    CmdPipe = Ut_CFE_SB_CreatePipe(CF_PIPE_NAME);

    /* Send Wakeup Request #1 */
    CFE_SB_InitMsg((void*)&WakeupCmdMsg, CF_WAKE_UP_REQ_CMD_MID,
                   sizeof(WakeupCmdMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&WakeupCmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    CF_Test_PrintCmdMsg((void*)&WakeupCmdMsg, sizeof(WakeupCmdMsg));

    /* Send Wakeup Request #2 */
    Ut_CFE_SB_AddMsgToPipe((void*)&WakeupCmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    /* Send Wakeup Request #3 to finish the remaining transactions */
    Ut_CFE_SB_AddMsgToPipe((void*)&WakeupCmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    /* To make the HandshakeSemId != CF_INVALID to send/receive PDUs */
    Ut_OSAPI_SetFunctionHook(UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX,
                             (void *)&OS_CountSemGetIdByNameHook);

    /* To give the unit test system time for SB Msg */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_TIMESTAMPMSG_INDEX,
                              (void *)&CFE_SB_TimeStampMsgHook);

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX,
                                (void *)&CFE_TIME_GetTimeHook);

    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_OPENDIR_INDEX,
                                 (void *)&OS_opendirHook);

    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CLOSEDIR_INDEX,
                                                    OS_FS_SUCCESS, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                                         UT_OSFILEAPI_CLOSEDIR_INDEX);

    /* Return invalid dir/filenames and then return
       TestPbFile3, TestPbFile2, TestPbFile1, ... */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_READDIR_INDEX,
                                 (void*)&OS_readdirHook);

    /* Return OS_FS_ERR_INVALID_FD: means that the file is not open */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX,
                               OS_FS_ERR_INVALID_FD, 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(
                               UT_OSFILEAPI_FDGETINFO_INDEX);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Give a valid file size and return success */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_STAT_INDEX,
                                 (void*)&OS_statHook);

    /* Return a buf pointer to send a PDU through SB */
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_ZEROCOPYGETPTR_INDEX,
                             (void *)&CFE_SB_ZeroCopyGetPtrHook);

    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_PUTPOOLBUF_INDEX,
                              (void*)&CFE_ES_PutPoolBufHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 4);

    /* Execute the function being tested */
    CF_AppMain();

    PbPendQ_0_EntryCnt = CF_AppData.Chan[0].PbQ[CF_PB_PENDINGQ].EntryCnt;
    PbActQ_0_EntryCnt = CF_AppData.Chan[0].PbQ[CF_PB_ACTIVEQ].EntryCnt;
    PbHistQ_0_EntryCnt = CF_AppData.Chan[0].PbQ[CF_PB_HISTORYQ].EntryCnt;

    PbPendQ_1_EntryCnt = CF_AppData.Chan[1].PbQ[CF_PB_PENDINGQ].EntryCnt;
    PbActQ_1_EntryCnt = CF_AppData.Chan[1].PbQ[CF_PB_ACTIVEQ].EntryCnt;
    PbHistQ_1_EntryCnt = CF_AppData.Chan[1].PbQ[CF_PB_HISTORYQ].EntryCnt;

    CF_ShowQs();

    /* Event for File 1 */
    sprintf(TransStr1, "%s%s", CF_AppData.Tbl->FlightEntityId, "_1");
    cfdp_trans_from_string(TransStr1, &trans);
    sprintf(SrcFilename1, "%s%s",
            CF_AppData.Tbl->OuCh[0].PollDir[0].SrcPath, TestPbFile3);
    sprintf(expEventPbSt1, "Outgoing trans started %d.%d_%d,src %s",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, SrcFilename1);

    sprintf(expEventPbFin1, "Outgoing trans success %d.%d_%d,src %s",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, SrcFilename1);

    /* Event for File 2 */
    sprintf(TransStr2, "%s%s", CF_AppData.Tbl->FlightEntityId, "_2");
    cfdp_trans_from_string(TransStr2, &trans);
    sprintf(SrcFilename2, "%s%s",
            CF_AppData.Tbl->OuCh[0].PollDir[0].SrcPath, TestPbFile2);
    sprintf(expEventPbSt2, "Outgoing trans started %d.%d_%d,src %s",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, SrcFilename2);

    sprintf(expEventPbFin2, "Outgoing trans success %d.%d_%d,src %s",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, SrcFilename2);

    /* Event for File 3 */
    sprintf(TransStr3, "%s%s", CF_AppData.Tbl->FlightEntityId, "_3");
    cfdp_trans_from_string(TransStr3, &trans);
    sprintf(SrcFilename3, "%s%s",
            CF_AppData.Tbl->OuCh[0].PollDir[0].SrcPath, TestPbFile1);
    sprintf(expEventPbSt3, "Outgoing trans started %d.%d_%d,src %s",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, SrcFilename3);

    sprintf(expEventPbFin3, "Outgoing trans success %d.%d_%d,src %s",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, SrcFilename3);

    /* Event for File 4 */
    sprintf(TransStr4, "%s%s", CF_AppData.Tbl->FlightEntityId, "_4");
    cfdp_trans_from_string(TransStr4, &trans);
    sprintf(SrcFilename4, "%s%s",
            CF_AppData.Tbl->OuCh[0].PollDir[2].SrcPath, TestPbFile4);
    sprintf(expEventPbSt4, "Outgoing trans started %d.%d_%d,src %s",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, SrcFilename4);

    sprintf(expEventPbFin4, "Outgoing trans success %d.%d_%d,src %s",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, SrcFilename4);

    /* Event for File 5 */
    sprintf(TransStr5, "%s%s", CF_AppData.Tbl->FlightEntityId, "_5");
    cfdp_trans_from_string(TransStr5, &trans);
    sprintf(SrcFilename5, "%s%s",
            CF_AppData.Tbl->OuCh[0].PollDir[2].SrcPath, TestPbFile5);
    sprintf(expEventPbSt5, "Outgoing trans started %d.%d_%d,src %s",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, SrcFilename5);

    sprintf(expEventPbFin5, "Outgoing trans success %d.%d_%d,src %s",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, SrcFilename5);

    /* Event for File 6 */
    sprintf(TransStr6, "%s%s", CF_AppData.Tbl->FlightEntityId, "_6");
    cfdp_trans_from_string(TransStr6, &trans);
    sprintf(SrcFilename6, "%s%s",
            CF_AppData.Tbl->OuCh[0].PollDir[2].SrcPath, TestPbFile6);
    sprintf(expEventPbSt6, "Outgoing trans started %d.%d_%d,src %s",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, SrcFilename6);

    sprintf(expEventPbFin6, "Outgoing trans success %d.%d_%d,src %s",
            trans.source_id.value[0], trans.source_id.value[1],
            (int)trans.number, SrcFilename6);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 0),
                  "CF_AppMain, WakeupReqCmd: No ErrCounter");

    UtAssert_True((CF_AppData.Hk.Chan[0].SuccessCounter == 6) &&
                  (CF_AppData.Hk.Chan[0].FailedCounter == 0),
                  "CF_AppMain, WakeupReqCmd: Chan Telemetry data");

    UtAssert_True((PbPendQ_0_EntryCnt == 0) && (PbActQ_0_EntryCnt == 0)
                  && (PbHistQ_0_EntryCnt == 6),
                  "CF_AppMain, WakeupReqCmd: PbQueue Chan 0 Entry cnt");

    UtAssert_True((PbPendQ_1_EntryCnt == 0) && (PbActQ_1_EntryCnt == 0)
                  && (PbHistQ_1_EntryCnt == 0),
                  "CF_AppMain, WakeupReqCmd: PbQueue Chan 1 Entry cnt");

    UtAssert_EventSent(CF_OUT_TRANS_START_EID, CFE_EVS_INFORMATION,
                  expEventPbSt1,
                  "CF_AppMain, WakeupReqCmd: PbFile1 Start Event Sent");

    UtAssert_EventSent(CF_OUT_TRANS_OK_EID, CFE_EVS_INFORMATION,
                  expEventPbFin1,
                  "CF_AppMain, WakeupReqCmd: PbFile1 Finish Event Sent");

    UtAssert_EventSent(CF_OUT_TRANS_START_EID, CFE_EVS_INFORMATION,
                  expEventPbSt2,
                  "CF_AppMain, WakeupReqCmd: PbFile2 Start Event Sent");

    UtAssert_EventSent(CF_OUT_TRANS_OK_EID, CFE_EVS_INFORMATION,
                  expEventPbFin2,
                  "CF_AppMain, WakeupReqCmd: PbFile2 Finish Event Sent");

    UtAssert_EventSent(CF_OUT_TRANS_START_EID, CFE_EVS_INFORMATION,
                  expEventPbSt3,
                  "CF_AppMain, WakeupReqCmd: PbFile3 Start Event Sent");

    UtAssert_EventSent(CF_OUT_TRANS_OK_EID, CFE_EVS_INFORMATION,
                  expEventPbFin3,
                  "CF_AppMain, WakeupReqCmd: PbFile3 Finish Event Sent");

    UtAssert_EventSent(CF_OUT_TRANS_START_EID, CFE_EVS_INFORMATION,
                  expEventPbSt4,
                  "CF_AppMain, WakeupReqCmd: PbFile4 Start Event Sent");

    UtAssert_EventSent(CF_OUT_TRANS_OK_EID, CFE_EVS_INFORMATION,
                  expEventPbFin4,
                  "CF_AppMain, WakeupReqCmd: PbFile4 Finish Event Sent");

    UtAssert_EventSent(CF_OUT_TRANS_START_EID, CFE_EVS_INFORMATION,
                  expEventPbSt5,
                  "CF_AppMain, WakeupReqCmd: PbFile5 Start Event Sent");

    UtAssert_EventSent(CF_OUT_TRANS_OK_EID, CFE_EVS_INFORMATION,
                  expEventPbFin5,
                  "CF_AppMain, WakeupReqCmd: PbFile5 Finish Event Sent");

    UtAssert_EventSent(CF_OUT_TRANS_START_EID, CFE_EVS_INFORMATION,
                  expEventPbSt6,
                  "CF_AppMain, WakeupReqCmd: PbFile6 Start Event Sent");

    UtAssert_EventSent(CF_OUT_TRANS_OK_EID, CFE_EVS_INFORMATION,
                  expEventPbFin6,
                  "CF_AppMain, WakeupReqCmd: PbFile6 Finish Event Sent");

    CF_ResetEngine();
}




/**
 * Test CF_AppMain, WakeupReqCmdPollingEnabled
 */
void Test_CF_AppMain_WakeupReqCmdPollingEnabled(void)
{
}


/**
 * Test CF_AppMain, WakeupReqCmdInvLen
 */
void Test_CF_AppMain_WakeupReqCmdInvLen(void)
{
    int32               CmdPipe;
    CF_NoArgsCmd_t      WakeupCmdMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CmdPipe = Ut_CFE_SB_CreatePipe(CF_PIPE_NAME);

    CFE_SB_InitMsg((void*)&WakeupCmdMsg, CF_WAKE_UP_REQ_CMD_MID,
                   sizeof(WakeupCmdMsg) + 5, TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&WakeupCmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    CF_Test_PrintCmdMsg((void*)&WakeupCmdMsg, sizeof(WakeupCmdMsg));

    /* Execute the function being tested */
    CF_AppMain();

    sprintf(expEvent, "Cmd Msg with Bad length Rcvd: ID = 0x%X, CC = %d, "
            "Exp Len = %d, Len = %d", CF_WAKE_UP_REQ_CMD_MID, 0,
            sizeof(WakeupCmdMsg), sizeof(WakeupCmdMsg) + 5);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.CmdCounter == 0) &&
                  (CF_AppData.Hk.ErrCounter == 1),
                  "CF_AppMain, WakeupReqCmdInvLen");

    UtAssert_EventSent(CF_CMD_LEN_ERR_EID, CFE_EVS_ERROR, expEvent,
                       "CF_AppMain, WakeupReqCmdInvLen: Event Sent");
}


/**
 * Test CF_SendPDUToEngine, InPDUInvalidHeaderLen
 */
void Test_CF_SendPDUToEngine_InPDUInvalidHeaderLen(void)
{
    uint8               RequestedHdrSize;
    uint16              PDataLen;
    CF_Test_InPDUInfo_t InPDUInfo;
    CF_Test_InPDUMsg_t  InMdPDUMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg), TRUE);
    InPDUInfo.pdu_type = FILE_DIR_PDU;
    InPDUInfo.direction = TEST_TO_RECEIVER;
    InPDUInfo.mode = TEST_UNACK_MODE;
    InPDUInfo.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo.trans.source_id);
    InPDUInfo.trans.source_id.length = MAX_ID_LENGTH + 1;
    InPDUInfo.trans.number = TEST_IN_TRANS_NUMBER;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo.dest_id);
    InPDUInfo.dest_id.length = MAX_ID_LENGTH + 1;
    InPDUInfo.file_transfer = YES;
    InPDUInfo.segmentation_control = NO;
    InPDUInfo.file_size = TEST_FILE_SIZE1;

    PDataLen = 0;
    CF_TstUtil_GenPDUHeader(&InMdPDUMsg, &InPDUInfo, PDataLen);

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    RequestedHdrSize = CF_PDUHDR_FIXED_FIELD_BYTES +
                       (InPDUInfo.trans.source_id.length * 2) +
                       TEST_TRANS_SEQ_NUM_LENGTH;
    sprintf(expEvent,
            "PDU Rcv Error:PDU Hdr illegal size - %d, must be %d bytes",
            RequestedHdrSize, CF_PDU_HDR_BYTES);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.App.PDUsReceived == 1) &&
                  (CF_AppData.Hk.App.PDUsRejected == 1),
                  "CF_SendPDUToEngine, InPDUInvalidHeaderLen");

    UtAssert_EventSent(CF_PDU_RCV_ERR1_EID, CFE_EVS_ERROR, expEvent,
                  "CF_SendPDUToEngine, InPDUInvalidHeaderLen: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_SendPDUToEngine, InPDUInvalidDataLenTooSmall
 */
void Test_CF_SendPDUToEngine_InPDUInvalidDataLenTooSmall(void)
{
    uint16              PDataLen;
    CF_Test_InPDUInfo_t InPDUInfo;
    CF_Test_InPDUMsg_t  InMdPDUMsg;
    char  expEventCfdp[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg), TRUE);
    InPDUInfo.pdu_type = FILE_DIR_PDU;
    InPDUInfo.direction = TEST_TO_RECEIVER;
    InPDUInfo.mode = TEST_UNACK_MODE;
    InPDUInfo.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo.trans.source_id);
    InPDUInfo.trans.number = TEST_IN_TRANS_NUMBER;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo.dest_id);
    InPDUInfo.file_transfer = YES;
    InPDUInfo.segmentation_control = NO;
    InPDUInfo.file_size = TEST_FILE_SIZE1;

    PDataLen = 0;
    CF_TstUtil_GenPDUHeader(&InMdPDUMsg, &InPDUInfo, PDataLen);

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    sprintf(expEventCfdp, "%s",
            "cfdp_engine: PDU rejected due to data-field-length (0); "
            "must be > 0.");

    sprintf(expEvent, "%s",
            "cfdp_give_pdu returned error in CF_SendPDUToEngine");

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.App.PDUsReceived == 1) &&
                  (CF_AppData.Hk.App.PDUsRejected == 1),
                  "CF_SendPDUToEngine, InPDUInvalidDataLenTooSmall");

    UtAssert_EventSent(CF_CFDP_ENGINE_ERR_EID, CFE_EVS_ERROR, expEventCfdp,
             "CF_SendPDUToEngine, InPDUInvalidDataLenTooSmall: Event Sent");

    UtAssert_EventSent(CF_PDU_RCV_ERR3_EID, CFE_EVS_ERROR, expEvent,
             "CF_SendPDUToEngine, InPDUInvalidDataLenTooSmall: Event Sent");

    CF_ResetEngine();
}


/**
 * Test CF_SendPDUToEngine, InPDUInvalidDataLenTooBig
 */
void Test_CF_SendPDUToEngine_InPDUInvalidDataLenTooBig(void)
{
    uint16              PDataLen;
    CF_Test_InPDUInfo_t InPDUInfo;
    CF_Test_InPDUMsg_t  InMdPDUMsg;
    char  expEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build CF_PPD_TO_CPD_PDU_MID (MD PDU) */
    CFE_SB_InitMsg((void*)&InMdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg), TRUE);
    InPDUInfo.pdu_type = FILE_DIR_PDU;
    InPDUInfo.direction = TEST_TO_RECEIVER;
    InPDUInfo.mode = TEST_UNACK_MODE;
    InPDUInfo.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo.trans.source_id);
    InPDUInfo.trans.number = TEST_IN_TRANS_NUMBER;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo.dest_id);
    InPDUInfo.file_transfer = YES;
    InPDUInfo.segmentation_control = NO;
    InPDUInfo.file_size = TEST_FILE_SIZE1;

    PDataLen = MAX_DATA_LENGTH;
    CF_TstUtil_GenPDUHeader(&InMdPDUMsg, &InPDUInfo, PDataLen);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.App.PDUsReceived == 1) &&
                  (CF_AppData.Hk.App.PDUsRejected == 1),
                  "CF_SendPDUToEngine, InPDUInvalidDataLenTooBig");

    CF_ResetEngine();
}


/**
 * Test CF_SendPDUToEngine, Nominal
 */
void Test_CF_SendPDUToEngine_Nominal(void)
{
    uint16              PDataLen;
    uint16              hdr_len;
    uint32              UpActQEntryCntMD;
    uint32              UpActQEntryCntFD;
    uint32              UpActQEntryCntEOF;
    uint32              UpHistQEntryCntMD;
    uint32              UpHistQEntryCntFD;
    uint32              UpHistQEntryCntEOF;
    CF_Test_InPDUInfo_t InPDUInfo;
    CF_Test_InPDUMsg_t  InMdPDUMsg;
    CF_Test_InPDUMsg_t  InFdPDUMsg;
    CF_Test_InPDUMsg_t  InEofPDUMsg;
    char  expEventMD[CFE_EVS_MAX_MESSAGE_LENGTH];
    char  expEventFinish[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Build Incoming MD PDU Msg */
    CFE_SB_InitMsg((void*)&InMdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InMdPDUMsg), TRUE);
    InPDUInfo.pdu_type = FILE_DIR_PDU;
    InPDUInfo.direction = TEST_TO_RECEIVER;
    InPDUInfo.mode = TEST_UNACK_MODE;
    InPDUInfo.use_crc = NO;
    cfdp_id_from_string(TestInSrcEntityId1, &InPDUInfo.trans.source_id);
    InPDUInfo.trans.number = TEST_IN_TRANS_NUMBER;
    cfdp_id_from_string(TestFlightEntityId, &InPDUInfo.dest_id);
    InPDUInfo.file_transfer = YES;
    InPDUInfo.segmentation_control = NO;
    InPDUInfo.file_size = TEST_FILE_SIZE1;  /* Total file size */
    sprintf(InPDUInfo.src_filename, "%s%s", TestInDir0, TestInFile1);
    sprintf(InPDUInfo.dst_filename, "%s%s", TestInDir0, TestInFile1);

    PDataLen = 6;
    PDataLen += strlen(InPDUInfo.src_filename) + 1;
    PDataLen += strlen(InPDUInfo.dst_filename) + 1;
    hdr_len = CF_TstUtil_GenPDUHeader(&InMdPDUMsg, &InPDUInfo, PDataLen);
    CF_TstUtil_BuildMDPdu(&InMdPDUMsg, &InPDUInfo, hdr_len);

    /* Build Incoming FD PDU Msg */
    CFE_SB_InitMsg((void*)&InFdPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InFdPDUMsg), TRUE);
    InPDUInfo.pdu_type = FILE_DATA_PDU;
    InPDUInfo.offset = 0;
    InPDUInfo.file_size = TEST_FILE_SIZE1;  /* File size for this FD PDU */

    PDataLen = InPDUInfo.file_size + 4;
    hdr_len = CF_TstUtil_GenPDUHeader(&InFdPDUMsg, &InPDUInfo, PDataLen);
    CF_TstUtil_BuildFDPdu(&InFdPDUMsg, &InPDUInfo, hdr_len);

    /* Build Incoming EOF PDU Msg */
    CFE_SB_InitMsg((void*)&InEofPDUMsg, CF_PPD_TO_CPD_PDU_MID,
                   sizeof(InEofPDUMsg), TRUE);
    InPDUInfo.pdu_type = FILE_DIR_PDU;
    InPDUInfo.cond_code = NO_ERROR;
    InPDUInfo.file_size = TEST_FILE_SIZE1;  /* Total file size */
    InPDUInfo.checksum = 0;                /* Not supported */

    PDataLen = 10;
    hdr_len = CF_TstUtil_GenPDUHeader(&InEofPDUMsg, &InPDUInfo, PDataLen);
    CF_TstUtil_BuildEOFPdu(&InEofPDUMsg, &InPDUInfo, hdr_len);

    /* Return the offset pointer of the CF_AppData.Mem.Partition */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_GETPOOLBUF_INDEX,
                              (void*)&CFE_ES_GetPoolBufHook);

    /* Execute the function being tested */
    CF_AppInit();

    /* Incoming MD PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InMdPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    UpActQEntryCntMD = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt;
    UpHistQEntryCntMD = CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt;

    /* Incoming FD PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InFdPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    UpActQEntryCntFD = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt;
    UpHistQEntryCntFD = CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt;

    /* Incoming EOF PDU */
    CF_AppData.MsgPtr = (CFE_SB_MsgPtr_t)&InEofPDUMsg;
    CF_AppPipe(CF_AppData.MsgPtr);

    UpActQEntryCntEOF = CF_AppData.UpQ[CF_UP_ACTIVEQ].EntryCnt;
    UpHistQEntryCntEOF = CF_AppData.UpQ[CF_UP_HISTORYQ].EntryCnt;

    CF_ShowQs();

    sprintf(expEventMD, "Incoming trans started %d.%d_%d,dest %s",
            InPDUInfo.trans.source_id.value[0],
            InPDUInfo.trans.source_id.value[1],
            (int)InPDUInfo.trans.number, InPDUInfo.dst_filename);

    sprintf(expEventFinish, "Incoming trans success %d.%d_%d,dest %s",
            InPDUInfo.trans.source_id.value[0],
            InPDUInfo.trans.source_id.value[1],
            (int)InPDUInfo.trans.number, InPDUInfo.dst_filename);

    /* Verify results */
    UtAssert_True((CF_AppData.Hk.App.PDUsReceived == 3) &&
                  (CF_AppData.Hk.App.PDUsRejected == 0),
                  "CF_SendPDUToEngine, Nominal: PDU Received cnt");

    UtAssert_True(CF_AppData.Hk.Up.MetaCount == 1,
                  "CF_SendPDUToEngine, Nominal: Received MetaCnt");

    UtAssert_True((UpActQEntryCntMD == 1) && (UpHistQEntryCntMD == 0)
               && (UpActQEntryCntFD == 1) && (UpHistQEntryCntFD == 0)
               && (UpActQEntryCntEOF == 0) && (UpHistQEntryCntEOF == 1),
               "CF_SendPDUToEngine, Nominal: QEntryCnt");

    UtAssert_EventSent(CF_IN_TRANS_START_EID, CFE_EVS_INFORMATION,
                  expEventMD,
                  "CF_SendPDUToEngine, Nominal: Meta Data Event Sent");

    UtAssert_EventSent(CF_IN_TRANS_OK_EID, CFE_EVS_INFORMATION,
                  expEventFinish,
                  "CF_SendPDUToEngine, Nominal: Finish Event Sent");

    CF_ResetEngine();
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
    UtTest_Add(Test_CF_AppInit_Fail_SendEvent,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppInit_Fail_SendEvent");
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
    UtTest_Add(Test_CF_AppMain_Fail_SemGetIdByName,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppMain_Fail_SemGetIdByName");
    UtTest_Add(Test_CF_AppMain_Fail_AcquireConfigPtrs,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppMain_Fail_AcquireConfigPtrs");
    UtTest_Add(Test_CF_AppMain_Fail_PipeError,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppMain_Fail_PipeError");

    UtTest_Add(Test_CF_AppMain_Nominal,
               CF_Test_Setup, CF_Test_TearDown,
               "Test_CF_AppMain_Nominal");

#if 0
    UtTest_Add(Test_CF_AppMain_IncomingMsgClass1,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppMain_IncomingMsgClass1");
    UtTest_Add(Test_CF_IncomingMsgClass2Nominal,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_IncomingMsgClass2Nominal");
    UtTest_Add(Test_CF_IncomingMsgClass2InactTimerExpired,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_IncomingMsgClass2InactTimerExpired");
    UtTest_Add(Test_CF_IncomingMsgClass2DataLoss,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_IncomingMsgClass2DataLoss");
#endif

    UtTest_Add(Test_CF_AppMain_WakeupReqCmd,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppMain_WakeupReqCmd");
    UtTest_Add(Test_CF_AppMain_WakeupReqCmdInvLen,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_AppMain_WakeupReqCmdInvLen");

    UtTest_Add(Test_CF_SendPDUToEngine_InPDUInvalidHeaderLen,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_SendPDUToEngine_InPDUInvalidHeaderLen");
    UtTest_Add(Test_CF_SendPDUToEngine_InPDUInvalidDataLenTooSmall,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_SendPDUToEngine_InPDUInvalidDataLenTooSmall");
    UtTest_Add(Test_CF_SendPDUToEngine_InPDUInvalidDataLenTooBig,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_SendPDUToEngine_InPDUInvalidDataLenTooBig");
#if 0
    UtTest_Add(Test_CF_SendPDUToEngine_Nominal,
               CF_Test_SetupUnitTest, CF_Test_TearDown,
               "Test_CF_SendPDUToEngine_Nominal");
#endif
}
