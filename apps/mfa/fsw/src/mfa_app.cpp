#include "cfe.h"
#include "mfa_version.h"
#include "mfa_app.h"


#ifdef __cplusplus
extern "C" {
#endif
	void MFA_AppMain()
	{
		int32  iStatus = 0;
		uint32 exitStatus = CFE_ES_APP_RUN;

		/*Register App */
		iStatus = CFE_ES_RegisterApp();
		if (CFE_SUCCESS != iStatus)
		{
			(void) CFE_ES_WriteToSysLog("MFA - Failed to register the app (0x%08X)\n", (unsigned int)iStatus);
			exitStatus = CFE_ES_APP_ERROR;
		}

		/* Register the table with CFE_EVS*/
		iStatus = CFE_EVS_Register(0, 0, CFE_EVS_BINARY_FILTER);
		if (CFE_SUCCESS != iStatus)
		{
			(void) CFE_ES_WriteToSysLog("MFA - Failed to register with EVS (0x%08X)\n", (unsigned int)iStatus);
			exitStatus = CFE_ES_APP_ERROR;
		}

		CFE_EVS_SendEvent(MFA_INIT_INF_EID,
						  CFE_EVS_DEBUG,
						  "Hello World, from My First App");

        (void) CFE_EVS_SendEvent(MFA_INIT_INF_EID, CFE_EVS_INFORMATION,
                                 "Initialized.  Version %d.%d.%d.%d",
                                 MFA_MAJOR_VERSION,
                                 MFA_MINOR_VERSION,
                                 MFA_REVISION,
                                 MFA_MISSION_REV);

		/* Application main loop */
		while (CFE_ES_RunLoop(&exitStatus) == TRUE)
		{
			OS_printf("Loop\n");
			OS_TaskDelay(1000);
			CFE_EVS_SendEvent(MFA_INITIALIZED_EID,
							  CFE_EVS_DEBUG,
							  "Hello World, from My First App");
		}

		iStatus = CFE_EVS_Unregister();

		if (iStatus != CFE_SUCCESS)
		{
			(void) CFE_ES_WriteToSysLog("MFA - Failed to unregister with EVS (0x%08X)\n", (unsigned int)iStatus);
		}

		/* Exit the application */
		CFE_ES_ExitApp(exitStatus);
	}
#ifdef __cplusplus
}

#endif /* MFA_APP_H */
