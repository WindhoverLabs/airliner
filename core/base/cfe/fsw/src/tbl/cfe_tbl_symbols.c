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

#include "cfe_tbl_msg.h"

/* Telemetry messages. */
CFE_TBL_HkPacket_t     CFE_TBL_HkPacket_S;
CFE_TBL_TblRegPacket_t CFE_TBL_TblRegPacket_S;

/* Command messages. */
CFE_TBL_NoArgsCmd_t    CFE_TBL_NoArgsCmd_S;
CFE_TBL_LoadCmd_t      CFE_TBL_LoadCmd_S;
CFE_TBL_DumpCmd_t      CFE_TBL_DumpCmd_S;
CFE_TBL_ValidateCmd_t  CFE_TBL_ValidateCmd_S;
CFE_TBL_ActivateCmd_t  CFE_TBL_ActivateCmd_S;
CFE_TBL_DumpRegCmd_t   CFE_TBL_DumpRegCmd_S;
CFE_TBL_TlmRegCmd_t    CFE_TBL_TlmRegCmd_S;
CFE_TBL_DelCDSCmd_t    CFE_TBL_DelCDSCmd_S;
CFE_TBL_AbortLdCmd_t   CFE_TBL_AbortLdCmd_S;
CFE_TBL_NotifyCmd_t    CFE_TBL_NotifyCmd_S;

