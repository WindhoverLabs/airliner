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

#include "sch_mock_os.h"

int OS_RUNTIME_MODE = 0;
int MINOR_FRAME;

void __wrap_OS_RtmEndFrame(void)
{
	if(OS_RUNTIME_MODE == 1)
	{
		//OS_RtmData.State = OS_RTM_STATE_NONREALTIME_HOLD;
		//pthread_cond_broadcast(&OS_RtmData.CV);
	}
	else
	{
		//OS_RtmData.CurrentFrameCount++;
		//OS_RtmData.CurrentMinorFrame = OS_RtmData.CurrentFrameCount % OS_RtmData.MinorFramesPerMajorFrame;
	}
}

OS_RunTimeModeEnum_t __wrap_OS_RtmGetRunMode(void)
{
    return OS_RUNTIME_MODE;
}

void __wrap_OS_RtmSetMajorAndMinorFrame(uint32 MajorFrameLength, uint32 MinorFramesPerMajorFrame)
{

}

uint32 __wrap_OS_RtmGetCurrentMinorFrame(void)
{
	return MINOR_FRAME;
}


void __wrap_OS_RtmEngageStepMode(void)
{

}
