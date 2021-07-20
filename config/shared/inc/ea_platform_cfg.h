#ifndef EA_PLATFORM_CFG_H
#define EA_PLATFORM_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "priorities.h"

#define EA_CHILD_TASK_FLAGS            OS_ENABLE_CORE_0


/*
** ea Platform Configuration Parameter Definitions
*/

/** \brief Mission specific version number for EA application
**  
**  \par Description:
**       An application version number consists of four parts:
**       major version number, minor version number, revision
**       number and mission specific revision number. The mission
**       specific revision number is defined here and the other
**       parts are defined in "ea_version.h".
**
**  \par Limits:
**       Must be defined as a numeric value that is greater than
**       or equal to zero.
*/
#define EA_MISSION_REV                (0)

/** \brief Pipe depth for the Scheduler pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define EA_SCH_PIPE_DEPTH             (2)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define EA_SCH_PIPE_NAME              ("EA_SCH_PIPE")

/** \brief The SB pend behavior type for the Scheduler pipe.
**
**  \par Limits:
**       One of: CFE_SB_POLL, CFE_SB_PEND_FOREVER, or the 
**       number of milliseconds to wait for a new message (recommended).
**       Note, using CFE_SB_PEND_FOREVER may cause an unresponsive
**       application if no messages arrive on this pipe.
*/
#define EA_SCH_PIPE_PEND_TIME         (2000)

/** \brief The number of WAKEUP messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max limited to CFE_SB_MAX_PIPE_DEPTH-1.  Note the
**       EA_SCH_PIPE_WAKEUP_RESERVED and EA_SCH_PIPE_SEND_HK_RESERVED
**       must be less than EA_SCH_PIPE_DEPTH.
*/
#define EA_SCH_PIPE_WAKEUP_RESERVED   (1)

/** \brief The number of SEND_HK messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.  Note the
**       EA_SCH_PIPE_WAKEUP_RESERVED and EA_SCH_PIPE_SEND_HK_RESERVED
**       must be less than EA_SCH_PIPE_DEPTH.
*/
#define EA_SCH_PIPE_SEND_HK_RESERVED  (1)

/** \brief Pipe depth for the command pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define EA_CMD_PIPE_DEPTH             (4)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define EA_CMD_PIPE_NAME              ("EA_CMD_PIPE")

/** \brief Pipe depth for the data pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define EA_DATA_PIPE_DEPTH            (4)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define EA_DATA_PIPE_NAME             ("EA_DATA_PIPE")

/** \brief The config table default filename
**
**  \par Limits:
**       The length of each string, including the NULL terminator cannot exceed
**       the #OS_MAX_PATH_LEN value.
*/
#define EA_CONFIG_TABLE_FILENAME      ("/cf/apps/ea_config.tbl")
/** \brief The timeout value, in milliseconds, to wait for ES application startup sync.
**
**  \par Limits:
**       This parameter must be at least 1000 (ms).
*/
#define EA_STARTUP_TIMEOUT_MSEC    (1000)

/* TODO:  Add Doxygen markup. */
#define EA_CUSTOM_DEV_PATH       "/tmp/ttyS0"

/** \cscfg EA Child Task Priority
 **  
 **  \par  Description:
 **        Priority of child tasks created by EA.  Lower numbers are higher priority,
 **        with 1 being the highest priority in the case of a child task.
 **
 **  \par Limits:
 **       Valid range for a child task is 1 to 255, but the priority cannot be 
 **       higher (lower number) than the EA App priority. 
 **
 */
#define EA_CHILD_TASK_PRIORITY              (187)
#define EA_APP_UTIL_THRESHOLD               (70)
#define EA_APP_NUM_THREADS                  (2)

#define EA_MAX_PATH_LEN                     (96)

#ifdef __cplusplus
}
#endif

#endif /* EA_PLATFORM_CFG_H */

/************************/
/*  End of File Comment */
/************************/
    
