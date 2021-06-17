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
#define CI_HK_TLM_MID                             TLM_MSG(   50 )
#define CI_OUT_DATA_MID                           TLM_MSG(   51 )
#define CI_CMD_MID                                CMD_MSG(   52 )
#define CI_INGEST_COMMANDS_MID                    CMD_MSG(   53 )
#define CI_SEND_HK_MID                            CMD_MSG(   54 )
#define CI_WAKEUP_MID                             CMD_MSG(   55 )


/* CF                                                          */
#define CF_CONFIG_TLM_MID                         TLM_MSG(   56 )
#define CF_HK_TLM_MID                             TLM_MSG(   57 )
#define CF_INCOMING_PDU_MID                       CMD_MSG(   58 )
#define CF_TRANS_TLM_MID                          TLM_MSG(   59 )
#define CF_CMD_MID                                CMD_MSG(   60 )
#define CF_SEND_HK_MID                            CMD_MSG(   61 )
#define CF_SPACE_TO_GND_PDU_MID                   TLM_MSG(   62 )
#define CF_WAKE_UP_REQ_CMD_MID                    CMD_MSG(   63 )


/* CS                                                          */
#define CS_HK_TLM_MID                             TLM_MSG(   80 )
#define CS_BACKGROUND_CYCLE_MID                   CMD_MSG(   81 )
#define CS_CMD_MID                                CMD_MSG(   82 )
#define CS_SEND_HK_MID                            CMD_MSG(   83 )


/* DS                                                          */
#define DS_DIAG_TLM_MID                           TLM_MSG(   84 )
#define DS_HK_TLM_MID                             TLM_MSG(   85 )
#define DS_CMD_MID                                CMD_MSG(   86 )
#define DS_SEND_HK_MID                            CMD_MSG(   87 )


/* FM                                                          */
#define FM_DIR_LIST_TLM_MID                       TLM_MSG(   88 )
#define FM_FILE_INFO_TLM_MID                      TLM_MSG(   89 )
#define FM_FREE_SPACE_TLM_MID                     TLM_MSG(   90 )
#define FM_HK_TLM_MID                             TLM_MSG(   91 )
#define FM_OPEN_FILES_TLM_MID                     TLM_MSG(   92 )
#define FM_CMD_MID                                CMD_MSG(   93 )
#define FM_SEND_HK_MID                            CMD_MSG(   94 )


/* HK                                                        */
#define HK_COMBINED_PKT1_MID                      TLM_MSG(  100 )
#define HK_COMBINED_PKT2_MID                      TLM_MSG(  101 )
#define HK_COMBINED_PKT3_MID                      TLM_MSG(  102 )
#define HK_COMBINED_PKT4_MID                      TLM_MSG(  103 )
#define HK_COMBINED_PKT5_MID                      TLM_MSG(  104 )
#define HK_COMBINED_PKT6_MID                      TLM_MSG(  105 )
#define HK_COMBINED_PKT7_MID                      TLM_MSG(  106 )
#define HK_COMBINED_PKT8_MID                      TLM_MSG(  107 )
#define HK_COMBINED_PKT9_MID                      TLM_MSG(  108 )
#define HK_COMBINED_PKT10_MID                     TLM_MSG(  109 )
#define HK_HK_TLM_MID                             TLM_MSG(  110 )
#define HK_CMD_MID                                CMD_MSG(  111 )
#define HK_SEND_COMBINED_PKT_MID                  CMD_MSG(  112 )
#define HK_SEND_HK_MID                            CMD_MSG(  113 )


/* HS                                                        */
#define HS_HK_TLM_MID                             TLM_MSG(  120 )
#define HS_CMD_MID                                CMD_MSG(  121 )
#define HS_SEND_HK_MID                            CMD_MSG(  122 )
#define HS_WAKEUP_MID                             CMD_MSG(  123 )


/* LC                                                        */
#define LC_HK_TLM_MID                             TLM_MSG(  130 )
#define LC_CMD_MID                                CMD_MSG(  131 )
#define LC_SAMPLE_AP_MID                          CMD_MSG(  132 )
#define LC_SEND_HK_MID                            CMD_MSG(  133 )


/* MD                                                        */
#define MD_DWELL_PKT_MID_BASE                     TLM_MSG(  134 )
#define MD_HK_TLM_MID                             TLM_MSG(  139 )
#define MD_CMD_MID                                CMD_MSG(  140 )
#define MD_SEND_HK_MID                            CMD_MSG(  141 )
#define MD_WAKEUP_MID                             CMD_MSG(  142 )


/* MM                                                        */
#define MM_HK_TLM_MID                             TLM_MSG(  143 )
#define MM_CMD_MID                                CMD_MSG(  144 )
#define MM_SEND_HK_MID                            CMD_MSG(  145 )


/* SC                                                        */
#define SC_HK_TLM_MID                             TLM_MSG(  146 )
#define SC_1HZ_WAKEUP_MID                         CMD_MSG(  147 )
#define SC_CMD_MID                                CMD_MSG(  148 )
#define SC_SEND_HK_MID                            CMD_MSG(  149 )


/* SCH                                                       */
#define SCH_ACTIVITY_DONE_MID                     TLM_MSG(  150 )
#define SCH_DIAG_TLM_MID                          TLM_MSG(  151 )
#define SCH_HK_TLM_MID                            TLM_MSG(  152 )
#define SCH_CMD_MID                               CMD_MSG(  153 )
#define SCH_SEND_HK_MID                           CMD_MSG(  154 )
#define SCH_UNUSED_MID                                    (0)


/* TO                                                        */
#define TO_DATA_TYPE_MID                          TLM_MSG(  155 )
#define TO_DIAG_MSG_FLOW_MID                      TLM_MSG(  156 )
#define TO_DIAG_TLM_MID                           TLM_MSG(  157 )
#define TO_HK_TLM_MID                             TLM_MSG(  158 )
#define TO_CMD_MID                                CMD_MSG(  159 )
#define TO_SEND_HK_MID                            CMD_MSG(  160 )
#define TO_SEND_TLM_MID                           CMD_MSG(  161 )


/* PX4                                                       */
#define PX4_ACTUATOR_ARMED_MID                    TLM_MSG(  162 )
#define PX4_ACTUATOR_CONTROLS_0_MID               TLM_MSG(  163 )
#define PX4_ACTUATOR_CONTROLS_1_MID               TLM_MSG(  164 )
#define PX4_ACTUATOR_CONTROLS_2_MID               TLM_MSG(  165 )
#define PX4_ACTUATOR_CONTROLS_3_MID               TLM_MSG(  166 )
#define PX4_ACTUATOR_DIRECT_MID                   TLM_MSG(  167 )
#define PX4_ACTUATOR_OUTPUTS_MID                  TLM_MSG(  168 )
#define PX4_ADC_REPORT_MID                        TLM_MSG(  169 )
#define PX4_AIRSPEED_MID                          TLM_MSG(  170 )
#define PX4_ATT_POS_MOCAP_MID                     TLM_MSG(  171 )
#define PX4_BATTERY_STATUS_MID                    TLM_MSG(  172 )
#define PX4_CAMERA_TRIGGER_MID                    TLM_MSG(  173 )
#define PX4_COMMANDER_STATE_MID                   TLM_MSG(  174 )
#define PX4_CONTROL_STATE_MID                     TLM_MSG(  175 )
#define PX4_CPULOAD_MID                           TLM_MSG(  176 )
#define PX4_DEBUG_KEY_VALUE_MID                   TLM_MSG(  177 )
#define PX4_DIFFERENTIAL_PRESSURE_MID             TLM_MSG(  178 )
#define PX4_DISTANCE_SENSOR_MID                   TLM_MSG(  179 )
#define PX4_FW_POS_CTRL_STATUS_MID                TLM_MSG(  180 )
#define PX4_FW_VIRTUAL_ATTITUDE_SETPOINT_MID      TLM_MSG(  181 )
#define PX4_FW_VIRTUAL_RATES_SETPOINT_MID         TLM_MSG(  182 )
#define PX4_EKF2_INNOVATIONS_MID                  TLM_MSG(  183 )
#define PX4_EKF2_REPLAY_MID                       TLM_MSG(  184 )
#define PX4_ESC_REPORT_MID                        TLM_MSG(  185 )
#define PX4_ESC_STATUS_MID                        TLM_MSG(  186 )
#define PX4_ESTIMATOR_STATUS_MID                  TLM_MSG(  187 )
#define PX4_FENCE_MID                             TLM_MSG(  188 )
#define PX4_FENCE_VERTEX_MID                      TLM_MSG(  189 )
#define PX4_FILTERED_BOTTOM_FLOW_MID              TLM_MSG(  190 )
#define PX4_FOLLOW_TARGET_MID                     TLM_MSG(  191 )
#define PX4_GEOFENCE_RESULT_MID                   TLM_MSG(  192 )
#define PX4_GPS_DUMP_MID                          TLM_MSG(  193 )
#define PX4_GPS_INJECT_DATA_MID                   TLM_MSG(  194 )
#define PX4_HIL_SENSOR_MID                        TLM_MSG(  195 )
#define PX4_HOME_POSITION_MID                     TLM_MSG(  196 )
#define PX4_INPUT_RC_MID                          TLM_MSG(  197 )
#define PX4_LED_CONTROL_MID                       TLM_MSG(  198 )
#define PX4_LOG_MESSAGE_MID                       TLM_MSG(  199 )
#define PX4_MANUAL_CONTROL_SETPOINT_MID           TLM_MSG(  200 )
#define PX4_MAVLINK_LOG_MID                       TLM_MSG(  201 )
#define PX4_MC_ATT_CTRL_STATUS_MID                TLM_MSG(  202 )
#define PX4_MC_VIRTUAL_ATTITUDE_SETPOINT_MID      TLM_MSG(  203 )
#define PX4_MC_VIRTUAL_RATES_SETPOINT_MID         TLM_MSG(  204 )
#define PX4_MISSION_MID                           TLM_MSG(  205 )
#define PX4_MISSION_RESULT_MID                    TLM_MSG(  206 )
#define PX4_MULTIROTOR_MOTOR_LIMITS_MID           TLM_MSG(  207 )
#define PX4_OFFBOARD_CONTROL_MODE_MID             TLM_MSG(  208 )
#define PX4_OPTICAL_FLOW_MID                      TLM_MSG(  209 )
#define PX4_OUTPUT_PWM_MID                        TLM_MSG(  210 )
#define PX4_PARAMETER_UPDATE_MID                  TLM_MSG(  211 )
#define PX4_POSITION_SETPOINT_MID                 TLM_MSG(  212 )
#define PX4_POSITION_SETPOINT_TRIPLET_MID         TLM_MSG(  213 )
#define PX4_PWM_INPUT_MID                         TLM_MSG(  214 )
#define PX4_QSHELL_REQ_MID                        TLM_MSG(  215 )
#define PX4_RC_CHANNELS_MID                       TLM_MSG(  216 )
#define PX4_RC_PARAMETER_MAP_MID                  TLM_MSG(  217 )
#define PX4_SAFETY_MID                            TLM_MSG(  218 )
#define PX4_SATELLITE_INFO_MID                    TLM_MSG(  219 )
#define PX4_SENSOR_ACCEL_MID                      TLM_MSG(  220 )
#define PX4_SENSOR_BARO_MID                       TLM_MSG(  221 )
#define PX4_SENSOR_COMBINED_MID                   TLM_MSG(  222 )
#define PX4_SENSOR_GYRO_MID                       TLM_MSG(  223 )
#define PX4_SENSOR_MAG_MID                        TLM_MSG(  224 )
#define PX4_SERVORAIL_STATUS_MID                  TLM_MSG(  225 )
#define PX4_SUBSYSTEM_INFO_MID                    TLM_MSG(  226 )
#define PX4_SYSTEM_POWER_MID                      TLM_MSG(  227 )
#define PX4_TECS_STATUS_MID                       TLM_MSG(  228 )
#define PX4_TELEMETRY_STATUS_MID                  TLM_MSG(  229 )
#define PX4_TEST_MOTOR_MID                        TLM_MSG(  230 )
#define PX4_TIME_OFFSET_MID                       TLM_MSG(  231 )
#define PX4_TRANSPONDER_REPORT_MID                TLM_MSG(  232 )
#define PX4_UAVCAN_PARAMETER_REQUEST_MID          TLM_MSG(  233 )
#define PX4_UAVCAN_PARAMETER_VALUE_MID            TLM_MSG(  234 )
#define PX4_VEHICLE_ATTITUDE_MID                  TLM_MSG(  235 )
#define PX4_VEHICLE_ATTITUDE_SETPOINT_MID         TLM_MSG(  236 )
#define PX4_VEHICLE_COMMAND_ACK_MID               TLM_MSG(  237 )
#define PX4_VEHICLE_COMMAND_MID                   TLM_MSG(  238 )
#define PX4_VEHICLE_CONTROL_MODE_MID              TLM_MSG(  239 )
#define PX4_VEHICLE_FORCE_SETPOINT_MID            TLM_MSG(  240 )
#define PX4_VEHICLE_GLOBAL_POSITION_MID           TLM_MSG(  241 )
#define PX4_VEHICLE_GLOBAL_VELOCITY_SETPOINT_MID  TLM_MSG(  242 )
#define PX4_VEHICLE_GPS_POSITION_MID              TLM_MSG(  243 )
#define PX4_VEHICLE_LAND_DETECTED_MID             TLM_MSG(  244 )
#define PX4_VEHICLE_LOCAL_POSITION_MID            TLM_MSG(  245 )
#define PX4_VEHICLE_LOCAL_POSITION_SETPOINT_MID   TLM_MSG(  246 )
#define PX4_VEHICLE_RATES_SETPOINT_MID            TLM_MSG(  247 )
#define PX4_VEHICLE_STATUS_MID                    TLM_MSG(  248 )
#define PX4_VISION_POSITION_ESTIMATE_MID          TLM_MSG(  249 )
#define PX4_VTOL_VEHICLE_STATUS_MID               TLM_MSG(  250 )
#define PX4_WIND_ESTIMATE_MID                     TLM_MSG(  251 )
#define PX4_SENSOR_CORRECTION_MID                 TLM_MSG(  252 )


/* AK8963                                                    */
#define AK8963_DIAG_TLM_MID                       TLM_MSG(  275 )
#define AK8963_HK_TLM_MID                         TLM_MSG(  276 )
#define AK8963_CMD_MID                            CMD_MSG(  277 )
#define AK8963_SEND_HK_MID                        CMD_MSG(  278 )
#define AK8963_WAKEUP_MID                         CMD_MSG(  279 )


/* AMC                                                       */
#define AMC_HK_TLM_MID                            TLM_MSG(  280 )
#define AMC_OUT_DATA_MID                          TLM_MSG(  281 )
#define AMC_CMD_MID                               CMD_MSG(  282 )
#define AMC_SEND_HK_MID                           CMD_MSG(  283 )
#define AMC_UPDATE_MOTORS_MID                     CMD_MSG(  284 )


/* BAT                                                       */
#define BAT_HK_TLM_MID                            TLM_MSG(  287 )
#define BAT_CMD_MID                               CMD_MSG(  288 )
#define BAT_SEND_HK_MID                           CMD_MSG(  289 )
#define BAT_WAKEUP_MID                            CMD_MSG(  290 )


/* EA                                                        */
#define EA_HK_TLM_MID                             TLM_MSG(  293 )
#define EA_CMD_MID                                CMD_MSG(  294 )
#define EA_SEND_HK_MID                            CMD_MSG(  295 )
#define EA_WAKEUP_MID                             CMD_MSG(  296 )


/* GPS                                                       */
#define GPS_ACK_ACK_MID                           TLM_MSG(  300 )
#define GPS_ACK_NAK_MID                           TLM_MSG(  301 )
#define GPS_CFG_MSG_MID                           TLM_MSG(  302 )
#define GPS_CFG_NAV5_MID                          TLM_MSG(  303 )
#define GPS_CFG_PRT_MID                           TLM_MSG(  304 )
#define GPS_CFG_RATE_MID                          TLM_MSG(  305 )
#define GPS_CFG_SBAS_MID                          TLM_MSG(  306 )
#define GPS_HK_TLM_MID                            TLM_MSG(  307 )
#define GPS_MON_HW_MID                            TLM_MSG(  308 )
#define GPS_NAV_DOP_MID                           TLM_MSG(  309 )
#define GPS_NAV_NAVPVT_MID                        TLM_MSG(  310 )
#define GPS_NAV_SVINFO_MID                        TLM_MSG(  311 )
#define GPS_CMD_MID                               CMD_MSG(  312 )
#define GPS_SEND_HK_MID                           CMD_MSG(  313 )
#define GPS_READ_SENSOR_MID                       CMD_MSG(  314 )
/* Individual GPS messages                                   */


/* HMC5883                                                   */
#define HMC5883_DIAG_TLM_MID                      TLM_MSG(  330 )
#define HMC5883_HK_TLM_MID                        TLM_MSG(  331 )
#define HMC5883_CMD_MID                           CMD_MSG(  332 )
#define HMC5883_SEND_HK_MID                       CMD_MSG(  333 )
#define HMC5883_WAKEUP_MID                        CMD_MSG(  334 )


/* LD                                                        */
#define LD_DIAG_TLM_MID                           TLM_MSG(  335 )
#define LD_HK_TLM_MID                             TLM_MSG(  336 )
#define LD_CMD_MID                                CMD_MSG(  337 )
#define LD_SEND_HK_MID                            CMD_MSG(  338 )
#define LD_WAKEUP_MID                             CMD_MSG(  339 )


/* LGC                                                       */
#define LGC_HK_TLM_MID                            TLM_MSG(  342 )
#define LGC_CMD_MID                               CMD_MSG(  343 )
#define LGC_SEND_HK_MID                           CMD_MSG(  344 )
#define LGC_WAKEUP_MID                            CMD_MSG(  345 )


/* MAC                                                       */
#define MAC_HK_TLM_MID                            TLM_MSG(  348 )
#define MAC_CMD_MID                               CMD_MSG(  349 )
#define MAC_RUN_CONTROLLER_MID                    CMD_MSG(  350 )
#define MAC_SEND_HK_MID                           CMD_MSG(  351 )


/* MPC                                                       */
#define MPC_DIAG_TLM_MID                          TLM_MSG(  355 )
#define MPC_HK_TLM_MID                            TLM_MSG(  356 )
#define MPC_CMD_MID                               CMD_MSG(  357 )
#define MPC_SEND_HK_MID                           CMD_MSG(  358 )
#define MPC_WAKEUP_MID                            CMD_MSG(  359 )


/* MPU6050                                                   */
#define MPU6050_DIAG_TLM_MID                      TLM_MSG(  362 )
#define MPU6050_HK_TLM_MID                        TLM_MSG(  363 )
#define MPU6050_CMD_MID                           CMD_MSG(  364 )
#define MPU6050_MEASURE_MID                       CMD_MSG(  365 )
#define MPU6050_SEND_HK_MID                       CMD_MSG(  366 )


/* MPU9250                                                   */
#define MPU9250_DIAG_TLM_MID                      TLM_MSG(  369 )
#define MPU9250_HK_TLM_MID                        TLM_MSG(  370 )
#define MPU9250_CMD_MID                           CMD_MSG(  371 )
#define MPU9250_MEASURE_MID                       CMD_MSG(  372 )
#define MPU9250_SEND_HK_MID                       CMD_MSG(  373 )


/* MS5607                                                    */
#define MS5607_DIAG_TLM_MID                       TLM_MSG(  376 )
#define MS5607_HK_TLM_MID                         TLM_MSG(  377 )
#define MS5607_CMD_MID                            CMD_MSG(  378 )
#define MS5607_MEASURE_MID                        CMD_MSG(  379 )
#define MS5607_SEND_HK_MID                        CMD_MSG(  380 )


/* MS5611                                                    */
#define MS5611_DIAG_TLM_MID                       TLM_MSG(  383 )
#define MS5611_HK_TLM_MID                         TLM_MSG(  384 )
#define MS5611_CMD_MID                            CMD_MSG(  385 )
#define MS5611_MEASURE_MID                        CMD_MSG(  386 )
#define MS5611_SEND_HK_MID                        CMD_MSG(  387 )


/* NAV                                                       */
#define NAV_HK_TLM_MID                            TLM_MSG(  390 )
#define NAV_CMD_MID                               CMD_MSG(  391 )
#define NAV_SEND_HK_MID                           CMD_MSG(  392 )
#define NAV_WAKEUP_MID                            CMD_MSG(  393 )


/* PE                                                        */
#define PE_HK_TLM_MID                             TLM_MSG(  396 )
#define PE_CMD_MID                                CMD_MSG(  397 )
#define PE_SEND_HK_MID                            CMD_MSG(  398 )
#define PE_WAKEUP_MID                             CMD_MSG(  399 )


/* PRM                                                       */
#define PRM_HK_TLM_MID                            TLM_MSG(  402 )
#define PRM_OUT_DATA_MID                          TLM_MSG(  403 )
#define PRM_CMD_MID                               CMD_MSG(  404 )
#define PRM_SEND_HK_MID                           CMD_MSG(  405 )
#define PRM_WAKEUP_MID                            CMD_MSG(  406 )


/* PRMLIB                                                    */
#define PRMLIB_PARAM_UPDATED_MID                  TLM_MSG(  409 )


/* QAE                                                       */
#define QAE_HK_TLM_MID                            TLM_MSG(  412 )
#define QAE_CMD_MID                               CMD_MSG(  413 )
#define QAE_SEND_HK_MID                           CMD_MSG(  414 )
#define QAE_WAKEUP_MID                            CMD_MSG(  415 )


/* RCIN                                                      */
#define RCIN_HK_TLM_MID                           TLM_MSG(  418 )
#define RCIN_CMD_MID                              CMD_MSG(  419 )
#define RCIN_SEND_HK_MID                          CMD_MSG(  420 )
#define RCIN_WAKEUP_MID                           CMD_MSG(  421 )


/* RGBLED                                                    */
#define RGBLED_HK_TLM_MID                         TLM_MSG(  424 )
#define RGBLED_CMD_MID                            CMD_MSG(  425 )
#define RGBLED_SEND_HK_MID                        CMD_MSG(  426 )
#define RGBLED_WAKEUP_MID                         CMD_MSG(  427 )


/* SBN                                                       */
#define SBN_TLM_MID                               TLM_MSG(  430 )
#define SBN_CMD_MID                               CMD_MSG(  431 )
#define SBN_WAKEUP_MID                            CMD_MSG(  432 )
/* These MIDs must match for SBN to SBN communication. */
#define SBN_SUB_MID                               TLM_MSG(  433 ) - CFE_MSG_CPU_BASE
#define SBN_ALLSUB_MID                            TLM_MSG(  434 ) - CFE_MSG_CPU_BASE
#define SBN_UNSUB_MID                             TLM_MSG(  435 ) - CFE_MSG_CPU_BASE

#define SBN_MODULE_HK_TLM_MID                     TLM_MSG(  436 )


/* SENS                                                      */
#define SENS_HK_TLM_MID                           TLM_MSG(  440 )
#define SENS_CMD_MID                              CMD_MSG(  441 )
#define SENS_SEND_HK_MID                          CMD_MSG(  442 )
#define SENS_WAKEUP_MID                           CMD_MSG(  443 )


/* SIM                                                       */
#define SIM_HK_TLM_MID                            TLM_MSG(  446 )
#define SIM_CMD_MID                               CMD_MSG(  447 )
#define SIM_SEND_HK_MID                           CMD_MSG(  448 )
#define SIM_WAKEUP_MID                            CMD_MSG(  449 )


/* SONAR                                                     */
#define SONAR_HK_TLM_MID                          TLM_MSG(  452 )
#define SONAR_CMD_MID                             CMD_MSG(  453 )
#define SONAR_MEASURE_MID                         CMD_MSG(  454 )
#define SONAR_SEND_HK_MID                         CMD_MSG(  455 )


/* ULR                                                       */
#define ULR_HK_TLM_MID                            TLM_MSG(  458 )
#define ULR_CMD_MID                               CMD_MSG(  459 )
#define ULR_MEASURE_MID                           CMD_MSG(  460 )
#define ULR_SEND_HK_MID                           CMD_MSG(  461 )


/* VC                                                        */
#define VC_HK_TLM_MID                             TLM_MSG(  464 )
#define FLOW_FRAME_MID                            TLM_MSG(  465 )
#define VC_CMD_MID                                CMD_MSG(  466 )
#define VC_PROCESS_CMDS_MID                       CMD_MSG(  467 )
#define VC_SEND_HK_MID                            CMD_MSG(  468 )


/* VM                                                        */
#define VM_CONFIG_TLM_MID                         TLM_MSG(  480 )
#define VM_HK_TLM_MID                             TLM_MSG(  481 )
#define VM_CMD_MID                                CMD_MSG(  482 )
#define VM_SEND_HK_MID                            CMD_MSG(  483 )
#define VM_WAKEUP_MID                             CMD_MSG(  484 )

#define CI_PROCESS_TIMEOUTS_MID                   CMD_MSG(  485 )


/* ZUSUP                                                     */
#define ZUSUP_CMD_MID                             CMD_MSG(  486 )
#define ZUSUP_SEND_HK_MID                         CMD_MSG(  487 )
#define ZUSUP_WAKEUP_MID                          CMD_MSG(  488 )
#define ZUSUP_HK_TLM_MID                          TLM_MSG(  489 )

#endif
