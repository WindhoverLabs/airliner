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

#include "common_types.h"

#define OS_MAX_API_NAME 255
#include "osapi-os-rtm.h"
#include "osapi-os-core.h"

#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

/*
** The __USE_UNIX98 is for advanced pthread features on linux
*/
#define __USE_UNIX98
#define _GNU_SOURCE
#include <pthread.h>

#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include "stubs.h"

#define OS_BUFFER_SIZE 1000

PSP_UT_StubData_t PSP_UT_StubData;

int32 OS_API_InitEx(OS_RunTimeModeEnum_t Mode)
{
    return 0;
}


void CFE_ES_Main(uint32 StartType, uint32 StartSubtype, uint32 ModeId , const char *StartFilePath )
{
	PSP_UT_StubData.CFE.CallCounts.CFE_ES_Main_Func++;
	PSP_UT_StubData.CFE.Main_Args.StartType = StartType;
	PSP_UT_StubData.CFE.Main_Args.StartSubtype = StartSubtype;
	PSP_UT_StubData.CFE.Main_Args.ModeId = ModeId;
	if(StartFilePath != 0)
	{
		strncpy(PSP_UT_StubData.CFE.Main_Args.StartFilePath, StartFilePath, sizeof(PSP_UT_StubData.CFE.Main_Args.StartFilePath));
	}
}


void CFE_TIME_Local1HzISR(void)
{
	PSP_UT_StubData.CFE.CallCounts.CFE_TIME_Local1HzISR_Func++;
}


int32 OS_TaskDelete(uint32 task_id)
{
    return 0;
}


int32 OS_BinSemDelete(uint32 sem_id)
{
    return 0;
}


int32 OS_CountSemDelete(uint32 sem_id)
{
    return 0;
}


int32 OS_MutSemDelete(uint32 sem_id)
{
    return 0;
}


int32 OS_QueueDelete(uint32 queue_id)
{
    return 0;
}


int32 OS_TimerDelete(uint32 timer_id)
{
    return 0;
}


void OS_printf( const char *String, ...)
{
    va_list     ptr;
    char msg_buffer[OS_BUFFER_SIZE];

    va_start(ptr,String);
    vsnprintf(&msg_buffer[0], (size_t)OS_BUFFER_SIZE, String, ptr);
    va_end(ptr);
    
    msg_buffer[OS_BUFFER_SIZE -1] = '\0';
    printf("%s", &msg_buffer[0]);
    fflush(stdout);
}


void CFE_ES_ProcessCoreException(uint32  HostTaskId,     const char *ReasonString,
                                 const uint32 *ContextPointer, uint32 ContextSize)
{
	PSP_UT_StubData.CFE.CallCounts.CFE_ES_ProcessCoreException_Func++;
	PSP_UT_StubData.CFE.ProcessCoreException_Args.HostTaskId = HostTaskId;
	if(ReasonString != 0)
	{
		strncpy(PSP_UT_StubData.CFE.ProcessCoreException_Args.ReasonString,
				ReasonString,
				sizeof(PSP_UT_StubData.CFE.ProcessCoreException_Args.ReasonString));
	}
	PSP_UT_StubData.CFE.ProcessCoreException_Args.ContextPointer = (uint32*)ContextPointer;
	PSP_UT_StubData.CFE.ProcessCoreException_Args.ContextSize = ContextSize;
}


int32 OS_GetLocalTime(OS_time_t *time_struct)
{
	time_struct->seconds = 123;
	time_struct->microsecs = 456;

    return 0;
}



void PSP_UT_ResetStubData(void)
{
	memset(&PSP_UT_StubData, 0, sizeof(PSP_UT_StubData_t));
}
