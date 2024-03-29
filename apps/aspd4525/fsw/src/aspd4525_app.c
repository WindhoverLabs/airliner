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
#include "aspd4525_version.h"
#include "aspd4525_custom.h"
#include "aspd4525_math.h"
#include "aspd4525_map.h"
#include "aspd4525_atm.h"
#include "aspd4525_config.h"

/************************************************************************
** Local Defines
*************************************************************************/
#undef STDIO_DEBUG

#if defined(STDIO_DEBUG)
//#include <stdio.h>
#endif

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

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(ASPD4525_AppData.EventTbl,
                               ASPD4525_EVT_CNT, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("ASPD4525 - Failed to register with EVS (0x%08X)\n", (unsigned int)iStatus);
    }

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

ASPD4525_InitPipe_Exit_Tag:
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

    /* Init housekeeping packet */
    CFE_SB_InitMsg(&ASPD4525_AppData.HkTlm,
                   ASPD4525_HK_TLM_MID, sizeof(ASPD4525_AppData.HkTlm), TRUE);

    CFE_SB_InitMsg(&ASPD4525_AppData.PX4_AirspeedMsg,
                   PX4_AIRSPEED_MID, sizeof(ASPD4525_AppData.PX4_AirspeedMsg), TRUE);

    ASPD4525_Custom_InitData();

    return (iStatus);
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
    boolean returnBool = FALSE;

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

    returnBool = ASPD4525_Custom_Init();
    if (FALSE == returnBool)
    {
        iStatus = -1;
        (void) CFE_EVS_SendEvent(ASPD4525_INIT_ERR_EID, CFE_EVS_ERROR,
                "Custom init failed");
        goto ASPD4525_InitApp_Exit_Tag;
    }

#if defined(STDIO_DEBUG)
    printf("ASPD4525 Custom Initialized\n");
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
            {
                ASPD4525_ProcessNewCmds();
                ASPD4525_ProcessNewData();

                ASPD4525_ReadDevice();

                ASPD4525_AppData.HkTlm.uPressureCount = ASPD4525_AppData.sPressureDiffCount;
                ASPD4525_AppData.HkTlm.uTemperatureCount = ASPD4525_AppData.sIemperatureCount;
                ASPD4525_AppData.HkTlm.uStatus = ASPD4525_AppData.ucStatus;

                if (ASPD4525_MAPS_STATUS_SUCCESS == ASPD4525_AppData.ucStatus) {
                    float airDensity;
                    ASPD4525_AppData.fPressureDiff = 
                        ASPD4525_MATH_GetDeltaPressure(
                            ASPD4525_AppData.ConfigTblPtr, 
                            ASPD4525_AppData.sPressureDiffCount
                        );

                    ASPD4525_AppData.fTemperature = 
                        ASPD4525_MATH_GetTemperature(
                            ASPD4525_AppData.ConfigTblPtr, 
                            ASPD4525_AppData.sIemperatureCount
                        );

                    switch (ASPD4525_AppData.ConfigTblPtr->uAirDensityCalculationMode) {
                        case ASPD4525_CONFIG_AIRDENSITY_NO_ALTITUDE_MODE:
                        {
                            float pressurePascals = 
                                ASPD4525_ATM_GetPressure(
                                    ASPD4525_AppData.ConfigTblPtr
                                );
                            
                            airDensity = 
                                ASPD4525_ATM_GetAirDensity(
                                    ASPD4525_AppData.ConfigTblPtr,
                                    ASPD4525_AppData.fTemperature,
                                    pressurePascals
                                );
                            #if defined(STDIO_DEBUG)
                            printf ("Came-> %ld %lf %lf\n", ASPD4525_AppData.ConfigTblPtr->uAirDensityCalculationMode, pressurePascals, airDensity);
                            #endif
                            break;
                        }
                        case ASPD4525_CONFIG_AIRDENSITY_ALTITUDE_MODE:
                        {
                            double altitudeMeters = 
                                ASPD4525_ATM_GetAltitude(
                                    ASPD4525_AppData.ConfigTblPtr
                                );
                            
                            airDensity = 
                                ASPD4525_ATM_GetAirDensityWithAlt(
                                    ASPD4525_AppData.ConfigTblPtr,
                                    altitudeMeters
                                );
                            #if defined(STDIO_DEBUG)
                            printf ("Came-> %ld %lf %lf\n", ASPD4525_AppData.ConfigTblPtr->uAirDensityCalculationMode, altitudeMeters, airDensity);
                            #endif
                            break;
                        }
                        case ASPD4525_CONFIG_AIRDENSITY_ALTITUDE_TEMPERATURE_MODE:
                        {
                            double altitudeMeters = 
                                ASPD4525_ATM_GetAltitude(
                                    ASPD4525_AppData.ConfigTblPtr
                                );
                            airDensity = 
                                ASPD4525_ATM_GetAirDensityWithAltTemp(
                                    ASPD4525_AppData.ConfigTblPtr,
                                    altitudeMeters,
                                    ASPD4525_AppData.fTemperature
                                );
                            #if defined(STDIO_DEBUG)
                            printf ("Came-> %ld %lf %lf\n", ASPD4525_AppData.ConfigTblPtr->uAirDensityCalculationMode, altitudeMeters, airDensity);
                            #endif
                            break;
                        }
                    }

                    ASPD4525_AppData.fTrueAirSpeedUnfiltered =
                        ASPD4525_ATM_GetAirSpeed
                        (
                            ASPD4525_AppData.fPressureDiff,
                            airDensity
                        );
                    
                    ASPD4525_AppData.fIndicatedAirSpeed =
                        ASPD4525_ATM_GetAirSpeed
                        (
                            ASPD4525_AppData.fPressureDiff,
                            AIR_DENSITY_SEA_LEVEL
                        );

                    #if defined(STDIO_DEBUG)
                    printf ("Airspeed-> %f, DeltaPressurePascals-> %f\n", ASPD4525_AppData.fTrueAirSpeedUnfiltered, ASPD4525_AppData.fPressureDiff);
                    #endif

                    /*Write out Housekeeping*/
                    ASPD4525_AppData.HkTlm.fTrueAirSpeedUnfiltered = ASPD4525_AppData.fTrueAirSpeedUnfiltered;
                    ASPD4525_AppData.HkTlm.fTemperature = ASPD4525_AppData.fTemperature;
                    ASPD4525_AppData.HkTlm.fIndicatedAirSpeed = ASPD4525_AppData.fIndicatedAirSpeed;
                    ASPD4525_AppData.fTrueAirSpeed = ASPD4525_AppData.fTrueAirSpeedUnfiltered;
                    ASPD4525_AppData.HkTlm.fTrueAirSpeed = ASPD4525_AppData.fTrueAirSpeed;

                    ASPD4525_AppData.HkTlm.fPressureMaximum_PSI = ASPD4525_AppData.ConfigTblPtr->fPressureMaximum_PSI;
                    ASPD4525_AppData.HkTlm.fPressureMinimum_PSI = ASPD4525_AppData.ConfigTblPtr->fPressureMinimum_PSI;
                    ASPD4525_AppData.HkTlm.fTemperatureMaximum_Celcius = ASPD4525_AppData.ConfigTblPtr->fTemperatureMaximum_Celcius;
                    ASPD4525_AppData.HkTlm.fTemperatureMinimum_Celcius = ASPD4525_AppData.ConfigTblPtr->fTemperatureMinimum_Celcius;

                    ASPD4525_AppData.HkTlm.uAirDensityCalculationMode = ASPD4525_AppData.ConfigTblPtr->uAirDensityCalculationMode;

                    /*Write out PX4 Telemetry*/
                    ASPD4525_AppData.PX4_AirspeedMsg.Timestamp = PX4LIB_GetPX4TimeUs();
                    ASPD4525_AppData.PX4_AirspeedMsg.IndicatedAirspeed = ASPD4525_AppData.fIndicatedAirSpeed;
                    ASPD4525_AppData.PX4_AirspeedMsg.TrueAirspeedUnfiltered = ASPD4525_AppData.fTrueAirSpeedUnfiltered;
                    ASPD4525_AppData.PX4_AirspeedMsg.TrueAirspeed = ASPD4525_AppData.fTrueAirSpeed;
                    ASPD4525_AppData.PX4_AirspeedMsg.AirTemperature = ASPD4525_AppData.fTemperature;
                    ASPD4525_AppData.PX4_AirspeedMsg.Confidence = 0;
                }

                /* TODO:  Add more code here to handle other things when app wakes up */

                /* The last thing to do at the end of this Wakeup cycle should be to
                 * automatically publish new output. */
                ASPD4525_SendOutData();
                break;
            }
            case ASPD4525_SEND_HK_MID:
            {
                ASPD4525_ReportHousekeeping();
                break;
            }
            default:
            {
                (void) CFE_EVS_SendEvent(ASPD4525_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid SCH msgId (0x%04X)", (unsigned short)MsgId);
                break;
            }
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

void ASPD4525_ReadDevice(void) {
    boolean returnBool = ASPD4525_Custom_Measure(&(ASPD4525_AppData.sPressureDiffCount), &(ASPD4525_AppData.sIemperatureCount), &(ASPD4525_AppData.ucStatus));

    #if defined(STDIO_DEBUG)
    printf("pressureDiff = 0x%04x, temperature = 0x%04x, status = 0x%02x\n", ASPD4525_AppData.sPressureDiffCount, ASPD4525_AppData.sIemperatureCount, ASPD4525_AppData.ucStatus);
    #endif
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
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void ASPD4525_ProcessNewCmds()
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

			case ASPD4525_MAN_CALIB_CC:
			{
				boolean sizeOk = ASPD4525_VerifyCmdLength(MsgPtr, sizeof(ASPD4525_ManCalibArgCmd_t));

				if (TRUE == sizeOk)
				{
					int32 status =0;
					ASPD4525_ManCalibArgCmd_t* manCalibArgCmdPtr = (ASPD4525_ManCalibArgCmd_t*) MsgPtr;

                    ASPD4525_AppData.ConfigTblPtr->fAirGasConstantR_SI = manCalibArgCmdPtr->fAirGasConstantR_SI;
                    ASPD4525_AppData.ConfigTblPtr->fAirMolarMass_SI = manCalibArgCmdPtr->fAirMolarMass_SI;
                    ASPD4525_AppData.ConfigTblPtr->fPressureMaximum_PSI = manCalibArgCmdPtr->fPressureMaximum_PSI;
                    ASPD4525_AppData.ConfigTblPtr->fPressureMinimum_PSI = manCalibArgCmdPtr->fPressureMinimum_PSI;
                    ASPD4525_AppData.ConfigTblPtr->fTemperatureMaximum_Celcius = manCalibArgCmdPtr->fTemperatureMaximum_Celcius;
                    ASPD4525_AppData.ConfigTblPtr->fTemperatureMinimum_Celcius = manCalibArgCmdPtr->fTemperatureMinimum_Celcius;
                    ASPD4525_AppData.ConfigTblPtr->fGravitationalAccereleration_SI = manCalibArgCmdPtr->fGravitationalAccereleration_SI;

                    ASPD4525_AppData.HkTlm.fPressureMaximum_PSI=ASPD4525_AppData.ConfigTblPtr->fPressureMaximum_PSI;
                    ASPD4525_AppData.HkTlm.fPressureMinimum_PSI=ASPD4525_AppData.ConfigTblPtr->fPressureMinimum_PSI;
                    ASPD4525_AppData.HkTlm.fTemperatureMaximum_Celcius=ASPD4525_AppData.ConfigTblPtr->fTemperatureMaximum_Celcius;
                    ASPD4525_AppData.HkTlm.fTemperatureMinimum_Celcius=ASPD4525_AppData.ConfigTblPtr->fTemperatureMinimum_Celcius;

					status = CFE_TBL_Modified(ASPD4525_AppData.ConfigTblHdl);
					if (CFE_SUCCESS!=status) {
						ASPD4525_AppData.HkTlm.usCmdErrCnt++;
						(void) CFE_EVS_SendEvent(
							ASPD4525_CONFIG_TABLE_ERR_EID, 
							CFE_EVS_ERROR,
							"Table Mod Error (0x%08X)", (unsigned int) status
						);
					} else {
						ASPD4525_AppData.HkTlm.usCmdCnt++;
					}
				}
				break;
			}

			case ASPD4525_LAB_CALIB_CC:
			{
				boolean sizeOk = ASPD4525_VerifyCmdLength(MsgPtr, sizeof(ASPD4525_LabCalibArgCmd_t));

				if (TRUE == sizeOk)
				{
					int32 status =0;
					ASPD4525_LabCalibArgCmd_t* labCalibArgCmdPtr = (ASPD4525_LabCalibArgCmd_t*) MsgPtr;

                    if (
                        ( labCalibArgCmdPtr->uPCountHigh > labCalibArgCmdPtr->uPCountLow ) &&
                        ( labCalibArgCmdPtr->fVelocityHigh_SI > labCalibArgCmdPtr->fVelocityLow_SI )
                    )
                    {
                        ASPD4525_MATH_CalibrateAirSpeedPressures(ASPD4525_AppData.ConfigTblPtr, labCalibArgCmdPtr);

                        #if defined(STDIO_DEBUG)
                        printf("Came Here 4: [%f, %f)\n", ASPD4525_AppData.ConfigTblPtr->fPressureMinimum_PSI, ASPD4525_AppData.ConfigTblPtr->fPressureMaximum_PSI);
                        #endif

                        ASPD4525_AppData.HkTlm.fPressureMaximum_PSI=ASPD4525_AppData.ConfigTblPtr->fPressureMaximum_PSI;
                        ASPD4525_AppData.HkTlm.fPressureMinimum_PSI=ASPD4525_AppData.ConfigTblPtr->fPressureMinimum_PSI;

                        status = CFE_TBL_Modified(ASPD4525_AppData.ConfigTblHdl);
                        if (CFE_SUCCESS!=status) {
                            ASPD4525_AppData.HkTlm.usCmdErrCnt++;
                            (void) CFE_EVS_SendEvent(
                                ASPD4525_CONFIG_TABLE_ERR_EID, 
                                CFE_EVS_ERROR,
                                "Table Mod Error (0x%08X)", (unsigned int) status
                            );
                        } else {
                            ASPD4525_AppData.HkTlm.usCmdCnt++;
                        }
                    }
				}
				break;
			}

			case ASPD4525_TEMP_CALIB_CC:
			{
				boolean sizeOk = ASPD4525_VerifyCmdLength(MsgPtr, sizeof(ASPD4525_TempCalibArgCmd_t));

				if (TRUE == sizeOk)
				{
					int32 status =0;
					ASPD4525_TempCalibArgCmd_t* tempCalibArgCmdPtr = (ASPD4525_TempCalibArgCmd_t*) MsgPtr;

                    if (
                        ( tempCalibArgCmdPtr->uTCountHigh > tempCalibArgCmdPtr->uTCountLow ) &&
                        ( tempCalibArgCmdPtr->fTemperatureHigh_Celcius > tempCalibArgCmdPtr->fTemperatureLow_Celcius )
                    )
                    {
                        ASPD4525_MATH_CalibrateTemperature(ASPD4525_AppData.ConfigTblPtr, tempCalibArgCmdPtr);

                        #if defined(STDIO_DEBUG)
                        printf("Came Here: [%f, %f)\n", ASPD4525_AppData.ConfigTblPtr->fTemperatureMinimum_Celcius, ASPD4525_AppData.ConfigTblPtr->fTemperatureMaximum_Celcius);
                        #endif

                        ASPD4525_AppData.HkTlm.fTemperatureMaximum_Celcius=ASPD4525_AppData.ConfigTblPtr->fTemperatureMaximum_Celcius;
                        ASPD4525_AppData.HkTlm.fTemperatureMinimum_Celcius=ASPD4525_AppData.ConfigTblPtr->fTemperatureMinimum_Celcius;

                        status = CFE_TBL_Modified(ASPD4525_AppData.ConfigTblHdl);
                        if (CFE_SUCCESS!=status) {
                            ASPD4525_AppData.HkTlm.usCmdErrCnt++;
                            (void) CFE_EVS_SendEvent(
                                ASPD4525_CONFIG_TABLE_ERR_EID, 
                                CFE_EVS_ERROR,
                                "Table Mod Error (0x%08X)", (unsigned int) status
                            );
                        } else {
                            ASPD4525_AppData.HkTlm.usCmdCnt++;
                        }
                    }
				}
				break;
			}

			case ASPD4525_PHYSICS_CALIB_CC:
			{
				boolean sizeOk = ASPD4525_VerifyCmdLength(MsgPtr, sizeof(ASPD4525_PhysicsCalibArgCmd_t));

				if (TRUE == sizeOk)
				{
					int32 status =0;
					ASPD4525_PhysicsCalibArgCmd_t* physicsCalibArgCmdPtr = (ASPD4525_PhysicsCalibArgCmd_t*) MsgPtr;

                    ASPD4525_AppData.ConfigTblPtr->fAirGasConstantR_SI = physicsCalibArgCmdPtr->fAirGasConstantR_SI;
                    ASPD4525_AppData.ConfigTblPtr->fAirMolarMass_SI = physicsCalibArgCmdPtr->fAirMolarMass_SI;
                    ASPD4525_AppData.ConfigTblPtr->fGravitationalAccereleration_SI = physicsCalibArgCmdPtr->fGravitationalAccereleration_SI;

                    #if defined(STDIO_DEBUG)
                    printf("Came Here: ASPD4525_PHYSICS_CALIB_CC\n");
                    #endif

					status = CFE_TBL_Modified(ASPD4525_AppData.ConfigTblHdl);
					if (CFE_SUCCESS!=status) {
						ASPD4525_AppData.HkTlm.usCmdErrCnt++;
						(void) CFE_EVS_SendEvent(
							ASPD4525_CONFIG_TABLE_ERR_EID, 
							CFE_EVS_ERROR,
							"Table Mod Error (0x%08X)", (unsigned int) status
						);
					} else {
						ASPD4525_AppData.HkTlm.usCmdCnt++;
					}
				}
				break;
			}

            case ASPD4525_AIR_COL_CALIB_CC:
            {
				boolean sizeOk = ASPD4525_VerifyCmdLength(MsgPtr, sizeof(ASPD4525_AirColCalibArgCmd_t));

				if (TRUE == sizeOk)
				{
					int32 status =0;
					ASPD4525_AirColCalibArgCmd_t* airColCalibArgCmdPtr = (ASPD4525_AirColCalibArgCmd_t*) MsgPtr;

                    ASPD4525_AppData.ConfigTblPtr->fAltitudeMeters_bs[0] = airColCalibArgCmdPtr->fh_b0;
                    ASPD4525_AppData.ConfigTblPtr->fAltitudeMeters_bs[1] = airColCalibArgCmdPtr->fh_b1;
                    ASPD4525_AppData.ConfigTblPtr->fAltitudeMeters_bs[2] = airColCalibArgCmdPtr->fh_b2;
                    ASPD4525_AppData.ConfigTblPtr->fAltitudeMeters_bs[3] = airColCalibArgCmdPtr->fh_b3;
                    ASPD4525_AppData.ConfigTblPtr->fAltitudeMeters_bs[4] = airColCalibArgCmdPtr->fh_b4;
                    ASPD4525_AppData.ConfigTblPtr->fAltitudeMeters_bs[5] = airColCalibArgCmdPtr->fh_b5;
                    ASPD4525_AppData.ConfigTblPtr->fAltitudeMeters_bs[6] = airColCalibArgCmdPtr->fh_b6;

                    ASPD4525_AppData.ConfigTblPtr->fRho_bs[0] = airColCalibArgCmdPtr->frho_b0;
                    ASPD4525_AppData.ConfigTblPtr->fRho_bs[1] = airColCalibArgCmdPtr->frho_b1;
                    ASPD4525_AppData.ConfigTblPtr->fRho_bs[2] = airColCalibArgCmdPtr->frho_b2;
                    ASPD4525_AppData.ConfigTblPtr->fRho_bs[3] = airColCalibArgCmdPtr->frho_b3;
                    ASPD4525_AppData.ConfigTblPtr->fRho_bs[4] = airColCalibArgCmdPtr->frho_b4;
                    ASPD4525_AppData.ConfigTblPtr->fRho_bs[5] = airColCalibArgCmdPtr->frho_b5;
                    ASPD4525_AppData.ConfigTblPtr->fRho_bs[6] = airColCalibArgCmdPtr->frho_b6;

                    ASPD4525_AppData.ConfigTblPtr->fTemp_bs[0] = airColCalibArgCmdPtr->fT_b0;
                    ASPD4525_AppData.ConfigTblPtr->fTemp_bs[1] = airColCalibArgCmdPtr->fT_b1;
                    ASPD4525_AppData.ConfigTblPtr->fTemp_bs[2] = airColCalibArgCmdPtr->fT_b2;
                    ASPD4525_AppData.ConfigTblPtr->fTemp_bs[3] = airColCalibArgCmdPtr->fT_b3;
                    ASPD4525_AppData.ConfigTblPtr->fTemp_bs[4] = airColCalibArgCmdPtr->fT_b4;
                    ASPD4525_AppData.ConfigTblPtr->fTemp_bs[5] = airColCalibArgCmdPtr->fT_b5;
                    ASPD4525_AppData.ConfigTblPtr->fTemp_bs[6] = airColCalibArgCmdPtr->fT_b6;

                    ASPD4525_AppData.ConfigTblPtr->fLapseRate_bs[0] = airColCalibArgCmdPtr->fL_b0;
                    ASPD4525_AppData.ConfigTblPtr->fLapseRate_bs[1] = airColCalibArgCmdPtr->fL_b1;
                    ASPD4525_AppData.ConfigTblPtr->fLapseRate_bs[2] = airColCalibArgCmdPtr->fL_b2;
                    ASPD4525_AppData.ConfigTblPtr->fLapseRate_bs[3] = airColCalibArgCmdPtr->fL_b3;
                    ASPD4525_AppData.ConfigTblPtr->fLapseRate_bs[4] = airColCalibArgCmdPtr->fL_b4;
                    ASPD4525_AppData.ConfigTblPtr->fLapseRate_bs[5] = airColCalibArgCmdPtr->fL_b5;
                    ASPD4525_AppData.ConfigTblPtr->fLapseRate_bs[6] = airColCalibArgCmdPtr->fL_b6;

                    #if defined(STDIO_DEBUG)
                    printf("Came Here: ASPD4525_AIR_COL_CALIB_CC\n");
                    #endif

					status = CFE_TBL_Modified(ASPD4525_AppData.ConfigTblHdl);
					if (CFE_SUCCESS!=status) {
						ASPD4525_AppData.HkTlm.usCmdErrCnt++;
						(void) CFE_EVS_SendEvent(
							ASPD4525_CONFIG_TABLE_ERR_EID, 
							CFE_EVS_ERROR,
							"Table Mod Error (0x%08X)", (unsigned int) status
						);
					} else {
						ASPD4525_AppData.HkTlm.usCmdCnt++;
					}
				}
				break;
            }
            
            case ASPD4525_SET_AIR_DENSITY_MODE_CC:
			{
				boolean sizeOk = ASPD4525_VerifyCmdLength(MsgPtr, sizeof(ASPD4525_AirDensityModeArgCmd_t));

				if (TRUE == sizeOk)
				{
					int32 status =0;
					ASPD4525_AirDensityModeArgCmd_t* airDensityModeArgCmdPtr = (ASPD4525_AirDensityModeArgCmd_t*) MsgPtr;

                    ASPD4525_AppData.ConfigTblPtr->uAirDensityCalculationMode = airDensityModeArgCmdPtr->uAirDensityCalculationMode;

                    #if defined(STDIO_DEBUG)
                    printf("Came Here: ASPD4525_PHYSICS_CALIB_CC\n");
                    #endif

					status = CFE_TBL_Modified(ASPD4525_AppData.ConfigTblHdl);
					if (CFE_SUCCESS!=status) {
						ASPD4525_AppData.HkTlm.usCmdErrCnt++;
						(void) CFE_EVS_SendEvent(
							ASPD4525_CONFIG_TABLE_ERR_EID, 
							CFE_EVS_ERROR,
							"Table Mod Error (0x%08X)", (unsigned int) status
						);
					} else {
						ASPD4525_AppData.HkTlm.usCmdCnt++;
					}
				}
				break;
			}


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
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&ASPD4525_AppData.PX4_AirspeedMsg);
    (void) CFE_SB_SendMsg((CFE_SB_Msg_t*)&ASPD4525_AppData.PX4_AirspeedMsg);
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
