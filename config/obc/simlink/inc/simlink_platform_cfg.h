    
#ifndef SIMLINK_PLATFORM_CFG_H
#define SIMLINK_PLATFORM_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** simlink Platform Configuration Parameter Definitions
*/

/** \brief Mission specific version number for SIMLINK application
**  
**  \par Description:
**       An application version number consists of four parts:
**       major version number, minor version number, revision
**       number and mission specific revision number. The mission
**       specific revision number is defined here and the other
**       parts are defined in "simlink_version.h".
**
**  \par Limits:
**       Must be defined as a numeric value that is greater than
**       or equal to zero.
*/
#define SIMLINK_MISSION_REV                (0)

/** \brief Pipe depth for the Scheduler pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define SIMLINK_SCH_PIPE_DEPTH             (2)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define SIMLINK_SCH_PIPE_NAME              ("SIMLINK_SCH_PIPE")

/** \brief The SB pend behavior type for the Scheduler pipe.
**
**  \par Limits:
**       One of: CFE_SB_POLL, CFE_SB_PEND_FOREVER, or the 
**       number of milliseconds to wait for a new message (recommended).
**       Note, using CFE_SB_PEND_FOREVER may cause an unresponsive
**       application if no messages arrive on this pipe.
*/
#define SIMLINK_SCH_PIPE_PEND_TIME         (2000)

/** \brief The number of WAKEUP messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max limited to CFE_SB_MAX_PIPE_DEPTH-1.  Note the
**       SIMLINK_SCH_PIPE_WAKEUP_RESERVED and SIMLINK_SCH_PIPE_SEND_HK_RESERVED
**       must be less than SIMLINK_SCH_PIPE_DEPTH.
*/
#define SIMLINK_SCH_PIPE_WAKEUP_RESERVED   (1)

/** \brief The number of SEND_HK messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.  Note the
**       SIMLINK_SCH_PIPE_WAKEUP_RESERVED and SIMLINK_SCH_PIPE_SEND_HK_RESERVED
**       must be less than SIMLINK_SCH_PIPE_DEPTH.
*/
#define SIMLINK_SCH_PIPE_SEND_HK_RESERVED  (1)

/** \brief Pipe depth for the command pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define SIMLINK_CMD_PIPE_DEPTH             (4)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define SIMLINK_CMD_PIPE_NAME              ("SIMLINK_CMD_PIPE")

/** \brief Pipe depth for the data pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define SIMLINK_DATA_PIPE_DEPTH            (4)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define SIMLINK_DATA_PIPE_NAME             ("SIMLINK_DATA_PIPE")

/** \brief The config table default filename
**
**  \par Limits:
**       The length of each string, including the NULL terminator cannot exceed
**       the #OS_MAX_PATH_LEN value.
*/
#define SIMLINK_CONFIG_TABLE_FILENAME      ("/cf/apps/simlink_cfg.tbl")
/** \brief The timeout value, in milliseconds, to wait for ES application startup sync.
**
**  \par Limits:
**       This parameter must be at least 1000 (ms).
*/
#define SIMLINK_STARTUP_TIMEOUT_MSEC    (1000)



#define SIMLINK_MUTEX_NAME                 "SIMLINK_MUTEX"
#define SIMLINK_LISTENER_TASK_STACK_SIZE   (20000)
#define SIMLINK_LISTENER_TASK_PRIORITY        (48)
#define SIMLINK_LISTENER_TASK_FLAGS            (0)
#define SIMLINK_LISTENER_TASK_NAME         "SIMLINK_LISTENER"
#define SIMLINK_SIM_ADDRESS_ENV_VAR_NAME   "SIMLINK_SIM_ADDRESS"
#define SIMLINK_SIM_PORT_ENV_VAR_NAME      "SIMLINK_SIM_PORT"
#define SIMLINK_UDP_PWM_ENV_VAR_NAME       "SIMLINK_UDP_PWM_PORT"
#define SIMLINK_MAX_MESSAGE_SIZE            (1500)
#define SIMLINK_GPS_DEVICE_COUNT               (4)
#define SIMLINK_GYRO_DEVICE_COUNT              (4)
#define SIMLINK_ACCEL_DEVICE_COUNT             (4)
#define SIMLINK_MAG_DEVICE_COUNT               (4)
#define SIMLINK_BARO_DEVICE_COUNT              (4)
#define SIMLINK_GPS_CONTAINER_NAME_SPEC    "SIMLINK_GPS_%ld"
#define SIMLINK_GYRO_CONTAINER_NAME_SPEC   "SIMLINK_GYRO_%ld"
#define SIMLINK_ACCEL_CONTAINER_NAME_SPEC  "SIMLINK_ACCEL_%ld"
#define SIMLINK_MAG_CONTAINER_NAME_SPEC    "SIMLINK_MAG_%ld"
#define SIMLINK_BARO_CONTAINER_NAME_SPEC   "SIMLINK_BARO_%ld"
#define SIMLINK_PWM_CONTAINER_NAME         "SIMLINK_PWM"

#define SIMLINK_PWM_CHANNEL_COUNT              (16)
#define SIMLINK_MAX_PWM_OUTPUTS                (16)


#ifdef __cplusplus
}
#endif

#endif /* SIMLINK_PLATFORM_CFG_H */

/************************/
/*  End of File Comment */
/************************/
    
