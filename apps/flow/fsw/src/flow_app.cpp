/************************************************************************
** Includes
*************************************************************************/
#include <string.h>
#include <cmath>
#include <math.h>

#include "cfe.h"

#include "flow_app.h"
#include "flow_msg.h"
#include "flow_verify.h"
#include "flow_version.h"
#include "px4lib_msgids.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Instantiate the application object.                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
FLOW oFLOW;



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Default constructor.                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
FLOW::FLOW()
{

}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Destructor constructor.                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
FLOW::~FLOW()
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables.                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 FLOW::InitEvent()
{
    int32  iStatus=CFE_SUCCESS;
    int32  i = 0;

    CFE_EVS_BinFilter_t EventTbl[FLOW_MAX_EVENT_FILTER];
    /* Initalize event filter tables */
    CFE_PSP_MemSet(EventTbl, 0x00 , sizeof(EventTbl));

	EventTbl[i].EventID = FLOW_RESERVED_EID;
	EventTbl[i++].Mask    = CFE_EVS_NO_FILTER;

	EventTbl[i].EventID = FLOW_INIT_INF_EID;
	EventTbl[i++].Mask    = CFE_EVS_NO_FILTER;

	EventTbl[i].EventID = FLOW_CMD_NOOP_EID;
	EventTbl[i++].Mask    = CFE_EVS_NO_FILTER;

	EventTbl[i].EventID = FLOW_SUBSCRIBE_ERR_EID;
	EventTbl[i++].Mask    = CFE_EVS_NO_FILTER;

	EventTbl[i].EventID = FLOW_PIPE_INIT_ERR_EID;
	EventTbl[i++].Mask    = CFE_EVS_NO_FILTER;

	EventTbl[i].EventID = FLOW_CFGTBL_MANAGE_ERR_EID;
	EventTbl[i++].Mask    = CFE_EVS_NO_FILTER;

	EventTbl[i].EventID = FLOW_CFGTBL_GETADDR_ERR_EID;
	EventTbl[i++].Mask    = CFE_EVS_NO_FILTER;

	EventTbl[i].EventID = FLOW_RCVMSG_ERR_EID;
	EventTbl[i++].Mask    = CFE_EVS_NO_FILTER;

	EventTbl[i].EventID = FLOW_MSGID_ERR_EID;
	EventTbl[i++].Mask    = CFE_EVS_NO_FILTER;

	EventTbl[i].EventID = FLOW_CC_ERR_EID;
	EventTbl[i++].Mask    = CFE_EVS_NO_FILTER;

	EventTbl[i].EventID = FLOW_MSGLEN_ERR_EID;
	EventTbl[i++].Mask    = CFE_EVS_NO_FILTER;

	EventTbl[i].EventID = FLOW_CFGTBL_REG_ERR_EID;
	EventTbl[i++].Mask    = CFE_EVS_NO_FILTER;

	EventTbl[i].EventID = FLOW_CFGTBL_LOAD_ERR_EID;
	EventTbl[i++].Mask    = CFE_EVS_NO_FILTER;

	EventTbl[i].EventID = FLOW_CFGTBL_VALIDATE_ERR_EID;
	EventTbl[i++].Mask    = CFE_EVS_NO_FILTER;

	EventTbl[i].EventID = FLOW_CMD_ACCEPTED_EID;
	EventTbl[i++].Mask    = CFE_EVS_NO_FILTER;

	EventTbl[i].EventID = FLOW_CMD_REJECTED_ERR_EID;
	EventTbl[i++].Mask    = CFE_EVS_NO_FILTER;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(EventTbl, i, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("FLOW - Failed to register with EVS (0x%08lX)\n", iStatus);
    }

    return iStatus;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 FLOW::InitPipe()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&SchPipeId,
    		FLOW_SCH_PIPE_DEPTH,
			FLOW_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(FLOW_WAKEUP_MID, SchPipeId, CFE_SB_Default_Qos, FLOW_WAKEUP_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(FLOW_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
            		"Sch Pipe failed to subscribe to FLOW_WAKEUP_MID. (0x%08lX)",
                    iStatus);
            goto FLOW_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(FLOW_SEND_HK_MID, SchPipeId, CFE_SB_Default_Qos, FLOW_SEND_HK_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(FLOW_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to FLOW_SEND_HK_MID. (0x%08X)",
					 (unsigned int)iStatus);
            goto FLOW_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(FLOW_FRAME_MID, SchPipeId, CFE_SB_Default_Qos, 1);
		if (iStatus != CFE_SUCCESS)
		{
			(void) CFE_EVS_SendEvent(FLOW_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to FLOW_FRAME_MID. (0x%08lX)",
					 iStatus);
			goto FLOW_InitPipe_Exit_Tag;
		}
    }
    else
    {
        (void) CFE_EVS_SendEvent(FLOW_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
			 "Failed to create SCH pipe (0x%08lX)",
			 iStatus);
        goto FLOW_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&CmdPipeId,
    		FLOW_CMD_PIPE_DEPTH,
			FLOW_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(FLOW_CMD_MID, CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(FLOW_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
				 "CMD Pipe failed to subscribe to FLOW_CMD_MID. (0x%08lX)",
				 iStatus);
            goto FLOW_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(FLOW_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
			 "Failed to create CMD pipe (0x%08lX)",
			 iStatus);
        goto FLOW_InitPipe_Exit_Tag;
    }

FLOW_InitPipe_Exit_Tag:
    return iStatus;
}
    
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Dedicated Gyro Pipes                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 FLOW::InitGyroPipe()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&GyroPipeId,
    		FLOW_GYRO_PIPE_DEPTH,
			FLOW_GYRO_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(PX4_SENSOR_GYRO_MID, GyroPipeId, CFE_SB_Default_Qos, FLOW_SENSOR_GYRO_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(FLOW_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
            		"GYRO Pipe failed to subscribe to PX4_SENSOR_GYRO_MID. (0x%08lX)",
                    iStatus);
            goto FLOW_InitPipe_Exit_Tag;
        }

    }
    else
    {
        (void) CFE_EVS_SendEvent(FLOW_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
			 "Failed to create GYRO pipe (0x%08lX)",
			 iStatus);
        goto FLOW_InitPipe_Exit_Tag;
    }

FLOW_InitPipe_Exit_Tag:
    return iStatus;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void FLOW::InitData()
{
	/* Init housekeeping message. */
	CFE_SB_InitMsg(&HkTlm, FLOW_HK_TLM_MID, sizeof(HkTlm), TRUE);
	/* Init output messages */
	CFE_SB_InitMsg(&OpticalFlowMsg, PX4_OPTICAL_FLOW_MID, sizeof(OpticalFlowMsg), TRUE);
	/* Init housekeeping message. */
	CFE_SB_InitMsg(&DiagTlm, FLOW_DIAG_TLM_MID, sizeof(DiagTlm), TRUE);

}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FLOW initialization                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 FLOW::InitApp()
{
    int32  iStatus   = CFE_SUCCESS;
    int8   hasEvents = 0;

    iStatus = InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("FLOW - Failed to init events (0x%08lX)\n", iStatus);
        goto FLOW_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        goto FLOW_InitApp_Exit_Tag;
    }

    iStatus = InitGyroPipe();
    if (iStatus != CFE_SUCCESS)
    {
    	goto FLOW_InitApp_Exit_Tag;
    }

    InitData();

    iStatus = InitConfigTbl();
    if (iStatus != CFE_SUCCESS)
    {
        goto FLOW_InitApp_Exit_Tag;
    }

    /* Initialize Flow Calculation Data */
	InitFlowApp();

FLOW_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(FLOW_INIT_INF_EID, CFE_EVS_INFORMATION,
                                 "Initialized.  Version %d.%d.%d.%d",
								 FLOW_MAJOR_VERSION,
								 FLOW_MINOR_VERSION,
								 FLOW_REVISION,
								 FLOW_MISSION_REV);
    }
    else
    {
        if (hasEvents == 1)
        {
            (void) CFE_EVS_SendEvent(FLOW_INIT_ERR_EID, CFE_EVS_ERROR, "Application failed to initialize");
        }
        else
        {
            (void) CFE_ES_WriteToSysLog("FLOW - Application failed to initialize\n");
        }
    }

    return iStatus;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 FLOW::RcvSchPipeMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    int32           iStatus2 = CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(FLOW_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(FLOW_MAIN_TASK_PERF_ID);


    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
        {
            case FLOW_WAKEUP_MID:
            {
                /* 10hz - Execute */
            	ProcessGyroPipe();
            	CFE_ES_PerfLogEntry(FLOW_COMPUTE_PERF_ID);
            	CalculateOpticalFlow();
            	CFE_ES_PerfLogExit(FLOW_COMPUTE_PERF_ID);
            	break;
            }
            case FLOW_SEND_HK_MID:
            {
                ProcessCmdPipe();
                ReportHousekeeping();
                break;
            }
            case FLOW_FRAME_MID:
            {
				CFE_PSP_MemCpy(&CVT.OpticalFlowFrameMsg, MsgPtr, sizeof(CVT.OpticalFlowFrameMsg));
				break;
            }
            default:
            {
                (void) CFE_EVS_SendEvent(FLOW_MSGID_ERR_EID, CFE_EVS_ERROR,
                     "Recvd invalid SCH msgId (0x%04X)", MsgId);
            }

        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(FLOW_RCVMSG_ERR_EID, CFE_EVS_ERROR,
			  "SCH pipe read error (0x%08lX).", iStatus);
    }

    return iStatus;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Gyro Messages                                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void FLOW::ProcessGyroPipe()
{
    int32 iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   GyroMsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Reset gyro rate integral to zero */
	HkTlm.GyroXRateIntegral = 0;
	HkTlm.GyroYRateIntegral = 0;
	HkTlm.GyroZRateIntegral = 0;

	int32 i;
    for(i = 0; i < FLOW_GYRO_PIPE_DEPTH; i++)
    {
    	/* Get message pointer */
        iStatus = CFE_SB_RcvMsg(&GyroMsgPtr, GyroPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
        		/* Get message ID */
                MsgId = CFE_SB_GetMsgId(GyroMsgPtr);
                if(MsgId == PX4_SENSOR_GYRO_MID)
                {
                        CFE_PSP_MemCpy(&CVT.SensorGyroMsg, GyroMsgPtr, sizeof(CVT.SensorGyroMsg));

                        /* Accumulate gyro measurements that are needed for the optical flow message */
                        if(HkTlm.LastGyroMsgAppeared == 0)
                        {
                        	HkTlm.LastGyroMsgAppeared = CVT.SensorGyroMsg.Timestamp;
                        }
                        uint64 GyroTimeDifference = CVT.SensorGyroMsg.Timestamp - HkTlm.LastGyroMsgAppeared;

                        /* Integrate only if current sensor arrival time is after a minimum defined time */
                        if (GyroTimeDifference > FLOW_MIN_INT_TIME_DIFF)
                        {
                        		/* Accumulate/Integrate gyrosocpic rates in x, y and z axis (rad/sec)*/
                                HkTlm.GyroXRateIntegral += CVT.SensorGyroMsg.X * (GyroTimeDifference / FLOW_US_IN_SEC);
                                HkTlm.GyroYRateIntegral += CVT.SensorGyroMsg.Y * (GyroTimeDifference / FLOW_US_IN_SEC);
                                HkTlm.GyroZRateIntegral += CVT.SensorGyroMsg.Z * (GyroTimeDifference / FLOW_US_IN_SEC);
                                HkTlm.LastGyroMsgAppeared = CVT.SensorGyroMsg.Timestamp;
                        }
                }

        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
		{
        		/* This condition is reached when maximum msg count is less than pipe depth */
				break;
		}
		else
		{
				(void) CFE_EVS_SendEvent(FLOW_RCVMSG_ERR_EID, CFE_EVS_ERROR,
						  "GYRO pipe read error (0x%08lX)", iStatus);
				break;
		}

    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FLOW::ProcessCmdPipe()
{
    int32 iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr=NULL;
    CFE_SB_MsgId_t  CmdMsgId;

    /* Process command messages until the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case FLOW_CMD_MID:
                {
                    ProcessAppCmds(CmdMsgPtr);
                    break;
                }
                default:
                {
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with this
                     *  pipe, but not handled in this switch-case.) */
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(FLOW_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "Recvd invalid CMD msgId (0x%04X)", (unsigned short)CmdMsgId);
                    break;
                }
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            (void) CFE_EVS_SendEvent(FLOW_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08lX)", iStatus);
            break;
        }
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process FLOW Commands                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FLOW::ProcessAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case FLOW_NOOP_CC:
            {
            	if(VerifyCmdLength(MsgPtr, sizeof(FLOW_NoArgCmd_t)))
            	{
                    HkTlm.usCmdCnt++;
                    (void) CFE_EVS_SendEvent(FLOW_CMD_NOOP_EID, CFE_EVS_INFORMATION,
    					"Recvd NOOP. Version %d.%d.%d.%d",
    					FLOW_MAJOR_VERSION,
    					FLOW_MINOR_VERSION,
    					FLOW_REVISION,
    					FLOW_MISSION_REV);
                    break;
            	}
            }
            case FLOW_SEND_DIAG_CC:
            {
            	if(VerifyCmdLength(MsgPtr, sizeof(FLOW_NoArgCmd_t)))
            	{
                	/** Send diagnostic message */
    				HkTlm.usCmdCnt++;
    				SendDiagMsg();
    				(void) CFE_EVS_SendEvent(FLOW_CMD_ACCEPTED_EID, CFE_EVS_INFORMATION,
    									"Diagnostic message is sent");
                	break;
            	}
            }
            case FLOW_RESET_CC:
            {
            	if(VerifyCmdLength(MsgPtr, sizeof(FLOW_NoArgCmd_t)))
            	{
                    HkTlm.usCmdCnt = 0;
                    HkTlm.usCmdErrCnt = 0;
                    break;
            	}
            }
            case FLOW_SET_HFOV_CC:
            {
            	/** Sliding window tile size */
            	if(VerifyCmdLength(MsgPtr, sizeof(FLOW_SetFloat_t)))
            	{
            		FLOW_SetFloat_t *cmd = (FLOW_SetFloat_t*)MsgPtr;
            		if(cmd == nullptr
            				|| (cmd->value < FLOW_HOV_MIN
							| 	cmd->value > FLOW_HOV_MAX))
            		{
            			HkTlm.usCmdErrCnt++;
						(void) CFE_EVS_SendEvent(FLOW_CMD_REJECTED_ERR_EID, CFE_EVS_ERROR,
											"Unable to set HFOV. Must be between %u and %lf",FLOW_HOV_MIN,FLOW_HOV_MAX);
            		}
            		else
            		{
            			ConfigTblPtr->HFOV = cmd->value;
            			CFE_TBL_Modified(ConfigTblHdl);
						HkTlm.usCmdCnt++;
						SendDiagMsg();
						InitFlowApp();
						(void) CFE_EVS_SendEvent(FLOW_CMD_ACCEPTED_EID, CFE_EVS_INFORMATION,
											"Command to set HFOV is accepted");
            		}
            	}
            	break;
            }
            case FLOW_SET_TILE_SIZE_CC:
            {
            	/** Sliding window tile size */
            	if(VerifyCmdLength(MsgPtr, sizeof(FLOW_SetInt32_t)))
            	{
            		FLOW_SetInt32_t *cmd = (FLOW_SetInt32_t*)MsgPtr;
            		if(cmd == nullptr
            				|| (cmd->value < FLOW_TILE_SIZE_MIN
							| 	cmd->value > FLOW_TILE_SIZE_MAX))
            		{
            			HkTlm.usCmdErrCnt++;
						(void) CFE_EVS_SendEvent(FLOW_CMD_REJECTED_ERR_EID, CFE_EVS_ERROR,
											"Unable to set tile size. Must be between %u and %u",FLOW_TILE_SIZE_MIN,FLOW_TILE_SIZE_MAX);
            		}
            		else
            		{
            			ConfigTblPtr->FLOW_TILE_SIZE = cmd->value;
            			CFE_TBL_Modified(ConfigTblHdl);
						HkTlm.usCmdCnt++;
						SendDiagMsg();
						InitFlowApp();
						(void) CFE_EVS_SendEvent(FLOW_CMD_ACCEPTED_EID, CFE_EVS_INFORMATION,
											"Command to set tile size is accepted");
            		}
            	}
            	break;
            }
            case FLOW_SET_BLOCKS_CC:
            {
            	/** Number of blocks of sliding window */
            	if(VerifyCmdLength(MsgPtr, sizeof(FLOW_SetInt32_t)))
            	{

            		FLOW_SetInt32_t *cmd = (FLOW_SetInt32_t*)MsgPtr;
            		if(cmd == nullptr
            				|| (cmd->value < FLOW_BLOCKS_MIN
							| 	cmd->value > FLOW_BLOCKS_MAX))
            		{
            			HkTlm.usCmdErrCnt++;
						(void) CFE_EVS_SendEvent(FLOW_CMD_REJECTED_ERR_EID, CFE_EVS_ERROR,
											"Unable to set blocks. Must be between %u and %u",FLOW_BLOCKS_MIN,FLOW_BLOCKS_MAX);
            		}
            		else
            		{
            			ConfigTblPtr->FLOW_NUM_BLOCKS = cmd->value;
            			CFE_TBL_Modified(ConfigTblHdl);
						HkTlm.usCmdCnt++;
						SendDiagMsg();
						InitFlowApp();
						(void) CFE_EVS_SendEvent(FLOW_CMD_ACCEPTED_EID, CFE_EVS_INFORMATION,
											"Command to set blocks is accepted");
            		}

            	}
            	break;
            }
            case FLOW_SET_OP_RATE_CC:
            {
            	/** Output rate */
            	if(VerifyCmdLength(MsgPtr, sizeof(FLOW_SetInt32_t)))
            	{

            		FLOW_SetInt32_t *cmd = (FLOW_SetInt32_t*)MsgPtr;
					if(cmd == nullptr
							|| (cmd->value < FLOW_OUTPUT_RATE_MIN
							| 	cmd->value > FLOW_OUTPUT_RATE_MAX))
            		{
            			HkTlm.usCmdErrCnt++;
						(void) CFE_EVS_SendEvent(FLOW_CMD_REJECTED_ERR_EID, CFE_EVS_ERROR,
											"Unable to set output rate. Must be between %u and %u",FLOW_OUTPUT_RATE_MIN,FLOW_OUTPUT_RATE_MAX);
            		}
            		else
            		{
            			ConfigTblPtr->FLOW_DEFAULT_OUTPUT_RATE = cmd->value;
            			CFE_TBL_Modified(ConfigTblHdl);
						HkTlm.usCmdCnt++;
						SendDiagMsg();
						InitFlowApp();
						(void) CFE_EVS_SendEvent(FLOW_CMD_ACCEPTED_EID, CFE_EVS_INFORMATION,
											"Command to set output rate is accepted");
            		}

            	}
            	break;
            }
            case FLOW_SET_IMG_WIDTH_CC:
            {
            	/** Image width and height */
            	if(VerifyCmdLength(MsgPtr, sizeof(FLOW_SetInt32_t)))
            	{

            		FLOW_SetInt32_t *cmd = (FLOW_SetInt32_t*)MsgPtr;
					if(cmd == nullptr
            				|| (cmd->value < FLOW_IMAGE_WIDTH_MIN
							| 	cmd->value > FLOW_IMAGE_WIDTH_MAX))
            		{
            			HkTlm.usCmdErrCnt++;
						(void) CFE_EVS_SendEvent(FLOW_CMD_REJECTED_ERR_EID, CFE_EVS_ERROR,
											"Unable to set image width. Must be between %u and %u",FLOW_IMAGE_WIDTH_MIN,FLOW_IMAGE_WIDTH_MAX);
            		}
            		else
            		{
            			ConfigTblPtr->FLOW_DEFAULT_IMAGE_WIDTH = cmd->value;
            			CFE_TBL_Modified(ConfigTblHdl);
						HkTlm.usCmdCnt++;
						SendDiagMsg();
						InitFlowApp();
						(void) CFE_EVS_SendEvent(FLOW_CMD_ACCEPTED_EID, CFE_EVS_INFORMATION,
											"Command to set image width is accepted");
            		}

            	}
            	break;
            }
            case FLOW_SET_IMG_HEIGHT_CC:
            {
            	/** Image width and height */
            	if(VerifyCmdLength(MsgPtr, sizeof(FLOW_SetInt32_t)))
            	{

            		FLOW_SetInt32_t *cmd = (FLOW_SetInt32_t*)MsgPtr;
					if(cmd == nullptr
							|| (cmd->value < FLOW_IMAGE_HEIGHT_MIN
							| 	cmd->value > FLOW_IMAGE_HEIGHT_MAX))
            		{
            			HkTlm.usCmdErrCnt++;
						(void) CFE_EVS_SendEvent(FLOW_CMD_REJECTED_ERR_EID, CFE_EVS_ERROR,
											"Unable to set image height. Must be between %u and %u",FLOW_IMAGE_HEIGHT_MIN,FLOW_IMAGE_HEIGHT_MAX);
            		}
            		else
            		{
            			ConfigTblPtr->FLOW_DEFAULT_IMAGE_HEIGHT = cmd->value;
            			CFE_TBL_Modified(ConfigTblHdl);
						HkTlm.usCmdCnt++;
						SendDiagMsg();
						InitFlowApp();
						(void) CFE_EVS_SendEvent(FLOW_CMD_ACCEPTED_EID, CFE_EVS_INFORMATION,
											"Command to set image height is accepted");
            		}

            	}
            	break;
            }
            case FLOW_SET_SEARCH_SIZE_CC:
            {
            	/** Image search size */
            	if(VerifyCmdLength(MsgPtr, sizeof(FLOW_SetInt32_t)))
            	{

            		FLOW_SetInt32_t *cmd = (FLOW_SetInt32_t*)MsgPtr;
					if(cmd == nullptr
							|| (cmd->value < FLOW_SEARCH_SIZE_MIN
							| 	cmd->value > FLOW_SEARCH_SIZE_MAX))
            		{
            			HkTlm.usCmdErrCnt++;
						(void) CFE_EVS_SendEvent(FLOW_CMD_REJECTED_ERR_EID, CFE_EVS_ERROR,
											"Unable to set search size. Must be between %u and %u",FLOW_SEARCH_SIZE_MIN,FLOW_SEARCH_SIZE_MAX);
            		}
            		else
            		{
            			ConfigTblPtr->FLOW_DEFAULT_SEARCH_SIZE = cmd->value;
            			CFE_TBL_Modified(ConfigTblHdl);
						HkTlm.usCmdCnt++;
						SendDiagMsg();
						InitFlowApp();
						(void) CFE_EVS_SendEvent(FLOW_CMD_ACCEPTED_EID, CFE_EVS_INFORMATION,
											"Command to set search size is accepted");
            		}

            	}
            	break;
            }
            case FLOW_SET_FEATURE_THRESHOLD_CC:
            {
            	/** Default feature threshold */
            	if(VerifyCmdLength(MsgPtr, sizeof(FLOW_SetInt32_t)))
            	{

            		FLOW_SetInt32_t *cmd = (FLOW_SetInt32_t*)MsgPtr;
					if(cmd == nullptr
							|| (cmd->value < FLOW_FEATURE_THRESHOLD_MIN
							| 	cmd->value > FLOW_FEATURE_THRESHOLD_MAX))
            		{
            			HkTlm.usCmdErrCnt++;
						(void) CFE_EVS_SendEvent(FLOW_CMD_REJECTED_ERR_EID, CFE_EVS_ERROR,
											"Unable to set feature threshold. Must be between %u and %u",FLOW_FEATURE_THRESHOLD_MIN,FLOW_FEATURE_THRESHOLD_MAX);
            		}
            		else
            		{
            			ConfigTblPtr->FLOW_DEFAULT_FEATURE_THRESHOLD = cmd->value;
            			CFE_TBL_Modified(ConfigTblHdl);
						HkTlm.usCmdCnt++;
						SendDiagMsg();
						InitFlowApp();
						(void) CFE_EVS_SendEvent(FLOW_CMD_ACCEPTED_EID, CFE_EVS_INFORMATION,
											"Command to set feature threshold is accepted");
            		}

            	}
            	break;
            }
            case FLOW_SET_VALUE_THRESHOLD_CC:
            {
            	/** Default value threshold */
            	if(VerifyCmdLength(MsgPtr, sizeof(FLOW_SetInt32_t)))
            	{

            		FLOW_SetInt32_t *cmd = (FLOW_SetInt32_t*)MsgPtr;
					if(cmd == nullptr
							|| (cmd->value < FLOW_VALUE_THRESHOLD_MIN
							| 	cmd->value > FLOW_VALUE_THRESHOLD_MAX))
            		{
            			HkTlm.usCmdErrCnt++;
						(void) CFE_EVS_SendEvent(FLOW_CMD_REJECTED_ERR_EID, CFE_EVS_ERROR,
											"Unable to set value threshold. Must be between %u and %u",FLOW_VALUE_THRESHOLD_MIN,FLOW_VALUE_THRESHOLD_MAX);
            		}
					else
            		{
            			ConfigTblPtr->FLOW_DEFAULT_VALUE_THRESHOLD = cmd->value;
            			CFE_TBL_Modified(ConfigTblHdl);
						HkTlm.usCmdCnt++;
						SendDiagMsg();
						InitFlowApp();
						(void) CFE_EVS_SendEvent(FLOW_CMD_ACCEPTED_EID, CFE_EVS_INFORMATION,
											"Command to set value threshold is accepted");
            		}

            	}
            	break;
            }
            case FLOW_SET_MEAN_COUNT_THRESHOLD_CC:
            {
            	/** Mean count threshold */
            	if(VerifyCmdLength(MsgPtr, sizeof(FLOW_SetInt32_t)))
            	{

            		FLOW_SetInt32_t *cmd = (FLOW_SetInt32_t*)MsgPtr;
					if(cmd == nullptr
							|| (cmd->value < FLOW_MEAN_COUNT_THRESHOLD_MIN
							| 	cmd->value > FLOW_MEAN_COUNT_THRESHOLD_MAX))
            		{
            			HkTlm.usCmdErrCnt++;
						(void) CFE_EVS_SendEvent(FLOW_CMD_REJECTED_ERR_EID, CFE_EVS_ERROR,
											"Unable to set mean count threshold. Must be between %u and %u",FLOW_MEAN_COUNT_THRESHOLD_MIN,FLOW_MEAN_COUNT_THRESHOLD_MAX);
            		}
            		else
            		{
            			ConfigTblPtr->FLOW_MEAN_COUNT_THRESHOLD = cmd->value;
            			CFE_TBL_Modified(ConfigTblHdl);
						HkTlm.usCmdCnt++;
						SendDiagMsg();
						InitFlowApp();
						(void) CFE_EVS_SendEvent(FLOW_CMD_ACCEPTED_EID, CFE_EVS_INFORMATION,
											"Command to set mean count threshold is accepted");
            		}

            	}
            	break;
            }
            default:
            {
                HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(FLOW_CC_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid command code (%u)", (unsigned int)uiCmdCode);
                break;
            }
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send FLOW Housekeeping                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FLOW::ReportHousekeeping()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&HkTlm);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Publish Output Data                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void FLOW::SendOpticalFlowMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&OpticalFlowMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&OpticalFlowMsg);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Publish Diagnostic Data                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void FLOW::SendDiagMsg()
{
    if (ConfigTblPtr != nullptr) {

    	DiagTlm.TileSize 				= ConfigTblPtr->FLOW_TILE_SIZE;
    	DiagTlm.Blocks 				    = ConfigTblPtr->FLOW_NUM_BLOCKS;
    	DiagTlm.OutputRate 			    = ConfigTblPtr->FLOW_DEFAULT_OUTPUT_RATE;
    	DiagTlm.ImgWidth 				= ConfigTblPtr->FLOW_DEFAULT_IMAGE_WIDTH;
    	DiagTlm.ImgHeight 				= ConfigTblPtr->FLOW_DEFAULT_IMAGE_HEIGHT;
    	DiagTlm.SearchSize 			    = ConfigTblPtr->FLOW_DEFAULT_SEARCH_SIZE;
    	DiagTlm.FeatureThreshold 		= ConfigTblPtr->FLOW_DEFAULT_FEATURE_THRESHOLD;
    	DiagTlm.ValueThreshold 		    = ConfigTblPtr->FLOW_DEFAULT_VALUE_THRESHOLD;
    	DiagTlm.MeancountThreshold 	    = ConfigTblPtr->FLOW_MEAN_COUNT_THRESHOLD;
    	DiagTlm.HFOV 					= ConfigTblPtr->HFOV;

    }

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&DiagTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&DiagTlm);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Command Length                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool FLOW::VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
                           uint16 usExpectedLen)
{
    osalbool bResult  = TRUE;
    uint16  usMsgLen = 0;

    if (MsgPtr != NULL)
    {
        usMsgLen = CFE_SB_GetTotalMsgLength(MsgPtr);

        if (usExpectedLen != usMsgLen)
        {
            bResult = FALSE;
            CFE_SB_MsgId_t MsgId = CFE_SB_GetMsgId(MsgPtr);
            uint16 usCmdCode = CFE_SB_GetCmdCode(MsgPtr);

            (void) CFE_EVS_SendEvent(FLOW_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            HkTlm.usCmdErrCnt++;
        }
    }

    return bResult;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FLOW Application C style main entry point.                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
extern "C" void FLOW_AppMain()
{
    oFLOW.AppMain();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FLOW Application C++ style main entry point.                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void FLOW::AppMain()
{

    /* Register the application with Executive Services */
    uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("FLOW - Failed to register the app (0x%08lX)\n", iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(FLOW_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(FLOW_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(FLOW_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(FLOW_MAIN_TASK_PERF_ID);
    }
    else
    {
        uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&uiRunStatus) == TRUE)
    {
        RcvSchPipeMsg(FLOW_SCH_PIPE_PEND_TIME);

        iStatus = AcquireConfigPointers();
        if(iStatus != CFE_SUCCESS)
        {
            /* We apparently tried to load a new table but failed.  Terminate the application. */
            uiRunStatus = CFE_ES_APP_ERROR;
        }
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(FLOW_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(uiRunStatus);
}

uint32 FLOW::USAD8(uint32 val1, uint32 val2)
{
	/* The USAD8 finds the four differences between the unsigned values in
	 * corresponding bytes of val1 and val2. It adds the absolute values of
	 * the four differences, and saves the result. */
	uint32 result = 0;
	/* Strip 32-bit value ptr to 4 8-bit value ptrs */
	uint8 * val1_bytes = (uint8 *)(&val1);
	uint8 * val2_bytes = (uint8 *)(&val2);
	int8 i;
	for(i = 0; i < 4; i++)
	{
		/* Find absolute difference between each 8-bit factor */
		int16 v1 = val1_bytes[i];
		int16 v2 = val2_bytes[i];
		/* Accumulate all factors */
		result += (uint32)(abs(v1-v2));
	}
	return result;
}

uint32 FLOW::USADA8(uint32 val1, uint32 val2, uint32 val3)
{
	/* The USADA8 adds the absolute values of the four differences
	 * to the value in val3, and saves the result.*/
	uint32 result = val3;
	/* Strip 32-bit value ptr to 4 8-bit value ptrs */
	uint8 * val1_bytes = (uint8 *)(&val1);
	uint8 * val2_bytes = (uint8 *)(&val2);
	int8 i;
	for(i = 0; i < 4; i++)
	{
		/* Find absolute difference between each 8-bit factor */
		int16 v1 = val1_bytes[i];
		int16 v2 = val2_bytes[i];
		/* Accumulate all factors to val3 */
		result += (uint32)(abs(v1 - v2));
	}
	return result;
}

uint32 FLOW::UHADD8(uint32 val1, uint32 val2)
{
	/* The UHADD* add 8-bit data and then to halve the result before writing
	 * the result to the destination register */
	uint32 result = 0;
	uint8 * result_bytes = (uint8 *)(&result);
	/* Strip 32-bit value ptr to 4 8-bit value ptrs */
	uint8 * val1_bytes = (uint8 *)(&val1);
	uint8 * val2_bytes = (uint8 *)(&val2);

	for (int32 i = 0; i < 4; i++) {
		/* Halving the sum of bytes */
		result_bytes[i] = (val1_bytes[i] + val2_bytes[i]) >> 1;
	}

	return result;
}

uint32 FLOW::ComputeSad8X8(const uint8 * Image1,const uint8 *Image2, uint16 Off1X, uint16 Off1Y, uint16 Off2X, uint16 Off2Y, uint16 RowSize)
{
	/* Calculate position in image buffer */
	/* Image 1 */
	uint16 off1 = Off1Y * RowSize + Off1X;
	/* Image 2 */
	uint16 off2 = Off2Y * RowSize + Off2X;

	uint32 acc;
	/* 8 pixels are selected in each row */

	/* SAD-Compare 0th row in Image 1 with 0th row in Image 2 */
	acc = USAD8(*((uint32 *) &Image1[off1 + 0 + 0 * RowSize]), *((uint32 *) &Image2[off2 + 0 + 0 * RowSize]));
	acc = USADA8(*((uint32 *) &Image1[off1 + 4 + 0 * RowSize]), *((uint32 *) &Image2[off2 + 4 + 0 * RowSize]), acc);

	/* SAD-Compare 1st row in Image 1 with 1st row in Image 2 */
	acc = USADA8(*((uint32 *) &Image1[off1 + 0 + 1 * RowSize]), *((uint32 *) &Image2[off2 + 0 + 1 * RowSize]), acc);
	acc = USADA8(*((uint32 *) &Image1[off1 + 4 + 1 * RowSize]), *((uint32 *) &Image2[off2 + 4 + 1 * RowSize]), acc);

	/* SAD-Compare 2nd row in Image 1 with 2nd row in Image 2 */
	acc = USADA8(*((uint32 *) &Image1[off1 + 0 + 2 * RowSize]), *((uint32 *) &Image2[off2 + 0 + 2 * RowSize]), acc);
	acc = USADA8(*((uint32 *) &Image1[off1 + 4 + 2 * RowSize]), *((uint32 *) &Image2[off2 + 4 + 2 * RowSize]), acc);

	/* SAD-Compare 3rd row in Image 1 with 3rd row in Image 2 */
	acc = USADA8(*((uint32 *) &Image1[off1 + 0 + 3 * RowSize]), *((uint32 *) &Image2[off2 + 0 + 3 * RowSize]), acc);
	acc = USADA8(*((uint32 *) &Image1[off1 + 4 + 3 * RowSize]), *((uint32 *) &Image2[off2 + 4 + 3 * RowSize]), acc);

	/* SAD-Compare 4th row in Image 1 with 4th row in Image 2 */
	acc = USADA8(*((uint32 *) &Image1[off1 + 0 + 4 * RowSize]), *((uint32 *) &Image2[off2 + 0 + 4 * RowSize]), acc);
	acc = USADA8(*((uint32 *) &Image1[off1 + 4 + 4 * RowSize]), *((uint32 *) &Image2[off2 + 4 + 4 * RowSize]), acc);

	/* SAD-Compare 5th row in Image 1 with 5th row in Image 2 */
	acc = USADA8(*((uint32 *) &Image1[off1 + 0 + 5 * RowSize]), *((uint32 *) &Image2[off2 + 0 + 5 * RowSize]), acc);
	acc = USADA8(*((uint32 *) &Image1[off1 + 4 + 5 * RowSize]), *((uint32 *) &Image2[off2 + 4 + 5 * RowSize]), acc);

	/* SAD-Compare 6th row in Image 1 with 6th row in Image 2 */
	acc = USADA8(*((uint32 *) &Image1[off1 + 0 + 6 * RowSize]), *((uint32 *) &Image2[off2 + 0 + 6 * RowSize]), acc);
	acc = USADA8(*((uint32 *) &Image1[off1 + 4 + 6 * RowSize]), *((uint32 *) &Image2[off2 + 4 + 6 * RowSize]), acc);

	/* SAD-Compare 7th row in Image 1 with 7th row in Image 2 */
	acc = USADA8(*((uint32 *) &Image1[off1 + 0 + 7 * RowSize]), *((uint32 *) &Image2[off2 + 0 + 7 * RowSize]), acc);
	acc = USADA8(*((uint32 *) &Image1[off1 + 4 + 7 * RowSize]), *((uint32 *) &Image2[off2 + 4 + 7 * RowSize]), acc);


	return acc;
}

uint32 FLOW::ComputeSubpixel(const uint8 *Image1, const uint8 *Image2, uint16 Off1X, uint16 Off1Y,uint16 Off2X, uint16 Off2Y, uint32 *acc, uint16 RowSize)
{
	/* Calculate position in image buffer */
	/* Image 1 */
	uint16 off1 = Off1Y * RowSize + Off1X;
	/* Image 2 */
	uint16 off2 = Off2Y * RowSize + Off2X;

	/* Surrounding pixel information*/
	uint32 s0;
	uint32 s1;
	uint32 s2;
	uint32 s3;
	uint32 s4;
	uint32 s5;
	uint32 s6;
	uint32 s7;

	/* Corner pixel information */
	uint32 t1;
	uint32 t3;
	uint32 t5;
	uint32 t7;

	/* A image pixel can have at most 8 neighbors we will
	 * need acc to store result for each neighbor */
	for (uint16 i = 0; i < 8; i++) {
		acc[i] = 0;
	}

	/*
	 * Calculate for each pixel in the 8x8 field with upper left corner (Off1X / Off1Y)
	 * every iteration is one line of the 8x8 field.
	 *
	 *  + - + - + - + - + - + - + - + - +
	 *  |   |   |   |   |   |   |   |   |
	 *  + - + - + - + - + - + - + - + - +
	 *
	 *
	 */

	/* A image pixel can have at most 8 neighbors we will
	 * iterate though each of them
	 */
	for (uint16 i = 0; i < 8; i++) {
		/*
		 * First column of 4 pixels:
		 *
		 *  + - + - + - + - + - + - + - + - +
		 *  | x | x | x | x |   |   |   |   |
		 *  + - + - + - + - + - + - + - + - +
		 *
		 * the 8 s values are from following positions for each pixel (X):
		 *  + - + - + - +
		 *  +   5   7   +
		 *  + - + 6 + - +
		 *  +   4 X 0   +
		 *  + - + 2 + - +
		 *  +   3   1   +
		 *  + - + - + - +
		 *
		 *  variables (s1, ...) contains all 4 results (32bit -> 4 * 8bit values)
		 *
		 */

		/* Compute average of two pixel values */
		s0 = ( UHADD8(*((uint32 *) &Image2[off2 +  (0 + ((i + 0) * RowSize))]), *((uint32 *) &Image2[off2 + (1 + ((i + 0) * RowSize))])));
		s1 = ( UHADD8(*((uint32 *) &Image2[off2 +  (0 + ((i + 1) * RowSize))]), *((uint32 *) &Image2[off2 + (1 + ((i + 1) * RowSize))])));
		s2 = ( UHADD8(*((uint32 *) &Image2[off2 +  (0 + ((i + 0) * RowSize))]), *((uint32 *) &Image2[off2 + (0 + ((i + 1) * RowSize))])));
		s3 = ( UHADD8(*((uint32 *) &Image2[off2 +  (0 + ((i + 1) * RowSize))]), *((uint32 *) &Image2[off2 - (1 + ((i + 1) * RowSize))])));
		s4 = ( UHADD8(*((uint32 *) &Image2[off2 +  (0 + ((i + 0) * RowSize))]), *((uint32 *) &Image2[off2 - (1 + ((i + 0) * RowSize))])));
		s5 = ( UHADD8(*((uint32 *) &Image2[off2 +  (0 + ((i - 1) * RowSize))]), *((uint32 *) &Image2[off2 - (1 + ((i - 1) * RowSize))])));
		s6 = ( UHADD8(*((uint32 *) &Image2[off2 +  (0 + ((i + 0) * RowSize))]), *((uint32 *) &Image2[off2 + (0 + ((i - 1) * RowSize))])));
		s7 = ( UHADD8(*((uint32 *) &Image2[off2 +  (0 + ((i - 1) * RowSize))]), *((uint32 *) &Image2[off2 + (1 + ((i - 1) * RowSize))])));

		/* These 4 t values are from the corners around the center pixel */
		t1 = ( UHADD8(s0, s1));
		t3 = ( UHADD8(s3, s4));
		t5 = ( UHADD8(s4, s5));
		t7 = ( UHADD8(s7, s0));

		/*
		 * Finally we got all 8 subpixels (s0, t1, s2, t3, s4, t5, s6, t7):
		 *  + - + - + - +
		 *  |   |   |   |
		 *  + - 5 6 7 - +
		 *  |   4 X 0   |
		 *  + - 3 2 1 - +
		 *  |   |   |   |
		 *  + - + - + - +
		 */

		/* Fill accumulation vector */
		acc[0] =  USADA8((*((uint32 *) &Image1[off1 + (0 + (i * RowSize))])), s0, acc[0]);
		acc[1] =  USADA8((*((uint32 *) &Image1[off1 + (0 + (i * RowSize))])), t1, acc[1]);
		acc[2] =  USADA8((*((uint32 *) &Image1[off1 + (0 + (i * RowSize))])), s2, acc[2]);
		acc[3] =  USADA8((*((uint32 *) &Image1[off1 + (0 + (i * RowSize))])), t3, acc[3]);
		acc[4] =  USADA8((*((uint32 *) &Image1[off1 + (0 + (i * RowSize))])), s4, acc[4]);
		acc[5] =  USADA8((*((uint32 *) &Image1[off1 + (0 + (i * RowSize))])), t5, acc[5]);
		acc[6] =  USADA8((*((uint32 *) &Image1[off1 + (0 + (i * RowSize))])), s6, acc[6]);
		acc[7] =  USADA8((*((uint32 *) &Image1[off1 + (0 + (i * RowSize))])), t7, acc[7]);

		/*
		 * Same for second column of 4 pixels:
		 *
		 *  + - + - + - + - + - + - + - + - +
		 *  |   |   |   |   | x | x | x | x |
		 *  + - + - + - + - + - + - + - + - +
		 *
		 */

		s0 = ( UHADD8(*((uint32 *) &Image2[off2 + (4 + ((i + 0) * RowSize))]), *((uint32 *) &Image2[off2 + (5 + ((i + 0) * RowSize))])));
		s1 = ( UHADD8(*((uint32 *) &Image2[off2 + (4 + ((i + 1) * RowSize))]), *((uint32 *) &Image2[off2 + (5 + ((i + 1) * RowSize))])));
		s2 = ( UHADD8(*((uint32 *) &Image2[off2 + (4 + ((i + 0) * RowSize))]), *((uint32 *) &Image2[off2 + (4 + ((i + 1) * RowSize))])));
		s3 = ( UHADD8(*((uint32 *) &Image2[off2 + (4 + ((i + 1) * RowSize))]), *((uint32 *) &Image2[off2 + (3 + ((i + 1) * RowSize))])));
		s4 = ( UHADD8(*((uint32 *) &Image2[off2 + (4 + ((i + 0) * RowSize))]), *((uint32 *) &Image2[off2 + (3 + ((i + 0) * RowSize))])));
		s5 = ( UHADD8(*((uint32 *) &Image2[off2 + (4 + ((i - 1) * RowSize))]), *((uint32 *) &Image2[off2 + (3 + ((i - 1) * RowSize))])));
		s6 = ( UHADD8(*((uint32 *) &Image2[off2 + (4 + ((i + 0) * RowSize))]), *((uint32 *) &Image2[off2 + (4 + ((i - 1) * RowSize))])));
		s7 = ( UHADD8(*((uint32 *) &Image2[off2 + (4 + ((i - 1) * RowSize))]), *((uint32 *) &Image2[off2 + (5 + ((i - 1) * RowSize))])));

		t1 = ( UHADD8(s0, s1));
		t3 = ( UHADD8(s3, s4));
		t5 = ( UHADD8(s4, s5));
		t7 = ( UHADD8(s7, s0));

		acc[0] =  USADA8((*((uint32 *) &Image1[off1 + (4 + (i * RowSize))])), s0, acc[0]);
		acc[1] =  USADA8((*((uint32 *) &Image1[off1 + (4 + (i * RowSize))])), t1, acc[1]);
		acc[2] =  USADA8((*((uint32 *) &Image1[off1 + (4 + (i * RowSize))])), s2, acc[2]);
		acc[3] =  USADA8((*((uint32 *) &Image1[off1 + (4 + (i * RowSize))])), t3, acc[3]);
		acc[4] =  USADA8((*((uint32 *) &Image1[off1 + (4 + (i * RowSize))])), s4, acc[4]);
		acc[5] =  USADA8((*((uint32 *) &Image1[off1 + (4 + (i * RowSize))])), t5, acc[5]);
		acc[6] =  USADA8((*((uint32 *) &Image1[off1 + (4 + (i * RowSize))])), s6, acc[6]);
		acc[7] =  USADA8((*((uint32 *) &Image1[off1 + (4 + (i * RowSize))])), t7, acc[7]);
	}

	return 0;
}

uint32 FLOW::ComputeDifference(const uint8 *image, uint16 offX, uint16 offY, uint16 RowSize)
{
	/* Returns the sum of number of pixels vary in rows and columns*/

	/* Use offsets to identify a 4x4 pattern*/
	uint16 off = (offY + 2) * RowSize + (offX +2);
	uint32 acc=0;

	/* Calculate row difference*/
	acc = USAD8(*((uint32 *)&image[off + 0 + 0 * RowSize]),*((uint32 *)&image[off + 0 + 1 * RowSize]));
	acc = USADA8(*((uint32 *) &image[off + 0 + 1 * RowSize]), *((uint32 *) &image[off + 0 + 2 * RowSize]), acc);
	acc = USADA8(*((uint32 *) &image[off + 0 + 2 * RowSize]), *((uint32 *) &image[off + 0 + 3 * RowSize]), acc);

	/* We need to get columns */
	/*  Rearranging columns like rows to perform SAD */
	uint32 col1 = (image[off + 0 + 0 * RowSize] << 24) | image[off + 0 + 1 * RowSize] << 16 | image[off + 0 + 2 *
			RowSize] << 8 | image[off + 0 + 3 * RowSize];
	uint32 col2 = (image[off + 1 + 0 * RowSize] << 24) | image[off + 1 + 1 * RowSize] << 16 | image[off + 1 + 2 *
			RowSize] << 8 | image[off + 1 + 3 * RowSize];
	uint32 col3 = (image[off + 2 + 0 * RowSize] << 24) | image[off + 2 + 1 * RowSize] << 16 | image[off + 2 + 2 *
			RowSize] << 8 | image[off + 2 + 3 * RowSize];
	uint32 col4 = (image[off + 3 + 0 * RowSize] << 24) | image[off + 3 + 1 * RowSize] << 16 | image[off + 3 + 2 *
			RowSize] << 8 | image[off + 3 + 3 * RowSize];

	/* Calc column diff */
	acc = USADA8(col1, col2, acc);
	acc = USADA8(col2, col3, acc);
	acc = USADA8(col3, col4, acc);


	return acc;
}

void FLOW::InitializeLimitRate()
{
	HkTlm.FlowSum_X = 0.0f;
	HkTlm.FlowSum_Y = 0.0f;
	HkTlm.FlowQualitySum = 0;
	HkTlm.ValidFrameCount = 0;
}

uint32 FLOW::LimitRate(int32 flow_quality, const uint32 frame_time_us)
{
	/* Looking at the timestamp, this function will calculate the rate it should publish the quality.
	 * If the rate is different than what is desired it will zero out quality until desired rate is achieved.
	 */

	/* Initial quality and time*/
	int32 q = 0;

	/* Reset limit variables */
	InitializeLimitRate();

	if(ConfigTblPtr->FLOW_DEFAULT_OUTPUT_RATE <= 0)
	{
		HkTlm.DeltaTime = frame_time_us - HkTlm.TimeLastPublished;
		HkTlm.TimeLastPublished = frame_time_us;
		q = flow_quality;
	}
	else
	{
		/* Output rate is defines and is valid */
		if(flow_quality > 0)
		{
			HkTlm.FlowSum_X += HkTlm.PixelFlowX;
			HkTlm.FlowSum_Y += HkTlm.PixelFlowY;
			HkTlm.FlowQualitySum += flow_quality;
			HkTlm.ValidFrameCount++;
		}
		/* Limit rate according to parameter ouput_rate */
		if ((frame_time_us - HkTlm.TimeLastPublished) > (FLOW_US_IN_SEC / ConfigTblPtr->FLOW_DEFAULT_OUTPUT_RATE))
		{
			int32 average_flow_quality = 0;

			/* Average the flow over the period since last update */
			if (HkTlm.ValidFrameCount > 0)
			{
				average_flow_quality = floor(HkTlm.FlowQualitySum / HkTlm.ValidFrameCount);
			}

			HkTlm.PixelFlowX = HkTlm.FlowSum_X;
			HkTlm.PixelFlowY = HkTlm.FlowSum_Y;

			HkTlm.DeltaTime = frame_time_us - HkTlm.TimeLastPublished;
			HkTlm.TimeLastPublished = frame_time_us;

			q = average_flow_quality;
		}
		else
		{
			/* Signaling that it should not yet publish the values */
			q = -1;
		}
	}
	return q;
}

uint32 FLOW::CalculateOpticalFlow()
{
	int32 quality = 0;

	if(!HkTlm.FlowInitalized)
	{
		CFE_PSP_MemCpy(&PreviousFrame.Frame, &CVT.OpticalFlowFrameMsg.Frame, sizeof(PreviousFrame.Frame));
		CFE_PSP_MemCpy(&PreviousFrame.Timestamp, &CVT.OpticalFlowFrameMsg.Timestamp, sizeof(PreviousFrame.Timestamp));
		HkTlm.FlowInitalized = true;
	}
	else
	{
		quality = ComputeFlow((uint8 *)&PreviousFrame.Frame, (uint8 *)&CVT.OpticalFlowFrameMsg.Frame);
		CFE_PSP_MemCpy(&PreviousFrame.Frame, &CVT.OpticalFlowFrameMsg.Frame, ConfigTblPtr->FLOW_DEFAULT_IMAGE_WIDTH * ConfigTblPtr->FLOW_DEFAULT_IMAGE_HEIGHT * sizeof(uint8));
		CFE_PSP_MemCpy(&PreviousFrame.Timestamp, &CVT.OpticalFlowFrameMsg.Timestamp, sizeof(uint64));
		quality = LimitRate(quality,CVT.OpticalFlowFrameMsg.Timestamp);
		HkTlm.AngularFlowX = atan2(HkTlm.PixelFlowX,DiagTlm.FocalLength_X);
		HkTlm.AngularFlowY = atan2(HkTlm.PixelFlowY,DiagTlm.FocalLength_Y);

		if(quality >= 0)
		{
			/* Copy findings to optical flow message */
			OpticalFlowMsg.Timestamp = PX4LIB_GetPX4TimeUs();
			OpticalFlowMsg.SensorID = FLOW_SENSOR_ID;
			OpticalFlowMsg.PixelFlowXIntegral = ((quality>=0) ? HkTlm.AngularFlowX : 0.0f);
			OpticalFlowMsg.PixelFlowYIntegral = ((quality>=0) ? HkTlm.AngularFlowY : 0.0f);
			OpticalFlowMsg.GyroXRateIntegral = ((quality>=0) ? -HkTlm.GyroYRateIntegral : 0.0f);
			OpticalFlowMsg.GyroYRateIntegral = ((quality>=0) ? HkTlm.GyroXRateIntegral : 0.0f);
			OpticalFlowMsg.GyroZRateIntegral = ((quality>=0) ? -HkTlm.GyroZRateIntegral : 0.0f);
			OpticalFlowMsg.IntegrationTimespan = ((quality>=0) ? HkTlm.DeltaTime : 0);
			OpticalFlowMsg.FrameCountSinceLastReadout = ((quality>=0) ? HkTlm.ValidFrameCount : 0);
			OpticalFlowMsg.Quality = quality;

			HkTlm.Quality = quality;
			HkTlm.Timestamp = OpticalFlowMsg.Timestamp;

			/* Publish message */
			SendOpticalFlowMsg();
		}
	}
	return quality;
}

uint32 FLOW::ComputeFlow(const uint8 *Image1, const uint8 *Image2)
{
	/* Constants */
	const int16 winmin = -ConfigTblPtr->FLOW_DEFAULT_SEARCH_SIZE;
	const int16 winmax = ConfigTblPtr->FLOW_DEFAULT_SEARCH_SIZE;
	const uint16 hist_size = 2 * (winmax - winmin + 1) + 1;

	/* Variables */
	uint16 pixLo = ConfigTblPtr->FLOW_DEFAULT_SEARCH_SIZE + 1;
	uint16 pixHi = ConfigTblPtr->FLOW_DEFAULT_IMAGE_WIDTH - (ConfigTblPtr->FLOW_DEFAULT_SEARCH_SIZE + 1) - ConfigTblPtr->FLOW_TILE_SIZE;
	uint16 pixStep = (pixHi - pixLo) / ConfigTblPtr->FLOW_NUM_BLOCKS + 1;
	uint16 i;
	uint16 j;
	/* Subpixels */
	uint32 acc[8];
	/* Counter for x shift */
	uint16 histx[hist_size];
	/* Counter for y shift */
	uint16 histy[hist_size];
	/* Shift directions in x */
	int8  dirsx[64];
	/* Shift directions in y */
	int8  dirsy[64];
	/* Shift directions of best subpixels */
	uint8  subdirs[64];
	float meanflowx = 0.0f;
	float meanflowy = 0.0f;
	uint16 meancount = 0;
	float histflowx = 0.0f;
	float histflowy = 0.0f;
	uint32 dist;
	int8 sumx;
	int8 sumy;
	int8 ii;
	int8 jj;
	uint32 temp_dist;
	uint32 mindist;
	uint8 mindir;
	uint8 hist_index_x;
	uint8 hist_index_y;
	uint32 meancount_x;
	uint32 meancount_y;
	float subdirx;
	float subdiry;
	uint8 qual;

	/* Initialize with 0 */
	for (j = 0; j < hist_size; j++)
	{
		histx[j] = 0;
		histy[j] = 0;
	}

	/* Iterate over all patterns */
	for(j = pixLo; j< pixHi; j+=pixStep)
	{
		for(i=pixLo; i<pixHi; i+=pixStep)
		{
			/* Check validity of a subpixels in image 1,
			 * ignore subpixels from calculation which are full white or grey
			 */
			uint32 diff = ComputeDifference(Image1, i, j, ConfigTblPtr->FLOW_DEFAULT_IMAGE_WIDTH);
			if(diff < ConfigTblPtr->FLOW_DEFAULT_FEATURE_THRESHOLD)
			{
				continue;
			}
			/* Set initial distance to "infinity" */
			dist = 0xFFFFFFFF;
			sumx = 0;
			sumy = 0;
			for(jj = winmin; jj <= winmax; jj++)
			{
				for(ii = winmin; ii <= winmax; ii++)
				{
					/* For valid subpixels match them with subpixels in image 2 and calculate SAD.
					 * Identify the subpixels with low distance or highest match
					 */
					temp_dist = ComputeSad8X8(Image1, Image2, i, j, i+ii, j+jj, ConfigTblPtr->FLOW_DEFAULT_IMAGE_WIDTH);

					if(temp_dist < dist)
					{
						sumx = ii;
						sumy = jj;
						dist = temp_dist;
					}
				}
			}
			/* Acceptance SAD distance threshold */
			if(dist < ConfigTblPtr->FLOW_DEFAULT_VALUE_THRESHOLD)
			{
				meanflowx += (float) sumx;
				meanflowy += (float) sumy;
				/* Calculate gradients across 8 neighboring pixels
				 */
				ComputeSubpixel(Image1, Image2, i, j, i + sumx, j + sumy, acc, ConfigTblPtr->FLOW_DEFAULT_IMAGE_WIDTH);
				/* Best SAD until now */
				mindist = dist;
				/* Direction 8 for no direction */
				mindir = 8;

				for (uint8 k = 0; k < 8; k++)
				{
					if (acc[k] < mindist)
					{
						/* SAD becomes better in direction k */
						mindist = acc[k];
						mindir = k;
					}
				}

				dirsx[meancount] = sumx;
				dirsy[meancount] = sumy;
				subdirs[meancount] = mindir;
				meancount++;

				/* Feed histogram filter*/
				hist_index_x = (2 * sumx) + (winmax - winmin + 1);

				if (subdirs[i] == 0 || subdirs[i] == 1 || subdirs[i] == 7)
				{
					hist_index_x += 1;
				}

				if (subdirs[i] == 3 || subdirs[i] == 4 || subdirs[i] == 5)
				{
					hist_index_x += -1;
				}

				hist_index_y = (2 * sumy) + (winmax - winmin + 1);

				if (subdirs[i] == 5 || subdirs[i] == 6 || subdirs[i] == 7)
				{
					hist_index_y += -1;
				}

				if (subdirs[i] == 1 || subdirs[i] == 2 || subdirs[i] == 3)
				{
					hist_index_y += 1;
				}

				if(hist_index_x < sizeof(histx))
				{
					histx[hist_index_x]++;
				}
				else
				{
					(void) CFE_EVS_SendEvent(FLOW_BUF_ORUN_ERR_EID, CFE_EVS_ERROR,
							"histx - buffer overrun");
				}

				if(hist_index_y < sizeof(histy))
				{
					histy[hist_index_y]++;
				}
				else
				{
					(void) CFE_EVS_SendEvent(FLOW_BUF_ORUN_ERR_EID, CFE_EVS_ERROR,
							"histy - buffer overrun");
				}



			}
		}
	}
	/* Evaluate flow calculation */
	if (meancount > ConfigTblPtr->FLOW_MEAN_COUNT_THRESHOLD)
	{
		meanflowx /= meancount;
		meanflowy /= meancount;

		/* Use average of accepted flow values */
		meancount_x = 0;
		meancount_y = 0;

		for (uint8 h = 0; h < meancount; h++)
		{
			subdirx = 0.0f;

			if (subdirs[h] == 0 || subdirs[h] == 1 || subdirs[h] == 7)
			{
				subdirx = 0.5f;
			}

			if (subdirs[h] == 3 || subdirs[h] == 4 || subdirs[h] == 5)
			{
				subdirx = -0.5f;
			}

			histflowx += (float)dirsx[h] + subdirx;
			meancount_x++;

			subdiry = 0.0f;

			if (subdirs[h] == 5 || subdirs[h] == 6 || subdirs[h] == 7)
			{
				subdiry = -0.5f;
			}

			if (subdirs[h] == 1 || subdirs[h] == 2 || subdirs[h] == 3)
			{
				subdiry = 0.5f;
			}

			histflowy += (float)dirsy[h] + subdiry;
			meancount_y++;
		}

		histflowx /= meancount_x;
		histflowy /= meancount_y;

		/* Without gyro compensation */
		HkTlm.PixelFlowX = histflowx;
		HkTlm.PixelFlowY = histflowy;


	}
	/* No peak value in histogram */
	else
	{
		HkTlm.PixelFlowX = 0.0f;
		HkTlm.PixelFlowY = 0.0f;
		goto FLOW_ComputeFlow_Exit_Tag;
	}

	/* Calculate quality */
	qual = (uint8)(meancount * 255 / (ConfigTblPtr->FLOW_NUM_BLOCKS * ConfigTblPtr->FLOW_NUM_BLOCKS));

FLOW_ComputeFlow_Exit_Tag:
	return qual;
}

void FLOW::InitFlowApp()
{
	HkTlm.PixelFlowX = 0.0f;
	HkTlm.PixelFlowY = 0.0f;
	HkTlm.AngularFlowX = 0.0f;
	HkTlm.AngularFlowY = 0.0f;
	HkTlm.TimeLastPublished = 0;
	HkTlm.FlowInitalized = false;
	DiagTlm.FocalLength_X = DiagTlm.FocalLength_Y = (ConfigTblPtr->FLOW_DEFAULT_IMAGE_WIDTH/2)/tan(ConfigTblPtr->HFOV/2);
	CFE_PSP_MemSet(&PreviousFrame, 0, sizeof(PreviousFrame));
	InitializeLimitRate();
	SendDiagMsg();
}

/************************/
/*  End of File Comment */
/************************/
