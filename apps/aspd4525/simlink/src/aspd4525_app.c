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

#include "aspd4525_app.h"
#include "aspd4525_msg.h"
#include "aspd4525_msgids.h"
#include "aspd4525_platform_cfg.h"
#include "aspd4525_version.h"
#include "aspd4525_math.h"
#include "sedlib.h"
#include "simlink.h"
#include "cvt_lib.h"

#define STDIO_DEBUG

#if defined(STDIO_DEBUG)
#include <stdio.h>
#endif

/*MSR_TODO Remove this*/
#include <stdio.h>


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
ASPD4525_AppData_t  ASPD4525_AppData;
ASPD4525_AppI2CData_t ASPD4525_AppI2CData;

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Definitions
*************************************************************************/
int32 ASPD4525_InitSedPipes(void);

SEDLIB_ReturnCode_t ASPD4525_SED_SendResponse(void);
SEDLIB_ReturnCode_t ASPD4525_SED_ParseCommand(void);


SEDLIB_ReturnCode_t ASPD4525_SED_SendResponse(void)
{
    SEDLIB_ReturnCode_t returnCode = SEDLIB_OK;

    ASPD4525_AppI2CData.TransferResp.Response[0].Status = SEDLIB_OK;
    ASPD4525_AppI2CData.TransferResp.Response[0].Count++;
    returnCode = SEDLIB_SendMsg(ASPD4525_AppI2CData.StatusPortHandle, 
                                (CFE_SB_MsgPtr_t)&ASPD4525_AppI2CData.TransferResp);

    return returnCode;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 ASPD4525_InitEvent()
{
    int32  iStatus=CFE_SUCCESS;
    int32  ind = 0;

    /* Initialize the event filter table.
     * Note: 0 is the CFE_EVS_NO_FILTER mask and event 0 is reserved (not used) */
    memset((void*)ASPD4525_AppData.EventTbl, 0x00, sizeof(ASPD4525_AppData.EventTbl));

    /* TODO: Choose the events you want to filter.  CFE_EVS_MAX_EVENT_FILTERS
     * limits the number of filters per app.  An explicit CFE_EVS_NO_FILTER 
     * (the default) has been provided as an example. */
    ASPD4525_AppData.EventTbl[  ind].EventID = ASPD4525_RESERVED_EID;
    ASPD4525_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    ASPD4525_AppData.EventTbl[  ind].EventID = ASPD4525_INF_EID;
    ASPD4525_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    ASPD4525_AppData.EventTbl[  ind].EventID = ASPD4525_CONFIG_TABLE_ERR_EID;
    ASPD4525_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    ASPD4525_AppData.EventTbl[  ind].EventID = ASPD4525_CDS_ERR_EID;
    ASPD4525_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    ASPD4525_AppData.EventTbl[  ind].EventID = ASPD4525_PIPE_ERR_EID;
    ASPD4525_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    ASPD4525_AppData.EventTbl[  ind].EventID = ASPD4525_MSGID_ERR_EID;
    ASPD4525_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    ASPD4525_AppData.EventTbl[  ind].EventID = ASPD4525_MSGLEN_ERR_EID;
    ASPD4525_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    ASPD4525_AppData.EventTbl[  ind].EventID = ASPD4525_CVT_ERR_EID;
    ASPD4525_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    ASPD4525_AppData.EventTbl[  ind].EventID = ASPD4525_SEDLIB_ERR_EID;
    ASPD4525_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(ASPD4525_AppData.EventTbl,
                               ASPD4525_EVT_CNT, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("ASPD4525 - Failed to register with EVS (0x%08X)\n", (unsigned int)iStatus);
    }

    #if defined(STDIO_DEBUG)
    printf("ASPD4525_InitEvent\n");
    #endif

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 ASPD4525_InitPipe()
{
    int32  iStatus=CFE_SUCCESS;
    osalbool hasEvents = TRUE;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&ASPD4525_AppData.SchPipeId,
                                 ASPD4525_SCH_PIPE_DEPTH,
                                 ASPD4525_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(ASPD4525_WAKEUP_MID, ASPD4525_AppData.SchPipeId, CFE_SB_Default_Qos, ASPD4525_SCH_PIPE_WAKEUP_RESERVED);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(ASPD4525_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "Sch Pipe failed to subscribe to ASPD4525_WAKEUP_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto ASPD4525_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(ASPD4525_SEND_HK_MID, ASPD4525_AppData.SchPipeId, CFE_SB_Default_Qos, ASPD4525_SCH_PIPE_SEND_HK_RESERVED);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(ASPD4525_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "CMD Pipe failed to subscribe to ASPD4525_SEND_HK_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto ASPD4525_InitPipe_Exit_Tag;
        }

    }
    else
    {
        (void) CFE_EVS_SendEvent(ASPD4525_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create SCH pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto ASPD4525_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&ASPD4525_AppData.CmdPipeId,
                                 ASPD4525_CMD_PIPE_DEPTH,
                                 ASPD4525_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(ASPD4525_CMD_MID, ASPD4525_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(ASPD4525_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "CMD Pipe failed to subscribe to ASPD4525_CMD_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto ASPD4525_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(ASPD4525_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create CMD pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto ASPD4525_InitPipe_Exit_Tag;
    }

    /* Init data pipe and subscribe to messages on the data pipe */
    iStatus = CFE_SB_CreatePipe(&ASPD4525_AppData.DataPipeId,
                                 ASPD4525_DATA_PIPE_DEPTH,
                                 ASPD4525_DATA_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* TODO:  Add CFE_SB_Subscribe() calls for other apps' output data here.
        **
        ** Examples:
        **     CFE_SB_Subscribe(GNCEXEC_OUT_DATA_MID, ASPD4525_AppData.DataPipeId);
        */
    }
    else
    {
        (void) CFE_EVS_SendEvent(ASPD4525_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create Data pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto ASPD4525_InitPipe_Exit_Tag;
    }

    iStatus = ASPD4525_InitCVT();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(ASPD4525_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init CVT (0x%08X)",
                                 (unsigned int)iStatus);
        goto ASPD4525_InitPipe_Exit_Tag;
    }

    iStatus = ASPD4525_InitSedPipes();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(ASPD4525_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init SED pipes (0x%08X)",
                                 (unsigned int)iStatus);
        goto ASPD4525_InitPipe_Exit_Tag;
    }

    #if defined(STDIO_DEBUG)
    printf("ASPD4525_InitPipe\n");
    #endif


ASPD4525_InitPipe_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(ASPD4525_INIT_INF_EID, CFE_EVS_INFORMATION,
                                 "Initialized Pipes.");
    }
    else
    {
        if (hasEvents == 1)
        {
            (void) CFE_EVS_SendEvent(ASPD4525_INIT_ERR_EID, CFE_EVS_ERROR, "Application failed to initialize");
        }
        else
        {
            (void) CFE_ES_WriteToSysLog("ASPD4525 - Application failed to initialize\n");
        }
    }
    return (iStatus);
}
    

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 ASPD4525_InitData()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init input data */
    memset((void*)&ASPD4525_AppData.InData, 0x00, sizeof(ASPD4525_AppData.InData));

    /* Init output data */
    memset((void*)&ASPD4525_AppData.OutData, 0x00, sizeof(ASPD4525_AppData.OutData));
    CFE_SB_InitMsg(&ASPD4525_AppData.OutData,
                   ASPD4525_OUT_DATA_MID, sizeof(ASPD4525_AppData.OutData), TRUE);

    /* Init housekeeping packet */
    memset((void*)&ASPD4525_AppData.HkTlm, 0x00, sizeof(ASPD4525_AppData.HkTlm));
    CFE_SB_InitMsg(&ASPD4525_AppData.HkTlm,
                   ASPD4525_HK_TLM_MID, sizeof(ASPD4525_AppData.HkTlm), TRUE);

    #if defined(STDIO_DEBUG)
    printf("ASPD4525_InitData\n");
    #endif

    return (iStatus);
}

int32 ASPD4525_InitCVT(void)
{
    int32 status;

    for(uint32 i = 0; i < ASPD4525_DEVICE_COUNT; ++i)
    {
        char name[CVT_CONTAINER_NAME_MAX_LENGTH];
        uint32 size;

        /* Baro */
        size = sizeof(SIMLINK_Baro_Msg_t);
        sprintf(name, SIMLINK_BARO_CONTAINER_NAME_SPEC, i);
        status = CVT_GetContainer(name, sizeof(SIMLINK_Baro_Msg_t), &ASPD4525_AppData.SimlinkBaroContainer[i]);
        if(CVT_SUCCESS != status)
        {
            (void) CFE_EVS_SendEvent(ASPD4525_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "Failed to get BARO %ld container. (%li)",
                                     i,
                                     status);
            goto end_of_function;
        }

        status = 
            CVT_GetContent(
                ASPD4525_AppData.SimlinkBaroContainer[i], 
                &ASPD4525_AppData.HkTlm.SimlinkBaroUpdateCount[i], 
                &ASPD4525_AppData.HkTlm.SimlinkBaroMsg[i], 
                &size
            );
        if(CVT_SUCCESS != status)
        {
            (void) CFE_EVS_SendEvent(ASPD4525_CVT_ERR_EID, CFE_EVS_ERROR,
                                     "Failed to get Baro %ld container. (%li)",
                                     i,
                                     status);
            goto end_of_function;
        }
    }

    #if defined(STDIO_DEBUG)
    printf("ASPD4525_InitCVT\n");
    #endif

end_of_function:

    return status;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* ASPD4525 initialization                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 ASPD4525_InitApp()
{
    int32  iStatus   = CFE_SUCCESS;
    int8   hasEvents = 0;

    iStatus = ASPD4525_InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("ASPD4525 - Failed to init events (0x%08X)\n", (unsigned int)iStatus);
        goto ASPD4525_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = ASPD4525_InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(ASPD4525_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init pipes (0x%08X)",
                                 (unsigned int)iStatus);
        goto ASPD4525_InitApp_Exit_Tag;
    }

    iStatus = ASPD4525_InitData();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(ASPD4525_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init data (0x%08X)",
                                 (unsigned int)iStatus);
        goto ASPD4525_InitApp_Exit_Tag;
    }

    iStatus = ASPD4525_InitConfigTbl();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(ASPD4525_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init config tables (0x%08X)",
                                 (unsigned int)iStatus);
        goto ASPD4525_InitApp_Exit_Tag;
    }

    iStatus = ASPD4525_InitCdsTbl();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(ASPD4525_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init CDS table (0x%08X)",
                                 (unsigned int)iStatus);
        goto ASPD4525_InitApp_Exit_Tag;
    }

    #if defined(STDIO_DEBUG)
    printf("ASPD4525_InitApp\n");
    #endif

ASPD4525_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(ASPD4525_INIT_INF_EID, CFE_EVS_INFORMATION,
                                 "Initialized.  Version %d.%d.%d.%d",
                                 ASPD4525_MAJOR_VERSION,
                                 ASPD4525_MINOR_VERSION,
                                 ASPD4525_REVISION,
                                 ASPD4525_MISSION_REV);
    }
    else
    {
        if (hasEvents == 1)
        {
            (void) CFE_EVS_SendEvent(ASPD4525_INIT_ERR_EID, CFE_EVS_ERROR, "Application failed to initialize");
        }
        else
        {
            (void) CFE_ES_WriteToSysLog("ASPD4525 - Application failed to initialize\n");
        }
    }

    return (iStatus);
}


int32 ASPD4525_InitSedPipes(void)
{
    int32 iStatus = SEDLIB_OK;

    CFE_PSP_MemSet(&ASPD4525_AppI2CData, 0, sizeof(ASPD4525_AppI2CData));

    /* Get a handle to the status pipe. */
    iStatus = SEDLIB_GetPipe(ASPD4525_SED_STATUS_PIPE_NAME,
                             sizeof(IIC_TransferResponse_t),
                             &ASPD4525_AppI2CData.StatusPortHandle);
    if(iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(ASPD4525_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to initialize MAG status MsgPort. (0x%08lX)",
                                 iStatus);
        goto end_of_function;
    }

    /* Get a handle to the command pipe. */
    iStatus = SEDLIB_GetPipe(ASPD4525_SED_CMD_PIPE_NAME,
                             sizeof(IIC_TransferCmd_t),
                             &ASPD4525_AppI2CData.CmdPortHandle);
    if(iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(ASPD4525_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to initialize MAG command MsgPort. (0x%08lX)",
                                 iStatus);
        goto end_of_function;
    }

    /* Initialize response message. */
    CFE_SB_InitMsg(&ASPD4525_AppI2CData.TransferResp,
                   IIC_RESPONSE_MSG_ID,
                   sizeof(ASPD4525_AppI2CData.TransferResp),
                   TRUE);

    /* Set version number. */
    ASPD4525_AppI2CData.TransferResp.Version = 1;

    #if defined(STDIO_DEBUG)
    printf("ASPD4525_InitSedPipes\n");
    #endif


end_of_function:

    return iStatus;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 ASPD4525_RcvMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(ASPD4525_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, ASPD4525_AppData.SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(ASPD4525_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
	{
            case ASPD4525_WAKEUP_MID:
                printf("ASPD4525 woke up.\n");
                ASPD4525_ProcessNewCmds();
                ASPD4525_ProcessNewData();

                /* TODO:  Add more code here to handle other things when app wakes up */

                /* The last thing to do at the end of this Wakeup cycle should be to
                 * automatically publish new output. */
                ASPD4525_SendOutData();
                break;

            case ASPD4525_SEND_HK_MID:
                printf("ASPD4525 sending hk.\n");
                ASPD4525_ReportHousekeeping();
                break;

            default:
                (void) CFE_EVS_SendEvent(ASPD4525_MSGID_ERR_EID, CFE_EVS_ERROR,
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
        (void) CFE_EVS_SendEvent(ASPD4525_PIPE_ERR_EID, CFE_EVS_ERROR,
			  "SB pipe read error (0x%08X), app will exit", (unsigned int)iStatus);
        ASPD4525_AppData.uiRunStatus= CFE_ES_APP_ERROR;
    }

    return (iStatus);
}


/**
 * @brief This function generates the number to be written out to APP from simulated device
 * 
 * @param fTemperature Temperature in Kelvin
 * @param fDiffPressure Pressure Differential in hPa
 * @param status 2 bit Device Status
 * @return uint32 The 4 bytes to be written out over "i2c"
 */
uint32 ASPD4525_PrepareOutgoingData(
    float fTemperature,
    float fDiffPressure,
    uint8 status
) {
    uint32 returnData = 0;
    /* MSR_TODO: finish this part of the code */
    printf("MSR_TODO: Need to finish this code\n");
    return returnData;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Data                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void ASPD4525_ProcessNewData()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   DataMsgPtr=NULL;
    CFE_SB_MsgId_t  DataMsgId;

    for (uint32 i = 0; i < ASPD4525_DEVICE_COUNT; i++)
    {
        char name[CVT_CONTAINER_NAME_MAX_LENGTH];
        uint32 size;

        /* Gazebo Baro*/
        size = sizeof(SIMLINK_Baro_Msg_t);
        iStatus = CVT_GetContent(ASPD4525_AppData.SimlinkBaroContainer[i], &ASPD4525_AppData.HkTlm.SimlinkBaroUpdateCount[i], &ASPD4525_AppData.HkTlm.SimlinkBaroMsg[i], &size);
        if(CVT_SUCCESS != iStatus)
        {
            (void) CFE_EVS_SendEvent(ASPD4525_CVT_ERR_EID, CFE_EVS_ERROR,
                                     "Failed to get GYRO %ld container. (%ui)",
                                     i,
                                     iStatus);
            goto end_of_function;
        }

        ASPD4525_AppData.HkTlm.Status_OUT = 1;
        ASPD4525_AppData.HkTlm.Temperature_OUT = ASPD4525_AppData.HkTlm.SimlinkBaroMsg[0].Temperature;
        ASPD4525_AppData.HkTlm.DiffPressure_OUT = ASPD4525_AppData.HkTlm.SimlinkBaroMsg[0].DiffPressure;

        printf("Gazebo Temp: %f; Gazebo DiffPress: %f\n", ASPD4525_AppData.HkTlm.Temperature_OUT, ASPD4525_AppData.HkTlm.DiffPressure_OUT);

        ASPD4525_AppData.HkTlm.ASPD4525_Simlink_OUT = 
            ASPD4525_PrepareOutgoingData(
                ASPD4525_AppData.HkTlm.Temperature_OUT,
                ASPD4525_AppData.HkTlm.DiffPressure_OUT,
                ASPD4525_AppData.HkTlm.Status_OUT
            );
    }

    /* Process telemetry messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&DataMsgPtr, ASPD4525_AppData.DataPipeId, CFE_SB_POLL);
        if (iStatus == CFE_SUCCESS)
        {
            DataMsgId = CFE_SB_GetMsgId(DataMsgPtr);
            switch (DataMsgId)
            {
                /* TODO:  Add code to process all subscribed data here
                **
                ** Example:
                **     case NAV_OUT_DATA_MID:
                **         ASPD4525_ProcessNavData(DataMsgPtr);
                **         break;
                */

                default:
                    (void) CFE_EVS_SendEvent(ASPD4525_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            (void) CFE_EVS_SendEvent(ASPD4525_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "Data pipe read error (0x%08X)", (unsigned int)iStatus);
            ASPD4525_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
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

void ASPD4525_ProcessNewCmds(void)
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr=NULL;
    CFE_SB_MsgId_t  CmdMsgId;

    /* Process command messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, ASPD4525_AppData.CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case ASPD4525_CMD_MID:
                    ASPD4525_ProcessNewAppCmds(CmdMsgPtr);
                    break;

                /* TODO:  Add code to process other subscribed commands here
                **
                ** Example:
                **     case CFE_TIME_DATA_CMD_MID:
                **         ASPD4525_ProcessTimeDataCmd(CmdMsgPtr);
                **         break;
                */

                default:
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with this
                     *  pipe, but not handled in this switch-case.) */
                    ASPD4525_AppData.HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(ASPD4525_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            (void) CFE_EVS_SendEvent(ASPD4525_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08X)", (unsigned int)iStatus);
            ASPD4525_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process ASPD4525 Commands                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void ASPD4525_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case ASPD4525_NOOP_CC:
                ASPD4525_AppData.HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(ASPD4525_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "Recvd NOOP cmd (%u), Version %d.%d.%d.%d",
                                  (unsigned int)uiCmdCode,
                                  ASPD4525_MAJOR_VERSION,
                                  ASPD4525_MINOR_VERSION,
                                  ASPD4525_REVISION,
                                  ASPD4525_MISSION_REV);
                break;

            case ASPD4525_RESET_CC:
                ASPD4525_AppData.HkTlm.usCmdCnt = 0;
                ASPD4525_AppData.HkTlm.usCmdErrCnt = 0;
                (void) CFE_EVS_SendEvent(ASPD4525_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "Recvd RESET cmd (%u)", (unsigned int)uiCmdCode);
                break;

            /* TODO:  Add code to process the rest of the ASPD4525 commands here */

            default:
                ASPD4525_AppData.HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(ASPD4525_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid cmdId (%u)", (unsigned int)uiCmdCode);
                break;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send ASPD4525 Housekeeping                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void ASPD4525_ReportHousekeeping()
{
    /* TODO:  Add code to update housekeeping data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&ASPD4525_AppData.HkTlm);
    int32 iStatus = CFE_SB_SendMsg((CFE_SB_Msg_t*)&ASPD4525_AppData.HkTlm);
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

void ASPD4525_SendOutData()
{
    /* TODO:  Add code to update output data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&ASPD4525_AppData.OutData);
    int32 iStatus = CFE_SB_SendMsg((CFE_SB_Msg_t*)&ASPD4525_AppData.OutData);
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

boolean ASPD4525_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
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

            (void) CFE_EVS_SendEvent(ASPD4525_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            ASPD4525_AppData.HkTlm.usCmdErrCnt++;
        }
    }

    return (bResult);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* ASPD4525 application entry point and main process loop               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void ASPD4525_AppMain()
{
    /* Register the application with Executive Services */
    ASPD4525_AppData.uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("ASPD4525 - Failed to register the app (0x%08X)\n", (unsigned int)iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(ASPD4525_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = ASPD4525_InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(ASPD4525_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(ASPD4525_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(ASPD4525_MAIN_TASK_PERF_ID);
    }
    else
    {
        ASPD4525_AppData.uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&ASPD4525_AppData.uiRunStatus) == TRUE)
    {
        int32 iStatus = ASPD4525_RcvMsg(ASPD4525_SCH_PIPE_PEND_TIME);
        if (iStatus != CFE_SUCCESS)
        {
            /* TODO: Decide what to do for other return values in ASPD4525_RcvMsg(). */
        }
        /* TODO: This is only a suggestion for when to update and save CDS table.
        ** Depends on the nature of the application, the frequency of update
        ** and save can be more or less independently.
        */
        ASPD4525_UpdateCdsTbl();
        ASPD4525_SaveCdsTbl();

        iStatus = ASPD4525_AcquireConfigPointers();
        if(iStatus != CFE_SUCCESS)
        {
            /* We apparently tried to load a new table but failed.  Terminate the application. */
            ASPD4525_AppData.uiRunStatus = CFE_ES_APP_ERROR;
        }
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(ASPD4525_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(ASPD4525_AppData.uiRunStatus);
}

/************************/
/*  End of File Comment */
/************************/
