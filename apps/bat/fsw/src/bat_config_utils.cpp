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

#include "bat_app.h"
#include <math.h>

/************************************************************************
** Function Definitions
*************************************************************************/


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Config Table                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 BAT::InitConfigTbl()
{
    int32  iStatus=0;

    /* Register Config table */
    iStatus = CFE_TBL_Register(&ConfigTblHdl,
                               BAT_CONFIG_TABLENAME,
                               (sizeof(BAT_ConfigTbl_t)),
                               CFE_TBL_OPT_DEFAULT,
                               BAT::ValidateConfigTbl);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, a critical table could return another nominal code.  If this table is
         * made critical this logic would have to change. */
        (void) CFE_EVS_SendEvent(BAT_CFGTBL_REG_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to register config table (0x%08lX)",
                                 iStatus);
        goto BAT_InitConfigTbl_Exit_Tag;
    }

    /* Load Config table file */
    iStatus = CFE_TBL_Load(ConfigTblHdl,
                           CFE_TBL_SRC_FILE,
                           BAT_CONFIG_TABLE_FILENAME);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, CFE_SUCCESS is for a successful full table load.  If a partial table
           load is desired then this logic would have to change. */
        (void) CFE_EVS_SendEvent(BAT_CFGTBL_LOAD_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to load Config Table (0x%08lX)",
                                 iStatus);
        goto BAT_InitConfigTbl_Exit_Tag;
    }

    iStatus = AcquireConfigPointers();

BAT_InitConfigTbl_Exit_Tag:
    return iStatus;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate BAT Configuration Table                                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 BAT::ValidateConfigTbl(void* ConfigTblPtr)
{
    int32  iStatus = CFE_SUCCESS;
    BAT_ConfigTbl_t* BAT_ConfigTblPtr = (BAT_ConfigTbl_t*)(ConfigTblPtr);

    if (BAT_ConfigTblPtr->VEmpty < BAT_V_EMPTY_MIN ||
        BAT_ConfigTblPtr->VEmpty > BAT_V_EMPTY_MAX)
    {
        iStatus = -1;
        goto BAT_ValidateConfigTbl_Exit_Tag;
    }
    
    if (BAT_ConfigTblPtr->VFull < BAT_V_CHARGED_MIN ||
        BAT_ConfigTblPtr->VFull > BAT_V_CHARGED_MAX)
    {
        iStatus = -1;
        goto BAT_ValidateConfigTbl_Exit_Tag;
    }
    
    if (BAT_ConfigTblPtr->NumCells < BAT_N_CELLS_MIN ||
        BAT_ConfigTblPtr->NumCells > BAT_N_CELLS_MAX)
    {
        iStatus = -1;
        goto BAT_ValidateConfigTbl_Exit_Tag;
    }
    
    if (BAT_ConfigTblPtr->Capacity < BAT_CAPACITY_MIN ||
        BAT_ConfigTblPtr->Capacity > BAT_CAPACITY_MAX)
    {
        iStatus = -1;
        goto BAT_ValidateConfigTbl_Exit_Tag;
    }
    
    if (BAT_ConfigTblPtr->LowThreshold < BAT_LOW_THR_MIN ||
        BAT_ConfigTblPtr->LowThreshold > BAT_LOW_THR_MAX)
    {
        iStatus = -1;
        goto BAT_ValidateConfigTbl_Exit_Tag;
    }
    
    if (BAT_ConfigTblPtr->CriticalThreshold < BAT_CRIT_THR_MIN ||
        BAT_ConfigTblPtr->CriticalThreshold > BAT_CRIT_THR_MAX)
    {
        iStatus = -1;
        goto BAT_ValidateConfigTbl_Exit_Tag;
    }
    
    if (BAT_ConfigTblPtr->EmergencyThreshold < BAT_EMERGEN_THR_MIN ||
        BAT_ConfigTblPtr->EmergencyThreshold > BAT_EMERGEN_THR_MAX)
    {
        iStatus = -1;
        goto BAT_ValidateConfigTbl_Exit_Tag;
    }
    
    if (BAT_ConfigTblPtr->VLoadDrop < BAT_V_LOAD_DROP_MIN ||
        BAT_ConfigTblPtr->VLoadDrop > BAT_V_LOAD_DROP_MAX)
    {
        iStatus = -1;
        goto BAT_ValidateConfigTbl_Exit_Tag;
    }
    
    if (BAT_ConfigTblPtr->RInternal < BAT_R_INTERNAL_MIN ||
        BAT_ConfigTblPtr->RInternal > BAT_R_INTERNAL_MAX)
    {
        iStatus = -1;
        goto BAT_ValidateConfigTbl_Exit_Tag;
    }
    
BAT_ValidateConfigTbl_Exit_Tag:
    return iStatus;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Acquire Config Pointers                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 BAT::AcquireConfigPointers(void)
{
    int32 iStatus = CFE_SUCCESS;

    OS_MutSemTake(BatteryMutex);

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
        (void) CFE_EVS_SendEvent(BAT_CFGTBL_MANAGE_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to manage BAT Config table (0x%08lX)",
                                 iStatus);
        goto BAT_AcquireConfigPointers_Exit_Tag;
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
        (void) CFE_EVS_SendEvent(BAT_CFGTBL_GETADDR_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to get Config table's address (0x%08lX)",
                                 iStatus);
    }

BAT_AcquireConfigPointers_Exit_Tag:
    OS_MutSemGive(BatteryMutex);
    return iStatus;
}


#ifdef __cplusplus
}
#endif

/************************/
/*  End of File Comment */
/************************/
    
