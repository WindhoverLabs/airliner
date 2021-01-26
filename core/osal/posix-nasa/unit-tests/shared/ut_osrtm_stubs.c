
/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_os_stubs.h"
#include <time.h>

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Function definitions
**--------------------------------------------------------------------------------*/


void 	OS_RtmSetCurrentTime(const struct timespec *t)
{
}

void 	OS_RtmGetCurrentTime(struct timespec *t)
{
}

void 	OS_RtmSetStopTime(const struct timespec *t)
{
}

void 	OS_RtmGetStopTime(struct timespec *t)
{
}

void 	OS_RtmTimeAddMicroseconds(struct timespec *t, uint32 m)
{
}

int 	OS_RtmCondTimedWait(pthread_cond_t *cond, pthread_mutex_t *mutex, uint32 microseconds)
{
    return 0;
}

void    OS_RtmAdvanceStopTime(uint32 microseconds)
{
}

OS_RunTimeModeEnum_t OS_RtmGetRunMode(void)
{
    return OS_RUNTIME_MODE_REALTIME;
}

int32   OS_RtmReplishTimeoutByCV(pthread_cond_t* cv, uint32 microseconds)
{
    return 0;
}

void    OS_RtmHoldTime(void)
{
}

int32 	OS_RtmInit(OS_RunTimeModeEnum_t RunMode)
{
    return 0;
}

int 	OS_NanoSleep(const struct timespec *requested_time, struct timespec *remaining)
{
	return nanosleep(requested_time, remaining);
}


/*--------------------------------------------------------------------------------*/

/*================================================================================*
** End of File: ut_oscore_stubs.c
**================================================================================*/
