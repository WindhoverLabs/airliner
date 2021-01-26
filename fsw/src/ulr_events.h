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

#ifndef ULR_EVENTS_H
#define ULR_EVENTS_H

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
    ULR_RESERVED_EID                             = 0,  /* Do not use this event ID */

/** \brief <tt> 'Initialized. Version \%d.\%d.\%d.\%d' </tt>
**  \event <tt> 'Initialized. Version \%d.\%d.\%d.\%d' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the ULR task has completed
**  initialization.
**
**  The first \c %d field contains the application major version defined
**      in #ULR_MAJOR_VERSION.
**  The second \c %d field contains the application minor version defined
**      in #ULR_MINOR_VERSION.
**  The third \c %d field contains the application revision number defined
**      in #ULR_REVISION.
**  The fourth \c %d field contains the application revision number defined
**      in #ULR_MISSION_REV.
*/
    ULR_INIT_INF_EID                             = 1,

/** \brief <tt> 'Recvd NOOP. Version \%d.\%d.\%d.\%d' </tt>
**  \event <tt> 'Recvd NOOP. Version \%d.\%d.\%d.\%d' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the CFS ULR Task receives a NoOp
**  command.
**
**  The first \c %u field contains the application major version defined
**      in #ULR_MAJOR_VERSION.
**  The first \c %d field contains the application major version defined
**      in #ULR_MAJOR_VERSION.
**  The second \c %d field contains the application minor version defined
**      in #ULR_MINOR_VERSION.
**  The third \c %d field contains the application revision number defined
**      in #ULR_REVISION.
**  The fourth \c %d field contains the application revision number defined
**      in #ULR_MISSION_REV.
*/
    ULR_CMD_NOOP_EID                             = 2,

    /** \brief <tt> '\%s Pipe failed to subscribe to \%s. (0x\%08X)' </tt>
    **  \event <tt> '\%s Pipe failed to subscribe to \%s. (0x\%08X)' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS ULR Task fails to subscribe
**  to a message at initialization.
**
**  The first \c %s field contains the name of the pipe.
**  The second \c %s field contains the macro name of the message.
**  The 32 bit hexadecimal number is the error code returned by CFE.
**
*/
    ULR_SUBSCRIBE_ERR_EID                        = 3,

/** \brief <tt> 'Failed to create \%s pipe (0x\%08X)' </tt>
**  \event <tt> 'Failed to create \%s pipe (0x\%08X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS ULR Task fails to create a
**  CFE Software Bus pipe at initialization.
**
**  The \c %s field contains the name of the pipe.
**  The 32 bit hexadecimal number is the error code returned by CFE.
**
*/
    ULR_PIPE_INIT_ERR_EID                        = 4,

/** \brief <tt> '\%s pipe read error (0x\%08X).' </tt>
**  \event <tt> '\%s pipe read error (0x\%08X).' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS ULR Task has
**  had an error reading from a pipe.
**
**  The \c %s field contains the name of the pipe.
**  The 32 bit hexadecimal number is the error code returned by CFE.
**
*/
    ULR_RCVMSG_ERR_EID                           = 5,

/** \brief <tt> 'Recvd invalid \%s msgId (0x\%04X)' </tt>
**  \event <tt> 'Recvd invalid \%s msgId (0x\%04X)' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS ULR Task has received an invalid
**  message ID.
**
**  The \c %s field contains the name of the pipe.
**  The 16 bit hexadecimal number is the actual message ID received.
*/
    ULR_MSGID_ERR_EID                            = 6,

/** \brief <tt> 'Recvd invalid command code (\%u)' </tt>
**  \event <tt> 'Recvd invalid command code (\%u)' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS ULR Task has received an invalid
**  message ID.
**
**  The %u field contains the actual command code received.
*/
    ULR_CC_ERR_EID                               = 7,

/** \brief <tt> 'Rcvd invalid msgLen: msgId=0x\%08X, cmdCode=\%d, msgLen=\%d, expectedLen=\%d" </tt>
**  \event <tt> 'Rcvd invalid msgLen: msgId=0x\%08X, cmdCode=\%d, msgLen=\%d, expectedLen=\%d" </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS ULRTask has
**  received a message with an invalid length.
**
**  The first \c %d field contains the message ID.
**  The second \c %d field contains the command code.
**  The third \c %d field contains the actual length.
**  The fourth \c %d field contains the expected length.
*/
    ULR_MSGLEN_ERR_EID                           = 8,

    /** \brief <tt> Platform specific message </tt>
    **  \event <tt> Platform specific message </tt>
    **
    **  \par Type: ERROR
    **
    **  \par Cause:
    **
    **  This event message is issued when the CustomInit call fails.  This is
    **  sent by the custom platform specific code so the exact text is
    **  specific to the platform.  See the platform developer for the exact
    **  meaning.
    **
    */
    ULR_CUSTOM_INIT_ERR_EID                      = 9,

    /** \brief <tt> 'Received 0x%02x before version ID.' </tt>
    **  \event <tt> 'Received 0x%02x before version ID.' </tt>
    **
    **  \par Type: ERROR
    **
    **  \par Cause:
    **  This event message is issued when the parser encounters data before
    **  the version ID is read.  This usually indicates that some of the
    **  message was either dropped or was injected with noise.  This should
    **  not latch or be a fatal error since the parser will just resync on
    **  the beginning of the next message.
    **
    **
    */
    ULR_UNEXPECTED_DATA_BEFORE_VERSION_ID        = 10,

    /** \brief <tt> 'Received 0x%02x before header.' </tt>
    **  \event <tt> 'Received 0x%02x before header.' </tt>
    **
    **  \par Type: ERROR
    **
    **  \par Cause:
    **  This event message is issued when the parser encounters data before
    **  the header is read.  This usually indicates that some of the
    **  message was either dropped or was injected with noise.  This should
    **  not latch or be a fatal error since the parser will just resync on
    **  the beginning of the next message.
    **
    */
    ULR_UNEXPECTED_DATA_BEFORE_HEADER            = 11,

    /** \brief <tt> 'UartMessage.Checksum %hhu != checksum %hhu\n' </tt>
    **  \event <tt> 'UartMessage.Checksum %hhu != checksum %hhu\n' </tt>
    **
    **  \par Type: ERROR
    **
    **  \par Cause:
    **  This event message is issued when the parser calculates a checksum
    **  that does not match the expected checksum reported by the device.
    **  This should not latch or be a fatal error since the parser will just
    **  resync on the beginning of the next message.
    **
    */
    ULR_INVALID_CHECKSUM                         = 12,

    /** \brief <tt> 'Parser in invalid state.' </tt>
    **  \event <tt> 'Parser in invalid state.' </tt>
    **
    **  \par Type: ERROR
    **
    **  \par Cause:
    **  This event message is issued when the parser is in an unknown state.
    **  This should not happen and could indicate memory corruption.
    **
    */
    ULR_INVALID_PARSER_STATE                     = 13,

    /** \brief <tt> 'Listener child task failed.  CFE_ES_CreateChildTask returned: 0x%08lX' </tt>
    **  \event <tt> 'Listener child task failed.  CFE_ES_CreateChildTask returned: 0x%08lX' </tt>
    **
    **  \par Type: ERROR
    **
    **  \par Cause:
    **  This event message is issued when the parent task fails to create the
    **  child task.  This should not happen and could indicate memory
    **  corruption or insufficient available resources.
    **
    */
    ULR_LISTENER_CREATE_CHDTASK_ERR_EID          = 14,

/** \brief <tt> This is a count of all the app events and should not be used. </tt> */
    ULR_EVT_CNT
} ULR_EventIds_t;


#ifdef __cplusplus
}
#endif

#endif /* ULR_EVENTS_H */

/************************/
/*  End of File Comment */
/************************/
    
