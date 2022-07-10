    
#ifndef ASPD4525_PLATFORM_CFG_H
#define ASPD4525_PLATFORM_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** aspd4525 Platform Configuration Parameter Definitions
*/

/** \brief Mission specific version number for ASPD4525 application
**  
**  \par Description:
**       An application version number consists of four parts:
**       major version number, minor version number, revision
**       number and mission specific revision number. The mission
**       specific revision number is defined here and the other
**       parts are defined in "aspd4525_version.h".
**
**  \par Limits:
**       Must be defined as a numeric value that is greater than
**       or equal to zero.
*/
#define ASPD4525_MISSION_REV                (0)

/** \brief Pipe depth for the Scheduler pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define ASPD4525_SCH_PIPE_DEPTH             (2)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define ASPD4525_SCH_PIPE_NAME              ("ASPD4525_SCH_PIPE")

/** \brief The SB pend behavior type for the Scheduler pipe.
**
**  \par Limits:
**       One of: CFE_SB_POLL, CFE_SB_PEND_FOREVER, or the 
**       number of milliseconds to wait for a new message (recommended).
**       Note, using CFE_SB_PEND_FOREVER may cause an unresponsive
**       application if no messages arrive on this pipe.
*/
#define ASPD4525_SCH_PIPE_PEND_TIME         (2000)

/** \brief The number of WAKEUP messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max limited to CFE_SB_MAX_PIPE_DEPTH-1.  Note the
**       ASPD4525_SCH_PIPE_WAKEUP_RESERVED and ASPD4525_SCH_PIPE_SEND_HK_RESERVED
**       must be less than ASPD4525_SCH_PIPE_DEPTH.
*/
#define ASPD4525_SCH_PIPE_WAKEUP_RESERVED   (1)

/** \brief The number of SEND_HK messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.  Note the
**       ASPD4525_SCH_PIPE_WAKEUP_RESERVED and ASPD4525_SCH_PIPE_SEND_HK_RESERVED
**       must be less than ASPD4525_SCH_PIPE_DEPTH.
*/
#define ASPD4525_SCH_PIPE_SEND_HK_RESERVED  (1)

/** \brief Pipe depth for the command pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define ASPD4525_CMD_PIPE_DEPTH             (4)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define ASPD4525_CMD_PIPE_NAME              ("ASPD4525_CMD_PIPE")

/** \brief Pipe depth for the data pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define ASPD4525_DATA_PIPE_DEPTH            (4)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define ASPD4525_DATA_PIPE_NAME             ("ASPD4525_DATA_PIPE")

/** \brief The config table default filename
**
**  \par Limits:
**       The length of each string, including the NULL terminator cannot exceed
**       the #OS_MAX_PATH_LEN value.
*/
#define ASPD4525_CONFIG_TABLE_FILENAME      ("/ram/apps/aspd4525_config.tbl")
/** \brief The timeout value, in milliseconds, to wait for ES application startup sync.
**
**  \par Limits:
**       This parameter must be at least 1000 (ms).
*/
#define ASPD4525_STARTUP_TIMEOUT_MSEC    (1000)

/* TODO:  Add Doxygen markup. */
#define ASPD4525_CUSTOM_DEV_PATH       "/tmp/ttyS0"


#ifdef __cplusplus
}
#endif

#endif /* ASPD4525_PLATFORM_CFG_H */

/************************/
/*  End of File Comment */
/************************/
    
