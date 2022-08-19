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
#include "fac_app.hpp"
#include "fac_msg.h"
#include "fac_version.h"
#include "cfs_utils.h"
#include "px4lib_msgids.h"
#include <px4lib.h>
#include <math/Quaternion.hpp>
#include <string.h>
#include <errno.h>
#include <float.h>
#include <math.h>
#include "math/Limits.hpp"
#include "math/Euler.hpp"
#include "math/Quaternion.hpp"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Instantiate the application object.                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
FAC oFAC;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Default constructor.                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
FAC::FAC()
{
    Init();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Destructor constructor.                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
FAC::~FAC()
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize member variables.                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void FAC::Init(void)
{
    CFE_PSP_MemSet((void*)EventTbl, 0x00, sizeof(EventTbl));

    SchPipeId = 0;
    CmdPipeId = 0;
    DataPipeId = 0;

    uiRunStatus = CFE_ES_APP_RUN;

    ParamTblHdl = 0;
    ParamTblPtr = NULL;

    CFE_PSP_MemSet((void*)&m_ActuatorControls0, 0x00, sizeof(PX4_ActuatorControlsMsg_t));
    CFE_PSP_MemSet((void*)&m_ActuatorControls2, 0x00, sizeof(PX4_ActuatorControlsMsg_t));
    CFE_PSP_MemSet((void*)&m_VehicleRatesSetpoint, 0x00, sizeof(PX4_VehicleRatesSetpointMsg_t));

    CFE_PSP_MemSet((void*)&HkTlm, 0x00, sizeof(FAC_HkTlm_t));

    CFE_PSP_MemSet((void*)&CVT, 0x00, sizeof(FAC_CurrentValueTable_t));

    _flaps_applied = 0;
    _flaperons_applied = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables.                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 FAC::InitEvent()
{
    int32  iStatus = CFE_SUCCESS;
    uint32  ind = 0;

    /* Initialize the event filter table.
     * Note: 0 is the CFE_EVS_NO_FILTER mask and event 0 is reserved (not used) */
    CFE_PSP_MemSet((void*)EventTbl, 0x00, sizeof(EventTbl));

    EventTbl[  ind].EventID = FAC_RESERVED_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    EventTbl[  ind].EventID = FAC_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    EventTbl[  ind].EventID = FAC_CONFIG_TABLE_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    EventTbl[  ind].EventID = FAC_PIPE_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    EventTbl[  ind].EventID = FAC_MSGID_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    EventTbl[  ind].EventID = FAC_MSGLEN_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    EventTbl[  ind].EventID = FAC_CMD_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EventTbl[  ind].EventID = FAC_INIT_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EventTbl[  ind].EventID = FAC_INIT_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(EventTbl, FAC_EVT_CNT, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("FAC - Failed to register with EVS (0x%08X)\n", (unsigned int)iStatus);
    }

    return (iStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 FAC::InitPipe()
{
    int32  iStatus = CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&SchPipeId,
                                 FAC_SCH_PIPE_DEPTH,
                                 FAC_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(FAC_SEND_HK_MID, SchPipeId, CFE_SB_Default_Qos, FAC_SCH_PIPE_SEND_HK_RESERVED);
        if (iStatus != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "SCH Pipe failed to subscribe to FAC_SEND_HK_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto FAC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(FAC_RUN_CONTROLLER_MID, SchPipeId, CFE_SB_Default_Qos,
                                     FAC_SCH_PIPE_RUN_CONTROLLER_RESERVED);
        if (iStatus != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "SCH Pipe failed to subscribe to FAC_RUN_CONTROLLER_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto FAC_InitPipe_Exit_Tag;
        }
    }
    else
    {
        CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create SCH pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto FAC_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&CmdPipeId,
                                 FAC_CMD_PIPE_DEPTH,
                                 FAC_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(FAC_CMD_MID, CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "CMD Pipe failed to subscribe to FAC_CMD_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto FAC_InitPipe_Exit_Tag;
        }
    }
    else
    {
        CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create CMD pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto FAC_InitPipe_Exit_Tag;
    }

    /* Init data pipe and subscribe to messages on the data pipe */
    iStatus = CFE_SB_CreatePipe(&DataPipeId,
                                 FAC_DATA_PIPE_DEPTH,
                                 FAC_DATA_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {

        iStatus = CFE_SB_SubscribeEx(PX4_BATTERY_STATUS_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "DATA Pipe failed to subscribe to PX4_BATTERY_STATUS_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto FAC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_MANUAL_CONTROL_SETPOINT_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "DATA Pipe failed to subscribe to PX4_MANUAL_CONTROL_SETPOINT_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto FAC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_ATTITUDE_SETPOINT_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "DATA Pipe failed to subscribe to PX4_VEHICLE_ATTITUDE_SETPOINT_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto FAC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_ATTITUDE_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "DATA Pipe failed to subscribe to PX4_VEHICLE_ATTITUDE_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto FAC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_CONTROL_MODE_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "DATA Pipe failed to subscribe to PX4_VEHICLE_CONTROL_MODE_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto FAC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_STATUS_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "DATA Pipe failed to subscribe to PX4_VEHICLE_STATUS_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto FAC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_GLOBAL_POSITION_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "DATA Pipe failed to subscribe to PX4_VEHICLE_GLOBAL_POSITION_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto FAC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_LAND_DETECTED_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "DATA Pipe failed to subscribe to PX4_VEHICLE_LAND_DETECTED_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto FAC_InitPipe_Exit_Tag;
        }
    }
    else
    {
        CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create Data pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto FAC_InitPipe_Exit_Tag;
    }

FAC_InitPipe_Exit_Tag:
    return (iStatus);
}
    
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 FAC::InitData()
{
    int32  iStatus = CFE_SUCCESS;

    CFE_SB_InitMsg((void*)&CVT.Airspeed, PX4_AIRSPEED_MID, sizeof(CVT.Airspeed), TRUE);
    CFE_SB_InitMsg((void*)&CVT.BatteryStatus, PX4_BATTERY_STATUS_MID, sizeof(CVT.BatteryStatus), TRUE);
    CFE_SB_InitMsg((void*)&CVT.ManualControlSp, PX4_MANUAL_CONTROL_SETPOINT_MID, sizeof(CVT.ManualControlSp), TRUE);
    CFE_SB_InitMsg((void*)&CVT.VAttSp, PX4_VEHICLE_ATTITUDE_SETPOINT_MID, sizeof(CVT.VAttSp), TRUE);
    CFE_SB_InitMsg((void*)&CVT.VAtt, PX4_VEHICLE_ATTITUDE_MID, sizeof(CVT.VAtt), TRUE);
    CFE_SB_InitMsg((void*)&CVT.VGlobalPosition, PX4_VEHICLE_GLOBAL_POSITION_MID, sizeof(CVT.VGlobalPosition), TRUE);
    CFE_SB_InitMsg((void*)&CVT.VControlMode, PX4_VEHICLE_CONTROL_MODE_MID, sizeof(CVT.VControlMode), TRUE);
    CFE_SB_InitMsg((void*)&CVT.VLandDetected, PX4_VEHICLE_LAND_DETECTED_MID, sizeof(CVT.VLandDetected), TRUE);
    CFE_SB_InitMsg((void*)&CVT.VehicleStatus, PX4_VEHICLE_STATUS_MID, sizeof(CVT.VehicleStatus), TRUE);

    /* Init actuator outputs message */
    CFE_SB_InitMsg((void*)&m_ActuatorControls0,
            PX4_ACTUATOR_CONTROLS_0_MID, sizeof(m_ActuatorControls0), TRUE);
    CFE_SB_InitMsg((void*)&m_ActuatorControls2,
            PX4_ACTUATOR_CONTROLS_2_MID, sizeof(m_ActuatorControls2), TRUE);

    /* Init vehicle rates setpoint outputs message. */
    CFE_SB_InitMsg((void*)&m_VehicleRatesSetpoint, PX4_VEHICLE_RATES_SETPOINT_MID, sizeof(m_VehicleRatesSetpoint), TRUE);

    /* Init housekeeping message. */
    CFE_SB_InitMsg((void*)&HkTlm, FAC_HK_TLM_MID, sizeof(HkTlm), TRUE);

    
    UpdateParams();

    return (iStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FAC initialization                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 FAC::InitApp()
{
    int32  iStatus   = CFE_SUCCESS;
    int8   hasEvents = 0;

    iStatus = InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("FAC - Failed to init events (0x%08X)\n", (unsigned int)iStatus);
        goto FAC_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init pipes (0x%08X)",
                                 (unsigned int)iStatus);
        goto FAC_InitApp_Exit_Tag;
    }

    iStatus = InitConfigTbl();
    if (iStatus != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init config tables (0x%08X)",
                                 (unsigned int)iStatus);
        goto FAC_InitApp_Exit_Tag;
    }
    
    iStatus = InitData();
    if (iStatus != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init data (0x%08X)",
                                 (unsigned int)iStatus);
        goto FAC_InitApp_Exit_Tag;
    }

FAC_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(FAC_INIT_INF_EID, CFE_EVS_INFORMATION,
                                 "Initialized.  Version %d.%d.%d.%d",
                                 FAC_MAJOR_VERSION,
                                 FAC_MINOR_VERSION,
                                 FAC_REVISION,
                                 FAC_MISSION_REV);
    }
    else
    {
        if (hasEvents == 1)
        {
            CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR, "Application failed to initialize");
        }
        else
        {
            CFE_ES_WriteToSysLog("FAC - Application failed to initialize\n");
        }
    }

    return (iStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 FAC::RcvSchPipeMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(FAC_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(FAC_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
        {
            case FAC_SEND_HK_MID:
            {
                HkTlm.SendHkMsgRcvCnt++;
                ReportHousekeeping();
                ProcessNewCmds();
                break;
            }

            case FAC_RUN_CONTROLLER_MID:
            {
                HkTlm.RunControllerMsgRcvCnt++;
            	if(ProcessIncomingData() == true)
            	{
                    RunController();
            	}
                break;
            }

            default:
            {
                HkTlm.usSchErrCnt++;
                CFE_EVS_SendEvent(FAC_MSGID_ERR_EID, CFE_EVS_ERROR,
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
        CFE_EVS_SendEvent(FAC_PIPE_ERR_EID, CFE_EVS_ERROR,
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

osalbool FAC::ProcessIncomingData()
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
				case PX4_AIRSPEED_MID:
				{
					HkTlm.AirSpeedMsgRcvCnt++;
					CFE_PSP_MemCpy(&CVT.Airspeed, MsgPtr, sizeof(CVT.Airspeed));
					break;
				}

				case PX4_BATTERY_STATUS_MID:
				{
					HkTlm.BatteryStatusMsgRcvCnt++;
					CFE_PSP_MemCpy(&CVT.BatteryStatus, MsgPtr, sizeof(CVT.BatteryStatus));
					break;
				}

				case PX4_MANUAL_CONTROL_SETPOINT_MID:
				{
					HkTlm.ManualControlSpMsgRcvCnt++;
					CFE_PSP_MemCpy(&CVT.ManualControlSp, MsgPtr, sizeof(CVT.ManualControlSp));
					break;
				}

				case PX4_VEHICLE_ATTITUDE_MID:
				{
					HkTlm.VAttMsgRcvCnt++;
					CFE_PSP_MemCpy(&CVT.VAtt, MsgPtr, sizeof(CVT.VAtt));
					break;
				}

				case PX4_VEHICLE_ATTITUDE_SETPOINT_MID:
				{
					HkTlm.VAttSpMsgRcvCnt++;
					CFE_PSP_MemCpy(&CVT.VAttSp, MsgPtr, sizeof(CVT.VAttSp));
					break;
				}

				case PX4_VEHICLE_CONTROL_MODE_MID:
				{
					HkTlm.VControlModeMsgRcvCnt++;
					CFE_PSP_MemCpy(&CVT.VControlMode, MsgPtr, sizeof(CVT.VControlMode));
					break;
				}

				case PX4_VEHICLE_GLOBAL_POSITION_MID:
				{
					HkTlm.VGlobalPositionMsgRcvCnt++;
					CFE_PSP_MemCpy(&CVT.VGlobalPosition, MsgPtr, sizeof(CVT.VGlobalPosition));
					break;
				}

				case PX4_VEHICLE_LAND_DETECTED_MID:
				{
					HkTlm.VLandDetectedMsgRcvCnt++;
					CFE_PSP_MemCpy(&CVT.VLandDetected, MsgPtr, sizeof(CVT.VLandDetected));
					break;
				}

				case PX4_VEHICLE_STATUS_MID:
				{
					HkTlm.VehicleStatusMsgRcvCnt++;
					CFE_PSP_MemCpy(&CVT.VehicleStatus, MsgPtr, sizeof(CVT.VehicleStatus));
					break;
				}

                default:
				{
					HkTlm.usDataErrCnt++;
                    CFE_EVS_SendEvent(FAC_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            CFE_EVS_SendEvent(FAC_PIPE_ERR_EID, CFE_EVS_ERROR,
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
void FAC::ProcessNewCmds()
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
                case FAC_CMD_MID:
                    ProcessAppCmds(CmdMsgPtr);
                    break;

                default:
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with this
                     *  pipe, but not handled in this switch-case.) */
                    HkTlm.usCmdErrCnt++;
                    CFE_EVS_SendEvent(FAC_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            CFE_EVS_SendEvent(FAC_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08X)", (unsigned int)iStatus);
            uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process FAC Commands                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void FAC::ProcessAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case FAC_NOOP_CC:
                HkTlm.usCmdCnt++;
                CFE_EVS_SendEvent(FAC_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "Recvd NOOP cmd (%u), Version %d.%d.%d.%d",
                                  (unsigned int)uiCmdCode,
                                  FAC_MAJOR_VERSION,
                                  FAC_MINOR_VERSION,
                                  FAC_REVISION,
                                  FAC_MISSION_REV);
                break;

            case FAC_RESET_CC:
                ResetHousekeeping();
                CFE_EVS_SendEvent(FAC_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "Recvd RESET cmd (%u)", (unsigned int)uiCmdCode);
                break;

            default:
                HkTlm.usCmdErrCnt++;
                CFE_EVS_SendEvent(FAC_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid cmdId (%u)", (unsigned int)uiCmdCode);
                break;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send FAC Housekeeping                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void FAC::ReportHousekeeping()
{
    HkTlm.HkMsgSndCnt++;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&HkTlm);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Reset FAC Housekeeping data                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void FAC::ResetHousekeeping()
{
	HkTlm.usCmdCnt = 0;
	HkTlm.usCmdErrCnt = 0;
	HkTlm.usSchErrCnt = 0;
	HkTlm.usDataErrCnt = 0;

	HkTlm.SendHkMsgRcvCnt = 0;
	HkTlm.RunControllerMsgRcvCnt = 0;

	HkTlm.AirSpeedMsgRcvCnt = 0;
	HkTlm.BatteryStatusMsgRcvCnt = 0;
	HkTlm.ManualControlSpMsgRcvCnt = 0;
	HkTlm.VAttMsgRcvCnt = 0;
	HkTlm.VAttSpMsgRcvCnt = 0;
	HkTlm.VControlModeMsgRcvCnt = 0;
	HkTlm.VGlobalPositionMsgRcvCnt = 0;
	HkTlm.VLandDetectedMsgRcvCnt = 0;
	HkTlm.VehicleStatusMsgRcvCnt = 0;

	HkTlm.ActuatorControls0MsgSndCnt = 0;
	HkTlm.ActuatorControls2MsgSndCnt = 0;
	HkTlm.VehicleRatesSetpointMsgSndCnt = 0;

	HkTlm.HkMsgSndCnt = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Publish Output Data                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void FAC::SendOutputData(CFE_SB_Msg_t* MsgPtr)
{
    CFE_SB_TimeStampMsg(MsgPtr);
    CFE_SB_SendMsg(MsgPtr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Command Length                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean FAC::VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
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

            CFE_EVS_SendEvent(FAC_MSGLEN_ERR_EID, CFE_EVS_ERROR,
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
/* FAC Application C style main entry point.                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
extern "C" void FAC_AppMain()
{
    oFAC.AppMain();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FAC Application C++ style main entry point.                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void FAC::AppMain()
{
    /* Register the application with Executive Services */
    uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("FAC - Failed to register the app (0x%08X)\n", (unsigned int)iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(FAC_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(FAC_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(FAC_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(FAC_MAIN_TASK_PERF_ID);
    }
    else
    {
        uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&uiRunStatus) == TRUE)
    {
        RcvSchPipeMsg(FAC_SCH_PIPE_PEND_TIME);

        iStatus = AcquireConfigPointers();
        if(iStatus != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(FAC_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
                                     "FAC Failed to AcquireConfigPointers. (0x%08X)",
                                     (unsigned int)iStatus);
            /* We apparently tried to load a new table but failed.  Terminate the application. */
            uiRunStatus = CFE_ES_APP_ERROR;
        }
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(FAC_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(uiRunStatus);
}

void FAC::RunController(void)
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

	/* get current rotation matrix and euler angles from control state quaternions */
    math::Quaternion q_att(CVT.VAtt.Q[0], CVT.VAtt.Q[1], CVT.VAtt.Q[2], CVT.VAtt.Q[3]);
    math::Matrix3F3 _R = q_att.RotationMatrix();

    math::Vector3F euler_angles;
    euler_angles = _R.ToEuler();

	float _roll = euler_angles[0];
	float _pitch = euler_angles[1];
	float _yaw = euler_angles[2];

	if (CVT.VehicleStatus.IsVtol && ParamTblPtr->VT_TYPE == vtol_type::TAILSITTER)
	{
		/* vehicle is a tailsitter, we need to modify the estimated attitude for fw mode
		 *
		 * Since the VTOL airframe is initialized as a multicopter we need to
		 * modify the estimated attitude for the fixed wing operation.
		 * Since the neutral position of the vehicle in fixed wing mode is -90 degrees rotated around
		 * the pitch axis compared to the neutral position of the vehicle in multicopter mode
		 * we need to swap the roll and the yaw axis (1st and 3rd column) in the rotation matrix.
		 * Additionally, in order to get the correct sign of the pitch, we need to multiply
		 * the new x axis of the rotation matrix with -1
		 *
		 * original:			modified:
		 *
		 * Rxx  Ryx  Rzx		-Rzx  Ryx  Rxx
		 * Rxy	Ryy  Rzy		-Rzy  Ryy  Rxy
		 * Rxz	Ryz  Rzz		-Rzz  Ryz  Rxz
		 * */
		math::Matrix3F3 R_adapted = _R;		//modified rotation matrix

		/* move z to x */
		R_adapted[0, 0] = _R[0, 2];
		R_adapted[1, 0] = _R[1, 2];
		R_adapted[2, 0] = _R[2, 2];

		/* move x to z */
		R_adapted[0, 2] = _R[0, 0];
		R_adapted[1, 2] = _R[1, 0];
		R_adapted[2, 2] = _R[2, 0];

		/* change direction of pitch (convert to right handed system) */
		R_adapted[0, 0] = -R_adapted[0, 0];
		R_adapted[1, 0] = -R_adapted[1, 0];
		R_adapted[2, 0] = -R_adapted[2, 0];
		euler_angles = R_adapted.ToEuler();  //adapted euler angles for fixed wing operation

		/* fill in new attitude data */
		_R = R_adapted;
		_roll    = euler_angles[0];
		_pitch   = euler_angles[1];
		_yaw     = euler_angles[2];

		/* lastly, roll- and yawspeed have to be swaped */
		float helper = CVT.VAtt.RollSpeed;
		CVT.VAtt.RollSpeed = -CVT.VAtt.YawSpeed;
		CVT.VAtt.YawSpeed = helper;
	}

	// the position controller will not emit attitude setpoints in some modes
	// we need to make sure that this flag is reset
	CVT.VAttSp.FwControlYaw = CVT.VAttSp.FwControlYaw && CVT.VControlMode.ControlAutoEnabled;

	/* lock integrator until control is started */
	bool lock_integrator = !(CVT.VControlMode.ControlRatesEnabled && !CVT.VehicleStatus.IsRotaryWing);

	/* Simple handling of failsafe: deploy parachute if failsafe is on */
	if (CVT.VControlMode.ControlTerminationEnabled)
	{
		m_ActuatorControls2.Control[7] = 1.0f;
		//warnx("_actuators_airframe.control[1] = 1.0f;");

	} else {
		m_ActuatorControls2.Control[7] = 0.0f;
		//warnx("_actuators_airframe.control[1] = -1.0f;");
	}

	/* if we are in rotary wing mode, do nothing */
	if (CVT.VehicleStatus.IsRotaryWing && !CVT.VehicleStatus.IsVtol)
	{
		return;
	}

	/* default flaps to center */
	float flap_control = 0.0f;

	/* map flaps by default to manual if valid */
	if(isfinite(CVT.ManualControlSp.Flaps) && CVT.VControlMode.ControlManualEnabled
	    && fabsf(ParamTblPtr->FW_FLAPS_SCL) > 0.01f)
	{
		flap_control = 0.5f * (CVT.ManualControlSp.Flaps + 1.0f) * ParamTblPtr->FW_FLAPS_SCL;

	} else if (CVT.VControlMode.ControlAutoEnabled
		   && fabsf(ParamTblPtr->FW_FLAPS_SCL) > 0.01f)
	{
		flap_control = CVT.VAttSp.ApplyFlaps ? 1.0f * ParamTblPtr->FW_FLAPS_SCL : 0.0f;
	}

	// move the actual control value continuous with time, full flap travel in 1sec
	if (fabsf(_flaps_applied - flap_control) > 0.01f)
	{
		_flaps_applied += (_flaps_applied - flap_control) < 0 ? dt : -dt;
	}
	else
	{
		_flaps_applied = flap_control;
	}

	/* default flaperon to center */
	float flaperon_control = 0.0f;

	/* map flaperons by default to manual if valid */
	if(isfinite(CVT.ManualControlSp.Aux2) && CVT.VControlMode.ControlManualEnabled
	    && fabsf(ParamTblPtr->FW_FLAPERON_SCL) > 0.01f)
	{
		flaperon_control = 0.5f * (CVT.ManualControlSp.Aux2 + 1.0f) * ParamTblPtr->FW_FLAPERON_SCL;
	}
	else if(CVT.VControlMode.ControlAutoEnabled
		   && fabsf(ParamTblPtr->FW_FLAPERON_SCL) > 0.01f)
	{
		flaperon_control = CVT.VAttSp.ApplyFlaps ? 1.0f * ParamTblPtr->FW_FLAPERON_SCL : 0.0f;
	}

	// move the actual control value continuous with time, full flap travel in 1sec
	if (fabsf(_flaperons_applied - flaperon_control) > 0.01f)
	{
		_flaperons_applied += (_flaperons_applied - flaperon_control) < 0 ? dt : -dt;
	}
	else
	{
		_flaperons_applied = flaperon_control;
	}

	// Check if we are in rattitude mode and the pilot is above the threshold on pitch
	if (CVT.VControlMode.ControlRattitudeEnabled)
	{
		if (fabsf(CVT.ManualControlSp.Y) > ParamTblPtr->FW_RATT_TH ||
		    fabsf(CVT.ManualControlSp.X) > ParamTblPtr->FW_RATT_TH)
		{
			CVT.VControlMode.ControlAttitudeEnabled = false;
		}
	}

	/* decide if in stabilized or full manual control */
	if (CVT.VControlMode.ControlRatesEnabled)
	{
		/* scale around tuning airspeed */
		float airspeed;

		/* if airspeed is non-finite or not valid or if we are asked not to control it, we assume the normal average speed */
		const bool airspeed_valid = isfinite(CVT.Airspeed.IndicatedAirspeed) && (CVT.Airspeed.Timestamp > 0);

		if(airspeed_valid)
		{
			/* prevent numerical drama by requiring 0.5 m/s minimal speed */
			airspeed = math::max(0.5f, CVT.Airspeed.IndicatedAirspeed);

		} else {
			airspeed = ParamTblPtr->FW_AIRSPD_TRIM;
		}

		/*
		 * For scaling our actuators using anything less than the min (close to stall)
		 * speed doesn't make any sense - its the strongest reasonable deflection we
		 * want to do in flight and its the baseline a human pilot would choose.
		 *
		 * Forcing the scaling to this value allows reasonable handheld tests.
		 */
		float airspeed_scaling = ParamTblPtr->FW_AIRSPD_TRIM / ((airspeed < ParamTblPtr->FW_AIRSPD_MIN) ? ParamTblPtr->FW_AIRSPD_MIN :
					 airspeed);

		/* Use min airspeed to calculate ground speed scaling region.
		 * Don't scale below gspd_scaling_trim
		 */
		float groundspeed = sqrtf(CVT.VGlobalPosition.VelN * CVT.VGlobalPosition.VelN +
								  CVT.VGlobalPosition.VelE * CVT.VGlobalPosition.VelE);
		float gspd_scaling_trim = (ParamTblPtr->FW_AIRSPD_MIN * 0.6f);
		float groundspeed_scaler = gspd_scaling_trim / ((groundspeed < gspd_scaling_trim) ? gspd_scaling_trim : groundspeed);

//		// in STABILIZED mode we need to generate the attitude setpoint
//		// from manual user inputs
//		if (!CVT.VControlMode.ControlClimbRateEnabled && !CVT.VControlMode.ControlOffboardEnabled)
//		{
//			CVT.VAttSp.Timestamp = now;
//			CVT.VAttSp.RollBody = CVT.ManualControlSp.Y * ParamTblPtr->FW_MAN_R_MAX + math::radians(ParamTblPtr->FW_RSP_OFF);
//			CVT.VAttSp.RollBody = math::constrain(CVT.VAttSp.RollBody, -ParamTblPtr->FW_MAN_R_MAX, ParamTblPtr->FW_MAN_R_MAX);
//			CVT.VAttSp.PitchBody = -CVT.ManualControlSp.X * ParamTblPtr->FW_MAN_P_MAX + math::radians(ParamTblPtr->FW_PSP_OFF);
//			CVT.VAttSp.PitchBody = math::constrain(CVT.VAttSp.PitchBody, -ParamTblPtr->FW_MAN_P_MAX, ParamTblPtr->FW_MAN_P_MAX);
//			CVT.VAttSp.YawBody = 0.0f;
//			CVT.VAttSp.Thrust = CVT.ManualControlSp.Z;
//
//			math::Quaternion q(math::Euler(CVT.VAttSp.RollBody, CVT.VAttSp.PitchBody, CVT.VAttSp.YawBody));
//			q.copyTo(CVT.VAttSp.Q_D);
//			CVT.VAttSp.Q_D_Valid = true;
//
//          SendOutputData((CFE_SB_Msg_t*)&CVT.VAttSp);
//		}

		/* reset integrals where needed */
		if (CVT.VAttSp.RollResetIntegral)
        {
			_roll_ctrl.reset_integrator();
		}

		if (CVT.VAttSp.PitchResetIntegral)
        {
			_pitch_ctrl.reset_integrator();
		}

		if (CVT.VAttSp.YawResetIntegral)
        {
			_yaw_ctrl.reset_integrator();
			_wheel_ctrl.reset_integrator();
		}

		/* Reset integrators if the aircraft is on ground
		 * or a multicopter (but not transitioning VTOL)
		 */
		if (CVT.VLandDetected.Landed
		    || (CVT.VehicleStatus.IsRotaryWing && !CVT.VehicleStatus.InTransitionMode))
		{

			_roll_ctrl.reset_integrator();
			_pitch_ctrl.reset_integrator();
			_yaw_ctrl.reset_integrator();
			_wheel_ctrl.reset_integrator();
		}

		float roll_sp = CVT.VAttSp.RollBody;
		float pitch_sp = CVT.VAttSp.PitchBody;
		float yaw_sp = CVT.VAttSp.YawBody;
		float throttle_sp = CVT.VAttSp.Thrust;

		/* Prepare data for attitude controllers */
		struct ECL_ControlData control_input = {};
		control_input.roll = _roll;
		control_input.pitch = _pitch;
		control_input.yaw = _yaw;
		control_input.body_x_rate = CVT.VAtt.RollSpeed;
		control_input.body_y_rate = CVT.VAtt.PitchSpeed;
		control_input.body_z_rate = CVT.VAtt.YawSpeed;
		control_input.roll_setpoint = roll_sp;
		control_input.pitch_setpoint = pitch_sp;
		control_input.yaw_setpoint = yaw_sp;
		control_input.airspeed_min = ParamTblPtr->FW_AIRSPD_MIN;
		control_input.airspeed_max = ParamTblPtr->FW_AIRSPD_MAX;
		control_input.airspeed = airspeed;
		control_input.scaler = airspeed_scaling;
		control_input.lock_integrator = lock_integrator;
		control_input.groundspeed = groundspeed;
		control_input.groundspeed_scaler = groundspeed_scaler;

		_yaw_ctrl.set_coordinated_method(ParamTblPtr->FW_YCO_METHOD);

		/* Run attitude controllers */
		if (CVT.VControlMode.ControlAttitudeEnabled)
		{
			if(isfinite(roll_sp) && isfinite(pitch_sp))
			{
				_roll_ctrl.control_attitude(control_input);
				_pitch_ctrl.control_attitude(control_input);
				_yaw_ctrl.control_attitude(control_input); //runs last, because is depending on output of roll and pitch attitude
				_wheel_ctrl.control_attitude(control_input);

				/* Update input data for rate controllers */
				control_input.roll_rate_setpoint = _roll_ctrl.get_desired_rate();
				control_input.pitch_rate_setpoint = _pitch_ctrl.get_desired_rate();
				control_input.yaw_rate_setpoint = _yaw_ctrl.get_desired_rate();

				/* Run attitude RATE controllers which need the desired attitudes from above, add trim */
				float roll_u = _roll_ctrl.control_euler_rate(control_input);
				m_ActuatorControls0.Control[PX4_ACTUATOR_CONTROL_ROLL] = (isfinite(roll_u)) ? roll_u + ParamTblPtr->TRIM_ROLL :
						ParamTblPtr->TRIM_ROLL;

				if (!isfinite(roll_u))
				{
					_roll_ctrl.reset_integrator();
				}

				float pitch_u = _pitch_ctrl.control_euler_rate(control_input);
				m_ActuatorControls0.Control[PX4_ACTUATOR_CONTROL_PITCH] = (isfinite(pitch_u)) ? pitch_u + ParamTblPtr->TRIM_PITCH :
						ParamTblPtr->TRIM_PITCH;

				if (!isfinite(pitch_u))
				{
					_pitch_ctrl.reset_integrator();
				}

				float yaw_u = 0.0f;

				if (ParamTblPtr->FW_W_EN && CVT.VAttSp.FwControlYaw)
				{
					yaw_u = _wheel_ctrl.control_bodyrate(control_input);
				}
				else
				{
					yaw_u = _yaw_ctrl.control_euler_rate(control_input);
				}

				m_ActuatorControls0.Control[PX4_ACTUATOR_CONTROL_YAW] = (isfinite(yaw_u)) ? yaw_u + ParamTblPtr->TRIM_YAW :
						ParamTblPtr->TRIM_YAW;

				/* add in manual rudder control in manual modes */
				if (CVT.VControlMode.ControlManualEnabled)
                {
					m_ActuatorControls0.Control[PX4_ACTUATOR_CONTROL_YAW] += CVT.ManualControlSp.R;
				}

				if (!isfinite(yaw_u)) {
					_yaw_ctrl.reset_integrator();
					_wheel_ctrl.reset_integrator();
				}

				/* throttle passed through if it is finite and if no engine failure was detected */
				m_ActuatorControls0.Control[PX4_ACTUATOR_CONTROL_THROTTLE] = (isfinite(throttle_sp) &&
						!(CVT.VehicleStatus.EngineFailure ||
								CVT.VehicleStatus.EngineFailureCmd)) ?
						throttle_sp : 0.0f;

				/* scale effort by battery status */
				if (ParamTblPtr->FW_BAT_SCALE_EN && CVT.BatteryStatus.Scale > 0.0f &&
						m_ActuatorControls0.Control[PX4_ACTUATOR_CONTROL_THROTTLE] > 0.1f)
				{
					m_ActuatorControls0.Control[PX4_ACTUATOR_CONTROL_THROTTLE] *= CVT.BatteryStatus.Scale;
				}
			}
		}
		else
		{
			// pure rate control
			_roll_ctrl.set_bodyrate_setpoint(CVT.ManualControlSp.Y * math::radians(ParamTblPtr->FW_ACRO_X_MAX));
			_pitch_ctrl.set_bodyrate_setpoint(-CVT.ManualControlSp.X * math::radians(ParamTblPtr->FW_ACRO_Y_MAX));
			_yaw_ctrl.set_bodyrate_setpoint(CVT.ManualControlSp.R * math::radians(ParamTblPtr->FW_ACRO_Z_MAX));

			float roll_u = _roll_ctrl.control_bodyrate(control_input);
			m_ActuatorControls0.Control[PX4_ACTUATOR_CONTROL_ROLL] = (isfinite(roll_u)) ? roll_u + ParamTblPtr->TRIM_ROLL :
					ParamTblPtr->TRIM_ROLL;

			float pitch_u = _pitch_ctrl.control_bodyrate(control_input);
			m_ActuatorControls0.Control[PX4_ACTUATOR_CONTROL_PITCH] = (isfinite(pitch_u)) ? pitch_u + ParamTblPtr->TRIM_PITCH :
					ParamTblPtr->TRIM_PITCH;

			float yaw_u = _yaw_ctrl.control_bodyrate(control_input);
			m_ActuatorControls0.Control[PX4_ACTUATOR_CONTROL_YAW] = (isfinite(yaw_u)) ? yaw_u + ParamTblPtr->TRIM_YAW :
					ParamTblPtr->TRIM_YAW;

			m_ActuatorControls0.Control[PX4_ACTUATOR_CONTROL_THROTTLE] = (isfinite(throttle_sp) &&
					//!(_vehicle_status.engine_failure ||
					!CVT.VehicleStatus.EngineFailureCmd) ?
					throttle_sp : 0.0f;
		}

		/*
		 * Lazily publish the rate setpoint (for analysis, the actuators are published below)
		 * only once available
		 */
		m_VehicleRatesSetpoint.Roll = _roll_ctrl.get_desired_bodyrate();
		m_VehicleRatesSetpoint.Pitch = _pitch_ctrl.get_desired_bodyrate();
		m_VehicleRatesSetpoint.Yaw = _yaw_ctrl.get_desired_bodyrate();

		m_VehicleRatesSetpoint.Timestamp = now;

		SendOutputData((CFE_SB_Msg_t*)&m_VehicleRatesSetpoint);
		HkTlm.VehicleRatesSetpointMsgSndCnt++;
	}
	else
	{
		/* Manual/direct control */
		m_ActuatorControls0.Control[PX4_ACTUATOR_CONTROL_ROLL]  = CVT.ManualControlSp.Y * ParamTblPtr->FW_MAN_R_SC + ParamTblPtr->TRIM_ROLL;
		m_ActuatorControls0.Control[PX4_ACTUATOR_CONTROL_PITCH] = -CVT.ManualControlSp.X * ParamTblPtr->FW_MAN_P_SC + ParamTblPtr->TRIM_PITCH;
		m_ActuatorControls0.Control[PX4_ACTUATOR_CONTROL_YAW]   = CVT.ManualControlSp.R * ParamTblPtr->FW_MAN_Y_SC + ParamTblPtr->TRIM_YAW;
		m_ActuatorControls0.Control[PX4_ACTUATOR_CONTROL_THROTTLE] = CVT.ManualControlSp.Z;
	}

	// Add feed-forward from roll control output to yaw control output
	// This can be used to counteract the adverse yaw effect when rolling the plane
	m_ActuatorControls0.Control[PX4_ACTUATOR_CONTROL_YAW] += ParamTblPtr->FW_RLL_TO_YAW_FF * math::constrain(
			m_ActuatorControls0.Control[PX4_ACTUATOR_CONTROL_ROLL], -1.0f, 1.0f);

	m_ActuatorControls0.Control[PX4_ACTUATOR_CONTROL_FLAPS] = _flaps_applied;
	m_ActuatorControls0.Control[PX4_ACTUATOR_CONTROL_SPOILERS] = CVT.ManualControlSp.Aux1;
	m_ActuatorControls0.Control[PX4_ACTUATOR_CONTROL_AIRBRAKES] = _flaperons_applied;
	// FIXME: this should use _vcontrol_mode.landing_gear_pos in the future
	m_ActuatorControls0.Control[PX4_ACTUATOR_CONTROL_LANDING_GEAR] = CVT.ManualControlSp.Aux3;

	/* lazily publish the setpoint only once available */
	m_ActuatorControls0.Timestamp = now;
	m_ActuatorControls0.SampleTime = CVT.VAtt.Timestamp;
	m_ActuatorControls2.Timestamp = now;
	m_ActuatorControls2.SampleTime = CVT.VAtt.Timestamp;

	/* Only publish if any of the proper modes are enabled */
	if (CVT.VControlMode.ControlRatesEnabled ||
	    CVT.VControlMode.ControlAttitudeEnabled ||
	    CVT.VControlMode.ControlManualEnabled)
	{
		/* publish the actuator controls */
		SendOutputData((CFE_SB_Msg_t*)&m_ActuatorControls0);
		HkTlm.ActuatorControls0MsgSndCnt++;
		SendOutputData((CFE_SB_Msg_t*)&m_ActuatorControls2);
		HkTlm.ActuatorControls2MsgSndCnt++;
	}

}

void FAC::UpdateParams(void)
{
	/* pitch control parameters */
	_pitch_ctrl.set_time_constant(ParamTblPtr->FW_P_TC);
	_pitch_ctrl.set_k_p(ParamTblPtr->FW_PR_P);
	_pitch_ctrl.set_k_i(ParamTblPtr->FW_PR_I);
	_pitch_ctrl.set_k_ff(ParamTblPtr->FW_PR_FF);
	_pitch_ctrl.set_integrator_max(ParamTblPtr->FW_PR_IMAX);
	_pitch_ctrl.set_max_rate_pos(math::radians(ParamTblPtr->FW_P_RMAX_POS));
	_pitch_ctrl.set_max_rate_neg(math::radians(ParamTblPtr->FW_P_RMAX_NEG));

	/* roll control parameters */
	_roll_ctrl.set_time_constant(ParamTblPtr->FW_R_TC);
	_roll_ctrl.set_k_p(ParamTblPtr->FW_RR_P);
	_roll_ctrl.set_k_i(ParamTblPtr->FW_RR_I);
	_roll_ctrl.set_k_ff(ParamTblPtr->FW_RR_FF);
	_roll_ctrl.set_integrator_max(ParamTblPtr->FW_RR_IMAX);
	_roll_ctrl.set_max_rate(math::radians(ParamTblPtr->FW_R_RMAX));

	/* yaw control parameters */
	_yaw_ctrl.set_k_p(ParamTblPtr->FW_YR_P);
	_yaw_ctrl.set_k_i(ParamTblPtr->FW_YR_I);
	_yaw_ctrl.set_k_ff(ParamTblPtr->FW_YR_FF);
	_yaw_ctrl.set_integrator_max(ParamTblPtr->FW_YR_IMAX);
	_yaw_ctrl.set_coordinated_min_speed(ParamTblPtr->FW_YCO_VMIN);
	_yaw_ctrl.set_coordinated_method(ParamTblPtr->FW_YCO_METHOD);
	_yaw_ctrl.set_max_rate(math::radians(ParamTblPtr->FW_Y_RMAX));

	/* wheel control parameters */
	_wheel_ctrl.set_k_p(ParamTblPtr->FW_WR_P);
	_wheel_ctrl.set_k_i(ParamTblPtr->FW_WR_I);
	_wheel_ctrl.set_k_ff(ParamTblPtr->FW_WR_FF);
	_wheel_ctrl.set_integrator_max(ParamTblPtr->FW_WR_IMAX);
	_wheel_ctrl.set_max_rate(math::radians(ParamTblPtr->FW_W_RMAX));
}

/************************/
/*  End of File Comment */
/************************/
