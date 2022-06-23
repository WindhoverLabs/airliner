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

#ifndef FAC_CDS_TASK_HPP
#define FAC_CDS_TASK_HPP

#include "fac_app.hpp"
#include "fac_tbldefs.h"
#include "fac_msg.h"

enum
{
   FAC_GROUP_RESERVED = 0,
   FAC_GROUP_NONE,
   FAC_GROUP_Actuator,
   FAC_GROUP_Fw,
   FAC_GROUP_Vehicle,
   FAC_GROUP_Sensor
};

typedef enum
{
   FAC_PX4_INC_ERR_CTR = -1,  /* Error detected, increment counter */
   FAC_PX4_DONT_INC_CTR = 0,  /* No errors detected but don't increment counter */
   FAC_PX4_INC_TLM_CTR = 1    /* No errors detected and increment counter */
} FAC_PX4MsgFuncRet_t;

typedef FAC_PX4MsgFuncRet_t (*FAC_PX4MsgFuncPtr_t)(const uint8 *Payload);

typedef struct
{
   uint16 cdsGrpId;
   uint32 ExpectedLength;
   FAC_PX4MsgFuncPtr_t  MsgFuncPtr;
} FAC_CDS_PX4HandlerTblRec_t;

typedef struct
{
   uint16  msgId;
   uint16  cdsGrpId;
} FAC_CDS_DataGrpTbl_t;


class CriticalDataStorage
{
public:
   CriticalDataStorage();
   ~CriticalDataStorage();

   int32 InitCdsTask();
   int32 RcvDataMsg(int32 iBlocking);

private:
   CFE_SB_PipeId_t     DataPipeId;

   FAC_InData_t      InData;
   FAC_OutData_t     OutData;

   int32 InitPipe();
   int32 InitData();
   int32 GetCdsGrpId(CFE_SB_MsgId_t DataMsgId);
};


typedef struct
{
   uint32 CdsTaskId;
   uint32 uiCdsRunStatus;

   CriticalDataStorage *pCds;

   CFE_TBL_Handle_t    ConfigTblHdl;
   CFE_ES_CDSHandle_t  CdsTblHdl;

   FAC_ConfigTblEntry_t *ConfigTblPtr;
   FAC_CdsTbl_t         CdsTbl;

} FAC_CdsTaskData_t;

void CdsTask();

#endif
