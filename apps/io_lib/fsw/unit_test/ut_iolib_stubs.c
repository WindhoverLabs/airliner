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
#include "ut_iolib_stubs.h"
#include <string.h>
#include "io_lib.h"

Ut_IOLIB_HookTable_t           Ut_IOLIB_HookTable;
Ut_IOLIB_ReturnCodeTable_t     Ut_IOLIB_ReturnCodeTable[UT_IO_LIB_MAX_INDEX];

void Ut_IOLIB_Reset(void)
{
    memset(&Ut_IOLIB_HookTable, 0, sizeof(Ut_IOLIB_HookTable));
    memset(&Ut_IOLIB_ReturnCodeTable, 0, sizeof(Ut_IOLIB_ReturnCodeTable));
}


void Ut_IOLIB_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if      (Index == UT_IO_LIB_UTIL_GENPSEUDORANDOMSEQ_INDEX)  { Ut_IOLIB_HookTable.IO_LIB_UTIL_GenPseudoRandomSeq = (int32 (*)(uint8*, uint8, uint8))FunPtr; }
    else if (Index == UT_TM_SDLP_INITIDLEPACKET_INDEX)          { Ut_IOLIB_HookTable.TM_SDLP_InitIdlePacket = (int32 (*)(CFE_SB_Msg_t*, uint8*, uint16, uint32))FunPtr; }
    else if (Index == UT_TM_SDLP_INITCHANNEL_INDEX)             { Ut_IOLIB_HookTable.TM_SDLP_InitChannel = (int32 (*)(TM_SDLP_FrameInfo_t*, uint8*, uint8*, TM_SDLP_GlobalConfig_t*, TM_SDLP_ChannelConfig_t*))FunPtr; }
    else if (Index == UT_TM_SDLP_FRAMEHASDATA_INDEX)            { Ut_IOLIB_HookTable.TM_SDLP_FrameHasData = (int32 (*)(TM_SDLP_FrameInfo_t*))FunPtr; }
    else if (Index == UT_TM_SDLP_ADDPACKET_INDEX)               { Ut_IOLIB_HookTable.TM_SDLP_AddPacket = (int32 (*)(TM_SDLP_FrameInfo_t*, CFE_SB_Msg_t*))FunPtr; }
    else if (Index == UT_TM_SDLP_ADDIDLEPACKET_INDEX)           { Ut_IOLIB_HookTable.TM_SDLP_AddIdlePacket = (int32 (*)(TM_SDLP_FrameInfo_t*, CFE_SB_Msg_t*))FunPtr; }
    else if (Index == UT_TM_SDLP_STARTFRAME_INDEX)              { Ut_IOLIB_HookTable.TM_SDLP_StartFrame = (int32 (*)(TM_SDLP_FrameInfo_t*))FunPtr; }
    else if (Index == UT_TM_SDLP_SETOIDFRAME_INDEX)             { Ut_IOLIB_HookTable.TM_SDLP_SetOidFrame = (int32 (*)(TM_SDLP_FrameInfo_t*, CFE_SB_Msg_t*))FunPtr; }
    else if (Index == UT_TM_SDLP_COMPLETEFRAME_INDEX)           { Ut_IOLIB_HookTable.TM_SDLP_CompleteFrame = (int32 (*)(TM_SDLP_FrameInfo_t*, uint8*, uint8*))FunPtr; }
    else if (Index == UT_COP1_INITCLCW_INDEX)                   { Ut_IOLIB_HookTable.COP1_InitClcw = (int32 (*)(COP1_Clcw_t*, uint16))FunPtr; }
    else if (Index == UT_COP1_GETCLCWVCID_INDEX)                { Ut_IOLIB_HookTable.COP1_GetClcwVcId = (uint16 (*)(COP1_Clcw_t*))FunPtr; }
    else if (Index == UT_COP1_PROCESSFRAME_INDEX)               { Ut_IOLIB_HookTable.COP1_ProcessFrame = (uint16 (*)(uint8*, COP1_Clcw_t*, TCTF_Hdr_t*, TCTF_ChannelService_t*))FunPtr; }
    else if (Index == UT_TM_SYNC_SYNCHRONIZE_INDEX)             { Ut_IOLIB_HookTable.TM_SYNC_Synchronize = (int32 (*)(uint8*, char*, uint8, uint16, boolean))FunPtr; }
    else if (Index == UT_TC_SYNC_GETTRANSFERFRAME_INDEX)        { Ut_IOLIB_HookTable.TC_SYNC_GetTransferFrame = (int32 (*)(uint8*, uint8*, uint16, uint16, boolean))FunPtr; }
    else if (Index == UT_TCTF_GETSCID_INDEX)                    { Ut_IOLIB_HookTable.TCTF_GetScId = (uint16 (*)(TCTF_Hdr_t*))FunPtr; }
    else if (Index == UT_TCTF_GETVCID_INDEX)                    { Ut_IOLIB_HookTable.TCTF_GetVcId = (uint16 (*)(TCTF_Hdr_t*))FunPtr; }
    else if (Index == UT_TCTF_GETPAYLOADLENGTH_INDEX)           { Ut_IOLIB_HookTable.TCTF_GetPayloadLength = (uint16 (*)(TCTF_Hdr_t*, TCTF_ChannelService_t*))FunPtr; }

    else
    {
        printf("Unsupported IOLIB Index In SetFunctionHook Call %lu\n", Index);
        UtAssert_True(FALSE, "Unsupported IOLIB Index In SetFunctionHook Call");
    }
}


void Ut_IOLIB_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < UT_IO_LIB_MAX_INDEX)
    {
        Ut_IOLIB_ReturnCodeTable[Index].Value = RtnVal;
        Ut_IOLIB_ReturnCodeTable[Index].Count = CallCnt;
    }
    else
    {
        printf("Unsupported IOLIB Index In SetReturnCode Call %lu\n", Index);
        UtAssert_True(FALSE, "Unsupported IOLIB Index In SetReturnCode Call");
    }
}


boolean Ut_IOLIB_UseReturnCode(uint32 Index)
{
    if (Ut_IOLIB_ReturnCodeTable[Index].Count > 0)
    {
        Ut_IOLIB_ReturnCodeTable[Index].Count--;
        if (Ut_IOLIB_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }
    else if (Ut_IOLIB_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero == TRUE)
    {
        return(TRUE);
    }

    return(FALSE);
}


void Ut_IOLIB_ContinueReturnCodeAfterCountZero(uint32 Index)
{
    Ut_IOLIB_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero = TRUE;
}


int32  IO_LIB_UTIL_GenPseudoRandomSeq(uint8 *pSeqBuff, uint8 poly, uint8 seed)
{
    /* Check for specified return */
    if (Ut_IOLIB_UseReturnCode(UT_IO_LIB_UTIL_GENPSEUDORANDOMSEQ_INDEX))
        return Ut_IOLIB_ReturnCodeTable[UT_IO_LIB_UTIL_GENPSEUDORANDOMSEQ_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_IOLIB_HookTable.IO_LIB_UTIL_GenPseudoRandomSeq)
        return Ut_IOLIB_HookTable.IO_LIB_UTIL_GenPseudoRandomSeq(pSeqBuff, poly, seed);

    return 0;
}


int32  TM_SDLP_InitIdlePacket(CFE_SB_Msg_t *pIdlePacket, uint8 *pIdlePattern,
                             uint16 bufferLength, uint32 patternBitLength)
{
    /* Check for specified return */
    if (Ut_IOLIB_UseReturnCode(UT_TM_SDLP_INITIDLEPACKET_INDEX))
        return Ut_IOLIB_ReturnCodeTable[UT_TM_SDLP_INITIDLEPACKET_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_IOLIB_HookTable.TM_SDLP_InitIdlePacket)
        return Ut_IOLIB_HookTable.TM_SDLP_InitIdlePacket(pIdlePacket, pIdlePattern, bufferLength, patternBitLength);

    return 0;
}


int32  TM_SDLP_InitChannel(TM_SDLP_FrameInfo_t *pFrameInfo,
                          uint8 *pTfBuffer, uint8 *pOfBuffer,
                          TM_SDLP_GlobalConfig_t *pGlobalConfig,
                          TM_SDLP_ChannelConfig_t *pChannelConfig)
{
    /* Check for specified return */
    if (Ut_IOLIB_UseReturnCode(UT_TM_SDLP_INITCHANNEL_INDEX))
        return Ut_IOLIB_ReturnCodeTable[UT_TM_SDLP_INITCHANNEL_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_IOLIB_HookTable.TM_SDLP_InitChannel)
        return Ut_IOLIB_HookTable.TM_SDLP_InitChannel(pFrameInfo, pTfBuffer, pOfBuffer, pGlobalConfig, pChannelConfig);

    return 0;
}


int32  TM_SDLP_FrameHasData(TM_SDLP_FrameInfo_t *pFrameInfo)
{
    /* Check for specified return */
    if (Ut_IOLIB_UseReturnCode(UT_TM_SDLP_FRAMEHASDATA_INDEX))
        return Ut_IOLIB_ReturnCodeTable[UT_TM_SDLP_FRAMEHASDATA_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_IOLIB_HookTable.TM_SDLP_FrameHasData)
        return Ut_IOLIB_HookTable.TM_SDLP_FrameHasData(pFrameInfo);

    return 0;
}


int32  TM_SDLP_AddPacket(TM_SDLP_FrameInfo_t *pFrameInfo,
                        CFE_SB_Msg_t *pPacket)
{
    /* Check for specified return */
    if (Ut_IOLIB_UseReturnCode(UT_TM_SDLP_ADDPACKET_INDEX))
        return Ut_IOLIB_ReturnCodeTable[UT_TM_SDLP_ADDPACKET_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_IOLIB_HookTable.TM_SDLP_AddPacket)
        return Ut_IOLIB_HookTable.TM_SDLP_AddPacket(pFrameInfo, pPacket);

    return 0;
}


int32  TM_SDLP_AddIdlePacket(TM_SDLP_FrameInfo_t *pFrameInfo,
                            CFE_SB_Msg_t *pIdlePacket)
{
    /* Check for specified return */
    if (Ut_IOLIB_UseReturnCode(UT_TM_SDLP_ADDIDLEPACKET_INDEX))
        return Ut_IOLIB_ReturnCodeTable[UT_TM_SDLP_ADDIDLEPACKET_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_IOLIB_HookTable.TM_SDLP_AddIdlePacket)
        return Ut_IOLIB_HookTable.TM_SDLP_AddIdlePacket(pFrameInfo, pIdlePacket);

    return 0;
}


int32  TM_SDLP_StartFrame(TM_SDLP_FrameInfo_t *pFrameInfo)
{
    /* Check for specified return */
    if (Ut_IOLIB_UseReturnCode(UT_TM_SDLP_STARTFRAME_INDEX))
        return Ut_IOLIB_ReturnCodeTable[UT_TM_SDLP_STARTFRAME_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_IOLIB_HookTable.TM_SDLP_StartFrame)
        return Ut_IOLIB_HookTable.TM_SDLP_StartFrame(pFrameInfo);

    return 0;
}


int32  TM_SDLP_SetOidFrame(TM_SDLP_FrameInfo_t *pFrameInfo,
                          CFE_SB_Msg_t *pIdlePacket)
{
    /* Check for specified return */
    if (Ut_IOLIB_UseReturnCode(UT_TM_SDLP_SETOIDFRAME_INDEX))
        return Ut_IOLIB_ReturnCodeTable[UT_TM_SDLP_SETOIDFRAME_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_IOLIB_HookTable.TM_SDLP_SetOidFrame)
        return Ut_IOLIB_HookTable.TM_SDLP_SetOidFrame(pFrameInfo, pIdlePacket);

    return 0;
}


int32  TM_SDLP_CompleteFrame(TM_SDLP_FrameInfo_t *pFrameInfo,
                            uint8 *pMcFrameCnt, uint8 *pOcf)
{
    /* Check for specified return */
    if (Ut_IOLIB_UseReturnCode(UT_TM_SDLP_COMPLETEFRAME_INDEX))
        return Ut_IOLIB_ReturnCodeTable[UT_TM_SDLP_COMPLETEFRAME_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_IOLIB_HookTable.TM_SDLP_CompleteFrame)
        return Ut_IOLIB_HookTable.TM_SDLP_CompleteFrame(pFrameInfo, pMcFrameCnt, pOcf);

    return 0;
}


uint16 COP1_GetClcwVcId(COP1_Clcw_t *clcwPtr)
{
    /* Check for specified return */
    if (Ut_IOLIB_UseReturnCode(UT_COP1_GETCLCWVCID_INDEX))
        return Ut_IOLIB_ReturnCodeTable[UT_COP1_GETCLCWVCID_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_IOLIB_HookTable.COP1_GetClcwVcId)
        return Ut_IOLIB_HookTable.COP1_GetClcwVcId(clcwPtr);

    return 0;
}


int32  TM_SYNC_Synchronize(uint8 *pBuff, char *asmStr, uint8 asmSize,
                          uint16 frameSize, boolean randomize)
{
    /* Check for specified return */
    if (Ut_IOLIB_UseReturnCode(UT_TM_SYNC_SYNCHRONIZE_INDEX))
        return Ut_IOLIB_ReturnCodeTable[UT_TM_SYNC_SYNCHRONIZE_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_IOLIB_HookTable.TM_SYNC_Synchronize)
        return Ut_IOLIB_HookTable.TM_SYNC_Synchronize(pBuff, asmStr, asmSize, frameSize, randomize);

    return 0;
}









int32   COP1_InitClcw(COP1_Clcw_t *clcwPtr, uint16 vcId)
{
    /* Check for specified return */
    if (Ut_IOLIB_UseReturnCode(UT_COP1_INITCLCW_INDEX))
        return Ut_IOLIB_ReturnCodeTable[UT_COP1_INITCLCW_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_IOLIB_HookTable.COP1_InitClcw)
        return Ut_IOLIB_HookTable.COP1_InitClcw(clcwPtr, vcId);

    return 0;
}


int32   COP1_ProcessFrame(uint8* toBuffer, COP1_Clcw_t *clcwPtr, TCTF_Hdr_t *tfPtr,
                       TCTF_ChannelService_t *channelService)
{
    /* Check for specified return */
    if (Ut_IOLIB_UseReturnCode(UT_COP1_PROCESSFRAME_INDEX))
        return Ut_IOLIB_ReturnCodeTable[UT_COP1_PROCESSFRAME_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_IOLIB_HookTable.COP1_ProcessFrame)
        return Ut_IOLIB_HookTable.COP1_ProcessFrame(toBuffer, clcwPtr, tfPtr, channelService);

    return 0;
}


int32 TC_SYNC_GetTransferFrame(uint8 *pTfBuff, uint8 *pCltu,
                               uint16 tfBuffSize, uint16 cltuLength,
                               boolean derandomize)
{
    /* Check for specified return */
    if (Ut_IOLIB_UseReturnCode(UT_TC_SYNC_GETTRANSFERFRAME_INDEX))
        return Ut_IOLIB_ReturnCodeTable[UT_TC_SYNC_GETTRANSFERFRAME_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_IOLIB_HookTable.TC_SYNC_GetTransferFrame)
        return Ut_IOLIB_HookTable.TC_SYNC_GetTransferFrame(pTfBuff, pCltu, tfBuffSize, cltuLength, derandomize);

    return 0;
}


uint16  TCTF_GetScId(TCTF_Hdr_t *tfPtr)
{
    /* Check for specified return */
    if (Ut_IOLIB_UseReturnCode(UT_TCTF_GETSCID_INDEX))
        return Ut_IOLIB_ReturnCodeTable[UT_TCTF_GETSCID_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_IOLIB_HookTable.TCTF_GetScId)
        return Ut_IOLIB_HookTable.TCTF_GetScId(tfPtr);

    return 0;
}


uint16  TCTF_GetVcId(TCTF_Hdr_t *tfPtr)
{
    /* Check for specified return */
    if (Ut_IOLIB_UseReturnCode(UT_TCTF_GETVCID_INDEX))
        return Ut_IOLIB_ReturnCodeTable[UT_TCTF_GETVCID_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_IOLIB_HookTable.TCTF_GetVcId)
        return Ut_IOLIB_HookTable.TCTF_GetVcId(tfPtr);

    return 0;
}


uint16  TCTF_GetPayloadLength(TCTF_Hdr_t *tfPtr, TCTF_ChannelService_t *channelService)
{
    /* Check for specified return */
    if (Ut_IOLIB_UseReturnCode(UT_TCTF_GETPAYLOADLENGTH_INDEX))
        return Ut_IOLIB_ReturnCodeTable[UT_TCTF_GETPAYLOADLENGTH_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_IOLIB_HookTable.TCTF_GetPayloadLength)
        return Ut_IOLIB_HookTable.TCTF_GetPayloadLength(tfPtr, channelService);

    return 0;
}

