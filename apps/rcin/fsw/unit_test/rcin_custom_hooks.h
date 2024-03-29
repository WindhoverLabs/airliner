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


#include "rcin_custom_stubs.h"

#include <time.h>


#ifdef __cplusplus
extern "C" {
#endif


extern uint32  SEDLIB_ReadMsg_Cnt;


#define UT_TEST_BUF_SIZE_12BYTES        12
#define UT_TEST_BUF_SIZE_1FRAME         25
#define UT_TEST_BUF_SIZE_2FRAME         UT_TEST_BUF_SIZE_1FRAME * 2
#define UT_TEST_BUF_SIZE_10FRAME        UT_TEST_BUF_SIZE_1FRAME * 10
#define UT_TEST_BUF_SIZE_30FRAME        UT_TEST_BUF_SIZE_1FRAME * 30


int32  RCIN_Util_Stream_Emulator(void);

SEDLIB_ReturnCode_t SEDLIB_GetPipeHook(char *PipeName, uint32 Size,
                                       uint32 *PipeHandle);
SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_1Frame_Nominal(uint32 PipeHandle,
                                                   CFE_SB_MsgPtr_t Msg);
SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_2Frame_Nominal(uint32 PipeHandle,
                                                   CFE_SB_MsgPtr_t Msg);
SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_2Frame_RcLost(uint32 PipeHandle,
                                                  CFE_SB_MsgPtr_t Msg);
SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_2Frame_1NoFooter(uint32 PipeHandle,
                                                     CFE_SB_MsgPtr_t Msg);
SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_2Frame_1NoHdr(uint32 PipeHandle,
                                                  CFE_SB_MsgPtr_t Msg);
SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_10Frame_1NoHdr1NoFooter(
                               uint32 PipeHandle, CFE_SB_MsgPtr_t Msg);

SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_Multiple_1NoHdr1NoFooter(
                            uint32 PipeHandle, CFE_SB_MsgPtr_t Msg);
SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_Multiple_25NoHdr(
                            uint32 PipeHandle, CFE_SB_MsgPtr_t Msg);
SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_Multiple_25Bytes(
                            uint32 PipeHandle, CFE_SB_MsgPtr_t Msg);
SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_Multiple_12Bytes(
                            uint32 PipeHandle, CFE_SB_MsgPtr_t Msg);
SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_0Byte(uint32 PipeHandle,
                                             CFE_SB_MsgPtr_t Msg);
SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_NoData(uint32 PipeHandle,
                                              CFE_SB_MsgPtr_t Msg);
SEDLIB_ReturnCode_t SEDLIB_ReadMsgHook_ReadErr(uint32 PipeHandle,
                                               CFE_SB_MsgPtr_t Msg);

uint32  GetChecksum_1Frame_Nominal(void);
uint32  GetChecksum_2Frame_Nominal(void);
uint32  GetChecksum_2Frame_RcLost(void);

CFE_TIME_SysTime_t  CFE_TIME_GetTimeHook(void);
void                CFE_SB_TimeStampMsgHook(CFE_SB_MsgPtr_t MsgPtr);

void                CFE_PSP_GetTimeHook(OS_time_t *LocalTime);

int32  OS_TaskDelayHook(uint32 millisecond);


#ifdef __cplusplus
}
#endif


#endif /* RCIN_CUSTOM_HOOKS_H */
