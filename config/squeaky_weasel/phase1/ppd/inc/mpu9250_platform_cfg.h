#ifndef MPU9250_PLATFORM_CFG_H
#define MPU9250_PLATFORM_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "math.h"

/*
** MPU9250 Platform Configuration Parameter Definitions
*/

/** \brief Mission specific version number for MPU9250 application
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
#define MPU9250_MISSION_REV                (0)

/** \brief Pipe depth for the Scheduler pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define MPU9250_SCH_PIPE_DEPTH             (2)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define MPU9250_SCH_PIPE_NAME              ("MPU9250_SCH_PIPE")

/** \brief The SB pend behavior type for the Scheduler pipe.
**
**  \par Limits:
**       One of: CFE_SB_POLL, CFE_SB_PEND_FOREVER, or the 
**       number of milliseconds to wait for a new message (recommended).
**       Note, using CFE_SB_PEND_FOREVER may cause an unresponsive
**       application if no messages arrive on this pipe.
*/
#define MPU9250_SCH_PIPE_PEND_TIME         (2000)

/** \brief Pipe depth for the params pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define MPU9250_PARAM_PIPE_DEPTH            (4)

/** \brief Pipe name for the params pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define MPU9250_PARAM_PIPE_NAME             ("MPU9250_PARAM_PIPE")

/** \brief The number of WAKEUP messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max limited to CFE_SB_MAX_PIPE_DEPTH-1.  Note the
**       MPU9250_MEASURE_MID_MAX_MSG_COUNT and MPU9250_SEND_HK_MID_MAX_MSG_COUNT
**       must be less than MPU9250_SCH_PIPE_DEPTH.
*/
#define MPU9250_MEASURE_MID_MAX_MSG_COUNT   (1)

/** \brief The number of SEND_HK messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.  Note the
**       MPU9250_MEASURE_MID_MAX_MSG_COUNT and MPU9250_SEND_HK_MID_MAX_MSG_COUNT
**       must be less than MPU9250_SCH_PIPE_DEPTH.
*/
#define MPU9250_SEND_HK_MID_MAX_MSG_COUNT  (1)

/** \brief Pipe depth for the command pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define MPU9250_CMD_PIPE_DEPTH             (4)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define MPU9250_CMD_PIPE_NAME              ("MPU9250_CMD_PIPE")

/** \brief Pipe depth for the data pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define MPU9250_DATA_PIPE_DEPTH            (4)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define MPU9250_DATA_PIPE_NAME             ("MPU9250_DATA_PIPE")

/** \brief The config table default filename
**
**  \par Limits:
**       The length of each string, including the NULL terminator cannot exceed
**       the #OS_MAX_PATH_LEN value.
*/
#define MPU9250_CONFIG_TABLE_FILENAME      ("/cf/apps/mpu9250_config.tbl")

/** \brief The timeout value, in milliseconds, to wait for ES application startup sync.
**
**  \par Limits:
**       This parameter must be at least 1000 (ms).
*/
#define MPU9250_STARTUP_TIMEOUT_MSEC        (1000)


/** \brief Fake accel device ID */
#define MPU9250_ACCEL_PX4_DEVICE_ID         (6789478)
/** \brief Fake gyro device ID */
#define MPU9250_GYRO_PX4_DEVICE_ID          (3467548)
/** \brief Integration publish rate */
#define MPU9250_ACCEL_INT_PUB_RATE          (1000000 / 400)
/** \brief Integration publish rate */
#define MPU9250_GYRO_INT_PUB_RATE           (1000000 / 400)
/** \brief Accel scale */
#define MPU9250_ACC_SCALE                   (16)
/** \brief Gyro scale */
#define MPU9250_GYRO_SCALE                  (2000)
/** \brief Magnetometer device ID. */
#define MPU9250_AK8963_ID                   (0x48)
/** \brief IMU device ID. */
#define MPU9250_DEVICE_ID                   (0x71)
/** \brief IMU accelerometer sample rate. */
#define MPU9250_ACCEL_SAMPLE_RATE           (200)
/** \brief IMU accelerometer filter cutoff frequency. */
#define MPU9250_ACCEL_FILTER_CUTOFF_FREQ    (30)
/** \brief IMU gyroscope sample rate. */
#define MPU9250_GYRO_SAMPLE_RATE            (200)
/** \brief IMU gyroscope filter cutoff frequency. */
#define MPU9250_GYRO_FILTER_CUTOFF_FREQ     (30)
/** \brief Set to never publish (0) in PX4 mpu9250 wrapper. */
#define MPU9250_NEVER_AUTOPUBLISH_US        (0)
/** \brief One gravity. */
#define MPU9250_ONE_G                       (9.80665f)
/** \brief Radians per degree. */
#define MPU9250_RADIANS_PER_DEGREE          (0.0174532f)
/** \brief Room temperature offset */
#define MPU9250_ROOM_TEMP_OFFSET            (0.0f)
/** \brief Temperature sensitivity */
#define MPU9250_TEMP_SENS                   (361.0f)


#ifdef __cplusplus
}
#endif

#endif /* MPU9250_PLATFORM_CFG_H */

/************************/
/*  End of File Comment */
/************************/
    
