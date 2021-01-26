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


int32 hookCalledCount = 0;
int INIT_CUSTOM_RET;
int READ_MSG_RET;
int PBLIB_RET_CODE;

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
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for wakeup");
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
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_SubscribeEx for sendhk");
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
    UtAssert_True (result == expected, "InitPipe, fail CFE_SB_Subscribe for CMD");
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
    //UtAssert_True (result == expected, "InitApp, fail init data");
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
 * Test CI_AppMain(), Fail RegisterApp
 */
void Test_CI_AppMain_Fail_RegisterApp(void)
{
    /* fail the register app */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX, CFE_ES_ERR_APP_REGISTER, 1);

    /* Execute the function being tested */
    CI_AppMain();
}


/**
 * Test CI_AppMain(), Fail InitApp
 */
void Test_CI_AppMain_Fail_InitApp(void)
{
    /* fail the register app */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, CFE_EVS_APP_NOT_REGISTERED, 1);

    /* Execute the function being tested */
    CI_AppMain();
}


/**
 * Test CI_AppMain(), Fail AcquireConfigPtrs
 */
void Test_CI_AppMain_Fail_AcquireConfigPtrs(void)
{
    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, CFE_TBL_ERR_INVALID_HANDLE, 2);

    /* Execute the function being tested */
    CI_AppMain();
}


/**
 * Test CI_AppMain(), Invalid Schedule Message
 */
void Test_CI_AppMain_InvalidSchMessage(void)
{
    /* The following will emulate behavior of receiving a SCH message to send HK */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, 0, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    CI_AppMain();

}


/**
 * Hook to support: CI_AppMain(), Nominal - SendHK
 */
int32 Test_CI_AppMain_Nominal_SendHK_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    hookCalledCount++;

    return CFE_SUCCESS;
}

/**
 * Test CI_AppMain(), Nominal - SendHK
 */
void Test_CI_AppMain_Nominal_SendHK(void)
{
    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, CI_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Used to verify HK was transmitted correctly. */
    hookCalledCount = 0;
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX, &Test_CI_AppMain_Nominal_SendHK_SendMsgHook);

    /* Execute the function being tested */
    CI_AppMain();

    /* Verify results */
    UtAssert_True (hookCalledCount == 1, "AppMain_Nominal_SendHK");

}


/**
 * Test CI_AppMain(), Nominal - Wakeup
 */
void Test_CI_AppMain_Nominal_Wakeup(void)
{
    /* The following will emulate behavior of receiving a SCH message to WAKEUP */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, CI_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    CI_AppMain();

}

/**
 * Test CI_InitListenerTask(), Fail create child task
 */
void Test_CI_InitListenerTask_FailChild(void)
{
	/* Set to generate error message CI_LISTENER_CREATE_CHDTASK_ERR_EID */
	Ut_CFE_ES_SetReturnCode(UT_CFE_ES_CREATECHILDTASK_INDEX, -1, 1);

	/* Execute the function being tested */
	CI_InitListenerTask();

	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
	UtAssert_EventSent(CI_LISTENER_CREATE_CHDTASK_ERR_EID,
						CFE_EVS_ERROR,
						"",
						"Invalid message length");
}

/**
 * Test CI_InitListenerTask(), Nominal
 */
void Test_CI_InitListenerTask_Nominal(void)
{
	int32 expected = CFE_SUCCESS;
	int32 ret = -1;

	/* Set to mimic success */
	Ut_CFE_ES_SetReturnCode(UT_CFE_ES_CREATECHILDTASK_INDEX, 0, 1);

	/* Execute the function being tested */
	ret = CI_InitListenerTask();

	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==0,"Event Count = 0");
	UtAssert_True(ret==expected,"Return CFE_SUCCESS");
}

/**
 * Test CI_ListenerTaskMain(), Fail CFE register
 */
void Test_CI_ListenerTaskMain_FailRegister(void)
{
	/* Set to cause message "CI Listener Child Task Registration failed!" to be printed */
	Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERCHILDTASK_INDEX, -1, 1);

	/* Execute the function being tested */
	CI_InitListenerTask();

	/* If this occurs no events will be sent because the task isn't registered with CFE */
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==0,"Event Count = 0");
}

/**
 * Test CI_ListenerTaskMain(), Long Cmd
 */
void Test_CI_ListenerTaskMain_LongCmd(void)
{
	/* Prevent more than one loop */
	CI_AppData.IngestActive = FALSE;

	/* Set to cause to fail */
	READ_MSG_RET = LONG_CMD;

	/* Execute the function being tested */
	CI_ListenerTaskMain();

	/* Verify results */
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
	UtAssert_EventSent(CI_CMD_INGEST_ERR_EID, CFE_EVS_ERROR, "", "Cmd dropped, too long");
}

/**
 * Test CI_ListenerTaskMain(), Invalid Cmd
 */
void Test_CI_ListenerTaskMain_Inv_Cmd(void)
{
	/* Prevent more than one loop */
	CI_AppData.IngestActive = FALSE;

	/* Set to cause to fail */
	READ_MSG_RET = INV_CMD;

	/* Execute the function being tested */
	CI_ListenerTaskMain();

	/* Verify results */
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
	UtAssert_EventSent(CI_CMD_INVALID_EID, CFE_EVS_ERROR, "Rcvd invalid cmd", "Rcvd invalid cmd");
}

/**
 * Test CI_ListenerTaskMain(), CI Cmd
 */
void Test_CI_ListenerTaskMain_CI_Cmd(void)
{
	/* Prevent more than one loop */
	CI_AppData.IngestActive = FALSE;

	/* Set to cause to fail */
	READ_MSG_RET = CI_CMD;

	/* Execute the function being tested */
	CI_ListenerTaskMain();

	/* Verify results */
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
	UtAssert_EventSent(CI_CMD_INF_EID, CFE_EVS_INFORMATION, "", "Recvd NOOP cmd");
}

/**
 * Test CI_ListenerTaskMain(), External Cmd Unauthorized
 */
void Test_CI_ListenerTaskMain_Ext_Cmd_Unauth(void)
{
	uint32 actualEventQueueDepth;

	/* Prevent more than one loop */
	CI_AppData.IngestActive = FALSE;

	/* Set to cause to fail */
	READ_MSG_RET = EXT_STEP_2;

	/* Execute the function being tested */
	CI_ListenerTaskMain();

	/* Verify results */
	actualEventQueueDepth = Ut_CFE_EVS_GetEventQueueDepth();
	UtAssert_True(actualEventQueueDepth==2,"Event Count = 2");
	UtAssert_EventSent(CI_CMD_UNAUTHORIZED_EID, CFE_EVS_ERROR, "", "Cmd not authorized");
	UtAssert_True(CI_AppData.HkTlm.IngestErrorCount==1,"CmdErrCnt = 1");
}

/**
 * Test CI_ListenerTaskMain(), External Cmd Step 1
 */
void Test_CI_ListenerTaskMain_Ext_Cmd_Auth_Step_1(void)
{
	/* Prevent more than one loop */
	CI_AppData.IngestActive = FALSE;

	/* Set to cause to pass */
	READ_MSG_RET = EXT_STEP_1;

	/* Execute the function being tested */
	CI_ListenerTaskMain();

	/* Verify results */
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
	UtAssert_True(CI_AppData.HkTlm.IngestMsgCount==1,"Ingest Count = 1");
}

/**
 * Test CI_ListenerTaskMain(), External Cmd Step 2 Authorized
 */
void Test_CI_ListenerTaskMain_Ext_Cmd_Auth_Step_2(void)
{
	CI_CmdData_t		*CmdData = NULL;
	uint32              i = CI_INVALID_VALUE;

	/* Prevent more than one loop */
	CI_AppData.IngestActive = FALSE;

	/* Set to cause to pass */
	READ_MSG_RET = EXT_STEP_2_AUTH;

	/* Execute the function being tested */
	CI_ListenerTaskMain();

	/* Verify results */
	CmdData = CI_GetRegisterdCmd(TEST_MSG_ID, TEST_CC, &i);
	UtAssert_True(CI_AppData.HkTlm.IngestMsgCount==1,"Ingest count = 1");
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

/**
 * Test CI_ProcessNewAppCmds(), Reset
 */
void Test_CI_ProcessNewAppCmds_Reset(void)
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

	/* Set to cause to reset */
	CCSDS_WR_FC(cmdPkt->SecHdr, 1);

	/* Updates values */
	CI_AppData.HkTlm.usCmdCnt = 1;
	CI_AppData.HkTlm.usCmdErrCnt = 1;
	CI_AppData.HkTlm.IngestMsgCount = 1;
	CI_AppData.HkTlm.IngestErrorCount = 1;

	/* Execute the function being tested */
	CI_ProcessNewAppCmds(CmdMsgPtr);

	/* Verify results */
	UtAssert_True(CI_AppData.HkTlm.usCmdCnt == 0,"Reset param");
	UtAssert_True(CI_AppData.HkTlm.usCmdErrCnt == 0,"Reset param");
	UtAssert_True(CI_AppData.HkTlm.IngestMsgCount == 0,"Reset param");
	UtAssert_True(CI_AppData.HkTlm.IngestErrorCount == 0,"Reset param");
	UtAssert_EventSent(CI_CMD_INF_EID, CFE_EVS_INFORMATION, "", "Reset cmd event");
}

/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void CI_App_Test_AddTestCases(void)
{
    UtTest_Add(Test_CI_InitEvent_Fail_Register, CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_InitEvent_Fail_Register");
    UtTest_Add(Test_CI_InitPipe_Fail_CreateSCHPipe, CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_InitPipe_Fail_CreateSCHPipe");
    UtTest_Add(Test_CI_InitPipe_Fail_SubscribeWakeup, CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_InitPipe_Fail_SubscribeWakeup");
    UtTest_Add(Test_CI_InitPipe_Fail_SubscribeSendHK, CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_InitPipe_Fail_SubscribeSendHK");
    UtTest_Add(Test_CI_InitPipe_Fail_CreateCMDPipe, CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_InitPipe_Fail_CreateCMDPipe");
    UtTest_Add(Test_CI_InitPipe_Fail_SubscribeCMD, CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_InitPipe_Fail_SubscribeCMD");
    UtTest_Add(Test_CI_InitData, CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_InitData");
    UtTest_Add(Test_CI_InitApp_Fail_InitEvent, CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_InitApp_Fail_InitEvent");
    UtTest_Add(Test_CI_InitApp_Fail_InitPipe, CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_InitApp_Fail_InitPipe");
    UtTest_Add(Test_CI_InitApp_Fail_InitData, CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_InitApp_Fail_InitData");
    UtTest_Add(Test_CI_InitApp_Fail_InitConfigTbl, CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_InitApp_Fail_InitConfigTbl");
    UtTest_Add(Test_CI_InitApp_Nominal, CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_InitApp_Nominal");
    UtTest_Add(Test_CI_CleanupCallback, CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_CleanupCallback");
    UtTest_Add(Test_CI_AppMain_Fail_RegisterApp, CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_AppMain_Fail_RegisterApp");
    UtTest_Add(Test_CI_AppMain_Fail_InitApp, CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_AppMain_Fail_InitApp");
    UtTest_Add(Test_CI_AppMain_Fail_AcquireConfigPtrs, CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_AppMain_Fail_AcquireConfigPtrs");
    UtTest_Add(Test_CI_AppMain_InvalidSchMessage, CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_AppMain_InvalidSchMessage");
    UtTest_Add(Test_CI_AppMain_Nominal_SendHK, CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_AppMain_Nominal_SendHK");
    UtTest_Add(Test_CI_AppMain_Nominal_Wakeup, CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_AppMain_Nominal_Wakeup");
    UtTest_Add(Test_CI_InitListenerTask_FailChild, CI_Test_Setup, CI_Test_TearDown,
               "Test_CI_InitListenerTask_FailChild");
    UtTest_Add(Test_CI_InitListenerTask_Nominal, CI_Test_Setup, CI_Test_TearDown,
			   "Test_CI_InitListenerTask_Nominal");
    UtTest_Add(Test_CI_ListenerTaskMain_FailRegister, CI_Test_Setup_InitTbls, CI_Test_TearDown,
			   "Test_CI_ListenerTaskMain_FailRegister");
    UtTest_Add(Test_CI_ListenerTaskMain_LongCmd, CI_Test_Setup_InitTbls, CI_Test_TearDown,
    			"Test_CI_ListenerTaskMain_LongCmd");
    UtTest_Add(Test_CI_ValidateCmd_Bad_CCSDS, CI_Test_Setup, CI_Test_TearDown,
    			"Test_CI_ValidateCmd_Bad_CCSDS");
    UtTest_Add(Test_CI_ValidateCmd_No_SecHdr, CI_Test_Setup, CI_Test_TearDown,
    			"Test_CI_ValidateCmd_No_SecHdr");
    UtTest_Add(Test_CI_ValidateCmd_Not_Cmd, CI_Test_Setup, CI_Test_TearDown,
    			"Test_CI_ValidateCmd_Not_Cmd");
    UtTest_Add(Test_CI_ValidateCmd_Inv_Len, CI_Test_Setup, CI_Test_TearDown,
    			"Test_CI_ValidateCmd_Inv_Len");
    UtTest_Add(Test_CI_ValidateCmd_Checksum_Invalid, CI_Test_Setup, CI_Test_TearDown,
    			"Test_CI_ValidateCmd_Checksum_Invalid");
    UtTest_Add(Test_CI_ValidateCmd_Checksum_Valid, CI_Test_Setup, CI_Test_TearDown,
    			"Test_CI_ValidateCmd_Checksum_Valid");
    UtTest_Add(Test_CI_ValidateCmd_No_Checksum_Go, CI_Test_Setup, CI_Test_TearDown,
    			"Test_CI_ValidateCmd_No_Checksum_Go");
    UtTest_Add(Test_CI_ValidateCmd_No_Checksum_NoGo, CI_Test_Setup, CI_Test_TearDown,
    			"Test_CI_ValidateCmd_No_Checksum_NoGo");
    if(CI_INGEST_MODE == CI_BHV_OPTIMISTIC)
    {
        UtTest_Add(Test_CI_GetCmdAuthorized_Unreg_Opt, CI_Test_Setup_InitTbls, CI_Test_TearDown,
    			"Test_CI_GetCmdAuthorized_Unreg_Opt");
    }
    else
    {
        UtTest_Add(Test_CI_GetCmdAuthorized_Unreg_Pess, CI_Test_Setup_InitTbls, CI_Test_TearDown,
    			"Test_CI_GetCmdAuthorized_Unreg_Pess");
    }
    UtTest_Add(Test_CI_GetCmdAuthorized_1_Step, CI_Test_Setup_InitTbls, CI_Test_TearDown,
    			"Test_CI_GetCmdAuthorized_1_Step");
    UtTest_Add(Test_CI_GetCmdAuthorized_2_Step_Unauth, CI_Test_Setup_InitTbls, CI_Test_TearDown,
    			"Test_CI_GetCmdAuthorized_2_Step_Unauth");
    UtTest_Add(Test_CI_GetCmdAuthorized_2_Step_Auth, CI_Test_Setup_InitTbls, CI_Test_TearDown,
    			"Test_CI_GetCmdAuthorized_2_Step_Auth");
    UtTest_Add(Test_CI_CmdAuthorize_Not_Reg, CI_Test_Setup_InitTbls, CI_Test_TearDown,
    			"Test_CI_CmdAuthorize_Not_Reg");
    UtTest_Add(Test_CI_CmdAuthorize_Not_2_Step, CI_Test_Setup_InitTbls, CI_Test_TearDown,
    			"Test_CI_CmdAuthorize_Not_2_Step");
    UtTest_Add(Test_CI_CmdAuthorize_Inv_State, CI_Test_Setup_InitTbls, CI_Test_TearDown,
    			"Test_CI_CmdAuthorize_Inv_State");
    UtTest_Add(Test_CI_CmdAuthorize_Nominal, CI_Test_Setup_InitTbls, CI_Test_TearDown,
    			"Test_CI_CmdAuthorize_Nominal");
    UtTest_Add(Test_CI_CmdDeauthorize_Not_Reg, CI_Test_Setup_InitTbls, CI_Test_TearDown,
    			"Test_CI_CmdDeauthorize_Not_Reg");
    UtTest_Add(Test_CI_CmdDeauthorize_Not_2_Step, CI_Test_Setup_InitTbls, CI_Test_TearDown,
    			"Test_CI_CmdDeauthorize_Not_2_Step");
    UtTest_Add(Test_CI_CmdDeauthorize_Not_Auth, CI_Test_Setup_InitTbls, CI_Test_TearDown,
    			"Test_CI_CmdDeauthorize_Not_Auth");
    UtTest_Add(Test_CI_CmdDeauthorize_Nominal, CI_Test_Setup_InitTbls, CI_Test_TearDown,
    			"Test_CI_CmdDeauthorize_Nominal");
    UtTest_Add(Test_CI_CmdRegister_Inv_MsgID, CI_Test_Setup_InitTbls, CI_Test_TearDown,
    			"Test_CI_CmdRegister_Inv_MsgID");
    UtTest_Add(Test_CI_CmdRegister_Reg_Exists, CI_Test_Setup_InitTbls, CI_Test_TearDown,
    			"Test_CI_CmdRegister_Reg_Exists");
    UtTest_Add(Test_CI_CmdRegister_Full_Table, CI_Test_Setup_InitTbls, CI_Test_TearDown,
    			"Test_CI_CmdRegister_Full_Table");
    UtTest_Add(Test_CI_CmdRegister_Nominal, CI_Test_Setup_InitTbls, CI_Test_TearDown,
    			"Test_CI_CmdRegister_Nominal");
    UtTest_Add(Test_CI_CmdDeregister_Inv_MsgID, CI_Test_Setup_InitTbls, CI_Test_TearDown,
    			"Test_CI_CmdDeregister_Inv_MsgID");
    UtTest_Add(Test_CI_CmdDeregister_Not_Reg, CI_Test_Setup_InitTbls, CI_Test_TearDown,
    			"Test_CI_CmdDeregister_Not_Reg");
    UtTest_Add(Test_CI_CmdDeregister_Nominal, CI_Test_Setup_InitTbls, CI_Test_TearDown,
    			"Test_CI_CmdDeregister_Nominal");
    UtTest_Add(Test_CI_UpdateCmdReg_Inv_MsgID, CI_Test_Setup_InitTbls, CI_Test_TearDown,
    			"Test_CI_UpdateCmdReg_Inv_MsgID");
    UtTest_Add(Test_CI_UpdateCmdReg_Not_Reg, CI_Test_Setup_InitTbls, CI_Test_TearDown,
    			"Test_CI_UpdateCmdReg_Not_Reg");
    UtTest_Add(Test_CI_UpdateCmdReg_2_Step_Nominal, CI_Test_Setup_InitTbls, CI_Test_TearDown,
    			"Test_CI_UpdateCmdReg_2_Step_Nominal");
    UtTest_Add(Test_CI_UpdateCmdReg_2_Step_Auth, CI_Test_Setup_InitTbls, CI_Test_TearDown,
    			"Test_CI_UpdateCmdReg_2_Step_Auth");
    UtTest_Add(Test_CI_UpdateCmdReg_1_Step_Nominal, CI_Test_Setup_InitTbls, CI_Test_TearDown,
    			"Test_CI_UpdateCmdReg_1_Step_Nominal");
    UtTest_Add(Test_CI_ProcessTimeouts_Nominal, CI_Test_Setup_InitTbls, CI_Test_TearDown,
    			"Test_CI_ProcessTimeouts_Nominal");
    UtTest_Add(Test_CI_ListenerTaskMain_Inv_Cmd, CI_Test_Setup_InitTbls, CI_Test_TearDown,
				"Test_CI_ListenerTaskMain_Inv_Cmd");
    UtTest_Add(Test_CI_ListenerTaskMain_CI_Cmd, CI_Test_Setup_InitTbls, CI_Test_TearDown,
				"Test_CI_ListenerTaskMain_CI_Cmd");
    UtTest_Add(Test_CI_ListenerTaskMain_Ext_Cmd_Unauth, CI_Test_Setup_InitTbls, CI_Test_TearDown,
				"Test_CI_ListenerTaskMain_Ext_Cmd_Unath");
    UtTest_Add(Test_CI_ListenerTaskMain_Ext_Cmd_Auth_Step_1, CI_Test_Setup_InitTbls, CI_Test_TearDown,
				"Test_CI_ListenerTaskMain_Ext_Cmd_Auth_Step_1");
    UtTest_Add(Test_CI_ListenerTaskMain_Ext_Cmd_Auth_Step_2, CI_Test_Setup_InitTbls, CI_Test_TearDown,
				"Test_CI_ListenerTaskMain_Ext_Cmd_Auth_Step_2");
    UtTest_Add(Test_CI_ProcessTimeouts_Nominal, CI_Test_Setup_InitTbls, CI_Test_TearDown,
				"Test_CI_ProcessTimeouts_Nominal");
    UtTest_Add(Test_CI_ProcessTimeouts_Update, CI_Test_Setup_InitTbls, CI_Test_TearDown,
				"Test_CI_ProcessTimeouts_Update");
    UtTest_Add(Test_CI_ProcessNewAppCmds_Reset, CI_Test_Setup_InitTbls, CI_Test_TearDown,
				"Test_CI_ProcessNewAppCmds_Reset");


}


