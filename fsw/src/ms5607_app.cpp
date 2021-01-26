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
#include "ms5607_custom.h"
#include "ms5607_app.h"
#include "ms5607_msg.h"
#include "ms5607_version.h"
#include <math.h>
#include "px4lib.h"
#include "px4lib_msgids.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Instantiate the application object.                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
MS5607 oMS5607;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Default constructor.                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
MS5607::MS5607()
{

}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Destructor constructor.                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
MS5607::~MS5607()
{

}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables.                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MS5607::InitEvent()
{
    int32  iStatus         = CFE_SUCCESS;
    int32  ind             = 0;
    int32 customEventCount = 0;
    
    CFE_EVS_BinFilter_t   EventTbl[CFE_EVS_MAX_EVENT_FILTERS];

    /* Initialize the event filter table.
     * Note: 0 is the CFE_EVS_NO_FILTER mask and event 0 is reserved (not used) */
    memset(EventTbl, 0x00, sizeof(EventTbl));

    /* CFE_EVS_MAX_EVENT_FILTERS limits the number of filters per app. */
    /* Add platform independent events to filter */
    EventTbl[  ind].EventID = MS5607_RESERVED_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = MS5607_READ_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_FIRST_16_STOP;

    /* Add custom events to the filter table */
    customEventCount = MS5607_Custom_Init_EventFilters(ind, EventTbl);
    
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
        (void) CFE_ES_WriteToSysLog("MS5607 - Failed to register with EVS (0x%08lX)\n", iStatus);
    }

end_of_function:

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MS5607::InitPipe()
{
    int32  iStatus = CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&SchPipeId,
            MS5607_SCH_PIPE_DEPTH,
            MS5607_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(MS5607_MEASURE_MID, SchPipeId, 
                CFE_SB_Default_Qos, MS5607_MEASURE_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MS5607_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "Sch Pipe failed to subscribe to MS5607_MEASURE_MID. (0x%08lX)",
                    iStatus);
            goto MS5607_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(MS5607_SEND_HK_MID, SchPipeId, 
                CFE_SB_Default_Qos, MS5607_SEND_HK_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MS5607_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                     "CMD Pipe failed to subscribe to MS5607_SEND_HK_MID. (0x%08X)",
                     (unsigned int)iStatus);
            goto MS5607_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(MS5607_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
             "Failed to create SCH pipe (0x%08lX)",
             iStatus);
        goto MS5607_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&CmdPipeId,
            MS5607_CMD_PIPE_DEPTH,
            MS5607_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(MS5607_CMD_MID, CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(MS5607_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                 "CMD Pipe failed to subscribe to MS5607_CMD_MID. (0x%08lX)",
                 iStatus);
            goto MS5607_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(MS5607_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
             "Failed to create CMD pipe (0x%08lX)",
             iStatus);
        goto MS5607_InitPipe_Exit_Tag;
    }

MS5607_InitPipe_Exit_Tag:
    return (iStatus);
}
    

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MS5607::InitData()
{
    /* Init housekeeping message. */
    CFE_SB_InitMsg(&HkTlm,
            MS5607_HK_TLM_MID, sizeof(HkTlm), TRUE);
    /* Init output messages */
    CFE_SB_InitMsg(&SensorBaro,
            PX4_SENSOR_BARO_MID, sizeof(PX4_SensorBaroMsg_t), TRUE);
    /* Init diagnostic message */
    CFE_SB_InitMsg(&Diag,
            MS5607_DIAG_TLM_MID, sizeof(MS5607_DiagPacket_t), TRUE);
    /* Init custom data */
    MS5607_Custom_InitData();
    /* Update params from the configuration table */
    UpdateParamsFromTable();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* MS5607 initialization                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MS5607::InitApp()
{
    int32  iStatus      = CFE_SUCCESS;
    int8   hasEvents    = 0;
    boolean returnBool  = TRUE;
    int32 i             = 0;
    
    iStatus = InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("MS5607 - Failed to init events (0x%08lX)\n", iStatus);
        goto MS5607_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        goto MS5607_InitApp_Exit_Tag;
    }

    iStatus = InitConfigTbl();
    if (iStatus != CFE_SUCCESS)
    {
        goto MS5607_InitApp_Exit_Tag;
    }

    /* Init data include copy params from config table */
    InitData();

    returnBool = MS5607_Custom_Init();
    if (FALSE == returnBool)
    {
        iStatus = -1;
        goto MS5607_InitApp_Exit_Tag;
    }

    /* Get calibration coefficients from device PROM */
    for(i = 0; i < MS5607_COEF_SIZE; ++i)
    {
        returnBool = MS5607_ReadPROM(i, &MS5607_Coefficients[i]);
        /* Copy to diagnostic message */
        Diag.Coefficients[i] = MS5607_Coefficients[i];
        if (FALSE == returnBool)
        {
            iStatus = -1;
            (void) CFE_EVS_SendEvent(MS5607_INIT_ERR_EID, CFE_EVS_ERROR,
                "MS5607 failed read from device PROM");
            goto MS5607_InitApp_Exit_Tag; 
        }
    }

    /* Validate the CRC code */
    returnBool = ValidateCRC();
    if (FALSE == returnBool)
    {
        (void) CFE_EVS_SendEvent(MS5607_INIT_ERR_EID, CFE_EVS_ERROR,
                "MS5607 failed CRC check");
        iStatus = -1;
        goto MS5607_InitApp_Exit_Tag;
    }

    /* Register the cleanup callback */
    iStatus = OS_TaskInstallDeleteHandler(&MS5607_CleanupCallback);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MS5607_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init register cleanup callback (0x%08X)",
                                 (unsigned int)iStatus);
        goto MS5607_InitApp_Exit_Tag;
    }

    /* Set application state to initialized */
    HkTlm.State = MS5607_INITIALIZED;

MS5607_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MS5607_INIT_INF_EID, CFE_EVS_INFORMATION,
                                 "Initialized.  Version %d.%d.%d.%d",
                                 MS5607_MAJOR_VERSION,
                                 MS5607_MINOR_VERSION,
                                 MS5607_REVISION,
                                 MS5607_MISSION_REV);
    }
    else
    {
        if (hasEvents == 1)
        {
            (void) CFE_ES_WriteToSysLog("MS5607 - Application failed to initialize\n");
        }
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MS5607::RcvSchPipeMsg(int32 iBlocking)
{
    int32           iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr  = NULL;
    CFE_SB_MsgId_t  MsgId   = 0;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(MS5607_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(MS5607_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
        {
            case MS5607_SEND_HK_MID:
            {
                ProcessCmdPipe();
                ReportHousekeeping();
                break;
            }
            case MS5607_MEASURE_MID:
            {
                ReadDevice();
                break;
            }
            default:
            {
                (void) CFE_EVS_SendEvent(MS5607_MSGID_ERR_EID, CFE_EVS_ERROR,
                        "Recvd invalid SCH msgId (0x%04X)", MsgId);
            }
        }
    }
    else if (iStatus == CFE_SB_NO_MESSAGE)
    {
        /* If there's no incoming message, do nothing. 
         *  Note, this section is dead code only if the iBlocking arg
         * is CFE_SB_PEND_FOREVER. */
        iStatus = CFE_SUCCESS;
    }
    else if (iStatus == CFE_SB_TIME_OUT)
    {
        /* If there's no incoming message within a specified time (via the
         * iBlocking arg, do nothing.
         * Note, this section is dead code only if the iBlocking arg
         * is CFE_SB_PEND_FOREVER. */
        iStatus = CFE_SUCCESS;
    }
    else
    {
        (void) CFE_EVS_SendEvent(MS5607_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                "SCH pipe read error (0x%08lX).", iStatus);
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MS5607::ProcessCmdPipe()
{
    int32 iStatus             = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr = NULL;
    CFE_SB_MsgId_t  CmdMsgId  = 0;

    /* Process command messages until the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case MS5607_CMD_MID:
                    ProcessAppCmds(CmdMsgPtr);
                    break;

                default:
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with this
                     *  pipe, but not handled in this switch-case.) */
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(MS5607_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            (void) CFE_EVS_SendEvent(MS5607_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08lX)", iStatus);
            break;
        }
    }
    return;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process MS5607 Commands                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MS5607::ProcessAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode = 0;

    if (MsgPtr != NULL)
    {
        uint16 ExpectedLength = sizeof(MS5607_NoArgCmd_t);
        /* Length verification is then the same for all three commands */
        if (VerifyCmdLength(MsgPtr, ExpectedLength))
        {
            uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
            switch (uiCmdCode)
            {
                case MS5607_NOOP_CC:
                {
                    HkTlm.usCmdCnt++;
                    (void) CFE_EVS_SendEvent(MS5607_CMD_NOOP_EID, CFE_EVS_INFORMATION,
                        "Recvd NOOP. Version %d.%d.%d.%d",
                        MS5607_MAJOR_VERSION,
                        MS5607_MINOR_VERSION,
                        MS5607_REVISION,
                        MS5607_MISSION_REV);
                    break;
                }
                case MS5607_RESET_CC:
                {
                    HkTlm.usCmdCnt = 0;
                    HkTlm.usCmdErrCnt = 0;
                    break;
                }
                case MS5607_SEND_DIAG_TLM_CC:
                {
                    HkTlm.usCmdCnt++;
                    ReportDiagnostic();
                    break;
                }
                default:
                {
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(MS5607_CC_ERR_EID, CFE_EVS_ERROR,
                                      "Recvd invalid command code (%u)", (unsigned int)uiCmdCode);
                    break;
                }
            }
        }
    }
    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send MS5607 Housekeeping                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MS5607::ReportHousekeeping()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&HkTlm);
    return;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send MS5607 Diagnostic                                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MS5607::ReportDiagnostic()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&Diag);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&Diag);
    return;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Command Length                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean MS5607::VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
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

            (void) CFE_EVS_SendEvent(MS5607_MSGLEN_ERR_EID, CFE_EVS_ERROR,
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
/* MS5607 Application C style main entry point.                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
extern "C" void MS5607_AppMain()
{
    oMS5607.AppMain();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* MS5607 Application C++ style main entry point.                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MS5607::AppMain()
{
    /* Register the application with Executive Services */
    uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("MS5607 - Failed to register the app (0x%08lX)\n", iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(MS5607_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(MS5607_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(MS5607_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(MS5607_MAIN_TASK_PERF_ID);
    }
    else
    {
        uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&uiRunStatus) == TRUE)
    {
        RcvSchPipeMsg(MS5607_SCH_PIPE_PEND_TIME);
        iStatus = AcquireConfigPointers();
        if(iStatus != CFE_SUCCESS)
        {
            /* We apparently tried to load a new table but failed.  Terminate the application. */
            uiRunStatus = CFE_ES_APP_ERROR;
        }
    }

    /* Pre-normal exit cleanup */
    CleanupExit();

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(MS5607_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(uiRunStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Get a raw pressure and temperature measurement from the device  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean MS5607::GetMeasurement(int32 *Pressure, int32 *Temperature)
{
    /* ADC value of the pressure conversion */
    uint32 D1           = 0;
    /* ADC value of the temperature conversion
     * static to keep temperature measurements between calls */
    static uint32 D2    = 0;
    /* difference between actual and measured temp */
    int32 dT            = 0;
    /* offset at actual temperature */
    int64 OFF           = 0;
    /* sensitivity at actual temperature */
    int64 SENS          = 0;
    int32 T2            = 0;
    int64 OFF2          = 0;
    int64 SENS2         = 0;
    int64 TEMP          = 0;
    int32 TempValidate  = 0;
    int32 PressValidate = 0;
    boolean returnBool  = TRUE;

    returnBool = MS5607_D1Conversion();
    if (FALSE == returnBool)
    {
        (void) CFE_EVS_SendEvent(MS5607_READ_ERR_EID, CFE_EVS_ERROR,
                "MS5607 get measurement D1 conversion failed");
        returnBool = FALSE;
        goto end_of_function;
    }

    returnBool = MS5607_ReadADCResult(&D1);
    if (FALSE == returnBool)
    {
        (void) CFE_EVS_SendEvent(MS5607_READ_ERR_EID, CFE_EVS_ERROR,
                "MS5607 read ADC result D1 failed");
        returnBool = FALSE;
        goto end_of_function;
    }

    /* Measurement ratio pressure per temperature measurements */
    if(0 == Diag.MeasureCount % MS5607_PRESS_TEMP_MEAS_RATIO)
    { 
        returnBool = MS5607_D2Conversion();
        if (FALSE == returnBool)
        {
            (void) CFE_EVS_SendEvent(MS5607_READ_ERR_EID, CFE_EVS_ERROR,
                    "MS5607 get measurement D2 conversion failed");
            returnBool = FALSE;
            goto end_of_function;
        }
        
        returnBool = MS5607_ReadADCResult(&D2);
        if (FALSE == returnBool)
        {
            (void) CFE_EVS_SendEvent(MS5607_READ_ERR_EID, CFE_EVS_ERROR,
                    "MS5607 read ADC result D2 failed");
            returnBool = FALSE;
            goto end_of_function;
        }
    }

    if(0xFF == Diag.MeasureCount)
    {
        /* Roll over */
        Diag.MeasureCount = 0;
    }
    /* Increment the measure count */
    Diag.MeasureCount++;

    /* D2 - C5 * 2^8 */
    dT    = (int32)D2 - ((int32)MS5607_Coefficients[5] << 8);

    /* The following two equations must be solved with 64-bit integers (long long int)
     * or overflow could occur. */

    /* C2 * 2^16 + (C4 * dT )/ 2^7 */
    OFF   = ((int64)MS5607_Coefficients[2] << 17) + (((int64)MS5607_Coefficients[4] * dT) >> 6);
    /* C1 * 2^15 + (C3 * dT )/ 2^8 */
    SENS  = ((int64)MS5607_Coefficients[1] << 16) + (((int64)MS5607_Coefficients[3] * dT) >> 7);
    /* 2000 + dT * C6 / 2^23 */
    TempValidate = 2000 + (int32)(((int64)dT * MS5607_Coefficients[6]) >> 23);

    /* Validate temperature */
    if(TempValidate > MS5607_TEMP_MIN && TempValidate < MS5607_TEMP_MAX)
    {
        *Temperature = TempValidate;
    }
    else
    {
        (void) CFE_EVS_SendEvent(MS5607_READ_ERR_EID, CFE_EVS_ERROR,
                "MS5607 temperature out of range value = %ld", TempValidate);
        returnBool = FALSE;
        goto end_of_function;
    }
    /* Second order temperature compensation */
    if(*Temperature < 2000)
    {
        T2    = (dT * dT) >> 31;
        TEMP  = (int64)*Temperature - 2000;
        OFF2  = (61 *  TEMP * TEMP) >> 4;
        SENS2 = 2 *  TEMP * TEMP;

        if(*Temperature < -1500)
        {
            TEMP  = *Temperature + 1500;
            OFF2  += 15 * TEMP * TEMP;
            SENS2 += 8 * TEMP * TEMP;
        }

        *Temperature -= T2;
        OFF   -= OFF2;
        SENS  -= SENS2;
    }
    /*  (D1 * SENS / 2^21 - OFF) / 2^15*/
    PressValidate = ((((D1 * SENS) >> 21) - OFF ) >> 15);

    /* Validate pressure */
    if(PressValidate > MS5607_PRESS_MIN && PressValidate < MS5607_PRESS_MAX)
    {
        *Pressure = PressValidate;
    }
    else
    {
        (void) CFE_EVS_SendEvent(MS5607_READ_ERR_EID, CFE_EVS_ERROR,
                "MS5607 pressure out of range value = %ld", PressValidate);
        returnBool = FALSE;
        goto end_of_function;
    }

    /* Update diagnostic message */
    Diag.RawPressure = *Pressure;
    Diag.RawTemperature = *Temperature;

end_of_function:
    return (returnBool);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Read from the device and convert to altitude                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MS5607::ReadDevice(void)
{
    int32 pressure      = 0;
    int32 temperature   = 0;
    boolean returnBool  = TRUE;

    returnBool = GetMeasurement(&pressure, &temperature);
    if (FALSE == returnBool)
    {
        (void) CFE_EVS_SendEvent(MS5607_READ_ERR_EID, CFE_EVS_ERROR,
                "MS5607 read failure, altitude not updated");
        goto end_of_function;
    }
    else
    {
        /* Stamp time */
        SensorBaro.Timestamp = PX4LIB_GetPX4TimeUs();
        /* Convert to Celsius */
        SensorBaro.Temperature = temperature / 100.0f;
        /* convert to millibar */
        SensorBaro.Pressure = pressure / 100.0f;
    
        /* tropospheric properties (0-11km) for standard atmosphere */
        /* temperature at base height in Kelvin */
        const double T1 = 15.0 + 273.15;
        /* temperature gradient in degrees per metre */
        const double a  = -6.5 / 1000;
        /* gravity constant in m/s/s */
        const double g  = 9.80665;
        /* ideal gas constant in J/kg/K */
        const double R  = 287.05;
        /* measured pressure in kPa */
        double p = static_cast<double>(pressure) / 1000.0;

        /* current pressure at MSL in kPa double p1 = 101325 / 1000.0 */

        /*
         * Solve:
         *
         *     /        -(aR / g)     \
         *    | (p / p1)          . T1 | - T1
         *     \                      /
         * h = -------------------------------  + h1
         *                   a
         */
        SensorBaro.Altitude = (((pow((p / m_Params.p1), (-(a * R) / g))) * T1) - T1) / a;
    
        /* Update diagnostic message */
        Diag.Pressure = SensorBaro.Pressure;
        Diag.Temperature = SensorBaro.Temperature;
        Diag.Altitude = SensorBaro.Altitude;
        
        /* Send the SensorBaro message */
        CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&SensorBaro);
        CFE_SB_SendMsg((CFE_SB_Msg_t*)&SensorBaro);
    }

end_of_function:
    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Calculate a CRC4 code                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
uint8 MS5607::CRC4(uint16 n_prom[])
{
    int cnt         = 0; 
    /* crc remainder */
    uint16 n_rem    = 0; 
    /* original crc value */
    uint16 crc_read = 0; 
    uint8 n_bit     = 0;
    n_rem           = 0x00;
    /* save the original CRC */
    crc_read        = n_prom[7];
    /* replace the crc byte with 0 */
    n_prom[7]       = (0xFF00 & (n_prom[7])); 
    /* operation is performed on bytes */
    for (cnt = 0; cnt < 16; cnt++) 
    {
        /* choose LSB or MSB */
        if (cnt%2 == 1) 
        {
            n_rem ^= (uint16) ((n_prom[cnt>>1]) & 0x00FF);
        }
        else 
        {
            n_rem ^= (uint16) (n_prom[cnt>>1]>>8);
        }
        for (n_bit = 8; n_bit > 0; n_bit--)
        {
            if (n_rem & (0x8000))
            {
                n_rem = (n_rem << 1) ^ 0x3000;
            }
            else
            {
                n_rem = (n_rem << 1);
            }
        }
    }
    /* the final 4-bit remainder is the CRC code */
    n_rem = (0x000F & (n_rem >> 12));
    /* Restore the crc_read to its original value */
    n_prom[7] = crc_read;
    return (n_rem ^ 0x00);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate the CRC code in PROM                                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean MS5607::ValidateCRC(void)
{
    boolean returnBool        = FALSE;
    unsigned char returnedCRC = 0;
    uint16 promCRC            = 0;
    /* CRC code is in the last 4-bits */
    uint16 bitmask            = 0x0F;
    /* Get CRC value from PROM */
    promCRC                   = bitmask & MS5607_Coefficients[7];

    /* Calculate CRC */
    returnedCRC = CRC4(MS5607_Coefficients);

    if(promCRC == returnedCRC)
    {
        returnBool = TRUE;
    }
    else
    {
        (void) CFE_EVS_SendEvent(MS5607_INIT_ERR_EID, CFE_EVS_ERROR,
                "MS5607 CRC check failed PROM = %u: CRC = %u",
                (unsigned int)promCRC, (unsigned int) returnedCRC);
    }
    return (returnBool);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Update params from the config table                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MS5607::UpdateParamsFromTable(void)
{
    if(0 != ConfigTblPtr)
    {
        /* MSL Pressure */
        m_Params.p1 = ConfigTblPtr->p1;
    }
    return;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Normal application exit                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MS5607::CleanupExit(void)
{
    HkTlm.State = MS5607_UNINITIALIZED;
    if(MS5607_Custom_Uninit() != TRUE)
    {
        CFE_EVS_SendEvent(MS5607_UNINIT_ERR_EID, CFE_EVS_ERROR,"MS5607_Uninit failed");
        HkTlm.State = MS5607_INITIALIZED;
    }
    return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Cleanup before exit                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MS5607_CleanupCallback(void)
{
    MS5607_Critical_Cleanup();
    return;
}


/************************/
/*  End of File Comment */
/************************/
