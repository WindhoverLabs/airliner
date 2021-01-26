/*
** Pragmas
*/

/*
** Include Files
*/
#include "cfe_tbl_filedef.h"
#include "flow_tbldefs.h"

/*
** Local Defines
*/

/*
** Local Structure Declarations
*/
static CFE_TBL_FileDef_t CFE_TBL_FileDef =
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

    "FLOW_ConfigTbl", "FLOW.CONFIG_TBL", "FLOW default config table",
    "flow_config.tbl", (sizeof(FLOW_ConfigTbl_t))
};

/*
** External Global Variables
*/

/*
** Global Variables
*/

/* Default FLOW config table data */
FLOW_ConfigTbl_t FLOW_ConfigTbl =
{
		/** \brief Horizontal field of view.
		 *
		 *  \par Limits:
		 *       default 0.088.
		 */
		0.088f,
		/** \brief Size of sliding window.
		 *
		 *  \par Limits:
		 *       Min > Max (incr.) 2 > 10, default 8.
		 */
		8,
		/** \brief Number of blocks of sliding window.
		 *
		 *  \par Limits:
		 *       Min > Max (incr.) 2 > 10, default 5.
		 */
		5,
		/** \brief Default image output rate.
		 *
		 *  \par Limits:
		 *       default 15.
		 */
		10,
		/** \brief Default image width.
		 *
		 *  \par Limits:
		 *       default 64.
		 */
		64,
		/** \brief Default image height.
		 *
		 *  \par Limits:
		 *       default 64.
		 */
		64,
		/** \brief Default image search size.
		 *
		 *  \par Limits:
		 *       default 6.
		 */
		6,
		/** \brief Default feature threshold.
		 *
		 *  \par Limits:
		 *       default 30.
		 */
		30,
		/** \brief Default value threshold.
		 *
		 *  \par Limits:
		 *       default 3000.
		 */
		3000,
		/** \brief Mean count threshold.
		 *
		 *  \par Limits:
		 *       default 10.
		 */
		10
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
** End of file flow_config.c
**=====================================================================================*/
    
