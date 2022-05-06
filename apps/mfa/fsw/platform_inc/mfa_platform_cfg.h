#ifndef MFA_PLATFORM_CFG_H
#define MFA_PLATFORM_CFG_H

#ifdef __cplusplus
extern "C" {
#endif


/*
** MFA Platform Configuration Parameter Definitions
*/

/** \brief Mission speMFAfic version number for MFA application
**  
**  \par Description:
**       An application version number consists of four parts:
**       major version number, minor version number, revision
**       number and mission speMFAfic revision number. The mission
**       speMFAfic revision number is defined here and the other
**       parts are defined in "MFA_version.h".
**
**  \par Limits:
**       Must be defined as a numeric value that is greater than
**       or equal to zero.
*/
#define MFA_MISSION_REV                (0)

/** \brief Pipe depth for the Scheduler pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define MFA_SCH_PIPE_DEPTH             (2)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define MFA_SCH_PIPE_NAME              ("MFA_SCH_PIPE")

/** \brief The SB pend behavior type for the Scheduler pipe.
**
**  \par Limits:
**       One of: CFE_SB_POLL, CFE_SB_PEND_FOREVER, or the 
**       number of milliseconds to wait for a new message (recommended).
**       Note, using CFE_SB_PEND_FOREVER may cause an unresponsive
**       application if no messages arrive on this pipe.
*/
#define MFA_SCH_PIPE_PEND_TIME         (2000)

/** \brief The number of WAKEUP messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max limited to CFE_SB_MAX_PIPE_DEPTH-1.  Note the
**       MFA_SCH_PIPE_WAKEUP_RESERVED and MFA_SCH_PIPE_SEND_HK_RESERVED
**       must be less than MFA_SCH_PIPE_DEPTH.
*/
#define MFA_SCH_PIPE_WAKEUP_RESERVED   (1)

/** \brief The number of SEND_HK messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.  Note the
**       MFA_SCH_PIPE_WAKEUP_RESERVED and MFA_SCH_PIPE_SEND_HK_RESERVED
**       must be less than MFA_SCH_PIPE_DEPTH.
*/
#define MFA_SCH_PIPE_SEND_HK_RESERVED  (1)

/** \brief The number of PROCESS_TIMEOUT messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define MFA_SCH_PIPE_PROC_TIMEOUT_RESERVED  (1)

/** \brief Pipe depth for the command pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define MFA_CMD_PIPE_DEPTH             (4)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define MFA_CMD_PIPE_NAME              ("MFA_CMD_PIPE")

/** \brief The config table default filename
**
**  \par Limits:
**       The length of each string, including the NULL terminator cannot exceed
**       the #OS_MAX_PATH_LEN value.
*/
#define MFA_CONFIG_TABLE_FILENAME      ("/cf/apps/MFA_config.tbl")

/** \brief The timeout table default filename
**
**  \par Limits:
**       The length of each string, including the NULL terminator cannot exceed
**       the #OS_MAX_PATH_LEN value.
*/
#define MFA_TIMEOUT_TABLE_FILENAME      ("/cf/apps/MFA_timeout.tbl")

/** \brief The timeout value, in milliseconds, to wait for ES application startup sync.
**
**  \par Limits:
**       This parameter must be at least 1000 (ms).
*/
#define MFA_STARTUP_TIMEOUT_MSEC        (5000)

/** \brief MFA Ingest Mode */
#define MFA_INGEST_MODE                 (MFA_BHV_OPTIMISTIC)

/** \brief The largest size message MFA can ingest */
#define MFA_MAX_CMD_INGEST                   (CFE_SB_MAX_SB_MSG_SIZE)

/** \brief MFA config table mutex name */
#define MFA_CFG_TBL_MUTEX_NAME               ("MFA_CFG_TBL_MUTEX")

/** \brief MFA timeout table mutex name */
#define MFA_TIME_TBL_MUTEX_NAME              ("MFA_TIME_TBL_MUTEX")

#ifdef __cplusplus
}
#endif

#endif /* MFA_PLATFORM_CFG_H */

/************************/
/*  End of File Comment */
/************************/
    
