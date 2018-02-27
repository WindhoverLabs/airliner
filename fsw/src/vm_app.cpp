/************************************************************************
** Includes
*************************************************************************/
#include <string.h>

#include "cfe.h"

#include "vm_app.h"
#include "vm_msg.h"
#include "vm_version.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Instantiate the application object.                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
VM oVM;



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Default constructor.                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
VM::VM() :
	ArmingSM(*this),
	MainSM(*this),
	NavigationSM(*this)
{

}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Destructor constructor.                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
VM::~VM()
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables.                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 VM::InitEvent()
{
    int32  iStatus=CFE_SUCCESS;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(0, 0, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("VM - Failed to register with EVS (0x%08lX)\n", iStatus);
    }

    return iStatus;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 VM::InitPipe()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&SchPipeId,
    		VM_SCH_PIPE_DEPTH,
			VM_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(VM_WAKEUP_MID, SchPipeId, CFE_SB_Default_Qos, VM_WAKEUP_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
            		"Sch Pipe failed to subscribe to VM_WAKEUP_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(VM_SEND_HK_MID, SchPipeId, CFE_SB_Default_Qos, VM_SEND_HK_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to VM_SEND_HK_MID. (0x%08X)",
					 (unsigned int)iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_SENSOR_MAG_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to PX4_SENSOR_MAG_MID. (0x%08lX)",
					 iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_SENSOR_GYRO_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to PX4_SENSOR_GYRO_MID. (0x%08lX)",
					 iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_SYSTEM_POWER_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to PX4_SYSTEM_POWER_MID. (0x%08lX)",
					 iStatus);
            goto VM_InitPipe_Exit_Tag;
        }

//        iStatus = CFE_SB_SubscribeEx(PX4_SENSOR_PREFLIGHT_MID, SchPipeId, CFE_SB_Default_Qos, 1);
//        if (iStatus != CFE_SUCCESS)
//        {
//            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
//					 "CMD Pipe failed to subscribe to PX4_SENSOR_PREFLIGHT_MID. (0x%08lX)",
//					 iStatus);
//            goto VM_InitPipe_Exit_Tag;
//        }

        iStatus = CFE_SB_SubscribeEx(PX4_TELEMETRY_STATUS_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to PX4_TELEMETRY_STATUS_MID. (0x%08lX)",
					 iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_SUBSYSTEM_INFO_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to PX4_SUBSYSTEM_INFO_MID. (0x%08lX)",
					 iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_GPS_POSITION_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to PX4_VEHICLE_GPS_POSITION_MID. (0x%08lX)",
					 iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_ATTITUDE_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to PX4_VEHICLE_ATTITUDE_MID. (0x%08lX)",
					 iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_LOCAL_POSITION_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to PX4_VEHICLE_LOCAL_POSITION_MID. (0x%08lX)",
					 iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_LAND_DETECTED_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to PX4_VEHICLE_LAND_DETECTED_MID. (0x%08lX)",
					 iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_GEOFENCE_RESULT_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to PX4_GEOFENCE_RESULT_MID. (0x%08lX)",
					 iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_MISSION_RESULT_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to PX4_MISSION_RESULT_MID. (0x%08lX)",
					 iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_MANUAL_CONTROL_SETPOINT_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to PX4_MANUAL_CONTROL_SETPOINT_MID. (0x%08lX)",
					 iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_POSITION_SETPOINT_TRIPLET_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to PX4_POSITION_SETPOINT_TRIPLET_MID. (0x%08lX)",
					 iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_OFFBOARD_CONTROL_MODE_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to PX4_OFFBOARD_CONTROL_MODE_MID. (0x%08lX)",
					 iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_SENSOR_ACCEL_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to PX4_SENSOR_ACCEL_MID. (0x%08lX)",
					 iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_SAFETY_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to PX4_SAFETY_MID. (0x%08lX)",
					 iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_SENSOR_CORRECTION_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to PX4_SENSOR_CORRECTION_MID. (0x%08lX)",
					 iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_STATUS_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to PX4_VEHICLE_STATUS_MID. (0x%08lX)",
					 iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_SENSOR_COMBINED_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to PX4_SENSOR_COMBINED_MID. (0x%08lX)",
					 iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(VM_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
			 "Failed to create SCH pipe (0x%08lX)",
			 iStatus);
        goto VM_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&CmdPipeId,
    		VM_CMD_PIPE_DEPTH,
			VM_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(VM_CMD_MID, CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
				 "CMD Pipe failed to subscribe to VM_CMD_MID. (0x%08lX)",
				 iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(VM_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
			 "Failed to create CMD pipe (0x%08lX)",
			 iStatus);
        goto VM_InitPipe_Exit_Tag;
    }

VM_InitPipe_Exit_Tag:
    return iStatus;
}
    

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM::InitData()
{
	/* Init housekeeping message. */
	CFE_SB_InitMsg(&HkTlm,
		VM_HK_TLM_MID, sizeof(HkTlm), TRUE);

	/* Init output messages */
	CFE_SB_InitMsg(&ActuatorArmedMsg,
		PX4_ACTUATOR_ARMED_MID, sizeof(PX4_ActuatorArmedMsg_t), TRUE);

	CFE_SB_InitMsg(&HomePositionMsg,
		PX4_HOME_POSITION_MID, sizeof(PX4_HomePositionMsg_t), TRUE);

	CFE_SB_InitMsg(&VehicleManagerStateMsg,
		PX4_COMMANDER_STATE_MID, sizeof(PX4_CommanderStateMsg_t), TRUE);

	CFE_SB_InitMsg(&MissionMsg,
		PX4_MISSION_MID, sizeof(PX4_MissionMsg_t), TRUE);

	CFE_SB_InitMsg(&LedControlMsg,
		PX4_LED_CONTROL_MID, sizeof(PX4_LedControlMsg_t), TRUE);

	CFE_SB_InitMsg(&VehicleStatusMsg,
		PX4_VEHICLE_STATUS_MID, sizeof(PX4_VehicleStatusMsg_t), TRUE);

//	CFE_SB_InitMsg(&VehicleRoiMsg,
//		PX4_VEHICLE_ROI_MID, sizeof(PX4_VehicleRoiMsg_t), TRUE);

	ConditionLocalPositionValid = true;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* VM initialization                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 VM::InitApp()
{
    int32  iStatus   = CFE_SUCCESS;
    int8   hasEvents = 0;

    iStatus = InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("VM - Failed to init events (0x%08lX)\n", iStatus);
        goto VM_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        goto VM_InitApp_Exit_Tag;
    }

    InitData();

    iStatus = InitConfigTbl();
    if (iStatus != CFE_SUCCESS)
    {
        goto VM_InitApp_Exit_Tag;
    }

VM_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(VM_INIT_INF_EID, CFE_EVS_INFORMATION,
                                 "Initialized.  Version %d.%d.%d.%d",
								 VM_MAJOR_VERSION,
								 VM_MINOR_VERSION,
								 VM_REVISION,
								 VM_MISSION_REV);
    }
    else
    {
        if (hasEvents == 1)
        {
            (void) CFE_ES_WriteToSysLog("VM - Application failed to initialize\n");
        }
    }

    return iStatus;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 VM::RcvSchPipeMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(VM_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(VM_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
        {
            case VM_WAKEUP_MID:
            	ArmingSM.DoAction();
            	MainSM.DoAction();
            	NavigationSM.DoAction();
                SendVehicleManagerStateMsg();
                SendVehicleStatusMsg();
                break;

            case VM_SEND_HK_MID:
                ProcessCmdPipe();
                ReportHousekeeping();
                break;

            case PX4_SENSOR_MAG_MID:
                memcpy(&CVT.SensorMagMsg, MsgPtr, sizeof(CVT.SensorMagMsg));
                break;

            case PX4_SENSOR_GYRO_MID:
                memcpy(&CVT.SensorGyroMsg, MsgPtr, sizeof(CVT.SensorGyroMsg));
                break;

            case PX4_SYSTEM_POWER_MID:
                memcpy(&CVT.SystemPowerMsg, MsgPtr, sizeof(CVT.SystemPowerMsg));
                break;

//            case PX4_SENSOR_PREFLIGHT_MID:
//                memcpy(&CVT.SensorPreflightMsg, MsgPtr, sizeof(CVT.SensorPreflightMsg));
//                break;

            case PX4_TELEMETRY_STATUS_MID:
                memcpy(&CVT.TelemetryStatusMsg, MsgPtr, sizeof(CVT.TelemetryStatusMsg));
                break;

            case PX4_SUBSYSTEM_INFO_MID:
                memcpy(&CVT.SubsystemInfoMsg, MsgPtr, sizeof(CVT.SubsystemInfoMsg));
                break;

            case PX4_VEHICLE_GPS_POSITION_MID:
                memcpy(&CVT.VehicleGpsPositionMsg, MsgPtr, sizeof(CVT.VehicleGpsPositionMsg));
                break;

            case PX4_VEHICLE_ATTITUDE_MID:
                memcpy(&CVT.VehicleAttitudeMsg, MsgPtr, sizeof(CVT.VehicleAttitudeMsg));
                break;

            case PX4_VEHICLE_LOCAL_POSITION_MID:
                memcpy(&CVT.VehicleLocalPositionMsg, MsgPtr, sizeof(CVT.VehicleLocalPositionMsg));
                break;

            case PX4_VEHICLE_LAND_DETECTED_MID:
                memcpy(&CVT.VehicleLandDetectedMsg, MsgPtr, sizeof(CVT.VehicleLandDetectedMsg));
                break;

            case PX4_GEOFENCE_RESULT_MID:
                memcpy(&CVT.GeofenceResultMsg, MsgPtr, sizeof(CVT.GeofenceResultMsg));
                break;

            case PX4_MISSION_RESULT_MID:
                memcpy(&CVT.MissionResultMsg, MsgPtr, sizeof(CVT.MissionResultMsg));
                break;

            case PX4_MANUAL_CONTROL_SETPOINT_MID:
                memcpy(&CVT.ManualControlSetpointMsg, MsgPtr, sizeof(CVT.ManualControlSetpointMsg));
                break;

            case PX4_POSITION_SETPOINT_TRIPLET_MID:
                memcpy(&CVT.PositionSetpointTripletMsg, MsgPtr, sizeof(CVT.PositionSetpointTripletMsg));
                break;

            case PX4_OFFBOARD_CONTROL_MODE_MID:
                memcpy(&CVT.OffboardControlModeMsg, MsgPtr, sizeof(CVT.OffboardControlModeMsg));
                break;

            case PX4_SENSOR_ACCEL_MID:
                memcpy(&CVT.SensorAccelMsg, MsgPtr, sizeof(CVT.SensorAccelMsg));
                break;

            case PX4_SAFETY_MID:
                memcpy(&CVT.SafetyMsg, MsgPtr, sizeof(CVT.SafetyMsg));
                break;

            case PX4_SENSOR_CORRECTION_MID:
                memcpy(&CVT.SensorCorrectionMsg, MsgPtr, sizeof(CVT.SensorCorrectionMsg));
                break;

            case PX4_VEHICLE_STATUS_MID:
                memcpy(&CVT.VehicleStatusMsg, MsgPtr, sizeof(CVT.VehicleStatusMsg));
                break;

            case PX4_SENSOR_COMBINED_MID:
                memcpy(&CVT.SensorCombinedMsg, MsgPtr, sizeof(CVT.SensorCombinedMsg));
                break;

            default:
                (void) CFE_EVS_SendEvent(VM_MSGID_ERR_EID, CFE_EVS_ERROR,
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
        (void) CFE_EVS_SendEvent(VM_RCVMSG_ERR_EID, CFE_EVS_ERROR,
			  "SCH pipe read error (0x%08lX).", iStatus);
    }

    return iStatus;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void VM::ProcessCmdPipe()
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
                case VM_CMD_MID:
                    ProcessAppCmds(CmdMsgPtr);
                    break;

                default:
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with this
                     *  pipe, but not handled in this switch-case.) */
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(VM_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            (void) CFE_EVS_SendEvent(VM_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08lX)", iStatus);
            break;
        }
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process VM Commands                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void VM::ProcessAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case VM_NOOP_CC:
                HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(VM_CMD_NOOP_EID, CFE_EVS_INFORMATION,
					"Recvd NOOP. Version %d.%d.%d.%d",
					VM_MAJOR_VERSION,
					VM_MINOR_VERSION,
					VM_REVISION,
					VM_MISSION_REV);
                break;

            case VM_RESET_CC:
                HkTlm.usCmdCnt = 0;
                HkTlm.usCmdErrCnt = 0;
                break;

            case VM_VEHICLE_ARM_CC:
            	try{
                    ArmingSM.FSM.Arm();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_ARMING_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Arming transition.  Command rejected.");
            	}
                break;

            case VM_VEHICLE_DISARM_CC:
            	try{
                    ArmingSM.FSM.Disarm();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_ARMING_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Arming transition.  Command rejected.");
            	}
                break;

            case VM_SET_MAIN_MANUAL_CC:
            	try{
                    MainSM.FSM.trManual();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_MAIN_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Main transition.  Command rejected.");
            	}
                break;

            case VM_SET_MAIN_ALTCTL_CC:
            	try{
                    MainSM.FSM.trAltitudeControl();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_MAIN_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Main transition.  Command rejected.");
            	}
                break;

            case VM_SET_MAIN_POSCTL_CC:
            	try{
                    MainSM.FSM.trPositionControl();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_MAIN_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Main transition.  Command rejected.");
            	}
                break;

            case VM_SET_MAIN_AUTO_MISSION_CC:
            	try{
                    MainSM.FSM.trAutoMission();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_MAIN_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Main transition.  Command rejected.");
            	}
                break;

            case VM_SET_MAIN_AUTO_LOITER_CC:
            	try{
                    MainSM.FSM.trAutoLoiter();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_MAIN_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Main transition.  Command rejected.");
            	}
                break;

            case VM_SET_MAIN_AUTO_RTL_CC:
            	try{
                    MainSM.FSM.trAutoReturnToLaunch();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_MAIN_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Main transition.  Command rejected.");
            	}
                break;

            case VM_SET_MAIN_ACRO_CC:
            	try{
                    MainSM.FSM.trAcrobatic();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_MAIN_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Main transition.  Command rejected.");
            	}
                break;

            case VM_SET_MAIN_OFFBOARD_CC:
            	try{
                    MainSM.FSM.trOffboard();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_MAIN_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Main transition.  Command rejected.");
            	}
                break;

            case VM_SET_MAIN_STAB_CC:
            	try{
                    MainSM.FSM.trStabilize();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_MAIN_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Main transition.  Command rejected.");
            	}
                break;

            case VM_SET_MAIN_RATTITUDE_CC:
            	try{
                    MainSM.FSM.trRattitude();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_MAIN_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Main transition.  Command rejected.");
            	}
                break;

            case VM_SET_MAIN_AUTO_TAKEOFF_CC:
            	try{
                    MainSM.FSM.trAutoTakeoff();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_MAIN_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Main transition.  Command rejected.");
            	}
                break;

            case VM_SET_MAIN_AUTO_LAND_CC:
            	try{
                    MainSM.FSM.trAutoLand();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_MAIN_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Main transition.  Command rejected.");
            	}
                break;

            case VM_SET_MAIN_AUTO_FOLLOW_TARGET_CC:
            	try{
                    MainSM.FSM.trAutoFollowTarget();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_MAIN_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Main transition.  Command rejected.");
            	}
                break;

            case VM_SET_MAIN_AUTO_PRECLAND_CC:
            	try{
                    MainSM.FSM.trAutoPrecland();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_MAIN_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Main transition.  Command rejected.");
            	}
                break;

            case VM_SET_NAV_MANUAL_CC:
            	try{
                    NavigationSM.FSM.trManual();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Main transition.  Command rejected.");
            	}
                break;

            case VM_SET_NAV_ALTCTL_CC:
            	try{
                    NavigationSM.FSM.trAltitudeControl();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Main transition.  Command rejected.");
            	}
                break;

            case VM_SET_NAV_POSCTL_CC:
            	try{
                    NavigationSM.FSM.trPositionControl();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Main transition.  Command rejected.");
            	}
                break;

            case VM_SET_NAV_AUTO_MISSION_CC:
            	try{
                    NavigationSM.FSM.trAutoMission();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Main transition.  Command rejected.");
            	}
                break;

            case VM_SET_NAV_AUTO_LOITER_CC:
            	try{
                    NavigationSM.FSM.trAutoLoiter();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Main transition.  Command rejected.");
            	}
                break;

            case VM_SET_NAV_AUTO_RTL_CC:
            	try{
                    NavigationSM.FSM.trAutoReturnToLaunch();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Main transition.  Command rejected.");
            	}
                break;

            case VM_SET_NAV_AUTO_RCRECOVER_CC:
            	try{
                    NavigationSM.FSM.trAutoRCRecover();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Main transition.  Command rejected.");
            	}
                break;

            case VM_SET_NAV_AUTO_RTGS_CC:
            	try{
                    NavigationSM.FSM.trAutoRtgs();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Main transition.  Command rejected.");
            	}
                break;

            case VM_SET_NAV_AUTO_LAND_ENG_FAIL_CC:
            	try{
                    NavigationSM.FSM.trAutoLandEngineFail();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Main transition.  Command rejected.");
            	}
                break;

            case VM_SET_NAV_AUTO_LAND_GPS_FAIL_CC:
            	try{
                    NavigationSM.FSM.trAutoLandGpsFail();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Main transition.  Command rejected.");
            	}
                break;

            case VM_SET_NAV_ACRO_CC:
            	try{
                    NavigationSM.FSM.trAcrobatic();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Main transition.  Command rejected.");
            	}
                break;

            case VM_SET_NAV_DESCEND_CC:
            	try{
                    NavigationSM.FSM.trDescend();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Main transition.  Command rejected.");
            	}
                break;

            default:
                HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(VM_CC_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid command code (%u)", (unsigned int)uiCmdCode);
                break;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send VM Housekeeping                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void VM::ReportHousekeeping()
{
	HkTlm.ArmingState = ArmingSM.GetCurrentStateID();
	HkTlm.MainState = MainSM.GetCurrentStateID();
	HkTlm.NavState = NavigationSM.GetCurrentStateID();
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&HkTlm);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Publish Output Data                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM::SendActuatorArmedMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&ActuatorArmedMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&ActuatorArmedMsg);
}

void VM::SendHomePositionMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&HomePositionMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&HomePositionMsg);
}

void VM::SendVehicleManagerStateMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&VehicleManagerStateMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&VehicleManagerStateMsg);
}

void VM::SendMissionMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&MissionMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&MissionMsg);
}

void VM::SendLedControlMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&LedControlMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&LedControlMsg);
}

void VM::SendVehicleStatusMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&VehicleStatusMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&VehicleStatusMsg);
}

//void VM::SendVehicleRoiMsg()
//{
//    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&VehicleRoiMsg);
//    CFE_SB_SendMsg((CFE_SB_Msg_t*)&VehicleRoiMsg);
//}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Command Length                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean VM::VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
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

            (void) CFE_EVS_SendEvent(VM_MSGLEN_ERR_EID, CFE_EVS_ERROR,
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
/* VM Application C style main entry point.                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
extern "C" void VM_AppMain()
{
    oVM.AppMain();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* VM Application C++ style main entry point.                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM::AppMain()
{
    /* Register the application with Executive Services */
    uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("VM - Failed to register the app (0x%08lX)\n", iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(VM_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(VM_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(VM_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(VM_MAIN_TASK_PERF_ID);
    }
    else
    {
        uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* TODO:  Replace with appropriate code to cause a transition. */
    ArmingSM.FSM.InitComplete();

    /* Application main loop */
    while (CFE_ES_RunLoop(&uiRunStatus) == TRUE)
    {
        RcvSchPipeMsg(VM_SCH_PIPE_PEND_TIME);

        iStatus = AcquireConfigPointers();
        if(iStatus != CFE_SUCCESS)
        {
            /* We apparently tried to load a new table but failed.  Terminate the application. */
            uiRunStatus = CFE_ES_APP_ERROR;
        }
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(VM_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(uiRunStatus);
}


boolean VM::IsVehicleArmed()
{
	return ActuatorArmedMsg.Armed;
}


/************************/
/*  End of File Comment */
/************************/
