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
    
#ifndef ASPD4525_EVENTS_H
#define ASPD4525_EVENTS_H

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
    ASPD4525_RESERVED_EID = 0,  /* Do not use this event ID */

/** \brief <tt> 'ASPD4525 - ' </tt>
**  \event <tt> 'ASPD4525 - ' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  TODO fill this in
**
*/
    ASPD4525_INF_EID,

/** \brief <tt> 'ASPD4525 Initialized. Version \%d.\%d.\%d' </tt>
**  \event <tt> 'ASPD4525 Initialized. Version \%d.\%d.\%d' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the CFS ASPD4525 Task has
**  completed initialization.
**
**  The first \c %d field contains the Application's Major Version Number
**  The second \c %d field contains the Application's Minor Version Number
**  The third \c %d field contains the Application's Revision Number
*/
    ASPD4525_INIT_INF_EID,

/** \brief <tt> 'ASPD4525 - ' </tt>
**  \event <tt> 'ASPD4525 - ' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This message is generated when a table is initialized.
**
*/
    ASPD4525_CONFIG_TABLE_INF_EID,

/** \brief <tt> 'ASPD4525 - ' </tt>
**  \event <tt> 'ASPD4525 - ' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when TBD
**
*/
    ASPD4525_CDS_INF_EID,

/** \brief <tt> 'ASPD4525 - Recvd $x cmd (%us)' </tt>
**  \event <tt> 'ASPD4525 - Recvd $x cmd (%us)' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the CFS ASPD4525 Task has
**  received and processed a command.
**
*/
    ASPD4525_CMD_INF_EID,

/** \brief <tt> 'ASPD4525 - ' </tt>
**  \event <tt> 'ASPD4525 - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when TBD
**
*/
    ASPD4525_ERR_EID,

/** \brief <tt> 'ASPD4525 - ' </tt>
**  \event <tt> 'ASPD4525 - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS ASPD4525 Task has
**  had an error in initialization.
**
*/
    ASPD4525_INIT_ERR_EID,

/** \brief <tt> 'ASPD4525 - ' </tt>
**  \event <tt> 'ASPD4525 - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS ASPD4525 Task has
**  had an error with the configuration table.
**
*/
    ASPD4525_CONFIG_TABLE_ERR_EID,

/** \brief <tt> 'ASPD4525 - ' </tt>
**  \event <tt> 'ASPD4525 - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS ASPD4525 Task has
**  had an error in CDS.
**
*/
    ASPD4525_CDS_ERR_EID,

/** \brief <tt> 'ASPD4525 - $commandError' </tt>
**  \event <tt> 'ASPD4525 - $commandError' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS ASPD4525 Task has
**  had an error processing a command.
**
*/
    ASPD4525_CMD_ERR_EID,

/** \brief <tt> 'ASPD4525: SB pipe read error (0x%08X), app will exit' </tt>
**  \event <tt> 'ASPD4525: SB pipe read error (0x%08X), app will exit' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS ASPD4525 Task has
**  had an error reading from a pipe.
**
*/
    ASPD4525_PIPE_ERR_EID,

/** \brief <tt> 'ASPD4525 - Recvd invalid $type msgId (0x%04x)' </tt>
**  \event <tt> 'ASPD4525 - Recvd invalid $type msgId (0x%04x)' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS ASPD4525 Task has
**  received an invalid message ID.
**
*/
    ASPD4525_MSGID_ERR_EID,

/** \brief <tt> 'ASPD4525 - Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, msgLen=%d, expectedLen=%d" </tt>
**  \event <tt> 'ASPD4525 - Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, msgLen=%d, expectedLen=%d" </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS ASPD4525 Task has
**  received a message with a bad length.
**
*/
    ASPD4525_MSGLEN_ERR_EID,


/** \brief <tt> This is a count of all the app events and should not be used. </tt> */
    ASPD4525_EVT_CNT
} ASPD4525_EventIds_t;


#ifdef __cplusplus
}
#endif

#endif /* ASPD4525_EVENTS_H */

/************************/
/*  End of File Comment */
/************************/
    
