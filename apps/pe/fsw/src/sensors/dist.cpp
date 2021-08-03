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

void PE::distInit()
{
    /* Measure */
	math::Vector1F y;

	if (distMeasure(y) != CFE_SUCCESS)
	{
		m_DistStats.reset();
		return;
	}

	/* If finished */
	if (m_DistStats.getCount() > REQ_DIST_INIT_COUNT)
	{
		HkTlm.DistAltOrigin = m_DistStats.getMean()[0];

		(void) CFE_EVS_SendEvent(PE_DIST_OK_INF_EID, CFE_EVS_INFORMATION,
								 "Distance sensor initialized. Mean: (%d) Std dev: (%d) cm",
								 (int)m_DistStats.getMean()[0],
								 (int)(100 * m_DistStats.getStdDev()[0]));

		HkTlm.DistTimeout = FALSE;

		if (!HkTlm.AltOriginInitialized)
		{
			HkTlm.AltOriginInitialized = TRUE;
			HkTlm.AltOrigin = HkTlm.DistAltOrigin;
		}
	}
}


int32 PE::distMeasure(math::Vector1F &y)
{
	int32 Status = CFE_SUCCESS;
	float d = m_DistanceSensor.CurrentDistance;
	float eps = 0.05f; // 5 cm
	float min_dist = m_DistanceSensor.MinDistance + eps;
	float max_dist = m_DistanceSensor.MaxDistance - eps;

	// prevent driver from setting min dist below eps
	if (min_dist < eps)
	{
		min_dist = eps;
	}

	// check for bad data
	if (d > max_dist || d < min_dist)
	{
		Status = -1;
		goto distMeasure_Exit_Tag;
	}

	// Check for horizontal speed


	/* Measure */
	y.Zero();
	m_DistStats.update(d);
	y[0] = (d + ConfigTblPtr->DIST_OFF_Z) * cosf(m_Euler[0]) * cosf(m_Euler[1]);
	HkTlm.TimeLastDist = CFE_SB_GetMsgTime((CFE_SB_MsgPtr_t)&m_DistanceSensor);

distMeasure_Exit_Tag:
	return Status;
}


void PE::distCorrect()
{
    CFE_ES_PerfLogEntry(PE_SENSOR_DIST_PERF_ID);
    float cov = 0.0f;

    if (distMeasure(m_Dist.y) != CFE_SUCCESS)
    {
        goto end_of_function;
    }

    /* subtract dist origin alt */
    //m_Dist.y[0] -= m_DistAltOrigin;

    /* measured altitude, negative down dir */
    m_Dist.C[Y_dist_z][X_z] = -1.0f;
    m_Dist.C[Y_dist_z][X_tz] = 1.0f;

    cov = m_DistanceSensor.Covariance;
    if (cov < 1.0e-3f)
    {
    	m_Dist.R[0][0] = ConfigTblPtr->DIST_STDDEV * ConfigTblPtr->DIST_STDDEV;
    }
    else
    {
    	m_Dist.R[0][0] = cov;
    }

    /* residual */
    /* ((1x10 * 10x10) * 10x1) + 1x1) */
    m_Dist.S_I = m_Dist.C * m_StateCov * m_Dist.C.Transpose() + m_Dist.R;
    
    /* Take the inverse of a 1x1 matrix (reciprical of the single entry) */
    m_Dist.S_I[0][0] = 1 / m_Dist.S_I[0][0];

    /* Vector1F -  (1x10 * Vector10F) */
    m_Dist.r = m_Dist.y - (m_Dist.C * m_StateVec);

    /* fault detection 1F * 1x1 * 1F */
    m_Dist.beta = m_Dist.r[0] * m_Dist.S_I[0][0] * m_Dist.r[0];

    HkTlm.DistBeta = m_Dist.beta;

    if (m_Dist.beta > DIST_BETA_MAX)
    {
        if (!HkTlm.DistFault)
        {
            if(Initialized())
            {
                (void) CFE_EVS_SendEvent(PE_DIST_FAULT_ERR_EID, CFE_EVS_ERROR,
                        "Dist fault, d %5.2f m r %5.2f m, beta %5.2f", m_DistanceSensor.CurrentDistance, m_Dist.r[0], m_Dist.beta);
            }
            HkTlm.DistFault = TRUE;
        }
        goto end_of_function;
    }
    else
    {
        if (HkTlm.DistFault)
        {
        	HkTlm.DistFault = FALSE;
            (void) CFE_EVS_SendEvent(PE_DIST_OK_INF_EID, CFE_EVS_INFORMATION,
                    "Dist OK r %5.2f m, beta %5.2f", m_Dist.r[0], m_Dist.beta);

            HkTlm.DistInitialized = TRUE;
        }
    }

    /* kalman filter correction */

	/* 10x10 * 10x1 * 1x1 */
	m_Dist.K = (m_StateCov * m_Dist.C.Transpose()) * m_Dist.S_I;

	/* 10x1 * 1x1 */
	m_Dist.temp = m_Dist.K * m_Dist.r;

	m_Dist.dx = m_Dist.temp.ToVector();

	/* 10F + 10F*/
	m_StateVec = m_StateVec + m_Dist.dx;

	/* 10x10 - 10x1 * 1x10 * 10x10 */
	m_StateCov = m_StateCov - m_Dist.K * m_Dist.C * m_StateCov;
end_of_function:

    CFE_ES_PerfLogExit(PE_SENSOR_DIST_PERF_ID);
}


void PE::distCheckTimeout()
{
    uint64 Timestamp = 0;

	if (CFE_TIME_Compare(HkTlm.Timestamp, HkTlm.TimeLastDist) == CFE_TIME_A_GT_B)
	{
        Timestamp = CFE_TIME_ConvertTimeToMicros(HkTlm.Timestamp) - CFE_TIME_ConvertTimeToMicros(HkTlm.TimeLastDist);
    }
	else if (CFE_TIME_Compare(HkTlm.Timestamp, HkTlm.TimeLastDist) == CFE_TIME_A_LT_B)
	{
        Timestamp = CFE_TIME_ConvertTimeToMicros(HkTlm.TimeLastDist) - CFE_TIME_ConvertTimeToMicros(HkTlm.Timestamp);
    }

	if (Timestamp > DIST_TIMEOUT)
	{
		if (!HkTlm.DistTimeout)
		{
			HkTlm.DistTimeout = TRUE;
			m_DistStats.reset();
			(void) CFE_EVS_SendEvent(PE_DIST_TIMEOUT_ERR_EID, CFE_EVS_ERROR,
									 "Dist timeout: %llu us", Timestamp);
		}
	}
}
