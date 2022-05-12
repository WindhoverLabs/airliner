#include "cfe.h"

#define MFA_MAJOR_VERSION 1
#define MFA_MINOR_VERSION 0
#define MFA_PATCH_VERSION 0

#define MFA_INITIALIZED_EID (1)
#define MFA_LOOPING_EID     (2)

void MFA_AppMain()
{
	int32  status = 0;
	uint32 exitStatus = CFE_ES_APP_RUN;
	
	/*Register the app*/
	status = CFE_ES_RegisterApp();
	if (status != CFE_SUCCESS) {
		(void) CFE_ES_WriteToSysLog("MFA - Failed to register the app (0x%08X)\n", (unsigned int) status);
		exitStatus = CFE_ES_APP_ERROR;
	}

	/*Register the table with CFE */
	status = CFE_EVS_Register(0, 0, CFE_EVS_BINARY_FILTER);
	if (status != CFE_SUCCESS) {
		(void) CFE_ES_WriteToSysLog("MFA - Failed to register with EVS (0x%08X)\n", (unsigned int) status);
		exitStatus = CFE_ES_APP_ERROR;
	}
	
	if (CFE_ES_APP_RUN == exitStatus) {
		/*Send an event notifying ground that we initialized.*/
		status = CFE_EVS_SendEvent(MFA_INITIALIZED_EID, CFE_EVS_INFORMATION, "Initialized v%u.%u.%u", 
			MFA_MAJOR_VERSION, MFA_MINOR_VERSION, MFA_PATCH_VERSION);
	}

	/*Enter our main loop*/
	while(CFE_ES_RunLoop(&exitStatus) == TRUE) {
		status = CFE_EVS_SendEvent(MFA_LOOPING_EID, CFE_EVS_INFORMATION, "Looping");
		OS_TaskDelay(10000);
	}

	status = CFE_EVS_Unregister();

	CFE_ES_ExitApp(exitStatus);
}
