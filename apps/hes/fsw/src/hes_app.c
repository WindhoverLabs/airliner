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
#include <math.h>

#include "cfe.h"

#include "hes_app.h"
#include "hes_msg.h"
#include "hes_version.h"
#include "px4lib_msgids.h"
#include "px4lib.h"


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
HES_AppData_t  HES_AppData;

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Definitions
*************************************************************************/
void HES_ProcessCVT();

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 HES_InitEvent()
{
    int32  iStatus=CFE_SUCCESS;
    int32  ind = 0;

    /* Initialize the event filter table.
     * Note: 0 is the CFE_EVS_NO_FILTER mask and event 0 is reserved (not used) */
    memset((void*)HES_AppData.EventTbl, 0x00, sizeof(HES_AppData.EventTbl));

    /* TODO: Choose the events you want to filter.  CFE_EVS_MAX_EVENT_FILTERS
     * limits the number of filters per app.  An explicit CFE_EVS_NO_FILTER 
     * (the default) has been provided as an example. */
    HES_AppData.EventTbl[  ind].EventID = HES_RESERVED_EID;
    HES_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    HES_AppData.EventTbl[  ind].EventID = HES_INF_EID;
    HES_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    HES_AppData.EventTbl[  ind].EventID = HES_CONFIG_TABLE_ERR_EID;
    HES_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    HES_AppData.EventTbl[  ind].EventID = HES_CDS_ERR_EID;
    HES_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    HES_AppData.EventTbl[  ind].EventID = HES_PIPE_ERR_EID;
    HES_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    HES_AppData.EventTbl[  ind].EventID = HES_MSGID_ERR_EID;
    HES_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    HES_AppData.EventTbl[  ind].EventID = HES_MSGLEN_ERR_EID;
    HES_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(HES_AppData.EventTbl,
                               HES_EVT_CNT, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("HES - Failed to register with EVS (0x%08X)\n", (unsigned int)iStatus);
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 HES_InitPipe()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&HES_AppData.SchPipeId,
                                 HES_SCH_PIPE_DEPTH,
                                 HES_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(HES_WAKEUP_MID, HES_AppData.SchPipeId, CFE_SB_Default_Qos, HES_SCH_PIPE_WAKEUP_RESERVED);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(HES_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "Sch Pipe failed to subscribe to HES_WAKEUP_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto HES_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(HES_SEND_HK_MID, HES_AppData.SchPipeId, CFE_SB_Default_Qos, HES_SCH_PIPE_SEND_HK_RESERVED);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(HES_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "CMD Pipe failed to subscribe to HES_SEND_HK_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto HES_InitPipe_Exit_Tag;
        }

    }
    else
    {
        (void) CFE_EVS_SendEvent(HES_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create SCH pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto HES_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&HES_AppData.CmdPipeId,
                                 HES_CMD_PIPE_DEPTH,
                                 HES_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(HES_CMD_MID, HES_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(HES_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "CMD Pipe failed to subscribe to HES_CMD_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto HES_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(HES_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create CMD pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto HES_InitPipe_Exit_Tag;
    }

    /* Init data pipe and subscribe to messages on the data pipe */
    iStatus = CFE_SB_CreatePipe(&HES_AppData.DataPipeId,
                                 HES_DATA_PIPE_DEPTH,
                                 HES_DATA_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(TO_CPD(PX4_VEHICLE_GLOBAL_POSITION_MID),
                HES_AppData.DataPipeId, CFE_SB_Default_Qos, 1);
        if(iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(HES_INIT_ERR_EID, CFE_EVS_ERROR,
                    "DATA Pipe failed to subscribe to PX4_VEHICLE_GLOBAL_POSITION_MID. (0x%08lX)",
                    iStatus);
            goto HES_InitPipe_Exit_Tag;
        }


        iStatus = CFE_SB_SubscribeEx(TO_CPD(PX4_VEHICLE_ATTITUDE_MID),
                HES_AppData.DataPipeId, CFE_SB_Default_Qos, 1);
        if(iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(HES_INIT_ERR_EID, CFE_EVS_ERROR,
                    "DATA Pipe failed to subscribe to PX4_VEHICLE_ATTITUDE_MID. (0x%08lX)",
                    iStatus);
            goto HES_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(TO_CPD(PX4_AIRSPEED_MID),
                HES_AppData.DataPipeId, CFE_SB_Default_Qos, 1);
        if(iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(HES_INIT_ERR_EID, CFE_EVS_ERROR,
                    "DATA Pipe failed to subscribe to PX4_AIRSPEED_MID. (0x%08lX)",
                    iStatus);
            goto HES_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(TO_CPD(PX4_BATTERY_STATUS_MID), HES_AppData.DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(HES_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "DATA Pipe failed to subscribe to PX4_BATTERY_STATUS_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto HES_InitPipe_Exit_Tag;
        }



        /* TODO:  Add CFE_SB_Subscribe() calls for other apps' output data here.
        **
        ** Examples:
        **     CFE_SB_Subscribe(GNCEXEC_OUT_DATA_MID, HES_AppData.DataPipeId);
        */
    }
    else
    {
        (void) CFE_EVS_SendEvent(HES_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create Data pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto HES_InitPipe_Exit_Tag;
    }

HES_InitPipe_Exit_Tag:
    return (iStatus);
}
    

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 HES_InitData()
{
    int32  iStatus=CFE_SUCCESS;

    memset((void*)&HES_AppData.CVT, 0x00, sizeof(HES_AppData.CVT));

    /* Init input data */
    memset((void*)&HES_AppData.InData, 0x00, sizeof(HES_AppData.InData));

    /* Init output data */
    memset((void*)&HES_AppData.OutData, 0x00, sizeof(HES_AppData.OutData));
    CFE_SB_InitMsg(&HES_AppData.OutData,
                   HES_OUT_DATA_MID, sizeof(HES_AppData.OutData), TRUE);

    /* Init housekeeping packet */
    memset((void*)&HES_AppData.HkTlm, 0x00, sizeof(HES_AppData.HkTlm));
    CFE_SB_InitMsg(&HES_AppData.HkTlm,
                   HES_HK_TLM_MID, sizeof(HES_AppData.HkTlm), TRUE);

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* HES initialization                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 HES_InitApp()
{
    int32  iStatus   = CFE_SUCCESS;
    int8   hasEvents = 0;

    iStatus = HES_InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("HES - Failed to init events (0x%08X)\n", (unsigned int)iStatus);
        goto HES_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = HES_InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(HES_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init pipes (0x%08X)",
                                 (unsigned int)iStatus);
        goto HES_InitApp_Exit_Tag;
    }

    iStatus = HES_InitData();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(HES_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init data (0x%08X)",
                                 (unsigned int)iStatus);
        goto HES_InitApp_Exit_Tag;
    }

    iStatus = HES_InitConfigTbl();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(HES_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init config tables (0x%08X)",
                                 (unsigned int)iStatus);
        goto HES_InitApp_Exit_Tag;
    }

    iStatus = HES_InitCdsTbl();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(HES_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init CDS table (0x%08X)",
                                 (unsigned int)iStatus);
        goto HES_InitApp_Exit_Tag;
    }

HES_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(HES_INIT_INF_EID, CFE_EVS_INFORMATION,
                                 "Initialized.  Version %d.%d.%d.%d",
                                 HES_MAJOR_VERSION,
                                 HES_MINOR_VERSION,
                                 HES_REVISION,
                                 HES_MISSION_REV);
    }
    else
    {
        if (hasEvents == 1)
        {
            (void) CFE_EVS_SendEvent(HES_INIT_ERR_EID, CFE_EVS_ERROR, "Application failed to initialize");
        }
        else
        {
            (void) CFE_ES_WriteToSysLog("HES - Application failed to initialize\n");
        }
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 HES_RcvMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(HES_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, HES_AppData.SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(HES_MAIN_TASK_PERF_ID);

    // printf("waking up code %x\n", HES_WAKEUP_MID);
    //printf("waking up code %x\n", TO_SEND_HK_MID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);

        // printf("getting here %d\n", MsgId);

        switch (MsgId)
	    {
            case HES_WAKEUP_MID:
                // printf("waking up\n");
                // printf("getting here %d\n", MsgId);

                // printf("wake up code %x\n", HES_WAKEUP_MID);
                // printf("Hk code %x\n", HES_HK_TLM_MID);
                // printf("Send Hk code %x\n", HES_SEND_HK_MID);
                // printf("Cmd code %x\n", HES_CMD_MID);

                HES_ProcessNewCmds();
                HES_ProcessNewData();
                HES_ProcessCVT();

                /* TODO:  Add more code here to handle other things when app wakes up */

                /* The last thing to do at the end of this Wakeup cycle should be to
                 * automatically publish new output. */
                HES_SendOutData();
                break;

            case HES_SEND_HK_MID:
                HES_ReportHousekeeping();
                break;

            default:
                (void) CFE_EVS_SendEvent(HES_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid SCH msgId (0x%04X)", (unsigned short)MsgId);
        }
    }
    else if (iStatus == CFE_SB_NO_MESSAGE)
    {
        /* TODO: If there's no incoming message, you can do something here, or 
         * nothing.  Note, this section is dead code only if the iBlocking arg
         * is CFE_SB_PEND_FOREVER. */
        // printf("no msg %ld\n", iStatus);
        iStatus = CFE_SUCCESS;
    }
    else if (iStatus == CFE_SB_TIME_OUT)
    {
        /* TODO: If there's no incoming message within a specified time (via the
         * iBlocking arg, you can do something here, or nothing.  
         * Note, this section is dead code only if the iBlocking arg
         * is CFE_SB_PEND_FOREVER. */
        // printf("timeout %ld\n", iStatus);
        iStatus = CFE_SUCCESS;
    }
    else
    {
        /* TODO: This is an example of exiting on an error (either CFE_SB_BAD_ARGUMENT, or
         * CFE_SB_PIPE_RD_ERROR).
         */
        // printf("error %d\n", iStatus);
        (void) CFE_EVS_SendEvent(HES_PIPE_ERR_EID, CFE_EVS_ERROR,
			  "SB pipe read error (0x%08X), app will exit", (unsigned int)iStatus);
        HES_AppData.uiRunStatus= CFE_ES_APP_ERROR;
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Data                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void HES_ProcessNewData()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   DataMsgPtr=NULL;
    CFE_SB_MsgId_t  DataMsgId;

    /* Process telemetry messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&DataMsgPtr, HES_AppData.DataPipeId, CFE_SB_POLL);
        if (iStatus == CFE_SUCCESS)
        {
            DataMsgId = CFE_SB_GetMsgId(DataMsgPtr);
            switch (DataMsgId)
            {
				case TO_CPD(PX4_AIRSPEED_MID):
				{
					HES_AppData.HkTlm.AirSpeedMsgRcvCnt++;
					CFE_PSP_MemCpy(&HES_AppData.CVT.Airspeed, DataMsgPtr, sizeof(HES_AppData.CVT.Airspeed));
					break;
				}

				case TO_CPD(PX4_BATTERY_STATUS_MID):
				{
					HES_AppData.HkTlm.BatteryStatusMsgRcvCnt++;
					CFE_PSP_MemCpy(&HES_AppData.CVT.BatteryStatus, DataMsgPtr, sizeof(HES_AppData.CVT.BatteryStatus));
					break;
				}

				case TO_CPD(PX4_VEHICLE_ATTITUDE_MID):
				{
					HES_AppData.HkTlm.VAttMsgRcvCnt++;
					CFE_PSP_MemCpy(&HES_AppData.CVT.VAtt, DataMsgPtr, sizeof(HES_AppData.CVT.VAtt));
					break;
				}

				case TO_CPD(PX4_VEHICLE_GLOBAL_POSITION_MID):
				{
					HES_AppData.HkTlm.VGlobalPositionMsgRcvCnt++;
					CFE_PSP_MemCpy(&HES_AppData.CVT.VGlobalPosition, DataMsgPtr, sizeof(HES_AppData.CVT.VGlobalPosition));
					break;
				}

				case TO_CPD(PX4_VEHICLE_LAND_DETECTED_MID):
				{
					HES_AppData.HkTlm.VLandDetectedMsgRcvCnt++;
					CFE_PSP_MemCpy(&HES_AppData.CVT.VLandDetected, DataMsgPtr, sizeof(HES_AppData.CVT.VLandDetected));
					break;
				}

				case TO_CPD(PX4_VEHICLE_STATUS_MID):
				{
					HES_AppData.HkTlm.VehicleStatusMsgRcvCnt++;
					CFE_PSP_MemCpy(&HES_AppData.CVT.VehicleStatus, DataMsgPtr, sizeof(HES_AppData.CVT.VehicleStatus));
					break;
				}

                default:
                    (void) CFE_EVS_SendEvent(HES_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            (void) CFE_EVS_SendEvent(HES_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "Data pipe read error (0x%08X)", (unsigned int)iStatus);
            HES_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}

/* Conjugate with inversed quaternion */
/* Body to NED Frame*/
void Quaternion_Conjugate(const float* v, const float* data, float* result)
{
    float q0q0 = data[0] * data[0];
    float q1q1 = data[1] * data[1];
    float q2q2 = data[2] * data[2];
    float q3q3 = data[3] * data[3];
    
    result[0] = (v[0] * (
            q0q0 + q1q1 - q2q2 - q3q3) +
            v[1] * 2.0f * (data[1] * data[2] - data[0] * data[3]) +
            v[2] * 2.0f * (data[0] * data[2] + data[1] * data[3]));

    result[1] = (v[0] * 2.0f * (data[1] * data[2] + data[0] * data[3]) +
            v[1] * (q0q0 - q1q1 + q2q2 - q3q3) +
            v[2] * 2.0f * (data[2] * data[3] - data[0] * data[1]));

    result[2] = (v[0] * 2.0f * (data[1] * data[3] - data[0] * data[2]) +
            v[1] * 2.0f * (data[0] * data[1] + data[2] * data[3]) +
            v[2] * (q0q0 - q1q1 - q2q2 + q3q3));
}


/* Conjugate with inversed quaternion */
/* NED to Body Frame*/
void Quaternion_ConjugateInversed(const float* v, const float* data, float* result)
{
    float q0q0 = data[0] * data[0];
    float q1q1 = data[1] * data[1];
    float q2q2 = data[2] * data[2];
    float q3q3 = data[3] * data[3];
    
    result[0] = (v[0] * (
            q0q0 + q1q1 - q2q2 - q3q3) +
            v[1] * 2.0f * (data[1] * data[2] + data[0] * data[3]) +
            v[2] * 2.0f * (data[1] * data[3] - data[0] * data[2]));

    result[1] = (v[0] * 2.0f * (data[1] * data[2] - data[0] * data[3]) +
            v[1] * (q0q0 - q1q1 + q2q2 - q3q3) +
            v[2] * 2.0f * (data[2] * data[3] + data[0] * data[1]));

    result[2] = (v[0] * 2.0f * (data[1] * data[3] + data[0] * data[2]) +
            v[1] * 2.0f * (data[2] * data[3] - data[0] * data[1]) +
            v[2] * (q0q0 - q1q1 - q2q2 + q3q3));
}


void Quaternion_RotationMatrix(float* data, float** R)
{
	float aSq = data[0] * data[0];
	float bSq = data[1] * data[1];
	float cSq = data[2] * data[2];
	float dSq = data[3] * data[3];
	R[0][0] = aSq + bSq - cSq - dSq;
	R[0][1] = 2.0f * (data[1] * data[2] - data[0] * data[3]);
	R[0][2] = 2.0f * (data[0] * data[2] + data[1] * data[3]);
	R[1][0] = 2.0f * (data[1] * data[2] + data[0] * data[3]);
	R[1][1] = aSq - bSq + cSq - dSq;
	R[1][2] = 2.0f * (data[2] * data[3] - data[0] * data[1]);
	R[2][0] = 2.0f * (data[1] * data[3] - data[0] * data[2]);
	R[2][1] = 2.0f * (data[0] * data[1] + data[2] * data[3]);
	R[2][2] = aSq - bSq - cSq + dSq;
}

void Quaternion_ToEuler(float* data, float* euler)
{
	euler[0] = atan2f(2.0f * (data[0] * data[1] + data[2] * data[3]), 1.0f - 2.0f * (data[1] * data[1] + data[2] * data[2]));
	euler[1] = asinf(2.0f * (data[0] * data[2] - data[3] * data[1]));
	euler[2] = atan2f(2.0f * (data[0] * data[3] + data[1] * data[2]), 1.0f - 2.0f * (data[2] * data[2] + data[3] * data[3]));
}

void HES_ProcessCVT() {
    float euler[3];
    float wind_n, wind_e;
    float vel_ned[3];
    float wind[3];
    float wind_body[3];
    float v_r;

    // printf("processing CVT\n");

    HES_AppData.HkTlm.altitude[0] = HES_AppData.CVT.VGlobalPosition.Alt;
    HES_AppData.HkTlm.altitude[1] = 3.28084 * HES_AppData.CVT.VGlobalPosition.Alt;
    HES_AppData.HkTlm.altitude[0] = HES_AppData.CVT.VGlobalPosition.Alt / 3;

    HES_AppData.HkTlm.Lat = HES_AppData.CVT.VGlobalPosition.Lat;
    HES_AppData.HkTlm.Lon = HES_AppData.CVT.VGlobalPosition.Lon;

    /*TODO CHange the following code to get stuff from NAV instead of hardcoding values*/
    HES_AppData.HkTlm.num_way_points = 4;
    HES_AppData.HkTlm.way_points_valid = TRUE;

    HES_AppData.HkTlm.way_points[0].Alt = HES_AppData.CVT.VGlobalPosition.Alt;
    HES_AppData.HkTlm.way_points[0].Lat = HES_AppData.CVT.VGlobalPosition.Lat - 0.001;
    HES_AppData.HkTlm.way_points[0].Lon = HES_AppData.CVT.VGlobalPosition.Lon;
    HES_AppData.HkTlm.way_points[0].radius = 30;

    HES_AppData.HkTlm.way_points[1].Alt = HES_AppData.CVT.VGlobalPosition.Alt;
    HES_AppData.HkTlm.way_points[1].Lat = HES_AppData.CVT.VGlobalPosition.Lat + 0.001;
    HES_AppData.HkTlm.way_points[1].Lon = HES_AppData.CVT.VGlobalPosition.Lon;
    HES_AppData.HkTlm.way_points[1].radius = 30;

    HES_AppData.HkTlm.way_points[2].Alt = HES_AppData.CVT.VGlobalPosition.Alt;
    HES_AppData.HkTlm.way_points[2].Lat = HES_AppData.CVT.VGlobalPosition.Lat;
    HES_AppData.HkTlm.way_points[2].Lon = HES_AppData.CVT.VGlobalPosition.Lon + 0.001;
    HES_AppData.HkTlm.way_points[2].radius = 30;

    HES_AppData.HkTlm.way_points[3].Alt = HES_AppData.CVT.VGlobalPosition.Alt;
    HES_AppData.HkTlm.way_points[3].Lat = HES_AppData.CVT.VGlobalPosition.Lat;
    HES_AppData.HkTlm.way_points[3].Lon = HES_AppData.CVT.VGlobalPosition.Lon - 0.001;
    HES_AppData.HkTlm.way_points[3].radius = 30;

    HES_AppData.CVT.filtered_vel_d = 
        (HES_VEL_FILTER_COEFF) * (HES_AppData.CVT.VGlobalPosition.VelD) +
        (1 - (HES_VEL_FILTER_COEFF)) * HES_AppData.CVT.filtered_vel_d;
    HES_AppData.CVT.filtered_vel_e = 
        (HES_VEL_FILTER_COEFF) * (HES_AppData.CVT.VGlobalPosition.VelE) +
        (1 - (HES_VEL_FILTER_COEFF)) * HES_AppData.CVT.filtered_vel_e;
    HES_AppData.CVT.filtered_vel_n = 
        (HES_VEL_FILTER_COEFF) * (HES_AppData.CVT.VGlobalPosition.VelN) +
        (1 - (HES_VEL_FILTER_COEFF)) * HES_AppData.CVT.filtered_vel_n;
    
    vel_ned[0] = HES_AppData.CVT.filtered_vel_n;
    vel_ned[1] = HES_AppData.CVT.filtered_vel_e;
    vel_ned[2] = HES_AppData.CVT.filtered_vel_d;

    HES_AppData.CVT.filtered_airspeed = 
        (HES_AIRSPEED_FILTER_COEFF) * HES_AppData.CVT.Airspeed.TrueAirspeed + 
        (1 - (HES_AIRSPEED_FILTER_COEFF)) * HES_AppData.CVT.filtered_airspeed;
    HES_AppData.HkTlm.airspeed[0] = HES_AppData.CVT.filtered_airspeed;
    HES_AppData.HkTlm.airspeed[1] = (1.943844) * HES_AppData.CVT.filtered_airspeed;
    HES_AppData.HkTlm.airspeed[2] = (3.6) * HES_AppData.CVT.filtered_airspeed;
    HES_AppData.HkTlm.airspeed[3] = (2.236936) * HES_AppData.CVT.filtered_airspeed;

    HES_AppData.CVT.filtered_groundspeed = 
        (HES_GROUNDSPEED_FILTER_COEFF) * (sqrtf(
            (HES_AppData.CVT.VGlobalPosition.VelN * HES_AppData.CVT.VGlobalPosition.VelN) + 
            (HES_AppData.CVT.VGlobalPosition.VelE * HES_AppData.CVT.VGlobalPosition.VelE))) + 
        (1 - (HES_GROUNDSPEED_FILTER_COEFF)) * HES_AppData.CVT.filtered_groundspeed;
    HES_AppData.HkTlm.groundspeed[0] = HES_AppData.CVT.filtered_groundspeed;
    HES_AppData.HkTlm.groundspeed[1] = (1.943844) * HES_AppData.CVT.filtered_groundspeed;
    HES_AppData.HkTlm.groundspeed[2] = (3.6) * HES_AppData.CVT.filtered_groundspeed;
    HES_AppData.HkTlm.groundspeed[3] = (2.236936) * HES_AppData.CVT.filtered_groundspeed;

    // HES_AppData.CVT.filtered_speed = 
    //     (HES_SPEED_FILTER_COEFF) * (sqrtf(
    //         (HES_AppData.CVT.VGlobalPosition.VelN * HES_AppData.CVT.VGlobalPosition.VelN) + 
    //         (HES_AppData.CVT.VGlobalPosition.VelE * HES_AppData.CVT.VGlobalPosition.VelE) + 
    //         (HES_AppData.CVT.VGlobalPosition.VelD * HES_AppData.CVT.VGlobalPosition.VelD))) + 
    //     (1 - (HES_SPEED_FILTER_COEFF)) * HES_AppData.CVT.filtered_speed;

    Quaternion_ToEuler(HES_AppData.CVT.VAtt.Q, euler);
    HES_AppData.CVT.filtered_euler[0] = (HES_EULER_FILTER_COEFF) * euler[0] + (1 - (HES_EULER_FILTER_COEFF)) * HES_AppData.CVT.filtered_euler[0];
    HES_AppData.CVT.filtered_euler[1] = (HES_EULER_FILTER_COEFF) * euler[1] + (1 - (HES_EULER_FILTER_COEFF)) * HES_AppData.CVT.filtered_euler[1];
    HES_AppData.CVT.filtered_euler[2] = (HES_EULER_FILTER_COEFF) * euler[2] + (1 - (HES_EULER_FILTER_COEFF)) * HES_AppData.CVT.filtered_euler[2];

    HES_AppData.HkTlm.rollDegrees = - (180/M_PI) * HES_AppData.CVT.filtered_euler[0];
    HES_AppData.HkTlm.pitchDegrees =  (180/M_PI) * HES_AppData.CVT.filtered_euler[1];
    HES_AppData.HkTlm.headingDegrees = (180/M_PI) * HES_AppData.CVT.filtered_euler[2];

    wind_n = 
        HES_AppData.CVT.filtered_vel_n
            - HES_AppData.CVT.filtered_airspeed*(cosf(HES_AppData.CVT.filtered_euler[2]))*(cosf(asinf(- (HES_AppData.CVT.filtered_vel_d) / (HES_AppData.CVT.filtered_airspeed))));
    wind_e = 
        HES_AppData.CVT.filtered_vel_e
            - HES_AppData.CVT.filtered_airspeed*(sinf(HES_AppData.CVT.filtered_euler[2]))*(cosf(asinf(- (HES_AppData.CVT.filtered_vel_d) / (HES_AppData.CVT.filtered_airspeed))));

    HES_AppData.HkTlm.windspeed[0] = sqrtf(wind_e*wind_e + wind_n*wind_n);
    HES_AppData.HkTlm.windspeed[1] = (1.943844) * HES_AppData.HkTlm.windspeed[0];
    HES_AppData.HkTlm.windspeed[2] = (3.6) * HES_AppData.HkTlm.windspeed[0];
    HES_AppData.HkTlm.windspeed[3] = (2.236936) * HES_AppData.HkTlm.windspeed[0];

    HES_AppData.HkTlm.winddirection[0] = atan2f(wind_e, wind_n);
    HES_AppData.HkTlm.winddirection[1] = (180/M_PI) * HES_AppData.HkTlm.winddirection[0];

    HES_AppData.HkTlm.groundtrackdirection[0] = atan2f(HES_AppData.CVT.filtered_vel_e, HES_AppData.CVT.filtered_vel_n);
    HES_AppData.HkTlm.groundtrackdirection[1] = (180/M_PI) * HES_AppData.HkTlm.groundtrackdirection[0];

    Quaternion_ConjugateInversed(vel_ned, HES_AppData.CVT.VAtt.Q, HES_AppData.HkTlm.vel_xyz);

    wind[0] = wind_n;
    wind[1] = wind_e;
    wind[2] = 0;
    Quaternion_ConjugateInversed(wind, HES_AppData.CVT.VAtt.Q, wind_body);

    v_r = HES_AppData.HkTlm.vel_xyz[1] - wind_body[1];

    if (HES_AppData.CVT.filtered_airspeed != 0) {
        HES_AppData.HkTlm.alpha[1] = HES_AppData.CVT.filtered_euler[1] - asinf(- (HES_AppData.CVT.filtered_vel_d) / (HES_AppData.CVT.filtered_airspeed));
        HES_AppData.HkTlm.alpha[0] = HES_AppData.HkTlm.alpha[1] * (180/M_PI);
        HES_AppData.HkTlm.beta[1] = asinf(v_r / HES_AppData.CVT.filtered_airspeed);
        HES_AppData.HkTlm.beta[0] = HES_AppData.HkTlm.beta[1] * (180/M_PI);
    } else {
        HES_AppData.HkTlm.alpha[0] = 0;
        HES_AppData.HkTlm.alpha[1] = 0;
        HES_AppData.HkTlm.beta[1] = 0;
        HES_AppData.HkTlm.beta[0] = 0;
    }

    HES_AppData.HkTlm.VoltageFiltered = HES_AppData.CVT.BatteryStatus.VoltageFiltered;
    HES_AppData.HkTlm.CurrentFiltered = HES_AppData.CVT.BatteryStatus.CurrentFiltered;
    HES_AppData.HkTlm.Discharged = HES_AppData.CVT.BatteryStatus.Discharged;
    HES_AppData.HkTlm.Remaining = HES_AppData.CVT.BatteryStatus.Remaining;
    HES_AppData.HkTlm.Warning = HES_AppData.CVT.BatteryStatus.Warning;

    HES_AppData.HkTlm.RcSignalLost = HES_AppData.CVT.VehicleStatus.RcSignalLost;
    HES_AppData.HkTlm.NavState = HES_AppData.CVT.VehicleStatus.NavState;
    HES_AppData.HkTlm.ArmingState = HES_AppData.CVT.VehicleStatus.ArmingState;
    HES_AppData.HkTlm.EngineFailure = HES_AppData.CVT.VehicleStatus.EngineFailure;
    HES_AppData.HkTlm.landed = HES_AppData.CVT.VLandDetected.Landed;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void HES_ProcessNewCmds()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr=NULL;
    CFE_SB_MsgId_t  CmdMsgId;

    /* Process command messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, HES_AppData.CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case HES_CMD_MID:
                    HES_ProcessNewAppCmds(CmdMsgPtr);
                    break;

                /* TODO:  Add code to process other subscribed commands here
                **
                ** Example:
                **     case CFE_TIME_DATA_CMD_MID:
                **         HES_ProcessTimeDataCmd(CmdMsgPtr);
                **         break;
                */

                default:
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with this
                     *  pipe, but not handled in this switch-case.) */
                    HES_AppData.HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(HES_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            (void) CFE_EVS_SendEvent(HES_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08X)", (unsigned int)iStatus);
            HES_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process HES Commands                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void HES_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case HES_NOOP_CC:
                HES_AppData.HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(HES_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "Recvd NOOP cmd (%u), Version %d.%d.%d.%d",
                                  (unsigned int)uiCmdCode,
                                  HES_MAJOR_VERSION,
                                  HES_MINOR_VERSION,
                                  HES_REVISION,
                                  HES_MISSION_REV);
                break;

            case HES_RESET_CC:
                HES_AppData.HkTlm.usCmdCnt = 0;
                HES_AppData.HkTlm.usCmdErrCnt = 0;
                (void) CFE_EVS_SendEvent(HES_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "Recvd RESET cmd (%u)", (unsigned int)uiCmdCode);
                break;

            /* TODO:  Add code to process the rest of the HES commands here */

            default:
                HES_AppData.HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(HES_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid cmdId (%u)", (unsigned int)uiCmdCode);
                break;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send HES Housekeeping                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void HES_ReportHousekeeping()
{
    /* TODO:  Add code to update housekeeping data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&HES_AppData.HkTlm);
    int32 iStatus = CFE_SB_SendMsg((CFE_SB_Msg_t*)&HES_AppData.HkTlm);
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

void HES_SendOutData()
{
    /* TODO:  Add code to update output data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&HES_AppData.OutData);
    int32 iStatus = CFE_SB_SendMsg((CFE_SB_Msg_t*)&HES_AppData.OutData);
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

boolean HES_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
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

            (void) CFE_EVS_SendEvent(HES_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            HES_AppData.HkTlm.usCmdErrCnt++;
        }
    }

    return (bResult);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* HES application entry point and main process loop               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void HES_AppMain()
{
    /* Register the application with Executive Services */
    HES_AppData.uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("HES - Failed to register the app (0x%08X)\n", (unsigned int)iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(HES_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = HES_InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(HES_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(HES_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(HES_MAIN_TASK_PERF_ID);
    }
    else
    {
        HES_AppData.uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&HES_AppData.uiRunStatus) == TRUE)
    {
        int32 iStatus = HES_RcvMsg(HES_SCH_PIPE_PEND_TIME);
        if (iStatus != CFE_SUCCESS)
        {
            /* TODO: Decide what to do for other return values in HES_RcvMsg(). */
        }
        /* TODO: This is only a suggestion for when to update and save CDS table.
        ** Depends on the nature of the application, the frequency of update
        ** and save can be more or less independently.
        */
        HES_UpdateCdsTbl();
        HES_SaveCdsTbl();

        iStatus = HES_AcquireConfigPointers();
        if(iStatus != CFE_SUCCESS)
        {
            /* We apparently tried to load a new table but failed.  Terminate the application. */
            HES_AppData.uiRunStatus = CFE_ES_APP_ERROR;
        }
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(HES_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(HES_AppData.uiRunStatus);
}

/************************/
/*  End of File Comment */
/************************/
