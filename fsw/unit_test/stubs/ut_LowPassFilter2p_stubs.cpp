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
#include "mixer/MultirotorMixer.h"
#include <string.h>
#include "mixer/MixerTables.h"
#include "ut_MultirotorMixer_stubs.h"
//#include "UT_MultirotorMixer.h"

Ut_MultirotorMixer_HookTable_t           Ut_MultirotorMixer_HookTable;
Ut_MultirotorMixer_ReturnCodeTable_t     Ut_MultirotorMixer_ReturnCodeTable[UT_MULTIROTORMIXER_MAX_INDEX];

void Ut_MultirotorMixer_Reset(void)
{
    memset(&Ut_MultirotorMixer_HookTable, 0, sizeof(Ut_MultirotorMixer_HookTable));
    memset(&Ut_MultirotorMixer_ReturnCodeTable, 0, sizeof(Ut_MultirotorMixer_ReturnCodeTable));
};


void Ut_MultirotorMixer_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if      (Index == UT_MULTIROTORMIXER_SETCONFIGTABLEPTR_INDEX)     { Ut_MultirotorMixer_HookTable.SetConfigTablePtr = (int32(*)(MultirotorMixer_ConfigTablePtr_t &))FunPtr; }
    else if (Index == UT_MULTIROTORMIXER_MIX_INDEX)                   { Ut_MultirotorMixer_HookTable.mix = (uint32 (*)(float *, uint32, uint16 *))FunPtr; }
    else if (Index == UT_MULTIROTORMIXER_GET_SATURATION_STATUS_INDEX) { Ut_MultirotorMixer_HookTable.get_saturation_status = (uint16 (*)(void))FunPtr; }
    else if (Index == UT_MULTIROTORMIXER_GROUPS_REQUIRED_INDEX)       { Ut_MultirotorMixer_HookTable.groups_required = (void   (*)(uint32 &))FunPtr; }
    else if (Index == UT_MULTIROTORMIXER_SET_TRIM_INDEX)              { Ut_MultirotorMixer_HookTable.set_trim = (uint32 (*)(float))FunPtr; }
    else if (Index == UT_MULTIROTORMIXER_SET_THRUST_FACTOR_INDEX)     { Ut_MultirotorMixer_HookTable.set_thrust_factor = (void   (*)(float))FunPtr; }
    else
    {
        printf("Unsupported MultirotorMixer Index In SetFunctionHook Call %lu\n", Index);
        UtAssert_True(FALSE, "Unsupported MultirotorMixer Index In SetFunctionHook Call");
    }
};

void Ut_MultirotorMixer_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < UT_MULTIROTORMIXER_MAX_INDEX)
    {
        Ut_MultirotorMixer_ReturnCodeTable[Index].Value = RtnVal;
        Ut_MultirotorMixer_ReturnCodeTable[Index].Count = CallCnt;
    }
    else
    {
        printf("Unsupported MULTIROTORMIXER Index In SetReturnCode Call %lu\n", Index);
        UtAssert_True(FALSE, "Unsupported MULTIROTORMIXER Index In SetReturnCode Call");
    }
}

boolean Ut_MultirotorMixer_UseReturnCode(uint32 Index)
{
    if (Ut_MultirotorMixer_ReturnCodeTable[Index].Count > 0)
    {
        Ut_MultirotorMixer_ReturnCodeTable[Index].Count--;
        if (Ut_MultirotorMixer_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }
    else if (Ut_MultirotorMixer_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero == TRUE)
    {
        return(TRUE);
    }

    return(FALSE);
}

void Ut_MultirotorMixer_ContinueReturnCodeAfterCountZero(uint32 Index)
{
    Ut_MultirotorMixer_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero = TRUE;
}



MultirotorMixer::MultirotorMixer(
    ControlCallback control_cb,
    cpuaddr cb_handle) :
        Mixer(control_cb, cb_handle),
        m_ThrustFactor(0.0f)
{
}

MultirotorMixer::~MultirotorMixer()
{
}

int32 MultirotorMixer::SetConfigTablePtr(
    MultirotorMixer_ConfigTablePtr_t &ConfigTablePtr)
{
    /* Check for specified return */
    if (Ut_MultirotorMixer_UseReturnCode(UT_MULTIROTORMIXER_SETCONFIGTABLEPTR_INDEX))
        return Ut_MultirotorMixer_ReturnCodeTable[UT_MULTIROTORMIXER_SETCONFIGTABLEPTR_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_MultirotorMixer_HookTable.SetConfigTablePtr)
        return Ut_MultirotorMixer_HookTable.SetConfigTablePtr(ConfigTablePtr);

    return 0;
};

uint32
MultirotorMixer::mix(float *outputs, uint32 space, uint16 *status_reg)
{
    /* Check for specified return */
    if (Ut_MultirotorMixer_UseReturnCode(UT_MULTIROTORMIXER_MIX_INDEX))
        return Ut_MultirotorMixer_ReturnCodeTable[UT_MULTIROTORMIXER_MIX_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_MultirotorMixer_HookTable.mix)
        return Ut_MultirotorMixer_HookTable.mix(outputs, space, status_reg);

    return 0;
};

uint16 MultirotorMixer::get_saturation_status()
{
    /* Check for specified return */
    if (Ut_MultirotorMixer_UseReturnCode(UT_MULTIROTORMIXER_GET_SATURATION_STATUS_INDEX))
        return Ut_MultirotorMixer_ReturnCodeTable[UT_MULTIROTORMIXER_GET_SATURATION_STATUS_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_MultirotorMixer_HookTable.get_saturation_status)
        return Ut_MultirotorMixer_HookTable.get_saturation_status();

    return 0;
};

void
MultirotorMixer::groups_required(uint32 &groups)
{
    /* Check for specified return */
    if (Ut_MultirotorMixer_UseReturnCode(UT_MULTIROTORMIXER_GROUPS_REQUIRED_INDEX))
        Ut_MultirotorMixer_ReturnCodeTable[UT_MULTIROTORMIXER_GROUPS_REQUIRED_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_MultirotorMixer_HookTable.groups_required)
        Ut_MultirotorMixer_HookTable.groups_required(groups);
};

uint32 MultirotorMixer::set_trim(float trim)
{
    /* Check for specified return */
    if (Ut_MultirotorMixer_UseReturnCode(UT_MULTIROTORMIXER_SET_TRIM_INDEX))
        return Ut_MultirotorMixer_ReturnCodeTable[UT_MULTIROTORMIXER_SET_TRIM_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_MultirotorMixer_HookTable.set_trim)
        return Ut_MultirotorMixer_HookTable.set_trim(trim);

    return 0;
}

void MultirotorMixer::set_thrust_factor(float val)
{
    /* Check for specified return */
    if (Ut_MultirotorMixer_UseReturnCode(UT_MULTIROTORMIXER_SET_THRUST_FACTOR_INDEX))
        Ut_MultirotorMixer_ReturnCodeTable[UT_MULTIROTORMIXER_SET_THRUST_FACTOR_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_MultirotorMixer_HookTable.set_thrust_factor)
        Ut_MultirotorMixer_HookTable.set_thrust_factor(val);
}



