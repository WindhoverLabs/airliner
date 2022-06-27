/****************************************************************************
 *
 *   Copyright (c) 2013, 2014 PX4 Development Team. All rights reserved.
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
 * @file CatapultLaunchMethod.h
 * Catpult Launch detection
 *
 * @author Thomas Gubler <thomasgubler@gmail.com>
 */

#ifndef CATAPULTLAUNCHMETHOD_H_
#define CATAPULTLAUNCHMETHOD_H_

#include "fpc_events.h"
#include "cfe.h"
#include <math.h>

namespace launchdetection
{

enum LaunchDetectionResult {
    LAUNCHDETECTION_RES_NONE = 0, /**< No launch has been detected */
    LAUNCHDETECTION_RES_DETECTED_ENABLECONTROL = 1, /**< Launch has been detected, the controller should
                              control the attitude. However any motors should not throttle
                              up. For instance this is used to have a delay for the motor
                              when launching a fixed wing aircraft from a bungee */
    LAUNCHDETECTION_RES_DETECTED_ENABLEMOTORS = 2 /**< Launch has been detected, the controller should control
                            attitude and also throttle up the motors. */
};

class CatapultLaunchMethod
{
public:
    CatapultLaunchMethod(float thresholdAccel, float thresholdTime, float motorDelay, float pitchMaxPreThrottle);
    CatapultLaunchMethod();
    ~CatapultLaunchMethod();

    void update(float accel_x);
    LaunchDetectionResult getLaunchDetected() const ;
    void reset();
    float getPitchMax(float pitchMaxDefault);

private:
    uint64 last_timestamp{0};
	float integrator{0.0f};
	float motorDelayCounter{0.0f};

	LaunchDetectionResult state{LAUNCHDETECTION_RES_NONE};

    float thresholdAccel;
    float thresholdTime;
    float motorDelay;
    float pitchMaxPreThrottle; /**< Upper pitch limit before throttle is turned on.
						       Can be used to make sure that the AC does not climb
						       too much while attached to a bungee */

};


} // namespace launchdetection

#endif /* CATAPULTLAUNCHMETHOD_H_ */
