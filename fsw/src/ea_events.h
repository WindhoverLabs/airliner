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

#ifndef EA_EVENTS_H
#define EA_EVENTS_H

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
    EA_RESERVED_EID = 0,  /* Do not use this event ID */

/** \brief <tt> 'EA - ' </tt>
**  \event <tt> 'EA - ' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is a generic information event
**
*/
    EA_INF_EID = 1,

/** \brief <tt> 'EA Initialized. Version \%d.\%d.\%d' </tt>
**  \event <tt> 'EA Initialized. Version \%d.\%d.\%d' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the CFS EA Task has
**  completed initialization.
**
**  The first \c %d field contains the Application's Major Version Number
**  The second \c %d field contains the Application's Minor Version Number
**  The third \c %d field contains the Application's Revision Number
*/
    EA_INIT_INF_EID = 2,

/** \brief <tt> 'EA - Recvd $x cmd (%us)' </tt>
**  \event <tt> 'EA - Recvd $x cmd (%us)' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the CFS EA Task has
**  received and processed a command.
**
*/
    EA_CMD_INF_EID = 3,

/** \brief <tt> 'External application started' </tt>
**  \event <tt> 'External application started' </tt>
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when an external app
**  has been started.
**
*/
    EA_INF_APP_START_EID = 4,

/** \brief <tt> 'External application terminated' </tt>
**  \event <tt> 'External application terminated' </tt>
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when an external app
**  has been manually stopped.
**
*/
    EA_INF_APP_TERM_EID = 5,

/** \brief <tt> 'Specified app does not exist' </tt> 
**  \event <tt> 'Specified app does not exist' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the specified
**  external app is unaccessible or does not exist
**
*/
    EA_APP_ERR_EID = 6,

/** \brief <tt> 'External application exceeded utilization threshold' </tt>
**  \event <tt> 'External application exceeded utilization threshold' </tt>
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the external app
**  has exceeded the utilization threshold specified in 
**  platform config
**
*/
    EA_WARN_APP_UTIL_EID = 7,

/** \brief <tt> 'EA - ' </tt>
**  \event <tt> 'EA - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when TBD
**
*/
    EA_ERR_EID = 8,

/** \brief <tt> 'EA - ' </tt>
**  \event <tt> 'EA - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS EA Task has
**  had an error in initialization.
**
*/
    EA_INIT_ERR_EID = 9,

/** \brief <tt> 'EA - $commandError' </tt>
**  \event <tt> 'EA - $commandError' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS EA Task has
**  had an error processing a command.
**
*/
    EA_CMD_ERR_EID = 10,

/** \brief <tt> 'EA: SB pipe read error (0x%08X), app will exit' </tt>
**  \event <tt> 'EA: SB pipe read error (0x%08X), app will exit' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS EA Task has
**  had an error reading from a pipe.
**
*/
    EA_PIPE_ERR_EID = 11,

/** \brief <tt> 'EA - Recvd invalid $type msgId (0x%04x)' </tt>
**  \event <tt> 'EA - Recvd invalid $type msgId (0x%04x)' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS EA Task has
**  received an invalid message ID.
**
*/
    EA_MSGID_ERR_EID = 12,

/** \brief <tt> 'EA - Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, msgLen=%d, expectedLen=%d" </tt>
**  \event <tt> 'EA - Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, msgLen=%d, expectedLen=%d" </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS EA Task has
**  received a message with a bad length.
**
*/
    EA_MSGLEN_ERR_EID = 13,

/** \brief <tt> 'Created child task for app start' </tt>
**  \event <tt> 'Created child task for app start' </tt>
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the EA_StartApp function has
**  successfully created a child task to execute the external app.
**
*/
    EA_CHILD_TASK_START_EID = 14,

/** \brief <tt> 'Create child tasked failed. Unable to start external application' </tt>
**  \event <tt> 'Create child tasked failed. Unable to start external application' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the EA_StartApp function has
**  failed to create a child task to execute the external app.
**
*/
    EA_CHILD_TASK_START_ERR_EID = 15,

/** \brief <tt> 'Recvd NOOP cmd (%u), Version %d.%d.%d.%d' </tt>
**  \event <tt> 'Recvd NOOP cmd (%u), Version %d.%d.%d.%d' </tt>
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when EA has performed a noop command.
**
*/
    EA_CMD_NOOP_EID = 16,

/** \brief <tt> 'External application exceeded utilization threshold' </tt>
**  \event <tt> 'External application exceeded utilization threshold' </tt>
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when EA has performed a reset command.
**
*/
    EA_CMD_RESET_EID = 17,

	/** \brief <tt> 'Recvd invalid cmd code (%u)' </tt>
	**  \event <tt> 'Recvd invalid cmd code (%u)' </tt>
	**
	**  \par Type: ERROR
	**
	**  \par Cause:
	**
	**  A command with an invalid command code was received.
	**
	*/
	EA_CC_ERR_EID = 18,

/** \brief <tt> This is a count of all the app events and should not be used. </tt> */
    EA_EVT_CNT
} EA_EventIds_t;


#ifdef __cplusplus
}
#endif

#endif /* EA_EVENTS_H */

/************************/
/*  End of File Comment */
/************************/
