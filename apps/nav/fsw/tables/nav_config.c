/*
** Pragmas
*/

/*
** Include Files
*/
#include "cfe_tbl_filedef.h"
#include "nav_tbldefs.h"

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

    "NAV_ConfigTbl", "NAV.CONFIG_TBL", "NAV default config table",
    "nav_config.tbl", (sizeof(NAV_ConfigTbl_t))
};

/*
** External Global Variables
*/

/*
** Global Variables
*/

/* Default NAV config table data */
NAV_ConfigTbl_t NAV_ConfigTbl =
{
	/** \brief Default Accepted Radius.
	 *
	 *  \par Limits:
	 *       Min > Max (incr.) 0.05 > 200.0 , default 2.0.
	 */
	2.0,

	/** \brief Altitude Acceptance Radius.
	 *
	 *  \par Limits:
	 *       Min > Max (incr.) 0.05 > 200 , default 0.8.
	 */
	0.8,

	/** \brief The Loiter Radius.
	 *
	 *  \par Limits:
	 *       Min > Max (incr.) 25 > 1000 , default 50.0.
	 */
	50.0,

	/** \brief Take-off Altitude.
	 *
	 *  \par Limits:
	 *       Min > Max (incr.) 0 > 80 , default 2.5.
	 */
	2.5,

	/** \brief Max yaw error in degrees needed for waypoint heading acceptance.
	 *
	 *  \par Limits:
	 *       Min > Max (incr.) 0 > 90 , default 12.0.
	 */
	12.0,

	/** \brief Time in seconds we wait on reaching target heading at a waypoint if it is forced.
	 *
	 *  \par Limits:
	 *       Min > Max (incr.) -1 > 20 , default -1.0.
	 */
	-1.0,

	/** \brief Minimum Loiter Altitude.
	 *
	 *  \par Limits:
	 *       Min > Max (incr.) -1 > 80 , default 1.20.
	 */
	1.20,

	/** \brief RTL Altitude.
	 *
	 *  \par Limits:
	 *       Min > Max (incr.) 0 > 150 , default 30.0.
	 */
	10.0,

	/** \brief RTL Loiter Altitude.
	 *
	 *  \par Limits:
	 *       Min > Max (incr.) 2 > 100 , default 10.0.
	 */
	5.0,

	/** \brief RTL Delay.
	 *
	 *  \par Limits:
	 *       Min > Max (incr.) -1 > 300 , default 0.0.
	 */
	0.0,

	/** \brief Minimum distance to trigger rising to a safe altitude.
	 *
	 *  \par Limits:
	 *       Min > Max (incr.) 0.5 > 20 , default 5.0.
	 */
	5.0
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
** End of file nav_config.c
**=====================================================================================*/
    
