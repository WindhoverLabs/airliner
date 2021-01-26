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

#include "flow_app.h"
#include <math.h>

/************************************************************************
** Function Definitions
*************************************************************************/


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Config Table                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 FLOW::InitConfigTbl()
{
    int32  iStatus=0;

    /* Register Config table */
    iStatus = CFE_TBL_Register(&ConfigTblHdl,
		FLOW_CONFIG_TABLENAME,
		(sizeof(FLOW_ConfigTbl_t)),
		CFE_TBL_OPT_DEFAULT,
		FLOW::ValidateConfigTbl);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, a critical table could return another nominal code.  If this table is
         * made critical this logic would have to change. */
        (void) CFE_EVS_SendEvent(FLOW_CFGTBL_REG_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to register config table (0x%08lX)",
                                 iStatus);
        goto FLOW_InitConfigTbl_Exit_Tag;
    }

    /* Load Config table file */
    iStatus = CFE_TBL_Load(ConfigTblHdl,
                           CFE_TBL_SRC_FILE,
						   FLOW_CONFIG_TABLE_FILENAME);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, CFE_SUCCESS is for a successful full table load.  If a partial table
           load is desired then this logic would have to change. */
        (void) CFE_EVS_SendEvent(FLOW_CFGTBL_LOAD_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to load Config Table (0x%08lX)",
                                 iStatus);
        goto FLOW_InitConfigTbl_Exit_Tag;
    }

    iStatus = AcquireConfigPointers();

FLOW_InitConfigTbl_Exit_Tag:
    return iStatus;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate FLOW Configuration Table                                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 FLOW::ValidateConfigTbl(void* ConfigTblPtr)
{
    int32  iStatus=0;
    FLOW_ConfigTbl_t* FLOW_ConfigTblPtr = (FLOW_ConfigTbl_t*)(ConfigTblPtr);

    /* HFOV */
    if(FLOW_ConfigTblPtr->HFOV < FLOW_HOV_MIN
    		|| FLOW_ConfigTblPtr->HFOV >FLOW_HOV_MAX
    		|| isnan(FLOW_ConfigTblPtr->HFOV)
			|| !isfinite(FLOW_ConfigTblPtr->HFOV))
    {
    	iStatus = -1;
    	(void) CFE_EVS_SendEvent(FLOW_CFGTBL_MANAGE_ERR_EID, CFE_EVS_ERROR,
    			"Failed to validate HFOV in FLOW Config table (0x%08lX). Must be between %u and %lf",
				iStatus, FLOW_HOV_MIN, FLOW_HOV_MAX);
    	goto FLOW_ValidateConfigTbl_Exit_Tag;

    }

    /* FLOW_TILE_SIZE */
    if(FLOW_ConfigTblPtr->FLOW_TILE_SIZE < FLOW_TILE_SIZE_MIN
    		|| FLOW_ConfigTblPtr->FLOW_TILE_SIZE > FLOW_TILE_SIZE_MAX)
    {
    	iStatus = -1;
    	(void) CFE_EVS_SendEvent(FLOW_CFGTBL_MANAGE_ERR_EID, CFE_EVS_ERROR,
    			"Failed to validate FLOW_TILE_SIZE in FLOW Config table (0x%08lX). Must be between %u and %u",
				iStatus, FLOW_TILE_SIZE_MIN, FLOW_TILE_SIZE_MAX);
    	goto FLOW_ValidateConfigTbl_Exit_Tag;

    }

    /* FLOW_NUM_BLOCKS */
    if(FLOW_ConfigTblPtr->FLOW_NUM_BLOCKS < FLOW_BLOCKS_MIN
    		|| FLOW_ConfigTblPtr->FLOW_NUM_BLOCKS > FLOW_BLOCKS_MAX)
    {
    	iStatus = -1;
    	(void) CFE_EVS_SendEvent(FLOW_CFGTBL_MANAGE_ERR_EID, CFE_EVS_ERROR,
    			"Failed to validate FLOW_NUM_BLOCKS in FLOW Config table (0x%08lX). Must be between %u and %u",
				iStatus, FLOW_BLOCKS_MIN, FLOW_BLOCKS_MAX);
    	goto FLOW_ValidateConfigTbl_Exit_Tag;

    }

    /* FLOW_DEFAULT_OUTPUT_RATE */
    if(FLOW_ConfigTblPtr->FLOW_DEFAULT_OUTPUT_RATE < FLOW_OUTPUT_RATE_MIN
    		|| FLOW_ConfigTblPtr->FLOW_DEFAULT_OUTPUT_RATE > FLOW_OUTPUT_RATE_MAX)
    {
    	iStatus = -1;
    	(void) CFE_EVS_SendEvent(FLOW_CFGTBL_MANAGE_ERR_EID, CFE_EVS_ERROR,
    			"Failed to validate FLOW_DEFAULT_OUTPUT_RATE in FLOW Config table (0x%08lX). Must be between %u and %u",
				iStatus, FLOW_OUTPUT_RATE_MIN, FLOW_OUTPUT_RATE_MAX);
    	goto FLOW_ValidateConfigTbl_Exit_Tag;

    }

    /* FLOW_DEFAULT_IMAGE_WIDTH */
    if(FLOW_ConfigTblPtr->FLOW_DEFAULT_IMAGE_WIDTH < FLOW_IMAGE_WIDTH_MIN
    		|| FLOW_ConfigTblPtr->FLOW_DEFAULT_IMAGE_WIDTH > FLOW_IMAGE_WIDTH_MAX)
    {
    	iStatus = -1;
    	(void) CFE_EVS_SendEvent(FLOW_CFGTBL_MANAGE_ERR_EID, CFE_EVS_ERROR,
    			"Failed to validate FLOW_DEFAULT_IMAGE_WIDTH in FLOW Config table (0x%08lX). Must be between %u and %u",
				iStatus, FLOW_IMAGE_WIDTH_MIN, FLOW_IMAGE_WIDTH_MAX);
    	goto FLOW_ValidateConfigTbl_Exit_Tag;

    }

    /* FLOW_DEFAULT_IMAGE_HEIGHT */
    if(FLOW_ConfigTblPtr->FLOW_DEFAULT_IMAGE_HEIGHT < FLOW_IMAGE_HEIGHT_MIN
    		|| FLOW_ConfigTblPtr->FLOW_DEFAULT_IMAGE_HEIGHT > FLOW_IMAGE_HEIGHT_MAX)
    {
    	iStatus = -1;
    	(void) CFE_EVS_SendEvent(FLOW_CFGTBL_MANAGE_ERR_EID, CFE_EVS_ERROR,
    			"Failed to validate FLOW_DEFAULT_IMAGE_HEIGHT in FLOW Config table (0x%08lX). Must be between %u and %u",
				iStatus, FLOW_IMAGE_HEIGHT_MIN, FLOW_IMAGE_HEIGHT_MAX);
    	goto FLOW_ValidateConfigTbl_Exit_Tag;

    }

    /* FLOW_DEFAULT_SEARCH_SIZE */
    if(FLOW_ConfigTblPtr->FLOW_DEFAULT_SEARCH_SIZE < FLOW_SEARCH_SIZE_MIN
    		|| FLOW_ConfigTblPtr->FLOW_DEFAULT_SEARCH_SIZE > FLOW_SEARCH_SIZE_MAX)
    {
    	iStatus = -1;
    	(void) CFE_EVS_SendEvent(FLOW_CFGTBL_MANAGE_ERR_EID, CFE_EVS_ERROR,
    			"Failed to validate FLOW_DEFAULT_SEARCH_SIZE in FLOW Config table (0x%08lX). Must be between %u and %u",
				iStatus, FLOW_SEARCH_SIZE_MIN, FLOW_SEARCH_SIZE_MAX);
    	goto FLOW_ValidateConfigTbl_Exit_Tag;

    }

    /* FLOW_DEFAULT_FEATURE_THRESHOLD */
    if(FLOW_ConfigTblPtr->FLOW_DEFAULT_FEATURE_THRESHOLD < FLOW_FEATURE_THRESHOLD_MIN
    		|| FLOW_ConfigTblPtr->FLOW_DEFAULT_FEATURE_THRESHOLD > FLOW_FEATURE_THRESHOLD_MAX)
    {
    	iStatus = -1;
    	(void) CFE_EVS_SendEvent(FLOW_CFGTBL_MANAGE_ERR_EID, CFE_EVS_ERROR,
    			"Failed to validate FLOW_DEFAULT_FEATURE_THRESHOLD in FLOW Config table (0x%08lX). Must be between %u and %u",
				iStatus, FLOW_FEATURE_THRESHOLD_MIN, FLOW_FEATURE_THRESHOLD_MAX);
    	goto FLOW_ValidateConfigTbl_Exit_Tag;

    }

    /* FLOW_DEFAULT_VALUE_THRESHOLD */
    if(FLOW_ConfigTblPtr->FLOW_DEFAULT_VALUE_THRESHOLD < FLOW_VALUE_THRESHOLD_MIN
    		|| FLOW_ConfigTblPtr->FLOW_DEFAULT_VALUE_THRESHOLD > FLOW_VALUE_THRESHOLD_MAX)
    {
    	iStatus = -1;
    	(void) CFE_EVS_SendEvent(FLOW_CFGTBL_MANAGE_ERR_EID, CFE_EVS_ERROR,
    			"Failed to validate FLOW_DEFAULT_VALUE_THRESHOLD in FLOW Config table (0x%08lX). Must be between %u and %u",
				iStatus, FLOW_VALUE_THRESHOLD_MIN, FLOW_VALUE_THRESHOLD_MAX);
    	goto FLOW_ValidateConfigTbl_Exit_Tag;

    }

    /* FLOW_MEAN_COUNT_THRESHOLD */
    if(FLOW_ConfigTblPtr->FLOW_MEAN_COUNT_THRESHOLD < FLOW_MEAN_COUNT_THRESHOLD_MIN
    		|| FLOW_ConfigTblPtr->FLOW_MEAN_COUNT_THRESHOLD > FLOW_MEAN_COUNT_THRESHOLD_MAX)
    {
    	iStatus = -1;
    	(void) CFE_EVS_SendEvent(FLOW_CFGTBL_MANAGE_ERR_EID, CFE_EVS_ERROR,
    			"Failed to validate FLOW_MEAN_COUNT_THRESHOLD in FLOW Config table (0x%08lX). Must be between %u and %u",
				iStatus, FLOW_MEAN_COUNT_THRESHOLD_MIN, FLOW_MEAN_COUNT_THRESHOLD_MAX);
    	goto FLOW_ValidateConfigTbl_Exit_Tag;

    }


FLOW_ValidateConfigTbl_Exit_Tag:
    return iStatus;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Acquire Config Pointers                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 FLOW::AcquireConfigPointers(void)
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
        (void) CFE_EVS_SendEvent(FLOW_CFGTBL_MANAGE_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to manage FLOW Config table (0x%08lX)",
                                 iStatus);
        goto FLOW_AcquireConfigPointers_Exit_Tag;
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
        (void) CFE_EVS_SendEvent(FLOW_CFGTBL_GETADDR_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to get Config table's address (0x%08lX)",
                                 iStatus);
    }

FLOW_AcquireConfigPointers_Exit_Tag:
    return iStatus;
}


#ifdef __cplusplus
}
#endif

/************************/
/*  End of File Comment */
/************************/
    
