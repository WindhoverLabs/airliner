/*
** Pragmas
*/

/*
** Include Files
*/
#include <mixer/MixerTables.h>
#include "cfe_tbl_filedef.h"
#include "amc_tbldefs.h"

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

    "AMC_MixerCfgTbl", "AMC.MIXERCFG_TBL", "AMC default mixer config table",
    "amc_mixercfg.tbl", (sizeof(MultirotorMixer_ConfigTable_t))
};


/*
** External Global Variables
*/

/*
** Global Variables
*/

/* Default CI config table data */
MultirotorMixer_ConfigTable_t AMC_MixerCfgTbl =
{
    1.0,          /* Roll Scale     */
    1.0,          /* Pitch Scale    */
    1.0,          /* Yaw Scale      */
    0.0,          /* Idle Speed     */
    1.0,          /* Delta Out Max  */
    4,            /* Rotor Count    */
    MIXER_QUAD_X, /* Rotor Geometry */
    {
        /* Rotor # | Roll Scale  |  Pitch Scale  |  Yaw Scale  |  Output Scale */
        /*  0  */  { -0.707107,     0.707107,       1.000000,     1.000000 },
        /*  1  */  {  0.707107,    -0.707107,       1.000000,     1.000000 },
        /*  2  */  {  0.707107,     0.707107,      -1.000000,     1.000000 },
        /*  3  */  { -0.707107,    -0.707107,      -1.000000,     1.000000 }
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
** End of file ci_config.c
**=====================================================================================*/
    
