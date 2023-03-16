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

#ifndef RCIN_CUSTOM_STUBS_H
#define RCIN_CUSTOM_STUBS_H

#ifdef __cplusplus
extern "C" {
#endif


#include "rcin_sed.h"


extern RCIN_AppCustomData_t RCIN_AppCustomData;


#define TEST_RCIN_MAX_ERROR_CNT        RCIN_MAX_ERROR_COUNT
#define TEST_RCIN_BUS_CHANNEL_CNT      RCIN_SBUS_CHANNEL_COUNT

#define TEST_RCIN_DEV_ERR_EID          RCIN_EVT_CNT
#define TEST_RCIN_OUT_SYNC_ERR_EID     (RCIN_EVT_CNT + 1)
#define TEST_RCIN_IN_SYNC_INF_EID      (RCIN_EVT_CNT + 3)


typedef enum
{
    UT_RCIN_CUSTOM_SEDLIB_GETPIPE_INDEX,
    UT_RCIN_CUSTOM_SEDLIB_READMSG_INDEX,
    UT_RCIN_CUSTOM_MAX_INDEX
} Ut_RCIN_Custom_INDEX_t;


typedef struct
{
    SEDLIB_ReturnCode_t (*SEDLIB_GetPipe)(char *PipeName, uint32 Size, uint32 *PipeHandle);
    SEDLIB_ReturnCode_t (*SEDLIB_ReadMsg)(uint32 PipeHandle, CFE_SB_MsgPtr_t Msg);
} Ut_RCIN_Custom_HookTable_t;


typedef struct
{
    int32   Value;
    uint32  Count;
    boolean ContinueReturnCodeAfterCountZero;
} Ut_RCIN_Custom_ReturnCodeTable_t;


void Ut_RCIN_Custom_Reset(void);
void Ut_RCIN_Custom_SetFunctionHook(uint32 Index, void *FunPtr);
void Ut_RCIN_Custom_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);
void Ut_RCIN_Custom_ContinueReturnCodeAfterCountZero(uint32 Index);


#ifdef __cplusplus
}
#endif

#endif /* RCIN_CUSTOM_STUBS_H */
