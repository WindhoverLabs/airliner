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

#include <px4_msgs.h>

#include "fac_tbldefs.h"
#include "fac_app.hpp"
#include "fac_data_utils.hpp"
#include "fac_msgids.h"

#include <string.h>


extern FAC_AppData_t  FAC_AppData;


AppDataProcess::AppDataProcess()
{
}

AppDataProcess::~AppDataProcess()
{
}

void AppDataProcess::SendOutData()
{
#if 0  // check this
   /* TODO:  Add code to update output data, if needed, here.  */

   CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&OutData);
   int32 iStatus = CFE_SB_SendMsg((CFE_SB_Msg_t*)&OutData);
   if (iStatus != CFE_SUCCESS)
   {
      /* TODO: Decide what to do if the send message fails. */
   }
#endif
}

void AppDataProcess::ProcessNewConfigTbl()
{
   /* TODO:  Add code to set new Config parameters with new values here.
   **
   ** Examples:
   **
   **    FAC_AppData.latest_sParam = FAC_AppData.ConfigTblPtr->sParam;
   **    FAC_AppData.latest_fParam = FAC_AppData.ConfigTblPtr->fParam;
   */
}

int32 AppDataProcess::AcquireConfigPointers()
{
   int32 iStatus = CFE_SUCCESS;

   /*
   ** Release the table
   */
   /* TODO: This return value can indicate success, error, or that the info has been
    * updated.  We ignore this return value in favor of checking CFE_TBL_Manage(), but
    * be sure this is the behavior you want. */
   CFE_TBL_ReleaseAddress(FAC_AppData.ConfigTblHdl);

   /*
   ** Manage the table
   */
   iStatus = CFE_TBL_Manage(FAC_AppData.ConfigTblHdl);
   if ((iStatus != CFE_SUCCESS) && (iStatus != CFE_TBL_INFO_UPDATED))
   {
      CFE_EVS_SendEvent(FAC_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
                        "Failed to manage Config table (0x%08X)",
                        (unsigned int)iStatus);
      goto AcquireConfigPointers_Exit_Tag;
   }
   else
   {
#if 0
      CFE_EVS_SendEvent(FAC_INF_EID, CFE_EVS_INFORMATION,
                        "Succeeded to manage Config Table (0x%08X)",
                        (unsigned int)iStatus);
#endif
   }

   /*
   ** Get a pointer to the table
   */
   iStatus = CFE_TBL_GetAddress((void**)&FAC_AppData.ConfigTblPtr, FAC_AppData.ConfigTblHdl);
   if (iStatus == CFE_SUCCESS)
   {
#if 1
      CFE_EVS_SendEvent(FAC_INF_EID, CFE_EVS_INFORMATION,
                        "Succeeded to get table address @@@@@@@@@ (0x%08X)",
                        (unsigned int)iStatus);
#endif
   }
   else if (iStatus == CFE_TBL_INFO_UPDATED)
   {
#if 1
      CFE_EVS_SendEvent(FAC_INF_EID, CFE_EVS_INFORMATION,
                        "Succeeded to get updated table address !!!!!!!!!!!! (0x%08X)",
                        (unsigned int)iStatus);
#endif
      ProcessNewConfigTbl();
      iStatus = CFE_SUCCESS;
   }
   else
   {
      FAC_AppData.ConfigTblPtr = NULL;
      CFE_EVS_SendEvent(FAC_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
                        "Failed to get Config table address (0x%08X)",
                        (unsigned int)iStatus);
      FAC_AppData.uiAppRunStatus = CFE_ES_APP_ERROR;
      goto AcquireConfigPointers_Exit_Tag;
   }

AcquireConfigPointers_Exit_Tag:
   return (iStatus);
}

int32 AppDataProcess::ValidateConfigTbl(void* ConfigTblPtr)
{
   int32  iStatus = CFE_SUCCESS;
   FAC_ConfigTbl_t* CfgTblPtr = (FAC_ConfigTbl_t*)(ConfigTblPtr);

   if (ConfigTblPtr == NULL)
   {
      iStatus = FAC_ERR_INVALID_POINTER;
      goto ValidateConfigTbl_Exit_Tag;
   }

   /* TODO:  Add code to validate new data values here.
   **
   ** Examples:
   ** if (CfgTblPtr->iParam <= 16) {
   **   (void) CFE_EVS_SendEvent(FAC_CONFIG_TABLE_INF_EID, CFE_EVS_ERROR,
    *                         "Invalid value for Config parameter sParam (%d)",
   **                         CfgTblPtr->iParam);
   ** }
   **/

ValidateConfigTbl_Exit_Tag:
   return (iStatus);
}

int32 AppDataProcess::VerifyDataMsgLength(CFE_SB_Msg_t *MsgPtr, uint16 usExpectedLen)
{
   uint16  usMsgLen = 0;
   int32   iStatus = CFE_SUCCESS;

   if (MsgPtr != NULL)
   {
      usMsgLen = CFE_SB_GetTotalMsgLength(MsgPtr);

      if (usExpectedLen != usMsgLen)
      {
         iStatus = FAC_ERR_MSG_LENGTH;
         CFE_SB_MsgId_t MsgId = CFE_SB_GetMsgId(MsgPtr);
         uint16 usCmdCode = CFE_SB_GetCmdCode(MsgPtr);

         CFE_EVS_SendEvent(FAC_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                           "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                           "msgLen=%d, expectedLen=%d", MsgId, usCmdCode,
                           usMsgLen, usExpectedLen);
      }
      // Add more to check
   }
   else
   {
      iStatus = FAC_ERR_INVALID_POINTER;
      
   }

   return (iStatus);
}

void AppDataProcess::HandleAirspeed()
{
   return;
}

void AppDataProcess::HandleVehAttitude()
{
   return;
}

void AppDataProcess::HandleVehAttitudeSetPoint()
{
   return;
}

void AppDataProcess::HandleVehControlMode()
{
   return;
}

void AppDataProcess::HandleParameterUpdate()
{
   return;
}

void AppDataProcess::HandleManualControlSetpoint()
{
   return;
}

void AppDataProcess::HandleVehGlobalPosition()
{
   return;
}

void AppDataProcess::HandleVehStatus()
{
   return;
}

void AppDataProcess::HandleVehLandDetected()
{
   return;
}

void AppDataProcess::HandleBatteryStatus()
{
   return;
}

int32 AppDataProcess::RcvDataMsg(int32 iBlocking)
{
   int32          iStatus = CFE_SUCCESS;
   CFE_SB_Msg_t   *DataMsgPtr = NULL;
   CFE_SB_MsgId_t DataMsgId;

   iStatus = CFE_SB_RcvMsg(&DataMsgPtr, DataPipeId, iBlocking);
   if (iStatus == CFE_SUCCESS)
   {
      CFE_EVS_SendEvent(FAC_INF_EID, CFE_EVS_INFORMATION,
                        "RcvDataMsg:Receivd message (0x%08X)",
                        (unsigned int)iStatus);
      if (FAC_AppData.bAppAwaken == TRUE)
      {
         DataMsgId = CFE_SB_GetMsgId(DataMsgPtr);
         switch (DataMsgId)
         {
            case PX4_AIRSPEED_MID:
               HandleAirspeed();
               break;

            case PX4_VEHICLE_ATTITUDE_MID:
               HandleVehAttitude();
               break;

            case PX4_VEHICLE_ATTITUDE_SETPOINT_MID:
               HandleVehAttitudeSetPoint();
               break;

            case PX4_VEHICLE_CONTROL_MODE_MID:
               HandleVehControlMode();
               break;

            case PX4_PARAMETER_UPDATE_MID:
               HandleParameterUpdate();
               break;

            case PX4_MANUAL_CONTROL_SETPOINT_MID:
               HandleManualControlSetpoint();
               break;

            case PX4_VEHICLE_GLOBAL_POSITION_MID:
               HandleVehGlobalPosition();
               break;

            case PX4_VEHICLE_STATUS_MID:
               HandleVehStatus();
               break;

            case PX4_VEHICLE_LAND_DETECTED_MID:
               HandleVehLandDetected();
               break;

            case PX4_BATTERY_STATUS_MID:
               HandleBatteryStatus();
               break;

            default:
               CFE_EVS_SendEvent(FAC_MSGID_ERR_EID, CFE_EVS_ERROR,
                                 "Recvd invalid Data msgId (0x%04X)",
                                 (unsigned short)DataMsgId);
               goto RcvDataMsg_Exit_Tag;
         }
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
      goto RcvDataMsg_Exit_Tag;
   }

   iStatus = AcquireConfigPointers();   // Decide where to execute this
   if (iStatus != CFE_SUCCESS)
   {
      CFE_EVS_SendEvent(FAC_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
                        "AcquireConfigPointers error (0x%08X)",
                        (unsigned int)iStatus);
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

RcvDataMsg_Exit_Tag:
   return iStatus;
}

int32 AppDataProcess::Subscribe(CFE_SB_MsgId_t MsgId)
{
   int32 iStatus = CFE_SUCCESS;

   iStatus = CFE_SB_Subscribe(MsgId, DataPipeId);
   if (iStatus != CFE_SUCCESS)
   {
      CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                        "Failed to subscribe to MID:0x%04X (0x%08X)",
                        (unsigned short)MsgId, (unsigned int)iStatus);
   }

   return (iStatus);
}

int32 AppDataProcess::InitConfigTbl()
{
   int32 iStatus = CFE_SUCCESS;

   /* Register Config table */
   iStatus = CFE_TBL_Register(&FAC_AppData.ConfigTblHdl, FAC_CONFIG_TABLENAME,
                              sizeof(FAC_ConfigTbl_t), CFE_TBL_OPT_DEFAULT,
                              ValidateConfigTbl);
   if (iStatus == CFE_SUCCESS)
   {
      CFE_EVS_SendEvent(FAC_INIT_INF_EID, CFE_EVS_INFORMATION,
                        "Succeeded to register Config table (0x%08X)",
                        (unsigned int)iStatus);
   }
   else if (iStatus == CFE_TBL_INFO_RECOVERED_TBL)
   {
      CFE_EVS_SendEvent(FAC_INIT_INF_EID, CFE_EVS_INFORMATION,
                        "Succeeded to recover Config table (0x%08X)",
                        (unsigned int)iStatus);
   }
   else if (iStatus == CFE_TBL_WARN_NOT_CRITICAL)
   {
      CFE_EVS_SendEvent(FAC_INIT_INF_EID, CFE_EVS_INFORMATION,
                        "Not Critical Initialization Error of Config table (0x%08X)",
                        (unsigned int)iStatus);
   }
   else
   {
      /* Note, a critical table could return another nominal code.  If this table is
       * made critical this logic would have to change. */
      CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                        "Failed to register Config table (0x%08X)",
                        (unsigned int)iStatus);
      goto InitConfigTbl_Exit_Tag;
   }

   /* Load Config table file */
   iStatus = CFE_TBL_Load(FAC_AppData.ConfigTblHdl, CFE_TBL_SRC_FILE, FAC_CONFIG_TABLE_FILENAME);
   if (iStatus != CFE_SUCCESS)
   {
      /* Note, CFE_SUCCESS is for a successful full table load.  If a partial table
         load is desired then this logic would have to change. */
      CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                        "Failed to load Config Table (0x%08X)",
                        (unsigned int)iStatus);
      goto InitConfigTbl_Exit_Tag;
   }
   else
   {
      CFE_EVS_SendEvent(FAC_INIT_INF_EID, CFE_EVS_INFORMATION,
                        "Succeeded to load Config Table (0x%08X)",
                        (unsigned int)iStatus);
   }

InitConfigTbl_Exit_Tag:
   return (iStatus);
}

int32 AppDataProcess::InitTables()
{
   int32 iStatus = CFE_SUCCESS;

   iStatus = InitData();
   if (iStatus != CFE_SUCCESS)
   {
      CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                        "Failed to initialize Data (0x%08X)",
                        (unsigned int)iStatus);
      goto InitTables_Exit_Tag;
   }

   iStatus = InitConfigTbl();
   if (iStatus != CFE_SUCCESS)
   {
      CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                        "Failed to init config tables (0x%08X)",
                        (unsigned int)iStatus);
      goto InitTables_Exit_Tag;
   }

InitTables_Exit_Tag:
   return iStatus;
}

int32 AppDataProcess::InitData()
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

int32 AppDataProcess::InitDataPipe()
{
   int32 iStatus = CFE_SUCCESS;

   iStatus = CFE_SB_CreatePipe(&DataPipeId, FAC_DATA_PIPE_DEPTH, FAC_DATA_PIPE_NAME);
   if (iStatus == CFE_SUCCESS)
   {
      iStatus = Subscribe(PX4_AIRSPEED_MID);
      if (iStatus != CFE_SUCCESS)
      {
         goto InitDataPipe_Exit_Tag;
      }
      iStatus = Subscribe(PX4_VEHICLE_ATTITUDE_MID);
      if (iStatus != CFE_SUCCESS)
      {
         goto InitDataPipe_Exit_Tag;
      }
      iStatus = Subscribe(PX4_VEHICLE_ATTITUDE_SETPOINT_MID);
      if (iStatus != CFE_SUCCESS)
      {
         goto InitDataPipe_Exit_Tag;
      }
      iStatus = Subscribe(PX4_VEHICLE_CONTROL_MODE_MID);
      if (iStatus != CFE_SUCCESS)
      {
         goto InitDataPipe_Exit_Tag;
      }
      iStatus = Subscribe(PX4_PARAMETER_UPDATE_MID);
      if (iStatus != CFE_SUCCESS)
      {
         goto InitDataPipe_Exit_Tag;
      }
      iStatus = Subscribe(PX4_MANUAL_CONTROL_SETPOINT_MID);
      if (iStatus != CFE_SUCCESS)
      {
         goto InitDataPipe_Exit_Tag;
      }
      iStatus = Subscribe(PX4_VEHICLE_GLOBAL_POSITION_MID);
      if (iStatus != CFE_SUCCESS)
      {
         goto InitDataPipe_Exit_Tag;
      }
      iStatus = Subscribe(PX4_VEHICLE_STATUS_MID);
      if (iStatus != CFE_SUCCESS)
      {
         goto InitDataPipe_Exit_Tag;
      }
      iStatus = Subscribe(PX4_VEHICLE_LAND_DETECTED_MID);
      if (iStatus != CFE_SUCCESS)
      {
         goto InitDataPipe_Exit_Tag;
      }
      iStatus = Subscribe(PX4_BATTERY_STATUS_MID);
      if (iStatus != CFE_SUCCESS)
      {
         goto InitDataPipe_Exit_Tag;
      }
   }
   else
   {
      CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                        "Failed to create Data pipe (0x%08X)",
                        (unsigned int)iStatus);
      goto InitDataPipe_Exit_Tag;
   }

InitDataPipe_Exit_Tag:
   return iStatus;
}
