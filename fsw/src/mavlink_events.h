    
#ifndef MAVLINK_EVENTS_H
#define MAVLINK_EVENTS_H

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

/** \brief <tt> Value of zero is reserved, and should not be used. </tt> */
    MAVLINK_RESERVED_EID = 0,  /* Do not use this event ID */

/** \brief <tt> 'MAVLINK - ' </tt>
**  \event <tt> 'MAVLINK - ' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  TODO fill this in
**
*/
    MAVLINK_INF_EID,

/** \brief <tt> 'MAVLINK Initialized. Version \%d.\%d.\%d' </tt>
**  \event <tt> 'MAVLINK Initialized. Version \%d.\%d.\%d' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the CFS MAVLINK Task has
**  completed initialization.
**
**  The first \c %d field contains the Application's Major Version Number
**  The second \c %d field contains the Application's Minor Version Number
**  The third \c %d field contains the Application's Revision Number
*/
    MAVLINK_INIT_INF_EID,

/** \brief <tt> 'MAVLINK - ' </tt>
**  \event <tt> 'MAVLINK - ' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This message is generated when a table is initialized.
**
*/
    MAVLINK_CONFIG_TABLE_INF_EID,

/** \brief <tt> 'MAVLINK - ' </tt>
**  \event <tt> 'MAVLINK - ' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when TBD
**
*/
    MAVLINK_CDS_INF_EID,

/** \brief <tt> 'MAVLINK - Recvd $x cmd (%us)' </tt>
**  \event <tt> 'MAVLINK - Recvd $x cmd (%us)' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the CFS MAVLINK Task has
**  received and processed a command.
**
*/
    MAVLINK_CMD_INF_EID,

/** \brief <tt> 'MAVLINK - ' </tt>
**  \event <tt> 'MAVLINK - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when TBD
**
*/
    MAVLINK_ERR_EID,

/** \brief <tt> 'MAVLINK - ' </tt>
**  \event <tt> 'MAVLINK - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS MAVLINK Task has
**  had an error in initialization.
**
*/
    MAVLINK_INIT_ERR_EID,

/** \brief <tt> 'MAVLINK - ' </tt>
**  \event <tt> 'MAVLINK - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS MAVLINK Task has
**  had an error with the configuration table.
**
*/
    MAVLINK_PARAM_TABLE_ERR_EID,

/** \brief <tt> 'MAVLINK - ' </tt>
**  \event <tt> 'MAVLINK - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS MAVLINK Task has
**  had an error in CDS.
**
*/
    MAVLINK_CDS_ERR_EID,

/** \brief <tt> 'MAVLINK - $commandError' </tt>
**  \event <tt> 'MAVLINK - $commandError' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS MAVLINK Task has
**  had an error processing a command.
**
*/
    MAVLINK_CMD_ERR_EID,

/** \brief <tt> 'MAVLINK: SB pipe read error (0x%08X), app will exit' </tt>
**  \event <tt> 'MAVLINK: SB pipe read error (0x%08X), app will exit' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS MAVLINK Task has
**  had an error reading from a pipe.
**
*/
    MAVLINK_PIPE_ERR_EID,

/** \brief <tt> 'MAVLINK - Recvd invalid $type msgId (0x%04x)' </tt>
**  \event <tt> 'MAVLINK - Recvd invalid $type msgId (0x%04x)' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS MAVLINK Task has
**  received an invalid message ID.
**
*/
    MAVLINK_MSGID_ERR_EID,

/** \brief <tt> 'MAVLINK - Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, msgLen=%d, expectedLen=%d" </tt>
**  \event <tt> 'MAVLINK - Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, msgLen=%d, expectedLen=%d" </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS MAVLINK Task has
**  received a message with a bad length.
**
*/
    MAVLINK_MSGLEN_ERR_EID,

	// TODO
	MAVLINK_SOCKET_ERR_EID,

	//TODO
	MAVLINK_ENA_INF_EID,

	MAVLINK_LISTENER_CREATE_CHDTASK_ERR_EID,

	MAVLINK_HANDLE_INF_EID,

	MAVLINK_HANDLE_ERR_EID,


/** \brief <tt> This is a count of all the app events and should not be used. </tt> */
    MAVLINK_EVT_CNT
} MAVLINK_EventIds_t;


#ifdef __cplusplus
}
#endif

#endif /* MAVLINK_EVENTS_H */

/************************/
/*  End of File Comment */
/************************/
    
