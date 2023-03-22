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

#ifndef IOLIB_STUBS_H
#define IOLIB_STUBS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tm_sdlp.h"
#include "cop1.h"

typedef enum
{
    UT_IO_LIB_UTIL_GENPSEUDORANDOMSEQ_INDEX,
	UT_TM_SDLP_INITIDLEPACKET_INDEX,
	UT_TM_SDLP_INITCHANNEL_INDEX,
	UT_TM_SDLP_FRAMEHASDATA_INDEX,
	UT_TM_SDLP_ADDPACKET_INDEX,
	UT_TM_SDLP_ADDIDLEPACKET_INDEX,
	UT_TM_SDLP_STARTFRAME_INDEX,
	UT_TM_SDLP_SETOIDFRAME_INDEX,
	UT_TM_SDLP_COMPLETEFRAME_INDEX,
	UT_COP1_INITCLCW_INDEX,
	UT_COP1_GETCLCWVCID_INDEX,
	UT_COP1_PROCESSFRAME_INDEX,
	UT_TM_SYNC_SYNCHRONIZE_INDEX,
	UT_TC_SYNC_GETTRANSFERFRAME_INDEX,
	UT_TCTF_GETSCID_INDEX,
	UT_TCTF_GETVCID_INDEX,
	UT_TCTF_GETPAYLOADLENGTH_INDEX,
    UT_IO_LIB_MAX_INDEX
} Ut_IOLIB_INDEX_t;


typedef struct
{
	int32  (*IO_LIB_UTIL_GenPseudoRandomSeq)(uint8 *pSeqBuff, uint8 poly, uint8 seed);
	int32  (*TM_SDLP_InitIdlePacket)(CFE_SB_Msg_t *pIdlePacket, uint8 *pIdlePattern,
	                             uint16 bufferLength, uint32 patternBitLength);
	int32  (*TM_SDLP_InitChannel)(TM_SDLP_FrameInfo_t *pFrameInfo,
	                          uint8 *pTfBuffer, uint8 *pOfBuffer,
	                          TM_SDLP_GlobalConfig_t *pGlobalConfig,
	                          TM_SDLP_ChannelConfig_t *pChannelConfig);
	int32  (*TM_SDLP_FrameHasData)(TM_SDLP_FrameInfo_t *pFrameInfo);
	int32  (*TM_SDLP_AddPacket)(TM_SDLP_FrameInfo_t *pFrameInfo,
	                        CFE_SB_Msg_t *pPacket);
	int32  (*TM_SDLP_AddIdlePacket)(TM_SDLP_FrameInfo_t *pFrameInfo,
	                            CFE_SB_Msg_t *pIdlePacket);
	int32  (*TM_SDLP_StartFrame)(TM_SDLP_FrameInfo_t *pFrameInfo);
	int32  (*TM_SDLP_SetOidFrame)(TM_SDLP_FrameInfo_t *pFrameInfo,
	                          CFE_SB_Msg_t *pIdlePacket);
	int32  (*TM_SDLP_CompleteFrame)(TM_SDLP_FrameInfo_t *pFrameInfo,
	                            uint8 *pMcFrameCnt, uint8 *pOcf);
	uint32 (*COP1_InitClcw)(COP1_Clcw_t *clcwPtr, uint16 vcId);
	uint16 (*COP1_GetClcwVcId)(COP1_Clcw_t *clcwPtr);
	int32  (*COP1_ProcessFrame)(uint8* toBuffer, COP1_Clcw_t *clcwPtr, TCTF_Hdr_t *tfPtr,
	                       TCTF_ChannelService_t *channelService);
	int32  (*TM_SYNC_Synchronize)(uint8 *pBuff, char *asmStr, uint8 asmSize,
	                          uint16 frameSize, boolean randomize);
	int32  (*TC_SYNC_GetTransferFrame)(uint8 *pTfBuff, uint8 *pCltu,
                              uint16 tfBuffSize, uint16 cltuLength,
                              boolean derandomize);
	uint16 (*TCTF_GetScId)(TCTF_Hdr_t *tfPtr);
	uint16 (*TCTF_GetVcId)(TCTF_Hdr_t *tfPtr);
	uint16 (*TCTF_GetPayloadLength)(TCTF_Hdr_t *tfPtr, TCTF_ChannelService_t *channelService);

} Ut_IOLIB_HookTable_t;

typedef struct
{
    int32   Value;
    uint32  Count;
    boolean ContinueReturnCodeAfterCountZero;
} Ut_IOLIB_ReturnCodeTable_t;

void Ut_IOLIB_Reset(void);
void Ut_IOLIB_SetFunctionHook(uint32 Index, void *FunPtr);
void Ut_IOLIB_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);
void Ut_IOLIB_ContinueReturnCodeAfterCountZero(uint32 Index);



#ifdef __cplusplus
}
#endif

#endif /* TO_STUBS_H */
