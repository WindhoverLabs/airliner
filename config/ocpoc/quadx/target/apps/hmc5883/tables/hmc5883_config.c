/*
** Pragmas
*/

/*
** Include Files
*/
#include "cfe_tbl_filedef.h"
#include "hmc5883_tbldefs.h"

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

    "HMC5883_ConfigTbl", "HMC5883.CONFIG_TBL", "HMC5883 default config table",
    "hmc5883_config.tbl", (sizeof(HMC5883_ConfigTbl_t))
};

/*
** External Global Variables
*/

/*
** Global Variables
*/

/* Default MPU9250 config table data */
HMC5883_ConfigTbl_t HMC5883_ConfigTbl =
{
    /* User calibration params */
    /* x_scale */
    0.999163627624511719f,
    /* y_scale */
    0.990580260753631592f,
    /* z_scale */
    1.010112524032592773f,
    /* x_offset */
    0.191671133041381836f,
    /* y_offset */
    0.131293490529060364f,
    /* z_offset */
    0.001145110232755542f
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
** End of file hmc5883_config.c
**=====================================================================================*/
    
