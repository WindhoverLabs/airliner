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
#include <string.h>
#include "amc_app.h"

#include "ut_amc_custom_stubs.h"
#include "ut_amc_custom_hooks.h"

Ut_AMC_Custom_HookTable_t           Ut_AMC_Custom_HookTable;
Ut_AMC_Custom_ReturnCodeTable_t     Ut_AMC_Custom_ReturnCodeTable[UT_AMC_CUSTOM_MAX_INDEX];

void Ut_AMC_Custom_Reset(void)
{
    memset(&Ut_AMC_Custom_HookTable, 0, sizeof(Ut_AMC_Custom_HookTable));
    memset(&Ut_AMC_Custom_ReturnCodeTable, 0, sizeof(Ut_AMC_Custom_ReturnCodeTable));

    Ut_AMC_Custom_SetFunctionHook(UT_AMC_CUSTOM_INITDEVICE_INDEX, (void*)&UT_InitDevice);
    Ut_AMC_Custom_SetFunctionHook(UT_AMC_CUSTOM_SETMOTOROUTPUTS_INDEX, (void*)&UT_SetMotorOutputs);
    Ut_AMC_Custom_SetFunctionHook(UT_AMC_PX4LIB_GETPX4TIMEUS_INDEX, (void*)&UT_PX4LIB_GetPX4TimeUs);
}

void Ut_AMC_Custom_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if (Index == UT_AMC_CUSTOM_INITDEVICE_INDEX)
    {
        Ut_AMC_Custom_HookTable.InitDevice = (int32 (*)(void))FunPtr;
    }
    else if (Index == UT_AMC_CUSTOM_SETMOTOROUTPUTS_INDEX)
    {
        Ut_AMC_Custom_HookTable.SetMotorOutputs = (void(*)(const uint16 *))FunPtr;
    }
    else if (Index == UT_AMC_PX4LIB_GETPX4TIMEUS_INDEX)
    {
        Ut_AMC_Custom_HookTable.PX4LIB_GetPX4TimeUs = (uint64  (*)(void))FunPtr;
    }
    else
    {
        printf("Unsupported AMC_CUSTOM Index In SetFunctionHook Call %lu\n", Index);
        UtAssert_True(FALSE, "Unsupported AMC_CUSTOM Index In SetFunctionHook Call");
    }
}

void Ut_AMC_Custom_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < UT_AMC_CUSTOM_MAX_INDEX)
    {
        Ut_AMC_Custom_ReturnCodeTable[Index].Value = RtnVal;
        Ut_AMC_Custom_ReturnCodeTable[Index].Count = CallCnt;
    }
    else
    {
        printf("Unsupported AMC_CUSTOM Index In SetReturnCode Call %lu\n", Index);
        UtAssert_True(FALSE, "Unsupported AMC_CUSTOM Index In SetReturnCode Call");
    }
}

boolean Ut_AMC_Custom_UseReturnCode(uint32 Index)
{
    if (Ut_AMC_Custom_ReturnCodeTable[Index].Count > 0)
    {
        Ut_AMC_Custom_ReturnCodeTable[Index].Count--;
        if (Ut_AMC_Custom_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }
    else if (Ut_AMC_Custom_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero == TRUE)
    {
        return(TRUE);
    }

    return(FALSE);
}

void Ut_AMC_Custom_ContinueReturnCodeAfterCountZero(uint32 Index)
{
    Ut_AMC_Custom_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero = TRUE;
}

int32 AMC::InitDevice(void)
{
    /* Check for specified return */
    if (Ut_AMC_Custom_UseReturnCode(UT_AMC_CUSTOM_INITDEVICE_INDEX))
        return Ut_AMC_Custom_ReturnCodeTable[UT_AMC_CUSTOM_INITDEVICE_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_AMC_Custom_HookTable.InitDevice)
        return Ut_AMC_Custom_HookTable.InitDevice();

    return CFE_SUCCESS;
}

void AMC::SetMotorOutputs(const uint16 *PWM)
{
    /* Can't specify return value - this is a void function */

    /* Check for Function Hook */
    if (Ut_AMC_Custom_HookTable.SetMotorOutputs)
        Ut_AMC_Custom_HookTable.SetMotorOutputs(PWM);

    return;
}

extern "C" uint64 PX4LIB_GetPX4TimeUs(void)
{
    uint64           outTime = 0;
    OS_time_t        localTime = {};

    /* Check for specified return */
    if (Ut_AMC_Custom_UseReturnCode(UT_AMC_PX4LIB_GETPX4TIMEUS_INDEX))
    {
        return Ut_AMC_Custom_ReturnCodeTable[UT_AMC_PX4LIB_GETPX4TIMEUS_INDEX].Value;
    }

    /* Check for Function Hook */
    if (Ut_AMC_Custom_HookTable.PX4LIB_GetPX4TimeUs)
    {
        return Ut_AMC_Custom_HookTable.PX4LIB_GetPX4TimeUs();
    }

    CFE_PSP_GetTime(&localTime);

    outTime = static_cast<uint64>(static_cast<uint64>(localTime.seconds)
              * static_cast<uint64>(1000000))
              + static_cast<uint64>(localTime.microsecs);

    return outTime;
}
