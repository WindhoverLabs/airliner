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
 * @author Lorenz Meier <lorenz@px4.io>
 */

#include "pwm_limit.h"
#include <math.h>
#include <float.h>
#include "px4lib.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* PwmLimit Init function.                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PwmLimit_Init(PwmLimit_Data_t *limit)
{
    limit->state = PWM_LIMIT_STATE_INIT;
    limit->time_armed = 0;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* PwmLimit Calc function.                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PwmLimit_Calc(const boolean armed, const boolean pre_armed,
        const unsigned num_channels, const uint16 reverse_mask,
        const uint16 *disarmed_pwm, const uint16 *min_pwm,
        const uint16 *max_pwm, const float *output, uint16 *effective_pwm,
        PwmLimit_Data_t *limit)
{
    /* first evaluate state changes */
    switch (limit->state)
    {
        case PWM_LIMIT_STATE_INIT:
        {
            if (armed)
            {
                uint32 sec = 0;
                uint32 ms = 0;
                uint64 now = 0;
                uint64 delta = 0;

                /* set arming time for the first call */
                if (limit->time_armed == 0)
                {
                    /* reset arming time, used for ramp timing */
                    limit->time_armed = PX4LIB_GetPX4TimeUs();
                }

                /* reset arming time, used for ramp timing */
                now = PX4LIB_GetPX4TimeUs();

                delta = now - limit->time_armed;

                if (delta >= INIT_TIME_US)
                {
                    limit->state = PWM_LIMIT_STATE_OFF;
                }
            }

            break;
        }

        case PWM_LIMIT_STATE_OFF:
        {
            if (armed)
            {
                /* reset arming time, used for ramp timing */
                limit->time_armed = PX4LIB_GetPX4TimeUs();

                limit->state = PWM_LIMIT_STATE_RAMP;
            }

            break;
        }

        case PWM_LIMIT_STATE_RAMP:
        {
            if (!armed)
            {
                limit->state = PWM_LIMIT_STATE_OFF;
            }
            else if (hrt_elapsed_time(&limit->time_armed) >= RAMP_TIME_US)
            {
                limit->state = PWM_LIMIT_STATE_ON;
            }

            break;
        }

        case PWM_LIMIT_STATE_ON:
        {
            if (!armed)
            {
                limit->state = PWM_LIMIT_STATE_OFF;
            }

            break;
        }

        default:
        {
            break;
        }
    }

    /* if the system is pre-armed, the limit state is temporarily on,
     * as some outputs are valid and the non-valid outputs have been
     * set to NaN. This is not stored in the state machine though,
     * as the throttle channels need to go through the ramp at
     * regular arming time.
     */

    unsigned local_limit_state = limit->state;

    if (pre_armed)
    {
        local_limit_state = PWM_LIMIT_STATE_ON;
    }

    unsigned progress;

    /* then set effective_pwm based on state */
    switch (local_limit_state)
    {
        case PWM_LIMIT_STATE_OFF:
        /* Fallthru. */
        case PWM_LIMIT_STATE_INIT:
        {
            uint32 i = 0;
            for (i = 0; i < num_channels; ++i)
            {
                effective_pwm[i] = disarmed_pwm[i];
            }

            break;
        }

        case PWM_LIMIT_STATE_RAMP:
        {
            uint32 i = 0;
            hrt_abstime diff = hrt_elapsed_time(&limit->time_armed);

            progress = diff * PROGRESS_INT_SCALING / RAMP_TIME_US;

            if (progress > PROGRESS_INT_SCALING)
            {
                progress = PROGRESS_INT_SCALING;
            }

            for (i = 0; i < num_channels; ++i)
            {
                float control_value = output[i];
                uint16 ramp_min_pwm;

                /* check for invalid / disabled channels */
                if (!isfinite(control_value))
                {
                    effective_pwm[i] = disarmed_pwm[i];
                    continue;
                }

                /* if a disarmed pwm value was set, blend between disarmed and min */
                if (disarmed_pwm[i] > 0)
                {
                    uint32 disarmed_min_diff;
                    /* safeguard against overflows */
                    uint32 disarmed = disarmed_pwm[i];

                    if (disarmed > min_pwm[i])
                    {
                        disarmed = min_pwm[i];
                    }

                    disarmed_min_diff = min_pwm[i] - disarmed;
                    ramp_min_pwm = disarmed
                            + (disarmed_min_diff * progress) / PROGRESS_INT_SCALING;

                }
                else
                {
                    /* no disarmed pwm value set, choose min pwm */
                    ramp_min_pwm = min_pwm[i];
                }

                if (reverse_mask & (1 << i))
                {
                    control_value = -1.0f * control_value;
                }

                effective_pwm[i] = control_value * (max_pwm[i] - ramp_min_pwm) / 2
                        + (max_pwm[i] + ramp_min_pwm) / 2;

                /* last line of defense against invalid inputs */
                if (effective_pwm[i] < ramp_min_pwm)
                {
                    effective_pwm[i] = ramp_min_pwm;
                }
                else if (effective_pwm[i] > max_pwm[i])
                {
                    effective_pwm[i] = max_pwm[i];
                }
            }
            break;
        }

        case PWM_LIMIT_STATE_ON:
        {
            printf("PWM_LIMIT_STATE_ON$$$$$$$$$$$$$$$$\n");
            uint32 i = 0;
            for (i = 0; i < num_channels; ++i)
            {
                float control_value = output[i];

                /* check for invalid / disabled channels */
                if (!isfinite(control_value))
                {
                    effective_pwm[i] = disarmed_pwm[i];
                    continue;
                }

                if (reverse_mask & (1 << i))
                {
                    control_value = -1.0f * control_value;
                }

                effective_pwm[i] = control_value * (max_pwm[i] - min_pwm[i]) / 2
                        + (max_pwm[i] + min_pwm[i]) / 2;

                /* last line of defense against invalid inputs */
                if (effective_pwm[i] < min_pwm[i])
                {
                    effective_pwm[i] = min_pwm[i];
                }
                else if (effective_pwm[i] > max_pwm[i])
                {
                    effective_pwm[i] = max_pwm[i];
                }
                if(i ==2)
                {
                    printf("PWM_LIMIT_STATE_ON Value:%d\n", effective_pwm[i] );
                }
            }

            break;
        }

        default:
        {
            break;
        }
    }
}

uint64 hrt_elapsed_time(uint64 *input)
{
    uint64 delta = 0;

    /* reset arming time, used for ramp timing */
    delta = PX4LIB_GetPX4TimeUs() - *input;

    return delta;
}
