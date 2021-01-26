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

#include "{{cookiecutter.app_name|lower}}_app.h"
#include "{{cookiecutter.app_name|lower}}_msg.h"
#include "{{cookiecutter.app_name|lower}}_version.h"

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
{{cookiecutter.app_name}}_AppData_t  {{cookiecutter.app_name}}_AppData;

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

int32 {{cookiecutter.app_name}}_InitEvent()
{
    int32  iStatus=CFE_SUCCESS;
    int32  ind = 0;

    /* Initialize the event filter table.
     * Note: 0 is the CFE_EVS_NO_FILTER mask and event 0 is reserved (not used) */
    memset((void*){{cookiecutter.app_name}}_AppData.EventTbl, 0x00, sizeof({{cookiecutter.app_name}}_AppData.EventTbl));

    /* TODO: Choose the events you want to filter.  CFE_EVS_MAX_EVENT_FILTERS
     * limits the number of filters per app.  An explicit CFE_EVS_NO_FILTER 
     * (the default) has been provided as an example. */
    {{cookiecutter.app_name}}_AppData.EventTbl[  ind].EventID = {{cookiecutter.app_name}}_RESERVED_EID;
    {{cookiecutter.app_name}}_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    {{cookiecutter.app_name}}_AppData.EventTbl[  ind].EventID = {{cookiecutter.app_name}}_INF_EID;
    {{cookiecutter.app_name}}_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    {{cookiecutter.app_name}}_AppData.EventTbl[  ind].EventID = {{cookiecutter.app_name}}_CONFIG_TABLE_ERR_EID;
    {{cookiecutter.app_name}}_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    {{cookiecutter.app_name}}_AppData.EventTbl[  ind].EventID = {{cookiecutter.app_name}}_CDS_ERR_EID;
    {{cookiecutter.app_name}}_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    {{cookiecutter.app_name}}_AppData.EventTbl[  ind].EventID = {{cookiecutter.app_name}}_PIPE_ERR_EID;
    {{cookiecutter.app_name}}_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    {{cookiecutter.app_name}}_AppData.EventTbl[  ind].EventID = {{cookiecutter.app_name}}_MSGID_ERR_EID;
    {{cookiecutter.app_name}}_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    {{cookiecutter.app_name}}_AppData.EventTbl[  ind].EventID = {{cookiecutter.app_name}}_MSGLEN_ERR_EID;
    {{cookiecutter.app_name}}_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register({{cookiecutter.app_name}}_AppData.EventTbl,
                               {{cookiecutter.app_name}}_EVT_CNT, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("{{cookiecutter.app_name}} - Failed to register with EVS (0x%08X)\n", (unsigned int)iStatus);
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 {{cookiecutter.app_name}}_InitPipe()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&{{cookiecutter.app_name}}_AppData.SchPipeId,
                                 {{cookiecutter.app_name}}_SCH_PIPE_DEPTH,
                                 {{cookiecutter.app_name}}_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx({{cookiecutter.app_name}}_WAKEUP_MID, {{cookiecutter.app_name}}_AppData.SchPipeId, CFE_SB_Default_Qos, {{cookiecutter.app_name}}_SCH_PIPE_WAKEUP_RESERVED);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent({{cookiecutter.app_name}}_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "Sch Pipe failed to subscribe to {{cookiecutter.app_name}}_WAKEUP_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto {{cookiecutter.app_name}}_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx({{cookiecutter.app_name}}_SEND_HK_MID, {{cookiecutter.app_name}}_AppData.SchPipeId, CFE_SB_Default_Qos, {{cookiecutter.app_name}}_SCH_PIPE_SEND_HK_RESERVED);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent({{cookiecutter.app_name}}_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "CMD Pipe failed to subscribe to {{cookiecutter.app_name}}_SEND_HK_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto {{cookiecutter.app_name}}_InitPipe_Exit_Tag;
        }

    }
    else
    {
        (void) CFE_EVS_SendEvent({{cookiecutter.app_name}}_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create SCH pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto {{cookiecutter.app_name}}_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&{{cookiecutter.app_name}}_AppData.CmdPipeId,
                                 {{cookiecutter.app_name}}_CMD_PIPE_DEPTH,
                                 {{cookiecutter.app_name}}_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe({{cookiecutter.app_name}}_CMD_MID, {{cookiecutter.app_name}}_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent({{cookiecutter.app_name}}_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "CMD Pipe failed to subscribe to {{cookiecutter.app_name}}_CMD_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto {{cookiecutter.app_name}}_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent({{cookiecutter.app_name}}_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create CMD pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto {{cookiecutter.app_name}}_InitPipe_Exit_Tag;
    }

    /* Init data pipe and subscribe to messages on the data pipe */
    iStatus = CFE_SB_CreatePipe(&{{cookiecutter.app_name}}_AppData.DataPipeId,
                                 {{cookiecutter.app_name}}_DATA_PIPE_DEPTH,
                                 {{cookiecutter.app_name}}_DATA_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* TODO:  Add CFE_SB_Subscribe() calls for other apps' output data here.
        **
        ** Examples:
        **     CFE_SB_Subscribe(GNCEXEC_OUT_DATA_MID, {{cookiecutter.app_name}}_AppData.DataPipeId);
        */
    }
    else
    {
        (void) CFE_EVS_SendEvent({{cookiecutter.app_name}}_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create Data pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto {{cookiecutter.app_name}}_InitPipe_Exit_Tag;
    }

{{cookiecutter.app_name}}_InitPipe_Exit_Tag:
    return (iStatus);
}
    

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 {{cookiecutter.app_name}}_InitData()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init input data */
    memset((void*)&{{cookiecutter.app_name}}_AppData.InData, 0x00, sizeof({{cookiecutter.app_name}}_AppData.InData));

    /* Init output data */
    memset((void*)&{{cookiecutter.app_name}}_AppData.OutData, 0x00, sizeof({{cookiecutter.app_name}}_AppData.OutData));
    CFE_SB_InitMsg(&{{cookiecutter.app_name}}_AppData.OutData,
                   {{cookiecutter.app_name}}_OUT_DATA_MID, sizeof({{cookiecutter.app_name}}_AppData.OutData), TRUE);

    /* Init housekeeping packet */
    memset((void*)&{{cookiecutter.app_name}}_AppData.HkTlm, 0x00, sizeof({{cookiecutter.app_name}}_AppData.HkTlm));
    CFE_SB_InitMsg(&{{cookiecutter.app_name}}_AppData.HkTlm,
                   {{cookiecutter.app_name}}_HK_TLM_MID, sizeof({{cookiecutter.app_name}}_AppData.HkTlm), TRUE);

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* {{cookiecutter.app_name}} initialization                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 {{cookiecutter.app_name}}_InitApp()
{
    int32  iStatus   = CFE_SUCCESS;
    int8   hasEvents = 0;

    iStatus = {{cookiecutter.app_name}}_InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("{{cookiecutter.app_name}} - Failed to init events (0x%08X)\n", (unsigned int)iStatus);
        goto {{cookiecutter.app_name}}_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = {{cookiecutter.app_name}}_InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent({{cookiecutter.app_name}}_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init pipes (0x%08X)",
                                 (unsigned int)iStatus);
        goto {{cookiecutter.app_name}}_InitApp_Exit_Tag;
    }

    iStatus = {{cookiecutter.app_name}}_InitData();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent({{cookiecutter.app_name}}_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init data (0x%08X)",
                                 (unsigned int)iStatus);
        goto {{cookiecutter.app_name}}_InitApp_Exit_Tag;
    }

    iStatus = {{cookiecutter.app_name}}_InitConfigTbl();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent({{cookiecutter.app_name}}_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init config tables (0x%08X)",
                                 (unsigned int)iStatus);
        goto {{cookiecutter.app_name}}_InitApp_Exit_Tag;
    }

    iStatus = {{cookiecutter.app_name}}_InitCdsTbl();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent({{cookiecutter.app_name}}_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init CDS table (0x%08X)",
                                 (unsigned int)iStatus);
        goto {{cookiecutter.app_name}}_InitApp_Exit_Tag;
    }

{{cookiecutter.app_name}}_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent({{cookiecutter.app_name}}_INIT_INF_EID, CFE_EVS_INFORMATION,
                                 "Initialized.  Version %d.%d.%d.%d",
                                 {{cookiecutter.app_name}}_MAJOR_VERSION,
                                 {{cookiecutter.app_name}}_MINOR_VERSION,
                                 {{cookiecutter.app_name}}_REVISION,
                                 {{cookiecutter.app_name}}_MISSION_REV);
    }
    else
    {
        if (hasEvents == 1)
        {
            (void) CFE_EVS_SendEvent({{cookiecutter.app_name}}_INIT_ERR_EID, CFE_EVS_ERROR, "Application failed to initialize");
        }
        else
        {
            (void) CFE_ES_WriteToSysLog("{{cookiecutter.app_name}} - Application failed to initialize\n");
        }
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 {{cookiecutter.app_name}}_RcvMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit({{cookiecutter.app_name}}_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, {{cookiecutter.app_name}}_AppData.SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry({{cookiecutter.app_name}}_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
	{
            case {{cookiecutter.app_name}}_WAKEUP_MID:
                {{cookiecutter.app_name}}_ProcessNewCmds();
                {{cookiecutter.app_name}}_ProcessNewData();

                /* TODO:  Add more code here to handle other things when app wakes up */

                /* The last thing to do at the end of this Wakeup cycle should be to
                 * automatically publish new output. */
                {{cookiecutter.app_name}}_SendOutData();
                break;

            case {{cookiecutter.app_name}}_SEND_HK_MID:
                {{cookiecutter.app_name}}_ReportHousekeeping();
                break;

            default:
                (void) CFE_EVS_SendEvent({{cookiecutter.app_name}}_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid SCH msgId (0x%04X)", (unsigned short)MsgId);
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
        /* TODO: This is an example of exiting on an error (either CFE_SB_BAD_ARGUMENT, or
         * CFE_SB_PIPE_RD_ERROR).
         */
        (void) CFE_EVS_SendEvent({{cookiecutter.app_name}}_PIPE_ERR_EID, CFE_EVS_ERROR,
			  "SB pipe read error (0x%08X), app will exit", (unsigned int)iStatus);
        {{cookiecutter.app_name}}_AppData.uiRunStatus= CFE_ES_APP_ERROR;
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Data                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void {{cookiecutter.app_name}}_ProcessNewData()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   DataMsgPtr=NULL;
    CFE_SB_MsgId_t  DataMsgId;

    /* Process telemetry messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&DataMsgPtr, {{cookiecutter.app_name}}_AppData.DataPipeId, CFE_SB_POLL);
        if (iStatus == CFE_SUCCESS)
        {
            DataMsgId = CFE_SB_GetMsgId(DataMsgPtr);
            switch (DataMsgId)
            {
                /* TODO:  Add code to process all subscribed data here
                **
                ** Example:
                **     case NAV_OUT_DATA_MID:
                **         {{cookiecutter.app_name}}_ProcessNavData(DataMsgPtr);
                **         break;
                */

                default:
                    (void) CFE_EVS_SendEvent({{cookiecutter.app_name}}_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "Recvd invalid data msgId (0x%04X)", (unsigned short)DataMsgId);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            (void) CFE_EVS_SendEvent({{cookiecutter.app_name}}_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "Data pipe read error (0x%08X)", (unsigned int)iStatus);
            {{cookiecutter.app_name}}_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void {{cookiecutter.app_name}}_ProcessNewCmds()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr=NULL;
    CFE_SB_MsgId_t  CmdMsgId;

    /* Process command messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, {{cookiecutter.app_name}}_AppData.CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case {{cookiecutter.app_name}}_CMD_MID:
                    {{cookiecutter.app_name}}_ProcessNewAppCmds(CmdMsgPtr);
                    break;

                /* TODO:  Add code to process other subscribed commands here
                **
                ** Example:
                **     case CFE_TIME_DATA_CMD_MID:
                **         {{cookiecutter.app_name}}_ProcessTimeDataCmd(CmdMsgPtr);
                **         break;
                */

                default:
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with this
                     *  pipe, but not handled in this switch-case.) */
                    {{cookiecutter.app_name}}_AppData.HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent({{cookiecutter.app_name}}_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            (void) CFE_EVS_SendEvent({{cookiecutter.app_name}}_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08X)", (unsigned int)iStatus);
            {{cookiecutter.app_name}}_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process {{cookiecutter.app_name}} Commands                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void {{cookiecutter.app_name}}_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case {{cookiecutter.app_name}}_NOOP_CC:
                {{cookiecutter.app_name}}_AppData.HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent({{cookiecutter.app_name}}_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "Recvd NOOP cmd (%u), Version %d.%d.%d.%d",
                                  (unsigned int)uiCmdCode,
                                  {{cookiecutter.app_name}}_MAJOR_VERSION,
                                  {{cookiecutter.app_name}}_MINOR_VERSION,
                                  {{cookiecutter.app_name}}_REVISION,
                                  {{cookiecutter.app_name}}_MISSION_REV);
                break;

            case {{cookiecutter.app_name}}_RESET_CC:
                {{cookiecutter.app_name}}_AppData.HkTlm.usCmdCnt = 0;
                {{cookiecutter.app_name}}_AppData.HkTlm.usCmdErrCnt = 0;
                (void) CFE_EVS_SendEvent({{cookiecutter.app_name}}_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "Recvd RESET cmd (%u)", (unsigned int)uiCmdCode);
                break;

            /* TODO:  Add code to process the rest of the {{cookiecutter.app_name}} commands here */

            default:
                {{cookiecutter.app_name}}_AppData.HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent({{cookiecutter.app_name}}_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid cmdId (%u)", (unsigned int)uiCmdCode);
                break;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send {{cookiecutter.app_name}} Housekeeping                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void {{cookiecutter.app_name}}_ReportHousekeeping()
{
    /* TODO:  Add code to update housekeeping data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&{{cookiecutter.app_name}}_AppData.HkTlm);
    int32 iStatus = CFE_SB_SendMsg((CFE_SB_Msg_t*)&{{cookiecutter.app_name}}_AppData.HkTlm);
    if (iStatus != CFE_SUCCESS)
    {
        /* TODO: Decide what to do if the send message fails. */
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Publish Output Data                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void {{cookiecutter.app_name}}_SendOutData()
{
    /* TODO:  Add code to update output data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&{{cookiecutter.app_name}}_AppData.OutData);
    int32 iStatus = CFE_SB_SendMsg((CFE_SB_Msg_t*)&{{cookiecutter.app_name}}_AppData.OutData);
    if (iStatus != CFE_SUCCESS)
    {
        /* TODO: Decide what to do if the send message fails. */
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Command Length                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

boolean {{cookiecutter.app_name}}_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
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

            (void) CFE_EVS_SendEvent({{cookiecutter.app_name}}_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            {{cookiecutter.app_name}}_AppData.HkTlm.usCmdErrCnt++;
        }
    }

    return (bResult);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* {{cookiecutter.app_name}} application entry point and main process loop               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void {{cookiecutter.app_name}}_AppMain()
{
    /* Register the application with Executive Services */
    {{cookiecutter.app_name}}_AppData.uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("{{cookiecutter.app_name}} - Failed to register the app (0x%08X)\n", (unsigned int)iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry({{cookiecutter.app_name}}_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = {{cookiecutter.app_name}}_InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit({{cookiecutter.app_name}}_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync({{cookiecutter.app_name}}_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry({{cookiecutter.app_name}}_MAIN_TASK_PERF_ID);
    }
    else
    {
        {{cookiecutter.app_name}}_AppData.uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&{{cookiecutter.app_name}}_AppData.uiRunStatus) == TRUE)
    {
        int32 iStatus = {{cookiecutter.app_name}}_RcvMsg({{cookiecutter.app_name}}_SCH_PIPE_PEND_TIME);
        if (iStatus != CFE_SUCCESS)
        {
            /* TODO: Decide what to do for other return values in {{cookiecutter.app_name}}_RcvMsg(). */
        }
        /* TODO: This is only a suggestion for when to update and save CDS table.
        ** Depends on the nature of the application, the frequency of update
        ** and save can be more or less independently.
        */
        {{cookiecutter.app_name}}_UpdateCdsTbl();
        {{cookiecutter.app_name}}_SaveCdsTbl();

        iStatus = {{cookiecutter.app_name}}_AcquireConfigPointers();
        if(iStatus != CFE_SUCCESS)
        {
            /* We apparently tried to load a new table but failed.  Terminate the application. */
            {{cookiecutter.app_name}}_AppData.uiRunStatus = CFE_ES_APP_ERROR;
        }
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit({{cookiecutter.app_name}}_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp({{cookiecutter.app_name}}_AppData.uiRunStatus);
}

/************************/
/*  End of File Comment */
/************************/
