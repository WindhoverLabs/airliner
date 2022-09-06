#ifndef TO_PLATFORM_CFG_H
#define TO_PLATFORM_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cfe_platform_cfg.h"

/*
** Platform Configuration Parameter Definitions
*/


/** \tocfg Child task flags
**
**  \par Description:
**       These are optional flags passed to the #CFE_ES_CreateChildTask
**       function when creating the CFE child task.
**
*/
#define TO_CUSTOM_CHILD_TASK_FLAGS     (OS_ENABLE_CORE_0)


/** \tocfg Child task stack size
**
**  \par Description:
**       Child task stack size passed to the #CFE_ES_CreateChildTask
**       function when creating the CFE child task.  Stack size is in
**       bytes.
**
*/
#define TO_CUSTOM_TASK_STACK_SIZE      (131072)


/** \tocfg Default telemetry destination IP address
**
**  \par Description:
**       Default IP address to send the telemetry stream to.  This is default
**       only and can be changed by command at runtime.
**
*/
#define TO_UDP_CHANNEL_ADDRESS         "127.0.0.1"


/** \tocfg Default telemetry destination UDP port
**
**  \par Description:
**       Default UDP port to send the telemetry stream to.  This is default
**       only and can be changed by command at runtime.
**
*/
#define TO_UDP_CHANNEL_PORT            (5011)


/** \tocfg Message Flow packet limit
**
**  \par Description:
**       This is the maximum number of message flow entries that can fit in
**       a message flow diagnostic message.
**
*/
#define TO_MSG_FLOW_PKT_LIMIT          (50)


/** \tocfg Maximum message length
**
**  \par Description:
**       The maximum size that an output telemetry message can be.  This is in
**       bytes.  If a message exceeds this size, it will be dropped by the
**       classifier.
**
*/
#define TO_MAX_MSG_LENGTH              (3000)

/** \tocfg The UDP channel's CF throttling semaphore name
**
**  \par Limits:
**       Two channels in CF must not have the same semaphore name. This must be unique
**       compared to other channels' throttling semaphores.
*/
#define TO_UDP_CF_THROTTLE_SEM_NAME    ("CFTOSem_UDP")


/** \tocfg The UDP channel's default and maximum value for the CF
 *         throttling semaphore
**
**  \par Limits:
**       Must not be larger than the TO_DATA_PIPE_DEPTH
*/
#define TO_UDP_CF_MAX_PDUS    (4)


/** \tocfg Mission specific version number for TO application
**  
**  \par Description:
**       An application version number consists of four parts:
**       major version number, minor version number, revision
**       number and mission specific revision number. The mission
**       specific revision number is defined here and the other
**       parts are defined in "to_version.h".
**
**  \par Limits:
**       Must be defined as a numeric value that is greater than
**       or equal to zero.
*/
#define TO_MISSION_REV                (1)

/** \tocfg Pipe depth for the Scheduler pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define TO_SCH_PIPE_DEPTH             (2)

/** \tocfg Pipe name for the Scheduler pipe
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define TO_SCH_PIPE_NAME              ("TO_SCH_PIPE")

/** \tocfg The SB pend behavior type for the Scheduler pipe.
**
**  \par Limits:
**       One of: CFE_SB_POLL, CFE_SB_PEND_FOREVER, or the 
**       number of milliseconds to wait for a new message (recommended).
**       Note, using CFE_SB_PEND_FOREVER may cause an unresponsive
**       application if no messages arrive on this pipe.
*/
#define TO_SCH_PIPE_PEND_TIME         (2000)

/** \tocfg The number of WAKEUP messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max limited to CFE_SB_MAX_PIPE_DEPTH-1.  Note the
**       TO_SCH_PIPE_WAKEUP_RESERVED and TO_SCH_PIPE_SEND_HK_RESERVED
**       must be less than TO_SCH_PIPE_DEPTH.
*/
#define TO_SCH_PIPE_WAKEUP_RESERVED   (1)

/** \tocfg The number of SEND_HK messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.  Note the
**       TO_SCH_PIPE_WAKEUP_RESERVED and TO_SCH_PIPE_SEND_HK_RESERVED
**       must be less than TO_SCH_PIPE_DEPTH.
*/
#define TO_SCH_PIPE_SEND_HK_RESERVED  (1)

/** \tocfg Pipe depth for the command pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define TO_CMD_PIPE_DEPTH             (4)

/** \tocfg Pipe name for the Scheduler pipe
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define TO_CMD_PIPE_NAME              ("TO_CMD_PIPE")

/** \tocfg Pipe depth for the data pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define TO_DATA_PIPE_DEPTH            (CFE_SB_MAX_PIPE_DEPTH)

/** \tocfg The config table default filename
**
**  \par Limits:
**       The length of each string, including the NULL terminator cannot exceed
**       the #OS_MAX_PATH_LEN value.
*/
#define TO_UDP_CONFIG_TABLE_FILENAME    ("/cf/apps/to_udp_cfg.tbl")


#define TO_MAX_MEMPOOL_BLK_SIZES      (8)

/**
**  \cfeescfg Define TO Memory Pool Block Sizes
**
**  \par Description:
**       TO Memory Pool Block Sizes
**
**  \par Limits
**       These sizes MUST be increasing and MUST be an integral multiple of 4.
**       The number of block sizes defined cannot exceed
**       #CFE_ES_MAX_MEMPOOL_BLOCK_SIZES
*/
#define TO_MEM_BLOCK_SIZE_01             32
#define TO_MEM_BLOCK_SIZE_02             64
#define TO_MEM_BLOCK_SIZE_03            128
#define TO_MEM_BLOCK_SIZE_04            256
#define TO_MEM_BLOCK_SIZE_05            512
#define TO_MEM_BLOCK_SIZE_06           2048
#define TO_MEM_BLOCK_SIZE_07           8192
#define TO_MAX_BLOCK_SIZE       (TO_MEM_BLOCK_SIZE_07 + TO_MEM_BLOCK_SIZE_07)

/**
 * \tocfg Defines the table identification name used for table registration
 *        of the configuration table.
 */
#define TO_UDP_CONFIG_TABLENAME        "UDP_CFG"

/**
 * \tocfg Defines the table identification name used for table registration
 *        of the dump table.
 */
#define TO_UDP_DUMP_TABLENAME          "UDP_DMP"

/** \tocfg The timeout value, in milliseconds, to wait for ES application startup sync.
**
**  \par Limits:
**       This parameter must be at least 1000 (ms).
*/
#define TO_STARTUP_TIMEOUT_MSEC    (1000)

/** \tocfg Maximum number of messages to classify per processing frame.
*/
#define TO_MAX_MSGS_OUT_PER_FRAME (100)

/** \tocfg Maximum number of messages to classify per processing frame.
*/

/** \tocfg Maximum number configurable channels.
 *
**  \par Description:  This is normally set to 1.
*/
#define TO_MAX_CHANNELS	          (1)

/** \tocfg Channel output queue depth.
 *
**  \par Description:  This defines how many messages can be queued into the
**       channel output queue by the scheduler.
*/
#define TO_OUTPUT_QUEUE_DEPTH     (20)

/** \tocfg Development UDP channel task priority.
 *
**  \par Description:  This defines the priority of the development UDP
**       child task.
*/
#define TO_UDP_CHANNEL_TASK_PRIORITY (119)

/**
**  \tocfg Number of bytes in the Message Memory Pool
**
**  \par Description:
**       The message memory pool contains the memory needed for the queued messages packets.
**       The queued messages are dynamically allocated from this pool when the
**       messages are received and queued, and deallocated when messages are transmitted
**       or a new table is loaded.
**
**  \par Limits
**       The Telemetry Output app does not place a limit on this parameter, but there is
**       an overhead cost in the memory pool.  The value must be larger than what is
**       needed.
*/
#define TO_NUM_BYTES_IN_MEM_POOL        (TO_MAX_CHANNELS * TO_OUTPUT_QUEUE_DEPTH * TO_MAX_BLOCK_SIZE)

#define TO_SPACE_TO_GND_PDU_MID         (0)

#ifdef __cplusplus
}
#endif

#endif /* TO_PLATFORM_CFG_H */

/************************/
/*  End of File Comment */
/************************/
    
