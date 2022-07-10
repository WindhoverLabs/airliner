
/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe_tbl_filedef.h"
#include "ms5611_tbldefs.h"

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Definitions
*************************************************************************/

/**
** \brief The cFE MS5611 config table definition.
**
** Content format: ObjName[64], TblName[38], Desc[32], TgtFileName[20], ObjSize
**    ObjName - variable name of config table, e.g., MS5611_ConfigDefTbl[]
**    TblName - app's table name, e.g., MS5611.CONFIG_TBL, where MS5611 is the same app name
**              used in cfe_es_startup.scr, and MS5611_defConfigTbl is the same table
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

    "MS5611_ConfigTbl", "MS5611.CONFIG_TBL", "MS5611 default config table",
    "ms5611_cfg.tbl", sizeof(MS5611_ConfigTblEntry_t)
};


/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/

/**
**  \brief Default MS5611 config table data
*/
MS5611_ConfigTblEntry_t MS5611_ConfigTbl =
{
    123
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
    
