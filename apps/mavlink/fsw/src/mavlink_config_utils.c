
/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "mavlink_config_utils.h"

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/************************************************************************
** External Global Variables
*************************************************************************/
extern MAVLINK_AppData_t  MAVLINK_AppData;

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

int32 MAVLINK_InitActionMap()
{
    int32  iStatus=0;

    /* Register Config table */
    iStatus = CFE_TBL_Register(&MAVLINK_AppData.ActionMapHdl,
    						   MAVLINK_ACTION_MAP_TABLENAME,
                               (sizeof(MAVLINK_ActionMapTbl_t)),
                               CFE_TBL_OPT_DEFAULT,
							   MAVLINK_ValidateActionMap);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, a critical table could return another nominal code.  If this table is
         * made critical this logic would have to change. */
        (void) CFE_EVS_SendEvent(MAVLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to register Config table (0x%08X)",
                                 (unsigned int)iStatus);
        goto MAVLINK_InitConfigTbl_Exit_Tag;
    }

    /* Load Config table file */
    iStatus = CFE_TBL_Load(MAVLINK_AppData.ActionMapHdl,
                           CFE_TBL_SRC_FILE,
                           MAVLINK_ACTION_MAP_TABLE_FILENAME);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, CFE_SUCCESS is for a successful full table load.  If a partial table
           load is desired then this logic would have to change. */
        (void) CFE_EVS_SendEvent(MAVLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to load Config Table (0x%08X)",
                                 (unsigned int)iStatus);
        goto MAVLINK_InitConfigTbl_Exit_Tag;
    }

    iStatus = MAVLINK_AcquireParamPointers();

MAVLINK_InitConfigTbl_Exit_Tag:
    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate Param Table                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 MAVLINK_ValidateActionMap(void* ConfigTblPtr)
{
    int32  iStatus=0;
    MAVLINK_ActionMapTbl_t* MAVLINK_ConfigTblPtr = (MAVLINK_ActionMapTbl_t*)(ConfigTblPtr);

    if (ConfigTblPtr == NULL)
    {
        iStatus = -1;
        goto MAVLINK_ValidateActionMap_Exit_Tag;
    }

    /* TODO:  Add code to validate new data values here.
    **
    ** Examples:
    ** if (MAVLINK_ConfigTblPtr->iParam <= 16) {
    **   (void) CFE_EVS_SendEvent(MAVLINK_PARAM_TABLE_INF_EID, CFE_EVS_ERROR,
     *                         "Invalid value for Config parameter sParam (%d)",
    **                         MAVLINK_ConfigTblPtr->iParam);
    ** }
    **/

MAVLINK_ValidateActionMap_Exit_Tag:
    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process New Config Table                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void MAVLINK_ProcessNewConfigTbl()
{
    /* TODO:  Add code to set new Config parameters with new values here.
    **
    ** Examples:
    **
    **    MAVLINK_AppData.latest_sParam = MAVLINK_AppData.ConfigTblPtr->sParam;
    **    MAVLINK_AppData.latest_fParam = MAVLINK.AppData.ConfigTblPtr->fParam;
    */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Acquire Conifg Pointers                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MAVLINK_AcquireParamPointers(void)
{
    int32 iStatus = CFE_SUCCESS;

    /*
    ** Release the table
    */
    /* TODO: This return value can indicate success, error, or that the info has been 
     * updated.  We ignore this return value in favor of checking CFE_TBL_Manage(), but
     * be sure this is the behavior you want. */
    (void) CFE_TBL_ReleaseAddress(MAVLINK_AppData.ActionMapHdl);

    /*
    ** Manage the table
    */
    iStatus = CFE_TBL_Manage(MAVLINK_AppData.ActionMapHdl);
    if ((iStatus != CFE_SUCCESS) && (iStatus != CFE_TBL_INFO_UPDATED))
    {
        (void) CFE_EVS_SendEvent(MAVLINK_PARAM_TABLE_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to manage Config table (0x%08X)",
                                 (unsigned int)iStatus);
        goto MAVLINK_AcquireConfigPointers_Exit_Tag;
    }

    /*
    ** Get a pointer to the table
    */
    iStatus = CFE_TBL_GetAddress((void*)&MAVLINK_AppData.ActionMapPtr,
                                 MAVLINK_AppData.ActionMapHdl);
    if (iStatus == CFE_TBL_INFO_UPDATED)
    {
        MAVLINK_ProcessNewConfigTbl();
        iStatus = CFE_SUCCESS;
    }
    else if(iStatus != CFE_SUCCESS)
    {
	MAVLINK_AppData.ActionMapPtr = 0;
        (void) CFE_EVS_SendEvent(MAVLINK_PARAM_TABLE_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to get Config table's address (0x%08X)",
                                 (unsigned int)iStatus);
    }

MAVLINK_AcquireConfigPointers_Exit_Tag:
    return (iStatus);

} /* End of MAVLINK_AcquirePointers */

/************************/
/*  End of File Comment */
/************************/
    
