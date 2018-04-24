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
        ArmingSM(*this), NavigationSM(*this) {

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Destructor constructor.                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
VM::~VM() {

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables.                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 VM::InitEvent() {
    int32 iStatus = CFE_SUCCESS;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(0, 0, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS) {
        (void) CFE_ES_WriteToSysLog(
                "VM - Failed to register with EVS (0x%08lX)\n", iStatus);
    }

    return iStatus;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 VM::InitPipe() {
    int32 iStatus = CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&SchPipeId,
    VM_SCH_PIPE_DEPTH,
    VM_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS) {
        iStatus = CFE_SB_SubscribeEx(VM_WAKEUP_MID, SchPipeId,
                CFE_SB_Default_Qos, VM_WAKEUP_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS) {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "Sch Pipe failed to subscribe to VM_WAKEUP_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(VM_SEND_HK_MID, SchPipeId,
                CFE_SB_Default_Qos, VM_SEND_HK_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS) {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to VM_SEND_HK_MID. (0x%08X)",
                    (unsigned int) iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_SENSOR_MAG_MID, SchPipeId,
                CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS) {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_SENSOR_MAG_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_SENSOR_GYRO_MID, SchPipeId,
                CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS) {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_SENSOR_GYRO_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_BATTERY_STATUS_MID, SchPipeId,
                CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS) {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_TELEMETRY_STATUS_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_COMMAND_MID, SchPipeId,
                CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS) {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_VEHICLE_COMMAND_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_CONTROL_MODE_MID, SchPipeId,
                CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS) {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_VEHICLE_COMMAND_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_GLOBAL_POSITION_MID, SchPipeId,
                CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS) {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_SUBSYSTEM_INFO_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_TELEMETRY_STATUS_MID, SchPipeId,
                CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS) {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_TELEMETRY_STATUS_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_SUBSYSTEM_INFO_MID, SchPipeId,
                CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS) {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_SUBSYSTEM_INFO_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_GPS_POSITION_MID, SchPipeId,
                CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS) {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_VEHICLE_GPS_POSITION_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_ATTITUDE_MID, SchPipeId,
                CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS) {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_VEHICLE_ATTITUDE_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_LOCAL_POSITION_MID, SchPipeId,
                CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS) {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_VEHICLE_LOCAL_POSITION_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_LAND_DETECTED_MID, SchPipeId,
                CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS) {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_VEHICLE_LAND_DETECTED_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_GEOFENCE_RESULT_MID, SchPipeId,
                CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS) {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_GEOFENCE_RESULT_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_MISSION_RESULT_MID, SchPipeId,
                CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS) {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_MISSION_RESULT_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_MANUAL_CONTROL_SETPOINT_MID, SchPipeId,
                CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS) {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_MANUAL_CONTROL_SETPOINT_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_POSITION_SETPOINT_TRIPLET_MID,
                SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS) {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_POSITION_SETPOINT_TRIPLET_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_OFFBOARD_CONTROL_MODE_MID, SchPipeId,
                CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS) {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_OFFBOARD_CONTROL_MODE_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_SENSOR_ACCEL_MID, SchPipeId,
                CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS) {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_SENSOR_ACCEL_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_SAFETY_MID, SchPipeId,
                CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS) {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_SAFETY_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_SENSOR_CORRECTION_MID, SchPipeId,
                CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS) {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_SENSOR_CORRECTION_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_STATUS_MID, SchPipeId,
                CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS) {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_VEHICLE_STATUS_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_SENSOR_COMBINED_MID, SchPipeId,
                CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS) {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to PX4_SENSOR_COMBINED_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
    } else {
        (void) CFE_EVS_SendEvent(VM_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
                "Failed to create SCH pipe (0x%08lX)", iStatus);
        goto VM_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&CmdPipeId,
    VM_CMD_PIPE_DEPTH,
    VM_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS) {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(VM_CMD_MID, CmdPipeId);

        if (iStatus != CFE_SUCCESS) {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to VM_CMD_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
    } else {
        (void) CFE_EVS_SendEvent(VM_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
                "Failed to create CMD pipe (0x%08lX)", iStatus);
        goto VM_InitPipe_Exit_Tag;
    }

    VM_InitPipe_Exit_Tag: return iStatus;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM::InitData() {
    /* Init housekeeping message. */
    CFE_SB_InitMsg(&HkTlm,
    VM_HK_TLM_MID, sizeof(HkTlm), TRUE);

    /* Init output messages */
    CFE_SB_InitMsg(&ActuatorArmedMsg, PX4_ACTUATOR_ARMED_MID,
            sizeof(PX4_ActuatorArmedMsg_t), TRUE);

    CFE_SB_InitMsg(&HomePositionMsg, PX4_HOME_POSITION_MID,
            sizeof(PX4_HomePositionMsg_t), TRUE);

    CFE_SB_InitMsg(&VehicleManagerStateMsg, PX4_COMMANDER_STATE_MID,
            sizeof(PX4_CommanderStateMsg_t), TRUE);

    CFE_SB_InitMsg(&MissionMsg, PX4_MISSION_MID, sizeof(PX4_MissionMsg_t),
    TRUE);

    CFE_SB_InitMsg(&LedControlMsg, PX4_LED_CONTROL_MID,
            sizeof(PX4_LedControlMsg_t), TRUE);

    CFE_SB_InitMsg(&VehicleStatusMsg, PX4_VEHICLE_STATUS_MID,
            sizeof(PX4_VehicleStatusMsg_t), TRUE);

    CFE_SB_InitMsg(&VehicleControlModeMsg, PX4_VEHICLE_CONTROL_MODE_MID,
            sizeof(PX4_VehicleControlModeMsg_t), TRUE);

    CFE_SB_InitMsg(&VehicleCommandMsg, PX4_VEHICLE_COMMAND_MID,
            sizeof(PX4_VehicleCommandMsg_t), TRUE);

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* VM initialization                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 VM::InitApp() {
    int32 iStatus = CFE_SUCCESS;
    int8 hasEvents = 0;

    iStatus = InitEvent();
    if (iStatus != CFE_SUCCESS) {
        (void) CFE_ES_WriteToSysLog("VM - Failed to init events (0x%08lX)\n",
                iStatus);
        goto VM_InitApp_Exit_Tag;
    } else {
        hasEvents = 1;
    }

    iStatus = InitPipe();
    if (iStatus != CFE_SUCCESS) {
        goto VM_InitApp_Exit_Tag;
    }

    InitData();

    iStatus = InitConfigTbl();
    if (iStatus != CFE_SUCCESS) {
        goto VM_InitApp_Exit_Tag;
    }

    /* Updating application params from platform-nav-config-table */
    UpdateParamsFromTable();

    /* Initialize the caution and warning helper */
    m_caws.InitCAWS();

    VM_InitApp_Exit_Tag: if (iStatus == CFE_SUCCESS) {
        (void) CFE_EVS_SendEvent(VM_INIT_INF_EID, CFE_EVS_INFORMATION,
                "Initialized.  Version %d.%d.%d.%d",
                VM_MAJOR_VERSION,
                VM_MINOR_VERSION,
                VM_REVISION,
                VM_MISSION_REV);
    } else {
        if (hasEvents == 1) {
            (void) CFE_ES_WriteToSysLog(
                    "VM - Application failed to initialize\n");
        }
    }

    return iStatus;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 VM::RcvSchPipeMsg(int32 iBlocking) {
    int32 iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t* MsgPtr = NULL;
    CFE_SB_MsgId_t MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(VM_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(VM_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS) {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId) {
        case VM_WAKEUP_MID: {

            /* Update status in caution and warning */
            m_caws.SetStatus(&VehicleStatusMsg);

            /* Wait till global position is initialized */
            if (VehicleGlobalPositionMsg.Timestamp == 0) {
                break;
            }

            /* Initialize home position and local variables */
            if (NotInitialized) {
                Initialization();
                SetHomePosition();
                NotInitialized = false;
            }

            /* Cyclic maintainance loop */
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
            memcpy(&VehicleGlobalPositionMsg, MsgPtr,
                    sizeof(VehicleGlobalPositionMsg));
            break;

        case PX4_TELEMETRY_STATUS_MID:
            memcpy(&TelemetryStatusMsg, MsgPtr, sizeof(TelemetryStatusMsg));
            break;

        case PX4_SUBSYSTEM_INFO_MID:
            memcpy(&SubsystemInfoMsg, MsgPtr, sizeof(SubsystemInfoMsg));
            break;

        case PX4_VEHICLE_GPS_POSITION_MID:
            memcpy(&VehicleGpsPositionMsg, MsgPtr,
                    sizeof(VehicleGpsPositionMsg));
            break;

        case PX4_VEHICLE_ATTITUDE_MID:
            memcpy(&VehicleAttitudeMsg, MsgPtr, sizeof(VehicleAttitudeMsg));
            break;

        case PX4_VEHICLE_LOCAL_POSITION_MID:
            memcpy(&VehicleLocalPositionMsg, MsgPtr,
                    sizeof(VehicleLocalPositionMsg));
            break;

        case PX4_VEHICLE_LAND_DETECTED_MID:
            memcpy(&VehicleLandDetectedMsg, MsgPtr,
                    sizeof(VehicleLandDetectedMsg));
            break;

//            case PX4_GEOFENCE_RESULT_MID:
//                memcpy(&GeofenceResultMsg, MsgPtr, sizeof(GeofenceResultMsg));
//                break;

        case PX4_MISSION_RESULT_MID:
            memcpy(&MissionResultMsg, MsgPtr, sizeof(MissionResultMsg));
            break;

        case PX4_MANUAL_CONTROL_SETPOINT_MID:
            memcpy(&ManualControlSetpointMsg, MsgPtr,
                    sizeof(ManualControlSetpointMsg));
            break;

        case PX4_POSITION_SETPOINT_TRIPLET_MID:
            memcpy(&PositionSetpointTripletMsg, MsgPtr,
                    sizeof(PositionSetpointTripletMsg));
            break;

//            case PX4_OFFBOARD_CONTROL_MODE_MID:
//                memcpy(&OffboardControlModeMsg, MsgPtr, sizeof(OffboardControlModeMsg));
//                break;

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
            memcpy(&VehicleControlModeMsg, MsgPtr,
                    sizeof(VehicleControlModeMsg));
            break;

        case PX4_SENSOR_COMBINED_MID:
            memcpy(&SensorCombinedMsg, MsgPtr, sizeof(SensorCombinedMsg));
            break;

        default:
            (void) CFE_EVS_SendEvent(VM_MSGID_ERR_EID, CFE_EVS_ERROR,
                    "Recvd invalid SCH msgId (0x%04X)", MsgId);
        }
    } else if (iStatus == CFE_SB_NO_MESSAGE) {
        /* TODO: If there's no incoming message, you can do something here, or 
         * nothing.  Note, this section is dead code only if the iBlocking arg
         * is CFE_SB_PEND_FOREVER. */
        iStatus = CFE_SUCCESS;
    } else if (iStatus == CFE_SB_TIME_OUT) {
        /* TODO: If there's no incoming message within a specified time (via the
         * iBlocking arg, you can do something here, or nothing.  
         * Note, this section is dead code only if the iBlocking arg
         * is CFE_SB_PEND_FOREVER. */
        iStatus = CFE_SUCCESS;
    } else {
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

void VM::ProcessCmdPipe() {
    int32 iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t* CmdMsgPtr = NULL;
    CFE_SB_MsgId_t CmdMsgId;

    /* Process command messages until the pipe is empty */
    while (1) {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, CmdPipeId, CFE_SB_POLL);
        if (iStatus == CFE_SUCCESS) {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId) {
            case VM_CMD_MID:
                ProcessAppCmds(CmdMsgPtr);
                break;

            default:
                /* Bump the command error counter for an unknown command.
                 * (This should only occur if it was subscribed to with this
                 *  pipe, but not handled in this switch-case.) */
                HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(VM_MSGID_ERR_EID, CFE_EVS_ERROR,
                        "Recvd invalid CMD msgId (0x%04X)",
                        (unsigned short) CmdMsgId);
                break;
            }
        } else if (iStatus == CFE_SB_NO_MESSAGE) {
            break;
        } else {
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

void VM::ProcessAppCmds(CFE_SB_Msg_t* MsgPtr) {
    uint32 uiCmdCode = 0;

    if (MsgPtr != NULL) {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);

        switch (uiCmdCode) {
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
            try {
                ArmingSM.FSM.Arm();
                HkTlm.usCmdCnt++;
            } catch (statemap::TransitionUndefinedException e) {
                HkTlm.usCmdErrCnt++;
                CFE_EVS_SendEvent(VM_ARMING_ILLEGAL_TRANSITION_ERR_EID,
                CFE_EVS_INFORMATION,
                        "Illegal Arming transition. [%s] Command rejected.",
                        "ARM");
            }
            break;

        case VM_VEHICLE_DISARM_CC:
            try {
                ArmingSM.FSM.Disarm();
                HkTlm.usCmdCnt++;
            } catch (statemap::TransitionUndefinedException e) {
                HkTlm.usCmdErrCnt++;
                CFE_EVS_SendEvent(VM_ARMING_ILLEGAL_TRANSITION_ERR_EID,
                CFE_EVS_INFORMATION,
                        "Illegal Arming transition. [%s] Command rejected.",
                        "DISARM");
            }
            break;

        case VM_SET_NAV_MANUAL_CC:
            try {
                NavigationSM.FSM.trManual();
                HkTlm.usCmdCnt++;
            } catch (statemap::TransitionUndefinedException e) {
                HkTlm.usCmdErrCnt++;
                uint32 PrevState = NavigationSM.GetCurrentStateID();
                CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                CFE_EVS_INFORMATION,
                        "Illegal Nav transition [%s -> %s].  Command rejected.",
                        GetNavStateAsString(PrevState), "MANUAL");
            }
            break;

        case VM_SET_NAV_ALTCTL_CC:
            try {
                NavigationSM.FSM.trAltitudeControl();
                HkTlm.usCmdCnt++;
            } catch (statemap::TransitionUndefinedException e) {
                HkTlm.usCmdErrCnt++;
                uint32 PrevState = NavigationSM.GetCurrentStateID();
                CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                CFE_EVS_INFORMATION,
                        "Illegal Nav transition [%s -> %s].  Command rejected.",
                        GetNavStateAsString(PrevState), "ALTCTL");
            }
            break;

        case VM_SET_NAV_POSCTL_CC:
            try {
                NavigationSM.FSM.trPositionControl();
                HkTlm.usCmdCnt++;
            } catch (statemap::TransitionUndefinedException e) {
                HkTlm.usCmdErrCnt++;
                uint32 PrevState = NavigationSM.GetCurrentStateID();
                CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                CFE_EVS_INFORMATION,
                        "Illegal Nav transition [%s -> %s].  Command rejected.",
                        GetNavStateAsString(PrevState), "POSCTL");
            }
            break;

        case VM_SET_NAV_AUTO_LOITER_CC:
            try {
                NavigationSM.FSM.trAutoLoiter();
                HkTlm.usCmdCnt++;
            } catch (statemap::TransitionUndefinedException e) {
                HkTlm.usCmdErrCnt++;
                uint32 PrevState = NavigationSM.GetCurrentStateID();
                CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                CFE_EVS_INFORMATION,
                        "Illegal Nav transition [%s -> %s].  Command rejected.",
                        GetNavStateAsString(PrevState), "AUTORTL");
            }
            break;

        case VM_SET_NAV_AUTO_RTL_CC:
            try {
                NavigationSM.FSM.trAutoReturnToLaunch();
                HkTlm.usCmdCnt++;
            } catch (statemap::TransitionUndefinedException e) {
                HkTlm.usCmdErrCnt++;
                uint32 PrevState = NavigationSM.GetCurrentStateID();
                CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                CFE_EVS_INFORMATION,
                        "Illegal Nav transition [%s -> %s].  Command rejected.",
                        GetNavStateAsString(PrevState), "AUTOLTR");
            }
            break;

        case VM_SET_NAV_ACRO_CC:
            try {
                NavigationSM.FSM.trAcrobatic();
                HkTlm.usCmdCnt++;
            } catch (statemap::TransitionUndefinedException e) {
                HkTlm.usCmdErrCnt++;
                uint32 PrevState = NavigationSM.GetCurrentStateID();
                CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                CFE_EVS_INFORMATION,
                        "Illegal Nav transition [%s -> %s].  Command rejected.",
                        GetNavStateAsString(PrevState), "ACRO");
            }
            break;

        case VM_SET_NAV_STABILIZE_CC:
            try {
                NavigationSM.FSM.trStabilize();
                HkTlm.usCmdCnt++;
            } catch (statemap::TransitionUndefinedException e) {
                HkTlm.usCmdErrCnt++;
                uint32 PrevState = NavigationSM.GetCurrentStateID();
                CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                CFE_EVS_INFORMATION,
                        "Illegal Nav transition [%s -> %s].  Command rejected.",
                        GetNavStateAsString(PrevState), "STAB");
            }
            break;

        case VM_SET_NAV_RATTITUDE_CC:
            try {
                NavigationSM.FSM.trRattitude();
                HkTlm.usCmdCnt++;
            } catch (statemap::TransitionUndefinedException e) {
                HkTlm.usCmdErrCnt++;
                uint32 PrevState = NavigationSM.GetCurrentStateID();
                CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                CFE_EVS_INFORMATION,
                        "Illegal Nav transition [%s -> %s].  Command rejected.",
                        GetNavStateAsString(PrevState), "RATT");
            }
            break;

        case VM_SET_NAV_AUTO_TAKEOFF_CC:
            try {
                NavigationSM.FSM.trAutoTakeoff();
                HkTlm.usCmdCnt++;
            } catch (statemap::TransitionUndefinedException e) {
                HkTlm.usCmdErrCnt++;
                uint32 PrevState = NavigationSM.GetCurrentStateID();
                CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                CFE_EVS_INFORMATION,
                        "Illegal Nav transition [%s -> %s].  Command rejected.",
                        GetNavStateAsString(PrevState), "AUTOTAKOF");
            }
            break;

        case VM_SET_NAV_AUTO_LAND_CC:
            try {
                NavigationSM.FSM.trAutoLand();
                HkTlm.usCmdCnt++;
            } catch (statemap::TransitionUndefinedException e) {
                HkTlm.usCmdErrCnt++;
                uint32 PrevState = NavigationSM.GetCurrentStateID();
                CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                CFE_EVS_INFORMATION,
                        "Illegal Nav transition [%s -> %s].  Command rejected.",
                        GetNavStateAsString(PrevState), "AUTOLND");
            }
            break;

        default:
            HkTlm.usCmdErrCnt++;
            (void) CFE_EVS_SendEvent(VM_CC_ERR_EID, CFE_EVS_ERROR,
                    "Recvd invalid command code (%u)",
                    (unsigned int) uiCmdCode);
            break;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send VM Housekeeping                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void VM::ReportHousekeeping() {
    HkTlm.ArmingState = ArmingSM.GetCurrentStateID();
    HkTlm.NavState = NavigationSM.GetCurrentStateID();
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*) &HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*) &HkTlm);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Publish Output Data                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM::SendActuatorArmedMsg() {
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*) &ActuatorArmedMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*) &ActuatorArmedMsg);
}

void VM::SendHomePositionMsg() {
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*) &HomePositionMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*) &HomePositionMsg);
}

void VM::SendVehicleManagerStateMsg() {
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*) &VehicleManagerStateMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*) &VehicleManagerStateMsg);
}

void VM::SendMissionMsg() {
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*) &MissionMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*) &MissionMsg);
}

void VM::SendLedControlMsg() {
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*) &LedControlMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*) &LedControlMsg);
}

void VM::SendVehicleStatusMsg() {
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*) &VehicleStatusMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*) &VehicleStatusMsg);
}

void VM::SendVehicleControlModeMsg() {
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*) &VehicleControlModeMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*) &VehicleControlModeMsg);

}

void VM::SendVehicleCommandMsg() {
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*) &VehicleCommandMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*) &VehicleCommandMsg);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Command Length                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean VM::VerifyCmdLength(CFE_SB_Msg_t* MsgPtr, uint16 usExpectedLen) {
    boolean bResult = TRUE;
    uint16 usMsgLen = 0;

    if (MsgPtr != NULL) {
        usMsgLen = CFE_SB_GetTotalMsgLength(MsgPtr);

        if (usExpectedLen != usMsgLen) {
            bResult = FALSE;
            CFE_SB_MsgId_t MsgId = CFE_SB_GetMsgId(MsgPtr);
            uint16 usCmdCode = CFE_SB_GetCmdCode(MsgPtr);

            (void) CFE_EVS_SendEvent(VM_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                    "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                            "msgLen=%d, expectedLen=%d", MsgId, usCmdCode,
                    usMsgLen, usExpectedLen);
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
extern "C" void VM_AppMain() {
    oVM.AppMain();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* VM Application C++ style main entry point.                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM::AppMain() {
    /* Register the application with Executive Services */
    uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS) {
        (void) CFE_ES_WriteToSysLog(
                "VM - Failed to register the app (0x%08lX)\n", iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(VM_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS) {
        iStatus = InitApp();
    }

    if (iStatus == CFE_SUCCESS) {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(VM_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(VM_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(VM_MAIN_TASK_PERF_ID);
    } else {
        uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Initialize state machine */

    ArmingSM.FSM.InitComplete();
    NavigationSM.FSM.trInitComplete();

    /* Application main loop */
    while (CFE_ES_RunLoop(&uiRunStatus) == TRUE) {
        RcvSchPipeMsg(VM_SCH_PIPE_PEND_TIME);

        iStatus = AcquireConfigPointers();
        if (iStatus != CFE_SUCCESS) {
            /* We apparently tried to load a new table but failed.  Terminate the application. */
            uiRunStatus = CFE_ES_APP_ERROR;
        }
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(VM_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(uiRunStatus);
}

boolean VM::IsVehicleArmed() {
    return ActuatorArmedMsg.Armed;
}

uint64 VM::TimeElapsed(uint64 *TimePtr) {
    uint64 now = PX4LIB_GetPX4TimeUs();
    uint64 delta = now - *TimePtr;
    return delta;
}

uint64 VM::TimeNow() {
    uint64 now = PX4LIB_GetPX4TimeUs();
    return now;
}

void VM::SetHomePosition() {

    if (!(VehicleGlobalPositionMsg.EpH > vm_params.home_h_t
            || VehicleGlobalPositionMsg.EpV > vm_params.home_v_t)) {

        /* Set the HomePosition message */
        float DistBottom = 0;

        /* Distance from ground should always be positive */
        if (VehicleLocalPositionMsg.DistBottom > 0)
        {

            DistBottom = VehicleLocalPositionMsg.DistBottom;

        }

        HomePositionMsg.Timestamp = TimeNow();
        HomePositionMsg.Lat = VehicleGlobalPositionMsg.Lat;
        HomePositionMsg.Lon = VehicleGlobalPositionMsg.Lon;
        HomePositionMsg.Alt = VehicleGlobalPositionMsg.Alt - DistBottom - vm_params.home_pos_alt_padding ;
        HomePositionMsg.X = VehicleLocalPositionMsg.X;
        HomePositionMsg.Y = VehicleLocalPositionMsg.Y;
        HomePositionMsg.Z = VehicleLocalPositionMsg.Z;

        math::Quaternion q(VehicleAttitudeMsg.Q[0], VehicleAttitudeMsg.Q[1],
                VehicleAttitudeMsg.Q[2], VehicleAttitudeMsg.Q[3]);

        math::Matrix3F3 rotationMat = math::Dcm(q);
        math::Vector3F euler = math::Euler(rotationMat);
        HomePositionMsg.Yaw = euler[2];

        (void) CFE_EVS_SendEvent(VM_HOMESET_INFO_EID, CFE_EVS_INFORMATION,
                "Home Position set. [Lat -> %.6f | Lon -> %.6f | Alt ->%.6f] [vgp -> %.6f |  padd ->%f]",
                HomePositionMsg.Lat, HomePositionMsg.Lon, HomePositionMsg.Alt, VehicleGlobalPositionMsg.Alt,vm_params.home_pos_alt_padding);

        SendHomePositionMsg();
    }
    else{

        (void) CFE_EVS_SendEvent(VM_HOMESET_INFO_EID, CFE_EVS_INFORMATION,
                "Home Position Could'nt be set.");
    }
}

void VM::Initialization() {

    /* TODO: check LED and Buzzer device initialization and report */
    /* Initialize status flags */
    status_flags.condition_system_sensors_initialized = true;

    /* Always accept RC input as default */
    status_flags.rc_input_blocked = false;
    status_flags.rc_signal_found_once = false;

    status_flags.usb_connected = false;

    /* Vehicle status defaults */
    VehicleStatusMsg.OnboardControlSensorsPresent = 0;
    VehicleStatusMsg.OnboardControlSensorsEnabled = 0;
    VehicleStatusMsg.OnboardControlSensorsHealth = 0;

    if (vm_params.rc_in_off == 2) {
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

    /* Onboard mission not supported, set default mission and publish */
    MissionMsg.Timestamp = TimeNow();
    MissionMsg.DatamanID = 0;
    MissionMsg.Count = 0;
    MissionMsg.CurrentSeq = 0;
    SendMissionMsg();

    /* Safety defaults */
    SafetyMsg.SafetySwitchAvailable = false;
    SafetyMsg.SafetyOff = false;

    // user adjustable duration required to assert arm/disarm via throttle/rudder stick
    vm_params.rc_arm_hyst *= COMMANDER_MONITORING_LOOPSPERMSEC;

    VmBootTimestamp = TimeNow();

    /* update parameters */
    if (!ActuatorArmedMsg.Armed) {
        vm_params.mav_type = VehicleStatusMsg.SystemType;
        VehicleStatusMsg.IsRotaryWing = true;
        VehicleStatusMsg.IsVtol = false;
        VehicleStatusMsg.SystemID = vm_params.system_id;
        VehicleStatusMsg.ComponentID = vm_params.component_id;
    }

}

void VM::Execute() {

    /* Set home position at launch */
    if (VehicleLandDetectedMsg.Landed && !VehicleLandDetectedMsg.Freefall
            && !VehicleLandDetectedMsg.GroundContact) {
        /* Vehicle has landed */
        if (!vh_prev_landed) {
            /* Rest arm and nav states */
            FlightSessionInit();
            /* Forget previous modes */
            previous_modes = {0};
        }
        vh_prev_landed = true;
        vh_prev_in_flight = false;
    }
    else if(!VehicleLandDetectedMsg.Landed && !VehicleLandDetectedMsg.Freefall && !VehicleLandDetectedMsg.GroundContact && vh_prev_landed && !vh_prev_in_flight ) {
        /* Vehicle is launched */
        SetHomePosition();
        vh_prev_landed = false;
        vh_prev_in_flight = true;
    }

    /* Vehicle status message handle */
    VehicleStatusMsg.SystemID = vm_params.system_id;
    VehicleStatusMsg.ComponentID = vm_params.component_id;

    /* Safety message handle */
    boolean previous_safety_off = SafetyMsg.SafetyOff;
    if(SafetyMsg.SafetySwitchAvailable && !SafetyMsg.SafetyOff && ActuatorArmedMsg.Armed && VehicleStatusMsg.ArmingState == PX4_ArmingState_t::PX4_ARMING_STATE_ARMED) {
        try {
            (void) CFE_EVS_SendEvent(VM_SAFETY_DISARM_INFO_EID, CFE_EVS_INFORMATION,
                    "Vehicle [DISARM] by safety message");
            ArmingSM.FSM.Disarm();
            HkTlm.usCmdCnt++;
            arming_state_changed = true;

        }
        catch(statemap::TransitionUndefinedException e)
        {
            HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(VM_ARMING_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
                    "Illegal Arming transition. [%s] Command rejected.","DISARM");
        }

    }

    /* Notify the user if the status of the safety switch changes*/
    if (SafetyMsg.SafetySwitchAvailable && previous_safety_off != SafetyMsg.SafetyOff) {

        if (SafetyMsg.SafetyOff) {
            (void) CFE_EVS_SendEvent(VM_SAFETY_INFO_EID, CFE_EVS_INFORMATION,
                    "Safety OFF");

        } else {
            (void) CFE_EVS_SendEvent(VM_SAFETY_INFO_EID, CFE_EVS_INFORMATION,
                    "Safety ON");
        }
    }

    /* Battery status handle */
    /* Only consider battery voltage if system has been running 6s (usb most likely detected) and battery voltage is valid */
    if(TimeNow() > VmBootTimestamp + 6000000 && BatteryStatusMsg.VoltageFiltered > 2.0f * FLT_EPSILON) {

        /* If battery voltage is getting lower, warn using buzzer, etc. */
        if(BatteryStatusMsg.Warning == PX4_BATTERY_WARNING_LOW && !low_battery_voltage_actions_done) {

            low_battery_voltage_actions_done = true;

            if (ActuatorArmedMsg.Armed) {
                (void) CFE_EVS_SendEvent(VM_LOW_BAT_INFO_EID, CFE_EVS_INFORMATION,
                        "Low battery, return to land");
            } else {
                (void) CFE_EVS_SendEvent(VM_LOW_BAT_INFO_EID, CFE_EVS_INFORMATION,
                        "Low battery, takeoff discouraged");
            }
        }
        else if (!status_flags.usb_connected && BatteryStatusMsg.Warning == PX4_BATTERY_WARNING_CRITICAL && !critical_battery_voltage_actions_done) {

            critical_battery_voltage_actions_done = true;

            if (!ActuatorArmedMsg.Armed) {
                (void) CFE_EVS_SendEvent(VM_CRITICAL_BAT_INFO_EID, CFE_EVS_INFORMATION,
                        "Critical battery, shutdown system");
            }
            else {

                if(vm_params.low_bat_act == 1 || vm_params.low_bat_act == 3) {

                    try {
                        (void) CFE_EVS_SendEvent(VM_CRITICAL_BAT_INFO_EID, CFE_EVS_INFORMATION,
                                "Critical battery, return to launch");
                        NavigationSM.FSM.trAutoReturnToLaunch();
                        HkTlm.usCmdCnt++;
                    }
                    catch(statemap::TransitionUndefinedException e)
                    {
                        HkTlm.usCmdErrCnt++;
                        CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
                                "Illegal Nav transition.  Command rejected.");
                    }

                }
                else if(vm_params.low_bat_act ==2) {
                    try {
                        (void) CFE_EVS_SendEvent(VM_CRITICAL_BAT_INFO_EID, CFE_EVS_INFORMATION,
                                "Critical battery, landing at current position");
                        NavigationSM.FSM.trAutoLand();
                        HkTlm.usCmdCnt++;
                    }
                    catch(statemap::TransitionUndefinedException e)
                    {
                        HkTlm.usCmdErrCnt++;
                        uint32 PrevState = NavigationSM.GetCurrentStateID();
                        CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                        CFE_EVS_INFORMATION,
                                "Illegal Nav transition [%s -> %s].  Command rejected.",
                                GetNavStateAsString(PrevState), "AUTOLND");
                    }

                }
                else {
                    (void) CFE_EVS_SendEvent(VM_CRITICAL_BAT_INFO_EID, CFE_EVS_INFORMATION,
                            "Critical battery, return to launch advised");
                }

            }
        }
        else if(!status_flags.usb_connected && BatteryStatusMsg.Warning == PX4_BATTERY_WARNING_EMERGENCY && !emergency_battery_voltage_actions_done) {
            emergency_battery_voltage_actions_done = true;

            if (!ActuatorArmedMsg.Armed) {
                (void) CFE_EVS_SendEvent(VM_DANGER_BAT_LEVEL_INFO_EID, CFE_EVS_INFORMATION,
                        "Dangerously low battery, shutdown system");
            }
            else {

                if(vm_params.low_bat_act == 2 || vm_params.low_bat_act == 3) {

                    try {
                        (void) CFE_EVS_SendEvent(VM_DANGER_BAT_LEVEL_INFO_EID, CFE_EVS_INFORMATION,
                                "Dangerously low battery, landing immediately");
                        NavigationSM.FSM.trAutoLand();
                        HkTlm.usCmdCnt++;
                    }
                    catch(statemap::TransitionUndefinedException e)
                    {
                        HkTlm.usCmdErrCnt++;
                        uint32 PrevState = NavigationSM.GetCurrentStateID();
                        CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                        CFE_EVS_INFORMATION,
                                "Illegal Nav transition [%s -> %s].  Command rejected.",
                                GetNavStateAsString(PrevState), "AUTOLND");
                    }

                }
                else {
                    (void) CFE_EVS_SendEvent(VM_DANGER_BAT_LEVEL_INFO_EID, CFE_EVS_INFORMATION,
                            "Dangerously low battery, landing advised");
                }
            }
        }
    }

    /* Subsystem message handle */
    if(SubsystemInfoMsg.Present) {

        VehicleStatusMsg.OnboardControlSensorsPresent |= SubsystemInfoMsg.SubsystemType;

    } else {

        VehicleStatusMsg.OnboardControlSensorsPresent &= ~SubsystemInfoMsg.SubsystemType;

    }
    if(SubsystemInfoMsg.Enabled) {

        VehicleStatusMsg.OnboardControlSensorsEnabled |= SubsystemInfoMsg.SubsystemType;

    } else {

        VehicleStatusMsg.OnboardControlSensorsEnabled &= ~SubsystemInfoMsg.SubsystemType;

    }
    if(SubsystemInfoMsg.Ok) {

        VehicleStatusMsg.OnboardControlSensorsHealth |= SubsystemInfoMsg.SubsystemType;

    } else {

        VehicleStatusMsg.OnboardControlSensorsHealth &= ~SubsystemInfoMsg.SubsystemType;

    }

    /* RC input handle */
    if(!status_flags.rc_input_blocked && ManualControlSetpointMsg.Timestamp!=0 && (TimeNow() < ManualControlSetpointMsg.Timestamp + uint64(vm_params.rc_loss_t * 1e6f))) {
        if(!status_flags.rc_signal_found_once) {
            status_flags.rc_signal_found_once = true;
        }
        else {
            if(VehicleStatusMsg.RcSignalLost) {
                uint64 Now = TimeNow();
                (void) CFE_EVS_SendEvent(VM_RC_SIGN_REGAINED_INFO_EID, CFE_EVS_INFORMATION,
                        "Manual control regained after (%llu)ums",(Now - rc_signal_lost_timestamp));
            }
        }
        VehicleStatusMsg.RcSignalLost = false;
        const boolean in_armed_state = (VehicleStatusMsg.ArmingState == PX4_ARMING_STATE_ARMED || VehicleStatusMsg.ArmingState == PX4_ARMING_STATE_ARMED_ERROR);
        const boolean arm_button_pressed = (vm_params.arm_switch_is_button == 1 && ManualControlSetpointMsg.ArmSwitch == PX4_SWITCH_POS_ON);

        /* DISARM */
        const boolean stick_in_lower_left = false; //((ManualControlSetpointMsg.R < -STICK_ON_OFF_LIMIT) && (ManualControlSetpointMsg.Z <0.1f));
        const boolean arm_switch_to_disarm_transition = ((vm_params.arm_switch_is_button == 0) && (last_sp_man_arm_switch ==PX4_SWITCH_POS_ON) && (ManualControlSetpointMsg.ArmSwitch == PX4_SWITCH_POS_OFF) );

        if(in_armed_state && VehicleStatusMsg.RcInputMode != PX4_RC_IN_MODE_OFF && (stick_in_lower_left || arm_button_pressed || arm_switch_to_disarm_transition)) {

            //current_nav_state = NavigationSM.GetCurrentStateID();//TODO
            if(VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_MANUAL &&
                    VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_ACRO &&
                    VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_STAB &&
                    VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_RATTITUDE &&
                    !VehicleLandDetectedMsg.Landed) {
                (void) CFE_EVS_SendEvent(VM_RC_STK_DISARM_REJ_INFO_EID, CFE_EVS_INFORMATION,
                        "Stick disarm rejected, vehicle in flight");
            }
            else if ((stick_off_counter == vm_params.rc_arm_hyst && stick_on_counter < vm_params.rc_arm_hyst) || arm_switch_to_disarm_transition) {
                try {
                    (void) CFE_EVS_SendEvent(VM_RC_DISARM_INFO_EID, CFE_EVS_INFORMATION,
                            "Disarm engaged by rc");
                    ArmingSM.FSM.Disarm();
                    arming_state_changed = true;
                    HkTlm.usCmdCnt++;
                }
                catch(statemap::TransitionUndefinedException e)
                {
                    HkTlm.usCmdErrCnt++;
                    CFE_EVS_SendEvent(VM_ARMING_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
                            "Illegal Arming transition. [%s] Command rejected.","DISARM");
                }
            }
            stick_off_counter++;
        }
        /* do not reset the counter when holding the arm button longer than needed */
        else if (!(vm_params.arm_switch_is_button ==1 && ManualControlSetpointMsg.ArmSwitch == PX4_SWITCH_POS_ON)) {
            stick_off_counter = 0;
        }

        /* ARM */
        const boolean stick_in_lower_right = false; // (ManualControlSetpointMsg.R > STICK_ON_OFF_LIMIT  && ManualControlSetpointMsg.Z <0.1f);
        const boolean arm_switch_to_arm_transition = ((vm_params.arm_switch_is_button == 0) && (last_sp_man_arm_switch ==PX4_SWITCH_POS_OFF) && (ManualControlSetpointMsg.ArmSwitch == PX4_SWITCH_POS_ON) );

        if(!in_armed_state && VehicleStatusMsg.RcInputMode != PX4_RC_IN_MODE_OFF && (stick_in_lower_right || arm_button_pressed || arm_switch_to_arm_transition)) {
            if((stick_on_counter == vm_params.rc_arm_hyst && stick_off_counter < vm_params.rc_arm_hyst) || arm_switch_to_arm_transition) {
                if((VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_MANUAL) &&
                        (VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_ACRO) &&
                        (VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_STAB) &&
                        (VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_RATTITUDE) &&
                        (VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_POSCTL) &&
                        (VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_ALTCTL)) {
                    (void) CFE_EVS_SendEvent(VM_RC_STK_ARM_REJ_INFO_EID, CFE_EVS_INFORMATION,
                            "Stick arm rejected, vehicle not in manual mode ");
                }
                else if (VehicleStatusMsg.ArmingState == PX4_ARMING_STATE_STANDBY) {
                    try {
                        (void) CFE_EVS_SendEvent(VM_RC_ARM_INFO_EID, CFE_EVS_INFORMATION,
                                "Arm engaged by rc ");
                        ArmingSM.FSM.Arm();
                        HkTlm.usCmdCnt++;
                    }
                    catch(statemap::TransitionUndefinedException e)
                    {
                        HkTlm.usCmdErrCnt++;
                        CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
                                "Illegal Nav transition.  Command rejected.");
                    }

                    arming_state_changed = true;
                }
            }
            stick_on_counter++;
        }
        /* do not reset the counter when holding the arm button longer than needed */
        else if (!(vm_params.arm_switch_is_button ==1 && ManualControlSetpointMsg.ArmSwitch == PX4_SWITCH_POS_ON)) {
            stick_on_counter = 0;
        }
        last_sp_man_arm_switch = ManualControlSetpointMsg.ArmSwitch;

        /* KILLSWITCH */
        if(ManualControlSetpointMsg.KillSwitch == PX4_SWITCH_POS_ON) {
            if(!ActuatorArmedMsg.ManualLockdown) {
                (void) CFE_EVS_SendEvent(VM_RC_KIL_SWTCH_INFO_EID, CFE_EVS_INFORMATION,
                        "Killswitch engaged ");
                ActuatorArmedMsg.ManualLockdown = true;
                SendActuatorArmedMsg();
            }
        }
        else if(ManualControlSetpointMsg.KillSwitch == PX4_SWITCH_POS_OFF) {
            if(ActuatorArmedMsg.ManualLockdown) {
                (void) CFE_EVS_SendEvent(VM_RC_KIL_SWTCH_INFO_EID, CFE_EVS_INFORMATION,
                        "killswitch disengaged ");
                ActuatorArmedMsg.ManualLockdown = false;
                SendActuatorArmedMsg();
            }
        }

        /* Mode change routine */
        RcModes();

    }
    else if(!status_flags.rc_input_blocked && !VehicleStatusMsg.RcSignalLost ) {
        uint64 Now = TimeNow();
        (void) CFE_EVS_SendEvent(VM_RC_SIGN_LOST_INFO_EID, CFE_EVS_INFORMATION,
                "Manual control lost at t = (%llu)ums", (Now));
        try {
            NavigationSM.FSM.trAutoReturnToLaunch();
            HkTlm.usCmdCnt++;
            (void) CFE_EVS_SendEvent(VM_RC_MAN_INFO_EID, CFE_EVS_INFORMATION,
                    "Mode switched to auto rtl autonomously ");
        }
        catch(statemap::TransitionUndefinedException e)
        {
            HkTlm.usCmdErrCnt++;
            uint32 PrevState = NavigationSM.GetCurrentStateID();
            CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
            CFE_EVS_INFORMATION,
                    "Illegal Nav transition [%s -> %s].  Command rejected.",
                    GetNavStateAsString(PrevState), "AUTORTL");
        }

        VehicleStatusMsg.RcSignalLost = true;
        rc_signal_lost_timestamp = ManualControlSetpointMsg.Timestamp;
    }

}

void VM::RcModes() {

    boolean posctl =
            (ManualControlSetpointMsg.PosctlSwitch == PX4_SWITCH_POS_ON);
    boolean rtl = (ManualControlSetpointMsg.ReturnSwitch == PX4_SWITCH_POS_ON);
    boolean loiter =
            (ManualControlSetpointMsg.LoiterSwitch == PX4_SWITCH_POS_ON);
    boolean takeoff = (ManualControlSetpointMsg.TransitionSwitch
            == PX4_SWITCH_POS_ON);
    boolean manual = (!posctl && !rtl && !loiter && !takeoff);
    boolean mode_changed = !(posctl == previous_modes.inPosCtl
            && rtl == previous_modes.inRtl && loiter == previous_modes.inLoiter
            && manual == previous_modes.inManual
            && takeoff == previous_modes.intakeoff);

    if (posctl && !rtl && !loiter && !takeoff && mode_changed) {

        try {
            NavigationSM.FSM.trPositionControl();
            HkTlm.usCmdCnt++;
            (void) CFE_EVS_SendEvent(VM_RC_POSCTL_INFO_EID, CFE_EVS_INFORMATION,
                    "Mode switched to position control by rc");
        } catch (statemap::TransitionUndefinedException e) {
            HkTlm.usCmdErrCnt++;
            uint32 PrevState = NavigationSM.GetCurrentStateID();
            CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
            CFE_EVS_INFORMATION,
                    "Illegal Nav transition [%s -> %s].  Command rejected.",
                    GetNavStateAsString(PrevState), "POSCTL");
        }

    } else if (!posctl && rtl && !loiter && !takeoff && mode_changed) {

        try {
            NavigationSM.FSM.trAutoReturnToLaunch();
            HkTlm.usCmdCnt++;
            (void) CFE_EVS_SendEvent(VM_RC_RTL_INFO_EID, CFE_EVS_INFORMATION,
                    "Mode switched to auto rtl by rc ");
        } catch (statemap::TransitionUndefinedException e) {
            HkTlm.usCmdErrCnt++;
            uint32 PrevState = NavigationSM.GetCurrentStateID();
            CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
            CFE_EVS_INFORMATION,
                    "Illegal Nav transition [%s -> %s].  Command rejected.",
                    GetNavStateAsString(PrevState), "AUTORTL");
        }

    } else if (!posctl && !rtl && loiter && !takeoff && mode_changed) {
        try {
            NavigationSM.FSM.trAutoLoiter();
            HkTlm.usCmdCnt++;
            (void) CFE_EVS_SendEvent(VM_RC_LTR_INFO_EID, CFE_EVS_INFORMATION,
                    "Mode switched to auto loiter by rc");
        } catch (statemap::TransitionUndefinedException e) {
            HkTlm.usCmdErrCnt++;
            uint32 PrevState = NavigationSM.GetCurrentStateID();
            CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
            CFE_EVS_INFORMATION,
                    "Illegal Nav transition [%s -> %s].  Command rejected.",
                    GetNavStateAsString(PrevState), "AUTOLTR");
        }
    } else if (!posctl && !rtl && !loiter && takeoff && mode_changed) {
        try {
            NavigationSM.FSM.trAutoTakeoff();
            HkTlm.usCmdCnt++;
            (void) CFE_EVS_SendEvent(VM_RC_TAKE_OFF_INFO_EID,
            CFE_EVS_INFORMATION, "Mode switched to auto takeoff by rc");
        } catch (statemap::TransitionUndefinedException e) {
            HkTlm.usCmdErrCnt++;
            uint32 PrevState = NavigationSM.GetCurrentStateID();
            CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
            CFE_EVS_INFORMATION,
                    "Illegal Nav transition [%s -> %s].  Command rejected.",
                    GetNavStateAsString(PrevState), "AUTOTAKOF");
        }
    } else if (manual && mode_changed) {
        try {
            NavigationSM.FSM.trManual();
            HkTlm.usCmdCnt++;
            (void) CFE_EVS_SendEvent(VM_RC_MAN_INFO_EID, CFE_EVS_INFORMATION,
                    "Mode switched to Manual by rc");
        } catch (statemap::TransitionUndefinedException e) {
            HkTlm.usCmdErrCnt++;
            uint32 PrevState = NavigationSM.GetCurrentStateID();
            CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
            CFE_EVS_INFORMATION,
                    "Illegal Nav transition [%s -> %s].  Command rejected.",
                    GetNavStateAsString(PrevState), "MANUAL");
        }
    }
    previous_modes.inPosCtl = posctl;
    previous_modes.inRtl = rtl;
    previous_modes.inLoiter = loiter;
    previous_modes.inManual = manual;
    previous_modes.intakeoff = takeoff;
}

void VM::FlightSessionInit() {

    (void) CFE_EVS_SendEvent(VM_LND_INIT_INFO_EID, CFE_EVS_INFORMATION,
            "Flight states reset. [Navigation -> MANUAL | Arming -> STANDBY]");

    /* Push states to init */
    ArmingSM.FSM.Reset();
    NavigationSM.FSM.Reset();

    /* Transition from init to default state */
    ArmingSM.FSM.InitComplete();
    NavigationSM.FSM.trInitComplete();

}

const char* VM::GetNavStateAsString(uint32 id) {

    const char * State_ptr;
    switch (id) {
    case 0:
        State_ptr = "MANUAL";
        break;
    case 1:
        State_ptr = "ALTCTL";
        break;
    case 2:
        State_ptr = "POSCTL";
        break;
    case 3:
        State_ptr = "AUTORTL";
        break;
    case 4:
        State_ptr = "AUTOLTR";
        break;
    case 5:
        State_ptr = "ACRO";
        break;
    case 6:
        State_ptr = "STAB";
        break;
    case 7:
        State_ptr = "RATT";
        break;
    case 8:
        State_ptr = "AUTOTAKOF";
        break;
    case 9:
        State_ptr = "AUTOLND";
        break;
    default:
        State_ptr = "UNKNOWN";
        break;
    }
    return State_ptr;
}

void VM::UpdateParamsFromTable() {
    if (ConfigTblPtr != nullptr) {

        vm_params.autostart_id = ConfigTblPtr->SYS_AUTOSTART;
        vm_params.rc_in_off = ConfigTblPtr->COM_RC_IN_MODE;
        vm_params.arm_switch_is_button = ConfigTblPtr->COM_ARM_SWISBTN;
        vm_params.arm_without_gps = ConfigTblPtr->COM_ARM_WO_GPS;
        vm_params.arm_mission_required = ConfigTblPtr->COM_ARM_MIS_REQ;
        vm_params.rc_arm_hyst = ConfigTblPtr->COM_RC_ARM_HYST;
        vm_params.mav_type = ConfigTblPtr->MAV_TYPE;
        vm_params.system_id = ConfigTblPtr->MAV_SYS_ID;
        vm_params.component_id = ConfigTblPtr->MAV_COMP_ID;
        vm_params.cbrk_supply_chk = ConfigTblPtr->CBRK_SUPPLY_CHK;
        vm_params.cbrk_usb_chk = ConfigTblPtr->CBRK_USB_CHK;
        vm_params.cbrk_airspd_chk = ConfigTblPtr->CBRK_AIRSPD_CHK;
        vm_params.cbrk_enginefail_chk = ConfigTblPtr->CBRK_ENGINEFAIL;
        vm_params.cbrk_gpsdail_chk = ConfigTblPtr->CBRK_GPSFAIL;
        vm_params.cbrk_flightterm_chk = ConfigTblPtr->CBRK_FLIGHTTERM;
        vm_params.cbrk_velposerr_chk = ConfigTblPtr->CBRK_VELPOSERR;
        vm_params.nav_dll_act = ConfigTblPtr->NAV_DLL_ACT;
        vm_params.nav_rcl_act = ConfigTblPtr->NAV_RCL_ACT;
        vm_params.dl_loss_t = ConfigTblPtr->COM_DL_LOSS_T;
        vm_params.rc_loss_t = ConfigTblPtr->COM_RC_LOSS_T;
        vm_params.rc_stick_ovrde = ConfigTblPtr->COM_RC_STICK_OV;
        vm_params.rc_ovrde = ConfigTblPtr->COM_RC_OVERRIDE;
        vm_params.dl_reg_t = ConfigTblPtr->COM_DL_REG_T;
        vm_params.ef_throt = ConfigTblPtr->COM_EF_THROT;
        vm_params.ef_c2t = ConfigTblPtr->COM_EF_C2T;
        vm_params.ef_time = ConfigTblPtr->COM_EF_TIME;
        vm_params.gf_action = ConfigTblPtr->GF_ACTION;
        vm_params.disarm_land = ConfigTblPtr->COM_DISARM_LAND;
        vm_params.low_bat_act = ConfigTblPtr->COM_LOW_BAT_ACT;
        vm_params.of_loss_t = ConfigTblPtr->COM_OF_LOSS_T;
        vm_params.obl_act = ConfigTblPtr->COM_OBL_ACT;
        vm_params.obl_rcl_act = ConfigTblPtr->COM_OBL_RC_ACT;
        vm_params.home_h_t = ConfigTblPtr->COM_HOME_H_T;
        vm_params.home_v_t = ConfigTblPtr->COM_HOME_V_T;
        vm_params.flt_mode_1 = ConfigTblPtr->COM_FLTMODE1;
        vm_params.flt_mode_2 = ConfigTblPtr->COM_FLTMODE2;
        vm_params.flt_mode_3 = ConfigTblPtr->COM_FLTMODE3;
        vm_params.flt_mode_4 = ConfigTblPtr->COM_FLTMODE4;
        vm_params.flt_mode_5 = ConfigTblPtr->COM_FLTMODE5;
        vm_params.flt_mode_6 = ConfigTblPtr->COM_FLTMODE6;
        vm_params.arm_ekf_pos = ConfigTblPtr->COM_ARM_EKF_POS;
        vm_params.arm_ekf_vel = ConfigTblPtr->COM_ARM_EKF_VEL;
        vm_params.arm_ekf_hgt = ConfigTblPtr->COM_ARM_EKF_HGT;
        vm_params.arm_ekf_yaw = ConfigTblPtr->COM_ARM_EKF_YAW;
        vm_params.arm_ekf_ab = ConfigTblPtr->COM_ARM_EKF_AB;
        vm_params.arm_ekf_gb = ConfigTblPtr->COM_ARM_EKF_GB;
        vm_params.arm_imu_acc = ConfigTblPtr->COM_ARM_IMU_ACC;
        vm_params.arm_imu_gyr = ConfigTblPtr->COM_ARM_IMU_GYR;
        vm_params.posctl_navl = ConfigTblPtr->COM_POSCTL_NAVL;
        vm_params.home_pos_alt_padding = ConfigTblPtr->HOME_POS_ALT_PADDING;
    }

}

/************************/
/*  End of File Comment */
/************************/
