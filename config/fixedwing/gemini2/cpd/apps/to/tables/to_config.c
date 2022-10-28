/************************************************************************
 ** Includes
 *************************************************************************/
#include "cfe_tbl_filedef.h"
#include "to_tbldefs.h"
#include "msg_ids.h"

/**
 ** \brief The cFE TO config table definition.
 **
 ** Content format: ObjName[64], TblName[38], Desc[32], TgtFileName[20], ObjSize
 **    ObjName - variable name of config table, e.g., TO_ConfigDefTbl[]
 **    TblName - app's table name, e.g., TO.CONFIG_TBL, where TO is the same app name
 **              used in cfe_es_startup.scr, and TO_defConfigTbl is the same table
 **              name passed in to CFE_TBL_Register()
 **    Desc - description of table in string format
 **    TgtFileName[20] - table file name, compiled as .tbl file extension
 **    ObjSize - size of the entire table
 **
 */
static CFE_TBL_FileDef_t CFE_TBL_FileDef OS_USED =
{
  "TO_ConfigTbl", "TO.CONFIG", "TO downlink table",
  "to_config.tbl", (sizeof(TO_ChannelTbl_t))
};


/************************************************************************
 ** Defines
 *************************************************************************/

#define TO_PQUEUE_CPD_SINGLE_PASS_IDX               0
#define TO_PQUEUE_PPD_SINGLE_PASS_IDX               1
#define TO_PQUEUE_CPD_HIGH_OPS_RSRVD_IDX            2
#define TO_PQUEUE_PPD_HIGH_OPS_RSRVD_IDX            3
#define TO_PQUEUE_CPD_HIGH_IDX                      4
#define TO_PQUEUE_CPD_EVENTS_IDX                    5
#define TO_PQUEUE_CPD_CFDP_IDX                      6
#define TO_PQUEUE_CPD_SHELL_IDX                     7
#define TO_PQUEUE_CPD_HK_IDX                        8
#define TO_PQUEUE_PPD_HIGH_IDX                      9
#define TO_PQUEUE_PPD_EVENTS_IDX                   10
#define TO_PQUEUE_PPD_CFDP_IDX                     11
#define TO_PQUEUE_PPD_SHELL_IDX                    12
#define TO_PQUEUE_PPD_HK_IDX                       13
#define TO_PQUEUE_CPD_PX4LOW_IDX                   14
#define TO_PQUEUE_PPD_PX4LOW_LOW_IDX               15

/**
 **  \brief Default TO config table data
 */
TO_ChannelTbl_t TO_ConfigTbl =
{
    /* Table ID */
    1,
    {
        /* Message Flows */
       // {AMC_HK_TLM_MID,                       1, TO_PQUEUE_CPD_HK_IDX        },
        {BAT_HK_TLM_MID,                       1, TO_PQUEUE_CPD_HK_IDX        },
        {CF_CONFIG_TLM_MID,                    1, TO_PQUEUE_CPD_HK_IDX        },
        {CF_HK_TLM_MID,                        1, TO_PQUEUE_CPD_HK_IDX        },
        {CF_CPD_TO_GND_PDU_MID,               32, TO_PQUEUE_CPD_CFDP_IDX      },
        {CF_TRANS_TLM_MID,                     1, TO_PQUEUE_CPD_HK_IDX        },
        {CFE_ES_APP_TLM_MID,                   1, TO_PQUEUE_CPD_HK_IDX        },
        {CFE_ES_HK_TLM_MID,                    1, TO_PQUEUE_CPD_HK_IDX        },
        {CFE_ES_MEMSTATS_TLM_MID,              1, TO_PQUEUE_CPD_HK_IDX        },
        {CFE_ES_SHELL_TLM_MID,                32, TO_PQUEUE_CPD_SHELL_IDX     },
        {CFE_EVS_EVENT_MSG_MID,               32, TO_PQUEUE_CPD_EVENTS_IDX    },
        {CFE_EVS_HK_TLM_MID,                   1, TO_PQUEUE_CPD_HK_IDX        },
        {CFE_SB_HK_TLM_MID,                    1, TO_PQUEUE_CPD_HK_IDX        },
        {CFE_SB_STATS_TLM_MID,                 1, TO_PQUEUE_CPD_HK_IDX        },
        {CFE_TBL_HK_TLM_MID,                   1, TO_PQUEUE_CPD_HK_IDX        },
        {CFE_TBL_REG_TLM_MID,                  1, TO_PQUEUE_CPD_HK_IDX        },
        {CFE_TIME_HK_TLM_MID,                  1, TO_PQUEUE_CPD_HK_IDX        },
        {CFE_TIME_DIAG_TLM_MID,                1, TO_PQUEUE_CPD_HK_IDX        },
        {CI_HK_TLM_MID,                        1, TO_PQUEUE_CPD_HK_IDX        },
        {CS_HK_TLM_MID,                        1, TO_PQUEUE_CPD_HK_IDX        },
        {DS_DIAG_TLM_MID,                      1, TO_PQUEUE_CPD_HIGH_IDX      },
        {DS_HK_TLM_MID,                        1, TO_PQUEUE_CPD_HK_IDX        },
        {FM_DIR_LIST_TLM_MID,                  1, TO_PQUEUE_CPD_HK_IDX        },
        {FM_FILE_INFO_TLM_MID,                 1, TO_PQUEUE_CPD_HK_IDX        },
        {FM_FREE_SPACE_TLM_MID,                1, TO_PQUEUE_CPD_HK_IDX        },
        {FM_HK_TLM_MID,                        1, TO_PQUEUE_CPD_HK_IDX        },
        {FM_OPEN_FILES_TLM_MID,                1, TO_PQUEUE_CPD_HK_IDX        },
        {HK_HK_TLM_MID,                        1, TO_PQUEUE_CPD_HK_IDX        },
        {HS_HK_TLM_MID,                        1, TO_PQUEUE_CPD_HK_IDX        },
        {LC_HK_TLM_MID,                        1, TO_PQUEUE_CPD_HK_IDX        },
       // {LD_DIAG_TLM_MID,                      1, TO_PQUEUE_CPD_HIGH_IDX      },
        {LD_HK_TLM_MID,                        1, TO_PQUEUE_CPD_HK_IDX        },
        {FAC_HK_TLM_MID,                       1, TO_PQUEUE_CPD_HK_IDX        },
       // {MD_HK_TLM_MID,                        1, TO_PQUEUE_CPD_HK_IDX        },
        {MM_HK_TLM_MID,                        1, TO_PQUEUE_CPD_HK_IDX        },
        {FPC_DIAG_TLM_MID,                     1, TO_PQUEUE_CPD_HIGH_IDX      },
        {FPC_HK_TLM_MID,                       1, TO_PQUEUE_CPD_HK_IDX        },
    //    {PX4_POSITION_CONTROL_STATUS_MID,      1, TO_PQUEUE_CPD_HK_IDX        },
      //  {NAV_HK_TLM_MID,                       1, TO_PQUEUE_CPD_HK_IDX        },
        //{PE_HK_TLM_MID,                        1, TO_PQUEUE_CPD_HK_IDX        },
        {PE_DIAG_TLM_MID,                      1, TO_PQUEUE_CPD_HIGH_IDX      },
        {RCIN_HK_TLM_MID,                      1, TO_PQUEUE_CPD_HK_IDX        },
        {SBN_MODULE_HK_TLM_MID,                1, TO_PQUEUE_CPD_HK_IDX        },
        {SBN_TLM_MID,                          1, TO_PQUEUE_CPD_HK_IDX        },
        {SCH_DIAG_TLM_MID,                     1, TO_PQUEUE_CPD_HIGH_IDX      },
        {SCH_HK_TLM_MID,                       1, TO_PQUEUE_CPD_HK_IDX        },
        //{SENS_HK_TLM_MID,                      1, TO_PQUEUE_CPD_HK_IDX        },
        {TO_DIAG_MSG_FLOW_MID,                 1, TO_PQUEUE_CPD_HIGH_IDX      },
        {TO_DIAG_TLM_MID,                      1, TO_PQUEUE_CPD_HIGH_IDX      },
        {TO_HK_TLM_MID,                        1, TO_PQUEUE_CPD_HK_IDX        },
        {ULR_HK_TLM_MID,                       1, TO_PQUEUE_CPD_HK_IDX        },
        {VM_CONFIG_TLM_MID,                    1, TO_PQUEUE_CPD_HK_IDX        },
        {VM_HK_TLM_MID,                        1, TO_PQUEUE_CPD_HK_IDX        },
        {ZUSUP_DIAG_TLM_MID,                   1, TO_PQUEUE_CPD_HIGH_IDX      },
        {ZUSUP_HK_TLM_MID,                     1, TO_PQUEUE_CPD_HK_IDX        },
        {SBND_HK_TLM_MID,                      1, TO_PQUEUE_CPD_HK_IDX        },
        {ICM20689_HK_TLM_MID,                  1, TO_PQUEUE_CPD_HK_IDX        },
        {ICM20689_DIAG_TLM_MID,                1, TO_PQUEUE_CPD_HIGH_IDX      },
        {MS5611_HK_TLM_MID,                    1, TO_PQUEUE_CPD_HK_IDX        },
        {MS5611_DIAG_TLM_MID,                  1, TO_PQUEUE_CPD_HIGH_IDX      },
        {HMC5883_HK_TLM_MID,                   1, TO_PQUEUE_CPD_HK_IDX        },
        {HMC5883_DIAG_TLM_MID,                 1, TO_PQUEUE_CPD_HIGH_IDX      },
        {ADSBR_HK_TLM_MID,                     1, TO_PQUEUE_CPD_HIGH_IDX      },
        {ASPD4525_HK_TLM_MID,                  1, TO_PQUEUE_CPD_HK_IDX        },
        {HES_HK_TLM_MID,                         1, TO_PQUEUE_PPD_HK_IDX},
     //   {TO_CUSTOM_HK_TLM_MID,                 1, TO_PQUEUE_CPD_HK_IDX        },
       // {CI_CUSTOM_HK_TLM_MID,                 1, TO_PQUEUE_CPD_HK_IDX        },

//        {PX4_ACTUATOR_ARMED_MID,               1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//		{PX4_ACTUATOR_CONTROLS_0_MID,          1, TO_PQUEUE_CPD_HIGH_IDX    },
//        {PX4_ACTUATOR_CONTROLS_1_MID,          1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_ACTUATOR_CONTROLS_2_MID,          1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_ACTUATOR_CONTROLS_3_MID,          1, TO_PQUEUE_CPD_HIGH_IDX    },
//        {PX4_ACTUATOR_DIRECT_MID,              1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//		{PX4_ACTUATOR_OUTPUTS_MID,             1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_ADC_REPORT_MID,                   1, TO_PQUEUE_CPD_PX4LOW_IDX    },
    //    {PX4_AIRSPEED_MID,                     1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_ATT_POS_MOCAP_MID,                1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_BATTERY_STATUS_MID,               1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_CAMERA_TRIGGER_MID,               1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_COMMANDER_STATE_MID,              1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_CONTROL_STATE_MID,                1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_CPULOAD_MID,                      1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_DEBUG_KEY_VALUE_MID,              1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_DIFFERENTIAL_PRESSURE_MID,        1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_DISTANCE_SENSOR_MID,              1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_FW_POS_CTRL_STATUS_MID,           1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_FW_VIRTUAL_ATTITUDE_SETPOINT_MID, 1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_FW_VIRTUAL_RATES_SETPOINT_MID,    1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_EKF2_INNOVATIONS_MID,             1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_EKF2_REPLAY_MID,                  1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_ESC_REPORT_MID,                   1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_ESC_STATUS_MID,                   1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_ESTIMATOR_STATUS_MID,             1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_FENCE_MID,                        1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_FENCE_VERTEX_MID,                 1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_FILTERED_BOTTOM_FLOW_MID,         1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_FOLLOW_TARGET_MID,                1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_GEOFENCE_RESULT_MID,              1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_GPS_DUMP_MID,                     1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_GPS_INJECT_DATA_MID,              1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_HIL_SENSOR_MID,                   1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_HOME_POSITION_MID,                1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//          {PX4_INPUT_RC_MID,                     1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_LED_CONTROL_MID,                  1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_LOG_MESSAGE_MID,                  1, TO_PQUEUE_CPD_PX4LOW_IDX    },
    //      {PX4_MANUAL_CONTROL_SETPOINT_MID,      1, TO_PQUEUE_CPD_HIGH_IDX      },
//        {PX4_MAVLINK_LOG_MID,                  1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_MC_ATT_CTRL_STATUS_MID,           1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_MC_VIRTUAL_ATTITUDE_SETPOINT_MID, 1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_MC_VIRTUAL_RATES_SETPOINT_MID,    1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_MISSION_MID,                      1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_MISSION_RESULT_MID,               1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_MULTIROTOR_MOTOR_LIMITS_MID,      1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_OFFBOARD_CONTROL_MODE_MID,        1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_OPTICAL_FLOW_MID,                 1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_OUTPUT_PWM_MID,                   1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_PARAMETER_UPDATE_MID,             1, TO_PQUEUE_CPD_PX4LOW_IDX    },
        {PX4_POSITION_SETPOINT_TRIPLET_MID,    1, TO_PQUEUE_CPD_HIGH_IDX    },
        {FROM_PPD(CFE_ES_HK_TLM_MID),               1, TO_PQUEUE_CPD_HK_IDX    },
//        {PX4_PWM_INPUT_MID,                    1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_QSHELL_REQ_MID,                   1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_RC_CHANNELS_MID,                  1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_RC_PARAMETER_MAP_MID,             1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_SAFETY_MID,                       1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_SATELLITE_INFO_MID,               1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_SENSOR_ACCEL_MID,                 1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_SENSOR_ACCEL_MID,                 1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_SENSOR_BARO_MID,                  1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_SENSOR_COMBINED_MID,              1, TO_PQUEUE_CPD_PX4LOW_IDX      },
//        {PX4_SENSOR_CORRECTION_MID,            1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_SENSOR_GYRO_MID,                  1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_SENSOR_MAG_MID,                   1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_SERVORAIL_STATUS_MID,             1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_SUBSYSTEM_INFO_MID,               1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_SYSTEM_POWER_MID,                 1, TO_PQUEUE_CPD_PX4LOW_IDX    },
     //   {PX4_TECS_STATUS_MID,                  1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_TELEMETRY_STATUS_MID,             1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_TEST_MOTOR_MID,                   1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_TIME_OFFSET_MID,                  1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_TRANSPONDER_REPORT_MID,           1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_UAVCAN_PARAMETER_REQUEST_MID,     1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_UAVCAN_PARAMETER_VALUE_MID,       1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_VEHICLE_ATTITUDE_MID,             1, TO_PQUEUE_CPD_PX4LOW_IDX    },
     //   {PX4_VEHICLE_ATTITUDE_SETPOINT_MID,    1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_VEHICLE_COMMAND_ACK_MID,          1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_VEHICLE_COMMAND_MID,              1, TO_PQUEUE_CPD_PX4LOW_IDX    },
      //  {PX4_VEHICLE_CONTROL_MODE_MID,         1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_VEHICLE_FORCE_SETPOINT_MID,       1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_VEHICLE_GLOBAL_POSITION_MID,      1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_VEHICLE_GLOBAL_VELOCITY_SETPOINT_MID, 1, TO_PQUEUE_CPD_PX4LOW_IDX},
//        {PX4_VEHICLE_GPS_POSITION_MID,         1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_VEHICLE_LAND_DETECTED_MID,        1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_VEHICLE_LOCAL_POSITION_MID,       1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_VEHICLE_LOCAL_POSITION_SETPOINT_MID, 1, TO_PQUEUE_CPD_PX4LOW_IDX },
//        {PX4_VEHICLE_RATES_SETPOINT_MID,       1, TO_PQUEUE_CPD_PX4LOW_IDX    },
    //    {PX4_VEHICLE_STATUS_MID,               1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_VISION_POSITION_ESTIMATE_MID,     1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_VTOL_VEHICLE_STATUS_MID,          1, TO_PQUEUE_CPD_PX4LOW_IDX    },
//        {PX4_WIND_ESTIMATE_MID,                1, TO_PQUEUE_CPD_PX4LOW_IDX    }
          {FROM_PPD(HES_HK_TLM_MID),                         1, TO_PQUEUE_CPD_HK_IDX}
  },{
        /* Priority Queues */
        /* TO_PQUEUE_CPD_SINGLE_PASS_IDX */
        {TO_PQUEUE_ENA, 20, TO_PRIORITY_QUEUE_TYPE_SINGLE                     },
        /* TO_PQUEUE_CPD_HIGH_OPS_RSRVD_IDX */
        {TO_PQUEUE_ENA, 20, TO_PRIORITY_QUEUE_TYPE_FIFO                       },
        /* TO_PQUEUE_CPD_HIGH_IDX */
        {TO_PQUEUE_ENA, 32, TO_PRIORITY_QUEUE_TYPE_FIFO                       },
        /* TO_PQUEUE_CPD_EVENTS_IDX */
        {TO_PQUEUE_ENA, 32, TO_PRIORITY_QUEUE_TYPE_FIFO                       },
        /* TO_PQUEUE_CPD_CFDP_IDX */
        {TO_PQUEUE_ENA, 64, TO_PRIORITY_QUEUE_TYPE_FIFO                       },
        /* TO_PQUEUE_CPD_HK_IDX */
        {TO_PQUEUE_ENA, 55, TO_PRIORITY_QUEUE_TYPE_FIFO                       },
        /* TO_PQUEUE_CPD_SHELL_IDX */
        {TO_PQUEUE_ENA, 64, TO_PRIORITY_QUEUE_TYPE_FIFO                       },
        /* TO_PQUEUE_CPD_PX4LOW_IDX */
        {TO_PQUEUE_ENA, 40, TO_PRIORITY_QUEUE_TYPE_FIFO                       },
        /* TO_PQUEUE_PPD_SINGLE_PASS_IDX */
        {TO_PQUEUE_ENA, 20, TO_PRIORITY_QUEUE_TYPE_SINGLE                     },
        /* TO_PQUEUE_PPD_HIGH_OPS_RSRVD_IDX */
        {TO_PQUEUE_ENA, 20, TO_PRIORITY_QUEUE_TYPE_FIFO                       },
        /* TO_PQUEUE_PPD_HIGH_IDX */
        {TO_PQUEUE_ENA, 32, TO_PRIORITY_QUEUE_TYPE_FIFO                       },
        /* TO_PQUEUE_PPD_EVENTS_IDX */
        {TO_PQUEUE_ENA, 32, TO_PRIORITY_QUEUE_TYPE_FIFO                       },
        /* TO_PQUEUE_PPD_CFDP_IDX */
        {TO_PQUEUE_ENA, 64, TO_PRIORITY_QUEUE_TYPE_FIFO                       },
        /* TO_PQUEUE_PPD_HK_IDX */
        {TO_PQUEUE_ENA, 50, TO_PRIORITY_QUEUE_TYPE_FIFO                       },
        /* TO_PQUEUE_PPD_SHELL_IDX */
        {TO_PQUEUE_ENA, 64, TO_PRIORITY_QUEUE_TYPE_FIFO                       },
        /* TO_PQUEUE_PPD_PX4LOW_IDX */
        {TO_PQUEUE_ENA, 40, TO_PRIORITY_QUEUE_TYPE_FIFO                       },

  }
};


/************************/
/*  End of File Comment */
/************************/
