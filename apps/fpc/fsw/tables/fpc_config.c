
/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe_tbl_filedef.h"
#include "fpc_tbldefs.h"

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Definitions
*************************************************************************/
#define FPC_CONFIG_TABLENAME ("CONFIG_TBL")
/**
** \brief The cFE FPC config table definition.
**
** Content format: ObjName[64], TblName[38], Desc[32], TgtFileName[20], ObjSize
**    ObjName - variable name of config table, e.g., FPC_ConfigDefTbl[]
**    TblName - app's table name, e.g., FPC.CONFIG_TBL, where FPC is the same app name
**              used in cfe_es_startup.scr, and FPC_defConfigTbl is the same table
**              name passed in to CFE_TBL_Register()
**    Desc - description of table in string format
**    TgtFileName[20] - table file name, compiled as .tbl file extension
**    ObjSize - size of the entire table
**
*/
static CFE_TBL_FileDef_t CFE_TBL_FileDef =
{
    "FPC_ConfigTbl", "FPC.CONFIG_TBL", "FPC default config table",
    "fpc_config.tbl", (sizeof(FPC_ConfigTbl_t))
};

/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/

/**
**  \brief Default FPC config table data
*/
/* Default MPC config table data */
FPC_ConfigTbl_t FPC_ConfigTbl;


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
    
