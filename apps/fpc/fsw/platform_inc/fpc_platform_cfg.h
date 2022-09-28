#ifndef FPC_PLATFORM_CFG_H
#define FPC_PLATFORM_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** FPC Platform Configuration Parameter Definitions
*/

/** \brief Mission specific version number for FPC application
**  
**  \par Description:
**       An application version number consists of four parts:
**       major version number, minor version number, revision
**       number and mission specific revision number. The mission
**       specific revision number is defined here and the other
**       parts are defined in "amc_version.h".
**
**  \par Limits:
**       Must be defined as a numeric value that is greater than
**       or equal to zero.
*/
#define FPC_MISSION_REV                (0)

/** \brief Pipe depth for the Scheduler pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define FPC_SCH_PIPE_DEPTH             (15)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define FPC_SCH_PIPE_NAME              ("FPC_SCH_PIPE")

/** \brief The SB pend behavior type for the Scheduler pipe.
**
**  \par Limits:
**       One of: CFE_SB_POLL, CFE_SB_PEND_FOREVER, or the 
**       number of milliseconds to wait for a new message (recommended).
**       Note, using CFE_SB_PEND_FOREVER may cause an unresponsive
**       application if no messages arrive on this pipe.
*/
#define FPC_SCH_PIPE_PEND_TIME         (2000)

/** \brief The number of WAKEUP messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max limited to CFE_SB_MAX_PIPE_DEPTH-1.  Note the
**       FPC_WAKEUP_MID_MAX_MSG_COUNT and FPC_SEND_HK_MID_MAX_MSG_COUNT
**       must be less than FPC_SCH_PIPE_DEPTH.
*/
#define FPC_WAKEUP_MID_MAX_MSG_COUNT   (1)

/** \brief The number of SEND_HK messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.  Note the
**       FPC_WAKEUP_MID_MAX_MSG_COUNT and FPC_SEND_HK_MID_MAX_MSG_COUNT
**       must be less than FPC_SCH_PIPE_DEPTH.
*/
#define FPC_SEND_HK_MID_MAX_MSG_COUNT  (1)

/** \brief Pipe depth for the command pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define FPC_CMD_PIPE_DEPTH             (4)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define FPC_CMD_PIPE_NAME              ("FPC_CMD_PIPE")

/** \brief The config table default filename
**
**  \par Limits:
**       The length of each string, including the NULL terminator cannot exceed
**       the #OS_MAX_PATH_LEN value.
*/
#define FPC_CONFIG_TABLE_FILENAME      ("/ram/apps/fpc_config.tbl")

/** \brief The timeout value, in milliseconds, to wait for ES application startup sync.
**
**  \par Limits:
**       This parameter must be at least 1000 (ms).
*/
#define FPC_STARTUP_TIMEOUT_MSEC    (1000)

/** \brief Time for direction change hysteresis */
#define DIRECTION_CHANGE_TRIGGER_TIME_US    (100000)

#define FPC_SCH_PIPE_WAKEUP_RESERVED  (1)

#define FPC_PIPE_SEND_HK_RESERVED     (1)

#define FPC_SCH_PIPE_SEND_HK_RESERVED  (1)
#define FPC_CMD_MID_RESERVED  (1)

#define FPC_DATA_PIPE_DEPTH            (11)

#define FPC_DATA_PIPE_NAME      ("FPC_DATA_PIPE_NAME")

#define FPC_NUM_MSG                    (11)

#define FPC_1SECOND_MICRO              (1e6)

#ifdef __cplusplus
}
#endif

#endif /* FPC_PLATFORM_CFG_H */

/************************/
/*  End of File Comment */
/************************/
    
