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
#include "mfa_mission_cfg.h"
#include "mfa_msgids.h"

#include "mfa_msgs.h"
#include <string.h>

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
uint32 MFA_InitData(void) {
    int32 status;
    uint32 exitStatus = CFE_ES_APP_RUN;
    CFE_SB_InitMsg(
        &MFA_AppData.HkTlm, 
        MFA_HK_TLM_MID, 
        sizeof(MFA_AppData.HkTlm), 
        TRUE);
    return exitStatus;
}

uint32 MFA_InitPipes(void) {
    int32 status;
    uint32 exitStatus = CFE_ES_APP_RUN;

    status = CFE_SB_CreatePipe(
        &MFA_AppData.SchPipeId, 
        MFA_SCH_PIPE_DEPTH, 
        MFA_SCH_PIPE_NAME);

	if (CFE_SUCCESS != status) {
		(void) CFE_EVS_SendEvent(
			MFA_INIT_ERR_EID,
			CFE_EVS_ERROR,
			"Failed to create sch pipe (0x%08X)", (unsigned int)status
		);
		exitStatus = CFE_ES_APP_ERROR;

	} else {
		status = CFE_SB_SubscribeEx(
			MFA_CMD_MID,
			MFA_AppData.SchPipeId,
			CFE_SB_Default_Qos,
			MFA_SCH_PIPE_WAKEUP_RESERVED);
		
		if (status != CFE_SUCCESS) {
			(void) CFE_EVS_SendEvent(MFA_INIT_ERR_EID, CFE_EVS_ERROR,
				"Sch Pipe failed to subscribe to MFA_CMD_MID, (0x%08X)",
				(unsigned int) status);
		}

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

		status = CFE_SB_SubscribeEx(
			MFA_SEND_HK_MID,
			MFA_AppData.SchPipeId,
			CFE_SB_Default_Qos,
			MFA_SCH_PIPE_WAKEUP_RESERVED);
		
		if (status != CFE_SUCCESS) {
			(void) CFE_EVS_SendEvent(MFA_INIT_ERR_EID, CFE_EVS_ERROR,
				"Sch Pipe failed to subscribe to MFA_SEND_HK_MID, (0x%08X)",
				(unsigned int) status);
		}

	}
	return exitStatus;
}


uint32 MFA_InitTables(void) {
    int32 status;
    uint32 exitStatus = CFE_ES_APP_RUN;
    /* Register Config table */
    status = CFE_TBL_Register(&MFA_AppData.ConfigTblHdl,
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
    return exitStatus;
}

uint32 MFA_LoadConfigTable(void) {
    int32 status;
    uint32 exitStatus = CFE_ES_APP_RUN;
    /* Load Config table file */
    status = CFE_TBL_Load(MFA_AppData.ConfigTblHdl,
                          CFE_TBL_SRC_FILE,
						  MFA_CONFIG_TABLE_FILENAME);
    if(status != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MFA_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to load Config Table (0x%08X)",
                                 (unsigned int)status);
        
        exitStatus = CFE_ES_APP_ERROR;
    }
    return exitStatus;
}

uint32 MFA_GetConfigTableAddress(void) {
    int32 status;
    uint32 exitStatus = CFE_ES_APP_RUN;
	/*
	** Get a pointer to the tables
	*/
	status = CFE_TBL_GetAddress((void*)&MFA_AppData.ConfigTblPtr,
			                            MFA_AppData.ConfigTblHdl);
	if(status != CFE_TBL_INFO_UPDATED)
	{
		MFA_AppData.ConfigTblPtr = 0;
		(void) CFE_EVS_SendEvent(MFA_INIT_ERR_EID, CFE_EVS_ERROR,
								 "Failed to get Config table's address (0x%08X)",
								 (unsigned int)status);
		exitStatus = CFE_ES_APP_ERROR;
	}
    return exitStatus;
}

void MFA_SendInitializedEvent(void) {
		/*Send an event notifying ground that we initialized.*/
		(void) CFE_EVS_SendEvent(MFA_INITIALIZED_EID, CFE_EVS_INFORMATION, "Initialized v%u.%u.%u", 
			MFA_MAJOR_VERSION, MFA_MINOR_VERSION, MFA_PATCH_VERSION);
}

uint32 MFA_AppInit(void) {
	uint32 exitStatus = MFA_Register_App_CFE_ES();

	if (CFE_ES_APP_RUN == exitStatus) {
		exitStatus = MFA_InitEvents();
	}

	if (CFE_ES_APP_RUN == exitStatus) {
		exitStatus = MFA_InitData();
	}

	if (CFE_ES_APP_RUN == exitStatus) {
		exitStatus = MFA_InitPipes();
	}

	if (CFE_ES_APP_RUN == exitStatus) {
		exitStatus = MFA_InitTables();
	}

	if (CFE_ES_APP_RUN == exitStatus) {
		exitStatus = MFA_LoadConfigTable();
	}
	
	if (CFE_ES_APP_RUN == exitStatus) {
		exitStatus = MFA_GetConfigTableAddress();
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Command Length                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool MFA_VerifyCmdLength(CFE_SB_Msg_t* msgPtr, uint16 expectedLen)
{
    uint16  msgLen = 0;
    CFE_SB_MsgId_t MsgId = 0;
    uint16 cmdCode = 0;

    if (msgPtr != NULL)
    {
        msgLen = CFE_SB_GetTotalMsgLength(msgPtr);

        if (expectedLen != msgLen)
        {
            MsgId = CFE_SB_GetMsgId(msgPtr);
            cmdCode = CFE_SB_GetCmdCode(msgPtr);

            (void) CFE_EVS_SendEvent(
				MFA_MSG_LEN_ERR_EID,
				CFE_EVS_ERROR,
				"Rcvd invalid msgLen: msgId=0x%04X, cmdCode=%d, ""msgLen=%d, expectedLen=%d", MsgId, cmdCode, msgLen, expectedLen
			);
                              
            //(void) OS_MutSemTake(TO_AppData.MutexID);
            MFA_AppData.HkTlm.CmdErrors++;
            //(void) OS_MutSemGive(TO_AppData.MutexID);
            return FALSE;
        }
    }
    else
    {
        /* msgPtr is NULL */
        return FALSE;
    }

    return TRUE;
}


void MFA_ProcessNewAppCmds(CFE_SB_Msg_t *msgPtr) {
	uint32  cmdCode = 0;
    osalbool sizeOk   = FALSE;
    osalbool isCmdOk  = FALSE;

    if (msgPtr != NULL)
    {
        cmdCode = CFE_SB_GetCmdCode(msgPtr);
        switch (cmdCode)
        {
			case MFA_NOOP_CC:
			{
				
				sizeOk = MFA_VerifyCmdLength(msgPtr, sizeof(MFA_NoArgCmd_t));

				if (TRUE == sizeOk)
				{
					MFA_AppData.HkTlm.Commands++;
					(void) CFE_EVS_SendEvent(
						MFA_CMD_NOOP_INF_EID, 
						CFE_EVS_INFORMATION,
						"Executed NOOP cmd (%u), Version %d.%d.%d", (unsigned int)cmdCode, MFA_MAJOR_VERSION, MFA_MINOR_VERSION, MFA_PATCH_VERSION
					);
					//MFA_PrintCustomVersion();
				}
				break;
			}

			case MFA_RESET_CC:
			{
				sizeOk = MFA_VerifyCmdLength(msgPtr, sizeof(MFA_NoArgCmd_t));

				if (TRUE == sizeOk)
				{
					//(void) OS_MutSemTake(MFA_AppData.MutexID);
					MFA_AppData.HkTlm.Commands = 0;
					MFA_AppData.HkTlm.CmdErrors = 0;
					MFA_AppData.HkTlm.HelloCount = 0;

					//(void) OS_MutSemGive(MFA_AppData.MutexID);

					//TO_Channel_ResetCountsAll();

					(void) CFE_EVS_SendEvent(
						MFA_CMD_RESET_INF_EID, 
						CFE_EVS_INFORMATION,
						"Executed RESET cmd (%u)", (unsigned int)cmdCode
					);
				}
				
				break;
			}

			case MFA_SET_MSG_CC:
			{
				sizeOk = MFA_VerifyCmdLength(msgPtr, sizeof(MFA_StringArgCmd_t));

				if (TRUE == sizeOk)
				{
					int32 status =0;
					MFA_StringArgCmd_t* msgStringArgCmdPtr = (MFA_StringArgCmd_t*) msgPtr;
					strncpy(
						MFA_AppData.ConfigTblPtr->Message, 
						msgStringArgCmdPtr->Message, 
						sizeof(msgStringArgCmdPtr->Message)
					);
					msgStringArgCmdPtr->Message[sizeof(msgStringArgCmdPtr->Message)-1] = 0;
					status = CFE_TBL_Modified(MFA_AppData.ConfigTblHdl);
					if (CFE_SUCCESS!=status) {
						MFA_AppData.HkTlm.CmdErrors++;
						(void) CFE_EVS_SendEvent(
							MFA_TABLEMOD_ERR_EID, 
							CFE_EVS_ERROR,
							"Table Mod Error (0x%08X)", (unsigned int) status
						);
					} else {
						MFA_AppData.HkTlm.Commands++;
					}
				}
				break;
			}
		}
	}
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
                case MFA_CMD_MID:
				{
                    MFA_ProcessNewAppCmds(msgPtr);
                    break;
				}
				case MFA_WAKEUP_MID:
				{
					status = CFE_EVS_SendEvent(MFA_LOOPING_EID, CFE_EVS_INFORMATION, "%s", MFA_AppData.ConfigTblPtr->Message);
					MFA_AppData.HkTlm.HelloCount++;
					break;
				}
                case MFA_SEND_HK_MID:
                {
                    //(void) CFE_EVS_SendEvent(MFA_HELLO_WORLD_EID, CFE_EVS_INFORMATION, "%s", MFA_AppData.ConfigTblPtr->Message);

                    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&MFA_AppData.HkTlm);
                    CFE_SB_SendMsg((CFE_SB_Msg_t *) &MFA_AppData.HkTlm);
                    break;
                }
				default:
					status = CFE_EVS_SendEvent(MFA_INVALID_MSG_EID, CFE_EVS_ERROR, "Unexpected message 0x%04x", msgId);
					break;
			}
		}
	}

	MFA_AppDeinit(exitStatus);
}
