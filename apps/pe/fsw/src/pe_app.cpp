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
** Includes
*************************************************************************/
#include <string.h>
#include "cfe.h"
#include "pe_app.h"
#include "pe_msg.h"
#include "pe_version.h"
#include "px4lib_msgids.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Instantiate the application object.                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
PE oPE;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Default constructor.                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
PE::PE()
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Destructor constructor.                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
PE::~PE()
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables.                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PE::InitEvent()
{
    int32  iStatus=CFE_SUCCESS;
    int32  ind = 0;

    CFE_EVS_BinFilter_t   EventTbl[PE_MAX_EVENT_FILTERS];

    /* Initialize the event filter table.
     * Note: 0 is the CFE_EVS_NO_FILTER mask and event 0 is reserved (not used) */
    CFE_PSP_MemSet(EventTbl, 0x00, sizeof(EventTbl));
    
    EventTbl[  ind].EventID = PE_RESERVED_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_LOCAL_ESTIMATOR_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_GLOBAL_ESTIMATOR_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_ESTIMATOR_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_BARO_FAULT_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_BARO_OK_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_BARO_TIMEOUT_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_GPS_TIMEOUT_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_GPS_FAULT_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_GPS_OK_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_LAND_FAULT_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_LAND_OK_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_LAND_TIMEOUT_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_DIST_FAULT_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_DIST_OK_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_DIST_TIMEOUT_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_MUTEX_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_LOCAL_POS_MSG_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_GLOBAL_POS_MSG_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_FUSE_DIST_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_FUSE_DIST_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_DISABLE_DIST_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_DISABLE_DIST_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_FUSE_GPS_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_FUSE_GPS_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_DISABLE_GPS_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_DISABLE_GPS_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_FUSE_BARO_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_FUSE_BARO_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_DISABLE_BARO_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_DISABLE_BARO_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_FUSE_LAND_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_FUSE_LAND_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_DISABLE_LAND_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_DISABLE_LAND_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_FUSE_FLOW_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_FUSE_FLOW_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_DISABLE_FLOW_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_DISABLE_FLOW_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_FLOW_FAULT_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_FLOW_OK_INF_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;
    EventTbl[  ind].EventID = PE_FLOW_TIMEOUT_ERR_EID;
    EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(EventTbl, ind, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("PE - Failed to register with EVS (0x%08lX)\n", iStatus);
    }

    return iStatus;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PE::InitPipe()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&SchPipeId,
            PE_SCH_PIPE_DEPTH,
            PE_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(PE_WAKEUP_MID, SchPipeId, CFE_SB_Default_Qos, PE_WAKEUP_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(PE_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                    "Sch Pipe failed to subscribe to PE_WAKEUP_MID. (0x%08lX)",
                    iStatus);
            goto PE_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(PE_SEND_HK_MID, SchPipeId, CFE_SB_Default_Qos, PE_SEND_HK_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(PE_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                     "CMD Pipe failed to subscribe to PE_SEND_HK_MID. (0x%08X)",
                     (unsigned int)iStatus);
            goto PE_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(PE_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
			 "Failed to create SCH pipe (0x%08lX)",
			 iStatus);
        goto PE_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&CmdPipeId,
    		PE_CMD_PIPE_DEPTH,
			PE_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(PE_CMD_MID, CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(PE_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
				 "CMD Pipe failed to subscribe to PE_CMD_MID. (0x%08lX)",
				 iStatus);
            goto PE_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(PE_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
			 "Failed to create CMD pipe (0x%08lX)",
			 iStatus);
        goto PE_InitPipe_Exit_Tag;
    }

    /* Init data pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&DataPipeId,
    		PE_DATA_PIPE_DEPTH,
			PE_DATA_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_GPS_POSITION_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(PE_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                     "DATA Pipe failed to subscribe to PX4_VEHICLE_GPS_POSITION_MID. (0x%08lX)",
                     iStatus);
            goto PE_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_STATUS_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(PE_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                     "DATA Pipe failed to subscribe to PX4_VEHICLE_STATUS_MID. (0x%08lX)",
                     iStatus);
            goto PE_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_LAND_DETECTED_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(PE_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                     "DATA Pipe failed to subscribe to PX4_VEHICLE_LAND_DETECTED_MID. (0x%08lX)",
                     iStatus);
            goto PE_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_ACTUATOR_ARMED_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(PE_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                     "DATA Pipe failed to subscribe to PX4_ACTUATOR_ARMED_MID. (0x%08lX)",
                     iStatus);
            goto PE_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_ATTITUDE_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(PE_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                     "DATA Pipe failed to subscribe to PX4_VEHICLE_ATTITUDE_MID. (0x%08lX)",
                     iStatus);
            goto PE_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_SENSOR_COMBINED_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(PE_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                     "DATA Pipe failed to subscribe to PX4_SENSOR_COMBINED_MID. (0x%08lX)",
                     iStatus);
            goto PE_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_VEHICLE_ATTITUDE_SETPOINT_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(PE_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                     "DATA Pipe failed to subscribe to PX4_VEHICLE_ATTITUDE_SETPOINT_MID. (0x%08lX)",
                     iStatus);
            goto PE_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_DISTANCE_SENSOR_MID, DataPipeId, CFE_SB_Default_Qos, 1);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(PE_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                     "DATA Pipe failed to subscribe to PX4_DISTANCE_SENSOR_MID. (0x%08lX)",
                     iStatus);
            goto PE_InitPipe_Exit_Tag;
        }
        iStatus = CFE_SB_SubscribeEx(PX4_OPTICAL_FLOW_MID, DataPipeId, CFE_SB_Default_Qos, 1);
		if (iStatus != CFE_SUCCESS)
		{
			(void) CFE_EVS_SendEvent(PE_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "DATA Pipe failed to subscribe to PX4_OPTICAL_FLOW_MID. (0x%08lX)",
					 iStatus);
			goto PE_InitPipe_Exit_Tag;
		}
    }
    else
    {
        (void) CFE_EVS_SendEvent(PE_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
			 "Failed to create DATA pipe (0x%08lX)",
			 iStatus);

        goto PE_InitPipe_Exit_Tag;
    }

PE_InitPipe_Exit_Tag:
    return (iStatus);
}
    

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PE::InitData()
{
    /* Init housekeeping message. */
    CFE_SB_InitMsg(&HkTlm, PE_HK_TLM_MID, sizeof(HkTlm), TRUE);
    /* Init output messages */
    /* Vehicle local position message*/
    CFE_SB_InitMsg(&m_VehicleLocalPositionMsg,
            PX4_VEHICLE_LOCAL_POSITION_MID, sizeof(PX4_VehicleLocalPositionMsg_t), TRUE);
    /* Estimator status message */
    CFE_SB_InitMsg(&m_EstimatorStatusMsg,
            PX4_ESTIMATOR_STATUS_MID, sizeof(PX4_EstimatorStatusMsg_t), TRUE);
    /* Vehicle global position message */
    CFE_SB_InitMsg(&m_VehicleGlobalPositionMsg,
            PX4_VEHICLE_GLOBAL_POSITION_MID, sizeof(PX4_VehicleGlobalPositionMsg_t), TRUE);

    /* Timestamps */
    m_Timestamp             = PX4LIB_GetPX4TimeUs();
    m_TimeLastBaro          = 0;
    m_TimeLastGps           = 0;
    m_TimeLastLand          = 0;
    m_TimeLastDist          = 0;
    m_TimeLastFlow          = 0;
    m_Timestamp_Hist        = 0;
    m_TimestampLastBaro     = 0;

    /* Timeouts */
    m_BaroTimeout           = TRUE;
    m_GpsTimeout            = TRUE;
    m_LandTimeout           = TRUE;
    m_DistTimeout           = TRUE;
    m_FlowTimeout           = TRUE;

    /* Faults */
    m_BaroFault             = TRUE;
    m_GpsFault              = TRUE;
    m_LandFault             = TRUE;
    m_DistFault             = TRUE;
    m_FlowFault             = TRUE;
    
    /* Fuse Flags */
    m_BaroFuse              = FALSE;
    m_GpsFuse               = FALSE;
    m_LandFuse              = FALSE;
    m_DistFuse              = FALSE;
    m_FlowFuse              = FALSE;

    /* Validity */
    m_XyEstValid            = FALSE;
    m_ZEstValid             = FALSE;
    m_TzEstValid            = FALSE;

    /* Reference altitudes */
    m_AltOrigin             = 0.0f;
    m_BaroAltOrigin         = 0.0f;
    m_GpsAltOrigin          = 0.0f;
    m_DistAltOrigin         = 0.0f;

    /* Status */
    m_EstimatorLocalInitialized   = FALSE;
    m_EstimatorGlobalInitialized  = FALSE;
    m_BaroInitialized             = FALSE;
    m_GpsInitialized              = FALSE;
    m_LandInitialized             = FALSE;
    m_DistInitialized             = FALSE;
    m_FlowInitialized             = FALSE;
    m_ReceivedGps                 = FALSE;
    m_LastArmedState              = FALSE;
    m_AltOriginInitialized        = FALSE;
    m_ParamsUpdated               = FALSE;

    /* Matrix/Vector Zero */
    m_RotationMat.Zero();
    m_Euler.Zero();
    
    /* Stats Zero */
    m_BaroStats.reset();
    m_GpsStats.reset();
    m_DistStats.reset();
    m_FlowQStats.reset();
    
    /* State Space */
    m_StateVec.Zero();
    m_InputVec.Zero();
    InitStateSpace();

    /* Map */
    m_MapRef.init_done      = FALSE;

    /* Sensor baro data */
    m_Baro.y.Zero();
    m_Baro.C.Zero();
    m_Baro.R.Zero();
    m_Baro.S_I.Zero();
    m_Baro.r.Zero();
    m_Baro.beta = 0.0f;
    m_Baro.K.Zero();
    m_Baro.temp.Zero();
    m_Baro.dx.Zero();
    
    /* Sensor GPS data */
    m_GPS.y_global.Zero();
    m_GPS.lat = 0.0f;
    m_GPS.lon = 0.0f;
    m_GPS.px = 0.0f;
    m_GPS.py = 0.0f;
    m_GPS.pz = 0.0f;
    m_GPS.y.Zero();
    m_GPS.C.Zero();
    m_GPS.R.Zero();
    m_GPS.var_xy = 0.0f;
    m_GPS.var_z = 0.0f;
    m_GPS.var_vxy = 0.0f;
    m_GPS.var_vz = 0.0f;
    m_GPS.gps_s_stddev = 0.0f;
    m_GPS.i_hist = 0;
    m_GPS.x0.Zero();
    m_GPS.r.Zero();
    m_GPS.S_I.Zero();
    m_GPS.rTranspose.Zero();
    m_GPS.beta = 0.0f;
    m_GPS.beta_thresh = 0.0f;
    m_GPS.K.Zero();
    m_GPS.dx.Zero();
    
    /* Sensor Land data */
    m_Land.y.Zero();
    m_Land.C.Zero();
    m_Land.R.Zero();
    m_Land.S_I.Zero();
    m_Land.r.Zero();
    m_Land.beta = 0.0f;
    m_Land.beta_thresh = 0.0f;
    m_Land.K.Zero();
    m_Land.dx.Zero();

    /* Sensor dist data */
    m_Dist.y.Zero();
    m_Dist.C.Zero();
    m_Dist.R.Zero();
    m_Dist.S_I.Zero();
    m_Dist.r.Zero();
    m_Dist.beta = 0.0f;
    m_Dist.K.Zero();
    m_Dist.temp.Zero();
    m_Dist.dx.Zero();

    /* Sensor flow data */
    m_Flow.y.Zero();
    m_Flow.C.Zero();
    m_Flow.R.Zero();
    m_Flow.S_I.Zero();
    m_Flow.r.Zero();
    m_Flow.beta = 0.0f;
    m_Flow.K.Zero();
    m_Flow.temp.Zero();
    m_Flow.dx.Zero();

    /* Predict data */
    m_Predict.q.Zero();
    m_Predict.a.Zero();
    m_Predict.k1.Zero();
    m_Predict.k2.Zero();
    m_Predict.k3.Zero();
    m_Predict.k4.Zero();
    m_Predict.dx.Zero();
    m_Predict.bx = 0.0f;
    m_Predict.by = 0.0f;
    m_Predict.bz = 0.0f;
    m_Predict.dP.Zero();

    /* Initialize delay blocks */
    m_XDelay.Initialize();
    m_TDelay.Initialize();
    
    /* Params */
    UpdateLocalParams();

}


void PE::initStateCov()
{
    m_StateCov.Zero();

    /* Initialize to twice valid condition. Note: We intialize with values from
    ** the configuration table. If a table is reloaded, we intentionally do not
    ** call this function again as this will reset the state covariance which 
    ** is updated in flight. */
    m_StateCov[X_x][X_x] = 2 * EST_STDDEV_XY_VALID * EST_STDDEV_XY_VALID;
    m_StateCov[X_y][X_y] = 2 * EST_STDDEV_XY_VALID * EST_STDDEV_XY_VALID;
    m_StateCov[X_z][X_z] = 2 * EST_STDDEV_Z_VALID * EST_STDDEV_Z_VALID;
    m_StateCov[X_vx][X_vx] = 2 * ConfigTblPtr->VXY_PUB_THRESH * ConfigTblPtr->VXY_PUB_THRESH;
    m_StateCov[X_vy][X_vy] = 2 * ConfigTblPtr->VXY_PUB_THRESH * ConfigTblPtr->VXY_PUB_THRESH;

    /* Use vxy threshold for vz init as well */
    m_StateCov[X_vz][X_vz] = 2 * ConfigTblPtr->VXY_PUB_THRESH * ConfigTblPtr->VXY_PUB_THRESH;

    /* Initialize bias uncertainty to small values to keep them stable */
    m_StateCov[X_bx][X_bx] = 1e-6;
    m_StateCov[X_by][X_by] = 1e-6;
    m_StateCov[X_bz][X_bz] = 1e-6;
    m_StateCov[X_tz][X_tz] = 2 * EST_STDDEV_TZ_VALID * EST_STDDEV_TZ_VALID;
}


void PE::InitStateSpace()
{
    initStateCov();

    /* Derivative of position is velocity */
    m_DynamicsMat.Zero();
    m_DynamicsMat[X_x][X_vx] = 1;
    m_DynamicsMat[X_y][X_vy] = 1;
    m_DynamicsMat[X_z][X_vz] = 1;

    /* Input matrix */
    m_InputMat.Zero();
    m_InputMat[X_vx][U_ax] = 1;
    m_InputMat[X_vy][U_ay] = 1;
    m_InputMat[X_vz][U_az] = 1;

    /* Update components that depend on current state */
    updateStateSpace();
    updateStateSpaceParams();
}


void PE::updateStateSpace()
{
    /* Derivative of velocity is accelerometer acceleration
     * (in input matrix) - bias (in body frame) */
    m_DynamicsMat[X_vx][X_bx] = -m_RotationMat[0][0];
    m_DynamicsMat[X_vx][X_by] = -m_RotationMat[0][1];
    m_DynamicsMat[X_vx][X_bz] = -m_RotationMat[0][2];

    m_DynamicsMat[X_vy][X_bx] = -m_RotationMat[1][0];
    m_DynamicsMat[X_vy][X_by] = -m_RotationMat[1][1];
    m_DynamicsMat[X_vy][X_bz] = -m_RotationMat[1][2];

    m_DynamicsMat[X_vz][X_bx] = -m_RotationMat[2][0];
    m_DynamicsMat[X_vz][X_by] = -m_RotationMat[2][1];
    m_DynamicsMat[X_vz][X_bz] = -m_RotationMat[2][2];
}


void PE::updateStateSpaceParams()
{
    /* Input noise covariance matrix */
    m_InputCov.Zero();
    m_InputCov[U_ax][U_ax] = ConfigTblPtr->ACCEL_XY_STDDEV * ConfigTblPtr->ACCEL_XY_STDDEV;
    m_InputCov[U_ay][U_ay] = ConfigTblPtr->ACCEL_XY_STDDEV * ConfigTblPtr->ACCEL_XY_STDDEV;
    m_InputCov[U_az][U_az] = ConfigTblPtr->ACCEL_Z_STDDEV * ConfigTblPtr->ACCEL_Z_STDDEV;

    /* Process noise power matrix */
    m_NoiseCov.Zero();
    float pn_p_sq = ConfigTblPtr->PN_P_NOISE_DENSITY * ConfigTblPtr->PN_P_NOISE_DENSITY;
    float pn_v_sq = ConfigTblPtr->PN_V_NOISE_DENSITY * ConfigTblPtr->PN_V_NOISE_DENSITY;
    m_NoiseCov[X_x][X_x] = pn_p_sq;
    m_NoiseCov[X_y][X_y] = pn_p_sq;
    m_NoiseCov[X_z][X_z] = pn_p_sq;
    m_NoiseCov[X_vx][X_vx] = pn_v_sq;
    m_NoiseCov[X_vy][X_vy] = pn_v_sq;
    m_NoiseCov[X_vz][X_vz] = pn_v_sq;

    /* Technically the noise is in the body frame,
    * but the components are all the same, so
    * ignoring for now */
    float pn_b_sq = ConfigTblPtr->PN_B_NOISE_DENSITY * ConfigTblPtr->PN_B_NOISE_DENSITY;
    m_NoiseCov[X_bx][X_bx] = pn_b_sq;
    m_NoiseCov[X_by][X_by] = pn_b_sq;
    m_NoiseCov[X_bz][X_bz] = pn_b_sq;

    /* Terrain random walk noise ((m/s)/sqrt(hz)), scales with velocity */
    float pn_t_noise_density = ConfigTblPtr->PN_T_NOISE_DENSITY +
        (ConfigTblPtr->T_MAX_GRADE / 100.0f) * sqrtf((m_StateVec[X_vx] * 
        m_StateVec[X_vx]) + (m_StateVec[X_vy] * m_StateVec[X_vy]));
    m_NoiseCov[X_tz][X_tz] = pn_t_noise_density * pn_t_noise_density;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* PE initialization                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PE::InitApp()
{
    int32  iStatus   = CFE_SUCCESS;
    int8   hasEvents = 0;

    iStatus = InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("PE - Failed to init events (0x%08lX)\n", iStatus);
        goto PE_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = OS_MutSemCreate(&ConfigMutex, PE_PARAMS_MUTEX, 0);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(PE_MUTEX_ERR_EID, CFE_EVS_ERROR,
                                 "PE - mutex creation failed (0x%08lX)", iStatus);
        goto PE_InitApp_Exit_Tag;
    }

    iStatus = InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        goto PE_InitApp_Exit_Tag;
    }


    iStatus = InitConfigTbl();
    if (iStatus != CFE_SUCCESS)
    {
        goto PE_InitApp_Exit_Tag;
    }

    InitData();

PE_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(PE_INIT_INF_EID, CFE_EVS_INFORMATION,
                                 "Initialized.  Version %d.%d.%d.%d",
                                 PE_MAJOR_VERSION,
                                 PE_MINOR_VERSION,
                                 PE_REVISION,
                                 PE_MISSION_REV);
    }
    else
    {
        if (hasEvents == 1)
        {
            (void) CFE_ES_WriteToSysLog("PE - Application failed to initialize\n");
        }
    }

    return iStatus;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 PE::RcvSchPipeMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    uint64          baroTimestamp = 0;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(PE_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(PE_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
        {
            /* Cyclic op - 125hz */
            case PE_WAKEUP_MID:
            	HkTlm.WakeupCount++;
            	if(ProcessDataPipe())
                {
            		Update();
                }
                break;

            case PE_SEND_HK_MID:
                ProcessCmdPipe();
                ReportHousekeeping();
                break;

            default:
                (void) CFE_EVS_SendEvent(PE_MSGID_ERR_EID, CFE_EVS_ERROR,
                     "Recvd invalid SCH msgId (0x%04X)", MsgId);
        }
    }
    else if (iStatus == CFE_SB_NO_MESSAGE)
    {
        iStatus = CFE_SUCCESS;
    }
    else if (iStatus == CFE_SB_TIME_OUT)
    {
        iStatus = CFE_SUCCESS;
    }
    else
    {
        (void) CFE_EVS_SendEvent(PE_RCVMSG_ERR_EID, CFE_EVS_ERROR,
			  "SCH pipe read error (0x%08lX).", iStatus);
    }

    return iStatus;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Data                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

osalbool PE::ProcessDataPipe()
{
    int32 iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Process command messages until the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&MsgPtr, DataPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            MsgId = CFE_SB_GetMsgId(MsgPtr);
            switch (MsgId)
            {
			    case PX4_VEHICLE_GPS_POSITION_MID:
			    {
            	    HkTlm.VehicleGpsPositionMsgCount++;
				    CFE_PSP_MemCpy(&m_VehicleGpsPositionMsg, MsgPtr, sizeof(m_VehicleGpsPositionMsg));

                    /* Check if fusing distance sensor */
                    if(TRUE == m_GpsFuse)
				    {
					    if(m_GpsTimeout)
					    {
						    gpsInit();
					    }
					    else
					    {
						    gpsCorrect();
					    }
				    }
				    break;
			    }

			    case PX4_VEHICLE_STATUS_MID:
			    {
            	    HkTlm.VehicleStatusMsgCount++;
				    CFE_PSP_MemCpy(&m_VehicleStatusMsg, MsgPtr, sizeof(m_VehicleStatusMsg));
				    break;
			    }

    			case PX4_VEHICLE_LAND_DETECTED_MID:
    			{
                	HkTlm.VehicleLandDetectedMsgCount++;
		    		CFE_PSP_MemCpy(&m_VehicleLandDetectedMsg, MsgPtr, sizeof(m_VehicleLandDetectedMsg));

				    /* Check if fusing land */
				    if(TRUE == m_LandFuse)
				    {
					    if(landed())
					    {
						    /* Throttle rate */
						    if((m_Timestamp - m_TimeLastLand) > 1.0e6f / LAND_RATE)
						    {
							    if(m_LandTimeout)
							    {
								    landInit();
							    }
							    else
							    {
								    landCorrect();
							    }
						    }
					    }
				    }
				    break;
    			}

			    case PX4_ACTUATOR_ARMED_MID:
			    {
            	    HkTlm.ActuatorArmedMsgCount++;
				    CFE_PSP_MemCpy(&m_ActuatorArmedMsg, MsgPtr, sizeof(m_ActuatorArmedMsg));
				    break;
			    }

			    case PX4_VEHICLE_ATTITUDE_MID:
		        {
            	    HkTlm.VehicleAttitudeMsgCount++;
				    CFE_PSP_MemCpy(&m_VehicleAttitudeMsg, MsgPtr, sizeof(m_VehicleAttitudeMsg));
				    break;
			    }

			    case PX4_SENSOR_COMBINED_MID:
			    {
            	    HkTlm.SensorCombinedMsgCount++;
				    /* Don't prevent memcpy - we fuse accel every cycle */
				    CFE_PSP_MemCpy(&m_SensorCombinedMsg, MsgPtr, sizeof(m_SensorCombinedMsg));

				    /* Check if fusing baro */
				    if(TRUE == m_BaroFuse)
				    {
					    /* If baro is valid */
					    if(m_SensorCombinedMsg.BaroTimestampRelative != PX4_RELATIVE_TIMESTAMP_INVALID)
					    {
						    /* Get the baro timestamp from the sensor combined timestamp
						     * (which is the gyro timestamp) plus the baro relative timestamp.
						     * Baro relative is the difference between the gyro and baro
						     * when received by the sensors application. If baro is fresh.
						     */
						    if((m_SensorCombinedMsg.Timestamp +
						       m_SensorCombinedMsg.BaroTimestampRelative)
						       != m_TimeLastBaro)
						    {
							    if(m_BaroTimeout)
							    {
							    	baroInit();
							    }
							    else
							    {
							    	baroCorrect();
							    }
							    /* Save the last valid timestamp */
							    m_TimeLastBaro = m_SensorCombinedMsg.Timestamp + m_SensorCombinedMsg.BaroTimestampRelative;
						    }
					    }
				    }
				    break;
			    }

			    case PX4_VEHICLE_ATTITUDE_SETPOINT_MID:
            	{
            	    HkTlm.VehicleAttitudeSetpointMsgCount++;
				    CFE_PSP_MemCpy(&m_VehicleAttitudeSetpointMsg, MsgPtr, sizeof(m_VehicleAttitudeSetpointMsg));
				    break;
			    }

			    case PX4_DISTANCE_SENSOR_MID:
			    {
            	    HkTlm.DistanceSensorMsgCount++;
				    CFE_PSP_MemCpy(&m_DistanceSensor, MsgPtr, sizeof(m_DistanceSensor));

				    /* Check that distance sensor is expected type for this platform */
				    if(m_DistanceSensor.Type == DIST_SENSOR_TYPE)
				    {
					    /* Check if fusing distance sensor */
					    if(TRUE == m_DistFuse)
					    {
						    /* Don't integrate while landed */
						    if(!landed())
						    {
							    /* Throttle rate */
							    if((m_Timestamp - m_TimeLastLand) > 1.0e6f / DIST_RATE)
							    {
								    if(m_DistTimeout)
								    {
									    distInit();
								    }
								    else
								    {
									    distCorrect();
								    }
							    }
						    }
					    }
				    }
				    break;
			    }

			    case PX4_OPTICAL_FLOW_MID:
		        {
            	    HkTlm.OpticalFlowMsgCount++;
				    CFE_PSP_MemCpy(&m_OpticalFlowMsg, MsgPtr, sizeof(m_OpticalFlowMsg));

				    /* Check if fusing distance sensor */
				    if(TRUE == m_FlowFuse)
				    {
					    if(m_FlowTimeout)
					    {
						    flowInit();
					    }
					    else
					    {
						    flowCorrect();
					    }
				    }
				    break;
		        }

                default:
		        {
                    (void) CFE_EVS_SendEvent(PE_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid DATA msgId (0x%04X)", (unsigned short)MsgId);
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
            (void) CFE_EVS_SendEvent(PE_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                  "DATA pipe read error (0x%08lX)", iStatus);
            break;
        }
    }

    return true;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void PE::ProcessCmdPipe()
{
    int32 iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr=NULL;
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
                case PE_CMD_MID:
                    ProcessAppCmds(CmdMsgPtr);
                    break;

                default:
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with this
                     *  pipe, but not handled in this switch-case.) */
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(PE_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            (void) CFE_EVS_SendEvent(PE_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08lX)", iStatus);
            break;
        }
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process PE Commands                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void PE::ProcessAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case PE_NOOP_CC:
                HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(PE_CMD_NOOP_EID, CFE_EVS_INFORMATION,
                    "Recvd NOOP. Version %d.%d.%d.%d",
                    PE_MAJOR_VERSION,
                    PE_MINOR_VERSION,
                    PE_REVISION,
                    PE_MISSION_REV);
                break;

            case PE_RESET_CC:
                HkTlm.usCmdCnt = 0;
                HkTlm.usCmdErrCnt = 0;
            	HkTlm.WakeupCount = 0;
                HkTlm.VehicleGpsPositionMsgCount = 0;
                HkTlm.VehicleStatusMsgCount = 0;
                HkTlm.VehicleLandDetectedMsgCount = 0;
                HkTlm.ActuatorArmedMsgCount = 0;
                HkTlm.VehicleAttitudeMsgCount = 0;
                HkTlm.SensorCombinedMsgCount = 0;
                HkTlm.VehicleAttitudeSetpointMsgCount = 0;
                HkTlm.DistanceSensorMsgCount = 0;
                HkTlm.OpticalFlowMsgCount = 0;
                break;

            case PE_FUSE_DIST_SENS_CC:
                if(FALSE == m_DistFuse)
                {
                    m_DistFuse = TRUE;
                    HkTlm.usCmdCnt++;
                    (void) CFE_EVS_SendEvent(PE_FUSE_DIST_INF_EID, CFE_EVS_INFORMATION,
                                  "Fusing distance sensor into estimation.");
                }
                else
                {
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(PE_FUSE_DIST_ERR_EID, CFE_EVS_ERROR,
                                  "Already fusing distance sensor into estimation.");
                }
                break;

            case PE_DISABLE_DIST_SENS_CC:
                if(TRUE == m_DistFuse)
                {
                    m_DistFuse = FALSE;
                    HkTlm.usCmdCnt++;
                    (void) CFE_EVS_SendEvent(PE_DISABLE_DIST_INF_EID, CFE_EVS_INFORMATION,
                                  "Disabling distance sensor fusion into estimation.");
                }
                else
                {
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(PE_DISABLE_DIST_ERR_EID, CFE_EVS_ERROR,
                                  "Failed to disable distance sensor fusion into estimation.");
                }
                break;

            case PE_FUSE_GPS_CC:
                if(FALSE == m_GpsFuse)
                {
                    m_GpsFuse = TRUE;
                    HkTlm.usCmdCnt++;
                    (void) CFE_EVS_SendEvent(PE_FUSE_GPS_INF_EID, CFE_EVS_INFORMATION,
                                  "Fusing GPS into estimation.");
                }
                else
                {
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(PE_FUSE_GPS_ERR_EID, CFE_EVS_ERROR,
                                  "Already fusing GPS into estimation.");
                }
                break;

            case PE_DISABLE_GPS_CC:
                if(TRUE == m_GpsFuse)
                {
                    m_GpsFuse = FALSE;
                    HkTlm.usCmdCnt++;
                    (void) CFE_EVS_SendEvent(PE_DISABLE_GPS_INF_EID, CFE_EVS_INFORMATION,
                                  "Disabling GPS fusion into estimation.");
                }
                else
                {
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(PE_DISABLE_GPS_ERR_EID, CFE_EVS_ERROR,
                                  "Failed to disable GPS fusion into estimation.");
                }
                break;

            case PE_FUSE_BARO_CC:
                if(FALSE == m_BaroFuse)
                {
                    m_BaroFuse = TRUE;
                    HkTlm.usCmdCnt++;
                    (void) CFE_EVS_SendEvent(PE_FUSE_BARO_INF_EID, CFE_EVS_INFORMATION,
                                  "Fusing baro into estimation.");
                }
                else
                {
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(PE_FUSE_BARO_ERR_EID, CFE_EVS_ERROR,
                                  "Already fusing baro into estimation.");
                }
                break;

            case PE_DISABLE_BARO_CC:
                if(TRUE == m_BaroFuse)
                {
                    m_BaroFuse = FALSE;
                    HkTlm.usCmdCnt++;
                    (void) CFE_EVS_SendEvent(PE_DISABLE_BARO_INF_EID, CFE_EVS_INFORMATION,
                                  "Disabling baro fusion into estimation.");
                }
                else
                {
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(PE_DISABLE_BARO_ERR_EID, CFE_EVS_ERROR,
                                  "Failed to disable baro fusion into estimation.");
                }
                break;

            case PE_FUSE_LAND_CC:
                if(FALSE == m_LandFuse)
                {
                    m_LandFuse = TRUE;
                    HkTlm.usCmdCnt++;
                    (void) CFE_EVS_SendEvent(PE_FUSE_LAND_INF_EID, CFE_EVS_INFORMATION,
                                  "Fusing land into estimation.");
                }
                else
                {
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(PE_FUSE_LAND_ERR_EID, CFE_EVS_ERROR,
                                  "Already fusing land into estimation.");
                }
                break;

            case PE_DISABLE_LAND_CC:
                if(TRUE == m_LandFuse)
                {
                    m_LandFuse = FALSE;
                    HkTlm.usCmdCnt++;
                    (void) CFE_EVS_SendEvent(PE_DISABLE_LAND_INF_EID, CFE_EVS_INFORMATION,
                                  "Disabling land fusion into estimation.");
                }
                else
                {
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(PE_DISABLE_LAND_ERR_EID, CFE_EVS_ERROR,
                                  "Failed to disable land fusion into estimation.");
                }
                break;

            case PE_FUSE_FLOW_CC:
                if(FALSE == m_FlowFuse)
                {
                    m_FlowFuse = TRUE;
                    HkTlm.usCmdCnt++;
                    (void) CFE_EVS_SendEvent(PE_FUSE_FLOW_INF_EID, CFE_EVS_INFORMATION,
                                  "Fusing flow into estimation.");
                }
                else
                {
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(PE_FUSE_FLOW_ERR_EID, CFE_EVS_ERROR,
                                  "Already fusing flow into estimation.");
                }
                break;

            case PE_DISABLE_FLOW_CC:
                if(TRUE == m_FlowFuse)
                {
                    m_FlowFuse = FALSE;
                    HkTlm.usCmdCnt++;
                    (void) CFE_EVS_SendEvent(PE_DISABLE_FLOW_INF_EID, CFE_EVS_INFORMATION,
                                  "Disabling flow fusion into estimation.");
                }
                else
                {
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(PE_DISABLE_FLOW_ERR_EID, CFE_EVS_ERROR,
                                  "Failed to disable flow fusion into estimation.");
                }
                break;

            default:
                HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(PE_CC_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid command code (%u)", (unsigned int)uiCmdCode);
                break;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send PE Housekeeping                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void PE::ReportHousekeeping()
{
    HkTlm.EstimatorLocalInitialized = m_EstimatorLocalInitialized;
    HkTlm.EstimatorGlobalInitialized = m_EstimatorGlobalInitialized;
    HkTlm.XyEstValid = m_XyEstValid;
    HkTlm.ZEstValid = m_ZEstValid;
    HkTlm.TzEstValid = m_TzEstValid;
    HkTlm.Timestamp = m_Timestamp;
    HkTlm.TimeLastBaro = m_TimeLastBaro;
    HkTlm.TimeLastGps = m_TimeLastGps;
    HkTlm.TimeLastLand = m_TimeLastLand;
    HkTlm.BaroTimeout = m_BaroTimeout;
    HkTlm.GpsTimeout = m_GpsTimeout;
    HkTlm.LandTimeout = m_LandTimeout;
    HkTlm.BaroFault = m_BaroFault;
    HkTlm.GpsFault = m_GpsFault;
    HkTlm.LandFault = m_LandFault;
    HkTlm.AltOrigin = m_AltOrigin;
    HkTlm.AltOriginInitialized = m_AltOriginInitialized;
    HkTlm.BaroAltOrigin = m_BaroAltOrigin;
    HkTlm.GpsAltOrigin = m_GpsAltOrigin;
    HkTlm.GpsInitialized = m_GpsInitialized;
    HkTlm.BaroInitialized = m_BaroInitialized;
    HkTlm.LandInitialized = m_LandInitialized;
    HkTlm.m_DistAltOrigin = m_DistAltOrigin;
    HkTlm.DistInitialized = m_DistInitialized;
    HkTlm.DistFault = m_DistFault;
    HkTlm.DistTimeout = m_DistTimeout;
    HkTlm.TimeLastDist = m_TimeLastDist;
    HkTlm.DistFused = m_DistFuse;
    HkTlm.GpsFused = m_GpsFuse;
    HkTlm.BaroFused = m_BaroFuse;
    HkTlm.LandFused = m_LandFuse;
    HkTlm.FlowFused = m_FlowFuse;
    HkTlm.FlowInitialized = m_FlowInitialized;
    HkTlm.FlowFault = m_FlowFault;
    HkTlm.FlowTimeout = m_FlowTimeout;
    HkTlm.TimeLastFlow = m_TimeLastFlow;

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&HkTlm);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Publish Output Data                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PE::SendVehicleLocalPositionMsg()
{
    float alt                    = 0.0f;
    float vxy_stddev             = 0.0f;
    float epv                    = 0.0f;
    float eph                    = 0.0f;
    float eph_thresh             = EPH_THRESH;
    float epv_thresh             = EPV_THRESH;
    osalbool data_valid          = FALSE;
    osalbool dist_bottom_valid   = FALSE;

    vxy_stddev = sqrtf(m_StateCov[X_vx][X_vx] + m_StateCov[X_vy][X_vy]);
    epv = sqrt(m_StateCov[X_z][X_z]);
    eph = sqrt(m_StateCov[X_x][X_x] + m_StateCov[X_y][X_y]);
    dist_bottom_valid = m_DistFuse && 
                        m_DistInitialized && 
                        !m_DistTimeout && 
                        !m_DistFault;

    if (vxy_stddev < ConfigTblPtr->VXY_PUB_THRESH)
    {
        if (eph > eph_thresh)
        {
            eph = eph_thresh;
        }

        if (epv > epv_thresh)
        {
            epv = epv_thresh;
        }
    }

    if( isfinite(m_XLowPass[X_vx]) &&
        isfinite(m_XLowPass[X_vy]) &&
        isfinite(m_XLowPass[X_vz]) &&
        isfinite(m_XLowPass[X_x]) &&
        isfinite(m_XLowPass[X_y]) &&
        isfinite(m_XLowPass[X_z]))
    {
        data_valid = TRUE;
    }

    if(data_valid)
    {
        m_VehicleLocalPositionMsg.Timestamp = m_Timestamp;
        m_VehicleLocalPositionMsg.XY_Valid = m_XyEstValid;
        m_VehicleLocalPositionMsg.Z_Valid = m_ZEstValid;
        m_VehicleLocalPositionMsg.V_XY_Valid = m_XyEstValid;
        m_VehicleLocalPositionMsg.V_Z_Valid = m_ZEstValid;
        m_VehicleLocalPositionMsg.X = m_XLowPass[X_x];
        m_VehicleLocalPositionMsg.Y = m_XLowPass[X_y];
        m_VehicleLocalPositionMsg.Z = -m_AglLowPass.m_State;
        m_VehicleLocalPositionMsg.VX = m_XLowPass[X_vx];
        m_VehicleLocalPositionMsg.VY = m_XLowPass[X_vy];
        m_VehicleLocalPositionMsg.VZ = m_XLowPass[X_vz];
        m_VehicleLocalPositionMsg.Yaw = m_Euler[2];
        m_VehicleLocalPositionMsg.XY_Global = m_XyEstValid;
        m_VehicleLocalPositionMsg.Z_Global = !m_BaroTimeout;
        m_VehicleLocalPositionMsg.RefTimestamp = m_Timestamp;
        m_VehicleLocalPositionMsg.RefLat = m_MapRef.lat_rad * 180/M_PI;
        m_VehicleLocalPositionMsg.RefLon = m_MapRef.lon_rad * 180/M_PI;
        m_VehicleLocalPositionMsg.RefAlt = m_AltOrigin;
        m_VehicleLocalPositionMsg.DistBottom = m_AglLowPass.m_State;
        m_VehicleLocalPositionMsg.DistBottomRate = m_XLowPass[X_vz];
        m_VehicleLocalPositionMsg.SurfaceBottomTimestamp = m_Timestamp;
        m_VehicleLocalPositionMsg.DistBottomValid = dist_bottom_valid;
        m_VehicleLocalPositionMsg.EpH = eph;
        m_VehicleLocalPositionMsg.EpV = epv;

        CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&m_VehicleLocalPositionMsg);
        CFE_SB_SendMsg((CFE_SB_Msg_t*)&m_VehicleLocalPositionMsg);
    }
    else
    {
        (void) CFE_EVS_SendEvent(PE_LOCAL_POS_MSG_ERR_EID, CFE_EVS_ERROR,
                                 "Local position message data invalid");
    }
}

void PE::SendVehicleGlobalPositionMsg()
{
    double lat          = 0.0f;
    double lon          = 0.0f;
    float alt           = 0.0f;
    float vxy_stddev    = 0.0f;
    float epv           = 0.0f;
    float eph           = 0.0f;
    osalbool data_valid = FALSE;

    map_projection_reproject(&m_MapRef, m_XLowPass[X_x], m_XLowPass[X_y], &lat, &lon);

    alt = -m_XLowPass[X_z] + m_AltOrigin;
    vxy_stddev = sqrtf(m_StateCov[X_vx][X_vx] + m_StateCov[X_vy][X_vy]);
    epv = sqrt(m_StateCov[X_z][X_z]);
    eph = sqrt(m_StateCov[X_x][X_x] + m_StateCov[X_y][X_y]);

    if (vxy_stddev < ConfigTblPtr->VXY_PUB_THRESH)
    {
        if (eph > EPH_THRESH)
        {
            eph = EPH_THRESH;
        }

        if (epv > EPV_THRESH)
        {
            epv = EPV_THRESH;
        }
    }

    if( isfinite(lat) &&
        isfinite(lon) &&
        isfinite(alt) &&
        isfinite(m_XLowPass[X_vx]) &&
        isfinite(m_XLowPass[X_vy]) &&
        isfinite(m_XLowPass[X_vz]))
    {
        data_valid = TRUE;
    }

    if(data_valid)
    {
        m_VehicleGlobalPositionMsg.Timestamp = m_Timestamp;
        m_VehicleGlobalPositionMsg.TimeUtcUsec = m_VehicleGpsPositionMsg.TimeUtcUsec;
        m_VehicleGlobalPositionMsg.Lat = lat;
        m_VehicleGlobalPositionMsg.Lon = lon;
        m_VehicleGlobalPositionMsg.Alt = alt;
        m_VehicleGlobalPositionMsg.VelN = m_XLowPass[X_vx];
        m_VehicleGlobalPositionMsg.VelE = m_XLowPass[X_vy];
        m_VehicleGlobalPositionMsg.VelD = m_XLowPass[X_vz];
        m_VehicleGlobalPositionMsg.Yaw = m_Euler[2];
        m_VehicleGlobalPositionMsg.EpH = eph;
        m_VehicleGlobalPositionMsg.EpV = epv;
        m_VehicleGlobalPositionMsg.TerrainAlt = m_AltOrigin - m_XLowPass[X_tz];
        m_VehicleGlobalPositionMsg.TerrainAltValid = m_TzEstValid;
        m_VehicleGlobalPositionMsg.DeadReckoning = !m_XyEstValid;
        m_VehicleGlobalPositionMsg.PressureAlt = m_SensorCombinedMsg.BaroAlt;

        CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&m_VehicleGlobalPositionMsg);
        CFE_SB_SendMsg((CFE_SB_Msg_t*)&m_VehicleGlobalPositionMsg);
    }
    else
    {
        (void) CFE_EVS_SendEvent(PE_GLOBAL_POS_MSG_ERR_EID, CFE_EVS_ERROR,
                                 "Global position message data invalid");
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Command Length                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool PE::VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
                           uint16 usExpectedLen)
{
    osalbool bResult = TRUE;
    uint16  usMsgLen = 0;

    if (MsgPtr != NULL)
    {
        usMsgLen = CFE_SB_GetTotalMsgLength(MsgPtr);

        if (usExpectedLen != usMsgLen)
        {
            bResult = FALSE;
            CFE_SB_MsgId_t MsgId = CFE_SB_GetMsgId(MsgPtr);
            uint16 usCmdCode = CFE_SB_GetCmdCode(MsgPtr);

            (void) CFE_EVS_SendEvent(PE_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            HkTlm.usCmdErrCnt++;
        }
    }

    return bResult;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* PE Application C style main entry point.                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
extern "C" void PE_AppMain()
{
    oPE.AppMain();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* PE Application C++ style main entry point.                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PE::AppMain()
{
    /* Register the application with Executive Services */
    uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("PE - Failed to register the app (0x%08lX)\n", iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(PE_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(PE_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(PE_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(PE_MAIN_TASK_PERF_ID);
    }
    else
    {
        uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&uiRunStatus) == TRUE)
    {
        RcvSchPipeMsg(PE_SCH_PIPE_PEND_TIME);

        iStatus = AcquireConfigPointers();
        if(iStatus != CFE_SUCCESS)
        {
            /* We apparently tried to load a new table but failed.  Terminate the application. */
            uiRunStatus = CFE_ES_APP_ERROR;
        }
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(PE_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(uiRunStatus);
}


void PE::CheckTimeouts()
{
    baroCheckTimeout();
    gpsCheckTimeout();
    landCheckTimeout();
    distCheckTimeout();
    flowCheckTimeout();
}


void PE::Update()
{
    CFE_ES_PerfLogEntry(PE_UPDATE_TASK_PERF_ID);
    osalbool TzStdDevValid = FALSE;
    osalbool ReinitStateVec = FALSE;
    osalbool ReinitStateCov = FALSE;
    osalbool ZStdDevValid = FALSE;
    osalbool VxyStdDevValid = FALSE;
    float dt = 0.0f;
    float dt_hist = 0.0f;
    uint64 newTimestamp = PX4LIB_GetPX4TimeUs();
    
    /* Update timestamps */
    dt = (newTimestamp - m_Timestamp) / 1.0e6f;
    m_Timestamp = newTimestamp;
    dt_hist = 1.0e-6f * (m_Timestamp - m_Timestamp_Hist);
    CheckTimeouts();

    /* Check if local initialized */
    if(!m_EstimatorLocalInitialized &&
            m_BaroInitialized &&
            m_LandInitialized)
    {
        m_EstimatorLocalInitialized = TRUE;
        (void) CFE_EVS_SendEvent(PE_LOCAL_ESTIMATOR_INF_EID, CFE_EVS_INFORMATION,
                                 "Local estimation initialized");
    }

    /* Check if global initialized */
    if(!m_EstimatorGlobalInitialized &&
            m_BaroInitialized &&
            m_GpsInitialized &&
            m_LandInitialized)
    {
        m_EstimatorGlobalInitialized = TRUE;
        (void) CFE_EVS_SendEvent(PE_GLOBAL_ESTIMATOR_INF_EID, CFE_EVS_INFORMATION,
                                 "Global estimation initialized");
    }

    /* Check if params are updated */
    if(m_ParamsUpdated)
    {
        updateStateSpaceParams();
        m_ParamsUpdated = FALSE;
    }

    /* Update current GPS validity */
    if(m_GpsInitialized)
    {
        /* Check if xy is valid */
        if (fmax(m_StateCov[X_vx][X_vx], m_StateCov[X_vy][X_vy]) <
            ConfigTblPtr->VXY_PUB_THRESH * ConfigTblPtr->VXY_PUB_THRESH)
        {
            VxyStdDevValid = TRUE;
        }

        if(m_XyEstValid)
        {
            if(!VxyStdDevValid && m_GpsTimeout)
            {
                m_XyEstValid = FALSE;
            }
        }
        else
        {
            if(VxyStdDevValid && !m_GpsTimeout)
            {
                m_XyEstValid = TRUE;
            }
        }
    }
    else
    {
        m_XyEstValid = FALSE;
    }

    /* Update current baro validity */
    if(m_BaroInitialized)
    {
        /* Check if z is valid */
        if(sqrtf(m_StateCov[X_z][X_z]) < ConfigTblPtr->Z_PUB_THRESH)
        {
            ZStdDevValid = TRUE;
        }

        if(m_ZEstValid)
        {
            if(!ZStdDevValid && m_BaroTimeout)
            {
                m_ZEstValid = FALSE;
            }
        }
        else
        {
            if(ZStdDevValid && !m_BaroTimeout)
            {
                m_ZEstValid = TRUE;
            }
        }
    }
    else
    {
        m_ZEstValid = FALSE;
    }

    /* Update current land validity */
    if(m_LandInitialized)
    {
        /* Check if terrain is valid */
        if(sqrtf(m_StateCov[X_tz][X_tz]) < ConfigTblPtr->Z_PUB_THRESH)
        {
            TzStdDevValid = TRUE;
        }

        if(m_TzEstValid)
        {
            if(!TzStdDevValid)
            {
                m_TzEstValid = FALSE;
            }
        }
        else
        {
            if(TzStdDevValid)
            {
                m_TzEstValid = TRUE;
            }
        }
    }
    else
    {
        m_TzEstValid = FALSE;
    }

    /* Initialize map projection to INIT_ORIGIN_LAT, INIT_ORIGIN_LON if we don't
     * have lat, lon data and allowing fake origin */
    if (!m_MapRef.init_done && m_XyEstValid && ConfigTblPtr->FAKE_ORIGIN)
    {
        map_projection_init(&m_MapRef,
                            ConfigTblPtr->INIT_ORIGIN_LAT,
                            ConfigTblPtr->INIT_ORIGIN_LON,
                            m_Timestamp);

        (void) CFE_EVS_SendEvent(PE_GPS_OK_INF_EID, CFE_EVS_INFORMATION,
                                 "GPS fake origin init. Lat: %6.2f Lon: %6.2f Alt: %5.1f m",
                                 ConfigTblPtr->INIT_ORIGIN_LAT,
                                 ConfigTblPtr->INIT_ORIGIN_LON,
                                 double(m_AltOrigin));
    }

    /* Check if state vector needs to be reinitialized  */
    for (int i = 0; i < n_x; i++)
    {
        if (!isfinite(m_StateVec[i]))
        {
            ReinitStateVec = TRUE;
            (void) CFE_EVS_SendEvent(PE_ESTIMATOR_ERR_EID, CFE_EVS_INFORMATION,
                                     "Reinitializing state vector. Index (%i) not finite", i);
            break;
        }
    }

    if (ReinitStateVec)
    {
        m_StateVec.Zero();
    }

    /* Force state covariance symmetry and reinitialize matrix if necessary */
    for (int i = 0; i < n_x; i++)
    {
        for (int j = 0; j <= i; j++)
        {
            if (!isfinite(m_StateCov[i][j]))
            {
                if(Initialized())
                {
                    (void) CFE_EVS_SendEvent(PE_ESTIMATOR_ERR_EID, CFE_EVS_ERROR,
                                         "Reinitializing state covariance. Index (%i, %i) not finite", i, j);
                }
                ReinitStateCov = TRUE;
                break;
            }
            if (i == j)
            {
                /* Ensure diagonal elements are positive */
                if (m_StateCov[i][i] <= 0)
                {
                    if(Initialized())
                    {
                        (void) CFE_EVS_SendEvent(PE_ESTIMATOR_ERR_EID, CFE_EVS_ERROR,
                                                 "Reinitializing state covariance. Index (%i, %i) negative", i, j);
                    }
                    ReinitStateCov = TRUE;
                    break;
                }
            }
            else
            {
                /* Copy element from upper triangle to force symmetry */
                m_StateCov[j][i] = m_StateCov[i][j];
            }
        }
    }

    if (ReinitStateCov)
    {
        initStateCov();
    }

    Predict(dt);

    /* Publish updated data if initialized */
    if(Initialized())
    {
        SendVehicleLocalPositionMsg();

        if(m_XyEstValid && (m_MapRef.init_done || ConfigTblPtr->FAKE_ORIGIN))
        {
            SendVehicleGlobalPositionMsg();
        }
    }

    /* Propagate delayed state, no matter what if state is frozen, 
     * delayed state still needs to be propagated with frozen state.
     */

    if (m_Timestamp_Hist == 0 || (dt_hist > HIST_STEP)) 
    {
        m_TDelay.Update(m_Timestamp);
        m_XDelay.Update(m_StateVec);
        m_Timestamp_Hist = m_Timestamp;
    }

    CFE_ES_PerfLogExit(PE_UPDATE_TASK_PERF_ID);
}


osalbool PE::Initialized(void)
{
    return m_EstimatorLocalInitialized || m_EstimatorGlobalInitialized;
}


void PE::Predict(float dt)
{
    /* Get acceleration */
    m_Predict.q[0] = m_VehicleAttitudeMsg.Q[0];
    m_Predict.q[1] = m_VehicleAttitudeMsg.Q[1];
    m_Predict.q[2] = m_VehicleAttitudeMsg.Q[2];
    m_Predict.q[3] = m_VehicleAttitudeMsg.Q[3];

    m_RotationMat = math::Dcm(m_Predict.q);
    m_Euler = math::Euler(m_RotationMat);
    m_Predict.a[0] = m_SensorCombinedMsg.Acc[0];
    m_Predict.a[1] = m_SensorCombinedMsg.Acc[1];
    m_Predict.a[2] = m_SensorCombinedMsg.Acc[2];

    /* Note: bias is removed in dynamics function */
    m_InputVec = m_RotationMat * m_Predict.a;
    m_InputVec[U_az] += PE_ONE_G; // add g

    /* Update state space based on new states */
    updateStateSpace();

    /* Continuous time Kalman filter prediction. Integrate runge kutta 4th order.
     * https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods */
    m_Predict.k1 = dynamics(m_StateVec, m_InputVec);
    m_Predict.k2 = dynamics(m_StateVec + m_Predict.k1 * dt / 2, m_InputVec);
    m_Predict.k3 = dynamics(m_StateVec + m_Predict.k2 * dt / 2, m_InputVec);
    m_Predict.k4 = dynamics(m_StateVec + m_Predict.k3 * dt, m_InputVec);
    m_Predict.dx = (m_Predict.k1 + m_Predict.k2 * 2 + m_Predict.k3 * 2 + 
    m_Predict.k4) * (dt / 6);
    /* Don't integrate position if no valid xy data */
    if (!m_XyEstValid)
    {
        m_Predict.dx[X_x]  = 0;
        m_Predict.dx[X_vx] = 0;
        m_Predict.dx[X_y]  = 0;
        m_Predict.dx[X_vy] = 0;
    }

    /* Don't integrate z if no valid z data */
    if (!m_ZEstValid)
    {
        m_Predict.dx[X_z] = 0;
    }

    /* Don't integrate tz if no valid tz data */
    if (!m_TzEstValid)
    {
        m_Predict.dx[X_tz] = 0;
    }

    /* Saturate bias */
    m_Predict.bx = m_Predict.dx[X_bx] + m_StateVec[X_bx];
    m_Predict.by = m_Predict.dx[X_by] + m_StateVec[X_by];
    m_Predict.bz = m_Predict.dx[X_bz] + m_StateVec[X_bz];

    if (::abs(m_Predict.bx) > BIAS_MAX)
    {
        m_Predict.bx = BIAS_MAX * m_Predict.bx / ::abs(m_Predict.bx);
        m_Predict.dx[X_bx] = m_Predict.bx - m_StateVec[X_bx];
    }

    if (::abs(m_Predict.by) > BIAS_MAX)
    {
        m_Predict.by = BIAS_MAX * m_Predict.by / ::abs(m_Predict.by);
        m_Predict.dx[X_by] = m_Predict.by - m_StateVec[X_by];
    }

    if (::abs(m_Predict.bz) > BIAS_MAX)
    {
        m_Predict.bz = BIAS_MAX * m_Predict.bz / ::abs(m_Predict.bz);
        m_Predict.dx[X_bz] = m_Predict.bz - m_StateVec[X_bz];
    }

    /* Propagate */
    m_StateVec += m_Predict.dx;
    m_Predict.dP = (m_DynamicsMat * m_StateCov + m_StateCov * 
            m_DynamicsMat.Transpose() + m_InputMat * m_InputCov * 
            m_InputMat.Transpose() + m_NoiseCov) * dt;

    /* Covariance propagation logic */
    for (int i = 0; i < n_x; i++) 
    {
        if (m_StateCov[i][i] > P_MAX)
        {
            /* If diagonal element greater than max stop propagating */
            m_Predict.dP[i][i] = 0;

            for (int j = 0; j < n_x; j++)
            {
                m_Predict.dP[i][j] = 0;
                m_Predict.dP[j][i] = 0;
            }
        }
    }

    /* Update state */
    m_StateCov = m_StateCov + m_Predict.dP;
    m_XLowPass.Update(m_StateVec, dt, LOW_PASS_CUTOFF);
    m_AglLowPass.Update(m_StateVec[X_tz] - m_StateVec[X_z], dt, LOW_PASS_CUTOFF);
}


math::Vector10F PE::dynamics(const math::Vector10F &x, const math::Vector3F &u)
{
    return (m_DynamicsMat * x) + (m_InputMat * u);
}


void PE::UpdateLocalParams()
{
    /* Lock the mutex */
    OS_MutSemTake(ConfigMutex);

    /* Update all locally stored params with the up to date value in the table */
    m_BaroFuse = ConfigTblPtr->BARO_FUSE;
    m_GpsFuse  = ConfigTblPtr->GPS_FUSE;
    m_LandFuse = ConfigTblPtr->LAND_FUSE;
    m_DistFuse = ConfigTblPtr->DIST_FUSE;
    m_FlowFuse = ConfigTblPtr->FLOW_FUSE;

    /* Unlock the mutex */
    OS_MutSemGive(ConfigMutex);
}


int PE::getDelayPeriods(float delay, uint8 *periods)
{
    float t_delay = 0;
    uint8 i_hist = 0;

    for(i_hist = 1; i_hist < HIST_LEN; i_hist++)
    {
        t_delay = 1.0e-6f * (m_Timestamp - m_TDelay.Get(i_hist));
        if(t_delay > delay)
        {
            break;
        }
    }

    *periods = i_hist;

    if(t_delay > DELAY_MAX)
    {
        if(Initialized())
        {
            (void) CFE_EVS_SendEvent(PE_ESTIMATOR_ERR_EID, CFE_EVS_INFORMATION,
                    "Delayed data old: %8.4f", t_delay);
        }
        return -1;
    }
    
    return CFE_SUCCESS;
}




/************************/
/*  End of File Comment */
/************************/
