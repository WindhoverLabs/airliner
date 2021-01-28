#ifndef SONAR_PLATFORM_CFG_H
#define SONAR_PLATFORM_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** SONAR Platform Configuration Parameter Definitions
*/

/** \brief Mission specific version number for SONAR application
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
#define SONAR_MISSION_REV                (0)

/** \brief Pipe depth for the Scheduler pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define SONAR_SCH_PIPE_DEPTH             (2)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define SONAR_SCH_PIPE_NAME              ("SONAR_SCH_PIPE")

/** \brief The SB pend behavior type for the Scheduler pipe.
**
**  \par Limits:
**       One of: CFE_SB_POLL, CFE_SB_PEND_FOREVER, or the 
**       number of milliseconds to wait for a new message (recommended).
**       Note, using CFE_SB_PEND_FOREVER may cause an unresponsive
**       application if no messages arrive on this pipe.
*/
#define SONAR_SCH_PIPE_PEND_TIME         (2000)

/** \brief The number of WAKEUP messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max limited to CFE_SB_MAX_PIPE_DEPTH-1.  Note the
**       SONAR_MEASURE_MID_MAX_MSG_COUNT and SONAR_SEND_HK_MID_MAX_MSG_COUNT
**       must be less than SONAR_SCH_PIPE_DEPTH.
*/
#define SONAR_MEASURE_MID_MAX_MSG_COUNT   (1)

/** \brief The number of SEND_HK messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.  Note the
**       SONAR_MEASURE_MID_MAX_MSG_COUNT and SONAR_SEND_HK_MID_MAX_MSG_COUNT
**       must be less than SONAR_SCH_PIPE_DEPTH.
*/
#define SONAR_SEND_HK_MID_MAX_MSG_COUNT  (1)

/** \brief Pipe depth for the command pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define SONAR_CMD_PIPE_DEPTH             (4)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define SONAR_CMD_PIPE_NAME              ("SONAR_CMD_PIPE")

/** \brief Pipe depth for the data pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define SONAR_DATA_PIPE_DEPTH            (4)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define SONAR_DATA_PIPE_NAME             ("SONAR_DATA_PIPE")

/** \brief The config table default filename
**
**  \par Limits:
**       The length of each string, including the NULL terminator cannot exceed
**       the #OS_MAX_PATH_LEN value.
*/
#define SONAR_CONFIG_TABLE_FILENAME      ("/cf/apps/sonar_config.tbl")

/** \brief The timeout value, in milliseconds, to wait for ES application startup sync.
**
**  \par Limits:
**       This parameter must be at least 1000 (ms).
*/
#define SONAR_STARTUP_TIMEOUT_MSEC    (1000)

/* TODO */
#define SONAR_SENS_VARIANCE           (1.0f)

#define SONAR_MIN_DISTANCE            (0.5f)

#define SONAR_MAX_DISTANCE            (8.5f)


#ifdef __cplusplus
}
#endif

#endif /* SONAR_PLATFORM_CFG_H */

/************************/
/*  End of File Comment */
/************************/
    
