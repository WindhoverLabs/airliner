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

#include <cfe.h>
#include <cfe_es.h>
#include <cfe_error.h>

#include "fac_tbldefs.h"
#include "fac_app.hpp"
#include "fac_fac.hpp"
#include "fac_data_utils.hpp"
#include "fac_cmds_utils.hpp"
#include "fac_msg.h"
#include "fac_version.h"

#include <string.h>

FAC_AppData_t  FAC_AppData;

FixedwingAttitudeControl objFAC;
AppCommandProcess        objCmds;
AppDataProcess           objData;


FixedwingAttitudeControl::FixedwingAttitudeControl()
{
   FAC_AppData.uiAppRunStatus = CFE_ES_APP_RUN;
   FAC_AppData.bAppAwaken = FALSE;

   FAC_AppData.ConfigTblHdl = 0;
   FAC_AppData.ConfigTblPtr = NULL;
}

FixedwingAttitudeControl::~FixedwingAttitudeControl()
{
   FAC_AppData.uiAppRunStatus = CFE_ES_APP_EXIT;
   FAC_AppData.bAppAwaken = FALSE;

   FAC_AppData.ConfigTblHdl = 0;
   FAC_AppData.ConfigTblPtr = NULL;
}

int32 FixedwingAttitudeControl::VerifySchCmdLength(CFE_SB_Msg_t *MsgPtr, uint16 usExpectedLen)
{
   uint16  usMsgLen = 0;
   int32   iStatus  = CFE_SUCCESS;

   if (MsgPtr != NULL)
   {
      usMsgLen = CFE_SB_GetTotalMsgLength(MsgPtr);

      if (usExpectedLen != usMsgLen)
      {
         iStatus = FAC_ERR_MSG_LENGTH;
         CFE_SB_MsgId_t MsgId = CFE_SB_GetMsgId(MsgPtr);
         uint16 usCmdCode = CFE_SB_GetCmdCode(MsgPtr);

         CFE_EVS_SendEvent(FAC_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                           "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                           "msgLen=%d, expectedLen=%d", MsgId, usCmdCode,
                           usMsgLen, usExpectedLen);
      }
   }
   else
   {
      iStatus = FAC_ERR_INVALID_POINTER;
   }

   return (iStatus);
}

void FixedwingAttitudeControl::ReportHousekeeping()
{
   int32 iStatus = CFE_SUCCESS;

   /* TODO:  Add code to update housekeeping data, if needed, here.  */

#if 1
   CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&FAC_AppData.HkTlm);
   iStatus = CFE_SB_SendMsg((CFE_SB_Msg_t*)&FAC_AppData.HkTlm);
   if (iStatus != CFE_SUCCESS)
   {
      /* TODO: Decide what to do if the send message fails. */
   }
#endif

   return;
}
int32 FixedwingAttitudeControl::SendHkMsg(CFE_SB_Msg_t *MsgPtr, CFE_SB_MsgId_t MsgId)
{
   int32 iStatus = CFE_SUCCESS;

   iStatus = VerifySchCmdLength(MsgPtr, sizeof(FAC_NoArgCmd_t));
   if (iStatus == CFE_SUCCESS)
   {
      CFE_EVS_SendEvent(FAC_INF_EID, CFE_EVS_INFORMATION,
                        "Recvd FAC_SEND_HK_MID msgId (0x%04X)",
                        (unsigned short)MsgId);
      if (FAC_AppData.bAppAwaken == TRUE)
      {
         ReportHousekeeping();
      }
   }
   else if (iStatus == FAC_ERR_INVALID_POINTER)
   {
      CFE_EVS_SendEvent(FAC_ERR_EID, CFE_EVS_ERROR, "Invalid Msg Pointer (0x%04X)",
                        (unsigned short)MsgId);
      FAC_AppData.HkTlm.usCmdErrCnt++;
   }
   else
   {
      FAC_AppData.HkTlm.usCmdErrCnt++;
   }

   return (iStatus);
}

int32 FixedwingAttitudeControl::WakeupValidate(CFE_SB_Msg_t *MsgPtr, CFE_SB_MsgId_t MsgId)
{
   int32 iStatus = CFE_SUCCESS;

   iStatus = VerifySchCmdLength(MsgPtr, sizeof(FAC_NoArgCmd_t));
   if (iStatus == CFE_SUCCESS)
   {
      CFE_EVS_SendEvent(FAC_INF_EID, CFE_EVS_INFORMATION,
                        "Recvd FAC_WAKEUP_MID msgId (0x%04X)",
                        (unsigned short)MsgId);

      FAC_AppData.bAppAwaken = TRUE;

      /* TODO:  Add more code here to handle other things when app wakes up */
   }
   else if (iStatus == FAC_ERR_INVALID_POINTER)
   {
      CFE_EVS_SendEvent(FAC_ERR_EID, CFE_EVS_ERROR, "Invalid Msg Pointer (0x%04X)",
                        (unsigned short)MsgId);
      FAC_AppData.HkTlm.usCmdErrCnt++;
      goto WakeupMsg_Exit_Tag;
   }
   else
   {
      FAC_AppData.HkTlm.usCmdErrCnt++;
      goto WakeupMsg_Exit_Tag;
   }

WakeupMsg_Exit_Tag:
   return (iStatus);
}

int32 FixedwingAttitudeControl::RcvSchMsg(int32 iBlocking)
{
   int32           iStatus = CFE_SUCCESS;
   CFE_SB_Msg_t*   MsgPtr = NULL;
   CFE_SB_MsgId_t  MsgId;

   /* Stop Performance Log entry */
   CFE_ES_PerfLogExit(FAC_MAIN_TASK_PERF_ID);

   /* Wait for WakeUp messages from scheduler */
   iStatus = CFE_SB_RcvMsg(&MsgPtr, SchPipeId, iBlocking);

   /* Start Performance Log entry */
   CFE_ES_PerfLogEntry(FAC_MAIN_TASK_PERF_ID);   // check when iStatus was failed

   if (iStatus == CFE_SUCCESS)
   {
      MsgId = CFE_SB_GetMsgId(MsgPtr);
      switch (MsgId)
      {
         case FAC_WAKEUP_MID:
            iStatus = WakeupValidate(MsgPtr, MsgId);    // can be removed later
            if (iStatus != CFE_SUCCESS)
            {
               goto RcvSchMsg_Exit_Tag;
            }

            iStatus = Execute();
            if (iStatus != CFE_SUCCESS)
            {
               goto RcvSchMsg_Exit_Tag;
            }

            /* The last thing to do at the end of this Wakeup cycle should be to
             * automatically publish new output. */
            objData.SendOutData();
            break;

         case FAC_SEND_HK_MID:
            iStatus = SendHkMsg(MsgPtr, MsgId);
            if (iStatus != CFE_SUCCESS)
            {
               goto RcvSchMsg_Exit_Tag;
            }
            break;

         default:
            CFE_EVS_SendEvent(FAC_MSGID_ERR_EID, CFE_EVS_ERROR,
                              "Recvd invalid SCH msgId (0x%04X)",
                              (unsigned short)MsgId);
            FAC_AppData.HkTlm.usCmdErrCnt++;
            break;
      }
   }
   else if (iStatus == CFE_SB_NO_MESSAGE)
   {
      /* TODO: If there's no incoming message, you can do something here, or
       * nothing.  Note, this section is dead code only if the iBlocking arg
       * is CFE_SB_PEND_FOREVER. */
      iStatus = CFE_SUCCESS;
   }
   else if (iStatus == CFE_SB_TIME_OUT)
   {
      /* TODO: If there's no incoming message within a specified time (via the
       * iBlocking arg, you can do something here, or nothing.
       * Note, this section is dead code only if the iBlocking arg
       * is CFE_SB_PEND_FOREVER. */
      iStatus = CFE_SUCCESS;
   }
   else
   {
      /* TODO: This is an example of exiting on an error (either CFE_SB_BAD_ARGUMENT,
       *  or CFE_SB_PIPE_RD_ERROR).
       */
      CFE_EVS_SendEvent(FAC_PIPE_ERR_EID, CFE_EVS_ERROR,
			"SB pipe read error (0x%08X), app will exit",
                        (unsigned int)iStatus);
      FAC_AppData.uiAppRunStatus= CFE_ES_APP_ERROR;
   }

RcvSchMsg_Exit_Tag:
   return (iStatus);
}

int32 FixedwingAttitudeControl::Execute()
{
	int32 status = CFE_SUCCESS;


//	static uint64_t last_run = 0;
//	float deltaT = (hrt_absolute_time() - last_run) / 1000000.0f;
//	last_run = hrt_absolute_time();
//
//	/* guard against too large deltaT's */
//	if (deltaT > 1.0f) {
//		deltaT = 0.01f;
//	}
//
//	/* load local copies */
//	orb_copy(ORB_ID(vehicle_attitude), _att_sub, &_att);
//
//	/* get current rotation matrix and euler angles from control state quaternions */
//	math::Quaternion q_att(_att.q[0], _att.q[1], _att.q[2], _att.q[3]);
//	_R = q_att.to_dcm();
//
//	math::Vector<3> euler_angles;
//	euler_angles = _R.to_euler();
//	_roll    = euler_angles(0);
//	_pitch   = euler_angles(1);
//	_yaw     = euler_angles(2);
//
//	if (_vehicle_status.is_vtol && _parameters.vtol_type == vtol_type::TAILSITTER) {
//		/* vehicle is a tailsitter, we need to modify the estimated attitude for fw mode
//		 *
//		 * Since the VTOL airframe is initialized as a multicopter we need to
//		 * modify the estimated attitude for the fixed wing operation.
//		 * Since the neutral position of the vehicle in fixed wing mode is -90 degrees rotated around
//		 * the pitch axis compared to the neutral position of the vehicle in multicopter mode
//		 * we need to swap the roll and the yaw axis (1st and 3rd column) in the rotation matrix.
//		 * Additionally, in order to get the correct sign of the pitch, we need to multiply
//		 * the new x axis of the rotation matrix with -1
//		 *
//		 * original:			modified:
//		 *
//		 * Rxx  Ryx  Rzx		-Rzx  Ryx  Rxx
//		 * Rxy	Ryy  Rzy		-Rzy  Ryy  Rxy
//		 * Rxz	Ryz  Rzz		-Rzz  Ryz  Rxz
//		 * */
//		math::Matrix<3, 3> R_adapted = _R;		//modified rotation matrix
//
//		/* move z to x */
//		R_adapted(0, 0) = _R(0, 2);
//		R_adapted(1, 0) = _R(1, 2);
//		R_adapted(2, 0) = _R(2, 2);
//
//		/* move x to z */
//		R_adapted(0, 2) = _R(0, 0);
//		R_adapted(1, 2) = _R(1, 0);
//		R_adapted(2, 2) = _R(2, 0);
//
//		/* change direction of pitch (convert to right handed system) */
//		R_adapted(0, 0) = -R_adapted(0, 0);
//		R_adapted(1, 0) = -R_adapted(1, 0);
//		R_adapted(2, 0) = -R_adapted(2, 0);
//		euler_angles = R_adapted.to_euler();  //adapted euler angles for fixed wing operation
//
//		/* fill in new attitude data */
//		_R = R_adapted;
//		_roll    = euler_angles(0);
//		_pitch   = euler_angles(1);
//		_yaw     = euler_angles(2);
//
//		/* lastly, roll- and yawspeed have to be swaped */
//		float helper = _att.rollspeed;
//		_att.rollspeed = -_att.yawspeed;
//		_att.yawspeed = helper;
//	}
//
//	_sub_airspeed.update();
//	vehicle_setpoint_poll();
//	vehicle_control_mode_poll();
//	vehicle_manual_poll();
//	global_pos_poll();
//	vehicle_status_poll();
//	vehicle_land_detected_poll();
//	battery_status_poll();
//
//	// the position controller will not emit attitude setpoints in some modes
//	// we need to make sure that this flag is reset
//	_att_sp.fw_control_yaw = _att_sp.fw_control_yaw && _vcontrol_mode.flag_control_auto_enabled;
//
//	/* lock integrator until control is started */
//	bool lock_integrator = !(_vcontrol_mode.flag_control_rates_enabled && !_vehicle_status.is_rotary_wing);
//
//	/* Simple handling of failsafe: deploy parachute if failsafe is on */
//	if (_vcontrol_mode.flag_control_termination_enabled) {
//		_actuators_airframe.control[7] = 1.0f;
//		//warnx("_actuators_airframe.control[1] = 1.0f;");
//
//	} else {
//		_actuators_airframe.control[7] = 0.0f;
//		//warnx("_actuators_airframe.control[1] = -1.0f;");
//	}
//
//	/* if we are in rotary wing mode, do nothing */
//	if (_vehicle_status.is_rotary_wing && !_vehicle_status.is_vtol) {
//		continue;
//	}
//
//	/* default flaps to center */
//	float flap_control = 0.0f;
//
//	/* map flaps by default to manual if valid */
//	if (PX4_ISFINITE(_manual.flaps) && _vcontrol_mode.flag_control_manual_enabled
//	    && fabsf(_parameters.flaps_scale) > 0.01f) {
//		flap_control = 0.5f * (_manual.flaps + 1.0f) * _parameters.flaps_scale;
//
//	} else if (_vcontrol_mode.flag_control_auto_enabled
//		   && fabsf(_parameters.flaps_scale) > 0.01f) {
//		flap_control = _att_sp.apply_flaps ? 1.0f * _parameters.flaps_scale : 0.0f;
//	}
//
//	// move the actual control value continuous with time, full flap travel in 1sec
//	if (fabsf(_flaps_applied - flap_control) > 0.01f) {
//		_flaps_applied += (_flaps_applied - flap_control) < 0 ? deltaT : -deltaT;
//
//	} else {
//		_flaps_applied = flap_control;
//	}
//
//	/* default flaperon to center */
//	float flaperon_control = 0.0f;
//
//	/* map flaperons by default to manual if valid */
//	if (PX4_ISFINITE(_manual.aux2) && _vcontrol_mode.flag_control_manual_enabled
//	    && fabsf(_parameters.flaperon_scale) > 0.01f) {
//		flaperon_control = 0.5f * (_manual.aux2 + 1.0f) * _parameters.flaperon_scale;
//
//	} else if (_vcontrol_mode.flag_control_auto_enabled
//		   && fabsf(_parameters.flaperon_scale) > 0.01f) {
//		flaperon_control = _att_sp.apply_flaps ? 1.0f * _parameters.flaperon_scale : 0.0f;
//	}
//
//	// move the actual control value continuous with time, full flap travel in 1sec
//	if (fabsf(_flaperons_applied - flaperon_control) > 0.01f) {
//		_flaperons_applied += (_flaperons_applied - flaperon_control) < 0 ? deltaT : -deltaT;
//
//	} else {
//		_flaperons_applied = flaperon_control;
//	}
//
//	// Check if we are in rattitude mode and the pilot is above the threshold on pitch
//	if (_vcontrol_mode.flag_control_rattitude_enabled) {
//		if (fabsf(_manual.y) > _parameters.rattitude_thres ||
//		    fabsf(_manual.x) > _parameters.rattitude_thres) {
//			_vcontrol_mode.flag_control_attitude_enabled = false;
//		}
//	}
//
//	/* decide if in stabilized or full manual control */
//	if (_vcontrol_mode.flag_control_rates_enabled) {
//		/* scale around tuning airspeed */
//		float airspeed;
//
//		/* if airspeed is non-finite or not valid or if we are asked not to control it, we assume the normal average speed */
//		const bool airspeed_valid = PX4_ISFINITE(_sub_airspeed.get().indicated_airspeed_m_s)
//					    && ((_sub_airspeed.get().timestamp - hrt_absolute_time()) < 1e6);
//
//		if (airspeed_valid) {
//			/* prevent numerical drama by requiring 0.5 m/s minimal speed */
//			airspeed = math::max(0.5f, _sub_airspeed.get().indicated_airspeed_m_s);
//
//		} else {
//			airspeed = _parameters.airspeed_trim;
//			perf_count(_nonfinite_input_perf);
//		}
//
//		/*
//		 * For scaling our actuators using anything less than the min (close to stall)
//		 * speed doesn't make any sense - its the strongest reasonable deflection we
//		 * want to do in flight and its the baseline a human pilot would choose.
//		 *
//		 * Forcing the scaling to this value allows reasonable handheld tests.
//		 */
//		float airspeed_scaling = _parameters.airspeed_trim / ((airspeed < _parameters.airspeed_min) ? _parameters.airspeed_min :
//					 airspeed);
//
//		/* Use min airspeed to calculate ground speed scaling region.
//		 * Don't scale below gspd_scaling_trim
//		 */
//		float groundspeed = sqrtf(_global_pos.vel_n * _global_pos.vel_n +
//					  _global_pos.vel_e * _global_pos.vel_e);
//		float gspd_scaling_trim = (_parameters.airspeed_min * 0.6f);
//		float groundspeed_scaler = gspd_scaling_trim / ((groundspeed < gspd_scaling_trim) ? gspd_scaling_trim : groundspeed);
//
//		// in STABILIZED mode we need to generate the attitude setpoint
//		// from manual user inputs
//		if (!_vcontrol_mode.flag_control_climb_rate_enabled && !_vcontrol_mode.flag_control_offboard_enabled) {
//			_att_sp.timestamp = hrt_absolute_time();
//			_att_sp.roll_body = _manual.y * _parameters.man_roll_max + _parameters.rollsp_offset_rad;
//			_att_sp.roll_body = math::constrain(_att_sp.roll_body, -_parameters.man_roll_max, _parameters.man_roll_max);
//			_att_sp.pitch_body = -_manual.x * _parameters.man_pitch_max + _parameters.pitchsp_offset_rad;
//			_att_sp.pitch_body = math::constrain(_att_sp.pitch_body, -_parameters.man_pitch_max, _parameters.man_pitch_max);
//			_att_sp.yaw_body = 0.0f;
//			_att_sp.thrust = _manual.z;
//
//			Quatf q(Eulerf(_att_sp.roll_body, _att_sp.pitch_body, _att_sp.yaw_body));
//			q.copyTo(_att_sp.q_d);
//			_att_sp.q_d_valid = true;
//
//			int instance;
//			orb_publish_auto(_attitude_setpoint_id, &_attitude_sp_pub, &_att_sp, &instance, ORB_PRIO_DEFAULT);
//		}
//
//		/* reset integrals where needed */
//		if (_att_sp.roll_reset_integral) {
//			_roll_ctrl.reset_integrator();
//		}
//
//		if (_att_sp.pitch_reset_integral) {
//			_pitch_ctrl.reset_integrator();
//		}
//
//		if (_att_sp.yaw_reset_integral) {
//			_yaw_ctrl.reset_integrator();
//			_wheel_ctrl.reset_integrator();
//		}
//
//		/* Reset integrators if the aircraft is on ground
//		 * or a multicopter (but not transitioning VTOL)
//		 */
//		if (_vehicle_land_detected.landed
//		    || (_vehicle_status.is_rotary_wing && !_vehicle_status.in_transition_mode)) {
//
//			_roll_ctrl.reset_integrator();
//			_pitch_ctrl.reset_integrator();
//			_yaw_ctrl.reset_integrator();
//			_wheel_ctrl.reset_integrator();
//		}
//
//		float roll_sp = _att_sp.roll_body;
//		float pitch_sp = _att_sp.pitch_body;
//		float yaw_sp = _att_sp.yaw_body;
//		float throttle_sp = _att_sp.thrust;
//
//		/* Prepare data for attitude controllers */
//		struct ECL_ControlData control_input = {};
//		control_input.roll = _roll;
//		control_input.pitch = _pitch;
//		control_input.yaw = _yaw;
//		control_input.body_x_rate = _att.rollspeed;
//		control_input.body_y_rate = _att.pitchspeed;
//		control_input.body_z_rate = _att.yawspeed;
//		control_input.roll_setpoint = roll_sp;
//		control_input.pitch_setpoint = pitch_sp;
//		control_input.yaw_setpoint = yaw_sp;
//		control_input.airspeed_min = _parameters.airspeed_min;
//		control_input.airspeed_max = _parameters.airspeed_max;
//		control_input.airspeed = airspeed;
//		control_input.scaler = airspeed_scaling;
//		control_input.lock_integrator = lock_integrator;
//		control_input.groundspeed = groundspeed;
//		control_input.groundspeed_scaler = groundspeed_scaler;
//
//		_yaw_ctrl.set_coordinated_method(_parameters.y_coordinated_method);
//
//		/* Run attitude controllers */
//		if (_vcontrol_mode.flag_control_attitude_enabled) {
//			if (PX4_ISFINITE(roll_sp) && PX4_ISFINITE(pitch_sp)) {
//				_roll_ctrl.control_attitude(control_input);
//				_pitch_ctrl.control_attitude(control_input);
//				_yaw_ctrl.control_attitude(control_input); //runs last, because is depending on output of roll and pitch attitude
//				_wheel_ctrl.control_attitude(control_input);
//
//				/* Update input data for rate controllers */
//				control_input.roll_rate_setpoint = _roll_ctrl.get_desired_rate();
//				control_input.pitch_rate_setpoint = _pitch_ctrl.get_desired_rate();
//				control_input.yaw_rate_setpoint = _yaw_ctrl.get_desired_rate();
//
//				/* Run attitude RATE controllers which need the desired attitudes from above, add trim */
//				float roll_u = _roll_ctrl.control_euler_rate(control_input);
//				_actuators.control[actuator_controls_s::INDEX_ROLL] = (PX4_ISFINITE(roll_u)) ? roll_u + _parameters.trim_roll :
//						_parameters.trim_roll;
//
//				if (!PX4_ISFINITE(roll_u)) {
//					_roll_ctrl.reset_integrator();
//					perf_count(_nonfinite_output_perf);
//
//					if (_debug && loop_counter % 10 == 0) {
//						warnx("roll_u %.4f", (double)roll_u);
//					}
//				}
//
//				float pitch_u = _pitch_ctrl.control_euler_rate(control_input);
//				_actuators.control[actuator_controls_s::INDEX_PITCH] = (PX4_ISFINITE(pitch_u)) ? pitch_u + _parameters.trim_pitch :
//						_parameters.trim_pitch;
//
//				if (!PX4_ISFINITE(pitch_u)) {
//					_pitch_ctrl.reset_integrator();
//					perf_count(_nonfinite_output_perf);
//
//					if (_debug && loop_counter % 10 == 0) {
//						warnx("pitch_u %.4f, _yaw_ctrl.get_desired_rate() %.4f,"
//						      " airspeed %.4f, airspeed_scaling %.4f,"
//						      " roll_sp %.4f, pitch_sp %.4f,"
//						      " _roll_ctrl.get_desired_rate() %.4f,"
//						      " _pitch_ctrl.get_desired_rate() %.4f"
//						      " att_sp.roll_body %.4f",
//						      (double)pitch_u, (double)_yaw_ctrl.get_desired_rate(),
//						      (double)airspeed, (double)airspeed_scaling,
//						      (double)roll_sp, (double)pitch_sp,
//						      (double)_roll_ctrl.get_desired_rate(),
//						      (double)_pitch_ctrl.get_desired_rate(),
//						      (double)_att_sp.roll_body);
//					}
//				}
//
//				float yaw_u = 0.0f;
//
//				if (_parameters.w_en && _att_sp.fw_control_yaw) {
//					yaw_u = _wheel_ctrl.control_bodyrate(control_input);
//
//				} else {
//					yaw_u = _yaw_ctrl.control_euler_rate(control_input);
//				}
//
//				_actuators.control[actuator_controls_s::INDEX_YAW] = (PX4_ISFINITE(yaw_u)) ? yaw_u + _parameters.trim_yaw :
//						_parameters.trim_yaw;
//
//				/* add in manual rudder control in manual modes */
//				if (_vcontrol_mode.flag_control_manual_enabled) {
//					_actuators.control[actuator_controls_s::INDEX_YAW] += _manual.r;
//				}
//
//				if (!PX4_ISFINITE(yaw_u)) {
//					_yaw_ctrl.reset_integrator();
//					_wheel_ctrl.reset_integrator();
//					perf_count(_nonfinite_output_perf);
//
//					if (_debug && loop_counter % 10 == 0) {
//						warnx("yaw_u %.4f", (double)yaw_u);
//					}
//				}
//
//				/* throttle passed through if it is finite and if no engine failure was detected */
//				_actuators.control[actuator_controls_s::INDEX_THROTTLE] = (PX4_ISFINITE(throttle_sp) &&
//						!(_vehicle_status.engine_failure ||
//						  _vehicle_status.engine_failure_cmd)) ?
//						throttle_sp : 0.0f;
//
//				/* scale effort by battery status */
//				if (_parameters.bat_scale_en && _battery_status.scale > 0.0f &&
//				    _actuators.control[actuator_controls_s::INDEX_THROTTLE] > 0.1f) {
//					_actuators.control[actuator_controls_s::INDEX_THROTTLE] *= _battery_status.scale;
//				}
//
//
//				if (!PX4_ISFINITE(throttle_sp)) {
//					if (_debug && loop_counter % 10 == 0) {
//						warnx("throttle_sp %.4f", (double)throttle_sp);
//					}
//				}
//
//			} else {
//				perf_count(_nonfinite_input_perf);
//
//				if (_debug && loop_counter % 10 == 0) {
//					warnx("Non-finite setpoint roll_sp: %.4f, pitch_sp %.4f", (double)roll_sp, (double)pitch_sp);
//				}
//			}
//
//		} else {
//			// pure rate control
//			_roll_ctrl.set_bodyrate_setpoint(_manual.y * _parameters.acro_max_x_rate_rad);
//			_pitch_ctrl.set_bodyrate_setpoint(-_manual.x * _parameters.acro_max_y_rate_rad);
//			_yaw_ctrl.set_bodyrate_setpoint(_manual.r * _parameters.acro_max_z_rate_rad);
//
//			float roll_u = _roll_ctrl.control_bodyrate(control_input);
//			_actuators.control[actuator_controls_s::INDEX_ROLL] = (PX4_ISFINITE(roll_u)) ? roll_u + _parameters.trim_roll :
//					_parameters.trim_roll;
//
//			float pitch_u = _pitch_ctrl.control_bodyrate(control_input);
//			_actuators.control[actuator_controls_s::INDEX_PITCH] = (PX4_ISFINITE(pitch_u)) ? pitch_u + _parameters.trim_pitch :
//					_parameters.trim_pitch;
//
//			float yaw_u = _yaw_ctrl.control_bodyrate(control_input);
//			_actuators.control[actuator_controls_s::INDEX_YAW] = (PX4_ISFINITE(yaw_u)) ? yaw_u + _parameters.trim_yaw :
//					_parameters.trim_yaw;
//
//			_actuators.control[actuator_controls_s::INDEX_THROTTLE] = (PX4_ISFINITE(throttle_sp) &&
//					//!(_vehicle_status.engine_failure ||
//					!_vehicle_status.engine_failure_cmd) ?
//					throttle_sp : 0.0f;
//		}
//
//		/*
//		 * Lazily publish the rate setpoint (for analysis, the actuators are published below)
//		 * only once available
//		 */
//		_rates_sp.roll = _roll_ctrl.get_desired_bodyrate();
//		_rates_sp.pitch = _pitch_ctrl.get_desired_bodyrate();
//		_rates_sp.yaw = _yaw_ctrl.get_desired_bodyrate();
//
//		_rates_sp.timestamp = hrt_absolute_time();
//
//		if (_rate_sp_pub != nullptr) {
//			/* publish the attitude rates setpoint */
//			orb_publish(_rates_sp_id, _rate_sp_pub, &_rates_sp);
//
//		} else if (_rates_sp_id) {
//			/* advertise the attitude rates setpoint */
//			_rate_sp_pub = orb_advertise(_rates_sp_id, &_rates_sp);
//		}
//
//	} else {
//		/* manual/direct control */
//		_actuators.control[actuator_controls_s::INDEX_ROLL] = _manual.y * _parameters.man_roll_scale + _parameters.trim_roll;
//		_actuators.control[actuator_controls_s::INDEX_PITCH] = -_manual.x * _parameters.man_pitch_scale +
//				_parameters.trim_pitch;
//		_actuators.control[actuator_controls_s::INDEX_YAW] = _manual.r * _parameters.man_yaw_scale + _parameters.trim_yaw;
//		_actuators.control[actuator_controls_s::INDEX_THROTTLE] = _manual.z;
//	}
//
//	// Add feed-forward from roll control output to yaw control output
//	// This can be used to counteract the adverse yaw effect when rolling the plane
//	_actuators.control[actuator_controls_s::INDEX_YAW] += _parameters.roll_to_yaw_ff * math::constrain(
//				_actuators.control[actuator_controls_s::INDEX_ROLL], -1.0f, 1.0f);
//
//	_actuators.control[actuator_controls_s::INDEX_FLAPS] = _flaps_applied;
//	_actuators.control[5] = _manual.aux1;
//	_actuators.control[actuator_controls_s::INDEX_AIRBRAKES] = _flaperons_applied;
//	// FIXME: this should use _vcontrol_mode.landing_gear_pos in the future
//	_actuators.control[7] = _manual.aux3;
//
//	/* lazily publish the setpoint only once available */
//	_actuators.timestamp = hrt_absolute_time();
//	_actuators.timestamp_sample = _att.timestamp;
//	_actuators_airframe.timestamp = hrt_absolute_time();
//	_actuators_airframe.timestamp_sample = _att.timestamp;
//
//	/* Only publish if any of the proper modes are enabled */
//	if (_vcontrol_mode.flag_control_rates_enabled ||
//	    _vcontrol_mode.flag_control_attitude_enabled ||
//	    _vcontrol_mode.flag_control_manual_enabled) {
//		/* publish the actuator controls */
//		if (_actuators_0_pub != nullptr) {
//			orb_publish(_actuators_id, _actuators_0_pub, &_actuators);
//
//		} else if (_actuators_id) {
//			_actuators_0_pub = orb_advertise(_actuators_id, &_actuators);
//		}
//
//		if (_actuators_2_pub != nullptr) {
//			/* publish the actuator controls*/
//			orb_publish(ORB_ID(actuator_controls_2), _actuators_2_pub, &_actuators_airframe);
//
//		} else {
//			/* advertise and publish */
//			_actuators_2_pub = orb_advertise(ORB_ID(actuator_controls_2), &_actuators_airframe);
//		}
//	}




	return status;
}

int32 FixedwingAttitudeControl::InitHk()
{
   int32 iStatus = CFE_SUCCESS;

   /* Init housekeeping packet */
//   memset((void*)&FAC_AppData.HkTlm, 0x00, sizeof(FAC_AppData.HkTlm)); // check this
   CFE_SB_InitMsg(&FAC_AppData.HkTlm, FAC_HK_TLM_MID,
                  sizeof(FAC_AppData.HkTlm), TRUE);

   return (iStatus);
}

int32 FixedwingAttitudeControl::InitSchPipe()
{
   int32  iStatus = CFE_SUCCESS;

   /* Init schedule pipe and subscribe to wakeup messages */
   iStatus = CFE_SB_CreatePipe(&SchPipeId, FAC_SCH_PIPE_DEPTH, FAC_SCH_PIPE_NAME);
   if (iStatus == CFE_SUCCESS)
   {
      iStatus = CFE_SB_SubscribeEx(FAC_WAKEUP_MID, SchPipeId, CFE_SB_Default_Qos,
                                   FAC_SCH_PIPE_WAKEUP_RESERVED);
      if (iStatus != CFE_SUCCESS)
      {
         CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                           "Sch Pipe failed to subscribe to FAC_WAKEUP_MID. (0x%08X)",
                           (unsigned int)iStatus);
         goto InitSchPipe_Exit_Tag;
      }

      iStatus = CFE_SB_SubscribeEx(FAC_SEND_HK_MID, SchPipeId, CFE_SB_Default_Qos,
                                   FAC_SCH_PIPE_SEND_HK_RESERVED);
      if (iStatus != CFE_SUCCESS)
      {
         CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                           "CMD Pipe failed to subscribe to FAC_SEND_HK_MID. (0x%08X)",
                           (unsigned int)iStatus);
         goto InitSchPipe_Exit_Tag;
      }
   }
   else
   {
      CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                        "Failed to create SCH pipe (0x%08X)",
                        (unsigned int)iStatus);
      goto InitSchPipe_Exit_Tag;
   }

InitSchPipe_Exit_Tag:
   return (iStatus);
}

int32 FixedwingAttitudeControl::InitEvent()
{
   int32  iStatus = CFE_SUCCESS;
   int32  ind = 0;

   /* Initialize the event filter table.
    * Note: 0 is the CFE_EVS_NO_FILTER mask and event 0 is reserved (not used) */
   memset((void*)EventTbl, 0x00, sizeof(EventTbl));   // check this

   /* TODO: Choose the events you want to filter.  CFE_EVS_MAX_EVENT_FILTERS
    * limits the number of filters per app.  An explicit CFE_EVS_NO_FILTER
    * (the default) has been provided as an example. */
   EventTbl[  ind].EventID = FAC_RESERVED_EID;
   EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

   EventTbl[  ind].EventID = FAC_INF_EID;
   EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

   EventTbl[  ind].EventID = FAC_CONFIG_TABLE_ERR_EID;
   EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

   EventTbl[  ind].EventID = FAC_CDS_ERR_EID;
   EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

   EventTbl[  ind].EventID = FAC_PIPE_ERR_EID;
   EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

   EventTbl[  ind].EventID = FAC_MSGID_ERR_EID;
   EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

   EventTbl[  ind].EventID = FAC_MSGLEN_ERR_EID;
   EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

   /* Register the table with CFE */
   iStatus = CFE_EVS_Register(EventTbl, FAC_EVT_CNT, CFE_EVS_BINARY_FILTER);
   if (iStatus != CFE_SUCCESS)
   {
      CFE_ES_WriteToSysLog("FAC - Failed to register with EVS (0x%08X)\n",
                           (unsigned int)iStatus);
   }

   return (iStatus);
}

int32 FAC_RcvMsg()
{
   int32 iStatus = CFE_SUCCESS;

   iStatus = objFAC.RcvSchMsg(FAC_SCH_PIPE_PEND_TIME);
   if (iStatus != CFE_SUCCESS)
   {
      goto RcvMsg_Exit_Tag;
   }

   iStatus = objCmds.RcvCmdMsg(CFE_SB_POLL);
   if (iStatus != CFE_SUCCESS)
   {
      goto RcvMsg_Exit_Tag;
   }

   iStatus = objData.RcvDataMsg(CFE_SB_POLL);
   if (iStatus != CFE_SUCCESS)
   {
      goto RcvMsg_Exit_Tag;
   }

RcvMsg_Exit_Tag:
   return iStatus;
}

/* Startup steps to initialize (or restore from CDS) FAC data structures */
int32 FAC_InitApp()
{
   int32  iStatus   = CFE_SUCCESS;
   int8   hasEvents = 0;

   iStatus = objFAC.InitEvent();
   if (iStatus != CFE_SUCCESS)
   {
      CFE_ES_WriteToSysLog("FAC - Failed to init events (0x%08X)\n",
                           (unsigned int)iStatus);
      goto InitApp_Exit_Tag;
   }
   else
   {
       hasEvents = 1;
   }

   iStatus = objFAC.InitSchPipe();
   if (iStatus != CFE_SUCCESS)
   {
      CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                        "Failed to init Sch pipe (0x%08X)",
                        (unsigned int)iStatus);
      goto InitApp_Exit_Tag;
   }

   iStatus = objCmds.InitCmdsPipe();
   if (iStatus != CFE_SUCCESS)
   {
      CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                        "Failed to init Cmds pipe (0x%08X)",
                        (unsigned int)iStatus);
      goto InitApp_Exit_Tag;
   }

   iStatus = objData.InitDataPipe();
   if (iStatus != CFE_SUCCESS)
   {
      CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                        "Failed to init Cds pipe (0x%08X)",
                        (unsigned int)iStatus);
      goto InitApp_Exit_Tag;
   }

   iStatus = objFAC.InitHk();
   if (iStatus != CFE_SUCCESS)
   {
      CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                        "Failed to init Hk (0x%08X)",
                        (unsigned int)iStatus);
      goto InitApp_Exit_Tag;
   }

   iStatus = objData.InitTables();
   if (iStatus != CFE_SUCCESS)
   {
      CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                        "Failed to init Tables (0x%08X)",
                        (unsigned int)iStatus);
      goto InitApp_Exit_Tag;
   }

InitApp_Exit_Tag:
   if (iStatus == CFE_SUCCESS)
   {
      CFE_EVS_SendEvent(FAC_INIT_INF_EID, CFE_EVS_INFORMATION,
                        "Initialized.  Version %d.%d.%d.%d",
                        FAC_MAJOR_VERSION,
                        FAC_MINOR_VERSION,
                        FAC_REVISION,
                        FAC_MISSION_REV);
   }
   else
   {
      if (hasEvents == 1)
      {
         CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                           "Application failed to initialize");
      }
      else
      {
         CFE_ES_WriteToSysLog("FAC - Application failed to initialize\n");
      }
   }

   return (iStatus);
}

extern "C" void FAC_AppMain()
{
   int32 iStatus = CFE_SUCCESS;

   /* Register the application with Executive Services */
   iStatus = CFE_ES_RegisterApp();
   if (iStatus != CFE_SUCCESS)
   {
      CFE_ES_WriteToSysLog("FAC - Failed to register the app (0x%08X)\n",
                           (unsigned int)iStatus);
   }

   /* Start Performance Log entry */
   CFE_ES_PerfLogEntry(FAC_MAIN_TASK_PERF_ID);

   /* Perform application initializations */
   if (iStatus == CFE_SUCCESS)
   {
      iStatus = FAC_InitApp();    //  Constructors
      if (iStatus == CFE_SUCCESS)
      {
         /* Do not perform performance monitoring on startup sync */
         CFE_ES_PerfLogExit(FAC_MAIN_TASK_PERF_ID);
         CFE_ES_WaitForStartupSync(FAC_STARTUP_TIMEOUT_MSEC);
         CFE_ES_PerfLogEntry(FAC_MAIN_TASK_PERF_ID);
      }
      else
      {
         FAC_AppData.uiAppRunStatus = CFE_ES_APP_ERROR;
      }
   }

   /* Application main loop */
   while (CFE_ES_RunLoop(&FAC_AppData.uiAppRunStatus) == TRUE)
   {
      iStatus = FAC_RcvMsg();
      if (iStatus == FAC_ERR_INVALID_POINTER)
      {
         CFE_EVS_SendEvent(FAC_ERR_EID, CFE_EVS_ERROR,
                           "Application Fatal Error(Invalid pointer)");
         FAC_AppData.uiAppRunStatus = CFE_ES_APP_ERROR;
         goto AppMain_Exit_Tag;
      }
      else if (iStatus != CFE_SUCCESS)
      {
         /* TODO: Decide what to do for other return values in FAC_RcvMsg(). */
      }


      /* TODO: This is only a suggestion for when to update and save CDS table.
      ** Depends on the nature of the application, the frequency of update
      ** and save can be more or less independently.
      */
   }

AppMain_Exit_Tag:
   CFE_ES_WriteToSysLog("FAC - Error detected. App will exit (0x%08X)\n",
                        (unsigned int)iStatus);

   /* Stop Performance Log entry */
   CFE_ES_PerfLogExit(FAC_MAIN_TASK_PERF_ID);

   /* Exit the application */
   CFE_ES_ExitApp(FAC_AppData.uiAppRunStatus);
}
