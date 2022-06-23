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
#include <cfe_es.h>
#include <cfe_error.h>

#include "fac_fac.hpp"
#include "fac_app.hpp"
#include "fac_cds_task.hpp"
#include "fac_cmds_task.hpp"
#include "fac_msg.h"
#include "fac_version.h"

#include <string.h>


FAC_AppData_t FAC_MainAppData =
{
   .MainAppId = 0,
   .uiAppRunStatus = CFE_ES_APP_RUN,
   .bCdsTaskCreated = FALSE,
   .bCmdsTaskCreated = FALSE,
   .bAppAwaken = FALSE
};

FixedwingAttitudeControl *pFPC = NULL;


extern FAC_CdsTaskData_t  FAC_CdsTaskData;
extern FAC_CmdsTaskData_t FAC_CmdsTaskData;


FixedwingAttitudeControl::FixedwingAttitudeControl()
{
}

FixedwingAttitudeControl::~FixedwingAttitudeControl()
{
}

void FixedwingAttitudeControl::SendOutData()
{
#if 0  // check this
   /* TODO:  Add code to update output data, if needed, here.  */

   CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&FAC_MainAppData.OutData);
   int32 iStatus = CFE_SB_SendMsg((CFE_SB_Msg_t*)&FAC_MainAppData.OutData);
   if (iStatus != CFE_SUCCESS)
   {
      /* TODO: Decide what to do if the send message fails. */
   }
#endif
}

void FixedwingAttitudeControl::ReportHousekeeping()
{
   int32 iStatus = CFE_SUCCESS;

   /* TODO:  Add code to update housekeeping data, if needed, here.  */

   CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&FAC_MainAppData.HkTlm);
   iStatus = CFE_SB_SendMsg((CFE_SB_Msg_t*)&FAC_MainAppData.HkTlm);
   if (iStatus != CFE_SUCCESS)
   {
      /* TODO: Decide what to do if the send message fails. */
   }

   return;
}

int32 FixedwingAttitudeControl::RcvSchMsg(int32 iBlocking)
{
   int32           iStatus = CFE_SUCCESS;
   CFE_SB_Msg_t*   MsgPtr = NULL;
   CFE_SB_MsgId_t  MsgId;

   /* Stop Performance Log entry */
   CFE_ES_PerfLogExit(FAC_MAIN_TASK_PERF_ID);

   /* Wait for WakeUp messages from scheduler */
   iStatus = CFE_SB_RcvMsg(&MsgPtr, SchPipeId, iBlocking);

   /* Start Performance Log entry */
   CFE_ES_PerfLogEntry(FAC_MAIN_TASK_PERF_ID);   // check when iStatus was failed

   if (iStatus == CFE_SUCCESS)
   {
      MsgId = CFE_SB_GetMsgId(MsgPtr);
      switch (MsgId)
      {
         case FAC_WAKEUP_MID:
            CFE_EVS_SendEvent(FAC_INF_EID, CFE_EVS_INFORMATION,
                              "Recvd FAC_WAKEUP_MID msgId (0x%04X)",
                              (unsigned short)MsgId);

            /* TODO:  Add more code here to handle other things when app wakes up */
            if (FAC_MainAppData.bCdsTaskCreated == FALSE)
            {
               iStatus = CFE_ES_CreateChildTask(&FAC_CdsTaskData.CdsTaskId,
                                                FAC_CDS_TASK_NAME, CdsTask,
                                                NULL, CFE_ES_DEFAULT_STACK_SIZE,
                                                FAC_CDS_TASK_PRIORITY, OS_FP_ENABLED);
               if(iStatus != CFE_SUCCESS)
               {
                  CFE_EVS_SendEvent(FAC_ERR_EID, CFE_EVS_ERROR,
                                    "Failed to create CDS Task (0x%08X)",
                                    (unsigned int)iStatus);
                  FAC_CdsTaskData.CdsTaskId = 0;
                  FAC_MainAppData.uiAppRunStatus = CFE_ES_APP_ERROR;
               }
               FAC_MainAppData.bCdsTaskCreated = TRUE;
            }

            if (FAC_MainAppData.bCmdsTaskCreated == FALSE)
            {
               iStatus = CFE_ES_CreateChildTask(&FAC_CmdsTaskData.CmdsTaskId,
                                                FAC_CMDS_TASK_NAME, CmdsTask,
                                                NULL, CFE_ES_DEFAULT_STACK_SIZE,
                                                FAC_CMDS_TASK_PRIORITY, OS_FP_ENABLED);
               if(iStatus != CFE_SUCCESS)
               {
                  CFE_EVS_SendEvent(FAC_ERR_EID, CFE_EVS_ERROR,
                                    "Failed to create CMDS Task (0x%08X)",
                                    (unsigned int)iStatus);
                  FAC_CmdsTaskData.CmdsTaskId = 0;
                  FAC_CmdsTaskData.uiCmdsRunStatus = CFE_ES_APP_ERROR;
               }
               FAC_MainAppData.bCmdsTaskCreated = TRUE;
            }

            FAC_MainAppData.bAppAwaken = TRUE;

            /* The last thing to do at the end of this Wakeup cycle should be to
             * automatically publish new output. */
            SendOutData();
            break;

         case FAC_SEND_HK_MID:
            CFE_EVS_SendEvent(FAC_INF_EID, CFE_EVS_INFORMATION,
                              "Recvd FAC_SEND_HK_MID msgId (0x%04X)",
                              (unsigned short)MsgId);
            if (FAC_MainAppData.bAppAwaken == TRUE)
            {
               ReportHousekeeping();
            }
            break;

         default:
            CFE_EVS_SendEvent(FAC_MSGID_ERR_EID, CFE_EVS_ERROR,
                              "Recvd invalid SCH msgId (0x%04X)",
                              (unsigned short)MsgId);
            break;
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
      /* TODO: This is an example of exiting on an error (either CFE_SB_BAD_ARGUMENT,
       *  or CFE_SB_PIPE_RD_ERROR).
       */
      CFE_EVS_SendEvent(FAC_PIPE_ERR_EID, CFE_EVS_ERROR,
			"SB pipe read error (0x%08X), app will exit",
                        (unsigned int)iStatus);
      FAC_MainAppData.uiAppRunStatus= CFE_ES_APP_ERROR;
   }

   return (iStatus);
}

int32 FixedwingAttitudeControl::InitHk()
{
   int32 iStatus = CFE_SUCCESS;

   /* Init housekeeping packet */
//   memset((void*)&FAC_MainAppData.HkTlm, 0x00, sizeof(FAC_MainAppData.HkTlm)); // check this
   CFE_SB_InitMsg(&FAC_MainAppData.HkTlm, FAC_HK_TLM_MID,
                  sizeof(FAC_MainAppData.HkTlm), TRUE);

   return (iStatus);
}

int32 FixedwingAttitudeControl::InitPipe()
{
   int32  iStatus = CFE_SUCCESS;

   /* Init schedule pipe and subscribe to wakeup messages */
   iStatus = CFE_SB_CreatePipe(&SchPipeId, FAC_SCH_PIPE_DEPTH, FAC_SCH_PIPE_NAME);
   if (iStatus == CFE_SUCCESS)
   {
      iStatus = CFE_SB_SubscribeEx(FAC_WAKEUP_MID, SchPipeId, CFE_SB_Default_Qos,
                                   FAC_SCH_PIPE_WAKEUP_RESERVED);
      if (iStatus != CFE_SUCCESS)
      {
         CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                           "Sch Pipe failed to subscribe to FAC_WAKEUP_MID. (0x%08X)",
                           (unsigned int)iStatus);
         goto InitPipe_Exit_Tag;
      }

      iStatus = CFE_SB_SubscribeEx(FAC_SEND_HK_MID, SchPipeId, CFE_SB_Default_Qos,
                                   FAC_SCH_PIPE_SEND_HK_RESERVED);
      if (iStatus != CFE_SUCCESS)
      {
         CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                           "CMD Pipe failed to subscribe to FAC_SEND_HK_MID. (0x%08X)",
                           (unsigned int)iStatus);
         goto InitPipe_Exit_Tag;
      }
   }
   else
   {
      CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                        "Failed to create SCH pipe (0x%08X)",
                        (unsigned int)iStatus);
      goto InitPipe_Exit_Tag;
   }

InitPipe_Exit_Tag:
   return (iStatus);
}

int32 FixedwingAttitudeControl::InitEvent()
{
   int32  iStatus = CFE_SUCCESS;
   int32  ind = 0;

   /* Initialize the event filter table.
    * Note: 0 is the CFE_EVS_NO_FILTER mask and event 0 is reserved (not used) */
   memset((void*)EventTbl, 0x00, sizeof(EventTbl));   // check this

   /* TODO: Choose the events you want to filter.  CFE_EVS_MAX_EVENT_FILTERS
    * limits the number of filters per app.  An explicit CFE_EVS_NO_FILTER
    * (the default) has been provided as an example. */
   EventTbl[  ind].EventID = FAC_RESERVED_EID;
   EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

   EventTbl[  ind].EventID = FAC_INF_EID;
   EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

   EventTbl[  ind].EventID = FAC_CONFIG_TABLE_ERR_EID;
   EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

   EventTbl[  ind].EventID = FAC_CDS_ERR_EID;
   EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

   EventTbl[  ind].EventID = FAC_PIPE_ERR_EID;
   EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

   EventTbl[  ind].EventID = FAC_MSGID_ERR_EID;
   EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

   EventTbl[  ind].EventID = FAC_MSGLEN_ERR_EID;
   EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

   /* Register the table with CFE */
   iStatus = CFE_EVS_Register(EventTbl, FAC_EVT_CNT, CFE_EVS_BINARY_FILTER);
   if (iStatus != CFE_SUCCESS)
   {
      CFE_ES_WriteToSysLog("FAC - Failed to register with EVS (0x%08X)\n",
                           (unsigned int)iStatus);
   }

   return (iStatus);
}

/* Startup steps to initialize (or restore from CDS) FAC data structures */
int32 FixedwingAttitudeControl::InitApp()
{
   int32  iStatus   = CFE_SUCCESS;
   int8   hasEvents = 0;

   iStatus = InitEvent();
   if (iStatus != CFE_SUCCESS)
   {
      CFE_ES_WriteToSysLog("FAC - Failed to init events (0x%08X)\n",
                           (unsigned int)iStatus);
      goto InitApp_Exit_Tag;
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
      goto InitApp_Exit_Tag;
   }

   iStatus = InitHk();
   if (iStatus != CFE_SUCCESS)
   {
      CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                        "Failed to init Hk (0x%08X)",
                        (unsigned int)iStatus);
      goto InitApp_Exit_Tag;
   }

#if 0
   iStatus = OS_TaskInstallDeleteHandler(&FAC_CleanupCallback);  // check this
   if (iStatus != CFE_SUCCESS)
   {
      CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                        "Failed to init register cleanup callback (0x%08X)",
                        (unsigned int)iStatus);
      goto InitApp_Exit_Tag;
   }
#endif

InitApp_Exit_Tag:
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
         CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                           "Application failed to initialize");
      }
      else
      {
         CFE_ES_WriteToSysLog("FAC - Application failed to initialize\n");
      }
   }

   return (iStatus);
}

extern "C" void FAC_AppMain()
{
   int32 iStatus = CFE_SUCCESS;

   /* Register the application with Executive Services */
   iStatus = CFE_ES_RegisterApp();
   if (iStatus != CFE_SUCCESS)
   {
      CFE_ES_WriteToSysLog("FAC - Failed to register the app (0x%08X)\n",
                           (unsigned int)iStatus);
   }

   /* Start Performance Log entry */
   CFE_ES_PerfLogEntry(FAC_MAIN_TASK_PERF_ID);

   /* Perform application initializations */
   if (iStatus == CFE_SUCCESS)
   {
      pFPC = new FixedwingAttitudeControl();

      if (pFPC != NULL)
      {
         iStatus = pFPC->InitApp();    //  Constructors
         if (iStatus == CFE_SUCCESS)
         {
            /* Do not perform performance monitoring on startup sync */
            CFE_ES_PerfLogExit(FAC_MAIN_TASK_PERF_ID);
            CFE_ES_WaitForStartupSync(FAC_STARTUP_TIMEOUT_MSEC);
            CFE_ES_PerfLogEntry(FAC_MAIN_TASK_PERF_ID);
         }
         else
         {
            FAC_MainAppData.uiAppRunStatus = CFE_ES_APP_ERROR;
         }
      }
      else
      {
         FAC_MainAppData.uiAppRunStatus = CFE_ES_APP_ERROR;
      }
   }

   /* Application main loop */
   while (CFE_ES_RunLoop(&FAC_MainAppData.uiAppRunStatus) == TRUE)
   {
      iStatus = pFPC->RcvSchMsg(FAC_SCH_PIPE_PEND_TIME);
      if (iStatus != CFE_SUCCESS)
      {
         /* TODO: Decide what to do for other return values in FAC_RcvMsg(). */
         goto AppMain_Exit_Tag;
      }


      /* TODO: This is only a suggestion for when to update and save CDS table.
      ** Depends on the nature of the application, the frequency of update
      ** and save can be more or less independently.
      */
#if 0  // check this
      FAC_UpdateCdsTbl();
      FAC_SaveCdsTbl();
#endif
   }

AppMain_Exit_Tag:

   // Destructors
   if (FAC_MainAppData.bCdsTaskCreated == TRUE)
   {
      if (FAC_CdsTaskData.pCds != NULL)
      {
         delete FAC_CdsTaskData.pCds;
      }
      CFE_ES_DeleteChildTask(FAC_CdsTaskData.CdsTaskId);  // check this
      FAC_MainAppData.bCdsTaskCreated = FALSE;
   }

   if (FAC_MainAppData.bCmdsTaskCreated == TRUE)
   {
      if (FAC_CmdsTaskData.pCmds != NULL)
      {
         delete FAC_CmdsTaskData.pCmds;
      }
      CFE_ES_DeleteChildTask(FAC_CmdsTaskData.CmdsTaskId);  // check this
      FAC_MainAppData.bCmdsTaskCreated = FALSE;
   }

   if (pFPC != NULL)
   {
      delete pFPC;
   }

   FAC_MainAppData.bAppAwaken = FALSE;

   /* Stop Performance Log entry */
   CFE_ES_PerfLogExit(FAC_MAIN_TASK_PERF_ID);

   /* Exit the application */
   CFE_ES_ExitApp(FAC_MainAppData.uiAppRunStatus);
}
