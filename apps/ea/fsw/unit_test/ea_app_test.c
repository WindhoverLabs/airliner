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


#include "ea_app_test.h"
#include "ea_app.h"
#include "ea_test_utils.h"
#include "ea_custom_stub.h"
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

char APP_PATH[EA_MAX_PATH_LEN] = "/usr/bin/python";
char TEST_ARG[EA_MAX_PATH_LEN] = "noop.py";

int32 hookCalledCount = 0;

int32 EA_CMDS_TEST_CFE_ES_CreateChildTaskHook(uint32                          *TaskIdPtr,
                                              const char                      *TaskName,
                                              CFE_ES_ChildTaskMainFuncPtr_t    FunctionPtr,
                                              uint32                          *StackPtr,
                                              uint32                           StackSize,
                                              uint32                           Priority,
                                              uint32                           Flags)
{
    *TaskIdPtr = 5;

    return CFE_SUCCESS;
}

/**************************************************************************
 * Tests for EA_InitEvent()
 **************************************************************************/

/**
 * Test EA_InitEvent() with failed CFE_EVS_Register
 */
void Test_EA_InitEvent_Fail_Register(void)
{
    /* Set a fail result for EVS */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EVENTS_SERVICE | CFE_EVS_NOT_IMPLEMENTED;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = EA_InitEvent();

    /* Verify results */
    UtAssert_True (result == expected, "InitEvent, failed EVS Register");
}


/**************************************************************************
 * Tests for EA_InitPipe()
 **************************************************************************/
/**
 * Test EA_InitPipe(), fail SCH CFE_SB_CreatePipe
 */
void Test_EA_InitPipe_Fail_CreateSCHPipe(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = EA_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create SCH pipe");
}


/**
 * Test EA_InitPipe(), fail CFE_SB_SubscribeEx for wakeup
 */
void Test_EA_InitPipe_Fail_SubscribeWakeup(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 1);

    /* Execute the function being tested */
    result = EA_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for wakeup");
}


/**
 * Test EA_InitPipe(), fail CFE_SB_SubscribeEx for sendhk
 */
void Test_EA_InitPipe_Fail_SubscribeSendHK(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX, expected, 2);

    /* Execute the function being tested */
    result = EA_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for sendhk");
}


/**
 * Test EA_InitPipe(), fail CMD CFE_SB_CreatePipe
 */
void Test_EA_InitPipe_Fail_CreateCMDPipe(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 2);

    /* Execute the function being tested */
    result = EA_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail SB create CMD pipe");
}


/**
 * Test EA_InitPipe(), fail CFE_SB_Subscribe for CMD msg
 */
void Test_EA_InitPipe_Fail_SubscribeCMD(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_SOFTWARE_BUS_SERVICE | CFE_SB_NOT_IMPLEMENTED;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = EA_InitPipe();

    /* Verify results */
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_Subscribe for CMD");
}


/**************************************************************************
 * Tests for EA_InitData()
 **************************************************************************/
/**
 * Test EA_InitData()
 */
void Test_EA_InitData(void)
{
    /* Set a fail result */
    int32 result = -1;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = EA_InitData();

    /* Verify results */
    UtAssert_True (result == expected, "InitData");
}


/**************************************************************************
 * Tests for EA_InitApp()
 **************************************************************************/
/**
 * Test EA_InitApp(), fail init event
 */
void Test_EA_InitApp_Fail_InitEvent(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_EVS_APP_NOT_REGISTERED;

    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = EA_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init event");
}


/**
 * Test EA_InitApp(), fail init pipe
 */
void Test_EA_InitApp_Fail_InitPipe(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SB_BAD_ARGUMENT;

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = EA_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init pipe");
}


/**
 * Test EA_InitApp(), fail init data.
 * NOTE: no current way to fail EA_InitData() in default
 */
void Test_EA_InitApp_Fail_InitData(void)
{
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = EA_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, fail init data");
}


/**
 * Test EA_InitApp(), Nominal
 */
void Test_EA_InitApp_Nominal(void)
{
    /* Set a fail result for SB */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = EA_InitApp();

    /* Verify results */
    UtAssert_True (result == expected, "InitApp, nominal");
}

/**************************************************************************
 * Tests for EA_AppMain()
 **************************************************************************/
/**
 * Test EA_AppMain(), Fail RegisterApp
 */
void Test_EA_AppMain_Fail_RegisterApp(void)
{
    /* fail the register app */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX, CFE_ES_ERR_APP_REGISTER, 1);

    /* Execute the function being tested */
    EA_AppMain();
}


/**
 * Test EA_AppMain(), Fail InitApp
 */
void Test_EA_AppMain_Fail_InitApp(void)
{
    /* fail the register app */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, CFE_EVS_APP_NOT_REGISTERED, 1);

    /* Execute the function being tested */
    EA_AppMain();
}


/**
 * Test EA_AppMain(), Invalid Schedule Message
 */
void Test_EA_AppMain_InvalidSchMessage(void)
{
    /* The following will emulate behavior of receiving a SCH message to send HK */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, 0, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    EA_AppMain();

}


/**
 * Hook to support: EA_AppMain(), Nominal - SendHK
 */
int32 Test_EA_AppMain_Nominal_SendHK_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    /* TODO:  Test the contents of your HK message here. */

    hookCalledCount++;

    return CFE_SUCCESS;
}

/**
 * Test EA_AppMain(), Nominal - SendHK
 */
void Test_EA_AppMain_Nominal_SendHK(void)
{
    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, EA_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Used to verify HK was transmitted correctly. */
    hookCalledCount = 0;
    Ut_CFE_ES_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX, &Test_EA_AppMain_Nominal_SendHK_SendMsgHook);

    /* Execute the function being tested */
    EA_AppMain();

    /* Verify results */
    UtAssert_True (hookCalledCount == 1, "AppMain_Nominal_SendHK");

}


/**
 * Test EA_AppMain(), Nominal - Wakeup
 */
void Test_EA_AppMain_Nominal_Wakeup(void)
{
    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, EA_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    EA_AppMain();

}

/**
 * Test EA_ProcessNewAppCmds(), Invalid Command Code
 */
void Test_EA_ProcessNewAppCmds_InvalidCommand(void)
{
	EA_NoArgCmd_t InSchMsg;
    EA_NoArgCmd_t InInvalidCmd;
    int32         DataPipe;
    int32         CmdPipe;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("EA_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, EA_WAKEUP_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("EA_CMD_PIPE");
    CFE_SB_InitMsg ((CFE_SB_MsgPtr_t)&InInvalidCmd, EA_CMD_MID, sizeof(InInvalidCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InInvalidCmd, 100);
    Ut_CFE_SB_AddMsgToPipe(&InInvalidCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    EA_AppMain();

    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
    UtAssert_EventSent(EA_CC_ERR_EID, CFE_EVS_ERROR, "", "Cmd with Invalid Cmd Code Sent");
}

/**
 * Test EA_ProcessNewAppCmds(), NOOP command, Nominal
 */
void Test_EA_ProcessNewAppCmds_Noop_Nominal(void)
{
	EA_NoArgCmd_t InSchMsg;
    EA_NoArgCmd_t InNoopCmd;
    int32         DataPipe;
    int32         CmdPipe;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("EA_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, EA_WAKEUP_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("EA_CMD_PIPE");
    CFE_SB_InitMsg (&InNoopCmd, EA_CMD_MID, sizeof(InNoopCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InNoopCmd, EA_NOOP_CC);
    Ut_CFE_SB_AddMsgToPipe(&InNoopCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    EA_AppMain();

    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
    UtAssert_EventSent(EA_CMD_NOOP_EID, CFE_EVS_INFORMATION, "", "NOOP Cmd Event Sent");
}



/**
 * Test EA_ProcessNewAppCmds(), Reset command, Nominal
 */
void Test_EA_ProcessNewAppCmds_Reset_Nominal(void)
{
	EA_NoArgCmd_t InSchMsg;
    EA_NoArgCmd_t InResetCmd;
    int32         DataPipe;
    int32         CmdPipe;
	uint32        i = 0;
	char		  emptyString[EA_MAX_PATH_LEN];

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("EA_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, EA_WAKEUP_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("EA_CMD_PIPE");
    CFE_SB_InitMsg (&InResetCmd, EA_CMD_MID, sizeof(InResetCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InResetCmd, EA_RESET_CC);
    Ut_CFE_SB_AddMsgToPipe(&InResetCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Now give all the counters we're going to clear a value to ensure that
     * the reset command actually clears them. */
    EA_AppData.HkTlm.usCmdCnt = 1;
	EA_AppData.HkTlm.usCmdErrCnt = 2;
	EA_AppData.HkTlm.ActiveAppUtil = 3;
	EA_AppData.HkTlm.ActiveAppPID = 4;
	EA_AppData.HkTlm.LastAppStatus = 5;
	memset(EA_AppData.HkTlm.ActiveApp, 'a', EA_MAX_PATH_LEN);
	memset(EA_AppData.HkTlm.LastAppRun, 'b', EA_MAX_PATH_LEN);

    /* Set test variable to correct value */
    memset(emptyString, '\0', EA_MAX_PATH_LEN);

    /* Execute the function being tested */
    EA_AppMain();

    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
    UtAssert_EventSent(EA_CMD_RESET_EID, CFE_EVS_INFORMATION, "", "RESET Cmd Event Sent");
    UtAssert_True(EA_AppData.HkTlm.usCmdCnt == 0, "EA_AppData.HkTlm.usCmdCnt == 0");
    UtAssert_True(EA_AppData.HkTlm.usCmdErrCnt == 0, "EA_AppData.HkTlm.usCmdErrCnt == 0");
    UtAssert_True(EA_AppData.HkTlm.ActiveAppUtil == 0, "EA_AppData.HkTlm. == 0");
    UtAssert_True(EA_AppData.HkTlm.ActiveAppPID == 0, "EA_AppData.HkTlm. == 0");
    UtAssert_True(EA_AppData.HkTlm.LastAppStatus == 0, "EA_AppData.HkTlm. == 0");
    UtAssert_StrCmp(EA_AppData.HkTlm.ActiveApp, emptyString,"EA_AppData.HkTlm.ActiveApp == NULL");
    UtAssert_StrCmp(EA_AppData.HkTlm.LastAppRun, emptyString,"EA_AppData.HkTlm.ActiveApp == NULL");
}

/**
 * Test EA_ProcessNewAppCmds(), Start App command, RcvCmd
 */
void Test_EA_ProcessNewAppCmds_StartApp_RcvCmd(void)
{
	EA_NoArgCmd_t InSchMsg;
	EA_StartCmd_t InStartCmd;
    int32         DataPipe;
    int32         CmdPipe;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("EA_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, EA_WAKEUP_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("EA_CMD_PIPE");
    CFE_SB_InitMsg (&InStartCmd, EA_CMD_MID, sizeof(InStartCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InStartCmd, EA_START_APP_CC);
    Ut_CFE_SB_AddMsgToPipe(&InStartCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    EA_AppMain();

    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==3,"Event Count = 3");
    UtAssert_EventSent(EA_CMD_INF_EID, CFE_EVS_INFORMATION, "Recvd Start App cmd (2)", "Recvd Start App cmd");
}

/**
 * Test EA_ProcessNewAppCmds(), Start App command, Invalid size
 */
void Test_EA_ProcessNewAppCmds_StartApp_InvalidSize(void)
{
	EA_NoArgCmd_t BadStartCmd;

	CFE_SB_InitMsg (&BadStartCmd, EA_CMD_MID, sizeof(BadStartCmd), TRUE);

	/* Execute the function being tested */
	EA_StartApp((CFE_SB_MsgPtr_t)(&BadStartCmd));

	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
	UtAssert_EventSent(EA_MSGLEN_ERR_EID, CFE_EVS_ERROR, "", "Invalid message length");
	UtAssert_True(EA_AppData.HkTlm.usCmdErrCnt==1,"Command Error Count = 1");
}

/**
 * Test EA_ProcessNewAppCmds(), Start App command, No args 
 */
void Test_EA_ProcessNewAppCmds_StartApp_NoArgs(void)
{
	EA_StartCmd_t InStartCmd;

	CFE_SB_InitMsg (&InStartCmd, EA_CMD_MID, sizeof(InStartCmd), TRUE);

	/* Execute the function being tested */
	EA_StartApp((CFE_SB_MsgPtr_t)(&InStartCmd));

	/* Verify results */
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
	UtAssert_EventSent(EA_APP_ERR_EID, CFE_EVS_ERROR,
						"Specified app does not exist", "Invalid argument event sent");
	UtAssert_True(EA_AppData.HkTlm.usCmdErrCnt==1,"Command Error Count = 1");
}

/**
 * Test EA_ProcessNewAppCmds(), Start App command, Invalid app arg 
 */
void Test_EA_ProcessNewAppCmds_StartApp_InvalidAppArg(void)
{
//	EA_StartCmd_t InStartCmd;
//
//	CFE_SB_InitMsg (&InStartCmd, EA_CMD_MID, sizeof(InStartCmd), TRUE);
//	strcpy(InStartCmd.interpreter, "InvalidDirectory");
//	strcpy(InStartCmd.script, TEST_ARG);
//
//	/* Execute the function being tested */
//	EA_StartApp((CFE_SB_MsgPtr_t)(&InStartCmd));
//
//	/* Verify results */
//	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
//	UtAssert_EventSent(EA_APP_ARG_ERR_EID, CFE_EVS_ERROR,
//						"Specified app does not exist", "Invalid argument event sent");
//	UtAssert_True(EA_AppData.HkTlm.usCmdErrCnt==1,"Command Error Count = 1");
}

/**
 * Test EA_ProcessNewAppCmds(), Start App command, Invalid arg arg
 */
void Test_EA_ProcessNewAppCmds_StartApp_InvalidArgArg(void)
{
//	EA_StartCmd_t InStartCmd;
//
//	CFE_SB_InitMsg (&InStartCmd, EA_CMD_MID, sizeof(InStartCmd), TRUE);
//	strcpy(InStartCmd.interpreter, "InvalidDirectory");
//	strcpy(InStartCmd.script, "InvalidDirectory");
//
//	/* Execute the function being tested */
//	EA_StartApp((CFE_SB_MsgPtr_t)(&InStartCmd));
//
//	/* Verify results */
//	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
//	UtAssert_EventSent(EA_APP_ARG_ERR_EID, CFE_EVS_ERROR,
//						"Specified app does not exist", "Invalid argument event sent");
//	UtAssert_True(EA_AppData.HkTlm.usCmdErrCnt==1,"Command Error Count = 1");
}

/**
 * Test EA_ProcessNewAppCmds(), Start App command, App already running
 */
void Test_EA_ProcessNewAppCmds_StartApp_AlreadyActive(void)
{
	EA_StartCmd_t InStartCmd;

	CFE_SB_InitMsg (&InStartCmd, EA_CMD_MID, sizeof(InStartCmd), TRUE);

	EA_AppData.ChildAppTaskInUse = TRUE;

	/* Execute the function being tested */
	EA_StartApp((CFE_SB_MsgPtr_t)(&InStartCmd));

	/* Verify results */
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
	UtAssert_EventSent(EA_CHILD_TASK_START_ERR_EID, CFE_EVS_ERROR,
						"Create child tasked failed. A child task is in use", "Child task already started");
	UtAssert_True(EA_AppData.HkTlm.usCmdErrCnt==1,"Command Error Count = 1");
}

/**
 * Test EA_ProcessNewAppCmds(), Start App command, Nominal
 */
void Test_EA_ProcessNewAppCmds_StartApp_Nominal(void)
{
//	EA_StartCmd_t InStartCmd;
//
//	CFE_SB_InitMsg (&InStartCmd, EA_CMD_MID, sizeof(InStartCmd), TRUE);
//	strcpy(InStartCmd.interpreter, APP_PATH);
//	strcpy(InStartCmd.script, TEST_ARG);
//
//	/* Sets ChildTaskID to 5 and returns CFE_SUCCESS */
//    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_CREATECHILDTASK_INDEX, &EA_CMDS_TEST_CFE_ES_CreateChildTaskHook);
//
//	/* Execute the function being tested */
//	EA_StartApp((CFE_SB_MsgPtr_t)(&InStartCmd));
//
//	/* Verify results */
//	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
//	UtAssert_EventSent(EA_CHILD_TASK_START_EID, CFE_EVS_DEBUG, "", "Child task started");
//	UtAssert_True(EA_AppData.ChildAppTaskInUse==TRUE,"Child task in use set to true");
//	UtAssert_True(EA_AppData.ChildAppTaskID==5,"Child task ID set");
//	UtAssert_True(EA_AppData.HkTlm.usCmdCnt==0,"Command Count = 0"); // 0 because incremented in child thread
//	UtAssert_True(EA_AppData.HkTlm.usCmdErrCnt==0,"Command Error Count = 0");
}

/**
 * Test EA_ProcessNewAppCmds(), Start App command, Fail Create Child task
 */
void Test_EA_ProcessNewAppCmds_StartApp_CreateChildTaskError(void)
{
//	EA_StartCmd_t InStartCmd;
//
//	CFE_SB_InitMsg (&InStartCmd, EA_CMD_MID, sizeof(InStartCmd), TRUE);
//	strcpy(InStartCmd.interpreter, APP_PATH);
//	strcpy(InStartCmd.script, TEST_ARG);
//
//	/* Set to generate error message EA_CHILD_TASK_START_ERR_EID */
//    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_CREATECHILDTASK_INDEX, -1, 1);
//
//	/* Execute the function being tested */
//	EA_StartApp((CFE_SB_MsgPtr_t)(&InStartCmd));
//
//	/* Verify results */
//	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
//	UtAssert_EventSent(EA_CHILD_TASK_START_ERR_EID, CFE_EVS_ERROR, "", "Child task failed to start");
//	UtAssert_True(EA_AppData.ChildAppTaskInUse==FALSE,"Child task in use set to false");
//	UtAssert_True(EA_AppData.ChildAppTaskID==0,"Child task ID not set");
//	UtAssert_True(EA_AppData.HkTlm.usCmdErrCnt==1,"Command Error Count = 1");
}

/**
 * Test EA_ProcessNewAppCmds(), Stop App command, RcvCmd
 */
void Test_EA_ProcessNewAppCmds_TermApp_RcvCmd(void)
{
	EA_NoArgCmd_t InSchMsg;
	EA_NoArgCmd_t InStopCmd;
    int32         DataPipe;
    int32         CmdPipe;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("EA_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, EA_WAKEUP_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("EA_CMD_PIPE");
    CFE_SB_InitMsg (&InStopCmd, EA_CMD_MID, sizeof(InStopCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&InStopCmd, EA_TERM_APP_CC);
    Ut_CFE_SB_AddMsgToPipe(&InStopCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    EA_AppMain();

    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==3,"Event Count = 3");
    UtAssert_EventSent(EA_CMD_INF_EID, CFE_EVS_INFORMATION, "Recvd Terminate App cmd (3)", "Recvd Terminate App cmd");
}

/**
 * Test EA_ProcessNewAppCmds(), Stop App command, Invalid Size
 */
void Test_EA_ProcessNewAppCmds_TermApp_InvalidSize(void)
{
	EA_StartCmd_t BadTermCmd;

	CFE_SB_InitMsg (&BadTermCmd, EA_CMD_MID, sizeof(BadTermCmd), TRUE);

	/* Execute the function being tested */
	EA_TermApp((CFE_SB_MsgPtr_t)(&BadTermCmd));

	/* Verify results */
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
	UtAssert_EventSent(EA_MSGLEN_ERR_EID, CFE_EVS_ERROR, "", "Invalid message length");
	UtAssert_True(EA_AppData.HkTlm.usCmdErrCnt==1,"Command Error Count = 1");
}

/**
 * Test EA_ProcessNewAppCmds(), Stop App command, None running
 */
void Test_EA_ProcessNewAppCmds_TermApp_NoneActive(void)
{
	EA_NoArgCmd_t StopCmd;

	CFE_SB_InitMsg (&StopCmd, EA_CMD_MID, sizeof(StopCmd), TRUE);

	/* Execute the function being tested */
	EA_TermApp((CFE_SB_MsgPtr_t)(&StopCmd));

	/* Verify results */
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
	UtAssert_EventSent(EA_CMD_ERR_EID, CFE_EVS_ERROR,
						"Attempted to terminate app while none executing", "Terminating when no active task");
	UtAssert_True(EA_AppData.HkTlm.usCmdErrCnt==1,"Command Error Count = 1");
}

/**
 * Test EA_ProcessNewAppCmds(), Stop App command, Nominal
 */
void Test_EA_ProcessNewAppCmds_TermApp_Nominal(void)
{
	EA_NoArgCmd_t StopCmd;

	CFE_SB_InitMsg (&StopCmd, EA_CMD_MID, sizeof(StopCmd), TRUE);

	EA_AppData.HkTlm.ActiveAppPID = 1;

	/* Execute the function being tested */
	EA_TermApp((CFE_SB_MsgPtr_t)(&StopCmd));

	/* Verify results */
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
	UtAssert_EventSent(EA_INF_APP_TERM_EID, CFE_EVS_INFORMATION, 
						"External application terminated", "External application terminated");
	UtAssert_True(EA_AppData.HkTlm.usCmdCnt==1,"Command Count = 1");
}

/**
 * Test EA_ProcessNewAppCmds(), Perfmon command, RcvCmd
 */
void Test_EA_ProcessNewAppCmds_Perfmon_RcvCmd(void)
{
	EA_NoArgCmd_t InSchMsg;
	EA_NoArgCmd_t PerfmonCmd;
    int32         DataPipe;
    int32         CmdPipe;

    /* The following will emulate behavior of receiving a SCH message to WAKEUP,
       and gives it a command to process. */
    DataPipe = Ut_CFE_SB_CreatePipe("EA_SCH_PIPE");
    CFE_SB_InitMsg (&InSchMsg, EA_WAKEUP_MID, sizeof(InSchMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe(&InSchMsg, DataPipe);

    CmdPipe = Ut_CFE_SB_CreatePipe("EA_CMD_PIPE");
    CFE_SB_InitMsg (&PerfmonCmd, EA_CMD_MID, sizeof(PerfmonCmd), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&PerfmonCmd, EA_PERFMON_CC);
    Ut_CFE_SB_AddMsgToPipe(&PerfmonCmd, CmdPipe);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    EA_AppMain();

    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==2,"Event Count = 2");
    UtAssert_EventSent(EA_CMD_INF_EID, CFE_EVS_INFORMATION, "Recvd Perfmon cmd (4)", "Recvd Perfmon App cmd");
}

/**
 * Test EA_Perfmon(), No active app
 */
void Test_EA_Perfmon_NoApp(void)
{
	EA_NoArgCmd_t PerfmonCmd;

	CFE_SB_InitMsg (&PerfmonCmd, EA_CMD_MID, sizeof(PerfmonCmd), TRUE);

	/* Ensure active app set to 0 */
	EA_AppData.HkTlm.ActiveAppPID = 0;

	/* Execute the function being tested */
	EA_Perfmon();

	/* Verify results - Nothing should happen */
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==0,"Event Count = 0");
	UtAssert_True(EA_AppData.HkTlm.ActiveAppUtil==0,"Active Utilization = 0");
}

/**
 * Test EA_Perfmon(), safe utilization
 */
void Test_EA_Perfmon_SafeUtil(void)
{
	EA_NoArgCmd_t PerfmonCmd;

	CFE_SB_InitMsg (&PerfmonCmd, EA_CMD_MID, sizeof(PerfmonCmd), TRUE);

	/* Ensure active app set to nonzero */
	EA_AppData.HkTlm.ActiveAppPID = 1;

	/* Execute the function being tested */
	EA_Perfmon();

	/* Verify results */
	// No events tripped under safe utilization
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==0,"Event Count = 0");
	UtAssert_True(EA_AppData.HkTlm.ActiveAppUtil==1,"Active Utilization = 1"); 
}

/**
 * Test EA_Perfmon(), unsafe utilization
 */
void Test_EA_Perfmon_WarnUtil(void)
{
	EA_NoArgCmd_t PerfmonCmd;

	CFE_SB_InitMsg (&PerfmonCmd, EA_CMD_MID, sizeof(PerfmonCmd), TRUE);

	/* Ensure active app set to nonzero and util exceeds threshold */
	EA_AppData.HkTlm.ActiveAppPID = 1;
	EA_AppData.HkTlm.ActiveAppUtil = EA_APP_UTIL_THRESHOLD + 1;

	/* Execute the function being tested */
	EA_Perfmon();

	/* Verify results */
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
	UtAssert_EventSent(EA_WARN_APP_UTIL_EID, CFE_EVS_INFORMATION, 
						"External application exceeded utilization threshold", "App utilization warning");
}




/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void EA_App_Test_AddTestCases(void)
{
    UtTest_Add(Test_EA_InitEvent_Fail_Register, EA_Test_Setup, EA_Test_TearDown,
               "Test_EA_InitEvent_Fail_Register");

    UtTest_Add(Test_EA_InitPipe_Fail_CreateSCHPipe, EA_Test_Setup, EA_Test_TearDown,
               "Test_EA_InitPipe_Fail_CreateSCHPipe");
    UtTest_Add(Test_EA_InitPipe_Fail_SubscribeWakeup, EA_Test_Setup, EA_Test_TearDown,
               "Test_EA_InitPipe_Fail_SubscribeWakeup");
    UtTest_Add(Test_EA_InitPipe_Fail_SubscribeSendHK, EA_Test_Setup, EA_Test_TearDown,
               "Test_EA_InitPipe_Fail_SubscribeSendHK");
    UtTest_Add(Test_EA_InitPipe_Fail_CreateCMDPipe, EA_Test_Setup, EA_Test_TearDown,
               "Test_EA_InitPipe_Fail_CreateCMDPipe");
    UtTest_Add(Test_EA_InitPipe_Fail_SubscribeCMD, EA_Test_Setup, EA_Test_TearDown,
               "Test_EA_InitPipe_Fail_SubscribeCMD");

    UtTest_Add(Test_EA_InitData, EA_Test_Setup, EA_Test_TearDown,
               "Test_EA_InitData");

    UtTest_Add(Test_EA_InitApp_Fail_InitEvent, EA_Test_Setup, EA_Test_TearDown,
               "Test_EA_InitApp_Fail_InitEvent");
    UtTest_Add(Test_EA_InitApp_Fail_InitPipe, EA_Test_Setup, EA_Test_TearDown,
               "Test_EA_InitApp_Fail_InitPipe");
    UtTest_Add(Test_EA_InitApp_Fail_InitData, EA_Test_Setup, EA_Test_TearDown,
               "Test_EA_InitApp_Fail_InitData");
    UtTest_Add(Test_EA_InitApp_Nominal, EA_Test_Setup, EA_Test_TearDown,
               "Test_EA_InitApp_Nominal");

    UtTest_Add(Test_EA_AppMain_Fail_RegisterApp, EA_Test_Setup, EA_Test_TearDown,
               "Test_EA_AppMain_Fail_RegisterApp");
    UtTest_Add(Test_EA_AppMain_Fail_InitApp, EA_Test_Setup, EA_Test_TearDown,
               "Test_EA_AppMain_Fail_InitApp");
    UtTest_Add(Test_EA_AppMain_InvalidSchMessage, EA_Test_Setup, EA_Test_TearDown,
               "Test_EA_AppMain_InvalidSchMessage");
    UtTest_Add(Test_EA_AppMain_Nominal_SendHK, EA_Test_Setup, EA_Test_TearDown,
               "Test_EA_AppMain_Nominal_SendHK");
    UtTest_Add(Test_EA_AppMain_Nominal_Wakeup, EA_Test_Setup, EA_Test_TearDown,
               "Test_EA_AppMain_Nominal_Wakeup");
    UtTest_Add(Test_EA_ProcessNewAppCmds_InvalidCommand, EA_Test_Setup, EA_Test_TearDown,
                   "Test_EA_ProcessNewAppCmds_InvalidCommand");
    UtTest_Add(Test_EA_ProcessNewAppCmds_Noop_Nominal, EA_Test_Setup, EA_Test_TearDown,
                       "Test_EA_ProcessNewAppCmds_Noop_Nominal");
    UtTest_Add(Test_EA_ProcessNewAppCmds_Reset_Nominal, EA_Test_Setup, EA_Test_TearDown,
                       "Test_EA_ProcessNewAppCmds_Reset_Nominal");
	UtTest_Add(Test_EA_ProcessNewAppCmds_StartApp_InvalidSize, EA_Test_Setup, EA_Test_TearDown,
                       "Test_EA_ProcessNewAppCmds_StartApp_InvalidSize");
	UtTest_Add(Test_EA_ProcessNewAppCmds_StartApp_NoArgs, EA_Test_Setup, EA_Test_TearDown,
                       "Test_EA_ProcessNewAppCmds_StartApp_NoArgs");
    UtTest_Add(Test_EA_ProcessNewAppCmds_StartApp_InvalidAppArg, EA_Test_Setup, EA_Test_TearDown,
                       "Test_EA_ProcessNewAppCmds_StartApp_InvalidAppArg");
    UtTest_Add(Test_EA_ProcessNewAppCmds_StartApp_InvalidArgArg, EA_Test_Setup, EA_Test_TearDown,
                       "Test_EA_ProcessNewAppCmds_StartApp_InvalidArgArg");
	UtTest_Add(Test_EA_ProcessNewAppCmds_StartApp_AlreadyActive, EA_Test_Setup, EA_Test_TearDown,
                       "Test_EA_ProcessNewAppCmds_StartApp_AlreadyActive");
	UtTest_Add(Test_EA_ProcessNewAppCmds_StartApp_Nominal, EA_Test_Setup, EA_Test_TearDown,
                       "Test_EA_ProcessNewAppCmds_StartApp_Nominal");
	UtTest_Add(Test_EA_ProcessNewAppCmds_StartApp_CreateChildTaskError, EA_Test_Setup, EA_Test_TearDown,
                       "Test_EA_ProcessNewAppCmds_StartApp_CreateChildTaskError");
	UtTest_Add(Test_EA_ProcessNewAppCmds_TermApp_InvalidSize, EA_Test_Setup, EA_Test_TearDown,
                       "Test_EA_ProcessNewAppCmds_StopApp_InvalidSize");
	UtTest_Add(Test_EA_ProcessNewAppCmds_TermApp_NoneActive, EA_Test_Setup, EA_Test_TearDown,
                       "Test_EA_ProcessNewAppCmds_StopApp_NoneActive");
	UtTest_Add(Test_EA_ProcessNewAppCmds_TermApp_Nominal, EA_Test_Setup, EA_Test_TearDown,
                       "Test_EA_ProcessNewAppCmds_StopApp_Nominal");
	UtTest_Add(Test_EA_Perfmon_NoApp, EA_Test_Setup, EA_Test_TearDown,
                       "Test_EA_Perfmon_NoApp");
	UtTest_Add(Test_EA_Perfmon_SafeUtil, EA_Test_Setup, EA_Test_TearDown,
                       "Test_EA_Perfmon_SafeUtil");
	UtTest_Add(Test_EA_Perfmon_WarnUtil, EA_Test_Setup, EA_Test_TearDown,
                       "Test_EA_Perfmon_WarnUtil");
	UtTest_Add(Test_EA_ProcessNewAppCmds_StartApp_RcvCmd, EA_Test_Setup, EA_Test_TearDown,
	                       "Test_EA_ProcessNewAppCmds_StartApp_RcvCmd");
	UtTest_Add(Test_EA_ProcessNewAppCmds_TermApp_RcvCmd, EA_Test_Setup, EA_Test_TearDown,
		                       "Test_EA_ProcessNewAppCmds_TermApp_RcvCmd");
	UtTest_Add(Test_EA_ProcessNewAppCmds_Perfmon_RcvCmd, EA_Test_Setup, EA_Test_TearDown,
			                       "Test_EA_ProcessNewAppCmds_Perfmon_RcvCmd");
}
