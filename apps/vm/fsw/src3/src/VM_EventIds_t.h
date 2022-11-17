#ifndef VM_EVENTIDS_T_H
#define VM_EVENTIDS_T_H

enum VM_EventIds_t {
	/**
	 * \brief <tt> Value of zero is reserved, and should not be used. </tt>
	 */
	VM_RESERVED_EID = 0, 
	/**
	 * \brief <tt> 'Initialized. Version \%d.\%d.\%d.\%d' </tt>
	 * *  \event <tt> 'Initialized. Version \%d.\%d.\%d.\%d' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the VM task has completed
	 * *  initialization.
	 * *
	 * *  The first \c %d field contains the application major version defined
	 * *      in #VM_MAJOR_VERSION.
	 * *  The second \c %d field contains the application minor version defined
	 * *      in #VM_MINOR_VERSION.
	 * *  The third \c %d field contains the application revision number defined
	 * *      in #VM_REVISION.
	 * *  The fourth \c %d field contains the application revision number defined
	 * *      in #VM_MISSION_REV.
	 */
	VM_INIT_INF_EID = 1, 
	/**
	 * \brief <tt> 'Recvd NOOP. Version \%d.\%d.\%d.\%d' </tt>
	 * *  \event <tt> 'Recvd NOOP. Version \%d.\%d.\%d.\%d' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task receives a NoOp
	 * *  command.
	 * *
	 * *  The first \c %u field contains the application major version defined
	 * *      in #VM_MAJOR_VERSION.
	 * *  The first \c %d field contains the application major version defined
	 * *      in #VM_MAJOR_VERSION.
	 * *  The second \c %d field contains the application minor version defined
	 * *      in #VM_MINOR_VERSION.
	 * *  The third \c %d field contains the application revision number defined
	 * *      in #VM_REVISION.
	 * *  The fourth \c %d field contains the application revision number defined
	 * *      in #VM_MISSION_REV.
	 */
	VM_CMD_NOOP_EID = 2, 
	/**
	 * \brief <tt> '\%s Pipe failed to subscribe to \%s. (0x\%08X)' </tt>
	 * *  \event <tt> '\%s Pipe failed to subscribe to \%s. (0x\%08X)' </tt>
	 * *
	 * *  \par Type: ERROR
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task fails to subscribe
	 * *  to a message at initialization.
	 * *
	 * *  The first \c %s field contains the name of the pipe.
	 * *  The second \c %s field contains the macro name of the message.
	 * *  The 32 bit hexadecimal number is the error code returned by CFE.
	 * *
	 */
	VM_SUBSCRIBE_ERR_EID = 3, 
	/**
	 * \brief <tt> 'Failed to create \%s pipe (0x\%08X)' </tt>
	 * *  \event <tt> 'Failed to create \%s pipe (0x\%08X' </tt>
	 * *
	 * *  \par Type: ERROR
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task fails to create a
	 * *  CFE Software Bus pipe at initialization.
	 * *
	 * *  The \c %s field contains the name of the pipe.
	 * *  The 32 bit hexadecimal number is the error code returned by CFE.
	 * *
	 */
	VM_PIPE_INIT_ERR_EID = 4, 
	/**
	 * \brief <tt> 'Failed to update parameter: %s' </tt>
	 * *  \event <tt> 'Failed to update parameter: %s' </tt>
	 * *
	 * *  \par Type: ERROR
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the application fails to
	 * *  update one of its named parameters.
	 * *
	 */
	VM_PARAM_UPDATE_ERR_EID = 5, 
	/**
	 * \brief <tt> 'Failed to manage Config table (0x\%08X)' </tt>
	 * *  \event <tt> 'Failed to manage Config table (0x\%08X)' </tt>
	 * *
	 * *  \par Type: ERROR
	 * *
	 * *  \par Cause:
	 * *
	 * *  The 32 bit hexadecimal number is the error code returned by CFE
	 * *  #CFE_TBL_Manage function.
	 * *
	 */
	VM_CFGTBL_MANAGE_ERR_EID = 6, 
	/**
	 * \brief <tt> 'Failed to get Config table's address (0x\%08X)' </tt>
	 * *  \event <tt> 'Failed to get Config table's address (0x\%08X)' </tt>
	 * *
	 * *  \par Type: ERROR
	 * *
	 * *  \par Cause:
	 * *
	 * *  The 32 bit hexadecimal number is the error code returned by CFE
	 * *  #CFE_TBL_GetAddress function.
	 * *
	 */
	VM_CFGTBL_GETADDR_ERR_EID = 7, 
	/**
	 * \brief <tt> '\%s pipe read error (0x\%08X).' </tt>
	 * *  \event <tt> '\%s pipe read error (0x\%08X).' </tt>
	 * *
	 * *  \par Type: ERROR
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task has
	 * *  had an error reading from a pipe.
	 * *
	 * *  The \c %s field contains the name of the pipe.
	 * *  The 32 bit hexadecimal number is the error code returned by CFE.
	 * *
	 */
	VM_RCVMSG_ERR_EID = 8, 
	/**
	 * \brief <tt> 'Recvd invalid \%s msgId (0x\%04X)' </tt>
	 * *  \event <tt> 'Recvd invalid \%s msgId (0x\%04X)' </tt>
	 * *
	 * *  \par Type: ERROR
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task has received an invalid
	 * *  message ID.
	 * *
	 * *  The \c %s field contains the name of the pipe.
	 * *  The 16 bit hexadecimal number is the actual message ID received.
	 */
	VM_MSGID_ERR_EID = 9, 
	/**
	 * \brief <tt> 'Recvd invalid command code (\%u)' </tt>
	 * *  \event <tt> 'Recvd invalid command code (\%u)' </tt>
	 * *
	 * *  \par Type: ERROR
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task has received an invalid
	 * *  message ID.
	 * *
	 * *  The %u field contains the actual command code received.
	 */
	VM_CC_ERR_EID = 10, 
	/**
	 * \brief <tt> 'Rcvd invalid msgLen: msgId=0x\%08X, cmdCode=\%d, msgLen=\%d, expectedLen=\%d" </tt>
	 * *  \event <tt> 'Rcvd invalid msgLen: msgId=0x\%08X, cmdCode=\%d, msgLen=\%d, expectedLen=\%d" </tt>
	 * *
	 * *  \par Type: ERROR
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VMTask has
	 * *  received a message with an invalid length.
	 * *
	 * *  The first \c %d field contains the message ID.
	 * *  The second \c %d field contains the command code.
	 * *  The third \c %d field contains the actual length.
	 * *  The fourth \c %d field contains the expected length.
	 */
	VM_MSGLEN_ERR_EID = 11, 
	/**
	 * \brief <tt> 'Failed to register config table (0x%08X)' </tt>
	 * *  \event <tt> 'Failed to register config table (0x%08X)' </tt>
	 * *
	 * *  \par Type: ERROR
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task fails to
	 * *  register the VM configuration table.
	 * *
	 * *  The 32 bit hexadecimal number is the error code returned by CFE
	 * *  #CFE_TBL_Register function.
	 * *
	 */
	VM_CFGTBL_REG_ERR_EID = 12, 
	/**
	 * \brief <tt> 'Failed to load Config Table (0x%08X)' </tt>
	 * *  \event <tt> 'Failed to load Config Table (0x%08X)' </tt>
	 * *
	 * *  \par Type: ERROR
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task fails to
	 * *  load the VM configuration table.
	 * *
	 * *  The 32 bit hexadecimal number is the error code returned by CFE
	 * *  #CFE_TBL_Load function.
	 * *
	 */
	VM_CFGTBL_LOAD_ERR_EID = 13, 
	/**
	 * \brief <tt> 'Illegal Arming transition. [%s] Command rejected.' </tt>
	 * *  \event <tt> 'Illegal Arming transition. [%s] Command rejected.' </tt>
	 * *
	 * *  \par Type: ERROR
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task executes
	 * *  illegal arming state transition.
	 * *
	 * *  The first \c %s field contains the command name.
	 * *
	 */
	VM_ARMING_ILLEGAL_TRANSITION_ERR_EID = 14, 
	/**
	 * \brief <tt> 'Illegal Nav transition [%s -> %s].  Command rejected.' </tt>
	 * *  \event <tt> 'Illegal Nav transition [%s -> %s].  Command rejected.' </tt>
	 * *
	 * *  \par Type: ERROR
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task executes
	 * *  illegal navigation state transition.
	 * *
	 */
	VM_NAV_ILLEGAL_TRANSITION_ERR_EID = 15, 
	/**
	 * \brief <tt> 'Arming::Standby' </tt>
	 * *  \event <tt> 'Arming::Standby' </tt>
	 * *
	 * *  \par Type: ERROR
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task enters
	 * *  arming - standby state.
	 * *
	 * *
	 */
	VM_ARMING_ENTERED_STANDBY_STATE_INFO_EID = 16, 
	/**
	 * \brief <tt> 'Arming::Armed' </tt>
	 * *  \event <tt> 'Arming::Armed' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task enters
	 * *  arming - armed state.
	 * *
	 */
	VM_ARMING_ENTERED_ARMED_STATE_INFO_EID = 17, 
	/**
	 * \brief <tt> 'Arming::StandbyError' </tt>
	 * *  \event <tt> 'Arming::StandbyError' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task enters
	 * *  arming - standby error state.
	 */
	VM_ARMING_ENTERED_STANDBY_ERROR_STATE_INFO_EID = 18, 
	/**
	 * \brief <tt> 'Arming::ArmedError' </tt>
	 * *  \event <tt> 'Arming::ArmedError' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task enters
	 * *  arming - armed error state.
	 * *
	 */
	VM_ARMING_ENTERED_ARMED_ERROR_STATE_INFO_EID = 19, 
	/**
	 * \brief <tt> 'Navigation::Manual' </tt>
	 * *  \event <tt> 'Navigation::Manual' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task enters
	 * *  manual navigation mode.
	 * *
	 */
	VM_NAVSM_ENTERED_MANUAL_INFO_EID = 20, 
	/**
	 * \brief <tt> 'Navigation::AltitudeControl' </tt>
	 * *  \event <tt> 'Navigation::AltitudeControl' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task enters
	 * *  atlitude control navigation mode.
	 * *
	 */
	VM_NAVSN_ENTERED_ALTCTL_INFO_EID = 21, 
	/**
	 * \brief <tt> 'Navigation::PositionControl' </tt>
	 * *  \event <tt> 'Navigation::PositionControl' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task enters
	 * *  position control navigation mode.
	 * *
	 */
	VM_NAVSN_ENTERED_POSCTL_INFO_EID = 22, 
	/**
	 * \brief <tt> 'Navigation::AutoLoiter' </tt>
	 * *  \event <tt> 'Navigation::AutoLoiter' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task enters
	 * *  auto loiter navigation mode.
	 * *
	 */
	VM_NAVSN_ENTERED_AUTOLOITER_INFO_EID = 23, 
	/**
	 * \brief <tt> 'Navigation::AutoRTL' </tt>
	 * *  \event <tt> 'Navigation::AutoRTL' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task enters
	 * *  auto return to launch navigation mode.
	 * *
	 */
	VM_NAVSN_ENTERED_RTL_INFO_EID = 24, 
	/**
	 * \brief <tt> 'Navigation::Acrobatic' </tt>
	 * *  \event <tt> 'Navigation::Acrobatic' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task enters
	 * *  acrobatic navigation mode.
	 * *
	 */
	VM_NAVSN_ENTERED_ACRO_INFO_EID = 25, 
	/**
	 * \brief <tt> 'Navigation::Stabilize' </tt>
	 * *  \event <tt> 'Navigation::Stabilize' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task enters
	 * *  stabilized naviagation mode.
	 * *
	 */
	VM_NAVSN_ENTERED_STABILIZE_INFO_EID = 26, 
	/**
	 * \brief <tt> 'Navigation::Rattitude' </tt>
	 * *  \event <tt> 'Navigation::Rattitude' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task enters
	 * *  rattitude navigation mode.
	 * *
	 * *
	 */
	VM_NAVSN_ENTERED_RATTITUDE_INFO_EID = 27, 
	/**
	 * \brief <tt> 'Navigation::AutoTakeoff' </tt>
	 * *  \event <tt> 'Navigation::AutoTakeoff' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task enters
	 * *  auto takeoff navigation mode.
	 * *
	 */
	VM_NAVSN_ENTERED_AUTO_TAKEOFF_INFO_EID = 28, 
	/**
	 * \brief <tt> 'Navigation::AutoLand' </tt>
	 * *  \event <tt> 'Navigation::AutoLand' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task enters
	 * *  auto land navigation mode.
	 * *
	 */
	VM_NAVSN_ENTERED_AUTO_LAND_INFO_EID = 29, 
	/**
	 * \brief <tt> 'VM_NavigationMap is in unknown state (%u, '%s')' </tt>
	 * *  \event <tt> 'VM_NavigationMap is in unknown state (%u, '%s')' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task enters
	 * *  into unknown navigation state.
	 * *
	 * *  The first \c %u field contains the navigation state ID.
	 * *  The second \c %s field contains the navigation state name.
	 * *
	 */
	VM_IN_UNKNOWN_STATE_ERR_EID = 30, 
	/**
	 * \brief <tt> 'Home Position set to Lat (%.2f) Lon (%.2f) Alt (%.2f)' </tt>
	 * *  \event <tt> 'Home Position set to Lat (%.2f) Lon (%.2f) Alt (%.2f)' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task set
	 * *  vehicle home position.
	 * *
	 * *  The first \c %.2f field contains the home latitude.
	 * *  The second \c %.2f field contains the home longitude.
	 * *  The third \c %.2f field contains the home altitude.
	 * *
	 */
	VM_HOMESET_INFO_EID = 31, 
	/**
	 * \brief <tt> 'Flight initialized' </tt>
	 * *  \event <tt> 'Flight initialized' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task resets
	 * *  flight parameters.
	 * *
	 */
	VM_LND_INIT_INFO_EID = 32, 
	/**
	 * \brief <tt> 'Vehicle disarming by safety message' </tt>
	 * *  \event <tt> 'Vehicle disarming by safety message' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the pilot arms
	 * *  vehicle while the safety is turned ON.
	 * *
	 */
	VM_SAFETY_DISARM_INFO_EID = 33, 
	/**
	 * \brief <tt> 'Safety is OFF/ON' </tt>
	 * *  \event <tt> 'Safety is OFF/ON' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task safety
	 * *  if changed to either OFF or ON.
	 * *
	 */
	VM_SAFETY_INFO_EID = 34, 
	/**
	 * \brief <tt> 'Low battery, return to land/takeoff discouraged' </tt>
	 * *  \event <tt> 'Low battery, return to land/takeoff discouraged' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task battery
	 * *  is low, executes one of two contingency actions based
	 * *  on arming status.
	 * *
	 */
	VM_LOW_BAT_INFO_EID = 35, 
	/**
	 * \brief <tt> 'Critical battery, shutdown system/return to launch' </tt>
	 * *  \event <tt> 'Critical battery, shutdown system/return to launch' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task battery
	 * *  is critical, executes one of two contingency actions based
	 * *  on arming status.
	 * *
	 */
	VM_CRITICAL_BAT_INFO_EID = 36, 
	/**
	 * \brief <tt> 'Dangerously low battery, shutdown system/landing immediately' </tt>
	 * *  \event <tt> 'Dangerously low battery, shutdown system/landing immediately' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task battery
	 * *  is dangerously low, executes one of two contingency actions based
	 * *  on arming status.
	 * *
	 */
	VM_DANGER_BAT_LEVEL_INFO_EID = 37, 
	/**
	 * \brief <tt> 'Manual control regained after (%llu)ums' </tt>
	 * *  \event <tt> 'Manual control regained after (%llu)ums' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task regains
	 * *  manual rc control.
	 * *
	 * *  The first \c %llu field contains the timestamp of event.
	 * *
	 */
	VM_RC_SIGN_REGAINED_INFO_EID = 38, 
	/**
	 * \brief <tt> 'Manual control lost at t = (%llu)ums' </tt>
	 * *  \event <tt> 'Manual control lost at t = (%llu)ums' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task loses
	 * *  manual rc control.
	 * *
	 * *  The first \c %llu field contains the timestamp of event.
	 * *
	 */
	VM_RC_SIGN_LOST_INFO_EID = 39, 
	/**
	 * \brief <tt> 'Stick disarm rejected, vehicle in flight' </tt>
	 * *  \event <tt> 'Stick disarm rejected, vehicle in flight' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task rejects
	 * *  disarm since vehicle is in flight.
	 * *
	 */
	VM_RC_STK_DISARM_REJ_INFO_EID = 40, 
	/**
	 * \brief <tt> 'Stick arm rejected, vehicle not in manual mode ' </tt>
	 * *  \event <tt> 'Stick arm rejected, vehicle not in manual mode ' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task rejects
	 * *  arming since vehicle not in manual mode.
	 * *
	 */
	VM_RC_STK_ARM_REJ_INFO_EID = 41, 
	/**
	 * \brief <tt> 'Disarm engaged by rc' </tt>
	 * *  \event <tt> 'Disarm engaged by rc' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task engages
	 * *  disarm action, called by rc.
	 * *
	 */
	VM_RC_DISARM_INFO_EID = 42, 
	/**
	 * \brief <tt> 'Arm engaged by rc' </tt>
	 * *  \event <tt> 'Arm engaged by rc' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task  engages
	 * *  arm action, called by rc.
	 * *
	 */
	VM_RC_ARM_INFO_EID = 43, 
	/**
	 * \brief <tt> 'Killswitch engaged/disengaged' </tt>
	 * *  \event <tt> 'Killswitch engaged/disengaged' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task engages/
	 * *  disengages killswitch.
	 * *
	 */
	VM_RC_KIL_SWTCH_INFO_EID = 44, 
	/**
	 * \brief <tt> 'Mode switched to auto loiter by rc' </tt>
	 * *  \event <tt> 'Mode switched to auto loiter by rc' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task switch
	 * *  navigation mode to auto loiter from rc.
	 * *
	 */
	VM_RC_LTR_INFO_EID = 45, 
	/**
	 * \brief <tt> 'Mode switched to auto rtl by rc' </tt>
	 * *  \event <tt> 'Mode switched to auto rtl by rc' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task  switch
	 * *  navigation mode to auto rtl from rc.
	 * *
	 */
	VM_RC_RTL_INFO_EID = 46, 
	/**
	 * \brief <tt> 'Mode switched to position control by rc' </tt>
	 * *  \event <tt> 'Mode switched to position control by rc' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task  switch
	 * *  navigation mode to position control from rc.
	 * *
	 */
	VM_RC_POSCTL_INFO_EID = 47, 
	/**
	 * \brief <tt> 'Mode switched to Manual by rc/autonomously' </tt>
	 * *  \event <tt> 'Mode switched to Manual by rc/autonomously' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task  switch
	 * *  navigation mode to manual control from rc or as a fall back.
	 * *
	 */
	VM_RC_MAN_INFO_EID = 48, 
	/**
	 * \brief <tt> 'Mode switched to auto takeoff by rc' </tt>
	 * *  \event <tt> 'Mode switched to auto takeoff by rc' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task  switch
	 * *  navigation mode to auto takeoff from rc.
	 * *
	 */
	VM_RC_TAKE_OFF_INFO_EID = 49, 
	/**
	 * \brief <tt> 'Low battery, cannot arm' </tt>
	 * *  \event <tt> 'Low battery, cannot arm' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task trying to arm
	 * *  in low battery.
	 * *
	 */
	VM_PRE_ARM_BAT_CHECK_INFO_EID = 50, 
	/**
	 * \brief <tt> 'Safety is ON, cannot arm' </tt>
	 * *  \event <tt> 'Safety is ON, cannot arm' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task trying to arm
	 * *  with safety on.
	 * *
	 */
	VM_PRE_ARM_SAFETY_CHECK_INFO_EID = 51, 
	/**
	 * \brief <tt> 'Sensors not setup correctly' </tt>
	 * *  \event <tt> 'Sensors not setup correctly' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task sensors are
	 * *  not setup correctly.
	 * *
	 */
	VM_PRE_ARM_SENSORS_CHECK_INFO_EID = 52, 
	/**
	 * \brief <tt> 'Sensors not ready' </tt>
	 * *  \event <tt> 'Sensors not ready' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task sensors
	 * *  not ready yet.
	 * *
	 */
	VM_SEN_NOT_READY_INFO_EID = 53, 
	/**
	 * \brief <tt> 'Position hold mode requirement failed' </tt>
	 * *  \event <tt> 'Position hold mode requirement failed' </tt>
	 * *
	 * *  \par Type: INFORMATION
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task fails position hold
	 * *  mode switch guard.
	 * *
	 */
	VM_REQ_POS_CTL_ERR_EID = 54, 
	/**
	 * \brief <tt> 'Altitude hold mode requirement failed' </tt>
	 * *  \event <tt> 'Altitude hold mode requirement failed' </tt>
	 * *
	 * *  \par Type: ERROR
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task fails altitude hold
	 * *  mode switch guard.
	 * *
	 */
	VM_REQ_ALT_CTL_ERR_EID = 55, 
	/**
	 * \brief <tt> 'Acrobatic mode requirement failed' </tt>
	 * *  \event <tt> 'Acrobatic mode requirement failed' </tt>
	 * *
	 * *  \par Type: ERROR
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task fails acrobatic
	 * *  mode switch guard.
	 * *
	 */
	VM_REQ_ACRO_ERR_EID = 56, 
	/**
	 * \brief <tt> 'WARN!!! position estimation not initialized' </tt>
	 * *  \event <tt> 'WARN!!! position estimation not initialized' </tt>
	 * *
	 * *  \par Type: ERROR
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the CFS VM Task starts executing
	 * *  without position estimator initialized
	 * *
	 */
	VM_NOPE_ERR_EID = 57, 
	/**
	 * \brief <tt> 'Set config parameter %s to %u' </tt>
	 * *  \event <tt> 'Set config parameter COM_LOW_BAT_ACT to %u' </tt>
	 * *
	 * *  \par Type: ERROR
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when a configuration parameter is set by
	 * *  command.
	 * *
	 */
	VM_CONFIG_CHANGE_EID = 58, 
	/**
	 * \brief <tt> 'Configuration parameter '%s = %u' out of bounds.' </tt>
	 * *  \event <tt> 'Configuration parameter 'COM_LOW_BAT_ACT' = %u' out of bounds.' </tt>
	 * *
	 * *  \par Type: ERROR
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when a configuration parameter is set by
	 * *  command.
	 * *
	 */
	VM_CONFIG_PARAMETER_OUT_OF_BOUNDS_EID = 59, 
	/**
	 * \brief <tt> 'Failed to initialize parameter: %s' </tt>
	 * *  \event <tt> 'Failed to initialize parameter: %s' </tt>
	 * *
	 * *  \par Type: ERROR
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the application fails to
	 * *  register all the named parameters.
	 * *
	 */
	VM_PARAM_INIT_ERR_EID = 60, 
	/**
	 * \brief <tt> 'Invalid Config Table: %s' </tt>
	 * *  \event <tt> 'Invalid Config Table.  COM_RC_IN_MODE.  Exp:<0-%u>  Act: %u', #COM_RC_IN_MODE_MAX </tt>
	 * *  \event <tt> 'Invalid Config Table.  COM_ARM_SWISBTN.  Exp:<0-1>  Act: %u' </tt>
	 * *  \event <tt> 'Invalid Config Table.  COM_RC_ARM_HYST.  Exp:<%u-%u>  Act: %u', #COM_RC_ARM_HYST_MIN, #COM_RC_ARM_HYST_MAX </tt>
	 * *  \event <tt> 'Invalid Config Table.  MAV_SYS_ID.  Exp:<%u-%u>  Act: %u', #MAV_SYS_ID_MIN, #MAV_SYS_ID_MAX </tt>
	 * *  \event <tt> 'Invalid Config Table.  MAV_COMP_ID.  Exp:<%u-%u>  Act: %u', #MAV_COMP_ID_MIN, #MAV_COMP_ID_MAX </tt>
	 * *  \event <tt> 'Invalid Config Table.  COM_RC_LOSS_T.  Exp:<0.0-%f>  Act: %f', #COM_RC_LOSS_T_MAX </tt>
	 * *  \event <tt> 'Invalid Config Table.  COM_LOW_BAT_ACT.  Exp:<0-%u>  Act: %u', #COM_LOW_BAT_ACT_MAX </tt>
	 * *  \event <tt> 'Invalid Config Table.  COM_HOME_H_T.  Exp:<%f-%f>  Act: %f', #COM_HOME_H_T_MIN, #COM_HOME_H_T_MAX </tt>
	 * *  \event <tt> 'Invalid Config Table.  COM_HOME_V_T.  Exp:<%f-%f>  Act: %f', #COM_HOME_V_T_MIN, #COM_HOME_V_T_MAX </tt>
	 * *
	 * *  \par Type: ERROR
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the application fails to
	 * *  validate a table or a table parameter.
	 * *
	 */
	VM_INVALID_CONFIG_TABLE_ERR_EID = 61, 
	/**
	 * \brief <tt> 'Home position cannot be set.' </tt>
	 * *  \event <tt> 'Home position cannot be set.' </tt>
	 * *
	 * *  \par Type: ERROR
	 * *
	 * *  \par Cause:
	 * *
	 * *  This event message is issued when the home position cannot be set
	 * *  because the Vehicle Global Position EpH and EpV is too large.
	 * *
	 */
	VM_HOMESET_ERROR_EID = 62, 
	/**
	 * \brief <tt> This is a count of all the app events and should not be used. </tt>
	 */
	VM_EVT_CNT
};

#endif
