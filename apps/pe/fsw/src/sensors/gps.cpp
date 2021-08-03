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

#include "../pe_app.h"
#include "cfs_utils.h"

void PE::gpsInit()
{
    /* check for good gps signal */
    uint8 nSat = m_VehicleGpsPositionMsg.SatellitesUsed;
    float eph = m_VehicleGpsPositionMsg.EpH;
    float epv = m_VehicleGpsPositionMsg.EpV;
    uint8 fix_type = m_VehicleGpsPositionMsg.FixType;
    math::Vector6F y;

    if (nSat < 6 ||
        eph > ConfigTblPtr->GPS_EPH_MAX ||
        epv > ConfigTblPtr->GPS_EPV_MAX ||
        fix_type < 3)
    {
        m_GpsStats.reset();
        goto gpsInit_Exit_Tag;
    }

    /* measure */
    if (gpsMeasure(y) != CFE_SUCCESS) 
    {
        m_GpsStats.reset();
        goto gpsInit_Exit_Tag;
    }

    /* if finished */
    if (m_GpsStats.getCount() > REQ_GPS_INIT_COUNT)
    {
        /* get mean gps values */
        double gpsLat = m_GpsStats.getMean()[0];
        double gpsLon = m_GpsStats.getMean()[1];
        float gpsAlt = m_GpsStats.getMean()[2];

        HkTlm.GpsTimeout = FALSE;
        m_GpsStats.reset();

        if (!HkTlm.ReceivedGps)
        {
            /* this is the first time we have received gps */
        	HkTlm.ReceivedGps = TRUE;

            /* note we subtract X_z which is in down directon so it is 
             * an addition
             **/
        	HkTlm.GpsAltOrigin = gpsAlt + m_StateVec[X_z];

            /* find lat, lon of current origin by subtracting x and y
             * if not using vision position since vision will
             * have it's own origin, not necessarily where vehicle starts
             **/ 
            if (!m_MapRef.init_done)
            {
                double gpsLatOrigin = 0;
                double gpsLonOrigin = 0;
                /* reproject at current coordinates */
                map_projection_init(&m_MapRef, gpsLat, gpsLon, CFE_TIME_ConvertTimeToMicros(HkTlm.Timestamp));
                /* find origin */
                map_projection_reproject(&m_MapRef, -m_StateVec[X_x], -m_StateVec[X_y], &gpsLatOrigin, &gpsLonOrigin);
                /* reinit origin */
                map_projection_init(&m_MapRef, gpsLatOrigin, gpsLonOrigin, CFE_TIME_ConvertTimeToMicros(HkTlm.Timestamp));

                /* always override alt origin on first GPS to fix
                 * possible baro offset in global altitude at init
                 **/
                HkTlm.AltOrigin = HkTlm.GpsAltOrigin;
                HkTlm.AltOriginInitialized = TRUE;

                (void) CFE_EVS_SendEvent(PE_GPS_OK_INF_EID, CFE_EVS_INFORMATION,
                        "GPS initialized origin. Lat: %6.2f Lon: %6.2f Alt: %5.1f m",
                        gpsLatOrigin, gpsLonOrigin, double(HkTlm.GpsAltOrigin));
            }

            (void) CFE_EVS_SendEvent(PE_GPS_OK_INF_EID, CFE_EVS_INFORMATION,
                    "GPS initialized. Lat: %6.2f Lon: %6.2f Alt: %5.1f m",
                    gpsLat, gpsLon, double(gpsAlt));
        }
    }
    
gpsInit_Exit_Tag:
    return;
}


int PE::gpsMeasure(math::Vector6F &y)
{
    /* gps measurement */
    y.Zero();
    y[0] = m_VehicleGpsPositionMsg.Lat * 1e-7;
    y[1] = m_VehicleGpsPositionMsg.Lon * 1e-7;
    y[2] = m_VehicleGpsPositionMsg.Alt * 1e-3;
    y[3] = m_VehicleGpsPositionMsg.Vel_n_m_s;
    y[4] = m_VehicleGpsPositionMsg.Vel_e_m_s;
    y[5] = m_VehicleGpsPositionMsg.Vel_d_m_s;

    /* increament sums for mean */
    m_GpsStats.update(y);
    HkTlm.TimeLastGps = HkTlm.Timestamp;
    return CFE_SUCCESS;
}


void PE::gpsCorrect()
{
    CFE_ES_PerfLogEntry(PE_SENSOR_GPS_PERF_ID);

    /* measure */
    if (gpsMeasure(m_GPS.y_global) != CFE_SUCCESS) 
    {
        goto end_of_function; 
    }

    /* gps measurement in local frame */
    m_GPS.lat = m_GPS.y_global[0];
    m_GPS.lon = m_GPS.y_global[1];
    m_GPS.alt = m_GPS.y_global[2];
    m_GPS.px = 0;
    m_GPS.py = 0;

    m_GPS.pz = -(m_GPS.alt - HkTlm.GpsAltOrigin);
    map_projection_project(&m_MapRef, m_GPS.lat, m_GPS.lon, &m_GPS.px, &m_GPS.py);

    m_GPS.y[0] = m_GPS.px;
    m_GPS.y[1] = m_GPS.py;
    m_GPS.y[2] = m_GPS.pz;
    m_GPS.y[3] = m_GPS.y_global[3];
    m_GPS.y[4] = m_GPS.y_global[4];
    m_GPS.y[5] = m_GPS.y_global[5];

    /* gps measurement matrix, measures position and velocity */
    m_GPS.C[Y_gps_x][X_x] = 1;
    m_GPS.C[Y_gps_y][X_y] = 1;
    m_GPS.C[Y_gps_z][X_z] = 1;
    m_GPS.C[Y_gps_vx][X_vx] = 1;
    m_GPS.C[Y_gps_vy][X_vy] = 1;
    m_GPS.C[Y_gps_vz][X_vz] = 1;

    /* default to parameter, use gps cov if provided */
    m_GPS.var_xy = ConfigTblPtr->GPS_XY_STDDEV * ConfigTblPtr->GPS_XY_STDDEV;
    m_GPS.var_z = ConfigTblPtr->GPS_Z_STDDEV * ConfigTblPtr->GPS_Z_STDDEV;
    m_GPS.var_vxy = ConfigTblPtr->GPS_VXY_STDDEV * ConfigTblPtr->GPS_VXY_STDDEV;
    m_GPS.var_vz = ConfigTblPtr->GPS_VZ_STDDEV * ConfigTblPtr->GPS_VZ_STDDEV;

    /* if field is not below minimum, set it to the value provided */
    if(m_VehicleGpsPositionMsg.EpH > ConfigTblPtr->GPS_XY_STDDEV)
    {
        m_GPS.var_xy = m_VehicleGpsPositionMsg.EpH * m_VehicleGpsPositionMsg.EpH;
    }
    if (m_VehicleGpsPositionMsg.EpV > ConfigTblPtr->GPS_Z_STDDEV)
    {
        m_GPS.var_z = m_VehicleGpsPositionMsg.EpV * m_VehicleGpsPositionMsg.EpV;
    }

    m_GPS.gps_s_stddev = m_VehicleGpsPositionMsg.SVariance;
    if(m_GPS.gps_s_stddev > ConfigTblPtr->GPS_VXY_STDDEV)
    {
        m_GPS.var_vxy = m_GPS.gps_s_stddev * m_GPS.gps_s_stddev;
    }

    if(m_GPS.gps_s_stddev > ConfigTblPtr->GPS_VZ_STDDEV)
    {
        m_GPS.var_vz = m_GPS.gps_s_stddev * m_GPS.gps_s_stddev;
    }

    m_GPS.R[0][0] = m_GPS.var_xy;
    m_GPS.R[1][1] = m_GPS.var_xy;
    m_GPS.R[2][2] = m_GPS.var_z;
    m_GPS.R[3][3] = m_GPS.var_vxy;
    m_GPS.R[4][4] = m_GPS.var_vxy;
    m_GPS.R[5][5] = m_GPS.var_vz;

    /* get delayed x */
    m_GPS.i_hist = 0;

    if(getDelayPeriods(ConfigTblPtr->GPS_DELAY, &m_GPS.i_hist) < 0)
    {
        goto end_of_function;
    }
    
    m_GPS.x0 = m_XDelay.Get(m_GPS.i_hist);

    /* residual */
    m_GPS.r = m_GPS.y - m_GPS.C * m_GPS.x0;

    m_GPS.S_I = m_GPS.C * m_StateCov * m_GPS.C.Transpose() + m_GPS.R;
    m_GPS.S_I = m_GPS.S_I.Inversed();

    /* fault detection */
    /* 6x6 * 6x1 */
    m_GPS.rTranspose[0][0] = m_GPS.r[0];
    m_GPS.rTranspose[0][1] = m_GPS.r[1];
    m_GPS.rTranspose[0][2] = m_GPS.r[2];
    m_GPS.rTranspose[0][3] = m_GPS.r[3];
    m_GPS.rTranspose[0][4] = m_GPS.r[4];
    m_GPS.rTranspose[0][5] = m_GPS.r[5];
    
    m_GPS.beta = (m_GPS.rTranspose * (m_GPS.S_I * m_GPS.r))[0][0];
    /* artifically increase beta threshhold to prevent fault during 
     * landing
     **/ 
    m_GPS.beta_thresh = 1e2f;

    /* Save GPS beta for HK */
    HkTlm.GpsBeta = m_GPS.beta;

    if (m_GPS.beta / BETA_TABLE[n_y_gps] > m_GPS.beta_thresh) 
    {
        if (!HkTlm.GpsFault)
        {
            if(Initialized())
            {
                (void) CFE_EVS_SendEvent(PE_GPS_FAULT_ERR_EID, CFE_EVS_ERROR,
                        "GPS fault, %3g %3g %3g %3g %3g %3g",
                        double(m_GPS.r[0]*m_GPS.r[0] / m_GPS.S_I[0][0]),  
                        double(m_GPS.r[1]*m_GPS.r[1] / m_GPS.S_I[1][1]), 
                        double(m_GPS.r[2]*m_GPS.r[2] / m_GPS.S_I[2][2]),
                        double(m_GPS.r[3]*m_GPS.r[3] / m_GPS.S_I[3][3]),  
                        double(m_GPS.r[4]*m_GPS.r[4] / m_GPS.S_I[4][4]), 
                        double(m_GPS.r[5]*m_GPS.r[5] / m_GPS.S_I[5][5]));
            }
            HkTlm.GpsFault = TRUE;
        }
    }
    else
    {
        if (HkTlm.GpsFault)
        {
        	HkTlm.GpsFault = FALSE;
            (void) CFE_EVS_SendEvent(PE_GPS_OK_INF_EID, CFE_EVS_INFORMATION,
                    "GPS OK");

            HkTlm.GpsInitialized = TRUE;
        }
    }

    /* kalman filter correction always for GPS */

	/* 10x10 * 6x10' (10x6) * 6x6 */
	m_GPS.K = (m_StateCov * m_GPS.C.Transpose()) * m_GPS.S_I;
	m_GPS.dx = m_GPS.K * m_GPS.r;

	/* 10F * 10F */
	m_StateVec = m_StateVec + m_GPS.dx;

	/* 10x10 - (10x6 * 6x10 * 10x10)*/
	m_StateCov = m_StateCov - m_GPS.K * m_GPS.C * m_StateCov;
end_of_function:

    CFE_ES_PerfLogExit(PE_SENSOR_GPS_PERF_ID);
}


void PE::gpsCheckTimeout()
{
	if ((CFE_TIME_ConvertTimeToMicros(HkTlm.Timestamp) - CFE_TIME_ConvertTimeToMicros(HkTlm.TimeLastGps)) > GPS_TIMEOUT)
	{
		if (!HkTlm.GpsTimeout)
		{
			HkTlm.GpsTimeout = TRUE;
			m_GpsStats.reset();
			(void) CFE_EVS_SendEvent(PE_GPS_TIMEOUT_ERR_EID, CFE_EVS_ERROR,
									 "GPS timeout");
		}
	}
}
