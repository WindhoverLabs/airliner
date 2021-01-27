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
#include "gps_custom_shared.h"
#include "px4_msgs.h"
#include "msg_ids.h"
#include "simlib.h"
#include "px4lib.h"

#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

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
GPS_AppCustomData_t GPS_AppCustomData;
/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Definitions
*************************************************************************/

void GPS_Custom_InitData(void)
{
    /* Set all struct zero values */
    bzero(&GPS_AppCustomData, sizeof(GPS_AppCustomData));
    //GPS_Parser_Reset();
    /* Set all non-zero values */
    GPS_AppCustomData.ContinueFlag          = TRUE;
    GPS_AppCustomData.Priority              = GPS_STREAMING_TASK_PRIORITY;
    GPS_AppCustomData.StreamingTask         = GPS_Stream_Task;
}

boolean GPS_Custom_Init()
{
    boolean returnBool = TRUE;
    int32 returnCode = 0;
    int32 Status = CFE_SUCCESS;
    
    Status = OS_MutSemCreate(&GPS_AppCustomData.MutexPosition, GPS_MUTEX_POS, 0);
    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(GPS_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "GPS mutex create failed in custom init");
        returnBool = FALSE;
        goto end_of_function;
    }
    Status = OS_MutSemCreate(&GPS_AppCustomData.MutexSatInfo, GPS_MUTEX_SAT, 0);
    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(GPS_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "GPS mutex create failed in custom init");
        returnBool = FALSE;
        goto end_of_function;
    }
    /* Create the streaming task */
    returnCode = CFE_ES_CreateChildTask(
            &GPS_AppCustomData.ChildTaskID,
            GPS_STREAMING_TASK_NAME,
            GPS_AppCustomData.StreamingTask,
            0,
            CFE_ES_DEFAULT_STACK_SIZE,
            GPS_AppCustomData.Priority,
            GPS_CUSTOM_CHILD_TASK_FLAGS);
    if(CFE_SUCCESS != returnCode)
    {
        GPS_AppCustomData.ContinueFlag = FALSE;
        returnBool = FALSE;
        CFE_EVS_SendEvent(GPS_DEVICE_ERR_EID, CFE_EVS_ERROR,
            "GPS create streaming task failed");
        goto end_of_function;
    }
    else
    {
        GPS_AppCustomData.Status = GPS_CUSTOM_INITIALIZED;
    }

end_of_function:
    return returnBool;
}


void GPS_Stream_Task(void)
{
    uint32 iStatus = -1;
    boolean returnBool = FALSE;
    
    iStatus = CFE_ES_RegisterChildTask();
    
    if (iStatus == CFE_SUCCESS)
    {
        GPS_AppCustomData.Status = GPS_CUSTOM_STREAMING;
        while (GPS_AppCustomData.ContinueFlag == TRUE)
        {
        	PX4_GpsFixType_t fixType = PX4_GPS_NO_FIX;
        	int32 latitude = 0;
        	int32 longitude = 0;
        	int32 altitude = 0;
        	uint16 eph = 0;
        	uint16 epv = 0;
        	uint16 velocity = 0;
			int16 vn = 0;
        	int16 ve = 0;
        	int16 vd = 0;
        	uint16 cog = 0;
        	uint8 satellitesVisible = 0;
        	uint64 timestamp = 0;
        	uint32 i = 0;

        	SIMLIB_GetGPS(
        			&fixType,
        			&latitude,
					&longitude,
        			&altitude,
        			&eph,
        			&epv,
        			&velocity,
        			&vn,
        			&ve,
        			&vd,
        			&cog,
        			&satellitesVisible);

        	OS_MutSemTake(GPS_AppCustomData.MutexSatInfo);

        	/* PX4_VehicleGpsPositionMsg_t */
        	timestamp = PX4LIB_GetPX4TimeUs();
        	GPS_AppCustomData.GpsPositionMsg.Timestamp = timestamp;
        	GPS_AppCustomData.GpsPositionMsg.TimeUtcUsec = 0;
        	GPS_AppCustomData.GpsPositionMsg.Lat = latitude;
        	GPS_AppCustomData.GpsPositionMsg.Lon = longitude;
        	GPS_AppCustomData.GpsPositionMsg.Alt = altitude;
        	GPS_AppCustomData.GpsPositionMsg.AltEllipsoid = 0;
        	GPS_AppCustomData.GpsPositionMsg.SVariance = 0.0f;
        	GPS_AppCustomData.GpsPositionMsg.CVariance = 0.0f;
        	GPS_AppCustomData.GpsPositionMsg.EpH = (float)eph * 0.01f;
        	GPS_AppCustomData.GpsPositionMsg.EpV =  (float)epv * 0.01f;
        	GPS_AppCustomData.GpsPositionMsg.HDOP = 0.0f;
        	GPS_AppCustomData.GpsPositionMsg.VDOP = 0.0f;
        	GPS_AppCustomData.GpsPositionMsg.NoisePerMs = 0;
        	GPS_AppCustomData.GpsPositionMsg.JammingIndicator = 0;
        	GPS_AppCustomData.GpsPositionMsg.Vel_m_s = (float)(velocity) / 100.0f;
        	GPS_AppCustomData.GpsPositionMsg.Vel_n_m_s = (float)(vn) / 100.0f;
        	GPS_AppCustomData.GpsPositionMsg.Vel_e_m_s = (float)(ve) / 100.0f;
        	GPS_AppCustomData.GpsPositionMsg.Vel_d_m_s = (float)(vd) / 100.0f;
        	GPS_AppCustomData.GpsPositionMsg.COG = (float)(cog) * 3.1415f / (100.0f * 180.0f);
        	GPS_AppCustomData.GpsPositionMsg.TimestampTimeRelative = 0;
        	GPS_AppCustomData.GpsPositionMsg.FixType = fixType;
        	GPS_AppCustomData.GpsPositionMsg.VelNedValid = FALSE;
        	GPS_AppCustomData.GpsPositionMsg.SatellitesUsed = satellitesVisible;

			OS_MutSemGive(GPS_AppCustomData.MutexSatInfo);

            GPS_EventDrivenPublish();

        	OS_TaskDelay(200);
        }
    }

    /* Streaming task is exiting so set app flag to initialized */
    GPS_AppCustomData.Status = GPS_CUSTOM_INITIALIZED;
    CFE_EVS_SendEvent(GPS_DEVICE_ERR_EID, CFE_EVS_ERROR,
        "GPS receive task exited with task status (0x%08lX)", iStatus);

    /* The child task was successfully registered so exit from it */
    if (iStatus == CFE_SUCCESS)
    {
        CFE_ES_ExitChildTask();
    }
}


boolean GPS_Custom_Uninit(void)
{
    boolean returnBool = TRUE;

    return returnBool;
}


boolean GPS_Custom_Max_Events_Not_Reached(int32 ind)
{
    if ((ind < CFE_EVS_MAX_EVENT_FILTERS) && (ind > 0))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


int32 GPS_Custom_Init_EventFilters(int32 ind, CFE_EVS_BinFilter_t *EventTbl)
{
    int32 customEventCount = ind;
    
    /* Null check */
    if(0 == EventTbl)
    {
        customEventCount = -1;
        goto end_of_function;
    }

    if(TRUE == GPS_Custom_Max_Events_Not_Reached(customEventCount))
    {
        EventTbl[  customEventCount].EventID = GPS_DEVICE_ERR_EID;
        EventTbl[customEventCount++].Mask    = CFE_EVS_FIRST_16_STOP;
    }
    else
    {
        customEventCount = -1;
        goto end_of_function;
    }
    
end_of_function:

    return customEventCount;
}


//boolean GPS_Custom_Measure_DumpMsg(PX4_GpsDumpMsg_t *Measure)
//{
    //boolean returnBool = TRUE;
    ///* Null check */
    //if(0 == Measure)
    //{
        //CFE_EVS_SendEvent(GPS_DEVICE_ERR_EID, CFE_EVS_ERROR,
                //"GPS measure dump message null pointer");
        //returnBool = FALSE;
        //goto end_of_function;
    //}

    //OS_MutSemTake(GPS_AppCustomData.MutexDump);
    //memcpy(Measure, &GPS_AppCustomData.GpsDumpMsg,
            //sizeof(GPS_AppCustomData.GpsDumpMsg));
    //OS_MutSemGive(GPS_AppCustomData.MutexDump);

//end_of_function:

    //return returnBool;
//}


boolean GPS_Custom_Measure_PositionMsg(PX4_VehicleGpsPositionMsg_t *Measure)
{
    boolean returnBool = TRUE;
    /* Null check */
    if(0 == Measure)
    {
        CFE_EVS_SendEvent(GPS_DEVICE_ERR_EID, CFE_EVS_ERROR,
                "GPS position message null pointer");
        returnBool = FALSE;
        goto end_of_function;
    }

    OS_MutSemTake(GPS_AppCustomData.MutexPosition);

    Measure->Timestamp = GPS_AppCustomData.GpsPositionMsg.Timestamp;
    Measure->TimeUtcUsec = GPS_AppCustomData.GpsPositionMsg.TimeUtcUsec;
    Measure->Lat = GPS_AppCustomData.GpsPositionMsg.Lat;
    Measure->Lon = GPS_AppCustomData.GpsPositionMsg.Lon;
    Measure->Alt = GPS_AppCustomData.GpsPositionMsg.Alt;
    Measure->AltEllipsoid = GPS_AppCustomData.GpsPositionMsg.AltEllipsoid;
    Measure->SVariance = GPS_AppCustomData.GpsPositionMsg.SVariance;
    Measure->CVariance = GPS_AppCustomData.GpsPositionMsg.CVariance;
    Measure->EpH = GPS_AppCustomData.GpsPositionMsg.EpH;
    Measure->EpV = GPS_AppCustomData.GpsPositionMsg.EpV;
    Measure->HDOP = GPS_AppCustomData.GpsPositionMsg.HDOP;
    Measure->VDOP = GPS_AppCustomData.GpsPositionMsg.VDOP;
    Measure->NoisePerMs = GPS_AppCustomData.GpsPositionMsg.NoisePerMs;
    Measure->JammingIndicator = GPS_AppCustomData.GpsPositionMsg.JammingIndicator;
    Measure->Vel_m_s = GPS_AppCustomData.GpsPositionMsg.Vel_m_s;
    Measure->Vel_n_m_s = GPS_AppCustomData.GpsPositionMsg.Vel_n_m_s;
    Measure->Vel_e_m_s = GPS_AppCustomData.GpsPositionMsg.Vel_e_m_s;
    Measure->Vel_d_m_s = GPS_AppCustomData.GpsPositionMsg.Vel_d_m_s;
    Measure->COG = GPS_AppCustomData.GpsPositionMsg.COG;
    Measure->TimestampTimeRelative = GPS_AppCustomData.GpsPositionMsg.TimestampTimeRelative;
    Measure->FixType = GPS_AppCustomData.GpsPositionMsg.FixType;
    Measure->VelNedValid = GPS_AppCustomData.GpsPositionMsg.VelNedValid;
    Measure->SatellitesUsed = GPS_AppCustomData.GpsPositionMsg.SatellitesUsed;

    OS_MutSemGive(GPS_AppCustomData.MutexPosition);

end_of_function:

    return returnBool;
}


boolean GPS_Custom_Measure_SatInfoMsg(PX4_SatelliteInfoMsg_t *Measure)
{
    boolean returnBool = TRUE;
    /* Null check */
    if(0 == Measure)
    {
        CFE_EVS_SendEvent(GPS_DEVICE_ERR_EID, CFE_EVS_ERROR,
                "GPS measure sat info null pointer");
        returnBool = FALSE;
        goto end_of_function;
    }

    OS_MutSemTake(GPS_AppCustomData.MutexSatInfo);
    memcpy(Measure, &GPS_AppCustomData.GpsSatInfoMsg,
            sizeof(GPS_AppCustomData.GpsSatInfoMsg));
    OS_MutSemGive(GPS_AppCustomData.MutexSatInfo);

end_of_function:

    return returnBool;
}



//boolean GPS_Custom_Measure_DumpMsg(PX4_GpsDumpMsg_t *Measure)
//{
    //boolean returnBool = TRUE;
    ///* Null check */
    //if(0 == Measure)
    //{
        //CFE_EVS_SendEvent(GPS_DEVICE_ERR_EID, CFE_EVS_ERROR,
                //"GPS measure dump message null pointer");
        //returnBool = FALSE;
        //goto end_of_function;
    //}

    //OS_MutSemTake(GPS_AppCustomData.MutexDump);
    //memcpy(Measure, &GPS_AppCustomData.GpsDumpMsg, 
            //sizeof(GPS_AppCustomData.GpsDumpMsg));
    //OS_MutSemGive(GPS_AppCustomData.MutexDump);

//end_of_function:

    //return returnBool;
//}
