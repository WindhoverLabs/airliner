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
#include <float.h>
#include <errno.h>

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
void SIMLINK_ProcessPwmOutputs(void);


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

    	status = CVT_GetContent(SIMLINK_AppData.AccelContainer[i], &updateCount, &SIMLINK_AppData.AccelMsg[i], &size);
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

    	status = CVT_GetContent(SIMLINK_AppData.MagContainer[i], &updateCount, &SIMLINK_AppData.MagMsg[i], &size);
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

    	status = CVT_GetContent(SIMLINK_AppData.BaroContainer[i], &updateCount, &SIMLINK_AppData.BaroMsg[i], &size);
    	if(CVT_SUCCESS != status)
    	{
            (void) CFE_EVS_SendEvent(SIMLINK_CVT_ERR_EID, CFE_EVS_ERROR,
                                     "Failed to get Baro %ld container. (%li)",
									 i,
                                     status);
	        goto end_of_function;
    	}
	}

	{
		uint32 updateCount = 0;
		uint32 size = sizeof(SIMLINK_PWM_Msg_t);
		status = CVT_GetContainer(SIMLINK_PWM_CONTAINER_NAME, sizeof(SIMLINK_PWM_Msg_t), &SIMLINK_AppData.PwmContainer);
		if(CVT_SUCCESS != status)
		{
			(void) CFE_EVS_SendEvent(SIMLINK_INIT_ERR_EID, CFE_EVS_ERROR,
									 "Failed to get PWM container. (%li)",
									 status);
			goto end_of_function;
		}

		status = CVT_GetContent(SIMLINK_AppData.PwmContainer, &updateCount, &SIMLINK_AppData.PwmMsg, &size);
		if(CVT_SUCCESS != status)
		{
			(void) CFE_EVS_SendEvent(SIMLINK_INIT_ERR_EID, CFE_EVS_ERROR,
									 "Failed to get PWM container. (%li)",
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
                SIMLINK_ProcessPwmOutputs();

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
                {
                    SIMLINK_ProcessNewAppCmds(CmdMsgPtr);
                    break;
                }

                default:
                {
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with this
                     *  pipe, but not handled in this switch-case.) */
                    OS_MutSemGive(SIMLINK_AppData.Mutex);
                    SIMLINK_AppData.HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(SIMLINK_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "Recvd invalid CMD msgId (0x%04X)", (unsigned short)CmdMsgId);
                    OS_MutSemTake(SIMLINK_AppData.Mutex);
                    break;
                }
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
                OS_MutSemTake(SIMLINK_AppData.Mutex);
                SIMLINK_AppData.HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(SIMLINK_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "Recvd NOOP cmd (%u), Version %d.%d.%d.%d",
                                  (unsigned int)uiCmdCode,
                                  SIMLINK_MAJOR_VERSION,
                                  SIMLINK_MINOR_VERSION,
                                  SIMLINK_REVISION,
                                  SIMLINK_MISSION_REV);
                OS_MutSemGive(SIMLINK_AppData.Mutex);
                break;

            case SIMLINK_RESET_CC:
                OS_MutSemTake(SIMLINK_AppData.Mutex);
                SIMLINK_AppData.HkTlm.usCmdCnt = 0;
                SIMLINK_AppData.HkTlm.usCmdErrCnt = 0;
                (void) CFE_EVS_SendEvent(SIMLINK_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "Recvd RESET cmd (%u)", (unsigned int)uiCmdCode);
                OS_MutSemGive(SIMLINK_AppData.Mutex);
                break;

            /* TODO:  Add code to process the rest of the SIMLINK commands here */

            default:
                OS_MutSemTake(SIMLINK_AppData.Mutex);
                SIMLINK_AppData.HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(SIMLINK_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid cmdId (%u)", (unsigned int)uiCmdCode);
                OS_MutSemGive(SIMLINK_AppData.Mutex);
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
    OS_MutSemTake(SIMLINK_AppData.Mutex);

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&SIMLINK_AppData.HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&SIMLINK_AppData.HkTlm);

    OS_MutSemGive(SIMLINK_AppData.Mutex);
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
            OS_MutSemTake(SIMLINK_AppData.Mutex);
            bResult = FALSE;
            CFE_SB_MsgId_t MsgId = CFE_SB_GetMsgId(MsgPtr);
            uint16 usCmdCode = CFE_SB_GetCmdCode(MsgPtr);

            (void) CFE_EVS_SendEvent(SIMLINK_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            SIMLINK_AppData.HkTlm.usCmdErrCnt++;
            OS_MutSemGive(SIMLINK_AppData.Mutex);
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

    /* Create mutex for shared data */
    status = OS_MutSemCreate(&SIMLINK_AppData.Mutex, SIMLINK_MUTEX_NAME, 0);
    if (status != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(SIMLINK_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create mutex (0x%08X)",
                                 (unsigned int)status);
        goto end_of_function;
    }

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
    	ssize_t bytesSent = 0;

    	while(length > 0)
    	{
            bytesSent = send(SIMLINK_AppData.Socket, (char *)buffer[bytesSent], length, 0);
            if(bytesSent < 0)
            {
                (void) CFE_EVS_SendEvent(SIMLINK_SOCKET_ERR_EID, CFE_EVS_ERROR,
                                         "Failed to send heartbeat (%d)",
                                         errno);
                goto end_of_function;
            }

            length = length - bytesSent;
    	}

        SIMLINK_AppData.HkTlm.DataOutMetrics.HeartbeatCount++;
    }

    status = 0;

end_of_function:

    return status;
}


void SIMLINK_ProcessPwmOutputs(void)
{
	int32 status;
	uint32 updateCount = 0;
	uint32 size = sizeof(SIMLINK_AppData.PwmMsg);

	status = CVT_GetContent(SIMLINK_AppData.PwmContainer, &updateCount, &SIMLINK_AppData.PwmMsg, &size);
	if(CVT_SUCCESS != status)
	{
		(void) CFE_EVS_SendEvent(SIMLINK_INIT_ERR_EID, CFE_EVS_ERROR,
								 "Failed to get PWM container. (%li)",
								 status);
	}
	else
	{
		if(updateCount != SIMLINK_AppData.PwmUpdateCount)
		{
			osalbool sendMsg = false;
                        osalbool armed   = false;

			SIMLINK_AppData.PwmUpdateCount = updateCount;

			mavlink_message_t mavlinkMessage = {};
			uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
			mavlink_hil_actuator_controls_t actuatorControlsMsg = {};
			uint32_t length = 0;

			for(uint32_t i = 0; i < SIMLINK_MAX_PWM_OUTPUTS; ++i)
			{
				if(i <= SIMLINK_PWM_CHANNEL_COUNT)
				{
					//actuatorControlsMsg.controls[i] = SIMLINK_PWM_To_Mavlink_Map(SIMLINK_AppData.PwmMsg.Channel[i], 1000, 3770, 0.0f, 1.0f);
					actuatorControlsMsg.controls[i] = SIMLINK_AppData.PwmMsg.Channel[i];

                    /* If any actuator control is non-zero. */
                    if(actuatorControlsMsg.controls[i] > 0)
                    {
                        /* Set the armed flag to true. */
                        armed = true;
                    }

					//if(actuatorControlsMsg.controls[i] >= FLT_EPSILON)
					//{
						sendMsg = true;
					//}

                                    // Scale everything else to [-1, 1]
                                    if(i==2 || i==7 || i==5 || i==6 | i==8 || i==3)
                                    {

//                                        inValue = (-A D + A Y + B C - B Y)/(C - D) and C!=D and A!=B
                                        //Reverse values from SEDPWM_Map_To_SIMLINK
                                        float  inMin = 1000;
                                        float outMax = 1.0f;
                                        float outMin = 0.0f;
                                        float inMax = 2000;
                                        float out = actuatorControlsMsg.controls[i];

                                        float inValue = ((-inMin * outMax) + (inMin * out) + (inMax * outMin) - (inMax*out))/(outMin - outMax);
                                        /**
                                         * @brief PWM_SIM_PWM_MIN_MAGIC
                                         *     /* Use the mapping equation:  Y = (X-A)/(B-A) * (D-C) + C */
                                        //float out = ((inValue - in_min) / (in_max - in_min)) * (out_max - out_min)
                                         //       + out_min;
                                        const uint16_t PWM_SIM_PWM_MIN_MAGIC = 1000;
                                        const uint16_t PWM_SIM_PWM_MAX_MAGIC = 2000;
                                        actuatorControlsMsg.controls[i] = inValue;
                                        const float pwm_center = (PWM_SIM_PWM_MAX_MAGIC + PWM_SIM_PWM_MIN_MAGIC) / 2.f;
                                        const float pwm_delta = (PWM_SIM_PWM_MAX_MAGIC - PWM_SIM_PWM_MIN_MAGIC) / 2.f;

                                        actuatorControlsMsg.controls[i] = (actuatorControlsMsg.controls[i] - pwm_center) / pwm_delta;
                                    }
				}
			}

			if(sendMsg)
			{
				actuatorControlsMsg.time_usec = 0;
				actuatorControlsMsg.flags = 0;

                if(armed == true)
                {
                    /* See MAV_MODE_FLAG_SAFETY_ARMED for this value. */
                    actuatorControlsMsg.mode = 128;
                }
                else
                {
                    actuatorControlsMsg.mode = 0;
                }

				mavlink_msg_hil_actuator_controls_encode(1, 1, &mavlinkMessage, &actuatorControlsMsg);
				length = mavlink_msg_to_send_buffer(buffer, &mavlinkMessage);

				if(SIMLINK_AppData.Socket != 0)
				{
			    	ssize_t bytesSent = 0;

			    	while(length > 0)
			    	{
			            bytesSent = send(SIMLINK_AppData.Socket, (char *)&buffer[bytesSent], length, 0);
			            if(bytesSent < 0)
			            {
			                (void) CFE_EVS_SendEvent(SIMLINK_SOCKET_ERR_EID, CFE_EVS_ERROR,
			                                         "Failed to send pwm message (%d)",
			                                         errno);
			                goto end_of_function;
			            }

			            length = length - bytesSent;
			    	}

			        SIMLINK_AppData.HkTlm.DataOutMetrics.PwmMsgCount++;
				}
			}
		}
	}

end_of_function:

    return;

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
                    OS_MutSemTake(SIMLINK_AppData.Mutex);

                    switch(msg.msgid)
                    {
                        case MAVLINK_MSG_ID_HIL_RC_INPUTS_RAW:
                        {
                            printf("MAVLINK_MSG_ID_HIL_RC_INPUTS_RAW\n");

                            mavlink_hil_rc_inputs_raw_t         decodedMsg;

                            mavlink_msg_hil_rc_inputs_raw_decode(&msg, &decodedMsg);

                            /* Update the metrics in HK. */
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilRcInputsRaw.Count++;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilRcInputsRaw.checksum = msg.checksum;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilRcInputsRaw.magic = msg.magic;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilRcInputsRaw.len = msg.len;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilRcInputsRaw.incompat_flags = msg.incompat_flags;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilRcInputsRaw.compat_flags = msg.compat_flags;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilRcInputsRaw.seq = msg.seq;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilRcInputsRaw.sysid = msg.sysid;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilRcInputsRaw.compid = msg.compid;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilRcInputsRaw.time_usec = decodedMsg.time_usec;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilRcInputsRaw.chan1_raw = decodedMsg.chan1_raw;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilRcInputsRaw.chan2_raw = decodedMsg.chan2_raw;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilRcInputsRaw.chan3_raw = decodedMsg.chan3_raw;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilRcInputsRaw.chan4_raw = decodedMsg.chan4_raw;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilRcInputsRaw.chan5_raw = decodedMsg.chan5_raw;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilRcInputsRaw.chan6_raw = decodedMsg.chan6_raw;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilRcInputsRaw.chan7_raw = decodedMsg.chan7_raw;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilRcInputsRaw.chan8_raw = decodedMsg.chan8_raw;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilRcInputsRaw.chan9_raw = decodedMsg.chan9_raw;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilRcInputsRaw.chan10_raw = decodedMsg.chan10_raw;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilRcInputsRaw.chan11_raw = decodedMsg.chan11_raw;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilRcInputsRaw.chan12_raw = decodedMsg.chan12_raw;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilRcInputsRaw.rssi = decodedMsg.rssi;

                            break;
                        }

                        case MAVLINK_MSG_ID_HIL_SENSOR:
                        {
                            mavlink_hil_sensor_t  decodedMsg;

                            mavlink_msg_hil_sensor_decode(&msg, &decodedMsg);

                            /* Update the metrics in HK. */
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilSensor.Count++;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilSensor.checksum = msg.checksum;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilSensor.magic = msg.magic;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilSensor.len = msg.len;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilSensor.incompat_flags = msg.incompat_flags;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilSensor.compat_flags = msg.compat_flags;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilSensor.seq = msg.seq;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilSensor.sysid = msg.sysid;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilSensor.compid = msg.compid;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilSensor.time_usec = decodedMsg.time_usec;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilSensor.fields_updated = decodedMsg.fields_updated;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilSensor.id = decodedMsg.id;

                            if(decodedMsg.fields_updated & (uint32_t)SIMLINK_ACCEL)
                            {
                                int32 rc;

                                SIMLINK_AppData.HkTlm.DataInMetrics.HilSensor.xacc = decodedMsg.xacc;
                                SIMLINK_AppData.HkTlm.DataInMetrics.HilSensor.yacc = decodedMsg.yacc;
                                SIMLINK_AppData.HkTlm.DataInMetrics.HilSensor.zacc = decodedMsg.zacc;

                                /* Update the outgoing message for consumers. */
                                SIMLINK_AppData.AccelMsg[0].TimeUsec = decodedMsg.time_usec;
                                SIMLINK_AppData.AccelMsg[0].X = decodedMsg.xacc;
                                SIMLINK_AppData.AccelMsg[0].Y = decodedMsg.yacc;
                                SIMLINK_AppData.AccelMsg[0].Z = decodedMsg.zacc;

                            	rc = CVT_SetContent(SIMLINK_AppData.AccelContainer[0], &SIMLINK_AppData.AccelMsg[0], sizeof(SIMLINK_Accel_Msg_t));
                            	if(CVT_SUCCESS == rc)
                            	{
                                    SIMLINK_AppData.HkTlm.DataOutMetrics.AccelMsgCount[0]++;
                            	}
                            	else
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

                                SIMLINK_AppData.HkTlm.DataInMetrics.HilSensor.xgyro = decodedMsg.xgyro;
                                SIMLINK_AppData.HkTlm.DataInMetrics.HilSensor.ygyro = decodedMsg.ygyro;
                                SIMLINK_AppData.HkTlm.DataInMetrics.HilSensor.zgyro = decodedMsg.zgyro;

                                /* Update the outgoing message for consumers. */
                                SIMLINK_AppData.GyroMsg[0].TimeUsec = decodedMsg.time_usec;
                                SIMLINK_AppData.GyroMsg[0].X = decodedMsg.xgyro;
                                SIMLINK_AppData.GyroMsg[0].Y = decodedMsg.ygyro;
                                SIMLINK_AppData.GyroMsg[0].Z = decodedMsg.zgyro;

                            	rc = CVT_SetContent(SIMLINK_AppData.GyroContainer[0], &SIMLINK_AppData.GyroMsg[0], sizeof(SIMLINK_Gyro_Msg_t));
                            	if(CVT_SUCCESS == rc)
                            	{
                                    SIMLINK_AppData.HkTlm.DataOutMetrics.GyroMsgCount[0]++;
                            	}
                            	else
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

                                SIMLINK_AppData.HkTlm.DataInMetrics.HilSensor.xmag = decodedMsg.xmag;
                                SIMLINK_AppData.HkTlm.DataInMetrics.HilSensor.ymag = decodedMsg.ymag;
                                SIMLINK_AppData.HkTlm.DataInMetrics.HilSensor.zmag = decodedMsg.zmag;

                                /* Update the outgoing message for consumers. */
                                SIMLINK_AppData.MagMsg[0].TimeUsec = decodedMsg.time_usec;
                                SIMLINK_AppData.MagMsg[0].X = decodedMsg.xmag;
                                SIMLINK_AppData.MagMsg[0].Y = decodedMsg.ymag;
                                SIMLINK_AppData.MagMsg[0].Z = decodedMsg.zmag;

                            	rc = CVT_SetContent(SIMLINK_AppData.MagContainer[0], &SIMLINK_AppData.MagMsg[0], sizeof(SIMLINK_Mag_Msg_t));
                            	if(CVT_SUCCESS == rc)
                            	{
                                    SIMLINK_AppData.HkTlm.DataOutMetrics.MagMsgCount[0]++;
                            	}
                            	else
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

                                SIMLINK_AppData.HkTlm.DataInMetrics.HilSensor.abs_pressure = decodedMsg.abs_pressure;
                                SIMLINK_AppData.HkTlm.DataInMetrics.HilSensor.pressure_alt = decodedMsg.pressure_alt;
                                SIMLINK_AppData.HkTlm.DataInMetrics.HilSensor.temperature = decodedMsg.temperature;

                                /* Update the outgoing message for consumers. */
                                SIMLINK_AppData.BaroMsg[0].TimeUsec = decodedMsg.time_usec;
                                SIMLINK_AppData.BaroMsg[0].Pressure = decodedMsg.abs_pressure;
                                SIMLINK_AppData.BaroMsg[0].Temperature = decodedMsg.temperature;
                                SIMLINK_AppData.BaroMsg[0].BarometricAltitude = decodedMsg.pressure_alt;

                            	rc = CVT_SetContent(SIMLINK_AppData.BaroContainer[0], &SIMLINK_AppData.BaroMsg[0], sizeof(SIMLINK_Baro_Msg_t));
                            	if(CVT_SUCCESS == rc)
                            	{
                                    SIMLINK_AppData.HkTlm.DataOutMetrics.BaroMsgCount[0]++;
                            	}
                            	else
                            	{
                                    (void) CFE_EVS_SendEvent(SIMLINK_CVT_ERR_EID, CFE_EVS_ERROR,
                                                             "Failed to set Baro %d container. (%li)",
                        									 i,
                                                             rc);
                            	}
                            }

                            if(decodedMsg.fields_updated & (uint32_t)SIMLINK_DIFF_PRESS)
                            {
                                int32 rc;

                                SIMLINK_AppData.HkTlm.DataInMetrics.HilSensor.diff_pressure = decodedMsg.diff_pressure;

                                /* Update the outgoing message for consumers. */
                                SIMLINK_AppData.BaroMsg[0].DiffPressure = decodedMsg.diff_pressure;

                            	rc = CVT_SetContent(SIMLINK_AppData.BaroContainer[0], &SIMLINK_AppData.BaroMsg[0], sizeof(SIMLINK_Baro_Msg_t));
                            	if(CVT_SUCCESS == rc)
                            	{
                                    SIMLINK_AppData.HkTlm.DataOutMetrics.DiffPressMsgCount[0]++;
                            	}
                            	else
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

                            /* Update the metrics in HK. */
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilGps.Count++;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilGps.checksum = msg.checksum;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilGps.magic = msg.magic;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilGps.len = msg.len;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilGps.incompat_flags = msg.incompat_flags;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilGps.compat_flags = msg.compat_flags;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilGps.seq = msg.seq;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilGps.sysid = msg.sysid;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilGps.compid = msg.compid;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilGps.time_usec = decodedMsg.time_usec;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilGps.lat = decodedMsg.lat;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilGps.lon = decodedMsg.lon;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilGps.alt = decodedMsg.alt;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilGps.eph = decodedMsg.eph;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilGps.epv = decodedMsg.epv;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilGps.vel = decodedMsg.vel;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilGps.vn = decodedMsg.vn;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilGps.ve = decodedMsg.ve;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilGps.vd = decodedMsg.vd;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilGps.cog = decodedMsg.cog;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilGps.fix_type = decodedMsg.fix_type;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilGps.satellites_visible = decodedMsg.satellites_visible;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilGps.id = decodedMsg.id;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilGps.yaw = decodedMsg.yaw;

                            /* Update the outgoing message for consumers. */
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
                        	if(CVT_SUCCESS == rc)
                        	{
                                SIMLINK_AppData.HkTlm.DataOutMetrics.GpsMsgCount[0]++;
                        	}
                        	else
                        	{
                                (void) CFE_EVS_SendEvent(SIMLINK_CVT_ERR_EID, CFE_EVS_ERROR,
                                                         "Failed to set GPS %d container. (0x%08X)",
                    									 i,
                                                         (unsigned int)rc);
                        	}

                            break;
                        }

                        case MAVLINK_MSG_ID_HIL_STATE_QUATERNION:
                        {
                            mavlink_hil_state_quaternion_t         decodedMsg;

                            mavlink_msg_hil_state_quaternion_decode(&msg, &decodedMsg);

                            /* Update the metrics in HK. */
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.Count++;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.checksum = msg.checksum;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.magic = msg.magic;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.len = msg.len;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.incompat_flags = msg.incompat_flags;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.compat_flags = msg.compat_flags;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.seq = msg.seq;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.sysid = msg.sysid;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.compid = msg.compid;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.time_usec = decodedMsg.time_usec;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.attitude_quaternion[0] = decodedMsg.attitude_quaternion[0];
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.attitude_quaternion[1] = decodedMsg.attitude_quaternion[1];
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.attitude_quaternion[2] = decodedMsg.attitude_quaternion[2];
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.attitude_quaternion[3] = decodedMsg.attitude_quaternion[3];
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.rollspeed = decodedMsg.rollspeed;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.pitchspeed = decodedMsg.pitchspeed;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.yawspeed = decodedMsg.yawspeed;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.lat = decodedMsg.lat;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.lon = decodedMsg.lon;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.alt = decodedMsg.alt;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.vx = decodedMsg.vx;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.vy = decodedMsg.vy;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.vz = decodedMsg.vz;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.ind_airspeed = decodedMsg.ind_airspeed;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.true_airspeed = decodedMsg.true_airspeed;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.xacc = decodedMsg.xacc;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.yacc = decodedMsg.yacc;
                            SIMLINK_AppData.HkTlm.DataInMetrics.HilStateQuaternion.zacc = decodedMsg.zacc;

                            break;
                        }

                        case MAVLINK_MSG_ID_HEARTBEAT:
                        {
                        	mavlink_heartbeat_t decodedMsg;

                        	mavlink_msg_heartbeat_decode(&msg, &decodedMsg);

                            /* Update the metrics in HK. */
                            SIMLINK_AppData.HkTlm.DataInMetrics.Heartbeat.Count++;
                            SIMLINK_AppData.HkTlm.DataInMetrics.Heartbeat.checksum = msg.checksum;
                            SIMLINK_AppData.HkTlm.DataInMetrics.Heartbeat.magic = msg.magic;
                            SIMLINK_AppData.HkTlm.DataInMetrics.Heartbeat.len = msg.len;
                            SIMLINK_AppData.HkTlm.DataInMetrics.Heartbeat.incompat_flags = msg.incompat_flags;
                            SIMLINK_AppData.HkTlm.DataInMetrics.Heartbeat.compat_flags = msg.compat_flags;
                            SIMLINK_AppData.HkTlm.DataInMetrics.Heartbeat.seq = msg.seq;
                            SIMLINK_AppData.HkTlm.DataInMetrics.Heartbeat.sysid = msg.sysid;
                            SIMLINK_AppData.HkTlm.DataInMetrics.Heartbeat.compid = msg.compid;
                            SIMLINK_AppData.HkTlm.DataInMetrics.Heartbeat.custom_mode = decodedMsg.custom_mode;
                            SIMLINK_AppData.HkTlm.DataInMetrics.Heartbeat.type = decodedMsg.type;
                            SIMLINK_AppData.HkTlm.DataInMetrics.Heartbeat.autopilot = decodedMsg.autopilot;
                            SIMLINK_AppData.HkTlm.DataInMetrics.Heartbeat.base_mode = decodedMsg.base_mode;
                            SIMLINK_AppData.HkTlm.DataInMetrics.Heartbeat.system_status = decodedMsg.system_status;
                            SIMLINK_AppData.HkTlm.DataInMetrics.Heartbeat.mavlink_version = decodedMsg.mavlink_version;

                            SIMLINK_SendHeartbeat();

                            break;
                        }

                        default:
                        {
                            (void) CFE_EVS_SendEvent(SIMLINK_MAVLINK_INFO_EID, CFE_EVS_ERROR,
                                                     "Received unprocessed Mavlink message. SYS: %d, COMP: %d, LEN: %d, MSG ID: %d",
                									 msg.sysid, msg.compid, msg.len, msg.msgid);
                            break;
                        }
                    }

                    OS_MutSemGive(SIMLINK_AppData.Mutex);
                }
            }
        }
    }
}



/************************/
/*  End of File Comment */
/************************/
