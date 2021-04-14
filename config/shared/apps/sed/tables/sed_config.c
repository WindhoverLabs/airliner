/*
** Pragmas
*/

/*
** Include Files
*/
#include "cfe_tbl_filedef.h"
#include "sed_tbldefs.h"

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

    "SED_ConfigTbl", "SED.CONFIG_TBL", "SED default config table",
    "sed_config.tbl", (sizeof(SED_ConfigTbl_t))
};

/*
** External Global Variables
*/

/*
** Global Variables
*/

/* Default SED config table data */
SED_ConfigTbl_t SED_ConfigTbl =
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
        /* MagXScale */
        1.0f,
        /* MagYScale */
        1.0f,
        /* MagZScale */
        1.0f,
        /* MagXOffset */
        0.0f,
        /* MagYOffset */
        0.0f,
        /* MagZOffset */
        0.0f,
        /* TempOffset */
        25.0f,
        /* TempSensitivity */
        326.8f,
        /* AccUnit */
        9.80665f,
        /* GyroUnit */
        0.0174532f,
        /* MagUnit */
        1.0f
        /* AccDivider */
        2048,
        /* GyroDivider */
        16.4,
        /* MagDivider */
        820.0f
        /* AccScale */
        16,
        /* GyroScale */
        2000,
        /* MagScale */
        0.0012195f,
        /* MagUnit */
        1.9
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
** End of file sed_config.c
**=====================================================================================*/
