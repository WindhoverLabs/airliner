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

void        Ut_CFE_ES_ClearSysLogQueue(void);
uint32      Ut_CFE_EVS_GetEventQueueDepth(void);
int32       Ut_CFE_ES_RunLoopHook(uint32 *ExitStatus);
int32       Ut_CFE_ES_WriteToSysLog(const char *SysLogEntryText);
osalbool     Ut_CFE_ES_SysLogWritten(const char *ExpectedSysLogEntryText);
uint32      Ut_CFE_ES_GetSysLogQueueDepth(void);




int32  Ut_SetConfigTablePtr(MultirotorMixer_ConfigTablePtr_t &ConfigTablePtr);
uint32 Ut_mix(float *outputs, uint32 space, uint16 *status_reg);
uint16 Ut_get_saturation_status(void);
void   Ut_groups_required(uint32 &groups);

uint32 Ut_set_trim(float trim)
{
    return 0;
}

/**
 * @brief      Sets the thrust factor used to calculate mapping from desired thrust to pwm.
 *
 * @param[in]  val   The value
 */
virtual void            set_thrust_factor(float val) {m_ThrustFactor = val;}

