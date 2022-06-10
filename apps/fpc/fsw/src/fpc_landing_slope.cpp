/****************************************************************************
 *
 *   Copyright (c) 2012-2017 PX4 Development Team. All rights reserved.
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
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
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
 ****************************************************************************/

/*
 * @file landingslope.cpp
 *
 * @author Thomas Gubler <thomasgubler@gmail.com>
 */

#include "fpc_landing_slope.h"

#include "math/Matrix3F3.hpp"
#include "math/Quaternion.hpp"
#include "math/Euler.hpp"
#include "math/Dcm.hpp"
#include "math/Expo.hpp"
#include "math/Limits.hpp"
#include "math/Functions.hpp"

void Landingslope::update(float landing_slope_angle_rad_new,
		     float flare_relative_alt_new,
		     float motor_lim_relative_alt_new,
		     float H1_virt_new)
{
    m_Landing_Slope_Angle_Rad = landing_slope_angle_rad_new;
    m_Flare_Relative_Alt = flare_relative_alt_new;
    m_Motor_Lim_Relative_Alt = motor_lim_relative_alt_new;
    m_H1_Virt = H1_virt_new;

	calculateSlopeValues();
}

void Landingslope::calculateSlopeValues()
{
    m_H0 =  m_Flare_Relative_Alt + m_H1_Virt;
    m_D1 = m_Flare_Relative_Alt / tanf(m_Landing_Slope_Angle_Rad);
    m_Flare_Constant = (m_H0 * m_D1) / m_Flare_Relative_Alt;
    m_Flare_Length = -logf(m_H1_Virt / m_H0) * m_Flare_Constant;
    m_Horizontal_Slope_Displacement = (m_Flare_Length - m_D1);
}

float Landingslope::getLandingSlopeRelativeAltitude(float wp_landing_distance)
{
    return Landingslope::getLandingSlopeRelativeAltitude(wp_landing_distance, m_Horizontal_Slope_Displacement,
            m_Landing_Slope_Angle_Rad);
}

float Landingslope::getLandingSlopeRelativeAltitudeSave(float wp_landing_distance, float bearing_lastwp_currwp,
		float bearing_airplane_currwp)
{
	/* If airplane is in front of waypoint return slope altitude, else return waypoint altitude */
	if (fabsf(bearing_airplane_currwp - bearing_lastwp_currwp) < math::radians(90.0f)) {
		return getLandingSlopeRelativeAltitude(wp_landing_distance);

	}

	return 0.0f;

}

float Landingslope::getFlareCurveRelativeAltitudeSave(float wp_landing_distance, float bearing_lastwp_currwp,
		float bearing_airplane_currwp)
{
	/* If airplane is in front of waypoint return flare curve altitude, else return waypoint altitude */
	if (fabsf(bearing_airplane_currwp - bearing_lastwp_currwp) < math::radians(90.0f)) {
        return m_H0 * expf(-math::max(0.0f, m_Flare_Length - wp_landing_distance) / m_Flare_Constant) - m_H1_Virt;

	}

	return 0.0f;

}
