    
#ifndef HES_PLATFORM_CFG_H
#define HES_PLATFORM_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** hes Platform Configuration Parameter Definitions
*/

/** \brief Mission specific version number for HES application
**  
**  \par Description:
**       An application version number consists of four parts:
**       major version number, minor version number, revision
**       number and mission specific revision number. The mission
**       specific revision number is defined here and the other
**       parts are defined in "hes_version.h".
**
**  \par Limits:
**       Must be defined as a numeric value that is greater than
**       or equal to zero.
*/
#define HES_MISSION_REV                (0)

/** \brief Pipe depth for the Scheduler pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define HES_SCH_PIPE_DEPTH             (2)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define HES_SCH_PIPE_NAME              ("HES_SCH_PIPE")

/** \brief The SB pend behavior type for the Scheduler pipe.
**
**  \par Limits:
**       One of: CFE_SB_POLL, CFE_SB_PEND_FOREVER, or the 
**       number of milliseconds to wait for a new message (recommended).
**       Note, using CFE_SB_PEND_FOREVER may cause an unresponsive
**       application if no messages arrive on this pipe.
*/
#define HES_SCH_PIPE_PEND_TIME         (2000)

/** \brief The number of WAKEUP messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max limited to CFE_SB_MAX_PIPE_DEPTH-1.  Note the
**       HES_SCH_PIPE_WAKEUP_RESERVED and HES_SCH_PIPE_SEND_HK_RESERVED
**       must be less than HES_SCH_PIPE_DEPTH.
*/
#define HES_SCH_PIPE_WAKEUP_RESERVED   (1)

/** \brief The number of SEND_HK messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.  Note the
**       HES_SCH_PIPE_WAKEUP_RESERVED and HES_SCH_PIPE_SEND_HK_RESERVED
**       must be less than HES_SCH_PIPE_DEPTH.
*/
#define HES_SCH_PIPE_SEND_HK_RESERVED  (1)

/** \brief Pipe depth for the command pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define HES_CMD_PIPE_DEPTH             (4)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define HES_CMD_PIPE_NAME              ("HES_CMD_PIPE")

/** \brief Pipe depth for the data pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define HES_DATA_PIPE_DEPTH            (4)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define HES_DATA_PIPE_NAME             ("HES_DATA_PIPE")

/** \brief The config table default filename
**
**  \par Limits:
**       The length of each string, including the NULL terminator cannot exceed
**       the #OS_MAX_PATH_LEN value.
*/
#define HES_CONFIG_TABLE_FILENAME      ("/cf/apps/hes_config.tbl")
/** \brief The timeout value, in milliseconds, to wait for ES application startup sync.
**
**  \par Limits:
**       This parameter must be at least 1000 (ms).
*/
#define HES_STARTUP_TIMEOUT_MSEC    (1000)


#define HES_PX4_VEHICLE_GLOBAL_POSITION_MID    (PX4_VEHICLE_GLOBAL_POSITION_MID)
#define HES_PX4_VEHICLE_ATTITUDE_MID            (PX4_VEHICLE_ATTITUDE_MID)
#define HES_PX4_AIRSPEED_MID                    (PX4_AIRSPEED_MID)
#define HES_PX4_BATTERY_STATUS_MID              (PX4_BATTERY_STATUS_MID)
#define HES_PX4_VEHICLE_LAND_DETECTED_MID       (PX4_VEHICLE_LAND_DETECTED_MID)
#define HES_PX4_VEHICLE_STATUS_MID              (PX4_VEHICLE_STATUS_MID)


#ifdef __cplusplus
}
#endif

#endif /* HES_PLATFORM_CFG_H */

/************************/
/*  End of File Comment */
/************************/
    
