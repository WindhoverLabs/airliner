#ifndef PE_PLATFORM_CFG_H
#define PE_PLATFORM_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** PE Platform Configuration Parameter Definitions
*/

/** \brief Mission specific version number for PE application
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
#define PE_MISSION_REV                (0)

/** \brief Pipe depth for the Scheduler pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define PE_SCH_PIPE_DEPTH             (20)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define PE_SCH_PIPE_NAME              ("PE_SCH_PIPE")

/** \brief The SB pend behavior type for the Scheduler pipe.
**
**  \par Limits:
**       One of: CFE_SB_POLL, CFE_SB_PEND_FOREVER, or the 
**       number of milliseconds to wait for a new message (recommended).
**       Note, using CFE_SB_PEND_FOREVER may cause an unresponsive
**       application if no messages arrive on this pipe.
*/
#define PE_SCH_PIPE_PEND_TIME         (2000)

/** \brief The number of WAKEUP messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max limited to CFE_SB_MAX_PIPE_DEPTH-1.  Note the
**       PE_WAKEUP_MID_MAX_MSG_COUNT and PE_SEND_HK_MID_MAX_MSG_COUNT
**       must be less than PE_SCH_PIPE_DEPTH.
*/
#define PE_WAKEUP_MID_MAX_MSG_COUNT   (1)

/** \brief The number of SEND_HK messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.  Note the
**       PE_WAKEUP_MID_MAX_MSG_COUNT and PE_SEND_HK_MID_MAX_MSG_COUNT
**       must be less than PE_SCH_PIPE_DEPTH.
*/
#define PE_SEND_HK_MID_MAX_MSG_COUNT  (1)

/** \brief Pipe depth for the command pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define PE_CMD_PIPE_DEPTH             (4)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define PE_CMD_PIPE_NAME              ("PE_CMD_PIPE")

/** \brief The config table default filename
**
**  \par Limits:
**       The length of each string, including the NULL terminator cannot exceed
**       the #OS_MAX_PATH_LEN value.
*/
#define PE_CONFIG_TABLE_FILENAME      ("/ram/apps/pe_config.tbl")

/** \brief The timeout value, in milliseconds, to wait for ES application startup sync.
**
**  \par Limits:
**       This parameter must be at least 1000 (ms).
*/
#define PE_STARTUP_TIMEOUT_MSEC    (1000)

/** \brief MAx event filters the PE app can use.
**
**  \par Limits:
**       This parameter cannot exceed TODO
*/
#define PE_MAX_EVENT_FILTERS       (64)

/*** Sensor Configuration ***/

/* Baro */
#define REQ_BARO_INIT_COUNT     (2000)
#define BARO_TIMEOUT            (100000) /* 0.1 s */

/* GPS */
#define REQ_GPS_INIT_COUNT      (50)
#define GPS_TIMEOUT             (1000000) /* 1.0 s */

/* Land */
#define REQ_LAND_INIT_COUNT     (1)
#define LAND_TIMEOUT   		    (1000000) /* 1.0 s */

/* Distance sensor */
#define REQ_DIST_INIT_COUNT     (50)
#define DIST_TIMEOUT            (150000) /* 0.1 s */
#define DIST_BETA_MAX           (700)
#define DIST_SENSOR_TYPE        (3) /* 3 = radar */

/* Optical flow */
#define REQ_FLOW_INIT_COUNT     (10)
#define FLOW_TIMEOUT            (1000000) /* 1.0 s */
#define FLOW_GYRO_HP_CUTOFF     (0.001f)


#ifdef __cplusplus
}
#endif

#endif /* PE_PLATFORM_CFG_H */

/************************/
/*  End of File Comment */
/************************/
    
