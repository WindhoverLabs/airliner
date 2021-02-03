/****************************************************************************
*
*   Copyright (c) 2017 Windhover Labs, L.L.C. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
* 3. Neither the name Windhover Labs nor the names of its 
*    contributors may be used to endorse or promote products derived 
*    from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/

#ifndef PQ_PLATFORM_CFG_H
#define PQ_PLATFORM_CFG_H

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
#define PQ_CUSTOM_CHILD_TASK_FLAGS     (OS_ENABLE_CORE_0)


/** \tocfg Child task stack size
**
**  \par Description:
**       Child task stack size passed to the #CFE_ES_CreateChildTask
**       function when creating the CFE child task.  Stack size is in
**       bytes.
**
*/
#define PQ_CUSTOM_TASK_STACK_SIZE      (131072)


/** \tocfg Default telemetry destination IP address
**
**  \par Description:
**       Default IP address to send the telemetry stream to.  This is default
**       only and can be changed by command at runtime.
**
*/
#define PQ_UDP_CHANNEL_ADDRESS         "127.0.0.1"


/** \tocfg Default telemetry destination UDP port
**
**  \par Description:
**       Default UDP port to send the telemetry stream to.  This is default
**       only and can be changed by command at runtime.
**
*/
#define PQ_UDP_CHANNEL_PORT            (5011)


/** \tocfg Message Flow packet limit
**
**  \par Description:
**       This is the maximum number of message flow entries that can fit in
**       a message flow diagnostic message.
**
*/
#define PQ_MSG_FLOW_PKT_LIMIT          (200)


/** \tocfg Maximum message length
**
**  \par Description:
**       The maximum size that an output telemetry message can be.  This is in
**       bytes.  If a message exceeds this size, it will be dropped by the
**       classifier.
**
*/
#define PQ_MAX_MSG_LENGTH              (32767)

/** \tocfg The UDP channel's CF throttling semaphore name
**
**  \par Limits:
**       Two channels in CF must not have the same semaphore name. This must be unique
**       compared to other channels' throttling semaphores.
*/
#define PQ_UDP_CF_THROTTLE_SEM_NAME    "PQ_CF_CH0_SEM"


/** \tocfg The UDP channel Name
*/
#define PQ_UDP_CHANNEL_NAME             "UDP"



/** \tocfg The UDP channel's default and maximum value for the CF
 *         throttling semaphore
**
**  \par Limits:
**       Must not be larger than the PQ_DATA_PIPE_DEPTH
*/
#define PQ_UDP_CF_MAX_PDUS    (4)


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
#define PQ_MISSION_REV                (1)

/** \tocfg Pipe depth for the Scheduler pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define PQ_SCH_PIPE_DEPTH             (2)

/** \tocfg Pipe name for the Scheduler pipe
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define PQ_SCH_PIPE_NAME              ("PQ_SCH_PIPE")

/** \tocfg The SB pend behavior type for the Scheduler pipe.
**
**  \par Limits:
**       One of: CFE_SB_POLL, CFE_SB_PEND_FOREVER, or the 
**       number of milliseconds to wait for a new message (recommended).
**       Note, using CFE_SB_PEND_FOREVER may cause an unresponsive
**       application if no messages arrive on this pipe.
*/
#define PQ_SCH_PIPE_PEND_TIME         (2000)

/** \tocfg The number of WAKEUP messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max limited to CFE_SB_MAX_PIPE_DEPTH-1.  Note the
**       PQ_SCH_PIPE_WAKEUP_RESERVED and PQ_SCH_PIPE_SEND_HK_RESERVED
**       must be less than PQ_SCH_PIPE_DEPTH.
*/
#define PQ_SCH_PIPE_WAKEUP_RESERVED   (1)

/** \tocfg The number of SEND_HK messages to reserve on the Scheduler pipe.
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.  Note the
**       PQ_SCH_PIPE_WAKEUP_RESERVED and PQ_SCH_PIPE_SEND_HK_RESERVED
**       must be less than PQ_SCH_PIPE_DEPTH.
*/
#define PQ_SCH_PIPE_SEND_HK_RESERVED  (1)

/** \tocfg Pipe depth for the command pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define PQ_CMD_PIPE_DEPTH             (4)

/** \tocfg Pipe name for the Scheduler pipe
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define PQ_CMD_PIPE_NAME              ("PQ_CMD_PIPE")

/** \tocfg Pipe depth for the data pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define PQ_DATA_PIPE_DEPTH            (CFE_SB_MAX_PIPE_DEPTH)

/** \tocfg The config table default filename
**
**  \par Limits:
**       The length of each string, including the NULL terminator cannot exceed
**       the #OS_MAX_PATH_LEN value.
*/
#define PQ_UDP_CONFIG_TABLE_FILENAME    ("/cf/apps/to_udp_cfg.tbl")


#define PQ_MAX_MEMPOOL_BLK_SIZES      (8)

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
#define PQ_MEM_BLOCK_SIZE_01             32
#define PQ_MEM_BLOCK_SIZE_02             64
#define PQ_MEM_BLOCK_SIZE_03            128
#define PQ_MEM_BLOCK_SIZE_04            256
#define PQ_MEM_BLOCK_SIZE_05            512
#define PQ_MEM_BLOCK_SIZE_06           2048
#define PQ_MEM_BLOCK_SIZE_07           8192
#define PQ_MAX_BLOCK_SIZE       (PQ_MEM_BLOCK_SIZE_07 + PQ_MEM_BLOCK_SIZE_07)

/**
 * \tocfg Defines the table identification name used for table registration
 *        of the configuration table.
 */
#define PQ_UDP_CONFIG_TABLENAME        "UDP_CFG"

/**
 * \tocfg Defines the table identification name used for table registration
 *        of the dump table.
 */
#define PQ_UDP_DUMP_TABLENAME          "UDP_DMP"

/** \tocfg The timeout value, in milliseconds, to wait for ES application startup sync.
**
**  \par Limits:
**       This parameter must be at least 1000 (ms).
*/
#define PQ_STARTUP_TIMEOUT_MSEC    (1000)

/** \tocfg Maximum number of messages to classify per processing frame.
*/
#define PQ_MAX_MSGS_OUT_PER_FRAME (100)

/** \tocfg Maximum number of messages to classify per processing frame.
*/

/** \tocfg Maximum number configurable channels.
 *
**  \par Description:  This is normally set to 1.
*/
#define PQ_MAX_CHANNELS	          (1)

/** \tocfg Channel output queue depth.
 *
**  \par Description:  This defines how many messages can be queued into the
**       channel output queue by the scheduler.
*/
#define PQ_OUTPUT_QUEUE_DEPTH     (20)

/** \tocfg Development UDP channel task priority.
 *
**  \par Description:  This defines the priority of the development UDP
**       child task.
*/
#define PQ_UDP_CHANNEL_TASK_PRIORITY (119)

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
#define PQ_NUM_BYTES_IN_MEM_POOL        (200 * PQ_MAX_BLOCK_SIZE)

/**
 * \brief Defines the number of entries in the table
 */
#define PQ_CONFIG_TABLE_MAX_ENTRIES  (1)

#ifdef __cplusplus
}
#endif

#endif /* PQ_PLATFORM_CFG_H */

/************************/
/*  End of File Comment */
/************************/
    
