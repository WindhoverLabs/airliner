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

#ifndef AMC_EVENTS_H
#define AMC_EVENTS_H

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
    AMC_RESERVED_EID = 0,  /* Do not use this event ID */

/** \brief <tt> 'Initialized. Version \%d.\%d.\%d.\%d' </tt>
**  \event <tt> 'Initialized. Version \%d.\%d.\%d.\%d' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the AMC task has completed
**  initialization.
**
**  The first \c %d field contains the application major version defined
**      in #AMC_MAJOR_VERSION.
**  The second \c %d field contains the application minor version defined
**      in #AMC_MINOR_VERSION.
**  The third \c %d field contains the application revision number defined
**      in #AMC_REVISION.
**  The fourth \c %d field contains the application revision number defined
**      in #AMC_MISSION_REV.
*/
    AMC_INIT_INF_EID = 1,

/** \brief <tt> 'PWM Tbl Vldt: Min (\%u) less than Disarmed (\%u) speed.' </tt>
**  \event <tt> 'PWM Tbl Vldt: Min (\%u) less than Disarmed (\%u) speed.' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  The first \c %d field contains the 'Minimum' value defined in the table.
**  The second \c %d field contains the 'Disarmed' value defined in the table.
**
*/
    AMC_PWM_CFGTBL_MIN_LT_DISARMED_ERR_EID = 2,

/** \brief <tt> 'PWM Tbl Vldt: Max (\%u) less than Min (\%u) speed.' </tt>
**  \event <tt> 'PWM Tbl Vldt: Max (\%u) less than Min (\%u) speed.' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  The first \c %d field contains the 'Maximum' value defined in the table.
**  The second \c %d field contains the 'Minimum' value defined in the table.
**
*/
    AMC_PWM_CFGTBL_MAX_LT_MIN_ERR_EID = 3,

/** \brief <tt> 'Recvd NOOP. Version \%d.\%d.\%d.\%d' </tt>
**  \event <tt> 'Recvd NOOP. Version \%d.\%d.\%d.\%d' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the CFS AMC Task receives a NoOp
**  command.
**
**  The first \c %u field contains the application major version defined
**      in #AMC_MAJOR_VERSION.
**  The first \c %d field contains the application major version defined
**      in #AMC_MAJOR_VERSION.
**  The second \c %d field contains the application minor version defined
**      in #AMC_MINOR_VERSION.
**  The third \c %d field contains the application revision number defined
**      in #AMC_REVISION.
**  The fourth \c %d field contains the application revision number defined
**      in #AMC_MISSION_REV.
*/
    AMC_CMD_NOOP_EID = 4,

    /** \brief <tt> '\%s Pipe failed to subscribe to \%s. (0x\%08X)' </tt>
    **  \event <tt> '\%s Pipe failed to subscribe to \%s. (0x\%08X)' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS AMC Task fails to subscribe
**  to a message at initialization.
**
**  The first \c %s field contains the name of the pipe.
**  The second \c %s field contains the macro name of the message.
**  The 32 bit hexadecimal number is the error code returned by CFE.
**
*/
    AMC_SUBSCRIBE_ERR_EID = 5,

/** \brief <tt> 'Failed to create \%s pipe (0x\%08X)' </tt>
**  \event <tt> 'Failed to create \%s pipe (0x\%08X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS AMC Task fails to create a
**  CFE Software Bus pipe at initialization.
**
**  The \c %s field contains the name of the pipe.
**  The 32 bit hexadecimal number is the error code returned by CFE.
**
*/
    AMC_PIPE_INIT_ERR_EID = 6,


/** \brief <tt> 'Failed to init mixer (0x\%08x)' </tt>
**  \event <tt> 'Failed to init mixer (0x\%08x)' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the Mixer function to set the
**  configuration table pointer (MulticopterMixer::SetConfigTablePtr)
**  returns a failure.
*/
    AMC_MIXER_INIT_ERR_EID = 7,

/** \brief <tt> 'Failed to manage PWM Config table (0x\%08X)' </tt>
**  \event <tt> 'Failed to manage PWM Config table (0x\%08X)' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  The 32 bit hexadecimal number is the error code returned by CFE
**  #CFE_TBL_Manage function.
**
*/
    AMC_PWM_CFGTBL_MANAGE_ERR_EID = 8,

/** \brief <tt> 'Failed to manage Mixer Config table (0x\%08X)' </tt>
**  \event <tt> 'Failed to manage Mixer Config table (0x\%08X)' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  The 32 bit hexadecimal number is the error code returned by CFE
**  #CFE_TBL_Manage function.
**
*/
    AMC_MIXER_CFGTBL_MANAGE_ERR_EID = 9,

/** \brief <tt> 'Failed to get PWM Config table's address (0x\%08X)' </tt>
**  \event <tt> 'Failed to get PWM Config table's address (0x\%08X)' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  The 32 bit hexadecimal number is the error code returned by CFE
**  #CFE_TBL_GetAddress function.
**
*/
    AMC_PWM_CFGTBL_GETADDR_ERR_EID = 10,

/** \brief <tt> 'Failed to get Mixer Config table's address (0x\%08X)' </tt>
**  \event <tt> 'Failed to get Mixer Config table's address (0x\%08X)' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  The 32 bit hexadecimal number is the error code returned by CFE
**  #CFE_TBL_GetAddress function.
**
*/
    AMC_MIXER_CFGTBL_GETADDR_ERR_EID = 11,

/** \brief <tt> '\%s pipe read error (0x\%08X).' </tt>
**  \event <tt> '\%s pipe read error (0x\%08X).' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS AMC Task has
**  had an error reading from a pipe.
**
**  The \c %s field contains the name of the pipe.
**  The 32 bit hexadecimal number is the error code returned by CFE.
**
*/
    AMC_RCVMSG_ERR_EID = 12,

/** \brief <tt> 'Recvd invalid \%s msgId (0x\%04X)' </tt>
**  \event <tt> 'Recvd invalid \%s msgId (0x\%04X)' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS AMC Task has received an invalid
**  message ID.
**
**  The \c %s field contains the name of the pipe.
**  The 16 bit hexadecimal number is the actual message ID received.
*/
    AMC_MSGID_ERR_EID = 13,

/** \brief <tt> 'Recvd invalid command code (\%u)' </tt>
**  \event <tt> 'Recvd invalid command code (\%u)' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS AMC Task has received an invalid
**  message ID.
**
**  The %u field contains the actual command code received.
*/
    AMC_CC_ERR_EID = 14,

/** \brief <tt> 'Rcvd invalid msgLen: msgId=0x\%08X, cmdCode=\%d, msgLen=\%d, expectedLen=\%d" </tt>
**  \event <tt> 'Rcvd invalid msgLen: msgId=0x\%08X, cmdCode=\%d, msgLen=\%d, expectedLen=\%d" </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS AMC Task has
**  received a message with an invalid length.
**
**  The first \c %d field contains the message ID.
**  The second \c %d field contains the command code.
**  The third \c %d field contains the actual length.
**  The fourth \c %d field contains the expected length.
*/
    AMC_MSGLEN_ERR_EID = 15,

/** \brief <tt> 'Failed to init device (0x\%08x)" </tt>
**  \event <tt> 'Failed to init device (0x\%08x)" </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS AMC Task fails to
**  properly initialize the device required to command the motors.
**
**  The 32 bit hexadecimal number is the error code returned by the
**  platform specific AMC::InitDevice() function.
*/
    AMC_DEVICE_INIT_ERR_EID = 16,

/** \brief <tt> 'Failed to register PWM table (0x%08X)' </tt>
**  \event <tt> 'Failed to register PWM table (0x%08X)' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS AMC Task fails to
**  register the PWM configuration table.
**
**  The 32 bit hexadecimal number is the error code returned by CFE
**  #CFE_TBL_Register function.
**
*/
    AMC_PWM_CFGTBL_REG_ERR_EID = 17,

/** \brief <tt> 'Failed to register Mixer table (0x%08X)' </tt>
**  \event <tt> 'Failed to register Mixer table (0x%08X)' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS AMC Task fails to
**  register the Mixer configuration table.
**
**  The 32 bit hexadecimal number is the error code returned by CFE
**  #CFE_TBL_Register function.
**
*/
    AMC_MIXER_CFGTBL_REG_ERR_EID = 18,

/** \brief <tt> 'Failed to load PWM Config Table (0x%08X)' </tt>
**  \event <tt> 'Failed to load PWM Config Table (0x%08X)' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS AMC Task fails to
**  load the PWM configuration table.
**
**  The 32 bit hexadecimal number is the error code returned by CFE
**  #CFE_TBL_Load function.
**
*/
    AMC_PWM_CFGTBL_LOAD_ERR_EID = 19,

/** \brief <tt> 'Failed to load Mixer Config Table (0x%08X)' </tt>
**  \event <tt> 'Failed to load Mixer Config Table (0x%08X)' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS AMC Task fails to
**  load the Mixer configuration table.
**
**  The 32 bit hexadecimal number is the error code returned by CFE
**  #CFE_TBL_Load function.
**
*/
    AMC_MIXER_CFGTBL_LOAD_ERR_EID = 20,

/** \brief <tt> 'Mixer cfg tbl invld.  R[%u] P[%u] Y[%u] I[%u] G[%u] D[%u] RC[%u]' </tt>
**  \event <tt> 'Mixer cfg tbl invld.  R[%u] P[%u] Y[%u] I[%u] G[%u] D[%u] RC[%u]' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS AMC Mixer configuration table
**  validation function detects an invalid table.  The specific error
**  condition is indicated by a number in the following fields:
**
**  R[1]:  Roll scale is not finite.
**  P[1]:  Pitch scale is not finite.
**  Y[1]:  Yaw scale is not finite.
**  I[1]:  Idle speed is not finite.
**  G[1]:  Invalid geometry enumeration.
**  D[1]:  Delta out max is not finite.
**  RC[1]: Rotor count is less than 1 or greater than the
**         maximum number of rotors supported, defined by the
**         #AMC_MAX_MOTOR_OUTPUTS macro.
**
**  Table validation will stop at the first error.
*/
    AMC_MIXER_INVALID_MIXER_ERR_EID = 21,

/** \brief <tt> 'Mixer cfg tbl invld rotor[%u] config.  R[%u] P[%u] Y[%u] O[%u]' </tt>
**  \event <tt> 'Mixer cfg tbl invld rotor[%u] config.  R[%u] P[%u] Y[%u] O[%u]' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS AMC Mixer configuration table
**  validation function detects an invalid rotor configuration.  The first
**  \%u indicates the index of the rotor in the configuration table.  The
**  specific error condition is indicated by a number in the following
**  fields:
**
**  R[1]:  Indicates the roll scale is not finite.
**  P[1]:  Indicates the pitch scale is not finite.
**  Y[1]:  Indicates the yaw scale is not finite.
**  O[1]:  Indicates the output scale is not finite.
**
**  Table validation will stop at the first error.
*/
    AMC_MIXER_INVLD_ROTOR_CONFIG_ERR_EID = 22,

/** \brief <tt> 'Mixer cfg tbl contains unused nonzero data.' </tt>
**  \event <tt> 'Mixer cfg tbl contains unused nonzero data.' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS AMC Mixer configuration table
**  validation function detects nonzero data in unused table rows.  This
**  may indicate the rotor count is incorrect.
**
**  Table validation will stop at the first error.
*/
    AMC_MIXER_TBLVAL_NONZER_DATA_ERR_EID = 23,

/** \brief <tt> '' </tt>
**  \event <tt> '' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when a #PRMLIB_PARAM_UPDATED_MID
**  message is received with one of this applications params and
**  an error is encountered getting the updated value.
*/
    AMC_PARAM_UPDATE_ERR_EID = 24,

    AMC_DELETE_CB_REG_ERR_EID = 25,

    AMC_ARM_DEBUG_INF_EID = 26,

    AMC_DISARM_DEBUG_INF_EID = 27,

    AMC_ENGAGE_DEBUG_INF_EID = 28,

    AMC_DISENGAGE_DEBUG_INF_EID = 29,

    AMC_CMD_DEBUG_INF_EID = 30,

    AMC_CMD_DEBUG_ERR_EID = 31,

/** \brief <tt> This is a count of all the app events and should not be used. </tt> */
    AMC_EVT_CNT
} AMC_EventIds_t;


#ifdef __cplusplus
}
#endif

#endif /* AMC_EVENTS_H */

/************************/
/*  End of File Comment */
/************************/
    
