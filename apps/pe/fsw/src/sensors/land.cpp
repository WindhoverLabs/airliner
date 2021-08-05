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

boolean PE::landed()
{
	boolean landed = FALSE;
	boolean disarmed_not_falling = FALSE;

	if(0 == m_VehicleLandDetectedMsg.Freefall && PX4_ARMING_STATE_ARMED != m_VehicleStatusMsg.ArmingState)
	{
		disarmed_not_falling = TRUE;
	}

	if(m_VehicleLandDetectedMsg.Landed || disarmed_not_falling)
	{
		landed = TRUE;
	}

	return landed;
}

void PE::landInit()
{
	/* measure */
	math::Vector3F y;

	if (landMeasure(y) != CFE_SUCCESS)
	{
		HkTlm.LandCount = 0;
	}

	/* if finished */
	if (HkTlm.LandCount > REQ_LAND_INIT_COUNT)
	{
		HkTlm.LandTimeout = FALSE;

		(void) CFE_EVS_SendEvent(PE_LAND_OK_INF_EID, CFE_EVS_INFORMATION,
								 "Land detector initialized");
	}
}

int PE::landMeasure(math::Vector3F &y)
{
	HkTlm.TimeLastLand = HkTlm.Timestamp;
	y.Zero();
	HkTlm.LandCount += 1;
	return CFE_SUCCESS;
}

void PE::landCorrect()
{
    CFE_ES_PerfLogEntry(PE_SENSOR_LAND_PERF_ID);

	if (landMeasure(m_Land.y) != CFE_SUCCESS)
	{
		goto end_of_function;
	}

	/* measurement matrix */
	m_Land.C[Y_land_vx][X_vx] = 1.0f;
	m_Land.C[Y_land_vy][X_vy] = 1.0f;
	m_Land.C[Y_land_agl][X_z] = -1.0f; // measured altitude, negative down dir.
	m_Land.C[Y_land_agl][X_tz] = 1.0f; // measured altitude, negative down dir.

	/* use parameter covariance */
	m_Land.R[Y_land_vx][Y_land_vx] = ConfigTblPtr->LAND_VXY_STDDEV * ConfigTblPtr->LAND_VXY_STDDEV;
	m_Land.R[Y_land_vy][Y_land_vy] = ConfigTblPtr->LAND_VXY_STDDEV * ConfigTblPtr->LAND_VXY_STDDEV;
	m_Land.R[Y_land_agl][Y_land_agl] = ConfigTblPtr->LAND_Z_STDDEV * ConfigTblPtr->LAND_Z_STDDEV;

	/* residual */
	m_Land.S_I = (m_Land.C * (m_StateCov * m_Land.C.Transpose())) + m_Land.R;
	m_Land.S_I = m_Land.S_I.Inversed();

	m_Land.r = m_Land.y - m_Land.C * m_StateVec;

	/* fault detection */
	m_Land.beta = (m_Land.r.Transpose() * (m_Land.S_I * m_Land.r));

        /* Save Land beta for Diag */
	DiagTlm.LandBeta = m_Land.beta;

	/* artifically increase beta threshhold to prevent fault during landing */
	m_Land.beta_thresh = 1e2f;

	if (m_Land.beta / BETA_TABLE[n_y_land] > m_Land.beta_thresh)
	{
		if (!HkTlm.LandFault)
		{
			HkTlm.LandFault = TRUE;
            if(Initialized())
            {
			    (void) CFE_EVS_SendEvent(PE_LAND_FAULT_ERR_EID, CFE_EVS_ERROR,
									     "Land detector fault, beta %5.2f", double(m_Land.beta));
            }
		}
	}
	else 
	{
	    if (HkTlm.LandFault)
	    {
	    	HkTlm.LandFault = FALSE;
		    (void) CFE_EVS_SendEvent(PE_LAND_OK_INF_EID, CFE_EVS_ERROR,
								     "Land detector OK");
		    HkTlm.LandInitialized = TRUE;
		}
	}

	/* kalman filter correction always for land detector */
	m_Land.K = (m_StateCov * m_Land.C.Transpose()) * m_Land.S_I;
	m_Land.dx = m_Land.K * m_Land.r;
	m_StateVec = m_StateVec + m_Land.dx;
	m_StateCov = m_StateCov - m_Land.K * m_Land.C * m_StateCov;
end_of_function:

    CFE_ES_PerfLogExit(PE_SENSOR_LAND_PERF_ID);
}

void PE::landCheckTimeout()
{
	if (CFE_TIME_ConvertTimeToMicros(HkTlm.Timestamp) - CFE_TIME_ConvertTimeToMicros(HkTlm.TimeLastLand) > LAND_TIMEOUT)
	{
		if (!HkTlm.LandTimeout)
		{
			HkTlm.LandTimeout = TRUE;
			HkTlm.LandCount = 0;
			(void) CFE_EVS_SendEvent(PE_LAND_TIMEOUT_ERR_EID, CFE_EVS_ERROR,
									 "Land detector timeout");
		}
	}
}

