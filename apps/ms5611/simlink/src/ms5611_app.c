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

#include "ms5611_app.h"
#include "ms5611_msg.h"
#include "ms5611_version.h"
#include "sedlib.h"
#include "ms5611_map.h"
#include "cvt_lib.h"
#include "simlink.h"
#include "ms5611_events.h"


/************************************************************************
** Local Defines
*************************************************************************/
#define MS5611_SED_CMD_PIPE_NAME             ("SPI0_CMD")
/* Response (status) pipe name. */
#define MS5611_SED_STATUS_PIPE_NAME          ("SPI0_STATUS")
/************************************************************************
** Local Structure Declarations
*************************************************************************/
/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/
MS5611_AppData_t  MS5611_AppData;
MS5611_AppSpiData_t MS5611_AppSpiData;

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

int32 MS5611_InitEvent()
{
    int32  iStatus=CFE_SUCCESS;
    int32  ind = 0;

    /* Initialize the event filter table.
     * Note: 0 is the CFE_EVS_NO_FILTER mask and event 0 is reserved (not used) */
    memset((void*)MS5611_AppData.EventTbl, 0x00, sizeof(MS5611_AppData.EventTbl));

    /* TODO: Choose the events you want to filter.  CFE_EVS_MAX_EVENT_FILTERS
     * limits the number of filters per app.  An explicit CFE_EVS_NO_FILTER 
     * (the default) has been provided as an example. */
    MS5611_AppData.EventTbl[  ind].EventID = MS5611_RESERVED_EID;
    MS5611_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    MS5611_AppData.EventTbl[  ind].EventID = MS5611_INF_EID;
    MS5611_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    MS5611_AppData.EventTbl[  ind].EventID = MS5611_CONFIG_TABLE_ERR_EID;
    MS5611_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    MS5611_AppData.EventTbl[  ind].EventID = MS5611_CDS_ERR_EID;
    MS5611_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    MS5611_AppData.EventTbl[  ind].EventID = MS5611_PIPE_ERR_EID;
    MS5611_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    MS5611_AppData.EventTbl[  ind].EventID = MS5611_MSGID_ERR_EID;
    MS5611_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    MS5611_AppData.EventTbl[  ind].EventID = MS5611_MSGLEN_ERR_EID;
    MS5611_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(MS5611_AppData.EventTbl,
                               MS5611_EVT_CNT, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("MS5611 - Failed to register with EVS (0x%08X)\n", (unsigned int)iStatus);
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 MS5611_InitPipe()
{
    int32  iStatus=CFE_SUCCESS;
    osalbool hasEvents = TRUE;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&MS5611_AppData.SchPipeId,
                                 MS5611_SCH_PIPE_DEPTH,
                                 MS5611_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(MS5611_WAKEUP_MID, MS5611_AppData.SchPipeId, CFE_SB_Default_Qos, MS5611_SCH_PIPE_WAKEUP_RESERVED);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MS5611_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "Sch Pipe failed to subscribe to MS5611_WAKEUP_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto MS5611_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(MS5611_SEND_HK_MID, MS5611_AppData.SchPipeId, CFE_SB_Default_Qos, MS5611_SCH_PIPE_SEND_HK_RESERVED);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MS5611_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "CMD Pipe failed to subscribe to MS5611_SEND_HK_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto MS5611_InitPipe_Exit_Tag;
        }

    }
    else
    {
        (void) CFE_EVS_SendEvent(MS5611_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create SCH pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto MS5611_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&MS5611_AppData.CmdPipeId,
                                 MS5611_CMD_PIPE_DEPTH,
                                 MS5611_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(MS5611_CMD_MID, MS5611_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MS5611_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "CMD Pipe failed to subscribe to MS5611_CMD_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto MS5611_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(MS5611_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create CMD pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto MS5611_InitPipe_Exit_Tag;
    }

    /* Init data pipe and subscribe to messages on the data pipe */
    iStatus = CFE_SB_CreatePipe(&MS5611_AppData.DataPipeId,
                                 MS5611_DATA_PIPE_DEPTH,
                                 MS5611_DATA_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* TODO:  Add CFE_SB_Subscribe() calls for other apps' output data here.
        **
        ** Examples:
        **     CFE_SB_Subscribe(GNCEXEC_OUT_DATA_MID, MS5611_AppData.DataPipeId);
        */
    }
    else
    {
        (void) CFE_EVS_SendEvent(MS5611_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create Data pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto MS5611_InitPipe_Exit_Tag;
    }

    iStatus = SEDLIB_GetPipe(
            MS5611_SED_CMD_PIPE_NAME,
            sizeof(SPI_TransferCmd_t),
            &MS5611_AppSpiData.CmdPortHandle);
    if(iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MS5611_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to initialize SPI CmdPortHandle. (0x%08lX)",
                                 iStatus);
    }

    iStatus = SEDLIB_GetPipe(
            MS5611_SED_STATUS_PIPE_NAME,
            sizeof(SPI_TransferResponse_t),
            &MS5611_AppSpiData.StatusPortHandle);
    if(iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MS5611_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to initialize SPI StatusPortHandle. (0x%08lX)",
                                 iStatus);
    }

    iStatus = MS5611_InitCVT();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MS5611_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init CVT (0x%08X)",
                                 (unsigned int)iStatus);
        goto MS5611_InitPipe_Exit_Tag;
    }



MS5611_InitPipe_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MS5611_INIT_INF_EID, CFE_EVS_INFORMATION,
                                 "Initialized Pipes.");
    }
    else
    {
        if (hasEvents == 1)
        {
            (void) CFE_EVS_SendEvent(MS5611_INIT_ERR_EID, CFE_EVS_ERROR, "Application failed to initialize");
        }
        else
        {
            (void) CFE_ES_WriteToSysLog("MS5611 - Application failed to initialize\n");
        }
    }
    return (iStatus);
}
    

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 MS5611_InitData()
{
    int32  iStatus=CFE_SUCCESS;
    /* Init input data */
    memset((void*)&MS5611_AppData.InData, 0x00, sizeof(MS5611_AppData.InData));

    /* Init output data */
    memset((void*)&MS5611_AppData.OutData, 0x00, sizeof(MS5611_AppData.OutData));
    CFE_SB_InitMsg(&MS5611_AppData.OutData,
                   MS5611_OUT_DATA_MID, sizeof(MS5611_AppData.OutData), TRUE);

    /* Init housekeeping packet */
    memset((void*)&MS5611_AppData.HkTlm, 0x00, sizeof(MS5611_AppData.HkTlm));
    CFE_SB_InitMsg(&MS5611_AppData.HkTlm,
                   MS5611_HK_TLM_MID, sizeof(MS5611_AppData.HkTlm), TRUE);

    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("MS5611 - Failed to initialize app data (0x%08X)\n", (unsigned int)iStatus);
        goto MS5611_InitData_Exit_Tag;
    }

MS5611_InitData_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MS5611_INIT_INF_EID, CFE_EVS_INFORMATION,
                                 "Initialized Data.  Version %d.%d.%d.%d",
                                 MS5611_MAJOR_VERSION,
                                 MS5611_MINOR_VERSION,
                                 MS5611_REVISION,
                                 MS5611_MISSION_REV);
    }
    else
    {
        (void) CFE_ES_WriteToSysLog("MS5611 - Application failed to initialize App data\n");
    }

    return (iStatus);
}

int32 MS5611_InitCVT(void)
{
    int32 status;

    for(uint32 i = 0; i < MS5611_DEVICE_COUNT; ++i)
    {
        char name[CVT_CONTAINER_NAME_MAX_LENGTH];
        uint32 size;

        /* Baro */
        size = sizeof(SIMLINK_Baro_Msg_t);
        sprintf(name, SIMLINK_BARO_CONTAINER_NAME_SPEC, i);
        status = CVT_GetContainer(name, sizeof(SIMLINK_Baro_Msg_t), &MS5611_AppData.BaroContainer[i]);
        if(CVT_SUCCESS != status)
        {
            (void) CFE_EVS_SendEvent(MS5611_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "Failed to get BARO %ld container. (%li)",
                                     i,
                                     status);
            goto end_of_function;
        }

        status = CVT_GetContent(MS5611_AppData.BaroContainer[i], &MS5611_AppData.HkTlm.BaroUpdateCount[i], &MS5611_AppData.HkTlm.BaroMsg[i], &size);
        if(CVT_SUCCESS != status)
        {
            (void) CFE_EVS_SendEvent(MS5611_CVT_ERR_EID, CFE_EVS_ERROR,
                                     "Failed to get Baro %ld container. (%li)",
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
/* MS5611 initialization                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 MS5611_InitApp()
{
    int32  iStatus   = CFE_SUCCESS;

    iStatus = MS5611_InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("MS5611 - Failed to init events (0x%08X)\n", (unsigned int)iStatus);
        goto MS5611_InitApp_Exit_Tag;
    }

    iStatus = MS5611_InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MS5611_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init pipes (0x%08X)",
                                 (unsigned int)iStatus);
        goto MS5611_InitApp_Exit_Tag;
    }

    iStatus = MS5611_InitData();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MS5611_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init data (0x%08X)",
                                 (unsigned int)iStatus);
        goto MS5611_InitApp_Exit_Tag;
    }

    iStatus = MS5611_InitConfigTbl();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MS5611_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init config tables (0x%08X)",
                                 (unsigned int)iStatus);
        goto MS5611_InitApp_Exit_Tag;
    }

    iStatus = MS5611_InitCdsTbl();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MS5611_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init CDS table (0x%08X)",
                                 (unsigned int)iStatus);
        goto MS5611_InitApp_Exit_Tag;
    }


MS5611_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MS5611_INIT_INF_EID, CFE_EVS_INFORMATION,
                                 "Initialized.  Version %d.%d.%d.%d",
                                 MS5611_MAJOR_VERSION,
                                 MS5611_MINOR_VERSION,
                                 MS5611_REVISION,
                                 MS5611_MISSION_REV);
    }
    else
    {
        (void) CFE_EVS_SendEvent(MS5611_INIT_ERR_EID, CFE_EVS_ERROR, "Application failed to initialize");
        (void) CFE_ES_WriteToSysLog("MS5611 - Application failed to initialize\n");
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 MS5611_RcvMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(MS5611_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, MS5611_AppData.SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(MS5611_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
	{
            case MS5611_WAKEUP_MID:
                MS5611_ProcessNewCmds();
                MS5611_ProcessNewData();

                /* TODO:  Add more code here to handle other things when app wakes up */

                /* The last thing to do at the end of this Wakeup cycle should be to
                 * automatically publish new output. */
                MS5611_SendOutData();
                break;

            case MS5611_SEND_HK_MID:
                MS5611_ReportHousekeeping();
                break;

            default:
                (void) CFE_EVS_SendEvent(MS5611_MSGID_ERR_EID, CFE_EVS_ERROR,
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
        (void) CFE_EVS_SendEvent(MS5611_PIPE_ERR_EID, CFE_EVS_ERROR,
			  "SB pipe read error (0x%08X), app will exit", (unsigned int)iStatus);
        MS5611_AppData.uiRunStatus= CFE_ES_APP_ERROR;
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Data                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void MS5611_ProcessNewData()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   DataMsgPtr=NULL;
    CFE_SB_MsgId_t  DataMsgId;

    for(uint32 i = 0; i < MS5611_DEVICE_COUNT; ++i)
    {
        char name[CVT_CONTAINER_NAME_MAX_LENGTH];
        uint32 size;

        /* Baro */
        size = sizeof(SIMLINK_Baro_Msg_t);
        iStatus = CVT_GetContent(MS5611_AppData.BaroContainer[i], &MS5611_AppData.HkTlm.BaroUpdateCount[i], &MS5611_AppData.HkTlm.BaroMsg[i], &size);
        if(CVT_SUCCESS != iStatus)
        {
            (void) CFE_EVS_SendEvent(MS5611_CVT_ERR_EID, CFE_EVS_ERROR,
                                     "Failed to get GYRO %ld container. (%ui)",
                                     i,
                                     iStatus);
            goto end_of_function;
        }
        //TODO:Mat conversions
    }

    /* Process telemetry messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&DataMsgPtr, MS5611_AppData.DataPipeId, CFE_SB_POLL);
        if (iStatus == CFE_SUCCESS)
        {
            DataMsgId = CFE_SB_GetMsgId(DataMsgPtr);
            switch (DataMsgId)
            {
                /* TODO:  Add code to process all subscribed data here
                **
                ** Example:
                **     case NAV_OUT_DATA_MID:
                **         MS5611_ProcessNavData(DataMsgPtr);
                **         break;
                */

                default:
                    (void) CFE_EVS_SendEvent(MS5611_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            (void) CFE_EVS_SendEvent(MS5611_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "Data pipe read error (0x%08X)", (unsigned int)iStatus);
            MS5611_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }

end_of_function:
    return;
}

void MS5611_SED_ParseCommand(void)
{
    SEDLIB_ReturnCode_t returnCode  = SEDLIB_OK;

    returnCode = SEDLIB_ReadMsg(MS5611_AppSpiData.CmdPortHandle,
                                (CFE_SB_MsgPtr_t)&MS5611_AppSpiData.TransferCmd);

    MS5611_AppData.HkTlm.SED_RC = returnCode;

    if(returnCode == SEDLIB_MSG_FRESH_OK)
    {
        /* Process command messages till the pipe is empty */

        CFE_SB_MsgId_t  msgID  = CFE_SB_GetMsgId((CFE_SB_MsgPtr_t)&MS5611_AppSpiData.TransferCmd);
        uint16          cmdCode;

        if(SPI_TRANSFER_CC != msgID)
        {
            (void) CFE_EVS_SendEvent(MS5611_SEDLIB_ERR_EID, CFE_EVS_ERROR,
                                     "Received invalid message from SED. MsgID: 0x%04x",
                                     msgID);
//            ICM20689_AppData.ExecutionMode = ICM20689_MODE_IDLE;
            goto end_of_function;
        }
    }

end_of_function:
    return;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void MS5611_ProcessNewCmds()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr=NULL;
    CFE_SB_MsgId_t  CmdMsgId;

    /* Process command messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, MS5611_AppData.CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case MS5611_CMD_MID:
                    MS5611_ProcessNewAppCmds(CmdMsgPtr);
                    break;

                /* TODO:  Add code to process other subscribed commands here
                **
                ** Example:
                **     case CFE_TIME_DATA_CMD_MID:
                **         MS5611_ProcessTimeDataCmd(CmdMsgPtr);
                **         break;
                */

                default:
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with this
                     *  pipe, but not handled in this switch-case.) */
                    MS5611_AppData.HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(MS5611_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            (void) CFE_EVS_SendEvent(MS5611_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08X)", (unsigned int)iStatus);
            MS5611_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process MS5611 Commands                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void MS5611_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case MS5611_NOOP_CC:
                MS5611_AppData.HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(MS5611_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "Recvd NOOP cmd (%u), Version %d.%d.%d.%d",
                                  (unsigned int)uiCmdCode,
                                  MS5611_MAJOR_VERSION,
                                  MS5611_MINOR_VERSION,
                                  MS5611_REVISION,
                                  MS5611_MISSION_REV);
                break;

            case MS5611_RESET_CC:
                MS5611_AppData.HkTlm.usCmdCnt = 0;
                MS5611_AppData.HkTlm.usCmdErrCnt = 0;
                (void) CFE_EVS_SendEvent(MS5611_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "Recvd RESET cmd (%u)", (unsigned int)uiCmdCode);
                break;

            /* TODO:  Add code to process the rest of the MS5611 commands here */

            default:
                MS5611_AppData.HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(MS5611_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid cmdId (%u)", (unsigned int)uiCmdCode);
                break;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send MS5611 Housekeeping                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void MS5611_ReportHousekeeping()
{
    /* TODO:  Add code to update housekeeping data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&MS5611_AppData.HkTlm);
    int32 iStatus = CFE_SB_SendMsg((CFE_SB_Msg_t*)&MS5611_AppData.HkTlm);
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

void MS5611_SendOutData()
{
    /* TODO:  Add code to update output data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&MS5611_AppData.OutData);
    int32 iStatus = CFE_SB_SendMsg((CFE_SB_Msg_t*)&MS5611_AppData.OutData);
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

boolean MS5611_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
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

            (void) CFE_EVS_SendEvent(MS5611_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            MS5611_AppData.HkTlm.usCmdErrCnt++;
        }
    }

    return (bResult);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* MS5611 application entry point and main process loop               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void MS5611_AppMain()
{
    /* Register the application with Executive Services */
    MS5611_AppData.uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("MS5611 - Failed to register the app (0x%08X)\n", (unsigned int)iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(MS5611_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = MS5611_InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(MS5611_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(MS5611_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(MS5611_MAIN_TASK_PERF_ID);
    }
    else
    {
        MS5611_AppData.uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&MS5611_AppData.uiRunStatus) == TRUE)
    {
        int32 iStatus = MS5611_RcvMsg(MS5611_SCH_PIPE_PEND_TIME);
        if (iStatus != CFE_SUCCESS)
        {
            /* TODO: Decide what to do for other return values in MS5611_RcvMsg(). */
        }
        /* TODO: This is only a suggestion for when to update and save CDS table.
        ** Depends on the nature of the application, the frequency of update
        ** and save can be more or less independently.
        */
        MS5611_UpdateCdsTbl();
        MS5611_SaveCdsTbl();

        iStatus = MS5611_AcquireConfigPointers();
        if(iStatus != CFE_SUCCESS)
        {
            /* We apparently tried to load a new table but failed.  Terminate the application. */
            MS5611_AppData.uiRunStatus = CFE_ES_APP_ERROR;
        }
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(MS5611_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(MS5611_AppData.uiRunStatus);
}

/************************/
/*  End of File Comment */
/************************/
