/*
** File: osapi-os-core.h
**
**      Copyright (c) 2004-2006, United States government as represented by the 
**      administrator of the National Aeronautics Space Administration.  
**      All rights reserved. This software was created at NASAs Goddard 
**      Space Flight Center pursuant to government contracts.
**
**      This is governed by the NASA Open Source Agreement and may be used, 
**      distributed and modified only pursuant to the terms of that agreement.
**
** Author:  Ezra Yeheksli -Code 582/Raytheon
**
** Purpose: Contains functions prototype definitions and variables declarations
**          for the OS Abstraction Layer, Core OS module
**
** $Revision: 1.8 $ 
**
** $Date: 2013/07/25 10:02:00GMT-05:00 $
**
** $Log: osapi-os-core.h  $
** Revision 1.8 2013/07/25 10:02:00GMT-05:00 acudmore 
** removed circular include "osapi.h"
** Revision 1.7 2012/04/11 09:30:48GMT-05:00 acudmore 
** Added OS_printf_enable and OS_printf_disable
** Revision 1.6 2010/11/12 12:00:17EST acudmore 
** replaced copyright character with (c) and added open source notice where needed.
** Revision 1.5 2010/11/10 15:33:14EST acudmore 
** Updated IntAttachHandler prototype
** Revision 1.4 2010/03/08 12:06:28EST acudmore 
** added function pointer type to get rid of warnings
** Revision 1.3 2010/02/01 12:37:15EST acudmore 
** added return code to OS API init
** Revision 1.2 2009/08/04 10:49:09EDT acudmore 
**   
*/

#ifndef _osapi_core_
#define _osapi_core_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>   /* for va_list */
#include "osapi-os-rtm.h" 

/* Defines constants for OS_BinSemCreate for state of semaphore  */
#define OS_SEM_FULL     1
#define OS_SEM_EMPTY    0

/* #define for enabling floating point operations on a task*/
#define OS_FP_ENABLED   0x0001

/* Bits 1-8, of the "flags" argument in the OS_TaskCreate function represent
 * per-thread core affinity.  Set all bits to 0 to allow the thread to run on
 * all available cores.  To allow a thread to run on core 0, set flags bit 1
 * to 1 and all others to 0.  To allow a thread to run on core 2 and 3, set
 * flags bit 3 and 4 to 1 and all others to 0.  Or use, the CORE_ID macros
 * below.
 */
#define OS_ENABLE_CORE_MASK   0x01fe
#define OS_ENABLE_CORE_OFFSET 1
#define OS_ENABLE_CORE_ALL    0x0000
#define OS_ENABLE_CORE_0      0x0002
#define OS_ENABLE_CORE_1      0x0004
#define OS_ENABLE_CORE_2      0x0008
#define OS_ENABLE_CORE_3      0x0010
#define OS_ENABLE_CORE_4      0x0020
#define OS_ENABLE_CORE_5      0x0040
#define OS_ENABLE_CORE_6      0x0080
#define OS_ENABLE_CORE_7      0x0100

/*  tables for the properties of objects */

/** 
**  \brief Tasks
*/
typedef struct
{
    char name [OS_MAX_API_NAME];
    uint32 creator;
    uint32 stack_size;
    uint32 priority;
    uint32 OStask_id;
}OS_task_prop_t;
    
/** 
**  \brief Queues
*/
typedef struct
{
    char name [OS_MAX_API_NAME];
    uint32 creator;
}OS_queue_prop_t;

/** 
**  \brief Binary Semaphores
*/
typedef struct
{                     
    char name [OS_MAX_API_NAME];
    uint32 creator;
    int32  value;
}OS_bin_sem_prop_t;

/** 
**  \brief Counting Semaphores
*/
typedef struct
{                     
    char name [OS_MAX_API_NAME];
    uint32 creator;
    int32 value;
}OS_count_sem_prop_t;

/** 
**  \brief Mutexes
*/
typedef struct
{
    char name [OS_MAX_API_NAME];
    uint32 creator;
}OS_mut_sem_prop_t;


/** 
**  \brief Struct for #OS_GetLocalTime()
*/
typedef struct 
{ 
    uint32 seconds; 
    uint32 microsecs;
}OS_time_t; 

/** 
**  \brief Heap info
*/
typedef struct
{
    uint32 free_bytes;
    uint32 free_blocks;
    uint32 largest_free_block;
}OS_heap_prop_t;

// TODO: what to do for typedef doxy

/* This typedef is for the OS_GetErrorName function, to ensure
 * everyone is making an array of the same length.
 *
 * Implementation note for developers:
 *
 * The sizes of strings in OSAL functions are built with this
 * OS_ERROR_NAME_LENGTH limit in mind.  Always check the uses of os_err_name_t
 * when changing this value.
 */

#define OS_ERROR_NAME_LENGTH     35
typedef char os_err_name_t[OS_ERROR_NAME_LENGTH];

/*
** These typedefs are for the task entry point
*/
typedef void osal_task;
typedef osal_task ((*osal_task_entry)(void));

/*
** Typedef for general purpose OSAL callback functions
** This may be used by multiple APIS
*/
typedef void (*OS_ArgCallback_t)(uint32 object_id, void *arg);


/*
**  External Declarations
*/

/*
** Prototype for application startup function.
** This is implemented by the user application
*/
/************************************************************************/
/** \brief TODO
**
**  \par Description
**       This function 
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void OS_Application_Startup(void);


/*
** Exported Functions
*/

/************************************************************************/
/** \brief Initialization of API
**
**  \par Description
**       Initialize the tables that the OS API uses to keep track of information
**       about objects
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   Mode    #OS_RunTimeModeEnum_t
**  
**  \returns
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \retcode #OS_ERROR  \retdesc \copydoc OS_ERROR \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_API_InitEx (OS_RunTimeModeEnum_t Mode);

/************************************************************************/
/** \brief Initialization of API
**
**  \par Description
**       Initialize the tables that the OS API uses to keep track of information
**       about objects
**
**  \par Assumptions, External Events, and Notes:
**       None
**  
**  \returns
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \retcode #OS_ERROR  \retdesc \copydoc OS_ERROR \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_API_Init (void);

/************************************************************************/
/** \brief OS-specific background thread implementation - waits forever for events to occur.
**
**  \par Description
**       This should be called from the BSP main routine / initial thread after all other 
**       board / application initialization has taken place and all other tasks are running.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void OS_IdleLoop (void);

/************************************************************************/
/** \brief Delete all objects
**
**  \par Description
**       OS_DeleteAllObjects() provides a means to clean up all resources allocated by this 
**       instance of OSAL.  It would typically be used during an orderly shutdown but may also
**       be helpful for testing purposes.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void OS_DeleteAllObjects       (void);

/************************************************************************/
/** \brief Application Shutdown
**
**  \par Description
**       OS_ApplicationShutdown() provides a means for a user-created thread to request the orderly
**       shutdown of the whole system, such as part of a user-commanded reset command.
**       This is preferred over e.g. ApplicationExit() which exits immediately and does not
**       provide for any means to clean up first.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void OS_ApplicationShutdown         (uint8 flag);


/*
** Task API
*/

/************************************************************************/
/** \brief Create a task
**
**  \par Description
**       Creates a task and starts running it.
**
**  \par Assumptions, External Events, and Notes:
**       task_id is passed back to the user as the ID. stack_pointer is usually null.
**       the flags parameter is unused.
**
**  \param [in/out]   task_id           #uint32 Pointer to task ID number
**  \param [in]   task_name         Task name
**  \param [in]   function_pointer  Pointer to task function
**  \param [in]   stack_pointer     #uint32 Pointer to stack
**  \param [in]   stack_size        #uint32 Pointer to stack size
**  \param [in]   priority          #uint32 Pointer to priority
**  \param [in]   flags             #uint32 Flags 
**  
**  \returns
**  \retcode #OS_INVALID_POINTER if any of the necessary pointers are NULL \endcode
**  \retcode #OS_ERR_NAME_TOO_LONG if the name of the task is too long to be copied \endcode
**  \retcode #OS_ERR_INVALID_PRIORITY if the priority is bad \endcode
**  \retcode #OS_ERR_NO_FREE_IDS if there can be no more tasks created \endcode
**  \retcode #OS_ERR_NAME_TAKEN if the name specified is already used by a task \endcode
**  \retcode #OS_ERROR  \retdesc \copydoc OS_ERROR \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_TaskCreate            (uint32 *task_id, const char *task_name, 
                                osal_task_entry function_pointer,
                                uint32 *stack_pointer,
                                uint32 stack_size,
                                uint32 priority, uint32 flags);

/************************************************************************/
/** \brief Delete task
**
**  \par Description
**       Deletes the specified Task and removes it from the OS_task_table.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   task_id           #uint32 Task ID number
**  
**  \returns
**  \retcode #OS_ERR_INVALID_ID if the ID given to it is invalid \endcode
**  \retcode #OS_ERROR  \retdesc \copydoc OS_ERROR \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_TaskDelete            (uint32 task_id); 

/************************************************************************/
/** \brief Exit task
**
**  \par Description
**       Exits the calling task and removes it from the OS_task_table.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void OS_TaskExit               (void);

/************************************************************************/
/** \brief Delete task handler
**
**  \par Description
**       Installs a handler for when the task is deleted.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   function_pointer  Pointer to task function
**  
**  \returns
**  \retcode #OS_ERR_INVALID_ID if the ID given to it is invalid \endcode
**  \retcode #OS_ERROR  \retdesc \copydoc OS_ERROR \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_TaskInstallDeleteHandler(osal_task_entry function_pointer);

/************************************************************************/
/** \brief Delay task
**
**  \par Description
**       Delay a task for specified amount of milliseconds
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   millisecond  #uint32 Time in milliseconds
**  
**  \returns
**  \retcode #OS_ERROR  \retdesc \copydoc OS_ERROR \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_TaskDelay             (uint32 millisecond);

/************************************************************************/
/** \brief Set task priority
**
**  \par Description
**       Sets the given task to a new priority
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   task_id           #uint32 Task ID number
**  \param [in]   new_priority      #uint32 Priority
**  
**  \returns
**  \retcode #OS_ERR_INVALID_ID if the ID given to it is invalid \endcode
**  \retcode #OS_ERR_INVALID_PRIORITY if the priority is greater than the max allowed \endcode
**  \retcode #OS_ERROR  \retdesc \copydoc OS_ERROR \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_TaskSetPriority       (uint32 task_id, uint32 new_priority);

/************************************************************************/
/** \brief Register task
**
**  \par Description
**       Registers the calling task id with the task by adding the var to the tcb
         It searches the OS_task_table to find the task_id corresponding to the tcb_id
**
**  \par Assumptions, External Events, and Notes:
**       None
**  
**  \returns
**  \retcode #OS_ERR_INVALID_ID if the ID given to it is invalid \endcode
**  \retcode #OS_ERROR  \retdesc \copydoc OS_ERROR \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_TaskRegister          (void);

/************************************************************************/
/** \brief Get task id
**
**  \par Description
**       This function returns the #defined task id of the calling task
**
**  \par Assumptions, External Events, and Notes:
**       The OS_task_key is initialized by the task switch if AND ONLY IF the 
**       OS_task_key has been registered via OS_TaskRegister(..).  If this is not 
**       called prior to this call, the value will be old and wrong.
**  
**  \returns
**  ID of calling task
**  \endreturns
**
*************************************************************************/
uint32 OS_TaskGetId            (void);

/************************************************************************/
/** \brief Get task id by name
**
**  \par Description
**       Sets the given task to a new priority
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in/out]   task_id           #uint32 Task ID number
**  \param [in]   task_name         Task name
**  
**  \returns
**  \retcode #OS_INVALID_POINTER if the pointers passed in are NULL \endcode
**  \retcode #OS_ERR_NAME_TOO_LONG if th ename to found is too long to begin with \endcode
**  \retcode #OS_ERR_NAME_NOT_FOUND if the name wasn't found in the table \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_TaskGetIdByName       (uint32 *task_id, const char *task_name);

/************************************************************************/
/** \brief Get task info
**
**  \par Description
**       This function will pass back a pointer to structure that contains 
**       all of the relevant info (creator, stack size, priority, name) about the 
**       specified task. 
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   task_id           #uint32 Task ID number
**  \param [in/out]   task_prop     #OS_task_prop_t Task properties
**  
**  \returns
**  \retcode #OS_INVALID_POINTER if the pointers passed in are NULL \endcode
**  \retcode #OS_ERR_INVALID_ID if the ID passed to it is invalid \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_TaskGetInfo           (uint32 task_id, OS_task_prop_t *);          



/*
** Message Queue API
*/

/*
** Queue Create now has the Queue ID returned to the caller.
*/

/************************************************************************/
/** \brief Create queue
**
**  \par Description
**       Create a message queue which can be refered to by name or ID
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in/out]   queue_id           #uint32 Queue ID number
**  \param [in]   queue_name         Queue name
**  \param [in]   queue_depth        #uint32 Queue depth
**  \param [in]   data_size          #uint32 Queue size
**  \param [in]   flags              #uint32 Flags
**  
**  \returns
**  \retcode #OS_INVALID_POINTER if the pointers passed in are NULL \endcode
**  \retcode #OS_ERR_NAME_TOO_LONG if th ename to found is too long to begin with \endcode
**  \retcode #OS_ERR_NO_FREE_IDS if there are already the max queues created \endcode
**  \retcode #OS_ERR_NAME_TAKEN if the name is already being used on another queue \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_QueueCreate           (uint32 *queue_id, const char *queue_name,
                                uint32 queue_depth, uint32 data_size, uint32 flags);

/************************************************************************/
/** \brief Delete queue
**
**  \par Description
**       Deletes the specified message queue.
**
**  \par Assumptions, External Events, and Notes:
**       If There are messages on the queue, they will be lost and any subsequent
**       calls to QueueGet or QueuePut to this queue will result in errors
**
**  \param [in]   queue_id           #uint32 Queue ID number
**  
**  \returns
**  \retcode #OS_ERR_INVALID_ID if the ID given to it is invalid \endcode
**  \retcode #OS_ERROR  \retdesc \copydoc OS_ERROR \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_QueueDelete           (uint32 queue_id);

/************************************************************************/
/** \brief Queue get
**
**  \par Description
**       Receive a message on a message queue.  Will pend or timeout on the receive.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]         queue_id           #uint32 Queue ID number
**  \param [in/out]     data               #uint32 Pointer to data
**  \param [in]         data_size          #uint32 Size of data buffer
**  \param [in/out]     size_copied        #uint32 Size copied
**  \param [in]         timeout            #int32 timeout
**  
**  \returns
**  \retcode #OS_INVALID_POINTER if the pointers passed in are NULL \endcode
**  \retcode #OS_ERR_INVALID_ID if the given ID does not exist \endcode
**  \retcode #OS_QUEUE_EMPTY if the Queue has no messages on it to be recieved \endcode
**  \retcode #OS_QUEUE_TIMEOUT if the timeout was OS_PEND and the time expired \endcode
**  \retcode #OS_QUEUE_INVALID_SIZE if the size of the buffer passed in is not big enough for the maximum size messaged \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_QueueGet              (uint32 queue_id, void *data, uint32 size, 
                                uint32 *size_copied, int32 timeout);

/************************************************************************/
/** \brief Queue put
**
**  \par Description
**       Put a message on a message queue.
**
**  \par Assumptions, External Events, and Notes:
**       The flags parameter is not used.  The message put is always configured to
**       immediately return an error if the receiving message queue is full.
**
**  \param [in]   queue_id           #uint32 Queue ID number
**  \param [in]   data               #uint32 Pointer to data
**  \param [in]   data_size          #uint32 Data size
**  \param [in]   flags              #uint32 Flags
**  
**  \returns
**  \retcode #OS_INVALID_POINTER if the pointers passed in are NULL \endcode
**  \retcode #OS_ERR_INVALID_ID if the queue id passed in is not a valid queue \endcode
**  \retcode #OS_QUEUE_FULL if the queue cannot accept another message \endcode
**  \retcode #OS_ERROR  \retdesc \copydoc OS_ERROR \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_QueuePut              (uint32 queue_id, const void *data, uint32 size,
                                uint32 flags);

/************************************************************************/
/** \brief Get queue ID by name
**
**  \par Description
**       This function tries to find a queue Id given the name of the queue. The
**       id of the queue is passed back in queue_id
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in/out]   queue_id           #uint32 Queue ID number
**  \param [in]   queue_name            Queue name
**  
**  \returns
**  \retcode #OS_INVALID_POINTER if the pointers passed in are NULL \endcode
**  \retcode #OS_ERR_NAME_TOO_LONG if th ename to found is too long to begin with \endcode
**  \retcode #OS_ERR_NAME_NOT_FOUND the name was not found in the table \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_QueueGetIdByName      (uint32 *queue_id, const char *queue_name);

/************************************************************************/
/** \brief Get queue info
**
**  \par Description
**       This function will pass back a pointer to structure that contains 
**       all of the relevant info (name and creator) about the specified queue. 
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]     queue_id           #uint32 Queue ID number
**  \param [in/out] queue_prop         #OS_queue_prop_t Queue properties struct pointer
**  
**  \returns
**  \retcode #OS_INVALID_POINTER if the pointers passed in are NULL \endcode
**  \retcode #OS_ERR_INVALID_ID if the ID given is not  a valid queue \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_QueueGetInfo          (uint32 queue_id, OS_queue_prop_t *queue_prop);

/*
** Semaphore API
*/

/************************************************************************/
/** \brief Create binary semaphore
**
**  \par Description
**       Creates a binary semaphore with initial value specified by
**       sem_initial_value and name specified by sem_name. sem_id will be 
**       returned to the caller
**
**  \par Assumptions, External Events, and Notes:
**       options is an unused parameter 
**
**  \param [in/out]   sem_id           #uint32 Semaphore ID number
**  \param [in]   sem_name             Semaphore name
**  \param [in]   sem_initial_value    #uint32 Semaphore initial value
**  \param [in]   options               #uint32 Semaphore size
**  
**  \returns
**  \retcode #OS_INVALID_POINTER if the pointers passed in are NULL \endcode
**  \retcode #OS_ERR_NAME_TOO_LONG if th ename to found is too long to begin with \endcode
**  \retcode #OS_ERR_NO_FREE_IDS if there are already the max queues created \endcode
**  \retcode #OS_ERR_NAME_TAKEN if the name is already being used on another queue \endcode
**  \retcode #OS_SEM_FAILURE if the OS call failed \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_BinSemCreate          (uint32 *sem_id, const char *sem_name, 
                                uint32 sem_initial_value, uint32 options);

/************************************************************************/
/** \brief Flush binary semaphore
**
**  \par Description
**       The function unblocks all tasks pending on the specified semaphore. However,
**       this function does not change the state of the semaphore.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   sem_id           #uint32 Semaphore ID number
**  
**  \returns
**  \retcode #OS_ERR_INVALID_ID if the id passed in is not a binary semaphore \endcode
**  \retcode #OS_SEM_FAILURE the semaphore was not previously  initialized or is not
**           in the array of semaphores defined by the system \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_BinSemFlush            (uint32 sem_id);

/************************************************************************/
/** \brief Give binary semaphore
**
**  \par Description
**       The function  unlocks the semaphore referenced by sem_id by performing
**       a semaphore unlock operation on that semaphore.If the semaphore value 
**       resulting from this operation is positive, then no threads were blocked             
**       waiting for the semaphore to become unlocked; the semaphore value is
**       simply incremented for this semaphore.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   sem_id           #uint32 Semaphore ID number
**  
**  \returns
**  \retcode #OS_ERR_INVALID_ID if the id passed in is not a binary semaphore \endcode
**  \retcode #OS_SEM_FAILURE the semaphore was not previously  initialized or is not
**           in the array of semaphores defined by the system \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_BinSemGive            (uint32 sem_id);

/************************************************************************/
/** \brief Take binary semaphore
**
**  \par Description
**       The locks the semaphore referenced by sem_id by performing a 
**       semaphore lock operation on that semaphore.If the semaphore value 
**       is currently zero, then the calling thread shall not return from 
**       the call until it either locks the semaphore or the call is 
**       interrupted by a signal.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   sem_id           #uint32 Semaphore ID number
**  
**  \returns
**  \retcode #OS_ERR_INVALID_ID if the id passed in is not a binary semaphore \endcode
**  \retcode #OS_SEM_FAILURE the semaphore was not previously  initialized or is not
**           in the array of semaphores defined by the system \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_BinSemTake            (uint32 sem_id);

/************************************************************************/
/** \brief Timed wait binary semaphore
**
**  \par Description
**       The function locks the semaphore referenced by sem_id . However,
**       if the semaphore cannot be locked without waiting for another process
**       or thread to unlock the semaphore , this wait shall be terminated when 
**       the specified timeout ,msecs, expires.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   sem_id           #uint32 Semaphore ID number
**  \param [in]   msecs            #uint32 Milliseconds to wait
**  
**  \returns
**  \retcode #OS_SEM_TIMEOUT if semaphore was not relinquished in time \endcode
**  \retcode #OS_ERR_INVALID_ID if the id passed in is not a binary semaphore \endcode
**  \retcode #OS_SEM_FAILURE the semaphore was not previously  initialized or is not
**           in the array of semaphores defined by the system \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_BinSemTimedWait       (uint32 sem_id, uint32 msecs);

/************************************************************************/
/** \brief Delete binary semaphore
**
**  \par Description
**       Deletes the specified Binary Semaphore.
**
**  \par Assumptions, External Events, and Notes:
**       Since we can't delete a semaphore which is currently locked by some task 
**       (as it may ber crucial to completing the task), the semaphore must be full to
**       allow deletion.
**
**  \param [in]   sem_id           #uint32 Semaphore ID number
**  
**  \returns
**  \retcode #OS_ERR_INVALID_ID if the id passed in is not a binary semaphore \endcode
**  \retcode #OS_SEM_FAILURE the semaphore was not previously  initialized or is not
**           in the array of semaphores defined by the system \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_BinSemDelete          (uint32 sem_id);

/************************************************************************/
/** \brief Get binary semaphore id by name
**
**  \par Description
**       This function tries to find a binary sem Id given the name of a bin_sem
**       The id is returned through sem_id
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in/out]   sem_id           #uint32 Semaphore ID number
**  \param [in]       sem_name         Semaphore name
**  
**  \returns
**  \retcode #OS_INVALID_POINTER is semid or sem_name are NULL pointers \endcode
**  \retcode #OS_ERR_NAME_TOO_LONG if the name given is to long to have been stored \endcode
**  \retcode #OS_ERR_NAME_NOT_FOUND if the name was not found in the table \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_BinSemGetIdByName     (uint32 *sem_id, const char *sem_name);

/************************************************************************/
/** \brief Get binary semaphore info
**
**  \par Description
**       This function will pass back a pointer to structure that contains 
**       all of the relevant info( name and creator) about the specified binary
**       semaphore.
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       sem_id           #uint32 Semaphore ID number
**  \param [in/out]   bin_prop         #OS_bin_sem_prop_t Semphore properties struct pointer
**  
**  \returns
**  \retcode #OS_ERR_INVALID_ID if the id passed in is not a binary semaphore \endcode
**  \retcode #OS_INVALID_POINTER if the bin_prop pointer is null \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_BinSemGetInfo         (uint32 sem_id, OS_bin_sem_prop_t *bin_prop);

/************************************************************************/
/** \brief Create counting semaphore
**
**  \par Description
**       Creates a counting semaphore with initial value specified by
**       sem_initial_value and name specified by sem_name. sem_id will be 
**       returned to the caller
**
**  \par Assumptions, External Events, and Notes:
**       options is an unused parameter 
**
**  \param [in/out]   sem_id           #uint32 Semaphore ID number
**  \param [in]   sem_name             Semaphore name
**  \param [in]   sem_initial_value    #uint32 Semaphore initial value
**  \param [in]   options               #uint32 Semaphore size
**  
**  \returns
**  \retcode #OS_INVALID_POINTER if the pointers passed in are NULL \endcode
**  \retcode #OS_ERR_NAME_TOO_LONG if th ename to found is too long to begin with \endcode
**  \retcode #OS_ERR_NO_FREE_IDS if there are already the max queues created \endcode
**  \retcode #OS_ERR_NAME_TAKEN if the name is already being used on another queue \endcode
**  \retcode #OS_SEM_FAILURE if the OS call failed \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_CountSemCreate          (uint32 *sem_id, const char *sem_name, 
                                uint32 sem_initial_value, uint32 options);

/************************************************************************/
/** \brief Give counting semaphore
**
**  \par Description
**       The function  unlocks the semaphore referenced by sem_id by performing
**       a semaphore unlock operation on that semaphore.If the semaphore value 
**       resulting from this operation is positive, then no threads were blocked             
**       waiting for the semaphore to become unlocked; the semaphore value is
**       simply incremented for this semaphore.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   sem_id           #uint32 Semaphore ID number
**  
**  \returns
**  \retcode #OS_ERR_INVALID_ID if the id passed in is not a counting semaphore \endcode
**  \retcode #OS_SEM_FAILURE the semaphore was not previously  initialized or is not
**           in the array of semaphores defined by the system \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_CountSemGive            (uint32 sem_id);

/************************************************************************/
/** \brief Take counting semaphore
**
**  \par Description
**       The locks the semaphore referenced by sem_id by performing a 
**       semaphore lock operation on that semaphore.If the semaphore value 
**       is currently zero, then the calling thread shall not return from 
**       the call until it either locks the semaphore or the call is 
**       interrupted by a signal.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   sem_id           #uint32 Semaphore ID number
**  
**  \returns
**  \retcode #OS_ERR_INVALID_ID if the id passed in is not a counting semaphore \endcode
**  \retcode #OS_SEM_FAILURE the semaphore was not previously  initialized or is not
**           in the array of semaphores defined by the system \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_CountSemTake            (uint32 sem_id);

/************************************************************************/
/** \brief Timed wait counting semaphore
**
**  \par Description
**       The function locks the semaphore referenced by sem_id . However,
**       if the semaphore cannot be locked without waiting for another process
**       or thread to unlock the semaphore , this wait shall be terminated when 
**       the specified timeout ,msecs, expires.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   sem_id           #uint32 Semaphore ID number
**  \param [in]   msecs            #uint32 Milliseconds to wait
**  
**  \returns
**  \retcode #OS_SEM_TIMEOUT if semaphore was not relinquished in time \endcode
**  \retcode #OS_ERR_INVALID_ID if the id passed in is not a counting semaphore \endcode
**  \retcode #OS_SEM_FAILURE the semaphore was not previously  initialized or is not
**           in the array of semaphores defined by the system \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_CountSemTimedWait       (uint32 sem_id, uint32 msecs);

/************************************************************************/
/** \brief Delete counting semaphore
**
**  \par Description
**       Deletes the specified counting Semaphore.
**
**  \par Assumptions, External Events, and Notes:
**       Since we can't delete a semaphore which is currently locked by some task 
**       (as it may ber crucial to completing the task), the semaphore must be full to
**       allow deletion.
**
**  \param [in]   sem_id           #uint32 Semaphore ID number
**  
**  \returns
**  \retcode #OS_ERR_INVALID_ID if the id passed in is not a counting semaphore \endcode
**  \retcode #OS_SEM_FAILURE the semaphore was not previously  initialized or is not
**           in the array of semaphores defined by the system \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_CountSemDelete          (uint32 sem_id);

/************************************************************************/
/** \brief Get counting semaphore id by name
**
**  \par Description
**       This function tries to find a counting sem Id given the name of a count_sem
**       The id is returned through sem_id
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in/out]   sem_id           #uint32 Semaphore ID number
**  \param [in]       sem_name         Semaphore name
**  
**  \returns
**  \retcode #OS_INVALID_POINTER is semid or sem_name are NULL pointers \endcode
**  \retcode #OS_ERR_NAME_TOO_LONG if the name given is to long to have been stored \endcode
**  \retcode #OS_ERR_NAME_NOT_FOUND if the name was not found in the table \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_CountSemGetIdByName     (uint32 *sem_id, const char *sem_name);

/************************************************************************/
/** \brief Get counting semaphore info
**
**  \par Description
**       This function will pass back a pointer to structure that contains 
**       all of the relevant info( name and creator) about the specified counting
**       semaphore.
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       sem_id           #uint32 Semaphore ID number
**  \param [in/out]   count_prop         #OS_count_sem_prop_t Semphore properties struct pointer
**  
**  \returns
**  \retcode #OS_ERR_INVALID_ID if the id passed in is not a counting semaphore \endcode
**  \retcode #OS_INVALID_POINTER if the count_prop pointer is null \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_CountSemGetInfo         (uint32 sem_id, OS_count_sem_prop_t *count_prop);


/*
** Mutex API
*/

/************************************************************************/
/** \brief Create mutex semaphore
**
**  \par Description
**       Creates a mutex semaphore initially full.
**
**  \par Assumptions, External Events, and Notes:
**       options is an unused parameter 
**
**  \param [in/out]   sem_id           #uint32 Semaphore ID number
**  \param [in]   sem_name             Semaphore name
**  \param [in]   sem_initial_value    #uint32 Semaphore initial value
**  \param [in]   options               #uint32 Semaphore size
**  
**  \returns
**  \retcode #OS_INVALID_POINTER if the pointers passed in are NULL \endcode
**  \retcode #OS_ERR_NAME_TOO_LONG if th ename to found is too long to begin with \endcode
**  \retcode #OS_ERR_NO_FREE_IDS if there are already the max queues created \endcode
**  \retcode #OS_ERR_NAME_TAKEN if the name is already being used on another queue \endcode
**  \retcode #OS_SEM_FAILURE if the OS call failed \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_MutSemCreate           (uint32 *sem_id, const char *sem_name, uint32 options);

/************************************************************************/
/** \brief Give mutex semaphore
**
**  \par Description
**       The function releases the mutex object referenced by sem_id.The 
**       manner in which a mutex is released is dependent upon the mutex's type 
**       attribute.  If there are threads blocked on the mutex object referenced by 
**       mutex when this function is called, resulting in the mutex becoming 
**       available, the scheduling policy shall determine which thread shall 
**       acquire the mutex.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   sem_id           #uint32 Semaphore ID number
**  
**  \returns
**  \retcode #OS_ERR_INVALID_ID if the id passed in is not a mutex semaphore \endcode
**  \retcode #OS_SEM_FAILURE the semaphore was not previously  initialized or is not
**           in the array of semaphores defined by the system \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_MutSemGive             (uint32 sem_id);

/************************************************************************/
/** \brief Take mutex semaphore
**
**  \par Description
**       The mutex object referenced by sem_id shall be locked by calling this
**       function. If the mutex is already locked, the calling thread shall
**       block until the mutex becomes available. This operation shall return
**       with the mutex object referenced by mutex in the locked state with the              
**       calling thread as its owner.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   sem_id           #uint32 Semaphore ID number
**  
**  \returns
**  \retcode #OS_ERR_INVALID_ID if the id passed in is not a mutex semaphore \endcode
**  \retcode #OS_SEM_FAILURE the semaphore was not previously  initialized or is not
**           in the array of semaphores defined by the system \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_MutSemTake             (uint32 sem_id);

/************************************************************************/
/** \brief Delete mutex semaphore
**
**  \par Description
**       Deletes the specified mutex Semaphore.
**
**  \par Assumptions, External Events, and Notes:
**       The mutex must be full to take it, so we have to check for fullness
**
**  \param [in]   sem_id           #uint32 Semaphore ID number
**  
**  \returns
**  \retcode #OS_ERR_INVALID_ID if the id passed in is not a mutex semaphore \endcode
**  \retcode #OS_SEM_FAILURE the semaphore was not previously  initialized or is not
**           in the array of semaphores defined by the system \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_MutSemDelete           (uint32 sem_id);  

/************************************************************************/
/** \brief Get mutex semaphore id by name
**
**  \par Description
**       This function tries to find a mutex sem Id given the name of a mut_sem
**       The id is returned through sem_id
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in/out]   sem_id           #uint32 Semaphore ID number
**  \param [in]       sem_name         Semaphore name
**  
**  \returns
**  \retcode #OS_INVALID_POINTER is semid or sem_name are NULL pointers \endcode
**  \retcode #OS_ERR_NAME_TOO_LONG if the name given is to long to have been stored \endcode
**  \retcode #OS_ERR_NAME_NOT_FOUND if the name was not found in the table \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_MutSemGetIdByName      (uint32 *sem_id, const char *sem_name); 

/************************************************************************/
/** \brief Get mutex semaphore info
**
**  \par Description
**       This function will pass back a pointer to structure that contains 
**       all of the relevant info( name and creator) about the specified mutex
**       semaphore.
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       sem_id           #uint32 Semaphore ID number
**  \param [in/out]   mut_prop         #OS_mut_sem_prop_t Semphore properties struct pointer
**  
**  \returns
**  \retcode #OS_ERR_INVALID_ID if the id passed in is not a mutex semaphore \endcode
**  \retcode #OS_INVALID_POINTER if the mut_prop pointer is null \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_MutSemGetInfo          (uint32 sem_id, OS_mut_sem_prop_t *mut_prop);

/*
** OS Time/Tick related API
*/

/************************************************************************/
/** \brief Milliseconds to ticks
**
**  \par Description
**       This function accepts a time interval in milliseconds, as an input and 
**       returns the tick equivalent  for this time period. The tick value is 
**       rounded up.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   milli_seconds           #uint32 the time interval, in milli second, to be translated to ticks
**  
**  \returns
**  #itn32 the number of ticks rounded up.
**  \endreturns
**
*************************************************************************/
int32 OS_Milli2Ticks           (uint32 milli_seconds);

/************************************************************************/
/** \brief Ticks to milliseconds
**
**  \par Description
**       This function returns the duration of a system tick in micro seconds.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**  #itn32 duration of a system tick in microseconds
**  \endreturns
**
*************************************************************************/
int32 OS_Tick2Micros           (void);

/************************************************************************/
/** \brief Get local time
**
**  \par Description
**       This functions get the local time of the machine its on
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in/out]   time_struct         #OS_time_t struct pointer
**
**  \returns
**  \retcode #OS_INVALID_POINTER if any of the necessary pointers are NULL \endcode
**  \retcode #OS_ERROR  \retdesc \copydoc OS_ERROR \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32  OS_GetLocalTime         (OS_time_t *time_struct);

/************************************************************************/
/** \brief Set local time
**
**  \par Description
**       This functions sets the local time of the machine its on
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**  \retcode #OS_INVALID_POINTER if any of the necessary pointers are NULL \endcode
**  \retcode #OS_ERR_NOT_IMPLEMENTED \endcode
**  \retcode #OS_ERROR  \retdesc \copydoc OS_ERROR \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32  OS_SetLocalTime         (OS_time_t *time_struct);  

/*
** Exception API
*/
// TODO: not implemented?
int32 OS_ExcAttachHandler      (uint32 ExceptionNumber, 
                                void (*ExceptionHandler)(uint32, const void *,uint32),
                                int32 parameter);


int32 OS_ExcEnable             (int32 ExceptionNumber);


int32 OS_ExcDisable            (int32 ExceptionNumber);

/*
** Floating Point Unit API
*/

// TODO: not implemented?
int32 OS_FPUExcAttachHandler   (uint32 ExceptionNumber, void * ExceptionHandler ,
                                 int32 parameter);


int32 OS_FPUExcEnable          (int32 ExceptionNumber);


int32 OS_FPUExcDisable         (int32 ExceptionNumber);


int32 OS_FPUExcSetMask         (uint32 mask);


int32 OS_FPUExcGetMask         (uint32 *mask);

/*
** Interrupt API
*/
// TODO: not implemented?
int32 OS_IntAttachHandler  (uint32 InterruptNumber, osal_task_entry InterruptHandler, int32 parameter);

int32 OS_IntUnlock         (int32 IntLevel);

int32 OS_IntLock           (void);

int32 OS_IntEnable         (int32 Level);

int32 OS_IntDisable        (int32 Level);


int32 OS_IntSetMask        (uint32 mask);


int32 OS_IntGetMask        (uint32 *mask);


int32 OS_IntAck             (int32 InterruptNumber);



/*
** Shared memory API 
*/

// TODO: not implemented?
int32 OS_ShMemInit          (void);


int32 OS_ShMemCreate        (uint32 *Id, uint32 NBytes, const char* SegName);


int32 OS_ShMemSemTake       (uint32 Id);


int32 OS_ShMemSemGive       (uint32 Id);


int32 OS_ShMemAttach        (cpuaddr * Address, uint32 Id);


int32 OS_ShMemGetIdByName   (uint32 *ShMemId, const char *SegName );

/*
** Heap API
*/
// TODO: not implemented?

int32 OS_HeapGetInfo       (OS_heap_prop_t *heap_prop);

/*
** API for useful debugging function
*/
/************************************************************************/
/** \brief Get error name
**
**  \par Description
**       This functions gets the string representation of an error code
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]       error_num           #int32 Error number
**  \param [in/out]   err_name            #os_err_name_t Error name
**
**  \returns
**  \retcode #OS_INVALID_POINTER if any of the necessary pointers are NULL \endcode
**  \retcode #OS_SUCCESS  \retdesc \copydoc OS_SUCCESS \endcode
**  \endreturns
**
*************************************************************************/
int32 OS_GetErrorName      (int32 error_num, os_err_name_t* err_name);


/* 
** Abstraction for printf statements 
*/

/************************************************************************/
/** \brief OS printf
**
**  \par Description
**       This function abstracts out the printf type statements. This is 
**       useful for using OS- specific thats that will allow non-polled
**       print statements for the real time systems.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void OS_printf( const char *string, ...) OS_PRINTF(1,2);

/************************************************************************/
/** \brief Disable OS printf
**
**  \par Description
**       This function disables the output to the UART from OS_printf.  
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void OS_printf_disable(void);

/************************************************************************/
/** \brief Enable OS printf
**
**  \par Description
**       This function enables the output to the UART through OS_printf.   
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void OS_printf_enable(void);

/************************************************************************/
/** \brief Exit running application
**
**  \par Description
**       Indicates that the OSAL application should exit and return control to the OS
**       This is intended for e.g. scripted unit testing where the test needs to end
**       without user intervention.  This function does not return
**
**  \par Assumptions, External Events, and Notes:
**       This exits the entire process including tasks that have been created.
**       It does not return.  Production embedded code typically would not ever call this.
**
*************************************************************************/
void OS_ApplicationExit(int32 Status);

#ifdef __cplusplus
}
#endif

#endif
