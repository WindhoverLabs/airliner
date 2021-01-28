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

#include "ea_perfmon_test.h"
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

char PERFMON_APP_PATH[OS_MAX_PATH_LEN] = "/usr/bin/python";
char PERFMON_TEST_ARG_FIB[OS_MAX_PATH_LEN] = "fib.py";
char PERFMON_TEST_ARG_SLP[OS_MAX_PATH_LEN] = "sleep.py";
int PERFMON_SAMPLES = 5;

/**
 * Test EA_PerfmonCustom(), Nominal
 */
void Test_EA_PerfmonCustom_Nominal(void)
{
	/*
	** Create child process to execute test app
	*/
	pid_t pid = fork();
	/*
	** Child process
	*/
	if (pid == 0)
	{
		char *argv[] = {PERFMON_APP_PATH, PERFMON_TEST_ARG_FIB, NULL};
		if(execvp(PERFMON_APP_PATH, argv) == -1)
		{
			printf("Unable to execute new process\n");
		}
		exit(0);
	}
	/*
	** Failed Fork
	*/
	else if (pid == -1)
	{
		printf("Unable to execute fork\n");
	}
	/*
	** Parent process
	*/
	else
	{
		int high_util_flag = 0;
		int low_util_flag = 0;
		int util = 0;
		int count = 0;
		while(count < PERFMON_SAMPLES)
		{
			/* Get current util */
			EA_PerfmonCustom(pid);
			util = EA_AppData.HkTlm.ActiveAppUtil;

			/* Test if util in expected ranges */
			if(util >= 0 && util <= 100)
			{
				low_util_flag = 1;
			}
			if(util > 30 && util <= 100)
			{
				high_util_flag = 1;
			}
			count += 1;
			sleep(1);
		}

		/* Verify results */
		UtAssert_True(low_util_flag==1, "Hit low util threshold");
		UtAssert_True(high_util_flag==1, "Hit high util threshold");
	}
}

/**
 * Test EA_PerfmonCustom(), Idle
 */
void Test_EA_PerfmonCustom_Idle(void)
{
	/*
	** Create child process to execute test app
	*/
	pid_t pid = fork();
	/*
	** Child process
	*/
	if (pid == 0)
	{
		char *argv[] = {PERFMON_APP_PATH, PERFMON_TEST_ARG_SLP, NULL};
		if(execvp(PERFMON_APP_PATH, argv) == -1)
		{
			printf("Unable to execute new process\n");
		}
		exit(0);
	}
	/*
	** Failed Fork
	*/
	else if (pid == -1)
	{
		printf("Unable to execute fork\n");
	}
	/*
	** Parent process
	*/
	else
	{
		int high_util_flag = 0;
		int low_util_flag = 0;
		int util = 0;
		int count = 0;
		while(count < PERFMON_SAMPLES)
		{
			/* Get current util */
			EA_PerfmonCustom(pid);
			util = EA_AppData.HkTlm.ActiveAppUtil;

			/* Test if util in expected ranges */
			if(util >= 0 && util <= 100)
			{
				low_util_flag = 1;
			}
			if(util > 50 && util <= 100)
			{
				high_util_flag = 1;
			}
			count += 1;
			sleep(1);
		}

		/* Verify results */
		UtAssert_True(low_util_flag==1, "Hit low util threshold");
		UtAssert_True(high_util_flag==0, "Hit high util threshold");
	}
}



/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void EA_Perfmon_Test_AddTestCases(void)
{
	UtTest_Add(Test_EA_PerfmonCustom_Nominal, EA_Test_Setup, EA_Test_TearDown, "Test_EA_PerfmonCustom_Nominal");
	UtTest_Add(Test_EA_PerfmonCustom_Idle, EA_Test_Setup, EA_Test_TearDown, "Test_EA_PerfmonCustom_Idle");
}

