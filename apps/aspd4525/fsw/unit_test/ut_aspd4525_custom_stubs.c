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

#include "cfe.h"
#include "utassert.h"
#include "ut_aspd4525_custom_stubs.h"

#include <string.h>

Ut_ASPD4525_Custom_HookTable_t        Ut_ASPD4525_Custom_HookTable;
Ut_ASPD4525_Custom_ReturnCodeTable_t  Ut_ASPD4525_Custom_ReturnCodeTable[UT_ASPD4525_CUSTOM_MAX_INDEX];


void Ut_ASPD4525_Custom_Reset(void)
{
    memset(&Ut_ASPD4525_Custom_HookTable, 0x00, sizeof(Ut_ASPD4525_Custom_HookTable));
    memset(&Ut_ASPD4525_Custom_ReturnCodeTable, 0x00, sizeof(Ut_ASPD4525_Custom_ReturnCodeTable));
}


void Ut_ASPD4525_Custom_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if      (Index == UT_ASPD4525_CUSTOM_INITDATA_INDEX)  { Ut_ASPD4525_Custom_HookTable.ASPD4525_Custom_InitData = (void (*)(void))FunPtr; }
    else if (Index == UT_ASPD4525_CUSTOM_INIT_INDEX)      { Ut_ASPD4525_Custom_HookTable.ASPD4525_Custom_Init = (boolean (*)(void))FunPtr; }
    else if (Index == UT_ASPD4525_CUSTOM_MEASURE_INDEX)   { Ut_ASPD4525_Custom_HookTable.ASPD4525_Custom_Measure = (boolean (*)(uint16*, uint16*, uint8*))FunPtr; }
    else
    {
        printf("Unsupported ASPD4525_CUSTOM Index In SetFunctionHook Call %lu\n", Index);
        UtAssert_True(FALSE, "Unsupported ASPD4525_CUSTOM Index In SetFunctionHook Call");
    }
}


void Ut_ASPD4525_Custom_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < UT_ASPD4525_CUSTOM_MAX_INDEX)
    {
        Ut_ASPD4525_Custom_ReturnCodeTable[Index].Value = RtnVal;
        Ut_ASPD4525_Custom_ReturnCodeTable[Index].Count = CallCnt;
    }
    else
    {
        printf("Unsupported ASPD4525_CUSTOM Index In SetReturnCode Call %lu\n", Index);
        UtAssert_True(FALSE, "Unsupported ASPD4525_CUSTOM Index In SetReturnCode Call");
    }
}


boolean Ut_ASPD4525_Custom_UseReturnCode(uint32 Index)
{
    if (Ut_ASPD4525_Custom_ReturnCodeTable[Index].Count > 0)
    {
        Ut_ASPD4525_Custom_ReturnCodeTable[Index].Count --;
        if (Ut_ASPD4525_Custom_ReturnCodeTable[Index].Count == 0)
        {
            return(TRUE);
        }
    }
    else if (Ut_ASPD4525_Custom_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero == TRUE)
    {
        return(TRUE);
    }

    return(FALSE);
}


void Ut_ASPD4525_Custom_ContinueReturnCodeAfterCountZero(uint32 Index)
{
    Ut_ASPD4525_Custom_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero = TRUE;
}


void ASPD4525_Custom_InitData(void)
{
    /* Can't specify return value - this is a void function */

    /* Check for Function Hook */
    if (Ut_ASPD4525_Custom_HookTable.ASPD4525_Custom_InitData)
    {
        return Ut_ASPD4525_Custom_HookTable.ASPD4525_Custom_InitData();
    }

    return;
}


boolean ASPD4525_Custom_Init(void)
{
    /* Check for specified return */
    if (Ut_ASPD4525_Custom_UseReturnCode(UT_ASPD4525_CUSTOM_INIT_INDEX))
    {
        return Ut_ASPD4525_Custom_ReturnCodeTable[UT_ASPD4525_CUSTOM_INIT_INDEX].Value;
    }

    /* Check for Function Hook */
    if (Ut_ASPD4525_Custom_HookTable.ASPD4525_Custom_Init)
    {
        return Ut_ASPD4525_Custom_HookTable.ASPD4525_Custom_Init();
    }

    return TRUE;
}


boolean ASPD4525_Custom_Measure(uint16 *pPressureDiffCount,
                                uint16 *pTemperatureCount, uint8 *pStatus)
{
    /* Check for specified return */
    if (Ut_ASPD4525_Custom_UseReturnCode(UT_ASPD4525_CUSTOM_MEASURE_INDEX))
    {
        return Ut_ASPD4525_Custom_ReturnCodeTable[UT_ASPD4525_CUSTOM_MEASURE_INDEX].Value;
    }

    /* Check for Function Hook */
    if (Ut_ASPD4525_Custom_HookTable.ASPD4525_Custom_Measure)
    {
        return Ut_ASPD4525_Custom_HookTable.ASPD4525_Custom_Measure(pPressureDiffCount,
                                                            pTemperatureCount, pStatus);
    }

    return TRUE;
}
