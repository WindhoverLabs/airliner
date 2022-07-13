/*
** Pragmas
*/

/*
** Include Files
*/
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

    "AMC_ConfigTbl", "AMC.CONFIG_TBL", "AMC default config table",
    "amc_config.tbl", (sizeof(AMC_ConfigTbl_t))
};

/*
** External Global Variables
*/

/*
** Global Variables
*/

/* Default config table data */
AMC_ConfigTbl_t AMC_ConfigTbl =
{
    /* Actuator  1 - Right Aileron    */  900, 1100, 2000, AMC_PWM_DISARM_BEHAVIOR_IGNORE, 1500,
    /* Actuator  2 - Elevator         */  900, 1100, 2000, AMC_PWM_DISARM_BEHAVIOR_IGNORE, 1500,
    /* Actuator  3 - Left Throttle    */  900, 1100, 2000, AMC_PWM_DISARM_BEHAVIOR_SAFE,   1500,
    /* Actuator  4 - Rudder/Nose Gear */  900, 1100, 2000, AMC_PWM_DISARM_BEHAVIOR_IGNORE, 1500,
    /* Actuator  5 - Unused           */  900, 1100, 2000, AMC_PWM_DISARM_BEHAVIOR_IGNORE, 1500,
    /* Actuator  6 - Left Aileron     */  900, 1100, 2000, AMC_PWM_DISARM_BEHAVIOR_IGNORE, 1500,
    /* Actuator  7 - Unused           */  900, 1100, 2000, AMC_PWM_DISARM_BEHAVIOR_IGNORE, 1500,
    /* Actuator  8 - Right Throttle   */  900, 1100, 2000, AMC_PWM_DISARM_BEHAVIOR_SAFE,   1500,
    /* Actuator  9 - Left Flap        */  900, 1100, 2000, AMC_PWM_DISARM_BEHAVIOR_IGNORE, 1500,
    /* Actuator 10 - Right Flap       */  900, 1100, 2000, AMC_PWM_DISARM_BEHAVIOR_IGNORE, 1500
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
    
