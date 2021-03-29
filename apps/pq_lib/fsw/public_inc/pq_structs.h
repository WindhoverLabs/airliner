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

#ifndef PQ_STRUCTS_H
#define PQ_STRUCTS_H

#include "cfe.h"
#include "pq_platform_cfg.h"
#include "pq_mission_cfg.h"

/* pq_priority_queue */

/**
** \brief Priority queue state.  Used to determine if a table entry is used or not.
*/
typedef enum
{
    /** \brief Priority Queue Unused state */
    PQ_PQUEUE_UNUSED = 0,
    
    /** \brief Priority Queue Enabled state */
    PQ_PQUEUE_ENA    = 1,

    /** \brief  This is a count of Priority Queue states and should not be used.  */
    PQ_PQUEUE_COUNT

} PQ_PriorityQueueState_t;

/**
** \brief Priority queue type.
*/
typedef enum
{
    /** \brief 'FIFO' queues are normal FIFO queues. */
    PQ_PRIORITY_QUEUE_TYPE_FIFO = 0,

    /** \brief 'SINGLE' queues are currently unused. */
    PQ_PRIORITY_QUEUE_TYPE_SINGLE = 1,

    /** \brief This is a count of Priority Queue types and should not be used.  */
    PQ_PRIORITY_QUEUE_TYPE_COUNT,

} PQ_PriorityQueueType_t;

/** \brief Definition for a single priority queue entry. */
typedef struct
{
    /** \brief Priority Queue State (unused or enabled) */
    uint8  State;
    
    /** \brief Priority Queue Message Limit.  */
    uint16 MsgLimit;
    
    /** \brief Priority Queue Type (only one type - FIFO) */
    PQ_PriorityQueueType_t  QType;

} PQ_PriorityQueue_t OS_ALIGN(2);

/** \brief Definition for Priority Queue Metrics */
typedef struct
{
    /** \brief Priority Queue Dropped Message Count */
    uint32 DroppedMsgCnt;
    
    /** \brief Priority Queue Queued Message Count */
    uint32 QueuedMsgCnt;
    
    /** \brief Priority Queue Passed (not filtered) Message Count */
    uint32 PassCnt;
    
    /** \brief Priority Queue Filtered (not sent/passed) Message Count */
    uint32 FilteredCnt;
    
    /** \brief Priority Queue Currently Queued Message Count */     
    uint16 CurrentlyQueuedCnt;
    
    /** \brief Priority Queue Highest recorded the Queued Message Count */
    uint16 HighwaterMark;
    
    /** \brief Priority Queue Queue ID  */ 
    uint32 OSALQueueID;
    
} PQ_PriorityQueueMetrics_t;

/* pq_message_flow */

/** \brief Definition for a single telemetry message flow entry */
typedef struct
{
    /**
     * \brief  The Message ID is used to select the message from the cFE Software Bus (SB) to send
     *  through the TO channel managed by the table.
     *
     * \par
     * Units: none.
     *
     * \par
     * Limits: must be defined in cfe_mission_cfg.h
     */
    CFE_SB_MsgId_t  MsgId;

    /**
     * \brief  Maximum number of messages in a Message Flow.
     *
     * \par
     * Units: Number of messages that can be in a Message Flow.
     *
     * \par
     * Limits: The mininum value is 1.  The maximum value depends on available memory and is
     *         an OPS driven parameter.  The sum of all the msglimits in a priority queue 
     *         should not be greater than the msglimit of that priority queue.
     */
    uint16          MsgLimit;

    /**
     * \brief  The Priority Queue ID is an index which selects the priority queue the message will be
     *  placed into.
     *
     * \par
     * Units: none.
     *
     * \par
     * Limits: If the Priority Queue ID does not match a priority queue then the table is invalid
     */
    uint16          PQueueID;
       
} PQ_MessageFlow_t OS_ALIGN(2);


/** \brief Definition for the metrics collected for a single telemetry message flow entry */
typedef struct
{
    /**
     * \brief  Dropped message count is a count of number of times a specific message was 
     *  dropped.  
     *
     * \par
     * Units: none.
     *
     * \par
     * Limits: none.
     */
    uint32          DroppedMsgCnt;

    /**
     * \brief  Queued message count is a count of number of times a specific message was queued for sending.  
     *
     * \par
     * Units: none.
     *
     * \par
     * Limits: none.
     */
    uint32          QueuedMsgCnt;

    /**
     * \brief  SBMsgCnt is the total number of messages for the particular message ID pulled from the 
     *         software bus for that channel. Used for message decimation.
     *
     * \par
     * Units: none.
     *
     * \par
     * Limits: Increments on each message received. Rolls over to 0 at max value.
     */
    uint32          SBMsgCnt;

} PQ_MessageFlowMetrics_t;

/* pq_output_queue structs */

/**
** \brief Definition for a single output channel queue entry.
*/
typedef struct
{
    /** \brief OSAL queue descriptor */
    uint32                  OSALQueueID;
    
    /** \brief Count of the number of bytes sent from the output queue */
    uint32                  SentBytes;
    
    /** \brief Count of the number of messages sent from the output queue */
    uint32                  SentCount;
    
    /** \brief Count of the number of messages currently queued on output queue */
    uint16                  CurrentlyQueuedCnt;
    
    /** \brief Count of the number of bytes sent from the output queue */
    uint16                  HighwaterMark;

} PQ_OutputQueue_t;


/* pq_channel structs */

/**
** \brief Channel States.
*
*  \par PQ channels can have three states:
* 
*   PQ_CHANNEL_UNKNOWN
*   PQ_CHANNEL_OPENED
*   PQ_CHANNEL_CLOSED  
*         
*  \note All channels that are opened will never be commanded to close.
*        The only time where a channel is closed is during clean-up.
*/
typedef enum
{

   /** \brief Channel is unknown.
     *
     * \par
     * This state means the channel was either never initialized or failed to initialized
     * and is in an unusable state (no data flow).
     */
    PQ_CHANNEL_UNKNOWN = 0,

    /** \brief Channel closed state.
     *
     * \par
     * This state means that the channel is not operational (no data flow).     
     */
    PQ_CHANNEL_CLOSED = 1,
    
   /** \brief Channel opened state.
     *
     * \par
     * This state means the channel is operational (data can flow).     
     */
    PQ_CHANNEL_OPENED = 2,

    /** \brief This is a count of Channel states and should not be used. */
    PQ_CHANNEL_STATE_COUNT

} PQ_ChannelState_t OS_ALIGN(4);

/*Make sure uint8 can store enum*/
CompileTimeAssert((PQ_CHANNEL_STATE_COUNT <= 0xff), PQ_ChannelState_t_less_than_256);

/* pq_tbldefs */

/** \pqtbl PQ Configuration Table 
 *
 * \brief Definition for a PQ Configuration table 
 */
typedef struct
{
    /**
     * \brief  The table identifier, used to identify a specific config table.
     *
     * \par
     * Units: none.
     *
     * \par
     * Limits: must be greater than or equal to 0
     *
     * \par
     * Note: TableID value 0xFFFFFFFF is reserved for each channel's 
     * backup emergency table.  The backup emergency tables are loaded
     * when the channel's table file could not be loaded.
     * It is the largest uint32 available 4294967295.     
     */
    uint32                       TableID;

    /**
     * \brief  MessageFlow defines a single telemetry message flow entry.
     *
     * \par
     * Units: none.
     *
     * \par
     * Limits: The maximum number of message flows is defined in to_mission_cfg.h
     */
    PQ_MessageFlow_t             MessageFlow[PQ_MAX_MESSAGE_FLOWS];

    /**
     * \brief  The Priority Queue defines a single priority queue entry.
     *
     * \par
     * Units: none.
     *
     * \par
     * Limits: The maximum number of priority queues is defined in to_mission_cfg.h
     */
    PQ_PriorityQueue_t           PriorityQueue[PQ_MAX_PRIORITY_QUEUES];

} PQ_ChannelTbl_t;

/** \brief Definition for a single PQ dump table entry */
typedef struct
{
    /**
     * \brief  The table identifier, used to identify a specific dump table.
     *
     * \par
     * Units: none.
     *
     * \par
     * Limits: must be greater than or equal to 0
     */
    uint16                       TableID;

    /**
     * \brief  This MessageFlow is a structure which defines the message flow metrics
     *  to be collected.
     *
     * \par
     * Units: none.
     *
     * \par
     * Limits: PQ_MAX_QUEUE_SIZE_LIMIT.
     */
    PQ_MessageFlowMetrics_t      MessageFlow[PQ_MAX_MESSAGE_FLOWS];

    /**
     * \brief  This PriorityQueue is a structure which defines the priority queue metrics
     *  to be collected.
     *
     * \par
     * Units: none.
     *
     * \par
     * Limits: PQ_MAX_PRIORITY_QUEUES
     */
    PQ_PriorityQueueMetrics_t    PriorityQueue[PQ_MAX_PRIORITY_QUEUES];

    /**
     * \brief  This OutputQueue is a structure which defines the output queue metrics
     *  to be collected.
     *
     * \par
     * Units: none.
     *
     * \par
     * Limits: none.
     */
    PQ_OutputQueue_t             OutputQueue;

} PQ_ChannelDumpTbl_t;

/**
** \brief Definition of Channel Data.
*
*/
typedef struct
{
    /* Config table-related */

    /** \brief Config Table Handle
     *
     *  \note CFE_TBL_Handle_t is a int16
     */
    CFE_TBL_Handle_t     ConfigTblHdl;

    /** \brief Dump Table Handle */
    CFE_TBL_Handle_t     DumpTblHdl;

    /** \brief Config Table Pointer
     *
     * \par
     * A pointer to config table data from cFE table services.
     *
     * \par
     * Note, this pointer can be null if the table data is not current or valid.
     * The reasons for this can occur outside of nominal app function and the
     * pointer could be restored to a non-null value at another time.
     */
    PQ_ChannelTbl_t*     ConfigTblPtr;

    /** \brief Pointer to the memory resident backup table, if the channel
     * was configured to have one.  If there is no backup table, this is null.
     */
    PQ_ChannelTbl_t*     BackupTblPtr;

    /** \brief Dump Table Pointer */
    PQ_ChannelDumpTbl_t  DumpTbl;

    /** \brief Memory pool buffer for queued messages. */
    uint8                MemPoolBuffer[PQ_NUM_BYTES_IN_MEM_POOL];

    /** \totlmmnemonic  PQ_MEMPOOLHNDL
        \brief Memory pool handle for queued messages. */
    uint32               MemPoolHandle;

    /** \totlmmnemonic  PQ_MEMINUSE
        \brief The total memory currently in use */
    uint32               MemInUse;

    /** \totlmmnemonic  PQ_PKMEMINUSE
        \brief The peak memory in use */
    uint32               PeakMemInUse;

    /** \totlmmnemonic  PQ_MEMFULLCNT
        \brief The number of times a message was not queued due to memory pool being full. */
    uint32               MemFullCount;

    /** \brief Mutex to protect the Channel data*/
    uint32               MutexID;
    
    /** \totlm Sent Channel Telemetry
    \brief Count of telemetry messages sent out the channel interface.
    Counter begins at zero with app initialization and rolls over. */
    uint32               SentMsgCount;

    /** \totlm Queued Channel Telemetry
    \brief Count of telemetry messages queued on the PQ channel's Priority Queues.
    Counter begins at zero with app initialization and rolls over. */
    uint32               QueuedMsgCount;

    /** \totlm Dropped Channel Telemetry
    \brief Count of telemetry messages rejected, and not passed into the channel's Priority Queues.
    Counter begins at zero with app initialization and rolls over. */
    uint32               DropMsgCount;

    /** \totlm Failed Channel Telemetry
    \brief Count of telemetry messages failed on interface write after being pulled from the output queue, and lost.
    Counter begins at zero with app initialization and rolls over. */
    uint32               FailedMsgCount;

    /** \totlm Channel Bytes Sent
    \brief The number of bytes sent out the channel's interface
    Counter begins at zero with app initialization and rolls over. */
    uint32               BytesSent;    

    /** \brief Struct for single output channel queue entry*/
    PQ_OutputQueue_t     OutputQueue;

    /**\brief  CF Throttling Semaphore ID*/
    uint32               CfCntSemId;

    /**\brief  CF Throttling Semaphore Max Count*/
    uint32               CfCntSemMax;
    
    /** \brief Channel index */
    uint32               channelIdx;

    /** \brief CF Throttling Semaphore Name*/
    char                 CfCntSemName[OS_MAX_API_NAME];

    /** \brief Config Table Name */
    char                 ConfigTableName[CFE_TBL_MAX_NAME_LENGTH]; 

    /** \brief Config Table File Name */
    char                 ConfigTableFileName[OS_MAX_PATH_LEN]; 

    /** \brief Dump Table Name */
    char                 DumpTableName[CFE_TBL_MAX_NAME_LENGTH]; 

    /** \brief Channel Name */
    char                 ChannelName[OS_MAX_API_NAME];

    /** \brief Data Pipe ID */
    CFE_SB_PipeId_t      DataPipeId;

    /** \brief To hold the value of PQ_ChannelState_t */
    uint8                State;

} PQ_ChannelData_t;

/* pq hk telemetry */

/**
 *  \brief Channel memory information.
 */
typedef struct
{
    /** \brief Total memory currently in use */
    uint32         MemInUse;
    
    /** \brief Peak channel memory usage */ 
    uint32         PeakMemInUse;

} PQ_ChannelMemoryInfo_t;

/**
**  \brief PQ housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \totlm Sent Channel Telemetry
    \totlmmnemonic PQ_RDOSENT
    \totlmmnemonic PQ_SNKLSENT
    \totlmmnemonic PQ_STORSENT
    \brief Count of telemetry messages sent out the channel interface.
    Counter begins at zero with app initialization and rolls over.
    Index of array is equivalent to the relevant PQ channel index. */
    uint32  uiSentMsgCountChannel;

    /** \totlm Queued Channel Telemetry
    \totlmmnemonic  PQ_RDOQUEUED
    \totlmmnemonic  PQ_SNKLQUEUED
    \totlmmnemonic  PQ_STORQUEUED
    \brief Count of telemetry messages queued on the PQ channel's Priority Queues.
    Counter begins at zero with app initialization and rolls over.
    Index of array is equivalent to the relevant PQ channel index. */
    uint32  uiQueuedMsgCountChannel;

    /** \totlm Dropped Channel Telemetry
    \totlmmnemonic PQ_RDODROP
    \totlmmnemonic PQ_SNKLDROP
    \totlmmnemonic PQ_STORDROP
    \brief Count of telemetry messages rejected, and not passed into the channel's Priority Queues.
    Counter begins at zero with app initialization and rolls over.
    Index of array is equivalent to the relevant PQ channel index. */
    uint32  uiDropMsgCountChannel;

    /** \totlm Failed Channel Telemetry
    \totlmmnemonic  PQ_RDOFAILED
    \totlmmnemonic  PQ_SNKLFAILED
    \totlmmnemonic  PQ_STORFAILED
    \brief Count of telemetry messages failed on interface write after being pulled 
    from the output queue, and lost.
    Counter begins at zero with app initialization and rolls over.
    Index of array is equivalent to the relevant PQ channel index. */
    uint32  uiFailedMsgCountChannel;

    /** \totlm Channel Bytes Sent
    \totlmmnemonic PQ_RDOB
    \totlmmnemonic PQ_SNKLB
    \totlmmnemonic PQ_STORB
    \brief The number of bytes sent out the channel's interface
    Counter begins at zero with app initialization and rolls over.
    Index of array is equivalent to the relevant PQ channel index. */
    uint32  uiBytesSentChannel;

    /** \totlmmnemonic PQ_CMDACPTCNT
        \brief Count of accepted commands */
    uint8   usCmdCnt;

    /** \totlmmnemonic PQ_CMDRJCTCNT
        \brief Count of failed commands */
    uint8   usCmdErrCnt;

    /** \totlmmnemonic PQ_TTLMSGDROP
        \brief Count of all messages dropped */
    uint16  usTotalMsgDropped;

    /** \totlmmnemonic PQ_SENTBYTES
        \brief Count of all bytes sent on all output channels */
    uint32  SentBytes;

    /** \totlmmnemonic PQ_QDINOUTCHN
        \brief Currently queued count for each Channel */
    uint16  QueuedInOutputChannel;

    /** \totlmmnemonic PQ_CHMAXMEM
        \brief Maximum memory available for each channel. */
    uint32  ChannelMaxMem;

    /** \totlmmnemonic PQ_CHMEMINFO
        \brief Memory information per channel. */
    PQ_ChannelMemoryInfo_t ChannelMemInfo;

    PQ_OutputQueue_t OutputQueue;

} PQ_HkTlm_t;


#endif
