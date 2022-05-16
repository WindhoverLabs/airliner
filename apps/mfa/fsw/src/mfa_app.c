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

#include "mfa_platform_cfg.h"
#include "mfa_msgids.h"


MFA_AppData_t MFA_AppData;

uint32 MFA_Register_App_CFE_ES(void) {
	int32  status = 0;
	uint32 exitStatus = CFE_ES_APP_RUN;

	/*Register the app*/
	status = CFE_ES_RegisterApp();
	if (status != CFE_SUCCESS) {
		(void) CFE_ES_WriteToSysLog("MFA - Failed to register the app (0x%08X)\n", (unsigned int) status);
		exitStatus = CFE_ES_APP_ERROR;
	}

	return exitStatus;
}

uint32 MFA_InitEvents(void) {
	int32  status = 0;
	uint32 exitStatus = CFE_ES_APP_RUN;

	/*Register the app with EVS */
	status = CFE_EVS_Register(0, 0, CFE_EVS_BINARY_FILTER);
	if (status != CFE_SUCCESS) {
		(void) CFE_ES_WriteToSysLog("MFA - Failed to register with EVS (0x%08X)\n", (unsigned int) status);
		exitStatus = CFE_ES_APP_ERROR;
	}

	return exitStatus;
}

void MFA_SendInitializedEvent(void) {
		/*Send an event notifying ground that we initialized.*/
		(void) CFE_EVS_SendEvent(MFA_INITIALIZED_EID, CFE_EVS_INFORMATION, "Initialized v%u.%u.%u", 
			MFA_MAJOR_VERSION, MFA_MINOR_VERSION, MFA_PATCH_VERSION);
}

uint32 MFA_InitPipes(void) {
    int32 status;
    uint32 exitStatus = CFE_ES_APP_RUN;

    status = CFE_SB_CreatePipe(
        &MFA_AppData.SchPipeId, 
        MFA_SCH_PIPE_DEPTH, 
        MFA_SCH_PIPE_NAME);

	if (CFE_SUCCESS != status) {
		(void) CFE_EVS_SendEvent(MFA_INIT_ERR_EID, CFE_EVS_ERROR,
								"Failed to create sch pipe (0x%08X)",
								(unsigned int)status);
		exitStatus = CFE_ES_APP_ERROR;
	} else {
		status = CFE_SB_SubscribeEx(
			MFA_WAKEUP_MID,
			MFA_AppData.SchPipeId,
			CFE_SB_Default_Qos,
			MFA_SCH_PIPE_WAKEUP_RESERVED);
		
		if (status != CFE_SUCCESS) {
			(void) CFE_EVS_SendEvent(MFA_INIT_ERR_EID, CFE_EVS_ERROR,
				"Sch Pipe failed to subscribe to MFA_WAKEUP_MID, (0x%08X)",
				(unsigned int) status);
		}
	}
	return exitStatus;
}

uint32 MFA_AppInit(void) {
	uint32 exitStatus = MFA_Register_App_CFE_ES();

	if (CFE_ES_APP_RUN == exitStatus) {
		exitStatus = MFA_InitEvents();
	}
	
	if (CFE_ES_APP_RUN == exitStatus) {
		MFA_SendInitializedEvent();
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
	uint32 exitStatus = MFA_AppInit();
	
	/*Enter our main loop*/
	while(CFE_ES_RunLoop(&exitStatus) == TRUE) {
		int32  status = CFE_SUCCESS;
        CFE_SB_Msg_t* msgPtr = NULL;
        CFE_SB_MsgId_t msgId =0 ;

        /*Wait for Wakeup messages*/
        status = CFE_SB_RcvMsg(&msgPtr, MFA_AppData.SchPipeId, MFA_SCH_PEND_TIME);
		if (CFE_SUCCESS==status) {
			msgId = CFE_SB_GetMsgId(msgPtr);

			switch(msgId) {
				case MFA_WAKEUP_MID:
				{
					status = CFE_EVS_SendEvent(MFA_LOOPING_EID, CFE_EVS_INFORMATION, "Looping");
					break;
				}
				default:
					break;
			}
		}
	}

	MFA_AppDeinit(exitStatus);
}
