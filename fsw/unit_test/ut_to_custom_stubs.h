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

#ifndef TO_CUSTOM_STUBS_H
#define TO_CUSTOM_STUBS_H

#include "to_channel.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    UT_TO_CUSTOM_INIT_INDEX,
    UT_TO_CUSTOM_INITEVENT_INDEX,
    UT_TO_OUTPUTCHANNEL_CUSTOMBUILDUP_INDEX,
    UT_TO_OUTPUTCHANNEL_CUSTOMTEARDOWN_INDEX,
    UT_TO_OUTPUTCHANNEL_CUSTOMCLEANUPALL_INDEX,
    UT_TO_OUTPUTCHANNEL_PROCESSNEWCUSTOMCMDS_INDEX,
    UT_TO_OUTPUTCHANNEL_STATUS_INDEX,
    UT_TO_GETCUSTOMVERSION_INDEX,
    UT_TO_PRINTCUSTOMVERSION_INDEX,
    UT_TO_RESETCUSTOMCHANNELCOUNTERS_INDEX,
    UT_TO_UPDATECUSTOMDATAFROMTABLE_INDEX,
    UT_TO_CUSTOM_MAX_INDEX
} Ut_TO_Custom_INDEX_t;


typedef struct
{
    int32  (*TO_Custom_Init)(void);
    int32  (*TO_Custom_InitEvent)(int32 ind);
    int32  (*TO_OutputChannel_CustomBuildup)(uint32 index);
    int32  (*TO_OutputChannel_CustomTeardown)(uint32 index);
    void   (*TO_OutputChannel_CustomCleanupAll)(void);
    void   (*TO_OutputChannel_ProcessNewCustomCmds)(CFE_SB_Msg_t* MsgPtr);
    uint8  (*TO_OutputChannel_Status)(uint32 index);
    uint32 (*TO_GetCustomVersion)(void);
    void   (*TO_PrintCustomVersion)(void);
    void   (*TO_ResetCustomChannelCounters)(void);
    void   (*TO_UpdateCustomDataFromTable)(uint8 ChannelID,
                                      osalbool sendEvent,
                                      osalbool abandonCurrentMsg);
} Ut_TO_Custom_HookTable_t;

typedef struct
{
    int32   Value;
    uint32  Count;
    boolean ContinueReturnCodeAfterCountZero;
} Ut_TO_Custom_ReturnCodeTable_t;

void Ut_TO_Custom_Reset(void);
void Ut_TO_Custom_SetFunctionHook(uint32 Index, void *FunPtr);
void Ut_TO_Custom_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);
void Ut_TO_Custom_ContinueReturnCodeAfterCountZero(uint32 Index);


#ifdef __cplusplus
}
#endif

#endif /* TO_CUSTOM_STUBS_H */
