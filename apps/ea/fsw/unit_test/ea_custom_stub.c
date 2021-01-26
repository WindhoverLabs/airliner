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

#include "ea_custom_stub.h"
#include "ea_platform_cfg.h"
#include "ea_app.h"
#include "ea_msg.h"

void EA_StartAppCustom()
{
	EA_AppData.HkTlm.usCmdCnt++;
	return;
}

void EA_TermAppCustom(CFE_SB_Msg_t* MsgPtr)
{
	if(EA_AppData.HkTlm.ActiveAppPID != 0)
	{
		EA_AppData.HkTlm.usCmdCnt++;
		EA_AppData.HkTlm.ActiveAppPID = 0;
		CFE_EVS_SendEvent(EA_INF_APP_TERM_EID, CFE_EVS_INFORMATION,
						"External application terminated");
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
	if(EA_AppData.HkTlm.ActiveAppPID != 0)
	{
		if(EA_AppData.HkTlm.ActiveAppUtil > EA_APP_UTIL_THRESHOLD)
		{
			CFE_EVS_SendEvent(EA_WARN_APP_UTIL_EID, CFE_EVS_INFORMATION,
								"External application exceeded utilization threshold");
		}
		EA_AppData.HkTlm.ActiveAppUtil = 1;
	}
	return;
}

EA_ProcData_t EA_ParsePidUtil(int32 pid)
{
	EA_ProcData_t procData;
	procData.p_time = 1;
	procData.p_time = 1;
	return(procData);
}










