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

#include "sch_custom_stubs.h"

#include "utassert.h"

#include <string.h>


Ut_OSTIMER_HookTable_t          Ut_OSTIMER_HookTable;
Ut_OSTIMER_ReturnCodeTable_t    Ut_OSTIMER_ReturnCodeTable[UT_OSTIMER_MAX_INDEX];

void Ut_OSTIMER_Reset(void)
{
    memset(&Ut_OSTIMER_HookTable, 0, sizeof(Ut_OSTIMER_HookTable));
    memset(&Ut_OSTIMER_ReturnCodeTable, 0, sizeof(Ut_OSTIMER_ReturnCodeTable));
}

void Ut_OSTIMER_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if      (Index == UT_OSTIMER_CREATE_INDEX)  { Ut_OSTIMER_HookTable.OS_TimerCreate =
                   (int32 (*)(uint32 *, const char *, uint32 *, OS_TimerCallback_t))FunPtr;}
    else if (Index == UT_OSTIMER_SET_INDEX)     { Ut_OSTIMER_HookTable.OS_TimerSet =
                   (int32 (*)(uint32, uint32, uint32))FunPtr;}
    else
    {
        printf("Unsupported OSTIMER Index In SetFunctionHook Call %lu", Index);
        UtAssert_True(FALSE, "Unsupported OSTIMER Index In SetFunctionHook Call");
    }
}

void Ut_OSTIMER_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < UT_OSTIMER_MAX_INDEX)
    {
        Ut_OSTIMER_ReturnCodeTable[Index].Value = RtnVal;
        Ut_OSTIMER_ReturnCodeTable[Index].Count = CallCnt;
    }
    else
    {
        printf("Unsupported OSTIMER Index In SetReturnCode Call %lu\n", Index);
        UtAssert_True(FALSE, "Unsupported OSTIMER Index In SetReturnCode Call");
    }
}

boolean Ut_OSTIMER_UseReturnCode(uint32 Index)
{
    if (Ut_OSTIMER_ReturnCodeTable[Index].Count > 0)
    {
        Ut_OSTIMER_ReturnCodeTable[Index].Count--;
        if (Ut_OSTIMER_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }
    else if (Ut_OSTIMER_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero == TRUE)
    {
        return(TRUE);
    }

    return(FALSE);
}

void Ut_OSTIMER_ContinueReturnCodeAfterCountZero(uint32 Index)
{
    Ut_OSTIMER_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero = TRUE;
}

int32 OS_TimerCreate(uint32 *timer_id, const char *timer_name, uint32 *clock_accuracy,
                     OS_TimerCallback_t  callback_ptr)
{
    /* Check for specified return */
    if (Ut_OSTIMER_UseReturnCode(UT_OSTIMER_CREATE_INDEX))
        return Ut_OSTIMER_ReturnCodeTable[UT_OSTIMER_CREATE_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_OSTIMER_HookTable.OS_TimerCreate)
    {
        return(Ut_OSTIMER_HookTable.OS_TimerCreate(timer_id, timer_name,
                                                   clock_accuracy, callback_ptr));
    }

    return CFE_SUCCESS;
}

int32 OS_TimerSet(uint32 timer_id, uint32 start_time, uint32 interval_time)
{
    /* Check for specified return */
    if (Ut_OSTIMER_UseReturnCode(UT_OSTIMER_SET_INDEX))
        return Ut_OSTIMER_ReturnCodeTable[UT_OSTIMER_SET_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_OSTIMER_HookTable.OS_TimerSet)
    {
        return(Ut_OSTIMER_HookTable.OS_TimerSet(timer_id, start_time, interval_time));
    }

    return CFE_SUCCESS;
}
