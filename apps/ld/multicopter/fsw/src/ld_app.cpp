/************************************************************************
 ** Includes
 *************************************************************************/
#include <string.h>
#include <cmath>
#include <math.h>
#include <float.h>
#include "cfe.h"
#include "ld_app.h"
#include "ld_msg.h"
#include "ld_version.h"
#include "px4lib_msgids.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Instantiate the application object.                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
LD oLD;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Default constructor.                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
LD::LD()
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Destructor constructor.                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
LD::~LD()
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables.                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 LD::InitEvent()
{
    int32  iStatus = CFE_SUCCESS;
    int32  ind = 0;

    CFE_EVS_BinFilter_t   EventTbl[LD_MAX_EVENT_FILTERS];

    /* Initialize the event filter table.
     * Note: 0 is the CFE_EVS_NO_FILTER mask and event 0 is reserved (not used) */
    CFE_PSP_MemSet(EventTbl, 0x00, sizeof(EventTbl));
    
    EventTbl[  ind].EventID = LD_RESERVED_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    EventTbl[  ind].EventID = LD_INIT_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    EventTbl[  ind].EventID = LD_CMD_NOOP_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EventTbl[  ind].EventID = LD_SUBSCRIBE_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EventTbl[  ind].EventID = LD_PIPE_INIT_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EventTbl[  ind].EventID = LD_CFGTBL_MANAGE_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EventTbl[  ind].EventID = LD_CFGTBL_GETADDR_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EventTbl[  ind].EventID = LD_RCVMSG_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EventTbl[  ind].EventID = LD_MSGID_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EventTbl[  ind].EventID = LD_CC_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EventTbl[  ind].EventID = LD_MSGLEN_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EventTbl[  ind].EventID = LD_CFGTBL_REG_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EventTbl[  ind].EventID = LD_CFGTBL_LOAD_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EventTbl[  ind].EventID = LD_FREEFALL_DETECTED_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EventTbl[  ind].EventID = LD_GROUNDCONTACT_DETECTED_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EventTbl[  ind].EventID = LD_LAND_DETECTED_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EventTbl[  ind].EventID = LD_FLIGHT_DETECTED_EID;
    EventTbl[ind++].Mask    = CFE_EVS_FIRST_ONE_STOP;
    
    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(EventTbl, ind, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS) 
    {
        (void) CFE_ES_WriteToSysLog(
                "LD - Failed to register with EVS (0x%08lX)\n", iStatus);
    }

    return (iStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 LD::InitPipe()
{
    int32 iStatus = CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&SchPipeId, LD_SCH_PIPE_DEPTH, LD_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to wakeup */
        iStatus = CFE_SB_SubscribeEx(LD_WAKEUP_MID, 
                                     SchPipeId,
                                     CFE_SB_Default_Qos, 
                                     LD_WAKEUP_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(LD_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "Sch Pipe failed to subscribe to LD_WAKEUP_MID. (0x%08lX)",
                    iStatus);
            goto LD_InitPipe_Exit_Tag;
        }

        /* Subscribe to send HK */
        iStatus = CFE_SB_SubscribeEx(LD_SEND_HK_MID, 
                                     SchPipeId,
                                     CFE_SB_Default_Qos, 
                                     LD_SEND_HK_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(LD_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "Sch Pipe failed to subscribe to LD_SEND_HK_MID. (0x%08X)",
                    (unsigned int) iStatus);
            goto LD_InitPipe_Exit_Tag;
        }

        /* Subscribe to actuator armed */
        iStatus = CFE_SB_SubscribeEx(PX4_ACTUATOR_ARMED_MID, 
                                     SchPipeId,
                                     CFE_SB_Default_Qos, 
                                     1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(LD_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "Sch Pipe failed to subscribe to PX4_ACTUATOR_ARMED_MID. (0x%08lX)",
                    iStatus);
            goto LD_InitPipe_Exit_Tag;
        }

        /* Subscribe to airspeeed */
        iStatus = CFE_SB_SubscribeEx(PX4_AIRSPEED_MID, 
                                     SchPipeId,
                                     CFE_SB_Default_Qos,
                                     1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(LD_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "Sch Pipe failed to subscribe to PX4_AIRSPEED_MID. (0x%08lX)",
                    iStatus);
            goto LD_InitPipe_Exit_Tag;
        }

        /* Subscribe to actuator controls */
        iStatus = CFE_SB_SubscribeEx(PX4_ACTUATOR_CONTROLS_0_MID,
                                     SchPipeId,
                                     CFE_SB_Default_Qos,
                                     1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(LD_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "Sch Pipe failed to subscribe to PX4_ACTUATOR_CONTROLS_0_MID. (0x%08lX)",
                    iStatus);
            goto LD_InitPipe_Exit_Tag;
        }

        /* Subscribe to control state */
        iStatus = CFE_SB_SubscribeEx(PX4_CONTROL_STATE_MID,
                                     SchPipeId,
                                     CFE_SB_Default_Qos, 
                                     1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(LD_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "Sch Pipe failed to subscribe to PX4_CONTROL_STATE_MID. (0x%08lX)",
                    iStatus);
            goto LD_InitPipe_Exit_Tag;
        }

        /* Subscribe to battery status */
        iStatus = CFE_SB_SubscribeEx(PX4_BATTERY_STATUS_MID,
                                     SchPipeId,
                                     CFE_SB_Default_Qos,
                                     1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(LD_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "Sch Pipe failed to subscribe to PX4_BATTERY_STATUS_MID. (0x%08lX)",
                    iStatus);
            goto LD_InitPipe_Exit_Tag;
        }

        /* Subscribe to vehicle attitude */
        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_ATTITUDE_MID,
                                     SchPipeId,
                                     CFE_SB_Default_Qos,
                                     1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(LD_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "Sch Pipe failed to subscribe to PX4_VEHICLE_ATTITUDE_MID. (0x%08lX)",
                    iStatus);
            goto LD_InitPipe_Exit_Tag;
        }

        /* Subscribe to manual control setpoint */
        iStatus = CFE_SB_SubscribeEx(PX4_MANUAL_CONTROL_SETPOINT_MID,
                                     SchPipeId,
                                     CFE_SB_Default_Qos,
                                     1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(LD_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "Sch Pipe failed to subscribe to PX4_MANUAL_CONTROL_SETPOINT_MID. (0x%08lX)",
                    iStatus);
            goto LD_InitPipe_Exit_Tag;
        }

        /* Subscribe to vehicle local position */
        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_LOCAL_POSITION_MID,
                                     SchPipeId,
                                     CFE_SB_Default_Qos,
                                     1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(LD_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "Sch Pipe failed to subscribe to PX4_VEHICLE_LOCAL_POSITION_MID. (0x%08lX)",
                    iStatus);
            goto LD_InitPipe_Exit_Tag;
        }

        /* Subscribe to vehicle control mode */
        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_CONTROL_MODE_MID,
                                     SchPipeId,
                                     CFE_SB_Default_Qos,
                                     1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(LD_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "Sch Pipe failed to subscribe to PX4_VEHICLE_CONTROL_MODE_MID. (0x%08lX)",
                    iStatus);
            goto LD_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(LD_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
                "Failed to create SCH pipe (0x%08lX)", iStatus);
        goto LD_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&CmdPipeId, LD_CMD_PIPE_DEPTH, LD_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(LD_CMD_MID, CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(LD_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "CMD Pipe failed to subscribe to LD_CMD_MID. (0x%08lX)",
                    iStatus);
            goto LD_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(LD_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
                "Failed to create CMD pipe (0x%08lX)", iStatus);
        goto LD_InitPipe_Exit_Tag;
    }

LD_InitPipe_Exit_Tag:
    return (iStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void LD::InitData()
{
    /* Use Trigger time when transitioning from in-air (false) to landed (true) / ground contact (true) */
    landed_history.setTimeSince(FALSE, LAND_DETECTOR_TRIGGER_TIME_US);
    ground_contact_history.setTimeSince(FALSE, GROUND_CONTACT_TRIGGER_TIME_US);

    /* Init housekeeping message */
    CFE_SB_InitMsg(&HkTlm, LD_HK_TLM_MID, sizeof(HkTlm), TRUE);

    /* Init output message */
    CFE_SB_InitMsg(&VehicleLandDetectedMsg, PX4_VEHICLE_LAND_DETECTED_MID,
                   sizeof(PX4_VehicleLandDetectedMsg_t), TRUE);

    /* Init diagnostic message */
    CFE_SB_InitMsg(&DiagTlm, LD_DIAG_TLM_MID, sizeof(DiagTlm), TRUE);
    
    CFE_PSP_MemSet(&CVT, 0, sizeof(CVT));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* LD initialization                                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 LD::InitApp()
{
    int32 iStatus = CFE_SUCCESS;
    int8 hasEvents = 0;

    InitData();

    iStatus = InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("LD - Failed to init events (0x%08lX)\n",
                                    iStatus);
        goto LD_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        goto LD_InitApp_Exit_Tag;
    }

    iStatus = InitConfigTbl();
    if (iStatus != CFE_SUCCESS)
    {
        goto LD_InitApp_Exit_Tag;
    }

    if(ConfigTblPtr->LD_OP_MODE == LD_OP_MODE_AUTO)
    {
        (void) CFE_EVS_SendEvent(LD_STARTUP_MODE_EID, CFE_EVS_INFORMATION,
                                 "Operational mode auto");
    }

    if(ConfigTblPtr->LD_OP_MODE == LD_OP_MODE_MANUAL)
    {
        (void) CFE_EVS_SendEvent(LD_STARTUP_MODE_EID, CFE_EVS_INFORMATION,
                                 "Operational mode manual");
    }

    if(ConfigTblPtr->LD_OP_MODE == LD_OP_MODE_MIXED)
    {
        (void) CFE_EVS_SendEvent(LD_STARTUP_MODE_EID, CFE_EVS_INFORMATION,
                                 "Operational mode mixed");
    }

    HkTlm.mode = ConfigTblPtr->LD_OP_MODE;

LD_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(LD_INIT_INF_EID, 
                                 CFE_EVS_INFORMATION,
                                 "Initialized.  Version %d.%d.%d.%d",
                                 LD_MAJOR_VERSION,
                                 LD_MINOR_VERSION,
                                 LD_REVISION,
                                 LD_MISSION_REV);
    }
    else
    {
        if (0 == hasEvents)
        {
            (void) CFE_ES_WriteToSysLog(
                    "LD - Application failed to initialize\n");
        }
    }

    return (iStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 LD::RcvSchPipeMsg(int32 iBlocking)
{
    int32 iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t* MsgPtr = NULL;
    CFE_SB_MsgId_t MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(LD_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(LD_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
        {
            case LD_WAKEUP_MID:
            {
                Execute();
                break;
            }
            case LD_SEND_HK_MID:
            {
                ProcessCmdPipe();
                ReportHousekeeping();
                break;
            }
            case PX4_ACTUATOR_ARMED_MID:
            {
                CFE_PSP_MemCpy(&CVT.ActuatorArmedMsg, MsgPtr, 
                               sizeof(CVT.ActuatorArmedMsg));
                break;
            }
            case PX4_AIRSPEED_MID:
            {
                CFE_PSP_MemCpy(&CVT.AirspeedMsg, MsgPtr, 
                               sizeof(CVT.AirspeedMsg));
                break;
            }
            case PX4_ACTUATOR_CONTROLS_0_MID:
            {
                CFE_PSP_MemCpy(&CVT.ActuatorControls0Msg, MsgPtr,
                               sizeof(CVT.ActuatorControls0Msg));
                break;
            }
            case PX4_CONTROL_STATE_MID:
            {
                CFE_PSP_MemCpy(&CVT.ControlStateMsg, MsgPtr, 
                               sizeof(CVT.ControlStateMsg));
                break;
            }
            case PX4_BATTERY_STATUS_MID:
            {
                CFE_PSP_MemCpy(&CVT.BatteryStatusMsg, MsgPtr, 
                               sizeof(CVT.BatteryStatusMsg));
                break;
            }
            case PX4_VEHICLE_ATTITUDE_MID:
            {
                CFE_PSP_MemCpy(&CVT.VehicleAttitudeMsg, MsgPtr,
                               sizeof(CVT.VehicleAttitudeMsg));
                break;
            }
            case PX4_MANUAL_CONTROL_SETPOINT_MID:
            {
                CFE_PSP_MemCpy(&CVT.ManualControlSetpointMsg, MsgPtr,
                               sizeof(CVT.ManualControlSetpointMsg));
                break;
            }
            case PX4_VEHICLE_LOCAL_POSITION_MID:
            {
                CFE_PSP_MemCpy(&CVT.VehicleLocalPositionMsg, MsgPtr,
                               sizeof(CVT.VehicleLocalPositionMsg));
                break;
            }
            case PX4_VEHICLE_CONTROL_MODE_MID:
            {
                CFE_PSP_MemCpy(&CVT.VehicleControlModeMsg, MsgPtr,
                               sizeof(CVT.VehicleControlModeMsg));
                break;
            }
            default:
            {
                (void) CFE_EVS_SendEvent(LD_MSGID_ERR_EID, CFE_EVS_ERROR,
                        "Recvd invalid SCH msgId (0x%04X)", MsgId);
                break;
            }
        }
    }
    else if (iStatus == CFE_SB_NO_MESSAGE)
    {
        iStatus = CFE_SUCCESS;
    }
    else if (iStatus == CFE_SB_TIME_OUT)
    {
        iStatus = CFE_SUCCESS;
    }
    else
    {
        (void) CFE_EVS_SendEvent(LD_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                "SCH pipe read error (0x%08lX).", iStatus);
    }

    return (iStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void LD::ProcessCmdPipe()
{
    int32 iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t* CmdMsgPtr = NULL;
    CFE_SB_MsgId_t CmdMsgId;

    /* Process command messages until the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, CmdPipeId, CFE_SB_POLL);
        if (iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case LD_CMD_MID:
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
                    (void) CFE_EVS_SendEvent(LD_MSGID_ERR_EID, CFE_EVS_ERROR,
                                             "Recvd invalid CMD msgId (0x%04X)",
                                             (unsigned short) CmdMsgId);
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
            (void) CFE_EVS_SendEvent(LD_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                    "CMD pipe read error (0x%08lX)", iStatus);
            break;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process LD Commands                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void LD::ProcessAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32 uiCmdCode  = 0;
    int32  returnCode = 0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case LD_NOOP_CC:
            {
                HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(LD_CMD_NOOP_EID, 
                                         CFE_EVS_INFORMATION,
                                         "Recvd NOOP. Version %d.%d.%d.%d",
                                         LD_MAJOR_VERSION,
                                         LD_MINOR_VERSION,
                                         LD_REVISION,
                                         LD_MISSION_REV);
                break;
            }
            case LD_RESET_CC:
            {
                HkTlm.usCmdCnt = 0;
                HkTlm.usCmdErrCnt = 0;
                break;
            }
            case LD_MODE_AUTO_CC:
            {
                if(ConfigTblPtr->LD_OP_MODE == LD_OP_MODE_AUTO)
                {
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(LD_MODE_CHANGE_ERROR_EID, 
                                             CFE_EVS_ERROR,
                                             "Command error LD already in auto mode.");
                }
                else
                {
                    HkTlm.usCmdCnt++;
                    ConfigTblPtr->LD_OP_MODE = LD_OP_MODE_AUTO;
                    HkTlm.mode = ConfigTblPtr->LD_OP_MODE;
                    returnCode = CFE_TBL_Modified(ConfigTblHdl);
                    if(returnCode != CFE_SUCCESS)
                    {
                        (void) CFE_EVS_SendEvent(LD_TBL_MODIFIED_ERROR_EID, 
                                                 CFE_EVS_ERROR,
                                                 "CFE_TBL_Modified error (%d)",
                                                 returnCode);
                    }
                    (void) CFE_EVS_SendEvent(LD_MODE_CHANGED_EID, 
                                             CFE_EVS_INFORMATION,
                                             "Operational mode changed to auto.");
                }
                break;
            }
            case LD_MODE_MANUAL_CC:
            {
                if(ConfigTblPtr->LD_OP_MODE == LD_OP_MODE_MANUAL)
                {
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(LD_MODE_CHANGE_ERROR_EID, 
                                             CFE_EVS_ERROR,
                                             "Command error LD already in manual mode.");
                }
                else
                {
                    HkTlm.usCmdCnt++;
                    ConfigTblPtr->LD_OP_MODE = LD_OP_MODE_MANUAL;
                    HkTlm.mode = ConfigTblPtr->LD_OP_MODE;
                    returnCode = CFE_TBL_Modified(ConfigTblHdl);
                    if(returnCode != CFE_SUCCESS)
                    {
                        (void) CFE_EVS_SendEvent(LD_TBL_MODIFIED_ERROR_EID, 
                                                 CFE_EVS_ERROR,
                                                 "CFE_TBL_Modified error (%d)", returnCode);
                    }
                    (void) CFE_EVS_SendEvent(LD_MODE_CHANGED_EID, 
                                             CFE_EVS_INFORMATION,
                                             "Operational mode changed to manual.");
                }
                break;
            }
            case LD_MODE_MIXED_CC:
            {
                if(ConfigTblPtr->LD_OP_MODE == LD_OP_MODE_MIXED)
                {
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(LD_MODE_CHANGE_ERROR_EID, 
                                             CFE_EVS_ERROR,
                                             "Command error LD already in mixed mode.");
                }
                else
                {
                    HkTlm.usCmdCnt++;
                    ConfigTblPtr->LD_OP_MODE = LD_OP_MODE_MIXED;
                    HkTlm.mode = ConfigTblPtr->LD_OP_MODE;
                    returnCode = CFE_TBL_Modified(ConfigTblHdl);
                    if(returnCode != CFE_SUCCESS)
                    {
                        (void) CFE_EVS_SendEvent(LD_TBL_MODIFIED_ERROR_EID, 
                                                 CFE_EVS_ERROR,
                                                 "CFE_TBL_Modified error (%d)", returnCode);
                    }
                    (void) CFE_EVS_SendEvent(LD_MODE_CHANGED_EID, 
                                             CFE_EVS_INFORMATION,
                                             "Operational mode changed to mixed.");
                }
                break;
            }
            default:
            {
                HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(LD_CC_ERR_EID, CFE_EVS_ERROR,
                        "Recvd invalid command code (%u)",
                        (unsigned int) uiCmdCode);
                break;
            }
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send LD Housekeeping                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void LD::ReportHousekeeping()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*) &HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*) &HkTlm);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Publish Output Data                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void LD::SendVehicleLandDetectedMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*) &VehicleLandDetectedMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*) &VehicleLandDetectedMsg);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Publish LD Diag                                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void LD::SendDiag()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*) &DiagTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*) &DiagTlm);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Command Length                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool LD::VerifyCmdLength(CFE_SB_Msg_t* MsgPtr, uint16 usExpectedLen)
{
    osalbool bResult = TRUE;
    uint16 usMsgLen = 0;

    if (MsgPtr != NULL)
    {
        usMsgLen = CFE_SB_GetTotalMsgLength(MsgPtr);

        if (usExpectedLen != usMsgLen)
        {
            bResult = FALSE;
            CFE_SB_MsgId_t MsgId = CFE_SB_GetMsgId(MsgPtr);
            uint16 usCmdCode = CFE_SB_GetCmdCode(MsgPtr);

            (void) CFE_EVS_SendEvent(LD_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                    "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                            "msgLen=%d, expectedLen=%d", MsgId, usCmdCode,
                    usMsgLen, usExpectedLen);
            HkTlm.usCmdErrCnt++;
        }
    }

    return (bResult);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* LD Application C style main entry point.                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
extern "C" void LD_AppMain()
{
    oLD.AppMain();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* LD Application C++ style main entry point.                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void LD::AppMain()
{
    /* Register the application with Executive Services */
    uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog(
                "LD - Failed to register the app (0x%08lX)\n", iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(LD_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(LD_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(LD_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(LD_MAIN_TASK_PERF_ID);
    }
    else
    {
        uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&uiRunStatus) == TRUE)
    {
        RcvSchPipeMsg(LD_SCH_PIPE_PEND_TIME);

        iStatus = AcquireConfigPointers();
        if (iStatus != CFE_SUCCESS)
        {
            /* We apparently tried to load a new table but failed.  Terminate the application. */
            uiRunStatus = CFE_ES_APP_ERROR;
        }
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(LD_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(uiRunStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Detect if vehicle in free fall.                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool LD::DetectFreeFall()
{
    osalbool inFreefall = FALSE;
    
    if (CVT.ControlStateMsg.Timestamp != 0)
    {
        float net_acc = CVT.ControlStateMsg.AccX * CVT.ControlStateMsg.AccX
                      + CVT.ControlStateMsg.AccY * CVT.ControlStateMsg.AccY
                      + CVT.ControlStateMsg.AccZ * CVT.ControlStateMsg.AccZ;
        
        net_acc = sqrtf(net_acc);
        if (net_acc < ConfigTblPtr->LD_FFALL_THR)
        {
            /* Free fall event */
            inFreefall = TRUE;
        }
    }

    return (inFreefall);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Get ground contact state.                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool LD::DetectGroundContactState()
{
    const uint64 now = PX4LIB_GetPX4TimeUs();
    osalbool inGroundContact     = FALSE;
    osalbool minimal_thrust      = FALSE;
    osalbool altitude_lock       = FALSE;
    osalbool position_lock       = FALSE;
    osalbool in_descent          = FALSE;
    osalbool hit_ground          = FALSE;
    osalbool horizontal_movement = FALSE;
    osalbool vertical_movement   = FALSE;
    float horizontal_velocity    = 0.0f;

    /* If vehicle is disarmed, ground contact is always true */
    if (!CVT.ActuatorArmedMsg.Armed)
    {
        arming_time = 0;
        inGroundContact = TRUE;
    }
    else if (0 == arming_time)
    {
        arming_time = now;
    }

    if (!inGroundContact)
    {
        /* Land speed threshold */
        const float land_speed_threshold = 0.9f * ConfigTblPtr->LD_LANDSPEED;

        const osalbool manual_control_idling = (ManualControlPresent() && CVT.ManualControlSetpointMsg.Z  < ConfigTblPtr->LD_POS_STK_DW_THRES);

        const osalbool manual_control_idling_or_automatic = 
                            manual_control_idling || 
                            !CVT.VehicleControlModeMsg.ControlManualEnabled;

        /* Vertical movement */
        if ((now - arming_time) < LAND_DETECTOR_ARM_PHASE_TIME_US)
        {
            vertical_movement = fabsf(CVT.VehicleLocalPositionMsg.VZ) > (ConfigTblPtr->LD_Z_VEL_MAX * LD_ARMING_THRESH_FACTOR);
        }
        else
        {
            const float max_climb_rate = (land_speed_threshold * 0.5f) < ConfigTblPtr->LD_Z_VEL_MAX ? (0.5f * land_speed_threshold) : ConfigTblPtr->LD_Z_VEL_MAX;
            vertical_movement = fabsf(CVT.VehicleLocalPositionMsg.VZ) > max_climb_rate;
        }

        /* Horizontal movement */
        horizontal_velocity = sqrtf((CVT.VehicleLocalPositionMsg.VX * CVT.VehicleLocalPositionMsg.VX) + 
                                    (CVT.VehicleLocalPositionMsg.VY * CVT.VehicleLocalPositionMsg.VY));
        
        if (horizontal_velocity > ConfigTblPtr->LD_XY_VEL_MAX)
        {
            horizontal_movement = TRUE;
        }
        else
        {
            horizontal_movement = FALSE;
        }

        minimal_thrust = MinimalThrust();
        altitude_lock = AltitudeLock();
        position_lock = altitude_lock && CVT.VehicleLocalPositionMsg.XY_Valid;
        in_descent = (CVT.VehicleLocalPositionMsg.VZ >= land_speed_threshold);
        hit_ground = in_descent && !vertical_movement;
        
        DiagTlm.GC_MinThrust = minimal_thrust;
        DiagTlm.GC_AltitudeLock = altitude_lock;
        DiagTlm.GC_PositionLock = position_lock;
        DiagTlm.GC_InDescent = in_descent;
        DiagTlm.GC_HitGround = hit_ground;
        DiagTlm.GC_HorMovement = horizontal_movement;
        DiagTlm.GC_VertMovement = vertical_movement;
        DiagTlm.GC_ManualControlIdlingOrAutomatic = manual_control_idling_or_automatic;
        
        if ((minimal_thrust || hit_ground) && 
            (!horizontal_movement || !position_lock) && 
            (!vertical_movement || !altitude_lock) && 
            manual_control_idling_or_automatic)
        {
            inGroundContact = TRUE;
        }
        else
        {
            inGroundContact = FALSE;
        }

        DiagTlm.GroundContact = inGroundContact;
    }

    return (inGroundContact);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Get landed state.                                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool LD::DetectLandedState()
{
    osalbool isLandDetected = FALSE;
    const uint64 now = PX4LIB_GetPX4TimeUs();
    float armingThreshFactor = 1.0f;

    if (!CVT.ActuatorArmedMsg.Armed)
    {
        isLandDetected = TRUE;
    }

    if (!isLandDetected && 
        state == LandDetectionState::LANDED && 
        ManualControlPresent())
    {
        if (CVT.ManualControlSetpointMsg.Z < TakeoffThrottle())
        {
            isLandDetected = TRUE;
        }
        else
        {
            ground_contact_history.setState(FALSE);
            isLandDetected = FALSE;
        }
    }
    
    if (!isLandDetected)
    {
        if (MinimalThrust())
        {
            if (0 == min_thrust_start)
            {
                min_thrust_start = now;
            }
        }
        else
        {
            min_thrust_start = 0;
        }

        if (!AltitudeLock())
        {
            if ((min_thrust_start > 0)
                    && ((now - min_thrust_start) > ConfigTblPtr->LD_MIN_THR_NO_ALT_TIMEOUT))
            {
                isLandDetected = TRUE;
            }
            else 
            {
                isLandDetected = FALSE;
            }
        }

        if (!isLandDetected && 
            (PX4LIB_GetPX4TimeUs() - now) < LAND_DETECTOR_ARM_PHASE_TIME_US) //TODO is this valid?
        {
            armingThreshFactor = LD_ARMING_THRESH_FACTOR;
        }

        osalbool horizontal_movement = sqrtf(
                CVT.VehicleLocalPositionMsg.VX * CVT.VehicleLocalPositionMsg.VX
                + CVT.VehicleLocalPositionMsg.VY * CVT.VehicleLocalPositionMsg.VY)
                > ConfigTblPtr->LD_XY_VEL_MAX;

        const float max_rotation_scaled = ConfigTblPtr->LD_ROT_MAX * armingThreshFactor;

        const osalbool rotation = (fabsf(CVT.VehicleAttitudeMsg.RollSpeed) > max_rotation_scaled) ||
                            (fabsf(CVT.VehicleAttitudeMsg.PitchSpeed) > max_rotation_scaled) ||
                            (fabsf(CVT.VehicleAttitudeMsg.YawSpeed) > max_rotation_scaled);

        DiagTlm.LD_GC_history_state = ground_contact_history.getState();
        DiagTlm.LD_MinThrust = MinimalThrust();
        DiagTlm.LD_Rotation = rotation;
        DiagTlm.LD_HorMovement = horizontal_movement;
        DiagTlm.LD_PositionLock = PositionLock();

        if (ground_contact_history.getState() && 
            MinimalThrust() &&
            !rotation && 
            (!horizontal_movement || !PositionLock()))
        {
            isLandDetected = TRUE;
        }

        DiagTlm.Landed = isLandDetected;
    }
    
    return (isLandDetected);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Get take off throttle.                                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
float LD::TakeoffThrottle()
{
    float toThrottle = 0.0f;
    
    if (CVT.VehicleControlModeMsg.ControlManualEnabled &&
        CVT.VehicleControlModeMsg.ControlAltitudeEnabled)
    {
        toThrottle = ConfigTblPtr->LD_POS_STK_UP_THRES;
    }
    else if (CVT.VehicleControlModeMsg.ControlManualEnabled &&
        CVT.VehicleControlModeMsg.ControlAttitudeEnabled)
    {
        toThrottle = ConfigTblPtr->LD_LOW_T_THR;
    }
    else
    {
        toThrottle = 0.0f;
    }

    return toThrottle;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Get maximum altitude.                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
float LD::MaxAltitude()
{
    float max_alt = ConfigTblPtr->LD_ALT_MAX;
    
    /* If we haven't received this message just use default max */
    if(0 == CVT.BatteryStatusMsg.Timestamp)
    {
        max_alt = ConfigTblPtr->LD_ALT_MAX;
    }
    else if (PX4_BATTERY_WARNING_LOW == CVT.BatteryStatusMsg.Warning)
    {
        max_alt = ConfigTblPtr->LD_ALT_MAX * LD_75_PERCENT;
    }
    else if (PX4_BATTERY_WARNING_CRITICAL == CVT.BatteryStatusMsg.Warning)
    {
        max_alt = ConfigTblPtr->LD_ALT_MAX * LD_50_PERCENT;
    }
    else if (PX4_BATTERY_WARNING_EMERGENCY == CVT.BatteryStatusMsg.Warning)
    {
        max_alt = ConfigTblPtr->LD_ALT_MAX * LD_25_PERCENT;
    }
    else
    {
        max_alt = ConfigTblPtr->LD_ALT_MAX;
    }

    return max_alt;

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Check altitude lock presence.                                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool LD::AltitudeLock()
{
    osalbool result = FALSE;
    uint64 dt = PX4LIB_GetPX4TimeUs() - CVT.VehicleLocalPositionMsg.Timestamp;
    
    result = CVT.VehicleLocalPositionMsg.Timestamp != 0 &&
             dt < LD_LOCAL_POSITION_TIMEOUT && 
             CVT.VehicleLocalPositionMsg.Z_Valid;
    
    return (result);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Check position lock presence.                                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool LD::PositionLock()
{
    return (AltitudeLock() && CVT.VehicleLocalPositionMsg.XY_Valid);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  Check manual control presence.                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool LD::ManualControlPresent()
{
    return CVT.VehicleControlModeMsg.ControlManualEnabled &&
           CVT.ManualControlSetpointMsg.Timestamp > 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Check for minimum thrust.                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool LD::MinimalThrust() 
{
    float min_throttle = ConfigTblPtr->LD_LOW_T_THR;
    osalbool result = FALSE;

    if (!CVT.VehicleControlModeMsg.ControlAltitudeEnabled)
    {
        min_throttle = (ConfigTblPtr->LD_MAN_MIN_THR + ONE_PERCENT_THRUST);
    }

    result = CVT.ActuatorControls0Msg.Control[3] <= min_throttle;
    return result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Update State.                                                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void LD::UpdateState()
{
    freefall_history.setState(DetectFreeFall());
    landed_history.setState(DetectLandedState());
    ground_contact_history.setState(landed_history.getState() 
            || DetectGroundContactState());

    if (freefall_history.getState())
    {
        state = LandDetectionState::FREEFALL;
    }
    else if (landed_history.getState())
    {
        state = LandDetectionState::LANDED;
    }
    else if (ground_contact_history.getState())
    {
        state = LandDetectionState::GROUND_CONTACT;
    }
    else
    {
        state = LandDetectionState::FLYING;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Execute                                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void LD::Execute()
{
    static boolean warningEventIssued = FALSE;

    if (0 == publish_counter)
    {
        VehicleLandDetectedMsg.Freefall = FALSE;
        VehicleLandDetectedMsg.Landed = FALSE;
        VehicleLandDetectedMsg.GroundContact = FALSE;
        publish_counter += 1;
    }

    uint64 now = PX4LIB_GetPX4TimeUs();

    UpdateState();

    float prev_altitude_max = altitude_max;
    altitude_max = MaxAltitude();

    const osalbool Freefall = (state == LandDetectionState::FREEFALL);
    const osalbool Land = (state == LandDetectionState::LANDED);
    const osalbool Ground = (state == LandDetectionState::GROUND_CONTACT);
    const osalbool Manual = ManualControlPresent();

    if ((VehicleLandDetectedMsg.Freefall != Freefall) ||
        (VehicleLandDetectedMsg.Landed != Land) ||
        (VehicleLandDetectedMsg.GroundContact != Ground) ||
        (fabsf(VehicleLandDetectedMsg.AltMax - prev_altitude_max) > FLT_EPSILON))
    {
        VehicleLandDetectedMsg.Timestamp = now;
        VehicleLandDetectedMsg.AltMax = altitude_max;
        VehicleLandDetectedMsg.Freefall = Freefall;
        VehicleLandDetectedMsg.Landed = Land;
        VehicleLandDetectedMsg.GroundContact = Ground;
        HkTlm.state = state;
    }

    DetectStateChange();

    if(!CVT.VehicleControlModeMsg.ControlManualEnabled
       && ConfigTblPtr->LD_OP_MODE == LD_OP_MODE_MANUAL)
    {
        if(warningEventIssued == FALSE)
        {
            (void) CFE_EVS_SendEvent(LD_MODE_WARNING_EID,
                                     CFE_EVS_INFORMATION,
                                     "Op mode is manual. Auto disarm is disabled.");
        }
        warningEventIssued = TRUE;
    }
    else
    {
        warningEventIssued = FALSE;
    }

    /* If in manual mode. */
    if(ConfigTblPtr->LD_OP_MODE == LD_OP_MODE_MANUAL || 
       (ConfigTblPtr->LD_OP_MODE == LD_OP_MODE_MIXED && Manual))
    {
        /* Check the arm switch to determine state. */
        if(CVT.ManualControlSetpointMsg.ArmSwitch == PX4_SWITCH_POS_ON
           || CVT.ActuatorArmedMsg.Armed == TRUE)
        {
            /* Set state to in-flight. */
            VehicleLandDetectedMsg.Timestamp     = now;
            VehicleLandDetectedMsg.AltMax        = altitude_max;
            VehicleLandDetectedMsg.Freefall      = FALSE;
            VehicleLandDetectedMsg.Landed        = FALSE;
            VehicleLandDetectedMsg.GroundContact = FALSE;
            HkTlm.state                          = LandDetectionState::FLYING;
        }
        else
        {
            /* Set state to landed. */
            VehicleLandDetectedMsg.Timestamp     = now;
            VehicleLandDetectedMsg.AltMax        = altitude_max;
            VehicleLandDetectedMsg.Freefall      = FALSE;
            VehicleLandDetectedMsg.Landed        = TRUE;
            VehicleLandDetectedMsg.GroundContact = FALSE;
            HkTlm.state                          = LandDetectionState::LANDED;
        }
    }

    SendVehicleLandDetectedMsg();
    SendDiag();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Detect State Change                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void LD::DetectStateChange()
{
    PX4_VehicleLandDetectedMsg_t CurrentLandDetectedMsg = VehicleLandDetectedMsg;

    if (&PreviousLandDetectedMsg == nullptr)
    {
        PreviousLandDetectedMsg = CurrentLandDetectedMsg;
    }
    else if (PreviousLandDetectedMsg.Landed == CurrentLandDetectedMsg.Landed
            && PreviousLandDetectedMsg.Freefall == CurrentLandDetectedMsg.Freefall
            && PreviousLandDetectedMsg.GroundContact == CurrentLandDetectedMsg.GroundContact)
    {
        PreviousLandDetectedMsg = CurrentLandDetectedMsg;
    }
    else
    {
        if (CurrentLandDetectedMsg.Landed)
        {
            (void) CFE_EVS_SendEvent(LD_LAND_DETECTED_EID, 
                                     CFE_EVS_INFORMATION,
                                     "Land detected");
        }
        else if (CurrentLandDetectedMsg.Freefall)
        {
            (void) CFE_EVS_SendEvent(LD_GROUNDCONTACT_DETECTED_EID,
                                     CFE_EVS_INFORMATION,
                                     "Freefall detected");
        }
        else if (CurrentLandDetectedMsg.GroundContact)
        {
            (void) CFE_EVS_SendEvent(LD_FREEFALL_DETECTED_EID,
                                     CFE_EVS_INFORMATION,
                                     "Ground contact detected");
        }
        else
        {
            (void) CFE_EVS_SendEvent(LD_FLIGHT_DETECTED_EID, 
                                     CFE_EVS_INFORMATION,
                                     "Vehicle in flight");
        }
        
        PreviousLandDetectedMsg = CurrentLandDetectedMsg;
    }
}

/************************/
/*  End of File Comment */
/************************/
