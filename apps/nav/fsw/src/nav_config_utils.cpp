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
extern "C"
{
#endif

#include "nav_app.h"
#include <math.h>

/************************************************************************
 ** Function Definitions
 *************************************************************************/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Config Table                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 NAV::InitConfigTbl()
{
    int32 iStatus = CFE_SUCCESS;

    /* Register Config table */
    iStatus = CFE_TBL_Register(&ConfigTblHdl,
                               NAV_CONFIG_TABLENAME,
                               (sizeof(NAV_ConfigTbl_t)),
                               CFE_TBL_OPT_DEFAULT,
                               NAV::ValidateConfigTbl);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, a critical table could return another nominal code.  If this table is
         * made critical this logic would have to change. */
        (void) CFE_EVS_SendEvent(NAV_CFGTBL_REG_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to register config table (0x%08lX)",
                                 iStatus);
        goto NAV_InitConfigTbl_Exit_Tag;
    }

    /* Load Config table file */
    iStatus = CFE_TBL_Load(ConfigTblHdl,
                           CFE_TBL_SRC_FILE,
                           NAV_CONFIG_TABLE_FILENAME);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, CFE_SUCCESS is for a successful full table load.  If a partial table
         load is desired then this logic would have to change. */
        (void) CFE_EVS_SendEvent(NAV_CFGTBL_LOAD_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to load Config Table (0x%08lX)",
                                 iStatus);
        goto NAV_InitConfigTbl_Exit_Tag;
    }

    iStatus = AcquireConfigPointers();

NAV_InitConfigTbl_Exit_Tag:
    return (iStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate NAV Configuration Table                                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 NAV::ValidateConfigTbl(void* ConfigTblPtr)
{
    int32 iStatus = CFE_SUCCESS;
    NAV_ConfigTbl_t* NAV_ConfigTblPtr = (NAV_ConfigTbl_t*)(ConfigTblPtr);

    if (NAV_ConfigTblPtr->NAV_ACC_RAD < NAV_ACC_RAD_MIN || 
        NAV_ConfigTblPtr->NAV_ACC_RAD > NAV_ACC_RAD_MAX)
    {
        iStatus = -1;
        goto NAV_ValidateConfigTbl_Exit_Tag;
    }
    
    if (NAV_ConfigTblPtr->NAV_ALT_RAD < NAV_ALT_RAD_MIN || 
        NAV_ConfigTblPtr->NAV_ALT_RAD > NAV_ALT_RAD_MAX)
    {
        iStatus = -1;
        goto NAV_ValidateConfigTbl_Exit_Tag;
    }

    if (NAV_ConfigTblPtr->NAV_LOITER_RAD < NAV_LOITER_RAD_MIN || 
        NAV_ConfigTblPtr->NAV_LOITER_RAD > NAV_LOITER_RAD_MAX)
    {
        iStatus = -1;
        goto NAV_ValidateConfigTbl_Exit_Tag;
    }
    
    if (NAV_ConfigTblPtr->NAV_MIS_TAKEOFF_ALT < NAV_MIS_TAKEOFF_ALT_MIN || 
        NAV_ConfigTblPtr->NAV_MIS_TAKEOFF_ALT > NAV_MIS_TAKEOFF_ALT_MAX)
    {
        iStatus = -1;
        goto NAV_ValidateConfigTbl_Exit_Tag;
    }
    
    if (NAV_ConfigTblPtr->NAV_MIS_YAW_ERR < NAV_MIS_YAW_ERR_MIN || 
        NAV_ConfigTblPtr->NAV_MIS_YAW_ERR > NAV_MIS_YAW_ERR_MAX)
    {
        iStatus = -1;
        goto NAV_ValidateConfigTbl_Exit_Tag;
    }
    
    if (NAV_ConfigTblPtr->NAV_MIS_YAW_TMT < NAV_MIS_YAW_TMT_MIN || 
        NAV_ConfigTblPtr->NAV_MIS_YAW_TMT > NAV_MIS_YAW_TMT_MAX)
    {
        iStatus = -1;
        goto NAV_ValidateConfigTbl_Exit_Tag;
    }
    
    if (NAV_ConfigTblPtr->NAV_MIS_LTRMIN_ALT < NAV_MIS_LTRMIN_ALT_MIN || 
        NAV_ConfigTblPtr->NAV_MIS_LTRMIN_ALT > NAV_MIS_LTRMIN_ALT_MAX)
    {
        iStatus = -1;
        goto NAV_ValidateConfigTbl_Exit_Tag;
    }
    
    if (NAV_ConfigTblPtr->NAV_RTL_RETURN_ALT < NAV_RTL_RETURN_ALT_MIN || 
        NAV_ConfigTblPtr->NAV_RTL_RETURN_ALT > NAV_RTL_RETURN_ALT_MAX)
    {
        iStatus = -1;
        goto NAV_ValidateConfigTbl_Exit_Tag;
    }
    
    if (NAV_ConfigTblPtr->NAV_RTL_DESCEND_ALT < NAV_RTL_DESCEND_ALT_MIN || 
        NAV_ConfigTblPtr->NAV_RTL_DESCEND_ALT > NAV_RTL_DESCEND_ALT_MAX)
    {
        iStatus = -1;
        goto NAV_ValidateConfigTbl_Exit_Tag;
    }
    
    if (NAV_ConfigTblPtr->NAV_RTL_LAND_DELAY < NAV_RTL_LAND_DELAY_MIN || 
        NAV_ConfigTblPtr->NAV_RTL_LAND_DELAY > NAV_RTL_LAND_DELAY_MAX)
    {
        iStatus = -1;
        goto NAV_ValidateConfigTbl_Exit_Tag;
    }
    
    if (NAV_ConfigTblPtr->NAV_RTL_MIN_DIST < NAV_RTL_MIN_DIST_MIN || 
        NAV_ConfigTblPtr->NAV_RTL_MIN_DIST > NAV_RTL_MIN_DIST_MAX)
    {
        iStatus = -1;
        goto NAV_ValidateConfigTbl_Exit_Tag;
    }

NAV_ValidateConfigTbl_Exit_Tag:
    return iStatus;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Acquire Config Pointers                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 NAV::AcquireConfigPointers(void)
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
        (void) CFE_EVS_SendEvent(NAV_CFGTBL_MANAGE_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to manage NAV Config table (0x%08lX)",
                                 iStatus);
        goto NAV_AcquireConfigPointers_Exit_Tag;
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
        (void) CFE_EVS_SendEvent(NAV_CFGTBL_GETADDR_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to get Config table's address (0x%08lX)",
                                 iStatus);
    }

NAV_AcquireConfigPointers_Exit_Tag:
    return iStatus;
}

#ifdef __cplusplus
}
#endif

/************************/
/*  End of File Comment */
/************************/

