/****************************************************************************
 *
 *   Copyright (c) 2013-2015 PX4 Development Team. All rights reserved.
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
 * @file pwm_limit.c
 *
 * Library for PWM output limiting
 *
 * @author Julian Oes <julian@px4.io>
 */


#ifndef PWM_LIMIT_H
#define PWM_LIMIT_H

#include "cfe.h"

#define PROGRESS_INT_SCALING    10000

/*
 * time for the ESCs to initialize
 * (this is not actually needed if PWM is sent right after boot)
 */
#define INIT_TIME_US 50000
/*
 * time to slowly ramp up the ESCs
 */
#define RAMP_TIME_US 500000

typedef uint64 hrt_abstime;

typedef enum {
    PWM_LIMIT_STATE_OFF = 0,
    PWM_LIMIT_STATE_INIT,
    PWM_LIMIT_STATE_RAMP,
    PWM_LIMIT_STATE_ON
} PwmLimit_State_t;

typedef struct
{
    PwmLimit_State_t state;
    uint64 time_armed;
} PwmLimit_Data_t;

void PwmLimit_Init(PwmLimit_Data_t *limit);

void PwmLimit_Calc(const boolean armed, const boolean pre_armed, const unsigned num_channels, const uint16 reverse_mask,
            const uint16 *disarmed_pwm, const uint16 *min_pwm, const uint16 *max_pwm,
            const float *output, uint16 *effective_pwm, PwmLimit_Data_t *limit);
#endif /* PWM_LIMIT_H_ */
