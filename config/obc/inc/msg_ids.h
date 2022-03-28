#ifndef MSG_IDS_H
#define MSG_IDS_H

#include "msg_ids_common.h"

#define FROM_PPD(X)     (PPD_CPU_BASE + X)
#define FROM_CPD(X)     (CPD_CPU_BASE + X)
#define FROM_SIMLINK(X) (SIMLINK_CPU_BASE + X)


/* General Airliner flight applications                                     */

/* AMC                                                                      */
#define AMC_HK_TLM_MID                            TLM_MSG(  128 )  /* 0x080 */
#define AMC_OUT_DATA_MID                          TLM_MSG(  129 )  /* 0x081 */
#define AMC_CMD_MID                               CMD_MSG(  130 )  /* 0x082 */
#define AMC_SEND_HK_MID                           CMD_MSG(  131 )  /* 0x083 */
#define AMC_UPDATE_MOTORS_MID                     CMD_MSG(  132 )  /* 0x084 */


/* LD                                                                       */
#define LD_DIAG_TLM_MID                           TLM_MSG(  133 )  /* 0x085 */
#define LD_HK_TLM_MID                             TLM_MSG(  134 )  /* 0x086 */
#define LD_CMD_MID                                CMD_MSG(  135 )  /* 0x087 */
#define LD_SEND_HK_MID                            CMD_MSG(  136 )  /* 0x088 */
#define LD_WAKEUP_MID                             CMD_MSG(  137 )  /* 0x089 */


/* LGC                                                                      */
#define LGC_HK_TLM_MID                            TLM_MSG(  138 )  /* 0x08a */
#define LGC_CMD_MID                               CMD_MSG(  139 )  /* 0x08b */
#define LGC_SEND_HK_MID                           CMD_MSG(  140 )  /* 0x08c */
#define LGC_WAKEUP_MID                            CMD_MSG(  141 )  /* 0x08d */


/* MAC                                                                      */
#define MAC_HK_TLM_MID                            TLM_MSG(  142 )  /* 0x08e */
#define MAC_CMD_MID                               CMD_MSG(  143 )  /* 0x08f */
#define MAC_RUN_CONTROLLER_MID                    CMD_MSG(  144 )  /* 0x090 */
#define MAC_SEND_HK_MID                           CMD_MSG(  145 )  /* 0x091 */


/* MPC                                                                      */
#define MPC_DIAG_TLM_MID                          TLM_MSG(  146 )  /* 0x092 */
#define MPC_HK_TLM_MID                            TLM_MSG(  147 )  /* 0x093 */
#define MPC_CMD_MID                               CMD_MSG(  148 )  /* 0x094 */
#define MPC_SEND_HK_MID                           CMD_MSG(  149 )  /* 0x095 */
#define MPC_WAKEUP_MID                            CMD_MSG(  150 )  /* 0x096 */


/* NAV                                                                      */
#define NAV_HK_TLM_MID                            TLM_MSG(  151 )  /* 0x097 */
#define NAV_CMD_MID                               CMD_MSG(  152 )  /* 0x098 */
#define NAV_SEND_HK_MID                           CMD_MSG(  153 )  /* 0x099 */
#define NAV_WAKEUP_MID                            CMD_MSG(  154 )  /* 0x09a */


/* PE                                                                       */
#define PE_HK_TLM_MID                             TLM_MSG(  155 )  /* 0x09b */
#define PE_CMD_MID                                CMD_MSG(  156 )  /* 0x09c */
#define PE_SEND_HK_MID                            CMD_MSG(  157 )  /* 0x09d */
#define PE_WAKEUP_MID                             CMD_MSG(  158 )  /* 0x09e */
#define PE_DIAG_TLM_MID                           TLM_MSG(  159 )  /* 0x09f */


/* QAE                                                                      */
#define QAE_HK_TLM_MID                            TLM_MSG(  160 )  /* 0x0a0 */
#define QAE_CMD_MID                               CMD_MSG(  161 )  /* 0x0a1 */
#define QAE_SEND_HK_MID                           CMD_MSG(  162 )  /* 0x0a2 */
#define QAE_WAKEUP_MID                            CMD_MSG(  163 )  /* 0x0a3 */


/* SBND                                                                     */
#define SBND_HK_TLM_MID                           TLM_MSG(  164 )  /* 0x0a4 */
#define SBND_CMD_MID                              CMD_MSG(  165 )  /* 0x0a5 */
#define SBND_SEND_HK_MID                          CMD_MSG(  166 )  /* 0x0a6 */
#define SBND_WAKEUP_MID                           CMD_MSG(  167 )  /* 0x0a7 */


/* SENS                                                                     */
#define SENS_HK_TLM_MID                           TLM_MSG(  168 )  /* 0x0a8 */
#define SENS_CMD_MID                              CMD_MSG(  169 )  /* 0x0a9 */
#define SENS_SEND_HK_MID                          CMD_MSG(  170 )  /* 0x0aa */
#define SENS_WAKEUP_MID                           CMD_MSG(  171 )  /* 0x0ab */


/* VM                                                                       */
#define VM_CONFIG_TLM_MID                         TLM_MSG(  172 )  /* 0x0ac */
#define VM_HK_TLM_MID                             TLM_MSG(  173 )  /* 0x0ad */
#define VM_CMD_MID                                CMD_MSG(  174 )  /* 0x0ae */
#define VM_SEND_HK_MID                            CMD_MSG(  175 )  /* 0x0af */
#define VM_WAKEUP_MID                             CMD_MSG(  176 )  /* 0x0b0 */


/* PX4 Messages                                                             */
#define PX4_ACTUATOR_ARMED_MID                    TLM_MSG(  177 )  /* 0x0b1 */
#define PX4_ACTUATOR_CONTROLS_0_MID               TLM_MSG(  178 )  /* 0x0b2 */
#define PX4_ACTUATOR_CONTROLS_1_MID               TLM_MSG(  179 )  /* 0x0b3 */
#define PX4_ACTUATOR_CONTROLS_2_MID               TLM_MSG(  180 )  /* 0x0b4 */
#define PX4_ACTUATOR_CONTROLS_3_MID               TLM_MSG(  181 )  /* 0x0b5 */
#define PX4_ACTUATOR_DIRECT_MID                   TLM_MSG(  182 )  /* 0x0b6 */
#define PX4_ACTUATOR_OUTPUTS_MID                  TLM_MSG(  183 )  /* 0x0b7 */
#define PX4_ADC_REPORT_MID                        TLM_MSG(  184 )  /* 0x0b8 */
#define PX4_AIRSPEED_MID                          TLM_MSG(  185 )  /* 0x0b9 */
#define PX4_ATT_POS_MOCAP_MID                     TLM_MSG(  186 )  /* 0x0ba */
#define PX4_BATTERY_STATUS_MID                    TLM_MSG(  187 )  /* 0x0bb */
#define PX4_CAMERA_TRIGGER_MID                    TLM_MSG(  188 )  /* 0x0bc */
#define PX4_COMMANDER_STATE_MID                   TLM_MSG(  189 )  /* 0x0bd */
#define PX4_CONTROL_STATE_MID                     TLM_MSG(  190 )  /* 0x0be */
#define PX4_CPULOAD_MID                           TLM_MSG(  191 )  /* 0x0bf */
#define PX4_DEBUG_KEY_VALUE_MID                   TLM_MSG(  192 )  /* 0x0c0 */
#define PX4_DIFFERENTIAL_PRESSURE_MID             TLM_MSG(  193 )  /* 0x0c1 */
#define PX4_DISTANCE_SENSOR_MID                   TLM_MSG(  194 )  /* 0x0c2 */
#define PX4_FW_POS_CTRL_STATUS_MID                TLM_MSG(  195 )  /* 0x0c3 */
#define PX4_FW_VIRTUAL_ATTITUDE_SETPOINT_MID      TLM_MSG(  196 )  /* 0x0c4 */
#define PX4_FW_VIRTUAL_RATES_SETPOINT_MID         TLM_MSG(  197 )  /* 0x0c5 */
#define PX4_EKF2_INNOVATIONS_MID                  TLM_MSG(  198 )  /* 0x0c6 */
#define PX4_EKF2_REPLAY_MID                       TLM_MSG(  199 )  /* 0x0c7 */
#define PX4_ESC_REPORT_MID                        TLM_MSG(  200 )  /* 0x0c8 */
#define PX4_ESC_STATUS_MID                        TLM_MSG(  201 )  /* 0x0c9 */
#define PX4_ESTIMATOR_STATUS_MID                  TLM_MSG(  202 )  /* 0x0ca */
#define PX4_FENCE_MID                             TLM_MSG(  203 )  /* 0x0cb */
#define PX4_FENCE_VERTEX_MID                      TLM_MSG(  204 )  /* 0x0cc */
#define PX4_FILTERED_BOTTOM_FLOW_MID              TLM_MSG(  205 )  /* 0x0cd */
#define PX4_FOLLOW_TARGET_MID                     TLM_MSG(  206 )  /* 0x0ce */
#define PX4_GEOFENCE_RESULT_MID                   TLM_MSG(  207 )  /* 0x0cf */
#define PX4_GPS_DUMP_MID                          TLM_MSG(  208 )  /* 0x0d0 */
#define PX4_GPS_INJECT_DATA_MID                   TLM_MSG(  209 )  /* 0x0d1 */
#define PX4_HIL_SENSOR_MID                        TLM_MSG(  210 )  /* 0x0d2 */
#define PX4_HOME_POSITION_MID                     TLM_MSG(  211 )  /* 0x0d3 */
#define PX4_INPUT_RC_MID                          TLM_MSG(  212 )  /* 0x0d4 */
#define PX4_LED_CONTROL_MID                       TLM_MSG(  213 )  /* 0x0d5 */
#define PX4_LOG_MESSAGE_MID                       TLM_MSG(  214 )  /* 0x0d6 */
#define PX4_MANUAL_CONTROL_SETPOINT_MID           TLM_MSG(  215 )  /* 0x0d7 */
#define PX4_MAVLINK_LOG_MID                       TLM_MSG(  216 )  /* 0x0d8 */
#define PX4_MC_ATT_CTRL_STATUS_MID                TLM_MSG(  217 )  /* 0x0d9 */
#define PX4_MC_VIRTUAL_ATTITUDE_SETPOINT_MID      TLM_MSG(  218 )  /* 0x0da */
#define PX4_MC_VIRTUAL_RATES_SETPOINT_MID         TLM_MSG(  219 )  /* 0x0db */
#define PX4_MISSION_MID                           TLM_MSG(  220 )  /* 0x0dc */
#define PX4_MISSION_RESULT_MID                    TLM_MSG(  221 )  /* 0x0dd */
#define PX4_MULTIROTOR_MOTOR_LIMITS_MID           TLM_MSG(  222 )  /* 0x0de */
#define PX4_OFFBOARD_CONTROL_MODE_MID             TLM_MSG(  223 )  /* 0x0df */
#define PX4_OPTICAL_FLOW_MID                      TLM_MSG(  224 )  /* 0x0e0 */
#define PX4_OUTPUT_PWM_MID                        TLM_MSG(  225 )  /* 0x0e1 */
#define PX4_PARAMETER_UPDATE_MID                  TLM_MSG(  226 )  /* 0x0e2 */
#define PX4_POSITION_SETPOINT_MID                 TLM_MSG(  227 )  /* 0x0e3 */
#define PX4_POSITION_SETPOINT_TRIPLET_MID         TLM_MSG(  228 )  /* 0x0e4 */
#define PX4_PWM_INPUT_MID                         TLM_MSG(  229 )  /* 0x0e5 */
#define PX4_QSHELL_REQ_MID                        TLM_MSG(  230 )  /* 0x0e6 */
#define PX4_RC_CHANNELS_MID                       TLM_MSG(  231 )  /* 0x0e7 */
#define PX4_RC_PARAMETER_MAP_MID                  TLM_MSG(  232 )  /* 0x0e8 */
#define PX4_SAFETY_MID                            TLM_MSG(  233 )  /* 0x0e9 */
#define PX4_SATELLITE_INFO_MID                    TLM_MSG(  234 )  /* 0x0ea */
#define PX4_SENSOR_ACCEL_MID                      TLM_MSG(  235 )  /* 0x0eb */
#define PX4_SENSOR_BARO_MID                       TLM_MSG(  236 )  /* 0x0ec */
#define PX4_SENSOR_COMBINED_MID                   TLM_MSG(  237 )  /* 0x0ed */
#define PX4_SENSOR_GYRO_MID                       TLM_MSG(  238 )  /* 0x0ee */
#define PX4_SENSOR_MAG_MID                        TLM_MSG(  239 )  /* 0x0ef */
#define PX4_SERVORAIL_STATUS_MID                  TLM_MSG(  240 )  /* 0x0f0 */
#define PX4_SUBSYSTEM_INFO_MID                    TLM_MSG(  241 )  /* 0x0f1 */
#define PX4_SYSTEM_POWER_MID                      TLM_MSG(  242 )  /* 0x0f2 */
#define PX4_TECS_STATUS_MID                       TLM_MSG(  243 )  /* 0x0f3 */
#define PX4_TELEMETRY_STATUS_MID                  TLM_MSG(  244 )  /* 0x0f4 */
#define PX4_TEST_MOTOR_MID                        TLM_MSG(  245 )  /* 0x0f5 */
#define PX4_TIME_OFFSET_MID                       TLM_MSG(  246 )  /* 0x0f6 */
#define PX4_TRANSPONDER_REPORT_MID                TLM_MSG(  247 )  /* 0x0f7 */
#define PX4_UAVCAN_PARAMETER_REQUEST_MID          TLM_MSG(  248 )  /* 0x0f8 */
#define PX4_UAVCAN_PARAMETER_VALUE_MID            TLM_MSG(  249 )  /* 0x0f9 */
#define PX4_VEHICLE_ATTITUDE_MID                  TLM_MSG(  250 )  /* 0x0fa */
#define PX4_VEHICLE_ATTITUDE_SETPOINT_MID         TLM_MSG(  251 )  /* 0x0fb */
#define PX4_VEHICLE_COMMAND_ACK_MID               TLM_MSG(  252 )  /* 0x0fc */
#define PX4_VEHICLE_COMMAND_MID                   TLM_MSG(  253 )  /* 0x0fd */
#define PX4_VEHICLE_CONTROL_MODE_MID              TLM_MSG(  254 )  /* 0x0fe */
#define PX4_VEHICLE_FORCE_SETPOINT_MID            TLM_MSG(  255 )  /* 0x0ff */
#define PX4_VEHICLE_GLOBAL_POSITION_MID           TLM_MSG(  256 )  /* 0x100 */
#define PX4_VEHICLE_GLOBAL_VELOCITY_SETPOINT_MID  TLM_MSG(  257 )  /* 0x101 */
#define PX4_VEHICLE_GPS_POSITION_MID              TLM_MSG(  258 )  /* 0x102 */
#define PX4_VEHICLE_LAND_DETECTED_MID             TLM_MSG(  259 )  /* 0x103 */
#define PX4_VEHICLE_LOCAL_POSITION_MID            TLM_MSG(  260 )  /* 0x104 */
#define PX4_VEHICLE_LOCAL_POSITION_SETPOINT_MID   TLM_MSG(  261 )  /* 0x105 */
#define PX4_VEHICLE_RATES_SETPOINT_MID            TLM_MSG(  262 )  /* 0x106 */
#define PX4_VEHICLE_STATUS_MID                    TLM_MSG(  263 )  /* 0x107 */
#define PX4_VISION_POSITION_ESTIMATE_MID          TLM_MSG(  264 )  /* 0x108 */
#define PX4_VTOL_VEHICLE_STATUS_MID               TLM_MSG(  265 )  /* 0x109 */
#define PX4_WIND_ESTIMATE_MID                     TLM_MSG(  266 )  /* 0x10a */
#define PX4_SENSOR_CORRECTION_MID                 TLM_MSG(  267 )  /* 0x10b */


/* Vehicle specific driver applications                                     */

/* BAT                                                                      */
#define BAT_HK_TLM_MID                            TLM_MSG(  268 )  /* 0x10c */
#define BAT_CMD_MID                               CMD_MSG(  269 )  /* 0x10d */
#define BAT_SEND_HK_MID                           CMD_MSG(  270 )  /* 0x10e */
#define BAT_WAKEUP_MID                            CMD_MSG(  271 )  /* 0x10f */


/* GPS                                                                      */
#define GPS_HK_TLM_MID                            TLM_MSG(  272 )  /* 0x110 */
#define GPS_CMD_MID                               CMD_MSG(  273 )  /* 0x111 */
#define GPS_READ_SENSOR_MID                       CMD_MSG(  274 )  /* 0x112 */
#define GPS_SEND_HK_MID                           CMD_MSG(  275 )  /* 0x113 */


/* HMC5883                                                                  */
#define HMC5883_DIAG_TLM_MID                      TLM_MSG(  276 )  /* 0x114 */
#define HMC5883_HK_TLM_MID                        TLM_MSG(  277 )  /* 0x115 */
#define HMC5883_CMD_MID                           CMD_MSG(  278 )  /* 0x116 */
#define HMC5883_SEND_HK_MID                       CMD_MSG(  279 )  /* 0x117 */
#define HMC5883_WAKEUP_MID                        CMD_MSG(  280 )  /* 0x118 */


/* ICM20689                                                                 */
#define ICM20689_DIAG_TLM_MID                     TLM_MSG(  281 )  /* 0x119 */
#define ICM20689_HK_TLM_MID                       TLM_MSG(  282 )  /* 0x11a */
#define ICM20689_CMD_MID                          CMD_MSG(  283 )  /* 0x11b */
#define ICM20689_MEASURE_MID                      CMD_MSG(  284 )  /* 0x11c */
#define ICM20689_SEND_HK_MID                      CMD_MSG(  285 )  /* 0x11d */


/* MS5611                                                                   */
#define MS5611_DIAG_TLM_MID                       TLM_MSG(  286 )  /* 0x11e */
#define MS5611_HK_TLM_MID                         TLM_MSG(  287 )  /* 0x11f */
#define MS5611_CMD_MID                            CMD_MSG(  288 )  /* 0x120 */
#define MS5611_MEASURE_MID                        CMD_MSG(  289 )  /* 0x121 */
#define MS5611_SEND_HK_MID                        CMD_MSG(  290 )  /* 0x122 */
#define MS5611_WAKEUP_MID                         CMD_MSG(  291 )  /* 0x123 */


/* RCIN                                                                     */
#define RCIN_HK_TLM_MID                           TLM_MSG(  292 )  /* 0x124 */
#define RCIN_CMD_MID                              CMD_MSG(  293 )  /* 0x125 */
#define RCIN_SEND_HK_MID                          CMD_MSG(  294 )  /* 0x126 */
#define RCIN_WAKEUP_MID                           CMD_MSG(  295 )  /* 0x127 */


/* SED                                                                      */
#define SED_DIAG_TLM_MID                          TLM_MSG(  296 )  /* 0x128 */
#define SED_HK_TLM_MID                            TLM_MSG(  297 )  /* 0x129 */
#define SED_CMD_MID                               CMD_MSG(  298 )  /* 0x12a */
#define SED_MEASURE_MID                           CMD_MSG(  299 )  /* 0x12b */
#define SED_SEND_HK_MID                           CMD_MSG(  300 )  /* 0x12c */


/* ULR                                                                      */
#define ULR_HK_TLM_MID                            TLM_MSG(  301 )  /* 0x12d */
#define ULR_CMD_MID                               CMD_MSG(  302 )  /* 0x12e */
#define ULR_MEASURE_MID                           CMD_MSG(  303 )  /* 0x12f */
#define ULR_SEND_HK_MID                           CMD_MSG(  304 )  /* 0x130 */


/* VC                                                                       */
#define VC_HK_TLM_MID                             TLM_MSG(  305 )  /* 0x131 */
#define VC_CMD_MID                                CMD_MSG(  306 )  /* 0x132 */
#define VC_PROCESS_CMDS_MID                       CMD_MSG(  307 )  /* 0x133 */
#define VC_SEND_HK_MID                            CMD_MSG(  308 )  /* 0x134 */


/* ZUSUP                                                                    */
#define ZUSUP_DIAG_TLM_MID                        TLM_MSG(  309 )  /* 0x135 */
#define ZUSUP_HK_TLM_MID                          TLM_MSG(  310 )  /* 0x136 */
#define ZUSUP_CMD_MID                             CMD_MSG(  311 )  /* 0x137 */
#define ZUSUP_SEND_HK_MID                         CMD_MSG(  312 )  /* 0x138 */
#define ZUSUP_WAKEUP_MID                          CMD_MSG(  313 )  /* 0x139 */


/* ADSBR                                                                    */
#define ADSBR_HK_TLM_MID                          TLM_MSG(  314 )  /* 0x13a */
#define ADSBR_OUT_DATA_MID                        TLM_MSG(  315 )  /* 0x13b */
#define ADSBR_CMD_MID                             CMD_MSG(  316 )  /* 0x13c */
#define ADSBR_SEND_HK_MID                         CMD_MSG(  317 )  /* 0x13d */
#define ADSBR_WAKEUP_MID                          CMD_MSG(  318 )  /* 0x13e */


/* SIMLINK                                                                  */
#define SIMLINK_HK_TLM_MID                        TLM_MSG(  319 )  /* 0x13f */
#define SIMLINK_OUT_DATA_MID                      TLM_MSG(  320 )  /* 0x140 */
#define SIMLINK_CMD_MID                           CMD_MSG(  321 )  /* 0x141 */
#define SIMLINK_SEND_HK_MID                       CMD_MSG(  322 )  /* 0x142 */
#define SIMLINK_WAKEUP_MID                        CMD_MSG(  323 )  /* 0x143 */


/* UBLOX_M8N                                                                */
#define UBLOX_M8N_OUT_DATA_MID                    TLM_MSG(  324 )  /* 0x144 */
#define UBLOX_M8N_HK_TLM_MID                      TLM_MSG(  325 )  /* 0x145 */
#define UBLOX_M8N_CMD_MID                         CMD_MSG(  326 )  /* 0x146 */
#define UBLOX_M8N_SEND_HK_MID                     CMD_MSG(  327 )  /* 0x147 */
#define UBLOX_M8N_WAKEUP_MID                      CMD_MSG(  328 )  /* 0x148 */


/* RFD900X                                                                  */
#define RFD900X_OUT_DATA_MID                      TLM_MSG(  329 )  /* 0x149 */
#define RFD900X_HK_TLM_MID                        TLM_MSG(  330 )  /* 0x14a */
#define RFD900X_CMD_MID                           CMD_MSG(  331 )  /* 0x14b */
#define RFD900X_SEND_HK_MID                       CMD_MSG(  332 )  /* 0x14c */
#define RFD900X_WAKEUP_MID                        CMD_MSG(  333 )  /* 0x14d */


/* SEDPWM                                                                   */
#define SEDPWM_OUT_DATA_MID                       TLM_MSG(  334 )  /* 0x14e */
#define SEDPWM_HK_TLM_MID                         TLM_MSG(  335 )  /* 0x14f */
#define SEDPWM_CMD_MID                            CMD_MSG(  336 )  /* 0x150 */
#define SEDPWM_SEND_HK_MID                        CMD_MSG(  337 )  /* 0x151 */
#define SEDPWM_WAKEUP_MID                         CMD_MSG(  338 )  /* 0x152 */


/* SEDSBUS                                                                  */
#define SBUS_CMD_MID                              CMD_MSG(  339 )  /* 0x153 */
#define SBUS_SEND_HK_MID                          CMD_MSG(  340 )  /* 0x154 */
#define SBUS_WAKEUP_MID                           CMD_MSG(  341 )  /* 0x155 */
#define SBUS_OUT_DATA_MID                         TLM_MSG(  342 )  /* 0x156 */
#define SBUS_HK_TLM_MID                           TLM_MSG(  343 )  /* 0x157 */

#endif
