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

#ifndef PE_EVENTS_H
#define PE_EVENTS_H

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
	PE_RESERVED_EID = 0,  /* Do not use this event ID */

/** \brief <tt> 'Initialized. Version \%d.\%d.\%d.\%d' </tt>
**  \event <tt> 'Initialized. Version \%d.\%d.\%d.\%d' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the PE task has completed
**  initialization.
**
**  The first \c %d field contains the application major version defined
**      in #PE_MAJOR_VERSION.
**  The second \c %d field contains the application minor version defined
**      in #PE_MINOR_VERSION.
**  The third \c %d field contains the application revision number defined
**      in #PE_REVISION.
**  The fourth \c %d field contains the application revision number defined
**      in #PE_MISSION_REV.
*/
	PE_INIT_INF_EID = 1,

/** \brief <tt> 'Recvd NOOP. Version \%d.\%d.\%d.\%d' </tt>
**  \event <tt> 'Recvd NOOP. Version \%d.\%d.\%d.\%d' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the CFS PE Task receives a NoOp
**  command.
**
**  The first \c %u field contains the application major version defined
**      in #PE_MAJOR_VERSION.
**  The first \c %d field contains the application major version defined
**      in #PE_MAJOR_VERSION.
**  The second \c %d field contains the application minor version defined
**      in #PE_MINOR_VERSION.
**  The third \c %d field contains the application revision number defined
**      in #PE_REVISION.
**  The fourth \c %d field contains the application revision number defined
**      in #PE_MISSION_REV.
*/
	PE_CMD_NOOP_EID = 2,

    /** \brief <tt> '\%s Pipe failed to subscribe to \%s. (0x\%08X)' </tt>
    **  \event <tt> '\%s Pipe failed to subscribe to \%s. (0x\%08X)' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS PE Task fails to subscribe
**  to a message at initialization.
**
**  The first \c %s field contains the name of the pipe.
**  The second \c %s field contains the macro name of the message.
**  The 32 bit hexadecimal number is the error code returned by CFE.
**
*/
	PE_SUBSCRIBE_ERR_EID = 3,

/** \brief <tt> 'Failed to create \%s pipe (0x\%08X)' </tt>
**  \event <tt> 'Failed to create \%s pipe (0x\%08X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS PE Task fails to create a
**  CFE Software Bus pipe at initialization.
**
**  The \c %s field contains the name of the pipe.
**  The 32 bit hexadecimal number is the error code returned by CFE.
**
*/
	PE_PIPE_INIT_ERR_EID = 4,

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
	PE_CFGTBL_MANAGE_ERR_EID = 5,

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
	PE_CFGTBL_GETADDR_ERR_EID = 6,

/** \brief <tt> '\%s pipe read error (0x\%08X).' </tt>
**  \event <tt> '\%s pipe read error (0x\%08X).' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS PE Task has
**  had an error reading from a pipe.
**
**  The \c %s field contains the name of the pipe.
**  The 32 bit hexadecimal number is the error code returned by CFE.
**
*/
	PE_RCVMSG_ERR_EID = 7,

/** \brief <tt> 'Recvd invalid \%s msgId (0x\%04X)' </tt>
**  \event <tt> 'Recvd invalid \%s msgId (0x\%04X)' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS PE Task has received an invalid
**  message ID.
**
**  The \c %s field contains the name of the pipe.
**  The 16 bit hexadecimal number is the actual message ID received.
*/
	PE_MSGID_ERR_EID = 8,

/** \brief <tt> 'Recvd invalid command code (\%u)' </tt>
**  \event <tt> 'Recvd invalid command code (\%u)' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS PE Task has received an invalid
**  message ID.
**
**  The %u field contains the actual command code received.
*/
	PE_CC_ERR_EID = 9,

/** \brief <tt> 'Rcvd invalid msgLen: msgId=0x\%08X, cmdCode=\%d, msgLen=\%d, expectedLen=\%d" </tt>
**  \event <tt> 'Rcvd invalid msgLen: msgId=0x\%08X, cmdCode=\%d, msgLen=\%d, expectedLen=\%d" </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS PETask has
**  received a message with an invalid length.
**
**  The first \c %d field contains the message ID.
**  The second \c %d field contains the command code.
**  The third \c %d field contains the actual length.
**  The fourth \c %d field contains the expected length.
*/
	PE_MSGLEN_ERR_EID = 10,

/** \brief <tt> 'Failed to register config table (0x%08X)' </tt>
**  \event <tt> 'Failed to register config table (0x%08X)' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS PE Task fails to
**  register the PE configuration table.
**
**  The 32 bit hexadecimal number is the error code returned by CFE
**  #CFE_TBL_Register function.
**
*/
	PE_CFGTBL_REG_ERR_EID = 11,

/** \brief <tt> 'Failed to load Config Table (0x%08X)' </tt>
**  \event <tt> 'Failed to load Config Table (0x%08X)' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS PE Task fails to
**  load the PE configuration table.
**
**  The 32 bit hexadecimal number is the error code returned by CFE
**  #CFE_TBL_Load function.
**
*/
	PE_CFGTBL_LOAD_ERR_EID = 12,


/** \brief Local estimation initialized
**  \event Local estimation initialized
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the local estimation has received enough data to initialize
**
*/
	PE_LOCAL_ESTIMATOR_INF_EID = 13,

/** \brief Global estimation initialized
**  \event Global estimation initialized
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the global estimation has received enough data to initialize
**
*/
	PE_GLOBAL_ESTIMATOR_INF_EID = 14,

/** \brief Reinit state covariance. Index (%i, %i) not finite
**  \event Reinit state covariance. Index (%i, %i) not finite
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when one of the estimator validity checks fails.
**  The estimator will reinitialize after this event is broadcast.
**
*/
	PE_ESTIMATOR_ERR_EID = 15,

/** \brief Baro fault, r %5.2f m, beta %5.2f
**  \event Baro fault, r %5.2f m, beta %5.2f
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the baro fails a validity check.
**  Baro will reinitialize after this event is broadcast.
**
*/
	PE_BARO_FAULT_ERR_EID = 16,

/** \brief Baro initialized. Mean: (%d) Std dev: (%d) cm
**  \event Baro initialized. Mean: (%d) Std dev: (%d) cm
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when baro has (re)initialized successfully.
**
*/
    PE_BARO_OK_INF_EID = 17,

/** \brief Baro timeout
**  \event Baro timeout
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the baro data times out and needs to be reset
**
*/
	PE_BARO_TIMEOUT_ERR_EID = 18,

/** \brief GPS timeout
**  \event GPS timeout
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the gps data times out and needs to be reset
**
*/
	PE_GPS_TIMEOUT_ERR_EID = 19,

/** \brief gps fault, %3g %3g %3g %3g %3g %3g
**  \event gps fault, %3g %3g %3g %3g %3g %3g
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the GPS fails a validity check.
**  GPS will reinitialize after this event is broadcast.
**
*/
	PE_GPS_FAULT_ERR_EID = 20,

/** \brief GPS OK
**  \event GPS OK
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when GPS has (re)initialized successfully.
**
*/
    PE_GPS_OK_INF_EID = 21,

/** \brief Land fault, beta %5.2f
**  \event Land fault, beta %5.2f
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the land detector fails a validity check.
**  Land detector will reinitialize after this event is broadcast.
**
*/
	PE_LAND_FAULT_ERR_EID = 22,

/** \brief Land detector initialized
**  \event Land detector initialized
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when land detector has (re)initialized successfully.
**
*/
	PE_LAND_OK_INF_EID = 23,

/** \brief Land detector timeout
**  \event Land detector timeout
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the land detector data times out and needs to be reset
**
*/
	PE_LAND_TIMEOUT_ERR_EID = 24,

/** \brief Dist fault, r %5.2f m, beta %5.2f
**  \event Dist fault, r %5.2f m, beta %5.2f
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the dist fails a validity check.
**  Dist will reinitialize after this event is broadcast.
**
*/
	PE_DIST_FAULT_ERR_EID = 25,

/** \brief Dist initialized. Mean: (%d) Std dev: (%d) cm
**  \event Dist initialized. Mean: (%d) Std dev: (%d) cm
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when dist has (re)initialized successfully.
**
*/
	PE_DIST_OK_INF_EID = 26,

/** \brief Dist timeout
**  \event Dist timeout
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the dist data times out and needs to be reset
**
*/
	PE_DIST_TIMEOUT_ERR_EID = 27,

/** \brief PE - mutex creation failed (0x%08lX)
**  \event PE - mutex creation failed (0x%08lX)
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when mutex creation failed.
**
*/
	PE_MUTEX_ERR_EID = 28,

/** \brief Local position message data invalid
**  \event Local position message data invalid
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the local position message data fails validity checks.
**
*/
	PE_LOCAL_POS_MSG_ERR_EID = 29,

/** \brief Global position message data invalid
**  \event Global position message data invalid
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the global position message data fails validity checks.
**
*/
	PE_GLOBAL_POS_MSG_ERR_EID = 30,

/** \brief Fusing distance sensor into estimation.
**  \event Fusing distance sensor into estimation.
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when commanded to fuse distance sensor into estimator.
**
*/
	PE_FUSE_DIST_INF_EID = 31,

/** \brief Already fusing distance sensor into estimation.
**  \event Already fusing distance sensor into estimation.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when commanded to fuse distance sensor into estimator.
**  but we are already fusing.
**
*/
	PE_FUSE_DIST_ERR_EID = 32,

/** \brief Disabling distance sensor fusion into estimation.
**  \event Disabling distance sensor fusion into estimation.
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when commanded to disable distance sensor fusion into estimator.
**
*/
	PE_DISABLE_DIST_INF_EID = 33,

/** \brief Failed to disable distance sensor fusion into estimation.
**  \event Failed to disable distance sensor fusion into estimation.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when commanded to disable distance sensor fusion into estimator
**  but it is not already fusing.
**
*/
	PE_DISABLE_DIST_ERR_EID = 34,

/** \brief Fusing GPS into estimation.
**  \event Fusing GPS into estimation.
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when commanded to fuse GPS into estimator.
**
*/
	PE_FUSE_GPS_INF_EID = 35,

/** \brief Already fusing GPS into estimation.
**  \event Already fusing GPS into estimation.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when commanded to fuse GPS into estimator.
**  but we are already fusing.
**
*/
	PE_FUSE_GPS_ERR_EID = 36,

/** \brief Disabling GPS fusion into estimation.
**  \event Disabling GPS fusion into estimation.
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when commanded to disable GPS fusion into estimator.
**
*/
	PE_DISABLE_GPS_INF_EID = 37,

/** \brief Failed to disable GPS fusion into estimation.
**  \event Failed to disable GPS fusion into estimation.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when commanded to disable GPS fusion into estimator
**  but it is not already fusing.
**
*/
	PE_DISABLE_GPS_ERR_EID = 38,

/** \brief Fusing Baro into estimation.
**  \event Fusing Baro into estimation.
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when commanded to fuse Baro into estimator.
**
*/
	PE_FUSE_BARO_INF_EID = 39,

/** \brief Already fusing Baro into estimation.
**  \event Already fusing Baro into estimation.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when commanded to fuse Baro into estimator.
**  but we are already fusing.
**
*/
	PE_FUSE_BARO_ERR_EID = 40,

/** \brief Disabling Baro fusion into estimation.
**  \event Disabling Baro fusion into estimation.
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when commanded to disable Baro fusion into estimator.
**
*/
	PE_DISABLE_BARO_INF_EID = 41,

/** \brief Failed to disable Baro fusion into estimation.
**  \event Failed to disable Baro fusion into estimation.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when commanded to disable Baro fusion into estimator
**  but it is not already fusing.
**
*/
	PE_DISABLE_BARO_ERR_EID = 42,

/** \brief Fusing land into estimation.
**  \event Fusing land into estimation.
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when commanded to fuse Land into estimator.
**
*/
	PE_FUSE_LAND_INF_EID = 43,

/** \brief Already fusing land into estimation.
**  \event Already fusing land into estimation.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when commanded to fuse Land into estimator.
**  but we are already fusing.
**
*/
	PE_FUSE_LAND_ERR_EID = 44,

/** \brief Disabling land fusion into estimation.
**  \event Disabling land fusion into estimation.
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when commanded to disable Land fusion into estimator.
**
*/
	PE_DISABLE_LAND_INF_EID = 45,

/** \brief Failed to disable land fusion into estimation.
**  \event Failed to disable land fusion into estimation.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when commanded to disable flow fusion into estimator
**  but it is not already fusing.
**
*/
	PE_DISABLE_LAND_ERR_EID = 46,

/** \brief Fusing flow into estimation.
**  \event Fusing flow into estimation.
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when commanded to fuse flow into estimator.
**
*/
	PE_FUSE_FLOW_INF_EID = 47,

/** \brief Already fusing flow into estimation.
**  \event Already fusing flow into estimation.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when commanded to fuse flow into estimator.
**  but we are already fusing.
**
*/
	PE_FUSE_FLOW_ERR_EID = 48,

/** \brief Disabling flow fusion into estimation.
**  \event Disabling flow fusion into estimation.
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when commanded to disable flow fusion into estimator.
**
*/
	PE_DISABLE_FLOW_INF_EID = 49,

/** \brief Failed to disable flow fusion into estimation.
**  \event Failed to disable flow fusion into estimation.
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when commanded to disable flow fusion into estimator
**  but it is not already fusing.
**
*/
	PE_DISABLE_FLOW_ERR_EID = 50,

/** \brief Flow fault, r %5.2f m, beta %5.2f
**  \event Flow fault, r %5.2f m, beta %5.2f
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the flow fails a validity check.
**  flow will reinitialize after this event is broadcast.
**
*/
	PE_FLOW_FAULT_ERR_EID = 51,

/** \brief Flow initialized. Mean: (%d) Std dev: (%d) cm
**  \event Flow initialized. Mean: (%d) Std dev: (%d) cm
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when flow has (re)initialized successfully.
**
*/
	PE_FLOW_OK_INF_EID = 52,

/** \brief Flow timeout
**  \event Flow timeout
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the flow data times out and needs to be reset
**
*/
	PE_FLOW_TIMEOUT_ERR_EID = 53,

/** \brief <tt> 'Table validation failed for parameter (%s)' </tt>
**  \event <tt> 'Table validation failed for parameter (%s)' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the PE configuration table fails to validate.
**
*/
	PE_TBL_VALIDATE_ERR_EID = 54,

/** \brief Sending diagnostic telemetry.
**  \event Sending diagnostic telemetry.
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when commanded to send diagnostic telemetry.
**
*/
	PE_SEND_DIAG_INF_EID =  55,

/** \brief <tt> This is a count of all the app events and should not be used. </tt> */
	PE_EVT_CNT
} PE_EventIds_t;


#ifdef __cplusplus
}
#endif

#endif /* PE_EVENTS_H */

/************************/
/*  End of File Comment */
/************************/
    
