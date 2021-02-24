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
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "mac_app.hpp"
#include "mac_msg.h"
#include "mac_version.h"
#include "cfs_utils.h"
#include "px4lib_msgids.h"
#include <px4lib.h>
#include <math/Quaternion.hpp>
#include <string.h>
#include <errno.h>
#include <float.h>
#include <math.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Instantiate the application object.                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
MAC oMAC;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Default constructor.                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
MAC::MAC()
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Destructor constructor.                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
MAC::~MAC()
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables.                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MAC::InitEvent()
{
    int32  iStatus = CFE_SUCCESS;
    uint32  ind = 0;

    /* Initialize the event filter table.
     * Note: 0 is the CFE_EVS_NO_FILTER mask and event 0 is reserved (not used) */
    CFE_PSP_MemSet((void*)EventTbl, 0x00, sizeof(EventTbl));

    EventTbl[  ind].EventID = MAC_RESERVED_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    EventTbl[  ind].EventID = MAC_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    EventTbl[  ind].EventID = MAC_CONFIG_TABLE_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    EventTbl[  ind].EventID = MAC_PIPE_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    EventTbl[  ind].EventID = MAC_MSGID_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    EventTbl[  ind].EventID = MAC_MSGLEN_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    EventTbl[  ind].EventID = MAC_CMD_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EventTbl[  ind].EventID = MAC_INIT_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EventTbl[  ind].EventID = MAC_INIT_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(EventTbl, MAC_EVT_CNT, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("MAC - Failed to register with EVS (0x%08X)\n", (unsigned int)iStatus);
    }

    return (iStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MAC::InitPipe()
{
    int32  iStatus = CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&SchPipeId,
                                 MAC_SCH_PIPE_DEPTH,
                                 MAC_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(MAC_SEND_HK_MID, SchPipeId, CFE_SB_Default_Qos, MAC_SCH_PIPE_SEND_HK_RESERVED);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "CMD Pipe failed to subscribe to MAC_SEND_HK_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto MAC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(MAC_RUN_CONTROLLER_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "CMD Pipe failed to subscribe to MAC_RUN_CONTROLLER_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto MAC_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(MAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create SCH pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto MAC_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&CmdPipeId,
                                 MAC_CMD_PIPE_DEPTH,
                                 MAC_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(MAC_CMD_MID, CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "CMD Pipe failed to subscribe to MAC_CMD_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto MAC_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(MAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create CMD pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto MAC_InitPipe_Exit_Tag;
    }

    /* Init data pipe and subscribe to messages on the data pipe */
    iStatus = CFE_SB_CreatePipe(&DataPipeId,
                                 MAC_DATA_PIPE_DEPTH,
                                 MAC_DATA_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(PX4_ACTUATOR_ARMED_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "DATA Pipe failed to subscribe to PX4_ACTUATOR_ARMED_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto MAC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_BATTERY_STATUS_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "DATA Pipe failed to subscribe to PX4_BATTERY_STATUS_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto MAC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_CONTROL_STATE_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "DATA Pipe failed to subscribe to PX4_CONTROL_STATE_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto MAC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_MANUAL_CONTROL_SETPOINT_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "DATA Pipe failed to subscribe to PX4_MANUAL_CONTROL_SETPOINT_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto MAC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_MULTIROTOR_MOTOR_LIMITS_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "DATA Pipe failed to subscribe to PX4_MULTIROTOR_MOTOR_LIMITS_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto MAC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_SENSOR_CORRECTION_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "DATA Pipe failed to subscribe to PX4_SENSOR_CORRECTION_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto MAC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_SENSOR_GYRO_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "DATA Pipe failed to subscribe to PX4_SENSOR_GYRO_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto MAC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_ATTITUDE_SETPOINT_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "DATA Pipe failed to subscribe to PX4_VEHICLE_ATTITUDE_SETPOINT_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto MAC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_CONTROL_MODE_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "DATA Pipe failed to subscribe to PX4_VEHICLE_CONTROL_MODE_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto MAC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_RATES_SETPOINT_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "DATA Pipe failed to subscribe to PX4_VEHICLE_RATES_SETPOINT_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto MAC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_STATUS_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "DATA Pipe failed to subscribe to PX4_VEHICLE_STATUS_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto MAC_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(MAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create Data pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto MAC_InitPipe_Exit_Tag;
    }

MAC_InitPipe_Exit_Tag:
    return (iStatus);
}
    
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MAC::InitData()
{
    int32  iStatus = CFE_SUCCESS;

    CFE_SB_InitMsg(&CVT.Armed, PX4_ACTUATOR_ARMED_MID, sizeof(CVT.Armed), TRUE);
    CFE_SB_InitMsg(&CVT.BatteryStatus, PX4_BATTERY_STATUS_MID, sizeof(CVT.BatteryStatus), TRUE);
    CFE_SB_InitMsg(&CVT.ControlState, PX4_CONTROL_STATE_MID, sizeof(CVT.ControlState), TRUE);
    CFE_SB_InitMsg(&CVT.ManualControlSp, PX4_MANUAL_CONTROL_SETPOINT_MID, sizeof(CVT.ManualControlSp), TRUE);
    CFE_SB_InitMsg(&CVT.MotorLimits, PX4_MULTIROTOR_MOTOR_LIMITS_MID, sizeof(CVT.MotorLimits), TRUE);
    CFE_SB_InitMsg(&CVT.SensorCorrection, PX4_SENSOR_CORRECTION_MID, sizeof(CVT.SensorCorrection), TRUE);
    CFE_SB_InitMsg(&CVT.SensorGyro, PX4_SENSOR_GYRO_MID, sizeof(CVT.SensorGyro), TRUE);
    CFE_SB_InitMsg(&CVT.VAttSp, PX4_VEHICLE_ATTITUDE_SETPOINT_MID, sizeof(CVT.VAttSp), TRUE);
    CFE_SB_InitMsg(&CVT.VControlMode, PX4_VEHICLE_CONTROL_MODE_MID, sizeof(CVT.VControlMode), TRUE);
    CFE_SB_InitMsg(&CVT.VRatesSp, PX4_VEHICLE_RATES_SETPOINT_MID, sizeof(CVT.VRatesSp), TRUE);
    CFE_SB_InitMsg(&CVT.VehicleStatus, PX4_VEHICLE_STATUS_MID, sizeof(CVT.VehicleStatus), TRUE);

    /* Init actuator outputs message */
    CFE_SB_InitMsg(&m_ActuatorControls0,
            PX4_ACTUATOR_CONTROLS_0_MID, sizeof(m_ActuatorControls0), TRUE);
    /* Init housekeeping message. */
    CFE_SB_InitMsg(&HkTlm, MAC_HK_TLM_MID, sizeof(HkTlm), TRUE);

    m_AngularRatesPrevious.Zero();
    m_AngularRatesSetpointPrevious.Zero();
    m_AngularRatesSetpoint.Zero();
    m_AngularRatesIntegralError.Zero();
    m_AttControl.Zero();
    m_ThrustSp = 0.0f;

    for (uint32 i = 0; i < MAX_GYRO_COUNT; ++i)
    {
        /* Used scale factors to unity */
        CVT.SensorCorrection.gyro_scale_0[i] = 1.0f;
        CVT.SensorCorrection.gyro_scale_1[i] = 1.0f;
        CVT.SensorCorrection.gyro_scale_2[i] = 1.0f;
    }
    
    UpdateParams();

    return (iStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* MAC initialization                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MAC::InitApp()
{
    int32  iStatus   = CFE_SUCCESS;
    int8   hasEvents = 0;

    iStatus = InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("MAC - Failed to init events (0x%08X)\n", (unsigned int)iStatus);
        goto MAC_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init pipes (0x%08X)",
                                 (unsigned int)iStatus);
        goto MAC_InitApp_Exit_Tag;
    }

    iStatus = InitConfigTbl();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init config tables (0x%08X)",
                                 (unsigned int)iStatus);
        goto MAC_InitApp_Exit_Tag;
    }
    
    iStatus = InitData();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init data (0x%08X)",
                                 (unsigned int)iStatus);
        goto MAC_InitApp_Exit_Tag;
    }

MAC_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MAC_INIT_INF_EID, CFE_EVS_INFORMATION,
                                 "Initialized.  Version %d.%d.%d.%d",
                                 MAC_MAJOR_VERSION,
                                 MAC_MINOR_VERSION,
                                 MAC_REVISION,
                                 MAC_MISSION_REV);
    }
    else
    {
        if (hasEvents == 1)
        {
            (void) CFE_EVS_SendEvent(MAC_INIT_ERR_EID, CFE_EVS_ERROR, "Application failed to initialize");
        }
        else
        {
            (void) CFE_ES_WriteToSysLog("MAC - Application failed to initialize\n");
        }
    }

    return (iStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 MAC::RcvSchPipeMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(MAC_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(MAC_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
        {
            case MAC_SEND_HK_MID:
            {
                ProcessNewCmds();
                ReportHousekeeping();
                break;
            }

            case MAC_RUN_CONTROLLER_MID:
            {
            	if(ProcessIncomingData() == true)
            	{
                    RunController();
            	}
                break;
            }

            default:
            {
                (void) CFE_EVS_SendEvent(MAC_MSGID_ERR_EID, CFE_EVS_ERROR,
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
        (void) CFE_EVS_SendEvent(MAC_PIPE_ERR_EID, CFE_EVS_ERROR,
              "SB pipe read error (0x%08X), app will exit", (unsigned int)iStatus);
        uiRunStatus= CFE_ES_APP_ERROR;
    }

    return (iStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Data                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

osalbool MAC::ProcessIncomingData()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;
    osalbool result = true;

    /* Process command messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&MsgPtr, DataPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            MsgId = CFE_SB_GetMsgId(MsgPtr);
            switch (MsgId)
            {
				case PX4_ACTUATOR_ARMED_MID:
				{
					CFE_PSP_MemCpy(&CVT.Armed, MsgPtr, sizeof(CVT.Armed));
					break;
				}

				case PX4_BATTERY_STATUS_MID:
				{
					CFE_PSP_MemCpy(&CVT.BatteryStatus, MsgPtr, sizeof(CVT.BatteryStatus));
					break;
				}

				case PX4_CONTROL_STATE_MID:
				{
					CFE_PSP_MemCpy(&CVT.ControlState, MsgPtr, sizeof(CVT.ControlState));
					break;
				}

				case PX4_MANUAL_CONTROL_SETPOINT_MID:
				{
					CFE_PSP_MemCpy(&CVT.ManualControlSp, MsgPtr, sizeof(CVT.ManualControlSp));
					break;
				}

				case PX4_MULTIROTOR_MOTOR_LIMITS_MID:
				{
					CFE_PSP_MemCpy(&CVT.MotorLimits, MsgPtr, sizeof(CVT.MotorLimits));
					break;
				}

				case PX4_SENSOR_CORRECTION_MID:
				{
					CFE_PSP_MemCpy(&CVT.SensorCorrection, MsgPtr, sizeof(CVT.SensorCorrection));
					if (CVT.SensorCorrection.selected_gyro_instance < MAX_GYRO_COUNT)
					{
						m_SelectedGyro = CVT.SensorCorrection.selected_gyro_instance;
					}
					break;
				}

				case PX4_SENSOR_GYRO_MID:
				{
					CFE_PSP_MemCpy(&CVT.SensorGyro, MsgPtr, sizeof(CVT.SensorGyro));
					break;
				}

				case PX4_VEHICLE_ATTITUDE_SETPOINT_MID:
				{
					CFE_PSP_MemCpy(&CVT.VAttSp, MsgPtr, sizeof(CVT.VAttSp));
					break;
				}

				case PX4_VEHICLE_CONTROL_MODE_MID:
				{
					CFE_PSP_MemCpy(&CVT.VControlMode, MsgPtr, sizeof(CVT.VControlMode));
					break;
				}

				case PX4_VEHICLE_RATES_SETPOINT_MID:
				{
					CFE_PSP_MemCpy(&CVT.VRatesSp, MsgPtr, sizeof(CVT.VRatesSp));
					break;
				}

				case PX4_VEHICLE_STATUS_MID:
				{
					CFE_PSP_MemCpy(&CVT.VehicleStatus, MsgPtr, sizeof(CVT.VehicleStatus));
					break;
				}

                default:
				{
                    (void) CFE_EVS_SendEvent(MAC_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            (void) CFE_EVS_SendEvent(MAC_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "DATA pipe read error (0x%08X)", (unsigned int)iStatus);
            uiRunStatus = CFE_ES_APP_ERROR;
            result = false;
            break;
        }
    }

    return result;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MAC::ProcessNewCmds()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr=NULL;
    CFE_SB_MsgId_t  CmdMsgId;

    /* Process command messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case MAC_CMD_MID:
                    ProcessAppCmds(CmdMsgPtr);
                    break;

                default:
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with this
                     *  pipe, but not handled in this switch-case.) */
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(MAC_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            (void) CFE_EVS_SendEvent(MAC_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08X)", (unsigned int)iStatus);
            uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process MAC Commands                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MAC::ProcessAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case MAC_NOOP_CC:
                HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(MAC_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "Recvd NOOP cmd (%u), Version %d.%d.%d.%d",
                                  (unsigned int)uiCmdCode,
                                  MAC_MAJOR_VERSION,
                                  MAC_MINOR_VERSION,
                                  MAC_REVISION,
                                  MAC_MISSION_REV);
                break;

            case MAC_RESET_CC:
                HkTlm.usCmdCnt = 0;
                HkTlm.usCmdErrCnt = 0;
                (void) CFE_EVS_SendEvent(MAC_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "Recvd RESET cmd (%u)", (unsigned int)uiCmdCode);
                break;

            default:
                HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(MAC_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid cmdId (%u)", (unsigned int)uiCmdCode);
                break;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send MAC Housekeeping                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MAC::ReportHousekeeping()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&HkTlm);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Publish Output Data                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MAC::SendActuatorControls()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&m_ActuatorControls0);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&m_ActuatorControls0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Command Length                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean MAC::VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
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

            (void) CFE_EVS_SendEvent(MAC_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            HkTlm.usCmdErrCnt++;
        }
    }

    return (bResult);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* MAC Application C style main entry point.                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
extern "C" void MAC_AppMain()
{
    oMAC.AppMain();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* MAC Application C++ style main entry point.                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MAC::AppMain()
{
    /* Register the application with Executive Services */
    uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("MAC - Failed to register the app (0x%08X)\n", (unsigned int)iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(MAC_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(MAC_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(MAC_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(MAC_MAIN_TASK_PERF_ID);
    }
    else
    {
        uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&uiRunStatus) == TRUE)
    {
        RcvSchPipeMsg(MAC_SCH_PIPE_PEND_TIME);

        iStatus = AcquireConfigPointers();
        if(iStatus != CFE_SUCCESS)
        {
            /* We apparently tried to load a new table but failed.  Terminate the application. */
            uiRunStatus = CFE_ES_APP_ERROR;
        }
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(MAC_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(uiRunStatus);
}

void MAC::RunController(void)
{
    static uint64 last_run = 0;
    uint64 now = PX4LIB_GetPX4TimeUs();
    float dt = (now - last_run) / MICRO_SEC_TO_SEC_DIV;
    last_run = now;
    uint32 i = 0;

    /* Guard against too small (< 2ms) and too large (> 20ms) dt's */
    if (dt < TWO_MILLISECONDS)
    {
        dt = TWO_MILLISECONDS;
    }
    else if (dt > TWENTY_MILLISECONDS)
    {
        dt = TWENTY_MILLISECONDS;
    }

    /* Check if we are in rattitude mode and the pilot is above the threshold on pitch
     * or roll (yaw can rotate 360 in normal att control).  If both are true don't
     * even bother running the attitude controllers */
    if (CVT.VControlMode.ControlRattitudeEnabled)
    {
        if (fabsf(CVT.ManualControlSp.Y) > m_Params.rattitude_thres ||
            fabsf(CVT.ManualControlSp.X) > m_Params.rattitude_thres)
        {
            CVT.VControlMode.ControlAttitudeEnabled = FALSE;
        }
    }

    if (CVT.VControlMode.ControlAttitudeEnabled)
    {
        ControlAttitude(dt);

        /* Publish attitude rates setpoint */
        CFE_SB_TimeStampMsg((CFE_SB_MsgPtr_t)&CVT.VRatesSp);
        CVT.VRatesSp.Roll = m_AngularRatesSetpoint[0];
        CVT.VRatesSp.Pitch = m_AngularRatesSetpoint[1];
        CVT.VRatesSp.Yaw = m_AngularRatesSetpoint[2];
        CVT.VRatesSp.Thrust = m_ThrustSp;

        CFE_SB_SendMsg((CFE_SB_Msg_t*)&CVT.VRatesSp);
    }
    else
    {
        /* Attitude controller disabled, poll rates setpoint topic */
        if (CVT.VControlMode.ControlManualEnabled)
        {
            /* Manual rates control - ACRO mode */
            m_AngularRatesSetpoint = math::Vector3F(CVT.ManualControlSp.Y, -CVT.ManualControlSp.X,
                    CVT.ManualControlSp.R).EMult(m_Params.acro_rate_max);
            m_ThrustSp = fmin(CVT.ManualControlSp.Z, MANUAL_THROTTLE_MAX_MULTICOPTER);

            /* Publish attitude rates setpoint */
            CFE_SB_TimeStampMsg((CFE_SB_MsgPtr_t)&CVT.VRatesSp);
            CVT.VRatesSp.Roll = m_AngularRatesSetpoint[0];
            CVT.VRatesSp.Pitch = m_AngularRatesSetpoint[1];
            CVT.VRatesSp.Yaw = m_AngularRatesSetpoint[2];
            CVT.VRatesSp.Thrust = m_ThrustSp;

            CFE_SB_SendMsg((CFE_SB_Msg_t*)&CVT.VRatesSp);
        }
        else
        {
            /* Attitude controller disabled, poll rates setpoint topic */
            m_AngularRatesSetpoint[0] = CVT.VRatesSp.Roll;
            m_AngularRatesSetpoint[1] = CVT.VRatesSp.Pitch;
            m_AngularRatesSetpoint[2] = CVT.VRatesSp.Yaw;
            m_ThrustSp = CVT.VRatesSp.Thrust;
        }
    }

    if (CVT.VControlMode.ControlRatesEnabled)
    {
        ControlAttitudeRates(dt);

        /* Publish actuator controls */
        m_ActuatorControls0.Timestamp = PX4LIB_GetPX4TimeUs();
        m_ActuatorControls0.SampleTime = CVT.ControlState.Timestamp;
        m_ActuatorControls0.Control[0] = (isfinite(m_AttControl[0])) ? m_AttControl[0] : 0.0f;
        m_ActuatorControls0.Control[1] = (isfinite(m_AttControl[1])) ? m_AttControl[1] : 0.0f;
        m_ActuatorControls0.Control[2] = (isfinite(m_AttControl[2])) ? m_AttControl[2] : 0.0f;
        m_ActuatorControls0.Control[3] = (isfinite(m_ThrustSp)) ? m_ThrustSp : 0.0f;

        /* Scale effort by battery status */
        if (m_Params.bat_scale_en && CVT.BatteryStatus.Scale > 0.0f)
        {
            for (i = 0; i < 4; ++i) // TODO: make this bat count a define somewhere
            {
                m_ActuatorControls0.Control[i] *= CVT.BatteryStatus.Scale;
            }
        }

        SendActuatorControls();

        PX4_McAttCtrlStatusMsg_t controllerStatus;
        CFE_SB_InitMsg(&controllerStatus,
                PX4_MC_ATT_CTRL_STATUS_MID, sizeof(controllerStatus), TRUE);
        controllerStatus.RollRateInteg = m_AngularRatesIntegralError[0];
        controllerStatus.PitchRateInteg = m_AngularRatesIntegralError[1];
        controllerStatus.YawRateInteg = m_AngularRatesIntegralError[2];
        controllerStatus.Timestamp = PX4LIB_GetPX4TimeUs();
    }
}

void MAC::ControlAttitude(float dt)
{
    int32 i = 0;
    m_ThrustSp = CVT.VAttSp.Thrust;

    /* Construct attitude setpoint rotation matrix */
    math::Quaternion q_sp(
            CVT.VAttSp.Q_D[0],
            CVT.VAttSp.Q_D[1],
            CVT.VAttSp.Q_D[2],
            CVT.VAttSp.Q_D[3]);

    math::Matrix3F3 R_sp = q_sp.RotationMatrix();

    /* Get current rotation matrix from control state quaternions */
    math::Quaternion q_att(CVT.ControlState.Q[0], CVT.ControlState.Q[1], CVT.ControlState.Q[2], CVT.ControlState.Q[3]);
    math::Matrix3F3 R = q_att.RotationMatrix();

    /* All input data is ready, run controller itself */

    /* Try to move thrust vector shortest way, because yaw response is slower than roll/pitch */
    math::Vector3F R_z(R[0][2], R[1][2], R[2][2]);
    math::Vector3F R_sp_z(R_sp[0][2], R_sp[1][2], R_sp[2][2]);

    /* Axis and sin(angle) of desired rotation */
    math::Vector3F e_R = R.Transpose() * (R_z % R_sp_z);

    /* Calculate angle error */
    float e_R_z_sin = e_R.Length();
    float e_R_z_cos = R_z * R_sp_z;

    /* Calculate weight for yaw control */
    float yaw_w = R_sp[2][2] * R_sp[2][2];

    /* Calculate rotation matrix after roll/pitch only rotation */
    math::Matrix3F3 R_rp;

    if (e_R_z_sin > 0.0f) 
    {
        /* Get axis-angle representation */
        float e_R_z_angle = atan2f(e_R_z_sin, e_R_z_cos);
        math::Vector3F e_R_z_axis = e_R / e_R_z_sin;

        e_R = e_R_z_axis * e_R_z_angle;

        /* Cross product matrix for e_R_axis */
        math::Matrix3F3 e_R_cp;
        e_R_cp.Zero();
        e_R_cp[0][1] = -e_R_z_axis[2];
        e_R_cp[0][2] = e_R_z_axis[1];
        e_R_cp[1][0] = e_R_z_axis[2];
        e_R_cp[1][2] = -e_R_z_axis[0];
        e_R_cp[2][0] = -e_R_z_axis[1];
        e_R_cp[2][1] = e_R_z_axis[0];

        /* Rotation matrix for roll/pitch only rotation */
        R_rp = R * (math::Matrix3F3::Identity() + e_R_cp * e_R_z_sin + e_R_cp * e_R_cp * (1.0f - e_R_z_cos));

    }
    else 
    {
        /* Zero roll/pitch rotation */
        R_rp = R;
    }

    /* R_rp and R_sp has the same Z axis, calculate yaw error */
    math::Vector3F R_sp_x(R_sp[0][0], R_sp[1][0], R_sp[2][0]);
    math::Vector3F R_rp_x(R_rp[0][0], R_rp[1][0], R_rp[2][0]);
    e_R[2] = atan2f((R_rp_x % R_sp_x) * R_sp_z, R_rp_x * R_sp_x) * yaw_w;

    if (e_R_z_cos < 0.0f) 
    {
        /* For large thrust vector rotations use another rotation method:
         * calculate angle and axis for R -> R_sp rotation directly */
        math::Quaternion q_error(R.Transpose() * R_sp);
        math::Vector3F e_R_d = q_error[0] >= 0.0f ? q_error.Imaginary()  * 2.0f : -q_error.Imaginary() * 2.0f;

        /* Use fusion of Z axis based rotation and direct rotation */
        float direct_w = e_R_z_cos * e_R_z_cos * yaw_w;
        e_R = e_R * (1.0f - direct_w) + e_R_d * direct_w;
    }

    /* Calculate angular rates setpoint */
    m_AngularRatesSetpoint = m_Params.att_p.EMult(e_R);

    /* Limit rates */
    for (i = 0; i < 3; ++i) 
    {
        if ((CVT.VControlMode.ControlVelocityEnabled || CVT.VControlMode.ControlAutoEnabled) &&
            !CVT.VControlMode.ControlManualEnabled)
        {
            m_AngularRatesSetpoint.Constrain(i, -m_Params.auto_rate_max[i], m_Params.auto_rate_max[i]);
        }
        else
        {
            m_AngularRatesSetpoint.Constrain(i, -m_Params.mc_rate_max[i], m_Params.mc_rate_max[i]);
        }
    }

    /* Feed forward yaw setpoint rate */
    m_AngularRatesSetpoint[2] += CVT.VAttSp.YawSpMoveRate * yaw_w;
}

void MAC::ControlAttitudeRates(float dt)
{
    /* Rotation matrix for the orientation that the board is mounted */
    math::Matrix3F3    boardRotation;
    uint32 i = 0;

    /* Reset integral if disarmed */
    if(!CVT.Armed.Armed || !CVT.VehicleStatus.IsRotaryWing)
    {
        m_AngularRatesIntegralError.Zero();
    }

    /* Get transformation matrix from sensor/board to body frame */
    boardRotation = boardRotation.RotationMatrix((math::Matrix3F3::Rotation_t)ParamTblPtr->SENS_BOARD_ROT);

    /* Fine tune the rotation */
    math::Matrix3F3 boardRotationOffset;
    boardRotationOffset = math::Matrix3F3::FromEuler(M_DEG_TO_RAD_F * ParamTblPtr->SENS_BOARD_X_OFF,
                     M_DEG_TO_RAD_F * ParamTblPtr->SENS_BOARD_Y_OFF,
                     M_DEG_TO_RAD_F * ParamTblPtr->SENS_BOARD_Z_OFF);
    boardRotation = boardRotationOffset * boardRotation;

    /* Get the raw gyro data and correct for thermal errors */
    math::Vector3F rates;

    if (m_SelectedGyro == 0)
    {
        rates[0] = (CVT.SensorGyro.X - CVT.SensorCorrection.gyro_offset_0[0]) * CVT.SensorCorrection.gyro_scale_0[0];
        rates[1] = (CVT.SensorGyro.Y - CVT.SensorCorrection.gyro_offset_0[1]) * CVT.SensorCorrection.gyro_scale_0[1];
        rates[2] = (CVT.SensorGyro.Z - CVT.SensorCorrection.gyro_offset_0[2]) * CVT.SensorCorrection.gyro_scale_0[2];
    }
    else if (m_SelectedGyro == 1)
    {
        rates[0] = (CVT.SensorGyro.X - CVT.SensorCorrection.gyro_offset_1[0]) * CVT.SensorCorrection.gyro_scale_1[0];
        rates[1] = (CVT.SensorGyro.Y - CVT.SensorCorrection.gyro_offset_1[1]) * CVT.SensorCorrection.gyro_scale_1[1];
        rates[2] = (CVT.SensorGyro.Z - CVT.SensorCorrection.gyro_offset_1[2]) * CVT.SensorCorrection.gyro_scale_1[2];
    }
    else if (m_SelectedGyro == 2)
    {
        rates[0] = (CVT.SensorGyro.X - CVT.SensorCorrection.gyro_offset_2[0]) * CVT.SensorCorrection.gyro_scale_2[0];
        rates[1] = (CVT.SensorGyro.Y - CVT.SensorCorrection.gyro_offset_2[1]) * CVT.SensorCorrection.gyro_scale_2[1];
        rates[2] = (CVT.SensorGyro.Z - CVT.SensorCorrection.gyro_offset_2[2]) * CVT.SensorCorrection.gyro_scale_2[2];
    }
    else
    {
        rates[0] = CVT.SensorGyro.X;
        rates[1] = CVT.SensorGyro.Y;
        rates[2] = CVT.SensorGyro.Z;
    }

    /* Rotate corrected measurements from sensor to body frame */
    rates = boardRotation * rates;

    /* Correct for in-run bias errors */
    rates[0] -= CVT.ControlState.RollRateBias;
    rates[1] -= CVT.ControlState.PitchRateBias;
    rates[2] -= CVT.ControlState.YawRateBias;

    math::Vector3F rates_p_scaled = m_Params.rate_p.EMult(PidAttenuations(m_Params.tpa_breakpoint_p, m_Params.tpa_rate_p));
    math::Vector3F rates_i_scaled = m_Params.rate_i.EMult(PidAttenuations(m_Params.tpa_breakpoint_i, m_Params.tpa_rate_i));
    math::Vector3F rates_d_scaled = m_Params.rate_d.EMult(PidAttenuations(m_Params.tpa_breakpoint_d, m_Params.tpa_rate_d));

    /* Angular rates error */
    math::Vector3F rates_err = m_AngularRatesSetpoint - rates;

    m_AttControl = rates_p_scaled.EMult(rates_err) +
            m_AngularRatesIntegralError +
            rates_d_scaled.EMult(m_AngularRatesPrevious - rates) / dt +
            m_Params.rate_ff.EMult(m_AngularRatesSetpoint);

    m_AngularRatesSetpointPrevious = m_AngularRatesSetpoint;
    m_AngularRatesPrevious = rates;

    /* Update integral only if motors are providing enough thrust to be effective */
    if (m_ThrustSp > MIN_TAKEOFF_THRUST)
    {
        for (int i = MAC_AXIS_INDEX_ROLL; i < MAC_AXIS_COUNT; i++)
        {
            /* Check for positive control saturation */
            osalbool positive_saturation =
                ((i == MAC_AXIS_INDEX_ROLL) && CVT.MotorLimits.SaturationStatus.Flags.RollPos) ||
                ((i == MAC_AXIS_INDEX_PITCH) && CVT.MotorLimits.SaturationStatus.Flags.PitchPos) ||
                ((i == MAC_AXIS_INDEX_YAW) && CVT.MotorLimits.SaturationStatus.Flags.YawPos);

            /* Check for negative control saturation */
            osalbool negative_saturation =
                ((i == MAC_AXIS_INDEX_ROLL) && CVT.MotorLimits.SaturationStatus.Flags.RollNeg) ||
                ((i == MAC_AXIS_INDEX_PITCH) && CVT.MotorLimits.SaturationStatus.Flags.PitchNeg) ||
                ((i == MAC_AXIS_INDEX_YAW) && CVT.MotorLimits.SaturationStatus.Flags.YawNeg);

            /* Prevent further positive control saturation */
            if (positive_saturation)
            {
                rates_err[i] = fmin(rates_err[i], 0.0f);
            }

            /* Prevent further negative control saturation */
            if (negative_saturation) {
                rates_err[i] = fmax(rates_err[i], 0.0f);
            }

            /* Perform the integration using a first order method and do not 
            *  propaate the result if out of range or invalid */
            const float rate_i = m_AngularRatesIntegralError[i] + m_Params.rate_i[i] * rates_err[i] * dt;
            
            osalbool valid_rate_i = isfinite(rate_i) && 
                                    rate_i > -m_Params.rate_int_lim[i] && 
                                    rate_i < m_Params.rate_int_lim[i];

            if (valid_rate_i)
            {
                m_AngularRatesIntegralError[i] = rate_i;
            }
        }
    }

    /* Explicitly limit the integrator state */
    for (i = MAC_AXIS_INDEX_ROLL; i < MAC_AXIS_COUNT; ++i)
    {
        m_AngularRatesIntegralError.Constrain(i, -m_Params.rate_int_lim[i], m_Params.rate_int_lim[i]);
    }
}

math::Vector3F MAC::PidAttenuations(float tpa_breakpoint, float tpa_rate)
{
    /* Throttle pid attenuation factor */
    float tpa = 1.0f - tpa_rate * (fabsf(CVT.VRatesSp.Thrust) - tpa_breakpoint) / (1.0f - tpa_breakpoint);
    tpa = fmaxf(TPA_RATE_LOWER_LIMIT, fminf(1.0f, tpa));

    math::Vector3F pidAttenuationPerAxis;
    pidAttenuationPerAxis[MAC_AXIS_INDEX_ROLL] = tpa;
    pidAttenuationPerAxis[MAC_AXIS_INDEX_PITCH] = tpa;
    pidAttenuationPerAxis[MAC_AXIS_INDEX_YAW] = 1.0;

    return (pidAttenuationPerAxis);
}

void MAC::UpdateParams(void)
{
    /* Roll gains */
    m_Params.att_p[0] = ParamTblPtr->MC_ROLL_P * (ATTITUDE_TC_DEFAULT / ParamTblPtr->MC_ROLL_TC);
    m_Params.rate_p[0] = ParamTblPtr->MC_ROLLRATE_P * (ATTITUDE_TC_DEFAULT / ParamTblPtr->MC_ROLL_TC);
    m_Params.rate_i[0] = ParamTblPtr->MC_ROLLRATE_I;
    m_Params.rate_int_lim[0] = ParamTblPtr->MC_RR_INT_LIM;
    m_Params.rate_d[0] = ParamTblPtr->MC_ROLLRATE_D * (ATTITUDE_TC_DEFAULT / ParamTblPtr->MC_ROLL_TC);
    m_Params.rate_ff[0] = ParamTblPtr->MC_ROLLRATE_FF;

    /* Pitch gains */
    m_Params.att_p[1] = ParamTblPtr->MC_PITCH_P * (ATTITUDE_TC_DEFAULT / ParamTblPtr->MC_PITCH_TC);
    m_Params.rate_p[1] = ParamTblPtr->MC_PITCHRATE_P * (ATTITUDE_TC_DEFAULT / ParamTblPtr->MC_PITCH_TC);
    m_Params.rate_i[1] = ParamTblPtr->MC_PITCHRATE_I;
    m_Params.rate_int_lim[1] = ParamTblPtr->MC_PR_INT_LIM;
    m_Params.rate_d[1] = ParamTblPtr->MC_PITCHRATE_D * (ATTITUDE_TC_DEFAULT / ParamTblPtr->MC_PITCH_TC);
    m_Params.rate_ff[1] = ParamTblPtr->MC_PITCHRATE_FF;

    /* Throttle pid attenuation */
    m_Params.tpa_breakpoint_p = ParamTblPtr->MC_TPA_BREAK_P;
    m_Params.tpa_breakpoint_i = ParamTblPtr->MC_TPA_BREAK_I;
    m_Params.tpa_breakpoint_d = ParamTblPtr->MC_TPA_BREAK_D;
    m_Params.tpa_rate_p = ParamTblPtr->MC_TPA_RATE_P;
    m_Params.tpa_rate_i = ParamTblPtr->MC_TPA_RATE_I;
    m_Params.tpa_rate_d = ParamTblPtr->MC_TPA_RATE_D;

    /* Yaw gains */
    m_Params.att_p[2] = ParamTblPtr->MC_YAW_P;
    m_Params.rate_p[2] = ParamTblPtr->MC_YAWRATE_P;
    m_Params.rate_i[2] = ParamTblPtr->MC_YAWRATE_I;
    m_Params.rate_int_lim[2] = ParamTblPtr->MC_YR_INT_LIM;
    m_Params.rate_d[2] = ParamTblPtr->MC_YAWRATE_D;
    m_Params.rate_ff[2] = ParamTblPtr->MC_YAWRATE_FF;

    /* Angular rate limits */
    m_Params.mc_rate_max[0] = (ParamTblPtr->MC_ROLLRATE_MAX / 180.0f) * M_PI;
    m_Params.mc_rate_max[1] = (ParamTblPtr->MC_PITCHRATE_MAX / 180.0f) * M_PI;
    m_Params.mc_rate_max[2] = (ParamTblPtr->MC_YAWRATE_MAX / 180.0f) * M_PI;

    /* Auto angular rate limits */
    m_Params.auto_rate_max[0] = (ParamTblPtr->MC_ROLLRATE_MAX / 180.0f) * M_PI;
    m_Params.auto_rate_max[1] = (ParamTblPtr->MC_PITCHRATE_MAX / 180.0f) * M_PI;
    m_Params.auto_rate_max[2] = (ParamTblPtr->MC_YAWRAUTO_MAX / 180.0f) * M_PI;

    /* Manual rate control scale and auto mode roll/pitch rate limits */
    m_Params.acro_rate_max[0] = (ParamTblPtr->MC_ACRO_R_MAX / 180.0f) * M_PI;
    m_Params.acro_rate_max[1] = (ParamTblPtr->MC_ACRO_P_MAX / 180.0f) * M_PI;
    m_Params.acro_rate_max[2] = (ParamTblPtr->MC_ACRO_Y_MAX / 180.0f) * M_PI;

    /* Stick deflection needed in rattitude mode to control rates not angles */
    m_Params.rattitude_thres = ParamTblPtr->MC_RATT_TH;

    /* Battery power scaling of control */
    m_Params.bat_scale_en = ParamTblPtr->MC_BAT_SCALE_EN;

    /* Rotation of the autopilot relative to the body */
    m_Params.board_rotation = ParamTblPtr->SENS_BOARD_ROT;
}

/************************/
/*  End of File Comment */
/************************/
