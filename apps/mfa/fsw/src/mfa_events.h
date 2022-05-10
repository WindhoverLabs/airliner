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
*    from this software without speMFAfic prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INMFADENTAL, SPEMFAAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/

#ifndef MFA_EVENTS_H
#define MFA_EVENTS_H

#ifdef __cplusplus
extern "C" {

#endif
#define MFA_MSG_MAX_SIZE (100)
#define MFA_INITIALIZED_EID  (1)
#define MFA_HELLO_WORLD_EID  (2)
#define MFA_INIT_ERR_EID     (3)
/************************************************************************
** Local Defines
*************************************************************************/


/* Event IDs
 * Conventions: _EID is the event identifier.  _EVT_CNT is the total number of
 * events and should always be last.  Events can be added before _EVT_CNT.
 * For long-term maintenance, consider not removing an event but replaMFAng it
 * with an unused, reserved, enum to preserve the IDs later in the list. */
//typedef enum {
//
///** \brief <tt> Value of zero is reserved, and should not be used. </tt> */
//    MFA_RESERVED_EID = 0,  /* Do not use this event ID */
//
///** \brief <tt> 'MFA Initialized. Version \%d.\%d.\%d' </tt>
//**  \event <tt> 'MFA Initialized. Version \%d.\%d.\%d' </tt>
//**
//**  \par Type: INFORMATION
//**
//**  \par Cause:
//**
//**  This event message is issued when the CFS MFA Task has
//**  completed initialization.
//**
//**  The first \c %d field contains the Application's Major Version Number
//**  The second \c %d field contains the Application's Minor Version Number
//**  The third \c %d field contains the Application's Revision Number
//*/
//    MFA_INIT_INF_EID = 1,
//
///** \brief <tt> 'MFA - ' </tt>
//**  \event <tt> 'MFA - ' </tt>
//**
//**  \par Type: INFORMATION
//**
//**  \par Cause:
//**
//**  This message is generated when a table is initialized.
//**
//*/
//    MFA_CONFIG_TABLE_INF_EID = 2,
//
///** \brief <tt> 'MFA - Recvd $x cmd (%us)' </tt>
//**  \event <tt> 'MFA - Recvd $x cmd (%us)' </tt>
//**
//**  \par Type: INFORMATION
//**
//**  \par Cause:
//**
//**  This event message is issued when the CFS MFA Task has
//**  received and processed a command.
//**
//*/
//    MFA_CMD_INF_EID = 3,
//
///** \brief <tt> 'MFA - ' </tt>
//**  \event <tt> 'MFA - ' </tt>
//**
//**  \par Type: ERROR
//**
//**  \par Cause:
//**
//**  This event message is issued when TBD
//**
//*/
//    MFA_ERR_EID = 4,
//
///** \brief <tt> 'MFA - ' </tt>
//**  \event <tt> 'MFA - ' </tt>
//**
//**  \par Type: ERROR
//**
//**  \par Cause:
//**
//**  This event message is issued when the CFS MFA Task has
//**  had an error in initialization.
//**
//*/
//    MFA_INIT_ERR_EID = 5,
//
///** \brief <tt> 'MFA - ' </tt>
//**  \event <tt> 'MFA - ' </tt>
//**
//**  \par Type: ERROR
//**
//**  \par Cause:
//**
//**  This event message is issued when the CFS MFA Task has
//**  had an error with the configuration table.
//**
//*/
//    MFA_CONFIG_TABLE_ERR_EID = 6,
//
///** \brief <tt> 'MFA - ' </tt>
//**  \event <tt> 'MFA - ' </tt>
//**
//**  \par Type: ERROR
//**
//**  \par Cause:
//**
//**  This event message is issued when the CFS MFA Task has
//**  had an error with the timeout table.
//**
//*/
//	MFA_TIMEOUT_TABLE_ERR_EID = 7,
//
///** \brief <tt> 'MFA - $commandError' </tt>
//**  \event <tt> 'MFA - $commandError' </tt>
//**
//**  \par Type: ERROR
//**
//**  \par Cause:
//**
//**  This event message is issued when the CFS MFA Task has
//**  had an error processing a command.
//**
//*/
//    MFA_CMD_ERR_EID = 8,
//
///** \brief <tt> 'MFA: SB pipe read error (0x%08X), app will exit' </tt>
//**  \event <tt> 'MFA: SB pipe read error (0x%08X), app will exit' </tt>
//**
//**  \par Type: ERROR
//**
//**  \par Cause:
//**
//**  This event message is issued when the CFS MFA Task has
//**  had an error reading from a pipe.
//**
//*/
//    MFA_PIPE_ERR_EID = 9,
//
///** \brief <tt> 'MFA - Recvd invalid $type msgId (0x%04x)' </tt>
//**  \event <tt> 'MFA - Recvd invalid $type msgId (0x%04x)' </tt>
//**
//**  \par Type: ERROR
//**
//**  \par Cause:
//**
//**  This event message is issued when the CFS MFA Task has
//**  received an invalid message ID.
//**
//*/
//    MFA_MSGID_ERR_EID = 10,
//
///** \brief <tt> 'MFA - Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, msgLen=%d, expectedLen=%d" </tt>
//**  \event <tt> 'MFA - Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, msgLen=%d, expectedLen=%d" </tt>
//**
//**  \par Type: ERROR
//**
//**  \par Cause:
//**
//**  This event message is issued when the CFS MFA Task has
//**  received a message with a bad length.
//**
//*/
//    MFA_MSGLEN_ERR_EID = 11,
//
///** \brief <tt> 'MFA - L%d, cmd %0x %0x dropped, too long' </tt>
//**  \event <tt> 'MFA - L%d, cmd %0x %0x dropped, too long' </tt>
//**
//**  \par Type: ERROR
//**
//**  \par Cause:
//**
//**  This event message is issued when MFA
//**  received a msg greater than the max allowed size.
//**
//*/
//    MFA_CMD_INGEST_ERR_EID = 13,
//
///** \brief <tt> 'MFA - UDP command input enabled on port %u.' </tt>
//**  \event <tt> 'MFA - UDP command input enabled on port %u.' </tt>
//**
//**  \par Type: INFORMATION
//**
//**  \par Cause:
//**
//**  This event message is issued when the MFA listener
//**  successfully binds to a socket.
//**
//*/
//	MFA_ENA_INF_EID = 15,
//
///** \brief <tt> 'MFA - Rcvd invalid cmd' </tt>
//**  \event <tt> 'MFA - Rcvd invalid cmd' </tt>
//**
//**  \par Type: ERROR
//**
//**  \par Cause:
//**
//**  This event message is issued when the CFS MFA Task has
//**  encountered an issue with a received command.
//**
//*/
//	MFA_CMD_INVALID_EID = 16,
//
///** \brief <tt> 'MFA - Cmd not authorized' </tt>
//**  \event <tt> 'MFA - Cmd not authorized' </tt>
//**
//**  \par Type: ERROR
//**
//**  \par Cause:
//**
//**  This event message is issued when the command MFA has
//**  received is not authorized to execute.
//**
//*/
//	MFA_CMD_UNAUTHORIZED_EID = 17,
//
///** \brief <tt> 'MFA - Cmd authorization timeout' </tt>
//**  \event <tt> 'MFA - Cmd authorization timeout' </tt>
//**
//**  \par Type: INFORMATION
//**
//**  \par Cause:
//**
//**  This event message is issued when an authorized
//**  command has timed out.
//**
//*/
//	MFA_CMD_AUTH_TIMEOUT_EID = 18,
//
///** \brief <tt> 'MFA - Cmd not registered' </tt>
//**  \event <tt> 'MFA - Cmd not registered' </tt>
//**
//**  \par Type: ERROR
//**
//**  \par Cause:
//**
//**  This event message is issued when attempting to
//**  authorize a command that is not registered.
//**
//*/
//	MFA_CMD_AUTH_NOT_REG_EID = 19,
//
///** \brief <tt> 'MFA - Cmd not 2-step' </tt>
//**  \event <tt> 'MFA - Cmd not 2-step' </tt>
//**
//**  \par Type: ERROR
//**
//**  \par Cause:
//**
//**  This event message is issued when attempting to
//**  authorize a command that is not 2-step.
//**
//*/
//	MFA_CMD_AUTH_INV_MODE_EID = 20,
//
///** \brief <tt> 'MFA - Cmd already authorized' </tt>
//**  \event <tt> 'MFA - Cmd already authorized' </tt>
//**
//**  \par Type: ERROR
//**
//**  \par Cause:
//**
//**  This event message is issued when attempting to
//**  authorize a command that is already authorized.
//**
//*/
//	MFA_CMD_AUTH_INV_STATE_EID = 21,
//
///** \brief <tt> 'MFA - Cmd authorized' </tt>
//**  \event <tt> 'MFA - Cmd authorized' </tt>
//**
//**  \par Type: INFORMATION
//**
//**  \par Cause:
//**
//**  This event message is issued when a command is authorized.
//**
//*/
//	MFA_CMD_AUTHORIZED_EID = 22,
//
///** \brief <tt> 'MFA - Cmd not registered' </tt>
//**  \event <tt> 'MFA - Cmd not registered' </tt>
//**
//**  \par Type: ERROR
//**
//**  \par Cause:
//**
//**  This event message is issued when a command
//**  is not registered.
//**
//*/
//	MFA_CMD_DEAUTH_NOT_REG_EID = 23,
//
///** \brief <tt> 'MFA - Cmd not 2-step' </tt>
//**  \event <tt> 'MFA - Cmd not 2-step' </tt>
//**
//**  \par Type: ERROR
//**
//**  \par Cause:
//**
//**  This event message is issued when attempting to
//**  deauthorize a command that is not 2-step.
//**
//*/
//	MFA_CMD_DEAUTH_INV_MODE_EID = 24,
//
///** \brief <tt> 'MFA - Cmd not authorized' </tt>
//**  \event <tt> 'MFA - Cmd not authorized' </tt>
//**
//**  \par Type: ERROR
//**
//**  \par Cause:
//**
//**  This event message is issued when attempting to
//**  deauthorize a command that is not authorized.
//**
//*/
//	MFA_CMD_DEAUTH_INV_STATE_EID = 25,
//
///** \brief <tt> 'MFA - Cmd deauthorized' </tt>
//**  \event <tt> 'MFA - Cmd deauthorized' </tt>
//**
//**  \par Type: INFORMATION
//**
//**  \par Cause:
//**
//**  This event message is issued when a command is deauthorized.
//**
//*/
//	MFA_CMD_DEAUTHORIZED_EID = 26,
//
///** \brief <tt> 'MFA - Cmd registered' </tt>
//**  \event <tt> 'MFA - Cmd registered' </tt>
//**
//**  \par Type: INFORMATION
//**
//**  \par Cause:
//**
//**  This event message is issued when a command is registered.
//**
//*/
//	MFA_CMD_REGISTERED_EID = 27,
//
///** \brief <tt> 'MFA - Cmd already registered' </tt>
//**  \event <tt> 'MFA - Cmd already registered' </tt>
//**
//**  \par Type: ERROR
//**
//**  \par Cause:
//**
//**  This event message is issued when attempting to register
//**  a command that is already registered.
//**
//*/
//	MFA_CMD_ALREADY_REGISTERED_EID = 28,
//
///** \brief <tt> 'MFA - Cmd deregistered' </tt>
//**  \event <tt> 'MFA - Cmd deregistered' </tt>
//**
//**  \par Type: INFORMATION
//**
//**  \par Cause:
//**
//**  This event message is issued when a command is deregistered.
//**
//*/
//	MFA_CMD_DEREGISTERED_EID = 29,
//
///** \brief <tt> 'MFA - Cmd not registered' </tt>
//**  \event <tt> 'MFA - Cmd not registered' </tt>
//**
//**  \par Type: ERROR
//**
//**  \par Cause:
//**
//**  This event message is issued when attempting to deregister
//**  a command that is not registered.
//**
//*/
//	MFA_CMD_NOT_REGISTERED_EID = 30,
//
///** \brief <tt> 'MFA - Unable to register cmd' </tt>
//**  \event <tt> 'MFA - Unable to register cmd' </tt>
//**
//**  \par Type: ERROR
//**
//**  \par Cause:
//**
//**  This event message is issued when attempting to register
//**  a command and an error is encountered.
//**
//*/
//	MFA_CMD_REG_ERR_EID = 31,
//
///** \brief <tt> 'MFA - Invalid state of command to update' </tt>
//**  \event <tt> 'MFA - Invalid state of command to update' </tt>
//**
//**  \par Type: ERROR
//**
//**  \par Cause:
//**
//**  This event message is issued when attempting to update
//**  a 2-step registered command while it is authorized
//*/
//	MFA_CMD_UPDT_REG_INVLD_STATE_EID = 32,
//
///** \brief <tt> 'MFA - Cmd updated' </tt>
//**  \event <tt> 'MFA - Cmd updated' </tt>
//**
//**  \par Type: INFORMATION
//**
//**  \par Cause:
//**
//**  This event message is issued when a command registration is updated.
//**
//*/
//	MFA_CMD_UPDATE_REG_EID = 33,
//
//	MFA_CMD_VALIDATION_FAIL_EID = 34,
//
//	MFA_CUSTOM_ERR_EID = 35,
//
///** \brief <tt> This is a count of all the app events and should not be used. </tt> */
//    MFA_EVT_CNT = 37,
//
//	MFA_INITIALIZED_EID = 38
//} MFA_EventIds_t;


#ifdef __cplusplus
}
#endif

#endif /* MFA_EVENTS_H */

/************************/
/*  End of File Comment */
/************************/

