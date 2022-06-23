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
#include "fac_cmds_task.hpp"
#include "fac_version.h"


extern FAC_AppData_t FAC_MainAppData;

FAC_CmdsTaskData_t FAC_CmdsTaskData =
{
   .CmdsTaskId = 0,
   .uiCmdsRunStatus = CFE_ES_APP_RUN,
   .pCmds = NULL
};

AppCommandProcess::AppCommandProcess()
{
}

AppCommandProcess::~AppCommandProcess()
{
}

boolean AppCommandProcess::VerifyCmdLength(CFE_SB_Msg_t* MsgPtr, uint16 usExpectedLen)
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
         FAC_MainAppData.HkTlm.usCmdErrCnt++;
      }
   }

   return (bResult);
}

void AppCommandProcess::ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr)
{
   uint32  uiCmdCode=0;

   if (MsgPtr != NULL)
   {
      uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
      switch (uiCmdCode)
      {
         case FAC_NOOP_CC:
            FAC_MainAppData.HkTlm.usCmdCnt++;
            CFE_EVS_SendEvent(FAC_CMD_INF_EID, CFE_EVS_INFORMATION,
                              "Recvd NOOP cmd (%u), Version %d.%d.%d.%d",
                              (unsigned int)uiCmdCode, FAC_MAJOR_VERSION,
                              FAC_MINOR_VERSION, FAC_REVISION, FAC_MISSION_REV);
            break;

         case FAC_RESET_CC:
            FAC_MainAppData.HkTlm.usCmdCnt = 0;
            FAC_MainAppData.HkTlm.usCmdErrCnt = 0;
            CFE_EVS_SendEvent(FAC_CMD_INF_EID, CFE_EVS_INFORMATION,
                              "Recvd RESET cmd (%u)", (unsigned int)uiCmdCode);
            break;

         /* TODO:  Add code to process the rest of the FAC commands here */

         default:
            FAC_MainAppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(FAC_MSGID_ERR_EID, CFE_EVS_ERROR,
                              "Recvd invalid cmdId (%u)", (unsigned int)uiCmdCode);
            break;
      }
   }
}

int32 AppCommandProcess::RcvCmdMsg(int32 iBlocking)
{
   int32 iStatus = CFE_SUCCESS;
   CFE_SB_Msg_t*   MsgPtr=NULL;
   CFE_SB_MsgId_t  MsgId;

   /* Process command messages till the pipe is empty */
   while (FAC_CmdsTaskData.uiCmdsRunStatus == CFE_ES_APP_RUN)
   {
      iStatus = CFE_SB_RcvMsg(&MsgPtr, CmdPipeId, CFE_SB_POLL);
      if(iStatus == CFE_SUCCESS)
      {
         CFE_EVS_SendEvent(FAC_INF_EID, CFE_EVS_INFORMATION,  // check display level
                           "RcvCmdMsg:Receivd message (0x%08X)",
                           (unsigned int)iStatus);
         MsgId = CFE_SB_GetMsgId(MsgPtr);
         switch (MsgId)
         {
            case FAC_CMD_MID:
               ProcessNewAppCmds(MsgPtr);
               break;

                /* TODO:  Add code to process other subscribed commands here
                **
                ** Example:
                **     case CFE_TIME_DATA_CMD_MID:
                **         FAC_ProcessTimeDataCmd(MsgPtr);
                **         break;
                */

            default:
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with this
                     *  pipe, but not handled in this switch-case.) */
               FAC_MainAppData.HkTlm.usCmdErrCnt++;
               CFE_EVS_SendEvent(FAC_MSGID_ERR_EID, CFE_EVS_ERROR,
                                 "Recvd invalid CMD msgId (0x%04X)",
                                 (unsigned short)MsgId);
               break;
         }
      }
      else if (iStatus == CFE_SB_NO_MESSAGE)
      {
         iStatus = CFE_SUCCESS;
      }
      else
      {
         CFE_EVS_SendEvent(FAC_PIPE_ERR_EID, CFE_EVS_ERROR,
                           "CMD pipe read error (0x%08X)", (unsigned int)iStatus);
         FAC_CmdsTaskData.uiCmdsRunStatus = CFE_ES_APP_ERROR;
         break;
      }
   }

   return iStatus;
}

int32 AppCommandProcess::InitCmdsTask()
{
   int32 iStatus = CFE_SUCCESS;

   iStatus = CFE_SB_CreatePipe(&CmdPipeId, FAC_CMD_PIPE_DEPTH, FAC_CMD_PIPE_NAME);
   if (iStatus == CFE_SUCCESS)
   {
      iStatus = CFE_SB_Subscribe(FAC_CMD_MID, CmdPipeId);  // Check SubscribeEx
      if (iStatus != CFE_SUCCESS)
      {
         CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                           "Failed to subscribe to FAC_CMD_MID. (0x%08X)",
                           (unsigned int)iStatus);
         goto InitCmdsTask_Exit_Tag;
      }
      else
      {
         CFE_EVS_SendEvent(FAC_INF_EID, CFE_EVS_INFORMATION,
                           "InitCmdsTask: Successfully subscribed to FAC_CMD_MID (0x%08X)\n",
                           (unsigned int)iStatus);
      }
   }
   else
   {
      CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                        "Failed to create CMD pipe (0x%08X)",
                        (unsigned int)iStatus);
      goto InitCmdsTask_Exit_Tag;
   }

InitCmdsTask_Exit_Tag:
   return iStatus;
}

void CmdsTask()
{
   int32 iStatus = CFE_SUCCESS;
   CFE_ES_TaskInfo_t CmdsTaskInfo;

   iStatus = CFE_ES_RegisterChildTask();
   if (iStatus != CFE_SUCCESS)
   {
      CFE_ES_WriteToSysLog("FAC - Failed to register the CmdsTask (0x%08X)\n",
                           (unsigned int)iStatus);
      FAC_CmdsTaskData.uiCmdsRunStatus = CFE_ES_APP_ERROR;
      goto CmdsTask_Exit_Tag;
   }

   iStatus = CFE_ES_GetTaskInfo(&CmdsTaskInfo, FAC_CmdsTaskData.CmdsTaskId);  //check taskId and appId
   if (iStatus != CFE_SUCCESS)
   {
      CFE_ES_WriteToSysLog("FAC - Failed to Get Cmds task info(0x%08X)\n",
                           (unsigned int)iStatus);
      FAC_CmdsTaskData.uiCmdsRunStatus = CFE_ES_APP_ERROR;
      goto CmdsTask_Exit_Tag;
   }
   else
   {
      CFE_EVS_SendEvent(FAC_INF_EID, CFE_EVS_INFORMATION,
             "Cmds TaskInfo: TaskID: 0x%08X, TaskName: %s, AppId: 0x%08X, AppName:%s (0x%08X)\n",
             (unsigned int)CmdsTaskInfo.TaskId, CmdsTaskInfo.TaskName,
             (unsigned int)CmdsTaskInfo.AppId, CmdsTaskInfo.AppName, (unsigned int)iStatus);
   }

   CFE_ES_IncrementTaskCounter();

   FAC_CmdsTaskData.pCmds = new AppCommandProcess();
   if (FAC_CmdsTaskData.pCmds != NULL)
   {
      iStatus = FAC_CmdsTaskData.pCmds->InitCmdsTask();
      if (iStatus != CFE_SUCCESS)
      {
         CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                           "Failed to init cmds task (0x%08X)",
                           (unsigned int)iStatus);
         FAC_CmdsTaskData.uiCmdsRunStatus = CFE_ES_APP_ERROR;
         goto CmdsTask_Exit_Tag;
      }
      else
      {
         CFE_EVS_SendEvent(FAC_INF_EID, CFE_EVS_INFORMATION,
                           "Succeeded to init cmds task: cmds task status: 0x%08X (0x%08X)",
                           (unsigned int)FAC_CmdsTaskData.uiCmdsRunStatus,
                           (unsigned int)iStatus);
      }
   }
   else
   {
      FAC_CmdsTaskData.uiCmdsRunStatus = CFE_ES_APP_ERROR;
      goto CmdsTask_Exit_Tag;
   }

   if (FAC_CmdsTaskData.uiCmdsRunStatus == CFE_ES_APP_RUN)
   {
      iStatus = FAC_CmdsTaskData.pCmds->RcvCmdMsg(FAC_CMD_PIPE_PEND_TIME);
      if (iStatus != CFE_SUCCESS)
      {
         FAC_CmdsTaskData.uiCmdsRunStatus = CFE_ES_APP_ERROR;
         goto CmdsTask_Exit_Tag;
      }
   }

CmdsTask_Exit_Tag:
   if (FAC_CmdsTaskData.pCmds != NULL)
   {
      delete FAC_CmdsTaskData.pCmds;
   }
   CFE_ES_ExitChildTask();
   FAC_CmdsTaskData.CmdsTaskId = 0;

   FAC_MainAppData.uiAppRunStatus = CFE_ES_APP_ERROR;
   return;
}
