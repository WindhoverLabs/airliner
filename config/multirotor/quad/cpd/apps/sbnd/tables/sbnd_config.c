/*
** Pragmas
*/

/*
** Include Files
*/
#include "cfe_tbl_filedef.h"
#include "sbnd_tbldefs.h"
#include "msg_ids.h"

/*
** Local Defines
*/

/*
** Local Structure Declarations
*/
static OS_USED CFE_TBL_FileDef_t CFE_TBL_FileDef =
{
    /* Content format: ObjName[64], TblName[38], Desc[32], TgtFileName[20], ObjSize
    **    ObjName - variable name of config table, e.g., CI_ConfigDefTbl[]
    **    TblName - app's table name, e.g., CI.CONFIG_TBL, where CI is the same app name
    **              used in cfe_es_startup.scr, and CI_defConfigTbl is the same table
    **              name passed in to CFE_TBL_Register()
    **    Desc - description of table in string format
    **    TgtFileName[20] - table file name, compiled as .tbl file extension
    **    ObjSize - size of the entire table
    */

    "SBND_ConfigTbl", "SBND.CONFIG_TBL", "SBND default config table",
    "sbnd_config.tbl", (sizeof(SBND_ConfigTbl_t))
};

/*
** External Global Variables
*/

/*
** Global Variables
*/

/* Default SBND config table data */
SBND_ConfigTbl_t SBND_ConfigTbl =
{
    {
    	    /*   0 */  CFE_EVS_EVENT_MSG_MID,
    	    /*   1 */  CFE_ES_HK_TLM_MID,
    	    /*   2 */  CFE_EVS_HK_TLM_MID,
    	    /*   3 */  CFE_SB_HK_TLM_MID,
    	    /*   4 */  CFE_TBL_HK_TLM_MID,
    	    /*   5 */  CFE_TIME_HK_TLM_MID,
    	    /*   6 */  CFE_TIME_DIAG_TLM_MID,
    	    /*   7 */  CFE_SB_STATS_TLM_MID,
    	    /*   8 */  CFE_ES_APP_TLM_MID,
    	    /*   9 */  CFE_TBL_REG_TLM_MID,
    	    /*  10 */  CFE_ES_MEMSTATS_TLM_MID,
    	    /*  11 */  CF_CONFIG_TLM_MID,
    	    /*  12 */  CF_HK_TLM_MID,
    	    /*  13 */  CF_TRANS_TLM_MID,
    	    /*  14 */  CI_HK_TLM_MID,
    	    /*  15 */  CS_HK_TLM_MID,
    	    /*  16 */  CVT_HK_TLM_MID,
    	    /*  17 */  DS_DIAG_TLM_MID,
    	    /*  18 */  DS_HK_TLM_MID,
    	    /*  19 */  EA_HK_TLM_MID,
    	    /*  20 */  FM_DIR_LIST_TLM_MID,
    	    /*  21 */  FM_FILE_INFO_TLM_MID,
    	    /*  22 */  FM_FREE_SPACE_TLM_MID,
    	    /*  23 */  FM_HK_TLM_MID,
    	    /*  24 */  FM_OPEN_FILES_TLM_MID,
    	    /*  25 */  HK_HK_TLM_MID,
    	    /*  26 */  HS_HK_TLM_MID,
    	    /*  27 */  LC_HK_TLM_MID,
    	    /*  28 */  MD_HK_TLM_MID,
    	    /*  29 */  MM_HK_TLM_MID,
    	    /*  30 */  SBN_TLM_MID,
    	    /*  31 */  SBN_MODULE_HK_TLM_MID,
    	    /*  32 */  SC_HK_TLM_MID,
    	    /*  33 */  SCH_DIAG_TLM_MID,
    	    /*  34 */  SCH_HK_TLM_MID,
    	    /*  35 */  TO_DIAG_MSG_FLOW_MID,
    	    /*  36 */  TO_DIAG_TLM_MID,
    	    /*  37 */  TO_HK_TLM_MID,
    	    /*  38 */  AMC_HK_TLM_MID,
    	    /*  39 */  LD_DIAG_TLM_MID,
    	    /*  40 */  LD_HK_TLM_MID,
    	    /*  41 */  LGC_HK_TLM_MID,
    	    /*  42 */  NAV_HK_TLM_MID,
    	    /*  43 */  PE_HK_TLM_MID,
    	    /*  44 */  PE_DIAG_TLM_MID,
    	    /*  45 */  QAE_HK_TLM_MID,
    	    /*  46 */  SBND_HK_TLM_MID,
    	    /*  47 */  SENS_HK_TLM_MID,
    	    /*  48 */  VM_CONFIG_TLM_MID,
    	    /*  49 */  VM_HK_TLM_MID,
    	    /*  50 */  BAT_HK_TLM_MID,
    	    /*  51 */  GPS_HK_TLM_MID,
    	    /*  52 */  HMC5883_DIAG_TLM_MID,
    	    /*  53 */  HMC5883_HK_TLM_MID,
    	    /*  54 */  ICM20689_DIAG_TLM_MID,
    	    /*  55 */  ICM20689_HK_TLM_MID,
    	    /*  56 */  MS5611_DIAG_TLM_MID,
    	    /*  57 */  MS5611_HK_TLM_MID,
    	    /*  58 */  RCIN_HK_TLM_MID,
    	    /*  59 */  SED_DIAG_TLM_MID,
    	    /*  60 */  SED_HK_TLM_MID,
    	    /*  61 */  ULR_HK_TLM_MID,
    	    /*  62 */  VC_HK_TLM_MID,
    	    /*  63 */  ZUSUP_DIAG_TLM_MID,
    	    /*  64 */  ZUSUP_HK_TLM_MID,
    	    /*  65 */  ADSBR_HK_TLM_MID,
    	    /*  66 */  ASPD4525_HK_TLM_MID,
    	    /*  67 */  PX4_ACTUATOR_ARMED_MID,
    	    /*  68 */  PX4_ACTUATOR_CONTROLS_0_MID,
    	    /*  69 */  PX4_ACTUATOR_CONTROLS_1_MID,
    	    /*  70 */  PX4_ACTUATOR_CONTROLS_2_MID,
    	    /*  71 */  PX4_ACTUATOR_CONTROLS_3_MID,
    	    /*  72 */  PX4_ACTUATOR_DIRECT_MID,
    	    /*  73 */  PX4_ACTUATOR_OUTPUTS_MID,
    	    /*  74 */  PX4_ADC_REPORT_MID,
    	    /*  75 */  PX4_AIRSPEED_MID,
    	    /*  76 */  PX4_ATT_POS_MOCAP_MID,
    	    /*  77 */  PX4_BATTERY_STATUS_MID,
    	    /*  78 */  PX4_CAMERA_TRIGGER_MID,
    	    /*  79 */  PX4_COMMANDER_STATE_MID,
    	    /*  80 */  PX4_CONTROL_STATE_MID,
    	    /*  81 */  PX4_CPULOAD_MID,
    	    /*  82 */  PX4_DEBUG_KEY_VALUE_MID,
    	    /*  83 */  PX4_DIFFERENTIAL_PRESSURE_MID,
    	    /*  84 */  PX4_DISTANCE_SENSOR_MID,
    	    /*  85 */  PX4_FW_POS_CTRL_STATUS_MID,
    	    /*  86 */  PX4_FW_VIRTUAL_ATTITUDE_SETPOINT_MID,
    	    /*  87 */  PX4_FW_VIRTUAL_RATES_SETPOINT_MID,
    	    /*  88 */  PX4_EKF2_INNOVATIONS_MID,
    	    /*  89 */  PX4_EKF2_REPLAY_MID,
    	    /*  90 */  PX4_ESC_REPORT_MID,
    	    /*  91 */  PX4_ESC_STATUS_MID,
    	    /*  92 */  PX4_ESTIMATOR_STATUS_MID,
    	    /*  93 */  PX4_FENCE_MID,
    	    /*  94 */  PX4_FENCE_VERTEX_MID,
    	    /*  95 */  PX4_FILTERED_BOTTOM_FLOW_MID,
    	    /*  96 */  PX4_FOLLOW_TARGET_MID,
    	    /*  97 */  PX4_GEOFENCE_RESULT_MID,
    	    /*  98 */  PX4_GPS_DUMP_MID,
    	    /*  99 */  PX4_GPS_INJECT_DATA_MID,
    	    /* 100 */  PX4_HIL_SENSOR_MID,
    	    /* 101 */  PX4_HOME_POSITION_MID,
    	    /* 102 */  PX4_INPUT_RC_MID,
    	    /* 103 */  PX4_LED_CONTROL_MID,
    	    /* 104 */  PX4_LOG_MESSAGE_MID,
    	    /* 105 */  PX4_MANUAL_CONTROL_SETPOINT_MID,
    	    /* 106 */  PX4_MAVLINK_LOG_MID,
    	    /* 107 */  PX4_MC_ATT_CTRL_STATUS_MID,
    	    /* 108 */  PX4_MC_VIRTUAL_ATTITUDE_SETPOINT_MID,
    	    /* 109 */  PX4_MC_VIRTUAL_RATES_SETPOINT_MID,
    	    /* 110 */  PX4_MISSION_MID,
    	    /* 111 */  PX4_MISSION_RESULT_MID,
    	    /* 112 */  PX4_MULTIROTOR_MOTOR_LIMITS_MID,
    	    /* 113 */  PX4_OFFBOARD_CONTROL_MODE_MID,
    	    /* 114 */  PX4_OPTICAL_FLOW_MID,
    	    /* 115 */  PX4_OUTPUT_PWM_MID,
    	    /* 116 */  PX4_PARAMETER_UPDATE_MID,
    	    /* 117 */  PX4_POSITION_SETPOINT_MID,
    	    /* 118 */  PX4_POSITION_SETPOINT_TRIPLET_MID,
    	    /* 119 */  PX4_PWM_INPUT_MID,
    	    /* 120 */  PX4_QSHELL_REQ_MID,
    	    /* 121 */  PX4_RC_CHANNELS_MID,
    	    /* 122 */  PX4_RC_PARAMETER_MAP_MID,
    	    /* 123 */  PX4_SAFETY_MID,
    	    /* 124 */  PX4_SATELLITE_INFO_MID,
    	    /* 125 */  PX4_SENSOR_ACCEL_MID,
    	    /* 126 */  PX4_SENSOR_BARO_MID,
    	    /* 127 */  PX4_SENSOR_COMBINED_MID,
    	    /* 128 */  PX4_SENSOR_GYRO_MID,
    	    /* 129 */  PX4_SENSOR_MAG_MID,
    	    /* 130 */  PX4_SERVORAIL_STATUS_MID,
    	    /* 131 */  PX4_SUBSYSTEM_INFO_MID,
    	    /* 132 */  PX4_SYSTEM_POWER_MID,
    	    /* 133 */  PX4_TECS_STATUS_MID,
    	    /* 134 */  PX4_TELEMETRY_STATUS_MID,
    	    /* 135 */  PX4_TEST_MOTOR_MID,
    	    /* 136 */  PX4_TIME_OFFSET_MID,
    	    /* 137 */  PX4_TRANSPONDER_REPORT_MID,
    	    /* 138 */  PX4_UAVCAN_PARAMETER_REQUEST_MID,
    	    /* 139 */  PX4_UAVCAN_PARAMETER_VALUE_MID,
    	    /* 140 */  PX4_VEHICLE_ATTITUDE_MID,
    	    /* 141 */  PX4_VEHICLE_ATTITUDE_SETPOINT_MID,
    	    /* 142 */  PX4_VEHICLE_COMMAND_ACK_MID,
    	    /* 143 */  PX4_VEHICLE_COMMAND_MID,
    	    /* 144 */  PX4_VEHICLE_CONTROL_MODE_MID,
    	    /* 145 */  PX4_VEHICLE_FORCE_SETPOINT_MID,
    	    /* 146 */  PX4_VEHICLE_GLOBAL_POSITION_MID,
    	    /* 147 */  PX4_VEHICLE_GLOBAL_VELOCITY_SETPOINT_MID,
    	    /* 148 */  PX4_VEHICLE_GPS_POSITION_MID,
    	    /* 149 */  PX4_VEHICLE_LAND_DETECTED_MID,
    	    /* 150 */  PX4_VEHICLE_LOCAL_POSITION_MID,
    	    /* 151 */  PX4_VEHICLE_LOCAL_POSITION_SETPOINT_MID,
    	    /* 152 */  PX4_VEHICLE_RATES_SETPOINT_MID,
    	    /* 153 */  PX4_VEHICLE_STATUS_MID,
    	    /* 154 */  PX4_VISION_POSITION_ESTIMATE_MID,
    	    /* 155 */  PX4_VTOL_VEHICLE_STATUS_MID,
    	    /* 156 */  PX4_WIND_ESTIMATE_MID,
    	    /* 157 */  PX4_SENSOR_CORRECTION_MID,
    	    /* 158 */  PX4_POSITION_CONTROL_STATUS_MID
    }
};

/*
** Local Variables
*/

/*
** Function Prototypes
*/

/*
** Function Definitions
*/

/*=======================================================================================
** End of file sbnd_config.c
**=====================================================================================*/
    
