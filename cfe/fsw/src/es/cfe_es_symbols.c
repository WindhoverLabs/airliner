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

#include "cfe_es_msg.h"

/* Command messages. */
CFE_ES_NoArgsCmd_t          CFE_ES_NoArgsCmd_S;
CFE_ES_RestartCmd_t         CFE_ES_RestartCmd_S;
CFE_ES_ShellCmd_t           CFE_ES_ShellCmd_S;
CFE_ES_QueryAllCmd_t        CFE_ES_QueryAllCmd_S;
CFE_ES_QueryAllTasksCmd_t   CFE_ES_QueryAllTasksCmd_S;
CFE_ES_WriteSyslogCmd_t     CFE_ES_WriteSyslogCmd_S;
CFE_ES_WriteERlogCmd_t      CFE_ES_WriteERlogCmd_S;
CFE_ES_OverWriteSysLogCmd_t CFE_ES_OverWriteSysLogCmd_S;
CFE_ES_StartAppCmd_t        CFE_ES_StartAppCmd_S;
CFE_ES_AppNameCmd_t         CFE_ES_AppNameCmd_S;
CFE_ES_AppReloadCmd_t       CFE_ES_AppReloadCmd_S;
CFE_ES_SetMaxPRCountCmd_t   CFE_ES_SetMaxPRCountCmd_S;
CFE_ES_DeleteCDSCmd_t       CFE_ES_DeleteCDSCmd_S;
CFE_ES_PerfStartCmd_t       CFE_ES_PerfStartCmd_S;
CFE_ES_PerfStopCmd_t        CFE_ES_PerfStopCmd_S;
CFE_ES_PerfSetFilterMaskCmd_t CFE_ES_PerfSetFilterMaskCmd_S;
CFE_ES_PerfSetTrigMaskCmd_t CFE_ES_PerfSetTrigMaskCmd_S;
CFE_ES_TlmPoolStatsCmd_t    CFE_ES_TlmPoolStatsCmd_S;
CFE_ES_DumpCDSRegCmd_t      CFE_ES_DumpCDSRegCmd_S;

/* Telemetry messages. */
CFE_ES_OneAppTlm_t    CFE_ES_OneAppTlm_S;
CFE_ES_PoolStatsTlm_t CFE_ES_PoolStatsTlm_S;
CFE_ES_HkPacket_t     CFE_ES_HkPacket_S;
CFE_ES_ShellPacket_t  CFE_ES_ShellPacket_S;
