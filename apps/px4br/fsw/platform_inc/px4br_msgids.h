#ifndef PX4BR_MSGIDS_H
#define PX4BR_MSGIDS_H


#define PX4BR_SEND_HK_MID   (0x0001)
#define PX4BR_CMD_MID       (0x0002)
#define PX4BR_PROC_CMD_MID  (0x0003)
#define PX4BR_HK_TLM_MID    (0x0004)


#define PX4_ACTUATOR_ARMED_MID                    (0x0000)
#define PX4_ACTUATOR_CONTROLS_0_MID               (0x0000)
#define PX4_ACTUATOR_CONTROLS_1_MID               (0x0000)
#define PX4_ACTUATOR_CONTROLS_2_MID               (0x0000)
#define PX4_ACTUATOR_CONTROLS_3_MID               (0x0000)
#define PX4_ACTUATOR_DIRECT_MID                   (0x0000)
#define PX4_ACTUATOR_OUTPUTS_MID                  (0x0000)
#define PX4_ADC_REPORT_MID                        (0x0000)
#define PX4_AIRSPEED_MID                          (0x0000)
#define PX4_ATT_POS_MOCAP_MID                     (0x0000)
#define PX4_BATTERY_STATUS_MID                    (0x0000)
#define PX4_CAMERA_TRIGGER_MID                    (0x0000)
#define PX4_COMMANDER_STATE_MID                   (0x0000)
#define PX4_CONTROL_STATE_MID                     (0x0000)
#define PX4_CPULOAD_MID                           (0x0000)
#define PX4_DEBUG_KEY_VALUE_MID                   (0x0000)
#define PX4_DIFFERENTIAL_PRESSURE_MID             (0x0000)
#define PX4_DISTANCE_SENSOR_MID                   (0x0000)
#define PX4_FW_POS_CTRL_STATUS_MID                (0x0000)
#define PX4_FW_VIRTUAL_ATTITUDE_SETPOINT_MID      (0x0000)
#define PX4_FW_VIRTUAL_RATES_SETPOINT_MID         (0x0000)
#define PX4_EKF2_INNOVATIONS_MID                  (0x0000)
#define PX4_EKF2_REPLAY_MID                       (0x0000)
#define PX4_ESC_REPORT_MID                        (0x0000)
#define PX4_ESC_STATUS_MID                        (0x0000)
#define PX4_ESTIMATOR_STATUS_MID                  (0x0000)
#define PX4_FENCE_MID                             (0x0000)
#define PX4_FENCE_VERTEX_MID                      (0x0000)
#define PX4_FILTERED_BOTTOM_FLOW_MID              (0x0000)
#define PX4_FOLLOW_TARGET_MID                     (0x0000)
#define PX4_GEOFENCE_RESULT_MID                   (0x0000)
#define PX4_GPS_DUMP_MID                          (0x0000)
#define PX4_GPS_INJECT_DATA_MID                   (0x0000)
#define PX4_HIL_SENSOR_MID                        (0x0000)
#define PX4_HOME_POSITION_MID                     (0x0000)
#define PX4_INPUT_RC_MID                          (0x0000)
#define PX4_LED_CONTROL_MID                       (0x0000)
#define PX4_LOG_MESSAGE_MID                       (0x0000)
#define PX4_MANUAL_CONTROL_SETPOINT_MID           (0x0000)
#define PX4_MAVLINK_LOG_MID                       (0x0000)
#define PX4_MC_ATT_CTRL_STATUS_MID                (0x0000)
#define PX4_MC_VIRTUAL_ATTITUDE_SETPOINT_MID      (0x0000)
#define PX4_MC_VIRTUAL_RATES_SETPOINT_MID         (0x0000)
#define PX4_MISSION_MID                           (0x0000)
#define PX4_MISSION_RESULT_MID                    (0x0000)
#define PX4_MULTIROTOR_MOTOR_LIMITS_MID           (0x0000)
#define PX4_OFFBOARD_CONTROL_MODE_MID             (0x0000)
#define PX4_OPTICAL_FLOW_MID                      (0x0000)
#define PX4_OUTPUT_PWM_MID                        (0x0000)
#define PX4_PARAMETER_UPDATE_MID                  (0x0000)
#define PX4_POSITION_SETPOINT_MID                 (0x0000)
#define PX4_POSITION_SETPOINT_TRIPLET_MID         (0x0000)
#define PX4_PWM_INPUT_MID                         (0x0000)
#define PX4_QSHELL_REQ_MID                        (0x0000)
#define PX4_RC_CHANNELS_MID                       (0x0000)
#define PX4_RC_PARAMETER_MAP_MID                  (0x0000)
#define PX4_SAFETY_MID                            (0x0000)
#define PX4_SATELLITE_INFO_MID                    (0x0000)
#define PX4_SENSOR_ACCEL_MID                      (0x0000)
#define PX4_SENSOR_BARO_MID                       (0x0000)
#define PX4_SENSOR_COMBINED_MID                   (0x0000)
#define PX4_SENSOR_GYRO_MID                       (0x0000)
#define PX4_SENSOR_MAG_MID                        (0x0000)
#define PX4_SERVORAIL_STATUS_MID                  (0x0000)
#define PX4_SUBSYSTEM_INFO_MID                    (0x0000)
#define PX4_SYSTEM_POWER_MID                      (0x0000)
#define PX4_TECS_STATUS_MID                       (0x0000)
#define PX4_TELEMETRY_STATUS_MID                  (0x0000)
#define PX4_TEST_MOTOR_MID                        (0x0000)
#define PX4_TIME_OFFSET_MID                       (0x0000)
#define PX4_TRANSPONDER_REPORT_MID                (0x0000)
#define PX4_UAVCAN_PARAMETER_REQUEST_MID          (0x0000)
#define PX4_UAVCAN_PARAMETER_VALUE_MID            (0x0000)
#define PX4_VEHICLE_ATTITUDE_MID                  (0x0000)
#define PX4_VEHICLE_ATTITUDE_SETPOINT_MID         (0x0000)
#define PX4_VEHICLE_COMMAND_ACK_MID               (0x0000)
#define PX4_VEHICLE_COMMAND_MID                   (0x0000)
#define PX4_VEHICLE_CONTROL_MODE_MID              (0x0000)
#define PX4_VEHICLE_FORCE_SETPOINT_MID            (0x0000)
#define PX4_VEHICLE_GLOBAL_POSITION_MID           (0x0000)
#define PX4_VEHICLE_GLOBAL_VELOCITY_SETPOINT_MID  (0x0000)
#define PX4_VEHICLE_GPS_POSITION_MID              (0x0000)
#define PX4_VEHICLE_LAND_DETECTED_MID             (0x0000)
#define PX4_VEHICLE_LOCAL_POSITION_MID            (0x0000)
#define PX4_VEHICLE_LOCAL_POSITION_SETPOINT_MID   (0x0000)
#define PX4_VEHICLE_RATES_SETPOINT_MID            (0x0000)
#define PX4_VEHICLE_STATUS_MID                    (0x0000)
#define PX4_VISION_POSITION_ESTIMATE_MID          (0x0000)
#define PX4_VTOL_VEHICLE_STATUS_MID               (0x0000)
#define PX4_WIND_ESTIMATE_MID                     (0x0000)
#define PX4_SENSOR_CORRECTION_MID                 (0x0000)
#define PX4_POSITION_CONTROL_STATUS_MID           (0x0000)



#endif
