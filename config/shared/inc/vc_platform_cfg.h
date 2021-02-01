#ifndef VC_PLATFORM_CFG_H
#define VC_PLATFORM_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <arpa/inet.h>

#define VC_STREAMING_TASK_FLAGS       OS_ENABLE_CORE_0

/*
** VC Platform Configuration Parameter Definitions
*/

/** \brief Mission specific version number for VC application
**  
**  \par Description:
**       An application version number consists of four parts:
**       major version number, minor version number, revision
**       number and mission specific revision number. The mission
**       specific revision number is defined here and the other
**       parts are defined in "vc_version.h".
**
**  \par Limits:
**       Must be defined as a numeric value that is greater than
**       or equal to zero.
*/
#define VC_MISSION_REV                (0)

/** \brief Pipe depth for the Scheduler pipe 
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define VC_SCH_PIPE_DEPTH             (2)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define VC_SCH_PIPE_NAME              ("VC_SCH_PIPE")

/** \brief The SB pend behavior type for the Scheduler pipe.
**
**  \par Limits:
**       One of: CFE_SB_POLL, CFE_SB_PEND_FOREVER, or the 
**       number of milliseconds to wait for a new message (recommended).
**       Note, using CFE_SB_PEND_FOREVER may cause an unresponsive
**       application if no messages arrive on this pipe.
*/
#define VC_SCH_PIPE_PEND_TIME         (2000)

/** \brief The number of WAKEUP messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max limited to CFE_SB_MAX_PIPE_DEPTH-1.  Note the
**       VC_SCH_PIPE_WAKEUP_RESERVED and VC_SCH_PIPE_SEND_HK_RESERVED
**       must be less than VC_SCH_PIPE_DEPTH.
*/
#define VC_SCH_PIPE_WAKEUP_RESERVED   (1)

/** \brief The number of SEND_HK messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.  Note the
**       VC_SCH_PIPE_WAKEUP_RESERVED and VC_SCH_PIPE_SEND_HK_RESERVED
**       must be less than VC_SCH_PIPE_DEPTH.
*/
#define VC_SCH_PIPE_SEND_HK_RESERVED    (1)

/** \brief Pipe depth for the command pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define VC_CMD_PIPE_DEPTH               (4)

/** \brief Pipe name for the Scheduler pipe 
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define VC_CMD_PIPE_NAME                ("VC_CMD_PIPE")

/** \brief The timeout value, in milliseconds, to wait for ES application startup sync.
**
**  \par Limits:
**       This parameter must be at least 1000 (ms).
*/
#define VC_STARTUP_TIMEOUT_MSEC         (1000)

/** \brief The max address length
**
**  \par Limits:
**       This parameter must be at least 16 for IPV4.
*/
#define VC_ADDRESS_LENGTH               INET_ADDRSTRLEN

/* Custom Settings */


/** \brief Max channels allowed
**
**  \par Limits:
**       Minimum of 1.
*/
#define VC_MAX_OUTPUT_CHANNELS          (5)

/** \brief Max device path length
**
**  \par Limits:
**       OS_MAX_LOCAL_PATH_LEN
*/
#define VC_MAX_DEVICE_PATH              OS_MAX_LOCAL_PATH_LEN

/** \brief Max number of camera devices
**
**  \par Limits:
**       Minimum of 1.
*/
#define VC_MAX_DEVICES                  (1)

/** \brief Max buffer size
**
**  \par Limits:
**       Minimum based on configured device driver requirements.
*/
//#define VC_MAX_BUFFER_SIZE              (115200)
#define VC_MAX_BUFFER_SIZE              (1228800)

/** \brief Max packet size
**
**  \par Limits:
**       UDP max payload size.
*/
#define VC_MAX_PACKET_SIZE              (65527)

/** \brief Retry attemps for interrupted calls
**
**  \par Limits:
**       None.
*/
#define VC_MAX_RETRY_ATTEMPTS           (2)

/** \brief Sleep time micro seconds for interrupted calls
**
**  \par Limits:
**       None.
*/
#define VC_MAX_RETRY_SLEEP_USEC         (10)

/** \brief Streaming task priority
**
**  \par Limits:
**       0 to MAX_PRIORITY (usually 255)
*/
#define VC_STREAMING_TASK_PRIORITY      (91)

/** \brief Streaming task name
**
**  \par Limits:
**       OS_MAX_API_NAME
*/
#define VC_STREAMING_TASK_NAME          "VC_STREAM"

/* Camera settings */

/** \brief Video resolution width
**
**  \par Limits:
**       Specific to device
*/
#define VC_FRAME_WIDTH                  (320)

/** \brief Video resolution height
**
**  \par Limits:
**       Specific to device
*/
#define VC_FRAME_HEIGHT                 (240)

/** \brief Video frame rate
**
**  \par Limits:
**       Specific to device
*/
#define VC_FRAMERATE                    (30)

/** \brief Video camera device path
**
**  \par Limits:
**       OS_MAX_LOCAL_PATH_LEN
*/
#define VC_DEVICE_PATH                  "/dev/video0"


/* Ground system settings */

/** \brief Ground system ip address
**
**  \par Limits:
**       INET_ADDRSTRLEN
*/
#define VC_DESTINATION_IP               "127.0.0.1"

/** \brief Ground system port 
**
**  \par Limits:
**       uint16 decimal
*/
#define VC_DESTINATION_PORT             (3001)

/** \brief Source system ip address 
**
**  \par Limits:
**       INET_ADDRSTRLEN
*/
#define VC_SOURCE_IP                    "0"

/** \brief Source system port
**
**  \par Limits:
**       uint16 decimal
*/
#define VC_SOURCE_PORT                  (0)

/* V4L settings */

/** \brief V4L streaming io method
**
**  \par Limits:
**       V4L specific.
*/
#define VC_V4L_IO_METHOD                V4L2_MEMORY_USERPTR

/** \brief V4L buffer type 
**
**  \par Limits:
**       V4L specific.
*/
#define VC_V4L_BUFFER_TYPE              V4L2_BUF_TYPE_VIDEO_CAPTURE

/** \brief V4L video format
**
**  \par Limits:
**       V4L specific.
*/
#define VC_V4L_VIDEO_FORMAT             V4L2_PIX_FMT_MJPEG

/** \brief V4L field order
**
**  \par Limits:
**       V4L specific.
*/
#define VC_V4L_VIDEO_FIELD_ORDER        V4L2_FIELD_NONE 

/** \brief V4L buffers to request 
**
**  \par Limits:
**       V4L specific.
*/
#define VC_V4L_BUFFER_REQUEST           (4)

/** \brief V4L Memory Type
**
**  \par Limits:
**       V4L specific.
*/
#define VC_V4L_MEMORY_TYPE              V4L2_MEMORY_USERPTR

/* Timeout settings */

/** \brief Wait time for camera data in seconds
**
**  \par Limits:
**       None.
*/
#define VC_BUFFER_FILL_TIMEOUT_SEC      (2)

/** \brief Wait time for camera data in microseconds
**
**  \par Limits:
**       None.
*/
#define VC_BUFFER_FILL_TIMEOUT_USEC     (0)

/** \brief Max timeouts until exiting streaming task
**
**  \par Limits:
**       None.
*/
#define VC_BUFFER_TIMEOUTS_ALLOWED      (2)


#ifdef __cplusplus
}
#endif

#endif /* VC_PLATFORM_CFG_H */

/************************/
/*  End of File Comment */
/************************/
