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

#include "ea_custom.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ea_platform_cfg.h"
#include "ea_app.h"
#include "ea_msg.h"

void EA_StartAppCustom()
{
	int32 Status = -1;
	Status = CFE_ES_RegisterChildTask();

	if (Status == CFE_SUCCESS)
	{
		/*
		** Create child process to execute app
		*/
		pid_t pid = fork();
		
		/*
		** Child process
		*/
		if (pid == 0)
		{
			char *argv[] = {EA_AppData.ChildData.Cmd, EA_AppData.ChildData.Args, NULL};
			if(execvp(EA_AppData.ChildData.Cmd, argv) == -1)
			{
				CFE_EVS_SendEvent(EA_CMD_ERR_EID, CFE_EVS_ERROR,
									"Error starting external application");
			}
            /*
		    ** NOTE: This exit is required here as a means to shut down the child process.
		    */  
			exit(0); 
		}
		/*
		** Failed Fork
		*/
		else if (pid == -1)
		{
			EA_AppData.HkTlm.usCmdErrCnt++;
			CFE_EVS_SendEvent(EA_CMD_ERR_EID, CFE_EVS_ERROR,
								"Error starting new process");
		}
		/*
		** Parent process
		*/
		else
		{
			EA_AppData.HkTlm.usCmdCnt++;
			CFE_EVS_SendEvent(EA_INF_APP_START_EID, CFE_EVS_INFORMATION,
								"External application started");
			strncpy(EA_AppData.HkTlm.ActiveApp, EA_AppData.ChildData.Args, OS_MAX_PATH_LEN);
			EA_AppData.HkTlm.ActiveAppPID = pid;
			waitpid(pid, (int*)&EA_AppData.HkTlm.LastAppStatus, 0);
			EA_AppData.HkTlm.LastAppStatus = EA_AppData.HkTlm.LastAppStatus;
			EA_AppData.HkTlm.ActiveAppPID = 0;
			EA_AppData.HkTlm.ActiveAppUtil = 0;
			strncpy(EA_AppData.HkTlm.LastAppRun, EA_AppData.HkTlm.ActiveApp, OS_MAX_PATH_LEN);
			memset(EA_AppData.HkTlm.ActiveApp, '\0', OS_MAX_PATH_LEN);
			memset(EA_AppData.ChildData.Cmd, '\0', OS_MAX_PATH_LEN);
			memset(EA_AppData.ChildData.Args, '\0', OS_MAX_PATH_LEN);

		}
	}/*end if register child task*/
    else
    {
        (void) CFE_ES_WriteToSysLog("EA - StartApp Child Task Registration failed!\n");
    }

	EA_AppData.ChildAppTaskID = 0;
	EA_AppData.ChildAppTaskInUse = FALSE;
	CFE_ES_ExitChildTask();

	return;
}

void EA_TermAppCustom()
{
	int kill_status = -1;

	/*
	** Ensure app is currently running
	*/
	if(EA_AppData.HkTlm.ActiveAppPID != 0)
	{
		CFE_ES_DeleteChildTask(EA_AppData.ChildAppTaskID);
		kill_status = kill(EA_AppData.HkTlm.ActiveAppPID, SIGKILL);

		/*
		** Check kill call return code
		*/
		if(kill_status == 0)
		{
			EA_AppData.HkTlm.usCmdCnt++;
			EA_AppData.HkTlm.ActiveAppPID = 0;
			EA_AppData.HkTlm.ActiveAppUtil = 0;
			strncpy(EA_AppData.HkTlm.LastAppRun, EA_AppData.HkTlm.ActiveApp, OS_MAX_PATH_LEN);
			EA_AppData.HkTlm.LastAppStatus = -1; // TODO: Add meaningful number to this
			memset(EA_AppData.HkTlm.ActiveApp, '\0', OS_MAX_PATH_LEN);
			EA_AppData.ChildAppTaskInUse = FALSE;
			CFE_EVS_SendEvent(EA_INF_APP_TERM_EID, CFE_EVS_INFORMATION,
						"External application terminated");
		}
		else
		{
			EA_AppData.HkTlm.usCmdErrCnt++;
			CFE_EVS_SendEvent(EA_CMD_ERR_EID, CFE_EVS_ERROR,
							"Unable to terminate application");
		}
	}
	else
	{
		EA_AppData.HkTlm.usCmdErrCnt++;
		CFE_EVS_SendEvent(EA_CMD_ERR_EID, CFE_EVS_ERROR,
						"Attempted to terminate app while none executing");
	}

	return;
}

void EA_PerfmonCustom(int32 pid)
{
	EA_ProcData_t proc_data = EA_ParsePidUtil(pid);
	int dp = proc_data.p_time - EA_AppData.ProcData.p_time;
	int dt = proc_data.total_time - EA_AppData.ProcData.total_time;
	float util = 100 * EA_APP_NUM_THREADS * (float)dp/ (float)dt;
	EA_AppData.ProcData.p_time = proc_data.p_time;
	EA_AppData.ProcData.total_time = proc_data.total_time;
	EA_AppData.HkTlm.ActiveAppUtil = util;

	return;
}

EA_ProcData_t EA_ParsePidUtil(int32 pid)
{
	EA_ProcData_t procData;
	unsigned long long int utime = 0;
	unsigned long long int ntime = 0;
	unsigned long long int stime = 0;
	unsigned long long int itime = 0;
	unsigned long long int io_wait = 0;
	unsigned long long int irq = 0;
	unsigned long long int soft_irq = 0;
	unsigned long long int steal = 0;
	unsigned long long int guest = 0;
	unsigned long long int guest_nice = 0;
	unsigned long long int sys_time, idle_time, total_time, virt_time;

	FILE* proc_stat = fopen("/proc/stat", "r");
	if (proc_stat == NULL) {
		OS_printf("Unable to open stat");
	}

	char buf[1024];
	char* data = fgets(buf, sizeof(buf) - 1, proc_stat);
	if (data == NULL)
	{
		OS_printf("Unable to read stat");
	}
	
	fclose(proc_stat);
	
	sscanf(buf, "cpu  %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu",
			&utime, &ntime, &stime, &itime, &io_wait, &irq, &soft_irq, &steal, &guest, &guest_nice);
	utime = utime - guest;
	ntime = ntime - guest_nice;
	idle_time = itime + io_wait;
	sys_time = stime + irq + soft_irq;
	virt_time = guest + guest_nice;
	total_time = utime + ntime + sys_time + idle_time + steal + virt_time;
	procData.total_time = total_time;

	char path[64];
	snprintf(path, sizeof(path), "/proc/%li/stat", pid);
	FILE* pid_stat = fopen(path, "r");
	if (pid_stat == NULL) {
		OS_printf("Unable to open pid_stat");
	}

	memset(buf, '\0', 1024);
	data = fgets(buf, sizeof(buf) - 1, pid_stat);
	if (data == NULL)
	{
		OS_printf("Unable to read pid_stat");
	}

	int utime_ln = 13;
	int stime_ln = 14;
	int count_ndx = 0;
	char *tok;
	for (tok = strtok(data," "); tok != NULL; tok = strtok(NULL, " "))
	{
		if (count_ndx == utime_ln)
		{
			utime = atol(tok);
		}
		if (count_ndx == stime_ln)
		{
			stime = atol(tok);
		}
		count_ndx++;
	}
	
	fclose(pid_stat);

	procData.p_time = utime + stime;

	return(procData);
}
