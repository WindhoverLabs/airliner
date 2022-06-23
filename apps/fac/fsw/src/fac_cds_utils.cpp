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

#include <string.h>
#include "fac_app.hpp"
#include "fac_cds_task.hpp"
#include "fac_cds_utils.hpp"

extern FAC_CdsTaskData_t  FAC_CdsTaskData;


int32 FAC_InitCdsTbl()
{
   int32  iStatus = CFE_SUCCESS;
   int32  iResetType = 0;
   uint32 uiResetSubType = 0;

   /* Only clear CDS table when it's a PowerOn reset, and not a Processor reset */
   iResetType = CFE_ES_GetResetType(&uiResetSubType);
   if (iResetType == CFE_ES_POWERON_RESET)
   {
      memset((void*)&FAC_CdsTaskData.CdsTbl, 0x00, sizeof(FAC_CdsTbl_t));
   }

   /* Register and manage CDS table */
   iStatus = CFE_ES_RegisterCDS(&FAC_CdsTaskData.CdsTblHdl, sizeof(FAC_CdsTbl_t),
                                FAC_CDS_TABLENAME);
   if (iStatus == CFE_SUCCESS)
   {
      /* Setup initial content of CDS table */
      iStatus = CFE_ES_CopyToCDS(FAC_CdsTaskData.CdsTblHdl, &FAC_CdsTaskData.CdsTbl);
      if (iStatus == CFE_SUCCESS)
      {
         CFE_EVS_SendEvent(FAC_CDS_INF_EID, CFE_EVS_INFORMATION,
                           "Successfully setup CDS");
      }
      else
      {
         CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                           "Failed to setup CDS");
      }
   }
   else if (iStatus == CFE_ES_CDS_ALREADY_EXISTS)
   {
      /* If one already exists, get a copy of its current content */
      memset((void*)&FAC_CdsTaskData.CdsTbl, 0x00, sizeof(FAC_CdsTbl_t));
      iStatus = CFE_ES_RestoreFromCDS(&FAC_CdsTaskData.CdsTbl, FAC_CdsTaskData.CdsTblHdl);
      if (iStatus == CFE_SUCCESS)
      {
         CFE_EVS_SendEvent(FAC_CDS_INF_EID, CFE_EVS_INFORMATION,
                           "Successfully restored data from CDS");
      }
      else
      {
         CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                           "Failed to restore data from CDS");
         memset((void*)&FAC_CdsTaskData.CdsTbl, 0x00, sizeof(FAC_CdsTbl_t));
      }
   }
   else
   {
      CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                        "Failed to create CDS (0x%08X)", (unsigned int)iStatus);
   }

   return (iStatus);
}

void FAC_UpdateCdsTbl()
{
   /* TODO:  Add code to update values in CDS table here */
}

void FAC_SaveCdsTbl()
{
   /* TODO This return value is not checked. Developer should decide what to do here
      in case of failure or should add a return value for higher-level logic to handle. */
   CFE_ES_CopyToCDS(FAC_CdsTaskData.CdsTblHdl, &FAC_CdsTaskData.CdsTbl);
}
