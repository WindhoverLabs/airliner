/*
** Pragmas
*/

/*
** Include Files
*/
#include "cfe_tbl_filedef.h"
#include "mfa_tbldefs.h"
#include "mfa_config.h"

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

    "MFA_ConfigTbl", "MFA.CONFIG_TBL", "MFA default config table",
    "mfa_config.tbl", sizeof(MFA_ConfigTbl_t)
};

/*
** External Global Variables
*/

/*
** Global Variables
*/

/* Default CI config table data */
MFA_ConfigTbl_t MFA_ConfigTbl =
{
    0.0,
    1.0,
    -50.0,
    150.0,
    8.3144598,
    9.80665,
    0.0289644,
    {0.0, 11000.0, 20000.0, 32000.0, 47000.0, 51000.0, 71000.0},
    {1.2250, 0.36391, 0.08803, 0.01322, 0.00143, 0.00086, 0.000064},
    {288.15, 216.65, 216.65, 228.65, 270.65, 270.65, 214.65},
    {-0.0065, 0.0, 0.001, 0.0028, 0.0, -0.0028, -0.002},
    {1,2,1,1,2,1,1},
    MFA_CONFIG_AIRDENSITY_ALTITUDE_MODE
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
** End of file ci_config.c
**=====================================================================================*/

