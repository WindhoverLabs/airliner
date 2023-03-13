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

#ifndef RCIN_CUSTOM_HOOKS_H
#define RCIN_CUSTOM_HOOKS_H

#ifdef __cplusplus
extern "C" {
#endif


#include "rcin_custom_stubs.h"

#include <time.h>


extern uint32  SEDLIB_ReadMsg_Cnt;

SEDLIB_ReturnCode_t SEDLIB_GetPipeHook(char *PipeName, uint32 Size,
                                       uint32 *PipeHandle);
SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_1Msg_Normal(uint32 PipeHandle,
                                                  CFE_SB_MsgPtr_t Msg);
SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_2Msg_Normal(uint32 PipeHandle,
                                                  CFE_SB_MsgPtr_t Msg);
SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_2Msg_RcLost(uint32 PipeHandle,
                                                  CFE_SB_MsgPtr_t Msg);
SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_2Msg_1NoFooter(uint32 PipeHandle,
                                                  CFE_SB_MsgPtr_t Msg);
SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_2Msg_1NoHdr(uint32 PipeHandle,
                                                 CFE_SB_MsgPtr_t Msg);
SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_10Msg_1NoHdr1NoFooter(
                            uint32 PipeHandle, CFE_SB_MsgPtr_t Msg);

CFE_TIME_SysTime_t  CFE_TIME_GetTimeHook(void);
void                CFE_SB_TimeStampMsgHook(CFE_SB_MsgPtr_t MsgPtr);

void                CFE_PSP_GetTimeHook(OS_time_t *LocalTime);


#ifdef __cplusplus
}
#endif


#endif /* RCIN_CUSTOM_HOOKS_H */
