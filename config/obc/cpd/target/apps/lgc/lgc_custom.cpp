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
#include "lgc_app.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

#define LGC_RCOUT_ZYNQ_PWM_BASE (0x43c00000)
#define LGC_FREQUENCY_PWM       (400)
#define LGC_TICK_PER_S          (50000000)
#define LGC_TICK_PER_US         (50)
#define LGC_DEVICE_PATH         "/dev/mem"
#define MOTOR_OUTPUTS_SKIPPED   (8)


/* The following struct is used by the LGC_SharedMemCmd_t struct and overlayed
 * over the ocpoc PPM registers to control the PWM hardware.
 */
typedef struct {
    uint32 Period;
    uint32 Hi;
} LGC_PeriodHi_t;


/* The following struct is overlayed over the ocpoc PPM registers to control
 * the PWM hardware.
 */
typedef struct
{
    LGC_PeriodHi_t PeriodHi[MOTOR_OUTPUTS_SKIPPED + LGC_MAX_GEAR_OUTPUTS];
} LGC_SharedMemCmd_t;

volatile LGC_SharedMemCmd_t *LGC_SharedMemCmd;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* LGC_Freq2tick function.                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
uint32 LGC_Freq2tick(uint16 FreqHz);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* LGC::InitDevice function.                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 LGC::InitDevice(void)
{
    uint32 i = 0;
    int returnVal = 0;
    int mem_fd = 0;

    /* Initialize just in case we were reloaded and the ctor wasn't called. */
    LGC_SharedMemCmd = 0;

    mem_fd = open(LGC_DEVICE_PATH, O_RDWR | O_SYNC);
    LGC_SharedMemCmd = (LGC_SharedMemCmd_t *) mmap(0, 0x1000,
            PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd,
            LGC_RCOUT_ZYNQ_PWM_BASE);
    close(mem_fd);

    if (LGC_SharedMemCmd == 0) 
    {
        returnVal = errno;
        goto end_of_function;
    }

    // Note: this appears to be required actuators to initialize
    for (i = MOTOR_OUTPUTS_SKIPPED; i < (MOTOR_OUTPUTS_SKIPPED + LGC_MAX_GEAR_OUTPUTS); ++i) 
    {
        LGC_SharedMemCmd->PeriodHi[i].Period =
                LGC_Freq2tick(LGC_FREQUENCY_PWM);
        LGC_SharedMemCmd->PeriodHi[i].Hi     =
                LGC_Freq2tick(LGC_FREQUENCY_PWM) / 2;
    }

end_of_function:
    return returnVal;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* LGC::SetMotorOutputs function.                                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void LGC::SetMotorOutputs(const uint16 *PWM)
{
    uint32 i = 0;

    /* Convert this to duty_cycle in ns */
    for (i = MOTOR_OUTPUTS_SKIPPED; i < (MOTOR_OUTPUTS_SKIPPED + LGC_MAX_GEAR_OUTPUTS); ++i)
    {
        LGC_SharedMemCmd->PeriodHi[i].Hi = LGC_TICK_PER_US * PWM[(i - MOTOR_OUTPUTS_SKIPPED)];
    }
    return;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* LGC_Freq2tick function.                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
uint32 LGC_Freq2tick(uint16 FreqHz)
{
    uint32 duty = LGC_TICK_PER_S / (unsigned long)FreqHz;

    return duty;
}
