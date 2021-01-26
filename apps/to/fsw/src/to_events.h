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

#ifndef TO_EVENTS_H
#define TO_EVENTS_H

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

/** \brief Value of zero is reserved, and should not be used. */
    TO_RESERVED_EID = 0,  /* Do not use this event ID */

    /*
     * START OF STANDARDIZED EVENT LIST
     *
     * The IDs for these events are based on a convention and should
     * be kept consistent between apps for ops support.  Events that
     * are unique to this app will follow this initial list.  Note,
     * for maintenance keep these values explicit to ensure the ID
     * convention.
     */

/** \brief TO Init App INFO.
**  \event Initialized.  Version \%d.\%d.\%d.\%d
**
**  \par Type: INFORMATION
*
**  \par Cause:
**
**  This event message is issued when the CFS TO Task has
**  completed initialization.
**
**  The first \c \b " %d " field contains the Application's Major Version Number
**  The second \c \b " %d " field contains the Application's Minor Version Number
**  The third \c \b " %d " field contains the Application's Revision Number
*/
    TO_INIT_APP_INF_EID      = 1,

/** \brief TO Configuration Table Initialized.
**  \event Config tables are initialized.
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This message is generated when a table is initialized.
**
*/
    TO_INIT_CONFIG_INF_EID   = 2,

/** \brief TO NOOP CMD INFO.
**  \event Executed NOOP cmd (\%u), Version \%d.\%d.\%d.\%d
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the CFS TO Task has
**  received and processed a Noop command.  Also, the
**  application version number is displayed.
*/
    TO_CMD_NOOP_INF_EID      = 3,

/** \brief TO RESET CMD INFO.
**  \event Executed RESET cmd (\%u)
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the CFS TO Task has
**  received and processed a Reset command.
**
*/
    TO_CMD_RESET_INF_EID     = 4,

/** \brief TO - 
**  \event TO - 
**
**  \par Type:
**
**  \par Cause:
**
**  This event message is reserved.
**  Reserved for common id - TO_SUBSCRIBE_INF_EID
**
*/
    TO_RESERVED_ONE_EID      = 5,

/** \brief TO Init App ERROR.
**  \event Application failed to initialize.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS TO Task has
**  had an error with initializing the app.
**
*/
    TO_INIT_APP_ERR_EID      = 6,

/** \brief TO - 
**  \event TO - 
**
**  \par Type:
**
**  \par Cause:
**
**  This event message is reserved.
**  Reserved for common id - TO_INIT_CLEANUP_ERR_EID
**
*/
    TO_RESERVED_TWO_EID      = 7,

/** \brief TO Init CMD Pipe ERROR.
**  \event Failed to create CMD pipe (0x%08X)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS TO Task has
**  had an error initializing the command pipe.
**
*/
    TO_INIT_CMDPIPE_ERR_EID  = 8,

/** \brief TO Init configuration table ERROR.
**  \event Failed to init config tables (0x%08X)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS TO Task has
**  had an error initializing a configuration table.
**
*/
    TO_INIT_CONFIG_ERR_EID   = 9,

/** \brief TO - 
**  \event TO - 
**
**  \par Type:
**
**  \par Cause:
**
**  This event message is reserved.
**  Reserved for common id - TO_INIT_DATA_ERR_EID
**
*/
    TO_RESERVED_THREE_EID    = 10,

/** \brief TO Init data pipe ERROR.
**  \event Failed to init config tables (0x%08X)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS TO Task has
**  had an error initializing the data pipe
**
*/
    TO_INIT_DATAPIPE_ERR_EID = 11,

/** \brief TO Init pipe ERROR.
**  \event Failed to init pipes (0x%08X)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS TO Task has
**  has at least 1 error initializing a pipe.
**
*/
    TO_INIT_PIPE_ERR_EID     = 12,

/** \brief TO Init SCH pipe ERROR.
**  \event Failed to create SCH pipe (0x%08X)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS TO Task has
**  had an error initializing the scheduler pipe
**
*/
    TO_INIT_SCHPIPE_ERR_EID  = 13,

/** \brief TO Command Code Error.
**  \event Recvd invalid cmdId (\%u)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS TO Task has
**  received a command with an invalid command code
**
*/
    TO_CC_ERR_EID            = 14,

/** \brief TO CONFIG TABLE Address Error.
**  \event Failed to get Config table's address for channel \%u, (0x%08X)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS TO Task has
**  had an error with getting the configuration table address.
**
*/
    TO_CONFIG_ADDR_ERR_EID   = 15,

/** \brief TO CONFIG TABLE Manage Error.
**  \event Failed to manage Config table for channel \%u, (0x%08X)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS TO Task has
**  had an error with managing the configuration table.
**
*/
    TO_CONFIG_MANAGE_ERR_EID = 16,

/** \brief TO received invalid message ID.
**  \event Recvd invalid $type msgId (0x%04x)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS TO Task has
**  received an invalid message ID.
**
*/
    TO_MSG_ID_ERR_EID        = 17,

/** \brief TO received a message with a invalid length
**  \event Rcvd invalid msgLen: msgId=0x%08X, cmdCode=\%d, msgLen=\%d, expectedLen=\%d"
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS TO Task has
**  received a message with a bad length.
**
*/
    TO_MSG_LEN_ERR_EID       = 18,

/** \brief TO Software Bus Pipe ERROR.
**  \event SB pipe read error (0x%08X)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS TO Task has
**  had an error reading from a pipe.
**
*/
    TO_PIPE_READ_ERR_EID     = 19,

/** \brief TO Subscribe ERROR.
**  \event CMD Pipe failed to subscribe to TO_CMD_MID. (0x%08X)
**  \event Sch Pipe failed to subscribe to TO_SEND_HK_MID. (0x%08X)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS TO Task has
**  had an error subscribing to messages.
**
*/
    TO_SUBSCRIBE_ERR_EID     = 20,

/** \brief TO Configuration ERROR.
**  \event Channel \%u config table handle failure! (0x%08X)
**  \event Failed to clear SB data pipe for channel \%u, (0x%08X)
**  \event Failed to create '\%s' output channel queue for channel \%d. err=\%ld
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS TO Task has
**  had an error with the configuration table.
**
*/
    TO_CONFIG_TABLE_ERR_EID  = 21,

/** \brief TO received and processed a command.
**  \event Enabled channel \%u to \%s:\%u.
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the CFS TO Task has
**  received and processed a command.
**
*/
    TO_CMD_INF_EID           = 22,

/** \brief TO Unsubscribe ERROR.
**  \event Message flow failed to unsubscribe from 0x%04x. (\%ld)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS TO Task has
**  had an error subscribing to messages.
**
*/
    TO_UNSUBSCRIBE_ERR_EID   = 23,

/** \brief TO Create child task failed.
**  \event Child task creation failed: Failed to create \%s: 0x%08lX
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when TO has failed to
**  create its child task.
**
*/
    TO_CREATE_CHDTASK_ERR_EID = 24,

/** \brief Message Dropped on Priority Queue.
**  \event PQ full (PQ \%lu, channel \%u). Error code (\%ld) Dropped message 0x%04x
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event message is issued when one of the following errors is generated:
**
**    - TO_PRIORITY_QUEUE_FULL_ERR (OS_QUEUE_FULL)
**    - TO_MEMORY_FULL_ERR
**    - CFE_ES_ERR_MEM_HANDLE
**    - OS_ERR_INVALID_ID 
**    - OS_INVALID_POINTER
**    - OS_ERROR  are all handled by this else clause
**
*/
    TO_MSG_DROP_FROM_FLOW_DBG_EID = 25,

/** \brief TO Create memory pool failed.
**  \event Error creating memory pool (0x%08X)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when TO has failed to
**  create memory pool.
**
*/
    TO_CR_POOL_ERR_EID        = 26,

    /* Deleted "TO_GET_POOL_ERR_EID  = 27" */

/** \brief Listener failed to write.
**  \event Listener passed NULL POINTER to TO_OutputChannel_Send().
**  \event Not all bytes written to storage locations.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when TO has failed to
**  write due to NULL pointer or not all bytes written.
**
*/
    TO_TLM_LISTEN_ERR_EID     = 28,

/** \brief Query a channel for a specified message flow.
**  \event CHANNEL=\%d MID=0x%04x ML=\%u PQI=\%u N=\%u AB=\%u D=\%lu Q=\%lu SB=\%lu FLT=\%lu FLD=\%lu S=\%lu
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the CFS TO Task has
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
    TO_MSG_FLOW_INFO_EID = 29,

/** \brief Failed query on channel for a specified message flow.
**  \event MsgID=0x%04x not found (channel = \%d)
**  \event Channel (\%d) not open.
**  \event Invalid channel (channel = \%d, but max = \%d).
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  TO Command to query a message flow has failed due to the following:
**
**    - MsgID not found
**    - Channel not open
**    - Channel index not valid (out of range)
*/
    TO_MSG_FLOW_INFO_ERR_EID = 30,

/** \brief Failed query on a channel for a specified message with NULL table pointer
**  \event Channel missing valid table data (channel = \%d).
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  TO Command to query a message flow has failed due to the following:
**
**    - Channel missing valid table data.
*/
    TO_MSG_FLOW_MISSING_TBL_ERR_EID = 31,

/** \brief Query a channel's priority queue.
**  \event CHANNEL=\%d PQI=\%u S=\%u ML=\%u QT=\%u D=\%lu Q=\%lu CQ=\%u HWM=\%u
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when TO Command Priority Queue Query executed
**  successfully.
**
*/
    TO_PQUEUE_INFO_EID = 32,

/** \brief Failed query on a channel's priority queue.
**  \event PQueueIdx \%u exceeds the largest available priority queue index
**  \event Channel \%d not open.
**  \event Invalid channel index (index = \%d, max = \%d)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when TO Command Priority Queue Query failed due to the following:
**
**    - Priority queue index exceeds largest available priority queue index
**    - Channel not open
**    - Channel index not valid (out of range)
**
*/
    TO_PQUEUE_INFO_ERR_EID = 33,    

/** \brief Command to query an output queue.
**  \event CHANNEL=\%d S=\%d ML=\%i SC=\%lu CQC=\%d HWM=\%d
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the CFS TO Task has
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
    TO_OUT_CH_INFO_EID = 34,

/** \brief Command to query an output queue failed.
**  \event Invalid channel index (index = \%d, max = \%d).
**  \event Channel \%d not open.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when TO Command Output Queue Query failed due to the following:
**
**    - Channel index not valid (out of range)
**    - Channel not open
**
*/
    TO_OUT_CH_INFO_ERR_EID = 35,    

/** \brief Failed to send out telemetry message, message too large.
**  \event Listener: message too big, MID=0x\%x, bytes=\%u
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when TO failed to send out telemetry message, message too large.
**
*/
    TO_OUT_CH_MSG_TOO_BIG_EID = 36,

/** \brief Command to add a message flow executed successfully.
**  \event Executed ADD_MESSAGE cmd (\%u)
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when TO Command Add a message flow executed successfully.
**
*/
    TO_CMD_ADD_MSG_FLOW_EID = 37,
    
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
**  This event message is issued when TO Command Add a message flow failed
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
**    - No available slots to insert message flow > TO_MAX_MESSAGE_FLOWS
*/
    TO_CMD_ADD_MSG_FLOW_ERR_EID = 38,    

/** \brief Command to remove a message flow executed successfully.
**  \event Executed remove message flow cmd (\%u)
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when TO command remove a message flow executed successfully.
**
*/
    TO_CMD_REMOVE_MSG_FLOW_EID = 39,

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
**  This event message is issued when TO Command Remove a message flow failed
**  for one of the following reasons:
**
**    - Channel index not valid (out of range)
**    - Channel not open
**    - Message flow does not exist on the channel
**    - Message flow failed to unsubscribe from channel
**
*/
    TO_CMD_REMOVE_MSG_FLOW_ERR_EID = 40,    

/** \brief Command to send TO Diagnostics telemetry executed successfully.
**  \event Successfully executed send diagnostics (\%u)
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when TO command to Send TO Diagnostics telemetry executed successfully.
**
*/
    TO_CMD_SEND_DIAG_EID = 41,

/** \brief Command to send TO Diagnostics telemetry failed - (per-channel event).
**  \event Invalid channel index.
**  \event Channel has not been opened.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message (per-channel event) is issued when TO Send TO Diagnostics telemetry failed
**  for one of the following reasons:
**
**    - Channel index not valid (out of range)
**    - Channel not open
*/
    TO_CMD_SEND_DIAG_ERR_EID = 42,

/** \brief Command to send diagnostics action did not occur because table data was missing - (per-channel event) 
**  \event Channel table is not available.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message (per-channel event) is issued when TO Command to sent 
**  TO diagnostic telemetry has failed due to the following:
**
**    - Channel table pointer is NULL.
*/
    TO_CMD_SEND_MISSING_TBL_ERR_EID = 43,

/** \brief Failed to create priority queue because table data was missing - (per-channel event).
**  \event Failed to create priority queues on channel \%d, missing table.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message (per-channel event) is issued when TO Command to query 
**  a priority queue has failed due to the following:
**
**    - Channel table pointer is NULL.
*/
    TO_PQUEUE_MISSING_TBL_ERR_EID  = 44,

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
    TO_PQUEUE_CREATE_ERR_EID  = 45,

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
    TO_PQUEUE_TEARDOWN_ERR_EID = 46,

/** \brief The Storage queue anomaly buffer value is not in valid range.
**  \event TO custom setting anomaly queue index error (\%u) using default anomaly buffer (10) index 6
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issed when TO custom setting anomaly queue index is not in range.  It must be
**  greater than TO_MIN_ANOMALY_QUEUE and less than TO_MAX_ANOMALY_QUEUE
**
*/
    TO_DECIMATE_SET_QUEUE_BIT_ERR_EID = 47,

/** \brief TO Table Validation failed - Configuration table pointer is NULL (not available).
**  \event ERROR: Config table pointer is NULL, table validation ended"
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issed when table validation failed due config table pointer being NULL (not available).
**
*/
    TO_CONFIG_TABLE_NULL_PTR_EID = 48,

/** \brief TO Table Validation failed - Priority Queue State is invalid.
**  \event ERROR: Priority Queue State is invalid (\%d), for Table ID (\%lu) at table index (\%d)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issed when table validation failed due Priority Queue State not valid.
**
*/
    TO_CONFIG_TABLE_PQUEUE_STATE_ERR_EID = 49,

/** \brief TO Table Validation failed - Priority Queue Type is invalid.
**  \event ERROR: Priority Queue Qtype is invalid (\%d) for Table ID (\%lu) at table index (\%d)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issed when table validation failed due to Priority Queue Type not valid. Only one type - FIFO.
**
*/
    TO_CONFIG_TABLE_PQUEUE_QTYPE_ERR_EID = 50,

/** \brief TO Table Validation failed - Priority Queue Message Limit is invalid.
**  \event ERROR: Priority Queue MsgLimit invalid (\%d) for Table ID (\%lu) at table index (\%d)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  Table validation failed due to Priority Queue Message Limit because invalid.
**
*/
    TO_CONFIG_TABLE_PQUEUE_MSG_LIMIT_ERR_EID = 51,

/** \brief TO Table Validation failed - no Priority Queues defined in the table.
**  \event ERROR: No Priority Queues or all queue states are unused in the config table with Table ID (\%lu)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issed when table validation failed due because no Priority Queues defined in the table.
**
*/
    TO_CONFIG_TABLE_NO_PQUEUES_ERR_EID = 52,

/** \brief TO Table Validation failed - secondary header is absent.
**  \event ERROR: Secondary Header is absent for table index (\%d), MsgId (0x%04X) in Table ID (\%lu)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issed when table validation failed due to CCSDS secondary header error (absent)
**
*/
    TO_CONFIG_TABLE_SHDR_ABSENT_EID = 53,

/** \brief TO Table Validation failed - Invalid CCSDS version
**  \event ERROR: Invalid CCSDS Version for table index (\%d), MsgId (0x%04X) in Table ID (\%lu)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issed when table validation failed due to CCSDS VERS error.
**
*/
    TO_CONFIG_TABLE_CCSDS_VER_INVALID_EID = 54,

/** \brief TO Table Validation failed - configuration table Message Flow Limit is invalid.
**  \event ERROR: Message Flow MsgLimit invalid (\%u) in Table ID (\%lu) at table index (\%d) 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issed when table validation failed due to Message Flow Limit is out of range.
**
*/
    TO_CONFIG_TABLE_MSG_FLOW_MSG_LIMIT_ERR_EID = 55,

/** \brief TO Table Validation failed - configuration table Message Flow Priority Queue ID is invalid.
**  \event ERROR: Message Flow Priority Queue ID is invalid (\%d) for Table ID (\%lu) at table index (\%d)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issed when table validation failed due to Message Flow Priority Queue ID being invalid.
**
*/
    TO_CONFIG_TABLE_MSG_FLOW_PQ_ID_ERR_EID = 56,

/** \brief TO Table Validation failed - configuration table has no Message Flows defined.
**  \event INFO: No Message Flow entries in the config table, Table Id (\%lu)
**
**  \par Type: INFO
**
**  \par Cause:
**
**  This event message is issed when table validation failed due to no Message Flows defined in table.
**
*/
    TO_CONFIG_TABLE_NO_MSG_FLOW_INF_EID  = 57,

/** \brief TO Table Validation failed - configuration table Message Flow Decimation Filter value is invalid.
**  \event ERROR: Storage Filter value is invalid must be 0 value is (\%u) for Table ID (\%lu) at table index (\%d)
**  \event ERROR: Ground or Onboard Filter value is invalid must at least 1, value is (\%d) for Table ID (\%lu) at table index (\%d)"
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  These event messages are issed when table validation failed due to Message Flow Decimation Filter value being invalid.
**  For Storage channel the filter value must be 0 and for the Ground and Onboard channels the filter value must be non-zero.
**
*/
    TO_CONFIG_TABLE_MSG_FLOW_FILTER_ERR_EID = 58,

/** \brief TO Table Validation failed - configuration table storage anomaly queue values are invalid for Ground and Onboard Channels.
**  \event ERROR: Queue value is invalid must be 0 value is (\%u) for Table ID (\%lu) at table index (\%d)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issed when table validation failed due to storage anomaly queue values being non-zero for Ground and Onboard Channels.
**
*/
    TO_CONFIG_TABLE_QUEUE_ERR_EID = 59,

/** \brief Memory pool error - failed deallocating the memory allocated.
**  \event PutPoolBuf: channel=\%d error=\%i
**
**  \par Type: ERROR
**
**  \par Cause:
**
**   This event message is issued when TO has failed to deallocate the memory allocated 
**   because CFE_ES_PutPoolBuf returned an error.
**
*/
    TO_PUT_POOL_ERR_EID = 60,

/** \brief Command to flush a TO channel failed due invalid channel identifier.
**  \event ChannelID \%u out of range.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when TO flush command failed for following reasons.
**
**    - Channel index not valid (out of range)
**
*/
    TO_FLUSH_INVALID_CHIDX_ERR_EID = 61,

/** \brief Command to flush a TO channel failed on output queue.
**  \event ChannelID \%u flush failure on output queue.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when TO flush command failed for following reasons.
**
**    - Output Queue teardown failure
**
*/
    TO_FLUSH_OQ_ERR_EID = 62,

/** \brief Command to flush a TO channel failed on priority queue wipe all.
**  \event ChannelID \%u flush failure on priority queue.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when TO flush command failed for following reasons.
**
**    - Priority queue wipe all failure
**
*/
    TO_FLUSH_PQ_ERR_EID = 63,

/** \brief Command to flush a TO channel failed due to a software bus dequeue failure.
**  \event ChannelID \%u software bus dequeue failure.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when TO flush command failed for following reasons.
**
**   - Software bus dequeue failure
**
*/
    TO_FLUSH_DEQUEUE_ERR_EID = 64,

/** \brief Command to flush a TO channel executed successfully.
**  \event ChannelID \%u flush successful.
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when TO Channel flush command executed successfully.
**
*/
    TO_FLUSH_SUCCESS_INF_EID = 65,

/** \brief Command to modify a message flow's decimation values executed successfully.
**  \event Executed Modify Decimation cmd (\%u)
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when TO Command to modify 
**  message flow decimation scheme executed successfully.
**
*/
    TO_CMD_MOD_DECIM_MSG_EID = 66,

/** \brief Command to modify a message flow's decimation values has failed.
**  \event Invalid channel (channel = \%d, but max = \%d).
**  \event Channel not open.
**  \event Message flow is not defined.
**  \event Ground/Onboard Channel: Command inputs for storage location queues must be zero.
**  \event Ground/Onboard Channel: Command input for Filter Value must be at least 1.
**  \event Storage Channel: Command input for Filter Value must be 0.
**  \event Storage: Command inputs for storage location queues are zero - Message flow not changed.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  These event messages are issued when TO Command Remove a message flow failed
**  for one of the following reasons:
**
**    - Channel index not valid (out of range)
**    - Channel not open
**    - Message flow does not exist on the channel
**    - Ground/Onboard channel command inputs for storage location queues are non-zero
**    - Ground/Onboard channel command input for filter value is less than one
**    - Storage channel command input for filter value is non-zero
**    - Storage channel command inputs for storage location queues are zero
**
*/
    TO_CMD_MOD_DECIM_MSG_ERR_EID = 67,

/** \brief Command to change anomaly buffer location executed successfully.
**  \event Updated Anomaly Queue to (\%u) from (\%u)
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when TO Command to change anomaly buffer location executed successfully.
**
*/
    TO_CMD_UPDATE_ANOMALY_BUFFER_EID = 68,

/** \brief Command to change anomaly buffer location failed due to invalid input not in range.
**  \event Invalid anomaly queue must be a value from (4 to 15) (input = \%d).
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when TO Command to change anomaly buffer location for following reason.
**
**    - Anomaly buffer queue location (out of range)
**
*/
    TO_CMD_UPDATE_ANOMALY_BUFFER_ERR_EID = 69,

/** \brief Command to update throttling rate executed successfully.
**  \event Throttling: Channel \%u Write Rate to (\%lu) bytes/second from (\%lu) bytes/second
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when TO Command to update throttling rate executed successfully.
**
*/
    TO_CMD_UPDATE_CHANNEL_THROTTLING_EID = 70,

/** \brief Command to update throttling rate failed due to rate not changed.
**  \event Throttling: Channel \%u Write Rate to (\%lu) bytes/second from (\%lu) bytes/second"
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event message is issued when TO Command to update the throttling rate was not
**  successful because the new rate was the same as the current rate.
**
*/
    TO_CMD_UPDATE_CHANNEL_THROTTLING_DBG_EID = 71,
    
/** \brief Command to update throttling rate failed due to rate not changed.
**  \event Invalid channel index (index = \%u, but max = \%d).
**  \event Invalid Channel (input = \%u). Only Ground and Onboard channels are valid for this command
**  \event Channel (\%u) not open.
**  \event ConfigTblPtr is NULL, command rejected
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  These event message are issued when TO Command to update the throttling rate failed for the following reasons:
**
**  - Channel index not valid
**  - Channel input was the Storage channel
**  - Channel not being open
**  - Channel with a NULL table pointer
**
*/
    TO_CMD_UPDATE_CHANNEL_THROTTLING_ERR_EID = 72,

/** \brief Wipe priority queue memory pool error - failed to return message back to memory pool.
**  \event Failed to return message back to memory pool on flush command. (\%ld)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when there is a failure to return message back to memory pool on flush command.
**
*/
    TO_WIPE_PQ_MEMPOOL_ERR_EID  = 73,

/** \brief Failed to pop all messages from channel when attempting to wipe all channel priority queues.
**  \event Failed to pop all messages from channel '\%s' priority queue '\%u'. (\%ld)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when there is a failure to pop all messages from channel.
**
*/
    TO_WIPE_PQ_NO_POP_ERR_EID = 74,

/** \brief Channel closed error, failure to process telemetry.
**  \event Failed to process telemetry from ChannelID \%u
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the channel closed, can not process telemetry.
**
*/
    TO_CHANNEL_CLOSED_ERR_EID = 75,

/** \brief Channel identifier value is out of range.
**  \event ChannelID (\%u) invalid.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when channel index is out of range.
**
*/
    TO_CHANNEL_OUT_OF_RANGE_ERR_EID = 76,

/** \brief Channel is not enabled.
**  \event ChannelID \%u not enabled.
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event message is issued when channel is not enabled
**
*/
    TO_CHANNEL_NOT_ENABLED_EID = 77,

/** \brief Channel teardown failed - TO channel cleanup failure.
**  \event Message Flow Teardown failure \%ld on channel \%u.
**  \event Priority Queue Teardown failure \%ld on channel \%u.
**  \event Output Queue Teardown failure \%ld on channel \%u.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  These event messages are issued when TO channel cleanup fails for the following reasons:
**
**    - Message flow teardown fails
**    - Priority queue teardown fails
**    - Output queue teardown fails
**
*/
    TO_CHANNEL_TEARDOWN_ERR_EID = 78,

/** \brief Channel mutex lock failure - (per-channel event).' </tt
**  \event Channel lock mutex failure (\%u).
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message (per-channel event) is issued when TO channel lock fails due to failure
**  returned by call to OS_MutSemTake.
**
*/
    TO_CHANNEL_LOCK_MUTEX_ERR_EID = 79,

/** \brief Channel mutex unlock failure - (per-channel event).
**  \event Channel unlock mutex failure (\%u).
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message (per-channel event) is issued when TO channel unlock fails due to failure
**  returned by call to OS_MutSemGive.
**
*/
    TO_CHANNEL_UNLOCK_MUTEX_ERR_EID = 80,
    
/** \brief Software Bus Read Error.
**  \event Main loop error: SB receive: result = 0x%08X
**
**  \par Type: CRITICAL
**
**  \par Cause:
**
**  This event message is issued by the TO main thread when the application
**  receives an error reading from the Software Bus
**
*/
    TO_SB_RECEIVE_ERR_EID = 81,
    
/** \brief Process new configuration table error - (per-channel event).
**  \event Process New Config Table Failed for channel \%u, (0x%08X)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS TO Task has
**  had an error with processing the configuration table.
**
*/
    TO_CONFIG_PROCESS_CONFIG_TBL_ERR_EID = 82,

/** \brief TO call to OS_QueuePut function failed when trying to queue message.
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
    TO_OSQUEUE_PUT_ERROR_EID = 83,

/** \brief TO call to OS_QueueGet function failed when trying to dequeue message.
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
    TO_OSQUEUE_GET_ERROR_EID = 84,

/** \brief Custom Init Event failure - Custom EID filters not set.
**  \event Custom Init Event - Event table index out of range. Custom EID filters not set.
**  \event Custom Init Event - Event table occupied at starting index. Custom EID filters not set.
**  \event Custom Init Event - Filter limit reached. Some custom EID filters not set.
**  \event Custom Init Event - Unknown error.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when TO has failed to set custom layer EID filters in its event table
**  for the following reasons:
**
**  - TO_CUSTOM_INITEVENT_BAD_IND_ERR
**  - TO_CUSTOM_INITEVENT_INDEX_OCCUPIED
**  - TO_CUSTOM_INITEVENT_TOO_MANY_FILTERS
**
*/
    TO_EVT_CUSTOM_FILTERS_ERR_EID = 85,

/** \brief Init Data Error - Failed to Create TO AppData Mutex.
**  \event Init Data: Failed to create TO_AppData mutex: 0x%08lX
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when TO has failed to Create TO AppData Mutex
**
*/
    TO_CREATE_APPDATA_MUTEX_ERR_EID = 86,

/** \brief Failed to load any of the TO configuration tables - loading a backup tables - (per-channel event).
**  \event Loaded built-in emergency backup config table! - channel \%u
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message (per-channel event) is issued when TO has loaded the built-in emergency backup 
**  configuration table because cFE TBL was not able to provide a table.
**
*/
    TO_BACKUP_TABLE_INF_EID = 87,
    
/** \brief TO Classifier failed to process a telemetry message - message too long - (per-channel event).
**  \event Message too long (size = \%lu > max = \%d) for msgId = (0x%04X) on channel (\%u)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message (per-channel event) is issued when the CFS TO Task attempts to process a telemetry
**  message length that is larger than TO_MAX_MSG_LENGTH.
**
*/    
    TO_TLM_MSG_LEN_ERR_EID  = 88,

/** \brief Classifier failed to retrieve a message flow - Message ID in the table - (per-channel event).
**  \event Classifier Recvd invalid msgId (0x%04X) or message flow was removed on channel (\%u)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message (per-channel event) is issued when the CFS TO Task attempts to retrieve a message flow
**  and the Message ID is not in the table.
**
*/    
    TO_MF_MSG_ID_ERR_EID = 89,

/** \brief Failed loading a TO backup configuration table - (per-channel event).
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
    TO_BACKUP_TABLE_ERR_EID = 90,       

/** \brief This is a count of all the app events and should not be used. */
    TO_EVT_CNT
} TO_EventIds_t;


#ifdef __cplusplus
}
#endif

#endif /* TO_EVENTS_H */

/************************/
/*  End of File Comment */
/************************/
    
