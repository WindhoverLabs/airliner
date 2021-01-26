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

#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include "osconfig.h"
#include "cfe_psp.h"

/*
** The __USE_UNIX98 is for advanced pthread features on linux
*/
#define __USE_UNIX98
#define _GNU_SOURCE
#include <pthread.h>

#include <limits.h>
#include <time.h>
#include <unistd.h>

/*
** User defined include files
*/
#include "common_types.h"
#include "osapi.h"
#include "ut_osapi_hooks.h"

/* To remove the "implicit declaration of function ‘pthread_setname_np’" warning. */
extern int pthread_getname_np(pthread_t __target_thread, char *__name);

#ifndef SEM_VALUE_MAX
#define SEM_VALUE_MAX INT_MAX
#endif

#define UNINITIALIZED 0
#define MAX_PRIORITY 255
#ifndef PTHREAD_STACK_MIN
   #define PTHREAD_STACK_MIN 8092
#endif

#define OS_API_UNINITIALIZED 0
#define OS_API_INITIALIZED   1

uint32 OS_API_Initialized = OS_API_UNINITIALIZED;

/*tasks */
typedef struct
{
    int       free;
    pthread_t id;
    char      name [OS_MAX_API_NAME];
    int       creator;
    uint32    stack_size;
    uint32    priority;
    void     *delete_hook_pointer;
}OS_task_record_t;

/* queues */
typedef struct
{
	uint32	size;
	char buffer[OS_MAX_QUEUE_WIDTH];
}OS_queue_data_t;

typedef struct
{
    int    			free;
    OS_queue_data_t	qData[OS_MAX_QUEUE_DEPTH];
    uint32 			max_size;
    char   			name [OS_MAX_API_NAME];
    int    			creator;
    pthread_cond_t  cv;
    int32	  		head;
    int32   		tail;
    uint32   		width;
    uint32   		depth;
}OS_queue_record_t;

/* Binary Semaphores */
typedef struct
{
    int             free;
    pthread_mutex_t id;
    pthread_cond_t  cv;
    char            name [OS_MAX_API_NAME];
    int             creator;
    int             max_value;
    int             current_value;
}OS_bin_sem_record_t;

/*Counting Semaphores */
typedef struct
{
    int             free;
    pthread_mutex_t id;
    pthread_cond_t  cv;
    char            name [OS_MAX_API_NAME];
    int             creator;
    int             max_value;
    int             current_value;
}OS_count_sem_record_t;

/* Mutexes */
typedef struct
{
    int             free;
    pthread_mutex_t id;
    char            name [OS_MAX_API_NAME];
    int             creator;
}OS_mut_sem_record_t;

/* function pointer type */
typedef void (*FuncPtr_t)(void);


OS_task_record_t    OS_task_table          [OS_MAX_TASKS];
OS_queue_record_t   OS_queue_table         [OS_MAX_QUEUES];
OS_bin_sem_record_t OS_bin_sem_table       [OS_MAX_BIN_SEMAPHORES];
OS_count_sem_record_t OS_count_sem_table   [OS_MAX_COUNT_SEMAPHORES];
OS_mut_sem_record_t OS_mut_sem_table       [OS_MAX_MUTEXES];

pthread_key_t    thread_key;

pthread_mutex_t OS_task_table_mut;
pthread_mutex_t OS_queue_table_mut;
pthread_mutex_t OS_bin_sem_table_mut;
pthread_mutex_t OS_mut_sem_table_mut;
pthread_mutex_t OS_count_sem_table_mut;
pthread_mutex_t OS_timeout_table_mut;

uint32          OS_printf_enabled = TRUE;
volatile uint32 OS_shutdown = FALSE;

uint32  OS_FindCreator(void);
int32   OS_PriorityRemap(uint32 InputPri);
int     OS_InterruptSafeLock(pthread_mutex_t *lock, sigset_t *set, sigset_t *previous);
void    OS_InterruptSafeUnlock(pthread_mutex_t *lock, sigset_t *previous);


int32 Ut_OS_API_Init()
{
   int                 i;
   int                 ret;
   pthread_mutexattr_t mutex_attr ;
   int32               return_code = OS_SUCCESS;
   int                 sched_policy;

   /* Initialize Task Table */

   for(i = 0; i < OS_MAX_TASKS; i++)
   {
        OS_task_table[i].free                = TRUE;
        OS_task_table[i].creator             = UNINITIALIZED;
        OS_task_table[i].delete_hook_pointer = NULL;
        strcpy(OS_task_table[i].name,"");
    }

    /* Initialize Message Queue Table */

    for(i = 0; i < OS_MAX_QUEUES; i++)
    {
        OS_queue_table[i].free        = TRUE;
        OS_queue_table[i].creator     = UNINITIALIZED;
        strcpy(OS_queue_table[i].name,"");
    }

    /* Initialize Binary Semaphore Table */

    for(i = 0; i < OS_MAX_BIN_SEMAPHORES; i++)
    {
        OS_bin_sem_table[i].free        = TRUE;
        OS_bin_sem_table[i].creator     = UNINITIALIZED;
        strcpy(OS_bin_sem_table[i].name,"");
    }

    /* Initialize Counting Semaphores */
    for(i = 0; i < OS_MAX_COUNT_SEMAPHORES; i++)
    {
        OS_count_sem_table[i].free        = TRUE;
        OS_count_sem_table[i].creator     = UNINITIALIZED;
        strcpy(OS_count_sem_table[i].name,"");
    }

    /* Initialize Mutex Semaphore Table */
    for(i = 0; i < OS_MAX_MUTEXES; i++)
    {
        OS_mut_sem_table[i].free        = TRUE;
        OS_mut_sem_table[i].creator     = UNINITIALIZED;
        strcpy(OS_mut_sem_table[i].name,"");
    }

   /*
   ** Initialize the module loader
   */
   //#ifdef OS_INCLUDE_MODULE_LOADER
   //   return_code = OS_ModuleTableInit();
   //   if ( return_code == OS_ERROR )
   //   {
   //      return(return_code);
   //   }
   //#endif

   /*
   ** Initialize the Timer API
   */
   //return_code = OS_TimerAPIInit();
   //if ( return_code == OS_ERROR )
   //{
   //   return(return_code);
   //}

   ret = pthread_key_create(&thread_key, NULL );
   if ( ret != 0 )
   {
      #ifdef OS_DEBUG_PRINTF
        printf("Error creating thread key\n");
      #endif
      return_code = OS_ERROR;
      return(return_code);
   }

   /*
   ** initialize the pthread mutex attribute structure with default values
   */
   return_code = pthread_mutexattr_init(&mutex_attr);
   if ( return_code != 0 )
   {
      #ifdef OS_DEBUG_PRINTF
         printf("Error: pthread_mutexattr_init failed\n");
      #endif
      return_code = OS_ERROR;
      return (return_code);
   }

   /*
   ** Allow the mutex to use priority inheritance
   */
   return_code = pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_INHERIT) ;
   if ( return_code != 0 )
   {
      #ifdef OS_DEBUG_PRINTF
         printf("Error: pthread_mutexattr_setprotocol failed\n");
      #endif
      return_code = OS_ERROR;
      return (return_code);
   }

   /*
   **  Set the mutex type to RECURSIVE so a thread can do nested locks
   */
   return_code = pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE);
   if ( return_code != 0 )
   {
      #ifdef OS_DEBUG_PRINTF
         printf("Error: pthread_mutexattr_settype failed\n");
      #endif
      return_code = OS_ERROR;
      return (return_code);
   }

   /*
   ** create the mutexes that protect the OSAPI structures
   ** the function returns on error, since we dont want to go through
   ** the extra trouble of creating and deleting resoruces for nothing
   */
   ret = pthread_mutex_init((pthread_mutex_t *) & OS_task_table_mut,&mutex_attr);
   if ( ret != 0 )
   {
      return_code = OS_ERROR;
      return(return_code);
   }
   ret = pthread_mutex_init((pthread_mutex_t *) & OS_queue_table_mut,&mutex_attr);
   if ( ret != 0 )
   {
      return_code = OS_ERROR;
      return(return_code);
   }
   ret = pthread_mutex_init((pthread_mutex_t *) & OS_bin_sem_table_mut,&mutex_attr);
   if ( ret != 0 )
   {
      return_code = OS_ERROR;
      return(return_code);
   }
   ret = pthread_mutex_init((pthread_mutex_t *) & OS_count_sem_table_mut,&mutex_attr);
   if ( ret != 0 )
   {
      return_code = OS_ERROR;
      return(return_code);
   }
   ret = pthread_mutex_init((pthread_mutex_t *) & OS_mut_sem_table_mut,&mutex_attr);
   if ( ret != 0 )
   {
      return_code = OS_ERROR;
      return(return_code);
   }
   ret = pthread_mutex_init((pthread_mutex_t *) & OS_timeout_table_mut,&mutex_attr);
   if ( ret != 0 )
   {
      return_code = OS_ERROR;
      return(return_code);
   }

   /*
   ** File system init
   */
   //return_code = OS_FS_Init();

   OS_API_Initialized = OS_API_INITIALIZED;

   return(return_code);
}


int32 Ut_OSAPI_QueueCreateHook(uint32 *queue_id, const char *queue_name, uint32 queue_depth,
                      uint32 data_size, uint32 flags)
{
	int						ret;
    int                     i;
    uint32                  possible_qid;
    sigset_t                previous;
    sigset_t                mask;

    /* Ensure the OSAL API is initialized first. */
    if(OS_API_Initialized != OS_API_INITIALIZED) {
    	Ut_OS_API_Init();
    }

    /* Check Parameters */

    if ( queue_id == NULL || queue_name == NULL)
    {
        return OS_INVALID_POINTER;
    }

    /* we don't want to allow names too long*/
    /* if truncated, two names might be the same */

    if (strlen(queue_name) >= OS_MAX_API_NAME)
    {
        return OS_ERR_NAME_TOO_LONG;
    }

    /* Ensure the queue depth isn't larger than our maximum depth. */
    if(queue_depth > OS_MAX_QUEUE_DEPTH) {
    	return OS_QUEUE_INVALID_SIZE;
    }

    /* Ensure the queue depth isn't zero. */
    if(queue_depth == 0) {
    	return OS_QUEUE_INVALID_SIZE;
    }

    OS_InterruptSafeLock(&OS_queue_table_mut, &mask, &previous);

    for(possible_qid = 0; possible_qid < OS_MAX_QUEUES; possible_qid++)
    {
        if (OS_queue_table[possible_qid].free == TRUE)
            break;
    }

    if( possible_qid >= OS_MAX_QUEUES || OS_queue_table[possible_qid].free != TRUE)
    {
        OS_InterruptSafeUnlock(&OS_queue_table_mut, &previous);
        return OS_ERR_NO_FREE_IDS;
    }

    /* Check to see if the name is already taken */
    for (i = 0; i < OS_MAX_QUEUES; i++)
    {
        if ((OS_queue_table[i].free == FALSE) &&
            strcmp ((char*) queue_name, OS_queue_table[i].name) == 0)
        {
            OS_InterruptSafeUnlock(&OS_queue_table_mut, &previous);
            return OS_ERR_NAME_TAKEN;
        }
    }

    /* Set the possible task Id to not free so that
     * no other task can try to use it */
    *queue_id = possible_qid;

    OS_queue_table[*queue_id].free = FALSE;

    ret = pthread_cond_init(&(OS_queue_table[possible_qid].cv), NULL);
	if ( ret != 0 )
	{
        OS_InterruptSafeUnlock(&OS_queue_table_mut, &previous);
        return OS_ERROR;
	}

    OS_queue_table[*queue_id].free = FALSE;
    OS_queue_table[*queue_id].max_size = data_size;
    strcpy( OS_queue_table[*queue_id].name, (char*) queue_name);
    OS_queue_table[*queue_id].creator = OS_FindCreator();
	OS_queue_table[*queue_id].width = data_size;
	OS_queue_table[*queue_id].depth = queue_depth;
	OS_queue_table[*queue_id].head = -1;
	OS_queue_table[*queue_id].tail = -1;

    OS_InterruptSafeUnlock(&OS_queue_table_mut, &previous);

    return OS_SUCCESS;

}/* end OS_QueueCreate */



int32 Ut_OSAPI_QueueGetHook(uint32 queue_id, void *data, uint32 size, uint32 *size_copied, int32 timeout)
{

    struct timespec ts;
    int				ret;
    sigset_t   		previous;
    sigset_t   		mask;
	int32 			headIndex;
	OS_queue_data_t *qData = 0;

    /* Ensure the OSAL API is initialized first. */
    if(OS_API_Initialized != OS_API_INITIALIZED) {
    	return OS_ERROR;
    }

    OS_InterruptSafeLock(&OS_queue_table_mut, &mask, &previous);
    /*
    ** Check Parameters
    */
    if(queue_id >= OS_MAX_QUEUES || OS_queue_table[queue_id].free == TRUE)
    {
		OS_InterruptSafeUnlock(&OS_queue_table_mut, &previous);
        return OS_ERR_INVALID_ID;
    }
    else if( (data == NULL) || (size_copied == NULL) )
    {
		OS_InterruptSafeUnlock(&OS_queue_table_mut, &previous);
        return OS_INVALID_POINTER;
    }
    else if( size < OS_queue_table[queue_id].max_size )
    {
        /*
        ** The buffer that the user is passing in is potentially too small
        ** RTEMS will just copy into a buffer that is too small
        */
        *size_copied = 0;
		OS_InterruptSafeUnlock(&OS_queue_table_mut, &previous);
        return OS_QUEUE_INVALID_SIZE;
    }

    if(OS_queue_table[queue_id].head == -1)
    {
    	if (timeout == OS_PEND)
    	{
    		return OS_ERROR;
    	}
    	else if (timeout == OS_CHECK)
    	{
        	/* The queue is empty so return 0. */
            *size_copied = 0;
			OS_InterruptSafeUnlock(&OS_queue_table_mut, &previous);
			return OS_QUEUE_EMPTY;
    	}
    	else /* timeout */
    	{
    		return OS_ERROR;
        }
    }

	/* The queue is not empty.  Pop an object from the head. */
	headIndex = OS_queue_table[queue_id].head;
	qData = &OS_queue_table[queue_id].qData[headIndex];
	memcpy(data, &qData->buffer[0], qData->size);
	if(qData->size != size)
	{
		*size_copied = 0;
		OS_InterruptSafeUnlock(&OS_queue_table_mut, &previous);
		OS_printf("OS_QueueGet ---  OS_QUEUE_INVALID_SIZE\n");
		return OS_QUEUE_INVALID_SIZE;
	}

	*size_copied = qData->size;

	/* Check to see if there are more objects in the queue. */
	if(OS_queue_table[queue_id].tail == headIndex)
	{
		/* There are no more objects in the queue.  Set the queue to empty
		 * by setting the head to -1.
		 */
		OS_queue_table[queue_id].head = -1;
	}
	else
	{
		/* There are more objects in the queue.  Move the head to the next
		 * object. */
		if((headIndex + 1) >= OS_queue_table[queue_id].depth)
		{
			/* The head is at the end of the array.  Move it back to the
			 * beginning.
			 */
			OS_queue_table[queue_id].head = 0;
		}
		else
		{
			OS_queue_table[queue_id].head++;
		}
	}

	OS_InterruptSafeUnlock(&OS_queue_table_mut, &previous);

	return OS_SUCCESS;
}



int32 Ut_OSAPI_QueuePutHook(uint32 queue_id, const void *data, uint32 size, uint32 flags)
{
    sigset_t   		previous;
    sigset_t   		mask;
    OS_queue_data_t *qData;
    int				ret;

	pthread_t tid = pthread_self();
	char threadName[100];
	pthread_getname_np(tid, threadName);

    /* Ensure the OSAL API is initialized first. */
    if(OS_API_Initialized != OS_API_INITIALIZED) {
    	return OS_ERROR;
    }

    OS_InterruptSafeLock(&OS_queue_table_mut, &mask, &previous);

    /*
    ** Check Parameters
    */
    if(queue_id >= OS_MAX_QUEUES || OS_queue_table[queue_id].free == TRUE)
    {
        return OS_ERR_INVALID_ID;
    }

    if (data == NULL)
    {
        return OS_INVALID_POINTER;
    }

    if(size > OS_queue_table[queue_id].width)
    {
    	return OS_QUEUE_INVALID_SIZE;
    }

    /* Check if queue is full. */
    if(((OS_queue_table[queue_id].head == 0) && (OS_queue_table[queue_id].tail == (OS_queue_table[queue_id].depth-1))) ||
       (OS_queue_table[queue_id].tail == (OS_queue_table[queue_id].head - 1)))
    {
    	OS_InterruptSafeUnlock(&OS_queue_table_mut, &previous);
    	return OS_QUEUE_FULL;
    }

    if(OS_queue_table[queue_id].head == -1)
    {
    	OS_queue_table[queue_id].head = 0;
    	OS_queue_table[queue_id].tail = 0;
    }
    else
    {
		/* Determine where to move the tail. */
		if(OS_queue_table[queue_id].tail >= (OS_queue_table[queue_id].depth - 1))
		{
			/* The tail is at the end of the array.  Move it back to the beginning.
			 */
			OS_queue_table[queue_id].tail = 0;
		}
		else
		{
			OS_queue_table[queue_id].tail++;
		}
    }

    /* Copy the data to the new tail object. */
    qData = &OS_queue_table[queue_id].qData[OS_queue_table[queue_id].tail];
    memcpy(&qData->buffer, data, size);
    qData->size = size;
    ret = pthread_cond_signal(&OS_queue_table[queue_id].cv);
	OS_InterruptSafeUnlock(&OS_queue_table_mut, &previous);

    if(ret < 0)
    {
    	return OS_ERROR;
    }

    return OS_SUCCESS;
}



int32 Ut_OSAPI_QueueGetIdByName (uint32 *queue_id, const char *queue_name)
{
    uint32 i;

    /* Ensure the OSAL API is initialized first. */
    if(OS_API_Initialized != OS_API_INITIALIZED) {
    	return OS_ERROR;
    }

    if(queue_id == NULL || queue_name == NULL)
    {
       return OS_INVALID_POINTER;
    }

    /* a name too long wouldn't have been allowed in the first place
     * so we definitely won't find a name too long*/

    if (strlen(queue_name) >= OS_MAX_API_NAME)
    {
       return OS_ERR_NAME_TOO_LONG;
    }

    for (i = 0; i < OS_MAX_QUEUES; i++)
    {
        if (OS_queue_table[i].free != TRUE &&
           (strcmp(OS_queue_table[i].name, (char*) queue_name) == 0 ))
        {
            *queue_id = i;
            return OS_SUCCESS;
        }
    }

    /* The name was not found in the table,
     *  or it was, and the queue_id isn't valid anymore */
    return OS_ERR_NAME_NOT_FOUND;

}/* end OS_QueueGetIdByName */



/*
**
**   Name: OS_InterruptSafeLock
**
**   Purpose: This function masks signals and locks a mutex for mutual exclusion
**
*/
int OS_InterruptSafeLock(pthread_mutex_t *lock, sigset_t *set, sigset_t *previous)
{
    /* Block all signals */
    sigemptyset(set);
    sigemptyset(previous);
    sigfillset(set);

    if (pthread_sigmask(SIG_SETMASK, set, previous) == 0)
    {
        /* Acquire the lock */
    	int rc = pthread_mutex_lock(lock);
    	if(rc != 0)
    	{
    		OS_printf("OS_InterruptSafeLock:  pthread_mutex_lock(lock) returned %i\n", rc);
    	}
    	return rc;
    }
    else
    {
        return EINVAL;
    }
}

/*
**
**   Name: OS_InterruptSafeUnlock
**
**   Purpose: This function unlocks the mutex and restores the signal mask
**
*/
void OS_InterruptSafeUnlock(pthread_mutex_t *lock, sigset_t *previous)
{
    /* Release the lock */
	int rc = pthread_mutex_unlock(lock);
	if(rc != 0)
	{
		OS_printf("OS_InterruptSafeUnlock:  pthread_mutex_unlock(lock) returned %i\n", rc);
	}

    /* Restore previous signals */
    pthread_sigmask(SIG_SETMASK, previous, 0);
    sigdelset(previous, 0);
}



uint32 OS_FindCreator(void)
{
    pthread_t    pthread_id;
    uint32 i;

    pthread_id = pthread_self();
    /*
    ** Get PTHREAD Id
    */
    for (i = 0; i < OS_MAX_TASKS; i++)
    {
        if (pthread_equal(pthread_id, OS_task_table[i].id) != 0 )
        {
            break;
        }
    }

    return i;
}

void Ut_OS_API_Clear(void)
{
    CFE_PSP_MemSet(&OS_task_table, 0x00, sizeof(OS_task_table));
    CFE_PSP_MemSet(&OS_queue_table, 0x00, sizeof(OS_queue_table));
    CFE_PSP_MemSet(&OS_bin_sem_table, 0x00, sizeof(OS_bin_sem_table));
    CFE_PSP_MemSet(&OS_count_sem_table, 0x00, sizeof(OS_count_sem_table));
    CFE_PSP_MemSet(&OS_mut_sem_table, 0x00, sizeof(OS_mut_sem_table));
    
    Ut_OS_API_Init();
}
