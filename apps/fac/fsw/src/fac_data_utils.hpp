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

#ifndef FAC_DATA_UTILS_HPP
#define FAC_DATA_UTILS_HPP


#if 0
typedef enum
{
   FAC_PX4_INC_ERR_CTR = -1,  /* Error detected, increment counter */
   FAC_PX4_DONT_INC_CTR = 0,  /* No errors detected but don't increment counter */
   FAC_PX4_INC_TLM_CTR = 1    /* No errors detected and increment counter */
} FAC_PX4MsgFuncRet_t;
#endif


class AppDataProcess
{
public:
   AppDataProcess();
   ~AppDataProcess();

   int32 InitDataPipe();
   int32 InitTables();
   int32 RcvDataMsg(int32 iBlocking);
   void  SendOutData();

   static int32 ValidateConfigTbl(void* ConfigTblPtr);

private:
   CFE_SB_PipeId_t     DataPipeId;

   FAC_InData_t      InData;
   FAC_OutData_t     OutData;

   int32 InitData();
   int32 InitConfigTbl();

   int32 Subscribe(CFE_SB_MsgId_t MsgId);
   int32 VerifyDataMsgLength(CFE_SB_Msg_t *MsgPtr, uint16 usExpectedLen);

   int32 AcquireConfigPointers();
   void  ProcessNewConfigTbl();

};


#endif
