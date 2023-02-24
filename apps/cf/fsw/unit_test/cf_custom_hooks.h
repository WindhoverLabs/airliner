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

#ifndef CF_CUSTOM_HOOKS_H
#define CF_CUSTOM_HOOKS_H

#include "cfe.h"

extern uint32       ReadHookCalledCnt;
extern boolean      ReadHook_IncomingFlag;
extern uint32       ReaddirHookReturnCnt;
extern os_dirent_t  ReaddirHookDirEntry;

extern uint32       SemGetInfoHookCallCnt;
extern uint32       SemGetIdByNameHookCallCnt;

extern uint32       ZeroCopyGetPtrHookCallCnt;
extern uint32       ZeroCopyGetPtrHookOffset;

extern uint32       CFE_ES_GetPoolBufHookCallCnt;


#ifdef __cplusplus
extern "C" {
#endif


int32   CFE_ES_GetPoolBufHook(uint32 **BufPtr, CFE_ES_MemHandle_t HandlePtr,
                              uint32 Size);
int32   CFE_ES_PutPoolBufHook(CFE_ES_MemHandle_t HandlePtr, uint32 *BufPtr);
int32   CFE_SB_ZeroCopyGetPtrHook(uint16 MsgSize,
                                  CFE_SB_ZeroCopyHandle_t *BufferHandle);
int32   CFE_FS_WriteHeaderHook(int32 FileDes, CFE_FS_Header_t *Hdr);

int32        OS_creatHook(const char *path, int32  access);
int32        OS_openHook(const char *path, int32 access, uint32 mode);
int32        OS_writeHook(int32 filedes, const void *buffer, uint32 nbytes);
int32        OS_closeHook(int32 filedes);

int32        OS_statHook(const char *path, os_fstat_t *filestats);
int32        OS_FDGetInfoHook (int32 filedes, OS_FDTableEntry *fd_prop);
int32        OS_readHook(int32  filedes, void *buffer, uint32 nbytes);
os_dirp_t    OS_opendirHook(const char *path);
os_dirent_t  *OS_readdirHook (os_dirp_t directory);

int32        OS_CountSemGetIdByNameHook(
                          uint32 *sem_id, const char *sem_name);
int32        OS_CountSemGetInfoHook(
                          uint32 sem_id, OS_count_sem_prop_t *count_prop);

void                CFE_PSP_GetTimeHook(OS_time_t *LocalTime);
CFE_TIME_SysTime_t  CFE_TIME_GetTimeHook(void);
void                CFE_SB_TimeStampMsgHook(CFE_SB_MsgPtr_t MsgPtr);


#ifdef __cplusplus
}
#endif


#endif /* CF_CUSTOM_HOOKS_H */
