/*
** Pragmas
*/

/*
** Include Files
*/
#include "cfe_tbl_filedef.h"
#include "ak8963_tbldefs.h"

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

    "AK8963_ConfigTbl", "AK8963.CONFIG_TBL", "AK8963 default config table",
    "ak8963_config.tbl", (sizeof(AK8963_ConfigTbl_t))
};

/*
** External Global Variables
*/

/*
** Global Variables
*/

/* Default MPU9250 config table data */
AK8963_ConfigTbl_t AK8963_ConfigTbl =
{
    /* User calibration params */
    /* x_scale */
    1.0f,
    /* y_scale */
    1.0f,
    /* z_scale */
    1.0f,
    /* x_offset */
    0.0f,
    /* y_offset */
    0.0f,
    /* z_offset */
    0.0f
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
** End of file ak8963_config.c
**=====================================================================================*/
    
