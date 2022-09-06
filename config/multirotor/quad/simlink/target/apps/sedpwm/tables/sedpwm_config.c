
/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe_tbl_filedef.h"
#include "sedpwm_tbldefs.h"

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Definitions
*************************************************************************/

/**
** \brief The cFE SEDPWM config table definition.
**
** Content format: ObjName[64], TblName[38], Desc[32], TgtFileName[20], ObjSize
**    ObjName - variable name of config table, e.g., SEDPWM_ConfigDefTbl[]
**    TblName - app's table name, e.g., SEDPWM.CONFIG_TBL, where SEDPWM is the same app name
**              used in cfe_es_startup.scr, and SEDPWM_defConfigTbl is the same table
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

    "SEDPWM_ConfigTbl", "SEDPWM.CONFIG_TBL", "SEDPWM default config table",
    "sedpwm_cfg.tbl", sizeof(SEDPWM_ConfigTblEntry_t)
};


/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/

/**
**  \brief Default SEDPWM config table data
*/
SEDPWM_ConfigTblEntry_t SEDPWM_ConfigTbl =
{
	/* ACC_X_SCALE     */         1.0f,
	/* ACC_Y_SCALE     */         1.0f,
	/* ACC_Z_SCALE     */         1.0f,
	/* ACC_X_OFFSET    */         0.0f,
	/* ACC_Y_OFFSET    */         0.0f,
	/* ACC_Z_OFFSET    */         0.0f,
	/* ACC_UNIT        */         9.80665f,
	/* ACC_DIVIDER     */         2048,
	/* GYRO_X_SCALE    */         1.0f,
	/* GYRO_Y_SCALE    */         1.0f,
	/* GYRO_Z_SCALE    */         1.0f,
	/* GYRO_X_OFFSET   */         0.0f,
	/* GYRO_Y_OFFSET   */         0.0f,
	/* GYRO_Z_OFFSET   */         0.0f,
	/* GYRO_UNIT       */         0.0174532f,
	/* GYRO_DIVIDER    */         16.4f,
	/* MAG_X_SCALE     */         1.0f,
	/* MAG_Y_SCALE     */         1.0f,
	/* MAG_Z_SCALE     */         1.0f,
	/* MAG_X_OFFSET    */         0.0f,
	/* MAG_Y_OFFSET    */         0.0f,
	/* MAG_Z_OFFSET    */         0.0f,
	/* MAG_UNIT        */         1.0f,
	/* MAG_DIVIDER     */         820.0f
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
    
