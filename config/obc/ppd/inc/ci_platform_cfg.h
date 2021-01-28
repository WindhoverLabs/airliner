#ifndef CI_PLATFORM_CFG_H
#define CI_PLATFORM_CFG_H

#ifdef __cplusplus
extern "C" {
#endif


/*
** ci Platform Configuration Parameter Definitions
*/

/** \brief Mission specific version number for CI application
**  
**  \par Description:
**       An application version number consists of four parts:
**       major version number, minor version number, revision
**       number and mission specific revision number. The mission
**       specific revision number is defined here and the other
**       parts are defined in "ci_version.h".
**
**  \par Limits:
**       Must be defined as a numeric value that is greater than
**       or equal to zero.
*/
#define CI_MISSION_REV                (1)

/** \brief Pipe depth for the Scheduler pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define CI_SCH_PIPE_DEPTH             (2)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define CI_SCH_PIPE_NAME              ("CI_SCH_PIPE")

/** \brief The SB pend behavior type for the Scheduler pipe.
**
**  \par Limits:
**       One of: CFE_SB_POLL, CFE_SB_PEND_FOREVER, or the 
**       number of milliseconds to wait for a new message (recommended).
**       Note, using CFE_SB_PEND_FOREVER may cause an unresponsive
**       application if no messages arrive on this pipe.
*/
#define CI_SCH_PIPE_PEND_TIME         (2000)

/** \brief The number of WAKEUP messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max limited to CFE_SB_MAX_PIPE_DEPTH-1.  Note the
**       CI_SCH_PIPE_WAKEUP_RESERVED and CI_SCH_PIPE_SEND_HK_RESERVED
**       must be less than CI_SCH_PIPE_DEPTH.
*/
#define CI_SCH_PIPE_WAKEUP_RESERVED   (1)

/** \brief The number of SEND_HK messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.  Note the
**       CI_SCH_PIPE_WAKEUP_RESERVED and CI_SCH_PIPE_SEND_HK_RESERVED
**       must be less than CI_SCH_PIPE_DEPTH.
*/
#define CI_SCH_PIPE_SEND_HK_RESERVED  (1)

/** \brief The number of PROCESS_TIMEOUT messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define CI_SCH_PIPE_PROC_TIMEOUT_RESERVED  (1)

/** \brief Pipe depth for the command pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define CI_CMD_PIPE_DEPTH             (4)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define CI_CMD_PIPE_NAME              ("CI_CMD_PIPE")

/** \brief The config table default filename
**
**  \par Limits:
**       The length of each string, including the NULL terminator cannot exceed
**       the #OS_MAX_PATH_LEN value.
*/
#define CI_CONFIG_TABLE_FILENAME      ("/cf/apps/ci_config.tbl")

/** \brief The timeout table default filename
**
**  \par Limits:
**       The length of each string, including the NULL terminator cannot exceed
**       the #OS_MAX_PATH_LEN value.
*/
#define CI_TIMEOUT_TABLE_FILENAME      ("/cf/apps/ci_timeout.tbl")

/** \brief The timeout value, in milliseconds, to wait for ES application startup sync.
**
**  \par Limits:
**       This parameter must be at least 1000 (ms).
*/
#define CI_STARTUP_TIMEOUT_MSEC        (5000)

/** \brief CI Ingest Mode */
#define CI_INGEST_MODE                 (CI_BHV_OPTIMISTIC)

/** \brief CI Listener Task Delay
**
**  \note The main ingest loop is a while loop and sleeps for this many  
**        milliseconds each iteration.
 */
#define CI_LISTENER_TASK_DELAY         (100)

/** \brief CI Lister Task Stack Size */
#define CI_LISTENER_TASK_STACK_SIZE         (160000)

/** \brief CI Lister Task Flags for CFE_ES_CreateChildTask() */
#define CI_LISTENER_TASK_FLAGS              (OS_ENABLE_CORE_0)

/** \brief CI Lister Task Priority */
#define CI_LISTENER_TASK_PRIORITY           (109)

/** \brief The largest size message CI can ingest */
#define CI_MAX_CMD_INGEST                   (CFE_SB_MAX_SB_MSG_SIZE)

/** \brief Listener task name */
#define CI_LISTENER_TASK_NAME               ("CI_LISTENER")

/** \brief CI config table mutex name */
#define CI_CFG_TBL_MUTEX_NAME               ("CI_CFG_TBL_MUTEX")

/** \brief CI timeout table mutex name */
#define CI_TIME_TBL_MUTEX_NAME              ("CI_TIME_TBL_MUTEX")

#ifdef __cplusplus
}
#endif

#endif /* CI_PLATFORM_CFG_H */

/************************/
/*  End of File Comment */
/************************/
    
