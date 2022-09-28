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
#include "ut_to_stubs.h"
#include "ut_to_hooks.h"
#include <string.h>
#include "to_app.h"

Ut_TO_HookTable_t           Ut_TO_HookTable;
Ut_TO_ReturnCodeTable_t     Ut_TO_ReturnCodeTable[UT_TO_MAX_INDEX];

void Ut_TO_Reset(void)
{
    memset(&Ut_TO_HookTable, 0, sizeof(Ut_TO_HookTable));
    memset(&Ut_TO_ReturnCodeTable, 0, sizeof(Ut_TO_ReturnCodeTable));
}


void Ut_TO_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if      (Index == UT_TO_CHANNEL_STATE_INDEX)       { Ut_TO_HookTable.TO_Channel_State = (uint8 (*)(uint8))FunPtr; }
    else if (Index == UT_TO_VERIFYCMDLENGTH_INDEX)     { Ut_TO_HookTable.TO_VerifyCmdLength = (osalbool (*)(CFE_SB_Msg_t*, uint16))FunPtr; }
    else if (Index == UT_TO_CHANNEL_LOCKBYINDEX_INDEX) { Ut_TO_HookTable.TO_Channel_LockByIndex = (void (*)(uint8))FunPtr; }
    else if (Index == UT_TO_MESSAGEFLOW_GETOBJECT_INDEX) {Ut_TO_HookTable.TO_MessageFlow_GetObject = (TO_MessageFlow_t* (*)(TO_ChannelData_t*, CFE_SB_MsgId_t, uint32*))FunPtr; }
    else if (Index == UT_TO_OUTPUTQUEUE_GETMSG_INDEX) {Ut_TO_HookTable.TO_OutputQueue_GetMsg = (int32* (*)(TO_ChannelData_t*, CFE_SB_MsgPtr_t*, uint32))FunPtr; }
    else
    {
        printf("Unsupported TO Index In SetFunctionHook Call %lu\n", Index);
        UtAssert_True(FALSE, "Unsupported TO Index In SetFunctionHook Call");
    }
}


void Ut_TO_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < UT_TO_MAX_INDEX)
    {
        Ut_TO_ReturnCodeTable[Index].Value = RtnVal;
        Ut_TO_ReturnCodeTable[Index].Count = CallCnt;
    }
    else
    {
        printf("Unsupported TO Index In SetReturnCode Call %lu\n", Index);
        UtAssert_True(FALSE, "Unsupported TO Index In SetReturnCode Call");
    }
}


boolean Ut_TO_UseReturnCode(uint32 Index)
{
    if (Ut_TO_ReturnCodeTable[Index].Count > 0)
    {
        Ut_TO_ReturnCodeTable[Index].Count--;
        if (Ut_TO_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }
    else if (Ut_TO_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero == TRUE)
    {
        return(TRUE);
    }

    return(FALSE);
}


void Ut_TO_ContinueReturnCodeAfterCountZero(uint32 Index)
{
    Ut_TO_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero = TRUE;
}


uint8  TO_Channel_State(uint16 index)
{
    /* Check for specified return */
    if (Ut_TO_UseReturnCode(UT_TO_CHANNEL_STATE_INDEX))
        return Ut_TO_ReturnCodeTable[UT_TO_CHANNEL_STATE_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_TO_HookTable.TO_Channel_State)
        return Ut_TO_HookTable.TO_Channel_State(index);

    return 0;
}



osalbool TO_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
                           uint16 usExpectedLen)
{
    /* Check for specified return */
    if (Ut_TO_UseReturnCode(UT_TO_VERIFYCMDLENGTH_INDEX))
        return Ut_TO_ReturnCodeTable[UT_TO_VERIFYCMDLENGTH_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_TO_HookTable.TO_VerifyCmdLength)
        return Ut_TO_HookTable.TO_VerifyCmdLength(MsgPtr, usExpectedLen);

    return TRUE;
}


void  TO_Channel_LockByIndex(uint16 index)
{
    /* Check for specified return */
    if (Ut_TO_UseReturnCode(UT_TO_CHANNEL_LOCKBYINDEX_INDEX))
        Ut_TO_ReturnCodeTable[UT_TO_CHANNEL_LOCKBYINDEX_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_TO_HookTable.TO_Channel_LockByIndex)
        Ut_TO_HookTable.TO_Channel_LockByIndex(index);
}


void  TO_Channel_UnlockByIndex(uint16 index)
{
    /* Check for specified return */
    if (Ut_TO_UseReturnCode(UT_TO_CHANNEL_UNLOCKBYINDEX_INDEX))
        Ut_TO_ReturnCodeTable[UT_TO_CHANNEL_UNLOCKBYINDEX_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_TO_HookTable.TO_Channel_UnlockByIndex)
        Ut_TO_HookTable.TO_Channel_UnlockByIndex(index);
}


void  TO_Channel_LockByRef(TO_ChannelData_t *channel)
{
    /* Check for specified return */
    if (Ut_TO_UseReturnCode(UT_TO_CHANNEL_LOCKBYREF_INDEX))
        Ut_TO_ReturnCodeTable[UT_TO_CHANNEL_LOCKBYREF_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_TO_HookTable.TO_Channel_LockByRef)
        Ut_TO_HookTable.TO_Channel_LockByRef(channel);
}


void  TO_Channel_UnlockByRef(TO_ChannelData_t *channel)
{
    /* Check for specified return */
    if (Ut_TO_UseReturnCode(UT_TO_CHANNEL_UNLOCKBYREF_INDEX))
        Ut_TO_ReturnCodeTable[UT_TO_CHANNEL_UNLOCKBYREF_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_TO_HookTable.TO_Channel_UnlockByRef)
        Ut_TO_HookTable.TO_Channel_UnlockByRef(channel);
}


TO_MessageFlow_t* TO_MessageFlow_GetObject(TO_ChannelData_t* channel, CFE_SB_MsgId_t MsgID, uint32 *Index)
{
    /* Check for specified return */
    if (Ut_TO_UseReturnCode(UT_TO_MESSAGEFLOW_GETOBJECT_INDEX))
        return (TO_MessageFlow_t*)Ut_TO_ReturnCodeTable[UT_TO_MESSAGEFLOW_GETOBJECT_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_TO_HookTable.TO_MessageFlow_GetObject)
        return Ut_TO_HookTable.TO_MessageFlow_GetObject(channel, MsgID, Index);

    return 0;
}


int32 TO_Channel_OpenChannel(const uint32 index, const char *ChannelName,
        const char *ConfigTableName, const char *ConfigTableFileName, TO_ChannelTbl_t *BackupTbl,
        const char *DumpTableName, const uint32 CfCntSemMax, const char *CfCntSemName)
{
    /* Check for specified return */
    if (Ut_TO_UseReturnCode(UT_TO_CHANNEL_OPENCHANNEL_INDEX))
        return Ut_TO_ReturnCodeTable[UT_TO_CHANNEL_OPENCHANNEL_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_TO_HookTable.TO_Channel_OpenChannel)
        return Ut_TO_HookTable.TO_Channel_OpenChannel(index, ChannelName, ConfigTableName, ConfigTableFileName, BackupTbl, DumpTableName, CfCntSemMax, CfCntSemName);

    return 0;
}


int32 TO_OutputQueue_GetMsg(TO_ChannelData_t *channel, CFE_SB_MsgPtr_t *MsgPtr, int32 Timeout )
{
    /* Check for specified return */
    if (Ut_TO_UseReturnCode(UT_TO_OUTPUTQUEUE_GETMSG_INDEX))
        return Ut_TO_ReturnCodeTable[UT_TO_OUTPUTQUEUE_GETMSG_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_TO_HookTable.TO_OutputQueue_GetMsg)
        return Ut_TO_HookTable.TO_OutputQueue_GetMsg(channel, MsgPtr, Timeout);

    return 0;
}
