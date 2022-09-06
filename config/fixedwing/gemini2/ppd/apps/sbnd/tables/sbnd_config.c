/*
** Pragmas
*/

/*
** Include Files
*/
#include "cfe_tbl_filedef.h"
#include "sbnd_tbldefs.h"
#include "msg_ids.h"

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

    "SBND_ConfigTbl", "SBND.CONFIG_TBL", "SBND default config table",
    "sbnd_config.tbl", (sizeof(SBND_ConfigTbl_t))
};

/*
** External Global Variables
*/

/*
** Global Variables
*/

/* Default SBND config table data */
SBND_ConfigTbl_t SBND_ConfigTbl =
{
    {
        /* Mids[0] */
        FROM_CPD(AMC_CMD_MID),
        /* Mids[1] */
	CF_PPD_TO_CPD_PDU_MID,
        /* Mids[2] */
        0,
        /* Mids[3] */
        SBND_HK_TLM_MID,
        /* Mids[4] */
	FROM_CPD(CFE_ES_CMD_MID),
        /* Mids[5] */
	FROM_CPD(CFE_EVS_CMD_MID),
        /* Mids[6] */
	FROM_CPD(CFE_SB_CMD_MID),
        /* Mids[7] */
	FROM_CPD(CFE_TBL_CMD_MID),
        /* Mids[8] */
	FROM_CPD(CFE_ES_CMD_MID),
        /* Mids[9] */
	FROM_CPD(CF_CMD_MID),
        /* Mids[10] */
	FROM_CPD(CI_CMD_MID),
        /* Mids[11] */
	FROM_CPD(CS_CMD_MID),
        /* Mids[12] */
	FROM_CPD(CVT_CMD_MID),
        /* Mids[13] */
	FROM_CPD(DS_CMD_MID),
        /* Mids[14] */
	FROM_CPD(FM_CMD_MID),
        /* Mids[15] */
	FROM_CPD(HK_CMD_MID),
        /* Mids[16] */
	FROM_CPD(HS_CMD_MID),
        /* Mids[17] */
	FROM_CPD(LC_CMD_MID),
        /* Mids[18] */
	FROM_CPD(MD_CMD_MID),
        /* Mids[19] */
	FROM_CPD(MM_CMD_MID),
        /* Mids[20] */
	FROM_CPD(SC_CMD_MID),
        /* Mids[21] */
	FROM_CPD(SCH_CMD_MID),
        /* Mids[22] */
	FROM_CPD(TO_CMD_MID),
        /* Mids[23] */
	FROM_CPD(AMC_CMD_MID),
        /* Mids[24] */
	FROM_CPD(LD_CMD_MID),
        /* Mids[25] */
	FROM_CPD(FAC_CMD_MID),
        /* Mids[26] */
	FROM_CPD(FPC_CMD_MID),
        /* Mids[27] */
	FROM_CPD(NAV_CMD_MID),
        /* Mids[28] */
	FROM_CPD(PE_CMD_MID),
        /* Mids[29] */
	FROM_CPD(QAE_CMD_MID),
        /* Mids[30] */
	FROM_CPD(SBND_CMD_MID),
        /* Mids[31] */
	FROM_CPD(SENS_CMD_MID),
        /* Mids[32] */
	FROM_CPD(VM_CMD_MID),
        /* Mids[33] */
	FROM_CPD(BAT_CMD_MID),
        /* Mids[34] */
	FROM_CPD(GPS_CMD_MID),
        /* Mids[35] */
	FROM_CPD(HMC5883_CMD_MID),
        /* Mids[36] */
	FROM_CPD(ICM20689_CMD_MID),
        /* Mids[37] */
	FROM_CPD(MS5611_CMD_MID),
        /* Mids[38] */
	FROM_CPD(RCIN_CMD_MID),
        /* Mids[39] */
	FROM_CPD(SED_CMD_MID),
        /* Mids[40] */
	FROM_CPD(VC_CMD_MID),
        /* Mids[41] */
	FROM_CPD(ZUSUP_CMD_MID),
        /* Mids[42] */
	FROM_CPD(ASPD4525_CMD_MID)
    },
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
** End of file sbnd_config.c
**=====================================================================================*/
    
