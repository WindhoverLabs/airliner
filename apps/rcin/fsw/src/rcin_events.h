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

#ifndef RCIN_EVENTS_H
#define RCIN_EVENTS_H

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
    RCIN_RESERVED_EID                            = 0,  /* Do not use this event ID */

/** \brief <tt> 'Initialized. Version \%d.\%d.\%d.\%d' </tt>
**  \event <tt> 'Initialized. Version \%d.\%d.\%d.\%d' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the RCIN task has completed
**  initialization.
**
**  The first \c %d field contains the application major version defined
**      in #RCIN_MAJOR_VERSION.
**  The second \c %d field contains the application minor version defined
**      in #RCIN_MINOR_VERSION.
**  The third \c %d field contains the application revision number defined
**      in #RCIN_REVISION.
**  The fourth \c %d field contains the application revision number defined
**      in #RCIN_MISSION_REV.
*/
    RCIN_INIT_INF_EID                            = 1,

/** \brief <tt> 'Recvd NOOP. Version \%d.\%d.\%d.\%d' </tt>
**  \event <tt> 'Recvd NOOP. Version \%d.\%d.\%d.\%d' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the CFS RCIN Task receives a NoOp
**  command.
**
**  The first \c %u field contains the application major version defined
**      in #RCIN_MAJOR_VERSION.
**  The first \c %d field contains the application major version defined
**      in #RCIN_MAJOR_VERSION.
**  The second \c %d field contains the application minor version defined
**      in #RCIN_MINOR_VERSION.
**  The third \c %d field contains the application revision number defined
**      in #RCIN_REVISION.
**  The fourth \c %d field contains the application revision number defined
**      in #RCIN_MISSION_REV.
*/
    RCIN_CMD_NOOP_EID                            = 2,

    /** \brief <tt> '\%s Pipe failed to subscribe to \%s. (0x\%08X)' </tt>
    **  \event <tt> '\%s Pipe failed to subscribe to \%s. (0x\%08X)' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS RCIN Task fails to subscribe
**  to a message at initialization.
**
**  The first \c %s field contains the name of the pipe.
**  The second \c %s field contains the macro name of the message.
**  The 32 bit hexadecimal number is the error code returned by CFE.
**
*/
    RCIN_SUBSCRIBE_ERR_EID                       = 3,

/** \brief <tt> 'Failed to create \%s pipe (0x\%08X)' </tt>
**  \event <tt> 'Failed to create \%s pipe (0x\%08X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS RCIN Task fails to create a
**  CFE Software Bus pipe at initialization.
**
**  The \c %s field contains the name of the pipe.
**  The 32 bit hexadecimal number is the error code returned by CFE.
**
*/
    RCIN_PIPE_INIT_ERR_EID                       = 4,

/** \brief <tt> 'Failed to manage Config table (0x\%08X)' </tt>
**  \event <tt> 'Failed to manage Config table (0x\%08X)' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  The 32 bit hexadecimal number is the error code returned by CFE
**  #CFE_TBL_Manage function.
**
*/
    RCIN_CFGTBL_MANAGE_ERR_EID                   = 5,

/** \brief <tt> 'Failed to get Config table's address (0x\%08X)' </tt>
**  \event <tt> 'Failed to get Config table's address (0x\%08X)' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  The 32 bit hexadecimal number is the error code returned by CFE
**  #CFE_TBL_GetAddress function.
**
*/
    RCIN_CFGTBL_GETADDR_ERR_EID                  = 6,

/** \brief <tt> '\%s pipe read error (0x\%08X).' </tt>
**  \event <tt> '\%s pipe read error (0x\%08X).' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS RCIN Task has
**  had an error reading from a pipe.
**
**  The \c %s field contains the name of the pipe.
**  The 32 bit hexadecimal number is the error code returned by CFE.
**
*/
    RCIN_RCVMSG_ERR_EID                          = 7,

/** \brief <tt> 'Recvd invalid \%s msgId (0x\%04X)' </tt>
**  \event <tt> 'Recvd invalid \%s msgId (0x\%04X)' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS RCIN Task has received an invalid
**  message ID.
**
**  The \c %s field contains the name of the pipe.
**  The 16 bit hexadecimal number is the actual message ID received.
*/
    RCIN_MSGID_ERR_EID                           = 8,

/** \brief <tt> 'Recvd invalid command code (\%u)' </tt>
**  \event <tt> 'Recvd invalid command code (\%u)' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS RCIN Task has received an invalid
**  message ID.
**
**  The %u field contains the actual command code received.
*/
    RCIN_CC_ERR_EID                              = 9,

/** \brief <tt> 'Rcvd invalid msgLen: msgId=0x\%08X, cmdCode=\%d, msgLen=\%d, expectedLen=\%d" </tt>
**  \event <tt> 'Rcvd invalid msgLen: msgId=0x\%08X, cmdCode=\%d, msgLen=\%d, expectedLen=\%d" </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS RCINTask has
**  received a message with an invalid length.
**
**  The first \c %d field contains the message ID.
**  The second \c %d field contains the command code.
**  The third \c %d field contains the actual length.
**  The fourth \c %d field contains the expected length.
*/
    RCIN_MSGLEN_ERR_EID                          = 10,

/** \brief <tt> 'RCIN - Failed to init register cleanup callback (0x%08X)' </tt>
**  \event <tt> 'RCIN - Failed to init register cleanup callback (0x%08X)' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the RCIN has
**  had an error in initialization.
**
*/
    RCIN_INIT_ERR_EID                            = 11,

/** \brief <tt> 'RCIN is publishing fresh data' </tt>
**  \event <tt> 'RCIN is publishing fresh data' </tt>
**  
**  \par Type: INFO
**
**  \par Cause:
**
**  This event message is issued when RCIN is publishing fresh data or 
**  previously not publishing fresh data.
**
*/
    RCIN_PUBLISHING_INF_EID                      = 12,

/** \brief <tt> 'RCIN is NOT publishing fresh data' </tt>
**  \event <tt> 'RCIN is NOT publishing fresh data' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when RCIN is not publishing fresh data.
**
*/
    RCIN_NOT_PUBLISHING_ERR_EID                  = 13,

/** \brief <tt> This is a count of all the app events and should not be used. </tt> */
    RCIN_EVT_CNT

} RCIN_EventIds_t;


#ifdef __cplusplus
}
#endif

#endif /* RCIN_EVENTS_H */

/************************/
/*  End of File Comment */
/************************/
    
