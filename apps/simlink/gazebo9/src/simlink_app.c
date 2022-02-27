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

#include "simlink_app.h"
#include "simlink_msg.h"
#include "simlink_version.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "mavlink.h"
#include <unistd.h>
#include "cvt_lib.h"

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Declarations
*************************************************************************/
typedef enum
{
	SIMLINK_ACCEL         = 0b111,
	SIMLINK_GYRO          = 0b111000,
	SIMLINK_MAG           = 0b111000000,
	SIMLINK_BARO          = 0b1101000000000,
	SIMLINK_DIFF_PRESS    = 0b10000000000
} SIMLINK_SensorSource_t;

/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/
SIMLINK_AppData_t  SIMLINK_AppData;

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Definitions
*************************************************************************/
int32 SIMLINK_InitListener(void);
void  SIMLINK_ListenerTaskMain(void);
int32 SIMLINK_SendHeartbeat(void);


int32 SIMLINK_InitCVT(void)
{
	int32 status;

	for(uint32 i = 0; i < SIMLINK_GPS_DEVICE_COUNT; ++i)
	{
		uint32 updateCount = 0;
		char name[CVT_CONTAINER_NAME_MAX_LENGTH];
		uint32 size = sizeof(SIMLINK_GPS_Msg_t);

		sprintf(name, SIMLINK_GPS_CONTAINER_NAME_SPEC, i);

		status = CVT_GetContainer(name, sizeof(SIMLINK_GPS_Msg_t), &SIMLINK_AppData.GpsContainer[i]);
		if(CVT_SUCCESS != status)
		{
	        (void) CFE_EVS_SendEvent(SIMLINK_INIT_ERR_EID, CFE_EVS_ERROR,
	                                 "Failed to get GPS %ld container. (%li)",
									 i,
	                                 status);
	        goto end_of_function;
		}

    	status = CVT_GetContent(SIMLINK_AppData.GpsContainer[i], &updateCount, &SIMLINK_AppData.GpsMsg[i], &size);
    	if(CVT_SUCCESS != status)
    	{
            (void) CFE_EVS_SendEvent(SIMLINK_CVT_ERR_EID, CFE_EVS_ERROR,
                                     "Failed to get GPS %ld container. (%li)",
									 i,
                                     status);
	        goto end_of_function;
    	}
	}

	for(uint32 i = 0; i < SIMLINK_GYRO_DEVICE_COUNT; ++i)
	{
		uint32 updateCount = 0;
		char name[CVT_CONTAINER_NAME_MAX_LENGTH];
		uint32 size = sizeof(SIMLINK_Gyro_Msg_t);

		sprintf(name, SIMLINK_GYRO_CONTAINER_NAME_SPEC, i);

		status = CVT_GetContainer(name, sizeof(SIMLINK_Gyro_Msg_t), &SIMLINK_AppData.GyroContainer[i]);
		if(CVT_SUCCESS != status)
		{
	        (void) CFE_EVS_SendEvent(SIMLINK_INIT_ERR_EID, CFE_EVS_ERROR,
	                                 "Failed to get GYRO %ld container. (%li)",
									 i,
	                                 status);
	        goto end_of_function;
		}

    	status = CVT_GetContent(SIMLINK_AppData.GyroContainer[i], &updateCount, &SIMLINK_AppData.GyroMsg[i], &size);
    	if(CVT_SUCCESS != status)
    	{
            (void) CFE_EVS_SendEvent(SIMLINK_CVT_ERR_EID, CFE_EVS_ERROR,
                                     "Failed to get GYRO %ld container. (%li)",
									 i,
                                     status);
	        goto end_of_function;
    	}
	}

	for(uint32 i = 0; i < SIMLINK_ACCEL_DEVICE_COUNT; ++i)
	{
		uint32 updateCount = 0;
		char name[CVT_CONTAINER_NAME_MAX_LENGTH];
		uint32 size = sizeof(SIMLINK_Accel_Msg_t);

		sprintf(name, SIMLINK_ACCEL_CONTAINER_NAME_SPEC, i);

		status = CVT_GetContainer(name, sizeof(SIMLINK_Accel_Msg_t), &SIMLINK_AppData.AccelContainer[i]);
		if(CVT_SUCCESS != status)
		{
	        (void) CFE_EVS_SendEvent(SIMLINK_INIT_ERR_EID, CFE_EVS_ERROR,
	                                 "Failed to get ACCEL %ld container. (%li)",
									 i,
	                                 status);
	        goto end_of_function;
		}

    	status = CVT_GetContent(SIMLINK_AppData.AccelContainer[0], &updateCount, &SIMLINK_AppData.AccelMsg[i], &size);
    	if(CVT_SUCCESS != status)
    	{
            (void) CFE_EVS_SendEvent(SIMLINK_CVT_ERR_EID, CFE_EVS_ERROR,
                                     "Failed to get ACCEL %ld container. (%li)",
									 i,
                                     status);
	        goto end_of_function;
    	}
	}

	for(uint32 i = 0; i < SIMLINK_MAG_DEVICE_COUNT; ++i)
	{
		uint32 updateCount = 0;
		char name[CVT_CONTAINER_NAME_MAX_LENGTH];
		uint32 size = sizeof(SIMLINK_Mag_Msg_t);

		sprintf(name, SIMLINK_MAG_CONTAINER_NAME_SPEC, i);

		status = CVT_GetContainer(name, sizeof(SIMLINK_Mag_Msg_t), &SIMLINK_AppData.MagContainer[i]);
		if(CVT_SUCCESS != status)
		{
	        (void) CFE_EVS_SendEvent(SIMLINK_INIT_ERR_EID, CFE_EVS_ERROR,
	                                 "Failed to get Mag %ld container. (%li)",
									 i,
	                                 status);
	        goto end_of_function;
		}

    	status = CVT_GetContent(SIMLINK_AppData.MagContainer[0], &updateCount, &SIMLINK_AppData.MagMsg[i], &size);
    	if(CVT_SUCCESS != status)
    	{
            (void) CFE_EVS_SendEvent(SIMLINK_CVT_ERR_EID, CFE_EVS_ERROR,
                                     "Failed to get Mag %ld container. (%li)",
									 i,
                                     status);
	        goto end_of_function;
    	}
	}

	for(uint32 i = 0; i < SIMLINK_BARO_DEVICE_COUNT; ++i)
	{
		uint32 updateCount = 0;
		char name[CVT_CONTAINER_NAME_MAX_LENGTH];
		uint32 size = sizeof(SIMLINK_Baro_Msg_t);

		sprintf(name, SIMLINK_BARO_CONTAINER_NAME_SPEC, i);

		status = CVT_GetContainer(name, sizeof(SIMLINK_Baro_Msg_t), &SIMLINK_AppData.BaroContainer[i]);
		if(CVT_SUCCESS != status)
		{
	        (void) CFE_EVS_SendEvent(SIMLINK_INIT_ERR_EID, CFE_EVS_ERROR,
	                                 "Failed to get Baro %ld container. (%li)",
									 i,
	                                 status);
	        goto end_of_function;
		}

    	status = CVT_GetContent(SIMLINK_AppData.BaroContainer[0], &updateCount, &SIMLINK_AppData.MagMsg[i], &size);
    	if(CVT_SUCCESS != status)
    	{
            (void) CFE_EVS_SendEvent(SIMLINK_CVT_ERR_EID, CFE_EVS_ERROR,
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
/* Initialize event tables                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 SIMLINK_InitEvent()
{
    int32  iStatus=CFE_SUCCESS;
    int32  ind = 0;

    /* Initialize the event filter table.
     * Note: 0 is the CFE_EVS_NO_FILTER mask and event 0 is reserved (not used) */
    memset((void*)SIMLINK_AppData.EventTbl, 0x00, sizeof(SIMLINK_AppData.EventTbl));

    /* TODO: Choose the events you want to filter.  CFE_EVS_MAX_EVENT_FILTERS
     * limits the number of filters per app.  An explicit CFE_EVS_NO_FILTER 
     * (the default) has been provided as an example. */
    SIMLINK_AppData.EventTbl[  ind].EventID = SIMLINK_RESERVED_EID;
    SIMLINK_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    SIMLINK_AppData.EventTbl[  ind].EventID = SIMLINK_INF_EID;
    SIMLINK_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    SIMLINK_AppData.EventTbl[  ind].EventID = SIMLINK_CONFIG_TABLE_ERR_EID;
    SIMLINK_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    SIMLINK_AppData.EventTbl[  ind].EventID = SIMLINK_CDS_ERR_EID;
    SIMLINK_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    SIMLINK_AppData.EventTbl[  ind].EventID = SIMLINK_PIPE_ERR_EID;
    SIMLINK_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    SIMLINK_AppData.EventTbl[  ind].EventID = SIMLINK_MSGID_ERR_EID;
    SIMLINK_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    SIMLINK_AppData.EventTbl[  ind].EventID = SIMLINK_MSGLEN_ERR_EID;
    SIMLINK_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(SIMLINK_AppData.EventTbl,
                               SIMLINK_EVT_CNT, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("SIMLINK - Failed to register with EVS (0x%08X)\n", (unsigned int)iStatus);
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 SIMLINK_InitPipe()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&SIMLINK_AppData.SchPipeId,
                                 SIMLINK_SCH_PIPE_DEPTH,
                                 SIMLINK_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(SIMLINK_WAKEUP_MID, SIMLINK_AppData.SchPipeId, CFE_SB_Default_Qos, SIMLINK_SCH_PIPE_WAKEUP_RESERVED);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(SIMLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "Sch Pipe failed to subscribe to SIMLINK_WAKEUP_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto SIMLINK_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(SIMLINK_SEND_HK_MID, SIMLINK_AppData.SchPipeId, CFE_SB_Default_Qos, SIMLINK_SCH_PIPE_SEND_HK_RESERVED);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(SIMLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "CMD Pipe failed to subscribe to SIMLINK_SEND_HK_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto SIMLINK_InitPipe_Exit_Tag;
        }

    }
    else
    {
        (void) CFE_EVS_SendEvent(SIMLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create SCH pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto SIMLINK_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&SIMLINK_AppData.CmdPipeId,
                                 SIMLINK_CMD_PIPE_DEPTH,
                                 SIMLINK_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(SIMLINK_CMD_MID, SIMLINK_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(SIMLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "CMD Pipe failed to subscribe to SIMLINK_CMD_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto SIMLINK_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(SIMLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create CMD pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto SIMLINK_InitPipe_Exit_Tag;
    }

    /* Init data pipe and subscribe to messages on the data pipe */
    iStatus = CFE_SB_CreatePipe(&SIMLINK_AppData.DataPipeId,
                                 SIMLINK_DATA_PIPE_DEPTH,
                                 SIMLINK_DATA_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* TODO:  Add CFE_SB_Subscribe() calls for other apps' output data here.
        **
        ** Examples:
        **     CFE_SB_Subscribe(GNCEXEC_OUT_DATA_MID, SIMLINK_AppData.DataPipeId);
        */
    }
    else
    {
        (void) CFE_EVS_SendEvent(SIMLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create Data pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto SIMLINK_InitPipe_Exit_Tag;
    }

SIMLINK_InitPipe_Exit_Tag:
    return (iStatus);
}
    

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 SIMLINK_InitData()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init input data */
    memset((void*)&SIMLINK_AppData.InData, 0x00, sizeof(SIMLINK_AppData.InData));

    /* Init output data */
    memset((void*)&SIMLINK_AppData.OutData, 0x00, sizeof(SIMLINK_AppData.OutData));
    CFE_SB_InitMsg(&SIMLINK_AppData.OutData,
                   SIMLINK_OUT_DATA_MID, sizeof(SIMLINK_AppData.OutData), TRUE);

    /* Init housekeeping packet */
    memset((void*)&SIMLINK_AppData.HkTlm, 0x00, sizeof(SIMLINK_AppData.HkTlm));
    CFE_SB_InitMsg(&SIMLINK_AppData.HkTlm,
                   SIMLINK_HK_TLM_MID, sizeof(SIMLINK_AppData.HkTlm), TRUE);

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* SIMLINK initialization                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 SIMLINK_InitApp()
{
    int32  iStatus   = CFE_SUCCESS;
    int8   hasEvents = 0;

    iStatus = SIMLINK_InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("SIMLINK - Failed to init events (0x%08X)\n", (unsigned int)iStatus);
        goto SIMLINK_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = SIMLINK_InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(SIMLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init pipes (0x%08X)",
                                 (unsigned int)iStatus);
        goto SIMLINK_InitApp_Exit_Tag;
    }

    iStatus = SIMLINK_InitData();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(SIMLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init data (0x%08X)",
                                 (unsigned int)iStatus);
        goto SIMLINK_InitApp_Exit_Tag;
    }

    iStatus = SIMLINK_InitConfigTbl();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(SIMLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init config tables (0x%08X)",
                                 (unsigned int)iStatus);
        goto SIMLINK_InitApp_Exit_Tag;
    }

    iStatus = SIMLINK_InitCdsTbl();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(SIMLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init CDS table (0x%08X)",
                                 (unsigned int)iStatus);
        goto SIMLINK_InitApp_Exit_Tag;
    }

    iStatus = SIMLINK_InitCVT();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(SIMLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init CVT (0x%08X)",
                                 (unsigned int)iStatus);
        goto SIMLINK_InitApp_Exit_Tag;
    }

    iStatus = SIMLINK_InitListener();
    if(iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(SIMLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init listener (0x%08X)",
                                 (unsigned int)iStatus);
        goto SIMLINK_InitApp_Exit_Tag;
    }

SIMLINK_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(SIMLINK_INIT_INF_EID, CFE_EVS_INFORMATION,
                                 "Initialized.  Version %d.%d.%d.%d",
                                 SIMLINK_MAJOR_VERSION,
                                 SIMLINK_MINOR_VERSION,
                                 SIMLINK_REVISION,
                                 SIMLINK_MISSION_REV);
    }
    else
    {
        if (hasEvents == 1)
        {
            (void) CFE_EVS_SendEvent(SIMLINK_INIT_ERR_EID, CFE_EVS_ERROR, "Application failed to initialize");
        }
        else
        {
            (void) CFE_ES_WriteToSysLog("SIMLINK - Application failed to initialize\n");
        }
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 SIMLINK_RcvMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(SIMLINK_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, SIMLINK_AppData.SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(SIMLINK_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
	{
            case SIMLINK_WAKEUP_MID:
                SIMLINK_ProcessNewCmds();
                SIMLINK_ProcessNewData();

                /* TODO:  Add more code here to handle other things when app wakes up */

                /* The last thing to do at the end of this Wakeup cycle should be to
                 * automatically publish new output. */
                SIMLINK_SendOutData();
                break;

            case SIMLINK_SEND_HK_MID:
                SIMLINK_ReportHousekeeping();
                break;

            default:
                (void) CFE_EVS_SendEvent(SIMLINK_MSGID_ERR_EID, CFE_EVS_ERROR,
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
        (void) CFE_EVS_SendEvent(SIMLINK_PIPE_ERR_EID, CFE_EVS_ERROR,
			  "SB pipe read error (0x%08X), app will exit", (unsigned int)iStatus);
        SIMLINK_AppData.uiRunStatus= CFE_ES_APP_ERROR;
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Data                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SIMLINK_ProcessNewData()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   DataMsgPtr=NULL;
    CFE_SB_MsgId_t  DataMsgId;

    /* Process telemetry messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&DataMsgPtr, SIMLINK_AppData.DataPipeId, CFE_SB_POLL);
        if (iStatus == CFE_SUCCESS)
        {
            DataMsgId = CFE_SB_GetMsgId(DataMsgPtr);
            switch (DataMsgId)
            {
                /* TODO:  Add code to process all subscribed data here
                **
                ** Example:
                **     case NAV_OUT_DATA_MID:
                **         SIMLINK_ProcessNavData(DataMsgPtr);
                **         break;
                */

                default:
                    (void) CFE_EVS_SendEvent(SIMLINK_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            (void) CFE_EVS_SendEvent(SIMLINK_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "Data pipe read error (0x%08X)", (unsigned int)iStatus);
            SIMLINK_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SIMLINK_ProcessNewCmds()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr=NULL;
    CFE_SB_MsgId_t  CmdMsgId;

    /* Process command messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, SIMLINK_AppData.CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case SIMLINK_CMD_MID:
                    SIMLINK_ProcessNewAppCmds(CmdMsgPtr);
                    break;

                /* TODO:  Add code to process other subscribed commands here
                **
                ** Example:
                **     case CFE_TIME_DATA_CMD_MID:
                **         SIMLINK_ProcessTimeDataCmd(CmdMsgPtr);
                **         break;
                */

                default:
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with this
                     *  pipe, but not handled in this switch-case.) */
                    SIMLINK_AppData.HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(SIMLINK_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            (void) CFE_EVS_SendEvent(SIMLINK_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08X)", (unsigned int)iStatus);
            SIMLINK_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process SIMLINK Commands                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SIMLINK_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case SIMLINK_NOOP_CC:
                SIMLINK_AppData.HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(SIMLINK_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "Recvd NOOP cmd (%u), Version %d.%d.%d.%d",
                                  (unsigned int)uiCmdCode,
                                  SIMLINK_MAJOR_VERSION,
                                  SIMLINK_MINOR_VERSION,
                                  SIMLINK_REVISION,
                                  SIMLINK_MISSION_REV);
                break;

            case SIMLINK_RESET_CC:
                SIMLINK_AppData.HkTlm.usCmdCnt = 0;
                SIMLINK_AppData.HkTlm.usCmdErrCnt = 0;
                (void) CFE_EVS_SendEvent(SIMLINK_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "Recvd RESET cmd (%u)", (unsigned int)uiCmdCode);
                break;

            /* TODO:  Add code to process the rest of the SIMLINK commands here */

            default:
                SIMLINK_AppData.HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(SIMLINK_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid cmdId (%u)", (unsigned int)uiCmdCode);
                break;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send SIMLINK Housekeeping                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SIMLINK_ReportHousekeeping()
{
    /* TODO:  Add code to update housekeeping data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&SIMLINK_AppData.HkTlm);
    int32 iStatus = CFE_SB_SendMsg((CFE_SB_Msg_t*)&SIMLINK_AppData.HkTlm);
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

void SIMLINK_SendOutData()
{
    /* TODO:  Add code to update output data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&SIMLINK_AppData.OutData);
    int32 iStatus = CFE_SB_SendMsg((CFE_SB_Msg_t*)&SIMLINK_AppData.OutData);
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

boolean SIMLINK_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
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

            (void) CFE_EVS_SendEvent(SIMLINK_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            SIMLINK_AppData.HkTlm.usCmdErrCnt++;
        }
    }

    return (bResult);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* SIMLINK application entry point and main process loop               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SIMLINK_AppMain()
{
    /* Register the application with Executive Services */
    SIMLINK_AppData.uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("SIMLINK - Failed to register the app (0x%08X)\n", (unsigned int)iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(SIMLINK_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = SIMLINK_InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(SIMLINK_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(SIMLINK_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(SIMLINK_MAIN_TASK_PERF_ID);
    }
    else
    {
        SIMLINK_AppData.uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&SIMLINK_AppData.uiRunStatus) == TRUE)
    {
        int32 iStatus = SIMLINK_RcvMsg(SIMLINK_SCH_PIPE_PEND_TIME);
        if (iStatus != CFE_SUCCESS)
        {
            /* TODO: Decide what to do for other return values in SIMLINK_RcvMsg(). */
        }
        /* TODO: This is only a suggestion for when to update and save CDS table.
        ** Depends on the nature of the application, the frequency of update
        ** and save can be more or less independently.
        */
        SIMLINK_UpdateCdsTbl();
        SIMLINK_SaveCdsTbl();

        iStatus = SIMLINK_AcquireConfigPointers();
        if(iStatus != CFE_SUCCESS)
        {
            /* We apparently tried to load a new table but failed.  Terminate the application. */
            SIMLINK_AppData.uiRunStatus = CFE_ES_APP_ERROR;
        }
    }

    SIMLINK_AppData.ChildContinueFlag = FALSE;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(SIMLINK_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(SIMLINK_AppData.uiRunStatus);
}


int32 SIMLINK_InitListener(void)
{
    int32 status = CFE_SUCCESS;
    char *simAddress = 0;
    char *strSimPort = 0;
    uint16 simPort = 0;
    struct             sockaddr_in servAddr;
    int                reuseaddr = 1;
    struct hostent    *he;
    int                rc;
    struct             sockaddr_in serv_addr;

    simAddress = getenv(SIMLINK_SIM_ADDRESS_ENV_VAR_NAME);
    if(0 == simAddress)
	{
        OS_printf("%s environment variable not defined.  Using '127.0.0.1'.\n", SIMLINK_SIM_ADDRESS_ENV_VAR_NAME);
        simAddress = "127.0.0.1";
    }

    strSimPort = getenv(SIMLINK_SIM_PORT_ENV_VAR_NAME);
    if(0 == strSimPort)
	{
        OS_printf("%s environment variable not defined.  Using '4560'.\n", SIMLINK_SIM_PORT_ENV_VAR_NAME);
        strSimPort = "4560";
    }

	simPort = atoi(strSimPort);

    SIMLINK_AppData.Socket = socket(AF_INET, SOCK_STREAM, 0);
    if(SIMLINK_AppData.Socket < 0)
    {
    	OS_printf("ERROR: Could not create socket.\n");
        exit(-3);
    }

    setsockopt(SIMLINK_AppData.Socket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));

    memset(&servAddr, 0, sizeof(servAddr));

    he = gethostbyname(simAddress);
    if(he == NULL)
    {  /* get the host info */
        printf("ERROR: SIM gethostbyname failed. Terminating.\n");
        exit(-4);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(simPort);
    serv_addr.sin_addr = *((struct in_addr *)he->h_addr);
    bzero(&(serv_addr.sin_zero), 8);

    rc = connect(SIMLINK_AppData.Socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(rc < 0)
    {
        printf("ERROR: SIM connect failed. Terminating.\n");
        exit(-5);
    }

    status = CFE_ES_CreateChildTask(&SIMLINK_AppData.ListenerTaskID,
                                   SIMLINK_LISTENER_TASK_NAME,
								   SIMLINK_ListenerTaskMain,
                                   NULL,
								   SIMLINK_LISTENER_TASK_STACK_SIZE,
								   SIMLINK_LISTENER_TASK_PRIORITY,
								   SIMLINK_LISTENER_TASK_FLAGS);
    if (status != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(SIMLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create child task (0x%08X)",
                                 (unsigned int)status);
        goto end_of_function;
    }

end_of_function:

    return status;
}


int32 SIMLINK_SendHeartbeat(void)
{
    int32  status = 0;
    mavlink_message_t msg = {};
    uint8  buffer[MAVLINK_MAX_PACKET_LEN];
    mavlink_heartbeat_t heartbeatMsg = {};
    uint32  length = 0;

    heartbeatMsg.type = MAV_TYPE_GENERIC;
    heartbeatMsg.autopilot = MAV_AUTOPILOT_GENERIC;
    heartbeatMsg.base_mode = 0; //MAV_MODE_FLAG_DECODE_POSITION_SAFETY + MAV_MODE_FLAG_DECODE_POSITION_CUSTOM_MODE;
    heartbeatMsg.custom_mode = 0;
    heartbeatMsg.system_status = MAV_STATE_ACTIVE;
    heartbeatMsg.mavlink_version = 1;

    mavlink_msg_heartbeat_encode(1, 1, &msg, &heartbeatMsg);
    length = mavlink_msg_to_send_buffer(buffer, &msg);

    if(SIMLINK_AppData.Socket != 0)
    {
        send(SIMLINK_AppData.Socket, (char *)buffer, length, 0);
    }

    status = 0;

    return status;
}


void SIMLINK_ListenerTaskMain(void)
{
	SIMLINK_AppData.ChildContinueFlag = TRUE;

	while(SIMLINK_AppData.ChildContinueFlag)
    {
	    char    buffer[SIMLINK_MAX_MESSAGE_SIZE] = {};
	    int32_t size = SIMLINK_MAX_MESSAGE_SIZE;

        size = read(SIMLINK_AppData.Socket, (char *)buffer, (size_t)size);

        if(size <= 0)
        {
            OS_TaskDelay(10);
        }
        else
        {
            mavlink_message_t msg;
            mavlink_status_t status;
            int32_t i = 0;

            for (i = 0; i < size; ++i)
            {
                if (mavlink_parse_char(MAVLINK_COMM_0, buffer[i], &msg, &status))
                {
                    switch(msg.msgid)
                    {
                        case MAVLINK_MSG_ID_HIL_RC_INPUTS_RAW:
                        {
                            printf("MAVLINK_MSG_ID_HIL_RC_INPUTS_RAW\n");

                            mavlink_hil_rc_inputs_raw_t         decodedMsg;

                            mavlink_msg_hil_rc_inputs_raw_decode(&msg, &decodedMsg);

                            break;
                        }

                        case MAVLINK_MSG_ID_HIL_SENSOR:
                        {
                            mavlink_hil_sensor_t  decodedMsg;

                            mavlink_msg_hil_sensor_decode(&msg, &decodedMsg);

                            if(decodedMsg.fields_updated & (uint32_t)SIMLINK_ACCEL)
                            {
                                int32 rc;

                                SIMLINK_AppData.AccelMsg[0].TimeUsec = decodedMsg.time_usec;
                                SIMLINK_AppData.AccelMsg[0].X = decodedMsg.xacc;
                                SIMLINK_AppData.AccelMsg[0].Y = decodedMsg.yacc;
                                SIMLINK_AppData.AccelMsg[0].Z = decodedMsg.zacc;

                            	rc = CVT_SetContent(SIMLINK_AppData.AccelContainer[0], &SIMLINK_AppData.AccelMsg[0], sizeof(SIMLINK_Accel_Msg_t));
                            	if(CVT_SUCCESS != rc)
                            	{
                                    (void) CFE_EVS_SendEvent(SIMLINK_CVT_ERR_EID, CFE_EVS_ERROR,
                                                             "Failed to set Accel %d container. (%li)",
                        									 i,
                                                             rc);
                            	}
                            }

                            if(decodedMsg.fields_updated & (uint32_t)SIMLINK_GYRO)
                            {
                                int32 rc;

                                SIMLINK_AppData.GyroMsg[0].TimeUsec = decodedMsg.time_usec;
                                SIMLINK_AppData.GyroMsg[0].X = decodedMsg.xgyro;
                                SIMLINK_AppData.GyroMsg[0].Y = decodedMsg.ygyro;
                                SIMLINK_AppData.GyroMsg[0].Z = decodedMsg.zgyro;

                            	rc = CVT_SetContent(SIMLINK_AppData.GyroContainer[0], &SIMLINK_AppData.GyroMsg[0], sizeof(SIMLINK_Gyro_Msg_t));
                            	if(CVT_SUCCESS != rc)
                            	{
                                    (void) CFE_EVS_SendEvent(SIMLINK_CVT_ERR_EID, CFE_EVS_ERROR,
                                                             "Failed to set Gyro %d container. (%li)",
                        									 i,
                                                             rc);
                            	}
                            }

                            if(decodedMsg.fields_updated & (uint32_t)SIMLINK_MAG)
                            {
                                int32 rc;

                                SIMLINK_AppData.MagMsg[0].TimeUsec = decodedMsg.time_usec;
                                SIMLINK_AppData.MagMsg[0].X = decodedMsg.xmag;
                                SIMLINK_AppData.MagMsg[0].Y = decodedMsg.ymag;
                                SIMLINK_AppData.MagMsg[0].Z = decodedMsg.zmag;

                            	rc = CVT_SetContent(SIMLINK_AppData.MagContainer[0], &SIMLINK_AppData.MagMsg[0], sizeof(SIMLINK_Mag_Msg_t));
                            	if(CVT_SUCCESS != rc)
                            	{
                                    (void) CFE_EVS_SendEvent(SIMLINK_CVT_ERR_EID, CFE_EVS_ERROR,
                                                             "Failed to set Mag %d container. (%li)",
                        									 i,
                                                             rc);
                            	}
                            }

                            if(decodedMsg.fields_updated & (uint32_t)SIMLINK_BARO)
                            {
                                int32 rc;

                                SIMLINK_AppData.BaroMsg[0].TimeUsec = decodedMsg.time_usec;
                                SIMLINK_AppData.BaroMsg[0].Pressure = decodedMsg.abs_pressure;
                                SIMLINK_AppData.BaroMsg[0].Temperature = decodedMsg.temperature;
                                SIMLINK_AppData.BaroMsg[0].BarometricAltitude = decodedMsg.pressure_alt;

                            	rc = CVT_SetContent(SIMLINK_AppData.BaroContainer[0], &SIMLINK_AppData.BaroMsg[0], sizeof(SIMLINK_Baro_Msg_t));
                            	if(CVT_SUCCESS != rc)
                            	{
                                    (void) CFE_EVS_SendEvent(SIMLINK_CVT_ERR_EID, CFE_EVS_ERROR,
                                                             "Failed to set Baro %d container. (%li)",
                        									 i,
                                                             rc);
                            	}
                            }

                            break;
                        }

                        case MAVLINK_MSG_ID_HIL_GPS:
                        {
                        	int32 rc;

                            mavlink_hil_gps_t decodedMsg;

                            mavlink_msg_hil_gps_decode(&msg, &decodedMsg);

                            SIMLINK_AppData.GpsMsg[0].TimeUsec = decodedMsg.time_usec;
                            SIMLINK_AppData.GpsMsg[0].Latitude = decodedMsg.lat;
                            SIMLINK_AppData.GpsMsg[0].Longitude = decodedMsg.lon;
                            SIMLINK_AppData.GpsMsg[0].Altitude = decodedMsg.alt;
                            SIMLINK_AppData.GpsMsg[0].EpH = decodedMsg.eph;
                            SIMLINK_AppData.GpsMsg[0].EpV = decodedMsg.epv;
                            SIMLINK_AppData.GpsMsg[0].Speed = decodedMsg.vel;
                            SIMLINK_AppData.GpsMsg[0].VelocityNorth = decodedMsg.vn;
                            SIMLINK_AppData.GpsMsg[0].VelocityEast = decodedMsg.ve;
                            SIMLINK_AppData.GpsMsg[0].VelocityDown = decodedMsg.vd;
                            SIMLINK_AppData.GpsMsg[0].COG = decodedMsg.cog;
                            SIMLINK_AppData.GpsMsg[0].FixType = (SIMLINK_GpsFixType_t)decodedMsg.fix_type;
                            SIMLINK_AppData.GpsMsg[0].SatellitesVisible = decodedMsg.satellites_visible;
                            SIMLINK_AppData.GpsMsg[0].Yaw = decodedMsg.yaw;

                        	rc = CVT_SetContent(SIMLINK_AppData.GpsContainer[0], &SIMLINK_AppData.GpsMsg[0], sizeof(SIMLINK_GPS_Msg_t));
                        	if(CVT_SUCCESS != rc)
                        	{
                                (void) CFE_EVS_SendEvent(SIMLINK_CVT_ERR_EID, CFE_EVS_ERROR,
                                                         "Failed to set GPS %d container. (0x%08X)",
                    									 i,
                                                         (unsigned int)rc);
                        	}

                            break;
                        }

                        case MAVLINK_MSG_ID_HIL_OPTICAL_FLOW:
                        {
                        	OS_printf("MAVLINK_MSG_ID_HIL_OPTICAL_FLOW\n");

                            mavlink_hil_optical_flow_t             decodedMsg;

                            mavlink_msg_hil_optical_flow_decode(&msg, &decodedMsg);

                            break;
                        }

                        case MAVLINK_MSG_ID_HIL_STATE_QUATERNION:
                        {
                            mavlink_hil_state_quaternion_t         decodedMsg;

                            mavlink_msg_hil_state_quaternion_decode(&msg, &decodedMsg);

                            break;
                        }

                        case MAVLINK_MSG_ID_VISION_POSITION_ESTIMATE:
                        {
                        	OS_printf("MAVLINK_MSG_ID_VISION_POSITION_ESTIMATE\n");

                            mavlink_vision_position_estimate_t     decodedMsg;

                            mavlink_msg_vision_position_estimate_decode(&msg, &decodedMsg);

                            break;
                        }

                        case MAVLINK_MSG_ID_DISTANCE_SENSOR:
                        {
                        	OS_printf("MAVLINK_MSG_ID_DISTANCE_SENSOR\n");

                            mavlink_distance_sensor_t     decodedMsg;

                            mavlink_msg_distance_sensor_decode(&msg, &decodedMsg);

                            break;
                        }

                        case MAVLINK_MSG_ID_HEARTBEAT:
                        {
                            SIMLINK_SendHeartbeat();

                            break;
                        }

                        case MAVLINK_MSG_ID_SYSTEM_TIME:
                        {
                        	OS_printf("MAVLINK_MSG_ID_SYSTEM_TIME\n");

                            break;
                        }

                        case MAVLINK_MSG_ID_SET_MODE:
                        {
                        	OS_printf("MAVLINK_MSG_ID_SET_MODE\n");

                            break;
                        }

                        default:
                        {
                            OS_printf("Received packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\n", msg.sysid, msg.compid, msg.len, msg.msgid);
                            break;
                        }
                    }
                }
            }
        }
    }
}



/************************/
/*  End of File Comment */
/************************/
