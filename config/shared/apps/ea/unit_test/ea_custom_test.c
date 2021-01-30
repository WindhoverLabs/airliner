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

#include "ea_custom_test.h"
#include "ea_test_utils.h"
#include "ea_app.h"
#include "ea_custom.h"
#include "ea_msg.h"

#include "uttest.h"
#include "ut_osapi_stubs.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_es_hooks.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_cfe_time_stubs.h"

char CUSTOM_APP_PATH[OS_MAX_PATH_LEN] = "/usr/bin/python";
char CUSTOM_TEST_ARG[OS_MAX_PATH_LEN] = "noop.py";

int FORK_RET_CODE;
int EXECVP_RET_CODE;
int KILL_RET_CODE;
int WAITPID_RET_CODE;

int PERFMON_SAMPLES = 50;

/**
 * Test EA_StartAppCustom(), Register Child Task Error
 */
void Test_EA_Custom_StartApp_RegisterChildTaskError(void)
{
    /* Set to cause message "StartApp Child Task Registration failed!" to be printed */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERCHILDTASK_INDEX, -1, 1);

    /* Execute the function being tested */
    EA_StartAppCustom();

    /* Verify results */
    /* Note: Cannot verify line OS_printf("StartApp Child Task Registration failed!\n") */
    UtAssert_True (EA_AppData.ChildAppTaskID == 0, "Child task ID = 0");
    UtAssert_True (EA_AppData.ChildAppTaskInUse == FALSE, "Child task not in use");
}

/**
 * Test EA_StartAppCustom(), Failed fork
 */
void Test_EA_Custom_StartApp_FailFork(void)
{
	/* Set to cause fork to fail */
	FORK_RET_CODE = -1;

	/* Execute the function being tested */
	EA_StartAppCustom();

	/* Verify results */
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
	UtAssert_EventSent(EA_CMD_ERR_EID, CFE_EVS_ERROR, 
						"Error starting new process", "Error starting new process");
	UtAssert_True(EA_AppData.HkTlm.usCmdErrCnt==1,"Command Error Count = 1");
}

/**
 * Test EA_StartAppCustom(), Nominal
 */
void Test_EA_Custom_StartApp_Nominal(void)
{
	/* Set so everything looks valid */
	strncpy(EA_AppData.ChildData.Cmd, CUSTOM_APP_PATH, OS_MAX_PATH_LEN);
	strncpy(EA_AppData.ChildData.Args, CUSTOM_TEST_ARG, OS_MAX_PATH_LEN);
	FORK_RET_CODE = 1;
	WAITPID_RET_CODE = 2;

	/* Execute the function being tested */
	EA_StartAppCustom();

	/* Verify results */
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
	UtAssert_EventSent(EA_INF_APP_START_EID, CFE_EVS_INFORMATION, "External application started", "External application started");
	UtAssert_True(EA_AppData.HkTlm.usCmdCnt==1,"Command Count = 1");

	/* Verify cleanup too since app instantly exits for test */
	UtAssert_True(EA_AppData.HkTlm.ActiveAppPID==0,"ActiveAppPID = 0");
	UtAssert_True(EA_AppData.HkTlm.ActiveAppUtil==0,"ActiveAppUtil = 0");//
	UtAssert_True(EA_AppData.ChildAppTaskInUse==FALSE,"ChildAppTaskInUse = FALSE");
	UtAssert_StrCmp(EA_AppData.HkTlm.LastAppRun, CUSTOM_TEST_ARG, "Last Active App set");
	UtAssert_True(EA_AppData.HkTlm.LastAppStatus==2,"LastAppStatus = waitpid return code");
}

/**
 * Test EA_TermAppCustom(), No app running
 */
void Test_EA_Custom_TermApp_NoneActive(void)
{
	/* Set to cause conditional to fail */
	EA_AppData.HkTlm.ActiveAppPID = 0;

	/* Execute the function being tested */
	EA_TermAppCustom();

	/* Verify results */
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
	UtAssert_EventSent(EA_CMD_ERR_EID, CFE_EVS_ERROR, 
						"Attempted to terminate app while none executing", "No active app");
	UtAssert_True(EA_AppData.HkTlm.usCmdErrCnt==1,"Command Error Count = 1"); 
}

/**
 * Test EA_TermAppCustom(), Failed kill
 */
void Test_EA_Custom_TermApp_KillFail(void)
{
	/* Set to cause kill to fail */
	EA_AppData.HkTlm.ActiveAppPID = 1;
	KILL_RET_CODE = -1;

	/* Execute the function being tested */
	EA_TermAppCustom();

	/* Verify results */
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
	UtAssert_EventSent(EA_CMD_ERR_EID, CFE_EVS_ERROR, 
						"Unable to terminate application", "Unable to terminate application");
	UtAssert_True(EA_AppData.HkTlm.usCmdErrCnt==1,"Command Error Count = 1");
}

/**
 * Test EA_TermAppCustom(), Nominal
 */
void Test_EA_Custom_TermApp_Nominal(void)
{
	/* Set active app params that TermApp will clear or copy to last app params */
	EA_AppData.HkTlm.ActiveAppPID = 1;
	EA_AppData.HkTlm.ActiveAppUtil = 2;
	EA_AppData.ChildAppTaskInUse = TRUE;
	strncpy(EA_AppData.HkTlm.ActiveApp, CUSTOM_TEST_ARG, OS_MAX_PATH_LEN);
	KILL_RET_CODE = 0;

	/* Execute the function being tested */
	EA_TermAppCustom();

	/* Verify results */
	UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth()==1,"Event Count = 1");
	UtAssert_EventSent(EA_INF_APP_TERM_EID, CFE_EVS_INFORMATION,"", "External application terminated");
	UtAssert_True(EA_AppData.HkTlm.usCmdCnt==1,"Command Count = 1");
	UtAssert_True(EA_AppData.HkTlm.ActiveAppPID==0,"ActiveAppPID = 0");
	UtAssert_True(EA_AppData.HkTlm.ActiveAppUtil==0,"ActiveAppUtil = 0");//
	UtAssert_True(EA_AppData.ChildAppTaskInUse==FALSE,"ChildAppTaskInUse = FALSE");
	UtAssert_StrCmp(EA_AppData.HkTlm.LastAppRun, CUSTOM_TEST_ARG, "Last Active App set");
	UtAssert_True(EA_AppData.HkTlm.LastAppStatus==-1,"LastAppStatus = -1");
}

/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void EA_Custom_Test_AddTestCases(void)
{
    UtTest_Add(Test_EA_Custom_StartApp_RegisterChildTaskError, EA_Test_Setup, EA_Test_TearDown, "Test_EA_Custom_StartApp_RegisterChildTaskError");
	UtTest_Add(Test_EA_Custom_StartApp_FailFork, EA_Test_Setup, EA_Test_TearDown, "Test_EA_Custom_StartApp_FailFork");
    UtTest_Add(Test_EA_Custom_StartApp_Nominal, EA_Test_Setup, EA_Test_TearDown, "Test_EA_Custom_StartApp_Nominal");
    UtTest_Add(Test_EA_Custom_TermApp_NoneActive, EA_Test_Setup, EA_Test_TearDown, "Test_EA_Custom_TermApp_NoneActive");
	UtTest_Add(Test_EA_Custom_TermApp_KillFail, EA_Test_Setup, EA_Test_TearDown, "Test_EA_Custom_TermApp_KillFail");
	UtTest_Add(Test_EA_Custom_TermApp_Nominal, EA_Test_Setup, EA_Test_TearDown, "Test_EA_Custom_TermApp_Nominal");
}

