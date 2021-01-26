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

#ifndef LGC_EVENTS_H
#define LGC_EVENTS_H

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
    LGC_RESERVED_EID = 0,  /* Do not use this event ID */

/** \brief <tt> 'Initialized. Version \%d.\%d.\%d.\%d' </tt>
**  \event <tt> 'Initialized. Version \%d.\%d.\%d.\%d' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the LGC task has completed
**  initialization.
**
**  The first \c %d field contains the application major version defined
**      in #LGC_MAJOR_VERSION.
**  The second \c %d field contains the application minor version defined
**      in #LGC_MINOR_VERSION.
**  The third \c %d field contains the application revision number defined
**      in #LGC_REVISION.
**  The fourth \c %d field contains the application revision number defined
**      in #LGC_MISSION_REV.
*/
    LGC_INIT_INF_EID,

/** \brief <tt> 'Recvd NOOP. Version \%d.\%d.\%d.\%d' </tt>
**  \event <tt> 'Recvd NOOP. Version \%d.\%d.\%d.\%d' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the CFS LGC Task receives a NoOp
**  command.
**
**  The first \c %u field contains the application major version defined
**      in #LGC_MAJOR_VERSION.
**  The first \c %d field contains the application major version defined
**      in #LGC_MAJOR_VERSION.
**  The second \c %d field contains the application minor version defined
**      in #LGC_MINOR_VERSION.
**  The third \c %d field contains the application revision number defined
**      in #LGC_REVISION.
**  The fourth \c %d field contains the application revision number defined
**      in #LGC_MISSION_REV.
*/
    LGC_CMD_NOOP_EID,

    /** \brief <tt> '\%s Pipe failed to subscribe to \%s. (0x\%08X)' </tt>
    **  \event <tt> '\%s Pipe failed to subscribe to \%s. (0x\%08X)' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS LGC Task fails to subscribe
**  to a message at initialization.
**
**  The first \c %s field contains the name of the pipe.
**  The second \c %s field contains the macro name of the message.
**  The 32 bit hexadecimal number is the error code returned by CFE.
**
*/
    LGC_SUBSCRIBE_ERR_EID,

/** \brief <tt> 'Failed to create \%s pipe (0x\%08X)' </tt>
**  \event <tt> 'Failed to create \%s pipe (0x\%08X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS LGC Task fails to create a
**  CFE Software Bus pipe at initialization.
**
**  The \c %s field contains the name of the pipe.
**  The 32 bit hexadecimal number is the error code returned by CFE.
**
*/
    LGC_PIPE_INIT_ERR_EID,

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
    LGC_CFGTBL_MANAGE_ERR_EID,

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
    LGC_CFGTBL_GETADDR_ERR_EID,

/** \brief <tt> '\%s pipe read error (0x\%08X).' </tt>
**  \event <tt> '\%s pipe read error (0x\%08X).' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS LGC Task has
**  had an error reading from a pipe.
**
**  The \c %s field contains the name of the pipe.
**  The 32 bit hexadecimal number is the error code returned by CFE.
**
*/
    LGC_RCVMSG_ERR_EID,

/** \brief <tt> 'Recvd invalid \%s msgId (0x\%04X)' </tt>
**  \event <tt> 'Recvd invalid \%s msgId (0x\%04X)' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS LGC Task has received an invalid
**  message ID.
**
**  The \c %s field contains the name of the pipe.
**  The 16 bit hexadecimal number is the actual message ID received.
*/
    LGC_MSGID_ERR_EID,

/** \brief <tt> 'Recvd invalid command code (\%u)' </tt>
**  \event <tt> 'Recvd invalid command code (\%u)' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS LGC Task has received an invalid
**  message ID.
**
**  The %u field contains the actual command code received.
*/
    LGC_CC_ERR_EID,

/** \brief <tt> 'Rcvd invalid msgLen: msgId=0x\%08X, cmdCode=\%d, msgLen=\%d, expectedLen=\%d" </tt>
**  \event <tt> 'Rcvd invalid msgLen: msgId=0x\%08X, cmdCode=\%d, msgLen=\%d, expectedLen=\%d" </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS LGCTask has
**  received a message with an invalid length.
**
**  The first \c %d field contains the message ID.
**  The second \c %d field contains the command code.
**  The third \c %d field contains the actual length.
**  The fourth \c %d field contains the expected length.
*/
    LGC_MSGLEN_ERR_EID,

/** \brief <tt> 'Failed to register config table (0x%08X)' </tt>
**  \event <tt> 'Failed to register config table (0x%08X)' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS LGC Task fails to
**  register the LGC configuration table.
**
**  The 32 bit hexadecimal number is the error code returned by CFE
**  #CFE_TBL_Register function.
**
*/
    LGC_CFGTBL_REG_ERR_EID,

/** \brief <tt> 'Failed to load Config Table (0x%08X)' </tt>
**  \event <tt> 'Failed to load Config Table (0x%08X)' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS LGC Task fails to
**  load the LGC configuration table.
**
**  The 32 bit hexadecimal number is the error code returned by CFE
**  #CFE_TBL_Load function.
**
*/
    LGC_CFGTBL_LOAD_ERR_EID,

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
    LGC_PWM_CFGTBL_MIN_LT_DISARMED_ERR_EID,

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
    LGC_PWM_CFGTBL_MAX_LT_MIN_ERR_EID,

/** \brief <tt> 'Failed to init device (0x\%08x)" </tt>
**  \event <tt> 'Failed to init device (0x\%08x)" </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS LGC Task fails to
**  properly initialize the device required to command the landing gear
**  actuator(s).
**
**  The 32 bit hexadecimal number is the error code returned by the
**  platform specific LGC::InitDevice() function.
*/
    LGC_DEVICE_INIT_ERR_EID,

/** \brief <tt> 'Landing Gear in Retracted State' </tt>
**  \event <tt> 'Landing Gear in Retracted State' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the landing gear is retracted.
*/
    LGC_RETRACT_INF_EID,

/** \brief <tt> 'Landing gear in Extended State' </tt>
**  \event <tt> 'Landing gear in Extended State' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the landing gear is extended.
*/
    LGC_EXTEND_INF_EID,

/** \brief <tt> This is a count of all the app events and should not be used. </tt> */
    LGC_EVT_CNT

} LGC_EventIds_t;


#ifdef __cplusplus
}
#endif

#endif /* LGC_EVENTS_H */

/************************/
/*  End of File Comment */
/************************/
    
