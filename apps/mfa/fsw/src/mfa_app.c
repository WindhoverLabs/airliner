#include "cfe.h"
#include "mfa_app.h"
#include "mfa_events.h"
#include "mfa_tbldefs.h"
#include "mfa_platform_cfg.h"
#include "mfa_mission_cfg.h"
#include "mfa_msgids.h"


MFA_AppData_t MFA_AppData;


uint32 MFA_InitEvents(void)
{
	int32  status;
    uint32 exitStatus = CFE_ES_APP_RUN;

    /* Register the table with CFE */
    status = CFE_EVS_Register(0, 0, CFE_EVS_BINARY_FILTER);
    if(status != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("MFA - Failed to register with EVS (0x%08X)\n", (unsigned int)status);
        exitStatus = CFE_ES_APP_ERROR;
    }

    return exitStatus;
}


uint32 MFA_InitTables(void)
{
    int32  status;
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

	/*
	** Get a pointer to the tables
	*/
	status = CFE_TBL_GetAddress((void**)&MFA_AppData.ConfigTblPtr,
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


uint32 MFA_InitData(void)
{
    int32  status;
    uint32 exitStatus = CFE_ES_APP_RUN;

    CFE_SB_InitMsg(&MFA_AppData.HkTlm,
                   MFA_HK_TLM_MID,
				   sizeof(MFA_AppData.HkTlm),
				   TRUE);

	return exitStatus;
}


uint32 MFA_InitPipes(void)
{
    int32  status;
    uint32 exitStatus = CFE_ES_APP_RUN;

    /* Init schedule pipe and subscribe to wakeup messages */
    status = CFE_SB_CreatePipe(&MFA_AppData.SchPipeId,
    		                    MFA_SCH_PIPE_DEPTH,
			                    MFA_SCH_PIPE_NAME);
    if(status != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MFA_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create SCH pipe (0x%08X)",
                                 (unsigned int)status);
        exitStatus = CFE_ES_APP_ERROR;
    }

    /*
    ** Create Software Bus message pipe
    */
    status = CFE_SB_CreatePipe(&MFA_AppData.CmdPipe,
                               MFA_SCH_PIPE_DEPTH,
                               MFA_SCH_CMD_NAME);
    if (status != CFE_SUCCESS)
    {
        (void)CFE_EVS_SendEvent(MFA_INIT_ERR_EID, CFE_EVS_ERROR,
                          "Error Creating SB Pipe, RC=0x%08X", (unsigned int)status);
        exitStatus = CFE_ES_APP_ERROR;
    }

    else
    {
        status = CFE_SB_SubscribeEx(MFA_WAKEUP_MID,
        		                    MFA_AppData.SchPipeId,
									CFE_SB_Default_Qos,
									MFA_SCH_PIPE_WAKEUP_RESERVED);

        if(status != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MFA_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "Sch Pipe failed to subscribe to MFA_WAKEUP_MID. (0x%08X)",
                                     MFA_SEND_HK_MID, (unsigned int)status);
            exitStatus = CFE_ES_APP_ERROR;
        }

        status = CFE_SB_SubscribeEx(MFA_SEND_HK_MID,
                                    MFA_AppData.SchPipeId,
									CFE_SB_Default_Qos,
                                    MFA_SCH_PIPE_WAKEUP_RESERVED);

        if(status != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MFA_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "Sch Pipe failed to subscribe to MFA_SEND_HK_MID. (0x%08X), RC= RC=0x%08X",
                                     MFA_SEND_HK_MID,(unsigned int)status);
            exitStatus = CFE_ES_APP_ERROR;
        }

        /*
        ** Subscribe to Housekeeping request commands
        */
        status = CFE_SB_Subscribe(MFA_CMD_MID, MFA_AppData.CmdPipe);
        if (status != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(MFA_INIT_ERR_EID,
                              CFE_EVS_ERROR,
                              "Error Subscribing to CMD Request(MID=0x%04X), RC=0x%08X",
                              MFA_CMD_MID, (unsigned int)status);
            exitStatus = CFE_ES_APP_ERROR;
        }
    }

	return exitStatus;
}


uint32 MFA_AppInit(void)
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

    if(CFE_ES_APP_RUN == exitStatus)
    {
        exitStatus = MFA_InitEvents();
    }

    if(CFE_ES_APP_RUN == exitStatus)
    {
    	exitStatus = MFA_InitTables();
    }

    if(CFE_ES_APP_RUN == exitStatus)
    {
    	exitStatus = MFA_InitData();
    }

    if(CFE_ES_APP_RUN == exitStatus)
    {
    	exitStatus = MFA_InitPipes();
    }

    if(CFE_ES_APP_RUN == exitStatus)
    {
        /* Send an event notifying the ground that we initialized. */
        (void) CFE_EVS_SendEvent(MFA_INITIALIZED_EID, CFE_EVS_INFORMATION, "Initialized v%u.%u.%u",
    	    	   MFA_MAJOR_VERSION,
			       MFA_MINOR_VERSION,
			       MFA_PATCH_VERSION);
    }

	return exitStatus;
}


void MFA_AppDeinit(void)
{
    /* Unregister with EVS */
	(void) CFE_EVS_Unregister();
}


void MFA_ProcessNewCmds(void)
{
    CFE_SB_Msg_t*   CmdMsgPtr = 0;
    int32           status = CFE_SB_RcvMsg(&CmdMsgPtr, MFA_AppData.CmdPipe, CFE_SB_POLL);

    if(status == CFE_SUCCESS)
    {
        CFE_SB_MsgId_t CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
        switch (CmdMsgId)
        {
            case MFA_CMD_MID:
            {
            //    CI_ProcessNewAppCmds(CmdMsgPtr);
                uint32  uiCmdCode = 0;

                if (CmdMsgPtr != NULL)
                    {
                        uiCmdCode = CFE_SB_GetCmdCode(CmdMsgPtr);
                        switch (uiCmdCode)
                        {
                            case MFA_NOOP_CC:
                             {
                                MFA_AppData.HkTlm.Commands++;
                                (void) CFE_EVS_SendEvent(MFA_CMD_INF_EID, CFE_EVS_INFORMATION,
                                                  "Recvd NOOP cmd (%u), Version %d.%d.%d.%d",
                                                  (unsigned int)uiCmdCode,
                                                  MFA_MAJOR_VERSION,
                                                  MFA_MINOR_VERSION,
                                                  MFA_REVISION,
                                                  MFA_MISSION_REV);
                                break;
                            }
                        default:
                        //Fallthru
                          break;
                        }
                     }
                    break;
               }

            default:
                /* Bump the command error counter for an unknown command.
                 * (This should only occur if it was subscribed to with this
                 *  pipe, but not handled in this switch-case.) */
                MFA_AppData.HkTlm.CmdErrors++;
                (void) CFE_EVS_SendEvent(MFA_CMD_INF_EID, CFE_EVS_ERROR,
                                  "Recvd invalid CMD msgId (0x%04X)", (unsigned short)CmdMsgId);
                break;
          }
     }
}



void MFA_AppMain()
{
    uint32 exitStatus;

    exitStatus = MFA_AppInit();

    /* Enter our main cyclic loop. */
    while(CFE_ES_RunLoop(&exitStatus) == TRUE)
    {
        int32           status = CFE_SUCCESS;
        CFE_SB_Msg_t*   msgPtr = 0;
        CFE_SB_MsgId_t  msgId = 0;

        /* Wait for WakeUp messages from scheduler */
        status = CFE_SB_RcvMsg(&msgPtr, MFA_AppData.SchPipeId, MFA_SCH_PEND_TIME);
        if(CFE_SUCCESS == status)
        {
            msgId = CFE_SB_GetMsgId(msgPtr);
            switch(msgId)
            {
                case MFA_WAKEUP_MID:
                {
                    (void) CFE_EVS_SendEvent(MFA_HELLO_WORLD_EID, CFE_EVS_INFORMATION, "%s", MFA_AppData.ConfigTblPtr->Message);
                    MFA_AppData.HkTlm.HelloCount++;
                    MFA_ProcessNewCmds();

                    break;
                }

                case MFA_SEND_HK_MID:
                {
                    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&MFA_AppData.HkTlm);
                    CFE_SB_SendMsg((CFE_SB_Msg_t *)&MFA_AppData.HkTlm);

                    break;
                }
            }
        }
    }

    MFA_AppDeinit();

    /* Exit the application */
    CFE_ES_ExitApp(exitStatus);
}
