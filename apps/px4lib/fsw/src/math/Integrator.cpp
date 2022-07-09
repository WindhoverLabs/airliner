/****************************************************************************
 *
 *   Copyright (c) 2015-2016 PX4 Development Team. All rights reserved.
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

/**
 * @file integrator.cpp
 *
 * A resettable integrator
 *
 * @author Lorenz Meier <lorenz@px4.io>
 * @author Julian Oes <julian@oes.ch>
 */

#include "math/Integrator.hpp"
//#include <drivers/drv_hrt.h>
#include <time.h>
#include <errno.h>
#include "px4lib.h"

///* TODO */
//uint64 PX4LIB_GetPX4TimeUs(void)
//{
//    struct timespec ts;
//    int returnCode = 0;
//    uint64 outTime = 0;
//
//    returnCode = clock_gettime(CLOCK_MONOTONIC, &ts);
//    if (-1 == returnCode)
//    {
//        OS_printf("PX4LIB_GetPX4Time clock_gettime errno: %i", errno);
//        goto end_of_function;
//    }
//
//    outTime = (uint64)(ts.tv_sec) * 1000000;
//    outTime += ts.tv_nsec / 1000;
//
//end_of_function:
//    return outTime;
//}

/* TODO */
//uint64 PX4LIB_GetPX4TimeUs(void)
//{
//    uint64 outTime = 0;
//    OS_time_t localTime = {};

//    CFE_PSP_GetTime(&localTime);

//    outTime = (localTime.seconds * 1000000) + localTime.microsecs;

//    return outTime;
//}

//uint64 PX4LIB_GetPX4ElapsedTimeUs(uint64 then)
//{
//    uint64 delta = PX4LIB_GetPX4TimeUs() - then;
//    return delta;
//}


Integrator::Integrator(uint64 auto_reset_interval, boolean coning_compensation) :
	_auto_reset_interval(auto_reset_interval),
	_last_integration_time(0),
	_last_reset_time(0),
	_alpha(0.0f, 0.0f, 0.0f),
	_last_alpha(0.0f, 0.0f, 0.0f),
	_beta(0.0f, 0.0f, 0.0f),
	_last_val(0.0f, 0.0f, 0.0f),
	_last_delta_alpha(0.0f, 0.0f, 0.0f),
	_coning_comp_on(coning_compensation)
{

}

Integrator::~Integrator()
{

}

boolean
Integrator::put(uint64 timestamp, math::Vector3F &val, math::Vector3F &integral, uint64 &integral_dt)
{
    if (0 == _last_integration_time) 
    {
        /* this is the first item in the integrator */
        _last_integration_time = timestamp;
        _last_reset_time = timestamp;
        _last_val = val;

        return FALSE;
    }

    double dt = 0.0;

    // Integrate:
    // Leave dt at 0 if the integration time does not make sense.
    // Without this check the integral is likely to explode.
    if (timestamp >= _last_integration_time) 
    {
        dt = (double)(timestamp - _last_integration_time) / 1000000.0;
    }

    // Use trapezoidal integration to calculate the delta integral
    math::Vector3F delta_alpha = (val + _last_val) * dt * 0.5f;
    _last_val = val;

    // Calculate coning corrections if required
    if (TRUE == _coning_comp_on) 
    {
        // Coning compensation derived by Paul Riseborough and Jonathan Challinger,
        // following:
        // Tian et al (2010) Three-loop Integration of GPS and Strapdown INS with Coning and Sculling Compensation
        // Sourced: http://www.sage.unsw.edu.au/snap/publications/tian_etal2010b.pdf
        // Simulated: https://github.com/priseborough/InertialNav/blob/master/models/imu_error_modelling.m
        _beta = _beta + ((_last_alpha + _last_delta_alpha * (1.0f / 6.0f)) % delta_alpha) * 0.5f;
        _last_delta_alpha = delta_alpha;
        _last_alpha = _alpha;
    }

    // accumulate delta integrals
    _alpha = _alpha + delta_alpha;
    _last_integration_time = timestamp;

    // Only do auto reset if auto reset interval is not 0.
    if (_auto_reset_interval > 0 && (timestamp - _last_reset_time) > _auto_reset_interval) 
    {
        // apply coning corrections if required
        if (TRUE == _coning_comp_on) 
        {
            integral = _alpha + _beta;
        } 
        else 
        {
            integral = _alpha;
        }
        // reset the integrals and coning corrections
        _reset(integral_dt);

        return TRUE;
    } 
    else 
    {
        return FALSE;
    }
}

boolean
Integrator::put_with_interval(uint32 interval_us, math::Vector3F &val, math::Vector3F &integral,
			      uint64 &integral_dt)
{
	if (_last_integration_time == 0) {
		/* this is the first item in the integrator */
		uint64 now = PX4LIB_GetPX4TimeUs();
		_last_integration_time = now;
		_last_reset_time = now;
		_last_val = val;

		return FALSE;
	}

	// Create the timestamp artifically.
	uint64 timestamp = _last_integration_time + interval_us;

	return put(timestamp, val, integral, integral_dt);
}


math::Vector3F
Integrator::get(boolean reset, uint64 &integral_dt)
{
	math::Vector3F val = _alpha;

	if (TRUE == reset) 
    {
		_reset(integral_dt);
	}

	return val;
}

math::Vector3F
Integrator::get_and_filtered(boolean reset, uint64 &integral_dt, 
        math::Vector3F &filtered_val)
{
	// Do the usual get with reset first but don't return yet.
	math::Vector3F ret_integral = get(reset, integral_dt);

	// Because we need both the integral and the integral_dt.
	filtered_val[0] = ret_integral[0] * 1000000 / integral_dt;
	filtered_val[1] = ret_integral[1] * 1000000 / integral_dt;
	filtered_val[2] = ret_integral[2] * 1000000 / integral_dt;

	return ret_integral;
}

void
Integrator::_reset(uint64 &integral_dt)
{
	_alpha[0] = 0.0f;
	_alpha[1] = 0.0f;
	_alpha[2] = 0.0f;
	_last_alpha[0] = 0.0f;
	_last_alpha[1] = 0.0f;
	_last_alpha[2] = 0.0f;
	_beta[0] = 0.0f;
	_beta[1] = 0.0f;
	_beta[2] = 0.0f;

	integral_dt = (_last_integration_time - _last_reset_time);
	_last_reset_time = _last_integration_time;
}
