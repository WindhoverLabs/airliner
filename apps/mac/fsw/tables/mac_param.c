/*
** Pragmas
*/

/*
** Include Files
*/
//#include <mixer/MixerTables.h>
#include "cfe_tbl_filedef.h"
#include "mac_tbldefs.h"

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

    "MAC_ParamTbl", "MAC.PARAM_TBL", "MAC default param table",
    "mac_param.tbl", (sizeof(MAC_ParamTbl_t))
};

/*
** External Global Variables
*/

/*
** Global Variables
*/

/* Default MAC config table data */
MAC_ParamTbl_t MAC_ParamTbl =
{
    5.0,	/* MC_ROLL_P 	    */
    0.1,	/* MC_ROLLRATE_P    */
    0.05,	/* MC_ROLLRATE_I    */
    0.30,	/* MC_RR_INT_LIM    */
    0.001,	/* MC_ROLLRATE_D    */
    0.0,	/* MC_ROLLRATE_FF   */
    5.0,	/* MC_PITCH_P       */
    0.1,	/* MC_PITCHRATE_P   */
    0.05,	/* MC_PITCHRATE_I   */
    0.30,	/* MC_PR_INT_LIM    */
    0.001,	/* MC_PITCHRATE_D   */
    0.0,	/* MC_PITCHRATE_FF  */
    1.0,	/* MC_TPA_BREAK_P   */
    1.0,	/* MC_TPA_BREAK_I   */
    1.0,	/* MC_TPA_BREAK_D   */
    0.0,	/* MC_TPA_RATE_P    */
    0.0,	/* MC_TPA_RATE_I    */
    0.0,	/* MC_TPA_RATE_D    */
    2.8,	/* MC_YAW_P         */
    0.08,	/* MC_YAWRATE_P	    */
    0.1,	/* MC_YAWRATE_I     */
    0.30,       /* MC_YR_INT_LIM    */
    0.0,	/* MC_YAWRATE_D     */
    0.0,	/* MC_YAWRATE_FF    */
    220.0,	/* MC_ROLLRATE_MAX  */
    220.0,	/* MC_PITCHRATE_MAX */
    200.0,	/* MC_YAWRATE_MAX   */
    45.0,       /* MC_YAWRAUTO_MAX  */
    360.0,      /* MC_ACRO_R_MAX    */
    360.0,      /* MC_ACRO_P_MAX    */
    360.0,	/* MC_ACRO_Y_MAX    */
    0.8,	/* MC_RATT_TH	    */
    0.2,	/* MC_ROLL_TC	    */
    0.2,	/* MC_PITCH_TC      */
    0,		/* MC_BAT_SCALE_EN  */
    0,		/* SENS_BOARD_ROT   */
    0.0,	/* SENS_BOARD_X_OFF */
    0.0,	/* SENS_BOARD_Y_OFF */
    0.0 	/* SENS_BOARD_Z_OFF */
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
** End of file mac_param.c
**=====================================================================================*/
    
