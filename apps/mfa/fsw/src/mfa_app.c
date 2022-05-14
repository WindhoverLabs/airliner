/**
 * @file mfa_app.c
 * @author Shahed Rahim (srahim@windhoverlabs.com)
 * @brief This is the Main File for MFA App
 * @version 1.0.0
 * @date 2022-05-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "cfe.h"
#include "mfa_app.h"
#include "mfa_events.h"

uint32 MFA_AppInit(void) {
	int32  status = 0;
	uint32 exitStatus = CFE_ES_APP_RUN;

	/*Register the app*/
	status = CFE_ES_RegisterApp();
	if (status != CFE_SUCCESS) {
		(void) CFE_ES_WriteToSysLog("MFA - Failed to register the app (0x%08X)\n", (unsigned int) status);
		exitStatus = CFE_ES_APP_ERROR;
	}

	/*Register the app with EVS */
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

	return exitStatus;
}

void MFA_AppDeinit(uint32 exitStatus) {
	/*Unregister with EVS*/
	(void) CFE_EVS_Unregister();

	/*Exit from ES*/
	CFE_ES_ExitApp(exitStatus);
}

void MFA_AppMain()
{
	int32  status = 0;
	uint32 exitStatus = MFA_AppInit();
	
	/*Enter our main loop*/
	while(CFE_ES_RunLoop(&exitStatus) == TRUE) {
		status = CFE_EVS_SendEvent(MFA_LOOPING_EID, CFE_EVS_INFORMATION, "Looping");
		OS_TaskDelay(10000);
	}

	MFA_AppDeinit(exitStatus);
}
