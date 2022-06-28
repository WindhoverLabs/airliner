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
#include "fac_cmds_utils.hpp"
#include "fac_version.h"


extern FAC_AppData_t  FAC_AppData;


AppCommandProcess::AppCommandProcess()
{
}

AppCommandProcess::~AppCommandProcess()
{
}

int32 AppCommandProcess::VerifyCmdLength(CFE_SB_Msg_t *MsgPtr, uint16 usExpectedLen)
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

int32 AppCommandProcess::ProcessNewAppCmds(CFE_SB_Msg_t *MsgPtr)
{
   int32   iStatus = CFE_SUCCESS;
   uint32  uiCmdCode = 0;

   if (MsgPtr != NULL)
   {
      uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
      switch (uiCmdCode)
      {
         case FAC_NOOP_CC:
            iStatus = VerifyCmdLength(MsgPtr, sizeof(FAC_NoArgCmd_t));
            if (iStatus == CFE_SUCCESS)
            {
               CFE_EVS_SendEvent(FAC_CMD_INF_EID, CFE_EVS_INFORMATION,
                                 "Recvd NOOP cmd (%u), Version %d.%d.%d.%d",
                                 (unsigned int)uiCmdCode, FAC_MAJOR_VERSION,
                                 FAC_MINOR_VERSION, FAC_REVISION, FAC_MISSION_REV);
               FAC_AppData.HkTlm.usCmdCnt++;
            }
            else if (iStatus == FAC_ERR_INVALID_POINTER)
            {
               CFE_EVS_SendEvent(FAC_ERR_EID, CFE_EVS_ERROR,
                                 "Invalid Cmd Msg Pointer (0x%08X)",
                                 (unsigned int)iStatus);
               FAC_AppData.HkTlm.usCmdErrCnt++;
            }
            else
            {
               FAC_AppData.HkTlm.usCmdErrCnt++;
            }
            break;

         case FAC_RESET_CC:
            iStatus = VerifyCmdLength(MsgPtr, sizeof(FAC_NoArgCmd_t));
            if (iStatus == CFE_SUCCESS)
            {
               FAC_AppData.HkTlm.usCmdCnt = 0;
               FAC_AppData.HkTlm.usCmdErrCnt = 0;
               CFE_EVS_SendEvent(FAC_CMD_INF_EID, CFE_EVS_INFORMATION,
                                 "Recvd RESET cmd (%u)", (unsigned int)uiCmdCode);
            }
            else if (iStatus == FAC_ERR_INVALID_POINTER)
            {
               CFE_EVS_SendEvent(FAC_ERR_EID, CFE_EVS_ERROR,
                                 "Invalid Cmd Msg Pointer (0x%08X)",
                                 (unsigned int)iStatus);
               FAC_AppData.HkTlm.usCmdErrCnt++;
            }
            else
            {
               FAC_AppData.HkTlm.usCmdErrCnt++;
            }
            break;

         /* TODO:  Add code to process the rest of the FAC commands here */

         default:
            FAC_AppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(FAC_MSGID_ERR_EID, CFE_EVS_ERROR,
                              "Recvd invalid cmdId (%u)", (unsigned int)uiCmdCode);
            break;
      }
   }
   else
   {
      iStatus = FAC_ERR_INVALID_POINTER;      // Maybe CFE_OS_INVALID_POINTER
   }

   return (iStatus);
}

int32 AppCommandProcess::RcvCmdMsg(int32 iBlocking)
{
   int32 iStatus = CFE_SUCCESS;
   CFE_SB_Msg_t*   MsgPtr = NULL;
   CFE_SB_MsgId_t  MsgId;

   /* Process command messages till the pipe is empty */
   iStatus = CFE_SB_RcvMsg(&MsgPtr, CmdPipeId, iBlocking);
   if(iStatus == CFE_SUCCESS)
   {
      CFE_EVS_SendEvent(FAC_INF_EID, CFE_EVS_INFORMATION,  // check display level
                        "RcvCmdMsg:Receivd message (0x%08X)",
                        (unsigned int)iStatus);
      if (FAC_AppData.bAppAwaken == TRUE)
      {
         MsgId = CFE_SB_GetMsgId(MsgPtr);
         switch (MsgId)
         {
            case FAC_CMD_MID:
               iStatus = ProcessNewAppCmds(MsgPtr);
               if (iStatus != CFE_SUCCESS)
               {
                  goto RcvCmdMsg_Exit_Tag;
               }
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
               FAC_AppData.HkTlm.usCmdErrCnt++;
               CFE_EVS_SendEvent(FAC_MSGID_ERR_EID, CFE_EVS_ERROR,
                                 "Recvd invalid CMD msgId (0x%04X)",
                                 (unsigned short)MsgId);
               break;
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
                        "CMD pipe read error (0x%08X)", (unsigned int)iStatus);
      goto RcvCmdMsg_Exit_Tag;
   }

RcvCmdMsg_Exit_Tag:
   return iStatus;
}

int32 AppCommandProcess::InitCmdsPipe()
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
         goto InitCmdsPipe_Exit_Tag;
      }
      else
      {
         CFE_EVS_SendEvent(FAC_INF_EID, CFE_EVS_INFORMATION,
                           "InitCmdsPipe: Successfully subscribed to FAC_CMD_MID (0x%08X)\n",
                           (unsigned int)iStatus);
      }
   }
   else
   {
      CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                        "Failed to create CMD pipe (0x%08X)",
                        (unsigned int)iStatus);
      goto InitCmdsPipe_Exit_Tag;
   }

InitCmdsPipe_Exit_Tag:
   return iStatus;
}
