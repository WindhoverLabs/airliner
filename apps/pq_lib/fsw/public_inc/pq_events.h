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

#ifndef PQ_EVENTS_H
#define PQ_EVENTS_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Local Defines
*************************************************************************/


/* Event IDs
 * Conventions: _EID is the event identifier.  _EVT_CNT is the total number of
 * events and should always be last.  Events can be added before _EVT_CNT.
 * For long-term maintenance, consider not removing an event but replacing it
 * with an unused, reserved, enum to preserve the IDs later in the list. */
typedef enum {
    /*
     * START OF STANDARDIZED EVENT LIST
     *
     * The IDs for these events are based on a convention and should
     * be kept consistent between apps for ops support.  Events that
     * are unique to this app will follow this initial list.  Note,
     * for maintenance keep these values explicit to ensure the ID
     * convention.
     */

/** \brief PQ Init ERROR.
**  \event Application failed to initialize.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when a CFS Task has
**  had an error with initializing the app.
**
*/
    PQ_INIT_APP_ERR_EID      = 1 + PQ_EVENT_ID_OFFSET,

/** \brief PQ Init configuration table ERROR.
**  \event Failed to init config tables (0x%08X)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS Task has
**  had an error initializing a configuration table.
**
*/
    PQ_INIT_CONFIG_ERR_EID   = 2 + PQ_EVENT_ID_OFFSET,

/** \brief PQ CONFIG TABLE Address Error.
**  \event Failed to get Config table's address for channel \%u, (0x%08X)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS Task has
**  had an error with getting the configuration table address.
**
*/
    PQ_CONFIG_ADDR_ERR_EID   = 3 + PQ_EVENT_ID_OFFSET,

/** \brief PQ CONFIG TABLE Manage Error.
**  \event Failed to manage Config table for channel \%u, (0x%08X)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS Task has
**  had an error with managing the configuration table.
**
*/
    PQ_CONFIG_MANAGE_ERR_EID = 4 + PQ_EVENT_ID_OFFSET,

/** \brief PQ Configuration ERROR.
**  \event Channel \%u config table handle failure! (0x%08X)
**  \event Failed to clear SB data pipe for channel \%u, (0x%08X)
**  \event Failed to create '\%s' output channel queue for channel \%d. err=\%ld
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS Task has
**  had an error with the configuration table.
**
*/
    PQ_CONFIG_TABLE_ERR_EID  = 5 + PQ_EVENT_ID_OFFSET,

/** \brief Message Dropped on Priority Queue.
**  \event PQ full (PQ \%lu, channel \%u). Error code (\%ld) Dropped message 0x%04x
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event message is issued when one of the following errors is generated:
**
**    - PQ_PRIORITY_QUEUE_FULL_ERR (OS_QUEUE_FULL)
**    - PQ_MEMORY_FULL_ERR
**    - CFE_ES_ERR_MEM_HANDLE
**    - OS_ERR_INVALID_ID 
**    - OS_INVALID_POINTER
**    - OS_ERROR  are all handled by this else clause
**
*/
    PQ_MSG_DROP_FROM_FLOW_DBG_EID = 6 + PQ_EVENT_ID_OFFSET,

/** \brief PQ Create memory pool failed.
**  \event Error creating memory pool (0x%08X)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when PQ has failed to
**  create memory pool.
**
*/
    PQ_CR_POOL_ERR_EID        = 7 + PQ_EVENT_ID_OFFSET,

/** \brief Query a channel for a specified message flow.
**  \event CHANNEL=\%d MID=0x%04x ML=\%u PQI=\%u N=\%u AB=\%u D=\%lu Q=\%lu SB=\%lu FLT=\%lu FLD=\%lu S=\%lu
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the CFS Task has
**  received and processed a Query Message Flow command.
**  The following information is displayed:
**
**    - Channel Id
**    - Message Id
**    - Message Limit
**    - Priority Queue Id
**    - Filter value (decimation)
**    - Anomaly Queue Position
**    - Dropped Message Count
**    - Queued Message Count
**    - Software Bus Message Count
**    - Filtered Message Count
**    - Failed Message Count
**    - Sent Message Count
*/
    PQ_MSG_FLOW_INFO_EID = 8 + PQ_EVENT_ID_OFFSET,

/** \brief Failed query on channel for a specified message flow.
**  \event MsgID=0x%04x not found (channel = \%d)
**  \event Channel (\%d) not open.
**  \event Invalid channel (channel = \%d, but max = \%d).
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  PQ Command to query a message flow has failed due to the following:
**
**    - MsgID not found
**    - Channel not open
**    - Channel index not valid (out of range)
*/
    PQ_MSG_FLOW_INFO_ERR_EID = 9 + PQ_EVENT_ID_OFFSET,

/** \brief Failed query on a channel for a specified message with NULL table pointer
**  \event Channel missing valid table data (channel = \%d).
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  PQ Command to query a message flow has failed due to the following:
**
**    - Channel missing valid table data.
*/
    PQ_MSG_FLOW_MISSING_TBL_ERR_EID = 10 + PQ_EVENT_ID_OFFSET,

/** \brief Query a channel's priority queue.
**  \event CHANNEL=\%d PQI=\%u S=\%u ML=\%u QT=\%u D=\%lu Q=\%lu CQ=\%u HWM=\%u
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when PQ Command Priority Queue Query executed
**  successfully.
**
*/
    PQ_PQUEUE_INFO_EID = 11 + PQ_EVENT_ID_OFFSET,

/** \brief Failed query on a channel's priority queue.
**  \event PQueueIdx \%u exceeds the largest available priority queue index
**  \event Channel \%d not open.
**  \event Invalid channel index (index = \%d, max = \%d)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when PQ Command Priority Queue Query failed due to the following:
**
**    - Priority queue index exceeds largest available priority queue index
**    - Channel not open
**    - Channel index not valid (out of range)
**
*/
    PQ_PQUEUE_INFO_ERR_EID = 12 + PQ_EVENT_ID_OFFSET,

/** \brief Command to query an output queue.
**  \event CHANNEL=\%d S=\%d ML=\%i SC=\%lu CQC=\%d HWM=\%d
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the CFS Task has
**  received and processed a Query Output Queue command.
**  The following information is displayed:
**
**    - Channel Id
**    - Channel State
**    - Output Queue depth
**    - Sent count
**    - Currently queued count
**    - HighwaterMark
**
*/
    PQ_OUT_CH_INFO_EID = 13 + PQ_EVENT_ID_OFFSET,

/** \brief Command to query an output queue failed.
**  \event Invalid channel index (index = \%d, max = \%d).
**  \event Channel \%d not open.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when PQ Command Output Queue Query failed due to the following:
**
**    - Channel index not valid (out of range)
**    - Channel not open
**
*/
    PQ_OUT_CH_INFO_ERR_EID = 14 + PQ_EVENT_ID_OFFSET,

/** \brief Command to add a message flow has failed.
**  \event Invalid channel (channel = \%d, but max = \%d).
**  \event Channel (\%d) not open.
**  \event Message flow is already defined for channel \%d.
**  \event Priority Queue Index \%u is invalid for channel \%d.
**  \event Ground/Onboard Channel: Command inputs for storage location queues must be zero.
**  \event Ground/Onboard Channel: Command input for Filter Value must be at least 1.
**  \event Storage Channel: Command input for Filter Value must be 0.
**  \event Storage: Command inputs for storage location queues are zero - No message flow.
**  \event Message flow failed to subscribe to (0x%08X) on channel \%d. (\%ld)
**  \event Failed to add message flow. No unused slots available on channel \%d.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when PQ Command Add a message flow failed
**  for one of the following reasons:
**
**    - Channel index not valid (out of range)
**    - Channel not open
**    - Message flow already exists
**    - Requested priority queue is invalid
**    - Decimation filter value for the ground/onboard channel is invalid
**    - Ground/onboard channel has non-zero values in storage queue inputs
**    - FilterValue is non-zero for storage channel
**    - Storage channel queue decimation values are all zero - no message flow
**    - Message flow failed to subscribe
**    - No available slots to insert message flow > PQ_MAX_MESSAGE_FLOWS
*/
    PQ_CMD_ADD_MSG_FLOW_ERR_EID = 15 + PQ_EVENT_ID_OFFSET,

/** \brief Command to remove a message flow has failed.
**  \event Invalid channel (channel = \%d, but max = \%d).
**  \event Channel (\%d) not open.
**  \event Message flow is not defined for channel \%d.
**  \event Message flow failed to unsubscribe from 0x%08X on channel \%d. (\%ld)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when PQ Command Remove a message flow failed
**  for one of the following reasons:
**
**    - Channel index not valid (out of range)
**    - Channel not open
**    - Message flow does not exist on the channel
**    - Message flow failed to unsubscribe from channel
**
*/
    PQ_CMD_REMOVE_MSG_FLOW_ERR_EID = 16 + PQ_EVENT_ID_OFFSET,

/** \brief Failed to create priority queue because table data was missing - (per-channel event).
**  \event Failed to create priority queues on channel \%d, missing table.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message (per-channel event) is issued when PQ Command to query 
**  a priority queue has failed due to the following:
**
**    - Channel table pointer is NULL.
*/
    PQ_PQUEUE_MISSING_TBL_ERR_EID  = 17 + PQ_EVENT_ID_OFFSET,

/** \brief A priority queue could not be created because call to OS_QueueCreate failed (per-channel event).
**  \event Failed to create '\%s' priority queue #\%u on channel \%d. (\%i)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message (per-channel event) is issued when 
**  OS_QueueCreate returned one of the following errors:
** 
**  - OS_INVALID_POINTER if a pointer passed in is NULL
**  - OS_ERR_NAME_TOO_LONG if the name passed in is too long
**  - OS_ERR_NO_FREE_IDS if there are already the max queues created
**  - OS_ERR_NAME_TAKEN if the name is already being used on another queue
**  - OS_ERROR if the OS create call fails
**
**  A priority queue could not be created.  Not a critical error.
*/
    PQ_PQUEUE_CREATE_ERR_EID  = 18 + PQ_EVENT_ID_OFFSET,

/** \brief A priority queue could not be torn down (per-channel event).
**  \event Failed to return message back to memory pool on tbl load for channel \%d. (\%ld)
**  \event Message flow failed to pop all messages from pqueue \%u for channel \%d. (\%ld)
**  \event Failed to delete priority queue \%u for channel \%d. (\%ld)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  These event messages are issued when a priority queue could not be torn down for one of the following reasons:
**
**  - Failed to return message back to memory pool on table load for the channel - error returned from call to OS_QueueGet
**  - Message flow failed to pop all messages from priority queue for the channel - error returned from call to CFE_ES_PutPoolBuf
**  - Failed to delete priority queue for the channel - error returned from call to OS_QueueDelete
** 
*/
    PQ_PQUEUE_TEARDOWN_ERR_EID = 19 + PQ_EVENT_ID_OFFSET,

/** \brief PQ Table Validation failed - Configuration table pointer is NULL (not available).
**  \event ERROR: Config table pointer is NULL, table validation ended"
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issed when table validation failed due config table pointer being NULL (not available).
**
*/
    PQ_CONFIG_TABLE_NULL_PTR_EID = 20 + PQ_EVENT_ID_OFFSET,

/** \brief PQ Table Validation failed - Priority Queue State is invalid.
**  \event ERROR: Priority Queue State is invalid (\%d), for Table ID (\%lu) at table index (\%d)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issed when table validation failed due Priority Queue State not valid.
**
*/
    PQ_CONFIG_TABLE_PQUEUE_STATE_ERR_EID = 21 + PQ_EVENT_ID_OFFSET,

/** \brief PQ Table Validation failed - Priority Queue Type is invalid.
**  \event ERROR: Priority Queue Qtype is invalid (\%d) for Table ID (\%lu) at table index (\%d)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issed when table validation failed due to Priority Queue Type not valid. Only one type - FIFO.
**
*/
    PQ_CONFIG_TABLE_PQUEUE_QTYPE_ERR_EID = 22 + PQ_EVENT_ID_OFFSET,

/** \brief PQ Table Validation failed - Priority Queue Message Limit is invalid.
**  \event ERROR: Priority Queue MsgLimit invalid (\%d) for Table ID (\%lu) at table index (\%d)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  Table validation failed due to Priority Queue Message Limit because invalid.
**
*/
    PQ_CONFIG_TABLE_PQUEUE_MSG_LIMIT_ERR_EID = 23 + PQ_EVENT_ID_OFFSET,

/** \brief PQ Table Validation failed - no Priority Queues defined in the table.
**  \event ERROR: No Priority Queues or all queue states are unused in the config table with Table ID (\%lu)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issed when table validation failed due because no Priority Queues defined in the table.
**
*/
    PQ_CONFIG_TABLE_NO_PQUEUES_ERR_EID = 24 + PQ_EVENT_ID_OFFSET,

/** \brief PQ Table Validation failed - secondary header is absent.
**  \event ERROR: Secondary Header is absent for table index (\%d), MsgId (0x%04X) in Table ID (\%lu)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issed when table validation failed due to CCSDS secondary header error (absent)
**
*/
    PQ_CONFIG_TABLE_SHDR_ABSENT_EID = 25 + PQ_EVENT_ID_OFFSET,

/** \brief PQ Table Validation failed - Invalid CCSDS version
**  \event ERROR: Invalid CCSDS Version for table index (\%d), MsgId (0x%04X) in Table ID (\%lu)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issed when table validation failed due to CCSDS VERS error.
**
*/
    PQ_CONFIG_TABLE_CCSDS_VER_INVALID_EID = 26 + PQ_EVENT_ID_OFFSET,

/** \brief PQ Table Validation failed - configuration table Message Flow Limit is invalid.
**  \event ERROR: Message Flow MsgLimit invalid (\%u) in Table ID (\%lu) at table index (\%d) 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issed when table validation failed due to Message Flow Limit is out of range.
**
*/
    PQ_CONFIG_TABLE_MSG_FLOW_MSG_LIMIT_ERR_EID = 27 + PQ_EVENT_ID_OFFSET,

/** \brief PQ Table Validation failed - configuration table Message Flow Priority Queue ID is invalid.
**  \event ERROR: Message Flow Priority Queue ID is invalid (\%d) for Table ID (\%lu) at table index (\%d)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issed when table validation failed due to Message Flow Priority Queue ID being invalid.
**
*/
    PQ_CONFIG_TABLE_MSG_FLOW_PQ_ID_ERR_EID = 28 + PQ_EVENT_ID_OFFSET,

/** \brief PQ Table Validation failed - configuration table has no Message Flows defined.
**  \event INFO: No Message Flow entries in the config table, Table Id (\%lu)
**
**  \par Type: INFO
**
**  \par Cause:
**
**  This event message is issed when table validation failed due to no Message Flows defined in table.
**
*/
    PQ_CONFIG_TABLE_NO_MSG_FLOW_INF_EID  = 29 + PQ_EVENT_ID_OFFSET,

/** \brief Memory pool error - failed deallocating the memory allocated.
**  \event PutPoolBuf: channel=\%d error=\%i
**
**  \par Type: ERROR
**
**  \par Cause:
**
**   This event message is issued when PQ has failed to deallocate the memory allocated 
**   because CFE_ES_PutPoolBuf returned an error.
**
*/
    PQ_PUT_POOL_ERR_EID = 30 + PQ_EVENT_ID_OFFSET,

/** \brief Channel teardown failed - PQ channel cleanup failure.
**  \event Message Flow Teardown failure \%ld on channel \%u.
**  \event Priority Queue Teardown failure \%ld on channel \%u.
**  \event Output Queue Teardown failure \%ld on channel \%u.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  These event messages are issued when PQ channel cleanup fails for the following reasons:
**
**    - Message flow teardown fails
**    - Priority queue teardown fails
**    - Output queue teardown fails
**
*/
    PQ_CHANNEL_TEARDOWN_ERR_EID = 31 + PQ_EVENT_ID_OFFSET,

/** \brief Channel mutex lock failure - (per-channel event).' </tt
**  \event Channel lock mutex failure (\%u).
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message (per-channel event) is issued when PQ channel lock fails due to failure
**  returned by call to OS_MutSemTake.
**
*/
    PQ_CHANNEL_LOCK_MUTEX_ERR_EID = 32 + PQ_EVENT_ID_OFFSET,

/** \brief Channel mutex unlock failure - (per-channel event).
**  \event Channel unlock mutex failure (\%u).
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message (per-channel event) is issued when PQ channel unlock fails due to failure
**  returned by call to OS_MutSemGive.
**
*/
    PQ_CHANNEL_UNLOCK_MUTEX_ERR_EID = 33 + PQ_EVENT_ID_OFFSET,

/** \brief Process new configuration table error - (per-channel event).
**  \event Process New Config Table Failed for channel \%u, (0x%08X)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS Task has
**  had an error with processing the configuration table.
**
*/
    PQ_CONFIG_PROCESS_CONFIG_TBL_ERR_EID = 34 + PQ_EVENT_ID_OFFSET,

/** \brief PQ call to OS_QueuePut function failed when trying to queue message.
**  \event OS_QueuePut failed: channel=\%d size=\%u error=\%i.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when OS_QueuePut returns the following errors:
**
**  - OS_ERR_INVALID_ID if the id passed in does not exist
**  - OS_ERROR if the OS call to delete the queue fails 
**
*/
    PQ_OSQUEUE_PUT_ERROR_EID = 35 + PQ_EVENT_ID_OFFSET,

/** \brief PQ call to OS_QueueGet function failed when trying to dequeue message.
**  \event Failed to pop all messages from channel '\%s' (channel id = \%d) output queue. (\%ld).
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when OS_QueueGet returns the following errors:
**
**  - OS_ERR_INVALID_ID if the given ID does not exist
**  - OS_INVALID_POINTER if a pointer passed in is NULL
**  - OS_QUEUE_EMPTY if the Queue has no messages on it to be recieved
**  - OS_QUEUE_TIMEOUT if the timeout was OS_PEND and the time expired
**  - OS_QUEUE_INVALID_SIZE if the size passed in may be too small for the message
**
*/
    PQ_OSQUEUE_GET_ERROR_EID = 36 + PQ_EVENT_ID_OFFSET,

/** \brief Failed to load any of the PQ configuration tables - loading a backup tables - (per-channel event).
**  \event Loaded built-in emergency backup config table! - channel \%u
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message (per-channel event) is issued when PQ has loaded the built-in emergency backup 
**  configuration table because cFE TBL was not able to provide a table.
**
*/
    PQ_BACKUP_TABLE_INF_EID = 37 + PQ_EVENT_ID_OFFSET,
    
/** \brief PQ Classifier failed to process a telemetry message - message too long - (per-channel event).
**  \event Message too long (size = \%lu > max = \%d) for msgId = (0x%04X) on channel (\%u)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message (per-channel event) is issued when the CFS Task attempts to process a telemetry
**  message length that is larger than PQ_MAX_MSG_LENGTH.
**
*/    
    PQ_TLM_MSG_LEN_ERR_EID  = 38 + PQ_EVENT_ID_OFFSET,

/** \brief Classifier failed to retrieve a message flow - Message ID in the table - (per-channel event).
**  \event Classifier Recvd invalid msgId (0x%04X) or message flow was removed on channel (\%u)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message (per-channel event) is issued when the CFS Task attempts to retrieve a message flow
**  and the Message ID is not in the table.
**
*/    
    PQ_MF_MSG_ID_ERR_EID = 39 + PQ_EVENT_ID_OFFSET,

/** \brief Failed loading a PQ backup configuration table - (per-channel event).
**  \event Failed to load built-in emergency backup config table! - channel \%u
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message (per-channel event) is issued when CFE_TBL_Load failed to load the 
**  built-in emergency backup configuration table.
**
*/
    PQ_BACKUP_TABLE_ERR_EID = 40 + PQ_EVENT_ID_OFFSET
} PQ_EventIds_t;


#ifdef __cplusplus
}
#endif

#endif /* PQ_EVENTS_H */

/************************/
/*  End of File Comment */
/************************/
    
