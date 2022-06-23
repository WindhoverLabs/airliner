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

#include "fac_config_utils.hpp"
#include "fac_cds_task.hpp"

extern FAC_CdsTaskData_t  FAC_CdsTaskData;


int32 FAC_InitConfigTbl()
{
   int32 iStatus = CFE_SUCCESS;

   /* Register Config table */
   iStatus = CFE_TBL_Register(&FAC_CdsTaskData.ConfigTblHdl,
                       FAC_CONFIG_TABLENAME,
                       (sizeof(FAC_ConfigTblEntry_t) * FAC_CONFIG_TABLE_MAX_ENTRIES),
                       CFE_TBL_OPT_DEFAULT,
                       FAC_ValidateConfigTbl);
   if (iStatus == CFE_SUCCESS)
   {
      CFE_EVS_SendEvent(FAC_INIT_INF_EID, CFE_EVS_INFORMATION,
                        "Succeeded to register Config table (0x%08X)",
                        (unsigned int)iStatus);
   }
   else if (iStatus == CFE_TBL_INFO_RECOVERED_TBL)
   {
      CFE_EVS_SendEvent(FAC_INIT_INF_EID, CFE_EVS_INFORMATION,
                        "Succeeded to recover Config table (0x%08X)",
                        (unsigned int)iStatus);
   }
   else if (iStatus == CFE_TBL_WARN_NOT_CRITICAL)
   {
      CFE_EVS_SendEvent(FAC_INIT_INF_EID, CFE_EVS_INFORMATION,
                        "Not Critical Initialization Error of Config table (0x%08X)",
                        (unsigned int)iStatus);
   }
   else
   {
      /* Note, a critical table could return another nominal code.  If this table is
       * made critical this logic would have to change. */
      CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                        "Failed to register Config table (0x%08X)",
                        (unsigned int)iStatus);
      goto FAC_InitConfigTbl_Exit_Tag;
   }

   /* Load Config table file */
   iStatus = CFE_TBL_Load(FAC_CdsTaskData.ConfigTblHdl, CFE_TBL_SRC_FILE,
                          FAC_CONFIG_TABLE_FILENAME);
   if (iStatus != CFE_SUCCESS)
   {
      /* Note, CFE_SUCCESS is for a successful full table load.  If a partial table
         load is desired then this logic would have to change. */
      CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                        "Failed to load Config Table (0x%08X)",
                        (unsigned int)iStatus);
      goto FAC_InitConfigTbl_Exit_Tag;
   }
   else
   {
      CFE_EVS_SendEvent(FAC_INIT_INF_EID, CFE_EVS_INFORMATION,
                        "Succeeded to load Config Table (0x%08X)",
                        (unsigned int)iStatus);
   }

FAC_InitConfigTbl_Exit_Tag:
   return (iStatus);
}

int32 FAC_ValidateConfigTbl(void* ConfigTblPtr)
{
   int32  iStatus = CFE_SUCCESS;
   FAC_ConfigTblEntry_t* FAC_ConfigTblPtr = (FAC_ConfigTblEntry_t*)(ConfigTblPtr);

   if (ConfigTblPtr == NULL)
   {
      iStatus = -1;
      goto FAC_ValidateConfigTbl_Exit_Tag;
   }

   /* TODO:  Add code to validate new data values here.
   **
   ** Examples:
   ** if (FAC_ConfigTblPtr->iParam <= 16) {
   **   (void) CFE_EVS_SendEvent(FAC_CONFIG_TABLE_INF_EID, CFE_EVS_ERROR,
    *                         "Invalid value for Config parameter sParam (%d)",
   **                         FAC_ConfigTblPtr->iParam);
   ** }
   **/

FAC_ValidateConfigTbl_Exit_Tag:
   return (iStatus);
}

void FAC_ProcessNewConfigTbl()
{
   /* TODO:  Add code to set new Config parameters with new values here.
   **
   ** Examples:
   **
   **    FAC_MainAppData.latest_sParam = FAC_MainAppData.ConfigTblPtr->sParam;
   **    FAC_MainAppData.latest_fParam = FAC_MainAppData.ConfigTblPtr->fParam;
   */
}

int32 FAC_AcquireConfigPointers()
{
   int32 iStatus = CFE_SUCCESS;

   /*
   ** Release the table
   */
   /* TODO: This return value can indicate success, error, or that the info has been
    * updated.  We ignore this return value in favor of checking CFE_TBL_Manage(), but
    * be sure this is the behavior you want. */
   CFE_TBL_ReleaseAddress(FAC_CdsTaskData.ConfigTblHdl);

   /*
   ** Manage the table
   */
   iStatus = CFE_TBL_Manage(FAC_CdsTaskData.ConfigTblHdl);
   if ((iStatus != CFE_SUCCESS) && (iStatus != CFE_TBL_INFO_UPDATED))
   {
      CFE_EVS_SendEvent(FAC_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
                        "Failed to manage Config table (0x%08X)",
                        (unsigned int)iStatus);
      goto FAC_AcquireConfigPointers_Exit_Tag;
   }
   else
   {
#if 0
      CFE_EVS_SendEvent(FAC_INF_EID, CFE_EVS_INFORMATION,
                        "Succeeded to manage Config Table (0x%08X)",
                        (unsigned int)iStatus);
#endif
   }

   /*
   ** Get a pointer to the table
   */
   iStatus = CFE_TBL_GetAddress((void**)&FAC_CdsTaskData.ConfigTblPtr,
                                FAC_CdsTaskData.ConfigTblHdl);
   if (iStatus == CFE_SUCCESS)
   {
#if 0
      CFE_EVS_SendEvent(FAC_INF_EID, CFE_EVS_INFORMATION,
                        "Succeeded to get address @@@@@@@@@ (0x%08X)",
                        (unsigned int)iStatus);
#endif
   }
   else if (iStatus == CFE_TBL_INFO_UPDATED)
   {
#if 0
      CFE_EVS_SendEvent(FAC_INF_EID, CFE_EVS_INFORMATION,
                        "Succeeded to get Updated add !!!!!!!!!!!!!!! (0x%08X)",
                        (unsigned int)iStatus);
#endif
      FAC_ProcessNewConfigTbl();
      iStatus = CFE_SUCCESS;
   }
   else
   {
      FAC_CdsTaskData.ConfigTblPtr = NULL;
      CFE_EVS_SendEvent(FAC_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
                        "Failed to get Config table's address (0x%08X)",
                        (unsigned int)iStatus);
   }

FAC_AcquireConfigPointers_Exit_Tag:
   return (iStatus);
}
