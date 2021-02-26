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
#include "ut_bat_custom_stubs.h"
#include "ut_bat_custom_hooks.h"
#include <string.h>
#include "bat_app.h"

Ut_BAT_Custom_HookTable_t           Ut_BAT_Custom_HookTable;
Ut_BAT_Custom_ReturnCodeTable_t     Ut_BAT_Custom_ReturnCodeTable[UT_BAT_CUSTOM_MAX_INDEX];

void Ut_BAT_Custom_Reset(void)
{
    memset(&Ut_BAT_Custom_HookTable, 0, sizeof(Ut_BAT_Custom_HookTable));
    memset(&Ut_BAT_Custom_ReturnCodeTable, 0, sizeof(Ut_BAT_Custom_ReturnCodeTable));
}

void Ut_BAT_Custom_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if      (Index == UT_BAT_CUSTOM_INITDEVICE_INDEX)     { Ut_BAT_Custom_HookTable.InitDevice  = (int32 (*)(void))FunPtr; }
    else if (Index == UT_BAT_CUSTOM_CLOSEDEVICE_INDEX)    { Ut_BAT_Custom_HookTable.CloseDevice = (void  (*)(void))FunPtr; }
    else if (Index == UT_BAT_CUSTOM_READDEVICE_INDEX)     { Ut_BAT_Custom_HookTable.ReadDevice  = (int32 (*)(float &, float &))FunPtr; }
    else
    {
        printf("Unsupported BAT_CUSTOM Index In SetFunctionHook Call %lu\n", Index);
        UtAssert_True(FALSE, "Unsupported BAT_CUSTOM Index In SetFunctionHook Call");
    }
}

void Ut_BAT_Custom_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < UT_BAT_CUSTOM_MAX_INDEX)
    {
        Ut_BAT_Custom_ReturnCodeTable[Index].Value = RtnVal;
        Ut_BAT_Custom_ReturnCodeTable[Index].Count = CallCnt;
    }
    else
    {
        printf("Unsupported BAT_CUSTOM Index In SetReturnCode Call %lu\n", Index);
        UtAssert_True(FALSE, "Unsupported BAT_CUSTOM Index In SetReturnCode Call");
    }
}

osalbool Ut_BAT_Custom_UseReturnCode(uint32 Index)
{
    if (Ut_BAT_Custom_ReturnCodeTable[Index].Count > 0)
    {
        Ut_BAT_Custom_ReturnCodeTable[Index].Count--;
        if (Ut_BAT_Custom_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }
    else if (Ut_BAT_Custom_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero == TRUE)
    {
        return(TRUE);
    }

    return(FALSE);
}

void Ut_BAT_Custom_ContinueReturnCodeAfterCountZero(uint32 Index)
{
    Ut_BAT_Custom_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero = TRUE;
}

int32 BAT::InitDevice(void)
{
    /* Check for specified return */
    if (Ut_BAT_Custom_UseReturnCode(UT_BAT_CUSTOM_INITDEVICE_INDEX))
        return Ut_BAT_Custom_ReturnCodeTable[UT_BAT_CUSTOM_INITDEVICE_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_BAT_Custom_HookTable.InitDevice)
        return Ut_BAT_Custom_HookTable.InitDevice();

    return 0;
}


void BAT::CloseDevice(void)
{
    /* Check for specified return */
    if (Ut_BAT_Custom_UseReturnCode(UT_BAT_CUSTOM_CLOSEDEVICE_INDEX))
        Ut_BAT_Custom_ReturnCodeTable[UT_BAT_CUSTOM_CLOSEDEVICE_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_BAT_Custom_HookTable.CloseDevice)
        Ut_BAT_Custom_HookTable.InitDevice();
}

int32 BAT::ReadDevice(float &Voltage, float &Current)
{
    /* Check for specified return */
    if (Ut_BAT_Custom_UseReturnCode(UT_BAT_CUSTOM_READDEVICE_INDEX))
    	return Ut_BAT_Custom_ReturnCodeTable[UT_BAT_CUSTOM_READDEVICE_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_BAT_Custom_HookTable.ReadDevice)
    	return Ut_BAT_Custom_HookTable.ReadDevice(Voltage, Current);

    return 0;
}


