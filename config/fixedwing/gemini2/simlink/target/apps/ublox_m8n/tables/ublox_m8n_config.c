
/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe_tbl_filedef.h"
#include "ublox_m8n_tbldefs.h"

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Definitions
*************************************************************************/

/**
** \brief The cFE UBLOX_M8N config table definition.
**
** Content format: ObjName[64], TblName[38], Desc[32], TgtFileName[20], ObjSize
**    ObjName - variable name of config table, e.g., UBLOX_M8N_ConfigDefTbl[]
**    TblName - app's table name, e.g., UBLOX_M8N.CONFIG_TBL, where UBLOX_M8N is the same app name
**              used in cfe_es_startup.scr, and UBLOX_M8N_defConfigTbl is the same table
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

    "UBLOX_M8N_ConfigTbl", "UBLOX_M8N.CONFIG_TBL", "UBLOX_M8N default config table",
    "ubm8n_cfg.tbl", sizeof(UBLOX_M8N_ConfigTblEntry_t)
};


/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/

/**
**  \brief Default UBLOX_M8N config table data
*/
UBLOX_M8N_ConfigTblEntry_t UBLOX_M8N_ConfigTbl =
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
    
