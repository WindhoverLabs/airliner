/************************************************************************
** Includes
*************************************************************************/
#include <string.h>
#include <float.h>
#include "cfe.h"
#include <math.h>


#include "vm_app.h"
#include "vm_msg.h"
#include "vm_version.h"
#include "px4lib.h"



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

		iStatus = CFE_SB_SubscribeEx(PX4_BATTERY_STATUS_MID, SchPipeId, CFE_SB_Default_Qos, 1);
		if (iStatus != CFE_SUCCESS)
		{
		   (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
				 "CMD Pipe failed to subscribe to PX4_TELEMETRY_STATUS_MID. (0x%08lX)",
				 iStatus);
		   goto VM_InitPipe_Exit_Tag;
		}

		iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_COMMAND_MID, SchPipeId, CFE_SB_Default_Qos, 1);
		if (iStatus != CFE_SUCCESS)
		{
		   (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
				 "CMD Pipe failed to subscribe to PX4_VEHICLE_COMMAND_MID. (0x%08lX)",
				 iStatus);
		   goto VM_InitPipe_Exit_Tag;
		}
		iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_CONTROL_MODE_MID, SchPipeId, CFE_SB_Default_Qos, 1);
		if (iStatus != CFE_SUCCESS)
		{
		   (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
				 "CMD Pipe failed to subscribe to PX4_VEHICLE_COMMAND_MID. (0x%08lX)",
				 iStatus);
		   goto VM_InitPipe_Exit_Tag;
		}

		iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_GLOBAL_POSITION_MID, SchPipeId, CFE_SB_Default_Qos, 1);
		if (iStatus != CFE_SUCCESS)
		{
		   (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
				 "CMD Pipe failed to subscribe to PX4_SUBSYSTEM_INFO_MID. (0x%08lX)",
				 iStatus);
		   goto VM_InitPipe_Exit_Tag;
		}
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
//        iStatus = CFE_SB_SubscribeEx(AE_HK_TLM_MID, SchPipeId, CFE_SB_Default_Qos, 1);
//        if (iStatus != CFE_SUCCESS)
//        {
//            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
//					 "CMD Pipe failed to subscribe to PX4_SENSOR_COMBINED_MID. (0x%08lX)",
//					 iStatus);
//            goto VM_InitPipe_Exit_Tag;
//        }
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

	CFE_SB_InitMsg(&VehicleControlModeMsg,
		PX4_VEHICLE_CONTROL_MODE_MID, sizeof(PX4_VehicleControlModeMsg_t), TRUE);

	/*CFE_SB_InitMsg(&VehicleGlobalPositionMsg,
		PX4_VEHICLE_CONTROL_MODE_MID, sizeof(PX4_VehicleGlobalPositionMsg_t), TRUE);

	CFE_SB_InitMsg(&VehicleGpsPositionMsg,
		PX4_VEHICLE_CONTROL_MODE_MID, sizeof(PX4_VehicleGpsPositionMsg_t), TRUE);*/

	CFE_SB_InitMsg(&VehicleCommandMsg,
			PX4_VEHICLE_COMMAND_MID, sizeof(PX4_VehicleCommandMsg_t), TRUE);

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
            {
            	/* Wait till global position is initialized */
            	if (VehicleGlobalPositionMsg.Timestamp==0){
            			break;
            	}

            	/* Initialize home position and local variables */
            	if(NotInitialized){

            		Initialization();
            		SetHomePosition();
            		NotInitialized = false;
            	}

            	Execute();
            	/* Get a common timestamp. */
            	uint64 timestamp;
				timestamp = PX4LIB_GetPX4TimeUs();
				ActuatorArmedMsg.Timestamp = timestamp;
				VehicleStatusMsg.Timestamp = timestamp;
				VehicleManagerStateMsg.Timestamp = timestamp;
				VehicleControlModeMsg.Timestamp = timestamp;

				/* Execute all stateful behavior. */
            	ArmingSM.DoAction();
            	NavigationSM.DoAction();

            	/* Publish the messages. */
            	SendVehicleStatusMsg();
            	SendVehicleManagerStateMsg();
                SendVehicleControlModeMsg();
                break;
            }

            case VM_SEND_HK_MID:
                ProcessCmdPipe();
                ReportHousekeeping();
                break;

            case PX4_SENSOR_MAG_MID:
                memcpy(&SensorMagMsg, MsgPtr, sizeof(SensorMagMsg));
                break;

            case PX4_SENSOR_GYRO_MID:
                memcpy(&SensorGyroMsg, MsgPtr, sizeof(SensorGyroMsg));
                break;
                
            case PX4_BATTERY_STATUS_MID:
                memcpy(&BatteryStatusMsg, MsgPtr, sizeof(BatteryStatusMsg));
                break;

            case PX4_VEHICLE_GLOBAL_POSITION_MID:
                memcpy(&VehicleGlobalPositionMsg, MsgPtr, sizeof(VehicleGlobalPositionMsg));
                break;

            case PX4_TELEMETRY_STATUS_MID:
                memcpy(&TelemetryStatusMsg, MsgPtr, sizeof(TelemetryStatusMsg));
                break;

            case PX4_SUBSYSTEM_INFO_MID:
                memcpy(&SubsystemInfoMsg, MsgPtr, sizeof(SubsystemInfoMsg));
                break;

            case PX4_VEHICLE_GPS_POSITION_MID:
                memcpy(&VehicleGpsPositionMsg, MsgPtr, sizeof(VehicleGpsPositionMsg));
                break;

            case PX4_VEHICLE_ATTITUDE_MID:
                memcpy(&VehicleAttitudeMsg, MsgPtr, sizeof(VehicleAttitudeMsg));
                break;

            case PX4_VEHICLE_LOCAL_POSITION_MID:
                memcpy(&VehicleLocalPositionMsg, MsgPtr, sizeof(VehicleLocalPositionMsg));
                break;

            case PX4_VEHICLE_LAND_DETECTED_MID:
                memcpy(&VehicleLandDetectedMsg, MsgPtr, sizeof(VehicleLandDetectedMsg));
                break;

            case PX4_GEOFENCE_RESULT_MID:
                memcpy(&GeofenceResultMsg, MsgPtr, sizeof(GeofenceResultMsg));
                break;

            case PX4_MISSION_RESULT_MID:
                memcpy(&MissionResultMsg, MsgPtr, sizeof(MissionResultMsg));
                break;

            case PX4_MANUAL_CONTROL_SETPOINT_MID:
                memcpy(&ManualControlSetpointMsg, MsgPtr, sizeof(ManualControlSetpointMsg));
                break;

            case PX4_POSITION_SETPOINT_TRIPLET_MID:
                memcpy(&PositionSetpointTripletMsg, MsgPtr, sizeof(PositionSetpointTripletMsg));
                break;

            case PX4_OFFBOARD_CONTROL_MODE_MID:
                memcpy(&OffboardControlModeMsg, MsgPtr, sizeof(OffboardControlModeMsg));
                break;

            case PX4_SENSOR_ACCEL_MID:
                memcpy(&SensorAccelMsg, MsgPtr, sizeof(SensorAccelMsg));
                break;

            case PX4_SAFETY_MID:
                memcpy(&SafetyMsg, MsgPtr, sizeof(SafetyMsg));
                break;

            case PX4_SENSOR_CORRECTION_MID:
                memcpy(&SensorCorrectionMsg, MsgPtr, sizeof(SensorCorrectionMsg));
                break;

            case PX4_VEHICLE_STATUS_MID:
                memcpy(&VehicleStatusMsg, MsgPtr, sizeof(VehicleStatusMsg));
                break;
            case PX4_VEHICLE_CONTROL_MODE_MID:
                memcpy(&VehicleControlModeMsg, MsgPtr, sizeof(VehicleControlModeMsg));
                break;

            case PX4_SENSOR_COMBINED_MID:
                memcpy(&SensorCombinedMsg, MsgPtr, sizeof(SensorCombinedMsg));
                break;
//            case AE_HK_TLM_MID:
//                memcpy(&Qae, MsgPtr, sizeof(Qae));
//                break;

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

            case VM_SET_NAV_MANUAL_CC:
            	try{
                    NavigationSM.FSM.trManual();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Nav transition.  Command rejected.");
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
            	    		"Illegal Nav transition.  Command rejected.");
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
            	    		"Illegal Nav transition.  Command rejected.");
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
            	    		"Illegal Nav transition.  Command rejected.");
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
            	    		"Illegal Nav transition.  Command rejected.");
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
            	    		"Illegal Nav transition.  Command rejected.");
            	}
                break;

            case VM_SET_NAV_STABILIZE_CC:
            	try{
                    NavigationSM.FSM.trStabilize();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Nav transition.  Command rejected.");
            	}
                break;

            case VM_SET_NAV_RATTITUDE_CC:
            	try{
                    NavigationSM.FSM.trRattitude();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Nav transition.  Command rejected.");
            	}
                break;

            case VM_SET_NAV_AUTO_TAKEOFF_CC:
            	try{
                    NavigationSM.FSM.trAutoTakeoff();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Nav transition.  Command rejected.");
            	}
                break;

            case VM_SET_NAV_AUTO_LAND_CC:
            	try{
                    NavigationSM.FSM.trAutoLand();
                    HkTlm.usCmdCnt++;
            	}
            	catch(statemap::TransitionUndefinedException e)
            	{
                    HkTlm.usCmdErrCnt++;
            	    CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
            	    		"Illegal Nav transition.  Command rejected.");
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

void VM::SendVehicleControlModeMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&VehicleControlModeMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&VehicleControlModeMsg);

}

void VM::SendVehicleGlobalPositionMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&VehicleGlobalPositionMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&VehicleGlobalPositionMsg);
}

void VM::SendVehicleGpsPositionMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&VehicleGpsPositionMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&VehicleGpsPositionMsg);

}


void VM::SendVehicleCommandMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&VehicleCommandMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&VehicleCommandMsg);
    OS_printf("message send\n");
}
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
    NavigationSM.FSM.trInitComplete();

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

uint64 VM::TimeElapsed(uint64 *then)
{
	uint64 now = PX4LIB_GetPX4TimeUs();
	uint64 delta = now - *then;
	return delta;
}

uint64 VM::TimeNow()
{
	uint64 now = PX4LIB_GetPX4TimeUs();
	return now;
}

void VM::SetHomePosition(){

	if (!(VehicleGlobalPositionMsg.EpH > vm_params.home_h_t || VehicleGlobalPositionMsg.EpV > vm_params.home_v_t)) {

		/* Set the HomePosition message */
		HomePositionMsg.Timestamp = TimeNow();
		HomePositionMsg.Lat = VehicleGlobalPositionMsg.Lat;
		HomePositionMsg.Lon = VehicleGlobalPositionMsg.Lon;
		HomePositionMsg.Alt = VehicleGlobalPositionMsg.Alt;
		HomePositionMsg.X = VehicleLocalPositionMsg.X;
		HomePositionMsg.Y = VehicleLocalPositionMsg.Y;
		HomePositionMsg.Z = VehicleLocalPositionMsg.Z;

		math::Quaternion q(VehicleAttitudeMsg.Q[0],
						   VehicleAttitudeMsg.Q[1],
						   VehicleAttitudeMsg.Q[2],
						   VehicleAttitudeMsg.Q[3]);

		math::Matrix3F3 rotationMat = math::Dcm(q);
		math::Vector3F euler = math::Euler(rotationMat);
		HomePositionMsg.Yaw = euler[2];
		OS_printf("            lat\tlon\talt\tyaw\n");
		OS_printf("VM_homeset  %f\t%f\t%f\t%f\n",HomePositionMsg.Lat,
												  HomePositionMsg.Lon,
												  HomePositionMsg.Alt,
												  HomePositionMsg.Yaw);
		SendHomePositionMsg();
	}



}

void VM::Initialization(){

	/* TODO: check LED and Buzzer device initialization and report */

	/* Initialize status flags */
	status_flags.condition_system_sensors_initialized = true;

	/* Always accept RC input as default */
	status_flags.rc_input_blocked = false;
	/* neither manual nor offboard control commands have been received */
	status_flags.offboard_control_signal_found_once = false;
	status_flags.rc_signal_found_once = false;

	/* assume we don't have a valid baro on startup */
	status_flags.barometer_failure = true;
	status_flags.ever_had_barometer_data = false;

	/* mark all signals lost as long as they haven't been found */
	status_flags.offboard_control_signal_lost = true;
	status_flags.offboard_control_loss_timeout = false;

	status_flags.condition_system_prearm_error_reported = false;
	status_flags.condition_system_hotplug_timeout = false;
	status_flags.condition_power_input_valid = true;
	AvionicsPowerRailVoltage = -1.0f;
	status_flags.usb_connected = false;

	// CIRCUIT BREAKERS
	status_flags.circuit_breaker_engaged_power_check = false;
	status_flags.circuit_breaker_engaged_airspd_check = false;
	status_flags.circuit_breaker_engaged_enginefailure_check = false;
	status_flags.circuit_breaker_engaged_gpsfailure_check = false;
	SetCircuitBreakers();

	/* Set position and velocity validty to false */
	status_flags.condition_global_position_valid = false;
	status_flags.condition_global_velocity_valid = false;
	status_flags.condition_local_position_valid = false;
	status_flags.condition_local_velocity_valid = false;
	status_flags.condition_local_altitude_valid = false;

	// Initialize gps failure to false if circuit breaker enabled
	if (status_flags.circuit_breaker_engaged_gpsfailure_check) {
		status_flags.gps_failure = false;
	} else {
		status_flags.gps_failure = true;
	}


	/* Vehicle status defaults */
	VehicleStatusMsg.OnboardControlSensorsPresent = 0;
	VehicleStatusMsg.OnboardControlSensorsEnabled = 0;
	VehicleStatusMsg.OnboardControlSensorsHealth = 0;

	if(vm_params.rc_in_off == 2){
		VehicleStatusMsg.RcInputMode = PX4_RcInMode_t::PX4_RC_IN_MODE_GENERATED;
	}

	VehicleStatusMsg.HilState = PX4_HIL_STATE_OFF;
	VehicleStatusMsg.Failsafe = false;
	VehicleStatusMsg.SystemType = PX4_SYSTEM_TYPE_HEXAROTOR;
	VehicleStatusMsg.IsRotaryWing = true;
	VehicleStatusMsg.IsVtol = false;
	VehicleStatusMsg.VtolFwPermanentStab = false;
	VehicleStatusMsg.InTransitionMode = false;
	VehicleStatusMsg.RcSignalLost = true;
	VehicleStatusMsg.RcInputMode = PX4_RC_IN_MODE_GENERATED;
	VehicleStatusMsg.DataLinkLost = true;
	VehicleStatusMsg.DataLinkLostCounter = 0;
	VehicleStatusMsg.EngineFailure = false;
	VehicleStatusMsg.EngineFailureCmd = false;
	VehicleStatusMsg.MissionFailure = false;

	/* Vehicle control mode defaults */
	VehicleControlModeMsg.SystemHilEnabled = false;
	VehicleControlModeMsg.ExternalManualOverrideOk = false;
	VehicleControlModeMsg.ControlOffboardEnabled = false;
	VehicleControlModeMsg.ControlManualEnabled = false;
	VehicleControlModeMsg.ControlAutoEnabled = true;
	VehicleControlModeMsg.ControlRatesEnabled = true;
	VehicleControlModeMsg.ControlAttitudeEnabled = true;
	VehicleControlModeMsg.ControlRattitudeEnabled = false;
	VehicleControlModeMsg.ControlAltitudeEnabled = true;
	VehicleControlModeMsg.ControlClimbRateEnabled = true;
	VehicleControlModeMsg.ControlPositionEnabled = true;
	VehicleControlModeMsg.ControlVelocityEnabled = true;
	VehicleControlModeMsg.ControlAccelerationEnabled = false;
	VehicleControlModeMsg.ControlTerminationEnabled = false;


	ConditionLocalPositionValid = true;


	/* Onboard mission not supported, set default mission and publish */
	MissionMsg.Timestamp = TimeNow();
	MissionMsg.DatamanID = 0;
	MissionMsg.Count = 0;
	MissionMsg.CurrentSeq = 0;
	SendMissionMsg();

	/* Safety defaults */
	SafetyMsg.SafetySwitchAvailable = false;
	SafetyMsg.SafetyOff = false;

	/* Global position defaults */
	VehicleGlobalPositionMsg.EpH = 1000.0f;
	VehicleGlobalPositionMsg.EpV = 1000.0f;

	/* Land detector message defaults */
	VehicleLandDetectedMsg.Landed = true;

	/* GPS position message defaults */
	VehicleGpsPositionMsg.EpH = FLT_MAX;
	VehicleGpsPositionMsg.EpV = FLT_MAX;



	/* Brief pre-flight check */


	// user adjustable duration required to assert arm/disarm via throttle/rudder stick
	vm_params.rc_arm_hyst *= COMMANDER_MONITORING_LOOPSPERMSEC;

	VmBootTimestamp = TimeNow();

	/* update parameters */
	if(!ActuatorArmedMsg.Armed){
		vm_params.mav_type = VehicleStatusMsg.SystemType;
		VehicleStatusMsg.IsRotaryWing = true;
		VehicleStatusMsg.IsVtol = false;
		VehicleStatusMsg.SystemID = vm_params.system_id;
		VehicleStatusMsg.ComponentID =	vm_params.component_id;
		SetCircuitBreakers();
		status_changed = true;
	}
	// percentage (* 0.01) needs to be doubled because RC total interval is 2, not 1
	// minimum stick change
	vm_params.rc_stick_ovrde *= 0.02f;
	auto_disarm_history.setTimeSince(false, (uint64)vm_params.disarm_land * 1000000 );


}

void VM::Execute(){

	/* Set home position at launch */
	if(VehicleLandDetectedMsg.Landed && !VehicleLandDetectedMsg.Freefall && !VehicleLandDetectedMsg.GroundContact){
		/* Vehicle has landed */
		if(!vh_prev_landed){
			FlightSessionInit();
		}
		vh_prev_landed = true;
		vh_prev_in_flight = false;
	}
	else if(!VehicleLandDetectedMsg.Landed && !VehicleLandDetectedMsg.Freefall && !VehicleLandDetectedMsg.GroundContact && vh_prev_landed && !vh_prev_in_flight ){
		/* Vehicle is launched */
		SetHomePosition();
		vh_prev_landed = false;
		vh_prev_in_flight = true;
	}

	/* Vehicle status message update */
	VehicleStatusMsg.SystemID = vm_params.system_id;
	VehicleStatusMsg.ComponentID = vm_params.component_id;

	/* Safety Msg */
	boolean previous_safety_off = SafetyMsg.SafetyOff;
	if(SafetyMsg.SafetySwitchAvailable && !SafetyMsg.SafetyOff && ActuatorArmedMsg.Armed && VehicleStatusMsg.ArmingState == PX4_ArmingState_t::PX4_ARMING_STATE_ARMED){
		OS_printf("Disamed by safety message\n");
		ArmingSM.FSM.Disarm();
		arming_state_changed = true;
	}
	//Notify the user if the status of the safety switch changes
	if (SafetyMsg.SafetySwitchAvailable && previous_safety_off != SafetyMsg.SafetyOff) {

		if (SafetyMsg.SafetyOff) {
			OS_printf("POSITIVE TONE\n");

		} else {
			OS_printf("NEUTRAL TONE\n");
		}

		status_changed = true;
	}


	/* Battery status */
	/* only consider battery voltage if system has been running 6s (usb most likely detected) and battery voltage is valid */
	if(TimeNow() > VmBootTimestamp + 6000000 && BatteryStatusMsg.VoltageFiltered > 2.0f * FLT_EPSILON){

		/* if battery voltage is getting lower, warn using buzzer, etc. */
		if(BatteryStatusMsg.Warning == PX4_BATTERY_WARNING_LOW && !low_battery_voltage_actions_done){
			low_battery_voltage_actions_done = true;
			if (ActuatorArmedMsg.Armed){
				OS_printf("LOW BATTERY, RETURN TO LAND ADVISED\n");
			}else{
				OS_printf("LOW BATTERY, TAKEOFF DISCOURAGED\n");
			}
		}
		else if (!status_flags.usb_connected && BatteryStatusMsg.Warning == PX4_BATTERY_WARNING_CRITICAL && !critical_battery_voltage_actions_done){
			critical_battery_voltage_actions_done = true;

			if (!ActuatorArmedMsg.Armed){
				OS_printf("CRITICAL BATTERY, SHUT SYSTEM DOWN\n");
			}
			else{

				if(vm_params.low_bat_act == 1 || vm_params.low_bat_act == 3){
					NavigationSM.FSM.trAutoReturnToLaunch();
					OS_printf("CRITICAL BATTERY, RETURNING TO LAND\n");
				}
				else if(vm_params.low_bat_act ==2){
					NavigationSM.FSM.trAutoLand();
					OS_printf("CRITICAL BATTERY, LANDING AT CURRENT POSITION\n");
				}
				else{
					OS_printf("CRITICAL BATTERY, RETURN TO LAND ADVISED\n");
				}

			}
		}
		else if(!status_flags.usb_connected && BatteryStatusMsg.Warning == PX4_BATTERY_WARNING_EMERGENCY && !emergency_battery_voltage_actions_done){
			emergency_battery_voltage_actions_done = true;

			if (!ActuatorArmedMsg.Armed){
				OS_printf("DANGEROUSLY LOW BATTERY, SHUT SYSTEM DOWN\n");
			}
			else{

				if(vm_params.low_bat_act == 2 || vm_params.low_bat_act == 3){
					NavigationSM.FSM.trAutoLand();
					OS_printf("DANGEROUS BATTERY LEVEL, LANDING IMMEDIATELY\n");
				}
				else{
					OS_printf("DANGEROUS BATTERY LEVEL, LANDING ADVISED!\n");
				}

			}

		}
	}



	/* Subsystem message */
	if(SubsystemInfoMsg.Present){
		VehicleStatusMsg.OnboardControlSensorsPresent |= SubsystemInfoMsg.SubsystemType;
	} else {
		VehicleStatusMsg.OnboardControlSensorsPresent &= ~SubsystemInfoMsg.SubsystemType;
	}
	if(SubsystemInfoMsg.Enabled){
		VehicleStatusMsg.OnboardControlSensorsEnabled |= SubsystemInfoMsg.SubsystemType;
	} else {
		VehicleStatusMsg.OnboardControlSensorsEnabled &= ~SubsystemInfoMsg.SubsystemType;
	}
	if(SubsystemInfoMsg.Ok){
		VehicleStatusMsg.OnboardControlSensorsHealth |= SubsystemInfoMsg.SubsystemType;
	} else {
		VehicleStatusMsg.OnboardControlSensorsHealth &= ~SubsystemInfoMsg.SubsystemType;
	}



	/* RC input check */
	if(!status_flags.rc_input_blocked && ManualControlSetpointMsg.Timestamp!=0 && (TimeNow() < ManualControlSetpointMsg.Timestamp + uint64(vm_params.rc_loss_t * 1e6f))){
		if(!status_flags.rc_signal_found_once){
			status_flags.rc_signal_found_once = true;
		}
		else{
			if(VehicleStatusMsg.RcSignalLost){
				OS_printf("MANUAL CONTROL REGAINED after %llums", (TimeNow()-rc_signal_lost_timestamp)/1000);
			}
		}
		VehicleStatusMsg.RcSignalLost = false;
		const bool in_armed_state = (VehicleStatusMsg.ArmingState == PX4_ARMING_STATE_ARMED || VehicleStatusMsg.ArmingState == PX4_ARMING_STATE_ARMED_ERROR);
		const bool arm_button_pressed = (vm_params.arm_switch_is_button == 1 && ManualControlSetpointMsg.ArmSwitch == PX4_SWITCH_POS_ON);

		/* DISARM */
		const bool stick_in_lower_left = ((ManualControlSetpointMsg.R < -STICK_ON_OFF_LIMIT) && (ManualControlSetpointMsg.Z <0.1f));
		const bool arm_switch_to_disarm_transition = ((vm_params.arm_switch_is_button == 0) && (last_sp_man_arm_switch ==PX4_SWITCH_POS_ON) && (ManualControlSetpointMsg.ArmSwitch == PX4_SWITCH_POS_OFF) );

		if(in_armed_state && VehicleStatusMsg.RcInputMode != PX4_RC_IN_MODE_OFF && (stick_in_lower_left || arm_button_pressed || arm_switch_to_disarm_transition)){

			//current_nav_state = NavigationSM.GetCurrentStateID();//TODO
			if(VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_MANUAL &&
			   VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_ACRO &&
			   VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_STAB &&
			   VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_RATTITUDE &&
			   !VehicleLandDetectedMsg.Landed){
				OS_printf("DISARM REJECTED\n");
			}
			else if ((stick_off_counter == vm_params.rc_arm_hyst && stick_on_counter < vm_params.rc_arm_hyst) || arm_switch_to_disarm_transition){
				ArmingSM.FSM.Disarm();
				OS_printf("Disarmed By RC \n");
				arming_state_changed = true;

			}
			stick_off_counter++;
		}
		/* do not reset the counter when holding the arm button longer than needed */
		else if (!(vm_params.arm_switch_is_button ==1 && ManualControlSetpointMsg.ArmSwitch == PX4_SWITCH_POS_ON)){
			stick_off_counter = 0;
		}

		/* ARM */
		const bool stick_in_lower_right = (ManualControlSetpointMsg.R > STICK_ON_OFF_LIMIT  && ManualControlSetpointMsg.Z <0.1f);
		const bool arm_switch_to_arm_transition = ((vm_params.arm_switch_is_button == 0) && (last_sp_man_arm_switch ==PX4_SWITCH_POS_OFF) && (ManualControlSetpointMsg.ArmSwitch == PX4_SWITCH_POS_ON) );

		if(!in_armed_state && VehicleStatusMsg.RcInputMode != PX4_RC_IN_MODE_OFF && (stick_in_lower_right || arm_button_pressed || arm_switch_to_arm_transition)){
			if((stick_on_counter == vm_params.rc_arm_hyst && stick_off_counter < vm_params.rc_arm_hyst) || arm_switch_to_arm_transition){
				if((VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_MANUAL) &&
				   (VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_ACRO) &&
				   (VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_STAB) &&
				   (VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_RATTITUDE) &&
				   (VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_POSCTL) &&
				   (VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_ALTCTL)){
					OS_printf("Not Arming: switch to manual mode\n");
				}
//				else if (!status_flags.condition_home_position_valid ){
//					OS_printf("home position is invalid \n");
//				}
				else if (VehicleStatusMsg.ArmingState == PX4_ARMING_STATE_STANDBY){
					ArmingSM.FSM.Arm();
					OS_printf("Armed By RC \n");
					arming_state_changed = true;
				}
			}
			stick_on_counter++;
		}
		/* do not reset the counter when holding the arm button longer than needed */
		else if (!(vm_params.arm_switch_is_button ==1 && ManualControlSetpointMsg.ArmSwitch == PX4_SWITCH_POS_ON)){
			stick_on_counter = 0;
		}
		last_sp_man_arm_switch = ManualControlSetpointMsg.ArmSwitch;

		/* Evaluate the state machine according to mode switched */
		/* TODO: State change trasitions */


		/* KILLSWITCH */
		if(ManualControlSetpointMsg.KillSwitch == PX4_SWITCH_POS_ON){
			if(!ActuatorArmedMsg.ManualLockdown){
				OS_printf("MANUAL KILLSWITCH ENGAGED\n");
			}
			ActuatorArmedMsg.ManualLockdown = true;

		}
		else if(ManualControlSetpointMsg.KillSwitch == PX4_SWITCH_POS_OFF){
			if(ActuatorArmedMsg.ManualLockdown){
				OS_printf("MANUAL KILLSWITCH ENGAGED\n");
			}
			ActuatorArmedMsg.ManualLockdown = false;
		}

		/* MANUAL SWITCH */
		if(ManualControlSetpointMsg.ManSwitch == PX4_SWITCH_POS_ON){
			if(!trasition_locked){
				NavigationSM.FSM.trManual();
				trasition_locked = true;
				OS_printf("Transitions Locked [Current: Manual]\n");
			}
		}
		else if(ManualControlSetpointMsg.ManSwitch == PX4_SWITCH_POS_OFF){
			if(trasition_locked){
				NavigationSM.FSM.trManual();
				trasition_locked = false;
				OS_printf("Transitions Locked [Current: NONE]\n");
			}

		}

		/* ALTCTL SWITCH */
		if(ManualControlSetpointMsg.ManSwitch == PX4_SWITCH_POS_ON){
			if(!trasition_locked){
				NavigationSM.FSM.trManual();
				trasition_locked = true;
				OS_printf("Transitions Locked [Current: Altitude Hold]\n");
			}
		}
		else if(ManualControlSetpointMsg.ManSwitch == PX4_SWITCH_POS_OFF){
			if(trasition_locked){
				NavigationSM.FSM.trManual();
				trasition_locked = false;
				OS_printf("Transitions Locked [Current: NONE]\n");
			}

		}

		/* POSCTL SWITCH */
		if(ManualControlSetpointMsg.ManSwitch == PX4_SWITCH_POS_ON){
			if(!trasition_locked){
				NavigationSM.FSM.trManual();
				trasition_locked = true;
				OS_printf("Transitions Locked [Current: Position Hold]\n");
			}
		}
		else if(ManualControlSetpointMsg.ManSwitch == PX4_SWITCH_POS_OFF){
			if(trasition_locked){
				NavigationSM.FSM.trManual();
				trasition_locked = false;
				OS_printf("Transitions Locked [Current: NONE]\n");
			}

		}

		/* RTL SWITCH */
		if(ManualControlSetpointMsg.ManSwitch == PX4_SWITCH_POS_ON){
			if(!trasition_locked){
				NavigationSM.FSM.trManual();
				trasition_locked = true;
				OS_printf("Transitions Locked [Current: RTL]\n");
			}
		}
		else if(ManualControlSetpointMsg.ManSwitch == PX4_SWITCH_POS_OFF){
			if(trasition_locked){
				NavigationSM.FSM.trManual();
				trasition_locked = false;
				OS_printf("Transitions Locked [Current: NONE]\n");
			}

		}





	}
	else if(!status_flags.rc_input_blocked && !VehicleStatusMsg.RcSignalLost ){
		OS_printf("MANUAL CONTROL LOST (at t=%llums)", TimeNow() / 1000);
		VehicleStatusMsg.RcSignalLost  = true;
		rc_signal_lost_timestamp = ManualControlSetpointMsg.Timestamp;

	}




}

void VM::FlightSessionInit(){

	OS_printf("session initialized\n");
	/* Push states to init */
	ArmingSM.FSM.Reset();
	NavigationSM.FSM.Reset();

	/* Transition from init to default state */
	ArmingSM.FSM.InitComplete();
	NavigationSM.FSM.trInitComplete();

	/* Set home postion */
	//SetHomePosition();

}

void VM::SetCircuitBreakers(){
	status_flags.circuit_breaker_engaged_power_check = CircuitBreakerEnabled(vm_params.cbrk_supply_chk, CBRK_SUPPLY_CHK_KEY);
	status_flags.circuit_breaker_engaged_usb_check = CircuitBreakerEnabled(vm_params.cbrk_usb_chk, CBRK_USB_CHK_KEY);
	status_flags.circuit_breaker_engaged_airspd_check = CircuitBreakerEnabled(vm_params.cbrk_airspd_chk, CBRK_AIRSPD_CHK_KEY);
	status_flags.circuit_breaker_engaged_enginefailure_check = CircuitBreakerEnabled(vm_params.cbrk_enginefail_chk, CBRK_ENGINEFAIL_KEY);
	status_flags.circuit_breaker_engaged_gpsfailure_check = CircuitBreakerEnabled(vm_params.cbrk_gpsdail_chk, CBRK_GPSFAIL_KEY);
	status_flags.circuit_breaker_flight_termination_disabled = CircuitBreakerEnabled(vm_params.cbrk_flightterm_chk, CBRK_FLIGHTTERM_KEY);
	status_flags.circuit_breaker_engaged_posfailure_check = CircuitBreakerEnabled(vm_params.cbrk_velposerr_chk, CBRK_VELPOSERR_KEY);
}

boolean VM::CircuitBreakerEnabled(int param, int magic_number){
	return (param == magic_number);
}

void VM::CheckValidity(uint64 timestamp, uint64 timeout, bool valid_in, bool *valid_out, bool *changed){

	uint64 now  = TimeNow();
	bool valid = (now < timestamp + timeout && now >timeout && valid_in);
	if(*valid_out != valid){
		*valid_out = valid;
		*changed = true;
	}
}


/************************/
/*  End of File Comment */
/************************/
