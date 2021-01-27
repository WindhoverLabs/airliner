#ifndef FLOW_PLATFORM_CFG_H
#define FLOW_PLATFORM_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** FLOW Platform Configuration Parameter Definitions
*/

/** \brief Mission specific version number for FLOW application
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
#define FLOW_MISSION_REV                (0)

/** \brief Pipe depth for the Scheduler pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define FLOW_SCH_PIPE_DEPTH             (2)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define FLOW_SCH_PIPE_NAME              ("FLOW_SCH_PIPE")

/** \brief The number of WAKEUP messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max limited to CFE_SB_MAX_PIPE_DEPTH-1.  Note the
**       FLOW_WAKEUP_MID_MAX_MSG_COUNT and FLOW_SEND_HK_MID_MAX_MSG_COUNT
**       must be less than FLOW_SCH_PIPE_DEPTH.
*/
#define FLOW_WAKEUP_MID_MAX_MSG_COUNT   (1)

/** \brief The number of SEND_HK messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.  Note the
**       FLOW_WAKEUP_MID_MAX_MSG_COUNT and FLOW_SEND_HK_MID_MAX_MSG_COUNT
**       must be less than FLOW_SCH_PIPE_DEPTH.
*/
#define FLOW_SEND_HK_MID_MAX_MSG_COUNT  (1)

/** \brief Pipe depth for the command pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define FLOW_CMD_PIPE_DEPTH             (4)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define FLOW_CMD_PIPE_NAME              ("FLOW_CMD_PIPE")

/** \brief Pipe depth for the data pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define FLOW_DATA_PIPE_DEPTH            (4)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define FLOW_DATA_PIPE_NAME             ("FLOW_DATA_PIPE")

/** \brief The config table default filename
**
**  \par Limits:
**       The length of each string, including the NULL terminator cannot exceed
**       the #OS_MAX_PATH_LEN value.
*/
#define FLOW_CONFIG_TABLE_FILENAME      ("/ram/apps/flow_config.tbl")

/** \brief The timeout value, in milliseconds, to wait for ES application startup sync.
**
**  \par Limits:
**       This parameter must be at least 1000 (ms).
*/
#define FLOW_STARTUP_TIMEOUT_MSEC    (1000)

/** \brief Pipe depth for the Gyroscope pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define FLOW_GYRO_PIPE_DEPTH	(30)

/** \brief Pipe name for the Gyroscope pipe
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define FLOW_GYRO_PIPE_NAME    ("FLOW_GYRO_PIPE")

/** \brief The number of SENSOR_GYRO messages to reserve on the Gyroscope pipe.
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.  Note the
**       FLOW_SENSOR_GYRO_MID_MAX_MSG_COUNT
**       must be less than FLOW_GYRO_PIPE_DEPTH.
*/
#define FLOW_SENSOR_GYRO_MAX_MSG_COUNT (25)


#ifdef __cplusplus
}
#endif

#endif /* FLOW_PLATFORM_CFG_H */

/************************/
/*  End of File Comment */
/************************/
    
