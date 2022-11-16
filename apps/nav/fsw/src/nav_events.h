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

#ifndef NAV_EVENTS_H
#define NAV_EVENTS_H

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
    NAV_RESERVED_EID                     = 0, /* Do not use this event ID */

    /** \brief <tt> 'Initialized. Version \%d.\%d.\%d.\%d' </tt>
     **  \event <tt> 'Initialized. Version \%d.\%d.\%d.\%d' </tt>
     **
     **  \par Type: INFORMATION
     **
     **  \par Cause:
     **
     **  This event message is issued when the NAV task has completed
     **  initialization.
     **
     **  The first \c %d field contains the application major version defined
     **      in #NAV_MAJOR_VERSION.
     **  The second \c %d field contains the application minor version defined
     **      in #NAV_MINOR_VERSION.
     **  The third \c %d field contains the application revision number defined
     **      in #NAV_REVISION.
     **  The fourth \c %d field contains the application revision number defined
     **      in #NAV_MISSION_REV.
     */
    NAV_INIT_INF_EID                     = 1,

    /** \brief <tt> 'Recvd NOOP. Version \%d.\%d.\%d.\%d' </tt>
     **  \event <tt> 'Recvd NOOP. Version \%d.\%d.\%d.\%d' </tt>
     **
     **  \par Type: INFORMATION
     **
     **  \par Cause:
     **
     **  This event message is issued when the CFS NAV Task receives a NoOp
     **  command.
     **
     **  The first \c %u field contains the application major version defined
     **      in #NAV_MAJOR_VERSION.
     **  The first \c %d field contains the application major version defined
     **      in #NAV_MAJOR_VERSION.
     **  The second \c %d field contains the application minor version defined
     **      in #NAV_MINOR_VERSION.
     **  The third \c %d field contains the application revision number defined
     **      in #NAV_REVISION.
     **  The fourth \c %d field contains the application revision number defined
     **      in #NAV_MISSION_REV.
     */
    NAV_CMD_NOOP_EID                     = 2,

    /** \brief <tt> '\%s Pipe failed to subscribe to \%s. (0x\%08X)' </tt>
     **  \event <tt> '\%s Pipe failed to subscribe to \%s. (0x\%08X)' </tt>
     **
     **  \par Type: ERROR
     **
     **  \par Cause:
     **
     **  This event message is issued when the CFS NAV Task fails to subscribe
     **  to a message at initialization.
     **
     **  The first \c %s field contains the name of the pipe.
     **  The second \c %s field contains the macro name of the message.
     **  The 32 bit hexadecimal number is the error code returned by CFE.
     **
     */
    NAV_SUBSCRIBE_ERR_EID                = 3,

    /** \brief <tt> 'Failed to create \%s pipe (0x\%08X)' </tt>
     **  \event <tt> 'Failed to create \%s pipe (0x\%08X' </tt>
     **
     **  \par Type: ERROR
     **
     **  \par Cause:
     **
     **  This event message is issued when the CFS NAV Task fails to create a
     **  CFE Software Bus pipe at initialization.
     **
     **  The \c %s field contains the name of the pipe.
     **  The 32 bit hexadecimal number is the error code returned by CFE.
     **
     */
    NAV_PIPE_INIT_ERR_EID                = 4,

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
    NAV_CFGTBL_MANAGE_ERR_EID            = 5,

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
    NAV_CFGTBL_GETADDR_ERR_EID           = 6,

    /** \brief <tt> '\%s pipe read error (0x\%08X).' </tt>
     **  \event <tt> '\%s pipe read error (0x\%08X).' </tt>
     **
     **  \par Type: ERROR
     **
     **  \par Cause:
     **
     **  This event message is issued when the CFS NAV Task has
     **  had an error reading from a pipe.
     **
     **  The \c %s field contains the name of the pipe.
     **  The 32 bit hexadecimal number is the error code returned by CFE.
     **
     */
    NAV_RCVMSG_ERR_EID                   = 7,

    /** \brief <tt> 'Recvd invalid \%s msgId (0x\%04X)' </tt>
     **  \event <tt> 'Recvd invalid \%s msgId (0x\%04X)' </tt>
     **
     **  \par Type: ERROR
     **
     **  \par Cause:
     **
     **  This event message is issued when the CFS NAV Task has received an invalid
     **  message ID.
     **
     **  The \c %s field contains the name of the pipe.
     **  The 16 bit hexadecimal number is the actual message ID received.
     */
    NAV_MSGID_ERR_EID                    = 8,

    /** \brief <tt> 'Recvd invalid command code (\%u)' </tt>
     **  \event <tt> 'Recvd invalid command code (\%u)' </tt>
     **
     **  \par Type: ERROR
     **
     **  \par Cause:
     **
     **  This event message is issued when the CFS NAV Task has received an invalid
     **  message ID.
     **
     **  The %u field contains the actual command code received.
     */
    NAV_CC_ERR_EID                       = 9,

    /** \brief <tt> 'Rcvd invalid msgLen: msgId=0x\%08X, cmdCode=\%d, msgLen=\%d, expectedLen=\%d" </tt>
     **  \event <tt> 'Rcvd invalid msgLen: msgId=0x\%08X, cmdCode=\%d, msgLen=\%d, expectedLen=\%d" </tt>
     **
     **  \par Type: ERROR
     **
     **  \par Cause:
     **
     **  This event message is issued when the CFS NAVTask has
     **  received a message with an invalid length.
     **
     **  The first \c %d field contains the message ID.
     **  The second \c %d field contains the command code.
     **  The third \c %d field contains the actual length.
     **  The fourth \c %d field contains the expected length.
     */
    NAV_MSGLEN_ERR_EID                   = 10,

    /** \brief <tt> 'Failed to register config table (0x%08X)' </tt>
     **  \event <tt> 'Failed to register config table (0x%08X)' </tt>
     **
     **  \par Type: ERROR
     **
     **  \par Cause:
     **
     **  This event message is issued when the CFS NAV Task fails to
     **  register the NAV configuration table.
     **
     **  The 32 bit hexadecimal number is the error code returned by CFE
     **  #CFE_TBL_Register function.
     **
     */
    NAV_CFGTBL_REG_ERR_EID               = 11,

    /** \brief <tt> 'Failed to load Config Table (0x%08X)' </tt>
     **  \event <tt> 'Failed to load Config Table (0x%08X)' </tt>
     **
     **  \par Type: ERROR
     **
     **  \par Cause:
     **
     **  This event message is issued when the CFS NAV Task fails to
     **  load the NAV configuration table.
     **
     **  The 32 bit hexadecimal number is the error code returned by CFE
     **  #CFE_TBL_Load function.
     **
     */
    NAV_CFGTBL_LOAD_ERR_EID              = 12,

    /** \brief <tt> 'Mission failed (\%s)' </tt>
     **  \event <tt> 'Mission failed (\%s)' </tt>
     **
     **  \par Type: ERROR
     **
     **  \par Cause:
     **
     **  This event message is issued when the CFS NAV Task fails to
     **  complete defined mission.
     **
     **  The first \c %s field contains the reason for mission failure.
     **
     */
    NAV_MSN_FAILED_ERR_EID               = 13,

    /** \brief <tt> 'Set altitude lower than minimum clearance : \%f meters' </tt>
     **  \event <tt> 'Set altitude lower than minimum clearance : \%f meters' </tt>
     **
     **  \par Type: INFORMATION
     **
     **  \par Cause:
     **
     **  This event message is issued when the CFS NAV Task reports
     **  altitude lower than minimum clearance.
     **
     **  The first \c %f field contains the altitude.
     **
     */
    NAV_LOW_CLEARANCE_ALT_EID            = 14,

    /** \brief <tt> 'Set altitude already higher than take off altitude' </tt>
     **  \event <tt> 'Set altitude already higher than take off altitude' </tt>
     **
     **  \par Type: INFORMATION
     **
     **  \par Cause:
     **
     **  This event message is issued when the CFS NAV Task reports if
     **  altitude is higher than current altitude.
     **
     */
    NAV_HIGH_ALT_EID                     = 15,

    /** \brief <tt> 'Commencing \%s' </tt>
     **  \event <tt> 'Commencing \%s' </tt>
     **
     **  \par Type: INFORMATION
     **
     **  \par Cause:
     **
     **  This event message is issued when the CFS NAV Task reports
     **  navigator action start.
     **
     **  The first \c %s field contains the action.
     **
     */
    NAV_ACTION_ST_EID                    = 16,

    /** \brief <tt> 'RTL: Commencing climb to \%d m (\%d m above home)' </tt>
     **  \event <tt> 'RTL: Commencing climb to \%d m (\%d m above home)' </tt>
     **
     **  \par Type: INFORMATION
     **
     **  \par Cause:
     **
     **  This event message is issued when the CFS NAV Task reports
     **  RTL climb action start.
     **
     **  The first \c %d field contains the climb altitude.
     **  The second \c %d field contains the altitude from home position.
     **
     */
    NAV_RTL_CLIMB_ST_EID                 = 17,

    /** \brief <tt> 'RTL: Commencing return at \%d m (\%d m above home)' </tt>
     **  \event <tt> 'RTL: Commencing return at \%d m (\%d m above home)' </tt>
     **
     **  \par Type: INFORMATION
     **
     **  \par Cause:
     **
     **  This event message is issued when the CFS NAV Task reports
     **  RTL return action start.
     **
     **  The first \c %d field contains the altitude.
     **  The second \c %d field contains the altitude from home position.
     **
     */
    NAV_RTL_RETURN_ST_EID                = 18,

    /** \brief <tt> 'RTL: Commencing descend to \%d m (\%d m above home)' </tt>
     **  \event <tt> 'RTL: Commencing descend to \%d m (\%d m above home)' </tt>
     **
     **  \par Type: INFORMATION
     **
     **  \par Cause:
     **
     **  This event message is issued when the CFS NAV Task reports
     **  RTL descend action start.
     **
     **  The first \c %d field contains the altitude.
     **  The second \c %d field contains the altitude from home position.
     **
     */
    NAV_RTL_DESCEND_ST_EID               = 19,

    /** \brief <tt> 'RTL: Commencing loiter for \%.1fs' </tt>
     **  \event <tt> 'RTL: Commencing loiter for \%.1fs' </tt>
     **
     **  \par Type: INFORMATION
     **
     **  \par Cause:
     **
     **  This event message is issued when the CFS NAV Task reports
     **  RTL loiter action start.
     **
     **  The first \c %f field contains the loiter time.
     **
     */
    NAV_RTL_LOITER_ST_EID                = 20,

    /** \brief <tt> 'RTL: Loiter completed' </tt>
     **  \event <tt> 'RTL: Loiter completed' </tt>
     **
     **  \par Type: INFORMATION
     **
     **  \par Cause:
     **
     **  This event message is issued when the CFS NAV Task reports
     **  RTL loiter action end.
     **
     */
    NAV_RTL_LOITER_EN_EID                = 21,

    /** \brief <tt> 'RTL: Commencing land at home' </tt>
     **  \event <tt> 'RTL: Commencing land at home' </tt>
     **
     **  \par Type: INFORMATION
     **
     **  \par Cause:
     **
     **  This event message is issued when the CFS NAV Task reports
     **  RTL land action start.
     **
     */
    NAV_RTL_LAND_ST_EID                  = 22,

    /** \brief <tt> 'RTL: Land completed' </tt>
     **  \event <tt> 'RTL: Land completed' </tt>
     **
     **  \par Type: INFORMATION
     **
     **  \par Cause:
     **
     **  This event message is issued when the CFS NAV Task reports
     **  RTL land action end.
     **
     */
    NAV_RTL_LAND_EN_EID                  = 23,

    /** \brief <tt> 'Already landed, not executing RTL' </tt>
     **  \event <tt> 'Already landed, not executing RTL' </tt>
     **
     **  \par Type: INFORMATION
     **
     **  \par Cause:
     **
     **  This event message is issued when the CFS NAV Task reports
     **  RTL execution when in landed state.
     **
     */
    NAV_RTL_LND_SFGA_EID                 = 24,

    /** \brief <tt> 'Subsequent takeoff is not allowed' </tt>
     **  \event <tt> 'Subsequent takeoff is not allowed' </tt>
     **
     **  \par Type: INFORMATION
     **
     **  \par Cause:
     **
     **  This event message is issued when the CFS NAV Task reports
     **  subsequent takeoff is requested.
     **
     */
    NAV_SUBSEQ_TAKEOFF_EID               = 25,
    NAV_AUTO_MISSION_STATE_EID           = 26,

    /** \brief <tt> This is a count of all the app events and should not be used. </tt> */
    NAV_EVT_CNT
} NAV_EventIds_t;

#ifdef __cplusplus
}
#endif

#endif /* NAV_EVENTS_H */

/************************/
/*  End of File Comment */
/************************/

