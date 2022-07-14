/*
** Pragmas
*/

/*
** Include Files
*/
#include "cfe_tbl_filedef.h"
#include "icm20689_tbldefs.h"

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

    "ICM20689_ConfigTbl", "ICM20689.CONFIG_TBL", "ICM20689 default config table",
    "icm20689_config.tbl", (sizeof(ICM20689_ConfigTbl_t))
};

/*
** External Global Variables
*/

/*
** Global Variables
*/

/* Default ICM20689 config table data */
ICM20689_ConfigTbl_t ICM20689_ConfigTbl =
{
        /* User calibration. */
        /* AccXScale */
        1.0f,
        /* AccYScale */
        1.0f,
        /* AccZScale */
        1.0f,
        /* AccXOffset */
        1.0f,
        /* AccYOffset */
        0.0f,
        /* AccZOffset */
        0.0f,
        /* GyroXScale */
        1.0f,
        /* GyroYScale */
        1.0f,
        /* GyroZScale */
        1.0f,
        /* GyroXOffset */
        0.0f,
        /* GyroYOffset */
        0.0f,
        /* GyroZOffset */
        0.0f,
        /* TempOffset */
        ICM20689_ROOM_TEMP_OFFSET,
        /* TempSensitivity */
        ICM20689_TEMPERATURE_SENS,
        /* WhoAmIValue */
        ICM20689_WHO_AM_I_VALUE,
        /* AccUnit */
        ICM20689_ONE_G,
        /* GyroUnit */
        ICM20689_RADIANS_PER_DEGREE,
        /* AccDivider */
        2048,
        /* GyroDivider */
        16.4,
        /* AccScale */
        16,
        /* GyroScale */
        2000,
        /* Settings */
        /* GyroScaleSetting */
        ICM20689_SET_GYRO_2000_DPS,
        /* AccelScaleSetting */
        ICM20689_SET_ACCEL_16_G,
        /* ClockSourceSetting */
        ICM20689_SET_CLKSEL_1,
        /* SampleRateDivSetting */
        ICM20689_SET_SMPLRT_100HZ,
        /* ResetDelayUs */
        100000,
        /* InitDelayUs */
        100000,
        /* WakeupDelayUs */
        35000,
        /* RetryDelayUs */
        10000,
        /* Configuration registers */
        {
            /* 1. Power management. */
            ICM20689_PWR_MGMT_1,
            /* 2. Reset signal paths. */
            ICM20689_SIGNAL_PATH_RESET,
            /* 3. User control. */
            ICM20689_USER_CTRL,
            /* 4. Config. */
            ICM20689_CONFIG,
            /* 5. Gyro config. */
            ICM20689_GYRO_CONFIG,
            /* 6. Accel config. */
            ICM20689_ACCEL_CONFIG
        },
        /* Value registers */
        {
            
            /* 1. Wakeup set clock source. */
            ICM20689_SET_CLKSEL_1,
            /* 2. Reset signal paths. */
            ICM20689_MASK_ACCEL_RST & ICM20689_MASK_TEMP_RST,
            /* 3. Reset sensor signal paths and disable I2C. */
            ICM20689_MASK_SIG_COND_RST & ICM20689_MASK_I2C_IF_DIS,
            /* 4. Config. */
            ICM20689_SET_DLPF_CFG_250HZ,
            /* 5. Gyro config. */
            ICM20689_SET_GYRO_2000_DPS,
            /* 6. Accel config. */
            ICM20689_SET_ACCEL_16_G
        },
        /* ConfigCount */
        6
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
** End of file icm20689_config.c
**=====================================================================================*/
