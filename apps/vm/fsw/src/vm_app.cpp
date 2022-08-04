/****************************************************************************
 *
 *   Copyright (c) 2017 Windhover Labs, L.L.C. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name Windhover Labs nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/

/************************************************************************
 ** Includes
 *************************************************************************/
#include <string.h>
#include <float.h>
#include <math.h>
#include "cfe.h"
#include "vm_app.h"
#include "vm_msg.h"
#include "vm_version.h"
#include "px4lib.h"
#include "px4lib_msgids.h"



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
                ArmingSM(*this), NavigationSM(*this)
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
    int32 iStatus = CFE_SUCCESS;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(0, 0, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
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
int32 VM::InitPipe()
{
    int32 iStatus = CFE_SUCCESS;

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
                    "SCH Pipe failed to subscribe to VM_WAKEUP_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(VM_SEND_HK_MID, SchPipeId, CFE_SB_Default_Qos, VM_SEND_HK_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "SCH Pipe failed to subscribe to VM_SEND_HK_MID. (0x%08X)",
                    (unsigned int) iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(VM_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
                "Failed to create SCH pipe (0x%08lX)", iStatus);
        goto VM_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&CmdPipeId, VM_CMD_PIPE_DEPTH, VM_CMD_PIPE_NAME);
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
                "Failed to create CMD pipe (0x%08lX)", iStatus);
        goto VM_InitPipe_Exit_Tag;
    }

    /* Init data pipe and subscribe to data messages */
    iStatus = CFE_SB_CreatePipe(&DataPipeId, VM_DATA_PIPE_DEPTH, VM_DATA_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(PX4_SENSOR_MAG_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "DATA Pipe failed to subscribe to PX4_SENSOR_MAG_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_SENSOR_GYRO_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "DATA Pipe failed to subscribe to PX4_SENSOR_GYRO_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_BATTERY_STATUS_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "DATA Pipe failed to subscribe to PX4_TELEMETRY_STATUS_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_COMMAND_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "DATA Pipe failed to subscribe to PX4_VEHICLE_COMMAND_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_CONTROL_MODE_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "DATA Pipe failed to subscribe to PX4_VEHICLE_COMMAND_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_GLOBAL_POSITION_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "DATA Pipe failed to subscribe to PX4_SUBSYSTEM_INFO_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_TELEMETRY_STATUS_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "DATA Pipe failed to subscribe to PX4_TELEMETRY_STATUS_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_SUBSYSTEM_INFO_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "DATA Pipe failed to subscribe to PX4_SUBSYSTEM_INFO_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_GPS_POSITION_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "DATA Pipe failed to subscribe to PX4_VEHICLE_GPS_POSITION_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_ATTITUDE_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "DATA Pipe failed to subscribe to PX4_VEHICLE_ATTITUDE_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_LOCAL_POSITION_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "DATA Pipe failed to subscribe to PX4_VEHICLE_LOCAL_POSITION_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_LAND_DETECTED_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "DATA Pipe failed to subscribe to PX4_VEHICLE_LAND_DETECTED_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_GEOFENCE_RESULT_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "DATA Pipe failed to subscribe to PX4_GEOFENCE_RESULT_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_MISSION_RESULT_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "DATA Pipe failed to subscribe to PX4_MISSION_RESULT_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_MANUAL_CONTROL_SETPOINT_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "DATA Pipe failed to subscribe to PX4_MANUAL_CONTROL_SETPOINT_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_POSITION_SETPOINT_TRIPLET_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "DATA Pipe failed to subscribe to PX4_POSITION_SETPOINT_TRIPLET_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_OFFBOARD_CONTROL_MODE_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "DATA Pipe failed to subscribe to PX4_OFFBOARD_CONTROL_MODE_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_SENSOR_ACCEL_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "DATA Pipe failed to subscribe to PX4_SENSOR_ACCEL_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_SAFETY_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "DATA Pipe failed to subscribe to PX4_SAFETY_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_SENSOR_CORRECTION_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "DATA Pipe failed to subscribe to PX4_SENSOR_CORRECTION_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_SENSOR_COMBINED_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(VM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "DATA Pipe failed to subscribe to PX4_SENSOR_COMBINED_MID. (0x%08lX)",
                    iStatus);
            goto VM_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(VM_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
                "Failed to create DATA pipe (0x%08lX)", iStatus);
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
    CFE_SB_InitMsg(&HkTlm, VM_HK_TLM_MID, sizeof(HkTlm), TRUE);

    /* Init configuration message. */
    CFE_SB_InitMsg(&ConfigTlm, VM_CONFIG_TLM_MID, sizeof(ConfigTlm), TRUE);

    /* Init output messages */
    CFE_SB_InitMsg(&ActuatorArmedMsg,       PX4_ACTUATOR_ARMED_MID,       sizeof(PX4_ActuatorArmedMsg_t),      TRUE);
    CFE_SB_InitMsg(&HomePositionMsg,        PX4_HOME_POSITION_MID,        sizeof(PX4_HomePositionMsg_t),       TRUE);
    CFE_SB_InitMsg(&VehicleManagerStateMsg, PX4_COMMANDER_STATE_MID,      sizeof(PX4_CommanderStateMsg_t),     TRUE);
    CFE_SB_InitMsg(&MissionMsg,             PX4_MISSION_MID,              sizeof(PX4_MissionMsg_t),            TRUE);
    CFE_SB_InitMsg(&LedControlMsg,          PX4_LED_CONTROL_MID,          sizeof(PX4_LedControlMsg_t),         TRUE);
    CFE_SB_InitMsg(&VehicleStatusMsg,       PX4_VEHICLE_STATUS_MID,       sizeof(PX4_VehicleStatusMsg_t),      TRUE);
    CFE_SB_InitMsg(&VehicleControlModeMsg,  PX4_VEHICLE_CONTROL_MODE_MID, sizeof(PX4_VehicleControlModeMsg_t), TRUE);
    CFE_SB_InitMsg(&VehicleCommandMsg,      PX4_VEHICLE_COMMAND_MID,      sizeof(PX4_VehicleCommandMsg_t),     TRUE);
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* VM initialization                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 VM::InitApp()
{
    int32 iStatus = CFE_SUCCESS;
    int8 hasEvents = 0;

    iStatus = InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("VM - Failed to init events (0x%08lX)\n",
                iStatus);
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

    /* Initialize the caution and warning helper */
    m_caws.InitCAWS();

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
        if (hasEvents != 1)
        {
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
int32 VM::RcvSchPipeMsg(int32 iBlocking)
{
    int32          iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*  MsgPtr = 0;
    CFE_SB_MsgId_t MsgId;

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
                uint64 timestamp;

                ProcessDataPipe();

                /* Update status in caution and warning */
                m_caws.SetStatus(&VehicleStatusMsg);

                /* Cyclic maintainance loop */
                Execute();

                /* Get a common timestamp. */
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
                SendActuatorArmedMsg();
                SendVehicleStatusMsg();

                break;
            }

            case VM_SEND_HK_MID:
            {
                ProcessCmdPipe();
                ReportHousekeeping();
                break;
            }

            default:
            {
                (void) CFE_EVS_SendEvent(VM_MSGID_ERR_EID, CFE_EVS_ERROR,
                    "Recvd invalid SCH msgId (0x%04X)", MsgId);
            }
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
                case VM_CMD_MID:
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
                    (void) CFE_EVS_SendEvent(VM_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            (void) CFE_EVS_SendEvent(VM_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                    "CMD pipe read error (0x%08lX)", iStatus);
            break;
        }
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Data Messages                                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM::ProcessDataPipe()
{
    int32 iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t* MsgPtr = NULL;
    CFE_SB_MsgId_t MsgId;

    /* Process command messages until the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&MsgPtr, DataPipeId, CFE_SB_POLL);
        if (iStatus == CFE_SUCCESS)
        {
            MsgId = CFE_SB_GetMsgId(MsgPtr);
            switch (MsgId)
            {
                case PX4_SENSOR_MAG_MID:
                {
                    CFE_PSP_MemCpy(&SensorMagMsg, MsgPtr, sizeof(SensorMagMsg));
                    HkTlm.SensorMagMsgCount++;
                    break;
                }

                case PX4_SENSOR_GYRO_MID:
                {
                    CFE_PSP_MemCpy(&SensorGyroMsg, MsgPtr, sizeof(SensorGyroMsg));
                    HkTlm.SensorGyroMsgCount++;
                    break;
                }

                case PX4_BATTERY_STATUS_MID:
                {
                    CFE_PSP_MemCpy(&BatteryStatusMsg, MsgPtr, sizeof(BatteryStatusMsg));
                    HkTlm.BatteryStatusMsgCount++;
                    break;
                }

                case PX4_VEHICLE_GLOBAL_POSITION_MID:
                {
                    CFE_PSP_MemCpy(&VehicleGlobalPositionMsg, MsgPtr, sizeof(VehicleGlobalPositionMsg));
                    HkTlm.VehicleGlobalPositionMsgCount++;
                    break;
                }

                case PX4_TELEMETRY_STATUS_MID:
                {
                    CFE_PSP_MemCpy(&TelemetryStatusMsg, MsgPtr, sizeof(TelemetryStatusMsg));
                    HkTlm.TelemetryStatusMsgCount++;
                    break;
                }

                case PX4_SUBSYSTEM_INFO_MID:
                {
                    CFE_PSP_MemCpy(&SubsystemInfoMsg, MsgPtr, sizeof(SubsystemInfoMsg));
                    HkTlm.SubsystemInfoMsgCount++;
                    break;
                }

                case PX4_VEHICLE_GPS_POSITION_MID:
                {
                    CFE_PSP_MemCpy(&VehicleGpsPositionMsg, MsgPtr, sizeof(VehicleGpsPositionMsg));
                    HkTlm.VehicleGpsPositionMsgCount++;
                    break;
                }

                case PX4_VEHICLE_ATTITUDE_MID:
                {
                    CFE_PSP_MemCpy(&VehicleAttitudeMsg, MsgPtr, sizeof(VehicleAttitudeMsg));
                    HkTlm.VehicleAttitudeMsgCount++;
                    break;
                }

                case PX4_VEHICLE_LOCAL_POSITION_MID:
                {
                    CFE_PSP_MemCpy(&VehicleLocalPositionMsg, MsgPtr, sizeof(VehicleLocalPositionMsg));
                    HkTlm.VehicleLocalPositionMsgCount++;
                    break;
                }

                case PX4_VEHICLE_LAND_DETECTED_MID:
                {
                    CFE_PSP_MemCpy(&VehicleLandDetectedMsg, MsgPtr, sizeof(VehicleLandDetectedMsg));
                    HkTlm.VehicleLandDetectedMsgCount++;
                    break;
                }

                case PX4_MISSION_RESULT_MID:
                {
                    CFE_PSP_MemCpy(&MissionResultMsg, MsgPtr, sizeof(MissionResultMsg));
                    HkTlm.MissionResultMsgCount++;
                    break;
                }

                case PX4_MANUAL_CONTROL_SETPOINT_MID:
                {
                    CFE_PSP_MemCpy(&ManualControlSetpointMsg, MsgPtr, sizeof(ManualControlSetpointMsg));
                    HkTlm.ManualControlSetpointMsgCount++;
                    break;
                }

                case PX4_POSITION_SETPOINT_TRIPLET_MID:
                {
                    CFE_PSP_MemCpy(&PositionSetpointTripletMsg, MsgPtr, sizeof(PositionSetpointTripletMsg));
                    HkTlm.PositionSetpointTripletMsgCount++;
                    break;
                }

                case PX4_SENSOR_ACCEL_MID:
                {
                    CFE_PSP_MemCpy(&SensorAccelMsg, MsgPtr, sizeof(SensorAccelMsg));
                    HkTlm.SensorAccelMsgCount++;
                    break;
                }

                case PX4_SAFETY_MID:
                {
                    CFE_PSP_MemCpy(&SafetyMsg, MsgPtr, sizeof(SafetyMsg));
                    HkTlm.SafetyMsgCount++;
                    break;
                }

                case PX4_SENSOR_CORRECTION_MID:
                {
                    CFE_PSP_MemCpy(&SensorCorrectionMsg, MsgPtr, sizeof(SensorCorrectionMsg));
                    HkTlm.SensorCorrectionMsgCount++;
                    break;
                }

                case PX4_VEHICLE_CONTROL_MODE_MID:
                {
                    CFE_PSP_MemCpy(&VehicleControlModeMsg, MsgPtr, sizeof(VehicleControlModeMsg));
                    HkTlm.VehicleControlModeMsgCount++;
                    break;
                }

                case PX4_SENSOR_COMBINED_MID:
                {
                    CFE_PSP_MemCpy(&SensorCombinedMsg, MsgPtr, sizeof(SensorCombinedMsg));
                    HkTlm.SensorCombinedMsgCount++;
                    break;
                }

                default:
                {
                    (void) CFE_EVS_SendEvent(VM_MSGID_ERR_EID, CFE_EVS_ERROR,
                        "Recvd invalid DATA msgId (0x%04X)",
                        (unsigned short) MsgId);
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
            (void) CFE_EVS_SendEvent(VM_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                    "DATA pipe read error (0x%08lX)", iStatus);
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
    uint32 uiCmdCode = 0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);

        switch (uiCmdCode)
        {
            case VM_NOOP_CC:
            {
                HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(VM_CMD_NOOP_EID, CFE_EVS_INFORMATION,
                    "Recvd NOOP. Version %d.%d.%d.%d",
                    VM_MAJOR_VERSION,
                    VM_MINOR_VERSION,
                    VM_REVISION,
                    VM_MISSION_REV);
                break;
            }

            case VM_RESET_CC:
            {
                HkTlm.usCmdCnt                        = 0;
                HkTlm.usCmdErrCnt                     = 0;
                HkTlm.WakeupCount                     = 0;
                HkTlm.SensorMagMsgCount               = 0;
                HkTlm.SensorGyroMsgCount              = 0;
                HkTlm.BatteryStatusMsgCount           = 0;
                HkTlm.TelemetryStatusMsgCount         = 0;
                HkTlm.SubsystemInfoMsgCount           = 0;
                HkTlm.VehicleAttitudeMsgCount         = 0;
                HkTlm.VehicleLocalPositionMsgCount    = 0;
                HkTlm.VehicleLandDetectedMsgCount     = 0;
                HkTlm.MissionResultMsgCount           = 0;
                HkTlm.ManualControlSetpointMsgCount   = 0;
                HkTlm.PositionSetpointTripletMsgCount = 0;
                HkTlm.SensorAccelMsgCount             = 0;
                HkTlm.SafetyMsgCount                  = 0;
                HkTlm.SensorCorrectionMsgCount        = 0;
                HkTlm.VehicleControlModeMsgCount      = 0;
                HkTlm.SensorCombinedMsgCount          = 0;
                HkTlm.VehicleCommandMsgCount          = 0;
                HkTlm.VehicleGlobalPositionMsgCount   = 0;
                HkTlm.VehicleGpsPositionMsgCount      = 0;
                break;
            }

            case VM_VEHICLE_ARM_CC:
            {
                try
                {
                    ArmingSM.FSM.Arm();
                    HkTlm.usCmdCnt++;
                }
                catch (statemap::TransitionUndefinedException e)
                {
                    HkTlm.usCmdErrCnt++;
                    CFE_EVS_SendEvent(VM_ARMING_ILLEGAL_TRANSITION_ERR_EID,
                        CFE_EVS_INFORMATION,
                        "Illegal Arming transition. [ARM] Command rejected.");
                }
                break;
            }

            case VM_VEHICLE_DISARM_CC:
            {
                try
                {
                    ArmingSM.FSM.Disarm();
                    HkTlm.usCmdCnt++;
                }
                catch (statemap::TransitionUndefinedException e)
                {
                    HkTlm.usCmdErrCnt++;
                    CFE_EVS_SendEvent(VM_ARMING_ILLEGAL_TRANSITION_ERR_EID,
                        CFE_EVS_INFORMATION,
                        "Illegal Arming transition. [DISARM] Command rejected.");
                }
                break;
            }

            case VM_SET_NAV_MANUAL_CC:
            {
                try
                {
                    NavigationSM.FSM.trManual();
                    HkTlm.usCmdCnt++;
                }
                catch (statemap::TransitionUndefinedException e)
                {
                    HkTlm.usCmdErrCnt++;
                    uint32 PrevState = NavigationSM.GetCurrentStateID();
                    CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                        CFE_EVS_ERROR,
                        "Illegal Nav transition [%s -> MANUAL].  Command rejected.",
                        GetNavStateAsString(PrevState));
                }
                break;
            }

            case VM_SET_NAV_ALTCTL_CC:
            {
                try
                {
                    NavigationSM.FSM.trAltitudeControl();
                    HkTlm.usCmdCnt++;
                }
                catch (statemap::TransitionUndefinedException e)
                {
                    HkTlm.usCmdErrCnt++;
                    uint32 PrevState = NavigationSM.GetCurrentStateID();
                    CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                        CFE_EVS_ERROR,
                        "Illegal Nav transition [%s -> ALTCTL].  Command rejected.",
                        GetNavStateAsString(PrevState));
                }
                break;
            }

            case VM_SET_NAV_POSCTL_CC:
            {
                try
                {
                    NavigationSM.FSM.trPositionControl();
                    HkTlm.usCmdCnt++;
                }
                catch (statemap::TransitionUndefinedException e)
                {
                    HkTlm.usCmdErrCnt++;
                    uint32 PrevState = NavigationSM.GetCurrentStateID();
                    CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                        CFE_EVS_ERROR,
                        "Illegal Nav transition [%s -> POSCTL].  Command rejected.",
                        GetNavStateAsString(PrevState));
                }
                break;
            }

            case VM_SET_NAV_AUTO_LOITER_CC:
            {
                try
                {
                    NavigationSM.FSM.trAutoLoiter();
                    HkTlm.usCmdCnt++;
                }
                catch (statemap::TransitionUndefinedException e)
                {
                    HkTlm.usCmdErrCnt++;
                    uint32 PrevState = NavigationSM.GetCurrentStateID();
                    CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                        CFE_EVS_ERROR,
                        "Illegal Nav transition [%s -> AUTORTL].  Command rejected.",
                        GetNavStateAsString(PrevState));
                }
                break;
            }

            case VM_SET_NAV_AUTO_RTL_CC:
            {
                try
                {
                    NavigationSM.FSM.trAutoReturnToLaunch();
                    HkTlm.usCmdCnt++;
                }
                catch (statemap::TransitionUndefinedException e)
                {
                    HkTlm.usCmdErrCnt++;
                    uint32 PrevState = NavigationSM.GetCurrentStateID();
                    CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                        CFE_EVS_ERROR,
                        "Illegal Nav transition [%s -> AUTOLTR].  Command rejected.",
                        GetNavStateAsString(PrevState));
                }
                break;
            }

            case VM_SET_NAV_ACRO_CC:
            {
                try
                {
                    NavigationSM.FSM.trAcrobatic();
                    HkTlm.usCmdCnt++;
                }
                catch (statemap::TransitionUndefinedException e)
                {
                    HkTlm.usCmdErrCnt++;
                    uint32 PrevState = NavigationSM.GetCurrentStateID();
                    CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                        CFE_EVS_ERROR,
                        "Illegal Nav transition [%s -> ACRO].  Command rejected.",
                        GetNavStateAsString(PrevState));
                }
                break;
            }

            case VM_SET_NAV_STABILIZE_CC:
            {
                try
                {
                    NavigationSM.FSM.trStabilize();
                    HkTlm.usCmdCnt++;
                }
                catch (statemap::TransitionUndefinedException e)
                {
                    HkTlm.usCmdErrCnt++;
                    uint32 PrevState = NavigationSM.GetCurrentStateID();
                    CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                        CFE_EVS_ERROR,
                        "Illegal Nav transition [%s -> STAB].  Command rejected.",
                        GetNavStateAsString(PrevState));
                }
                break;
            }

            case VM_SET_NAV_RATTITUDE_CC:
            {
                try
                {
                    NavigationSM.FSM.trRattitude();
                    HkTlm.usCmdCnt++;
                }
                catch (statemap::TransitionUndefinedException e)
                {
                    HkTlm.usCmdErrCnt++;
                    uint32 PrevState = NavigationSM.GetCurrentStateID();
                    CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                        CFE_EVS_ERROR,
                        "Illegal Nav transition [%s -> RATT].  Command rejected.",
                        GetNavStateAsString(PrevState));
                }
                break;
            }

            case VM_SET_NAV_AUTO_TAKEOFF_CC:
            {
                try
                {
                    NavigationSM.FSM.trAutoTakeoff();
                    HkTlm.usCmdCnt++;
                }
                catch (statemap::TransitionUndefinedException e)
                {
                    HkTlm.usCmdErrCnt++;
                    uint32 PrevState = NavigationSM.GetCurrentStateID();
                    CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                        CFE_EVS_ERROR,
                        "Illegal Nav transition [%s -> AUTOTAKOF].  Command rejected.",
                        GetNavStateAsString(PrevState));
                }
                break;
            }

            case VM_SET_NAV_AUTO_LAND_CC:
            {
                try
                {
                    NavigationSM.FSM.trAutoLand();
                    HkTlm.usCmdCnt++;
                }
                catch (statemap::TransitionUndefinedException e)
                {
                    HkTlm.usCmdErrCnt++;
                    uint32 PrevState = NavigationSM.GetCurrentStateID();
                    CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                        CFE_EVS_ERROR,
                        "Illegal Nav transition [%s -> AUTOLND].  Command rejected.",
                        GetNavStateAsString(PrevState));
                }
                break;
            }

            case VM_SEND_CONFIGURATION_CC:
            {
                HkTlm.usCmdCnt++;
                ReportConfiguration();
                break;
            }

            default:
            {
                HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(VM_CC_ERR_EID, CFE_EVS_ERROR,
                    "Recvd invalid command code (%u)",
                    (unsigned int) uiCmdCode);
                break;
            }
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

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*) &HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*) &HkTlm);
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Publish Output Data                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM::SendActuatorArmedMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*) &ActuatorArmedMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*) &ActuatorArmedMsg);
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  Send HomePositionMsg                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM::SendHomePositionMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*) &HomePositionMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*) &HomePositionMsg);
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  Send VehicleManagerStateMsg                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM::SendVehicleManagerStateMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*) &VehicleManagerStateMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*) &VehicleManagerStateMsg);
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  Send MissionMsg                                                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM::SendMissionMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*) &MissionMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*) &MissionMsg);
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  Send LedControlMsg                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM::SendLedControlMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*) &LedControlMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*) &LedControlMsg);
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  Send VehicleStatusMsg                                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM::SendVehicleStatusMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*) &VehicleStatusMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*) &VehicleStatusMsg);
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  Send VehicleControlModeMsg                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM::SendVehicleControlModeMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*) &VehicleControlModeMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*) &VehicleControlModeMsg);

}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  Send VehicleCommandMsg                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM::SendVehicleCommandMsg()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*) &VehicleCommandMsg);
    CFE_SB_SendMsg((CFE_SB_Msg_t*) &VehicleCommandMsg);
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Command Length                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean VM::VerifyCmdLength(CFE_SB_Msg_t* MsgPtr, uint16 usExpectedLen)
{
    boolean bResult = TRUE;
    uint16 usMsgLen = 0;

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
        (void) CFE_ES_WriteToSysLog(
                "VM - Failed to register the app (0x%08lX)\n", iStatus);
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

        /* Initialize state machine */
        ArmingSM.FSM.InitComplete();
        NavigationSM.FSM.trInitComplete();

        Initialization();
        SetHomePosition();
    }
    else
    {
        uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&uiRunStatus) == TRUE)
    {
        RcvSchPipeMsg(VM_SCH_PIPE_PEND_TIME);

        iStatus = AcquireConfigPointers();
        if (iStatus != CFE_SUCCESS)
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



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  Is Vehicle Armed?                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool VM::IsVehicleArmed()
{
    return ActuatorArmedMsg.Armed;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  Time Elapsed                                                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
uint64 VM::TimeElapsed(uint64 *TimePtr)
{
    uint64 now = PX4LIB_GetPX4TimeUs();
    uint64 delta = now - *TimePtr;

    return delta;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  Time Now                                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
uint64 VM::TimeNow()
{
    uint64 now = PX4LIB_GetPX4TimeUs();

    return now;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  Set Home Position                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM::SetHomePosition()
{
    if (!(VehicleGlobalPositionMsg.EpH > ConfigTblPtr->COM_HOME_H_T ||
          VehicleGlobalPositionMsg.EpV > ConfigTblPtr->COM_HOME_V_T))
    {
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
        HomePositionMsg.Alt = VehicleGlobalPositionMsg.Alt - DistBottom - ConfigTblPtr->HOME_POS_ALT_PADDING;
        HomePositionMsg.X = VehicleLocalPositionMsg.X;
        HomePositionMsg.Y = VehicleLocalPositionMsg.Y;
        HomePositionMsg.Z = VehicleLocalPositionMsg.Z;

        math::Quaternion q(VehicleAttitudeMsg.Q[0], VehicleAttitudeMsg.Q[1],
                VehicleAttitudeMsg.Q[2], VehicleAttitudeMsg.Q[3]);

        math::Matrix3F3 rotationMat = math::Dcm(q);
        math::Vector3F euler = math::Euler(rotationMat);
        HomePositionMsg.Yaw = euler[2];

        (void) CFE_EVS_SendEvent(VM_HOMESET_INFO_EID, CFE_EVS_INFORMATION,
                "Home Position set. [Lat -> %.6f | Lon -> %.6f | Alt ->%.6f]",
                HomePositionMsg.Lat, HomePositionMsg.Lon, HomePositionMsg.Alt);

        SendHomePositionMsg();
    }
    else
    {
        (void) CFE_EVS_SendEvent(VM_HOMESET_INFO_EID, CFE_EVS_ERROR,
                "Home position cannot be set.");
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  Initialization                                                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM::Initialization()
{
    /* Initialize status flags */
    HkTlm.StatusFlags.SensorsInitialized = true;

    /* Always accept RC input as default */
    HkTlm.StatusFlags.RcInputIsTemporarilyBlocked = false;
    HkTlm.StatusFlags.RcSignalFoundOnce = false;

    HkTlm.StatusFlags.UsbPowerConnected = false;

    /* Vehicle status defaults */
    VehicleStatusMsg.OnboardControlSensorsPresent = 0;
    VehicleStatusMsg.OnboardControlSensorsEnabled = 0;
    VehicleStatusMsg.OnboardControlSensorsHealth = 0;

    if (ConfigTblPtr->COM_RC_IN_MODE == 2)
    {
        VehicleStatusMsg.RcInputMode = PX4_RcInMode_t::PX4_RC_IN_MODE_GENERATED;
    }

    VehicleStatusMsg.HilState = PX4_HIL_STATE_OFF;
    VehicleStatusMsg.Failsafe = false;
    VehicleStatusMsg.SystemType = PX4_SYSTEM_TYPE_HEXAROTOR;
    if(VM_VEHICLE_TYPE_ROTARY_WING == ConfigTblPtr->VEHICLE_TYPE)
    {
        VehicleStatusMsg.IsRotaryWing = true;
    }
    else
    {
        VehicleStatusMsg.IsRotaryWing = false;
    }

    if(VM_VEHICLE_TYPE_VTOL == ConfigTblPtr->VEHICLE_TYPE)
    {
        VehicleStatusMsg.IsVtol = true;
    }
    else
    {
        VehicleStatusMsg.IsVtol = false;
    }

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
    ConfigTblPtr->COM_RC_ARM_HYST *= COMMANDER_MONITORING_LOOPSPERMSEC;

    HkTlm.BootTimestamp = TimeNow();

    /* update parameters */
    if (!ActuatorArmedMsg.Armed)
    {
        VehicleStatusMsg.SystemID = ConfigTblPtr->MAV_SYS_ID;
        VehicleStatusMsg.ComponentID = ConfigTblPtr->MAV_COMP_ID;
    }

}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  Execute                                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM::Execute()
{
    /* Set home position at launch */
    if (VehicleLandDetectedMsg.Landed && !VehicleLandDetectedMsg.Freefall &&
        !VehicleLandDetectedMsg.GroundContact)
    {
        /* Vehicle has landed */
        if (!HkTlm.PrevLanded)
        {
            /* Rest arm and nav states */
            FlightSessionInit();
            /* Forget previous modes */
            HkTlm.PreviousModes = {0};
        }
        HkTlm.PrevLanded = true;
        HkTlm.PrevInFlight = false;
    }
    else if(!VehicleLandDetectedMsg.Landed && !VehicleLandDetectedMsg.Freefall && !VehicleLandDetectedMsg.GroundContact && HkTlm.PrevLanded && !HkTlm.PrevInFlight )
    {
        /* Vehicle is launched */
        SetHomePosition();
        HkTlm.PrevLanded = false;
        HkTlm.PrevInFlight = true;
    }

    /* Vehicle status message handle */
    VehicleStatusMsg.SystemID = ConfigTblPtr->MAV_SYS_ID;
    VehicleStatusMsg.ComponentID = ConfigTblPtr->MAV_COMP_ID;

    /* Safety message handle */
    osalbool previous_safety_off = SafetyMsg.SafetyOff;
    if(SafetyMsg.SafetySwitchAvailable && !SafetyMsg.SafetyOff && ActuatorArmedMsg.Armed && VehicleStatusMsg.ArmingState == PX4_ArmingState_t::PX4_ARMING_STATE_ARMED)
    {
        try
        {
            (void) CFE_EVS_SendEvent(VM_SAFETY_DISARM_INFO_EID, CFE_EVS_INFORMATION,
                    "Vehicle [DISARM] by safety message");
            ArmingSM.FSM.Disarm();
            HkTlm.usCmdCnt++;
            HkTlm.ArmingStateChanged = true;

        }
        catch(statemap::TransitionUndefinedException e)
        {
            HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(VM_ARMING_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_ERROR,
                    "Illegal Arming transition. [%s] Command rejected.","DISARM");
        }
    }

    /* Notify the user if the status of the safety switch changes*/
    if (SafetyMsg.SafetySwitchAvailable && previous_safety_off != SafetyMsg.SafetyOff)
    {
        if (SafetyMsg.SafetyOff)
        {
            (void) CFE_EVS_SendEvent(VM_SAFETY_INFO_EID, CFE_EVS_INFORMATION,
                    "Safety OFF");
        }
        else
        {
            (void) CFE_EVS_SendEvent(VM_SAFETY_INFO_EID, CFE_EVS_INFORMATION,
                    "Safety ON");
        }
    }

    /* Battery status handle */
    /* Only consider battery voltage if system has been running 6s (usb most likely detected) and battery voltage is valid */
    if((TimeNow() > HkTlm.BootTimestamp + VM_BATTERY_VOLTAGE_CHECK_DELAY) && (BatteryStatusMsg.VoltageFiltered > (VM_MINIMUM_VALID_BATTERY_VOLTAGE * FLT_EPSILON)))
    {
        /* If battery voltage is getting lower, warn using buzzer, etc. */
        if(BatteryStatusMsg.Warning == PX4_BATTERY_WARNING_LOW && !HkTlm.LowBatteryVoltageActionsDone)
        {
            HkTlm.LowBatteryVoltageActionsDone = true;

            if (ActuatorArmedMsg.Armed)
            {
                (void) CFE_EVS_SendEvent(VM_LOW_BAT_INFO_EID, CFE_EVS_INFORMATION,
                        "Low battery, return to land");
            }
            else
            {
                (void) CFE_EVS_SendEvent(VM_LOW_BAT_INFO_EID, CFE_EVS_INFORMATION,
                        "Low battery, takeoff discouraged");
            }
        }
        else if (!HkTlm.StatusFlags.UsbPowerConnected && BatteryStatusMsg.Warning == PX4_BATTERY_WARNING_CRITICAL && !HkTlm.CriticalBatteryVoltageActionsDone)
        {
            HkTlm.CriticalBatteryVoltageActionsDone = true;

            if (!ActuatorArmedMsg.Armed)
            {
                (void) CFE_EVS_SendEvent(VM_CRITICAL_BAT_INFO_EID, CFE_EVS_INFORMATION,
                        "Critical battery, shutdown system");
            }
            else
            {
                if(
                        ConfigTblPtr->COM_LOW_BAT_ACT == VM_BATTERY_FAILSAFE_MODE_RETURN ||
                        ConfigTblPtr->COM_LOW_BAT_ACT == VM_BATTERY_FAILSAFE_MODE_RETURN_IF_CRIT_LOW_LAND_IF_DANGER_LOW)
                {
                    try
                    {
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
                else if(ConfigTblPtr->COM_LOW_BAT_ACT == VM_BATTERY_FAILSAFE_MODE_LAND)
                {
                    try
                    {
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
                else
                {
                    (void) CFE_EVS_SendEvent(VM_CRITICAL_BAT_INFO_EID, CFE_EVS_INFORMATION,
                            "Critical battery, return to launch advised");
                }
            }
        }
        else if(!HkTlm.StatusFlags.UsbPowerConnected && BatteryStatusMsg.Warning == PX4_BATTERY_WARNING_EMERGENCY && !HkTlm.EmergencyBatteryVoltageActionsDone)
        {
            HkTlm.EmergencyBatteryVoltageActionsDone = true;

            if (!ActuatorArmedMsg.Armed)
            {
                (void) CFE_EVS_SendEvent(VM_DANGER_BAT_LEVEL_INFO_EID, CFE_EVS_INFORMATION,
                        "Dangerously low battery, shutdown system");
            }
            else
            {
                if(
                        ConfigTblPtr->COM_LOW_BAT_ACT == VM_BATTERY_FAILSAFE_MODE_LAND ||
                        ConfigTblPtr->COM_LOW_BAT_ACT == VM_BATTERY_FAILSAFE_MODE_RETURN_IF_CRIT_LOW_LAND_IF_DANGER_LOW)
                {
                    try
                    {
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
                else
                {
                    (void) CFE_EVS_SendEvent(VM_DANGER_BAT_LEVEL_INFO_EID, CFE_EVS_INFORMATION,
                            "Dangerously low battery, landing advised");
                }
            }
        }
    }

    /* Subsystem message handle */
    if(SubsystemInfoMsg.Present)
    {
        VehicleStatusMsg.OnboardControlSensorsPresent |= SubsystemInfoMsg.SubsystemType;
    }
    else
    {
        VehicleStatusMsg.OnboardControlSensorsPresent &= ~SubsystemInfoMsg.SubsystemType;
    }

    if(SubsystemInfoMsg.Enabled)
    {
        VehicleStatusMsg.OnboardControlSensorsEnabled |= SubsystemInfoMsg.SubsystemType;
    }
    else
    {
        VehicleStatusMsg.OnboardControlSensorsEnabled &= ~SubsystemInfoMsg.SubsystemType;
    }

    if(SubsystemInfoMsg.Ok)
    {
        VehicleStatusMsg.OnboardControlSensorsHealth |= SubsystemInfoMsg.SubsystemType;
    }
    else
    {
        VehicleStatusMsg.OnboardControlSensorsHealth &= ~SubsystemInfoMsg.SubsystemType;
    }

    /* RC input handle */
    if(!HkTlm.StatusFlags.RcInputIsTemporarilyBlocked && ManualControlSetpointMsg.Timestamp!=0 && (TimeNow() < ManualControlSetpointMsg.Timestamp + uint64(ConfigTblPtr->COM_RC_LOSS_T * 1e6f)))
    {
        const osalbool in_armed_state = (VehicleStatusMsg.ArmingState == PX4_ARMING_STATE_ARMED || VehicleStatusMsg.ArmingState == PX4_ARMING_STATE_ARMED_ERROR);
        const osalbool arm_button_pressed = (ConfigTblPtr->COM_ARM_SWISBTN == 1 && ManualControlSetpointMsg.ArmSwitch == PX4_SWITCH_POS_ON);

        /* DISARM */
        const osalbool stick_in_lower_left = false; //((ManualControlSetpointMsg.R < -STICK_ON_OFF_LIMIT) && (ManualControlSetpointMsg.Z <0.1f));
        const osalbool arm_switch_to_disarm_transition = ((ConfigTblPtr->COM_ARM_SWISBTN == 0) && (HkTlm.LastSpManArmSwitch == PX4_SWITCH_POS_ON) && (ManualControlSetpointMsg.ArmSwitch == PX4_SWITCH_POS_OFF) );

        /* ARM */
        const osalbool stick_in_lower_right = false; // (ManualControlSetpointMsg.R > STICK_ON_OFF_LIMIT  && ManualControlSetpointMsg.Z <0.1f);
        const osalbool arm_switch_to_arm_transition = ((ConfigTblPtr->COM_ARM_SWISBTN == 0) && (HkTlm.LastSpManArmSwitch ==PX4_SWITCH_POS_OFF) && (ManualControlSetpointMsg.ArmSwitch == PX4_SWITCH_POS_ON) );

        if(!HkTlm.StatusFlags.RcSignalFoundOnce)
        {
            HkTlm.StatusFlags.RcSignalFoundOnce = true;
        }
        else
        {
            if(VehicleStatusMsg.RcSignalLost)
            {
                uint64 Now = TimeNow();
                (void) CFE_EVS_SendEvent(VM_RC_SIGN_REGAINED_INFO_EID, CFE_EVS_INFORMATION,
                        "Manual control regained after (%llu)ums",(Now - HkTlm.RCSignalLostTimestamp));
            }
        }

        VehicleStatusMsg.RcSignalLost = false;

        if(in_armed_state && VehicleStatusMsg.RcInputMode != PX4_RC_IN_MODE_OFF && (stick_in_lower_left || arm_button_pressed || arm_switch_to_disarm_transition))
        {
            //current_nav_state = NavigationSM.GetCurrentStateID();//TODO
            if(VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_MANUAL &&
                    VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_ACRO &&
                    VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_STAB &&
                    VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_RATTITUDE &&
                    !VehicleLandDetectedMsg.Landed)
            {
                (void) CFE_EVS_SendEvent(VM_RC_STK_DISARM_REJ_INFO_EID, CFE_EVS_INFORMATION,
                        "Stick disarm rejected, vehicle in flight");
            }
            else if ((HkTlm.StickOffCounter == ConfigTblPtr->COM_RC_ARM_HYST && HkTlm.StickOnCounter < ConfigTblPtr->COM_RC_ARM_HYST) || arm_switch_to_disarm_transition)
            {
                try
                {
                    (void) CFE_EVS_SendEvent(VM_RC_DISARM_INFO_EID, CFE_EVS_INFORMATION,
                            "Disarm engaged by rc");
                    ArmingSM.FSM.Disarm();
                    HkTlm.ArmingStateChanged = true;
                    HkTlm.usCmdCnt++;
                }
                catch(statemap::TransitionUndefinedException e)
                {
                    HkTlm.usCmdErrCnt++;
                    CFE_EVS_SendEvent(VM_ARMING_ILLEGAL_TRANSITION_ERR_EID, CFE_EVS_INFORMATION,
                            "Illegal Arming transition. [%s] Command rejected.","DISARM");
                }
            }
            HkTlm.StickOffCounter++;
        }
        /* do not reset the counter when holding the arm button longer than needed */
        else if (!(ConfigTblPtr->COM_ARM_SWISBTN ==1 && ManualControlSetpointMsg.ArmSwitch == PX4_SWITCH_POS_ON))
        {
            HkTlm.StickOffCounter = 0;
        }

        if(!in_armed_state && VehicleStatusMsg.RcInputMode != PX4_RC_IN_MODE_OFF && (stick_in_lower_right || arm_button_pressed || arm_switch_to_arm_transition))
        {
            if((HkTlm.StickOnCounter == ConfigTblPtr->COM_RC_ARM_HYST && HkTlm.StickOffCounter < ConfigTblPtr->COM_RC_ARM_HYST) || arm_switch_to_arm_transition)
            {
                if((VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_MANUAL) &&
                        (VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_ACRO) &&
                        (VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_STAB) &&
                        (VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_RATTITUDE) &&
                        (VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_POSCTL) &&
                        (VehicleStatusMsg.NavState != PX4_NAVIGATION_STATE_ALTCTL))
                {
                    (void) CFE_EVS_SendEvent(VM_RC_STK_ARM_REJ_INFO_EID, CFE_EVS_INFORMATION,
                            "Stick arm rejected, vehicle not in manual mode ");
                }
                else if (VehicleStatusMsg.ArmingState == PX4_ARMING_STATE_STANDBY)
                {
                    try
                    {
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

                    HkTlm.ArmingStateChanged = true;
                }
            }
            HkTlm.StickOnCounter++;
        }
        /* do not reset the counter when holding the arm button longer than needed */
        else if (!(ConfigTblPtr->COM_ARM_SWISBTN ==1 && ManualControlSetpointMsg.ArmSwitch == PX4_SWITCH_POS_ON))
        {
            HkTlm.StickOnCounter = 0;
        }

        HkTlm.LastSpManArmSwitch = ManualControlSetpointMsg.ArmSwitch;

        /* KILLSWITCH */
        if(ManualControlSetpointMsg.KillSwitch == PX4_SWITCH_POS_ON)
        {
            if(!ActuatorArmedMsg.ManualLockdown)
            {
                (void) CFE_EVS_SendEvent(VM_RC_KIL_SWTCH_INFO_EID, CFE_EVS_INFORMATION,
                        "Killswitch engaged ");
                ActuatorArmedMsg.ManualLockdown = true;
            }
        }
        else if(ManualControlSetpointMsg.KillSwitch == PX4_SWITCH_POS_OFF)
        {
            if(ActuatorArmedMsg.ManualLockdown)
            {
                (void) CFE_EVS_SendEvent(VM_RC_KIL_SWTCH_INFO_EID, CFE_EVS_INFORMATION,
                        "killswitch disengaged ");
                ActuatorArmedMsg.ManualLockdown = false;
            }
        }

        /* Mode change routine */
        RcModes();
    }
    else if(!HkTlm.StatusFlags.RcInputIsTemporarilyBlocked && !VehicleStatusMsg.RcSignalLost )
    {
        uint64 Now = TimeNow();
        (void) CFE_EVS_SendEvent(VM_RC_SIGN_LOST_INFO_EID, CFE_EVS_INFORMATION,
                "Manual control lost at t = (%llu)ums", (Now));
        try
        {
            if(VehicleStatusMsg.NavState != PX4_NavigationState_t::PX4_NAVIGATION_STATE_AUTO_RTL)
            {
                NavigationSM.FSM.trAutoLoiter();
                HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(VM_RC_MAN_INFO_EID, CFE_EVS_INFORMATION,
                        "Mode switched to auto loiter autonomously ");
            }
            else
            {
                (void) CFE_EVS_SendEvent(VM_RC_MAN_INFO_EID, CFE_EVS_INFORMATION,
                        "Mode switched will stay in RTL ");
            }
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
        HkTlm.RCSignalLostTimestamp = ManualControlSetpointMsg.Timestamp;
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  RC Modes                                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM::RcModes()
{
    osalbool posctl = (ManualControlSetpointMsg.PosctlSwitch == PX4_SWITCH_POS_ON);
    osalbool altctl = (ManualControlSetpointMsg.AltctlSwitch == PX4_SWITCH_POS_ON);
    osalbool rtl = (ManualControlSetpointMsg.ReturnSwitch == PX4_SWITCH_POS_ON);
    osalbool loiter = (ManualControlSetpointMsg.LoiterSwitch == PX4_SWITCH_POS_ON);
    osalbool takeoff = (ManualControlSetpointMsg.TransitionSwitch == PX4_SWITCH_POS_ON);
    osalbool manual = (!posctl && !rtl && !loiter && !takeoff);
    osalbool mode_changed = !(posctl == HkTlm.PreviousModes.inPosCtl &&
                              rtl == HkTlm.PreviousModes.inRtl &&
                              loiter == HkTlm.PreviousModes.inLoiter &&
                              manual == HkTlm.PreviousModes.inManual &&
                              takeoff == HkTlm.PreviousModes.inTakeoff &&
                              altctl == HkTlm.PreviousModes.inAltCtl);

    if (posctl && !altctl && !rtl && !loiter && !takeoff && mode_changed)
    {
        try
        {
            NavigationSM.FSM.trPositionControl();
            HkTlm.usCmdCnt++;
            (void) CFE_EVS_SendEvent(VM_RC_POSCTL_INFO_EID, CFE_EVS_INFORMATION,
                    "Mode switched to position control by rc");
        }
        catch (statemap::TransitionUndefinedException e)
        {
            HkTlm.usCmdErrCnt++;
            uint32 PrevState = NavigationSM.GetCurrentStateID();
            CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                    CFE_EVS_INFORMATION,
                    "Illegal Nav transition [%s -> %s].  Command rejected.",
                    GetNavStateAsString(PrevState), "POSCTL");
        }

    }
    else if (rtl && mode_changed)
    {
        try
        {
            NavigationSM.FSM.trAutoReturnToLaunch();
            HkTlm.usCmdCnt++;
            (void) CFE_EVS_SendEvent(VM_RC_RTL_INFO_EID, CFE_EVS_INFORMATION,
                    "Mode switched to auto rtl by rc ");
        }
        catch (statemap::TransitionUndefinedException e)
        {
            HkTlm.usCmdErrCnt++;
            uint32 PrevState = NavigationSM.GetCurrentStateID();
            CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                    CFE_EVS_INFORMATION,
                    "Illegal Nav transition [%s -> %s].  Command rejected.",
                    GetNavStateAsString(PrevState), "AUTORTL");
        }

    }
    else if (!posctl && !altctl && !rtl && loiter && !takeoff && mode_changed)
    {
        try
        {
            NavigationSM.FSM.trAutoLoiter();
            HkTlm.usCmdCnt++;
            (void) CFE_EVS_SendEvent(VM_RC_LTR_INFO_EID, CFE_EVS_INFORMATION,
                    "Mode switched to auto loiter by rc");
        }
        catch (statemap::TransitionUndefinedException e)
        {
            HkTlm.usCmdErrCnt++;
            uint32 PrevState = NavigationSM.GetCurrentStateID();
            CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                    CFE_EVS_INFORMATION,
                    "Illegal Nav transition [%s -> %s].  Command rejected.",
                    GetNavStateAsString(PrevState), "AUTOLTR");
        }
    }
    else if (takeoff && mode_changed)
    {
        try
        {
            NavigationSM.FSM.trAutoTakeoff();
            HkTlm.usCmdCnt++;
            (void) CFE_EVS_SendEvent(VM_RC_TAKE_OFF_INFO_EID,
                    CFE_EVS_INFORMATION, "Mode switched to auto takeoff by rc");
        }
        catch (statemap::TransitionUndefinedException e)
        {
            HkTlm.usCmdErrCnt++;
            uint32 PrevState = NavigationSM.GetCurrentStateID();
            CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                    CFE_EVS_INFORMATION,
                    "Illegal Nav transition [%s -> %s].  Command rejected.",
                    GetNavStateAsString(PrevState), "AUTOTAKOF");
        }
    }
    else if (!posctl && altctl && !rtl && !loiter && !takeoff && mode_changed)
    {
        try
        {
            NavigationSM.FSM.trAltitudeControl();
            HkTlm.usCmdCnt++;
            (void) CFE_EVS_SendEvent(VM_RC_TAKE_OFF_INFO_EID,
                    CFE_EVS_INFORMATION, "Mode switched to altitude control by rc");
        }
        catch (statemap::TransitionUndefinedException e)
        {
            HkTlm.usCmdErrCnt++;
            uint32 PrevState = NavigationSM.GetCurrentStateID();
            CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                    CFE_EVS_INFORMATION,
                    "Illegal Nav transition [%s -> %s].  Command rejected.",
                    GetNavStateAsString(PrevState), "ALTCTL");
        }
    }
    else if (manual && mode_changed)
    {
        try
        {
            NavigationSM.FSM.trManual();
            HkTlm.usCmdCnt++;
            (void) CFE_EVS_SendEvent(VM_RC_MAN_INFO_EID, CFE_EVS_INFORMATION,
                    "Mode switched to Manual by rc");
        }
        catch (statemap::TransitionUndefinedException e)
        {
            HkTlm.usCmdErrCnt++;
            uint32 PrevState = NavigationSM.GetCurrentStateID();
            CFE_EVS_SendEvent(VM_NAV_ILLEGAL_TRANSITION_ERR_EID,
                    CFE_EVS_INFORMATION,
                    "Illegal Nav transition [%s -> %s].  Command rejected.",
                    GetNavStateAsString(PrevState), "MANUAL");
        }
    }
    else
    {
        /* Do nothing. */
    }

    HkTlm.PreviousModes.inPosCtl = posctl;
    HkTlm.PreviousModes.inAltCtl = altctl;
    HkTlm.PreviousModes.inRtl = rtl;
    HkTlm.PreviousModes.inLoiter = loiter;
    HkTlm.PreviousModes.inManual = manual;
    HkTlm.PreviousModes.inTakeoff = takeoff;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  Flight Session Initialization                                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM::FlightSessionInit()
{
    (void) CFE_EVS_SendEvent(VM_LND_INIT_INFO_EID, CFE_EVS_INFORMATION,
            "Flight states reset. [Navigation -> MANUAL | Arming -> STANDBY]");

    /* Push states to init */
    ArmingSM.FSM.Reset();
    NavigationSM.FSM.Reset();

    /* Transition from init to default state */
    ArmingSM.FSM.InitComplete();
    NavigationSM.FSM.trInitComplete();
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  Get Navigation State as String                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
const char* VM::GetNavStateAsString(uint32 id)
{
    const char * State_ptr;

    switch (id)
    {
        case 0:
        {
            State_ptr = "MANUAL";
            break;
        }

        case 1:
        {
            State_ptr = "ALTCTL";
            break;
        }

        case 2:
        {
            State_ptr = "POSCTL";
            break;
        }

        case 3:
        {
            State_ptr = "AUTORTL";
            break;
        }

        case 4:
        {
            State_ptr = "AUTOLTR";
            break;
        }

        case 5:
        {
            State_ptr = "ACRO";
            break;
        }

        case 6:
        {
            State_ptr = "STAB";
            break;
        }

        case 7:
        {
            State_ptr = "RATT";
            break;
        }

        case 8:
        {
            State_ptr = "AUTOTAKOF";
            break;
        }

        case 9:
        {
            State_ptr = "AUTOLND";
            break;
        }

        default:
        {
            State_ptr = "UNKNOWN";
            break;
        }
    }

    return State_ptr;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  Report Configuration                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM::ReportConfiguration()
{
    CFE_PSP_MemCpy(&ConfigTlm.ConfigTbl, ConfigTblPtr, sizeof(ConfigTlm.ConfigTbl));

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*) &ConfigTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*) &ConfigTlm);
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  onParamsValidate for uint32 values                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool VM::onParamValidate(void* Address, uint32 Value)
{
    osalbool valid;

    if (Address == &ConfigTblPtr->COM_RC_IN_MODE)
    {
        valid = VM::Validate_COM_RC_IN_MODE(Value);
    }
    else if (Address == &ConfigTblPtr->COM_ARM_SWISBTN)
    {
        valid = VM::Validate_COM_ARM_SWISBTN(Value);
    }
    else if (Address == &ConfigTblPtr->COM_RC_ARM_HYST)
    {
        valid = VM::Validate_COM_RC_ARM_HYST(Value);
    }
    else if (Address == &ConfigTblPtr->MAV_SYS_ID)
    {
        valid = VM::Validate_MAV_SYS_ID(Value);
    }
    else if (Address == &ConfigTblPtr->MAV_COMP_ID)
    {
        valid = VM::Validate_MAV_COMP_ID(Value);
    }
    else if (Address == &ConfigTblPtr->COM_LOW_BAT_ACT)
    {
        valid = VM::Validate_COM_LOW_BAT_ACT(Value);
    }
    else
    {
        /* Flag this as invalid. */
        valid = false;
    }

    return valid;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  onParamsValidate for float values                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool VM::onParamValidate(void* Address, float Value)
{
    osalbool valid;

    if (Address == &ConfigTblPtr->COM_RC_LOSS_T)
    {
        valid = VM::Validate_COM_RC_LOSS_T(Value);
    }
    else if (Address == &ConfigTblPtr->COM_HOME_H_T)
    {
        valid = VM::Validate_COM_HOME_H_T(Value);
    }
    else if (Address == &ConfigTblPtr->COM_HOME_V_T)
    {
        valid = VM::Validate_COM_HOME_V_T(Value);
    }
    else if (Address == &ConfigTblPtr->HOME_POS_ALT_PADDING)
    {
        valid = true;
    }
    else
    {
        /* Flag this as invalid. */
        valid = false;
    }

    return valid;
}

/************************/
/*  End of File Comment */
/************************/
