
/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include <string.h>

#include "cfe.h"

#include "mavlink_version.h"
#include "mavlink_app.h"
#include "mavlink_msg.h"


/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/
MAVLINK_AppData_t  MAVLINK_AppData;

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Definitions
*************************************************************************/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 MAVLINK_InitEvent()
{
    int32  iStatus=CFE_SUCCESS;
    int32  ind = 0;

    /* Initialize the event filter table.
     * Note: 0 is the CFE_EVS_NO_FILTER mask and event 0 is reserved (not used) */
    memset((void*)MAVLINK_AppData.EventTbl, 0x00, sizeof(MAVLINK_AppData.EventTbl));

    /* TODO: Choose the events you want to filter.  CFE_EVS_MAX_EVENT_FILTERS
     * limits the number of filters per app.  An explicit CFE_EVS_NO_FILTER 
     * (the default) has been provided as an example. */
    MAVLINK_AppData.EventTbl[  ind].EventID = MAVLINK_RESERVED_EID;
    MAVLINK_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    MAVLINK_AppData.EventTbl[  ind].EventID = MAVLINK_INF_EID;
    MAVLINK_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    MAVLINK_AppData.EventTbl[  ind].EventID = MAVLINK_PARAM_TABLE_ERR_EID;
    MAVLINK_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    MAVLINK_AppData.EventTbl[  ind].EventID = MAVLINK_CDS_ERR_EID;
    MAVLINK_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    MAVLINK_AppData.EventTbl[  ind].EventID = MAVLINK_PIPE_ERR_EID;
    MAVLINK_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    MAVLINK_AppData.EventTbl[  ind].EventID = MAVLINK_MSGID_ERR_EID;
    MAVLINK_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    MAVLINK_AppData.EventTbl[  ind].EventID = MAVLINK_MSGLEN_ERR_EID;
    MAVLINK_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(MAVLINK_AppData.EventTbl,
                               MAVLINK_EVT_CNT, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("MAVLINK - Failed to register with EVS (0x%08X)\n", (unsigned int)iStatus);
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 MAVLINK_InitPipe()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&MAVLINK_AppData.SchPipeId,
                                 MAVLINK_SCH_PIPE_DEPTH,
                                 MAVLINK_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(MAVLINK_WAKEUP_MID, MAVLINK_AppData.SchPipeId, CFE_SB_Default_Qos, MAVLINK_SCH_PIPE_WAKEUP_RESERVED);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MAVLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "Sch Pipe failed to subscribe to MAVLINK_WAKEUP_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto MAVLINK_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(MAVLINK_SEND_HK_MID, MAVLINK_AppData.SchPipeId, CFE_SB_Default_Qos, MAVLINK_SCH_PIPE_SEND_HK_RESERVED);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MAVLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "CMD Pipe failed to subscribe to MAVLINK_SEND_HK_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto MAVLINK_InitPipe_Exit_Tag;
        }

    }
    else
    {
        (void) CFE_EVS_SendEvent(MAVLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create SCH pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto MAVLINK_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&MAVLINK_AppData.CmdPipeId,
                                 MAVLINK_CMD_PIPE_DEPTH,
                                 MAVLINK_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(MAVLINK_CMD_MID, MAVLINK_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MAVLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "CMD Pipe failed to subscribe to MAVLINK_CMD_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto MAVLINK_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(MAVLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create CMD pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto MAVLINK_InitPipe_Exit_Tag;
    }

    /* Init data pipe and subscribe to messages on the data pipe */
    iStatus = CFE_SB_CreatePipe(&MAVLINK_AppData.DataPipeId,
                                 MAVLINK_DATA_PIPE_DEPTH,
                                 MAVLINK_DATA_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {

    }
    else
    {
        (void) CFE_EVS_SendEvent(MAVLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create Data pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto MAVLINK_InitPipe_Exit_Tag;
    }

MAVLINK_InitPipe_Exit_Tag:
    return (iStatus);
}
    

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 MAVLINK_InitData()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init input data */
    memset((void*)&MAVLINK_AppData.InData, 0x00, sizeof(MAVLINK_AppData.InData));

    /* Init output data */
    memset((void*)&MAVLINK_AppData.OutData, 0x00, sizeof(MAVLINK_AppData.OutData));
    CFE_SB_InitMsg(&MAVLINK_AppData.OutData,
                   MAVLINK_OUT_DATA_MID, sizeof(MAVLINK_AppData.OutData), TRUE);

    /* Init housekeeping packet */
    memset((void*)&MAVLINK_AppData.HkTlm, 0x00, sizeof(MAVLINK_AppData.HkTlm));
    CFE_SB_InitMsg(&MAVLINK_AppData.HkTlm,
                   MAVLINK_HK_TLM_MID, sizeof(MAVLINK_AppData.HkTlm), TRUE);

    return (iStatus);
}




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* MAVLINK initialization                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 MAVLINK_InitApp()
{
    int32  iStatus   = CFE_SUCCESS;
    int8   hasEvents = 0;

    iStatus = MAVLINK_InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("MAVLINK - Failed to init events (0x%08X)\n", (unsigned int)iStatus);
        goto MAVLINK_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = MAVLINK_InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MAVLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init pipes (0x%08X)",
                                 (unsigned int)iStatus);
        goto MAVLINK_InitApp_Exit_Tag;
    }

    iStatus = MAVLINK_InitData();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MAVLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init data (0x%08X)",
                                 (unsigned int)iStatus);
        goto MAVLINK_InitApp_Exit_Tag;
    }

    iStatus = MAVLINK_InitActionMap();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MAVLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init config tables (0x%08X)",
                                 (unsigned int)iStatus);
        goto MAVLINK_InitApp_Exit_Tag;
    }

    iStatus = MAVLINK_InitCdsTbl();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MAVLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init CDS table (0x%08X)",
                                 (unsigned int)iStatus);
        goto MAVLINK_InitApp_Exit_Tag;
    }

    /* Initialize custom code. */
	iStatus = MAVLINK_InitCustom();
	if(iStatus != CFE_SUCCESS)
	{
		CFE_ES_WriteToSysLog("MAVLINK - InitCustom failed (%i)\n", (int)iStatus);
		goto MAVLINK_InitApp_Exit_Tag;
	}

    /* Register the cleanup callback */
    iStatus = OS_TaskInstallDeleteHandler(&MAVLINK_CleanupCallback);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MAVLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init register cleanup callback (0x%08X)",
                                 (unsigned int)iStatus);
        goto MAVLINK_InitApp_Exit_Tag;
    }

    /* Initialize listener child task. */
    iStatus = MAVLINK_InitChildTasks();
    if(iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("MAVLINK - Failed to initialize child tasks (0x%08X)\n", (unsigned int)iStatus);
        goto MAVLINK_InitApp_Exit_Tag;
    }

MAVLINK_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MAVLINK_INIT_INF_EID, CFE_EVS_INFORMATION,
                                 "Initialized.  Version %d.%d.%d.%d",
                                 MAVLINK_MAJOR_VERSION,
                                 MAVLINK_MINOR_VERSION,
                                 MAVLINK_REVISION,
                                 MAVLINK_MISSION_REV);
    }
    else
    {
        if (hasEvents == 1)
        {
            (void) CFE_EVS_SendEvent(MAVLINK_INIT_ERR_EID, CFE_EVS_ERROR, "Application failed to initialize");
        }
        else
        {
            (void) CFE_ES_WriteToSysLog("MAVLINK - Application failed to initialize\n");
        }
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* MAVLINK Cleanup                                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void MAVLINK_CleanupCallback()
{
	MAVLINK_CleanupCustom();
	MAVLINK_AppData.IngestActive = FALSE;
	OS_MutSemGive(MAVLINK_AppData.ActionMapMutex);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Spawn Child Tasks			                                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MAVLINK_InitChildTasks(void)
{
    int32 Status = CFE_SUCCESS;

    MAVLINK_AppData.ListenersAlive = TRUE;

	Status= CFE_ES_CreateChildTask(&MAVLINK_AppData.ListenerTaskID,
								   MAVLINK_LISTENER_TASK_NAME,
								   MAVLINK_ListenerTaskMain,
								   NULL,
								   MAVLINK_LISTENER_TASK_STACK_SIZE,
								   MAVLINK_LISTENER_TASK_PRIORITY,
								   MAVLINK_LISTENER_TASK_FLAGS);
	if (Status != CFE_SUCCESS)
	{
		goto MAVLINK_InitListenerTask_Exit_Tag;
	}

	Status= CFE_ES_CreateChildTask(&MAVLINK_AppData.PassThruListenerTaskID,
								   MAVLINK_PASSTHRU_LISTENER_TASK_NAME,
								   MAVLINK_PassThruListenerTaskMain,
								   NULL,
								   MAVLINK_LISTENER_TASK_STACK_SIZE,
								   MAVLINK_LISTENER_TASK_PRIORITY,
								   MAVLINK_LISTENER_TASK_FLAGS);
	if (Status != CFE_SUCCESS)
	{
		goto MAVLINK_InitListenerTask_Exit_Tag;
	}

MAVLINK_InitListenerTask_Exit_Tag:
	if (Status != CFE_SUCCESS)
	{
		CFE_EVS_SendEvent (MAVLINK_LISTENER_CREATE_CHDTASK_ERR_EID,
						   CFE_EVS_ERROR,
						   "Listener child task failed.  CFE_ES_CreateChildTask returned: 0x%08lX",
						   Status);
	}

    return Status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Enable Connections		                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MAVLINK_EnableConnection(void)
{
	MAVLINK_AppData.HkTlm.HeartbeatActive = TRUE;
	MAVLINK_AppData.IngestActive = TRUE;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Disable Connections		                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MAVLINK_DisableConnection(void)
{
	MAVLINK_AppData.HkTlm.HeartbeatActive = FALSE;
	MAVLINK_AppData.IngestActive = FALSE;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Get Action for Mavlink Message                                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
MAVLINK_MsgAction_t MAVLINK_GetMessageAction(mavlink_message_t msg)
{
	MAVLINK_MsgAction_t 	action = ACTION_PASSTHRU;
    int i = 0;

	/* Lock the mutex */
	OS_MutSemTake(MAVLINK_AppData.ActionMapMutex);

	/* Search for msg id in action map  */
	for (i = 0; i < MAVLINK_ACTION_MAP_ENTRIES; ++i)
	{
		if(MAVLINK_AppData.ActionMapPtr->ActionMap[i].MsgId == msg.msgid)
		{
			action = MAVLINK_AppData.ActionMapPtr->ActionMap[i].Action;
			break;
		}
	}

	/* Unlock the mutex */
	OS_MutSemGive(MAVLINK_AppData.ActionMapMutex);

	return action;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Child Task Pass Thru Listener Main                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MAVLINK_PassThruListenerTaskMain(void)
{
	int32 Status = -1;
	mavlink_message_t 		msg;
	mavlink_status_t 		msg_status;
    uint32 					msg_size = MAVLINK_MAX_PACKET_LEN;
    int index = 0;

	Status = CFE_ES_RegisterChildTask();
	if (Status == CFE_SUCCESS)
	{
		while(MAVLINK_AppData.ListenersAlive == TRUE)
		{
			/* Ingest Loop */
			do{
				/* Receive px4 message */
				MAVLINK_ReadPassThru(MAVLINK_AppData.PassThruIngestBuffer, &msg_size);

				/* Verify message length. If equal to -1 there is no data on the socket */
				if(msg_size == -1)
				{
					msg_size = MAVLINK_MAX_PACKET_LEN;
					continue;
				}
				/* Parse the message */
				for (index = 0; index < msg_size; ++index)
				{
					if (mavlink_parse_char(MAVLINK_COMM_0, MAVLINK_AppData.PassThruIngestBuffer[index], &msg, &msg_status))
					{
						MAVLINK_SendMessage((char *) &MAVLINK_AppData.PassThruIngestBuffer, msg_size); //TODO: does send need thread safety
//						if(msg.msgid != MAVLINK_MSG_ID_HEARTBEAT)
//						{
//							// TODO: Remove
//							if(msg.msgid == MAVLINK_MSG_ID_AUTOPILOT_VERSION)
//							{
//								mavlink_autopilot_version_t decodedMsg;
//								mavlink_msg_autopilot_version_decode(&msg, &decodedMsg);
//								OS_printf("capability: %u", decodedMsg.capabilities);
//							}
//
//							if(msg.msgid == MAVLINK_MSG_ID_COMMAND_LONG)
//							{
//								mavlink_command_long_t 		decodedMsg;
//								mavlink_msg_command_long_decode(&msg, &decodedMsg);
//
//								MAVLINK_HandleCommandLong(decodedMsg);
//							}
//
//							/* Send msg to GCS. Any other message from PX4 is not intended for us */
//							MAVLINK_SendMessage((char *) &MAVLINK_AppData.PassThruIngestBuffer, msg_size); //TODO: does send need thread safety
//						}
//						else
//						{
//							mavlink_heartbeat_t decodedMsg;
//							mavlink_msg_heartbeat_decode(&msg, &decodedMsg);
//							memcpy(&MAVLINK_AppData.HkTlm.SystemStatus, &decodedMsg.system_status, sizeof(uint8));
//							memcpy(&MAVLINK_AppData.HkTlm.BaseMode, &decodedMsg.base_mode, sizeof(uint8));
//							memcpy(&MAVLINK_AppData.HkTlm.CustomMode, &decodedMsg.custom_mode, sizeof(uint32));
//						}
					}
				}
			}while(MAVLINK_AppData.IngestActive == TRUE);
		}

		CFE_ES_ExitChildTask();
	}
	else
	{
		/* Can't send event or write to syslog because this task isn't registered with the cFE. */
		OS_printf("MAVLINK Listener Child Task Registration failed!\n");
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Child Task Listener Main		                                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MAVLINK_ListenerTaskMain(void)
{
	int32 Status = -1;
	mavlink_message_t 		msg;
	mavlink_status_t 		msg_status;
    uint32 					msg_size = MAVLINK_MAX_PACKET_LEN;
    MAVLINK_MsgAction_t		action = ACTION_PASSTHRU;
    int index = 0;

	Status = CFE_ES_RegisterChildTask();
	if (Status == CFE_SUCCESS)
	{
		while(MAVLINK_AppData.ListenersAlive == TRUE)
		{
			/* Ingest Loop */
			do{
				/* Receive mavlink message */
				MAVLINK_ReadMessage(MAVLINK_AppData.IngestBuffer, &msg_size);

				/* Verify message length. If equal to -1 there is no data on the socket */
				if(msg_size == -1)
				{
					msg_size = MAVLINK_MAX_PACKET_LEN;
					continue;
				}

				/* Decode the message */
				for (index = 0; index < msg_size; ++index)
				{
					if (mavlink_parse_char(MAVLINK_COMM_0, MAVLINK_AppData.IngestBuffer[index], &msg, &msg_status))
					{
						action = MAVLINK_GetMessageAction(msg);
						if(action == ACTION_HANDLE)
						{
							/* Pass to message router */
							MAVLINK_MessageRouter(msg);
						}
						else if(action == ACTION_PASSTHRU)
						{
							/* Message Pass Thru */
							MAVLINK_MessagePassThru(msg);
							CFE_EVS_SendEvent(MAVLINK_HANDLE_ERR_EID, CFE_EVS_ERROR,
													"Passing GCS msg to px4 with id: (%u)", msg.msgid);
						}
						else
						{
							/* Default behavior pass thru */
							MAVLINK_MessagePassThru(msg);
							CFE_EVS_SendEvent(MAVLINK_HANDLE_ERR_EID, CFE_EVS_ERROR,
													"Passing GCS msg to px4 with id: (%u)", msg.msgid);
						}
					}
				}
			}while(MAVLINK_AppData.IngestActive == TRUE);
		}

		CFE_ES_ExitChildTask();
	}
	else
	{
		/* Can't send event or write to syslog because this task isn't registered with the cFE. */
		OS_printf("MAVLINK Listener Child Task Registration failed!\n");
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Mavlink Pass Thru from GCS to PX4                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MAVLINK_MessagePassThru(mavlink_message_t msg)
{
	uint8 msgBuf[MAVLINK_MAX_PACKET_LEN] = {0};
	uint16 msg_size = 0;

	/* We don't handle this message. Send it to PX4 */
	msg_size = mavlink_msg_to_send_buffer(msgBuf, &msg);
	MAVLINK_SendPassThru((char *) &msgBuf, msg_size);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Mavlink Message Router	                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MAVLINK_MessageRouter(mavlink_message_t msg)
{
	int32 	Status = CFE_SUCCESS;

	switch(msg.msgid)
	{
		case MAVLINK_MSG_ID_HEARTBEAT:
		{
			mavlink_heartbeat_t 		decodedMsg;
			mavlink_msg_heartbeat_decode(&msg, &decodedMsg);
			MAVLINK_ProcessHeartbeat(decodedMsg);
			break;
		}
		case MAVLINK_MSG_ID_PARAM_REQUEST_LIST:
		{
			CFE_EVS_SendEvent(MAVLINK_HANDLE_INF_EID, CFE_EVS_INFORMATION,
							  "QGC requesting param list");
			MAVLINK_MessagePassThru(msg);
			//Status = MAVLINK_HandleRequestParams();
			break;
		}
		case MAVLINK_MSG_ID_PARAM_SET:
		{
			mavlink_param_set_t decodedMsg;
			mavlink_msg_param_set_decode(&msg, &decodedMsg);
			CFE_EVS_SendEvent(MAVLINK_HANDLE_INF_EID, CFE_EVS_INFORMATION,
										  "QGC setting param: %s",
										  decodedMsg.param_id);

			Status = MAVLINK_HandleSetParam(decodedMsg);
			break;
		}
		case MAVLINK_MSG_ID_PARAM_REQUEST_READ:
		{
			mavlink_param_request_read_t decodedMsg;
			mavlink_msg_param_request_read_decode(&msg, &decodedMsg);
			CFE_EVS_SendEvent(MAVLINK_HANDLE_INF_EID, CFE_EVS_INFORMATION,
							  "QGC requseting specified param at index: %i", decodedMsg.param_index);
			MAVLINK_MessagePassThru(msg);

			//Status = MAVLINK_HandleRequestParamRead(decodedMsg);
			break;
		}
		case MAVLINK_MSG_ID_COMMAND_LONG:
		{
			CFE_EVS_SendEvent(MAVLINK_HANDLE_INF_EID, CFE_EVS_INFORMATION,
							  "QGC sending command long");
			mavlink_command_long_t 		decodedMsg;
			mavlink_msg_command_long_decode(&msg, &decodedMsg);

			MAVLINK_HandleCommandLong(decodedMsg);
			break;
		}
		case MAVLINK_MSG_ID_MISSION_REQUEST_LIST:
		{
			CFE_EVS_SendEvent(MAVLINK_HANDLE_INF_EID, CFE_EVS_INFORMATION,
							  "QGC requseting mission list");

			Status = MAVLINK_HandleRequestMission();
			break;
		}
		case MAVLINK_MSG_ID_MISSION_ACK:
		{
			CFE_EVS_SendEvent(MAVLINK_HANDLE_INF_EID, CFE_EVS_INFORMATION,
							  "QGC ACK mission list");
			break;
		}
		default:
			OS_printf("\nReceived unknown packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\n", msg.sysid, msg.compid, msg.len, msg.msgid);
			break;
	}

	if (Status != CFE_SUCCESS)
	{
		CFE_EVS_SendEvent(MAVLINK_HANDLE_ERR_EID, CFE_EVS_ERROR,
						"Error handling mavlink msg with id: (%u)", msg.msgid);
	}
}

boolean MAVLINK_VerifyParamForAirliner(MAVLINK_ParamData_t param)
{
	boolean ReturnValue = FALSE;

	/* Verify that system and component IDs are correct */
	if(param.vehicle_id == MAVLINK_SYSTEM_ID && param.component_id == MAVLINK_COMPONENT_ID)
	{
		ReturnValue = TRUE;
	}

	return ReturnValue;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Handle Request Parameter List                                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MAVLINK_HandleRequestParams()
{
	int32 				Status = CFE_SUCCESS;
	CFE_SB_MsgPtr_t 	CmdMsgPtr;
	uint16 				ParamCount = 0;
	MAVLINK_ParamData_t param_data = {0};
	PRMLIB_ParamData_t  params[PRMLIB_PARAM_TBL_MAX_ENTRY];
    int i = 0;

	/* Get params from lib */
	PRMLIB_GetParams(params, &ParamCount);

	/* Iterate over params and send to GCS */
	for(i = 0; i < ParamCount; ++i)
	{
		/* Copy into mavlink format */
		param_data.vehicle_id = MAVLINK_SYSTEM_ID;
		param_data.component_id = MAVLINK_COMPONENT_ID;
		param_data.type = params[i].type;
		strcpy(param_data.name, params[i].name);
		memcpy(&param_data.value, &params[i].value, sizeof(param_data.value));
		MAVLINK_SendParamToGCS(param_data, i, ParamCount);
	}

	return Status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Handle Set Parameter			                                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MAVLINK_HandleSetParam(mavlink_param_set_t param)
{
	int32 				Status = CFE_SUCCESS;
	MAVLINK_ParamData_t param_data;
	PRMLIB_ParamData_t  param_lib_data;
	uint16 				ParamCount = 0;
	uint16 				ParamIndex = 0;
	PRMLIB_UpdatedParamMsg_t UpdatedMsg = {0};
	CFE_SB_MsgPtr_t MsgPtr = (CFE_SB_MsgPtr_t) &UpdatedMsg;

	/* Copy data from msg to mav format */
	param_data.vehicle_id = param.target_system;
	param_data.component_id = param.target_component;
	param_data.type = param.param_type;
	memcpy(&param_data.value, &param.param_value, sizeof(param_data.value));
	strcpy(param_data.name, param.param_id);

	if(MAVLINK_VerifyParamForAirliner(param_data) != TRUE)
	{
		Status = -1;
		goto MAVLINK_HandleSetParam_Exit_Tag;
	}

	/* Copy data from msg to lib format */
	param_lib_data.type = param.param_type;
	strcpy(param_lib_data.name, param.param_id);
	PRMLIB_SetParamValue(&param_lib_data, &param_data.value);

	/* Check if param exists */
	if(PRMLIB_ParamExists(param_lib_data.name) == TRUE)
	{
		Status = PRMLIB_UpdateParam(param_lib_data);
	}
	else
	{
		//TODO: verify expected
		//Status = PRMLIB_AddParam(param_data);
	}

	if(Status != CFE_SUCCESS)
	{
		CFE_EVS_SendEvent(MAVLINK_HANDLE_ERR_EID, CFE_EVS_ERROR,
						 "Error setting param in param lib");
		goto MAVLINK_HandleSetParam_Exit_Tag;
	}

	/* Notify apps a param has changed */
	strcpy(UpdatedMsg.name, param.param_id);
	CFE_SB_InitMsg(MsgPtr, PRMLIB_PARAM_UPDATED_MID, sizeof(PRMLIB_UpdatedParamMsg_t), FALSE);
	Status = CFE_SB_SendMsg(MsgPtr);

	if(Status != CFE_SUCCESS)
	{
		CFE_EVS_SendEvent(MAVLINK_HANDLE_ERR_EID, CFE_EVS_ERROR,
						 "Error sending set param to SB");
		goto MAVLINK_HandleSetParam_Exit_Tag;
	}

	/* Update param with values stored in prmlib and send back to GCS */
	PRMLIB_GetParamData(&param_lib_data, &ParamIndex, &ParamCount);

	/* Copy data from lib back to mav format */
	param_data.type = param_lib_data.type;
	strcpy(param_data.name, param_lib_data.name);
	memcpy(&param_data.value, &param_lib_data.value, sizeof(param_data.value));

	MAVLINK_SendParamToGCS(param_data, ParamIndex, ParamCount);

MAVLINK_HandleSetParam_Exit_Tag:

	if (Status != CFE_SUCCESS)
	{
		CFE_EVS_SendEvent(MAVLINK_HANDLE_ERR_EID, CFE_EVS_ERROR,
						 "Error encountered setting param: (%s)", param.param_id);
	}

	return Status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Handle Request Parameter Read                                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MAVLINK_HandleRequestParamRead(mavlink_param_request_read_t param)
{
	int32 				Status = CFE_SUCCESS;
	MAVLINK_ParamData_t param_data = {0};
	PRMLIB_ParamData_t  param_lib_data = {0};
	uint16 				ParamCount = 0;
	uint16 				ParamIndex = 0;

	/* Copy data from msg */
	param_data.vehicle_id = param.target_system;
	param_data.component_id = param.target_component;
	strcpy(param_data.name, param.param_id);
	ParamIndex = param.param_index;

	/* Copy data from msg to lib format */
	strcpy(param_lib_data.name, param.param_id);

	/* Check if index is specified */
	if(param.param_index == -1)
	{
		Status = PRMLIB_GetParamData(&param_lib_data, &ParamIndex, &ParamCount);
	}
	else
	{
		Status = PRMLIB_GetParamDataAtIndex(&param_lib_data, ParamIndex);
		ParamCount = PRMLIB_GetParamCount();
	}

	/* Copy data back into mav format */
	PRMLIB_GetParamValue(param_lib_data, &param_data.value);
	param_data.type = param_lib_data.type;
	strcpy(param_data.name, param_lib_data.name);

	MAVLINK_SendParamToGCS(param_data, ParamIndex, ParamCount);

	return Status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Handle Request Mission List                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MAVLINK_HandleRequestMission()
{
	int32 Status = CFE_SUCCESS;
	mavlink_mission_count_t missionMsg;
	mavlink_message_t msg 	= {0};
	uint16 msg_size 		= 0;
	uint8 msgBuf[MAVLINK_MAX_PACKET_LEN] = {0};

	/* Reply with mission count zero for now */
	missionMsg.count = 0;
	missionMsg.target_system = MAVLINK_SYSTEM_ID;
	missionMsg.target_component = MAVLINK_COMPONENT_ID;

	/* Encode mavlink message and send to ground station */
	mavlink_msg_mission_count_encode(MAVLINK_SYSTEM_ID, MAVLINK_COMPONENT_ID, &msg, &missionMsg);
	msg_size = mavlink_msg_to_send_buffer(msgBuf, &msg);
	MAVLINK_SendMessage((char *) &msgBuf, msg_size);
	return Status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Handle Command Long		                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MAVLINK_HandleCommandLong(mavlink_command_long_t msg)
{
	int32 Status = CFE_SUCCESS;
	uint16 msg_size 		= 0;
	uint8 msgBuf[MAVLINK_MAX_PACKET_LEN] = {0};

	OS_printf("Command long param: %i\n", msg.command);
	OS_printf("Command long param: %f\n", msg.param1);
	OS_printf("Command long param: %f\n", msg.param2);
	OS_printf("Command long param: %f\n", msg.param3);
	OS_printf("Command long param: %f\n", msg.param4);
	OS_printf("Command long param: %f\n", msg.param5);
	OS_printf("Command long param: %f\n", msg.param6);
	OS_printf("Command long param: %f\n", msg.param7);


	return Status;
}


void MAVLINK_ProcessHeartbeat(mavlink_heartbeat_t heartbeat)
{
	if(heartbeat.type == MAV_TYPE_GCS)
	{
		//OS_printf("Found QGC heartbeat\n");
	}
	else if(heartbeat.type == MAV_TYPE_OCTOROTOR)
	{
		OS_printf("Found our heartbeat\n");
	}
	else
	{
		OS_printf("Found UNKNOWN heartbeat\n");
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Check for Initialized Parameters                                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MAVLINK_CheckParamsInit()
{
	// TODO: Find better way to do this
	if(MAVLINK_AppData.WakeupCount > MAVLINK_HEARTBEAT_WAIT_CYCLES)
	{
		MAVLINK_EnableConnection();
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send Heartbeat				                                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MAVLINK_SendHeartbeat(void)
{
	uint8 system_id 		= MAVLINK_SYSTEM_ID;
	uint8 component_id 		= MAV_COMP_ID_AUTOPILOT1;
	mavlink_message_t msg 	= {0};
	uint8 type				= MAV_TYPE_OCTOROTOR;
	uint8 autopilot 		= MAV_AUTOPILOT_GENERIC;
	uint8 base_mode 		= MAVLINK_AppData.HkTlm.BaseMode;
	uint32 custom_mode 		= MAVLINK_AppData.HkTlm.CustomMode;
	uint8 system_status 	= MAVLINK_AppData.HkTlm.SystemStatus;
	uint16 msg_size 		= 0;
	uint8 msgBuf[MAVLINK_MAX_PACKET_LEN] = {0};

	MAVLINK_CheckParamsInit();

	if (MAVLINK_AppData.HkTlm.HeartbeatActive == TRUE)
	{
		mavlink_msg_heartbeat_pack(system_id, component_id, &msg, type,
											 autopilot, base_mode, custom_mode, system_status);
		msg_size = mavlink_msg_to_send_buffer(msgBuf, &msg);

		//MAVLINK_SendMessage((char *) &msgBuf, msg_size);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send Parameter to GCS		                                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MAVLINK_SendParamToGCS(MAVLINK_ParamData_t param_data, uint16 param_index, uint16 param_count)
{
	mavlink_message_t msg 	= {0};
	mavlink_param_value_t param = {0};
	uint16 msg_size 		= 0;
	uint8 msgBuf[MAVLINK_MAX_PACKET_LEN] = {0};

	/* Copy values from params msg mavlink msg */
	param.param_index = param_index;
	param.param_count = param_count;
	strcpy(param.param_id, param_data.name);
	param.param_id[MAVLINK_MSG_PARAM_VALUE_FIELD_PARAM_ID_LEN] = '\0';
	param.param_type = param_data.type;
	memcpy(&param.param_value, &param_data.value, sizeof(param_data.value));

	/* Encode mavlink message and send to ground station */
	mavlink_msg_param_value_encode(MAVLINK_SYSTEM_ID, MAVLINK_COMPONENT_ID, &msg, &param);
	msg_size = mavlink_msg_to_send_buffer(msgBuf, &msg);
    MAVLINK_SendMessage((char *) &msgBuf, msg_size);
}

void MAVLINK_PrintParam(MAVLINK_ParamData_t param) // TODO Remove
{
	uint8 val_uint8 	= 0;
	int8 val_int8 		= 0;
	uint16 val_uint16 	= 0;
	int16 val_int16		= 0;
	uint32 val_uint32	= 0;
	int32 val_int32 	= 0;

	OS_printf("name: %s \n", param.name);
	OS_printf("type: %u \n", param.type);
	switch(param.type)
	{
		case TYPE_UINT32:
			memcpy(&val_uint32, &param.value, sizeof(uint32));
			OS_printf("uint value: %u \n", val_uint32);
			break;
		case TYPE_REAL32:
			OS_printf("value: %f \n", param.value);
			break;
		default:
			//unsupported type
			break;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 MAVLINK_RcvMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(MAVLINK_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, MAVLINK_AppData.SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(MAVLINK_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
	{
            case MAVLINK_WAKEUP_MID:
                MAVLINK_SendHeartbeat();
                MAVLINK_ProcessNewCmds();
                MAVLINK_ProcessNewData();
                /* TODO:  Add more code here to handle other things when app wakes up */

                /* The last thing to do at the end of this Wakeup cycle should be to
                 * automatically publish new output. */
                MAVLINK_AppData.WakeupCount++;
                MAVLINK_SendOutData();
                break;

            case MAVLINK_SEND_HK_MID:
                MAVLINK_ReportHousekeeping();
                break;

            default:
                (void) CFE_EVS_SendEvent(MAVLINK_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid SCH msgId (0x%04X)", (unsigned short)MsgId);
        }
    }
    else if (iStatus == CFE_SB_NO_MESSAGE)
    {
        /* TODO: If there's no incoming message, you can do something here, or 
         * nothing.  Note, this section is dead code only if the iBlocking arg
         * is CFE_SB_PEND_FOREVER. */
        iStatus = CFE_SUCCESS;
    }
    else if (iStatus == CFE_SB_TIME_OUT)
    {
        /* TODO: If there's no incoming message within a specified time (via the
         * iBlocking arg, you can do something here, or nothing.  
         * Note, this section is dead code only if the iBlocking arg
         * is CFE_SB_PEND_FOREVER. */
        iStatus = CFE_SUCCESS;
    }
    else
    {
        /* TODO: This is an example of exiting on an error (either CFE_SB_BAD_ARGUMENT, or
         * CFE_SB_PIPE_RD_ERROR).
         */
        (void) CFE_EVS_SendEvent(MAVLINK_PIPE_ERR_EID, CFE_EVS_ERROR,
			  "SB pipe read error (0x%08X), app will exit", (unsigned int)iStatus);
        MAVLINK_AppData.uiRunStatus= CFE_ES_APP_ERROR;
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Data                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void MAVLINK_ProcessNewData()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   DataMsgPtr=NULL;
    CFE_SB_MsgId_t  DataMsgId;

    /* Process telemetry messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&DataMsgPtr, MAVLINK_AppData.DataPipeId, CFE_SB_POLL);
        if (iStatus == CFE_SUCCESS)
        {
            DataMsgId = CFE_SB_GetMsgId(DataMsgPtr);
            switch (DataMsgId)
            {
                default:
                    (void) CFE_EVS_SendEvent(MAVLINK_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "Recvd invalid data msgId (0x%04X)", (unsigned short)DataMsgId);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            (void) CFE_EVS_SendEvent(MAVLINK_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "Data pipe read error (0x%08X)", (unsigned int)iStatus);
            MAVLINK_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void MAVLINK_ProcessNewCmds()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr=NULL;
    CFE_SB_MsgId_t  CmdMsgId;

    /* Process command messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, MAVLINK_AppData.CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case MAVLINK_CMD_MID:
                    MAVLINK_ProcessNewAppCmds(CmdMsgPtr);
                    break;

                default:
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with this
                     *  pipe, but not handled in this switch-case.) */
                    MAVLINK_AppData.HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(MAVLINK_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "Recvd invalid CMD msgId (0x%04X)", (unsigned short)CmdMsgId);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            (void) CFE_EVS_SendEvent(MAVLINK_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08X)", (unsigned int)iStatus);
            MAVLINK_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process MAVLINK Commands                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void MAVLINK_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case MAVLINK_NOOP_CC:
                MAVLINK_AppData.HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(MAVLINK_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "Recvd NOOP cmd (%u), Version %d.%d.%d.%d",
                                  (unsigned int)uiCmdCode,
                                  MAVLINK_MAJOR_VERSION,
                                  MAVLINK_MINOR_VERSION,
                                  MAVLINK_REVISION,
                                  MAVLINK_MISSION_REV);
                break;

            case MAVLINK_RESET_CC:
                MAVLINK_AppData.HkTlm.usCmdCnt = 0;
                MAVLINK_AppData.HkTlm.usCmdErrCnt = 0;
                MAVLINK_AppData.HkTlm.HeartbeatActive = FALSE;
                (void) CFE_EVS_SendEvent(MAVLINK_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "Recvd RESET cmd (%u)", (unsigned int)uiCmdCode);
                break;

            default:
                MAVLINK_AppData.HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(MAVLINK_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid cmdId (%u)", (unsigned int)uiCmdCode);
                break;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send MAVLINK Housekeeping                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void MAVLINK_ReportHousekeeping()
{
    /* TODO:  Add code to update housekeeping data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&MAVLINK_AppData.HkTlm);
    int32 iStatus = CFE_SB_SendMsg((CFE_SB_Msg_t*)&MAVLINK_AppData.HkTlm);
    if (iStatus != CFE_SUCCESS)
    {
        /* TODO: Decide what to do if the send message fails. */
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Publish Output Data                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void MAVLINK_SendOutData()
{
    /* TODO:  Add code to update output data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&MAVLINK_AppData.OutData);
    int32 iStatus = CFE_SB_SendMsg((CFE_SB_Msg_t*)&MAVLINK_AppData.OutData);
    if (iStatus != CFE_SUCCESS)
    {
        /* TODO: Decide what to do if the send message fails. */
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Command Length                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

boolean MAVLINK_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
                           uint16 usExpectedLen)
{
    boolean bResult  = TRUE;
    uint16  usMsgLen = 0;

    if (MsgPtr != NULL)
    {
        usMsgLen = CFE_SB_GetTotalMsgLength(MsgPtr);

        if (usExpectedLen != usMsgLen)
        {
            bResult = FALSE;
            CFE_SB_MsgId_t MsgId = CFE_SB_GetMsgId(MsgPtr);
            uint16 usCmdCode = CFE_SB_GetCmdCode(MsgPtr);

            (void) CFE_EVS_SendEvent(MAVLINK_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            MAVLINK_AppData.HkTlm.usCmdErrCnt++;
        }
    }

    return (bResult);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* MAVLINK application entry point and main process loop           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void MAVLINK_AppMain()
{
    /* Register the application with Executive Services */
    MAVLINK_AppData.uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("MAVLINK - Failed to register the app (0x%08X)\n", (unsigned int)iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(MAVLINK_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = MAVLINK_InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(MAVLINK_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(MAVLINK_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(MAVLINK_MAIN_TASK_PERF_ID);
    }
    else
    {
        MAVLINK_AppData.uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&MAVLINK_AppData.uiRunStatus) == TRUE)
    {
        int32 iStatus = MAVLINK_RcvMsg(MAVLINK_SCH_PIPE_PEND_TIME);
        if (iStatus != CFE_SUCCESS)
        {
            /* TODO: Decide what to do for other return values in MAVLINK_RcvMsg(). */
        }
        /* TODO: This is only a suggestion for when to update and save CDS table.
        ** Depends on the nature of the application, the frequency of update
        ** and save can be more or less independently.
        */
        MAVLINK_UpdateCdsTbl();
        MAVLINK_SaveCdsTbl();
        iStatus = MAVLINK_AcquireParamPointers();
        if(iStatus != CFE_SUCCESS)
        {
            /* We apparently tried to load a new table but failed.  Terminate the application. */
            MAVLINK_AppData.uiRunStatus = CFE_ES_APP_ERROR;
        }

    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(MAVLINK_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(MAVLINK_AppData.uiRunStatus);
}

/************************/
/*  End of File Comment */
/************************/
