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

#include "cfe_evs_msg.h"

/* Telemetry messages. */
CFE_EVS_TlmPkt_t CFE_EVS_TlmPkt_S;
CFE_EVS_Packet_t CFE_EVS_Packet_S;

/* Command messages. */
CFE_EVS_LogFileCmd_t CFE_EVS_LogFileCmd_S;
CFE_EVS_AppDataCmd_t CFE_EVS_AppDataCmd_S;
CFE_EVS_ModeCmd_t    CFE_EVS_ModeCmd_S;
CFE_EVS_BitMaskCmd_t CFE_EVS_BitMaskCmd_S;
CFE_EVS_AppNameCmd_t CFE_EVS_AppNameCmd_S;
CFE_EVS_AppNameEventIDCmd_t CFE_EVS_AppNameEventIDCmd_S;
CFE_EVS_AppNameBitMaskCmd_t CFE_EVS_AppNameBitMaskCmd_S;
CFE_EVS_AppNameEventIDMaskCmd_t CFE_EVS_AppNameEventIDMaskCmd_S;
