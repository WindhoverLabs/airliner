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

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "simlink_config_utils.h"

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/************************************************************************
** External Global Variables
*************************************************************************/
extern SIMLINK_AppData_t  SIMLINK_AppData;

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

int32 SIMLINK_InitConfigTbl()
{
    int32  iStatus=0;

    /* Register Config table */
    iStatus = CFE_TBL_Register(&SIMLINK_AppData.ConfigTblHdl,
                               SIMLINK_CONFIG_TABLENAME,
                               (sizeof(SIMLINK_ConfigTbl_t) * SIMLINK_CONFIG_TABLE_MAX_ENTRIES),
                               CFE_TBL_OPT_DEFAULT,
                               SIMLINK_ValidateConfigTbl);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, a critical table could return another nominal code.  If this table is
         * made critical this logic would have to change. */
        (void) CFE_EVS_SendEvent(SIMLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to register Config table (0x%08X)",
                                 (unsigned int)iStatus);
        goto SIMLINK_InitConfigTbl_Exit_Tag;
    }

    /* Load Config table file */
    iStatus = CFE_TBL_Load(SIMLINK_AppData.ConfigTblHdl,
                           CFE_TBL_SRC_FILE,
                           SIMLINK_CONFIG_TABLE_FILENAME);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, CFE_SUCCESS is for a successful full table load.  If a partial table
           load is desired then this logic would have to change. */
        (void) CFE_EVS_SendEvent(SIMLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to load Config Table (0x%08X)",
                                 (unsigned int)iStatus);
        goto SIMLINK_InitConfigTbl_Exit_Tag;
    }

    iStatus = SIMLINK_AcquireConfigPointers();

SIMLINK_InitConfigTbl_Exit_Tag:
    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate Config Table                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 SIMLINK_ValidateConfigTbl(void* ConfigTblPtr)
{
    int32  iStatus=0;
    SIMLINK_ConfigTbl_t* SIMLINK_ConfigTblPtr = (SIMLINK_ConfigTbl_t*)(ConfigTblPtr);

    if (ConfigTblPtr == NULL)
    {
        iStatus = -1;
        goto SIMLINK_ValidateConfigTbl_Exit_Tag;
    }

    /* TODO:  Add code to validate new data values here.
    **
    ** Examples:
    ** if (SIMLINK_ConfigTblPtr->iParam <= 16) {
    **   (void) CFE_EVS_SendEvent(SIMLINK_CONFIG_TABLE_INF_EID, CFE_EVS_ERROR,
     *                         "Invalid value for Config parameter sParam (%d)",
    **                         SIMLINK_ConfigTblPtr->iParam);
    ** }
    **/

SIMLINK_ValidateConfigTbl_Exit_Tag:
    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process New Config Table                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SIMLINK_ProcessNewConfigTbl()
{
    /* TODO:  Add code to set new Config parameters with new values here.
    **
    ** Examples:
    **
    **    SIMLINK_AppData.latest_sParam = SIMLINK_AppData.ConfigTblPtr->sParam;
    **    SIMLINK_AppData.latest_fParam = SIMLINK.AppData.ConfigTblPtr->fParam;
    */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Acquire Conifg Pointers                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 SIMLINK_AcquireConfigPointers(void)
{
    int32 iStatus = CFE_SUCCESS;

    /*
    ** Release the table
    */
    /* TODO: This return value can indicate success, error, or that the info has been 
     * updated.  We ignore this return value in favor of checking CFE_TBL_Manage(), but
     * be sure this is the behavior you want. */
    (void) CFE_TBL_ReleaseAddress(SIMLINK_AppData.ConfigTblHdl);

    /*
    ** Manage the table
    */
    iStatus = CFE_TBL_Manage(SIMLINK_AppData.ConfigTblHdl);
    if ((iStatus != CFE_SUCCESS) && (iStatus != CFE_TBL_INFO_UPDATED))
    {
        (void) CFE_EVS_SendEvent(SIMLINK_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to manage Config table (0x%08X)",
                                 (unsigned int)iStatus);
        goto SIMLINK_AcquireConfigPointers_Exit_Tag;
    }

    /*
    ** Get a pointer to the table
    */
    iStatus = CFE_TBL_GetAddress((void*)&SIMLINK_AppData.ConfigTblPtr,
                                 SIMLINK_AppData.ConfigTblHdl);
    if (iStatus == CFE_TBL_INFO_UPDATED)
    {
        SIMLINK_ProcessNewConfigTbl();
        iStatus = CFE_SUCCESS;
    }
    else if(iStatus != CFE_SUCCESS)
    {
    	exit(-1);
	    SIMLINK_AppData.ConfigTblPtr = 0;
        (void) CFE_EVS_SendEvent(SIMLINK_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to get Config table's address (0x%08X)",
                                 (unsigned int)iStatus);
    }

SIMLINK_AcquireConfigPointers_Exit_Tag:
    return (iStatus);

} /* End of SIMLINK_AcquirePointers */

/************************/
/*  End of File Comment */
/************************/
    
