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
#include <time.h>

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
    //void *userDataPtr = 0;
    //void *copyDataPtr = 0;
    //uint16 userDataLength = 0;

    ///* Null check */
    //if(0 == Measure)
    //{
        //CFE_EVS_SendEvent(GPS_DEVICE_ERR_EID, CFE_EVS_ERROR,
                //"GPS measure dump message null pointer");
        //returnBool = FALSE;
        //goto end_of_function;
    //}

    //OS_MutSemTake(GPS_AppCustomData.MutexDump);
    //userDataPtr = CFE_SB_GetUserData((CFE_SB_MsgPtr_t)&GPS_AppCustomData.GpsDumpMsg);
    //userDataLength = CFE_SB_GetUserDataLength((CFE_SB_MsgPtr_t)Measure);
    //copyDataPtr = CFE_SB_GetUserData((CFE_SB_MsgPtr_t)Measure);
    //memcpy(copyDataPtr, userDataPtr, userDataLength);
    //OS_MutSemGive(GPS_AppCustomData.MutexDump);

//end_of_function:

    //return returnBool;
//}


boolean GPS_Custom_Measure_PositionMsg(PX4_VehicleGpsPositionMsg_t *Measure)
{
    boolean returnBool = TRUE;
    void *userDataPtr = 0;
    void *copyDataPtr = 0;
    uint16 userDataLength = 0;
    
    /* Null check */
    if(0 == Measure)
    {
        CFE_EVS_SendEvent(GPS_DEVICE_ERR_EID, CFE_EVS_ERROR,
                "GPS measure dump message null pointer");
        returnBool = FALSE;
        goto end_of_function;
    }

    OS_MutSemTake(GPS_AppCustomData.MutexPosition);
    //userDataPtr = CFE_SB_GetUserData((CFE_SB_MsgPtr_t)&GPS_AppCustomData.GpsPositionMsg);
    //userDataLength = CFE_SB_GetUserDataLength((CFE_SB_MsgPtr_t)Measure);
    //copyDataPtr = CFE_SB_GetUserData((CFE_SB_MsgPtr_t)Measure);
    
    //memcpy(copyDataPtr, userDataPtr, userDataLength);
    CFE_SB_CopyMsgTime((CFE_SB_MsgPtr_t)&Measure, (CFE_SB_MsgPtr_t)&GPS_AppCustomData.GpsPositionMsg);
    Measure->TimeUtc = GPS_AppCustomData.GpsPositionMsg.TimeUtc;
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
    boolean returnBool = FALSE;
    //void *userDataPtr = 0;
    //void *copyDataPtr = 0;
    //uint16 userDataLength = 0;
    ///* Null check */
    //if(0 == Measure)
    //{
        //CFE_EVS_SendEvent(GPS_DEVICE_ERR_EID, CFE_EVS_ERROR,
                //"GPS measure dump message null pointer");
        //returnBool = FALSE;
        //goto end_of_function;
    //}

    //OS_MutSemTake(GPS_AppCustomData.MutexSatInfo);
    //userDataPtr = CFE_SB_GetUserData((CFE_SB_MsgPtr_t)&GPS_AppCustomData.GpsSatInfoMsg);
    //userDataLength = CFE_SB_GetUserDataLength((CFE_SB_MsgPtr_t)Measure);
    //copyDataPtr = CFE_SB_GetUserData((CFE_SB_MsgPtr_t)Measure);
    
    //memcpy(copyDataPtr, userDataPtr, userDataLength);
    //OS_MutSemGive(GPS_AppCustomData.MutexSatInfo);

end_of_function:

    return returnBool;
}
