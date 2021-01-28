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

/* Default AK8963 config table data */
AK8963_ConfigTbl_t AK8963_ConfigTbl =
{
        /* User calibration params */
        /* MagXScale */
        0.868795096874237061f,
        /* MagYScale */
        1.071108341217041016,
        /* MagZScale */
        1.129628062248229980f,
        /* MagXOffset */
        -0.026660522446036339f,
        /* MagYOffset */
        -0.064023584127426147f,
        /* MagZOffset */
        0.521035254001617432f
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
    
