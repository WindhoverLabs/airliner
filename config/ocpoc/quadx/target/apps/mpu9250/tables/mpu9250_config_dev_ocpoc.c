/*
** Pragmas
*/

/*
** Include Files
*/
#include "cfe_tbl_filedef.h"
#include "mpu9250_tbldefs.h"

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

    "MPU9250_ConfigTbl", "MPU9250.CONFIG_TBL", "MPU9250 default config table",
    "mpu9250_config.tbl", (sizeof(MPU9250_ConfigTbl_t))
};

/*
** External Global Variables
*/

/*
** Global Variables
*/

/* Default MPU9250 config table data */
MPU9250_ConfigTbl_t MPU9250_ConfigTbl =
{
        /* User calibration params */
        /* AccXScale */
        1.002483367919921875f,
        /* AccYScale */
        1.002597928047180176f,
        /* AccZScale */
        0.991139471530914307f,
        /* AccXOffset */
        0.018333911895751953f,
        /* AccYOffset */
        -0.134463787078857422f,
        /* AccZOffset */
        0.048464775085449219f,
        /* GyroXScale */
        1.0f,
        /* GyroYScale */
        1.0f,
        /* GyroZScale */
        1.0f,
        /* GyroXOffset */
        0.019233696162700653f,
        /* GyroYOffset */
        0.024173684418201447f,
        /* GyroZOffset */
        -0.008248925209045410f
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
** End of file mpu9250_config.c
**=====================================================================================*/
    
