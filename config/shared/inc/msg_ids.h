#ifndef MSG_IDS_H
#define MSG_IDS_H


#include "cfe_mission_cfg.h"
#include "cfe_platform_cfg.h"


#define CMD_MSG(X)  (CFE_MSG_CPU_BASE + CFE_CMD_MID_BASE + X)
#define TLM_MSG(X)  (CFE_MSG_CPU_BASE + CFE_TLM_MID_BASE + X)



#define CFE_EVS_CMD_MID                           CMD_MSG(    0 )
#define CFE_SB_CMD_MID                            CMD_MSG(    1 )
#define CFE_TBL_CMD_MID                           CMD_MSG(    2 )
#define CFE_TIME_CMD_MID                          CMD_MSG(    3 )
#define CFE_ES_CMD_MID                            CMD_MSG(    4 )

#define CFE_ES_SEND_HK_MID                        CMD_MSG(    5 )
#define CFE_EVS_SEND_HK_MID                       CMD_MSG(    6 )
#define CFE_SB_SEND_HK_MID                        CMD_MSG(    7 )
#define CFE_TBL_SEND_HK_MID                       CMD_MSG(    8 )
#define CFE_TIME_SEND_HK_MID                      CMD_MSG(    9 )

#define CFE_TIME_TONE_CMD_MID                     CMD_MSG(   10 )
#define CFE_TIME_1HZ_CMD_MID                      CMD_MSG(   11 )


/*
** cFE Global Command Message Id's
*/
#define CFE_TIME_DATA_CMD_MID                     TLM_MSG(   12 )
#define CFE_TIME_FAKE_CMD_MID                     TLM_MSG(   13 )
#define CFE_TIME_SEND_CMD_MID                     TLM_MSG(   14 )


/*
** CFE Telemetry Message Id's
*/
#define CFE_ES_HK_TLM_MID                         TLM_MSG(   15 )
#define CFE_EVS_HK_TLM_MID                        TLM_MSG(   16 )
#define CFE_SB_HK_TLM_MID                         TLM_MSG(   17 )
#define CFE_TBL_HK_TLM_MID                        TLM_MSG(   18 )
#define CFE_TIME_HK_TLM_MID                       TLM_MSG(   19 )
#define CFE_TIME_DIAG_TLM_MID                     TLM_MSG(   20 )
#define CFE_EVS_EVENT_MSG_MID                     TLM_MSG(   21 )
#define CFE_SB_STATS_TLM_MID                      TLM_MSG(   22 )
#define CFE_ES_APP_TLM_MID                        TLM_MSG(   23 )
#define CFE_TBL_REG_TLM_MID                       TLM_MSG(   24 )
#define CFE_SB_ALLSUBS_TLM_MID                    TLM_MSG(   25 )
#define CFE_SB_ONESUB_TLM_MID                     TLM_MSG(   26 )
#define CFE_ES_SHELL_TLM_MID                      TLM_MSG(   27 )
#define CFE_ES_MEMSTATS_TLM_MID                   TLM_MSG(   28 )


/* CI                                                          */
#define CI_CMD_MID                                CMD_MSG(   50 )
#define CI_SEND_HK_MID                            CMD_MSG(   51 )
#define CI_HK_TLM_MID                             TLM_MSG(   52 )
#define CI_WAKEUP_MID                             CMD_MSG(   53 )
#define CI_OUT_DATA_MID                           TLM_MSG(   54 )
#define CI_PROCESS_TIMEOUTS_MID                   CMD_MSG(   55 )


/* CF                                                          */
#define CF_CMD_MID                                CMD_MSG(   56 )
#define CF_INCOMING_PDU_MID                       TLM_MSG(   57 )
#define CF_SEND_HK_MID                            CMD_MSG(   58 )
#define CF_SPARE1_CMD_MID                         CMD_MSG(   59 )
#define CF_SPARE2_CMD_MID                         CMD_MSG(   60 )
#define CF_SPARE3_CMD_MID                         CMD_MSG(   61 )
#define CF_SPARE4_CMD_MID                         CMD_MSG(   62 )
#define CF_SPARE5_CMD_MID                         CMD_MSG(   63 )
#define CF_WAKE_UP_REQ_CMD_MID                    CMD_MSG(   64 )
#define CF_CONFIG_TLM_MID                         TLM_MSG(   65 )
#define CF_HK_TLM_MID                             TLM_MSG(   66 )
#define CF_SPARE0_TLM_MID                         TLM_MSG(   67 )
#define CF_SPARE1_TLM_MID                         TLM_MSG(   68 )
#define CF_SPARE2_TLM_MID                         TLM_MSG(   69 )
#define CF_SPARE3_TLM_MID                         TLM_MSG(   70 )
#define CF_SPARE4_TLM_MID                         TLM_MSG(   71 )
#define CF_TRANS_TLM_MID                          TLM_MSG(   72 )
#define CF_SPACE_TO_GND_PDU_MID                   CMD_MSG(   73 )


/* CS                                                          */
#define CS_CMD_MID                                CMD_MSG(   80 )
#define CS_BACKGROUND_CYCLE_MID                   CMD_MSG(   81 )
#define CS_SEND_HK_MID                            CMD_MSG(   82 )
#define CS_HK_TLM_MID                             TLM_MSG(   83 )


/* DS                                                          */
#define DS_CMD_MID                                CMD_MSG(   84 )
#define DS_SEND_HK_MID                            CMD_MSG(   85 )
#define DS_HK_TLM_MID                             TLM_MSG(   86 )
#define DS_DIAG_TLM_MID                           TLM_MSG(   87 )


/* FM                                                          */
#define FM_CMD_MID                                CMD_MSG(   88 )
#define FM_SEND_HK_MID                            CMD_MSG(   89 )
#define FM_HK_TLM_MID                             TLM_MSG(   90 )
#define FM_OPEN_FILES_TLM_MID                     TLM_MSG(   91 )
#define FM_FREE_SPACE_TLM_MID                     TLM_MSG(   92 )
#define FM_FILE_INFO_TLM_MID                      TLM_MSG(   93 )
#define FM_DIR_LIST_TLM_MID                       TLM_MSG(   94 )


/* HK                                                        */
#define HK_CMD_MID                                CMD_MSG(  100 )
#define HK_SEND_HK_MID                            CMD_MSG(  101 )
#define HK_SEND_COMBINED_PKT_MID                  CMD_MSG(  102 )
#define HK_HK_TLM_MID                             TLM_MSG(  103 )
#define HK_COMBINED_PKT1_MID                      TLM_MSG(  104 )
#define HK_COMBINED_PKT2_MID                      TLM_MSG(  105 )
#define HK_COMBINED_PKT3_MID                      TLM_MSG(  106 )
#define HK_COMBINED_PKT4_MID                      TLM_MSG(  107 )
#define HK_COMBINED_PKT5_MID                      TLM_MSG(  108 )
#define HK_COMBINED_PKT6_MID                      TLM_MSG(  109 )
#define HK_COMBINED_PKT7_MID                      TLM_MSG(  110 )
#define HK_COMBINED_PKT8_MID                      TLM_MSG(  111 )
#define HK_COMBINED_PKT9_MID                      TLM_MSG(  112 )
#define HK_COMBINED_PKT10_MID                     TLM_MSG(  113 )


/* HS                                                        */
#define HS_CMD_MID                                CMD_MSG(  120 )
#define HS_SEND_HK_MID                            CMD_MSG(  121 )
#define HS_WAKEUP_MID                             CMD_MSG(  122 )
#define HS_HK_TLM_MID                             TLM_MSG(  123 )
#define HS_CUSTOM_HK_TLM_MID                      TLM_MSG(  124 )


/* LC                                                        */
#define LC_HK_TLM_MID                             TLM_MSG(  130 )
#define LC_SEND_HK_MID                            CMD_MSG(  131 )
#define LC_CMD_MID                                CMD_MSG(  132 )
#define LC_SAMPLE_AP_MID                          CMD_MSG(  133 )


/* MD                                                        */
#define MD_CMD_MID                                CMD_MSG(  134 )
#define MD_WAKEUP_MID                             CMD_MSG(  135 )
#define MD_SEND_HK_MID                            CMD_MSG(  136 )
#define MD_HK_TLM_MID                             TLM_MSG(  137 )
#define MD_DWELL_PKT_MID_BASE                     TLM_MSG(  138 )


/* MM                                                        */
#define MM_CMD_MID                                CMD_MSG(  139 )
#define MM_SEND_HK_MID                            CMD_MSG(  140 )
#define MM_HK_TLM_MID                             TLM_MSG(  141 )


/* TO                                                        */
#define TO_CMD_MID                                CMD_MSG(  142 )
#define TO_SEND_HK_MID                            CMD_MSG(  143 )
#define TO_HK_TLM_MID                             TLM_MSG(  144 )
#define TO_SEND_TLM_MID                           CMD_MSG(  145 )
#define TO_DATA_TYPE_MID                          TLM_MSG(  146 )
#define TO_DIAG_TLM_MID                           TLM_MSG(  147 )
#define TO_DIAG_MSG_FLOW_MID                      TLM_MSG(  148 )


/* SC                                                        */
#define SC_CMD_MID                                CMD_MSG(  150 )
#define SC_SEND_HK_MID                            CMD_MSG(  151 )
#define SC_1HZ_WAKEUP_MID                         CMD_MSG(  152 )
#define SC_HK_TLM_MID                             TLM_MSG(  153 )


/* SCH                                                       */
#define SCH_CMD_MID                               CMD_MSG(  154 )
#define SCH_SEND_HK_MID                           CMD_MSG(  155 )
#define SCH_UNUSED_MID                                    (0)
#define SCH_DIAG_TLM_MID                          TLM_MSG(  156 )
#define SCH_HK_TLM_MID                            TLM_MSG(  157 )
#define SCH_ACTIVITY_DONE_MID                     CMD_MSG(  158 )


/* PX4                                                       */
#define PX4_ACTUATOR_ARMED_MID                    TLM_MSG(  160 )
#define PX4_ACTUATOR_CONTROLS_0_MID               TLM_MSG(  161 )
#define PX4_ACTUATOR_CONTROLS_1_MID               TLM_MSG(  162 )
#define PX4_ACTUATOR_CONTROLS_2_MID               TLM_MSG(  163 )
#define PX4_ACTUATOR_CONTROLS_3_MID               TLM_MSG(  164 )
#define PX4_ACTUATOR_DIRECT_MID                   TLM_MSG(  165 )
#define PX4_ACTUATOR_OUTPUTS_MID                  TLM_MSG(  166 )
#define PX4_ADC_REPORT_MID                        TLM_MSG(  167 )
#define PX4_AIRSPEED_MID                          TLM_MSG(  168 )
#define PX4_ATT_POS_MOCAP_MID                     TLM_MSG(  169 )
#define PX4_BATTERY_STATUS_MID                    TLM_MSG(  170 )
#define PX4_CAMERA_TRIGGER_MID                    TLM_MSG(  171 )
#define PX4_COMMANDER_STATE_MID                   TLM_MSG(  172 )
#define PX4_CONTROL_STATE_MID                     TLM_MSG(  173 )
#define PX4_CPULOAD_MID                           TLM_MSG(  174 )
#define PX4_DEBUG_KEY_VALUE_MID                   TLM_MSG(  175 )
#define PX4_DIFFERENTIAL_PRESSURE_MID             TLM_MSG(  176 )
#define PX4_DISTANCE_SENSOR_MID                   TLM_MSG(  177 )
#define PX4_FW_POS_CTRL_STATUS_MID                TLM_MSG(  178 )
#define PX4_FW_VIRTUAL_ATTITUDE_SETPOINT_MID      TLM_MSG(  179 )
#define PX4_FW_VIRTUAL_RATES_SETPOINT_MID         TLM_MSG(  180 )
#define PX4_EKF2_INNOVATIONS_MID                  TLM_MSG(  181 )
#define PX4_EKF2_REPLAY_MID                       TLM_MSG(  182 )
#define PX4_ESC_REPORT_MID                        TLM_MSG(  183 )
#define PX4_ESC_STATUS_MID                        TLM_MSG(  184 )
#define PX4_ESTIMATOR_STATUS_MID                  TLM_MSG(  185 )
#define PX4_FENCE_MID                             TLM_MSG(  186 )
#define PX4_FENCE_VERTEX_MID                      TLM_MSG(  187 )
#define PX4_FILTERED_BOTTOM_FLOW_MID              TLM_MSG(  188 )
#define PX4_FOLLOW_TARGET_MID                     TLM_MSG(  189 )
#define PX4_GEOFENCE_RESULT_MID                   TLM_MSG(  190 )
#define PX4_GPS_DUMP_MID                          TLM_MSG(  191 )
#define PX4_GPS_INJECT_DATA_MID                   TLM_MSG(  192 )
#define PX4_HIL_SENSOR_MID                        TLM_MSG(  193 )
#define PX4_HOME_POSITION_MID                     TLM_MSG(  194 )
#define PX4_INPUT_RC_MID                          TLM_MSG(  195 )
#define PX4_LED_CONTROL_MID                       TLM_MSG(  196 )
#define PX4_LOG_MESSAGE_MID                       TLM_MSG(  197 )
#define PX4_MANUAL_CONTROL_SETPOINT_MID           TLM_MSG(  198 )
#define PX4_MAVLINK_LOG_MID                       TLM_MSG(  199 )
#define PX4_MC_ATT_CTRL_STATUS_MID                TLM_MSG(  200 )
#define PX4_MC_VIRTUAL_ATTITUDE_SETPOINT_MID      TLM_MSG(  201 )
#define PX4_MC_VIRTUAL_RATES_SETPOINT_MID         TLM_MSG(  202 )
#define PX4_MISSION_MID                           TLM_MSG(  203 )
#define PX4_MISSION_RESULT_MID                    TLM_MSG(  204 )
#define PX4_MULTIROTOR_MOTOR_LIMITS_MID           TLM_MSG(  205 )
#define PX4_OFFBOARD_CONTROL_MODE_MID             TLM_MSG(  206 )
#define PX4_OPTICAL_FLOW_MID                      TLM_MSG(  207 )
#define PX4_OUTPUT_PWM_MID                        TLM_MSG(  208 )
#define PX4_PARAMETER_UPDATE_MID                  TLM_MSG(  209 )
#define PX4_POSITION_SETPOINT_MID                 TLM_MSG(  210 )
#define PX4_POSITION_SETPOINT_TRIPLET_MID         TLM_MSG(  211 )
#define PX4_PWM_INPUT_MID                         TLM_MSG(  212 )
#define PX4_QSHELL_REQ_MID                        TLM_MSG(  213 )
#define PX4_RC_CHANNELS_MID                       TLM_MSG(  214 )
#define PX4_RC_PARAMETER_MAP_MID                  TLM_MSG(  215 )
#define PX4_SAFETY_MID                            TLM_MSG(  216 )
#define PX4_SATELLITE_INFO_MID                    TLM_MSG(  217 )
#define PX4_SENSOR_ACCEL_MID                      TLM_MSG(  218 )
#define PX4_SENSOR_BARO_MID                       TLM_MSG(  219 )
#define PX4_SENSOR_COMBINED_MID                   TLM_MSG(  220 )
#define PX4_SENSOR_GYRO_MID                       TLM_MSG(  221 )
#define PX4_SENSOR_MAG_MID                        TLM_MSG(  222 )
#define PX4_SERVORAIL_STATUS_MID                  TLM_MSG(  223 )
#define PX4_SUBSYSTEM_INFO_MID                    TLM_MSG(  224 )
#define PX4_SYSTEM_POWER_MID                      TLM_MSG(  225 )
#define PX4_TECS_STATUS_MID                       TLM_MSG(  226 )
#define PX4_TELEMETRY_STATUS_MID                  TLM_MSG(  227 )
#define PX4_TEST_MOTOR_MID                        TLM_MSG(  228 )
#define PX4_TIME_OFFSET_MID                       TLM_MSG(  229 )
#define PX4_TRANSPONDER_REPORT_MID                TLM_MSG(  230 )
#define PX4_UAVCAN_PARAMETER_REQUEST_MID          TLM_MSG(  231 )
#define PX4_UAVCAN_PARAMETER_VALUE_MID            TLM_MSG(  232 )
#define PX4_VEHICLE_ATTITUDE_MID                  TLM_MSG(  233 )
#define PX4_VEHICLE_ATTITUDE_SETPOINT_MID         TLM_MSG(  234 )
#define PX4_VEHICLE_COMMAND_ACK_MID               TLM_MSG(  235 )
#define PX4_VEHICLE_COMMAND_MID                   TLM_MSG(  236 )
#define PX4_VEHICLE_CONTROL_MODE_MID              TLM_MSG(  237 )
#define PX4_VEHICLE_FORCE_SETPOINT_MID            TLM_MSG(  238 )
#define PX4_VEHICLE_GLOBAL_POSITION_MID           TLM_MSG(  239 )
#define PX4_VEHICLE_GLOBAL_VELOCITY_SETPOINT_MID  TLM_MSG(  240 )
#define PX4_VEHICLE_GPS_POSITION_MID              TLM_MSG(  241 )
#define PX4_VEHICLE_LAND_DETECTED_MID             TLM_MSG(  242 )
#define PX4_VEHICLE_LOCAL_POSITION_MID            TLM_MSG(  243 )
#define PX4_VEHICLE_LOCAL_POSITION_SETPOINT_MID   TLM_MSG(  244 )
#define PX4_VEHICLE_RATES_SETPOINT_MID            TLM_MSG(  245 )
#define PX4_VEHICLE_STATUS_MID                    TLM_MSG(  246 )
#define PX4_VISION_POSITION_ESTIMATE_MID          TLM_MSG(  247 )
#define PX4_VTOL_VEHICLE_STATUS_MID               TLM_MSG(  248 )
#define PX4_WIND_ESTIMATE_MID                     TLM_MSG(  249 )
#define PX4_SENSOR_CORRECTION_MID                 TLM_MSG(  250 )


/* AK8963                                                    */
#define AK8963_HK_TLM_MID                         TLM_MSG(  275 )
#define AK8963_SEND_HK_MID                        CMD_MSG(  276 )
#define AK8963_WAKEUP_MID                         CMD_MSG(  277 )
#define AK8963_CMD_MID                            CMD_MSG(  278 )
#define AK8963_DIAG_TLM_MID                       TLM_MSG(  279 )


/* AMC                                                       */
#define AMC_CMD_MID                               CMD_MSG(  280 )
#define AMC_SEND_HK_MID                           CMD_MSG(  281 )
#define AMC_UPDATE_MOTORS_MID                     CMD_MSG(  282 )
#define AMC_OUT_DATA_MID                          TLM_MSG(  283 )
#define AMC_HK_TLM_MID                            TLM_MSG(  284 )


/* BAT                                                       */
#define BAT_HK_TLM_MID                            TLM_MSG(  287 )
#define BAT_SEND_HK_MID                           CMD_MSG(  288 )
#define BAT_WAKEUP_MID                            CMD_MSG(  289 )
#define BAT_CMD_MID                               CMD_MSG(  290 )


/* EA                                                        */
#define EA_CMD_MID                                CMD_MSG(  293 )
#define EA_SEND_HK_MID                            CMD_MSG(  294 )
#define EA_HK_TLM_MID                             TLM_MSG(  295 )
#define EA_WAKEUP_MID                             CMD_MSG(  296 )
#define EA_OUT_DATA_MID                           TLM_MSG(  297 )
#define EA_PERFMON_MID                            TLM_MSG(  298 )


/* FLOW                                                      */
#define FLOW_HK_TLM_MID                           TLM_MSG(  301 )
#define FLOW_SEND_HK_MID                          CMD_MSG(  302 )
#define FLOW_WAKEUP_MID                           CMD_MSG(  303 )
#define FLOW_CMD_MID                              CMD_MSG(  304 )
#define FLOW_FRAME_MID                            TLM_MSG(  305 )
#define FLOW_DIAG_TLM_MID                         TLM_MSG(  306 )


/* GPS                                                       */
#define GPS_CMD_MID                               CMD_MSG(  309 )
#define GPS_SEND_HK_MID                           TLM_MSG(  310 )
#define GPS_READ_SENSOR_MID                       CMD_MSG(  311 )
#define GPS_HK_TLM_MID                            TLM_MSG(  312 )
/* Individual GPS messages                                   */
#define GPS_NAV_DOP_MID                           TLM_MSG(  313 )
#define GPS_NAV_NAVPVT_MID                        TLM_MSG(  314 )
#define GPS_NAV_SVINFO_MID                        TLM_MSG(  315 )
#define GPS_ACK_NAK_MID                           TLM_MSG(  316 )
#define GPS_ACK_ACK_MID                           TLM_MSG(  317 )
#define GPS_CFG_PRT_MID                           TLM_MSG(  318 )
#define GPS_CFG_MSG_MID                           TLM_MSG(  319 )
#define GPS_CFG_RATE_MID                          TLM_MSG(  320 )
#define GPS_CFG_SBAS_MID                          TLM_MSG(  321 )
#define GPS_CFG_NAV5_MID                          TLM_MSG(  322 )
#define GPS_MON_HW_MID                            TLM_MSG(  323 )


/* HMC5883                                                   */
#define HMC5883_HK_TLM_MID                        TLM_MSG(  330 )
#define HMC5883_SEND_HK_MID                       CMD_MSG(  331 )
#define HMC5883_WAKEUP_MID                        CMD_MSG(  332 )
#define HMC5883_CMD_MID                           CMD_MSG(  333 )
#define HMC5883_DIAG_TLM_MID                      TLM_MSG(  334 )


/* LD                                                        */
#define LD_HK_TLM_MID                             TLM_MSG(  335 )
#define LD_SEND_HK_MID                            CMD_MSG(  336 )
#define LD_WAKEUP_MID                             CMD_MSG(  337 )
#define LD_CMD_MID                                CMD_MSG(  338 )
#define LD_DIAG_TLM_MID                           TLM_MSG(  339 )


/* LGC                                                       */
#define LGC_HK_TLM_MID                            TLM_MSG(  342 )
#define LGC_SEND_HK_MID                           CMD_MSG(  343 )
#define LGC_WAKEUP_MID                            CMD_MSG(  344 )
#define LGC_CMD_MID                               CMD_MSG(  345 )


/* MAC                                                       */
#define MAC_CMD_MID                               CMD_MSG(  348 )
#define MAC_SEND_HK_MID                           CMD_MSG(  349 )
#define MAC_RUN_CONTROLLER_MID                    CMD_MSG(  350 )
#define MAC_OUT_DATA_MID                          TLM_MSG(  351 )
#define MAC_HK_TLM_MID                            TLM_MSG(  352 )


/* MAVLINK                                                   */
#define MAVLINK_CMD_MID                           CMD_MSG(  355 )
#define MAVLINK_SEND_HK_MID                       CMD_MSG(  356 )
#define MAVLINK_HK_TLM_MID                        TLM_MSG(  357 )
#define MAVLINK_WAKEUP_MID                        TLM_MSG(  358 )
#define MAVLINK_OUT_DATA_MID                      TLM_MSG(  359 )
#define MAVLINK_PARAM_DATA_MID                    TLM_MSG(  360 )
#define MAVLINK_REQUEST_PARAMS_MID                CMD_MSG(  361 )


/* MPC                                                       */
#define MPC_HK_TLM_MID                            TLM_MSG(  364 )
#define MPC_SEND_HK_MID                           CMD_MSG(  365 )
#define MPC_WAKEUP_MID                            CMD_MSG(  366 )
#define MPC_CMD_MID                               CMD_MSG(  367 )
#define MPC_DIAG_TLM_MID                          TLM_MSG(  368 )


/* MPU6050                                                   */
#define MPU6050_SEND_HK_MID                       CMD_MSG(  371 )
#define MPU6050_HK_TLM_MID                        TLM_MSG(  372 )
#define MPU6050_DIAG_TLM_MID                      TLM_MSG(  373 )
#define MPU6050_CMD_MID                           CMD_MSG(  374 )
#define MPU6050_MEASURE_MID                       CMD_MSG(  375 )


/* MPU9250                                                   */
#define MPU9250_SEND_HK_MID                       CMD_MSG(  378 )
#define MPU9250_HK_TLM_MID                        TLM_MSG(  379 )
#define MPU9250_DIAG_TLM_MID                      TLM_MSG(  380 )
#define MPU9250_CMD_MID                           CMD_MSG(  381 )
#define MPU9250_MEASURE_MID                       CMD_MSG(  382 )


/* MS5607                                                    */
#define MS5607_SEND_HK_MID                        CMD_MSG(  385 )
#define MS5607_HK_TLM_MID                         TLM_MSG(  386 )
#define MS5607_DIAG_TLM_MID                       TLM_MSG(  387 )
#define MS5607_CMD_MID                            CMD_MSG(  388 )
#define MS5607_MEASURE_MID                        CMD_MSG(  389 )


/* MS5611                                                    */
#define MS5611_SEND_HK_MID                        CMD_MSG(  392 )
#define MS5611_HK_TLM_MID                         TLM_MSG(  393 )
#define MS5611_DIAG_TLM_MID                       TLM_MSG(  394 )
#define MS5611_CMD_MID                            CMD_MSG(  395 )
#define MS5611_MEASURE_MID                        CMD_MSG(  396 )


/* NAV                                                       */
#define NAV_HK_TLM_MID                            TLM_MSG(  399 )
#define NAV_SEND_HK_MID                           CMD_MSG(  400 )
#define NAV_WAKEUP_MID                            CMD_MSG(  401 )
#define NAV_CMD_MID                               CMD_MSG(  402 )


/* PE                                                        */
#define PE_HK_TLM_MID                             TLM_MSG(  405 )
#define PE_SEND_HK_MID                            CMD_MSG(  406 )
#define PE_WAKEUP_MID                             CMD_MSG(  407 )
#define PE_CMD_MID                                CMD_MSG(  408 )


/* PRM                                                       */
#define PRM_CMD_MID                               CMD_MSG(  411 )
#define PRM_SEND_HK_MID                           CMD_MSG(  412 )
#define PRM_WAKEUP_MID                            CMD_MSG(  413 )
#define PRM_OUT_DATA_MID                          TLM_MSG(  414 )
#define PRM_HK_TLM_MID                            TLM_MSG(  415 )


/* PRMLIB                                                    */
#define PRMLIB_PARAM_UPDATED_MID                  TLM_MSG(  418 )


/* QAE                                                       */
#define QAE_HK_TLM_MID                            TLM_MSG(  421 )
#define QAE_SEND_HK_MID                           CMD_MSG(  422 )
#define QAE_WAKEUP_MID                            CMD_MSG(  423 )
#define QAE_CMD_MID                               CMD_MSG(  424 )


/* RCIN                                                      */
#define RCIN_HK_TLM_MID                           TLM_MSG(  425 )
#define RCIN_SEND_HK_MID                          CMD_MSG(  426 )
#define RCIN_WAKEUP_MID                           CMD_MSG(  427 )
#define RCIN_CMD_MID                              CMD_MSG(  428 )


/* RGBLED                                                    */
#define RGBLED_CMD_MID                            CMD_MSG(  431 )
#define RGBLED_SEND_HK_MID                        CMD_MSG(  432 )
#define RGBLED_WAKEUP_MID                         CMD_MSG(  433 )
#define RGBLED_HK_TLM_MID                         TLM_MSG(  434 )


/* SBN                                                       */
#define SBN_WAKEUP_MID                            CMD_MSG(  437 )
#define SBN_CMD_MID                               CMD_MSG(  438 )
#define SBN_TLM_MID                               TLM_MSG(  439 )
/* These MIDs must match for SBN to SBN communication. */
#define SBN_SUB_MID                               TLM_MSG(  440 ) - CFE_MSG_CPU_BASE
#define SBN_ALLSUB_MID                            TLM_MSG(  441 ) - CFE_MSG_CPU_BASE
#define SBN_UNSUB_MID                             TLM_MSG(  442 ) - CFE_MSG_CPU_BASE
#define SBN_MODULE_HK_MID                         TLM_MSG(  443 )

/* SENS                                                      */
#define SENS_HK_TLM_MID                           TLM_MSG(  450 )
#define SENS_SEND_HK_MID                          CMD_MSG(  451 )
#define SENS_WAKEUP_MID                           CMD_MSG(  452 )
#define SENS_CMD_MID                              CMD_MSG(  453 )


/* SIM                                                       */
#define SIM_HK_TLM_MID                            TLM_MSG(  456 )
#define SIM_SEND_HK_MID                           CMD_MSG(  457 )
#define SIM_WAKEUP_MID                            CMD_MSG(  458 )
#define SIM_CMD_MID                               CMD_MSG(  459 )


/* SONAR                                                     */
#define SONAR_CMD_MID                             CMD_MSG(  462 )
#define SONAR_SEND_HK_MID                         CMD_MSG(  463 )
#define SONAR_MEASURE_MID                         CMD_MSG(  464 )
#define SONAR_OUT_DATA_MID                        TLM_MSG(  465 )
#define SONAR_HK_TLM_MID                          TLM_MSG(  466 )


/* ULR                                                       */
#define ULR_CMD_MID                               CMD_MSG(  469 )
#define ULR_SEND_HK_MID                           CMD_MSG(  470 )
#define ULR_MEASURE_MID                           CMD_MSG(  471 )
#define ULR_OUT_DATA_MID                          TLM_MSG(  472 )
#define ULR_HK_TLM_MID                            TLM_MSG(  473 )


/* VC                                                        */
#define VC_HK_TLM_MID                             TLM_MSG(  476 )
#define VC_SEND_HK_MID                            CMD_MSG(  477 )
#define VC_CMD_MID                                CMD_MSG(  478 )
#define VC_PROCESS_CMDS_MID                       CMD_MSG(  479 )


/* VID                                                       */
#define VID_CMD_MID                               CMD_MSG(  482 )
#define VID_SEND_HK_MID                           CMD_MSG(  483 )
#define VID_HK_TLM_MID                            TLM_MSG(  484 )
#define VID_GRAB_FRAME_MID                        CMD_MSG(  485 )
#define VID_FWD_CAM_FRAME_MID                     TLM_MSG(  486 )


/* VM                                                        */
#define VM_HK_TLM_MID                             TLM_MSG(  487 )
#define VM_SEND_HK_MID                            CMD_MSG(  488 )
#define VM_WAKEUP_MID                             CMD_MSG(  489 )
#define VM_CMD_MID                                CMD_MSG(  490 )
#define VM_CONFIG_TLM_MID                         TLM_MSG(  491 )


#endif
