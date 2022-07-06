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
	    /** \brief Fixed Wing max climb rate.
	     *
	     *  \par Limits: [5, 20)]
	     *  	default 10.0.
	     */
	    10.0f,

	    /** \brief Fixed Wing max horizontal velocity.
	     *
	     *  \par Limits: [0.5, 10)
	     *  	default 5.0.
	     */
	    5.0f,

	    /** \brief Multicopter maximum altitude.
	     *
	     *  \par Limits:
	     *  	default 10000.0.
	     */
	    10000.0,

	    /** \brief Multicopter low throttle threshold.
	     *
	     *  \par Limits:
	     *  	default 0.3.
	     */
	    0.35,

	    /** \brief Multicopter minimum throttle in manual mode.
	     *
	     *  \par Limits:
	     *  	default 0.08.
	     */
	    0.08,

	    /** \brief Multicopter takeoff stick up threshold in position control mode.
	     *
	     *  \par Limits:
	     *  	default 0.65.
	     */
	    0.65,

	    /** \brief Multicopter takeoff stick down threshold in position control mode.
	     *
	     *  \par Limits:
	     *  	default 0.15.
	     */
	    0.15,

        /** \brief Landing descend rate.
         *
         *  \par Limits:
         *      default 0.5.
         */
        0.5f,

		/**
		 * @brief Airspeed max
		 * 
		 */
		8.0f,

        /** \brief Min Thust No Altitude Lock Land Timout.
         *
         *  \par Limits:
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
