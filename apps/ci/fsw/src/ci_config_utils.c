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
#include "ci_config_utils.h"
#include "ci_app.h"

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/************************************************************************
** External Global Variables
*************************************************************************/
extern CI_AppData_t  CI_AppData;

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
int32 CI_InitTbls(void)
{
    int32  iStatus=0;

    /* Register Config table */
    iStatus = CFE_TBL_Register(&CI_AppData.ConfigTblHdl,
                               CI_CONFIG_TABLENAME,
                               sizeof(CI_ConfigTblEntry_t),
                               CFE_TBL_OPT_DEFAULT,
                               CI_ValidateConfigTbl);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(CI_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to register Config table (0x%08X)",
                                 (unsigned int)iStatus);
        goto CI_InitTbls_Exit_Tag;
    }

    /* Register Timeout dump table */
	iStatus = CFE_TBL_Register(&CI_AppData.TimeoutTblHdl,
							   CI_TIMEOUT_TABLENAME,
							   sizeof(CI_TimeoutTblEntry_t),
							   CFE_TBL_OPT_USR_DEF_ADDR,
							   0);
	if (iStatus != CFE_SUCCESS)
	{
		(void) CFE_EVS_SendEvent(CI_INIT_ERR_EID, CFE_EVS_ERROR,
								 "Failed to register Timeout table (0x%08X)",
								 (unsigned int)iStatus);
        goto CI_InitTbls_Exit_Tag;
	}

    /* Load Config table file */
    iStatus = CFE_TBL_Load(CI_AppData.ConfigTblHdl,
                           CFE_TBL_SRC_FILE,
                           CI_CONFIG_TABLE_FILENAME);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(CI_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to load Config Table (0x%08X)",
                                 (unsigned int)iStatus);
        goto CI_InitTbls_Exit_Tag;
    }

    /* Load Timeout table file */
	iStatus = CFE_TBL_Load(CI_AppData.TimeoutTblHdl,
						   CFE_TBL_SRC_ADDRESS,
						   &CI_AppData.TimeoutTbl);
	if (iStatus != CFE_SUCCESS)
	{
		(void) CFE_EVS_SendEvent(CI_INIT_ERR_EID, CFE_EVS_ERROR,
								 "Failed to load Timeout Table (0x%08X)",
								 (unsigned int)iStatus);
        goto CI_InitTbls_Exit_Tag;
	}

    iStatus = CI_AcquireConfigPointers();

CI_InitTbls_Exit_Tag:
    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate Config Table                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 CI_ValidateConfigTbl(void* ConfigTblPtr)
{
    int32  iStatus = 0;
    uint32 i = 0;
    CI_ConfigTblEntry_t* CI_ConfigTblPtr = (CI_ConfigTblEntry_t*)(ConfigTblPtr);

    if (ConfigTblPtr == NULL)
    {
        iStatus = -1;
        goto CI_ValidateConfigTbl_Exit_Tag;
    }

    /* Perform simple validation on registered command states */
    for(i = 0; i < CI_MAX_RGST_CMDS; ++i)
    {
        /* If message is non-zero perform validation. Skip otherwise. */
        if(CI_ConfigTblPtr->cmds[i].mid != 0)
        {
            /* Check if stepping is valid */
            if(CI_ConfigTblPtr->cmds[i].step != STEP_1 && 
               CI_ConfigTblPtr->cmds[i].step != STEP_2)
            {
                iStatus = -1;
                break;
            }

            /* Check if state is valid */
            if(CI_ConfigTblPtr->cmds[i].state != UNAUTHORIZED && 
               CI_ConfigTblPtr->cmds[i].state != AUTHORIZED)
            {
                iStatus = -1;
                break;
            }
        }
    }
    
CI_ValidateConfigTbl_Exit_Tag:
    return (iStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Acquire Conifg Pointers                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 CI_AcquireConfigPointers(void)
{
	 int32 iStatus = CFE_SUCCESS;

	/*
	** Release the table
	*/
	(void) CFE_TBL_ReleaseAddress(CI_AppData.ConfigTblHdl);
	(void) CFE_TBL_ReleaseAddress(CI_AppData.TimeoutTblHdl);

	/*
	** Manage the tables
	*/
	iStatus = CFE_TBL_Manage(CI_AppData.ConfigTblHdl);
	if ((iStatus != CFE_SUCCESS) && (iStatus != CFE_TBL_INFO_UPDATED))
	{
		(void) CFE_EVS_SendEvent(CI_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
								 "Failed to manage Config table (0x%08X)",
								 (unsigned int)iStatus);
		goto CI_AcquireConfigPointers_Exit_Tag;
	}

	iStatus = CFE_TBL_Manage(CI_AppData.TimeoutTblHdl);
	if ((iStatus != CFE_SUCCESS) && (iStatus != CFE_TBL_INFO_UPDATED))
	{
		(void) CFE_EVS_SendEvent(CI_TIMEOUT_TABLE_ERR_EID, CFE_EVS_ERROR,
								 "Failed to manage Timeout table (0x%08X)",
								 (unsigned int)iStatus);
		goto CI_AcquireConfigPointers_Exit_Tag;
	}

	/*
	** Get a pointer to the tables
	*/
	iStatus = CFE_TBL_GetAddress((void*)&CI_AppData.ConfigTblPtr,
								 CI_AppData.ConfigTblHdl);
	if (iStatus == CFE_TBL_INFO_UPDATED)
	{
		iStatus = CFE_SUCCESS;
	}
	else if(iStatus != CFE_SUCCESS)
	{
		CI_AppData.ConfigTblPtr = 0;
		(void) CFE_EVS_SendEvent(CI_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
								 "Failed to get Config table's address (0x%08X)",
								 (unsigned int)iStatus);
	}

	iStatus = CFE_TBL_GetAddress((void*)&CI_AppData.TimeoutTbl,
									 CI_AppData.TimeoutTblHdl);
	if (iStatus == CFE_TBL_INFO_UPDATED)
	{
		iStatus = CFE_SUCCESS;
	}
	else if(iStatus != CFE_SUCCESS)
	{
		(void) CFE_EVS_SendEvent(CI_TIMEOUT_TABLE_ERR_EID, CFE_EVS_ERROR,
								 "Failed to get Timeout table's address (0x%08X)",
								 (unsigned int)iStatus);
	}

CI_AcquireConfigPointers_Exit_Tag:
    return (iStatus);

} /* End of CI_AcquirePointers */

/************************/
/*  End of File Comment */
/************************/
    
