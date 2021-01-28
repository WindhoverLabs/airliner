#ifndef MAVLINK_PLATFORM_CFG_H
#define MAVLINK_PLATFORM_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** ci Platform Configuration Parameter Definitions
*/

/** \brief Mission specific version number for MAVLINK application
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
#define MAVLINK_MISSION_REV                (0)

/** \brief Pipe depth for the Scheduler pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define MAVLINK_SCH_PIPE_DEPTH             (2)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define MAVLINK_SCH_PIPE_NAME              ("MAVLINK_SCH_PIPE")

/** \brief The SB pend behavior type for the Scheduler pipe.
**
**  \par Limits:
**       One of: CFE_SB_POLL, CFE_SB_PEND_FOREVER, or the 
**       number of milliseconds to wait for a new message (recommended).
**       Note, using CFE_SB_PEND_FOREVER may cause an unresponsive
**       application if no messages arrive on this pipe.
*/
#define MAVLINK_SCH_PIPE_PEND_TIME         (2000)

/** \brief The number of WAKEUP messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max limited to CFE_SB_MAX_PIPE_DEPTH-1.  Note the
**       MAVLINK_SCH_PIPE_WAKEUP_RESERVED and MAVLINK_SCH_PIPE_SEND_HK_RESERVED
**       must be less than MAVLINK_SCH_PIPE_DEPTH.
*/
#define MAVLINK_SCH_PIPE_WAKEUP_RESERVED   (1)

/** \brief The number of SEND_HK messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.  Note the
**       MAVLINK_SCH_PIPE_WAKEUP_RESERVED and MAVLINK_SCH_PIPE_SEND_HK_RESERVED
**       must be less than MAVLINK_SCH_PIPE_DEPTH.
*/
#define MAVLINK_SCH_PIPE_SEND_HK_RESERVED  (1)

/** \brief The number of PROCESS_TIMEOUT messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define MAVLINK_SCH_PIPE_PROC_TIMEOUT_RESERVED  (1)

/** \brief Pipe depth for the command pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define MAVLINK_CMD_PIPE_DEPTH             (CFE_SB_MAX_PIPE_DEPTH)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define MAVLINK_CMD_PIPE_NAME              ("MAVLINK_CMD_PIPE")

/** \brief Pipe depth for the data pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define MAVLINK_DATA_PIPE_DEPTH            (4)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define MAVLINK_DATA_PIPE_NAME             ("MAVLINK_DATA_PIPE")

/** \brief The config table default filename
**
**  \par Limits:
**       The length of each string, including the NULL terminator cannot exceed
**       the #OS_MAX_PATH_LEN value.
*/
#define MAVLINK_ACTION_MAP_TABLE_FILENAME      ("/cf/apps/mavlink_action.tbl")

/** \brief The timeout table default filename
**
**  \par Limits:
**       The length of each string, including the NULL terminator cannot exceed
**       the #OS_MAX_PATH_LEN value.
*/
#define MAVLINK_TIMEOUT_TABLE_FILENAME      ("/cf/apps/mavlink_timeout.tbl")

/** \brief The timeout value, in milliseconds, to wait for ES application startup sync.
**
**  \par Limits:
**       This parameter must be at least 1000 (ms).
*/
#define MAVLINK_STARTUP_TIMEOUT_MSEC    (5000)


#define MAVLINK_MSG_PARAM_NAME_LEN      (64)

#define MAVLINK_SYSTEM_ID         (1)
#define MAVLINK_COMPONENT_ID      (1)
#define MAVLINK_GCS_IP                  ("10.10.0.13")
#define MAVLINK_GCS_PORT                (14550)
#define MAVLINK_PASSTHRU_IP_SRC         ("127.0.0.1")
#define MAVLINK_PASSTHRU_INGEST_PORT    (14550)

#define MAVLINK_ACTION_MAP_ENTRIES         (10)
#define MAVLINK_HEARTBEAT_WAIT_CYCLES      (5)


#ifdef __cplusplus
}
#endif

#endif /* MAVLINK_PLATFORM_CFG_H */

/************************/
/*  End of File Comment */
/************************/
    
