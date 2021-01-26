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

#ifndef TO_MSG_H
#define TO_MSG_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "to_channel.h"
#include "to_priority_queue.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** TO Command Codes
*************************************************************************/

/** \tocmd Noop
**
**  \par Description
**       Implements the Noop command that demonstrates the TO task is alive
**
**  \tocmdmnemonic TO_NOOP
**
**  \par Command Structure
**       #TO_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c TO_CMDACPTCNT - command counter will increment
**       - The #TO_CMD_INF_EID informational event message will be
**         generated when the command is received
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c TO_CMDRJCTCNT - command error counter will increment
**
**  \par Criticality
**       None
**
**  \sa #TO_RESET_CC
*/
#define TO_NOOP_CC                           (0)

/** \tocmd Reset Counters
**
**  \par Description
**       Resets the to housekeeping counters
**
**  \tocmdmnemonic TO_TLMRST
**
**  \par Command Structure
**       #TO_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c TO_CMDACTPCNT       - command counter will be cleared
**       - \b \c TO_CMDRJCTCNT       - command error counter will be cleared
**       - The #TO_CMD_INF_EID debug event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c TO_CMDRJCTCNT - command error counter will increment
**
**  \par Criticality
**       None
**
**  \sa #TO_NOOP_CC
*/
#define TO_RESET_CC                          (1)

/** \tocmd Add message flow
**
**  \par Description
**       Command to add a message flow 
**
**  \tocmdmnemonic TO_ADDMSGFLW
**
**  \par Command Structure
**       #TO_AddMessageFlowCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c TO_CMDACTPCNT       - command counter will increment
**       - The #TO_CMD_ADD_MSG_FLOW_EID info event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**       - Channel is NULL
**       - Channel index is invalid
**       - Channel is not open
**       - Command inputs are invalid
**       - Table Pointer is not available
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c TO_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #TO_CMD_ADD_MSG_FLOW_ERR_EID
**       - Error specific event message #TO_MSG_FLOW_MISSING_TBL_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #TO_REMOVE_MESSAGE_FLOW_CC, TO_QUERY_MESSAGE_FLOW_CC 
*/
#define TO_ADD_MESSAGE_FLOW_CC               (2)

/** \tocmd Remove a message flow
**
**  \par Description
**       Command to remove a message flow 
**
**  \tocmdmnemonic TO_RMMSGFLW
**
**  \par Command Structure
**       #TO_RemoveMessageFlowCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c TO_CMDACTPCNT       - command counter will increment
**       - The #TO_CMD_REMOVE_MSG_FLOW_EID info event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**       - Channel is NULL
**       - Channel index is invalid
**       - Channel is not open
**       - Command inputs are invalid
**       - Table Pointer is not available
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c TO_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #TO_CMD_REMOVE_MSG_FLOW_ERR_EID
**       - Error specific event message #TO_MSG_FLOW_MISSING_TBL_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #TO_ADD_MESSAGE_FLOW_CC, TO_QUERY_MESSAGE_FLOW_CC 
*/
#define TO_REMOVE_MESSAGE_FLOW_CC            (3)

/** \tocmd Query a message flow for a channel
**
**  \par Description
**       Command to query a message flow for a channel 
**
**  \tocmdmnemonic TO_QRYMSGFLW
**
**  \par Command Structure
**       #TO_QueryMessageFlowCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c TO_CMDACTPCNT       - command counter will increment
**       - The #TO_MSG_FLOW_INFO_EID info event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**       - Channel is NULL
**       - Channel index is invalid
**       - Channel is not open
**       - Command inputs are invalid
**       - Message ID not found
**       - Table Pointer is not available
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c TO_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #TO_MSG_FLOW_INFO_ERR_EID
**       - Error specific event message #TO_MSG_FLOW_MISSING_TBL_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #TO_ADD_MESSAGE_FLOW_CC, TO_REMOVE_MESSAGE_FLOW_CC 
*/
#define TO_QUERY_MESSAGE_FLOW_CC             (4)

/** \tocmd Query a priority queue for a channel
**
**  \par Description
**       Command to query a priority queue for a channel 
**
**  \tocmdmnemonic TO_QRYPQ
**
**  \par Command Structure
**       #TO_QueryPriorityQueueCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c TO_CMDACTPCNT       - command counter will increment
**       - The #TO_PQUEUE_INFO_EID info event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**       - Channel is NULL
**       - Channel index is invalid
**       - Channel is not open
**       - Command inputs are invalid
**       - Priority queue index out of bounds for a channel
**       - Table Pointer is not available
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c TO_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #TO_PQUEUE_INFO_ERR_EID
**       - Error specific event message #TO_PQUEUE_MISSING_TBL_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #TO_QUERY_MESSAGE_FLOW_CC
*/
#define TO_QUERY_PRIORITY_QUEUE_CC           (5)

/** \tocmd Query an output channel
**
**  \par Description
**       Command to query an output channel 
**
**  \tocmdmnemonic TO_QRYOUTCHL
**
**  \par Command Structure
**       #TO_QueryOutputChannelCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c TO_CMDACTPCNT       - command counter will increment
**       - The #TO_OUT_CH_INFO_EID info event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**       - Channel is NULL
**       - Channel index is invalid
**       - Channel is not open
**       - Command inputs are invalid
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c \TO_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #TO_OUT_CH_INFO_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #TO_QUERY_PRIORITY_QUEUE_CC, TO_QUERY_MESSAGE_FLOW_CC
*/
#define TO_QUERY_OUTPUT_CHANNEL_CC           (6)

/** \tocmd Send diagnostic telemetry for a channel
**
**  \par Description
**       Command to send diagnostic telemetry for a channel 
**
**  \tocmdmnemonic TO_SNDDIAGCMD
**
**  \par Command Structure
**       #TO_SendDiagCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c TO_CMDACTPCNT       - command counter will increment
**       - The #TO_CMD_SEND_DIAG_EID info event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**       - Channel is NULL
**       - Channel index is invalid
**       - Channel is not open
**       - Table Pointer is not available
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c \TO_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #TO_CMD_SEND_DIAG_ERR_EID
**       - Error specific event message #TO_CMD_SEND_MISSING_TBL_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #TO_QUERY_PRIORITY_QUEUE_CC, TO_QUERY_MESSAGE_FLOW_CC
*/
#define TO_SEND_DIAG_CC                      (7)


/************************************************************************
** Local Structure Declarations
*************************************************************************/

/**
**  \brief No Arguments Command
**
**  \par
**   For command details see #TO_NOOP_CC, #TO_RESET_CC
**
*/
typedef struct
{
    /** \brief TC message headers */
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];

} TO_NoArgCmd_t;

/**
**  \brief Add Message Flow command.
*/
typedef struct
{
    /** \brief TC message headers */
    uint8          ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    
    /** \brief Index of the channel for which this command applies */
    uint16         ChannelIdx;   

    /** \brief The cFE SB Message ID of the message flow */
    CFE_SB_MsgId_t MsgID;

    /** \brief The MsgLimit defines the queue size in the Priority Queues structure */ 
    uint16         MsgLimit;
    
    /** \brief Index of the priority queue */
    uint16         PQueueIdx;

} TO_AddMessageFlowCmd_t;

/**
**  \brief Remove Message Flow command.
*/
typedef struct
{
    /** brief TC message headers */
    uint8          ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    
    /** \brief Index of the channel for which this command applies */
    uint16         ChannelIdx; 

    /** \brief The cFE SB Message ID of the message flow */
    CFE_SB_MsgId_t MsgID;   

} TO_RemoveMessageFlowCmd_t;

/**
**  \brief Query Message Flow command.
*/
typedef struct
{
    /** brief TC message headers */
    uint8          ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    
    /** \brief Index of the channel for which this command applies */
    uint16         ChannelIdx;   

    /** \brief The cFE SB Message ID of the message flow */
    CFE_SB_MsgId_t MsgID;

} TO_QueryMessageFlowCmd_t;

/**
**  \brief Query Priority Queue command.
*/
typedef struct
{
    /** brief TC message headers */
    uint8          ucCmdHeader[CFE_SB_CMD_HDR_SIZE];

    /** \brief Index of the channel for which this command applies */
    uint16         ChannelIdx;    

    /** \brief Index of the channel priority queue */
    uint16         PQueueIndex;       

} TO_QueryPriorityQueueCmd_t;

/**
**  \brief Query output channel command.
*/
typedef struct
{
    /** brief TC message headers */
    uint8          ucCmdHeader[CFE_SB_CMD_HDR_SIZE];

    /** \brief Index of the channel for which this command applies */
    uint16         ChannelIdx;

} TO_QueryOutputChannelCmd_t;

/**
**  \brief Diagnostic telemetry request command
*/
typedef struct
{
    /** brief TC message headers */
    uint8          ucCmdHeader[CFE_SB_CMD_HDR_SIZE];

    /** \brief Index of the channel for which this command applies */
    uint16         ChannelIdx;

} TO_SendDiagCmd_t;


/**
 *  \brief Channel memory information.
 */
typedef struct
{
    /** \brief Total memory currently in use */
    uint32         MemInUse;
    
    /** \brief Peak channel memory usage */ 
    uint32         PeakMemInUse;

} TO_ChannelMemoryInfo_t;


/**
**  \brief TO application housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \totlm Sent Channel Telemetry
    \totlmmnemonic TO_RDOSENT
    \totlmmnemonic TO_SNKLSENT
    \totlmmnemonic TO_STORSENT
    \brief Count of telemetry messages sent out the channel interface.
    Counter begins at zero with app initialization and rolls over.
    Index of array is equivalent to the relevant TO channel index. */
    uint32  uiSentMsgCountChannel[TO_MAX_CHANNELS];

    /** \totlm Queued Channel Telemetry
    \totlmmnemonic  TO_RDOQUEUED
    \totlmmnemonic  TO_SNKLQUEUED
    \totlmmnemonic  TO_STORQUEUED
    \brief Count of telemetry messages queued on the TO channel's Priority Queues.
    Counter begins at zero with app initialization and rolls over.
    Index of array is equivalent to the relevant TO channel index. */
    uint32  uiQueuedMsgCountChannel[TO_MAX_CHANNELS];

    /** \totlm Dropped Channel Telemetry
    \totlmmnemonic TO_RDODROP
    \totlmmnemonic TO_SNKLDROP
    \totlmmnemonic TO_STORDROP
    \brief Count of telemetry messages rejected, and not passed into the channel's Priority Queues.
    Counter begins at zero with app initialization and rolls over.
    Index of array is equivalent to the relevant TO channel index. */
    uint32  uiDropMsgCountChannel[TO_MAX_CHANNELS];

    /** \totlm Failed Channel Telemetry
    \totlmmnemonic  TO_RDOFAILED
    \totlmmnemonic  TO_SNKLFAILED
    \totlmmnemonic  TO_STORFAILED
    \brief Count of telemetry messages failed on interface write after being pulled 
    from the output queue, and lost.
    Counter begins at zero with app initialization and rolls over.
    Index of array is equivalent to the relevant TO channel index. */
    uint32  uiFailedMsgCountChannel[TO_MAX_CHANNELS];

    /** \totlm Channel Bytes Sent
    \totlmmnemonic TO_RDOB
    \totlmmnemonic TO_SNKLB
    \totlmmnemonic TO_STORB
    \brief The number of bytes sent out the channel's interface
    Counter begins at zero with app initialization and rolls over.
    Index of array is equivalent to the relevant TO channel index. */
    uint32  uiBytesSentChannel[TO_MAX_CHANNELS];

    /** \totlmmnemonic TO_CMDACPTCNT
        \brief Count of accepted commands */
    uint8   usCmdCnt;

    /** \totlmmnemonic TO_CMDRJCTCNT
        \brief Count of failed commands */
    uint8   usCmdErrCnt;

    /** \totlmmnemonic TO_TTLMSGDROP
        \brief Count of all messages dropped */
    uint16  usTotalMsgDropped;

    /** \totlmmnemonic TO_SENTBYTES
        \brief Count of all bytes sent on all output channels */
    uint32  SentBytes;

    /** \totlmmnemonic TO_QDINOUTCHN
        \brief Currently queued count for each Channel */
    uint16  QueuedInOutputChannel[TO_MAX_CHANNELS];

    /** \totlmmnemonic TO_CHMAXMEM
        \brief Maximum memory available for each channel. */
    uint32  ChannelMaxMem;

    /** \totlmmnemonic TO_CHMEMINFO
        \brief Memory information per channel. */
    TO_ChannelMemoryInfo_t ChannelMemInfo[TO_MAX_CHANNELS];

} TO_HkTlm_t;

/**
**  \brief TO Message Flow Diagnostics telemetry
*/
typedef struct
{
    /**
     * \brief  Message Flow Message Id
     */
    CFE_SB_MsgId_t          MsgId;

    /**
     * \brief  Message Flow Message Limit
     */
    uint16                  MsgLimit;

    /**
     * \brief  Message Flow Priority Queue Id
     */
    uint16                  PQueueID;

    /**
     * \brief  Message Flow Dropped Message Count
     */
    uint32                  DroppedMsgCnt;

    /**
     * \brief  Message Flow Queued Message Count
     */
    uint32                  QueuedMsgCnt;

} TO_MessageFlowDiagTlm_t;


/**
**  \brief TO Priority Queue Diagnostics telemetry
*/
typedef struct
{
    /**
     * \brief  Priority Queue dropped message Count
     */
    uint32                  DroppedMsgCnt;

    /**
     * \brief  Priority Queue queued message Count
     */
    uint32                  QueuedMsgCnt;

    /**
     * \brief  Priority Queue currently queued count
     */
    uint16                  CurrentlyQueuedCnt;

    /**
     * \brief  Priority Queue highest recorded queued count
     */
    uint16                  HighwaterMark;
    
    /**
     * \brief  Priority Queue message limit
     */
    uint16                  MsgLimit;    

    /**
     * \brief  Priority Queue current state
     */
    uint8                   State;

    /**
     * \brief  Priority Queue type (type of priority queue ie FIFO)
     */
    uint8                   QType;

} TO_PriorityDiagTlm_t;


/**
**  \brief TO Output Queue Diagnostics telemetry
*/
typedef struct
{
    /** \brief Count of the number of messages sent from the output queue */
    uint32                  SentCount;

    /** \brief Count of the number of messages currently queued on output queue */
    uint16                  CurrentlyQueuedCnt;

    /** \brief Highest recorded number of messages queued on output queue */
    uint16                  HighwaterMark;

    /** \brief Count of the number of bytes sent from the output queue */
    uint32                  SentBytes;

} TO_OutputQueueDiagTlm_t;


/**
**  \brief TO Channel Diagnostics telemetry
*/
typedef struct
{
    uint8                   ucTlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \brief Channel ID or Index */
    uint16                  ChannelIndex;

    /** \brief Memory pool handle */
    uint32                  MemPoolHandle;

    /** \brief Memory full count */
    uint32                  MemFullCount;
    
    /** \brief Channel Config Table Id */
    uint32                  TableID;    

    /** \brief Channel Priority Queue Diagnostics telemetry */
    TO_PriorityDiagTlm_t    PQueue[TO_MAX_PRIORITY_QUEUES];

    /** \brief Channel Output Queue Diagnostics telemetry */
    TO_OutputQueueDiagTlm_t OQueue;

    /** \brief Channel Config Table name */
    char                    ConfigTableName[CFE_TBL_MAX_NAME_LENGTH];

    /** \brief Channel Config Table File name */
    char                    ConfigTableFileName[OS_MAX_PATH_LEN];

    /** \brief Channel Dump Table name */
    char                    DumpTableName[CFE_TBL_MAX_NAME_LENGTH];

    /** \brief Channel name */
    char                    ChannelName[OS_MAX_API_NAME];

    /** \brief Channel State */
    uint8                   State;

} TO_ChannelDiagTlm_t;

/**
**  \brief TO diagnostic information for message flows.
**  Split from ChannelDiagTlm_t to accommodate smaller packet sizes
*/
typedef struct
{
    uint8                   ucTlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \brief Channel ID or Index */
    uint16                  ChannelIndex;

    /** \brief Offset to a set of message flow diagnostic data */
    uint32                  Offset;

    /** \brief Message flow diagnostic data */
    TO_MessageFlowDiagTlm_t MessageFlow[TO_MSG_FLOW_PKT_LIMIT];

} TO_MsgFlowListPkt_t;


#ifdef __cplusplus
}
#endif

#endif /* TO_MSG_H */

/************************/
/*  End of File Comment */
/************************/
