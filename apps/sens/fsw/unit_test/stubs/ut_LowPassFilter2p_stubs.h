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
#ifndef UT_LOWPASSFILTER2P_STUBS_H
#define UT_LOWPASSFILTER2P_STUBS_H

#include "cfe.h"
#include "utassert.h"
#include "mixer/MultirotorMixer.h"
#include <string.h>
//#include "mixer/MixerTables.h"
//#include "UT_MultirotorMixer.h"

typedef enum
{
    UT_LOWPASSFILTER2P_SETCONFIGTABLEPTR_INDEX,
    UT_LOWPASSFILTER2P_MAX_INDEX
} Ut_LowPassFilter2P_INDEX_t;

typedef struct
{
    uint32 (*mix)(float *, uint32, uint16 *);
} Ut_LowPassFilter2P_HookTable_t;

typedef struct
{
    int32 mix_Return;
} Ut_LowPassFilter2P_Returns_t;

typedef struct
{
    int32   Value;
    uint32  Count;
    boolean ContinueReturnCodeAfterCountZero;
} Ut_LowPassFilter2P_ReturnCodeTable_t;

int32 Ut_LowPassFilter2PInit(void);
void  Ut_LowPassFilter2P_Reset(void);
void  Ut_LowPassFilter2P_SetFunctionHook(uint32 Index, void *FunPtr);
void  Ut_LowPassFilter2P_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);
void  Ut_LowPassFilter2P_ContinueReturnCodeAfterCountZero(uint32 Index);

#endif
