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

#include "rcin_custom_stubs.h"
#include "utassert.h"

#include <string.h>


Ut_RCIN_Custom_HookTable_t        Ut_RCIN_Custom_HookTable;
Ut_RCIN_Custom_ReturnCodeTable_t
            Ut_RCIN_Custom_ReturnCodeTable[UT_RCIN_CUSTOM_MAX_INDEX];


void Ut_RCIN_Custom_Reset(void)
{
    memset(&Ut_RCIN_Custom_HookTable, 0x00,
                                      sizeof(Ut_RCIN_Custom_HookTable));
    memset(&Ut_RCIN_Custom_ReturnCodeTable, 0x00,
                                sizeof(Ut_RCIN_Custom_ReturnCodeTable));
}


void Ut_RCIN_Custom_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if(Index == UT_RCIN_CUSTOM_SEDLIB_GETPIPE_INDEX)
    {
        Ut_RCIN_Custom_HookTable.SEDLIB_GetPipe =
                (SEDLIB_ReturnCode_t (*)(char *, uint32, uint32 *))FunPtr;
    }
    else if (Index == UT_RCIN_CUSTOM_SEDLIB_READMSG_INDEX)
    {
        Ut_RCIN_Custom_HookTable.SEDLIB_ReadMsg =
                 (SEDLIB_ReturnCode_t (*)(uint32, CFE_SB_MsgPtr_t))FunPtr;
    }
    else
    {
        printf("Unsupported RCIN_CUSTOM Index In SetFunctionHook "
                                                     "Call %lu\n", Index);
        UtAssert_True(FALSE,
                "Unsupported RCIN_CUSTOM Index In SetFunctionHook Call");
    }
}


void Ut_RCIN_Custom_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < UT_RCIN_CUSTOM_MAX_INDEX)
    {
        Ut_RCIN_Custom_ReturnCodeTable[Index].Value = RtnVal;
        Ut_RCIN_Custom_ReturnCodeTable[Index].Count = CallCnt;
    }
    else
    {
        printf("Unsupported RCIN_CUSTOM Index In SetReturnCode "
                                                     "Call %lu\n", Index);
        UtAssert_True(FALSE,
                   "Unsupported RCIN_CUSTOM Index In SetReturnCode Call");
    }
}


boolean Ut_RCIN_Custom_UseReturnCode(uint32 Index)
{
    if (Ut_RCIN_Custom_ReturnCodeTable[Index].Count > 0)
    {
        Ut_RCIN_Custom_ReturnCodeTable[Index].Count --;
        if (Ut_RCIN_Custom_ReturnCodeTable[Index].Count == 0)
        {
            return(TRUE);
        }
    }
    else if(
         Ut_RCIN_Custom_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero
         == TRUE)
    {
        return(TRUE);
    }

    return(FALSE);
}


void Ut_RCIN_Custom_ContinueReturnCodeAfterCountZero(uint32 Index)
{
    Ut_RCIN_Custom_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero
        = TRUE;
}


SEDLIB_ReturnCode_t SEDLIB_GetPipe(char *PipeName, uint32 Size,
                                   uint32 *PipeHandle)
{
    /* Check for specified return */
    if (Ut_RCIN_Custom_UseReturnCode(UT_RCIN_CUSTOM_SEDLIB_GETPIPE_INDEX))
    {
        return
          (SEDLIB_ReturnCode_t)Ut_RCIN_Custom_ReturnCodeTable[UT_RCIN_CUSTOM_SEDLIB_GETPIPE_INDEX].Value;
    }

    /* Check for Function Hook */
    if (Ut_RCIN_Custom_HookTable.SEDLIB_GetPipe)
    {
        return Ut_RCIN_Custom_HookTable.SEDLIB_GetPipe(
                                        PipeName, Size, PipeHandle);
    }

    return SEDLIB_OK;
}


SEDLIB_ReturnCode_t SEDLIB_ReadMsg(uint32 PipeHandle, CFE_SB_MsgPtr_t Msg)
{
    /* Check for specified return */
    if (Ut_RCIN_Custom_UseReturnCode(UT_RCIN_CUSTOM_SEDLIB_READMSG_INDEX))
    {
        return
          (SEDLIB_ReturnCode_t)Ut_RCIN_Custom_ReturnCodeTable[UT_RCIN_CUSTOM_SEDLIB_READMSG_INDEX].Value;
    }

    /* Check for Function Hook */
    if (Ut_RCIN_Custom_HookTable.SEDLIB_ReadMsg)
    {
        return Ut_RCIN_Custom_HookTable.SEDLIB_ReadMsg(PipeHandle, Msg);
    }

    return SEDLIB_MSG_FRESH_OK;
}
