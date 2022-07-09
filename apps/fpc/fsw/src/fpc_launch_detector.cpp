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
 * @file launchDetection.cpp
 * Auto Detection for different launch methods (e.g. catapult)
 *
 * @author Thomas Gubler <thomasgubler@gmail.com>
 */

#include "fpc_catapult_launch_method.h"
#include "fpc_launch_detector.h"

namespace launchdetection
{

LaunchDetector::LaunchDetector() :
    thresholdAccel{0},
    thresholdTime{0},
    motorDelay{0},
    pitchMaxPreThrottle{0},
    launchDetectionOn{0}
//	launchdetection_on(this, "ALL_ON")
{
    /* init all detectors */
    launchMethod = CatapultLaunchMethod{};

    /* update all parameters of all detectors */
//	updateParams();
}


LaunchDetector::~LaunchDetector()
{
}

void LaunchDetector::reset()
{
    launchMethod.reset();
}

void LaunchDetector::update(float accel_x)
{
	if (launchDetectionEnabled()) {
        launchMethod.update(accel_x);
	}
}

LaunchDetectionResult LaunchDetector::getLaunchDetected()
{
    if (launchDetectionEnabled()) {
        if (launchMethod.getLaunchDetected() != LAUNCHDETECTION_RES_NONE) {
            return launchMethod.getLaunchDetected();
        }
    }

	return LAUNCHDETECTION_RES_NONE;
}

/**
 * @brief LaunchDetector::getPitchMax
 * @param pitchMaxDefault
 * @return PitchMax in radians.
 */
float LaunchDetector::getPitchMax(float pitchMaxDefault)
{
	if (!launchDetectionEnabled()) {
		return pitchMaxDefault;
	}

    //The following code could be useful if there is more than one launch detection method, maybe.
//	/* if a lauchdetectionmethod is active or only one exists return the pitch limit from this method,
//	 * otherwise use the default limit */
//	if (activeLaunchDetectionMethodIndex < 0) {
//        if (sizeof(launchMethod) / sizeof(LaunchMethod *) > 1) {
//			return pitchMaxDefault;

//		} else {
//            return launchMethod[0]->getPitchMax(pitchMaxDefault);
//		}

//	} else {
//        return launchMethod[activeLaunchDetectionMethodIndex]->getPitchMax(pitchMaxDefault);
//	}

    return launchMethod.getPitchMax(pitchMaxDefault);
}

void LaunchDetector::UpdateParamsFromTable(float newThresholdAccel,
                                float newThresholdTime,
                                float newMotorDelay,
                                float newPitchMaxPreThrottle,
                                bool newLaunchDetectionEnabled)
{
    thresholdAccel = newThresholdAccel;
    thresholdTime = newThresholdTime;
    motorDelay = newMotorDelay;
    pitchMaxPreThrottle = newPitchMaxPreThrottle;
    launchDetectionOn = newLaunchDetectionEnabled;

    launchMethod.Initialize(thresholdAccel,thresholdTime, motorDelay, newPitchMaxPreThrottle);

}
} // namespace launchdetection
