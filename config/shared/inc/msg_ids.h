#ifndef MSG_IDS_H
#define MSG_IDS_H

#include "cfe_msgids.h"

/* Commands */

/* TO                                                      */
/* Range: 0x900 - 0x90f                                    */
#define TO_CMD_MID                                   (0x1900)
#define TO_SEND_HK_MID                               (0x1901)
#define TO_HK_TLM_MID                                (0x0902)
#define TO_SEND_TLM_MID                              (0x1903)
#define TO_DATA_TYPE_MID                             (0x0904)
#define TO_DIAG_TLM_MID                              (0x0911)
#define TO_DIAG_MSG_FLOW_MID                         (0x0912)

/* CI                                                      */
/* Range: 0x900 - 0x90f                                    */
#define CI_CMD_MID                                   (0x1905)
#define CI_SEND_HK_MID                               (0x1906)
#define CI_HK_TLM_MID                                (0x0907)
#define CI_WAKEUP_MID                                (0x1908)
#define CI_OUT_DATA_MID                              (0x1909)
#define CI_PROCESS_TIMEOUTS_MID                      (0x190a)

/* CF                                                      */
/* Range: 0x910 - 0x92f                                    */
#define CF_CMD_MID                                   (0x1910)
#define CF_INCOMING_PDU_MID                          (0x0FFD)
#define CF_SEND_HK_MID                               (0x1912)
#define CF_SPARE1_CMD_MID                            (0x1913)
#define CF_SPARE2_CMD_MID                            (0x1914)
#define CF_SPARE3_CMD_MID                            (0x1915)
#define CF_SPARE4_CMD_MID                            (0x1916)
#define CF_SPARE5_CMD_MID                            (0x1917)
#define CF_WAKE_UP_REQ_CMD_MID                       (0x1918)
#define CF_CONFIG_TLM_MID                            (0x0919)
#define CF_HK_TLM_MID                                (0x091a)
#define CF_SPARE0_TLM_MID                            (0x091b)
#define CF_SPARE1_TLM_MID                            (0x091c)
#define CF_SPARE2_TLM_MID                            (0x091d)
#define CF_SPARE3_TLM_MID                            (0x091e)
#define CF_SPARE4_TLM_MID                            (0x091f)
#define CF_TRANS_TLM_MID                             (0x0920)
/*
** NOTE: the definition below is NOT used by the code. The code uses the MsgId
** defined in the CF table. For the purpose of keeping all CF related message
** IDs defined in this file, the CF table should reference this macro
** definition.
*/
#define CF_SPACE_TO_GND_PDU_MID                      (0x1FFE)


/* CS                                                      */
/* Range: 0x930 - 0x93f                                    */
#define CS_CMD_MID                                   (0x1930)
#define CS_BACKGROUND_CYCLE_MID                      (0x1931)
#define CS_SEND_HK_MID                               (0x1932)
#define CS_HK_TLM_MID                                (0x0933)


/* DS                                                      */
/* Range: 0x940 - 0x94f                                    */
#define DS_CMD_MID                                   (0x1940)
#define DS_SEND_HK_MID                               (0x1941)
#define DS_HK_TLM_MID                                (0x0942)
#define DS_DIAG_TLM_MID                              (0x0943)


/* FM                                                      */
/* Range: 0x980 - 0x98f                                    */
#define FM_CMD_MID                                   (0x1980)
#define FM_SEND_HK_MID                               (0x1981)
#define FM_HK_TLM_MID                                (0x0982)
#define FM_OPEN_FILES_TLM_MID                        (0x0983)
#define FM_FREE_SPACE_TLM_MID                        (0x0984)
#define FM_FILE_INFO_TLM_MID                         (0x0985)
#define FM_DIR_LIST_TLM_MID                          (0x0986)


/* HK                                                      */
/* Range: 0x990 - 0x99f                                    */
#define HK_CMD_MID                                   (0x1990)
#define HK_SEND_HK_MID                               (0x1991)
#define HK_SEND_COMBINED_PKT_MID                     (0x1992)
#define HK_HK_TLM_MID                                (0x0993)
#define HK_COMBINED_PKT1_MID                         (0x0994)
#define HK_COMBINED_PKT2_MID                         (0x0995)
#define HK_COMBINED_PKT3_MID                         (0x0996)
#define HK_COMBINED_PKT4_MID                         (0x0997)
#define HK_COMBINED_PKT5_MID                         (0x0998)
#define HK_COMBINED_PKT6_MID                         (0x0999)
#define HK_COMBINED_PKT7_MID                         (0x099a)
#define HK_COMBINED_PKT8_MID                         (0x099b)
#define HK_COMBINED_PKT9_MID                         (0x099c)
#define HK_COMBINED_PKT10_MID                        (0x099d)


/* HS                                                      */
/* Range: 0x9a0 - 0x9af                                    */
#define HS_CMD_MID                                   (0x19a0)
#define HS_SEND_HK_MID                               (0x19a1)
#define HS_WAKEUP_MID                                (0x19a2)
#define HS_HK_TLM_MID                                (0x09a3)
#define HS_CUSTOM_HK_TLM_MID                         (0x09a4)


/* LC                                                      */
/* Range: 0x9b0 - 0x9bf                                    */
#define LC_HK_TLM_MID                                (0x09b0)
#define LC_SEND_HK_MID                               (0x19b1)
#define LC_CMD_MID                                   (0x19b2)
#define LC_SAMPLE_AP_MID                             (0x09b3)


/* MM                                                      */
/* Range: 0x9c0 - 0x9c2                                    */
#define MM_CMD_MID                                   (0x19c0)
#define MM_SEND_HK_MID                               (0x19c1)
#define MM_HK_TLM_MID                                (0x09c2)


/* MD                                                      */
/* Range: 0x9c3 - 0x9cf                                    */
#define MD_CMD_MID                                   (0x19c3)
#define MD_WAKEUP_MID                                (0x19c4)
#define MD_SEND_HK_MID                               (0x19c5)
#define MD_HK_TLM_MID                                (0x09c6)
#define MD_DWELL_PKT_MID_BASE                        (0x09c7)


/* SC                                                      */
/* Range: 0x9e0 - 0x9e3                                    */
#define SC_CMD_MID                                   (0x19e0)
#define SC_SEND_HK_MID                               (0x19e1)
#define SC_1HZ_WAKEUP_MID                            (0x19e2)
#define SC_HK_TLM_MID                                (0x09e3)


/* SCH                                                     */
/* Range: 0x9e4 - 0x9ef                                    */
#define SCH_CMD_MID                                  (0x19e4)
#define SCH_SEND_HK_MID                              (0x19e5)
#define SCH_UNUSED_MID                                    (0)
#define SCH_DIAG_TLM_MID                             (0x09e7)
#define SCH_HK_TLM_MID                               (0x09e8)
//#define SCH_ACTIVITY_DONE_MID                      (0x09e9)


/* GOPRO                                                   */
/* Range: 0x9f0 - 0x9ff                                    */
#define GOPRO_CMD_MID                                (0x19f0)
#define GOPRO_SEND_HK_MID                            (0x19f1)
#define GOPRO_HK_TLM_MID                             (0x09f2)


/* PX4                                                     */
/* Range: 0x9f0 - 0x9ff                                    */
#define PX4_ACTUATOR_ARMED_MID                       (0x0a05)
#define PX4_ACTUATOR_CONTROLS_0_MID                  (0x0a06)
#define PX4_ACTUATOR_CONTROLS_1_MID                  (0x0a5f)
#define PX4_ACTUATOR_CONTROLS_2_MID                  (0x0a5d)
#define PX4_ACTUATOR_CONTROLS_3_MID                  (0x0a5e)
#define PX4_ACTUATOR_DIRECT_MID                      (0x0a07)
#define PX4_ACTUATOR_OUTPUTS_MID                     (0x0a08)
#define PX4_ADC_REPORT_MID                           (0x0a09)
#define PX4_AIRSPEED_MID                             (0x0a0a)
#define PX4_ATT_POS_MOCAP_MID                        (0x0a0b)
#define PX4_BATTERY_STATUS_MID                       (0x0a0c)
#define PX4_CAMERA_TRIGGER_MID                       (0x0a0d)
#define PX4_COMMANDER_STATE_MID                      (0x0a0e)
#define PX4_CONTROL_STATE_MID                        (0x0a0f)
#define PX4_CPULOAD_MID                              (0x0a10)
#define PX4_DEBUG_KEY_VALUE_MID                      (0x0a11)
#define PX4_DIFFERENTIAL_PRESSURE_MID                (0x0a12)
#define PX4_DISTANCE_SENSOR_MID                      (0x0a13)
#define PX4_FW_POS_CTRL_STATUS_MID                   (0x0a14)
#define PX4_FW_VIRTUAL_ATTITUDE_SETPOINT_MID         (0x0a15)
#define PX4_FW_VIRTUAL_RATES_SETPOINT_MID            (0x0a16)
#define PX4_EKF2_INNOVATIONS_MID                     (0x0a17)
#define PX4_EKF2_REPLAY_MID                          (0x0a18)
#define PX4_ESC_REPORT_MID                           (0x0a19)
#define PX4_ESC_STATUS_MID                           (0x0a1a)
#define PX4_ESTIMATOR_STATUS_MID                     (0x0a1b)
#define PX4_FENCE_MID                                (0x0a1c)
#define PX4_FENCE_VERTEX_MID                         (0x0a1d)
#define PX4_FILTERED_BOTTOM_FLOW_MID                 (0x0a1e)
#define PX4_FOLLOW_TARGET_MID                        (0x0a1f)
#define PX4_GEOFENCE_RESULT_MID                      (0x0a20)
#define PX4_GPS_DUMP_MID                             (0x0a21)
#define PX4_GPS_INJECT_DATA_MID                      (0x0a22)
#define PX4_HIL_SENSOR_MID                           (0x0a23)
#define PX4_HOME_POSITION_MID                        (0x0a24)
#define PX4_INPUT_RC_MID                             (0x0a25)
#define PX4_LED_CONTROL_MID                          (0x0a5c)
#define PX4_LOG_MESSAGE_MID                          (0x0a26)
#define PX4_MANUAL_CONTROL_SETPOINT_MID              (0x0a27)
#define PX4_MAVLINK_LOG_MID                          (0x0a28)
#define PX4_MC_ATT_CTRL_STATUS_MID                   (0x0a29)
#define PX4_MC_VIRTUAL_ATTITUDE_SETPOINT_MID         (0x0a2a)
#define PX4_MC_VIRTUAL_RATES_SETPOINT_MID            (0x0a2b)
#define PX4_MISSION_MID                              (0x0a2c)
#define PX4_MISSION_RESULT_MID                       (0x0a2d)
#define PX4_MULTIROTOR_MOTOR_LIMITS_MID              (0x0a2e)
#define PX4_OFFBOARD_CONTROL_MODE_MID                (0x0a2f)
#define PX4_OPTICAL_FLOW_MID                         (0x0a30)
#define PX4_OUTPUT_PWM_MID                           (0x0a31)
#define PX4_PARAMETER_UPDATE_MID                     (0x0a32)
#define PX4_POSITION_SETPOINT_MID                    (0x0a33)
#define PX4_POSITION_SETPOINT_TRIPLET_MID            (0x0a34)
#define PX4_PWM_INPUT_MID                            (0x0a35)
#define PX4_QSHELL_REQ_MID                           (0x0a36)
#define PX4_RC_CHANNELS_MID                          (0x0a37)
#define PX4_RC_PARAMETER_MAP_MID                     (0x0a38)
#define PX4_SAFETY_MID                               (0x0a39)
#define PX4_SATELLITE_INFO_MID                       (0x0a3a)
#define PX4_SENSOR_ACCEL_MID                         (0x0a3b)
#define PX4_SENSOR_BARO_MID                          (0x0a3c)
#define PX4_SENSOR_COMBINED_MID                      (0x0a3d)
#define PX4_SENSOR_GYRO_MID                          (0x0a3e)
#define PX4_SENSOR_MAG_MID                           (0x0a3f)
#define PX4_SERVORAIL_STATUS_MID                     (0x0a40)
#define PX4_SUBSYSTEM_INFO_MID                       (0x0a41)
#define PX4_SYSTEM_POWER_MID                         (0x0a42)
#define PX4_TECS_STATUS_MID                          (0x0a43)
#define PX4_TELEMETRY_STATUS_MID                     (0x0a44)
#define PX4_TEST_MOTOR_MID                           (0x0a45)
#define PX4_TIME_OFFSET_MID                          (0x0a46)
#define PX4_TRANSPONDER_REPORT_MID                   (0x0a47)
#define PX4_UAVCAN_PARAMETER_REQUEST_MID             (0x0a48)
#define PX4_UAVCAN_PARAMETER_VALUE_MID               (0x0a49)
#define PX4_VEHICLE_ATTITUDE_MID                     (0x0a4a)
#define PX4_VEHICLE_ATTITUDE_SETPOINT_MID            (0x0a4b)
#define PX4_VEHICLE_COMMAND_ACK_MID                  (0x0a4c)
#define PX4_VEHICLE_COMMAND_MID                      (0x0a4d)
#define PX4_VEHICLE_CONTROL_MODE_MID                 (0x0a4e)
#define PX4_VEHICLE_FORCE_SETPOINT_MID               (0x0a4f)
#define PX4_VEHICLE_GLOBAL_POSITION_MID              (0x0a50)
#define PX4_VEHICLE_GLOBAL_VELOCITY_SETPOINT_MID     (0x0a51)
#define PX4_VEHICLE_GPS_POSITION_MID                 (0x0a52)
#define PX4_VEHICLE_LAND_DETECTED_MID                (0x0a53)
#define PX4_VEHICLE_LOCAL_POSITION_MID               (0x0a54)
#define PX4_VEHICLE_LOCAL_POSITION_SETPOINT_MID      (0x0a55)
#define PX4_VEHICLE_RATES_SETPOINT_MID               (0x0a56)
#define PX4_VEHICLE_STATUS_MID                       (0x0a57)
#define PX4_VISION_POSITION_ESTIMATE_MID             (0x0a58)
#define PX4_VTOL_VEHICLE_STATUS_MID                  (0x0a59)
#define PX4_WIND_ESTIMATE_MID                        (0x0a5a)
#define PX4_SENSOR_CORRECTION_MID                    (0x0a5b)

/* VID                                                     */
/* Range: 0xb20 - 0xb2f                                    */
#define VID_CMD_MID                                  (0x1b40)
#define VID_SEND_HK_MID                              (0x1b41)
#define VID_HK_TLM_MID                               (0x0b42)
#define VID_GRAB_FRAME_MID                           (0x1b43)
#define VID_FWD_CAM_FRAME_MID                        (0x0b44)

#define MPU9250_SEND_HK_MID                          (0x1b50)
#define MPU9250_HK_TLM_MID                           (0x0b51)
#define MPU9250_DIAG_TLM_MID                         (0x0b52)
#define MPU9250_CMD_MID                              (0x1b53)
#define MPU9250_MEASURE_MID                          (0x1b54)

#define MS5611_SEND_HK_MID                           (0x1b60)
#define MS5611_HK_TLM_MID                            (0x0b61)
#define MS5611_DIAG_TLM_MID                          (0x0b62)
#define MS5611_CMD_MID                               (0x1b63)
#define MS5611_MEASURE_MID                           (0x1b64)

#define NEOM8N_SEND_HK_MID                           (0x1b70)
#define NEOM8N_HK_TLM_MID                            (0x0b71)
#define NEOM8N_DIAG_TLM_MID                          (0x0b72)
#define NEOM8N_CMD_MID                               (0x1b73)
#define NEOM8N_MEASURE_MID                           (0x1b74)
#define NEOM8N_PROC_CMDS_MID                         (0x1b75)
#define NEOM8N_CAL_MEAS_MID                          (0x0b76)
#define NEOM8N_NAV_POSECEF_MID                       (0x0b77)
#define NEOM8N_NAV_POSLLH_MID                        (0x0b78)
#define NEOM8N_NAV_STATUS_MID                        (0x0b79)
#define NEOM8N_NAV_DOP_MID                           (0x0b7a)
#define NEOM8N_NAV_ATT_MID                           (0x0b7b)
#define NEOM8N_NAV_SOL_MID                           (0x0b7c)
#define NEOM8N_NAV_NAVPVT_MID                        (0x0b7d)
#define NEOM8N_NAV_ODO_MID                           (0x0b7e)
#define NEOM8N_NAV_RESETODO_MID                      (0x0b7f)
#define NEOM8N_NAV_VELECEF_MID                       (0x0b80)
#define NEOM8N_NAV_VELNED_MID                        (0x0b81)
#define NEOM8N_NAV_TIMEGPS_MID                       (0x0b82)
#define NEOM8N_NAV_TIMEUTC_MID                       (0x0b83)
#define NEOM8N_NAV_CLOCK_MID                         (0x0b84)
#define NEOM8N_NAV_TIMEGLO_MID                       (0x0b85)
#define NEOM8N_NAV_TIMEBDS_MID                       (0x0b86)
#define NEOM8N_NAV_TIMEGAL_MID                       (0x0b87)
#define NEOM8N_NAV_TIMELS_MID                        (0x0b88)
#define NEOM8N_NAV_SVINFO_MID                        (0x0b89)
#define NEOM8N_NAV_DGPS_MID                          (0x0b8a)
#define NEOM8N_NAV_SBAS_MID                          (0x0b8b)
#define NEOM8N_NAV_ORB_MID                           (0x0b8c)
#define NEOM8N_NAV_NAVSAT_MID                        (0x0b8d)
#define NEOM8N_NAV_GEOFENCE_MID                      (0x0b8e)
#define NEOM8N_NAV_AOPSTATUS_MID                     (0x0b8f)
#define NEOM8N_NAV_EOE_MID                           (0x0b90)
#define NEOM8N_MON_IO_MID                            (0x0b91)
#define NEOM8N_MON_VER_MID                           (0x0b92)
#define NEOM8N_MON_MSGPP_MID                         (0x0b93)
#define NEOM8N_MON_RXBUF_MID                         (0x0b94)
#define NEOM8N_MON_TXBUF_MID                         (0x0b95)
#define NEOM8N_MON_HW_MID                            (0x0b96)
#define NEOM8N_MON_HW2_MID                           (0x0b97)
#define NEOM8N_MON_RXR_MID                           (0x0b98)
#define NEOM8N_MON_PATCH_MID                         (0x0b99)
#define NEOM8N_MON_GNSS_MID                          (0x0b9a)
#define NEOM8N_MON_SMGR_MID                          (0x0b9b)


/* SBN                                                     */
#define SBN_WAKEUP_MID                               (0x0ba0)
#define SBN_CMD_MID                                  (0x1ba1)
#define SBN_TLM_MID                                  (0x0ba2)

/* ROS                                                     */
/* Range: 0x900 - 0x90f                                    */
#define ROS_CMD_MID                                  (0x1bb0)
#define ROS_SEND_HK_MID                              (0x1bb1)
#define ROS_HK_TLM_MID                               (0x0bb3)
#define ROS_SEND_TLM_MID                             (0x0bb4)

/* ETA                                                     */
/* Range: 0x900 - 0x90f                                    */
#define ETA_CMD_MID                                  (0x1bc0)
#define ETA_SEND_HK_MID                              (0x1bc1)
#define ETA_DATA_TYPES_MID                           (0x0bc2)
#define ETA_HK_TLM_MID                               (0x0bc3)
#define ETA_SEND_TLM_MID                             (0x0bc4)


/* PX4UORB                                                 */
/* Range: 0x900 - 0x90f                                    */
#define PX4UORB_CMD_MID                              (0x1bd4)
#define PX4UORB_SEND_HK_MID                          (0x1bd5)
#define PX4UORB_HK_TLM_MID                           (0x0bd6)
#define PX4UORB_READ_CMD_MID                         (0x1bd7)


/* PX4AEQ                                                  */
/* Range: 0x900 - 0x90f                                    */
#define PX4AEQ_CMD_MID                               (0x1bd8)
#define PX4AEQ_SEND_HK_MID                           (0x1bd9)
#define PX4AEQ_HK_TLM_MID                            (0x0bda)
#define PX4AEQ_READ_CMD_MID                          (0x1bdb)


/* PX4SEN                                                  */
/* Range: 0x900 - 0x90f                                    */
#define PX4SEN_CMD_MID                               (0x1bdc)
#define PX4SEN_SEND_HK_MID                           (0x1bdd)
#define PX4SEN_HK_TLM_MID                            (0x0bde)
#define PX4SEN_READ_CMD_MID                          (0x1bdf)


/* PX4SIM                                                  */
/* Range: 0x900 - 0x90f                                    */
#define PX4SIM_CMD_MID                               (0x1be0)
#define PX4SIM_SEND_HK_MID                           (0x1be1)
#define PX4SIM_HK_TLM_MID                            (0x0be2)
#define PX4SIM_READ_CMD_MID                          (0x1be3)


/* PX4ASIM                                                 */
/* Range: 0x900 - 0x90f                                    */
#define PX4ASIM_CMD_MID                              (0x1be4)
#define PX4ASIM_SEND_HK_MID                          (0x1be5)
#define PX4ASIM_HK_TLM_MID                           (0x0be6)
#define PX4ASIM_READ_CMD_MID                         (0x1be7)


/* PX4ADSM                                                 */
/* Range: 0x900 - 0x90f                                    */
#define PX4ADSM_CMD_MID                              (0x1be8)
#define PX4ADSM_SEND_HK_MID                          (0x1be9)
#define PX4ADSM_HK_TLM_MID                           (0x0bea)
#define PX4ADSM_READ_CMD_MID                         (0x1beb)


/* PX4BASM                                                 */
/* Range: 0x900 - 0x90f                                    */
#define PX4BASM_CMD_MID                              (0x1bf0)
#define PX4BASM_SEND_HK_MID                          (0x1bf1)
#define PX4BASM_HK_TLM_MID                           (0x0bf2)
#define PX4BASM_READ_CMD_MID                         (0x1bf3)


/* PX4DMAN                                                 */
/* Range: 0x900 - 0x90f                                    */
#define PX4DMAN_CMD_MID                              (0x1bf8)
#define PX4DMAN_SEND_HK_MID                          (0x1bf9)
#define PX4DMAN_HK_TLM_MID                           (0x0bfa)
#define PX4DMAN_READ_CMD_MID                         (0x1bfb)


/* PX4GPSM                                                 */
/* Range: 0x900 - 0x90f                                    */
#define PX4GPSM_CMD_MID                              (0x1bfc)
#define PX4GPSM_SEND_HK_MID                          (0x1bfd)
#define PX4GPSM_HK_TLM_MID                           (0x0bfe)
#define PX4GPSM_READ_CMD_MID                         (0x1bff)

/* PX4GSIM                                                 */
/* Range: 0x900 - 0x90f                                    */
#define PX4GSIM_CMD_MID                              (0x1c04)
#define PX4GSIM_SEND_HK_MID                          (0x1c05)
#define PX4GSIM_HK_TLM_MID                           (0x0c06)
#define PX4GSIM_READ_CMD_MID                         (0x1c07)


/* PX4LPEQ                                                 */
/* Range: 0x900 - 0x90f                                    */
#define PX4LPEQ_CMD_MID                              (0x1c08)
#define PX4LPEQ_SEND_HK_MID                          (0x1c09)
#define PX4LPEQ_HK_TLM_MID                           (0x0c0a)
#define PX4LPEQ_READ_CMD_MID                         (0x1c0b)


/* PX4MAC                                                  */
/* Range: 0x900 - 0x90f                                    */
#define PX4MAC_CMD_MID                               (0x1c0c)
#define PX4MAC_SEND_HK_MID                           (0x1c0d)
#define PX4MAC_HK_TLM_MID                            (0x0c0e)
#define PX4MAC_READ_CMD_MID                          (0x1c0f)


/* PX4MPC                                                  */
/* Range: 0x900 - 0x90f                                    */
#define PX4MPC_CMD_MID                               (0x1c10)
#define PX4MPC_SEND_HK_MID                           (0x1c11)
#define PX4MPC_HK_TLM_MID                            (0x0c12)
#define PX4MPC_READ_CMD_MID                          (0x1c13)


/* PX4NAV                                                  */
/* Range: 0x900 - 0x90f                                    */
#define PX4NAV_CMD_MID                               (0x1c14)
#define PX4NAV_SEND_HK_MID                           (0x1c15)
#define PX4NAV_HK_TLM_MID                            (0x0c16)
#define PX4NAV_READ_CMD_MID                          (0x1c17)


/* PX4POS                                                  */
/* Range: 0x900 - 0x90f                                    */
#define PX4POS_CMD_MID                               (0x1c18)
#define PX4POS_SEND_HK_MID                           (0x1c19)
#define PX4POS_HK_TLM_MID                            (0x0c1a)
#define PX4POS_READ_CMD_MID                          (0x1c1b)


/* PX4RSIM                                                 */
/* Range: 0x900 - 0x90f                                    */
#define PX4RSIM_CMD_MID                              (0x1c1c)
#define PX4RSIM_SEND_HK_MID                          (0x1c1d)
#define PX4RSIM_HK_TLM_MID                           (0x0c1e)
#define PX4RSIM_READ_CMD_MID                         (0x1c1f)


/* PX4TARM                                                 */
/* Range: 0x900 - 0x90f                                    */
#define PX4TARM_CMD_MID                              (0x1c20)
#define PX4TARM_SEND_HK_MID                          (0x1c21)
#define PX4TARM_HK_TLM_MID                           (0x0c22)
#define PX4TARM_READ_CMD_MID                         (0x1c23)


/* PX4LDSM                                                 */
/* Range: 0x900 - 0x90f                                    */
#define PX4LDSM_CMD_MID                              (0x1c24)
#define PX4LDSM_SEND_HK_MID                          (0x1c25)
#define PX4LDSM_HK_TLM_MID                           (0x0c26)
#define PX4LDSM_READ_CMD_MID                         (0x1c27)

#define SBNPX4_WAKEUP_MID                            (0x0c28)

/* EA                                                      */
#define EA_CMD_MID                                   (0x1c29)
#define EA_SEND_HK_MID                               (0x1c2a)
#define EA_HK_TLM_MID                                (0x0c2b)
#define EA_WAKEUP_MID                                (0x1c2c)
#define EA_OUT_DATA_MID                              (0x0c2d)
#define EA_PERFMON_MID                               (0x1c2e)

/* VC                                                      */
#define VC_HK_TLM_MID                                (0x0c39)
#define VC_SEND_HK_MID                               (0x1c3a)
#define VC_CMD_MID                                   (0x1c3b)
#define VC_PROCESS_CMDS_MID                          (0x1c3c)

/* AMC                                                     */
#define AMC_CMD_MID                                  (0x1c3d)
#define AMC_SEND_HK_MID                              (0x1c3e)
#define AMC_UPDATE_MOTORS_MID                        (0x1c3f)
#define AMC_OUT_DATA_MID                             (0x0c40)
#define AMC_HK_TLM_MID                               (0x0c41)

/* MAC                                                     */
#define MAC_CMD_MID                                  (0x1c42)
#define MAC_SEND_HK_MID                              (0x1c43)
#define MAC_RUN_CONTROLLER_MID                       (0x1c44)
#define MAC_OUT_DATA_MID                             (0x0c45)
#define MAC_HK_TLM_MID                               (0x0c46)

/* ULR                                                     */
#define ULR_CMD_MID                                  (0x1c47)
#define ULR_SEND_HK_MID                              (0x1c48)
#define ULR_MEASURE_MID                              (0x1c49)
#define ULR_OUT_DATA_MID                             (0x0c4a)
#define ULR_HK_TLM_MID                               (0x0c4b)

/* RGBLED                                                  */
#define RGBLED_CMD_MID                               (0x1c4c)
#define RGBLED_SEND_HK_MID                           (0x1c4d)
#define RGBLED_WAKEUP_MID                            (0x1c4e)
#define RGBLED_HK_TLM_MID                            (0x0c4f)

/* GPS                                                     */
#define GPS_CMD_MID                                  (0x1c51)
#define GPS_SEND_HK_MID                              (0x1c52)
#define GPS_READ_SENSOR_MID                          (0x1c53)
#define GPS_HK_TLM_MID                               (0x0c54)

/* SENS                                                    */
#define SENS_HK_TLM_MID                              (0x0c55)
#define SENS_SEND_HK_MID                             (0x1c56)
#define SENS_WAKEUP_MID                              (0x1c57)
#define SENS_CMD_MID                                 (0x1c58)

/* QAE                                                     */
#define QAE_HK_TLM_MID                               (0x0c59)
#define QAE_SEND_HK_MID                              (0x1c5a)
#define QAE_WAKEUP_MID                               (0x1c5b)
#define QAE_CMD_MID                                  (0x1c5c)

/* LD                                                      */
#define LD_HK_TLM_MID                                (0x0c5d)
#define LD_SEND_HK_MID                               (0x1c5e)
#define LD_WAKEUP_MID                                (0x1c5f)
#define LD_CMD_MID                                   (0x1c60)

/* MPC                                                     */
#define MPC_HK_TLM_MID                               (0x0c61)
#define MPC_SEND_HK_MID                              (0x1c62)
#define MPC_WAKEUP_MID                               (0x1c63)
#define MPC_CMD_MID                                  (0x1c64)

/* NAV                                                     */
#define NAV_HK_TLM_MID                               (0x0c65)
#define NAV_SEND_HK_MID                              (0x1c66)
#define NAV_WAKEUP_MID                               (0x1c67)
#define NAV_CMD_MID                                  (0x1c68)

/* RCIN                                                    */
#define RCIN_HK_TLM_MID                              (0x0c69)
#define RCIN_SEND_HK_MID                             (0x1c6a)
#define RCIN_WAKEUP_MID                              (0x1c6b)
#define RCIN_CMD_MID                                 (0x1c6c)

/* VM                                                      */
#define VM_HK_TLM_MID                                (0x0c6d)
#define VM_SEND_HK_MID                               (0x1c6e)
#define VM_WAKEUP_MID                                (0x1c6f)
#define VM_CMD_MID                                   (0x1c70)

/* BAT                                                     */
#define BAT_HK_TLM_MID                               (0x0c71)
#define BAT_SEND_HK_MID                              (0x1c72)
#define BAT_WAKEUP_MID                               (0x1c73)
#define BAT_CMD_MID                                  (0x1c74)

/* PE                                                      */
#define PE_HK_TLM_MID                                (0x0c79)
#define PE_SEND_HK_MID                               (0x1c7a)
#define PE_WAKEUP_MID                                (0x1c7b)
#define PE_CMD_MID                                   (0x1c7c)

/* SIM                                                     */
#define SIM_HK_TLM_MID                               (0x0c7d)
#define SIM_SEND_HK_MID                              (0x1c7e)
#define SIM_WAKEUP_MID                               (0x1c7f)
#define SIM_CMD_MID                                  (0x1c80)

/* MAVLINK                                                 */
#define MAVLINK_CMD_MID                              (0x1c81)
#define MAVLINK_SEND_HK_MID                          (0x1c82)
#define MAVLINK_HK_TLM_MID                           (0x0c83)
#define MAVLINK_WAKEUP_MID                           (0x1c84)
#define MAVLINK_OUT_DATA_MID                         (0x1c85)
#define MAVLINK_PARAM_DATA_MID                       (0x1c86)
#define MAVLINK_REQUEST_PARAMS_MID                   (0x1c87)

/* Individual GPS messages                                 */
#define GPS_NAV_DOP_MID                              (0x0c87)
#define GPS_NAV_NAVPVT_MID                           (0x0c88)
#define GPS_NAV_SVINFO_MID                           (0x0c89)
#define GPS_ACK_NAK_MID                              (0x0c90)
#define GPS_ACK_ACK_MID                              (0x0c91)
#define GPS_CFG_PRT_MID                              (0x0c92)
#define GPS_CFG_MSG_MID                              (0x0c93)
#define GPS_CFG_RATE_MID                             (0x0c94)
#define GPS_CFG_SBAS_MID                             (0x0c95)
#define GPS_CFG_NAV5_MID                             (0x0c96)
#define GPS_MON_HW_MID                               (0x0c97)

/* HMC5883                                                 */
#define HMC5883_HK_TLM_MID                           (0x0ca1)
#define HMC5883_SEND_HK_MID                          (0x1ca2)
#define HMC5883_WAKEUP_MID                           (0x1ca3)
#define HMC5883_CMD_MID                              (0x1ca4)
#define HMC5883_DIAG_TLM_MID                         (0x0ca5)

/* PRMLIB                                                  */
#define PRMLIB_PARAM_UPDATED_MID                     (0x0ca6)

/* LGC                                                     */
#define LGC_HK_TLM_MID                               (0x0cb0)
#define LGC_SEND_HK_MID                              (0x1cb1)
#define LGC_WAKEUP_MID                               (0x1cb2)
#define LGC_CMD_MID                                  (0x1cb3)

/* MS5607                                                  */
#define MS5607_SEND_HK_MID                           (0x0cb5)
#define MS5607_HK_TLM_MID                            (0x0cb6)
#define MS5607_DIAG_TLM_MID                          (0x0cb7)
#define MS5607_CMD_MID                               (0x0cb8)
#define MS5607_MEASURE_MID                           (0x0cb9)

/* AK8963                                                  */
#define AK8963_HK_TLM_MID                            (0x0cc1)
#define AK8963_SEND_HK_MID                           (0x1cc2)
#define AK8963_WAKEUP_MID                            (0x1cc3)
#define AK8963_CMD_MID                               (0x1cc4)
#define AK8963_DIAG_TLM_MID                          (0x0cc5)

/* MPU6050                                                 */
#define MPU6050_SEND_HK_MID                          (0x1cd0)
#define MPU6050_HK_TLM_MID                           (0x0cd1)
#define MPU6050_DIAG_TLM_MID                         (0x0cd2)
#define MPU6050_CMD_MID                              (0x1cd3)
#define MPU6050_MEASURE_MID                          (0x1cd4)

/* SONAR                                                   */
#define SONAR_CMD_MID                                (0x1cd6)
#define SONAR_SEND_HK_MID                            (0x1cd7)
#define SONAR_MEASURE_MID                            (0x1cd8)
#define SONAR_OUT_DATA_MID                           (0x0cd9)
#define SONAR_HK_TLM_MID                             (0x0ce0)

/* FLOW                                                    */
#define FLOW_HK_TLM_MID                              (0x0ce1)
#define FLOW_SEND_HK_MID                             (0x1ce2)
#define FLOW_WAKEUP_MID                              (0x1ce3)
#define FLOW_CMD_MID                                 (0x1ce4)
#define FLOW_FRAME_MID                               (0x0ce5)
#define FLOW_DIAG_TLM_MID                            (0x0ce6)

/* MPC                                                     */
#define MPC_DIAG_TLM_MID                             (0x0cea)

/* LD                                                      */
#define LD_DIAG_TLM_MID                              (0x0ceb)


#define VM_CONFIG_TLM_MID                            (0x0cec)

/* PRM                                                     */
#define PRM_CMD_MID                                  (0x1ced)
#define PRM_SEND_HK_MID                              (0x1cee)
#define PRM_WAKEUP_MID                               (0x1cef)
#define PRM_OUT_DATA_MID                             (0x1cf0)
#define PRM_HK_TLM_MID                               (0x0cf1)

#define SCH_ACTIVITY_DONE_MID                        (0x0f00)

#endif
