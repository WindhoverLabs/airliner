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

#include "ci_app_test.h"
#include "ci_app.h"
#include "ci_test_utils.h"
#include "ci_mock_custom.h"
#include "ci_version.h"

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
 * Tests for CI_InitEvent()
 **************************************************************************/

/**
 * Test CI_InitEvent() with failed CFE_EVS_Register
 */
void Test_CI_InitEvent_Fail_Register(void)
{
    /* Set a fail result for EVS */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EVENTS_SERVICE | CFE_EVS_NOT_IMPLEMENTED;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = CI_InitEvent();

    /* Verify results */
    UtAssert_True (result == expected, "InitEvent, failed EVS Register");
}


/**************************************************************************
 * Tests for CI_InitPipe()
 **************************************************************************/
/**
 * Test CI_InitPipe(), fail SCH CFE_SB_CreatePipe
 */
void Test_CI_InitPipe_Fail_CreateSCHPipe(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = CI_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create SCH pipe");
}


/**
 * Test CI_InitPipe(), fail CFE_SB_SubscribeEx for wakeup
 */
void Test_CI_InitPipe_Fail_SubscribeWakeup(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    result = CI_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for wakeup");
}


/**
 * Test CI_InitPipe(), fail CFE_SB_SubscribeEx for sendhk
 */
void Test_CI_InitPipe_Fail_SubscribeSendHK(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 2);

    /* Execute the function being tested */
    result = CI_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for sendhk");
}


/**
 * Test CI_InitPipe(), fail CFE_SB_SubscribeEx for ProcessTimeouts
 */
void Test_CI_InitPipe_Fail_SubscribeProcessTimeouts(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 3);

    /* Execute the function being tested */
    result = CI_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for ProcessTimeouts");
}


/**
 * Test CI_InitPipe(), fail CFE_SB_SubscribeEx for IngestCommands
 */
void Test_CI_InitPipe_Fail_SubscribeIngestCommands(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 4);

    /* Execute the function being tested */
    result = CI_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_SubscribeEx for IngestCommands");
}


/**
 * Test CI_InitPipe(), fail CMD CFE_SB_CreatePipe
 */
void Test_CI_InitPipe_Fail_CreateCMDPipe(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = CI_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create CMD pipe");
}


/**
 * Test CI_InitPipe(), fail CFE_SB_Subscribe for CMD msg
 */
void Test_CI_InitPipe_Fail_SubscribeCMD(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = CI_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitPipe, fail CFE_SB_Subscribe for CMD");
}


/**************************************************************************
 * Tests for CI_InitData()
 **************************************************************************/
/**
 * Test CI_InitData()
 */
void Test_CI_InitData(void)
{
    /* Set a fail result */
    int32 result = -1;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = CI_InitData();

    /* Verify results */
    UtAssert_True (result == expected, "InitData");
}


/**************************************************************************
 * Tests for CI_InitApp()
 **************************************************************************/
/**
 * Test CI_InitApp(), fail init event
 */
void Test_CI_InitApp_Fail_InitEvent(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = CI_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init event");
}


/**
 * Test CI_InitApp(), fail init pipe
 */
void Test_CI_InitApp_Fail_InitPipe(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = CI_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init pipe");
}


/**
 * Test CI_InitApp(), fail init data.
 * NOTE: no current way to fail CI_InitData() in default
 */
void Test_CI_InitApp_Fail_InitData(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = CI_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init data");
}


/**
 * Test CI_InitApp(), fail init config table
 */
void Test_CI_InitApp_Fail_InitConfigTbl(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_NAME;

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = CI_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init config table");
}


/**
 * Test CI_InitApp(), Nominal
 */
void Test_CI_InitApp_Nominal(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = CI_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, nominal");
}


/**************************************************************************
 * Tests for CI_CleanupCallback()
 **************************************************************************/
/**
 * Test CI_CleanupCallback(), Nominal
 * NOTE: empty function, so nothing to assert
 */
void Test_CI_CleanupCallback(void)
{
    /* Execute the function being tested */
    CI_CleanupCallback();
}



/**************************************************************************
 * Tests for CI_AppMain()
 **************************************************************************/
/**
 * Test CI AppMain(), WriteToSysLogHook
 */
int32 Test_CI_AppMain_WriteToSysLogHook(const char *StringPtr, ...)
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
 * Test CI_AppMain(), Fail RegisterApp
 */
void Test_CI_AppMain_Fail_RegisterApp(void)
{
    /* fail the register app */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX,
                            CFE_ES_ERR_APP_REGISTER, 1);

    WriteToSysLog_HookCalledCnt = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_WRITETOSYSLOG_INDEX,
                              (void*)&Test_CI_AppMain_WriteToSysLogHook);

    /* Execute the function being tested */
    CI_AppMain();

    /* Verify results */
    UtAssert_True(WriteToSysLog_HookCalledCnt == 1,
                  "AppMain, Fail RegisterApp");
}


/**
 * Test CI_AppMain(), Fail InitApp
 */
void Test_CI_AppMain_Fail_InitApp(void)
{
    /* fail the register app */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX,
                             CFE_EVS_APP_NOT_REGISTERED, 1);

    WriteToSysLog_HookCalledCnt = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_WRITETOSYSLOG_INDEX,
                              (void*)&Test_CI_AppMain_WriteToSysLogHook);

    /* Execute the function being tested */
    CI_AppMain();

    /* Verify results */
    UtAssert_True(WriteToSysLog_HookCalledCnt == 3,
                  "AppMain, Fail InitApp");
}


/**
 * Test CI_AppMain(), Fail AcquireConfigPtrs
 */
void Test_CI_AppMain_Fail_AcquireConfigPtrs(void)
{
    char  expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX,
                             CFE_TBL_ERR_INVALID_HANDLE, 2);

    /* Execute the function being tested */
    CI_AppMain();

    sprintf(expEventText, "Failed to get Timeout table's address (0x%08X)",
                          (unsigned int)CFE_TBL_ERR_INVALID_HANDLE);

    /* Verify results */
    UtAssert_EventSent(CI_TIMEOUT_TABLE_ERR_EID, CFE_EVS_ERROR,
                       expEventText, "AppMain, Fail AcquireConfigPtrs");
}


/**
 * Test CI_AppMain(), Invalid Schedule Message
 */
void Test_CI_AppMain_InvalidSchMessage(void)
{
    char  expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will the emulate the behavior of receiving a
       invalid SCH message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, 0, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    CI_AppMain();

    sprintf(expEventText, "Recvd invalid SCH msgId (0x%04X)", 0);

    /* Verify results */
    UtAssert_EventSent(CI_MSGID_ERR_EID, CFE_EVS_ERROR,
                       expEventText, "AppMain, Invalid Schedule Message");
}


/**
 * Test CI_AppMain(), No Schedule Message
 */
void Test_CI_AppMain_NoSchMessage(void)
{
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_NO_MESSAGE, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    CI_AppMain();
}


/**
 * Hook to support: CI_AppMain(), Nominal - SendHK
 */
int32 Test_CI_AppMain_Nominal_SendHK_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    unsigned char*     pBuff = NULL;
    uint16             msgLen = 0;
    int                i = 0;
    CFE_SB_MsgId_t     MsgId;
    time_t             localTime;
    struct tm          *loc_time;
    CFE_TIME_SysTime_t TimeFromMsg;
    CI_HkTlm_t         HkMsg;

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
    localTime = CI_Test_GetTimeFromMsg(TimeFromMsg);
    loc_time = localtime(&localTime);
    printf("TimeFromMessage: %s", asctime(loc_time));

    MsgId = (((MsgPtr->Hdr).StreamId[0] << 8) + ((MsgPtr->Hdr).StreamId[1]));
    switch (MsgId)
    {
        case CI_HK_TLM_MID:
        {
            SendHK_SendMsgHook_MsgId = CI_HK_TLM_MID;
            CFE_PSP_MemCpy((void*)&HkMsg, (void*)MsgPtr, sizeof(HkMsg));

            printf("Sent CI_HK_TLM_MID:\n");
            printf("usCmdCnt: %u\n", CI_AppData.HkTlm.usCmdCnt);
            printf("usCmdErrCnt: %u\n", CI_AppData.HkTlm.usCmdErrCnt);
            printf("IngestMsgCount: %u\n", CI_AppData.HkTlm.IngestMsgCount);
            printf("IngestErrorCount: %u\n",
                    CI_AppData.HkTlm.IngestErrorCount);
            printf("IngestActive: %u\n", CI_AppData.HkTlm.IngestActive);
            printf("IngestActiveCount: %lu\n",
                    CI_AppData.HkTlm.IngestActiveCount);
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
 * Test CI_AppMain(), Schedule Message Timeout
 */
void Test_CI_AppMain_SchMessageTimeout(void)
{
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_TIME_OUT, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Used to verify HK was transmitted correctly. */
    SendHK_SendMsgHook_MsgId = 0;
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                     (void*)&Test_CI_AppMain_Nominal_SendHK_SendMsgHook);

    /* Execute the function being tested */
    CI_AppMain();

    /* Verify results */
    UtAssert_True (SendHK_SendMsgHook_MsgId == CI_HK_TLM_MID,
                   "AppMain, Schedule Message Timeout");
}


/**
 * Test CI_AppMain(), Schedule Pipe Error
 */
void Test_CI_AppMain_SchPipeError(void)
{
    char   expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_PIPE_RD_ERR, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    CI_AppMain();

    sprintf(expEventText, "SCH pipe read error (0x%08X), app will exit",
                          (unsigned int)CFE_SB_PIPE_RD_ERR);

    /* Verify results */
    UtAssert_EventSent(CI_PIPE_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "AppMain, Schedule Pipe Error Event Sent");
}


/**
 * Test CI_AppMain(), Nominal - SendHK
 */
void Test_CI_AppMain_Nominal_SendHK(void)
{
    /* The following will emulate the behavior of receiving a SCH message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, CI_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Used to verify HK was transmitted correctly. */
    SendHK_SendMsgHook_MsgId = 0;
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                       (void*)&Test_CI_AppMain_Nominal_SendHK_SendMsgHook);

    /* Execute the function being tested */
    CI_AppMain();

    /* Verify results */
    UtAssert_True (SendHK_SendMsgHook_MsgId == CI_HK_TLM_MID,
                   "AppMain_Nominal_SendHK");

}


/**
 * Test CI_AppMain(), Nominal - Wakeup
 */
void Test_CI_AppMain_Nominal_Wakeup(void)
{
    /* The following will emulate the behavior of receiving a SCH message
       to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, CI_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    CI_AppMain();
}


/**
 * Test CI_AppMain(), Nominal - ProcessTimeouts
 */
void Test_CI_AppMain_Nominal_ProcessTimeouts(void)
{
    /* The following will emulate the behavior of receiving a SCH message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX,
                            CI_PROCESS_TIMEOUTS_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    CI_AppMain();
}


/**
 * Test CI_AppMain(), Nominal - IngestCommands
 */
void Test_CI_AppMain_Nominal_IngestCommands(void)
{
    char expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];
    int  cntIngestCmds = 0;

    /* The following will emulate the behavior of receiving a SCH message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX,
                            CI_INGEST_COMMANDS_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    READ_MSG_RET = CI_CMD;
    cntIngestCmds = 10;
    cntReadMessages = cntIngestCmds;

    /* Execute the function being tested */
    CI_AppMain();

    sprintf(expEventText, "Recvd NOOP cmd (%u), Version %d.%d.%d.%d",
                          CI_NOOP_CC, CI_MAJOR_VERSION, CI_MINOR_VERSION,
                          CI_REVISION, CI_MISSION_REV);

    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == (cntIngestCmds + 2),
                  "AppMain, Nominal - IngestCommands");
    UtAssert_EventSent(CI_CMD_INF_EID, CFE_EVS_INFORMATION, expEventText,
                       "AppMain, Nominal - IngestCommands Event Sent");
}


/**
 * Test CI_ValidateCmd(), Bad CCSDS Version
 */
void Test_CI_ValidateCmd_Bad_CCSDS(void)
{
	boolean		  	retCode = FALSE;
	CI_NoArgCmd_t 	cmd;
	uint32  		MsgSize = sizeof(cmd);
	CFE_SB_MsgPtr_t CmdMsgPtr;

	/* Create CFE_SB_Msg_t */
	CFE_SB_InitMsg(&cmd, CI_CMD_MID, MsgSize, TRUE);
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmd;

	/* Set to cause to fail */
	CCSDS_WR_VERS(CmdMsgPtr->Hdr, 1);

	/* Execute the function being tested */
	retCode = CI_ValidateCmd(CmdMsgPtr, MsgSize);

	/* Verify results */
	UtAssert_True(retCode==FALSE,"Valid = False");
}

/**
 * Test CI_ValidateCmd(), No Secondary Header
 */
void Test_CI_ValidateCmd_No_SecHdr(void)
{
	boolean		  	retCode = FALSE;
	CI_NoArgCmd_t 	cmd;
	uint32  		MsgSize = sizeof(cmd);
	CFE_SB_MsgPtr_t CmdMsgPtr;

	/* Create CFE_SB_Msg_t */
	CFE_SB_InitMsg(&cmd, CI_CMD_MID, MsgSize, TRUE);
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmd;

	/* Set to cause to fail */
	CCSDS_WR_SHDR(CmdMsgPtr->Hdr, 0);

	/* Execute the function being tested */
	retCode = CI_ValidateCmd(CmdMsgPtr, MsgSize);

	/* Verify results */
	UtAssert_True(retCode==FALSE,"Valid = False");
}

/**
 * Test CI_ValidateCmd(), Telemetry Packet
 */
void Test_CI_ValidateCmd_Not_Cmd(void)
{
	boolean		  	retCode = FALSE;
	CI_NoArgCmd_t 	cmd;
	uint32  		MsgSize = sizeof(cmd);
	CFE_SB_MsgPtr_t CmdMsgPtr;

	/* Create CFE_SB_Msg_t */
	CFE_SB_InitMsg(&cmd, CI_CMD_MID, MsgSize, TRUE);
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmd;

	/* Set to cause to fail */
	CCSDS_WR_TYPE(CmdMsgPtr->Hdr, 0);

	/* Execute the function being tested */
	retCode = CI_ValidateCmd(CmdMsgPtr, MsgSize);

	/* Verify results */
	UtAssert_True(retCode==TRUE,"Valid = True");
}

/**
 * Test CI_ValidateCmd(), Invalid Length
 */
void Test_CI_ValidateCmd_Inv_Len(void)
{
	boolean		  	retCode = FALSE;
	CI_NoArgCmd_t 	cmd;
	uint32  		MsgSize = sizeof(cmd);
	CFE_SB_MsgPtr_t CmdMsgPtr;

	/* Create CFE_SB_Msg_t */
	CFE_SB_InitMsg(&cmd, CI_CMD_MID, MsgSize, TRUE);
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmd;

	/* Set to cause to fail */
	MsgSize++;

	/* Execute the function being tested */
	retCode = CI_ValidateCmd(CmdMsgPtr, MsgSize);

	/* Verify results */
	UtAssert_True(retCode==FALSE,"Valid = False");
}

/**
 * Test CI_ValidateCmd(), Checksum Invalid
 */
void Test_CI_ValidateCmd_Checksum_Invalid(void)
{
	boolean		  	retCode = FALSE;
	CI_NoArgCmd_t 	cmd;
	uint32  		MsgSize = sizeof(cmd);
	CFE_SB_MsgPtr_t CmdMsgPtr;
	CCSDS_CmdPkt_t	*cmdPkt = 0;
	uint8 			CheckSum;

	/* Create CFE_SB_Msg_t */
	CFE_SB_InitMsg(&cmd, CI_CMD_MID, MsgSize, TRUE);
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmd;
	cmdPkt = (CCSDS_CmdPkt_t *)CmdMsgPtr;

	/* Set to cause to fail */
	CheckSum = CCSDS_ComputeCheckSum(cmdPkt);
	CheckSum++;
	CCSDS_WR_CHECKSUM(cmdPkt->SecHdr, CheckSum);

	/* Execute the function being tested */
	retCode = CI_ValidateCmd(CmdMsgPtr, MsgSize);

	/* Verify results */
	UtAssert_True(retCode==FALSE,"Valid = False");
}

/**
 * Test CI_ValidateCmd(), Checksum Valid
 */
void Test_CI_ValidateCmd_Checksum_Valid(void)
{
	boolean		  	retCode = FALSE;
	CI_NoArgCmd_t 	cmd;
	uint32  		MsgSize = sizeof(cmd);
	CFE_SB_MsgPtr_t CmdMsgPtr;
	CCSDS_CmdPkt_t	*cmdPkt = 0;

	/* Create CFE_SB_Msg_t */
	CFE_SB_InitMsg(&cmd, CI_CMD_MID, MsgSize, TRUE);
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmd;
	cmdPkt = (CCSDS_CmdPkt_t *)CmdMsgPtr;

	/* Set to cause to pass */
	CCSDS_LoadCheckSum(cmdPkt);

	/* Execute the function being tested */
	retCode = CI_ValidateCmd(CmdMsgPtr, MsgSize);

	/* Verify results */
	UtAssert_True(retCode==TRUE,"Valid = TRUE");
}

/**
 * Test CI_ValidateCmd(), No Checksum and Not Required
 */
void Test_CI_ValidateCmd_No_Checksum_Go(void)
{
	boolean		  	retCode = FALSE;
	CI_NoArgCmd_t 	cmd;
	uint32  		MsgSize = sizeof(cmd);
	CFE_SB_MsgPtr_t CmdMsgPtr;

	/* Create CFE_SB_Msg_t */
	CFE_SB_InitMsg(&cmd, CI_CMD_MID, MsgSize, TRUE);
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmd;

	/* Execute the function being tested */
	retCode = CI_ValidateCmd(CmdMsgPtr, MsgSize);

	/* Verify results */
	UtAssert_True(retCode==TRUE,"Valid = TRUE");
}

/**
 * Test CI_ValidateCmd(), No Checksum and Required
 * NOTE: This will not pass unless macro is changed
 * in the mission config.
 */
void Test_CI_ValidateCmd_No_Checksum_NoGo(void)
{
	boolean		  	retCode = FALSE;
	CI_NoArgCmd_t 	cmd;
	uint32  		MsgSize = sizeof(cmd);
	CFE_SB_MsgPtr_t CmdMsgPtr;

	/* Create CFE_SB_Msg_t */
	CFE_SB_InitMsg(&cmd, CI_CMD_MID, MsgSize, TRUE);
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmd;

	/* Execute the function being tested */
	retCode = CI_ValidateCmd(CmdMsgPtr, MsgSize);

	/* Verify results */
	//UtAssert_True(retCode==FALSE,"Valid = False");
}

/**
 * Test CI_GetCmdAuthorized(), Unregistered Optimistic
 */
void Test_CI_GetCmdAuthorized_Unreg_Opt(void)
{
	boolean		  		retCode = FALSE;
	CI_CmdAuthData_t 	cmd;
	uint32  			MsgSize = sizeof(cmd);
	CFE_SB_MsgPtr_t 	CmdMsgPtr;

	/* Create CFE_SB_Msg_t */
	CFE_SB_InitMsg(&cmd, 0, MsgSize, TRUE);
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmd;

	/* Set to cause to pass */
	//CI_AppData.IngestBehavior = BHV_OPTIMISTIC;

	/* Execute the function being tested */
	retCode = CI_GetCmdAuthorized(CmdMsgPtr);

	/* Verify results */
	UtAssert_True(retCode==TRUE,"Authorized = TRUE");
}

/**
 * Test CI_GetCmdAuthorized(), Unregistered Pessimistic
 */
void Test_CI_GetCmdAuthorized_Unreg_Pess(void)
{
	boolean		  		retCode = FALSE;
	CI_CmdAuthData_t 	cmd;
	uint32  			MsgSize = sizeof(cmd);
	CFE_SB_MsgPtr_t 	CmdMsgPtr;

	/* Create CFE_SB_Msg_t */
	CFE_SB_InitMsg(&cmd, 0, MsgSize, TRUE);
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmd;

	/* Set to cause to fail */
	//CI_AppData.IngestBehavior = CI_BHV_PESSIMISTIC;

	/* Execute the function being tested */
	retCode = CI_GetCmdAuthorized(CmdMsgPtr);

	/* Verify results */
	UtAssert_True(retCode==FALSE, "Authorized = FALSE");
}

/**
 * Test CI_GetCmdAuthorized(), 1 Step
 */
void Test_CI_GetCmdAuthorized_1_Step(void)
{
	boolean		  		retCode = FALSE;
	CI_CmdAuthData_t 	cmd;
	CI_CmdAuthData_t 	*cmdPtr;
	CI_CmdRegData_t 	cmdReg;
	CI_CmdRegData_t 	*regDataPtr;
	uint32  			MsgSize = sizeof(cmdReg);
	CFE_SB_MsgPtr_t 	CmdMsgPtr;


	/* Register a new 1-step cmd */
	CFE_SB_InitMsg(&cmdReg, TEST_MSG_ID, MsgSize, TRUE);
	regDataPtr = ((CI_CmdRegData_t *) &cmdReg);
	regDataPtr->msgID = TEST_MSG_ID;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmdReg;
	CI_CmdRegister(CmdMsgPtr);

	/* Create CFE_SB_Msg_t */
	MsgSize = sizeof(cmd);
	CFE_SB_InitMsg(&cmd, TEST_MSG_ID, MsgSize, TRUE);
	cmdPtr = ((CI_CmdAuthData_t *) &cmd);
	cmdPtr->msgID = TEST_MSG_ID;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)cmdPtr;

	/* Execute the function being tested */
	retCode = CI_GetCmdAuthorized(CmdMsgPtr);

	/* Verify results */
	UtAssert_True(retCode==TRUE,"Authorized = TRUE");
}

/**
 * Test CI_GetCmdAuthorized(), 2 Step Unauth
 */
void Test_CI_GetCmdAuthorized_2_Step_Unauth(void)
{
	boolean		  		retCode = FALSE;
	CI_CmdAuthData_t 	cmd;
	CI_CmdAuthData_t 	*cmdPtr;
	CI_CmdRegData_t 	cmdReg;
	CI_CmdRegData_t 	*regDataPtr;
	uint32  			MsgSize = sizeof(cmdReg);
	CFE_SB_MsgPtr_t 	CmdMsgPtr;

	/* Register a new 2-step cmd */
	CFE_SB_InitMsg(&cmdReg, TEST_MSG_ID, MsgSize, TRUE);
	regDataPtr = ((CI_CmdRegData_t *) &cmdReg);
	regDataPtr->msgID = TEST_MSG_ID;
	regDataPtr->step = STEP_2;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmdReg;
	CI_CmdRegister(CmdMsgPtr);

	/* Create CFE_SB_Msg_t */
	MsgSize = sizeof(cmd);
	CFE_SB_InitMsg(&cmd, TEST_MSG_ID, MsgSize, TRUE);
	cmdPtr = ((CI_CmdAuthData_t *) &cmd);
	cmdPtr->msgID = TEST_MSG_ID;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)cmdPtr;

	/* Execute the function being tested */
	retCode = CI_GetCmdAuthorized(CmdMsgPtr);

	/* Verify results */
	UtAssert_True(retCode==FALSE,"Authorized = FALSE");
}

/**
 * Test CI_GetCmdAuthorized(), 2 Step Authorized
 */
void Test_CI_GetCmdAuthorized_2_Step_Auth(void)
{
	boolean		  		retCode = FALSE;
	CI_CmdAuthData_t 	cmd;
	CI_CmdAuthData_t 	*cmdPtr;
	CI_CmdRegData_t 	cmdReg;
	CI_CmdRegData_t 	*regDataPtr;
	uint32  			MsgSize = sizeof(cmdReg);
	CFE_SB_MsgPtr_t 	CmdMsgPtr;

	/* Register a new 2-step cmd */
	CFE_SB_InitMsg(&cmdReg, TEST_MSG_ID, MsgSize, TRUE);
	regDataPtr = ((CI_CmdRegData_t *) &cmdReg);
	regDataPtr->msgID = TEST_MSG_ID;
	regDataPtr->step = STEP_2;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmdReg;
	CI_CmdRegister(CmdMsgPtr);

	/* Authorize the cmd */
	MsgSize = sizeof(cmd);
	CFE_SB_InitMsg(&cmd, TEST_MSG_ID, MsgSize, TRUE);
	cmdPtr = ((CI_CmdAuthData_t *) &cmd);
	cmdPtr->msgID = TEST_MSG_ID;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)cmdPtr;
	CI_CmdAuthorize(CmdMsgPtr);

	/* Execute the function being tested */
	retCode = CI_GetCmdAuthorized(CmdMsgPtr);

	/* Verify results */
	UtAssert_True(retCode==TRUE,"Authorized = TRUE");
}

/**
 * Test CI_CmdAuthorize(), Not Registered
 */
void Test_CI_CmdAuthorize_Not_Reg(void)
{
	CI_CmdAuthData_t 	cmd;
	CI_CmdAuthData_t 	*cmdPtr;
	uint32  			MsgSize = sizeof(cmd);
	CFE_SB_MsgPtr_t 	CmdMsgPtr;

	/* Init the cmd */
	MsgSize = sizeof(cmd);
	CFE_SB_InitMsg(&cmd, TEST_MSG_ID, MsgSize, TRUE);
	cmdPtr = ((CI_CmdAuthData_t *) &cmd);
	cmdPtr->msgID = TEST_MSG_ID;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)cmdPtr;

	/* Execute the function being tested */
	CI_CmdAuthorize(CmdMsgPtr);

	/* Verify results */
	UtAssert_EventSent(CI_CMD_AUTH_NOT_REG_EID, CFE_EVS_ERROR, "", "Cmd not registered");
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
	UtAssert_True(CI_AppData.HkTlm.usCmdErrCnt==1,"CmdErrCnt = 1");
}

/**
 * Test CI_CmdAuthorize(), Not 2 Step
 */
void Test_CI_CmdAuthorize_Not_2_Step(void)
{
	CI_CmdAuthData_t 	cmd;
	CI_CmdAuthData_t 	*cmdPtr;
	CI_CmdRegData_t 	cmdReg;
	CI_CmdRegData_t 	*regDataPtr;
	uint32  			MsgSize = sizeof(cmdReg);
	CFE_SB_MsgPtr_t 	CmdMsgPtr;

	/* Register a new 1-step cmd - generates 1 event */
	CFE_SB_InitMsg(&cmdReg, TEST_MSG_ID, MsgSize, TRUE);
	regDataPtr = ((CI_CmdRegData_t *) &cmdReg);
	regDataPtr->msgID = TEST_MSG_ID;
	regDataPtr->step = STEP_1;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmdReg;
	CI_CmdRegister(CmdMsgPtr);

	/* Authorize the cmd */
	MsgSize = sizeof(cmd);
	CFE_SB_InitMsg(&cmd, TEST_MSG_ID, MsgSize, TRUE);
	cmdPtr = ((CI_CmdAuthData_t *) &cmd);
	cmdPtr->msgID = TEST_MSG_ID;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)cmdPtr;

	/* Execute the function being tested */
	CI_CmdAuthorize(CmdMsgPtr);

	/* Verify results */
	UtAssert_EventSent(CI_CMD_AUTH_INV_MODE_EID, CFE_EVS_ERROR, "", "Cmd not 2-step");
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
	UtAssert_True(CI_AppData.HkTlm.usCmdErrCnt==1,"CmdErrCnt = 1");
}

/**
 * Test CI_CmdAuthorize(), Already Authorized
 */
void Test_CI_CmdAuthorize_Inv_State(void)
{
	CI_CmdAuthData_t 	cmd;
	CI_CmdAuthData_t 	*cmdPtr;
	CI_CmdRegData_t 	cmdReg;
	CI_CmdRegData_t 	*regDataPtr;
	uint32  			MsgSize = sizeof(cmdReg);
	CFE_SB_MsgPtr_t 	CmdMsgPtr;

	/* Register a new 2-step cmd - generates 1 event */
	CFE_SB_InitMsg(&cmdReg, TEST_MSG_ID, MsgSize, TRUE);
	regDataPtr = ((CI_CmdRegData_t *) &cmdReg);
	regDataPtr->msgID = TEST_MSG_ID;
	regDataPtr->step = STEP_2;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmdReg;
	CI_CmdRegister(CmdMsgPtr);

	/* Authorize the cmd */
	MsgSize = sizeof(cmd);
	CFE_SB_InitMsg(&cmd, TEST_MSG_ID, MsgSize, TRUE);
	cmdPtr = ((CI_CmdAuthData_t *) &cmd);
	cmdPtr->msgID = TEST_MSG_ID;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)cmdPtr;

	/* Execute the function being tested twice */
	CI_CmdAuthorize(CmdMsgPtr);
	CI_CmdAuthorize(CmdMsgPtr);

	/* Verify results */
	UtAssert_EventSent(CI_CMD_AUTHORIZED_EID, CFE_EVS_INFORMATION, "", "Cmd authorized");
	UtAssert_EventSent(CI_CMD_AUTH_INV_STATE_EID, CFE_EVS_ERROR, "", "Cmd already authorized");
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==3,"Event Count = 3");
	UtAssert_True(CI_AppData.HkTlm.usCmdErrCnt==1,"CmdErrCnt = 1");
	UtAssert_True(CI_AppData.HkTlm.usCmdCnt==2,"CmdCnt = 2");
}

/**
 * Test CI_CmdAuthorize(), Nominal
 */
void Test_CI_CmdAuthorize_Nominal(void)
{
	CI_CmdAuthData_t 	cmd;
	CI_CmdAuthData_t 	*cmdPtr;
	CI_CmdRegData_t 	cmdReg;
	CI_CmdRegData_t 	*regDataPtr;
	uint32  			MsgSize = sizeof(cmdReg);
	CFE_SB_MsgPtr_t 	CmdMsgPtr;

	/* Register a new 2-step cmd - generates 1 event */
	CFE_SB_InitMsg(&cmdReg, TEST_MSG_ID, MsgSize, TRUE);
	regDataPtr = ((CI_CmdRegData_t *) &cmdReg);
	regDataPtr->msgID = TEST_MSG_ID;
	regDataPtr->step = STEP_2;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmdReg;
	CI_CmdRegister(CmdMsgPtr);

	/* Authorize the cmd */
	MsgSize = sizeof(cmd);
	CFE_SB_InitMsg(&cmd, TEST_MSG_ID, MsgSize, TRUE);
	cmdPtr = ((CI_CmdAuthData_t *) &cmd);
	cmdPtr->msgID = TEST_MSG_ID;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)cmdPtr;

	/* Execute the function being tested */
	CI_CmdAuthorize(CmdMsgPtr);

	/* Verify results */
	UtAssert_EventSent(CI_CMD_AUTHORIZED_EID, CFE_EVS_INFORMATION, "", "Cmd authorized");
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
	UtAssert_True(CI_AppData.HkTlm.usCmdCnt==2,"CmdCnt = 2");
}

/**
 * Test CI_CmdDeauthorize(), Not Registered
 */
void Test_CI_CmdDeauthorize_Not_Reg(void)
{
	CI_CmdAuthData_t 	cmd;
	CI_CmdAuthData_t 	*cmdPtr;
	uint32  			MsgSize;
	CFE_SB_MsgPtr_t 	CmdMsgPtr;

	/* Deauthorize the cmd */
	MsgSize = sizeof(cmd);
	CFE_SB_InitMsg(&cmd, TEST_MSG_ID, MsgSize, TRUE);
	cmdPtr = ((CI_CmdAuthData_t *) &cmd);
	cmdPtr->msgID = TEST_MSG_ID;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)cmdPtr;

	/* Execute the function being tested */
	CI_CmdDeauthorize(CmdMsgPtr);

	/* Verify results */
	UtAssert_EventSent(CI_CMD_DEAUTH_NOT_REG_EID, CFE_EVS_ERROR, "", "Cmd not registered");
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
	UtAssert_True(CI_AppData.HkTlm.usCmdErrCnt==1,"CmdErrCnt = 1");
}

/**
 * Test CI_CmdDeauthorize(), Not 2 Step
 */
void Test_CI_CmdDeauthorize_Not_2_Step(void)
{
	CI_CmdAuthData_t 	cmd;
	CI_CmdAuthData_t 	*cmdPtr;
	CI_CmdRegData_t 	cmdReg;
	CI_CmdRegData_t 	*regDataPtr;
	uint32  			MsgSize = sizeof(cmdReg);
	CFE_SB_MsgPtr_t 	CmdMsgPtr;

	/* Register a new 2-step cmd - generates 1 event */
	CFE_SB_InitMsg(&cmdReg, TEST_MSG_ID, MsgSize, TRUE);
	regDataPtr = ((CI_CmdRegData_t *) &cmdReg);
	regDataPtr->msgID = TEST_MSG_ID;
	regDataPtr->step = STEP_1;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmdReg;
	CI_CmdRegister(CmdMsgPtr);

	/* Deauthorize the cmd */
	MsgSize = sizeof(cmd);
	CFE_SB_InitMsg(&cmd, TEST_MSG_ID, MsgSize, TRUE);
	cmdPtr = ((CI_CmdAuthData_t *) &cmd);
	cmdPtr->msgID = TEST_MSG_ID;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)cmdPtr;

	/* Execute the function being tested */
	CI_CmdDeauthorize(CmdMsgPtr);

	/* Verify results */
	UtAssert_EventSent(CI_CMD_DEAUTH_INV_MODE_EID, CFE_EVS_ERROR, "", "Cmd not 2-step");
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
	UtAssert_True(CI_AppData.HkTlm.usCmdCnt==1,"CmdCnt = 1");
	UtAssert_True(CI_AppData.HkTlm.usCmdErrCnt==1,"CmdErrCnt = 1");
}

/**
 * Test CI_CmdDeauthorize(), Not Auth
 */
void Test_CI_CmdDeauthorize_Not_Auth(void)
{
	CI_CmdAuthData_t 	cmd;
	CI_CmdAuthData_t 	*cmdPtr;
	CI_CmdRegData_t 	cmdReg;
	CI_CmdRegData_t 	*regDataPtr;
	uint32  			MsgSize = sizeof(cmdReg);
	CFE_SB_MsgPtr_t 	CmdMsgPtr;

	/* Register a new 2-step cmd - generates 1 event */
	CFE_SB_InitMsg(&cmdReg, TEST_MSG_ID, MsgSize, TRUE);
	regDataPtr = ((CI_CmdRegData_t *) &cmdReg);
	regDataPtr->msgID = TEST_MSG_ID;
	regDataPtr->step = STEP_2;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmdReg;
	CI_CmdRegister(CmdMsgPtr);

	/* Deauthorize the cmd */
	MsgSize = sizeof(cmd);
	CFE_SB_InitMsg(&cmd, TEST_MSG_ID, MsgSize, TRUE);
	cmdPtr = ((CI_CmdAuthData_t *) &cmd);
	cmdPtr->msgID = TEST_MSG_ID;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)cmdPtr;

	/* Execute the function being tested */
	CI_CmdDeauthorize(CmdMsgPtr);

	/* Verify results */
	UtAssert_EventSent(CI_CMD_DEAUTH_INV_STATE_EID, CFE_EVS_ERROR, "", "Cmd not authorized");
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
	UtAssert_True(CI_AppData.HkTlm.usCmdCnt==1,"CmdCnt = 1");
	UtAssert_True(CI_AppData.HkTlm.usCmdErrCnt==1,"CmdErrCnt = 1");
}

/**
 * Test CI_CmdDeauthorize(), Nominal
 */
void Test_CI_CmdDeauthorize_Nominal(void)
{
	CI_CmdAuthData_t 	cmd;
	CI_CmdAuthData_t 	*cmdPtr;
	CI_CmdRegData_t 	cmdReg;
	CI_CmdRegData_t 	*regDataPtr;
	uint32  			MsgSize = sizeof(cmdReg);
	CFE_SB_MsgPtr_t 	CmdMsgPtr;

	/* Register a new 2-step cmd - generates 1 event */
	CFE_SB_InitMsg(&cmdReg, TEST_MSG_ID, MsgSize, TRUE);
	regDataPtr = ((CI_CmdRegData_t *) &cmdReg);
	regDataPtr->msgID = TEST_MSG_ID;
	regDataPtr->step = STEP_2;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmdReg;
	CI_CmdRegister(CmdMsgPtr);

	/* Deauthorize the cmd */
	MsgSize = sizeof(cmd);
	CFE_SB_InitMsg(&cmd, TEST_MSG_ID, MsgSize, TRUE);
	cmdPtr = ((CI_CmdAuthData_t *) &cmd);
	cmdPtr->msgID = TEST_MSG_ID;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)cmdPtr;

	/* Execute the function being tested */
	CI_CmdAuthorize(CmdMsgPtr);
	CI_CmdDeauthorize(CmdMsgPtr);

	/* Verify results */
	UtAssert_EventSent(CI_CMD_DEAUTHORIZED_EID, CFE_EVS_INFORMATION, "", "Cmd deauthorized");
	UtAssert_EventSent(CI_CMD_AUTHORIZED_EID, CFE_EVS_INFORMATION, "", "Cmd authorized");
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==3,"Event Count = 3");
	UtAssert_True(CI_AppData.HkTlm.usCmdCnt==3,"CmdCnt = 3");
	UtAssert_True(CI_AppData.HkTlm.usCmdErrCnt==0,"CmdErrCnt = 0");
}

/**
 * Test CI_CmdRegister(), Invalid MsgId
 */
void Test_CI_CmdRegister_Inv_MsgID(void)
{
	CI_CmdRegData_t 	cmdReg;
	CI_CmdRegData_t 	*regDataPtr;
	uint32  			MsgSize = sizeof(cmdReg);
	CFE_SB_MsgPtr_t 	CmdMsgPtr;

	/* Register a new cmd */
	CFE_SB_InitMsg(&cmdReg, TEST_MSG_ID, MsgSize, TRUE);
	regDataPtr = ((CI_CmdRegData_t *) &cmdReg);
	regDataPtr->msgID = 0;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmdReg;

	/* Execute the function being tested */
	CI_CmdRegister(CmdMsgPtr);

	/* Verify results */
	UtAssert_EventSent(CI_CMD_REG_ERR_EID, CFE_EVS_ERROR, "Unable to register cmd", "Unable to register cmd");
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
	UtAssert_True(CI_AppData.HkTlm.usCmdErrCnt==1,"CmdErrCnt = 1");
}

/**
 * Test CI_CmdRegister(), Already Registered
 */
void Test_CI_CmdRegister_Reg_Exists(void)
{
	CI_CmdRegData_t 	cmdReg;
	CI_CmdRegData_t 	*regDataPtr;
	uint32  			MsgSize = sizeof(cmdReg);
	CFE_SB_MsgPtr_t 	CmdMsgPtr;

	/* Register a new cmd */
	CFE_SB_InitMsg(&cmdReg, TEST_MSG_ID, MsgSize, TRUE);
	regDataPtr = ((CI_CmdRegData_t *) &cmdReg);
	regDataPtr->msgID = TEST_MSG_ID;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmdReg;

	/* Execute the function being tested */
	CI_CmdRegister(CmdMsgPtr);
	CI_CmdRegister(CmdMsgPtr);

	/* Verify results */
	UtAssert_EventSent(CI_CMD_ALREADY_REGISTERED_EID, CFE_EVS_ERROR, "", "Cmd already registered");
	UtAssert_EventSent(CI_CMD_REGISTERED_EID, CFE_EVS_INFORMATION, "", "Cmd registered");
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
	UtAssert_True(CI_AppData.HkTlm.usCmdErrCnt==1,"CmdErrCnt = 1");
	UtAssert_True(CI_AppData.HkTlm.usCmdCnt==1,"CmdCnt = 1");
}

/**
 * Test CI_CmdRegister(), Full Table
 */
void Test_CI_CmdRegister_Full_Table(void)
{
	CI_CmdRegData_t 	cmdReg;
	CI_CmdRegData_t 	*regDataPtr;
	uint32  			MsgSize = sizeof(cmdReg);
	uint32 				i = 0;
	CFE_SB_MsgPtr_t 	CmdMsgPtr;

	/* Populate entire config tbl */
	for(i = 0; i < CI_MAX_RGST_CMDS; ++i)
	{
		CI_AppData.ConfigTblPtr->cmds[i].mid = 1;
	}

	/* Register a new cmd */
	CFE_SB_InitMsg(&cmdReg, TEST_MSG_ID, MsgSize, TRUE);
	regDataPtr = ((CI_CmdRegData_t *) &cmdReg);
	regDataPtr->msgID = TEST_MSG_ID;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmdReg;

	/* Execute the function being tested */
	CI_CmdRegister(CmdMsgPtr);

	/* Verify results */
	UtAssert_EventSent(CI_CMD_REG_ERR_EID, CFE_EVS_ERROR, "Unable to register cmd", "Unable to register cmd");
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
	UtAssert_True(CI_AppData.HkTlm.usCmdErrCnt==1,"CmdErrCnt = 1");
}

/**
 * Test CI_CmdRegister(), Nominal
 */
void Test_CI_CmdRegister_Nominal(void)
{
	CI_CmdRegData_t 	cmdReg;
	CI_CmdRegData_t 	*regDataPtr;
	uint32  			MsgSize = sizeof(cmdReg);
	CFE_SB_MsgPtr_t 	CmdMsgPtr;

	/* Register a new cmd */
	CFE_SB_InitMsg(&cmdReg, TEST_MSG_ID, MsgSize, TRUE);
	regDataPtr = ((CI_CmdRegData_t *) &cmdReg);
	regDataPtr->msgID = TEST_MSG_ID;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmdReg;

	/* Execute the function being tested */
	CI_CmdRegister(CmdMsgPtr);

	/* Verify results */
	UtAssert_EventSent(CI_CMD_REGISTERED_EID, CFE_EVS_INFORMATION, "", "Cmd registered");
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
	UtAssert_True(CI_AppData.HkTlm.usCmdCnt==1,"CmdCnt = 1");
}

/**
 * Test CI_CmdDeregister(), Invalid MsgId
 */
void Test_CI_CmdDeregister_Inv_MsgID(void)
{
	/* Deregister only needs data from CI_CmdAuthData_t,
	 * not CI_CmdRegData_t */
	CI_CmdAuthData_t 	cmdReg;
	CI_CmdAuthData_t 	*regDataPtr;
	uint32  			MsgSize = sizeof(cmdReg);
	CFE_SB_MsgPtr_t 	CmdMsgPtr;

	/* Deregister a cmd */
	CFE_SB_InitMsg(&cmdReg, TEST_MSG_ID, MsgSize, TRUE);
	regDataPtr = ((CI_CmdAuthData_t *) &cmdReg);
	regDataPtr->msgID = 0;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmdReg;

	/* Execute the function being tested */
	CI_CmdDeregister(CmdMsgPtr);

	/* Verify results */
	UtAssert_EventSent(CI_CMD_REG_ERR_EID, CFE_EVS_ERROR, "Unable to deregister cmd", "Unable to deregister cmd");
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
	UtAssert_True(CI_AppData.HkTlm.usCmdErrCnt==1,"CmdErrCnt = 1");
}

/**
 * Test CI_CmdDeregister(), Not Registered
 */
void Test_CI_CmdDeregister_Not_Reg(void)
{
	CI_CmdAuthData_t 	cmdReg;
	CI_CmdAuthData_t 	*regDataPtr;
	uint32  			MsgSize = sizeof(cmdReg);
	CFE_SB_MsgPtr_t 	CmdMsgPtr;

	/* Deregister a new cmd */
	CFE_SB_InitMsg(&cmdReg, TEST_MSG_ID, MsgSize, TRUE);
	regDataPtr = ((CI_CmdAuthData_t *) &cmdReg);
	regDataPtr->msgID = TEST_MSG_ID;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmdReg;

	/* Execute the function being tested */
	CI_CmdDeregister(CmdMsgPtr);

	/* Verify results */
	UtAssert_EventSent(CI_CMD_NOT_REGISTERED_EID, CFE_EVS_ERROR, "", "Cmd not registered");
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
	UtAssert_True(CI_AppData.HkTlm.usCmdErrCnt==1,"CmdErrCnt = 1");
}

/**
 * Test CI_CmdDeregister(), Nominal
 */
void Test_CI_CmdDeregister_Nominal(void)
{
	CI_CmdAuthData_t 	cmd;
	CI_CmdAuthData_t 	*cmdPtr;
	CI_CmdRegData_t 	cmdReg;
	CI_CmdRegData_t 	*regDataPtr;
	uint32  			MsgSize = sizeof(cmdReg);
	CFE_SB_MsgPtr_t 	CmdMsgPtr;

	/* Register and Deregister a new cmd */
	CFE_SB_InitMsg(&cmdReg, TEST_MSG_ID, MsgSize, TRUE);
	regDataPtr = ((CI_CmdRegData_t *) &cmdReg);
	regDataPtr->msgID = TEST_MSG_ID;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmdReg;

	MsgSize = sizeof(cmd);
	CFE_SB_InitMsg(&cmd, TEST_MSG_ID, MsgSize, TRUE);
	cmdPtr = ((CI_CmdAuthData_t *) &cmd);
	cmdPtr->msgID = TEST_MSG_ID;

	/* Execute the function being tested */
	CI_CmdRegister(CmdMsgPtr);
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmd;
	CI_CmdDeregister(CmdMsgPtr);

	/* Verify results */
	UtAssert_EventSent(CI_CMD_REGISTERED_EID, CFE_EVS_INFORMATION, "", "Cmd registered");
	UtAssert_EventSent(CI_CMD_DEREGISTERED_EID, CFE_EVS_INFORMATION, "", "Cmd deregistered");
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
	UtAssert_True(CI_AppData.HkTlm.usCmdCnt==2,"CmdCnt = 2");
}

/**
 * Test CI_UpdateCmdReg(), Invalid MsgId
 */
void Test_CI_UpdateCmdReg_Inv_MsgID(void)
{
	CI_CmdRegData_t 	cmdReg;
	CI_CmdRegData_t 	*regDataPtr;
	uint32  			MsgSize = sizeof(cmdReg);
	CFE_SB_MsgPtr_t 	CmdMsgPtr;

	/* Register a new cmd */
	CFE_SB_InitMsg(&cmdReg, TEST_MSG_ID, MsgSize, TRUE);
	regDataPtr = ((CI_CmdRegData_t *) &cmdReg);
	regDataPtr->msgID = 0;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmdReg;

	/* Execute the function being tested */
	CI_UpdateCmdReg(CmdMsgPtr);

	/* Verify results */
	UtAssert_EventSent(CI_CMD_REG_ERR_EID, CFE_EVS_ERROR, "Unable to update cmd", "Unable to update cmd");
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
	UtAssert_True(CI_AppData.HkTlm.usCmdErrCnt==1,"CmdErrCnt = 1");
}

/**
 * Test CI_UpdateCmdReg(), Not Registered
 */
void Test_CI_UpdateCmdReg_Not_Reg(void)
{
	CI_CmdRegData_t 	cmdReg;
	CI_CmdRegData_t 	*regDataPtr;
	uint32  			MsgSize = sizeof(cmdReg);
	CFE_SB_MsgPtr_t 	CmdMsgPtr;

	/* Register a new cmd */
	CFE_SB_InitMsg(&cmdReg, TEST_MSG_ID, MsgSize, TRUE);
	regDataPtr = ((CI_CmdRegData_t *) &cmdReg);
	regDataPtr->msgID = TEST_MSG_ID;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmdReg;

	/* Execute the function being tested */
	CI_UpdateCmdReg(CmdMsgPtr);

	/* Verify results */
	UtAssert_EventSent(CI_CMD_NOT_REGISTERED_EID, CFE_EVS_ERROR, "", "Cmd not registered");
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
	UtAssert_True(CI_AppData.HkTlm.usCmdErrCnt==1,"CmdErrCnt = 1");
}

/**
 * Test CI_UpdateCmdReg(), 2 Step Unauthorized (Nominal)
 */
void Test_CI_UpdateCmdReg_2_Step_Nominal(void)
{
	CI_CmdAuthData_t 	cmd;
	CI_CmdAuthData_t 	*cmdPtr;
	CI_CmdRegData_t 	cmdReg;
	CI_CmdRegData_t 	*regDataPtr;
	uint32  			MsgSize = sizeof(cmdReg);
	CI_CmdData_t		*CmdData = NULL;
	CFE_SB_MsgPtr_t 	CmdMsgPtr;
	uint32              i = CI_INVALID_VALUE;

	/* Register a new cmd */
	CFE_SB_InitMsg(&cmdReg, TEST_MSG_ID, MsgSize, TRUE);
	regDataPtr = ((CI_CmdRegData_t *) &cmdReg);
	regDataPtr->msgID = TEST_MSG_ID;
	regDataPtr->cmdCode = TEST_CC;
	regDataPtr->step = STEP_2;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmdReg;

	/* Execute functions required */
	CI_CmdRegister(CmdMsgPtr);

	/* Change a value */
	regDataPtr->step = STEP_1;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmdReg;

	/* Execute the function being tested */
	CI_UpdateCmdReg(CmdMsgPtr);

	/* Verify results */
	CmdData = CI_GetRegisterdCmd(TEST_MSG_ID, TEST_CC, &i);
	UtAssert_EventSent(CI_CMD_REGISTERED_EID, CFE_EVS_INFORMATION, "", "Cmd registered");
	UtAssert_EventSent(CI_CMD_UPDATE_REG_EID, CFE_EVS_INFORMATION, "", "Cmd updated");
	UtAssert_True(CI_AppData.HkTlm.usCmdCnt==2,"CmdCnt = 2");
	UtAssert_True(CmdData->mid==TEST_MSG_ID,"Correct data row");
	UtAssert_True(CmdData->step==STEP_1,"Successfully updated cmd stepping");
}

/**
 * Test CI_UpdateCmdReg(), 2 Step Authorized
 */
void Test_CI_UpdateCmdReg_2_Step_Auth(void)
{
	CI_CmdAuthData_t 	cmd;
	CI_CmdAuthData_t 	*cmdPtr;
	CI_CmdRegData_t 	cmdReg;
	CI_CmdRegData_t 	*regDataPtr;
	uint32  			MsgSize = sizeof(cmdReg);
	CFE_SB_MsgPtr_t 	CmdMsgPtr;

	/* Register a new cmd */
	CFE_SB_InitMsg(&cmdReg, TEST_MSG_ID, MsgSize, TRUE);
	regDataPtr = ((CI_CmdRegData_t *) &cmdReg);
	regDataPtr->msgID = TEST_MSG_ID;
	regDataPtr->step = STEP_2;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmdReg;

	/* Authorize the cmd */
	MsgSize = sizeof(cmd);
	CFE_SB_InitMsg(&cmd, TEST_MSG_ID, MsgSize, TRUE);
	cmdPtr = ((CI_CmdAuthData_t *) &cmd);
	cmdPtr->msgID = TEST_MSG_ID;

	/* Execute functions required */
	CI_CmdRegister(CmdMsgPtr);
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmd;
	CI_CmdAuthorize(CmdMsgPtr);

	/* Execute the function being tested */
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmdReg;
	CI_UpdateCmdReg(CmdMsgPtr);

	/* Verify results */
	UtAssert_EventSent(CI_CMD_REGISTERED_EID, CFE_EVS_INFORMATION, "", "Cmd registered");
	UtAssert_EventSent(CI_CMD_AUTHORIZED_EID, CFE_EVS_INFORMATION, "", "Cmd authorized");
	UtAssert_EventSent(CI_CMD_UPDT_REG_INVLD_STATE_EID, CFE_EVS_ERROR, "", "Invalid state of command to update");
	UtAssert_True(CI_AppData.HkTlm.usCmdCnt==2,"CmdCnt = 2");
	UtAssert_True(CI_AppData.HkTlm.usCmdErrCnt==1,"CmdErrCnt = 1");
}

/**
 * Test CI_UpdateCmdReg(), 1 Step (Nominal)
 */
void Test_CI_UpdateCmdReg_1_Step_Nominal(void)
{
	CI_CmdAuthData_t 	cmd;
	CI_CmdAuthData_t 	*cmdPtr;
	CI_CmdRegData_t 	cmdReg;
	CI_CmdRegData_t 	*regDataPtr;
	uint32  			MsgSize = sizeof(cmdReg);
	CI_CmdData_t		*CmdData = NULL;
	CFE_SB_MsgPtr_t 	CmdMsgPtr;
	uint32              i = CI_INVALID_VALUE;

	/* Register a new cmd */
	CFE_SB_InitMsg(&cmdReg, TEST_MSG_ID, MsgSize, TRUE);
	regDataPtr = ((CI_CmdRegData_t *) &cmdReg);
	regDataPtr->msgID = TEST_MSG_ID;
	regDataPtr->step = STEP_1;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmdReg;

	/* Execute functions required */
	CI_CmdRegister(CmdMsgPtr);

	/* Change a value */
	regDataPtr->step = STEP_2;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmdReg;

	/* Execute the function being tested */
	CI_UpdateCmdReg(CmdMsgPtr);

	/* Verify results */
	CmdData = CI_GetRegisterdCmd(TEST_MSG_ID, TEST_CC, &i);
	UtAssert_EventSent(CI_CMD_REGISTERED_EID, CFE_EVS_INFORMATION, "", "Cmd registered");
	UtAssert_EventSent(CI_CMD_UPDATE_REG_EID, CFE_EVS_INFORMATION, "", "Cmd updated");
	UtAssert_True(CI_AppData.HkTlm.usCmdCnt==2,"CmdCnt = 2");
	UtAssert_True(CmdData->mid==TEST_MSG_ID,"Correct data row");
	UtAssert_True(CmdData->step==STEP_2,"Successfully updated cmd stepping");
}

/**
 * Test CI_ProcessTimeouts(), Nominal
 */
void Test_CI_ProcessTimeouts_Nominal(void)
{
	CI_CmdAuthData_t 	cmd;
	CI_CmdAuthData_t 	*cmdPtr;
	CI_CmdRegData_t 	cmdReg;
	CI_CmdRegData_t 	*regDataPtr;
	uint32  			MsgSize = sizeof(cmdReg);
	uint32 				i = -1;
	uint32 				timeout = -1;
	CFE_SB_MsgPtr_t 	CmdMsgPtr;

	/* Register a new cmd */
	CFE_SB_InitMsg(&cmdReg, TEST_MSG_ID, MsgSize, TRUE);
	regDataPtr = ((CI_CmdRegData_t *) &cmdReg);
	regDataPtr->msgID = TEST_MSG_ID;
	regDataPtr->step = STEP_2;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmdReg;

	/* Authorize the cmd */
	MsgSize = sizeof(cmd);
	CFE_SB_InitMsg(&cmd, TEST_MSG_ID, MsgSize, TRUE);
	cmdPtr = ((CI_CmdAuthData_t *) &cmd);
	cmdPtr->msgID = TEST_MSG_ID;

	/* Execute functions required */
	CI_CmdRegister(CmdMsgPtr);
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmd;
	CI_CmdAuthorize(CmdMsgPtr);

	/* Get the current timeout for the authorized cmd */
	CI_GetRegisterdCmd(TEST_MSG_ID, TEST_CC, &i);
	timeout = CI_AppData.TimeoutTbl.time[i];

	/* Execute the function being tested */
	CI_ProcessTimeouts();

	/* Verify results */
	UtAssert_True(CI_AppData.TimeoutTbl.time[i]==timeout-1,"Timeout decrement");
}

/**
 * Test CI_ProcessTimeouts(), Update
 */
void Test_CI_ProcessTimeouts_Update(void)
{
	CI_CmdAuthData_t 	cmd;
	CI_CmdAuthData_t 	*cmdPtr;
	CI_CmdRegData_t 	cmdReg;
	CI_CmdRegData_t 	*regDataPtr;
	uint32  			MsgSize = sizeof(cmdReg);
	uint32 				i = -1;
	uint32 				timeout = -1;
	CFE_SB_MsgPtr_t 	CmdMsgPtr;

	/* Register a new cmd */
	CFE_SB_InitMsg(&cmdReg, TEST_MSG_ID, MsgSize, TRUE);
	regDataPtr = ((CI_CmdRegData_t *) &cmdReg);
	regDataPtr->msgID = TEST_MSG_ID;
	regDataPtr->step = STEP_2;
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmdReg;

	/* Authorize the cmd */
	MsgSize = sizeof(cmd);
	CFE_SB_InitMsg(&cmd, TEST_MSG_ID, MsgSize, TRUE);
	cmdPtr = ((CI_CmdAuthData_t *) &cmd);
	cmdPtr->msgID = TEST_MSG_ID;

	/* Execute functions required */
	CI_CmdRegister(CmdMsgPtr);
	CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmd;
	CI_CmdAuthorize(CmdMsgPtr);

	/* Set the current timeout for the authorized cmd to 1 */
	CI_GetRegisterdCmd(TEST_MSG_ID, TEST_CC, &i);
	CI_AppData.TimeoutTbl.time[i] = 1;

	/* Execute the function being tested */
	CI_ProcessTimeouts();

	/* Verify results */
	UtAssert_True(CI_AppData.TimeoutTbl.time[i]==0,"Timeout decrement");
	UtAssert_True(CI_GetCmdAuthorized(CmdMsgPtr)==FALSE,"Command deauthorized");
}



/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void CI_App_Test_AddTestCases(void)
{
    UtTest_Add(Test_CI_InitEvent_Fail_Register,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_InitEvent_Fail_Register");
    UtTest_Add(Test_CI_InitPipe_Fail_CreateSCHPipe,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_InitPipe_Fail_CreateSCHPipe");
    UtTest_Add(Test_CI_InitPipe_Fail_SubscribeWakeup,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_InitPipe_Fail_SubscribeWakeup");
    UtTest_Add(Test_CI_InitPipe_Fail_SubscribeSendHK,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_InitPipe_Fail_SubscribeSendHK");
    UtTest_Add(Test_CI_InitPipe_Fail_SubscribeProcessTimeouts,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_InitPipe_Fail_SubscribeProcessTimeouts");
    UtTest_Add(Test_CI_InitPipe_Fail_SubscribeIngestCommands,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_InitPipe_Fail_SubscribeIngestCommands");
    UtTest_Add(Test_CI_InitPipe_Fail_CreateCMDPipe,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_InitPipe_Fail_CreateCMDPipe");
    UtTest_Add(Test_CI_InitPipe_Fail_SubscribeCMD,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_InitPipe_Fail_SubscribeCMD");
    UtTest_Add(Test_CI_InitData,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_InitData");
    UtTest_Add(Test_CI_InitApp_Fail_InitEvent,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_InitApp_Fail_InitEvent");
    UtTest_Add(Test_CI_InitApp_Fail_InitPipe,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_InitApp_Fail_InitPipe");
    UtTest_Add(Test_CI_InitApp_Fail_InitData,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_InitApp_Fail_InitData");
    UtTest_Add(Test_CI_InitApp_Fail_InitConfigTbl,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_InitApp_Fail_InitConfigTbl");
    UtTest_Add(Test_CI_InitApp_Nominal,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_InitApp_Nominal");
    UtTest_Add(Test_CI_CleanupCallback,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_CleanupCallback");
    UtTest_Add(Test_CI_AppMain_Fail_RegisterApp,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_AppMain_Fail_RegisterApp");
    UtTest_Add(Test_CI_AppMain_Fail_InitApp,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_AppMain_Fail_InitApp");
    UtTest_Add(Test_CI_AppMain_Fail_AcquireConfigPtrs,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_AppMain_Fail_AcquireConfigPtrs");
    UtTest_Add(Test_CI_AppMain_InvalidSchMessage,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_AppMain_InvalidSchMessage");
    UtTest_Add(Test_CI_AppMain_NoSchMessage,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_AppMain_NoSchMessage");
    UtTest_Add(Test_CI_AppMain_SchMessageTimeout,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_AppMain_SchMessageTimeout");
    UtTest_Add(Test_CI_AppMain_SchPipeError,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_AppMain_SchPipeError");
    UtTest_Add(Test_CI_AppMain_Nominal_SendHK,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_AppMain_Nominal_SendHK");
    UtTest_Add(Test_CI_AppMain_Nominal_Wakeup,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_AppMain_Nominal_Wakeup");
    UtTest_Add(Test_CI_AppMain_Nominal_ProcessTimeouts,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_AppMain_Nominal_ProcessTimeouts");
    UtTest_Add(Test_CI_AppMain_Nominal_IngestCommands,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_AppMain_Nominal_IngestCommands");
    UtTest_Add(Test_CI_ValidateCmd_Bad_CCSDS,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_ValidateCmd_Bad_CCSDS");
    UtTest_Add(Test_CI_ValidateCmd_No_SecHdr,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_ValidateCmd_No_SecHdr");
    UtTest_Add(Test_CI_ValidateCmd_Not_Cmd,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_ValidateCmd_Not_Cmd");
    UtTest_Add(Test_CI_ValidateCmd_Inv_Len,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_ValidateCmd_Inv_Len");
    UtTest_Add(Test_CI_ValidateCmd_Checksum_Invalid,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_ValidateCmd_Checksum_Invalid");
    UtTest_Add(Test_CI_ValidateCmd_Checksum_Valid,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_ValidateCmd_Checksum_Valid");
    UtTest_Add(Test_CI_ValidateCmd_No_Checksum_Go,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_ValidateCmd_No_Checksum_Go");
    UtTest_Add(Test_CI_ValidateCmd_No_Checksum_NoGo,
               CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_ValidateCmd_No_Checksum_NoGo");
    if(CI_INGEST_MODE == CI_BHV_OPTIMISTIC)
    {
        UtTest_Add(Test_CI_GetCmdAuthorized_Unreg_Opt,
                   CI_Test_Setup_InitTbls, CI_Test_TearDown,
                   "Test_CI_GetCmdAuthorized_Unreg_Opt");
    }
    else
    {
        UtTest_Add(Test_CI_GetCmdAuthorized_Unreg_Pess,
                   CI_Test_Setup_InitTbls, CI_Test_TearDown,
                   "Test_CI_GetCmdAuthorized_Unreg_Pess");
    }
    UtTest_Add(Test_CI_GetCmdAuthorized_1_Step,
               CI_Test_Setup_InitTbls, CI_Test_TearDown,
               "Test_CI_GetCmdAuthorized_1_Step");
    UtTest_Add(Test_CI_GetCmdAuthorized_2_Step_Unauth,
               CI_Test_Setup_InitTbls, CI_Test_TearDown,
               "Test_CI_GetCmdAuthorized_2_Step_Unauth");
    UtTest_Add(Test_CI_GetCmdAuthorized_2_Step_Auth,
               CI_Test_Setup_InitTbls, CI_Test_TearDown,
               "Test_CI_GetCmdAuthorized_2_Step_Auth");
    UtTest_Add(Test_CI_CmdAuthorize_Not_Reg,
               CI_Test_Setup_InitTbls, CI_Test_TearDown,
               "Test_CI_CmdAuthorize_Not_Reg");
    UtTest_Add(Test_CI_CmdAuthorize_Not_2_Step,
               CI_Test_Setup_InitTbls, CI_Test_TearDown,
               "Test_CI_CmdAuthorize_Not_2_Step");
    UtTest_Add(Test_CI_CmdAuthorize_Inv_State,
               CI_Test_Setup_InitTbls, CI_Test_TearDown,
               "Test_CI_CmdAuthorize_Inv_State");
    UtTest_Add(Test_CI_CmdAuthorize_Nominal,
               CI_Test_Setup_InitTbls, CI_Test_TearDown,
               "Test_CI_CmdAuthorize_Nominal");
    UtTest_Add(Test_CI_CmdDeauthorize_Not_Reg,
               CI_Test_Setup_InitTbls, CI_Test_TearDown,
               "Test_CI_CmdDeauthorize_Not_Reg");
    UtTest_Add(Test_CI_CmdDeauthorize_Not_2_Step,
               CI_Test_Setup_InitTbls, CI_Test_TearDown,
               "Test_CI_CmdDeauthorize_Not_2_Step");
    UtTest_Add(Test_CI_CmdDeauthorize_Not_Auth,
               CI_Test_Setup_InitTbls, CI_Test_TearDown,
               "Test_CI_CmdDeauthorize_Not_Auth");
    UtTest_Add(Test_CI_CmdDeauthorize_Nominal,
               CI_Test_Setup_InitTbls, CI_Test_TearDown,
               "Test_CI_CmdDeauthorize_Nominal");
    UtTest_Add(Test_CI_CmdRegister_Inv_MsgID,
               CI_Test_Setup_InitTbls, CI_Test_TearDown,
               "Test_CI_CmdRegister_Inv_MsgID");
    UtTest_Add(Test_CI_CmdRegister_Reg_Exists,
               CI_Test_Setup_InitTbls, CI_Test_TearDown,
               "Test_CI_CmdRegister_Reg_Exists");
    UtTest_Add(Test_CI_CmdRegister_Full_Table,
               CI_Test_Setup_InitTbls, CI_Test_TearDown,
               "Test_CI_CmdRegister_Full_Table");
    UtTest_Add(Test_CI_CmdRegister_Nominal,
               CI_Test_Setup_InitTbls, CI_Test_TearDown,
               "Test_CI_CmdRegister_Nominal");
    UtTest_Add(Test_CI_CmdDeregister_Inv_MsgID,
               CI_Test_Setup_InitTbls, CI_Test_TearDown,
               "Test_CI_CmdDeregister_Inv_MsgID");
    UtTest_Add(Test_CI_CmdDeregister_Not_Reg,
               CI_Test_Setup_InitTbls, CI_Test_TearDown,
               "Test_CI_CmdDeregister_Not_Reg");
    UtTest_Add(Test_CI_CmdDeregister_Nominal,
               CI_Test_Setup_InitTbls, CI_Test_TearDown,
               "Test_CI_CmdDeregister_Nominal");
    UtTest_Add(Test_CI_UpdateCmdReg_Inv_MsgID,
               CI_Test_Setup_InitTbls, CI_Test_TearDown,
               "Test_CI_UpdateCmdReg_Inv_MsgID");
    UtTest_Add(Test_CI_UpdateCmdReg_Not_Reg,
               CI_Test_Setup_InitTbls, CI_Test_TearDown,
               "Test_CI_UpdateCmdReg_Not_Reg");
    UtTest_Add(Test_CI_UpdateCmdReg_2_Step_Nominal,
               CI_Test_Setup_InitTbls, CI_Test_TearDown,
               "Test_CI_UpdateCmdReg_2_Step_Nominal");
    UtTest_Add(Test_CI_UpdateCmdReg_2_Step_Auth,
               CI_Test_Setup_InitTbls, CI_Test_TearDown,
               "Test_CI_UpdateCmdReg_2_Step_Auth");
    UtTest_Add(Test_CI_UpdateCmdReg_1_Step_Nominal,
               CI_Test_Setup_InitTbls, CI_Test_TearDown,
               "Test_CI_UpdateCmdReg_1_Step_Nominal");
    UtTest_Add(Test_CI_ProcessTimeouts_Nominal,
               CI_Test_Setup_InitTbls, CI_Test_TearDown,
               "Test_CI_ProcessTimeouts_Nominal");
    UtTest_Add(Test_CI_ProcessTimeouts_Update,
               CI_Test_Setup_InitTbls, CI_Test_TearDown,
               "Test_CI_ProcessTimeouts_Update");
}
