#ifndef OSCONFIG_H
#define OSCONFIG_H

#define OS_RUNTIME_MANAGER_CMD_QUEUE_NAME 		"OS_RUNTIME_MGR_CMD"
#define OS_RUNTIME_MANAGER_CMD_QUEUE_DEPTH		1
#define OS_RUNTIME_MANAGER_CMD_TASK_NAME		"OS_RUNTIME_MGR_CMD"
#define OS_RUNTIME_MANAGER_CMD_TASK_STACK_SIZE  32535
#define OS_RUNTIME_MANAGER_CMD_TASK_PRIORITY	0
#define OS_RUNTIME_MANAGER_EXEC_TASK_NAME		"OS_RUNTIME_MGR_EXEC"
#define OS_RUNTIME_MANAGER_EXEC_TASK_STACK_SIZE  32535
#define OS_RUNTIME_MANAGER_EXEC_TASK_PRIORITY	99
#define OS_RUNTIME_MANAGER_SEM_NAME				"OS_RUNTIME_MGR_SEM"

/*#define OS_USE_EMBEDDED_PRINTF*/

#define OS_MAX_QUEUE_WIDTH          8
#define OS_MAX_QUEUE_DEPTH          256

/*
** Platform Configuration Parameters for the OS API
*/

#define OS_MAX_TASKS                100
#define OS_MAX_QUEUES               255
#define OS_MAX_COUNT_SEMAPHORES     100
#define OS_MAX_BIN_SEMAPHORES       100
#define OS_MAX_MUTEXES              100
#define OS_MAX_TIMEOUTS				(OS_MAX_QUEUES+OS_MAX_COUNT_SEMAPHORES+OS_MAX_BIN_SEMAPHORES)

/*
** Maximum length for an absolute path name
*/
#define OS_MAX_PATH_LEN     64

/*
** Maximum length for a local or host path/filename.
**   This parameter can consist of the OSAL filename/path + 
**   the host OS physical volume name or path.
*/
#define OS_MAX_LOCAL_PATH_LEN (OS_MAX_PATH_LEN + OS_FS_PHYS_NAME_LEN)

/* 
** The maxium length allowed for a object (task,queue....) name 
*/
#define OS_MAX_API_NAME     40

/* 
** The maximum length for a file name 
*/
#define OS_MAX_FILE_NAME    20

/* 
** These defines are for OS_printf
*/
#define OS_BUFFER_SIZE 4000
#define OS_BUFFER_MSG_DEPTH 100

/* This #define turns on a utility task that
 * will read the statements to print from
 * the OS_printf function. If you want OS_printf
 * to print the text out itself, comment this out 
 * 
 * NOTE: The Utility Task #defines only have meaning 
 * on the VxWorks operating systems
 */
 
#define OS_UTILITY_TASK_ON


#ifdef OS_UTILITY_TASK_ON 
    #define OS_UTILITYTASK_STACK_SIZE 2048
    /* some room is left for other lower priority tasks */
    #define OS_UTILITYTASK_PRIORITY   245
#endif


/* 
** the size of a command that can be passed to the underlying OS 
*/
#define OS_MAX_CMD_LEN 1000

/*
** This define will include the OS network API.
** It should be turned off for targtets that do not have a network stack or 
** device ( like the basic RAD750 vxWorks BSP )
*/
#define OS_INCLUDE_NETWORK

/* 
** This is the maximum number of open file descriptors allowed at a time 
*/
#define OS_MAX_NUM_OPEN_FILES 50 

/* 
** This defines the filethe input command of OS_ShellOutputToFile
** is written to in the VxWorks6 port 
*/
#define OS_SHELL_CMD_INPUT_FILE_NAME "/ram/OS_ShellCmd.in"

/* 
** This define sets the queue implentation of the Linux port to use sockets 
** commenting this out makes the Linux port use the POSIX message queues.
*/
#define OSAL_SOCKET_QUEUE

/*
** Module loader/symbol table is optional
*/
#define OS_INCLUDE_MODULE_LOADER

#ifdef OS_INCLUDE_MODULE_LOADER
   /*
   ** This define sets the size of the OS Module Table, which keeps track of the loaded modules in 
   ** the running system. This define must be set high enough to support the maximum number of
   ** loadable modules in the system. If the the table is filled up at runtime, a new module load
   ** would fail.
   */
   #define OS_MAX_MODULES 64

   /*
   ** The Static Loader define is used for switching between the Dynamic and Static loader implementations.
   */
   /* #define OS_STATIC_LOADER */

#endif


/*
** This define sets the maximum symbol name string length. It is used in implementations that 
** support the symbols and symbol lookup.
*/
#define OS_MAX_SYM_LEN 64


/*
** This define sets the maximum number of timers available
*/
#define OS_MAX_TIMERS         5

#endif
