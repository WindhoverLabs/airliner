/************************************************************************
** Includes
*************************************************************************/
#include <string.h>

#include "cfe.h"

#include "sens_app.h"
#include "sens_msg.h"
#include "sens_version.h"
#include <math/Limits.hpp>
#include <math.h>
#include "px4lib.h"
#include "px4lib_msgids.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Instantiate the application object.                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
SENS oSENS;



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Default constructor.                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
SENS::SENS() :
	FilterRoll(50.0f, 10.f), /* get replaced by parameter */
	FilterPitch(50.0f, 10.f),
	FilterYaw(50.0f, 10.f),
	FilterThrottle(50.0f, 10.f)
{

}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Destructor constructor.                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
SENS::~SENS()
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables.                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 SENS::InitEvent()
{
    int32  iStatus=CFE_SUCCESS;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(0, 0, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("SENS - Failed to register with EVS (0x%08lX)\n", iStatus);
    }

    return iStatus;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 SENS::InitPipe()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&SchPipeId,
    		SENS_SCH_PIPE_DEPTH,
			SENS_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(SENS_WAKEUP_MID, SchPipeId, CFE_SB_Default_Qos, SENS_WAKEUP_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(SENS_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
            		"Sch Pipe failed to subscribe to SENS_WAKEUP_MID. (0x%08lX)",
                    iStatus);
            goto SENS_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(SENS_SEND_HK_MID, SchPipeId, CFE_SB_Default_Qos, SENS_SEND_HK_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(SENS_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to SENS_SEND_HK_MID. (0x%08X)",
					 (unsigned int)iStatus);
            goto SENS_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_INPUT_RC_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(SENS_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to PX4_INPUT_RC_MID. (0x%08lX)",
					 iStatus);
            goto SENS_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_DIFFERENTIAL_PRESSURE_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(SENS_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to PX4_DIFFERENTIAL_PRESSURE_MID. (0x%08lX)",
					 iStatus);
            goto SENS_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_SENSOR_BARO_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(SENS_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to PX4_SENSOR_BARO_MID. (0x%08lX)",
					 iStatus);
            goto SENS_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_SENSOR_ACCEL_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(SENS_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to PX4_SENSOR_ACCEL_MID. (0x%08lX)",
					 iStatus);
            goto SENS_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_SENSOR_MAG_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(SENS_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to PX4_SENSOR_MAG_MID. (0x%08lX)",
					 iStatus);
            goto SENS_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_SENSOR_GYRO_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(SENS_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to PX4_SENSOR_GYRO_MID. (0x%08lX)",
					 iStatus);
            goto SENS_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_CONTROL_MODE_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(SENS_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to PX4_VEHICLE_CONTROL_MODE_MID. (0x%08lX)",
					 iStatus);
            goto SENS_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(SENS_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
			 "Failed to create SCH pipe (0x%08lX)",
			 iStatus);
        goto SENS_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&CmdPipeId,
    		SENS_CMD_PIPE_DEPTH,
			SENS_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(SENS_CMD_MID, CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(SENS_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
				 "CMD Pipe failed to subscribe to SENS_CMD_MID. (0x%08lX)",
				 iStatus);
            goto SENS_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(SENS_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
			 "Failed to create CMD pipe (0x%08lX)",
			 iStatus);
        goto SENS_InitPipe_Exit_Tag;
    }

SENS_InitPipe_Exit_Tag:
    return iStatus;
}
    

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SENS::InitData()
{
	/* Init housekeeping message. */
	CFE_SB_InitMsg(&HkTlm,
    		SENS_HK_TLM_MID, sizeof(HkTlm), TRUE);

	/* Init output messages */
	CFE_SB_InitMsg(&ActuatorControls3Msg,
			PX4_ACTUATOR_CONTROLS_3_MID, sizeof(PX4_ActuatorControlsMsg_t), TRUE);

	CFE_SB_InitMsg(&BatteryStatusMsg,
      		PX4_BATTERY_STATUS_MID, sizeof(PX4_BatteryStatusMsg_t), TRUE);

	CFE_SB_InitMsg(&AirspeedMsg,
      		PX4_AIRSPEED_MID, sizeof(PX4_AirspeedMsg_t), TRUE);

	CFE_SB_InitMsg(&RcChannelsMsg,
      		PX4_RC_CHANNELS_MID, sizeof(PX4_RcChannelsMsg_t), TRUE);

	CFE_SB_InitMsg(&ManualControlSetpointMsg,
      		PX4_MANUAL_CONTROL_SETPOINT_MID, sizeof(PX4_ManualControlSetpointMsg_t), TRUE);

	//CFE_SB_InitMsg(&SensorPreflightMsg,
	//		PX4_SENSOR_PREFLIGHT_MID, sizeof(PX4_SensorPreflightMsg_t), TRUE);
	/* Init output messages */

	CFE_SB_InitMsg(&SensorCombinedMsg,
      		PX4_SENSOR_COMBINED_MID, sizeof(PX4_SensorCombinedMsg_t), TRUE);

	/* Initialized member attributes */
	Armed = false;

	SensorCombinedMsg.AccInvalid = true;
	SensorCombinedMsg.BaroInvalid = true;
	SensorCombinedMsg.MagInvalid = true;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* SENS initialization                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 SENS::InitApp()
{
    int32  iStatus   = CFE_SUCCESS;
    int8   hasEvents = 0;

    iStatus = InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("SENS - Failed to init events (0x%08lX)\n", iStatus);
        goto SENS_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        goto SENS_InitApp_Exit_Tag;
    }

    InitData();

    iStatus = InitConfigTbl();
    if (iStatus != CFE_SUCCESS)
    {
        goto SENS_InitApp_Exit_Tag;
    }

	UpdateRcFunctions();

SENS_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(SENS_INIT_INF_EID, CFE_EVS_INFORMATION,
                                 "Initialized.  Version %d.%d.%d.%d",
								 SENS_MAJOR_VERSION,
								 SENS_MINOR_VERSION,
								 SENS_REVISION,
								 SENS_MISSION_REV);
    }
    else
    {
        if (hasEvents == 1)
        {
            CFE_EVS_SendEvent(SENS_INIT_ERR_EID, CFE_EVS_ERROR, "Application failed to initialize");
        }
        else
        {
            CFE_ES_WriteToSysLog("SENS - Application failed to initialize\n");
        }
    }

    return iStatus;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 SENS::RcvSchPipeMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(SENS_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(SENS_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
        {
            case SENS_WAKEUP_MID:
                //CyclicProcessing();
                break;

            case SENS_SEND_HK_MID:
            	ProcessCmdPipe();
                memcpy(&HkTlm.SensorCombinedMsg, &SensorCombinedMsg, sizeof(SensorCombinedMsg));
                ReportHousekeeping();
                break;

            case PX4_INPUT_RC_MID:
                memcpy(&CVT.InputRcMsg, MsgPtr, sizeof(CVT.InputRcMsg));
            	ProcessRCInput();
                break;

            case PX4_DIFFERENTIAL_PRESSURE_MID:
                memcpy(&CVT.DifferentialPressureMsg, MsgPtr, sizeof(CVT.DifferentialPressureMsg));
                break;

            case PX4_SENSOR_BARO_MID:
                memcpy(&CVT.SensorBaroMsg, MsgPtr, sizeof(CVT.SensorBaroMsg));
                break;

            case PX4_SENSOR_ACCEL_MID:
                memcpy(&CVT.SensorAccelMsg, MsgPtr, sizeof(CVT.SensorAccelMsg));
                break;

            case PX4_SENSOR_MAG_MID:
                memcpy(&CVT.SensorMagMsg, MsgPtr, sizeof(CVT.SensorMagMsg));
                break;

            case PX4_SENSOR_GYRO_MID:
                memcpy(&CVT.SensorGyroMsg, MsgPtr, sizeof(CVT.SensorGyroMsg));
            	CombineSensorInput();
                break;

            case PX4_VEHICLE_CONTROL_MODE_MID:
                memcpy(&CVT.VehicleControlModeMsg, MsgPtr, sizeof(CVT.VehicleControlModeMsg));
                Armed = CVT.VehicleControlModeMsg.Armed;
                break;

            default:
                (void) CFE_EVS_SendEvent(SENS_MSGID_ERR_EID, CFE_EVS_ERROR,
                     "Recvd invalid SCH msgId (0x%04X)", MsgId);
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
        (void) CFE_EVS_SendEvent(SENS_RCVMSG_ERR_EID, CFE_EVS_ERROR,
			  "SCH pipe read error (0x%08lX).", iStatus);
    }

    return iStatus;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SENS::ProcessCmdPipe()
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
                case SENS_CMD_MID:
                    ProcessAppCmds(CmdMsgPtr);
                    break;

                default:
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with this
                     *  pipe, but not handled in this switch-case.) */
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(SENS_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            (void) CFE_EVS_SendEvent(SENS_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08lX)", iStatus);
            break;
        }
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process SENS Commands                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SENS::ProcessAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case SENS_NOOP_CC:
                HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(SENS_CMD_NOOP_EID, CFE_EVS_INFORMATION,
					"Recvd NOOP. Version %d.%d.%d.%d",
					SENS_MAJOR_VERSION,
					SENS_MINOR_VERSION,
					SENS_REVISION,
					SENS_MISSION_REV);
                break;

            case SENS_RESET_CC:
                HkTlm.usCmdCnt = 0;
                HkTlm.usCmdErrCnt = 0;
                break;

            default:
                HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(SENS_CC_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid command code (%u)", (unsigned int)uiCmdCode);
                break;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send SENS Housekeeping                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SENS::ReportHousekeeping()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&HkTlm);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Publish Output Data                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SENS::SendActuatorControls3Msg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&ActuatorControls3Msg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&ActuatorControls3Msg);
}

void SENS::SendBatteryStatusMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&BatteryStatusMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&BatteryStatusMsg);
}

void SENS::SendAirspeedMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&AirspeedMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&AirspeedMsg);
}

void SENS::SendRcChannelsMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&RcChannelsMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&RcChannelsMsg);
}

void SENS::SendManualControlSetpointMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&ManualControlSetpointMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&ManualControlSetpointMsg);
}

//void SENS::SendSensorPreflightMsg()
//{
//    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&SensorPreflightMsg);
//    CFE_SB_SendMsg((CFE_SB_Msg_t*)&SensorPreflightMsg);
//}

void SENS::SendSensorCombinedMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&SensorCombinedMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&SensorCombinedMsg);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Command Length                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean SENS::VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
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

            (void) CFE_EVS_SendEvent(SENS_MSGLEN_ERR_EID, CFE_EVS_ERROR,
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
/* SENS Application C style main entry point.                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
extern "C" void SENS_AppMain()
{
    oSENS.AppMain();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* SENS Application C++ style main entry point.                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SENS::AppMain()
{
    /* Register the application with Executive Services */
    uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("SENS - Failed to register the app (0x%08lX)\n", iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(SENS_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(SENS_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(SENS_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(SENS_MAIN_TASK_PERF_ID);
    }
    else
    {
        uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&uiRunStatus) == TRUE)
    {
        RcvSchPipeMsg(SENS_SCH_PIPE_PEND_TIME);

        iStatus = AcquireConfigPointers();
        if(iStatus != CFE_SUCCESS)
        {
            /* We apparently tried to load a new table but failed.  Terminate the application. */
            uiRunStatus = CFE_ES_APP_ERROR;
        }
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(SENS_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(uiRunStatus);
}




float SENS::GetRcValue(uint8 func, float min_value, float max_value)
{
	if (RcChannelsMsg.Function[func] >= 0)
    {
		float value = RcChannelsMsg.Channels[RcChannelsMsg.Function[func]];
		return math::constrain(value, min_value, max_value);
	}
	else
	{
		return 0.0f;
	}
}



PX4_SwitchPos_t SENS::GetRcSw3PosPosition(uint8 func, float on_th, bool on_inv, float mid_th, bool mid_inv)
{
	if (RcChannelsMsg.Function[func] >= 0)
	{
		float value = 0.5f * RcChannelsMsg.Channels[RcChannelsMsg.Function[func]] + 0.5f;

		if (on_inv ? value < on_th : value > on_th)
		{
			return PX4_SWITCH_POS_ON;

		}
		else if (mid_inv ? value < mid_th : value > mid_th)
		{
			return PX4_SWITCH_POS_MIDDLE;

		}
		else
		{
			return PX4_SWITCH_POS_OFF;
		}
	}
	else
	{
		return PX4_SWITCH_POS_NONE;
	}
}


PX4_SwitchPos_t SENS::GetRcSw2PosPosition(uint8 func, float on_th, bool on_inv)
{
	if (RcChannelsMsg.Function[func] >= 0)
	{
		float value = 0.5f * RcChannelsMsg.Channels[RcChannelsMsg.Function[func]] + 0.5f;

		if (on_inv ? value < on_th : value > on_th)
		{
			return PX4_SWITCH_POS_ON;

		}
		else
		{
			return PX4_SWITCH_POS_OFF;
		}

	}
	else
	{
		return PX4_SWITCH_POS_NONE;
	}
}



void SENS::UpdateRcFunctions()
{
	/* Update RC function mappings */
	RcChannelsMsg.Function[PX4_RC_CHANNELS_FUNCTION_THROTTLE] = (PX4_RcChannelFunction_t)(ConfigTblPtr->MapThrottle - 1);
	RcChannelsMsg.Function[PX4_RC_CHANNELS_FUNCTION_ROLL] = (PX4_RcChannelFunction_t)(ConfigTblPtr->MapRoll - 1);
	RcChannelsMsg.Function[PX4_RC_CHANNELS_FUNCTION_PITCH] = (PX4_RcChannelFunction_t)(ConfigTblPtr->MapPitch - 1);
	RcChannelsMsg.Function[PX4_RC_CHANNELS_FUNCTION_YAW] = (PX4_RcChannelFunction_t)(ConfigTblPtr->MapYaw - 1);

	RcChannelsMsg.Function[PX4_RC_CHANNELS_FUNCTION_MODE] = (PX4_RcChannelFunction_t)(ConfigTblPtr->MapModeSw - 1);
	RcChannelsMsg.Function[PX4_RC_CHANNELS_FUNCTION_RETURN] = (PX4_RcChannelFunction_t)(ConfigTblPtr->MapReturnSw - 1);
	RcChannelsMsg.Function[PX4_RC_CHANNELS_FUNCTION_RATTITUDE] = (PX4_RcChannelFunction_t)(ConfigTblPtr->MapRattitudeSw - 1);
	RcChannelsMsg.Function[PX4_RC_CHANNELS_FUNCTION_POSCTL] = (PX4_RcChannelFunction_t)(ConfigTblPtr->MapPosctlSw - 1);
	RcChannelsMsg.Function[PX4_RC_CHANNELS_FUNCTION_LOITER] = (PX4_RcChannelFunction_t)(ConfigTblPtr->MapLoiterSw - 1);
	RcChannelsMsg.Function[PX4_RC_CHANNELS_FUNCTION_ACRO] = (PX4_RcChannelFunction_t)(ConfigTblPtr->MapAcroSw - 1);
	RcChannelsMsg.Function[PX4_RC_CHANNELS_FUNCTION_OFFBOARD] = (PX4_RcChannelFunction_t)(ConfigTblPtr->MapOffboardSw - 1);
	RcChannelsMsg.Function[PX4_RC_CHANNELS_FUNCTION_KILLSWITCH] = (PX4_RcChannelFunction_t)(ConfigTblPtr->MapKillSw - 1);
	RcChannelsMsg.Function[PX4_RC_CHANNELS_FUNCTION_ARMSWITCH] = (PX4_RcChannelFunction_t)(ConfigTblPtr->MapArmSw - 1);
	RcChannelsMsg.Function[PX4_RC_CHANNELS_FUNCTION_TRANSITION] = (PX4_RcChannelFunction_t)(ConfigTblPtr->MapTransSw - 1);
	RcChannelsMsg.Function[PX4_RC_CHANNELS_FUNCTION_GEAR] = (PX4_RcChannelFunction_t)(ConfigTblPtr->MapGearSw - 1);
	RcChannelsMsg.Function[PX4_RC_CHANNELS_FUNCTION_STAB] = (PX4_RcChannelFunction_t)(ConfigTblPtr->MapStabSw - 1);
	RcChannelsMsg.Function[PX4_RC_CHANNELS_FUNCTION_MAN] = (PX4_RcChannelFunction_t)(ConfigTblPtr->MapManSw - 1);

	RcChannelsMsg.Function[PX4_RC_CHANNELS_FUNCTION_FLAPS] = (PX4_RcChannelFunction_t)(ConfigTblPtr->MapFlaps - 1);
	RcChannelsMsg.Function[PX4_RC_CHANNELS_FUNCTION_ALTCTL] = (PX4_RcChannelFunction_t)(ConfigTblPtr->MapAltctlSw - 1);

	RcChannelsMsg.Function[PX4_RC_CHANNELS_FUNCTION_AUX_1] = (PX4_RcChannelFunction_t)(ConfigTblPtr->MapAux1 - 1);
	RcChannelsMsg.Function[PX4_RC_CHANNELS_FUNCTION_AUX_2] = (PX4_RcChannelFunction_t)(ConfigTblPtr->MapAux2 - 1);
	RcChannelsMsg.Function[PX4_RC_CHANNELS_FUNCTION_AUX_3] = (PX4_RcChannelFunction_t)(ConfigTblPtr->MapAux3 - 1);
	RcChannelsMsg.Function[PX4_RC_CHANNELS_FUNCTION_AUX_4] = (PX4_RcChannelFunction_t)(ConfigTblPtr->MapAux4 - 1);
	RcChannelsMsg.Function[PX4_RC_CHANNELS_FUNCTION_AUX_5] = (PX4_RcChannelFunction_t)(ConfigTblPtr->MapAux5 - 1);
	

	for (int i = 0; i < PX4_RC_PARAM_MAP_NCHAN; i++)
	{
		RcChannelsMsg.Function[PX4_RC_CHANNELS_FUNCTION_PARAM_1 + i] = (PX4_RcChannelFunction_t)(ConfigTblPtr->MapParam[i] - 1);
	}

	/* Update the RC low pass filter frequencies */
	FilterRoll.set_cutoff_frequency(ConfigTblPtr->FltSmpRate, ConfigTblPtr->FltCutoff);
	FilterPitch.set_cutoff_frequency(ConfigTblPtr->FltSmpRate, ConfigTblPtr->FltCutoff);
	FilterYaw.set_cutoff_frequency(ConfigTblPtr->FltSmpRate, ConfigTblPtr->FltCutoff);
	FilterThrottle.set_cutoff_frequency(ConfigTblPtr->FltSmpRate, ConfigTblPtr->FltCutoff);
	FilterRoll.reset(0.0f);
	FilterPitch.reset(0.0f);
	FilterYaw.reset(0.0f);
	FilterThrottle.reset(0.0f);
}


void SENS::ProcessRCInput(void)
{
	if(CVT.InputRcMsg.Timestamp > CVT.LastInputRcTime)
	{
		/* Read low-level values from FMU or IO RC inputs (PPM, Spektrum, S.Bus) */
		//struct rc_input_values rc_input;

		/* Detect RC signal loss */
		bool signal_lost;

		/* Check flags and require at least four channels to consider the signal valid */
		if (CVT.InputRcMsg.RcLost || CVT.InputRcMsg.RcFailsafe || CVT.InputRcMsg.ChannelCount < 4)
		{
			/* Signal is lost or no enough channels */
			signal_lost = true;
		}
		else
		{
			/* Signal looks good */
			signal_lost = false;

			/* Check failsafe */
			int8 fs_ch = RcChannelsMsg.Function[ConfigTblPtr->MapFailsafe];

			/* If not 0, use channel number instead of rc.function mapping */
			if (ConfigTblPtr->MapFailsafe > 0)
			{
				fs_ch = ConfigTblPtr->MapFailsafe - 1;
			}

			if (ConfigTblPtr->FailsThr > 0 && fs_ch >= 0)
			{
				/* Failsafe configured */
				if ((ConfigTblPtr->FailsThr < ConfigTblPtr->Min[fs_ch] && CVT.InputRcMsg.Values[fs_ch] < ConfigTblPtr->FailsThr) ||
					(ConfigTblPtr->FailsThr > ConfigTblPtr->Max[fs_ch] && CVT.InputRcMsg.Values[fs_ch] > ConfigTblPtr->FailsThr))
				{
					/* Failsafe triggered, signal is lost by receiver */
					signal_lost = true;
				}
			}
		}

		uint32 channel_limit = CVT.InputRcMsg.ChannelCount;

		if (channel_limit > PX4_RC_CHANNELS_FUNCTION_COUNT) {
			channel_limit = PX4_RC_CHANNELS_FUNCTION_COUNT;
		}

		/* Read out and scale values from raw message even if signal is invalid */
		for (uint32 i = 0; i < channel_limit; i++)
		{
			/*
			 * 1) Constrain to min/max values, as later processing depends on bounds.
			 */
			if (CVT.InputRcMsg.Values[i] < ConfigTblPtr->Min[i]) {
				CVT.InputRcMsg.Values[i] = ConfigTblPtr->Min[i];
			}

			if (CVT.InputRcMsg.Values[i] > ConfigTblPtr->Max[i]) {
				CVT.InputRcMsg.Values[i] = ConfigTblPtr->Max[i];
			}

			/*
			 * 2) Scale around the mid point differently for lower and upper range.
			 *
			 * This is necessary as they don't share the same endpoints and slope.
			 *
			 * First normalize to 0..1 range with correct sign (below or above center),
			 * the total range is 2 (-1..1).
			 * If center (trim) == min, scale to 0..1, if center (trim) == max,
			 * scale to -1..0.
			 *
			 * As the min and max bounds were enforced in step 1), division by zero
			 * cannot occur, as for the case of center == min or center == max the if
			 * statement is mutually exclusive with the arithmetic NaN case.
			 *
			 * DO NOT REMOVE OR ALTER STEP 1!
			 */
			if (CVT.InputRcMsg.Values[i] > (ConfigTblPtr->Trim[i] + ConfigTblPtr->DZ[i]))
			{
				RcChannelsMsg.Channels[i] = (CVT.InputRcMsg.Values[i] - ConfigTblPtr->Trim[i] - ConfigTblPtr->DZ[i]) / (float)(
						ConfigTblPtr->Max[i] - ConfigTblPtr->Trim[i] - ConfigTblPtr->DZ[i]);

				RcChannelsMsg.ChannelCount = CVT.InputRcMsg.ChannelCount;
				RcChannelsMsg.RSSI = CVT.InputRcMsg.RSSI;
				RcChannelsMsg.SignalLost = signal_lost;
				RcChannelsMsg.Timestamp = CVT.InputRcMsg.Timestamp;
				RcChannelsMsg.TimestampLastValid = CVT.InputRcMsg.LastSignal;
				RcChannelsMsg.FrameDropCount = CVT.InputRcMsg.RcLostFrameCount;
			}
			else if (CVT.InputRcMsg.Values[i] < (ConfigTblPtr->Trim[i] - ConfigTblPtr->DZ[i]))
			{
				RcChannelsMsg.Channels[i] = (CVT.InputRcMsg.Values[i] - ConfigTblPtr->Trim[i] + ConfigTblPtr->DZ[i]) / (float)(
						ConfigTblPtr->Trim[i] - ConfigTblPtr->Min[i] - ConfigTblPtr->DZ[i]);

			}
			else
			{
				/* in the configured dead zone, output zero */
				RcChannelsMsg.Channels[i] = 0.0f;
			}

			RcChannelsMsg.Channels[i] *= ConfigTblPtr->Rev[i];

			/* handle any parameter-induced blowups */
			if (!isfinite(RcChannelsMsg.Channels[i])) {
				RcChannelsMsg.Channels[i] = 0.0f;
			}
		}

		RcChannelsMsg.ChannelCount = CVT.InputRcMsg.ChannelCount;
		RcChannelsMsg.RSSI = CVT.InputRcMsg.RSSI;
		RcChannelsMsg.SignalLost = signal_lost;
		RcChannelsMsg.Timestamp = CVT.InputRcMsg.LastSignal;
		RcChannelsMsg.FrameDropCount = CVT.InputRcMsg.RcLostFrameCount;

		/* Publish rc_channels topic even if signal is invalid, for debug */
		SendRcChannelsMsg();

		/* Only publish manual control if the signal is still present and was present once */
		if (!signal_lost && CVT.InputRcMsg.LastSignal > 0)
		{
			/* Set mode slot to unassigned */
			ManualControlSetpointMsg.ModeSlot = PX4_MODE_SLOT_NONE;

			/* Set the timestamp to the last signal time */
			ManualControlSetpointMsg.Timestamp = CVT.InputRcMsg.LastSignal;
			ManualControlSetpointMsg.DataSource = PX4_MANUAL_CONTROL_SOURCE_RC;

			/* Limit controls */
			ManualControlSetpointMsg.Y = GetRcValue(PX4_RC_CHANNELS_FUNCTION_ROLL, -1.0, 1.0);
			ManualControlSetpointMsg.X = GetRcValue(PX4_RC_CHANNELS_FUNCTION_PITCH, -1.0, 1.0);
			ManualControlSetpointMsg.R = GetRcValue(PX4_RC_CHANNELS_FUNCTION_YAW, -1.0, 1.0);
			ManualControlSetpointMsg.Z = GetRcValue(PX4_RC_CHANNELS_FUNCTION_THROTTLE, 0.0, 1.0);

			ManualControlSetpointMsg.Flaps = GetRcValue(PX4_RC_CHANNELS_FUNCTION_FLAPS, -1.0, 1.0);
			ManualControlSetpointMsg.Aux1 = GetRcValue(PX4_RC_CHANNELS_FUNCTION_AUX_1, -1.0, 1.0);
			ManualControlSetpointMsg.Aux2 = GetRcValue(PX4_RC_CHANNELS_FUNCTION_AUX_2, -1.0, 1.0);
			ManualControlSetpointMsg.Aux3 = GetRcValue(PX4_RC_CHANNELS_FUNCTION_AUX_3, -1.0, 1.0);
			ManualControlSetpointMsg.Aux4 = GetRcValue(PX4_RC_CHANNELS_FUNCTION_AUX_4, -1.0, 1.0);
			ManualControlSetpointMsg.Aux5 = GetRcValue(PX4_RC_CHANNELS_FUNCTION_AUX_5, -1.0, 1.0);

			/* Filter controls */
			//ManualControlSetpointMsg.Y = math::constrain(FilterRoll.apply(ManualControlSetpointMsg.Y), -1.f, 1.f);
			//ManualControlSetpointMsg.X = math::constrain(FilterPitch.apply(ManualControlSetpointMsg.X), -1.f, 1.f);
			//ManualControlSetpointMsg.R = math::constrain(FilterYaw.apply(ManualControlSetpointMsg.R), -1.f, 1.f);
			//ManualControlSetpointMsg.Z = math::constrain(FilterThrottle.apply(ManualControlSetpointMsg.Z), 0.f, 1.f););

			if (ConfigTblPtr->MapFlightMode > 0)
			{
				/* The number of valid slots equals the index of the max marker minus one */
				const int num_slots = PX4_MODE_SLOT_MAX;

				/* The half width of the range of a slot is the total range
				 * divided by the number of slots, again divided by two
				 */
				const float slot_width_half = 2.0f / num_slots / 2.0f;

				/* min is -1, max is +1, range is 2. We offset below min and max */
				const float slot_min = -1.0f - 0.05f;
				const float slot_max = 1.0f + 0.05f;

				/* The slot gets mapped by first normalizing into a 0..1 interval using min
				 * and max. Then the right slot is obtained by multiplying with the number of
				 * slots. And finally we add half a slot width to ensure that integer rounding
				 * will take us to the correct final index.
				 */
				ManualControlSetpointMsg.ModeSlot = (PX4_ModeSlot_t)(((((RcChannelsMsg.Channels[ConfigTblPtr->MapFlightMode - 1] - slot_min) * num_slots) + slot_width_half) /
							 (slot_max - slot_min)) + (1.0f / num_slots));

				if (ManualControlSetpointMsg.ModeSlot >= num_slots)
				{
					ManualControlSetpointMsg.ModeSlot = (PX4_ModeSlot_t)(num_slots - 1);
				}
			}

			/* Mode switches */
			ManualControlSetpointMsg.ModeSwitch = GetRcSw3PosPosition(PX4_RC_CHANNELS_FUNCTION_MODE,
					ConfigTblPtr->AutoTh, (ConfigTblPtr->AutoTh < 0),
					ConfigTblPtr->AssistTh, (ConfigTblPtr->AssistTh < 0));
			ManualControlSetpointMsg.RattitudeSwitch = GetRcSw2PosPosition(PX4_RC_CHANNELS_FUNCTION_RATTITUDE,
					ConfigTblPtr->RattitudeTh, (ConfigTblPtr->RattitudeTh < 0));
			ManualControlSetpointMsg.PosctlSwitch = GetRcSw2PosPosition(PX4_RC_CHANNELS_FUNCTION_POSCTL,
					ConfigTblPtr->PosctlTh, (ConfigTblPtr->PosctlTh < 0));
			ManualControlSetpointMsg.ReturnSwitch = GetRcSw2PosPosition(PX4_RC_CHANNELS_FUNCTION_RETURN,
					ConfigTblPtr->ReturnTh, (ConfigTblPtr->ReturnTh < 0));
			ManualControlSetpointMsg.LoiterSwitch = GetRcSw2PosPosition(PX4_RC_CHANNELS_FUNCTION_LOITER,
					ConfigTblPtr->LoiterTh, (ConfigTblPtr->LoiterTh < 0));
			ManualControlSetpointMsg.AcroSwitch = GetRcSw2PosPosition(PX4_RC_CHANNELS_FUNCTION_ACRO,
					ConfigTblPtr->AcroTh, (ConfigTblPtr->AcroTh < 0));
			ManualControlSetpointMsg.OffboardSwitch = GetRcSw2PosPosition(PX4_RC_CHANNELS_FUNCTION_OFFBOARD,
					ConfigTblPtr->OffboardTh, (ConfigTblPtr->OffboardTh < 0));
			ManualControlSetpointMsg.KillSwitch = GetRcSw2PosPosition(PX4_RC_CHANNELS_FUNCTION_KILLSWITCH,
					ConfigTblPtr->KillswitchTh, (ConfigTblPtr->KillswitchTh < 0));
			ManualControlSetpointMsg.ArmSwitch = GetRcSw2PosPosition(PX4_RC_CHANNELS_FUNCTION_ARMSWITCH,
					ConfigTblPtr->ArmswitchTh, (ConfigTblPtr->ArmswitchTh < 0));
			ManualControlSetpointMsg.TransitionSwitch = GetRcSw2PosPosition(PX4_RC_CHANNELS_FUNCTION_TRANSITION,
					ConfigTblPtr->TransTh, (ConfigTblPtr->TransTh < 0));
			ManualControlSetpointMsg.GearSwitch = GetRcSw2PosPosition(PX4_RC_CHANNELS_FUNCTION_GEAR,
					ConfigTblPtr->GearTh, (ConfigTblPtr->GearTh < 0));
			ManualControlSetpointMsg.StabSwitch = GetRcSw2PosPosition(PX4_RC_CHANNELS_FUNCTION_STAB,
					ConfigTblPtr->StabTh, (ConfigTblPtr->StabTh < 0));
			ManualControlSetpointMsg.ManSwitch = GetRcSw2PosPosition(PX4_RC_CHANNELS_FUNCTION_MAN,
					ConfigTblPtr->ManTh, (ConfigTblPtr->ManTh < 0));
			ManualControlSetpointMsg.AltctlSwitch = GetRcSw2PosPosition(PX4_RC_CHANNELS_FUNCTION_ALTCTL,
					ConfigTblPtr->AltctlTh, (ConfigTblPtr->AltctlTh < 0));

            /* Set flight mode flags according to mode switch. TODO: Make modes configurable */
            if(1 == ConfigTblPtr->ModeSwitchControl)
            {
                ManualControlSetpointMsg.AltctlSwitch = PX4_SWITCH_POS_OFF;
                ManualControlSetpointMsg.PosctlSwitch = PX4_SWITCH_POS_OFF;

                if(ManualControlSetpointMsg.ModeSwitch == PX4_SWITCH_POS_OFF)
                {
                    // Do nothing
                }
                else if(ManualControlSetpointMsg.ModeSwitch == PX4_SWITCH_POS_MIDDLE)
                {
                    ManualControlSetpointMsg.AltctlSwitch = PX4_SWITCH_POS_ON;
                }
                else if(ManualControlSetpointMsg.ModeSwitch == PX4_SWITCH_POS_ON)
                {
                    ManualControlSetpointMsg.PosctlSwitch = PX4_SWITCH_POS_ON;
                }
                else
                {
                    // Invalid mode switch pos
                }
            }

			/* Copy from mapped manual control to control group 3 */
			ActuatorControls3Msg.Timestamp = CVT.InputRcMsg.LastSignal;

			ActuatorControls3Msg.Control[0] = ManualControlSetpointMsg.Y;
			ActuatorControls3Msg.Control[1] = ManualControlSetpointMsg.X;
			ActuatorControls3Msg.Control[2] = ManualControlSetpointMsg.R;
			ActuatorControls3Msg.Control[3] = ManualControlSetpointMsg.Z;
			ActuatorControls3Msg.Control[4] = ManualControlSetpointMsg.Flaps;
			ActuatorControls3Msg.Control[5] = ManualControlSetpointMsg.Aux1;
			ActuatorControls3Msg.Control[6] = ManualControlSetpointMsg.Aux2;
			ActuatorControls3Msg.Control[7] = ManualControlSetpointMsg.Aux3;

			/* Publish Manual Control Setpoint message */
			SendManualControlSetpointMsg();

			/* Publish Actuator Controls 3 message */
			SendActuatorControls3Msg();
		}

		CVT.LastInputRcTime = CVT.InputRcMsg.Timestamp;
	}
}


void SENS::CombineSensorInput(void)
{
    SensorCombinedMsg.Timestamp = CVT.SensorGyroMsg.Timestamp;

    /* Gyro */
	/* See if we have a new gyro measurement.  Gyro is a required sensor, so we only
	 * publish a new message if there is a new gyro measurement. */
    if(CVT.SensorGyroMsg.Timestamp > CVT.LastGyroTime)
    {
    	/* We do have a new measurement.  Update the fields accordingly.
    	 * First populate the measurement values.
    	 */
    	SensorCombinedMsg.GyroRad[0] = CVT.SensorGyroMsg.X;
    	SensorCombinedMsg.GyroRad[1] = CVT.SensorGyroMsg.Y;
    	SensorCombinedMsg.GyroRad[2] = CVT.SensorGyroMsg.Z;

    	/* Finally, calculate and populate the integral dt field, in seconds.  If the incoming
    	 * message does not include a integral dt field, estimate the integral by using the delta
    	 * from the previously received message that we calculated above.
    	 */
		if(CVT.SensorGyroMsg.IntegralDt == 0)
		{
			/* Calculate an estimate from the previous value.  But use
			 * an even less accurate estimate if we don't yet have a
			 * previous time.
			 */
	    	uint32 deltaTimeUs = 0;

	    	if(CVT.LastGyroTime == 0)
	    	{
	    		deltaTimeUs = CVT.SensorGyroMsg.Timestamp - 1000;
	    	}
	    	else
	    	{
	    		deltaTimeUs = CVT.SensorGyroMsg.Timestamp - CVT.LastGyroTime;
	    	}

			SensorCombinedMsg.GyroIntegralDt = deltaTimeUs / 1000000.0f;
		}
		else
		{
			/* The sample includes an integral dt time.  Use this but
			 * convert it to seconds.
			 */
			SensorCombinedMsg.GyroIntegralDt = CVT.SensorGyroMsg.IntegralDt / 1000000.0f;
		}

		/* Store the time so we can use it in the next iteration. */
    	CVT.LastGyroTime = CVT.SensorGyroMsg.Timestamp;

		/* Accelerometer. */
		/* See if we have a new accelerometer measurement. */
		if(CVT.SensorAccelMsg.Timestamp > CVT.LastAccelTime)
		{
			/* We do have a new measurement.  Update the fields accordingly.
			 * First populate the new values.
			 */
			SensorCombinedMsg.Acc[0] = CVT.SensorAccelMsg.X;
			SensorCombinedMsg.Acc[1] = CVT.SensorAccelMsg.Y;
			SensorCombinedMsg.Acc[2] = CVT.SensorAccelMsg.Z;

			/* Now calculate and populate the time relative to the main timestamp. */
			SensorCombinedMsg.AccTimestamp = CVT.SensorAccelMsg.Timestamp;

			/* Finally, calculate and pupulate the integral dt field, in seconds.  If the incoming
			 * message does not include a integral dt field, estimate the integral by using the delta
			 * from the previously received message that we calculated above.
			 */
			if(CVT.SensorAccelMsg.IntegralDt == 0)
			{
				/* Calculate an estimate from the previous value.  But use
				 * an even less accurate estimate if we don't yet have a
				 * previous time.
				 */
				uint32 deltaTimeUs = 0;

				if(CVT.LastAccelTime == 0)
				{
					deltaTimeUs = CVT.SensorAccelMsg.Timestamp - 1000;
				}
				else
				{
					deltaTimeUs = CVT.SensorAccelMsg.Timestamp - CVT.LastAccelTime;
				}

				SensorCombinedMsg.AccIntegralDt = deltaTimeUs / 1000000.0f;
			}
			else
			{
				/* The sample includes an integral dt time.  Use this but
				 * convert it to seconds.
				 */
				SensorCombinedMsg.AccIntegralDt = CVT.SensorAccelMsg.IntegralDt / 1000000.0f;
			}

			SensorCombinedMsg.AccInvalid = false;

			/* Store the time so we can use it in the next iteration. */
			CVT.LastAccelTime = CVT.SensorAccelMsg.Timestamp;
		}
		else
		{
			/* No new measurement was received.  Update the fields accordingly.  */
			//CVT.SensorAccelMsg.Timestamp  = PX4_RELATIVE_TIMESTAMP_INVALID;
			SensorCombinedMsg.AccInvalid = true;
		}

		/* Mag. */
		/* See if we have a new magnetometer measurement. */
		if(CVT.SensorMagMsg.Timestamp > CVT.LastMagTime)
		{
			/* We do have a new measurement.  Update the fields accordingly.
			 * First populate the new values.
			 */
			SensorCombinedMsg.Mag[0] = CVT.SensorMagMsg.X;
			SensorCombinedMsg.Mag[1] = CVT.SensorMagMsg.Y;
			SensorCombinedMsg.Mag[2] = CVT.SensorMagMsg.Z;

			/* Now calculate and populate the time relative to the main timestamp. */
			SensorCombinedMsg.MagTimestamp = CVT.SensorMagMsg.Timestamp;

			SensorCombinedMsg.MagInvalid = false;

			/* Store the time so we can use it in the next iteration. */
			CVT.LastMagTime = CVT.SensorMagMsg.Timestamp;
		}
		else
		{
			/* No new measurement was received.  Update the fields accordingly.  */
			//CVT.SensorMagMsg.Timestamp  = PX4_RELATIVE_TIMESTAMP_INVALID;
			SensorCombinedMsg.MagInvalid = true;
		}

		/* Baro. */
		/* See if we have a new baro measurement. */
		//baro_timestamp = sensors.timestamp + sensors.baro_timestamp_relative;
		if(CVT.SensorBaroMsg.Timestamp > CVT.LastBaroTime)
		{
			/* We do have a new measurement.  Update the fields accordingly.
			 * First populate the new values.
			 */
			SensorCombinedMsg.BaroAlt = CVT.SensorBaroMsg.Altitude;
			SensorCombinedMsg.BaroTemp = CVT.SensorBaroMsg.Temperature;

			/* Now calculate and populate the time relative to the main timestamp. */
			if(SensorCombinedMsg.Timestamp > CVT.SensorBaroMsg.Timestamp)
			{
                /* if gyro is after baro */
                SensorCombinedMsg.BaroTimestamp = CVT.SensorBaroMsg.Timestamp;
			}
			else if (SensorCombinedMsg.Timestamp < CVT.SensorBaroMsg.Timestamp)
			{
                /* if gyro is before baro */
                SensorCombinedMsg.BaroTimestamp = CVT.SensorBaroMsg.Timestamp;
			}
            else
            {
                /* should never get here */
                SensorCombinedMsg.BaroTimestamp = 0;
            }

			SensorCombinedMsg.BaroInvalid = false;

			/* Store the time so we can use it in the next iteration. */
			CVT.LastBaroTime = CVT.SensorBaroMsg.Timestamp;
		}
		else
		{
			/* No new measurement was received.  Update the fields accordingly.  */
			//CVT.SensorBaroMsg.Timestamp  = PX4_RELATIVE_TIMESTAMP_INVALID;
			SensorCombinedMsg.BaroInvalid = true;
		}

		/* The message is ready for publishing.  Push it out the door. */
		SendSensorCombinedMsg();
	}
}


void SENS::CyclicProcessing(void)
{
	CombineSensorInput();
	ProcessRCInput();
}




/************************/
/*  End of File Comment */
/************************/
