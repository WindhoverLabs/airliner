
/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include <string.h>
#include "mavlink_app.h"
#include "mavlink_cds_utils.h"

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
/* Initialize CDS Tables                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 MAVLINK_InitCdsTbl()
{
    int32  iStatus=0;
    int32  iResetType=0;
    uint32 uiResetSubType=0;

    /* Only clear CDS table when it's a PowerOn reset, and not a Processor reset */
    iResetType = CFE_ES_GetResetType(&uiResetSubType);
    if (iResetType == CFE_ES_POWERON_RESET)
    {
        memset((void*)&MAVLINK_AppData.CdsTbl, 0x00, sizeof(MAVLINK_CdsTbl_t));
    }

    /* Register and manage CDS table */
    iStatus = CFE_ES_RegisterCDS(&MAVLINK_AppData.CdsTblHdl, sizeof(MAVLINK_CdsTbl_t),
                                 MAVLINK_CDS_TABLENAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Setup initial content of CDS table */
        iStatus = CFE_ES_CopyToCDS(MAVLINK_AppData.CdsTblHdl, &MAVLINK_AppData.CdsTbl);
        if (iStatus == CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MAVLINK_CDS_INF_EID, CFE_EVS_INFORMATION,
                                     "Successfully setup CDS");
        }
        else
        {
            (void) CFE_EVS_SendEvent(MAVLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "Failed to setup CDS");
        }
    }
    else if (iStatus == CFE_ES_CDS_ALREADY_EXISTS)
    {
        /* If one already exists, get a copy of its current content */
        memset((void*)&MAVLINK_AppData.CdsTbl, 0x00, sizeof(MAVLINK_CdsTbl_t));
        iStatus = CFE_ES_RestoreFromCDS(&MAVLINK_AppData.CdsTbl, MAVLINK_AppData.CdsTblHdl);
        if (iStatus == CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MAVLINK_CDS_INF_EID, CFE_EVS_INFORMATION,
                                     "Successfully restored data from CDS");
        }
        else
        {
            (void) CFE_EVS_SendEvent(MAVLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "Failed to restore data from CDS");
            memset((void*)&MAVLINK_AppData.CdsTbl, 0x00, sizeof(MAVLINK_CdsTbl_t));
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(MAVLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create CDS (0x%08X)", (unsigned int)iStatus);
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Update CDS Tables                                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void MAVLINK_UpdateCdsTbl()
{
    /* TODO:  Add code to update values in CDS table here */
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Save CDS Tables                                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void MAVLINK_SaveCdsTbl()
{
    /* TODO This return value is not checked. Developer should decide what to do here
       in case of failure or should add a return value for higher-level logic to handle. */
    CFE_ES_CopyToCDS(MAVLINK_AppData.CdsTblHdl, &MAVLINK_AppData.CdsTbl);
}

/************************/
/*  End of File Comment */
/************************/
    
