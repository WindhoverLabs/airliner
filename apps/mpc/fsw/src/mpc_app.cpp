/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "mpc_app.h"
#include "mpc_msg.h"
#include "mpc_msgids.h"
#include "mpc_version.h"
#include "px4lib.h"
#include "geo/geo.h"
#include "math/Matrix3F3.hpp"
#include "math/Quaternion.hpp"
#include "math/Euler.hpp"
#include "math/Dcm.hpp"
#include "math/Expo.hpp"
#include "math/Limits.hpp"
#include "math/Functions.hpp"
#include <string.h>
#include <float.h>
#include <math.h>
#include "px4lib_msgids.h"
#include "mpc_tbldefs.h"
#include "cfs_utils.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Local definitions                                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define MPC_CONSTANTS_ONE_G    9.80665f   /* m/s^2        */
#define SIGMA_SINGLE_OP        0.000001f
#define SIGMA_NORM             0.001f

/* Manual takeoff threshold.  Throttle must be greater than or equal
 * to this value to trigger a takeoff.
 */
#define MPC_MANUAL_TAKEOFF_THRESHOLD (0.65f)

/* Use this horizontal jerk limit when we just want a really big jerk limit. */
#define MPC_HOR_JERK_LIMIT_MAX (1000000.0f)


#define MPC_TILT_COS_MAX (0.7f)



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Instantiate the application object.                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
MPC oMPC;



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Default constructor.                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
MPC::MPC() :
        m_FilterManualPitch(50.0f, 10.0f),
        m_FilterManualRoll(50.0f, 10.0f),
        m_ManualDirectionChangeHysteresis(FALSE)
{

}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Destructor constructor.                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
MPC::~MPC()
{

}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables.                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MPC::InitEvent()
{
    int32  iStatus=CFE_SUCCESS;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(0, 0, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("MPC - Failed to register with EVS (0x%08lX)\n", iStatus);
    }

    return iStatus;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MPC::InitPipe()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&SchPipeId,
            MPC_SCH_PIPE_DEPTH,
            MPC_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(MPC_WAKEUP_MID, SchPipeId, CFE_SB_Default_Qos, MPC_WAKEUP_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MPC_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "Sch Pipe failed to subscribe to MPC_WAKEUP_MID. (0x%08lX)",
                    iStatus);
            goto MPC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(MPC_SEND_HK_MID, SchPipeId, CFE_SB_Default_Qos, MPC_SEND_HK_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MPC_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                     "CMD Pipe failed to subscribe to MPC_SEND_HK_MID. (0x%08X)",
                     (unsigned int)iStatus);
            goto MPC_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(MPC_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
             "Failed to create SCH pipe (0x%08lX)",
             iStatus);
        goto MPC_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&CmdPipeId,
            MPC_CMD_PIPE_DEPTH,
            MPC_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(MPC_CMD_MID, CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MPC_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                 "CMD Pipe failed to subscribe to MPC_CMD_MID. (0x%08lX)",
                 iStatus);
            goto MPC_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(MPC_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
             "Failed to create CMD pipe (0x%08lX)",
             iStatus);
        goto MPC_InitPipe_Exit_Tag;
    }

    /* Init data pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&DataPipeId,
            MPC_DATA_PIPE_DEPTH,
            MPC_DATA_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(PX4_CONTROL_STATE_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MPC_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                     "DATA Pipe failed to subscribe to PX4_CONTROL_STATE_MID. (0x%08lX)",
                     iStatus);
            goto MPC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_MANUAL_CONTROL_SETPOINT_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MPC_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                     "DATA Pipe failed to subscribe to PX4_MANUAL_CONTROL_SETPOINT_MID. (0x%08lX)",
                     iStatus);
            goto MPC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_HOME_POSITION_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MPC_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                     "DATA Pipe failed to subscribe to PX4_HOME_POSITION_MID. (0x%08lX)",
                     iStatus);
            goto MPC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_CONTROL_MODE_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MPC_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                     "DATA Pipe failed to subscribe to PX4_VEHICLE_CONTROL_MODE_MID. (0x%08lX)",
                     iStatus);
            goto MPC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_POSITION_SETPOINT_TRIPLET_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MPC_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                     "DATA Pipe failed to subscribe to PX4_POSITION_SETPOINT_TRIPLET_MID. (0x%08lX)",
                     iStatus);
            goto MPC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_STATUS_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MPC_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                     "DATA Pipe failed to subscribe to PX4_VEHICLE_STATUS_MID. (0x%08lX)",
                     iStatus);
            goto MPC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_LAND_DETECTED_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MPC_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                     "DATA Pipe failed to subscribe to PX4_VEHICLE_LAND_DETECTED_MID. (0x%08lX)",
                     iStatus);
            goto MPC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_LOCAL_POSITION_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MPC_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                     "DATA Pipe failed to subscribe to PX4_VEHICLE_LOCAL_POSITION_MID. (0x%08lX)",
                     iStatus);
            goto MPC_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(MPC_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
             "Failed to create DATA pipe (0x%08lX)",
             iStatus);
        goto MPC_InitPipe_Exit_Tag;
    }

MPC_InitPipe_Exit_Tag:
    return iStatus;
}
    


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::InitData()
{
    /* Init housekeeping message. */
    CFE_SB_InitMsg(&HkTlm,
        MPC_HK_TLM_MID, sizeof(HkTlm), TRUE);

    /* Init diagnostic message. */
    CFE_SB_InitMsg(&DiagTlm,
        MPC_DIAG_TLM_MID, sizeof(DiagTlm), TRUE);

    /* Init output messages */
    CFE_SB_InitMsg(&m_VehicleAttitudeSetpointMsg,
        PX4_VEHICLE_ATTITUDE_SETPOINT_MID, sizeof(PX4_VehicleAttitudeSetpointMsg_t), TRUE);
    m_VehicleAttitudeSetpointMsg.Q_D[0] = 1.0f;

    CFE_SB_InitMsg(&m_VehicleLocalPositionSetpointMsg,
        PX4_VEHICLE_LOCAL_POSITION_SETPOINT_MID, sizeof(PX4_VehicleLocalPositionSetpointMsg_t), TRUE);

    /* Clear input messages */
    CFE_PSP_MemSet(&m_ControlStateMsg, 0, sizeof(m_ControlStateMsg));
    CFE_PSP_MemSet(&m_ManualControlSetpointMsg, 0, sizeof(m_ManualControlSetpointMsg));
    CFE_PSP_MemSet(&m_HomePositionMsg, 0, sizeof(m_HomePositionMsg));
    CFE_PSP_MemSet(&m_VehicleControlModeMsg, 0, sizeof(m_VehicleControlModeMsg));
    CFE_PSP_MemSet(&m_PositionSetpointTripletMsg, 0, sizeof(m_PositionSetpointTripletMsg));
    CFE_PSP_MemSet(&m_VehicleStatusMsg, 0, sizeof(m_VehicleStatusMsg));
    CFE_PSP_MemSet(&m_VehicleLandDetectedMsg, 0, sizeof(m_VehicleLandDetectedMsg));
    CFE_PSP_MemSet(&m_VehicleLocalPositionMsg, 0, sizeof(m_VehicleLocalPositionMsg));
    CFE_PSP_MemSet(&m_VehicleLocalPositionSetpointMsg, 0, sizeof(m_VehicleLocalPositionSetpointMsg));

    m_ResetCounterZ = 0;
    m_ResetCounterXy = 0;
    m_HeadingResetCounter = 0;

    m_Position.Zero();
    m_PositionSetpoint.Zero();
    m_Velocity.Zero();
    m_VelocitySetpoint.Zero();
    m_VelocityPrevious.Zero();
    m_VelocitySetpointPrevious.Zero();
    m_VelocityErrD.Zero();
    m_CurrentPositionSetpoint.Zero();
    m_PreviousPositionSetpoint.Zero();
    m_RSetpoint.Identity();
    m_Rotation.Identity();
    m_ThrustInt.Zero();
    m_DerivativeZ = 0.0f;
    m_Yaw = 0.0f;
    m_ManYawOffset = 0.0f;

    m_ModeAuto = FALSE;
    m_PositionHoldEngaged = FALSE;
    m_AltitudeHoldEngaged = FALSE;
    m_RunPosControl = TRUE;
    m_RunAltControl = TRUE;
    m_ResetPositionSetpoint = TRUE;
    m_ResetAltitudeSetpoint = TRUE;
    m_DoResetAltPos = TRUE;
    m_WasLanded = TRUE;
    m_ResetIntZ = TRUE;
    m_ResetIntXY = TRUE;
    m_ResetYawSetpoint = TRUE;
    m_HoldOffboardXY = FALSE;
    m_HoldOffboardZ = FALSE;
    m_InTakeoff = FALSE;
    m_TripletLatLonFinite = FALSE;

    m_RefTimestamp = 0;
    m_RefPos = {};
    m_RefAlt = 0.0f;
    m_RefAltIsGlobal = FALSE;
    m_YawTakeoff = 0.0f;

    m_PosP.Zero();
    m_VelP.Zero();
    m_VelI.Zero();
    m_VelD.Zero();

    m_AccelerationStateLimitXY = 0.0f;
    m_AccelerationStateLimitZ  = 0.0f;
    m_ManualJerkLimitXY        = ConfigTblPtr->MPC_JERK_MAX;
    m_ManualJerkLimitZ         = (ConfigTblPtr->MPC_JERK_MAX > ConfigTblPtr->MPC_JERK_MIN) ? ConfigTblPtr->MPC_JERK_MAX: 1000000.0f;
    m_VelMaxXy                 = 0.0f;
    m_TakeoffVelLimit          = 0.0f;

    /* Set trigger time for manual direction change detection */
    m_ManualDirectionChangeHysteresis.set_hysteresis_time_from(FALSE, DIRECTION_CHANGE_TRIGGER_TIME_US);
    m_UserIntentionXY = BRAKE; // NOTE: This needs to be initialized to BRAKE to work.
    m_UserIntentionZ  = NONE;
    m_StickInputXyPrev.Zero();

    UpdateParamsFromTable();
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* MPC initialization                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MPC::InitApp()
{
    int32  iStatus   = CFE_SUCCESS;
    int8   hasEvents = 0;

    iStatus = InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("MPC - Failed to init events (0x%08lX)\n", iStatus);
        goto MPC_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        goto MPC_InitApp_Exit_Tag;
    }

    iStatus = InitConfigTbl();
    if (iStatus != CFE_SUCCESS)
    {
        goto MPC_InitApp_Exit_Tag;
    }

    InitData();

MPC_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MPC_INIT_INF_EID, CFE_EVS_INFORMATION,
                                 "Initialized.  Version %d.%d.%d.%d",
                                 MPC_MAJOR_VERSION,
                                 MPC_MINOR_VERSION,
                                 MPC_REVISION,
                                 MPC_MISSION_REV);
    }
    else
    {
        if (hasEvents != 1)
        {
            (void) CFE_ES_WriteToSysLog("MPC - Application failed to initialize\n");
        }
    }

    return iStatus;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MPC::RcvSchPipeMsg(int32 iBlocking)
{
    int32           iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr  = NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(MPC_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(MPC_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
        {
            case MPC_WAKEUP_MID:
            {
                /* If vehicle local position has been received begin
                 * cyclic ops. */
                if(ProcessDataPipe() == true) {
                    Execute();
                }
                break;
            }

            case MPC_SEND_HK_MID:
            {
                ProcessCmdPipe();
                ReportHousekeeping();
                break;
            }

            default:
            {
                (void) CFE_EVS_SendEvent(MPC_MSGID_ERR_EID, CFE_EVS_ERROR,
                     "Recvd invalid SCH msgId (0x%04X)", MsgId);
            }
        }
    }
    else if (iStatus == CFE_SB_NO_MESSAGE)
    {
        /* If there's no incoming message, you can do something here, or
         * nothing.  Note, this section is dead code only if the iBlocking arg
         * is CFE_SB_PEND_FOREVER. */
        iStatus = CFE_SUCCESS;
    }
    else if (iStatus == CFE_SB_TIME_OUT)
    {
        /* If there's no incoming message within a specified time (via the
         * iBlocking arg, you can do something here, or nothing.  
         * Note, this section is dead code only if the iBlocking arg
         * is CFE_SB_PEND_FOREVER. */
        iStatus = CFE_SUCCESS;
    }
    else
    {
        (void) CFE_EVS_SendEvent(MPC_RCVMSG_ERR_EID, CFE_EVS_ERROR,
              "SCH pipe read error (0x%08lX).", iStatus);
    }

    return iStatus;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool MPC::ProcessDataPipe()
{
    int32 iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Process command messages until the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&MsgPtr, DataPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            MsgId = CFE_SB_GetMsgId(MsgPtr);
            switch (MsgId)
            {
                case PX4_CONTROL_STATE_MID:
                {
                    ProcessControlStateMsg();
                    CFE_PSP_MemCpy(&m_ControlStateMsg, MsgPtr, sizeof(m_ControlStateMsg));
                    break;
                }

                case PX4_MANUAL_CONTROL_SETPOINT_MID:
                {
                    CFE_PSP_MemCpy(&m_ManualControlSetpointMsg, MsgPtr, sizeof(m_ManualControlSetpointMsg));
                    break;
                }

                case PX4_HOME_POSITION_MID:
                {
                    CFE_PSP_MemCpy(&m_HomePositionMsg, MsgPtr, sizeof(m_HomePositionMsg));
                    break;
                }

                case PX4_VEHICLE_CONTROL_MODE_MID:
                {
                    CFE_PSP_MemCpy(&m_VehicleControlModeMsg, MsgPtr, sizeof(m_VehicleControlModeMsg));
                    break;
                }

                case PX4_POSITION_SETPOINT_TRIPLET_MID:
                {
                    CFE_PSP_MemCpy(&m_PositionSetpointTripletMsg, MsgPtr, sizeof(m_PositionSetpointTripletMsg));
                    ProcessPositionSetpointTripletMsg();
                    break;
                }

                case PX4_VEHICLE_STATUS_MID:
                {
                    CFE_PSP_MemCpy(&m_VehicleStatusMsg, MsgPtr, sizeof(m_VehicleStatusMsg));
                    break;
                }

                case PX4_VEHICLE_LAND_DETECTED_MID:
                {
                    CFE_PSP_MemCpy(&m_VehicleLandDetectedMsg, MsgPtr, sizeof(m_VehicleLandDetectedMsg));
                    break;
                }

                case PX4_VEHICLE_LOCAL_POSITION_MID:
                {
                    CFE_PSP_MemCpy(&m_VehicleLocalPositionMsg, MsgPtr, sizeof(m_VehicleLocalPositionMsg));
                    ProcessVehicleLocalPositionMsg();
                    break;
                }

                default:
                {
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with this
                     *  pipe, but not handled in this switch-case.) */
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(MPC_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "Recvd invalid DATA msgId (0x%04X)", (unsigned short)MsgId);
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
            (void) CFE_EVS_SendEvent(MPC_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                  "DATA pipe read error (0x%08lX)", iStatus);
            break;
        }
    }

    return true;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::ProcessCmdPipe()
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
                case MPC_CMD_MID:
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
                    (void) CFE_EVS_SendEvent(MPC_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            (void) CFE_EVS_SendEvent(MPC_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08lX)", iStatus);
            break;
        }
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process MPC Commands                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::ProcessAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode = 0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case MPC_NOOP_CC:
            {
                if(VerifyCmdLength(MsgPtr, sizeof(MPC_NoArgCmd_t)) == TRUE)
                {
                    HkTlm.usCmdCnt++;
                    (void) CFE_EVS_SendEvent(MPC_CMD_NOOP_EID, CFE_EVS_INFORMATION,
                        "Recvd NOOP. Version %d.%d.%d.%d",
                        MPC_MAJOR_VERSION,
                        MPC_MINOR_VERSION,
                        MPC_REVISION,
                        MPC_MISSION_REV);
                }
                break;
            }

            case MPC_RESET_CC:
            {
                if(VerifyCmdLength(MsgPtr, sizeof(MPC_NoArgCmd_t)) == TRUE)
                {
                    HkTlm.usCmdCnt    = 0;
                    HkTlm.usCmdErrCnt = 0;
                }
                break;
            }

            case MPC_SET_XY_PID_CC:
            {
                if(VerifyCmdLength(MsgPtr, sizeof(MPC_SetPidCmd_t)) == TRUE)
                {
                    if(UpdateXyPids((MPC_SetPidCmd_t *) MsgPtr) == TRUE)
                    {
                        HkTlm.usCmdCnt++;
                        (void) CFE_EVS_SendEvent(MPC_PID_UPDATE_EID, CFE_EVS_INFORMATION,
                            "Updated XY PID values. Gain: %f, P: %f, I: %f, D: %f",
                            m_PosP[0],
                            m_VelP[0],
                            m_VelI[0],
                            m_VelD[0]);
                    }
                    else
                    {
                        HkTlm.usCmdErrCnt++;
                    }
                }
                break;
            }

            case MPC_SET_Z_PID_CC:
            {
                if(VerifyCmdLength(MsgPtr, sizeof(MPC_SetPidCmd_t)) == TRUE)
                {
                    if(UpdateZPids((MPC_SetPidCmd_t *) MsgPtr) == TRUE)
                    {
                        HkTlm.usCmdCnt++;
                        (void) CFE_EVS_SendEvent(MPC_PID_UPDATE_EID, CFE_EVS_INFORMATION,
                                "Updating Z PID values. Gain: %f, P: %f, I: %f, D: %f",
                                m_PosP[2],
                                m_VelP[2],
                                m_VelI[2],
                                m_VelD[2]);
                    }
                    else
                    {
                        HkTlm.usCmdErrCnt++;
                    }
                }
                break;
            }

            case MPC_SET_HOLD_DZ_CC:
            {
                if(VerifyCmdLength(MsgPtr, sizeof(MPC_SetDzCmd_t)) == TRUE)
                {
                    if(UpdateHoldDz((MPC_SetDzCmd_t *) MsgPtr) == TRUE)
                    {
                        HkTlm.usCmdCnt++;
                        (void) CFE_EVS_SendEvent(MPC_SET_DZ_EID, CFE_EVS_INFORMATION,
                                "Updated HOLD_DZ value: %f", ConfigTblPtr->HOLD_DZ);
                    }
                    else
                    {
                        HkTlm.usCmdErrCnt++;
                    }
                }
                break;
            }

            case MPC_SEND_DIAG_CC:
            {
                if(VerifyCmdLength(MsgPtr, sizeof(MPC_NoArgCmd_t)) == TRUE)
                {
                    ReportDiagnostic();
                    HkTlm.usCmdCnt++;
                    (void) CFE_EVS_SendEvent(MPC_SEND_DIAG_EID, CFE_EVS_INFORMATION, "Sending Diag packet.");
                }
                break;
            }

            case MPC_SET_STICK_EXPO_CC:
            {
                if(VerifyCmdLength(MsgPtr, sizeof(MPC_SetStickExpoCmd_t)) == TRUE)
                {
                    if(UpdateStickExpo((MPC_SetStickExpoCmd_t *) MsgPtr) == TRUE)
                    {
                        HkTlm.usCmdCnt++;
                        (void) CFE_EVS_SendEvent(MPC_SET_EXPO_EID, CFE_EVS_INFORMATION,
                                            "Updated stick expo values: XY: %f Z: %f",
                                            ConfigTblPtr->XY_MAN_EXPO,
                                            ConfigTblPtr->Z_MAN_EXPO);
                    }
                    else
                    {
                        HkTlm.usCmdErrCnt++;
                    }
                }
                break;
            }

            case MPC_SET_TKO_RAMP_CC:
            {
                if(VerifyCmdLength(MsgPtr, sizeof(MPC_SetTkoRampCmd_t)) == TRUE)
                {
                    if(UpdateTakeoffRampTime((MPC_SetTkoRampCmd_t *) MsgPtr) == TRUE)
                    {
                        HkTlm.usCmdCnt++;
                        (void) CFE_EVS_SendEvent(MPC_SET_TKO_RAMP_EID, CFE_EVS_INFORMATION,
                                            "Updated takeoff ramp time: %f",
                                            ConfigTblPtr->TKO_RAMP_T);
                    }
                    else
                    {
                        HkTlm.usCmdErrCnt++;
                    }
                }
                break;
            }

            default:
            {
                HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(MPC_CC_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid command code (%u)", (unsigned int)uiCmdCode);
                break;
            }
        }
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send MPC Housekeeping                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::ReportHousekeeping()
{
    HkTlm.AccelerationStateLimitXY = m_AccelerationStateLimitXY;
    HkTlm.AccelerationStateLimitZ  = m_AccelerationStateLimitZ;
    HkTlm.ManualJerkLimitXY        = m_ManualJerkLimitXY;
    HkTlm.ManualJerkLimitZ         = m_ManualJerkLimitZ;
    HkTlm.TakeoffVelLimit          = m_TakeoffVelLimit;
    HkTlm.VelMaxXy                 = m_VelMaxXy;
    HkTlm.YawTakeoff               = m_YawTakeoff;
    HkTlm.Yaw                      = m_Yaw;
    HkTlm.UserIntentionXY          = m_UserIntentionXY;
    HkTlm.UserIntentionZ           = m_UserIntentionZ;
    HkTlm.ModeAuto                 = m_ModeAuto;
    HkTlm.PositionHoldEngaged      = m_PositionHoldEngaged;
    HkTlm.AltitudeHoldEngaged      = m_AltitudeHoldEngaged;
    HkTlm.RunPosControl            = m_RunPosControl;
    HkTlm.RunAltControl            = m_RunAltControl;
    HkTlm.InTakeoff                = m_InTakeoff;
    HkTlm.TripletLatLonFinite      = m_TripletLatLonFinite;

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&HkTlm);
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send MPC Diagnostic Data                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::ReportDiagnostic()
{
    DiagTlm.Z_P              = m_PosP[2];
    DiagTlm.Z_VEL_P          = m_VelP[2];
    DiagTlm.Z_VEL_I          = m_VelI[2];
    DiagTlm.Z_VEL_D          = m_VelD[2];
    DiagTlm.Z_VEL_MAX_UP     = ConfigTblPtr->Z_VEL_MAX_UP;
    DiagTlm.Z_VEL_MAX_DN     = ConfigTblPtr->Z_VEL_MAX_DN;
    DiagTlm.Z_FF             = ConfigTblPtr->Z_FF;
    DiagTlm.XY_P             = m_PosP[0];
    DiagTlm.XY_VEL_P         = m_VelP[0];
    DiagTlm.XY_VEL_I         = m_VelI[0];
    DiagTlm.XY_VEL_D         = m_VelD[0];
    DiagTlm.XY_CRUISE        = ConfigTblPtr->XY_CRUISE;
    DiagTlm.MPC_VEL_MANUAL   = ConfigTblPtr->MPC_VEL_MANUAL;
    DiagTlm.XY_VEL_MAX       = ConfigTblPtr->XY_VEL_MAX;
    DiagTlm.XY_FF            = ConfigTblPtr->XY_FF;
    DiagTlm.TILTMAX_AIR      = ConfigTblPtr->TILTMAX_AIR;
    DiagTlm.ACC_HOR_MAX      = ConfigTblPtr->ACC_HOR_MAX;
    DiagTlm.ACC_UP_MAX       = ConfigTblPtr->ACC_UP_MAX;
    DiagTlm.ACC_DOWN_MAX     = ConfigTblPtr->ACC_DOWN_MAX;
    DiagTlm.MPC_DEC_HOR_SLOW = ConfigTblPtr->MPC_DEC_HOR_SLOW;
    DiagTlm.MPC_HOLD_DZ      = ConfigTblPtr->HOLD_DZ;
    DiagTlm.XY_MAN_EXPO      = ConfigTblPtr->XY_MAN_EXPO;
    DiagTlm.Z_MAN_EXPO       = ConfigTblPtr->Z_MAN_EXPO;
    DiagTlm.TKO_RAMP_T       = ConfigTblPtr->TKO_RAMP_T;

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&DiagTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&DiagTlm);
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Publish VehicleAttitudeSetpoint message                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::SendVehicleAttitudeSetpointMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&m_VehicleAttitudeSetpointMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&m_VehicleAttitudeSetpointMsg);
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Publish VehicleLocalPositionSetpoint message                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::SendVehicleLocalPositionSetpointMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&m_VehicleLocalPositionSetpointMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&m_VehicleLocalPositionSetpointMsg);
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Command Length                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool MPC::VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
                           uint16 usExpectedLen)
{
    osalbool bResult  = TRUE;
    uint16   usMsgLen = 0;

    if (MsgPtr != NULL)
    {
        usMsgLen = CFE_SB_GetTotalMsgLength(MsgPtr);

        if (usExpectedLen != usMsgLen)
        {
            bResult = FALSE;
            CFE_SB_MsgId_t MsgId = CFE_SB_GetMsgId(MsgPtr);
            uint16 usCmdCode = CFE_SB_GetCmdCode(MsgPtr);

            (void) CFE_EVS_SendEvent(MPC_MSGLEN_ERR_EID, CFE_EVS_ERROR,
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
/* MPC Application C style main entry point.                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
extern "C" void MPC_AppMain()
{
    oMPC.AppMain();
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* MPC Application C++ style main entry point.                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::AppMain()
{
    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("MPC - Failed to register the app (0x%08lX)\n", iStatus);
    }

    /* Register the application with Executive Services */
    uiRunStatus = CFE_ES_APP_RUN;

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(MPC_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(MPC_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(MPC_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(MPC_MAIN_TASK_PERF_ID);
    }
    else
    {
        uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&uiRunStatus) == TRUE)
    {
        RcvSchPipeMsg(MPC_SCH_PIPE_PEND_TIME);

        iStatus = AcquireConfigPointers();
        if(iStatus != CFE_SUCCESS)
        {
            /* We apparently tried to load a new table but failed.  Terminate the application. */
            uiRunStatus = CFE_ES_APP_ERROR;
        }
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(MPC_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(uiRunStatus);
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* ProcessVehicleLocalPositionMsg                                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::ProcessVehicleLocalPositionMsg(void)
{
    /* Check if a reset event has happened if the vehicle is in manual mode
     * we will shift the setpoints of the states which were reset. In auto
     * mode we do not shift the setpoints since we want the vehicle to track
     * the original state. */
    // TODO: PE doesn't increment the reset counters. This may be dead code.
    if (m_VehicleControlModeMsg.ControlManualEnabled)
    {
        if (m_ResetCounterZ != m_VehicleLocalPositionMsg.Z_ResetCounter)
        {
            m_PositionSetpoint[2] = m_VehicleLocalPositionMsg.Z;
        }

        if (m_ResetCounterXy != m_VehicleLocalPositionMsg.XY_ResetCounter)
        {
            m_PositionSetpoint[0] = m_VehicleLocalPositionMsg.X;
            m_PositionSetpoint[1] = m_VehicleLocalPositionMsg.Y;
        }
    }

    /* Update the reset counters in any case. */
    m_ResetCounterZ = m_VehicleLocalPositionMsg.Z_ResetCounter;
    m_ResetCounterXy = m_VehicleLocalPositionMsg.XY_ResetCounter;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* ProcessControlStateMsg                                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::ProcessControlStateMsg(void)
{
    math::Quaternion q_att(
            m_ControlStateMsg.Q[0],
            m_ControlStateMsg.Q[1],
            m_ControlStateMsg.Q[2],
            m_ControlStateMsg.Q[3]);

    m_Rotation = q_att.RotationMatrix();
    math::Vector3F euler_angles;

    euler_angles = m_Rotation.ToEuler();

    m_Yaw = euler_angles[2];

    if(m_VehicleControlModeMsg.ControlManualEnabled)
    {
        if (m_HeadingResetCounter != m_ControlStateMsg.QuatResetCounter)
        {
            m_HeadingResetCounter = m_ControlStateMsg.QuatResetCounter;
            math::Quaternion delta_q(m_ControlStateMsg.DeltaQReset[0],
                    m_ControlStateMsg.DeltaQReset[1],
                    m_ControlStateMsg.DeltaQReset[2],
                    m_ControlStateMsg.DeltaQReset[3]);

            /* We only extract the heading change from the delta quaternion */
            math::Vector3F delta_euler = delta_q.ToEuler();
            m_VehicleAttitudeSetpointMsg.YawBody += delta_euler[2];
        }
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* ProcessPositionSetpointTripletMsg                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::ProcessPositionSetpointTripletMsg(void)
{
    /* Set current position setpoint invalid if alt is infinite. */
    if (!isfinite(m_PositionSetpointTripletMsg.Current.Alt))
    {
        m_PositionSetpointTripletMsg.Current.Valid = FALSE;
    }

    /* To be a valid previous triplet, lat/lon/alt has to be finite */
    if (!isfinite(m_PositionSetpointTripletMsg.Previous.Lat) &&
        !isfinite(m_PositionSetpointTripletMsg.Previous.Lon) &&
        !isfinite(m_PositionSetpointTripletMsg.Previous.Alt))
    {
        m_PositionSetpointTripletMsg.Previous.Valid = FALSE;
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Execute                                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::Execute(void)
{
    static uint64 t_prev = 0;

    uint64 t = PX4LIB_GetPX4TimeUs();
    float dt = t_prev != 0 ? (t - t_prev) / 1e6f : 0.004f;
    t_prev = t;

    /* Set default max velocity in xy to vel_max */
    m_VelMaxXy = ConfigTblPtr->XY_VEL_MAX;

    /* Reset flags when landed */
    if (m_VehicleLandDetectedMsg.Landed)
    {
        m_ResetPositionSetpoint = TRUE;
        m_ResetAltitudeSetpoint = TRUE;
        m_DoResetAltPos         = TRUE;
        m_ModeAuto              = FALSE;
        m_PositionHoldEngaged   = FALSE;
        m_AltitudeHoldEngaged   = FALSE;
        m_RunPosControl         = TRUE;
        m_RunAltControl         = TRUE;
        m_ResetIntZ             = TRUE;
        m_ResetIntXY            = TRUE;
        m_ResetYawSetpoint      = TRUE;
        m_HoldOffboardXY        = FALSE;
        m_HoldOffboardZ         = FALSE;

        /* Also reset previous setpoints */
        m_YawTakeoff            = m_Yaw;
        m_VelocitySetpointPrevious.Zero();
        m_VelocityPrevious.Zero();

        /* Make sure attitude setpoint output "disables" attitude control */
        m_VehicleAttitudeSetpointMsg.Thrust = 0.0f;
        m_VehicleAttitudeSetpointMsg.Timestamp = PX4LIB_GetPX4TimeUs();
    }

    if (!m_InTakeoff && (!m_VehicleLandDetectedMsg.Landed && m_WasLanded) && m_VehicleControlModeMsg.Armed == PX4_ARMING_STATE_ARMED)
    {
        m_InTakeoff = TRUE;
        /* This ramp starts negative and goes to positive later because we want to
        *  be as smooth as possible. If we start at 0, we alrady jump to hover throttle. */
        m_TakeoffVelLimit = -.5f;
    }

    else if (!m_VehicleControlModeMsg.Armed == PX4_ARMING_STATE_ARMED) {
        /* If we're disarmed and for some reason were in a smooth takeoff, we reset that. */
        m_InTakeoff = FALSE;
    }

    /* Set triplets to invalid if we just landed */
    if (m_VehicleLandDetectedMsg.Landed && !m_WasLanded)
    {
        m_PositionSetpointTripletMsg.Current.Valid = FALSE;
    }

    m_WasLanded = m_VehicleLandDetectedMsg.Landed;
    UpdateRef();
    UpdateVelocityDerivative(dt);

    /* Reset the horizontal and vertical position hold flags for non-manual modes
     * or if position / altitude is not controlled. */
    if (!m_VehicleControlModeMsg.ControlPositionEnabled || !m_VehicleControlModeMsg.ControlManualEnabled)
    {
        m_PositionHoldEngaged = FALSE;
    }

    if (!m_VehicleControlModeMsg.ControlAltitudeEnabled || !m_VehicleControlModeMsg.ControlManualEnabled)
    {
        m_AltitudeHoldEngaged = FALSE;
    }

    if (m_VehicleControlModeMsg.ControlAltitudeEnabled ||
        m_VehicleControlModeMsg.ControlPositionEnabled ||
        m_VehicleControlModeMsg.ControlClimbRateEnabled ||
        m_VehicleControlModeMsg.ControlVelocityEnabled ||
        m_VehicleControlModeMsg.ControlAccelerationEnabled)
    {
        DoControl(dt);

        /* Fill local position, velocity and thrust setpoint */
        CFE_SB_TimeStampMsg((CFE_SB_MsgPtr_t)&m_VehicleLocalPositionSetpointMsg);
        m_VehicleLocalPositionSetpointMsg.X = m_PositionSetpoint[0];
        m_VehicleLocalPositionSetpointMsg.Y = m_PositionSetpoint[1];
        m_VehicleLocalPositionSetpointMsg.Z = m_PositionSetpoint[2];
        m_VehicleLocalPositionSetpointMsg.Yaw = m_VehicleAttitudeSetpointMsg.YawBody;
        m_VehicleLocalPositionSetpointMsg.VX = m_VelocitySetpoint[0];
        m_VehicleLocalPositionSetpointMsg.VY = m_VelocitySetpoint[1];
        m_VehicleLocalPositionSetpointMsg.VZ = m_VelocitySetpoint[2];

        /* Publish local position setpoint */
        SendVehicleLocalPositionSetpointMsg();
    }
    else
    {
        /* Position controller disabled, reset setpoints */
        m_ResetPositionSetpoint = TRUE;
        m_ResetAltitudeSetpoint = TRUE;
        m_DoResetAltPos         = TRUE;
        m_ModeAuto              = FALSE;
        m_ResetIntZ             = TRUE;
        m_ResetIntXY            = TRUE;

        /* Store last velocity in case a mode switch to position control occurs */
        m_VelocitySetpointPrevious = m_Velocity;
    }

    /* Generate attitude setpoint from manual controls */
    if (m_VehicleControlModeMsg.ControlManualEnabled && m_VehicleControlModeMsg.ControlAttitudeEnabled)
    {
        GenerateAttitudeSetpoint(dt);
    }
    else
    {
        m_ResetYawSetpoint = TRUE;
        m_VehicleAttitudeSetpointMsg.YawSpMoveRate = 0.0f;
    }

    /* Update previous velocity for velocity controller D part */
    m_VelocityPrevious = m_Velocity;

    /* publish attitude setpoint
     * Do not publish if
     * - offboard is enabled but position/velocity/accel control is disabled,
     * in this case the attitude setpoint is published by the mavlink app.
     * - if the vehicle is a VTOL and it's just doing a transition (the VTOL attitude control module will generate
     * attitude setpoints for the transition).
     * - if not armed
     */
    if (m_VehicleControlModeMsg.Armed == PX4_ARMING_STATE_ARMED &&
        (!(m_VehicleControlModeMsg.ControlOffboardEnabled &&
          !(m_VehicleControlModeMsg.ControlPositionEnabled ||
            m_VehicleControlModeMsg.ControlVelocityEnabled ||
            m_VehicleControlModeMsg.ControlAccelerationEnabled))))
    {
        SendVehicleAttitudeSetpointMsg();
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* UpdateRef                                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::UpdateRef(void)
{
    /* The reference point is only allowed to change when the vehicle is in standby state which is the
    normal state when the estimator origin is set. Changing reference point in flight causes large controller
    setpoint changes. Changing reference point in other arming states is untested and should not be performed. */
    if ((m_VehicleLocalPositionMsg.RefTimestamp != m_RefTimestamp)
        && ((m_VehicleStatusMsg.ArmingState == PX4_ARMING_STATE_STANDBY)
        || (!m_RefAltIsGlobal && m_VehicleLocalPositionMsg.Z_Global)))
    {
        double LatitudeSetpoint  = 0.0f;
        double LongitudeSetpoint = 0.0f;
        float AltitudeSetpoint   = 0.0f;
        uint64 CurrentTime       = 0;

        if(m_RefTimestamp != 0)
        {
            /* Calculate current position setpoint in global frame. */
            map_projection_reproject(&m_RefPos, m_PositionSetpoint[0], m_PositionSetpoint[1], &LatitudeSetpoint, &LongitudeSetpoint);

            /* The altitude setpoint is the reference altitude (Z up) plus the (Z down)
             * NED setpoint, multiplied out to minus*/
            AltitudeSetpoint = m_RefAlt - m_PositionSetpoint[2];
        }

        /* Update local projection reference including altitude. */
        CurrentTime = PX4LIB_GetPX4TimeUs();
        map_projection_init(&m_RefPos, m_VehicleLocalPositionMsg.RefLat, m_VehicleLocalPositionMsg.RefLon, CurrentTime);
        m_RefAlt = m_VehicleLocalPositionMsg.RefAlt;

        if(m_VehicleLocalPositionMsg.Z_Global)
        {
            m_RefAltIsGlobal = TRUE;
        }

        if (m_RefTimestamp != 0)
        {
            /* Reproject position setpoint to new reference this effectively
             * adjusts the position setpoint to keep the vehicle in its
             * current local position. It would only change its global
             * position on the next setpoint update. */
            map_projection_project(&m_RefPos, LatitudeSetpoint, LongitudeSetpoint, &m_PositionSetpoint[0], &m_PositionSetpoint[1]);
            m_PositionSetpoint[2] = -(AltitudeSetpoint - m_RefAlt);
        }

        m_RefTimestamp = m_VehicleLocalPositionMsg.RefTimestamp;
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* UpdateVelocityDerivative                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::UpdateVelocityDerivative(float dt)
{
    /* Update velocity derivative independent of the current flight mode */
    if (CFE_SB_IsMsgTimeZero((CFE_SB_MsgPtr_t)&m_VehicleLocalPositionMsg))
    {
        return;
    }

    if (isfinite(m_VehicleLocalPositionMsg.X) &&
        isfinite(m_VehicleLocalPositionMsg.Y) &&
        isfinite(m_VehicleLocalPositionMsg.Z))
    {
        m_Position[0] = m_VehicleLocalPositionMsg.X;
        m_Position[1] = m_VehicleLocalPositionMsg.Y;

        if (ConfigTblPtr->ALT_MODE == 1 && m_VehicleLocalPositionMsg.DistBottomValid)
        {
            m_Position[2] = -m_VehicleLocalPositionMsg.DistBottom;
        }
        else
        {
            m_Position[2] = m_VehicleLocalPositionMsg.Z;
        }
    }

    if (isfinite(m_VehicleLocalPositionMsg.VX) &&
        isfinite(m_VehicleLocalPositionMsg.VY) &&
        isfinite(m_VehicleLocalPositionMsg.VZ))
    {
        m_Velocity[0] = m_VehicleLocalPositionMsg.VX;
        m_Velocity[1] = m_VehicleLocalPositionMsg.VY;

        if (ConfigTblPtr->ALT_MODE == 1 && m_VehicleLocalPositionMsg.DistBottomValid)
        {
            m_Velocity[2] = -m_VehicleLocalPositionMsg.DistBottomRate;
        }
        else
        {
            m_Velocity[2] = m_VehicleLocalPositionMsg.VZ;
        }

        if (!m_RunAltControl)
        {
            /* Set velocity to the derivative of position
             * because it has less bias but blend it in across the landing speed range*/
            float weighting = fminf(fabsf(m_VelocitySetpoint[2]) / ConfigTblPtr->LAND_SPEED, 1.0f);
            m_Velocity[2] = m_DerivativeZ * weighting + m_Velocity[2] * (1.0f - weighting);

        }
    }

    if (isfinite(m_VehicleLocalPositionMsg.VZ))
    {
        m_DerivativeZ = m_VehicleLocalPositionMsg.VZ;
    };

    m_VelocityErrD[0] = m_VelXDeriv.Update(-m_Velocity[0], dt, ConfigTblPtr->VELD_LP);
    m_VelocityErrD[1] = m_VelYDeriv.Update(-m_Velocity[1], dt, ConfigTblPtr->VELD_LP);
    m_VelocityErrD[2] = m_VelZDeriv.Update(-m_Velocity[2], dt, ConfigTblPtr->VELD_LP);
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DoControl                                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::DoControl(float dt)
{
    /* By default, run position/altitude controller. the control_* functions
     * can disable this and run velocity controllers directly in this cycle */
    m_RunPosControl = TRUE;
    m_RunAltControl = TRUE;
    
    if (m_VehicleControlModeMsg.ControlManualEnabled)
    {
        /* Manual control */
        ControlManual(dt);
        m_ModeAuto = FALSE;

        /* We set tiplets to FALSE.  This ensures that when switching to auto,
         * the position controller will not use the old triplets but waits
         * until triplets have been updated. */
        m_PositionSetpointTripletMsg.Current.Valid = FALSE;
        m_PositionSetpointTripletMsg.Previous.Valid = FALSE;
        m_CurrentPositionSetpoint = math::Vector3F(NAN, NAN, NAN);

        m_HoldOffboardXY = FALSE;
        m_HoldOffboardZ = FALSE;
    }
    else
    {
        /* reset acceleration to default */
        m_AccelerationStateLimitXY = ConfigTblPtr->ACC_HOR_MAX;
        m_AccelerationStateLimitZ = ConfigTblPtr->ACC_UP_MAX;
        ControlNonManual(dt);
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* GenerateAttitudeSetpoint                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::GenerateAttitudeSetpoint(float dt)
{
    /* Yaw setpoint is integrated over time, but we don't want to integrate the offset's */
    m_VehicleAttitudeSetpointMsg.YawBody -= m_ManYawOffset;
    m_ManYawOffset = 0.0f;

    /* Reset yaw setpoint to current position if needed. */
    if (m_ResetYawSetpoint)
    {
        m_ResetYawSetpoint = FALSE;
        m_VehicleAttitudeSetpointMsg.YawBody = m_Yaw;
    }
    else if (!m_VehicleLandDetectedMsg.Landed &&
           !(!m_VehicleControlModeMsg.ControlAltitudeEnabled && m_ManualControlSetpointMsg.Z < 0.1f))
    {
        float YawTarget;
        float YawOffs;

        /* Do not move yaw while sitting on the ground. */

        /* We want to know the real constraint, and global overrides manual. */
        const float YawRateMax = (math::radians(ConfigTblPtr->MAN_Y_MAX) < math::radians(ConfigTblPtr->MC_YAWRATE_MAX)) ? math::radians(ConfigTblPtr->MAN_Y_MAX) :
                math::radians(ConfigTblPtr->MC_YAWRATE_MAX);
        const float YawOffsetMax = YawRateMax / math::radians(ConfigTblPtr->MC_YAW_P);

        m_VehicleAttitudeSetpointMsg.YawSpMoveRate = m_ManualControlSetpointMsg.R * YawRateMax;
        YawTarget = _wrap_pi(m_VehicleAttitudeSetpointMsg.YawBody + m_VehicleAttitudeSetpointMsg.YawSpMoveRate * dt);
        YawOffs = _wrap_pi(YawTarget - m_Yaw);

        /* If the yaw offset became too big for the system to track stop
         * shifting it, only allow if it would make the offset smaller again. */
        if (fabsf(YawOffs) < YawOffsetMax ||
            (m_VehicleAttitudeSetpointMsg.YawSpMoveRate > 0 && YawOffs < 0) ||
            (m_VehicleAttitudeSetpointMsg.YawSpMoveRate < 0 && YawOffs > 0))
        {
            m_VehicleAttitudeSetpointMsg.YawBody = YawTarget;
        }
    }

    /* Control throttle directly if no climb rate controller is active */
    if (!m_VehicleControlModeMsg.ControlClimbRateEnabled)
    {
        float ThrVal = ThrottleCurve(m_ManualControlSetpointMsg.Z, ConfigTblPtr->THR_HOVER);
        m_VehicleAttitudeSetpointMsg.Thrust = fmin(ThrVal, ConfigTblPtr->MANTHR_MAX);

        /* Enforce minimum throttle if not landed */
        if (!m_VehicleLandDetectedMsg.Landed)
        {
            m_VehicleAttitudeSetpointMsg.Thrust = fmax(m_VehicleAttitudeSetpointMsg.Thrust, ConfigTblPtr->MANTHR_MIN);
        }
    }

    /* Control roll and pitch directly if no aiding velocity controller is active. */
    if (!m_VehicleControlModeMsg.ControlVelocityEnabled)
    {
        m_VehicleAttitudeSetpointMsg.RollBody = m_ManualControlSetpointMsg.Y * math::radians(ConfigTblPtr->MAN_TILT_MAX);
        m_VehicleAttitudeSetpointMsg.PitchBody = -m_ManualControlSetpointMsg.X * math::radians(ConfigTblPtr->MAN_TILT_MAX);

        /* Only if optimal recovery is not used, modify roll/pitch. */
        if (ConfigTblPtr->VT_OPT_RECOV_EN <= 0)
        {
            /* Construct attitude setpoint rotation matrix. modify the setpoints for roll
             * and pitch such that they reflect the user's intention even if a yaw error
             * (yaw_sp - yaw) is present. In the presence of a yaw error constructing a rotation matrix
             * from the pure euler angle setpoints will lead to unexpected attitude behaviour from
             * the user's view as the euler angle sequence uses the  yaw setpoint and not the current
             * heading of the vehicle.
             */

            /* Calculate our current yaw error. */
            float yaw_error = _wrap_pi(m_VehicleAttitudeSetpointMsg.YawBody - m_Yaw);

            // Compute the vector obtained by rotating a z unit vector by the rotation
            // given by the roll and pitch commands of the user
            math::Vector3F zB = {0, 0, 1};
            math::Matrix3F3 R_sp_roll_pitch = math::Matrix3F3::FromEuler(m_VehicleAttitudeSetpointMsg.RollBody, m_VehicleAttitudeSetpointMsg.PitchBody, 0);
            math::Vector3F z_roll_pitch_sp = R_sp_roll_pitch * zB;

            /* Transform the vector into a new frame which is rotated around the z axis
             * by the current yaw error. this vector defines the desired tilt when we look
             * into the direction of the desired heading.
             */
            math::Matrix3F3 R_yaw_correction = math::Matrix3F3::FromEuler(0.0f, 0.0f, -yaw_error);
            z_roll_pitch_sp = R_yaw_correction * z_roll_pitch_sp;

            /* Use the formula z_roll_pitch_sp = R_tilt * [0;0;1]
             * R_tilt is computed from_euler; only TRUE if cos(roll) not equal zero
             * -> valid if roll is not +-pi/2;
             */
            m_VehicleAttitudeSetpointMsg.RollBody = -asinf(z_roll_pitch_sp[1]);
            m_VehicleAttitudeSetpointMsg.PitchBody = atan2f(z_roll_pitch_sp[0], z_roll_pitch_sp[2]);
        }

        /* Copy quaternion setpoint to attitude setpoint topic. */
        math::Quaternion q_sp(m_VehicleAttitudeSetpointMsg.RollBody, m_VehicleAttitudeSetpointMsg.PitchBody, m_VehicleAttitudeSetpointMsg.YawBody);
        q_sp.copyTo(m_VehicleAttitudeSetpointMsg.Q_D);
        m_VehicleAttitudeSetpointMsg.Q_D_Valid = TRUE;
    }

    m_VehicleAttitudeSetpointMsg.Timestamp = PX4LIB_GetPX4TimeUs();
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* ControlManual                                                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::ControlManual(float dt)
{
    float    YawInputFrame;
    float    StickZ;
    float    MaxAccZ;
    osalbool AltHoldDesired;
    osalbool PosHoldDesired;

    /* Velocity setpoint commanded by user stick input. */
    math::Vector3F ManVelSp(0.0f, 0.0f, 0.0f);

    /* Entering manual control from non-manual control mode, reset alt/pos setpoints */
    if(m_ModeAuto == TRUE)
    {
        m_ModeAuto = FALSE;

        /* Reset alt pos flags if resetting is enabled. */
        if(m_DoResetAltPos == TRUE)
        {
            m_ResetPositionSetpoint = TRUE;
            m_ResetAltitudeSetpoint = TRUE;
        }
    }

    /*
     * Map from stick input to velocity setpoint.
     */

    if(m_VehicleControlModeMsg.ControlAltitudeEnabled)
    {
        /* Set vertical velocity setpoint with throttle stick, remapping of
         * manual.z [0,1] to up and down command [-1,1] */
        ManVelSp[2] = -math::expof_deadzone(
                (m_ManualControlSetpointMsg.Z - 0.5f) * 2.0f,
                ConfigTblPtr->Z_MAN_EXPO, ConfigTblPtr->HOLD_DZ);

        /* Reset alt setpoint to current altitude if needed. */
        ResetAltSetpoint();
    }

    if (m_VehicleControlModeMsg.ControlPositionEnabled)
    {
        float ManVelHorLength;

        /* Set horizontal velocity setpoint with roll/pitch stick */
        ManVelSp[0] = math::expof_deadzone(
                m_ManualControlSetpointMsg.X,
                ConfigTblPtr->XY_MAN_EXPO, ConfigTblPtr->HOLD_DZ);
        ManVelSp[1] = math::expof_deadzone(
                m_ManualControlSetpointMsg.Y,
                ConfigTblPtr->XY_MAN_EXPO, ConfigTblPtr->HOLD_DZ);

        ManVelHorLength = math::Vector2F(ManVelSp[0], ManVelSp[1]).Length();

        /* Saturate such that magnitude is never larger than 1 */
        if (ManVelHorLength > 1.0f)
        {
            ManVelSp[0] /= ManVelHorLength;
            ManVelSp[1] /= ManVelHorLength;
        }

        /* Reset position setpoint to current position if needed */
        ResetPosSetpoint();
    }
    
    /* Prepare yaw to rotate into NED frame */
    YawInputFrame = m_VehicleControlModeMsg.ControlFixedHdgEnabled ? m_YawTakeoff : m_VehicleAttitudeSetpointMsg.YawBody;

    /* Setpoint in NED frame and scaled to cruise velocity */
    ManVelSp = math::Matrix3F3::FromEuler(0.0f, 0.0f, YawInputFrame) * ManVelSp;

    /* Adjust acceleration based on stick input */
    math::Vector2F StickXy(ManVelSp[0], ManVelSp[1]);
    SetManualAccelerationXY(StickXy, dt);
    StickZ = ManVelSp[2];
    MaxAccZ = 0.0f;
    SetManualAccelerationZ(MaxAccZ, StickZ, dt);

    /* Prepare cruise speed (m/s) vector to scale the velocity setpoint */
    m_VelMaxXy = (ConfigTblPtr->MPC_VEL_MANUAL < m_VelMaxXy) ? ConfigTblPtr->MPC_VEL_MANUAL : m_VelMaxXy;
    math::Vector3F VelCruiseScale(m_VelMaxXy, m_VelMaxXy, (ManVelSp[2] > 0.0f) ? ConfigTblPtr->Z_VEL_MAX_DN : ConfigTblPtr->Z_VEL_MAX_UP);

    /* Setpoint scaled to cruise speed */
    ManVelSp = ManVelSp.EMult(VelCruiseScale);

    /*
     * Assisted velocity mode: User controls velocity, but if velocity is small enough, position
     * hold is activated for the corresponding axis.
     */

    /* Want to get/stay in altitude hold if user has z stick in the middle (accounted for deadzone already) */
    AltHoldDesired = m_VehicleControlModeMsg.ControlAltitudeEnabled && (m_UserIntentionZ == BRAKE);

    /* Want to get/stay in position hold if user has xy stick in the middle (accounted for deadzone already) */
    PosHoldDesired = m_VehicleControlModeMsg.ControlPositionEnabled && (m_UserIntentionXY ==  BRAKE);

    /* Check vertical hold engaged flag. */
    if (m_AltitudeHoldEngaged)
    {
        m_AltitudeHoldEngaged = AltHoldDesired;
    }
    else
    {
        /* Check if we switch to alt_hold_engaged. */
        osalbool SmoothAltTransition = AltHoldDesired && ((MaxAccZ - m_AccelerationStateLimitZ) < FLT_EPSILON) &&
                                 (ConfigTblPtr->HOLD_MAX_Z < FLT_EPSILON || fabsf(m_Velocity[2]) < ConfigTblPtr->HOLD_MAX_Z);

        /* During transition predict setpoint forward. */
        if (SmoothAltTransition)
        {
            /* Time to travel from current velocity to zero velocity. */
            float DeltaT = fabsf(m_Velocity[2] / MaxAccZ);

            /* Set desired position setpoint assuming max acceleraiton. */
            m_PositionSetpoint[2] = m_Position[2] + m_Velocity[2] * DeltaT + 0.5f * MaxAccZ * DeltaT * DeltaT;

            m_AltitudeHoldEngaged = TRUE;
        }
    }

    /* Check horizontal hold engaged flag. */
    if (m_PositionHoldEngaged)
    {
        m_PositionHoldEngaged = PosHoldDesired;

        /* Use max acceleration */
        if (m_PositionHoldEngaged)
        {
            m_AccelerationStateLimitXY = ConfigTblPtr->ACC_HOR_MAX;
        }
    }
    else
    {
        /* Check if we switch to pos_hold_engaged. */
        float VelXyMag = sqrtf(m_Velocity[0] * m_Velocity[0] + m_Velocity[1] * m_Velocity[1]);
        osalbool SmoothPosTransition = PosHoldDesired
                                 && (fabsf(ConfigTblPtr->ACC_HOR_MAX - m_AccelerationStateLimitXY) < FLT_EPSILON) &&
                                 (ConfigTblPtr->HOLD_MAX_XY < FLT_EPSILON || VelXyMag < ConfigTblPtr->HOLD_MAX_XY);

        /* During transition predict setpoint forward. */
        if (SmoothPosTransition)
        {
            /* Time to travel from current velocity to zero velocity. */
            float DeltaT = sqrtf(m_Velocity[0] * m_Velocity[0] + m_Velocity[1] * m_Velocity[1]) / ConfigTblPtr->ACC_HOR_MAX;

            /* Position setpoint in xy from max acceleration and current velocity */
            math::Vector2F Pos(m_Position[0], m_Position[1]);
            math::Vector2F Vel(m_Velocity[0], m_Velocity[1]);
            math::Vector2F PosSp = Pos + Vel * DeltaT - Vel.Normalized() * 0.5f * ConfigTblPtr->ACC_HOR_MAX * DeltaT * DeltaT;
            m_PositionSetpoint[0] = PosSp[0];
            m_PositionSetpoint[1] = PosSp[1];

            m_PositionHoldEngaged = TRUE;
        }
    }

    /* Set requested velocity setpoints */
    if (!m_AltitudeHoldEngaged)
    {
        m_PositionSetpoint[2] = m_Position[2];
        /* Request velocity setpoint to be used, instead of altitude setpoint */
        m_RunAltControl = FALSE;
        m_VelocitySetpoint[2] = ManVelSp[2];
    }

    if (!m_PositionHoldEngaged)
    {
        m_PositionSetpoint[0] = m_Position[0];
        m_PositionSetpoint[1] = m_Position[1];
        /* Request velocity setpoint to be used, instead of position setpoint */
        m_RunPosControl = FALSE;
        m_VelocitySetpoint[0] = ManVelSp[0];
        m_VelocitySetpoint[1] = ManVelSp[1];
    }

    ControlPosition(dt);
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* ControlNonManual                                                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::ControlNonManual(float dt)
{
    osalbool VelocityValid;

    /* Select control source. */
    if(m_VehicleControlModeMsg.ControlOffboardEnabled)
    {
        /* Offboard control */
        ControlOffboard(dt);
        m_ModeAuto = FALSE;
    }
    else
    {
        m_HoldOffboardXY = FALSE;
        m_HoldOffboardZ = FALSE;

        /* AUTO */
        ControlAuto(dt);
    }

    /* Guard against any bad velocity values. */
    VelocityValid = isfinite(m_PositionSetpointTripletMsg.Current.VX) &&
            isfinite(m_PositionSetpointTripletMsg.Current.VY) &&
            m_PositionSetpointTripletMsg.Current.VelocityValid;

    /* Do not go slower than the follow target velocity when position tracking
     * is active (set to valid)     */
    if (m_PositionSetpointTripletMsg.Current.Type == PX4_SETPOINT_TYPE_FOLLOW_TARGET &&
        VelocityValid &&
        m_PositionSetpointTripletMsg.Current.PositionValid)
    {
        math::Vector3F FtVel(m_PositionSetpointTripletMsg.Current.VX, m_PositionSetpointTripletMsg.Current.VY, 0);
        float CosRatio = (FtVel * m_VelocitySetpoint) / (FtVel.Length() * m_VelocitySetpoint.Length());

        /* Only override velocity set points when uav is traveling in same
         * direction as target and vector component is greater than
         * calculated position set point velocity component. */
        if (CosRatio > 0)
        {
            FtVel = FtVel * CosRatio;
            /* Min speed a little faster than target vel. */
            FtVel = FtVel + FtVel.Normalized() * 1.5f;

        }
        else
        {
            FtVel.Zero();
        }

        /* Track target using velocity only. */
        m_VelocitySetpoint[0] = fabsf(FtVel[0]) > fabsf(m_VelocitySetpoint[0]) ? FtVel[0] : m_VelocitySetpoint[0];
        m_VelocitySetpoint[1] = fabsf(FtVel[1]) > fabsf(m_VelocitySetpoint[1]) ? FtVel[1] : m_VelocitySetpoint[1];
    }
    else if(m_PositionSetpointTripletMsg.Current.Type == PX4_SETPOINT_TYPE_FOLLOW_TARGET &&
           VelocityValid)
    {
        m_VelocitySetpoint[0] = m_PositionSetpointTripletMsg.Current.VX;
        m_VelocitySetpoint[1] = m_PositionSetpointTripletMsg.Current.VY;
    }

    /* Use constant descend rate when landing, ignore altitude setpoint. */
    if (m_PositionSetpointTripletMsg.Current.Valid
        && m_PositionSetpointTripletMsg.Current.Type == PX4_SETPOINT_TYPE_LAND)
    {
        m_VelocitySetpoint[2] = ConfigTblPtr->LAND_SPEED;
        m_RunAltControl = FALSE;
    }

    if (m_PositionSetpointTripletMsg.Current.Valid
        && m_PositionSetpointTripletMsg.Current.Type == PX4_SETPOINT_TYPE_IDLE)
    {
        /* Idle state, don't run controller and set zero thrust. */
        m_RSetpoint.Identity();
        math::Quaternion Qd(m_RSetpoint);
        Qd.copyTo(m_VehicleAttitudeSetpointMsg.Q_D);
        m_VehicleAttitudeSetpointMsg.Q_D_Valid = TRUE;

        m_VehicleAttitudeSetpointMsg.RollBody = 0.0f;
        m_VehicleAttitudeSetpointMsg.PitchBody = 0.0f;
        m_VehicleAttitudeSetpointMsg.YawBody = m_Yaw;
        m_VehicleAttitudeSetpointMsg.Thrust = 0.0f;

        m_VehicleAttitudeSetpointMsg.Timestamp = PX4LIB_GetPX4TimeUs();
    }
    else
    {
        ControlPosition(dt);
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* ThrottleCurve                                                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
float MPC::ThrottleCurve(float ctl, float ctr)
{
    float result;

    /* Piecewise linear mapping: 0:ctr -> 0:0.5
     * and ctr:1 -> 0.5:1 */
    if (ctl < 0.5f)
    {
        result = 2.0f * ctl * ctr;
    }
    else
    {
        result = ctr + 2.0f * (ctl - 0.5f) * (1.0f - ctr);
    }

    return result;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* ResetPosSetpoint                                                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::ResetPosSetpoint(void)
{
    if (m_ResetPositionSetpoint)
    {
        m_ResetPositionSetpoint = FALSE;

        /* We have logic in the main function which chooses the velocity setpoint such that the attitude setpoint is
         * continuous when switching into velocity controlled mode, therefore, we don't need to bother about resetting
         * altitude in a special way. */
        m_PositionSetpoint[0] = m_Position[0];
        m_PositionSetpoint[1] = m_Position[1];
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* ResetAltSetpoint                                                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::ResetAltSetpoint(void)
{
    if (m_ResetAltitudeSetpoint)
    {
        m_ResetAltitudeSetpoint = FALSE;

        /* We have logic in the main function which chooses the velocity setpoint such that the attitude setpoint is
         * continuous when switching into velocity controlled mode, therefore, we don't need to bother about resetting
         * position in a special way. In position control mode the position will be reset anyway until the vehicle has reduced speed.
         */
        m_PositionSetpoint[2] = m_Position[2];
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* ControlPosition                                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::ControlPosition(float dt)
{
    CalculateVelocitySetpoint(dt);

    if (m_VehicleControlModeMsg.ControlClimbRateEnabled ||
        m_VehicleControlModeMsg.ControlVelocityEnabled ||
        m_VehicleControlModeMsg.ControlAccelerationEnabled)
    {
        CalculateThrustSetpoint(dt);
    }
    else
    {
        m_ResetIntZ = TRUE;
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* ControlOffboard                                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::ControlOffboard(float dt)
{
    if(m_PositionSetpointTripletMsg.Current.Valid)
    {
        if (m_VehicleControlModeMsg.ControlPositionEnabled && m_PositionSetpointTripletMsg.Current.PositionValid)
        {
            /* Control position */
            m_PositionSetpoint[0] = m_PositionSetpointTripletMsg.Current.X;
            m_PositionSetpoint[1] = m_PositionSetpointTripletMsg.Current.Y;
            m_RunPosControl = TRUE;

            m_HoldOffboardXY = FALSE;
        }
        else if(m_VehicleControlModeMsg.ControlVelocityEnabled &&
                m_PositionSetpointTripletMsg.Current.VelocityValid)
        {
            /* Control velocity */

            /* Reset position setpoint to current position if needed */
            ResetPosSetpoint();

            if (fabsf(m_PositionSetpointTripletMsg.Current.VX) <= FLT_EPSILON &&
                fabsf(m_PositionSetpointTripletMsg.Current.VY) <= FLT_EPSILON &&
                m_VehicleLocalPositionMsg.XY_Valid)
            {
                if (!m_HoldOffboardXY)
                {
                    m_PositionSetpoint[0] = m_Position[0];
                    m_PositionSetpoint[1] = m_Position[1];
                    m_HoldOffboardXY = TRUE;
                }

                m_RunPosControl = TRUE;
            }
            else
            {
                if (m_PositionSetpointTripletMsg.Current.VelocityFrame == PX4_VELOCITY_FRAME_LOCAL_NED)
                {
                    /* Set position setpoint move rate */
                    m_VelocitySetpoint[0] = m_PositionSetpointTripletMsg.Current.VX;
                    m_VelocitySetpoint[1] = m_PositionSetpointTripletMsg.Current.VY;
                }
                else if (m_PositionSetpointTripletMsg.Current.VelocityFrame == PX4_VELOCITY_FRAME_BODY_NED) {
                    /* Transform velocity command from body frame to NED frame */
                    m_VelocitySetpoint[0] = cosf(m_Yaw) * m_PositionSetpointTripletMsg.Current.VX - sinf(m_Yaw) * m_PositionSetpointTripletMsg.Current.VY;
                    m_VelocitySetpoint[1] = sinf(m_Yaw) * m_PositionSetpointTripletMsg.Current.VX + cosf(m_Yaw) * m_PositionSetpointTripletMsg.Current.VY;
                }
                else
                {
                    (void) CFE_EVS_SendEvent(MPC_UNK_VEL_FRM_ERR_EID, CFE_EVS_ERROR,
                            "Unknown velocity offboard coordinate frame. (%u)",
                            m_PositionSetpointTripletMsg.Current.VelocityFrame);
                }

                m_RunPosControl = FALSE;
                m_HoldOffboardXY = FALSE;
            }
        }

        if (m_VehicleControlModeMsg.ControlAltitudeEnabled && m_PositionSetpointTripletMsg.Current.AltValid)
        {
            /* Control altitude as it is enabled. */
            m_PositionSetpoint[2] = m_PositionSetpointTripletMsg.Current.Z;
            m_RunAltControl = TRUE;
            m_HoldOffboardZ = FALSE;
        }
        else if (m_VehicleControlModeMsg.ControlClimbRateEnabled && m_PositionSetpointTripletMsg.Current.VelocityValid)
        {
            /* Reset alt setpoint to current altitude if needed */
            ResetAltSetpoint();

            if (fabsf(m_PositionSetpointTripletMsg.Current.VZ) <= FLT_EPSILON &&
                m_VehicleLocalPositionMsg.Z_Valid)
            {
                if (!m_HoldOffboardZ)
                {
                    m_PositionSetpoint[2] = m_Position[2];
                    m_HoldOffboardZ = TRUE;
                }

                m_RunAltControl = TRUE;
            }
            else
            {
                /* Set position setpoint move rate */
                m_VelocitySetpoint[2] = m_PositionSetpointTripletMsg.Current.VZ;
                m_RunAltControl = FALSE;
                m_HoldOffboardZ = FALSE;
            }
        }

        if (m_PositionSetpointTripletMsg.Current.YawValid)
        {
            m_VehicleAttitudeSetpointMsg.YawBody = m_PositionSetpointTripletMsg.Current.Yaw;
        }
        else if (m_PositionSetpointTripletMsg.Current.YawspeedValid)
        {
            float YawTarget = _wrap_pi(m_VehicleAttitudeSetpointMsg.YawBody + m_PositionSetpointTripletMsg.Current.Yawspeed * dt);
            float YawOffs = _wrap_pi(YawTarget - m_Yaw);

            const float YawRateMax = (math::radians(ConfigTblPtr->MAN_Y_MAX) < math::radians(ConfigTblPtr->MC_YAWRATE_MAX)) ? math::radians(ConfigTblPtr->MAN_Y_MAX) :
                    math::radians(ConfigTblPtr->MC_YAWRATE_MAX);
            const float YawOffsetMax = YawRateMax / math::radians(ConfigTblPtr->MC_YAW_P);

            /* If the yaw offset became too big for the system to track stop
            *  shifting it, only allow if it would make the offset smaller again. */
            if (fabsf(YawOffs) < YawOffsetMax ||
                (m_PositionSetpointTripletMsg.Current.Yawspeed > 0 && YawOffs < 0) ||
                (m_PositionSetpointTripletMsg.Current.Yawspeed < 0 && YawOffs > 0))
            {
                m_VehicleAttitudeSetpointMsg.YawBody = YawTarget;
            }
        }
    }
    else
    {
        m_HoldOffboardXY = FALSE;
        m_HoldOffboardZ = FALSE;
        ResetPosSetpoint();
        ResetAltSetpoint();
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* ControlAuto                                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::ControlAuto(float dt)
{
    osalbool CurrentSetpointValid  = FALSE;
    osalbool PreviousSetpointValid = FALSE;
    osalbool NextSetpointValid     = FALSE;
    osalbool TripletUpdated        = FALSE;

    math::Vector3F PrevSp(0.0f, 0.0f, 0.0f);
    math::Vector3F NextSp(0.0f, 0.0f, 0.0f);

    /* Reset position setpoint on AUTO mode activation or if we are not in MC mode */
    if (!m_ModeAuto || !m_VehicleStatusMsg.IsRotaryWing)
    {
        if (!m_ModeAuto)
        {
            m_ModeAuto = TRUE;
            m_TripletLatLonFinite = TRUE;
        }

        m_ResetPositionSetpoint = TRUE;
        m_ResetAltitudeSetpoint = TRUE;
    }

    /* Always check reset state of altitude and position control flags in auto. */
    ResetPosSetpoint();
    ResetAltSetpoint();

    if (m_PositionSetpointTripletMsg.Current.Valid)
    {
        float diff = 0.0f;
        math::Vector3F CurrPosSp = m_CurrentPositionSetpoint;

        /* Only project setpoints if they are finite, else use current
         * position. */
        if (isfinite(m_PositionSetpointTripletMsg.Current.Lat) &&
            isfinite(m_PositionSetpointTripletMsg.Current.Lon))
        {
            /* Project setpoint to local frame. */
            map_projection_project(&m_RefPos,
                    m_PositionSetpointTripletMsg.Current.Lat, m_PositionSetpointTripletMsg.Current.Lon,
                           &CurrPosSp[0], &CurrPosSp[1]);

            m_TripletLatLonFinite = TRUE;
        }
        /* Use current position if NAN -> e.g. land */
        else
        {
            if (m_TripletLatLonFinite)
            {
                CurrPosSp[0] = m_Position[0];
                CurrPosSp[1] = m_Position[1];
                m_TripletLatLonFinite = FALSE;
            }
        }

        /* Only project setpoints if they are finite, else use current position. */
        if (isfinite(m_PositionSetpointTripletMsg.Current.Alt))
        {
            CurrPosSp[2] = -(m_PositionSetpointTripletMsg.Current.Alt - m_RefAlt);
        }

        /* Sanity check */
        if (isfinite(m_CurrentPositionSetpoint[0]) &&
            isfinite(m_CurrentPositionSetpoint[1]) &&
            isfinite(m_CurrentPositionSetpoint[2]))
        {
            CurrentSetpointValid = TRUE;
        }

        /* Check if triplets have been updated
         * note: we only can look at xy since navigator applies slewrate to z */

        if (m_TripletLatLonFinite)
        {
            diff = math::Vector2F((m_CurrentPositionSetpoint[0] - CurrPosSp[0]), (m_CurrentPositionSetpoint[1] - CurrPosSp[1])).Length();
        }
        else
        {
            diff = fabsf(m_CurrentPositionSetpoint[2] - CurrPosSp[2]);
        }

        if (diff > FLT_EPSILON || !isfinite(diff))
        {
            TripletUpdated = TRUE;
        }

        /* We need to update m_CurrentPositionSetpoint always since navigator applies slew rate on z */
        m_CurrentPositionSetpoint = CurrPosSp;
    }

    if (m_PositionSetpointTripletMsg.Previous.Valid)
    {
        map_projection_project(&m_RefPos,
                m_PositionSetpointTripletMsg.Previous.Lat, m_PositionSetpointTripletMsg.Previous.Lon,
                       &PrevSp[0], &PrevSp[1]);
        PrevSp[2] = -(m_PositionSetpointTripletMsg.Previous.Alt - m_RefAlt);

        if (isfinite(PrevSp[0]) &&
            isfinite(PrevSp[1]) &&
            isfinite(PrevSp[2]))
        {
            m_PreviousPositionSetpoint = PrevSp;
            PreviousSetpointValid = TRUE;
        }
    }

    /* Set previous setpoint to current position if no previous setpoint available */
    if (!PreviousSetpointValid && TripletUpdated) {
        m_PreviousPositionSetpoint = m_Position;
        PreviousSetpointValid = TRUE; /* currrently not necessary to set to TRUE since not used*/
    }

    if (m_PositionSetpointTripletMsg.Next.Valid)
    {
        map_projection_project(&m_RefPos,
                m_PositionSetpointTripletMsg.Next.Lat, m_PositionSetpointTripletMsg.Next.Lon,
                       &NextSp[0], &NextSp[1]);
        NextSp[2] = -(m_PositionSetpointTripletMsg.Next.Alt - m_RefAlt);

        if (isfinite(NextSp[0]) &&
            isfinite(NextSp[1]) &&
            isfinite(NextSp[2]))
        {
            NextSetpointValid = TRUE;
        }
    }

    /* Auto logic:
     * The vehicle should follow the line previous-current.
     * - if there is no next setpoint or the current is a loiter point, then slowly approach the current along the line
     * - if there is a next setpoint, then the velocity is adjusted depending on the angle of the corner prev-current-next.
     * When following the line, the PosSp is computed from the orthogonal distance to the closest point on line and the desired cruise speed along the track.
     */

    /* create new _PosSp from triplets */
    if (CurrentSetpointValid &&
        (m_PositionSetpointTripletMsg.Current.Type != PX4_SETPOINT_TYPE_IDLE))
    {
        float YawDiff;

        /* update yaw setpoint if needed */
        if (m_PositionSetpointTripletMsg.Current.YawspeedValid
            && m_PositionSetpointTripletMsg.Current.Type == PX4_SETPOINT_TYPE_FOLLOW_TARGET)
        {
            m_VehicleAttitudeSetpointMsg.YawBody = m_VehicleAttitudeSetpointMsg.YawBody + m_PositionSetpointTripletMsg.Current.Yawspeed * dt;
        }
        else if (isfinite(m_PositionSetpointTripletMsg.Current.Yaw))
        {
            m_VehicleAttitudeSetpointMsg.YawBody = m_PositionSetpointTripletMsg.Current.Yaw;
        }

        YawDiff = _wrap_pi(m_VehicleAttitudeSetpointMsg.YawBody - m_Yaw);

        /* Only follow previous-current-line for specific triplet type */
        if (m_PositionSetpointTripletMsg.Current.Type == PX4_SETPOINT_TYPE_POSITION  ||
            m_PositionSetpointTripletMsg.Current.Type == PX4_SETPOINT_TYPE_LOITER ||
            m_PositionSetpointTripletMsg.Current.Type == PX4_SETPOINT_TYPE_FOLLOW_TARGET)
        {
            /* By default, use current setpoint as is. */
            math::Vector3F PosSp = m_CurrentPositionSetpoint;

            /*
             * Z-DIRECTION
             */

            /* Get various distances */
            float TotalDistZ = fabsf(m_CurrentPositionSetpoint[2] - m_PreviousPositionSetpoint[2]);
            float DistToPrevZ = fabsf(m_Position[2] - m_PreviousPositionSetpoint[2]);
            float DistToCurrentZ = fabsf(m_CurrentPositionSetpoint[2] - m_Position[2]);

            /* If PosSp has not reached target setpoint (=m_CurrentPositionSetpoint[2]),
             * then compute setpoint depending on vel_max */
            if ((TotalDistZ >  SIGMA_NORM) && (fabsf(m_PositionSetpoint[2] - m_CurrentPositionSetpoint[2]) > SIGMA_NORM))
            {
                float FinalVelZ;
                float TargetThresholdZ;
                osalbool Is2TargetThresholdZ;
                float Slope;
                float MinVelZ;
                float VelSpZ;

                /* Check sign */
                osalbool FlyingUpward;
                if(m_CurrentPositionSetpoint[2] < m_Position[2])
                {
                    FlyingUpward = TRUE;
                }
                else
                {
                    FlyingUpward = FALSE;
                }

                /* Final_vel_z is the max velocity which depends on the distance of TotalDistZ
                 * with default params.vel_max_up/down */
                FinalVelZ = (FlyingUpward) ? ConfigTblPtr->Z_VEL_MAX_UP : ConfigTblPtr->Z_VEL_MAX_DN;

                /* Target threshold defines the distance to m_CurrentPositionSetpoint[2] at which
                 * the vehicle starts to slow down to approach the target smoothly */
                TargetThresholdZ = FinalVelZ * 1.5f;

                /* If the total distance in z is NOT 2x distance of target_threshold, we
                 * will need to adjust the final_vel_z */
                Is2TargetThresholdZ = TotalDistZ >= 2.0f * TargetThresholdZ;
                Slope = (FinalVelZ) / (TargetThresholdZ); /* defines the the acceleration when slowing down */
                MinVelZ = 0.2f; // minimum velocity: this is needed since estimation is not perfect

                if (!Is2TargetThresholdZ)
                {
                    float FinalVelZTmp;

                    /* Adjust final_vel_z since we are already very close
                     * to current and therefore it is not necessary to accelerate
                     * up to full speed (=final_vel_z) */
                    TargetThresholdZ = TotalDistZ * 0.5f;

                    /* Get the velocity at TargetThresholdZ */
                    FinalVelZTmp = Slope * (TargetThresholdZ) + MinVelZ;

                    /* Make sure that final_vel_z is never smaller than 0.5 of the default final_vel_z
                     * this is mainly done because the estimation in z is not perfect and therefore
                     * it is necessary to have a minimum speed */
                    FinalVelZ = math::constrain(FinalVelZTmp, FinalVelZ * 0.5f, FinalVelZ);
                }

                VelSpZ = FinalVelZ;

                /* We want to slow down */
                if (DistToCurrentZ < TargetThresholdZ)
                {
                    VelSpZ = Slope * DistToCurrentZ + MinVelZ;
                }
                else if (DistToPrevZ < TargetThresholdZ)
                {
                    /* We want to accelerate */
                    float AccZ = (VelSpZ - fabsf(m_VelocitySetpoint[2])) / dt;
                    float AccMax = (FlyingUpward) ? (ConfigTblPtr->ACC_UP_MAX * 0.5f) : (ConfigTblPtr->ACC_DOWN_MAX * 0.5f);

                    if (AccZ > AccMax)
                    {
                        VelSpZ = ConfigTblPtr->ACC_UP_MAX * dt + fabsf(m_VelocitySetpoint[2]);
                    }

                }

                /* If we already close to current, then just take over the velocity that
                 * we would have computed if going directly to the current setpoint
                 */
                if (VelSpZ >= (DistToCurrentZ * m_PosP[2]))
                {
                    VelSpZ = DistToCurrentZ * m_PosP[2];
                }

                /* Make sure VelSpZ is always positive */
                VelSpZ = math::constrain(VelSpZ, 0.0f, FinalVelZ);
                /* Get the sign of VelSpZ */
                VelSpZ = (FlyingUpward) ? -VelSpZ : VelSpZ;
                /* Compute PosSp[2] */
                PosSp[2] = m_Position[2] + VelSpZ / m_PosP[2];
            }

            /*
             * XY-DIRECTION
             */

            /* Line from previous to current and from pos to current */
            math::Vector2F VecPrevToCurrent((m_CurrentPositionSetpoint[0] - m_PreviousPositionSetpoint[0]), (m_CurrentPositionSetpoint[1] - m_PreviousPositionSetpoint[1]));
            math::Vector2F VecPosToCurrent((m_CurrentPositionSetpoint[0] - m_Position[0]), (m_CurrentPositionSetpoint[1] - m_Position[1]));


            /* Check if we just want to stay at current position */
            math::Vector2F PosSpDiff((m_CurrentPositionSetpoint[0] - m_PositionSetpoint[0]), (m_CurrentPositionSetpoint[1] - m_PositionSetpoint[1]));
            osalbool StayAtCurrentPos = (m_PositionSetpointTripletMsg.Current.Type == PX4_SETPOINT_TYPE_LOITER
                            || !NextSetpointValid)
                           && ((PosSpDiff.Length()) < SIGMA_NORM);

            /* Only follow line if previous to current has a minimum distance */
            if ((VecPrevToCurrent.Length()  > ConfigTblPtr->NAV_ACC_RAD) && !StayAtCurrentPos)
            {
                float    VelSpAlongTrackPrev;
                float    TargetThresholdXy;
                osalbool CloseToCurrent;
                osalbool CloseToPrev;
                osalbool Is2TargetThreshold;
                osalbool CurrentBehind;
                osalbool PreviousInFront;
                float    VelSpAlongTrack;
                float    VelSpOrthogonal;
                float    CruiseSpMag;

                /* Normalize prev-current line (always > nav_rad) */
                math::Vector2F UnitPrevToCurrent = VecPrevToCurrent.Normalized();

                /* Unit vector from current to next */
                math::Vector2F UnitCurrentToNext(0.0f, 0.0f);

                if (NextSetpointValid)
                {
                    UnitCurrentToNext = math::Vector2F((NextSp[0] - PosSp[0]), (NextSp[1] - PosSp[1]));
                    UnitCurrentToNext = (UnitCurrentToNext.Length() > SIGMA_NORM) ? UnitCurrentToNext.Normalized() :
                                   UnitCurrentToNext;
                }

                /* Point on line closest to pos */
                math::Vector2F ClosestPoint = math::Vector2F(m_PreviousPositionSetpoint[0], m_PreviousPositionSetpoint[1]) + UnitPrevToCurrent *
                                 (math::Vector2F((m_Position[0] - m_PreviousPositionSetpoint[0]), (m_Position[1] - m_PreviousPositionSetpoint[1])) * UnitPrevToCurrent);

                math::Vector2F VecClosestToCurrent((m_CurrentPositionSetpoint[0] - ClosestPoint[0]), (m_CurrentPositionSetpoint[1] - ClosestPoint[1]));

                /* Compute vector from position-current and previous-position */
                math::Vector2F VecPrevToPos((m_Position[0] - m_PreviousPositionSetpoint[0]), (m_Position[1] - m_PreviousPositionSetpoint[1]));

                /* Current velocity along track */
                VelSpAlongTrackPrev = math::Vector2F(m_VelocitySetpoint[0], m_VelocitySetpoint[1]) * UnitPrevToCurrent;

                /* Distance to target when brake should occur */
                TargetThresholdXy = 1.5f * GetCruisingSpeedXY();

                CloseToCurrent = VecPosToCurrent.Length() < TargetThresholdXy;
                CloseToPrev = (VecPrevToPos.Length() < TargetThresholdXy) &&
                             (VecPrevToPos.Length() < VecPosToCurrent.Length());

                /* Indicates if we are at least half the distance from previous to current close to previous */
                Is2TargetThreshold = VecPrevToCurrent.Length() >= 2.0f * TargetThresholdXy;

                /* Check if the current setpoint is behind */
                CurrentBehind = ((VecPosToCurrent * -1.0f) * UnitPrevToCurrent) > 0.0f;

                /* Check if the previous is in front */
                PreviousInFront = (VecPrevToPos * UnitPrevToCurrent) < 0.0f;
                
                /* Default velocity along line prev-current */
                VelSpAlongTrack = GetCruisingSpeedXY();

                /*
                 * Compute velocity setpoint along track
                 */

                /* Only go directly to previous setpoint if more than 5m away and previous in front*/
                if (PreviousInFront && (VecPrevToPos.Length() > 1.0f))
                {
                    /* Just use the default velocity along track */
                    VelSpAlongTrack = VecPrevToPos.Length() * m_PosP[0];

                    if (VelSpAlongTrack > GetCruisingSpeedXY())
                    {
                        VelSpAlongTrack = GetCruisingSpeedXY();
                    }

                }
                else if (CurrentBehind)
                {
                    /* Go directly to current setpoint */
                    VelSpAlongTrack = VecPosToCurrent.Length() * m_PosP[0];
                    VelSpAlongTrack = (VelSpAlongTrack < GetCruisingSpeedXY()) ? VelSpAlongTrack : GetCruisingSpeedXY();

                }
                else if (CloseToPrev)
                {
                    float acc_track;
                    float acc;

                    /* Accelerate from previous setpoint towards current setpoint */

                    /* We are close to previous and current setpoint
                     * we first compute the start velocity when close to current septoint and use
                     * this velocity as final velocity when transition occurs from acceleration to deceleration.
                     * This ensures smooth transition */
                    float FinalCruiseSpeed = GetCruisingSpeedXY();

                    if (!Is2TargetThreshold)
                    {
                        /* Velocity close to current setpoint with default zero if no next setpoint is available */
                        float vel_close = 0.0f;
                        float acceptance_radius = 0.0f;

                        /* Set target threshold to half dist pre-current */
                        float TargetThresholdTmp = TargetThresholdXy;
                        TargetThresholdXy = VecPrevToCurrent.Length() * 0.5f;

                        if ((TargetThresholdXy - ConfigTblPtr->NAV_ACC_RAD) < SIGMA_NORM)
                        {
                            TargetThresholdXy = ConfigTblPtr->NAV_ACC_RAD;
                        }

                        /* We want to pass and need to compute the desired velocity close to current setpoint */
                        if (NextSetpointValid &&  !(m_PositionSetpointTripletMsg.Current.Type == PX4_SETPOINT_TYPE_LOITER))
                        {
                            /* Get velocity close to current that depends on angle between prev-current and current-next line */
                            vel_close = GetVelClose(UnitPrevToCurrent, UnitCurrentToNext);
                            acceptance_radius = ConfigTblPtr->NAV_ACC_RAD;
                        }

                        /* Compute velocity at transition where vehicle switches from acceleration to deceleration */
                        if ((TargetThresholdTmp - acceptance_radius) < SIGMA_NORM)
                        {
                            FinalCruiseSpeed = vel_close;
                        }
                        else
                        {
                            float slope = (GetCruisingSpeedXY() - vel_close) / (TargetThresholdTmp - acceptance_radius);
                            FinalCruiseSpeed = slope  * (TargetThresholdXy - acceptance_radius) + vel_close;
                            FinalCruiseSpeed = (FinalCruiseSpeed > vel_close) ? FinalCruiseSpeed : vel_close;
                        }
                    }

                    /* Make sure final cruise speed is larger than 0*/
                    FinalCruiseSpeed = (FinalCruiseSpeed > SIGMA_NORM) ? FinalCruiseSpeed : SIGMA_NORM;
                    VelSpAlongTrack = FinalCruiseSpeed;

                    /* We want to accelerate not too fast */
                    acc_track = (FinalCruiseSpeed - VelSpAlongTrackPrev) / dt;

                    /* If yaw offset is large, only accelerate with 0.5m/s^2 */
                    acc = (fabsf(YawDiff) >  math::radians(ConfigTblPtr->NAV_MIS_YAW_ERR)) ? 0.5f : ConfigTblPtr->ACC_HOR_MAX;

                    if (acc_track > acc)
                    {
                        VelSpAlongTrack = acc * dt + VelSpAlongTrackPrev;
                    }

                    /* Enforce minimum cruise speed */
                    VelSpAlongTrack  = math::constrain(VelSpAlongTrack, SIGMA_NORM, FinalCruiseSpeed);
                }
                else if (CloseToCurrent)
                {
                    /* Slow down when close to current setpoint */

                    /* Check if altidue is within acceptance radius */
                    osalbool reached_altitude = (DistToCurrentZ < ConfigTblPtr->NAV_ACC_RAD) ? TRUE : FALSE;

                    if (reached_altitude && NextSetpointValid
                        && !(m_PositionSetpointTripletMsg.Current.Type == PX4_SETPOINT_TYPE_LOITER))
                    {
                        /* Since we have a next setpoint use the angle prev-current-next to compute velocity setpoint limit */

                        /* Get velocity close to current that depends on angle between prev-current and current-next line */
                        float VelClose = GetVelClose(UnitPrevToCurrent, UnitCurrentToNext);

                        /* Compute velocity along line which depends on distance to current setpoint */
                        if (VecClosestToCurrent.Length() < ConfigTblPtr->NAV_ACC_RAD)
                        {
                            VelSpAlongTrack = VelClose;
                        }
                        else
                        {
                            if (TargetThresholdXy - ConfigTblPtr->NAV_ACC_RAD < SIGMA_NORM)
                            {
                                VelSpAlongTrack = VelClose;
                            }
                            else
                            {
                                float slope = (GetCruisingSpeedXY() - VelClose) / (TargetThresholdXy - ConfigTblPtr->NAV_ACC_RAD) ;
                                VelSpAlongTrack = slope  * (VecClosestToCurrent.Length() - ConfigTblPtr->NAV_ACC_RAD) + VelClose;
                            }
                        }

                        /* Since we want to slow down take over previous velocity setpoint along track if it was lower */
                        if ((VelSpAlongTrackPrev < VelSpAlongTrack) && (VelSpAlongTrack * VelSpAlongTrackPrev > 0.0f))
                        {
                            VelSpAlongTrack = VelSpAlongTrackPrev;
                        }

                        /* If we are close to target and the previous velocity setpoints was smaller than
                         * VelSpAlongTrack, then take over the previous one
                         * this ensures smoothness since we anyway want to slow down
                         */
                        if ((VelSpAlongTrackPrev < VelSpAlongTrack) && (VelSpAlongTrack * VelSpAlongTrackPrev > 0.0f)
                            && (VelSpAlongTrackPrev > VelClose))
                        {
                            VelSpAlongTrack = VelSpAlongTrackPrev;
                        }

                        /* Make sure that vel_sp_along track is at least min */
                        VelSpAlongTrack = (VelSpAlongTrack < VelClose) ? VelClose : VelSpAlongTrack;


                    }
                    else
                    {
                        /* We want to stop at current setpoint */
                        float slope = (GetCruisingSpeedXY())  / TargetThresholdXy;
                        VelSpAlongTrack =  slope * (VecClosestToCurrent.Length());

                        /* Since we want to slow down take over previous velocity setpoint along track if it was lower but ensure its not zero */
                        if ((VelSpAlongTrackPrev < VelSpAlongTrack) && (VelSpAlongTrack * VelSpAlongTrackPrev > 0.0f)
                            && (VelSpAlongTrackPrev > 0.5f))
                        {
                            VelSpAlongTrack = VelSpAlongTrackPrev;
                        }
                    }
                }

                /* Compute velocity orthogonal to prev-current-line to position*/
                math::Vector2F VecPosToClosest = ClosestPoint - math::Vector2F(m_Position[0], m_Position[1]);
                VelSpOrthogonal = VecPosToClosest.Length() * m_PosP[0];

                /* Compute the cruise speed from velocity along line and orthogonal velocity setpoint */
                CruiseSpMag = sqrtf(VelSpOrthogonal * VelSpOrthogonal + VelSpAlongTrack * VelSpAlongTrack);

                /* Sanity check */
                CruiseSpMag = (isfinite(CruiseSpMag)) ? CruiseSpMag : VelSpOrthogonal;

                /* Orthogonal velocity setpoint is smaller than cruise speed */
                if (VelSpOrthogonal < GetCruisingSpeedXY() && !CurrentBehind)
                {
                    /* We need to limit VelSpAlongTrack such that cruise speed  is never exceeded but still can keep velocity orthogonal to track */
                    if (CruiseSpMag > GetCruisingSpeedXY())
                    {
                        VelSpAlongTrack = sqrtf(GetCruisingSpeedXY() * GetCruisingSpeedXY() - VelSpOrthogonal * VelSpOrthogonal);
                    }

                    PosSp[0] = ClosestPoint[0] + UnitPrevToCurrent[0] * VelSpAlongTrack / m_PosP[0];
                    PosSp[1] = ClosestPoint[1] + UnitPrevToCurrent[1] * VelSpAlongTrack / m_PosP[1];

                }
                else if (CurrentBehind)
                {
                    /* Current is behind */
                    if (VecPosToCurrent.Length()  > 0.01f)
                    {
                        PosSp[0] = m_Position[0] + VecPosToCurrent[0] / VecPosToCurrent.Length() * VelSpAlongTrack / m_PosP[0];
                        PosSp[1] = m_Position[1] + VecPosToCurrent[1] / VecPosToCurrent.Length() * VelSpAlongTrack / m_PosP[1];
                    }
                    else
                    {
                        PosSp[0] = m_CurrentPositionSetpoint[0];
                        PosSp[1] = m_CurrentPositionSetpoint[1];
                    }

                }
                else
                {
                    float cruise_sp;

                    /* We are more than cruise_speed away from track */

                    /* If previous is in front just go directly to previous point */
                    if (PreviousInFront)
                    {
                        VecPosToClosest[0] = m_PreviousPositionSetpoint[0] - m_Position[0];
                        VecPosToClosest[1] = m_PreviousPositionSetpoint[1] - m_Position[1];
                    }

                    /* Make sure that we never exceed maximum cruise speed */
                    cruise_sp = VecPosToClosest.Length() * m_PosP[0];

                    if (cruise_sp > GetCruisingSpeedXY())
                    {
                        cruise_sp = GetCruisingSpeedXY();
                    }

                    /* Sanity check: don't divide by zero */
                    if (VecPosToClosest.Length() > SIGMA_NORM)
                    {
                        PosSp[0] = m_Position[0] + VecPosToClosest[0] / VecPosToClosest.Length() * cruise_sp / m_PosP[0];
                        PosSp[1] = m_Position[1] + VecPosToClosest[1] / VecPosToClosest.Length() * cruise_sp / m_PosP[1];
                    }
                    else
                    {
                        PosSp[0] = ClosestPoint[0];
                        PosSp[1] = ClosestPoint[1];
                    }
                    
                }
            }

            m_PositionSetpoint = PosSp;

        }
        else if (m_PositionSetpointTripletMsg.Current.Type == PX4_SETPOINT_TYPE_VELOCITY)
        {
            float VelXyMag = sqrtf(m_Velocity[0] * m_Velocity[0] + m_Velocity[1] * m_Velocity[1]);

            if (VelXyMag > SIGMA_NORM)
            {
                m_VelocitySetpoint[0] = m_Velocity[0] / VelXyMag * GetCruisingSpeedXY();
                m_VelocitySetpoint[1] = m_Velocity[1] / VelXyMag * GetCruisingSpeedXY();
            }
            else
            {
                m_VelocitySetpoint[0] = 0.0f;
                m_VelocitySetpoint[1] = 0.0f;
            }

            m_RunPosControl = FALSE;

        }
        else
        {
            /* Just go to the target point */;
            m_PositionSetpoint = m_CurrentPositionSetpoint;

            /* Set max velocity to cruise */
            m_VelMaxXy = GetCruisingSpeedXY();
        }

        /* Sanity check */
        if (!(isfinite(m_PositionSetpoint[0]) && isfinite(m_PositionSetpoint[1]) &&
              isfinite(m_PositionSetpoint[2])))
        {
            //warn_rate_limited("Auto: Position setpoint not finite");
            m_PositionSetpoint = m_CurrentPositionSetpoint;
            (void) CFE_EVS_SendEvent(MPC_POS_SP_NOT_FINITE_ERR_EID, CFE_EVS_ERROR,
                    "Auto: Position setpoint not finite.");
        }


        /* If we're already near the current takeoff setpoint don't reset in case we switch back to posctl.
         * this makes the takeoff finish smoothly. */
        if ((m_PositionSetpointTripletMsg.Current.Type == PX4_SETPOINT_TYPE_TAKEOFF
             || m_PositionSetpointTripletMsg.Current.Type == PX4_SETPOINT_TYPE_LOITER)
            && m_PositionSetpointTripletMsg.Current.AcceptanceRadius > 0.0f
            /* Need to detect we're close a bit before the navigator switches from takeoff to next waypoint */
            && (m_Position - m_PositionSetpoint).Length() < m_PositionSetpointTripletMsg.Current.AcceptanceRadius * 1.2f)
        {
            m_DoResetAltPos = FALSE;
        }
        else
        {
            /* Otherwise: in case of interrupted mission don't go to waypoint but stay at current position */
            m_DoResetAltPos = TRUE;
        }
    }
    else
    {
        /* Idle or triplet not valid, set velocity setpoint to zero */
        m_VelocitySetpoint.Zero();
        m_RunPosControl = FALSE;
        m_RunAltControl = FALSE;
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* CalculateVelocitySetpoint                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::CalculateVelocitySetpoint(float dt)
{
    float LandVelLimit;
    float VelNormXy;

    /* Run position & altitude controllers if enabled (otherwise use already
     * computed velocity setpoints) */
    if(m_RunPosControl)
    {
        /* If for any reason, we get a NaN position setpoint, we better just
         * stay where we are. */
        if(isfinite(m_PositionSetpoint[0]) && isfinite(m_PositionSetpoint[1]))
        {
            m_VelocitySetpoint[0] = (m_PositionSetpoint[0] - m_Position[0]) * m_PosP[0];
            m_VelocitySetpoint[1] = (m_PositionSetpoint[1] - m_Position[1]) * m_PosP[1];
        }
        else
        {
            m_VelocitySetpoint[0] = 0.0f;
            m_VelocitySetpoint[1] = 0.0f;
        }
    }

    /* In auto the setpoint is already limited by the navigator */
    if (!m_VehicleControlModeMsg.ControlAutoEnabled)
    {
        LimitAltitude();
    }

    if (m_RunAltControl)
    {
        if(isfinite(m_PositionSetpoint[2]))
        {
            m_VelocitySetpoint[2] = (m_PositionSetpoint[2] - m_Position[2]) * m_PosP[2];
        }
        else
        {
            m_VelocitySetpoint[2] = 0.0f;
        }
    }

    if (!m_VehicleControlModeMsg.ControlPositionEnabled)
    {
        m_ResetPositionSetpoint = TRUE;
    }

    if (!m_VehicleControlModeMsg.ControlAltitudeEnabled)
    {
        m_ResetAltitudeSetpoint = TRUE;
    }

    if (!m_VehicleControlModeMsg.ControlVelocityEnabled)
    {
        m_VelocitySetpointPrevious[0] = m_Velocity[0];
        m_VelocitySetpointPrevious[1] = m_Velocity[1];
        m_VelocitySetpoint[0] = 0.0f;
        m_VelocitySetpoint[1] = 0.0f;
    }

    if (!m_VehicleControlModeMsg.ControlClimbRateEnabled)
    {
        m_VelocitySetpoint[2] = 0.0f;
    }

    /* Limit vertical upwards speed in auto takeoff and close to ground */
    float AltitudeAboveHome = -m_Position[2] + m_HomePositionMsg.Z;

    if (m_PositionSetpointTripletMsg.Current.Valid
        && m_PositionSetpointTripletMsg.Current.Type == PX4_SETPOINT_TYPE_TAKEOFF
        && !m_VehicleControlModeMsg.ControlManualEnabled)
    {
        float TkVelLimit = math::gradual(AltitudeAboveHome,
                                        ConfigTblPtr->LAND_ALT2, ConfigTblPtr->LAND_ALT1,
                                        ConfigTblPtr->TKO_SPEED, ConfigTblPtr->Z_VEL_MAX_UP);
        m_VelocitySetpoint[2] = math::max(m_VelocitySetpoint[2], -TkVelLimit);
    }

    /* Limit vertical downwards speed (positive z) close to ground. For now we use the
     * altitude above home and assume that we want to land at same height as we took off */
    LandVelLimit = math::gradual(AltitudeAboveHome,
            ConfigTblPtr->LAND_ALT2, ConfigTblPtr->LAND_ALT1,
            ConfigTblPtr->LAND_SPEED, ConfigTblPtr->Z_VEL_MAX_DN);

    m_VelocitySetpoint[2] = math::min(m_VelocitySetpoint[2], LandVelLimit);


    /* Apply slew rate (aka acceleration limit) for smooth flying. */
    if (!m_VehicleControlModeMsg.ControlAutoEnabled && !m_InTakeoff)
    {
        ApplyVelocitySetpointSlewRate(dt);
    }

    /* Special velocity setpoint limitation for smooth takeoff. */
    if (m_InTakeoff)
    {
        m_InTakeoff = m_TakeoffVelLimit < -m_VelocitySetpoint[2];
        /* Ramp vertical velocity limit up to takeoff speed. */
        m_TakeoffVelLimit += -m_VelocitySetpoint[2] * dt / ConfigTblPtr->TKO_RAMP_T;
        /* Limit vertical velocity to the current ramp value. */
        m_VelocitySetpoint[2] = math::max(m_VelocitySetpoint[2], -m_TakeoffVelLimit);

        /* Do not control XY in takeoff */
        m_VelocitySetpoint[0] = 0.0f;
        m_VelocitySetpoint[1] = 0.0f;
        
        m_PositionSetpoint[0] = NAN;
        m_PositionSetpoint[1] = NAN;
    }

    /* Make sure velocity setpoint is constrained in all directions. */
    VelNormXy = sqrtf(m_VelocitySetpoint[0] * m_VelocitySetpoint[0] + m_VelocitySetpoint[1] * m_VelocitySetpoint[1]);
    if (VelNormXy > ConfigTblPtr->XY_VEL_MAX)
    {
        m_VelocitySetpoint[0] = m_VelocitySetpoint[0] * ConfigTblPtr->XY_VEL_MAX / VelNormXy;
        m_VelocitySetpoint[1] = m_VelocitySetpoint[1] * ConfigTblPtr->XY_VEL_MAX / VelNormXy;
    }

    m_VelocitySetpoint[2] = math::constrain(m_VelocitySetpoint[2], -ConfigTblPtr->Z_VEL_MAX_UP, ConfigTblPtr->Z_VEL_MAX_DN);

    m_VelocitySetpointPrevious = m_VelocitySetpoint;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* CalculateThrustSetpoint                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::CalculateThrustSetpoint(float dt)
{
    osalbool SaturationXy = FALSE;
    osalbool SaturationZ  = FALSE;
    float    ThrMin       = ConfigTblPtr->THR_MIN;
    float    TiltMax;
    float    ThrMax;
    float    ThrustBodyZ;

    /* Reset integrals if needed. */
    if (m_VehicleControlModeMsg.ControlClimbRateEnabled)
    {
        if (m_ResetIntZ)
        {
            m_ResetIntZ = FALSE;
            m_ThrustInt[2] = 0.0f;
        }
    }
    else
    {
        m_ResetIntZ = TRUE;
    }

    if (m_VehicleControlModeMsg.ControlVelocityEnabled)
    {
        if (m_ResetIntXY)
        {
            m_ResetIntXY = FALSE;
            m_ThrustInt[0] = 0.0f;
            m_ThrustInt[1] = 0.0f;
        }
    }
    else
    {
        m_ResetIntXY = TRUE;
    }

    /* If any of the velocity setpoint is bogus, it's probably safest to command no velocity at all. */
    for (int i = 0; i < 3; ++i)
    {
        if (!isfinite(m_VelocitySetpoint[i]))
        {
            m_VelocitySetpoint[i] = 0.0f;
        }
    }

    /* Velocity error */
    math::Vector3F VelErr = m_VelocitySetpoint - m_Velocity;

    /* Thrust vector in NED frame. */
    math::Vector3F ThrustSp(0.0f, 0.0f, 0.0f);

    if (m_VehicleControlModeMsg.ControlAccelerationEnabled && m_PositionSetpointTripletMsg.Current.AccelerationValid)
    {
        ThrustSp = math::Vector3F(m_PositionSetpointTripletMsg.Current.AX, m_PositionSetpointTripletMsg.Current.AY, m_PositionSetpointTripletMsg.Current.AZ);
    }
    else
    {
        ThrustSp = VelErr.EMult(m_VelP) + m_VelocityErrD.EMult(m_VelD)
                + m_ThrustInt - math::Vector3F(0.0f, 0.0f, ConfigTblPtr->THR_HOVER);
    }

    if (!m_VehicleControlModeMsg.ControlVelocityEnabled && !m_VehicleControlModeMsg.ControlAccelerationEnabled)
    {
        ThrustSp[0] = 0.0f;
        ThrustSp[1] = 0.0f;
    }

    /* If still or already on ground command zero xy velocity and zero xy
     * ThrustSp in body frame to consider uneven ground. */
    if ((m_VehicleLandDetectedMsg.GroundContact || m_VehicleLandDetectedMsg.Landed) && !InAutoTakeoff() && !ManualWantsTakeoff())
    {
        /* Thrust setpoint in body frame*/
        math::Vector3F ThrustSpBody = m_Rotation.Transpose() * ThrustSp;

        /* We dont want to make any correction in body x and y*/
        ThrustSpBody[0] = 0.0f;
        ThrustSpBody[1] = 0.0f;

        /* Make sure z component of ThrustSpBody is larger than 0 (positive thrust is downward) */
        ThrustSpBody[2] = ThrustSp[2] > 0.0f ? ThrustSp[2] : 0.0f;

        /* Convert back to local frame (NED) */
        ThrustSp = m_Rotation * ThrustSpBody;
    }

    if (!m_VehicleControlModeMsg.ControlClimbRateEnabled && !m_VehicleControlModeMsg.ControlAccelerationEnabled)
    {
        ThrustSp[2] = 0.0f;
    }

    /* Limit thrust vector and check for saturation. */

    if (!m_VehicleControlModeMsg.ControlVelocityEnabled && ThrMin < 0.0f)
    {
        /* Don't allow downside thrust direction in manual attitude mode. */
        ThrMin = 0.0f;
    }

    TiltMax = math::radians(ConfigTblPtr->TILTMAX_AIR);
    ThrMax = ConfigTblPtr->THR_MAX;

    /* We can only run the control if we're already in-air, have a takeoff setpoint,
     * or if we're in offboard control.  Otherwise, we should just bail out. */
    if (m_VehicleLandDetectedMsg.Landed && !InAutoTakeoff() && !ManualWantsTakeoff())
    {
        /* Keep throttle low while still on ground. */
        ThrMax = 0.0f;
    }
    else if (!m_VehicleControlModeMsg.ControlManualEnabled && m_PositionSetpointTripletMsg.Current.Valid &&
            m_PositionSetpointTripletMsg.Current.Type == PX4_SETPOINT_TYPE_LAND)
    {
        /* Adjust limits for landing mode.  Limit max tilt and min lift when landing. */
        TiltMax = math::radians(ConfigTblPtr->TILTMAX_LND);
    }

    /* Limit min lift */
    if (-ThrustSp[2] < ThrMin)
    {
        ThrustSp[2] = -ThrMin;

        /* Don't freeze altitude integral if it wants to throttle up. */
        SaturationZ = VelErr[2] > 0.0f ? TRUE : SaturationZ;
    }

    if (m_VehicleControlModeMsg.ControlVelocityEnabled || m_VehicleControlModeMsg.ControlAccelerationEnabled)
    {
        /* Limit max tilt */
        if (ThrMin >= 0.0f && TiltMax < M_PI / 2.0f - 0.05f)
        {
            /* Absolute horizontal thrust */
            float ThrustSpXyLen = math::Vector2F(ThrustSp[0], ThrustSp[1]).Length();
            if (ThrustSpXyLen > 0.01f)
            {
                /* Max horizontal thrust for given vertical thrust. */
                float ThrustXyMax = -ThrustSp[2] * tanf(TiltMax);
                if (ThrustSpXyLen > ThrustXyMax)
                {
                    float k = ThrustXyMax / ThrustSpXyLen;
                    ThrustSp[0] *= k;
                    ThrustSp[1] *= k;

                    /* Don't freeze x,y integrals if they both want to throttle down. */
                    SaturationXy = ((VelErr[0] * m_VelocitySetpoint[0] < 0.0f) && (VelErr[1] * m_VelocitySetpoint[1] < 0.0f)) ? SaturationXy : TRUE;
                }
            }
        }
    }

    if (m_VehicleControlModeMsg.ControlClimbRateEnabled && !m_VehicleControlModeMsg.ControlVelocityEnabled)
    {
        /* Thrust compensation when vertical velocity but not horizontal velocity is controlled. */
        float AttComp;

        const float TiltCosMax = MPC_TILT_COS_MAX;

        if (m_Rotation[2][2] > TiltCosMax)
        {
            AttComp = 1.0f / m_Rotation[2][2];
        }
        else if (m_Rotation[2][2] > 0.0f)
        {
            AttComp = ((1.0f / TiltCosMax - 1.0f) / TiltCosMax) * m_Rotation[2][2] + 1.0f;
            SaturationZ = TRUE;
        }
        else
        {
            AttComp = 1.0f;
            SaturationZ = TRUE;
        }

        ThrustSp[2] *= AttComp;
    }

    /* Calculate desired total thrust amount in body z direction. */
    /* To compensate for excess thrust during attitude tracking errors we
     * project the desired thrust force vector F onto the real vehicle's thrust axis in NED:
     * body thrust axis [0,0,-1]' rotated by R is: R*[0,0,-1]' = -R_z */
    math::Vector3F R_z(m_Rotation[0][2], m_Rotation[1][2], m_Rotation[2][2]);
    math::Vector3F F(ThrustSp);

    /* Recalculate because it might have changed. */
    ThrustBodyZ = F * -R_z;

    /* Limit max thrust. */
    if (fabsf(ThrustBodyZ) > ThrMax)
    {
        if (ThrustSp[2] < 0.0f)
        {
            if (-ThrustSp[2] > ThrMax)
            {
                /* Thrust Z component is too large, limit it. */
                ThrustSp[0] = 0.0f;
                ThrustSp[1] = 0.0f;
                ThrustSp[2] = -ThrMax;
                SaturationXy = TRUE;

                /* Don't freeze altitude integral if it wants to throttle down. */
                SaturationZ = VelErr[2] < 0.0f ? TRUE : SaturationZ;
            }
            else
            {
                /* Preserve thrust Z component and lower XY, keeping altitude is more important than position. */
                float ThrustXyMax = sqrtf(ThrMax * ThrMax - ThrustSp[2] * ThrustSp[2]);
                float ThrustXyAbs = math::Vector2F(ThrustSp[0], ThrustSp[1]).Length();
                float k = ThrustXyMax / ThrustXyAbs;
                ThrustSp[0] *= k;
                ThrustSp[1] *= k;
                /* Don't freeze x,y integrals if they both want to throttle down */
                SaturationXy = ((VelErr[0] * m_VelocitySetpoint[0] < 0.0f) && (VelErr[1] * m_VelocitySetpoint[1] < 0.0f)) ? SaturationXy : TRUE;
            }
        }
        else
        {
            /* Z component is positive, going down (Z is positive down in NED), simply limit thrust vector. */
            float k = ThrMax / fabsf(ThrustBodyZ);
            ThrustSp = ThrustSp * k;
            SaturationXy = TRUE;
            SaturationZ = TRUE;
        }

        ThrustBodyZ = ThrMax;
    }

    /* Allow for zero thrust while landed, but not in flight */
    if(m_VehicleLandDetectedMsg.Landed || m_InTakeoff)
    {        
        m_VehicleAttitudeSetpointMsg.Thrust = math::max(ThrustBodyZ, 0.0f);
    }
    else
    {
        m_VehicleAttitudeSetpointMsg.Thrust = math::max(ThrustBodyZ, ThrMin);
    }

    /* Update integrals */
    if (m_VehicleControlModeMsg.ControlVelocityEnabled && !SaturationXy)
    {
        m_ThrustInt[0] += VelErr[0] * m_VelI[0] * dt;
        m_ThrustInt[1] += VelErr[1] * m_VelI[1] * dt;
    }

    if (m_VehicleControlModeMsg.ControlClimbRateEnabled && !SaturationZ)
    {
        m_ThrustInt[2] += VelErr[2] * m_VelI[2] * dt;
    }

    /* Calculate attitude setpoint from thrust vector. */
    if (m_VehicleControlModeMsg.ControlVelocityEnabled || m_VehicleControlModeMsg.ControlAccelerationEnabled)
    {
        /* Desired BodyZ axis = -normalize(thrust_vector) */
        math::Vector3F BodyX(0.0f, 0.0f, 0.0f);
        math::Vector3F BodyY(0.0f, 0.0f, 0.0f);
        math::Vector3F BodyZ(0.0f, 0.0f, 0.0f);

        if (ThrustSp.Length() > SIGMA_NORM)
        {
            BodyZ = -ThrustSp.Normalized();
        }
        else
        {
            /* No thrust, set Z axis to safe value. */
            BodyZ.Zero();
            BodyZ[2] = 1.0f;
        }

        /* Vector of desired yaw direction in XY plane, rotated by PI/2. */
        math::Vector3F y_C(-sinf(m_VehicleAttitudeSetpointMsg.YawBody), cosf(m_VehicleAttitudeSetpointMsg.YawBody), 0.0f);

        if (fabsf(BodyZ[2]) > SIGMA_SINGLE_OP)
        {
            /* Desired BodyX axis, orthogonal to BodyZ. */
            BodyX = y_C % BodyZ;

            /* Keep nose to front while inverted upside down. */
            if (BodyZ[2] < 0.0f)
            {
                BodyX = -BodyX;
            }

            BodyX.Normalize();
        }
        else
        {
            /* Desired thrust is in XY plane, set X downside to construct
             * correct matrix, but yaw component will not be used actually */
            BodyX.Zero();
            BodyX[2] = 1.0f;
        }

        /* Desired BodyY axis */
        BodyY = BodyZ % BodyX;

        /* Fill rotation matrix */
        for (uint32 i = 0; i < 3; i++)
        {
            m_RSetpoint[i][0] = BodyX[i];
            m_RSetpoint[i][1] = BodyY[i];
            m_RSetpoint[i][2] = BodyZ[i];
        }

        /* Copy quaternion setpoint to attitude setpoint topic. */
        math::Quaternion q_sp(m_RSetpoint);
        q_sp.copyTo(m_VehicleAttitudeSetpointMsg.Q_D);
        m_VehicleAttitudeSetpointMsg.Q_D_Valid = TRUE;

        /* Calculate euler angles, for logging only.  Must not be used for
         * control. */
        math::Vector3F euler = m_RSetpoint.ToEuler();
        m_VehicleAttitudeSetpointMsg.RollBody = euler[0];
        m_VehicleAttitudeSetpointMsg.PitchBody = euler[1];
        /* Yaw already used to construct rot matrix, but actual rotation
         * matrix can have different yaw near singularity. */
    }
    else if (!m_VehicleControlModeMsg.ControlManualEnabled)
    {
        /* Autonomous altitude control without position control (failsafe
         * landing).  Force level attitude, don't change yaw. */
        m_RSetpoint = math::Matrix3F3::FromEuler(0.0f, 0.0f, m_VehicleAttitudeSetpointMsg.YawBody);

        /* Copy quaternion setpoint to attitude setpoint topic. */
        math::Quaternion q_sp(m_RSetpoint);
        q_sp.copyTo(m_VehicleAttitudeSetpointMsg.Q_D);
        m_VehicleAttitudeSetpointMsg.Q_D_Valid = TRUE;

        m_VehicleAttitudeSetpointMsg.RollBody = 0.0f;
        m_VehicleAttitudeSetpointMsg.PitchBody = 0.0f;
    }

    /* Save thrust setpoint for logging. */
    m_VehicleLocalPositionSetpointMsg.AccX = ThrustSp[0] * MPC_CONSTANTS_ONE_G;
    m_VehicleLocalPositionSetpointMsg.AccY = ThrustSp[1] * MPC_CONSTANTS_ONE_G;
    m_VehicleLocalPositionSetpointMsg.AccZ = ThrustSp[2] * MPC_CONSTANTS_ONE_G;

    m_VehicleAttitudeSetpointMsg.Timestamp = PX4LIB_GetPX4TimeUs();
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* GetCruisingSpeedXY                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
float MPC::GetCruisingSpeedXY(void)
{
    float Result = 0.0f;

    /* In missions, the user can choose cruising speed different to default. */
    Result = ((isfinite(m_PositionSetpointTripletMsg.Current.CruisingSpeed) &&
            !(m_PositionSetpointTripletMsg.Current.CruisingSpeed < SIGMA_NORM)) ?
                    m_PositionSetpointTripletMsg.Current.CruisingSpeed : ConfigTblPtr->XY_CRUISE);

    return Result;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* UpdateXyPids                                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool MPC::UpdateXyPids(MPC_SetPidCmd_t* PidMsg)
{
    osalbool success = TRUE;

    if((MPC::Validate_XY_P(PidMsg->PidGain) == FALSE) ||
       (MPC::Validate_XY_VEL_P(PidMsg->PidVelP) == FALSE) ||
       (MPC::Validate_XY_VEL_I(PidMsg->PidVelI) == FALSE) ||
       (MPC::Validate_XY_VEL_D(PidMsg->PidVelD) == FALSE))
    {
        success = FALSE;
    }
    else
    {
        /* Update values in Vector3F copies used for math operations */
        /* X Index */
        m_PosP[0] = PidMsg->PidGain;
        m_VelP[0] = PidMsg->PidVelP;
        m_VelI[0] = PidMsg->PidVelI;
        m_VelD[0] = PidMsg->PidVelD;

        /* Y Index */
        m_PosP[1] = PidMsg->PidGain;
        m_VelP[1] = PidMsg->PidVelP;
        m_VelI[1] = PidMsg->PidVelI;
        m_VelD[1] = PidMsg->PidVelD;;

        /* Update in table */
        ConfigTblPtr->XY_P = PidMsg->PidGain;
        ConfigTblPtr->XY_VEL_P = PidMsg->PidVelP;
        ConfigTblPtr->XY_VEL_I = PidMsg->PidVelI;
        ConfigTblPtr->XY_VEL_D = PidMsg->PidVelD;

        CFE_TBL_Modified(ConfigTblHdl);
    }
    
    return success;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* UpdateZPids                                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool MPC::UpdateZPids(MPC_SetPidCmd_t* PidMsg)
{
    osalbool success = TRUE;

    if((MPC::Validate_Z_P(PidMsg->PidGain) == FALSE) ||
       (MPC::Validate_Z_VEL_P(PidMsg->PidVelP) == FALSE) ||
       (MPC::Validate_Z_VEL_I(PidMsg->PidVelI) == FALSE) ||
       (MPC::Validate_Z_VEL_D(PidMsg->PidVelD) == FALSE))
    {
        success = FALSE;
    }
    else
    {
        /* Update values in Vector3F copy used for math operations */
        /* Z Index */
        m_PosP[2] = PidMsg->PidGain;
        m_VelP[2] = PidMsg->PidVelP;
        m_VelI[2] = PidMsg->PidVelI;
        m_VelD[2] = PidMsg->PidVelD;

        /* Update in table */
        ConfigTblPtr->Z_P = PidMsg->PidGain;
        ConfigTblPtr->Z_VEL_P = PidMsg->PidVelP;
        ConfigTblPtr->Z_VEL_I = PidMsg->PidVelI;
        ConfigTblPtr->Z_VEL_D = PidMsg->PidVelD;

        CFE_TBL_Modified(ConfigTblHdl);
    }

    return success;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* UpdateHoldDz                                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool MPC::UpdateHoldDz(MPC_SetDzCmd_t* DzMsg)
{
    osalbool success = TRUE;

    if(MPC::Validate_HOLD_DZ(DzMsg->Deadzone) == FALSE)
    {
        success = FALSE;
    }
    else
    {
        ConfigTblPtr->HOLD_DZ = DzMsg->Deadzone;

        CFE_TBL_Modified(ConfigTblHdl);
    }

    return success;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* UpdateStickExpo                                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool MPC::UpdateStickExpo(MPC_SetStickExpoCmd_t* ExpoMsg)
{
    osalbool success = TRUE;

    if((MPC::Validate_XY_MAN_EXPO(ExpoMsg->XY) == FALSE) ||
       (MPC::Validate_Z_MAN_EXPO(ExpoMsg->Z) == FALSE))
    {
        success = FALSE;
    }
    else
    {
        ConfigTblPtr->XY_MAN_EXPO = ExpoMsg->XY;
        ConfigTblPtr->Z_MAN_EXPO = ExpoMsg->Z;

        CFE_TBL_Modified(ConfigTblHdl);
    }

    return success;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* UpdateTakeoffRampTime                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool MPC::UpdateTakeoffRampTime(MPC_SetTkoRampCmd_t* TkoRampMsg)
{
    osalbool success = TRUE;

    if(MPC::Validate_TKO_RAMP_T(TkoRampMsg->TKO_RAMP_T) == FALSE)
    {
        success = FALSE;
    }
    else
    {
        ConfigTblPtr->TKO_RAMP_T = TkoRampMsg->TKO_RAMP_T;
        CFE_TBL_Modified(ConfigTblHdl);
    }

    return success;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* UpdateParamsFromTable                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::UpdateParamsFromTable(void)
{
    if(ConfigTblPtr != 0)
    {
        m_PosP[0] = ConfigTblPtr->XY_P;
        m_PosP[1] = ConfigTblPtr->XY_P;
        m_PosP[2] = ConfigTblPtr->Z_P;

        m_VelP[0] = ConfigTblPtr->XY_VEL_P;
        m_VelP[1] = ConfigTblPtr->XY_VEL_P;
        m_VelP[2] = ConfigTblPtr->Z_VEL_P;

        m_VelI[0] = ConfigTblPtr->XY_VEL_I;
        m_VelI[1] = ConfigTblPtr->XY_VEL_I;
        m_VelI[2] = ConfigTblPtr->Z_VEL_I;

        m_VelD[0] = ConfigTblPtr->XY_VEL_D;
        m_VelD[1] = ConfigTblPtr->XY_VEL_D;
        m_VelD[2] = ConfigTblPtr->Z_VEL_D;
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* LimitAltitude                                                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::LimitAltitude(void)
{
    float AltitudeAboveHome = 0.0f;
    float DeltaT = 0.0f;
    float PosZNext = 0.0f;

    if (m_VehicleLandDetectedMsg.AltMax < 0.0f)
    {
        /* There is no altitude limitation present */
        goto MPC_LimitAltitude_ExitTag;
    }

    AltitudeAboveHome = -(m_Position[2] - m_HomePositionMsg.Z);

    if (m_RunAltControl && (AltitudeAboveHome > m_VehicleLandDetectedMsg.AltMax))
    {
        /* We are above maximum altitude */
        m_PositionSetpoint[2] = -m_VehicleLandDetectedMsg.AltMax +  m_HomePositionMsg.Z;
    }
    else if (!m_RunAltControl && m_VelocitySetpoint[2] <= 0.0f)
    {
        /* We want to fly upwards: check if vehicle does not exceed altitude */

        /* Time to reach zero velocity */
        DeltaT = -m_Velocity[2] / ConfigTblPtr->ACC_DOWN_MAX;

        /* Predict next position based on current position, velocity, max acceleration downwards and time to reach zero velocity */
        PosZNext = m_Position[2] + m_Velocity[2] * DeltaT + 0.5f * ConfigTblPtr->ACC_DOWN_MAX * DeltaT * DeltaT;

        if (-(PosZNext - m_HomePositionMsg.Z) > m_VehicleLandDetectedMsg.AltMax)
        {
            /* Prevent vehicle from exceeding maximum altitude by switching back to altitude control with maximum altitude as setpoint */
            m_PositionSetpoint[2] = -m_VehicleLandDetectedMsg.AltMax + m_HomePositionMsg.Z;
            m_RunAltControl = TRUE;
        }
    }

MPC_LimitAltitude_ExitTag:
    return;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* ApplyVelocitySetpointSlewRate                                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::ApplyVelocitySetpointSlewRate(float dt)
{
    float AccZ;
    float MaxAccZ;
    math::Vector2F VelSpXy(m_VelocitySetpoint[0], m_VelocitySetpoint[1]);
    math::Vector2F VelSpPrevXy(m_VelocitySetpointPrevious[0], m_VelocitySetpointPrevious[1]);
    math::Vector2F AccXy = (VelSpXy - VelSpPrevXy) / dt;

    /* limit total horizontal acceleration */
    if (AccXy.Length() > m_AccelerationStateLimitXY)
    {
        VelSpXy = (AccXy.Normalized() * m_AccelerationStateLimitXY * dt) + VelSpPrevXy;
        m_VelocitySetpoint[0] = VelSpXy[0];
        m_VelocitySetpoint[1] = VelSpXy[1];
    }

    /* limit vertical acceleration */
    AccZ = (m_VelocitySetpoint[2] - m_VelocitySetpointPrevious[2]) / dt;


    if (m_VehicleControlModeMsg.ControlManualEnabled)
    {
        MaxAccZ = (AccZ < 0.0f) ? -m_AccelerationStateLimitZ : m_AccelerationStateLimitZ;
    }
    else
    {
        MaxAccZ = (AccZ < 0.0f) ? -ConfigTblPtr->ACC_UP_MAX : ConfigTblPtr->ACC_DOWN_MAX;
    }

    if (fabsf(AccZ) > fabsf(MaxAccZ))
    {
        m_VelocitySetpoint[2] = MaxAccZ * dt + m_VelocitySetpointPrevious[2];
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* InAutoTakeoff                                                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool MPC::InAutoTakeoff(void)
{
    osalbool Result = FALSE;

    /* In auto mode, check if we do a takeoff */
    if(m_PositionSetpointTripletMsg.Current.Valid &&
            m_PositionSetpointTripletMsg.Current.Type == PX4_SETPOINT_TYPE_TAKEOFF)
    {
        /* We are in takeoff mode. */
        Result = TRUE;
    }
    else if( m_VehicleControlModeMsg.ControlOffboardEnabled)
    {
        /* Assume TRUE in offboard mode. */
        Result = TRUE;
    }

    return Result;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* GetVelClose                                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
float MPC::GetVelClose(const math::Vector2F &UnitPrevToCurrent, const math::Vector2F &UnitCurrentToNext)
{
    /* Minimum cruise speed when passing waypoint */
    float    MinCruiseSpeed = 0.2f;
    float    MiddleCruiseSpeed = ConfigTblPtr->MPC_CRUISE_90;
    osalbool UseLinearApproach = FALSE;
    float    Angle = 2.0f;
    float    Slope = 0.0f;
    float    VelClose = 0.0f;
    float    a = 0.0f;
    float    b = 0.0f;
    float    c = 0.0f;

    /* Make sure that cruise speed is larger than minimum*/
    if ((GetCruisingSpeedXY() - MinCruiseSpeed) < SIGMA_NORM)
    {
        VelClose = GetCruisingSpeedXY();
        goto MPC_GetVelClose_ExitTag;
    }

    /* Middle cruise speed is a number between maximum cruising speed and minimum cruising speed and corresponds to speed at angle of 90degrees
     * it needs to be always larger than minimum cruise speed */
    if ((MiddleCruiseSpeed - MinCruiseSpeed) < SIGMA_NORM)
    {
        MiddleCruiseSpeed = MinCruiseSpeed + SIGMA_NORM;
    }

    if ((GetCruisingSpeedXY() - MiddleCruiseSpeed) < SIGMA_NORM)
    {
        MiddleCruiseSpeed = (GetCruisingSpeedXY() + MinCruiseSpeed) * 0.5f;
    }

    /* If middle cruise speed is exactly in the middle, then compute
     * vel_close linearly */
    if (((GetCruisingSpeedXY() + MinCruiseSpeed) * 0.5f) - MiddleCruiseSpeed < SIGMA_NORM)
    {
        UseLinearApproach = TRUE;
    }

    /* Angle = cos(x) + 1.0
     * Angle goes from 0 to 2 with 0 = large angle, 2 = small angle:   0 = PI ; 2 = PI*0 */
    if (UnitCurrentToNext.Length() > SIGMA_NORM)
    {
        Angle = UnitCurrentToNext * (UnitPrevToCurrent * -1.0f) + 1.0f;
    }

    /* Compute velocity target close to waypoint */
    if (UseLinearApproach)
    {
        /* Velocity close to target adjusted to angle vel_close = m*x+q */
        Slope = -(GetCruisingSpeedXY() - MinCruiseSpeed) / 2.0f;
        VelClose = Slope * Angle + GetCruisingSpeedXY();

    }
    else
    {
        /* Velocity close to target adjusted to angle
         * vel_close = a *b ^x + c; where at angle = 0 -> vel_close = vel_cruise; angle = 1 -> vel_close = MiddleCruiseSpeed (this means that at 90degrees
         * the velocity at target is MiddleCruiseSpeed);
         * angle = 2 -> vel_close = min_cruising_speed */

        /* From maximum cruise speed, minimum cruise speed and middle cruise speed compute constants a, b and c */
        a = -((MiddleCruiseSpeed -  GetCruisingSpeedXY()) * (MiddleCruiseSpeed -  GetCruisingSpeedXY())) /
              (2.0f * MiddleCruiseSpeed - GetCruisingSpeedXY() - MinCruiseSpeed);
        c =  GetCruisingSpeedXY() - a;
        b = (MiddleCruiseSpeed - c) / a;
        VelClose = a * powf(b, Angle) + c;
    }

MPC_GetVelClose_ExitTag:
    /* vel_close needs to be in between max and min */
    return math::constrain(VelClose, MinCruiseSpeed, GetCruisingSpeedXY());
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* SetManualAccelerationZ                                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::SetManualAccelerationZ(float &MaxAcceleration, const float StickZ, const float Dt)
{
    /* In manual altitude control apply acceleration limit based on stick input
     * we consider two states
     * 1.) brake
     * 2.) accelerate */

    /* Check if zero input stick */
    const osalbool IsCurrentZero = (fabsf(StickZ) <= FLT_EPSILON);

    /* Default is acceleration */
    ManualStickInput Intention = ACCELERATION;

    float Jerk = 0.0f;

    /* Check zero input stick */
    if (IsCurrentZero)
    {
        Intention = BRAKE;
    }

    /* Get max and min acceleration where min acceleration is just 1/5 of max acceleration */
    MaxAcceleration = (StickZ <= 0.0f) ? ConfigTblPtr->ACC_UP_MAX : ConfigTblPtr->ACC_DOWN_MAX;

    /* Update user input */
    if ((m_UserIntentionZ != BRAKE) && (Intention  == BRAKE))
    {
        /* We start with lowest acceleration */
        m_AccelerationStateLimitZ = ConfigTblPtr->ACC_DOWN_MAX;

        /* Reset slew rate */
        m_VelocitySetpointPrevious[2] = m_Velocity[2];
        m_UserIntentionZ = BRAKE;
    }

    m_UserIntentionZ = Intention;

    /* Apply acceleration depending on state */
    if (m_UserIntentionZ == BRAKE)
    {
        /* Limit jerk when braking to zero */
        Jerk = (ConfigTblPtr->ACC_UP_MAX - m_AccelerationStateLimitZ) / Dt;

        if (Jerk > m_ManualJerkLimitZ)
        {
            m_AccelerationStateLimitZ = m_ManualJerkLimitZ * Dt + m_AccelerationStateLimitZ;
        }
        else
        {
            m_AccelerationStateLimitZ = ConfigTblPtr->ACC_UP_MAX;
        }
    }
    else if (m_UserIntentionZ == ACCELERATION)
    {
        m_AccelerationStateLimitZ = (MaxAcceleration - ConfigTblPtr->ACC_DOWN_MAX) * fabsf(
                StickZ) + ConfigTblPtr->ACC_DOWN_MAX;
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* SetManualAccelerationXY                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MPC::SetManualAccelerationXY(math::Vector2F &StickXy, const float Dt)
{

    /*
     * In manual mode we consider four states with different acceleration handling:
     * 1. user wants to stop
     * 2. user wants to quickly change direction
     * 3. user wants to accelerate
     * 4. user wants to decelerate
     */

    /* Get normalized stick input vector */
    math::Vector2F StickXyNorm = (StickXy.Length() > 0.0f) ? StickXy.Normalized() : StickXy;
    math::Vector2F StickXyPrevNorm = (m_StickInputXyPrev.Length() > 0.0f) ? m_StickInputXyPrev.Normalized() :
                          m_StickInputXyPrev;

    /* Check if stick direction and current velocity are within 60angle */
    const osalbool IsAligned = (StickXyNorm * StickXyPrevNorm) > 0.5f;

    /* Check if zero input stick */
    const osalbool IsPrevZero = (fabsf(m_StickInputXyPrev.Length()) <= FLT_EPSILON);
    const osalbool IsCurrentZero = (fabsf(StickXy.Length()) <= FLT_EPSILON);

    /* Check intentions */
    const osalbool DoAcceleration = IsPrevZero || (IsAligned &&
                     ((StickXy.Length() > m_StickInputXyPrev.Length()) || (fabsf(StickXy.Length() - 1.0f) < FLT_EPSILON)));
    const osalbool DoDeceleration = (IsAligned && (StickXy.Length() <= m_StickInputXyPrev.Length()));
    const osalbool DoDirectionChange = !IsAligned;

    ManualStickInput Intention;

    if (IsCurrentZero)
    {
        /* We want to stop */
        Intention = BRAKE;
    }
    else if (DoAcceleration)
    {
        /* We do manual acceleration */
        Intention = ACCELERATION;
    }
    else if (DoDeceleration)
    {
        /* We do manual deceleration */
        Intention = DECELERATION;
    }
    else if (DoDirectionChange)
    {
        /* We have a direction change */
        Intention = DIRECTION_CHANGE;
    }
    else
    {
        /* Catch all: acceleration */
        Intention = ACCELERATION;
    }

    /* Update user intention */

    /* We always want to brake starting with slow deceleration */
    if ((m_UserIntentionXY != BRAKE) && (Intention  == BRAKE))
    {
        if (ConfigTblPtr->MPC_JERK_MAX > ConfigTblPtr->MPC_JERK_MIN)
        {
            m_ManualJerkLimitXY = (ConfigTblPtr->MPC_JERK_MAX - ConfigTblPtr->MPC_JERK_MIN) / m_VelMaxXy *
                        sqrtf(m_Velocity[0] * m_Velocity[0] + m_Velocity[1] * m_Velocity[1]) + ConfigTblPtr->MPC_JERK_MIN;

            /* We start braking with lowest accleration */
            m_AccelerationStateLimitXY = ConfigTblPtr->MPC_DEC_HOR_SLOW;
        }
        else
        {
            /* Set the jerk limit large since we don't know it better*/
            m_ManualJerkLimitXY = MPC_HOR_JERK_LIMIT_MAX;

            /* At brake we use max acceleration */
            m_AccelerationStateLimitXY = ConfigTblPtr->ACC_HOR_MAX;
        }

        /* Reset slew rate */
        m_VelocitySetpointPrevious[0] = m_Velocity[0];
        m_VelocitySetpointPrevious[1] = m_Velocity[1];
    }

    switch (m_UserIntentionXY)
    {
        case BRAKE:
        {
            if (Intention != BRAKE)
            {
                m_UserIntentionXY = ACCELERATION;
                /* We initialize with lowest acceleration */
                m_AccelerationStateLimitXY = ConfigTblPtr->MPC_DEC_HOR_SLOW;
            }

            break;
        }

        case DIRECTION_CHANGE:
        {
            /* Only exit direction change if brake or aligned */
            math::Vector2F VelXy(m_Velocity[0], m_Velocity[1]);
            math::Vector2F VelXyNorm = (VelXy.Length() > 0.0f) ? VelXy.Normalized() : VelXy;
            osalbool StickVelAligned = (VelXyNorm * StickXyNorm > 0.0f);

            /* Update manual direction change hysteresis */
            m_ManualDirectionChangeHysteresis.set_state_and_update(!StickVelAligned, PX4LIB_GetPX4TimeUs());

            /* Exit direction change if one of the condition is met */
            if (Intention == BRAKE)
            {
                m_UserIntentionXY = Intention;
            }
            else if (StickVelAligned)
            {
                m_UserIntentionXY = ACCELERATION;
            }
            else if (m_ManualDirectionChangeHysteresis.get_state())
            {
                if (StickXy.Length() > 0.6f)
                {
                    m_AccelerationStateLimitXY = ConfigTblPtr->ACC_HOR_MAX;
                }
            }

            break;
        }

        case ACCELERATION:
        {
            m_UserIntentionXY = Intention;

            if (m_UserIntentionXY == DIRECTION_CHANGE)
            {
                m_VelocitySetpointPrevious[0] = m_Velocity[0];
                m_VelocitySetpointPrevious[1] = m_Velocity[1];
            }

            break;
        }

        case DECELERATION:
        {
            m_UserIntentionXY = Intention;

            if (m_UserIntentionXY == DIRECTION_CHANGE)
            {
                m_VelocitySetpointPrevious[0] = m_Velocity[0];
                m_VelocitySetpointPrevious[1] = m_Velocity[1];
            }

            break;
        }
    }

    /* Apply acceleration based on state */
    switch (m_UserIntentionXY)
    {
        case BRAKE:
        {
            /* Limit jerk when braking to zero */
            float jerk = (ConfigTblPtr->ACC_HOR_MAX - m_AccelerationStateLimitXY) / Dt;

            if (jerk > m_ManualJerkLimitXY)
            {
                m_AccelerationStateLimitXY = m_ManualJerkLimitXY * Dt + m_AccelerationStateLimitXY;
            }
            else
            {
                m_AccelerationStateLimitXY = ConfigTblPtr->ACC_HOR_MAX;
            }

            break;
        }

        case DIRECTION_CHANGE:
        {
            /* Limit acceleration linearly on stick input*/
            m_AccelerationStateLimitXY = (ConfigTblPtr->ACC_HOR_MAX - ConfigTblPtr->MPC_DEC_HOR_SLOW) * StickXy.Length() +
                               ConfigTblPtr->MPC_DEC_HOR_SLOW;
            break;
        }

        case ACCELERATION:
        {
            /* Limit acceleration linearly on stick input*/
            float acc_limit  = (ConfigTblPtr->ACC_HOR_MAX - ConfigTblPtr->MPC_DEC_HOR_SLOW) * StickXy.Length()
                       + ConfigTblPtr->MPC_DEC_HOR_SLOW;

            if (m_AccelerationStateLimitXY > acc_limit)
            {
                acc_limit = m_AccelerationStateLimitXY;
            }

            m_AccelerationStateLimitXY = acc_limit;
            break;
        }

        case DECELERATION:
        {
            m_AccelerationStateLimitXY = ConfigTblPtr->MPC_DEC_HOR_SLOW;
            break;
        }

        default:
        {
            m_AccelerationStateLimitXY = ConfigTblPtr->ACC_HOR_MAX;
            (void) CFE_EVS_SendEvent(MPC_UNKNOWN_USER_INTENTION_ERR_EID, CFE_EVS_ERROR,
                    "User intention not recognized. %u",
                    m_UserIntentionXY);
        }
    }

    /* Update previous stick input */
    m_StickInputXyPrev = math::Vector2F(m_FilterManualPitch.apply(StickXy[0]),
                    m_FilterManualRoll.apply(StickXy[1]));


    if (m_StickInputXyPrev.Length() > 1.0f)
    {
        m_StickInputXyPrev = m_StickInputXyPrev.Normalized();
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* ManualWantsTakeoff                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool MPC::ManualWantsTakeoff()
{
    const osalbool ManualControlPresent = m_VehicleControlModeMsg.ControlManualEnabled && m_ManualControlSetpointMsg.Timestamp > 0;

    /* Manual takeoff is triggered if the throttle stick is above 65%. */
    return (ManualControlPresent && m_ManualControlSetpointMsg.Z > MPC_MANUAL_TAKEOFF_THRESHOLD);
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate_XY_P                                                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool MPC::Validate_XY_P(float value)
{
    osalbool isValid = TRUE;

    if(!(value >= XY_P_MIN && value <= XY_P_MAX))
    {
        (void) CFE_EVS_SendEvent(MPC_INVLD_PARAM_ERR_EID, CFE_EVS_ERROR,
                "Parameter XY_P is invalid.");
        isValid = FALSE;
    }

    return isValid;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate_XY_VEL_P                                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool MPC::Validate_XY_VEL_P(float value)
{
    osalbool isValid = TRUE;

    if(!(value >= XY_VEL_P_MIN && value <= XY_VEL_P_MAX))
    {
        (void) CFE_EVS_SendEvent(MPC_INVLD_PARAM_ERR_EID, CFE_EVS_ERROR,
                "Parameter XY_VEL_P is invalid.");
        isValid = FALSE;
    }

    return isValid;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate_XY_VEL_I                                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool MPC::Validate_XY_VEL_I(float value)
{
    osalbool isValid = TRUE;

    if(!(value >= XY_VEL_I_MIN && value <= XY_VEL_I_MAX))
    {
        (void) CFE_EVS_SendEvent(MPC_INVLD_PARAM_ERR_EID, CFE_EVS_ERROR,
                "Parameter XY_VEL_I is invalid.");
        isValid = FALSE;
    }

    return isValid;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate_XY_VEL_D                                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool MPC::Validate_XY_VEL_D(float value)
{
    osalbool isValid = TRUE;

    if(!(value >= XY_VEL_D_MIN && value <= XY_VEL_D_MAX))
    {
        (void) CFE_EVS_SendEvent(MPC_INVLD_PARAM_ERR_EID, CFE_EVS_ERROR,
                "Parameter XY_VEL_D is invalid.");
        isValid = FALSE;
    }

    return isValid;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate_Z_P                                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool MPC::Validate_Z_P(float value)
{
    osalbool isValid = TRUE;

    if(!(value >= Z_P_MIN && value <= Z_P_MAX))
    {
        (void) CFE_EVS_SendEvent(MPC_INVLD_PARAM_ERR_EID, CFE_EVS_ERROR,
                "Parameter Z_P is invalid.");
        isValid = FALSE;
    }

    return isValid;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate_Z_VEL_P                                                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool MPC::Validate_Z_VEL_P(float value)
{
    osalbool isValid = TRUE;

    if(!(value >= Z_VEL_P_MIN && value <= Z_VEL_P_MAX))
    {
        (void) CFE_EVS_SendEvent(MPC_INVLD_PARAM_ERR_EID, CFE_EVS_ERROR,
                "Parameter Z_VEL_P is invalid.");
        isValid = FALSE;
    }

    return isValid;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate_Z_VEL_I                                                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool MPC::Validate_Z_VEL_I(float value)
{
    osalbool isValid = TRUE;

    if(!(value >= Z_VEL_I_MIN && value <= Z_VEL_I_MAX))
    {
        (void) CFE_EVS_SendEvent(MPC_INVLD_PARAM_ERR_EID, CFE_EVS_ERROR,
                "Parameter Z_VEL_I is invalid.");
        isValid = FALSE;
    }

    return isValid;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate_Z_VEL_D                                                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool MPC::Validate_Z_VEL_D(float value)
{
    osalbool isValid = TRUE;

    if(!(value >= Z_VEL_D_MIN && value <= Z_VEL_D_MAX))
    {
        (void) CFE_EVS_SendEvent(MPC_INVLD_PARAM_ERR_EID, CFE_EVS_ERROR,
                "Parameter Z_VEL_D is invalid.");
        isValid = FALSE;
    }

    return isValid;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate_HOLD_DZ                                                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool MPC::Validate_HOLD_DZ(float value)
{
    osalbool isValid = TRUE;

    if(!(value >= HOLD_DZ_MIN && value <= HOLD_DZ_MAX))
    {
        (void) CFE_EVS_SendEvent(MPC_INVLD_PARAM_ERR_EID, CFE_EVS_ERROR,
                "Parameter HOLD_DZ is invalid.");
        isValid = FALSE;
    }

    return isValid;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate_XY_MAN_EXPO                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool MPC::Validate_XY_MAN_EXPO(float value)
{
    osalbool isValid = TRUE;

    if(!(value >= XY_MAN_EXPO_MIN && value <= XY_MAN_EXPO_MAX))
    {
        (void) CFE_EVS_SendEvent(MPC_INVLD_PARAM_ERR_EID, CFE_EVS_ERROR,
                "Parameter XY_MAN_EXPO is invalid.");
        isValid = FALSE;
    }

    return isValid;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate_Z_MAN_EXPO                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool MPC::Validate_Z_MAN_EXPO(float value)
{
    osalbool isValid = TRUE;

    if(!(value >= Z_MAN_EXPO_MIN && value <= Z_MAN_EXPO_MAX))
    {
        (void) CFE_EVS_SendEvent(MPC_INVLD_PARAM_ERR_EID, CFE_EVS_ERROR,
                "Parameter Z_MAN_EXPO is invalid.");
        isValid = FALSE;
    }

    return isValid;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate_TKO_RAMP_T                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool MPC::Validate_TKO_RAMP_T(float value)
{
    osalbool isValid = TRUE;

    if(!(value >= TKO_RAMP_T_MIN && value <= TKO_RAMP_T_MAX))
    {
        (void) CFE_EVS_SendEvent(MPC_INVLD_PARAM_ERR_EID, CFE_EVS_ERROR,
                "Parameter TKO_RAMP_T is invalid.");
        isValid = FALSE;
    }

    return isValid;
}



/************************/
/*  End of File Comment */
/************************/
