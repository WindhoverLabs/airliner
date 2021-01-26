/****************************************************************************
 *
 *   Copyright (c) 2019 Windhover Labs, L.L.C. All rights reserved.
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

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "prm_config_utils.h"

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/************************************************************************
** External Global Variables
*************************************************************************/
extern PRM_AppData_t  PRM_AppData;

/************************************************************************
** Global Variables
*************************************************************************/

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Function Prototypes
*************************************************************************/

/************************************************************************
** Function Definitions
*************************************************************************/


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Config Table                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 PRM_InitConfigTbl()
{
    int32  iStatus=0;

    /* Register Config table */
    iStatus = CFE_TBL_Register(&PRM_AppData.ConfigTblHdl,
                               PRM_CONFIG_TABLENAME,
                               (sizeof(PRM_ConfigTbl_t) * PRM_CONFIG_TABLE_MAX_ENTRIES),
                               CFE_TBL_OPT_DEFAULT,
                               PRM_ValidateConfigTbl);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, a critical table could return another nominal code.  If this table is
         * made critical this logic would have to change. */
        (void) CFE_EVS_SendEvent(PRM_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to register Config table (0x%08X)",
                                 (unsigned int)iStatus);
        goto PRM_InitConfigTbl_Exit_Tag;
    }

    /* Load Config table file */
    iStatus = CFE_TBL_Load(PRM_AppData.ConfigTblHdl,
                           CFE_TBL_SRC_FILE,
                           PRM_CONFIG_TABLE_FILENAME);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, CFE_SUCCESS is for a successful full table load.  If a partial table
           load is desired then this logic would have to change. */
        (void) CFE_EVS_SendEvent(PRM_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to load Config Table (0x%08X)",
                                 (unsigned int)iStatus);
        goto PRM_InitConfigTbl_Exit_Tag;
    }

    iStatus = PRM_AcquireConfigPointers();

PRM_InitConfigTbl_Exit_Tag:
    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate Config Table                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 PRM_ValidateConfigTbl(void* ConfigTblPtr)
{
    int32  iStatus=0;
    PRM_ConfigTbl_t* PRM_ConfigTblPtr = (PRM_ConfigTbl_t*)(ConfigTblPtr);

    if (ConfigTblPtr == 0)
    {
        iStatus = -1;
        goto PRM_ValidateConfigTbl_Exit_Tag;
    }

    /* TODO:  Add code to validate new data values here.
    **
    ** Examples:
    ** if (PRM_ConfigTblPtr->iParam <= 16) {
    **   (void) CFE_EVS_SendEvent(PRM_CONFIG_TABLE_INF_EID, CFE_EVS_ERROR,
     *                         "Invalid value for Config parameter sParam (%d)",
    **                         PRM_ConfigTblPtr->iParam);
    ** }
    **/

PRM_ValidateConfigTbl_Exit_Tag:
    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process New Config Table                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void PRM_ProcessNewConfigTbl()
{
    /* TODO:  Add code to set new Config parameters with new values here.
    **
    ** Examples:
    **
    **    PRM_AppData.latest_sParam = PRM_AppData.ConfigTblPtr->sParam;
    **    PRM_AppData.latest_fParam = PRM.AppData.ConfigTblPtr->fParam;
    */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Acquire Conifg Pointers                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PRM_AcquireConfigPointers(void)
{
    int32 iStatus = CFE_SUCCESS;

    /*
    ** Release the table
    */
    /* TODO: This return value can indicate success, error, or that the info has been 
     * updated.  We ignore this return value in favor of checking CFE_TBL_Manage(), but
     * be sure this is the behavior you want. */
    (void) CFE_TBL_ReleaseAddress(PRM_AppData.ConfigTblHdl);

    /*
    ** Manage the table
    */
    iStatus = CFE_TBL_Manage(PRM_AppData.ConfigTblHdl);
    if ((iStatus != CFE_SUCCESS) && (iStatus != CFE_TBL_INFO_UPDATED))
    {
        (void) CFE_EVS_SendEvent(PRM_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to manage Config table (0x%08X)",
                                 (unsigned int)iStatus);
        goto PRM_AcquireConfigPointers_Exit_Tag;
    }

    /*
    ** Get a pointer to the table
    */
    iStatus = CFE_TBL_GetAddress((void*)&PRM_AppData.ConfigTblPtr,
                                 PRM_AppData.ConfigTblHdl);
    if (iStatus == CFE_TBL_INFO_UPDATED)
    {
        PRM_ProcessNewConfigTbl();
        iStatus = CFE_SUCCESS;
    }
    else if(iStatus != CFE_SUCCESS)
    {
	PRM_AppData.ConfigTblPtr = 0;
        (void) CFE_EVS_SendEvent(PRM_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to get Config table's address (0x%08X)",
                                 (unsigned int)iStatus);
    }

PRM_AcquireConfigPointers_Exit_Tag:
    return (iStatus);

} /* End of PRM_AcquirePointers */

/************************/
/*  End of File Comment */
/************************/
    
