/*
** Pragmas
*/

/*
** Include Files
*/
#include "cfe_tbl_filedef.h"
#include "mpu6050_tbldefs.h"

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

    "MPU6050_ConfigTbl", "MPU6050.CONFIG_TBL", "MPU6050 default config table",
    "mpu6050_config.tbl", (sizeof(MPU6050_ConfigTbl_t))
};

/*
** External Global Variables
*/

/*
** Global Variables
*/

/* Default MPU6050 config table data */
MPU6050_ConfigTbl_t MPU6050_ConfigTbl =
{
        /* User calibration params */
        /* AccXScale */
        0.996452748775482178f,
        /* AccYScale */
        0.999744415283203125f,
        /* AccZScale */
        0.989718735218048096f,
        /* AccXOffset */
        0.127070903778076172f,
        /* AccYOffset */
        -0.047137737274169922f,
        /* AccZOffset */
        -0.334469795227050781f,
        /* GyroXScale */
        1.0f,
        /* GyroYScale */
        1.0f,
        /* GyroZScale */
        1.0f,
        /* GyroXOffset */
        -0.097985103726387024f,
        /* GyroYOffset */
        -0.031116940081119537f,
        /* GyroZOffset */
        -0.020994935184717178f
        /* MagXScale */
        /* MagYScale */
        /* MagZScale */
        /* MagXOffset */
        /* MagYOffset */
        /* MagZOffset */
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
** End of file mpu6050_config.c
**=====================================================================================*/
    
