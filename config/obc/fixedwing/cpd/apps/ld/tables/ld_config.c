/*
** Pragmas
*/

/*
** Include Files
*/
#include "cfe_tbl_filedef.h"
#include "ld_tbldefs.h"

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

    "LD_ConfigTbl", "LD.CONFIG_TBL", "LD default config table",
    "ld_config.tbl", (sizeof(LD_ConfigTbl_t))
};

/*
** External Global Variables
*/

/*
** Global Variables
*/

/* Default LD config table data */
LD_ConfigTbl_t LD_ConfigTbl =
{
		/** \ldcfg LD_Z_VEL_MAX
		 *
		 *  \brief Fixed Wing max climb rate.
	     *
	     *  \par Limits: [5, 20)]
	     *  	default 10.0.
	     */
	    10.0f,

		/** \ldcfg LD_XY_VEL_MAX
		 *
		 *  \brief Fixed Wing max horizontal velocity.
	     *
	     *  \par Limits: [0.5, 10)
	     *  	default 5.0.
	     */
	    2.0f,

		/** \ldcfg LD_ALT_MAX
		 *
		 *  \brief Fixed Wing maximum altitude.
	     *
	     *  \par Limits: [-1.0, 10000.0)
	     *  	default 10000.0.
	     */
	    10000.0,

		/** \ldcfg LD_LOW_T_THR
		 *
		 *  \brief Fixed Wing low throttle threshold.
	     *
	     *  \par Limits: [0.1, 0.9)
	     *  	default 0.3.
	     */
	    0.35,

		/** \ldcfg LD_MAN_MIN_THR
		 *
		 *  \brief Fixed Wing minimum throttle in manual mode.
	     *
	     *  \par Limits: [0.0, 1.0)
	     *  	default 0.08.
	     */
	    0.08,

		/** \ldcfg LD_POS_STK_UP_THRES
		 *
		 *  \brief Fixed Wing takeoff stick up threshold in position control mode.
	     *
	     *  \par Limits: [0.5, 1.0)
	     *  	default 0.65.
	     */
	    0.65,

		/** \ldcfg LD_POS_STK_DW_THRES
		 *
		 *  \brief Fixed Wing takeoff stick down threshold in position control mode.
	     *
	     *  \par Limits: [0.0, 0.5)
	     *  	default 0.15.
	     */
	    0.15,

		/** \ldcfg LD_LANDSPEED
		 *
		 *  \brief Landing descend rate.
         *
         *  \par Limits: [0.0, 10.0)
         *      default 0.5.
         */
        0.5f,

		/**
		 * \ldcfg LD_LNDFW_AIRSPD_MAX Airspeed max
		 *
		 * \brief Maximum airspeed allowed in the landed state (m/s)
		 *
		 * @unit m/s
		 * @min 4
		 * @max 20
		 * @decimal 1
		 *
		 * @group Land Detector
		 */
		8.0f,

		/** \ldcfg LD_MIN_THR_NO_ALT_TIMEOUT
		 *
		 *  \brief Min Thust No Altitude Lock Land Timout.
         *
         *  \par Limits: [1, 9000000)
         *      default 8000000.
         */
        8000000
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
** End of file ld_config.c
**=====================================================================================*/
