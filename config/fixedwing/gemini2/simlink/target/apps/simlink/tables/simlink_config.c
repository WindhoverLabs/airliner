
/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe_tbl_filedef.h"
#include "simlink_tbldefs.h"

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Definitions
*************************************************************************/

/**
** \brief The cFE SIMLINK config table definition.
**
** Content format: ObjName[64], TblName[38], Desc[32], TgtFileName[20], ObjSize
**    ObjName - variable name of config table, e.g., SIMLINK_ConfigDefTbl[]
**    TblName - app's table name, e.g., SIMLINK.CONFIG_TBL, where SIMLINK is the same app name
**              used in cfe_es_startup.scr, and SIMLINK_defConfigTbl is the same table
**              name passed in to CFE_TBL_Register()
**    Desc - description of table in string format
**    TgtFileName[20] - table file name, compiled as .tbl file extension
**    ObjSize - size of the entire table
**
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

    "SIMLINK_ConfigTbl", "SIMLINK.CONFIG_TBL", "SIMLINK default config table",
    "simlink_cfg.tbl", sizeof(SIMLINK_ConfigTbl_t)
};


/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/

/**
**  \brief Default SIMLINK config table data
*/
SIMLINK_ConfigTbl_t SIMLINK_ConfigTbl =
{
	/*  0 */ -1.0f,  1.0f,
	/*  1 */ -1.0f,  1.0f,
	/*  2 */  0.0f,  1.0f,
	/*  3 */ -1.0f,  1.0f,
	/*  4 */  0.0f,  0.0f,
	/*  5 */ -1.0f,  1.0f,
	/*  6 */ -1.0f,  1.0f,
	/*  7 */  0.0f,  0.0f,
	/*  8 */  0.0f,  0.0f,
	/*  9 */  0.0f,  0.0f,
	/* 10 */  0.0f,  0.0f,
	/* 11 */  0.0f,  0.0f,
	/* 12 */  0.0f,  0.0f,
	/* 13 */  0.0f,  0.0f,
	/* 14 */  0.0f,  0.0f,
	/* 15 */  0.0f,  0.0f
};

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Prototypes
*************************************************************************/

/************************************************************************
** Function Definitions
*************************************************************************/

/************************/
/*  End of File Comment */
/************************/
    
