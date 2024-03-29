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

#ifdef __cplusplus
extern "C" {
#endif

#include "ld_app.h"
#include <math.h>

/************************************************************************
 ** Function Definitions
 *************************************************************************/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Config Table                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 LD::InitConfigTbl()
{
    int32 iStatus = 0;

    /* Register Config table */
    iStatus = CFE_TBL_Register(&ConfigTblHdl,
                               LD_CONFIG_TABLENAME,
                               (sizeof(LD_ConfigTbl_t)),
                               CFE_TBL_OPT_DEFAULT,
                               LD::ValidateConfigTbl);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, a critical table could return another nominal code.  If this table is
         * made critical this logic would have to change. */
        (void) CFE_EVS_SendEvent(LD_CFGTBL_REG_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to register config table (0x%08lX)",
                                 iStatus);
        goto LD_InitConfigTbl_Exit_Tag;
    }

    /* Load Config table file */
    iStatus = CFE_TBL_Load(ConfigTblHdl,
                           CFE_TBL_SRC_FILE,
                           LD_CONFIG_TABLE_FILENAME);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, CFE_SUCCESS is for a successful full table load.  If a partial table
         load is desired then this logic would have to change. */
        (void) CFE_EVS_SendEvent(LD_CFGTBL_LOAD_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to load Config Table (0x%08lX)",
                                 iStatus);
        goto LD_InitConfigTbl_Exit_Tag;
    }

    iStatus = AcquireConfigPointers();

LD_InitConfigTbl_Exit_Tag:
    return (iStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate LD Configuration Table                                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 LD::ValidateConfigTbl(void* ConfigTblPtr)
{
    int32 iStatus = 0;
    LD_ConfigTbl_t* LD_ConfigTblPtr = (LD_ConfigTbl_t*)(ConfigTblPtr);
    char Param[LD_PARAM_NAME_MAX_LEN];

    if (LD_ConfigTblPtr->LD_FFALL_THR < LD_FFALL_THR_MIN ||
        LD_ConfigTblPtr->LD_FFALL_THR > LD_FFALL_THR_MAX)
    {
        snprintf(Param, LD_PARAM_NAME_MAX_LEN, "LD_FFALL_THR");
        iStatus = -1;
        goto LD_ValidateConfigTbl_Exit_Tag;
    }
    
    if (LD_ConfigTblPtr->LD_FFALL_TTRI < LD_FFALL_TTRI_MIN ||
        LD_ConfigTblPtr->LD_FFALL_TTRI > LD_FFALL_TTRI_MAX)
    {
        snprintf(Param, LD_PARAM_NAME_MAX_LEN, "LD_FFALL_TTRI");
        iStatus = -1;
        goto LD_ValidateConfigTbl_Exit_Tag;
    }
    
    if (LD_ConfigTblPtr->LD_MAN_DWNTHR < LD_MAN_DWNTHR_MIN ||
        LD_ConfigTblPtr->LD_MAN_DWNTHR > LD_MAN_DWNTHR_MAX)
    {
        snprintf(Param, LD_PARAM_NAME_MAX_LEN, "LD_MAN_DWNTHR");
        iStatus = -1;
        goto LD_ValidateConfigTbl_Exit_Tag;
    }
    
    if (LD_ConfigTblPtr->LD_ALT_MAX < LD_ALT_MAX_MIN ||
        LD_ConfigTblPtr->LD_ALT_MAX > LD_ALT_MAX_MAX)
    {
        snprintf(Param, LD_PARAM_NAME_MAX_LEN, "LD_ALT_MAX");
        iStatus = -1;
        goto LD_ValidateConfigTbl_Exit_Tag;
    }
    
    if (LD_ConfigTblPtr->LD_LOW_T_THR < LD_LOW_T_THR_MIN ||
        LD_ConfigTblPtr->LD_LOW_T_THR > LD_LOW_T_THR_MAX)
    {
        snprintf(Param, LD_PARAM_NAME_MAX_LEN, "LD_LOW_T_THR");
        iStatus = -1;
        goto LD_ValidateConfigTbl_Exit_Tag;
    }
    
    if (LD_ConfigTblPtr->LD_MAN_MIN_THR < LD_MAN_MIN_THR_MIN ||
        LD_ConfigTblPtr->LD_MAN_MIN_THR > LD_MAN_MIN_THR_MAX)
    {
        snprintf(Param, LD_PARAM_NAME_MAX_LEN, "LD_MAN_MIN_THR");
        iStatus = -1;
        goto LD_ValidateConfigTbl_Exit_Tag;
    }
    
    if (LD_ConfigTblPtr->LD_POS_STK_UP_THRES < LD_POS_STK_UP_THRES_MIN ||
        LD_ConfigTblPtr->LD_POS_STK_UP_THRES > LD_POS_STK_UP_THRES_MAX)
    {
        snprintf(Param, LD_PARAM_NAME_MAX_LEN, "LD_POS_STK_UP_THRES");
        iStatus = -1;
        goto LD_ValidateConfigTbl_Exit_Tag;
    }
    
    if (LD_ConfigTblPtr->LD_POS_STK_DW_THRES < LD_POS_STK_DW_THRES_MIN ||
        LD_ConfigTblPtr->LD_POS_STK_DW_THRES > LD_POS_STK_DW_THRES_MAX)
    {
        snprintf(Param, LD_PARAM_NAME_MAX_LEN, "LD_POS_STK_DW_THRES");
        iStatus = -1;
        goto LD_ValidateConfigTbl_Exit_Tag;
    }
    
    if (LD_ConfigTblPtr->LD_LANDSPEED < LD_LANDSPEED_MIN ||
        LD_ConfigTblPtr->LD_LANDSPEED > LD_LANDSPEED_MAX)
    {
        snprintf(Param, LD_PARAM_NAME_MAX_LEN, "LD_LANDSPEED");
        iStatus = -1;
        goto LD_ValidateConfigTbl_Exit_Tag;
    }
    
    if (LD_ConfigTblPtr->LD_MIN_THR_NO_ALT_TIMEOUT < LD_MIN_THR_NO_ALT_TIMEOUT_MIN ||
        LD_ConfigTblPtr->LD_MIN_THR_NO_ALT_TIMEOUT > LD_MIN_THR_NO_ALT_TIMEOUT_MAX)
    {
        snprintf(Param, LD_PARAM_NAME_MAX_LEN, "LD_MIN_THR_NO_ALT_TIMEOUT");
        iStatus = -1;
        goto LD_ValidateConfigTbl_Exit_Tag;
    }
    
LD_ValidateConfigTbl_Exit_Tag:
    return (iStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Acquire Config Pointers                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 LD::AcquireConfigPointers(void)
{
    int32 iStatus = CFE_SUCCESS;

    /*
     ** Release the table
     */
    (void) CFE_TBL_ReleaseAddress(ConfigTblHdl);

    /*
     ** Manage the table
     */
    iStatus = CFE_TBL_Manage(ConfigTblHdl);
    if ((iStatus != CFE_SUCCESS) && (iStatus != CFE_TBL_INFO_UPDATED))
    {
        (void) CFE_EVS_SendEvent(LD_CFGTBL_MANAGE_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to manage LD Config table (0x%08lX)",
                                 iStatus);
        goto LD_AcquireConfigPointers_Exit_Tag;
    }

    if(iStatus == CFE_TBL_INFO_UPDATED)
    {
        HkTlm.mode = ConfigTblPtr->LD_OP_MODE;
    }

    /*
     ** Get a pointer to the table
     */
    iStatus = CFE_TBL_GetAddress((void**)&ConfigTblPtr, ConfigTblHdl);
    if (iStatus == CFE_TBL_INFO_UPDATED)
    {
        iStatus = CFE_SUCCESS;
    }
    else if(iStatus != CFE_SUCCESS)
    {
        ConfigTblPtr = 0;
        (void) CFE_EVS_SendEvent(LD_CFGTBL_GETADDR_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to get Config table's address (0x%08lX)",
                                 iStatus);
    }

LD_AcquireConfigPointers_Exit_Tag:
    return (iStatus);
}

#ifdef __cplusplus
}
#endif

/************************/
/*  End of File Comment */
/************************/

