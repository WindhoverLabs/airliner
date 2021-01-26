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
#include "ut_to_custom_stubs.h"
#include "ut_to_custom_hooks.h"
#include <string.h>
#include "to_app.h"

Ut_TO_Custom_HookTable_t           Ut_TO_Custom_HookTable;
Ut_TO_Custom_ReturnCodeTable_t     Ut_TO_Custom_ReturnCodeTable[UT_TO_CUSTOM_MAX_INDEX];

void Ut_TO_Custom_Reset(void)
{
    memset(&Ut_TO_Custom_HookTable, 0, sizeof(Ut_TO_Custom_HookTable));
    memset(&Ut_TO_Custom_ReturnCodeTable, 0, sizeof(Ut_TO_Custom_ReturnCodeTable));
}


void Ut_TO_Custom_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if      (Index == UT_TO_CUSTOM_INIT_INDEX)     { Ut_TO_Custom_HookTable.TO_Custom_Init = (int32 (*)(void))FunPtr; }
    else if (Index == UT_TO_CUSTOM_INITEVENT_INDEX)     { Ut_TO_Custom_HookTable.TO_Custom_InitEvent = (int32 (*)(int32))FunPtr; }
    else if (Index == UT_TO_OUTPUTCHANNEL_CUSTOMBUILDUP_INDEX)   { Ut_TO_Custom_HookTable.TO_OutputChannel_CustomBuildup = (int32 (*)(uint32))FunPtr; }
    else if (Index == UT_TO_OUTPUTCHANNEL_CUSTOMTEARDOWN_INDEX)   { Ut_TO_Custom_HookTable.TO_OutputChannel_CustomTeardown = (int32 (*)(uint32))FunPtr; }
    else if (Index == UT_TO_OUTPUTCHANNEL_CUSTOMCLEANUPALL_INDEX)   { Ut_TO_Custom_HookTable.TO_OutputChannel_CustomCleanupAll = (void (*)(void))FunPtr; }
    else if (Index == UT_TO_OUTPUTCHANNEL_PROCESSNEWCUSTOMCMDS_INDEX)   { Ut_TO_Custom_HookTable.TO_OutputChannel_ProcessNewCustomCmds = (void (*)(CFE_SB_Msg_t*))FunPtr; }
    else if (Index == UT_TO_OUTPUTCHANNEL_STATUS_INDEX)   { Ut_TO_Custom_HookTable.TO_OutputChannel_Status = (uint8 (*)(uint32))FunPtr; }
    else if (Index == UT_TO_GETCUSTOMVERSION_INDEX)   { Ut_TO_Custom_HookTable.TO_GetCustomVersion = (uint32 (*)(void))FunPtr; }
    else if (Index == UT_TO_PRINTCUSTOMVERSION_INDEX)   { Ut_TO_Custom_HookTable.TO_PrintCustomVersion = (void (*)(void))FunPtr; }
    else if (Index == UT_TO_RESETCUSTOMCHANNELCOUNTERS_INDEX)   { Ut_TO_Custom_HookTable.TO_ResetCustomChannelCounters = (void (*)(void))FunPtr; }
    else if (Index == UT_TO_UPDATECUSTOMDATAFROMTABLE_INDEX)   { Ut_TO_Custom_HookTable.TO_UpdateCustomDataFromTable = (void (*)(uint8,osalbool,osalbool))FunPtr; }
    else
    {
        printf("Unsupported TO_CUSTOM Index In SetFunctionHook Call %lu\n", Index);
        UtAssert_True(FALSE, "Unsupported TO_CUSTOM Index In SetFunctionHook Call");
    }
}


void Ut_TO_Custom_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < UT_TO_CUSTOM_MAX_INDEX)
    {
        Ut_TO_Custom_ReturnCodeTable[Index].Value = RtnVal;
        Ut_TO_Custom_ReturnCodeTable[Index].Count = CallCnt;
    }
    else
    {
        printf("Unsupported TO_CUSTOM Index In SetReturnCode Call %lu\n", Index);
        UtAssert_True(FALSE, "Unsupported TO_CUSTOM Index In SetReturnCode Call");
    }
}


boolean Ut_TO_Custom_UseReturnCode(uint32 Index)
{
    if (Ut_TO_Custom_ReturnCodeTable[Index].Count > 0)
    {
        Ut_TO_Custom_ReturnCodeTable[Index].Count--;
        if (Ut_TO_Custom_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }
    else if (Ut_TO_Custom_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero == TRUE)
    {
        return(TRUE);
    }

    return(FALSE);
}


void Ut_TO_Custom_ContinueReturnCodeAfterCountZero(uint32 Index)
{
    Ut_TO_Custom_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero = TRUE;
}


int32  TO_Custom_Init(void)
{
    /* Check for specified return */
    if (Ut_TO_Custom_UseReturnCode(UT_TO_CUSTOM_INIT_INDEX))
        return Ut_TO_Custom_ReturnCodeTable[UT_TO_CUSTOM_INIT_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_TO_Custom_HookTable.TO_Custom_Init)
        return Ut_TO_Custom_HookTable.TO_Custom_Init();

    return 0;
}


int32  TO_Custom_InitEvent(int32 *ind)
{
    /* Check for specified return */
    if (Ut_TO_Custom_UseReturnCode(UT_TO_CUSTOM_INITEVENT_INDEX))
        return Ut_TO_Custom_ReturnCodeTable[UT_TO_CUSTOM_INITEVENT_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_TO_Custom_HookTable.TO_Custom_InitEvent)
        return Ut_TO_Custom_HookTable.TO_Custom_InitEvent(*ind);

    return 0;
}


int32  TO_OutputChannel_CustomBuildup(uint32 index)
{
    /* Check for specified return */
    if (Ut_TO_Custom_UseReturnCode(UT_TO_OUTPUTCHANNEL_CUSTOMBUILDUP_INDEX))
        return Ut_TO_Custom_ReturnCodeTable[UT_TO_OUTPUTCHANNEL_CUSTOMBUILDUP_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_TO_Custom_HookTable.TO_OutputChannel_CustomBuildup)
        return Ut_TO_Custom_HookTable.TO_OutputChannel_CustomBuildup(index);

    return 0;
}


int32  TO_OutputChannel_CustomTeardown(uint32 index)
{
    /* Check for specified return */
    if (Ut_TO_Custom_UseReturnCode(UT_TO_OUTPUTCHANNEL_CUSTOMTEARDOWN_INDEX))
        return Ut_TO_Custom_ReturnCodeTable[UT_TO_OUTPUTCHANNEL_CUSTOMTEARDOWN_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_TO_Custom_HookTable.TO_OutputChannel_CustomTeardown)
        return Ut_TO_Custom_HookTable.TO_OutputChannel_CustomTeardown(index);

    return 0;
}


void   TO_OutputChannel_CustomCleanupAll(void)
{
    /* Check for specified return */
    if (Ut_TO_Custom_UseReturnCode(UT_TO_OUTPUTCHANNEL_CUSTOMCLEANUPALL_INDEX))
        Ut_TO_Custom_ReturnCodeTable[UT_TO_OUTPUTCHANNEL_CUSTOMCLEANUPALL_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_TO_Custom_HookTable.TO_OutputChannel_CustomCleanupAll)
        Ut_TO_Custom_HookTable.TO_OutputChannel_CustomCleanupAll();
}


void   TO_OutputChannel_ProcessNewCustomCmds(CFE_SB_Msg_t* MsgPtr)
{
    /* Check for specified return */
    if (Ut_TO_Custom_UseReturnCode(UT_TO_OUTPUTCHANNEL_PROCESSNEWCUSTOMCMDS_INDEX))
        Ut_TO_Custom_ReturnCodeTable[UT_TO_OUTPUTCHANNEL_PROCESSNEWCUSTOMCMDS_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_TO_Custom_HookTable.TO_OutputChannel_ProcessNewCustomCmds)
        Ut_TO_Custom_HookTable.TO_OutputChannel_ProcessNewCustomCmds(MsgPtr);
}


uint8  TO_OutputChannel_Status(uint32 index)
{
    /* Check for specified return */
    if (Ut_TO_Custom_UseReturnCode(UT_TO_OUTPUTCHANNEL_STATUS_INDEX))
        return Ut_TO_Custom_ReturnCodeTable[UT_TO_OUTPUTCHANNEL_STATUS_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_TO_Custom_HookTable.TO_OutputChannel_Status)
        return Ut_TO_Custom_HookTable.TO_OutputChannel_Status(index);

    return 0;
}


uint32 TO_GetCustomVersion(void)
{
    /* Check for specified return */
    if (Ut_TO_Custom_UseReturnCode(UT_TO_GETCUSTOMVERSION_INDEX))
        return Ut_TO_Custom_ReturnCodeTable[UT_TO_GETCUSTOMVERSION_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_TO_Custom_HookTable.TO_GetCustomVersion)
        return Ut_TO_Custom_HookTable.TO_GetCustomVersion();

    return 0;
}


void   TO_PrintCustomVersion(void)
{
    /* Check for specified return */
    if (Ut_TO_Custom_UseReturnCode(UT_TO_PRINTCUSTOMVERSION_INDEX))
        Ut_TO_Custom_ReturnCodeTable[UT_TO_PRINTCUSTOMVERSION_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_TO_Custom_HookTable.TO_PrintCustomVersion)
        Ut_TO_Custom_HookTable.TO_PrintCustomVersion();
}


void   TO_ResetCustomChannelCounters(void)
{
    /* Check for specified return */
    if (Ut_TO_Custom_UseReturnCode(UT_TO_RESETCUSTOMCHANNELCOUNTERS_INDEX))
        Ut_TO_Custom_ReturnCodeTable[UT_TO_RESETCUSTOMCHANNELCOUNTERS_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_TO_Custom_HookTable.TO_ResetCustomChannelCounters)
        Ut_TO_Custom_HookTable.TO_ResetCustomChannelCounters();
}


void   TO_UpdateCustomDataFromTable(uint16 ChannelID,
                                  osalbool sendEvent,
                                  osalbool abandonCurrentMsg)
{
    /* Check for specified return */
    if (Ut_TO_Custom_UseReturnCode(UT_TO_UPDATECUSTOMDATAFROMTABLE_INDEX))
        Ut_TO_Custom_ReturnCodeTable[UT_TO_UPDATECUSTOMDATAFROMTABLE_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_TO_Custom_HookTable.TO_UpdateCustomDataFromTable)
        Ut_TO_Custom_HookTable.TO_UpdateCustomDataFromTable(ChannelID, sendEvent, abandonCurrentMsg);
}
