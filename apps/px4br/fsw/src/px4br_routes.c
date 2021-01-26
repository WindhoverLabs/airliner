/*
 * px4br_routes.c
 *
 *  Created on: Oct 22, 2016
 *      Author: mbenson
 */

#include "px4br_routes.h"
#include "msg_ids.h"
#include "px4br_serialization.h"
#include "pb_msgs/actuator_armed.pb.h"
#include <pb_msgs/actuator_armed.pb.h>
#include "pb_msgs/actuator_controls.pb.h"
#include "pb_msgs/actuator_direct.pb.h"
#include "pb_msgs/actuator_outputs.pb.h"
#include "pb_msgs/adc_report.pb.h"
#include "pb_msgs/airspeed.pb.h"
#include "pb_msgs/att_pos_mocap.pb.h"
#include "pb_msgs/battery_status.pb.h"
#include "pb_msgs/camera_trigger.pb.h"
#include "pb_msgs/commander_state.pb.h"
#include "pb_msgs/control_state.pb.h"
#include "pb_msgs/cpuload.pb.h"
#include "pb_msgs/debug_key_value.pb.h"
#include "pb_msgs/differential_pressure.pb.h"
#include "pb_msgs/distance_sensor.pb.h"
#include "pb_msgs/ekf2_innovations.pb.h"
#include "pb_msgs/ekf2_replay.pb.h"
#include "pb_msgs/esc_report.pb.h"
#include "pb_msgs/esc_status.pb.h"
#include "pb_msgs/estimator_status.pb.h"
#include "pb_msgs/fence.pb.h"
#include "pb_msgs/fence_vertex.pb.h"
#include "pb_msgs/filtered_bottom_flow.pb.h"
#include "pb_msgs/follow_target.pb.h"
#include "pb_msgs/fw_pos_ctrl_status.pb.h"
#include "pb_msgs/fw_virtual_attitude_setpoint.pb.h"
#include "pb_msgs/fw_virtual_rates_setpoint.pb.h"
#include "pb_msgs/geofence_result.pb.h"
#include "pb_msgs/gps_dump.pb.h"
#include "pb_msgs/gps_inject_data.pb.h"
#include "pb_msgs/hil_sensor.pb.h"
#include "pb_msgs/home_position.pb.h"
#include "pb_msgs/input_rc.pb.h"
#include "pb_msgs/log_message.pb.h"
#include "pb_msgs/manual_control_setpoint.pb.h"
#include "pb_msgs/mavlink_log.pb.h"
#include "pb_msgs/mc_att_ctrl_status.pb.h"
#include "pb_msgs/mc_virtual_attitude_setpoint.pb.h"
#include "pb_msgs/mc_virtual_rates_setpoint.pb.h"
#include "pb_msgs/mission.pb.h"
#include "pb_msgs/mission_result.pb.h"
#include "pb_msgs/multirotor_motor_limits.pb.h"
#include "pb_msgs/offboard_control_mode.pb.h"
#include "pb_msgs/optical_flow.pb.h"
#include "pb_msgs/output_pwm.pb.h"
#include "pb_msgs/parameter_update.pb.h"
#include "pb_msgs/position_setpoint.pb.h"
#include "pb_msgs/position_setpoint_triplet.pb.h"
#include "pb_msgs/pwm_input.pb.h"
#include "pb_msgs/qshell_req.pb.h"
#include "pb_msgs/rc_channels.pb.h"
#include "pb_msgs/rc_parameter_map.pb.h"
#include "pb_msgs/safety.pb.h"
#include "pb_msgs/satellite_info.pb.h"
#include "pb_msgs/sensor_accel.pb.h"
#include "pb_msgs/sensor_baro.pb.h"
#include "pb_msgs/sensor_combined.pb.h"
#include "pb_msgs/sensor_gyro.pb.h"
#include "pb_msgs/sensor_mag.pb.h"
#include "pb_msgs/servorail_status.pb.h"
#include "pb_msgs/subsystem_info.pb.h"
#include "pb_msgs/system_power.pb.h"
#include "pb_msgs/tecs_status.pb.h"
#include "pb_msgs/telemetry_status.pb.h"
#include "pb_msgs/test_motor.pb.h"
#include "pb_msgs/time_offset.pb.h"
#include "pb_msgs/transponder_report.pb.h"
#include "pb_msgs/uavcan_parameter_request.pb.h"
#include "pb_msgs/uavcan_parameter_value.pb.h"
#include "pb_msgs/vehicle_attitude.pb.h"
#include "pb_msgs/vehicle_attitude_setpoint.pb.h"
#include "pb_msgs/vehicle_command_ack.pb.h"
#include "pb_msgs/vehicle_command.pb.h"
#include "pb_msgs/vehicle_control_mode.pb.h"
#include "pb_msgs/vehicle_force_setpoint.pb.h"
#include "pb_msgs/vehicle_global_position.pb.h"
#include "pb_msgs/vehicle_global_velocity_setpoint.pb.h"
#include "pb_msgs/vehicle_gps_position.pb.h"
#include "pb_msgs/vehicle_land_detected.pb.h"
#include "pb_msgs/vehicle_local_position.pb.h"
#include "pb_msgs/vehicle_local_position_setpoint.pb.h"
#include "pb_msgs/vehicle_rates_setpoint.pb.h"
#include "pb_msgs/vehicle_status.pb.h"
#include "pb_msgs/vision_position_estimate.pb.h"
#include "pb_msgs/vtol_vehicle_status.pb.h"
#include "pb_msgs/wind_estimate.pb.h"

PX4BR_Route_t  PX4BR_Route[PX4BR_MAX_ROUTE_COUNT] =
{
//		{"actuator_armed", (PX4_ENCODE_FUNC) PX4BR_ActuatorArmed_Enc, (PX4_DECODE_FUNC) PX4BR_ActuatorArmed_Dec, PX4_ACTUATOR_ARMED_MID, px4_actuator_armed_pb_size, 1, 0, 0},
//		{"actuator_controls", (PX4_ENCODE_FUNC) PX4BR_ActuatorControls_Enc, (PX4_DECODE_FUNC) PX4BR_ActuatorControls_Dec, PX4_ACTUATOR_CONTROLS_MID, px4_actuator_controls_pb_size, 1, 0, 0},
//		{"actuator_direct", (PX4_ENCODE_FUNC) PX4BR_ActuatorDirect_Enc, (PX4_DECODE_FUNC) PX4BR_ActuatorDirect_Dec, PX4_ACTUATOR_DIRECT_MID, px4_actuator_direct_pb_size, 1, 0, 0},
//		{"actuator_outputs", (PX4_ENCODE_FUNC) PX4BR_ActuatorOutputs_Enc, (PX4_DECODE_FUNC) PX4BR_ActuatorOutputs_Dec, PX4_ACTUATOR_OUTPUTS_MID, px4_actuator_outputs_pb_size, 1, 0, 0},
//		{"adc_report", (PX4_ENCODE_FUNC) PX4BR_AdcReport_Enc, (PX4_DECODE_FUNC) PX4BR_AdcReport_Dec, PX4_ADC_REPORT_MID, px4_adc_report_pb_size, 1, 0, 0},
//		{"airspeed", (PX4_ENCODE_FUNC) PX4BR_Airspeed_Enc, (PX4_DECODE_FUNC) PX4BR_Airspeed_Dec, PX4_AIRSPEED_MID, px4_airspeed_pb_size, 1, 0, 0},
//		{"att_pos_mocap", (PX4_ENCODE_FUNC) PX4BR_AttPosMoCap_Enc, (PX4_DECODE_FUNC) PX4BR_AttPosMoCap_Dec, PX4_ATT_POS_MOCAP_MID, px4_att_pos_mocap_pb_size, 1, 0, 0},
//		{"battery_status", (PX4_ENCODE_FUNC) PX4BR_BatteryStatus_Enc, (PX4_DECODE_FUNC) PX4BR_BatteryStatus_Dec, PX4_BATTERY_STATUS_MID, px4_battery_status_pb_size, 1, 0, 0},
//		{"camera_trigger", (PX4_ENCODE_FUNC) PX4BR_CameraTrigger_Enc, (PX4_DECODE_FUNC) PX4BR_CameraTrigger_Dec, PX4_CAMERA_TRIGGER_MID, px4_camera_trigger_pb_size, 1, 0, 0},
//		{"commander_state", (PX4_ENCODE_FUNC) PX4BR_CommanderState_Enc, (PX4_DECODE_FUNC) PX4BR_CommanderState_Dec, PX4_COMMANDER_STATE_MID, px4_commander_state_pb_size, 1, 0, 0},
//		{"control_state", (PX4_ENCODE_FUNC) PX4BR_ControlState_Enc, (PX4_DECODE_FUNC) PX4BR_ControlState_Dec, PX4_CONTROL_STATE_MID, px4_control_state_pb_size, 1, 0, 0},
//		{"cpuload", (PX4_ENCODE_FUNC) PX4BR_CpuLoad_Enc, (PX4_DECODE_FUNC) PX4BR_CpuLoad_Dec, PX4_CPULOAD_MID, px4_cpuload_pb_size, 1, 0, 0},
//		{"debug_key_value", (PX4_ENCODE_FUNC) PX4BR_DebugKeyValue_Enc, (PX4_DECODE_FUNC) PX4BR_DebugKeyValue_Dec, PX4_DEBUG_KEY_VALUE_MID, px4_debug_key_value_pb_size, 1, 0, 0},
//		{"differential_pressure", (PX4_ENCODE_FUNC) PX4BR_DifferentialPressure_Enc, (PX4_DECODE_FUNC) PX4BR_DifferentialPressure_Dec, PX4_DIFFERENTIAL_PRESSURE_MID, px4_differential_pressure_pb_size, 1, 0, 0},
//		{"distance_sensor", (PX4_ENCODE_FUNC) PX4BR_DistanceSensor_Enc, (PX4_DECODE_FUNC) PX4BR_DistanceSensor_Dec, PX4_DISTANCE_SENSOR_MID, px4_distance_sensor_pb_size, 1, 0, 0},
//		{"fw_pos_ctrl_status", (PX4_ENCODE_FUNC) PX4BR_FwPosCtrlStatus_Enc, (PX4_DECODE_FUNC) PX4BR_FwPosCtrlStatus_Dec, PX4_FW_POS_CTRL_STATUS_MID, px4_fw_pos_ctrl_status_pb_size, 1, 0, 0},
//		{"fw_virtual_attitude_setpoint", (PX4_ENCODE_FUNC) PX4BR_FwVirtualAttitudeSetpoint_Enc, (PX4_DECODE_FUNC) PX4BR_FwVirtualAttitudeSetpoint_Dec, PX4_FW_VIRTUAL_ATTITUDE_SETPOINT_MID, px4_fw_virtual_attitude_setpoint_pb_size, 1, 0, 0},
//		{"fw_virtual_rates_setpoint", (PX4_ENCODE_FUNC) PX4BR_FwVirtualRatesSetpoint_Enc, (PX4_DECODE_FUNC) PX4BR_FwVirtualRatesSetpoint_Dec, PX4_FW_VIRTUAL_RATES_SETPOINT_MID, px4_fw_virtual_rates_setpoint_pb_size, 1, 0, 0},
//		{"ekf2_innovations", (PX4_ENCODE_FUNC) PX4BR_Ekf2Innovations_Enc, (PX4_DECODE_FUNC) PX4BR_Ekf2Innovations_Dec, PX4_EKF2_INNOVATIONS_MID, px4_ekf2_innovations_pb_size, 1, 0, 0},
//		{"ekf2_replay", (PX4_ENCODE_FUNC) PX4BR_Ekf2Replay_Enc, (PX4_DECODE_FUNC) PX4BR_Ekf2Replay_Dec, PX4_EKF2_REPLAY_MID, px4_ekf2_replay_pb_size, 1, 0, 0},
//		{"esc_report", (PX4_ENCODE_FUNC) PX4BR_EscReport_Enc, (PX4_DECODE_FUNC) PX4BR_EscReport_Dec, PX4_ESC_REPORT_MID, px4_esc_report_pb_size, 1, 0, 0},
//		{"esc_status", (PX4_ENCODE_FUNC) PX4BR_EscStatus_Enc, (PX4_DECODE_FUNC) PX4BR_EscStatus_Dec, PX4_ESC_STATUS_MID, px4_esc_status_pb_size, 1, 0, 0},
//		{"estimator_status", (PX4_ENCODE_FUNC) PX4BR_EstimatorStatus_Enc, (PX4_DECODE_FUNC) PX4BR_EstimatorStatus_Dec, PX4_ESTIMATOR_STATUS_MID, px4_estimator_status_pb_size, 1, 0, 0},
//		{"fence", (PX4_ENCODE_FUNC) PX4BR_Fence_Enc, (PX4_DECODE_FUNC) PX4BR_Fence_Dec, PX4_FENCE_MID, px4_fence_pb_size, 1, 0, 0},
//		{"fence_vertex", (PX4_ENCODE_FUNC) PX4BR_FenceVertex_Enc, (PX4_DECODE_FUNC) PX4BR_FenceVertex_Dec, PX4_FENCE_VERTEX_MID, px4_fence_vertex_pb_size, 1, 0, 0},
//		{"filtered_bottom_flow", (PX4_ENCODE_FUNC) PX4BR_FilteredBottomFlow_Enc, (PX4_DECODE_FUNC) PX4BR_FilteredBottomFlow_Dec, PX4_FILTERED_BOTTOM_FLOW_MID, px4_filtered_bottom_flow_pb_size, 1, 0, 0},
//		{"follow_target", (PX4_ENCODE_FUNC) PX4BR_FollowTarget_Enc, (PX4_DECODE_FUNC) PX4BR_FollowTarget_Dec, PX4_FOLLOW_TARGET_MID, px4_follow_target_pb_size, 1, 0, 0},
//		{"geofence_result", (PX4_ENCODE_FUNC) PX4BR_GeofenceResult_Enc, (PX4_DECODE_FUNC) PX4BR_GeofenceResult_Dec, PX4_GEOFENCE_RESULT_MID, px4_geofence_result_pb_size, 1, 0, 0},
//		{"gps_dump", (PX4_ENCODE_FUNC) PX4BR_GpsDump_Enc, (PX4_DECODE_FUNC) PX4BR_GpsDump_Dec, PX4_GPS_DUMP_MID, px4_gps_dump_pb_size, 1, 0, 0},
//		{"gps_inject_data", (PX4_ENCODE_FUNC) PX4BR_GpsInjectData_Enc, (PX4_DECODE_FUNC) PX4BR_GpsInjectData_Dec, PX4_GPS_INJECT_DATA_MID, px4_gps_inject_data_pb_size, 1, 0, 0},
//		{"hil_sensor", (PX4_ENCODE_FUNC) PX4BR_HilSensor_Enc, (PX4_DECODE_FUNC) PX4BR_HilSensor_Dec, PX4_HIL_SENSOR_MID, px4_hil_sensor_pb_size, 1, 0, 0},
//		{"home_position", (PX4_ENCODE_FUNC) PX4BR_HomePosition_Enc, (PX4_DECODE_FUNC) PX4BR_HomePosition_Dec, PX4_HOME_POSITION_MID, px4_home_position_pb_size, 1, 0, 0},
//		{"input_rc", (PX4_ENCODE_FUNC) PX4BR_InputRc_Enc, (PX4_DECODE_FUNC) PX4BR_InputRc_Dec, PX4_INPUT_RC_MID, px4_input_rc_pb_size, 1, 0, 0},
//		{"log_message", (PX4_ENCODE_FUNC) PX4BR_LogMessage_Enc, (PX4_DECODE_FUNC) PX4BR_LogMessage_Dec, PX4_LOG_MESSAGE_MID, px4_log_message_pb_size, 1, 0, 0},
//		{"manual_control_setpoint", (PX4_ENCODE_FUNC) PX4BR_ManualControlSetpoint_Enc, (PX4_DECODE_FUNC) PX4BR_ManualControlSetpoint_Dec, PX4_MANUAL_CONTROL_SETPOINT_MID, px4_manual_control_setpoint_pb_size, 1, 0, 0},
//		{"mavlink_log", (PX4_ENCODE_FUNC) PX4BR_MavlinkLog_Enc, (PX4_DECODE_FUNC) PX4BR_MavlinkLog_Dec, PX4_MAVLINK_LOG_MID, px4_mavlink_log_pb_size, 1, 0, 0},
//		{"mc_att_ctrl_status", (PX4_ENCODE_FUNC) PX4BR_McAttCtrlStatus_Enc, (PX4_DECODE_FUNC) PX4BR_McAttCtrlStatus_Dec, PX4_MC_ATT_CTRL_STATUS_MID, px4_mc_att_ctrl_status_pb_size, 1, 0, 0},
//		{"mc_virtual_attitude_setpoint", (PX4_ENCODE_FUNC) PX4BR_McVirtualAttitudeSetpoint_Enc, (PX4_DECODE_FUNC) PX4BR_McVirtualAttitudeSetpoint_Dec, PX4_MC_VIRTUAL_ATTITUDE_SETPOINT_MID, px4_mc_virtual_attitude_setpoint_pb_size, 1, 0, 0},
//		{"mc_virtual_rates_setpoint", (PX4_ENCODE_FUNC) PX4BR_McVirtualRatesSetpoint_Enc, (PX4_DECODE_FUNC) PX4BR_McVirtualRatesSetpoint_Dec, PX4_MC_VIRTUAL_RATES_SETPOINT_MID, px4_mc_virtual_rates_setpoint_pb_size, 1, 0, 0},
//		{"mission", (PX4_ENCODE_FUNC) PX4BR_Mission_Enc, (PX4_DECODE_FUNC) PX4BR_Mission_Dec, PX4_MISSION_MID, px4_mission_pb_size, 1, 0, 0},
//		{"mission_result", (PX4_ENCODE_FUNC) PX4BR_MissionResult_Enc, (PX4_DECODE_FUNC) PX4BR_MissionResult_Dec, PX4_MISSION_RESULT_MID, px4_mission_result_pb_size, 1, 0, 0},
//		{"multirotor_motor_limits", (PX4_ENCODE_FUNC) PX4BR_MultirotorMotorLimits_Enc, (PX4_DECODE_FUNC) PX4BR_MultirotorMotorLimits_Dec, PX4_MULTIROTOR_MOTOR_LIMITS_MID, px4_multirotor_motor_limits_pb_size, 1, 0, 0},
//		{"offboard_control_mode", (PX4_ENCODE_FUNC) PX4BR_OffboardControlMode_Enc, (PX4_DECODE_FUNC) PX4BR_OffboardControlMode_Dec, PX4_OFFBOARD_CONTROL_MODE_MID, px4_offboard_control_mode_pb_size, 1, 0, 0},
//		{"optical_flow", (PX4_ENCODE_FUNC) PX4BR_OpticalFlow_Enc, (PX4_DECODE_FUNC) PX4BR_OpticalFlow_Dec, PX4_OPTICAL_FLOW_MID, px4_optical_flow_pb_size, 1, 0, 0},
//		{"output_pwm", (PX4_ENCODE_FUNC) PX4BR_OutputPwm_Enc, (PX4_DECODE_FUNC) PX4BR_OutputPwm_Dec, PX4_OUTPUT_PWM_MID, px4_output_pwm_pb_size, 1, 0, 0},
//		{"parameter_update", (PX4_ENCODE_FUNC) PX4BR_ParameterUpdate_Enc, (PX4_DECODE_FUNC) PX4BR_ParameterUpdate_Dec, PX4_PARAMETER_UPDATE_MID, px4_parameter_update_pb_size, 1, 0, 0},
//		{"position_setpoint", (PX4_ENCODE_FUNC) PX4BR_PositionSetpoint_Enc, (PX4_DECODE_FUNC) PX4BR_PositionSetpoint_Dec, PX4_POSITION_SETPOINT_MID, px4_position_setpoint_pb_size, 1, 0, 0},
//		{"position_setpoint_triplet", (PX4_ENCODE_FUNC) PX4BR_PositionSetpointTriplet_Enc, (PX4_DECODE_FUNC) PX4BR_PositionSetpointTriplet_Dec, PX4_POSITION_SETPOINT_TRIPLET_MID, px4_position_setpoint_triplet_pb_size, 1, 0, 0},
//		{"pwm_input", (PX4_ENCODE_FUNC) PX4BR_PwmInput_Enc, (PX4_DECODE_FUNC) PX4BR_PwmInput_Dec, PX4_PWM_INPUT_MID, px4_pwm_input_pb_size, 1, 0, 0},
//		{"qshell_req", (PX4_ENCODE_FUNC) PX4BR_QShellReq_Enc, (PX4_DECODE_FUNC) PX4BR_QShellReq_Dec, PX4_QSHELL_REQ_MID, px4_qshell_req_pb_size, 1, 0, 0},
//		{"rc_channels", (PX4_ENCODE_FUNC) PX4BR_RcChannels_Enc, (PX4_DECODE_FUNC) PX4BR_RcChannels_Dec, PX4_RC_CHANNELS_MID, px4_rc_channels_pb_size, 1, 0, 0},
//		{"rc_parameter_map", (PX4_ENCODE_FUNC) PX4BR_RcParameterMap_Enc, (PX4_DECODE_FUNC) PX4BR_RcParameterMap_Dec, PX4_RC_PARAMETER_MAP_MID, px4_rc_parameter_map_pb_size, 1, 0, 0},
//		{"safety_enc", (PX4_ENCODE_FUNC) PX4BR_Safety_Enc, (PX4_DECODE_FUNC) PX4BR_Safety_Dec, PX4_SAFETY_MID, px4_safety_pb_size, 1, 0, 0},
//		{"satellite_info", (PX4_ENCODE_FUNC) PX4BR_SatelliteInfo_Enc, (PX4_DECODE_FUNC) PX4BR_SatelliteInfo_Dec, PX4_SATELLITE_INFO_MID, px4_satellite_info_pb_size, 1, 0, 0},
//		{"sensor_accel", (PX4_ENCODE_FUNC) PX4BR_SensorAccel_Enc, (PX4_DECODE_FUNC) PX4BR_SensorAccel_Dec, PX4_SENSOR_ACCEL_MID, px4_sensor_accel_pb_size, 1, 0, 0},
		{"sensor_baro", (PX4_ENCODE_FUNC) PX4BR_SensorBaro_Enc, (PX4_DECODE_FUNC) PX4BR_SensorBaro_Dec, PX4_SENSOR_BARO_MID, px4_sensor_baro_pb_size, 1, 0, 0},
//		{"sensor_combined", (PX4_ENCODE_FUNC) PX4BR_SensorCombined_Enc, (PX4_DECODE_FUNC) PX4BR_SensorCombined_Dec, PX4_SENSOR_COMBINED_MID, px4_sensor_combined_pb_size, 1, 0, 0},
//		{"sensor_gyro", (PX4_ENCODE_FUNC) PX4BR_SensorGyro_Enc, (PX4_DECODE_FUNC) PX4BR_SensorGyro_Dec, PX4_SENSOR_GYRO_MID, px4_sensor_gyro_pb_size, 1, 0, 0},
//		{"sensor_mag", (PX4_ENCODE_FUNC) PX4BR_SensorMag_Enc, (PX4_DECODE_FUNC) PX4BR_SensorMag_Dec, PX4_SENSOR_MAG_MID, px4_sensor_mag_pb_size, 1, 0, 0},
//		{"servorail_status", (PX4_ENCODE_FUNC) PX4BR_ServorailStatus_Enc, (PX4_DECODE_FUNC) PX4BR_ServorailStatus_Dec, PX4_SERVORAIL_STATUS_MID, px4_servorail_status_pb_size, 1, 0, 0},
//		{"subsystem_info", (PX4_ENCODE_FUNC) PX4BR_SubsystemInfo_Enc, (PX4_DECODE_FUNC) PX4BR_SubsystemInfo_Dec, PX4_SUBSYSTEM_INFO_MID, px4_subsystem_info_pb_size, 1, 0, 0},
//		{"system_power", (PX4_ENCODE_FUNC) PX4BR_SystemPower_Enc, (PX4_DECODE_FUNC) PX4BR_SystemPower_Dec, PX4_SYSTEM_POWER_MID, px4_system_power_pb_size, 1, 0, 0},
//		{"tecs_status", (PX4_ENCODE_FUNC) PX4BR_TecsStatus_Enc, (PX4_DECODE_FUNC) PX4BR_TecsStatus_Dec, PX4_TECS_STATUS_MID, px4_tecs_status_pb_size, 1, 0, 0},
//		{"telemetry_status", (PX4_ENCODE_FUNC) PX4BR_TelemetryStatus_Enc, (PX4_DECODE_FUNC) PX4BR_TelemetryStatus_Dec, PX4_TELEMETRY_STATUS_MID, px4_telemetry_status_pb_size, 1, 0, 0},
//		{"test_motor", (PX4_ENCODE_FUNC) PX4BR_TestMotor_Enc, (PX4_DECODE_FUNC) PX4BR_TestMotor_Dec, PX4_TEST_MOTOR_MID, px4_test_motor_pb_size, 1, 0, 0},
//		{"time_offset", (PX4_ENCODE_FUNC) PX4BR_TimeOffset_Enc, (PX4_DECODE_FUNC) PX4BR_TimeOffset_Dec, PX4_TIME_OFFSET_MID, px4_time_offset_pb_size, 1, 0, 0},
//		{"transponder_report", (PX4_ENCODE_FUNC) PX4BR_TransponderReport_Enc, (PX4_DECODE_FUNC) PX4BR_TransponderReport_Dec, PX4_TRANSPONDER_REPORT_MID, px4_transponder_report_pb_size, 1, 0, 0},
//		{"uavcan_parameter_request", (PX4_ENCODE_FUNC) PX4BR_UavCanParameterRequest_Enc, (PX4_DECODE_FUNC) PX4BR_UavCanParameterRequest_Dec, PX4_UAVCAN_PARAMETER_REQUEST_MID, px4_uavcan_parameter_request_pb_size, 1, 0, 0},
//		{"uavcan_parameter_value", (PX4_ENCODE_FUNC) PX4BR_UavCanParameterValue_Enc, (PX4_DECODE_FUNC) PX4BR_UavCanParameterValue_Dec, PX4_UAVCAN_PARAMETER_VALUE_MID, px4_uavcan_parameter_value_pb_size, 1, 0, 0},
//		{"vehicle_attitude", (PX4_ENCODE_FUNC) PX4BR_VehicleAttitude_Enc, (PX4_DECODE_FUNC) PX4BR_VehicleAttitude_Dec, PX4_VEHICLE_ATTITUDE_MID, px4_vehicle_attitude_pb_size, 1, 0, 0},
//		{"vehicle_attitude_setpoint", (PX4_ENCODE_FUNC) PX4BR_VehicleAttitudeSetpoint_Enc, (PX4_DECODE_FUNC) PX4BR_VehicleAttitudeSetpoint_Dec, PX4_VEHICLE_ATTITUDE_SETPOINT_MID, px4_vehicle_attitude_setpoint_pb_size,  1, 0, 0},
//		{"vehicle_command_ack", (PX4_ENCODE_FUNC) PX4BR_VehicleCommandAck_Enc, (PX4_DECODE_FUNC) PX4BR_VehicleCommandAck_Dec, PX4_VEHICLE_COMMAND_ACK_MID, px4_vehicle_command_ack_pb_size, 1, 0, 0},
//		{"vehicle_command", (PX4_ENCODE_FUNC) PX4BR_VehicleCommand_Enc, (PX4_DECODE_FUNC) PX4BR_VehicleCommand_Dec, PX4_VEHICLE_COMMAND_MID, px4_vehicle_command_pb_size, 1, 0, 0},
//		{"vehicle_control_mode", (PX4_ENCODE_FUNC) PX4BR_VehicleControlMode_Enc, (PX4_DECODE_FUNC) PX4BR_VehicleControlMode_Dec, PX4_VEHICLE_CONTROL_MODE_MID, px4_vehicle_control_mode_pb_size, 1, 0, 0},
//		{"vehicle_force_setpoint", (PX4_ENCODE_FUNC) PX4BR_VehicleForceSetpoint_Enc, (PX4_DECODE_FUNC) PX4BR_VehicleForceSetpoint_Dec, PX4_VEHICLE_FORCE_SETPOINT_MID, px4_vehicle_force_setpoint_pb_size, 1, 0, 0},
//		{"vehicle_global_position", (PX4_ENCODE_FUNC) PX4BR_VehicleGlobalPosition_Enc, (PX4_DECODE_FUNC) PX4BR_VehicleGlobalPosition_Dec, PX4_VEHICLE_GLOBAL_POSITION_MID, px4_vehicle_global_position_pb_size, 1, 0, 0},
//		{"vehicle_global_velocity_setpoint", (PX4_ENCODE_FUNC) PX4BR_VehicleGlobalVelocitySetpoint_Enc, (PX4_DECODE_FUNC) PX4BR_VehicleGlobalVelocitySetpoint_Dec, PX4_VEHICLE_GLOBAL_VELOCITY_SETPOINT_MID, px4_vehicle_global_velocity_setpoint_pb_size, 1, 0, 0},
		{"vehicle_gps_position", (PX4_ENCODE_FUNC) PX4BR_VehicleGpsPosition_Enc, (PX4_DECODE_FUNC) PX4BR_VehicleGpsPosition_Dec, PX4_VEHICLE_GPS_POSITION_MID, px4_vehicle_gps_position_pb_size, 1, 0, 0},
//		{"vehicle_land_detected", (PX4_ENCODE_FUNC) PX4BR_VehicleLandDetected_Enc, (PX4_DECODE_FUNC) PX4BR_VehicleLandDetected_Dec, PX4_VEHICLE_LAND_DETECTED_MID, px4_vehicle_land_detected_pb_size, 1, 0, 0},
//		{"vehicle_local_position", (PX4_ENCODE_FUNC) PX4BR_VehicleLocalPosition_Enc, (PX4_DECODE_FUNC) PX4BR_VehicleLocalPosition_Dec, PX4_VEHICLE_LOCAL_POSITION_MID, px4_vehicle_local_position_pb_size, 1, 0, 0},
//		{"vehicle_local_position_setpoint", (PX4_ENCODE_FUNC) PX4BR_VehicleLocalPositionSetpoint_Enc, (PX4_DECODE_FUNC) PX4BR_VehicleLocalPositionSetpoint_Dec, PX4_VEHICLE_LOCAL_POSITION_SETPOINT_MID, px4_vehicle_local_position_setpoint_pb_size, 1, 0, 0},
//		// RealtimeArchiveFiller ocpoc SEVERE Error when adding data to realtime segments: java.lang.NullPointerException
////		{"vehicle_rates_setpoint", (PX4_ENCODE_FUNC) PX4BR_VehicleRatesSetpoint_Enc, (PX4_DECODE_FUNC) PX4BR_VehicleRatesSetpoint_Dec, PX4_VEHICLE_RATES_SETPOINT_MID, px4_vehicle_rates_setpoint_pb_size, 1, 0, 0},
//		// RealtimeArchiveFiller ocpoc SEVERE Error when adding data to realtime segments: java.lang.NullPointerException
////		{"vehicle_status", (PX4_ENCODE_FUNC) PX4BR_VehicleStatus_Enc, (PX4_DECODE_FUNC) PX4BR_VehicleStatus_Dec, PX4_VEHICLE_STATUS_MID, px4_vehicle_status_pb_size, 1, 0, 0},
//		{"Vision_position_estimate", (PX4_ENCODE_FUNC) PX4BR_VisionPositionEstimate_Enc, (PX4_DECODE_FUNC) PX4BR_VisionPositionEstimate_Dec, PX4_VISION_POSITION_ESTIMATE_MID, px4_vision_position_estimate_pb_size, 1, 0, 0},
//		{"vtol_vehicle_status", (PX4_ENCODE_FUNC) PX4BR_VtolVehicleStatus_Enc, (PX4_DECODE_FUNC) PX4BR_VtolVehicleStatus_Dec, PX4_VTOL_VEHICLE_STATUS_MID, px4_vtol_vehicle_status_pb_size, 1, 0, 0},
//		{"wind_estimate", (PX4_ENCODE_FUNC) PX4BR_WindEstimate_Enc, (PX4_DECODE_FUNC) PX4BR_WindEstimate_Dec, PX4_WIND_ESTIMATE_MID, px4_wind_estimate_pb_size, 1, 0, 0}

};
