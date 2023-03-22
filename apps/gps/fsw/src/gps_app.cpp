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
#include "gps_custom.h"
#include "gps_app.h"
#include "gps_msg.h"
#include "gps_version.h"
#include "gps_event_driven.h"
#include "px4lib_msgids.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Instantiate the application object.                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
GPS oGPS;



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Default constructor.                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
GPS::GPS()
{

}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Destructor constructor.                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
GPS::~GPS()
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables.                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 GPS::InitEvent()
{
    int32  iStatus         = CFE_SUCCESS;
    int32  ind             = 0;
    int32 customEventCount = 0;
    
    CFE_EVS_BinFilter_t   EventTbl[CFE_EVS_MAX_EVENT_FILTERS];

    /* Initialize the event filter table.
     * Note: 0 is the CFE_EVS_NO_FILTER mask and event 0 is reserved (not used) */
    memset(EventTbl, 0x00, sizeof(EventTbl));
    
    /* TODO: Choose the events you want to filter.  CFE_EVS_MAX_EVENT_FILTERS
     * limits the number of filters per app.  An explicit CFE_EVS_NO_FILTER 
     * (the default) has been provided as an example. */
    EventTbl[  ind].EventID = GPS_RESERVED_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = GPS_READ_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_FIRST_16_STOP;

    /* Add custom events to the filter table */
    customEventCount = GPS_Custom_Init_EventFilters(ind, EventTbl);
    
    if(-1 == customEventCount)
    {
        iStatus = CFE_EVS_APP_FILTER_OVERLOAD;
        (void) CFE_ES_WriteToSysLog("Failed to init custom event filters (0x%08X)\n", (unsigned int)iStatus);
        goto end_of_function;
    }

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(EventTbl, (ind + customEventCount), CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("MPU9250 - Failed to register with EVS (0x%08lX)\n", iStatus);
    }

end_of_function:

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 GPS::InitPipe()
{
    int32  iStatus = CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&SchPipeId,
            GPS_SCH_PIPE_DEPTH,
            GPS_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(GPS_READ_SENSOR_MID, SchPipeId, CFE_SB_Default_Qos, GPS_READ_SENSOR_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(GPS_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "Sch Pipe failed to subscribe to GPS_READ_SENSOR_MID. (0x%08lX)",
                    iStatus);
            goto GPS_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(GPS_SEND_HK_MID, SchPipeId, CFE_SB_Default_Qos, GPS_SEND_HK_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(GPS_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                     "CMD Pipe failed to subscribe to GPS_SEND_HK_MID. (0x%08X)",
                     (unsigned int)iStatus);
            goto GPS_InitPipe_Exit_Tag;
        }
        //iStatus = CFE_SB_SubscribeEx(PX4_GPS_INJECT_DATA_MID, SchPipeId, CFE_SB_Default_Qos, 1);
        //if (iStatus != CFE_SUCCESS)
        //{
            //(void) CFE_EVS_SendEvent(GPS_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                     //"CMD Pipe failed to subscribe to PX4_GPS_INJECT_DATA_MID. (0x%08lX)",
                     //iStatus);
            //goto GPS_InitPipe_Exit_Tag;
        //}
    }
    else
    {
        (void) CFE_EVS_SendEvent(GPS_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
             "Failed to create SCH pipe (0x%08lX)",
             iStatus);
        goto GPS_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&CmdPipeId,
            GPS_CMD_PIPE_DEPTH,
            GPS_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(GPS_CMD_MID, CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(GPS_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                 "CMD Pipe failed to subscribe to GPS_CMD_MID. (0x%08lX)",
                 iStatus);
            goto GPS_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(GPS_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
             "Failed to create CMD pipe (0x%08lX)",
             iStatus);
        goto GPS_InitPipe_Exit_Tag;
    }

GPS_InitPipe_Exit_Tag:
    return (iStatus);
}
    

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void GPS::InitData()
{
    /* Init housekeeping message. */
    CFE_SB_InitMsg(&HkTlm,
            GPS_HK_TLM_MID, sizeof(HkTlm), TRUE);
    /* Init output messages */
    //CFE_SB_InitMsg(&GpsDump,
            //PX4_GPS_DUMP_MID, sizeof(PX4_GpsDumpMsg_t), TRUE);
    CFE_SB_InitMsg(&VehicleGps,
            PX4_VEHICLE_GPS_POSITION_MID, sizeof(PX4_VehicleGpsPositionMsg_t), TRUE);
    //CFE_SB_InitMsg(&SatelliteInfo,
            //PX4_SATELLITE_INFO_MID, sizeof(PX4_SatelliteInfoMsg_t), TRUE);
    /* Init custom data */
    GPS_Custom_InitData();
    return;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* GPS initialization                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 GPS::InitApp()
{
    int32  iStatus     = CFE_SUCCESS;
    boolean returnBool = TRUE;
    int8   hasEvents   = 0;

    iStatus = InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("GPS - Failed to init events (0x%08lX)\n", iStatus);
        goto GPS_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        goto GPS_InitApp_Exit_Tag;
    }

    InitData();

    iStatus = InitConfigTbl();
    if (iStatus != CFE_SUCCESS)
    {
        goto GPS_InitApp_Exit_Tag;
    }

    returnBool = GPS_Custom_Init();
    if (FALSE == returnBool)
    {
        iStatus = -1;
        goto GPS_InitApp_Exit_Tag;
    }
    
    HkTlm.State = GPS_INITIALIZED;

    /* Register the cleanup callback */
    iStatus = OS_TaskInstallDeleteHandler(&GPS_CleanupCallback);
    if (iStatus != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(GPS_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init register cleanup callback (0x%08X)",
                                 (unsigned int)iStatus);
        goto GPS_InitApp_Exit_Tag;
    }

GPS_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(GPS_INIT_INF_EID, CFE_EVS_INFORMATION,
                                 "Initialized.  Version %d.%d.%d.%d",
                                 GPS_MAJOR_VERSION,
                                 GPS_MINOR_VERSION,
                                 GPS_REVISION,
                                 GPS_MISSION_REV);
    }
    else
    {
        if (hasEvents == 1)
        {
            CFE_EVS_SendEvent(GPS_INIT_ERR_EID, CFE_EVS_ERROR, "Application failed to initialize");
        }
        else
        {
            CFE_ES_WriteToSysLog("GPS - Application failed to initialize\n");
        }
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 GPS::RcvSchPipeMsg(int32 iBlocking)
{
    int32           iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr  = NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(GPS_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(GPS_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
        {
            case GPS_READ_SENSOR_MID:
            {
                //returnBool = GPS_Custom_Measure_PositionMsg(&VehicleGps);
                //if(TRUE == returnBool)
                //{
                    //SendVehicleGps();
                //}
                //returnBool = GPS_Custom_Measure_SatInfoMsg(&SatelliteInfo);
                //if(TRUE == returnBool)
                //{
                    //SendSatelliteInfo();
                //}
                break;
            }
            case GPS_SEND_HK_MID:
            {
                ProcessCmdPipe();
                // TODO:  Need a mutex for this in the current event driven publish.
                memcpy(&HkTlm.VehicleGpsMsg, &VehicleGps, sizeof(VehicleGps));
                ReportHousekeeping();
                break;
            }
            //case PX4_GPS_INJECT_DATA_MID:
            //{
                //memcpy(&CVT.GpsInjectData, MsgPtr, sizeof(CVT.GpsInjectData));
                //break;
            //}
            default:
            {
                (void) CFE_EVS_SendEvent(GPS_MSGID_ERR_EID, CFE_EVS_ERROR,
                     "Recvd invalid SCH msgId (0x%04X)", MsgId);
            }
        }
    }
    else if (iStatus == CFE_SB_NO_MESSAGE)
    {
        /* If there's no incoming message, do nothing here.  
         * Note, this section is dead code only if the iBlocking arg
         * is CFE_SB_PEND_FOREVER. */
        iStatus = CFE_SUCCESS;
    }
    else if (iStatus == CFE_SB_TIME_OUT)
    {
        /* If there's no incoming message within a specified time (via the
         * iBlocking arg, do nothing here.  
         * Note, this section is dead code only if the iBlocking arg
         * is CFE_SB_PEND_FOREVER. */
        iStatus = CFE_SUCCESS;
    }
    else
    {
        (void) CFE_EVS_SendEvent(GPS_RCVMSG_ERR_EID, CFE_EVS_ERROR,
              "SCH pipe read error (0x%08lX).", iStatus);
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void GPS::ProcessCmdPipe()
{
    int32 iStatus             = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr = NULL;
    CFE_SB_MsgId_t  CmdMsgId;

    /* Process command messages until the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case GPS_CMD_MID:
                {
                    ProcessAppCmds(CmdMsgPtr);
                    break;
                }
                default:
                {
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with this
                     *  pipe, but not handled in this switch-case.) */
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(GPS_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "Recvd invalid CMD msgId (0x%04X)", (unsigned short)CmdMsgId);
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
            (void) CFE_EVS_SendEvent(GPS_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08lX)", iStatus);
            break;
        }
    }
    return;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process GPS Commands                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void GPS::ProcessAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode = 0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case GPS_NOOP_CC:
            {
                HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(GPS_CMD_NOOP_EID, CFE_EVS_INFORMATION,
                    "Recvd NOOP. Version %d.%d.%d.%d",
                    GPS_MAJOR_VERSION,
                    GPS_MINOR_VERSION,
                    GPS_REVISION,
                    GPS_MISSION_REV);
                break;
            }
            case GPS_RESET_CC:
            {
                HkTlm.usCmdCnt = 0;
                HkTlm.usCmdErrCnt = 0;
                break;
            }
            default:
            {
                HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(GPS_CC_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid command code (%u)", (unsigned int)uiCmdCode);
                break;
            }
        }
    }
    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send GPS Housekeeping                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void GPS::ReportHousekeeping()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&HkTlm);
    return;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Publish Output Data                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//void GPS::SendGpsDump()
//{
    //CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&GpsDump);
    //CFE_SB_SendMsg((CFE_SB_Msg_t*)&GpsDump);
    //return;
//}

void GPS::SendVehicleGps()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&VehicleGps);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&VehicleGps);
    return;
}

//void GPS::SendSatelliteInfo()
//{
    //CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&SatelliteInfo);
    //CFE_SB_SendMsg((CFE_SB_Msg_t*)&SatelliteInfo);
    //return;
//}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Command Length                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean GPS::VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
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

            (void) CFE_EVS_SendEvent(GPS_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            HkTlm.usCmdErrCnt++;
        }
    }

    return (bResult);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* GPS Application C style main entry point.                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
extern "C" void GPS_AppMain()
{
    oGPS.AppMain();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* GPS Application C++ style main entry point.                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void GPS::AppMain()
{
    /* Register the application with Executive Services */
    uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("GPS - Failed to register the app (0x%08lX)\n", iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(GPS_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(GPS_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(GPS_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(GPS_MAIN_TASK_PERF_ID);
    }
    else
    {
        uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&uiRunStatus) == TRUE)
    {
        RcvSchPipeMsg(GPS_SCH_PIPE_PEND_TIME);

        iStatus = AcquireConfigPointers();
        if(iStatus != CFE_SUCCESS)
        {
            /* We apparently tried to load a new table but failed.  Terminate the application. */
            uiRunStatus = CFE_ES_APP_ERROR;
        }
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(GPS_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(uiRunStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Cleanup prior to exit                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void GPS_CleanupCallback(void)
{
    oGPS.HkTlm.State = GPS_UNINITIALIZED;
    if(GPS_Custom_Uninit() != TRUE)
    {
        CFE_EVS_SendEvent(GPS_UNINIT_ERR_EID, CFE_EVS_ERROR,"GPS_Uninit failed");
        oGPS.HkTlm.State = GPS_INITIALIZED;
    }
    return;
}


void GPS_EventDrivenPublish(void)
{
    oGPS.EventDrivenPublish();
}


void GPS::EventDrivenPublish(void)
{
    boolean returnBool = FALSE;

    returnBool = GPS_Custom_Measure_PositionMsg(&VehicleGps);
    if(TRUE == returnBool)
    {
        SendVehicleGps();
    }
    //returnBool = GPS_Custom_Measure_SatInfoMsg(&SatelliteInfo);
    //if(TRUE == returnBool)
    //{
        //SendSatelliteInfo();
    //}
    return;
}

/************************/
/*  End of File Comment */
/************************/
