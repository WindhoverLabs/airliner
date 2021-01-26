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
#include <string.h>
#include "cfe.h"
#include "ea_app.h"
#include "ea_msg.h"
#include "ea_version.h"
#include <unistd.h>

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/
EA_AppData_t  EA_AppData;

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Definitions
*************************************************************************/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 EA_InitEvent()
{
    int32  iStatus = CFE_SUCCESS;
    int32  ind = 0;

    /* Initialize the event filter table.
     * Note: 0 is the CFE_EVS_NO_FILTER mask and event 0 is reserved (not used) */
    CFE_PSP_MemSet((void*)EA_AppData.EventTbl, 0x00, sizeof(EA_AppData.EventTbl));

    EA_AppData.EventTbl[  ind].EventID = EA_RESERVED_EID;
    EA_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    EA_AppData.EventTbl[  ind].EventID = EA_INF_EID;
    EA_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    EA_AppData.EventTbl[  ind].EventID = EA_INIT_INF_EID;
    EA_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    EA_AppData.EventTbl[  ind].EventID = EA_PIPE_ERR_EID;
    EA_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    EA_AppData.EventTbl[  ind].EventID = EA_MSGID_ERR_EID;
    EA_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    EA_AppData.EventTbl[  ind].EventID = EA_MSGLEN_ERR_EID;
    EA_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    EA_AppData.EventTbl[  ind].EventID = EA_CMD_INF_EID;
    EA_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EA_AppData.EventTbl[  ind].EventID = EA_INF_APP_START_EID;
    EA_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EA_AppData.EventTbl[  ind].EventID = EA_INF_APP_TERM_EID;
    EA_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EA_AppData.EventTbl[  ind].EventID = EA_APP_ERR_EID;
    EA_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EA_AppData.EventTbl[  ind].EventID = EA_WARN_APP_UTIL_EID;
    EA_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EA_AppData.EventTbl[  ind].EventID = EA_ERR_EID;
    EA_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EA_AppData.EventTbl[  ind].EventID = EA_INIT_ERR_EID;
    EA_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EA_AppData.EventTbl[  ind].EventID = EA_CMD_ERR_EID;
    EA_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EA_AppData.EventTbl[  ind].EventID = EA_CHILD_TASK_START_EID;
    EA_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EA_AppData.EventTbl[  ind].EventID = EA_CHILD_TASK_START_ERR_EID;
    EA_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    EA_AppData.EventTbl[  ind].EventID = EA_CMD_NOOP_EID;
    EA_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    
    EA_AppData.EventTbl[  ind].EventID = EA_CMD_RESET_EID;
    EA_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(EA_AppData.EventTbl,
                               EA_EVT_CNT, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("EA - Failed to register with EVS (0x%08X)\n", (unsigned int)iStatus);
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 EA_InitPipe()
{
    int32  iStatus = CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&EA_AppData.SchPipeId,
                                 EA_SCH_PIPE_DEPTH,
                                 EA_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(EA_WAKEUP_MID, EA_AppData.SchPipeId, CFE_SB_Default_Qos, EA_SCH_PIPE_WAKEUP_RESERVED);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(EA_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "Sch Pipe failed to subscribe to EA_WAKEUP_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto EA_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(EA_SEND_HK_MID, EA_AppData.SchPipeId, CFE_SB_Default_Qos, EA_SCH_PIPE_SEND_HK_RESERVED);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(EA_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "CMD Pipe failed to subscribe to EA_SEND_HK_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto EA_InitPipe_Exit_Tag;
        }

    }
    else
    {
        (void) CFE_EVS_SendEvent(EA_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create SCH pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto EA_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&EA_AppData.CmdPipeId,
                                 EA_CMD_PIPE_DEPTH,
                                 EA_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(EA_CMD_MID, EA_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(EA_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "CMD Pipe failed to subscribe to EA_CMD_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto EA_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(EA_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create CMD pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto EA_InitPipe_Exit_Tag;
    }

EA_InitPipe_Exit_Tag:
    return (iStatus);
}
    

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 EA_InitData()
{
    int32  iStatus = CFE_SUCCESS;

    /* Init housekeeping packet */
    CFE_PSP_MemSet((void*)&EA_AppData.HkTlm, 0x00, sizeof(EA_AppData.HkTlm));
    CFE_SB_InitMsg(&EA_AppData.HkTlm,
                   EA_HK_TLM_MID, sizeof(EA_AppData.HkTlm), TRUE);

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* EA initialization                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 EA_InitApp()
{
    int32  iStatus   = CFE_SUCCESS;
    int8   hasEvents = 0;

    iStatus = EA_InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("EA - Failed to init events (0x%08X)\n", (unsigned int)iStatus);
        goto EA_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = EA_InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(EA_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init pipes (0x%08X)",
                                 (unsigned int)iStatus);
        goto EA_InitApp_Exit_Tag;
    }

    iStatus = EA_InitData();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(EA_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init data (0x%08X)",
                                 (unsigned int)iStatus);
        goto EA_InitApp_Exit_Tag;
    }

EA_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(EA_INIT_INF_EID, CFE_EVS_INFORMATION,
                                 "Initialized.  Version %d.%d.%d.%d",
                                 EA_MAJOR_VERSION,
                                 EA_MINOR_VERSION,
                                 EA_REVISION,
                                 EA_MISSION_REV);
    }
    else
    {
        if (hasEvents == 1)
        {
            (void) CFE_EVS_SendEvent(EA_INIT_ERR_EID, CFE_EVS_ERROR, "Application failed to initialize");
        }
        else
        {
            (void) CFE_ES_WriteToSysLog("EA - Application failed to initialize\n");
        }
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 EA_RcvMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;
    
    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(EA_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, EA_AppData.SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(EA_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
        {
            case EA_WAKEUP_MID:
                EA_ProcessNewCmds();
                break;

            case EA_SEND_HK_MID:
                EA_ReportHousekeeping();
                break;

            default:
                (void) CFE_EVS_SendEvent(EA_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid SCH msgId (0x%04X)", (unsigned short)MsgId);
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
        /* This is an example of exiting on an error (either CFE_SB_BAD_ARGUMENT, or
         * CFE_SB_PIPE_RD_ERROR).
         */
        (void) CFE_EVS_SendEvent(EA_PIPE_ERR_EID, CFE_EVS_ERROR,
              "SB pipe read error (0x%08X), app will exit", (unsigned int)iStatus);
        EA_AppData.uiRunStatus= CFE_ES_APP_ERROR;
    }
    
    return (iStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void EA_ProcessNewCmds()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr = NULL;
    CFE_SB_MsgId_t  CmdMsgId;
    
    /* Process command messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, EA_AppData.CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case EA_CMD_MID:
                    EA_ProcessNewAppCmds(CmdMsgPtr);
                    break;

                default:
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with this
                     *  pipe, but not handled in this switch-case.) */
                    EA_AppData.HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(EA_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            (void) CFE_EVS_SendEvent(EA_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08X)", (unsigned int)iStatus);
            EA_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process EA Commands                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void EA_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;
    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case EA_NOOP_CC:
                EA_AppData.HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(EA_CMD_NOOP_EID, CFE_EVS_INFORMATION,
                                  "Recvd NOOP cmd (%u), Version %d.%d.%d.%d",
                                  (unsigned int)uiCmdCode,
                                  EA_MAJOR_VERSION,
                                  EA_MINOR_VERSION,
                                  EA_REVISION,
                                  EA_MISSION_REV);
                break;

            case EA_RESET_CC:
                EA_AppData.HkTlm.usCmdCnt = 0;
                EA_AppData.HkTlm.usCmdErrCnt = 0;
                CFE_PSP_MemSet(EA_AppData.HkTlm.ActiveApp, '\0', EA_MAX_PATH_LEN);
                EA_AppData.HkTlm.ActiveAppUtil = 0;
                EA_AppData.HkTlm.ActiveAppPID = 0;
                CFE_PSP_MemSet(EA_AppData.HkTlm.LastAppRun, '\0', EA_MAX_PATH_LEN);
                EA_AppData.HkTlm.LastAppStatus = 0;
                EA_AppData.ProcData.p_time = 0;
                EA_AppData.ProcData.total_time = 0;
                EA_AppData.ChildAppTaskInUse = FALSE;

                (void) CFE_EVS_SendEvent(EA_CMD_RESET_EID, CFE_EVS_INFORMATION,
                                  "Recvd RESET cmd (%u)", (unsigned int)uiCmdCode);
                break;

            case EA_START_APP_CC:
                (void) CFE_EVS_SendEvent(EA_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "Recvd Start App cmd (%u)", (unsigned int)uiCmdCode);
                EA_StartApp(MsgPtr);
                break;

            case EA_TERM_APP_CC:
                (void) CFE_EVS_SendEvent(EA_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "Recvd Terminate App cmd (%u)", (unsigned int)uiCmdCode);
                EA_TermApp(MsgPtr);
                break;

            case EA_PERFMON_CC:
                (void) CFE_EVS_SendEvent(EA_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "Recvd Perfmon cmd (%u)", (unsigned int)uiCmdCode);
                EA_Perfmon();
                break;

            default:
                EA_AppData.HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(EA_CC_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid cmd code (%u)", (unsigned int)uiCmdCode);
                break;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* EA Start App                                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 EA_StartApp(CFE_SB_Msg_t* MsgPtr)
{
    uint16              ExpectedLength = sizeof(EA_StartCmd_t);
    uint32              ChildAppTaskID = 0;
    int32               Status = CFE_ES_APP_ERROR;
    EA_StartCmd_t       *CmdPtr = 0;
    os_fstat_t          FileStats;

    /* Verify command packet length... */
    if (EA_VerifyCmdLength(MsgPtr, ExpectedLength))
    {
        if (EA_AppData.ChildAppTaskInUse == FALSE)
        {
            CmdPtr = ((EA_StartCmd_t *) MsgPtr);

            /*
            ** NUL terminate the very end of the filename string as a
            ** safety measure
            */
            CmdPtr->Cmd[EA_MAX_PATH_LEN - 1] = '\0';
            CmdPtr->Args[EA_MAX_PATH_LEN - 1] = '\0';

            /*
            ** Check if specified interpreter exists
            */
            if(access(CmdPtr->Cmd, F_OK) != -1)
            {
                /*
                ** Update ChildData with validated data
                */
                (void) strncpy(EA_AppData.ChildData.Cmd, CmdPtr->Cmd, EA_MAX_PATH_LEN);
                (void) strncpy(EA_AppData.ChildData.Args, CmdPtr->Args, EA_MAX_PATH_LEN);

                Status = CFE_ES_CreateChildTask(&ChildAppTaskID,
                                                EA_START_APP_TASK_NAME,
                                                EA_StartAppCustom,
                                                NULL,
                                                CFE_ES_DEFAULT_STACK_SIZE,
                                                EA_CHILD_TASK_PRIORITY,
                                                EA_CHILD_TASK_FLAGS);
                if (Status == CFE_SUCCESS)
                {
                    (void) CFE_EVS_SendEvent (EA_CHILD_TASK_START_EID, CFE_EVS_DEBUG, "Created child task for app start");
                    EA_AppData.ChildAppTaskInUse = TRUE;
                    EA_AppData.ChildAppTaskID = ChildAppTaskID;
                }
                else /* Child task creation failed */
                {
                    (void) CFE_EVS_SendEvent (EA_CHILD_TASK_START_ERR_EID,
                                              CFE_EVS_ERROR,
                                              "Create child tasked failed (%lu). Unable to start external application",
                                              Status);

                    EA_AppData.HkTlm.usCmdErrCnt++;
                    EA_AppData.ChildAppTaskInUse   = FALSE;
                    CFE_PSP_MemSet(EA_AppData.ChildData.Cmd, '\0', EA_MAX_PATH_LEN);
                    CFE_PSP_MemSet(EA_AppData.ChildData.Args, '\0', EA_MAX_PATH_LEN);
                }
            }
            else
            {
                EA_AppData.HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(EA_APP_ERR_EID, CFE_EVS_ERROR,
                                         "Specified app does not exist");
            }
        }
        else
        {
            /* Send event that we can't start another task right now */
            (void) CFE_EVS_SendEvent (EA_CHILD_TASK_START_ERR_EID,
                                      CFE_EVS_ERROR,
                                      "Create child tasked failed. A child task is in use");

            EA_AppData.HkTlm.usCmdErrCnt++;
        }
    }
    return (Status);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* EA Terminate App                                                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void EA_TermApp(CFE_SB_Msg_t* MsgPtr)
{
    uint16 ExpectedLength = sizeof(EA_NoArgCmd_t);

    /*
    ** Verify command packet length
    */
    if(EA_VerifyCmdLength(MsgPtr, ExpectedLength))
    {
        EA_TermAppCustom();
    }

    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* EA Performance Monitor                                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void EA_Perfmon()
{
    /*
    ** Only run when ActiveAppPID is set to nonzero value.
    */
    if(EA_AppData.HkTlm.ActiveAppPID != 0)
    {
        EA_PerfmonCustom(EA_AppData.HkTlm.ActiveAppPID);
        if(EA_AppData.HkTlm.ActiveAppUtil > EA_APP_UTIL_THRESHOLD)
        {
            (void) CFE_EVS_SendEvent(EA_WARN_APP_UTIL_EID, CFE_EVS_INFORMATION,
                                    "External application exceeded utilization threshold");
        }
    }

    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send EA Housekeeping                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void EA_ReportHousekeeping()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&EA_AppData.HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&EA_AppData.HkTlm);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Command Length                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean EA_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
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

            (void) CFE_EVS_SendEvent(EA_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            EA_AppData.HkTlm.usCmdErrCnt++;
        }
    }

    return (bResult);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* EA application entry point and main process loop               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void EA_AppMain()
{
    /* Register the application with Executive Services */
    EA_AppData.uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("EA - Failed to register the app (0x%08X)\n", (unsigned int)iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(EA_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = EA_InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(EA_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(EA_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(EA_MAIN_TASK_PERF_ID);
    }
    else
    {
        EA_AppData.uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&EA_AppData.uiRunStatus) == TRUE)
    {
        EA_RcvMsg(EA_SCH_PIPE_PEND_TIME);
        
        /* This could be scheduled in the future. It won't actually do perf
         * monitoring unless the child task is currently executing */
        EA_Perfmon();
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(EA_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(EA_AppData.uiRunStatus);
}

/************************/
/*  End of File Comment */
/************************/
