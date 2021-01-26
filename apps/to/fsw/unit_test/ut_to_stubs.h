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

#ifndef TO_STUBS_H
#define TO_STUBS_H

#include "to_channel.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    UT_TO_CHANNEL_STATE_INDEX,
    UT_TO_VERIFYCMDLENGTH_INDEX,
    UT_TO_CHANNEL_LOCKBYINDEX_INDEX,
    UT_TO_CHANNEL_UNLOCKBYINDEX_INDEX,
    UT_TO_CHANNEL_LOCKBYREF_INDEX,
    UT_TO_CHANNEL_UNLOCKBYREF_INDEX,
    UT_TO_MESSAGEFLOW_GETOBJECT_INDEX,
    UT_TO_CHANNEL_OPENCHANNEL_INDEX,
    UT_TO_MAX_INDEX
} Ut_TO_INDEX_t;


typedef struct
{
    uint8    (*TO_Channel_State)(uint8 index);
    osalbool (*TO_VerifyCmdLength)(CFE_SB_Msg_t* MsgPtr, uint16 usExpectedLen);
    void     (*TO_Channel_LockByIndex)(uint8 index);
    void     (*TO_Channel_UnlockByIndex)(uint8 index);
    void     (*TO_Channel_LockByRef)(TO_ChannelData_t *channel);
    void     (*TO_Channel_UnlockByRef)(TO_ChannelData_t *channel);
    TO_MessageFlow_t* (*TO_MessageFlow_GetObject)(TO_ChannelData_t* channel, CFE_SB_MsgId_t MsgID, uint32 *Index);
    int32 (*TO_Channel_OpenChannel)(const uint32 index, const char *ChannelName,
            const char *ConfigTableName, const char *ConfigTableFileName, TO_ChannelTbl_t *BackupTbl,
            const char *DumpTableName, const uint32 CfCntSemMax, const char *CfCntSemName);
} Ut_TO_HookTable_t;

typedef struct
{
    int32   Value;
    uint32  Count;
    boolean ContinueReturnCodeAfterCountZero;
} Ut_TO_ReturnCodeTable_t;

void Ut_TO_Reset(void);
void Ut_TO_SetFunctionHook(uint32 Index, void *FunPtr);
void Ut_TO_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);
void Ut_TO_ContinueReturnCodeAfterCountZero(uint32 Index);


#ifdef __cplusplus
}
#endif

#endif /* TO_STUBS_H */
