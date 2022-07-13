/*
** Pragmas
*/

/*
** Include Files
*/
#include "cfe_tbl_filedef.h"
#include "sens_tbldefs.h"

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

    "SENS_ConfigTbl", "SENS.CONFIG_TBL", "SENS default config table",
    "sens_config.tbl", (sizeof(SENS_ConfigTbl_t))
};

/*
** External Global Variables
*/

/*
** Global Variables
*/

/* Default SENS config table data */
SENS_ConfigTbl_t SENS_ConfigTbl =
{
    /* Values here based on: 
    * https://docs.google.com/spreadsheets/d/1paD_elfTrYbCI3wqHR-x-OO1nnJUPQNuIw-M6-BSDVA/edit?usp=sharing
    *
    * Expected RC mapping:
    * CH 1: Roll
    * CH 2: Pitch
    * CH 3: Throttle
    * CH 4: Yaw
    */
   
    /* RC%d_MIN  (0..17)    */
								982.0f, // Throttle min
								982.0f, // Roll min
								982.0f, // Pitch min
								982.0f, // Yaw min
		                        982.0f,
		                        982.0f,
		                        982.0f,
		                        982.0f,
								982.0f,
								982.0f,
								982.0f,
								982.0f,
								982.0f,
								982.0f,
								982.0f,
								982.0f,
								982.0f,
								982.0f,
    /* RC%d_TRIM (0..17)    */
								982.0f,  // Throttle trim - set this equal to min
								1493.0f, // Roll trim
								1493.0f, // Pitch trim
								1493.0f, // Yaw trim
		                        1493.0f,
		                        1493.0f,
		                        1493.0f,
		                        1493.0f,
								1493.0f,
								1493.0f,
								1493.0f,
								1493.0f,
								1493.0f,
								1493.0f,
								1493.0f,
								1493.0f,
								1493.0f,
								1493.0f,
	/* RC%d_MAX  (0..17)    */
								2006.0f, // Throttle max
								2006.0f, // Roll max
								2006.0f, // Pitch max
								2006.0f, // Yaw max
		                        2006.0f,
		                        2006.0f,
		                        2006.0f,
		                        2006.0f,
								2006.0f,
								2006.0f,
								2006.0f,
								2006.0f,
								2006.0f,
								2006.0f,
								2006.0f,
								2006.0f,
								2006.0f,
								2006.0f,
	/* RC%d_REV  (0..17)    */
								1.0f,
								1.0f,
								1.0f,
								1.0f,
								1.0f,
								1.0f,
								1.0f,
								1.0f,
								1.0f,
								1.0f,
								1.0f,
								1.0f,
								1.0f,
								1.0f,
								1.0f,
								1.0f,
								1.0f,
								1.0f,
	/* RC%d_DZ   (0..17)    */
								10.0f, 
								10.0f, 
								10.0f, 
								10.0f,
								10.0f,
								10.0f,
								10.0f,
								10.0f,
								10.0f,
								10.0f,
								10.0f,
								10.0f,
								10.0f,
								10.0f,
								10.0f,
								10.0f,
								10.0f,
								10.0f,

	/* RC_MAP_ROLL          */  2,
	/* RC_MAP_PITCH         */  3,
	/* RC_MAP_YAW           */  4,
	/* RC_MAP_THROTTLE      */  1,
	/* RC_MAP_FAILSAFE      */  0,

	/* RC_MAP_MODE_SW       */  8,
	/* RC_MAP_RETURN_SW     */  12,
	/* RC_MAP_RATT_SW       */  10,
	/* RC_MAP_POSCTL_SW     */  0,
	/* RC_MAP_LOITER_SW     */  0,
	/* RC_MAP_ACRO_SW       */  0,
	/* RC_MAP_OFFB_SW       */  0,
	/* RC_MAP_KILL_SW       */  7,
	/* RC_MAP_ARM_SW        */  6,
	/* RC_MAP_TRANS_SW      */  11,
	/* RC_MAP_GEAR_SW       */  5,
	/* RC_MAP_STAB_SW       */  0,
	/* RC_MAP_MAN_SW        */  0,
	/* RC_MAP_FLAPS         */  0,
	/* RC_MAP_ALTCTL_SW     */  0,

	/* RC_MAP_AUX1          */  0,
	/* RC_MAP_AUX2          */  0,
	/* RC_MAP_AUX3          */  0,
	/* RC_MAP_AUX4          */  0,
	/* RC_MAP_AUX5          */  0,

	/* RC_MAP_PARAM%d (1)   */  0,
	/* RC_MAP_PARAM%d (2)   */  0,
	/* RC_MAP_PARAM%d (3)   */  0,

	/* RC_MAP_FLTMODE       */  0,

	/* RC_FAILS_THR         */  0,
	/* RC_ASSIST_TH         */  0.2313f,
	/* RC_AUTO_TH           */  0.9770f,
	/* RC_RATT_TH           */  0.5f,
	/* RC_POSCTL_TH         */  0.6185f,
	/* RC_RETURN_TH         */  0.5f,
	/* RC_LOITER_TH         */  0.5f,
	/* RC_ACRO_TH           */  0.5f,
	/* RC_OFFB_TH           */  0.5f,
	/* RC_KILLSWITCH_TH     */  0.5f,
	/* RC_ARMSWITCH_TH      */  0.5f,
	/* RC_TRANS_TH          */  0.25f,
	/* RC_GEAR_TH           */  0.25f,
	/* RC_STAB_TH           */  0.5f,
	/* RC_MAN_TH            */  0.5f,
	/* RC_ALTCTL_TH         */  0.5f,

	/* RC_FLT_SMP_RATE      */  50.0f,
	/* RC_FLT_CUTOFF        */  10.0f,

	/* ModeSwitchControl    */  1
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
** End of file sens_config.c
**=====================================================================================*/
