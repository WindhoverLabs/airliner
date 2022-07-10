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

#include "cfe.h"
#include "amc_app.h"
#include "simlib.h"
#include <math.h>

#define PWM_CUSTOM_OUT_MIN  (0.0f)
#define PWM_CUSTOM_OUT_MAX  (1.0f)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* AMC::InitDevice function.                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 AMC::InitDevice(void)
{
    float controls[16];
    uint32 controlCount = 16;
    uint32 i = 0;

    for (i = 0; i < 16; ++i)
    {
        controls[i] = PWM_CUSTOM_OUT_MIN;
    }

    /* gimbal roll */
    controls[6] = 0.0f;
    /* gimbal pitch */
    controls[7] = 0.0f;
    /* gimbal yaw */
    controls[8] = 0.0f;
    /* left leg min value (see sdf scale and offset) */
    controls[9] = -1.0f;
    /* right leg min value (see sdf scale and offset) */
    controls[10] = -1.0f;
    /* unused in typhoon sdf below this point i.e. 11-15 */
    controls[11] = 0.0f;
    controls[12] = 0.0f;
    controls[13] = 0.0f;
    controls[14] = 0.0f;
    controls[15] = 0.0f;

    SIMLIB_SetActuatorControls(controls, controlCount, 0);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* AMC_Map function.                                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
float AMC_Map(float inValue, uint16 in_min, uint16 in_max, float out_min,
        float out_max)
{
    /* Use the mapping equation:  Y = (X-A)/(B-A) * (D-C) + C */
    float out = (inValue - in_min) / (in_max - in_min) * (out_max - out_min)
            + out_min;

    if (out > out_max)
    {
        out = out_max;
    }

    if (out < out_min)
    {
        out = out_min;
    }

    return out;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* AMC::SetMotorOutputs function.                                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void AMC::SetMotorOutputs(const uint16 *PWM)
{
    float controls[16];
    uint32 controlCount = 4;
    uint32 i = 0;

    for (i = 0; i < 4; ++i)
    {
        controls[i] = AMC_Map(PWM[i], ConfigTblPtr->PwmMin,
                ConfigTblPtr->PwmMax, PWM_CUSTOM_OUT_MIN,
                PWM_CUSTOM_OUT_MAX);
    }

    SIMLIB_SetActuatorControls(controls, controlCount, 0);
}
