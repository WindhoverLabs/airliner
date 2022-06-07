    
#ifndef ZUSUP_PLATFORM_CFG_H
#define ZUSUP_PLATFORM_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** zusup Platform Configuration Parameter Definitions
*/

/** \brief Mission specific version number for ZUSUP application
**  
**  \par Description:
**       An application version number consists of four parts:
**       major version number, minor version number, revision
**       number and mission specific revision number. The mission
**       specific revision number is defined here and the other
**       parts are defined in "zusup_version.h".
**
**  \par Limits:
**       Must be defined as a numeric value that is greater than
**       or equal to zero.
*/
#define ZUSUP_MISSION_REV                (0)

/** \brief Pipe depth for the Scheduler pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define ZUSUP_SCH_PIPE_DEPTH             (2)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define ZUSUP_SCH_PIPE_NAME              ("ZUSUP_SCH_PIPE")

/** \brief The SB pend behavior type for the Scheduler pipe.
**
**  \par Limits:
**       One of: CFE_SB_POLL, CFE_SB_PEND_FOREVER, or the 
**       number of milliseconds to wait for a new message (recommended).
**       Note, using CFE_SB_PEND_FOREVER may cause an unresponsive
**       application if no messages arrive on this pipe.
*/
#define ZUSUP_SCH_PIPE_PEND_TIME         (2000)

/** \brief The number of WAKEUP messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max limited to CFE_SB_MAX_PIPE_DEPTH-1.  Note the
**       ZUSUP_SCH_PIPE_WAKEUP_RESERVED and ZUSUP_SCH_PIPE_SEND_HK_RESERVED
**       must be less than ZUSUP_SCH_PIPE_DEPTH.
*/
#define ZUSUP_WAKEUP_MID_MAX_MSG_COUNT   (1)

/** \brief The number of SEND_HK messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.  Note the
**       ZUSUP_SCH_PIPE_WAKEUP_RESERVED and ZUSUP_SCH_PIPE_SEND_HK_RESERVED
**       must be less than ZUSUP_SCH_PIPE_DEPTH.
*/
#define ZUSUP_SEND_HK_MID_MAX_MSG_COUNT  (1)

/** \brief Pipe depth for the command pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define ZUSUP_CMD_PIPE_DEPTH             (4)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define ZUSUP_CMD_PIPE_NAME              ("ZUSUP_CMD_PIPE")

/** \brief The config table default filename
**
**  \par Limits:
**       The length of each string, including the NULL terminator cannot exceed
**       the #OS_MAX_PATH_LEN value.
*/
#define ZUSUP_CONFIG_TABLE_FILENAME      ("/ram/apps/zusup_config.tbl")
/** \brief The timeout value, in milliseconds, to wait for ES application startup sync.
**
**  \par Limits:
**       This parameter must be at least 1000 (ms).
*/
#define ZUSUP_STARTUP_TIMEOUT_MSEC    (1000)


#ifdef __cplusplus
}
#endif

#endif /* ZUSUP_PLATFORM_CFG_H */

/************************/
/*  End of File Comment */
/************************/
    
