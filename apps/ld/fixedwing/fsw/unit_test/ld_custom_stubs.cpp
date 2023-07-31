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

#include "ld_custom_stubs.hpp"

#include <time.h>


CFE_TIME_SysTime_t  CFE_TIME_GetTimeHook(void)
{
    int                 iStatus;
    CFE_TIME_SysTime_t  CfeTime;
    struct timespec     time;

    iStatus = clock_gettime(CLOCK_REALTIME, &time);
    if (iStatus == 0)
    {
        CfeTime.Seconds = time.tv_sec;
        CfeTime.Subseconds = time.tv_nsec / 1000;
    }

    return CfeTime;
}


void CFE_SB_TimeStampMsgHook(CFE_SB_MsgPtr_t MsgPtr)
{
    CFE_SB_SetMsgTime(MsgPtr, CFE_TIME_GetTime());

    return;
}


void CFE_PSP_GetTimeHook(OS_time_t *LocalTime)
{
    int              iStatus;
    struct timespec  time;

    iStatus = clock_gettime(CLOCK_REALTIME, &time);
    if (iStatus == 0)
    {
        LocalTime->seconds = time.tv_sec;
        LocalTime->microsecs = time.tv_nsec / 1000;
    }

    return;
}


uint64 PX4LIB_GetPX4TimeUs(void)
{
    uint64           outTime = 0;
    OS_time_t        localTime = {};

    CFE_PSP_GetTime(&localTime);

    outTime = static_cast<uint64>(static_cast<uint64>(localTime.seconds)
              * static_cast<uint64>(1000000))
              + static_cast<uint64>(localTime.microsecs);

    return outTime;
}


uint64 PX4LIB_GetPX4TimeMs(void)
{
    uint64          outTime = 0;
    OS_time_t       localTime = {};

    CFE_PSP_GetTime(&localTime);

    outTime = static_cast<uint64>(static_cast<uint64>(localTime.seconds)
              * static_cast<uint64>(1000))
              + static_cast<uint64>(static_cast<uint64>(localTime.microsecs)
              % static_cast<uint64>(1000));

    return outTime;
}


uint64 PX4LIB_GetPX4ElapsedTimeUs(uint64 then)
{
    uint64 delta = PX4LIB_GetPX4TimeUs() - then;

    return delta;
}
