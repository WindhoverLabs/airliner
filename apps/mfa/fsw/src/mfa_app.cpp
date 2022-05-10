#include "cfe.h"
#include "mfa_tbldefs.h"

#define MFA_INITIALIZED_EID  (1)
#define MFA_HELLO_WORLD_EID  (2)
#define MFA_INIT_ERR_EID     (3)

#define MFA_MAJOR_VERSION    (1)
#define MFA_MINOR_VERSION    (0)
#define MFA_PATCH_VERSION    (0)

#define MFA_CONFIG_TABLENAME       "CONFIG_TBL"
#define MFA_CONFIG_TABLE_FILENAME  "/cf/apps/mfa_config.tbl"

CFE_TBL_Handle_t  MFA_ConfigTblHdl;




void MFA_AppMain()
{
    int32  status;
    uint32 exitStatus = CFE_ES_APP_RUN;

    /* Register our application */
    status = CFE_ES_RegisterApp();
    if(status != CFE_SUCCESS)
    {
    	(void) CFE_ES_WriteToSysLog("MFA - Failed to register the app (0x%08X)\n", (unsigned int)status);
        exitStatus = CFE_ES_APP_ERROR;
    }

    /* Register the table with CFE */
    status = CFE_EVS_Register(0, 0, CFE_EVS_BINARY_FILTER);
    if(status != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("MFA - Failed to register with EVS (0x%08X)\n", (unsigned int)status);
        exitStatus = CFE_ES_APP_ERROR;
    }

    /* Register Config table */
    status = CFE_TBL_Register(&MFA_ConfigTblHdl,
                               MFA_CONFIG_TABLENAME,
                               sizeof(MFA_ConfigTbl_t),
                               CFE_TBL_OPT_DEFAULT,
                               0);
    if(status != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MFA_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to register Config table (0x%08X)",
                                 (unsigned int)status);
        exitStatus = CFE_ES_APP_ERROR;
    }

    /* Load Config table file */
    status = CFE_TBL_Load(MFA_ConfigTblHdl,
                          CFE_TBL_SRC_FILE,
						  MFA_CONFIG_TABLE_FILENAME);
    if(status != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MFA_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to load Config Table (0x%08X)",
                                 (unsigned int)status);
        exitStatus = CFE_ES_APP_ERROR;
    }

    if(CFE_ES_APP_RUN == exitStatus)
    {
        /* Send an event notifying the ground that we initialized. */
        (void) CFE_EVS_SendEvent(MFA_INITIALIZED_EID, CFE_EVS_INFORMATION, "Initialized v%u.%u.%u",
    	    	   MFA_MAJOR_VERSION,
			       MFA_MINOR_VERSION,
			       MFA_PATCH_VERSION);
    }

    /* Enter our main cyclic loop. */
    while(CFE_ES_RunLoop(&exitStatus) == TRUE)
    {
        (void) CFE_EVS_SendEvent(MFA_HELLO_WORLD_EID, CFE_EVS_INFORMATION, "Hello world!");
        OS_TaskDelay(1000);
    }

    /* Unregister with EVS */
	(void) CFE_EVS_Unregister();

    /* Exit the application */
    CFE_ES_ExitApp(0);
}
