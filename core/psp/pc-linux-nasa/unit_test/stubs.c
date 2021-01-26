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

int32 OS_API_Init()
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
	PSP_UT_StubData.CFE.ProcessCoreException_Args.ContextPointer = ContextPointer;
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
