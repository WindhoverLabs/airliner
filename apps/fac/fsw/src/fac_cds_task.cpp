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

#include <cfe.h>
#include <cfe_error.h>

#include "fac_app.hpp"
#include "fac_cds_task.hpp"
#include "fac_msgids.h"

#include <string.h>


const FAC_CDS_DataGrpTbl_t FAC_Cds_GrpTbl[] =
{
   { PX4_ACTUATOR_ARMED_MID,        FAC_GROUP_NONE },
   { PX4_ACTUATOR_CONTROLS_0_MID,   FAC_GROUP_NONE },
   { PX4_ACTUATOR_CONTROLS_1_MID,   FAC_GROUP_NONE },
   { PX4_ACTUATOR_CONTROLS_2_MID,   FAC_GROUP_NONE },
   { PX4_ACTUATOR_CONTROLS_3_MID,   FAC_GROUP_NONE },
   { PX4_ACTUATOR_DIRECT_MID,       FAC_GROUP_NONE },
   { PX4_ACTUATOR_OUTPUTS_MID,      FAC_GROUP_NONE },

   { PX4_ADC_REPORT_MID,            FAC_GROUP_NONE },
   { PX4_AIRSPEED_MID,              FAC_GROUP_NONE },
   { PX4_ATT_POS_MOCAP_MID,         FAC_GROUP_NONE },
   { PX4_BATTERY_STATUS_MID,        FAC_GROUP_NONE },
   { PX4_CAMERA_TRIGGER_MID,        FAC_GROUP_NONE },
   { PX4_COMMANDER_STATE_MID,       FAC_GROUP_NONE },
   { PX4_CONTROL_STATE_MID,         FAC_GROUP_NONE },
   { PX4_CPULOAD_MID,               FAC_GROUP_NONE },
   { PX4_DEBUG_KEY_VALUE_MID,       FAC_GROUP_NONE },
   { PX4_DIFFERENTIAL_PRESSURE_MID, FAC_GROUP_NONE },
   { PX4_DISTANCE_SENSOR_MID,       FAC_GROUP_NONE },

   { PX4_FW_POS_CTRL_STATUS_MID,           FAC_GROUP_NONE },
   { PX4_FW_VIRTUAL_ATTITUDE_SETPOINT_MID, FAC_GROUP_NONE },
   { PX4_FW_VIRTUAL_RATES_SETPOINT_MID,    FAC_GROUP_NONE },

   { PX4_EKF2_INNOVATIONS_MID,      FAC_GROUP_NONE },
   { PX4_EKF2_REPLAY_MID,           FAC_GROUP_NONE },

   { PX4_ESC_REPORT_MID,            FAC_GROUP_NONE },
   { PX4_ESC_STATUS_MID,            FAC_GROUP_NONE },

   { PX4_ESTIMATOR_STATUS_MID,      FAC_GROUP_NONE },

   { PX4_FENCE_MID,                 FAC_GROUP_NONE },
   { PX4_FENCE_VERTEX_MID,          FAC_GROUP_NONE },

   { PX4_FILTERED_BOTTOM_FLOW_MID,  FAC_GROUP_NONE },
   { PX4_FOLLOW_TARGET_MID,         FAC_GROUP_NONE },
   { PX4_GEOFENCE_RESULT_MID,       FAC_GROUP_NONE },

   { PX4_GPS_DUMP_MID,              FAC_GROUP_NONE },
   { PX4_GPS_INJECT_DATA_MID,       FAC_GROUP_NONE },

   { PX4_HIL_SENSOR_MID,            FAC_GROUP_NONE },
   { PX4_HOME_POSITION_MID,         FAC_GROUP_NONE },
   { PX4_INPUT_RC_MID,              FAC_GROUP_NONE },
   { PX4_LED_CONTROL_MID,           FAC_GROUP_NONE },
   { PX4_LOG_MESSAGE_MID,           FAC_GROUP_NONE },
   { PX4_MANUAL_CONTROL_SETPOINT_MID, FAC_GROUP_NONE },
   { PX4_MAVLINK_LOG_MID,           FAC_GROUP_NONE },

   { PX4_MC_ATT_CTRL_STATUS_MID,    FAC_GROUP_NONE },
   { PX4_MC_VIRTUAL_ATTITUDE_SETPOINT_MID, FAC_GROUP_NONE },
   { PX4_MC_VIRTUAL_RATES_SETPOINT_MID, FAC_GROUP_NONE },

   { PX4_MISSION_MID,               FAC_GROUP_NONE },
   { PX4_MISSION_RESULT_MID,        FAC_GROUP_NONE },

   { PX4_MULTIROTOR_MOTOR_LIMITS_MID, FAC_GROUP_NONE },
   { PX4_OFFBOARD_CONTROL_MODE_MID, FAC_GROUP_NONE },
   { PX4_OPTICAL_FLOW_MID,          FAC_GROUP_NONE },
   { PX4_OUTPUT_PWM_MID,            FAC_GROUP_NONE },
   { PX4_PARAMETER_UPDATE_MID,      FAC_GROUP_NONE },

   { PX4_POSITION_SETPOINT_MID,     FAC_GROUP_NONE },
   { PX4_POSITION_SETPOINT_TRIPLET_MID, FAC_GROUP_NONE },

   { PX4_PWM_INPUT_MID,             FAC_GROUP_NONE },
   { PX4_QSHELL_REQ_MID,            FAC_GROUP_NONE },

   { PX4_RC_CHANNELS_MID,           FAC_GROUP_NONE },
   { PX4_RC_PARAMETER_MAP_MID,      FAC_GROUP_NONE },

   { PX4_SAFETY_MID,                FAC_GROUP_NONE },
   { PX4_SATELLITE_INFO_MID,        FAC_GROUP_NONE },

   { PX4_SENSOR_ACCEL_MID,          FAC_GROUP_NONE },
   { PX4_SENSOR_BARO_MID,           FAC_GROUP_NONE },
   { PX4_SENSOR_COMBINED_MID,       FAC_GROUP_NONE },
   { PX4_SENSOR_GYRO_MID,           FAC_GROUP_NONE },
   { PX4_SENSOR_MAG_MID,            FAC_GROUP_NONE },

   { PX4_SERVORAIL_STATUS_MID,      FAC_GROUP_NONE },
   { PX4_SUBSYSTEM_INFO_MID,        FAC_GROUP_NONE },
   { PX4_SYSTEM_POWER_MID,          FAC_GROUP_NONE },
   { PX4_TECS_STATUS_MID,           FAC_GROUP_NONE },
   { PX4_TELEMETRY_STATUS_MID,      FAC_GROUP_NONE },
   { PX4_TEST_MOTOR_MID,            FAC_GROUP_NONE },
   { PX4_TIME_OFFSET_MID,           FAC_GROUP_NONE },
   { PX4_TRANSPONDER_REPORT_MID,    FAC_GROUP_NONE },

   { PX4_UAVCAN_PARAMETER_REQUEST_MID, FAC_GROUP_NONE },
   { PX4_UAVCAN_PARAMETER_VALUE_MID, FAC_GROUP_NONE },

   { PX4_VEHICLE_ATTITUDE_MID,      FAC_GROUP_NONE },
   { PX4_VEHICLE_ATTITUDE_SETPOINT_MID, FAC_GROUP_NONE },
   { PX4_VEHICLE_COMMAND_ACK_MID,   FAC_GROUP_NONE },
   { PX4_VEHICLE_COMMAND_MID,       FAC_GROUP_NONE },
   { PX4_VEHICLE_CONTROL_MODE_MID,  FAC_GROUP_NONE },
   { PX4_VEHICLE_FORCE_SETPOINT_MID, FAC_GROUP_NONE },
   { PX4_VEHICLE_GLOBAL_POSITION_MID, FAC_GROUP_NONE },
   { PX4_VEHICLE_GLOBAL_VELOCITY_SETPOINT_MID, FAC_GROUP_NONE },
   { PX4_VEHICLE_GPS_POSITION_MID,  FAC_GROUP_NONE },
   { PX4_VEHICLE_LAND_DETECTED_MID, FAC_GROUP_NONE },
   { PX4_VEHICLE_LOCAL_POSITION_MID, FAC_GROUP_NONE },
   { PX4_VEHICLE_LOCAL_POSITION_SETPOINT_MID, FAC_GROUP_NONE },
   { PX4_VEHICLE_RATES_SETPOINT_MID, FAC_GROUP_NONE },
   { PX4_VEHICLE_STATUS_MID,        FAC_GROUP_NONE },

   { PX4_VISION_POSITION_ESTIMATE_MID, FAC_GROUP_NONE },
   { PX4_VTOL_VEHICLE_STATUS_MID,   FAC_GROUP_NONE },
   { PX4_WIND_ESTIMATE_MID,         FAC_GROUP_NONE },
   { PX4_SENSOR_CORRECTION_MID,     FAC_GROUP_NONE }
};

const FAC_CDS_PX4HandlerTblRec_t FAC_Cds_PX4MsgHandlerTbl[] =
{
#if 0
   {FAC_GROUP_NONE,     sizeof(FAC_GrpNone_t),     FAC_PX4GrpNoneFunc},
   {FAC_GROUP_Actuator, sizeof(FAC_GrpActuator_t), FAC_PX4GrpActuatorFunc},
   {FAC_GROUP_Fw,       sizeof(FAC_GrpFw_t),       FAC_PX4GrpFwFunc},
   {FAC_GROUP_Vehicle,  sizeof(FAC_GrpVehicle_t),  FAC_PX4GrpVehicleFunc},
    /*   ....   */
   {FAC_GROUP_Sensor,   sizeof(FAC_GrpSensor_t),   FAC_PX4GrpSensorFunc}
#endif
};

FAC_CdsTaskData_t FAC_CdsTaskData;

extern FAC_AppData_t FAC_MainAppData;

CriticalDataStorage::CriticalDataStorage()
{
   FAC_CdsTaskData.CdsTaskId = 0;
   FAC_CdsTaskData.uiCdsRunStatus = FAC_TASK_RUN;
   FAC_CdsTaskData.pCds = NULL;

   FAC_CdsTaskData.ConfigTblPtr = NULL;
}

CriticalDataStorage::~CriticalDataStorage()
{
   FAC_CdsTaskData.CdsTaskId = 0;
   FAC_CdsTaskData.uiCdsRunStatus = FAC_TASK_EXIT;

   FAC_CdsTaskData.ConfigTblPtr = NULL;
}

int32 CriticalDataStorage::GetCdsGrpId(CFE_SB_MsgId_t DataMsgId)
{
   int32 GrpId = 0;
   int32 cds_len = ( sizeof(FAC_Cds_GrpTbl) / sizeof(FAC_CDS_DataGrpTbl_t) );  // check real size

   for (int i = 0; i < cds_len; i++)
   {
      if (FAC_Cds_GrpTbl[i].msgId == DataMsgId)
      {
         GrpId = FAC_Cds_GrpTbl[i].cdsGrpId;
         break;
      }
   }

   return GrpId;
}

int32 CriticalDataStorage::RcvDataMsg(int32 iBlocking)
{
   int32          CdsGrpId = 0;
   int32          iStatus = CFE_SUCCESS;
   CFE_SB_Msg_t   *DataMsgPtr = NULL;
   CFE_SB_MsgId_t DataMsgId;

   while (FAC_CdsTaskData.uiCdsRunStatus == FAC_TASK_RUN)
   {
      iStatus = CFE_SB_RcvMsg(&DataMsgPtr, DataPipeId, CFE_SB_POLL); // check CFE_SB_POLL
      if (iStatus == CFE_SUCCESS)
      {
         DataMsgId = CFE_SB_GetMsgId(DataMsgPtr);
         CdsGrpId = GetCdsGrpId(DataMsgId);
         if (CdsGrpId > 0)
         {
            // Find the handler and execute it
         }
         else
         {
            CFE_EVS_SendEvent(FAC_MSGID_ERR_EID, CFE_EVS_ERROR,
                              "Recvd invalid data msgId (0x%04X)",
                              (unsigned short)DataMsgId);
            FAC_CdsTaskData.uiCdsRunStatus = FAC_TASK_ERROR; // Decide what to do: continue?
            goto RcvDataMsg_Exit_Tag;
         }
      }
      else if (iStatus == CFE_SB_NO_MESSAGE)
      {
         iStatus = CFE_SUCCESS;
      }
      else
      {
         CFE_EVS_SendEvent(FAC_PIPE_ERR_EID, CFE_EVS_ERROR,
                           "Data pipe read error (0x%08X)", (unsigned int)iStatus);
         FAC_CdsTaskData.uiCdsRunStatus = FAC_TASK_ERROR;
         goto RcvDataMsg_Exit_Tag;
      }

      iStatus = FAC_AcquireConfigPointers();
      if (iStatus != CFE_SUCCESS)
      {
         CFE_EVS_SendEvent(FAC_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
                           "AcquireConfigPointers error (0x%08X)",
                           (unsigned int)iStatus);
         FAC_CdsTaskData.uiCdsRunStatus = FAC_TASK_ERROR;
         goto RcvDataMsg_Exit_Tag;
      }
      else
      {
#if 0
         CFE_EVS_SendEvent(FAC_CONFIG_TABLE_INF_EID, CFE_EVS_INFORMATION,
                           "Succeeded to AcquireConfigPointers ********** (0x%08X)",
                           (unsigned int)iStatus);
#endif
      }
   }

RcvDataMsg_Exit_Tag:
   return iStatus;
}

boolean CriticalDataStorage::VerifyCdsMsgLength(CFE_SB_Msg_t *MsgPtr, uint16 usExpectedLen)
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
                           "msgLen=%d, expectedLen=%d", MsgId, usCmdCode,
                           usMsgLen, usExpectedLen);
      }
// Add more to check
   }

   return (bResult);
}

int32 CriticalDataStorage::InitData()
{
   int32  iStatus = CFE_SUCCESS;

   /* Init input data */
   memset((void*)&InData, 0x00, sizeof(InData));

   /* Init output data */
//   memset((void*)&OutData, 0x00, sizeof(OutData));   // Check for memset
   /* memset entire OutData length and set the stream ID and length and
    * restore sequence count if not TRUE
    */
// check cfe function for length
   CFE_SB_InitMsg(&OutData, FAC_OUT_DATA_MID, sizeof(OutData), TRUE); // check length

   return (iStatus);
}

int32 CriticalDataStorage::InitPipe()
{
   int32 iStatus = CFE_SUCCESS;
   int32 cds_len = ( sizeof(FAC_Cds_GrpTbl) / sizeof(FAC_CDS_DataGrpTbl_t) );  // check real size

   iStatus = CFE_SB_CreatePipe(&DataPipeId, FAC_DATA_PIPE_DEPTH, FAC_DATA_PIPE_NAME);
   if (iStatus == CFE_SUCCESS)
   {
      for ( int i = 0; i < cds_len; i++)
      {
         iStatus = CFE_SB_SubscribeEx(FAC_Cds_GrpTbl[i].msgId, DataPipeId,
                                      CFE_SB_Default_Qos, FAC_DATA_PIPE_RESERVED);
         if (iStatus != CFE_SUCCESS)
         {
            CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                              "Failed to subscribe to MID:0x%04X (0x%08X)",
                              FAC_Cds_GrpTbl[i].msgId, (unsigned int)iStatus);
            FAC_CdsTaskData.uiCdsRunStatus = FAC_TASK_ERROR;
            goto InitPipe_Exit_Tag;
         }
      }
   }
   else
   {
      CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                        "Failed to create Data pipe (0x%08X)",
                        (unsigned int)iStatus);
      FAC_CdsTaskData.uiCdsRunStatus = FAC_TASK_ERROR;
      goto InitPipe_Exit_Tag;
   }

InitPipe_Exit_Tag:
   return iStatus;
}

int32 CriticalDataStorage::InitCdsTask()
{
   int32 iStatus = CFE_SUCCESS;

   iStatus = InitPipe();
   if (iStatus != CFE_SUCCESS)
   {
      CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                        "Failed to Init pipe (0x%08X)", (unsigned int)iStatus);
      FAC_CdsTaskData.uiCdsRunStatus = FAC_TASK_ERROR;
      goto InitCdsTask_Exit_Tag;
   }

   iStatus = InitData();
   if (iStatus != CFE_SUCCESS)
   {
      CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                        "Failed to initialize Data (0x%08X)",
                        (unsigned int)iStatus);
      FAC_CdsTaskData.uiCdsRunStatus = FAC_TASK_ERROR;
      goto InitCdsTask_Exit_Tag;
   }

   iStatus = FAC_InitConfigTbl();
   if (iStatus != CFE_SUCCESS)
   {
      CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                        "Failed to init config tables (0x%08X)",
                        (unsigned int)iStatus);
      FAC_CdsTaskData.uiCdsRunStatus = FAC_TASK_ERROR;
      goto InitCdsTask_Exit_Tag;
   }

   iStatus = FAC_InitCdsTbl();
   if (iStatus != CFE_SUCCESS)
   {
      CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                        "Failed to init CDS table (0x%08X)",
                        (unsigned int)iStatus);
      FAC_CdsTaskData.uiCdsRunStatus = FAC_TASK_ERROR;
      goto InitCdsTask_Exit_Tag;
   }

InitCdsTask_Exit_Tag:
   return iStatus;
}

void CdsTask()
{
   int32 iStatus = CFE_SUCCESS;
   CFE_ES_TaskInfo_t CdsTaskInfo;

   iStatus = CFE_ES_RegisterChildTask();
   if (iStatus != CFE_SUCCESS)
   {
      CFE_ES_WriteToSysLog("FAC - Failed to register the child task (0x%08X)\n",
                           (unsigned int)iStatus);
      FAC_CdsTaskData.uiCdsRunStatus = FAC_TASK_ERROR;
      goto CdsTask_Exit_Tag;
   }

   // Shoud this task wait for startup sync again?(done in the main apptask)

   iStatus = CFE_ES_GetTaskInfo(&CdsTaskInfo, FAC_CdsTaskData.CdsTaskId);  //check taskId and appId
   if (iStatus != CFE_SUCCESS)
   {
      CFE_ES_WriteToSysLog("FAC - Failed to Get child task info(0x%08X)\n",
                           (unsigned int)iStatus);
      FAC_CdsTaskData.uiCdsRunStatus = FAC_TASK_ERROR;
      goto CdsTask_Exit_Tag;
   }
   else
   {
      FAC_MainAppData.MainAppId = CdsTaskInfo.AppId;
      CFE_EVS_SendEvent(FAC_INF_EID, CFE_EVS_INFORMATION,
             "Cds TaskInfo: TaskID: 0x%08X, TaskName: %s, AppId: 0x%08X, AppName:%s (0x%08X)\n",
             (unsigned int)CdsTaskInfo.TaskId, CdsTaskInfo.TaskName,
             (unsigned int)CdsTaskInfo.AppId, CdsTaskInfo.AppName, (unsigned int)iStatus);
   }

   CFE_ES_IncrementTaskCounter();

   // Start CDS task Performance Log entry?

   FAC_CdsTaskData.pCds = new CriticalDataStorage();
   if (FAC_CdsTaskData.pCds != NULL)
   {
      // Check performance monitoring

      iStatus = FAC_CdsTaskData.pCds->InitCdsTask();
      if (iStatus != CFE_SUCCESS)
      {
         CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                           "Failed to init cds task (0x%08X)",
                           (unsigned int)iStatus);
         FAC_CdsTaskData.uiCdsRunStatus = FAC_TASK_ERROR;
         goto CdsTask_Exit_Tag;
      }
   }
   else
   {
      FAC_CdsTaskData.uiCdsRunStatus = FAC_TASK_ERROR;
      goto CdsTask_Exit_Tag;
   }

   if (FAC_CdsTaskData.uiCdsRunStatus == FAC_TASK_RUN)
   {
      iStatus = FAC_CdsTaskData.pCds->RcvDataMsg(FAC_DATA_PIPE_PEND_TIME);
      if (iStatus != CFE_SUCCESS)
      {
         FAC_CdsTaskData.uiCdsRunStatus = FAC_TASK_ERROR;
         goto CdsTask_Exit_Tag;
      }
   }

CdsTask_Exit_Tag:
   if (FAC_CdsTaskData.pCds != NULL)
   {
      delete FAC_CdsTaskData.pCds;
      FAC_CdsTaskData.pCds = NULL;
   }
   CFE_ES_WriteToSysLog("FAC - CdsTask: Error detected. Task will exit (0x%08X)\n",
                        (unsigned int)iStatus);
   CFE_ES_ExitChildTask();
   FAC_MainAppData.uiAppRunStatus = CFE_ES_APP_EXIT;

   return;
}
