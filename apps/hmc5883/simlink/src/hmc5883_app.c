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

#include "hmc5883_app.h"
#include "hmc5883_msg.h"
#include "hmc5883_version.h"
#include "simlink.h"

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
HMC5883_AppData_t  HMC5883_AppData;

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Definitions
*************************************************************************/
int32 HMC5883_InitCVT(void);


int32 HMC5883_InitCVT(void)
{
	int32 status = CFE_SUCCESS;

	for(uint32 i = 0; i < HMC5883_MAG_DEVICE_COUNT; ++i)
	{
		uint32 updateCount = 0;
		char name[CVT_CONTAINER_NAME_MAX_LENGTH];
		uint32 size = sizeof(SIMLINK_Mag_Msg_t);

		sprintf(name, SIMLINK_MAG_CONTAINER_NAME_SPEC, i);

		status = CVT_GetContainer(name, sizeof(SIMLINK_Mag_Msg_t), &HMC5883_AppData.MagContainer[i]);
		if(CVT_SUCCESS != status)
		{
	        (void) CFE_EVS_SendEvent(HMC5883_INIT_ERR_EID, CFE_EVS_ERROR,
	                                 "Failed to get Mag %ld container. (%li)",
									 i,
	                                 status);
	        goto end_of_function;
		}

    	status = CVT_GetContent(HMC5883_AppData.MagContainer[i], &updateCount, &HMC5883_AppData.MagMsg[i], &size);
    	if(CVT_SUCCESS != status)
    	{
            (void) CFE_EVS_SendEvent(HMC5883_CVT_ERR_EID, CFE_EVS_ERROR,
                                     "Failed to get Mag %ld container. (%li)",
									 i,
                                     status);
	        goto end_of_function;
    	}
	}

end_of_function:

    return status;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 HMC5883_InitEvent()
{
    int32  iStatus=CFE_SUCCESS;
    int32  ind = 0;

    /* Initialize the event filter table.
     * Note: 0 is the CFE_EVS_NO_FILTER mask and event 0 is reserved (not used) */
    memset((void*)HMC5883_AppData.EventTbl, 0x00, sizeof(HMC5883_AppData.EventTbl));

    /* TODO: Choose the events you want to filter.  CFE_EVS_MAX_EVENT_FILTERS
     * limits the number of filters per app.  An explicit CFE_EVS_NO_FILTER 
     * (the default) has been provided as an example. */
    HMC5883_AppData.EventTbl[  ind].EventID = HMC5883_RESERVED_EID;
    HMC5883_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    HMC5883_AppData.EventTbl[  ind].EventID = HMC5883_INF_EID;
    HMC5883_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    HMC5883_AppData.EventTbl[  ind].EventID = HMC5883_CONFIG_TABLE_ERR_EID;
    HMC5883_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    HMC5883_AppData.EventTbl[  ind].EventID = HMC5883_CDS_ERR_EID;
    HMC5883_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    HMC5883_AppData.EventTbl[  ind].EventID = HMC5883_PIPE_ERR_EID;
    HMC5883_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    HMC5883_AppData.EventTbl[  ind].EventID = HMC5883_MSGID_ERR_EID;
    HMC5883_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    HMC5883_AppData.EventTbl[  ind].EventID = HMC5883_MSGLEN_ERR_EID;
    HMC5883_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(HMC5883_AppData.EventTbl,
                               HMC5883_EVT_CNT, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("HMC5883 - Failed to register with EVS (0x%08X)\n", (unsigned int)iStatus);
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 HMC5883_InitPipe()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&HMC5883_AppData.SchPipeId,
                                 HMC5883_SCH_PIPE_DEPTH,
                                 HMC5883_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(HMC5883_WAKEUP_MID, HMC5883_AppData.SchPipeId, CFE_SB_Default_Qos, HMC5883_SCH_PIPE_WAKEUP_RESERVED);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(HMC5883_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "Sch Pipe failed to subscribe to HMC5883_WAKEUP_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto HMC5883_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(HMC5883_SEND_HK_MID, HMC5883_AppData.SchPipeId, CFE_SB_Default_Qos, HMC5883_SCH_PIPE_SEND_HK_RESERVED);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(HMC5883_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "CMD Pipe failed to subscribe to HMC5883_SEND_HK_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto HMC5883_InitPipe_Exit_Tag;
        }

    }
    else
    {
        (void) CFE_EVS_SendEvent(HMC5883_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create SCH pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto HMC5883_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&HMC5883_AppData.CmdPipeId,
                                 HMC5883_CMD_PIPE_DEPTH,
                                 HMC5883_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(HMC5883_CMD_MID, HMC5883_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(HMC5883_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "CMD Pipe failed to subscribe to HMC5883_CMD_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto HMC5883_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(HMC5883_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create CMD pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto HMC5883_InitPipe_Exit_Tag;
    }

    /* Init data pipe and subscribe to messages on the data pipe */
    iStatus = CFE_SB_CreatePipe(&HMC5883_AppData.DataPipeId,
                                 HMC5883_DATA_PIPE_DEPTH,
                                 HMC5883_DATA_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* TODO:  Add CFE_SB_Subscribe() calls for other apps' output data here.
        **
        ** Examples:
        **     CFE_SB_Subscribe(GNCEXEC_OUT_DATA_MID, HMC5883_AppData.DataPipeId);
        */
    }
    else
    {
        (void) CFE_EVS_SendEvent(HMC5883_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create Data pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto HMC5883_InitPipe_Exit_Tag;
    }

HMC5883_InitPipe_Exit_Tag:
    return (iStatus);
}
    

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 HMC5883_InitData()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init input data */
    memset((void*)&HMC5883_AppData.InData, 0x00, sizeof(HMC5883_AppData.InData));

    /* Init output data */
    memset((void*)&HMC5883_AppData.OutData, 0x00, sizeof(HMC5883_AppData.OutData));
    CFE_SB_InitMsg(&HMC5883_AppData.OutData,
                   HMC5883_OUT_DATA_MID, sizeof(HMC5883_AppData.OutData), TRUE);

    /* Init housekeeping packet */
    memset((void*)&HMC5883_AppData.HkTlm, 0x00, sizeof(HMC5883_AppData.HkTlm));
    CFE_SB_InitMsg(&HMC5883_AppData.HkTlm,
                   HMC5883_HK_TLM_MID, sizeof(HMC5883_AppData.HkTlm), TRUE);

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* HMC5883 initialization                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 HMC5883_InitApp()
{
    int32  iStatus   = CFE_SUCCESS;
    int8   hasEvents = 0;

    iStatus = HMC5883_InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("HMC5883 - Failed to init events (0x%08X)\n", (unsigned int)iStatus);
        goto HMC5883_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = HMC5883_InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(HMC5883_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init pipes (0x%08X)",
                                 (unsigned int)iStatus);
        goto HMC5883_InitApp_Exit_Tag;
    }

    iStatus = HMC5883_InitData();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(HMC5883_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init data (0x%08X)",
                                 (unsigned int)iStatus);
        goto HMC5883_InitApp_Exit_Tag;
    }

    iStatus = HMC5883_InitConfigTbl();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(HMC5883_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init config tables (0x%08X)",
                                 (unsigned int)iStatus);
        goto HMC5883_InitApp_Exit_Tag;
    }

    iStatus = HMC5883_InitCdsTbl();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(HMC5883_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init CDS table (0x%08X)",
                                 (unsigned int)iStatus);
        goto HMC5883_InitApp_Exit_Tag;
    }

    iStatus = HMC5883_InitCVT();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(HMC5883_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init CVT (0x%08X)",
                                 (unsigned int)iStatus);
        goto HMC5883_InitApp_Exit_Tag;
    }

HMC5883_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(HMC5883_INIT_INF_EID, CFE_EVS_INFORMATION,
                                 "Initialized.  Version %d.%d.%d.%d",
                                 HMC5883_MAJOR_VERSION,
                                 HMC5883_MINOR_VERSION,
                                 HMC5883_REVISION,
                                 HMC5883_MISSION_REV);
    }
    else
    {
        if (hasEvents == 1)
        {
            (void) CFE_EVS_SendEvent(HMC5883_INIT_ERR_EID, CFE_EVS_ERROR, "Application failed to initialize");
        }
        else
        {
            (void) CFE_ES_WriteToSysLog("HMC5883 - Application failed to initialize\n");
        }
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 HMC5883_RcvMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(HMC5883_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, HMC5883_AppData.SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(HMC5883_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
	{
            case HMC5883_WAKEUP_MID:
                HMC5883_ProcessNewCmds();
                HMC5883_ProcessNewData();

                /* TODO:  Add more code here to handle other things when app wakes up */

                /* The last thing to do at the end of this Wakeup cycle should be to
                 * automatically publish new output. */
                HMC5883_SendOutData();
                break;

            case HMC5883_SEND_HK_MID:
                HMC5883_ReportHousekeeping();
                break;

            default:
                (void) CFE_EVS_SendEvent(HMC5883_MSGID_ERR_EID, CFE_EVS_ERROR,
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
        (void) CFE_EVS_SendEvent(HMC5883_PIPE_ERR_EID, CFE_EVS_ERROR,
			  "SB pipe read error (0x%08X), app will exit", (unsigned int)iStatus);
        HMC5883_AppData.uiRunStatus= CFE_ES_APP_ERROR;
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Data                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void HMC5883_ProcessNewData()
{
    int32 status = CFE_SUCCESS;
    CFE_SB_Msg_t*   DataMsgPtr=NULL;
    CFE_SB_MsgId_t  DataMsgId;
    uint32          updateCount;
    uint32          size = sizeof(HMC5883_AppData.MagMsg[0]);

	status = CVT_GetContent(HMC5883_AppData.MagContainer[0], &updateCount, &HMC5883_AppData.MagMsg[0], &size);
	if(CVT_SUCCESS != status)
	{
        (void) CFE_EVS_SendEvent(HMC5883_CVT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to get Mag content. (%li)",
                                 status);
        goto end_of_function;
	}

	if(updateCount != HMC5883_AppData.MagUpdateCount)
	{
		/* We received new data.  Store this update count and act on the new data. */
		HMC5883_AppData.MagUpdateCount = updateCount;

		/* TODO */
		//OS_printf("Time: %llu   X: %f   Y: %f   Z: %f\n",
		//	HMC5883_AppData.MagMsg[0].TimeUsec,
		//	HMC5883_AppData.MagMsg[0].X,
		//	HMC5883_AppData.MagMsg[0].Y,
		//	HMC5883_AppData.MagMsg[0].Z);
	}

end_of_function:

    return;

}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void HMC5883_ProcessNewCmds()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr=NULL;
    CFE_SB_MsgId_t  CmdMsgId;

    /* Process command messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, HMC5883_AppData.CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case HMC5883_CMD_MID:
                    HMC5883_ProcessNewAppCmds(CmdMsgPtr);
                    break;

                /* TODO:  Add code to process other subscribed commands here
                **
                ** Example:
                **     case CFE_TIME_DATA_CMD_MID:
                **         HMC5883_ProcessTimeDataCmd(CmdMsgPtr);
                **         break;
                */

                default:
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with this
                     *  pipe, but not handled in this switch-case.) */
                    HMC5883_AppData.HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(HMC5883_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            (void) CFE_EVS_SendEvent(HMC5883_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08X)", (unsigned int)iStatus);
            HMC5883_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process HMC5883 Commands                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void HMC5883_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case HMC5883_NOOP_CC:
                HMC5883_AppData.HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(HMC5883_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "Recvd NOOP cmd (%u), Version %d.%d.%d.%d",
                                  (unsigned int)uiCmdCode,
                                  HMC5883_MAJOR_VERSION,
                                  HMC5883_MINOR_VERSION,
                                  HMC5883_REVISION,
                                  HMC5883_MISSION_REV);
                break;

            case HMC5883_RESET_CC:
                HMC5883_AppData.HkTlm.usCmdCnt = 0;
                HMC5883_AppData.HkTlm.usCmdErrCnt = 0;
                (void) CFE_EVS_SendEvent(HMC5883_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "Recvd RESET cmd (%u)", (unsigned int)uiCmdCode);
                break;

            /* TODO:  Add code to process the rest of the HMC5883 commands here */

            default:
                HMC5883_AppData.HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(HMC5883_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid cmdId (%u)", (unsigned int)uiCmdCode);
                break;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send HMC5883 Housekeeping                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void HMC5883_ReportHousekeeping()
{
    /* TODO:  Add code to update housekeeping data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&HMC5883_AppData.HkTlm);
    int32 iStatus = CFE_SB_SendMsg((CFE_SB_Msg_t*)&HMC5883_AppData.HkTlm);
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

void HMC5883_SendOutData()
{
    /* TODO:  Add code to update output data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&HMC5883_AppData.OutData);
    int32 iStatus = CFE_SB_SendMsg((CFE_SB_Msg_t*)&HMC5883_AppData.OutData);
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

boolean HMC5883_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
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

            (void) CFE_EVS_SendEvent(HMC5883_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            HMC5883_AppData.HkTlm.usCmdErrCnt++;
        }
    }

    return (bResult);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* HMC5883 application entry point and main process loop               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void HMC5883_AppMain()
{
    /* Register the application with Executive Services */
    HMC5883_AppData.uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("HMC5883 - Failed to register the app (0x%08X)\n", (unsigned int)iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(HMC5883_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = HMC5883_InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(HMC5883_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(HMC5883_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(HMC5883_MAIN_TASK_PERF_ID);
    }
    else
    {
        HMC5883_AppData.uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&HMC5883_AppData.uiRunStatus) == TRUE)
    {
        int32 iStatus = HMC5883_RcvMsg(HMC5883_SCH_PIPE_PEND_TIME);
        if (iStatus != CFE_SUCCESS)
        {
            /* TODO: Decide what to do for other return values in HMC5883_RcvMsg(). */
        }
        /* TODO: This is only a suggestion for when to update and save CDS table.
        ** Depends on the nature of the application, the frequency of update
        ** and save can be more or less independently.
        */
        HMC5883_UpdateCdsTbl();
        HMC5883_SaveCdsTbl();

        iStatus = HMC5883_AcquireConfigPointers();
        if(iStatus != CFE_SUCCESS)
        {
            /* We apparently tried to load a new table but failed.  Terminate the application. */
            HMC5883_AppData.uiRunStatus = CFE_ES_APP_ERROR;
        }
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(HMC5883_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(HMC5883_AppData.uiRunStatus);
}

/************************/
/*  End of File Comment */
/************************/