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

#ifndef MPC_EVENTS_H
#define MPC_EVENTS_H

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
typedef enum
{

/** \brief <tt> Value of zero is reserved, and should not be used. </tt> */
    MPC_RESERVED_EID                             = 0,  /* Do not use this event ID */

/** \brief <tt> 'Initialized. Version \%d.\%d.\%d.\%d' </tt>
**  \event <tt> 'Initialized. Version \%d.\%d.\%d.\%d' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the MPC task has completed
**  initialization.
**
**  The first \c %d field contains the application major version defined
**      in #MPC_MAJOR_VERSION.
**  The second \c %d field contains the application minor version defined
**      in #MPC_MINOR_VERSION.
**  The third \c %d field contains the application revision number defined
**      in #MPC_REVISION.
**  The fourth \c %d field contains the application revision number defined
**      in #MPC_MISSION_REV.
*/
    MPC_INIT_INF_EID                             = 1,

/** \brief <tt> 'Recvd NOOP. Version \%d.\%d.\%d.\%d' </tt>
**  \event <tt> 'Recvd NOOP. Version \%d.\%d.\%d.\%d' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the CFS MPC Task receives a NoOp
**  command.
**
**  The first \c %u field contains the application major version defined
**      in #MPC_MAJOR_VERSION.
**  The first \c %d field contains the application major version defined
**      in #MPC_MAJOR_VERSION.
**  The second \c %d field contains the application minor version defined
**      in #MPC_MINOR_VERSION.
**  The third \c %d field contains the application revision number defined
**      in #MPC_REVISION.
**  The fourth \c %d field contains the application revision number defined
**      in #MPC_MISSION_REV.
*/
    MPC_CMD_NOOP_EID                             = 2,

    /** \brief <tt> '\%s Pipe failed to subscribe to \%s. (0x\%08X)' </tt>
    **  \event <tt> '\%s Pipe failed to subscribe to \%s. (0x\%08X)' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS MPC Task fails to subscribe
**  to a message at initialization.
**
**  The first \c %s field contains the name of the pipe.
**  The second \c %s field contains the macro name of the message.
**  The 32 bit hexadecimal number is the error code returned by CFE.
**
*/
    MPC_SUBSCRIBE_ERR_EID                        = 3,

/** \brief <tt> 'Failed to create \%s pipe (0x\%08X)' </tt>
**  \event <tt> 'Failed to create \%s pipe (0x\%08X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS MPC Task fails to create a
**  CFE Software Bus pipe at initialization.
**
**  The \c %s field contains the name of the pipe.
**  The 32 bit hexadecimal number is the error code returned by CFE.
**
*/
    MPC_PIPE_INIT_ERR_EID                        = 4,

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
    MPC_CFGTBL_MANAGE_ERR_EID                    = 5,

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
    MPC_CFGTBL_GETADDR_ERR_EID                   = 6,

/** \brief <tt> '\%s pipe read error (0x\%08X).' </tt>
**  \event <tt> '\%s pipe read error (0x\%08X).' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS MPC Task has
**  had an error reading from a pipe.
**
**  The \c %s field contains the name of the pipe.
**  The 32 bit hexadecimal number is the error code returned by CFE.
**
*/
    MPC_RCVMSG_ERR_EID                           = 7,

/** \brief <tt> 'Recvd invalid \%s msgId (0x\%04X)' </tt>
**  \event <tt> 'Recvd invalid \%s msgId (0x\%04X)' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS MPC Task has received an invalid
**  message ID.
**
**  The \c %s field contains the name of the pipe.
**  The 16 bit hexadecimal number is the actual message ID received.
*/
    MPC_MSGID_ERR_EID                            = 8,

/** \brief <tt> 'Recvd invalid command code (\%u)' </tt>
**  \event <tt> 'Recvd invalid command code (\%u)' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS MPC Task has received an invalid
**  message ID.
**
**  The %u field contains the actual command code received.
*/
    MPC_CC_ERR_EID                               = 9,

/** \brief <tt> 'Rcvd invalid msgLen: msgId=0x\%08X, cmdCode=\%d, msgLen=\%d, expectedLen=\%d" </tt>
**  \event <tt> 'Rcvd invalid msgLen: msgId=0x\%08X, cmdCode=\%d, msgLen=\%d, expectedLen=\%d" </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS MPCTask has
**  received a message with an invalid length.
**
**  The first \c %d field contains the message ID.
**  The second \c %d field contains the command code.
**  The third \c %d field contains the actual length.
**  The fourth \c %d field contains the expected length.
*/
    MPC_MSGLEN_ERR_EID                           = 10,

/** \brief <tt> 'Failed to register config table (0x%08X)' </tt>
**  \event <tt> 'Failed to register config table (0x%08X)' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS MPC Task fails to
**  register the MPC configuration table.
**
**  The 32 bit hexadecimal number is the error code returned by CFE
**  #CFE_TBL_Register function.
**
*/
    MPC_CFGTBL_REG_ERR_EID                       = 11,

/** \brief <tt> 'Failed to load Config Table (0x%08X)' </tt>
**  \event <tt> 'Failed to load Config Table (0x%08X)' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS MPC Task fails to
**  load the MPC configuration table.
**
**  The 32 bit hexadecimal number is the error code returned by CFE
**  #CFE_TBL_Load function.
**
*/
    MPC_CFGTBL_LOAD_ERR_EID                      = 12,

/** \brief <tt> 'Unknown velocity offboard coordinate frame. (%u)' </tt>
**  \event <tt> 'Unknown velocity offboard coordinate frame. (%u)' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the software receives a current
**  position setpoint with an unknown velocity frame.  The actual
**  enumeration received is presented in parenthesis.
**
*/
    MPC_UNK_VEL_FRM_ERR_EID                      = 13,

/** \brief <tt> "Updated <XY,Z> PID values. Gain: %f, P: %f, I: %f, D: %f" </tt>
**  \event <tt> "Updated XY PID values. Gain: %f, P: %f, I: %f, D: %f" </tt>
**  \event <tt> "Updated Z PID values. Gain: %f, P: %f, I: %f, D: %f" </tt>
**
**
**  \par Type: CFE_EVS_INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the software receives a command to
**  update the XY or Z PID values.
**
**  \sa #MPC_SET_XY_PID_CC, #MPC_SET_Z_PID_CC.
**
**  This event message is issued when
*/
    MPC_PID_UPDATE_EID                           = 14,

/** \brief <tt> "Sending Diag packet." </tt>
**  \event <tt> "Sending Diag packet." </tt>
**
**  \par Type: CFE_EVS_INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the software receives a command to
**  send a diagnostic message.
**
**  \sa #MPC_SEND_DIAG_CC, #ReportDiagnostic
*/
    MPC_SEND_DIAG_EID                            = 15,
    
/** \brief <tt> "Updated HOLD_DZ value: %f" </tt>
**  \event <tt> "Updated HOLD_DZ value: %f" </tt>
**
**  \par Type: CFE_EVS_INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the software receives a command to
**  update the #HOLD_DZ configuration parameter.
**
**  \sa #MPC_SET_HOLD_DZ_CC, #UpdateHoldDz
*/
    MPC_SET_DZ_EID                               = 16,

/** \brief <tt> "Updated stick expo values: XY: %f Z: %f" </tt>
**  \event <tt> "Updated stick expo values: XY: %f Z: %f" </tt>
**
**  \par Type: CFE_EVS_INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the software receives a command to
**  update the #XY_MAN_EXPO and #Z_MAN_EXPO configuration parameters.
**
**  /sa #MPC_SET_STICK_EXPO_CC, #UpdateStickExpo
*/
    MPC_SET_EXPO_EID                             = 17,

/** \brief <tt> "Updated takeoff ramp time: %f" </tt>
**  \event <tt> "Updated takeoff ramp time: %f" </tt>
**
**  \par Type: CFE_EVS_INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the software receives a command to
**  update the #TKO_RAMP_T configuration parameter.
**
**  /sa #MPC_SET_TKO_RAMP_CC, #UpdateTakeoffRampTime
*/
    MPC_SET_TKO_RAMP_EID                         = 18,

    /** \brief <tt> "User intention not recognized. %u" </tt>
    **  \event <tt> "User intention not recognized. %u" </tt>
    **
    **  \par Type: CFE_EVS_ERROR
    **
    **  \par Cause:
    **
    **  This event message is issued when the software encounters
    **  an unrecognized user intention..
    **
    **  /sa #m_UserIntentionXY
    */
    MPC_UNKNOWN_USER_INTENTION_ERR_EID           = 19,

    /** \brief <tt> "Auto: Position setpoint not finite." </tt>
    **  \event <tt> "Auto: Position setpoint not finite." </tt>
    **
    **  \par Type: CFE_EVS_ERROR
    **
    **  \par Cause:
    **
    **  This event message is issued when the software encounters
    **  a Position Setpoint that is not finite while in an auto
    **  mode.
    **
    **  /sa #m_PositionSetpoint
    */
    MPC_POS_SP_NOT_FINITE_ERR_EID                = 20,

    /** \brief <tt> "Parameter %s is invalid." </tt>
    **  \event <tt> "Parameter %s is invalid." </tt>
    **
    **  \par Type: CFE_EVS_ERROR
    **
    **  \par Cause:
    **
    **  This event message is issued when a parameter validation function
    **  fails to validate a parameter.
    **
    */
	MPC_INVLD_PARAM_ERR_EID                      = 21,






/** \brief <tt> This is a count of all the app events and should not be used. </tt> */
    MPC_EVT_CNT
} MPC_EventIds_t;


#ifdef __cplusplus
}
#endif

#endif /* MPC_EVENTS_H */

/************************/
/*  End of File Comment */
/************************/
    
