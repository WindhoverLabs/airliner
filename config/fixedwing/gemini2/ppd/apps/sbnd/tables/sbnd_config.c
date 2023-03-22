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
        /* 0 */   TO_CPD(AMC_CMD_MID),
        /* 1 */   CF_PPD_TO_CPD_PDU_MID,
        /* 2 */   0,
        /* 3 */   SBND_HK_TLM_MID,
        /* 4 */	  TO_CPD(CFE_ES_CMD_MID),
        /* 5 */	  TO_CPD(CFE_EVS_CMD_MID),
        /* 6 */	  TO_CPD(CFE_SB_CMD_MID),
        /* 7 */	  TO_CPD(CFE_TBL_CMD_MID),
        /* 8 */	  TO_CPD(CFE_TIME_CMD_MID),
        /* 9 */	  TO_CPD(CF_CMD_MID),
        /* 10 */  TO_CPD(CI_CMD_MID),
        /* 11 */  TO_CPD(CS_CMD_MID),
        /* 12 */  TO_CPD(CVT_CMD_MID),
        /* 13 */  0,
        /* 14 */  TO_CPD(FM_CMD_MID),
        /* 15 */  TO_CPD(HK_CMD_MID),
        /* 16 */  TO_CPD(HS_CMD_MID),
        /* 17 */  TO_CPD(LC_CMD_MID),
        /* 18 */  TO_CPD(MD_CMD_MID),
        /* 19 */  TO_CPD(MM_CMD_MID),
        /* 20 */  TO_CPD(SC_CMD_MID),
        /* 21 */  TO_CPD(SCH_CMD_MID),
        /* 22 */  TO_CPD(TO_CMD_MID),
        /* 23 */  TO_CPD(AMC_CMD_MID),
        /* 24 */  TO_CPD(LD_CMD_MID),
        /* 25 */  TO_CPD(FAC_CMD_MID),
        /* 26 */  TO_CPD(FPC_CMD_MID),
        /* 27 */  TO_CPD(NAV_CMD_MID),
        /* 28 */  TO_CPD(PE_CMD_MID),
        /* 29 */  TO_CPD(QAE_CMD_MID),
        /* 30 */  TO_CPD(SBND_CMD_MID),
        /* 31 */  TO_CPD(SENS_CMD_MID),
        /* 32 */  TO_CPD(VM_CMD_MID),
        /* 33 */  TO_CPD(BAT_CMD_MID),
        /* 34 */  TO_CPD(GPS_CMD_MID),
        /* 35 */  TO_CPD(HMC5883_CMD_MID),
        /* 36 */  TO_CPD(ICM20689_CMD_MID),
        /* 37 */  TO_CPD(MS5611_CMD_MID),
        /* 38 */  TO_CPD(RCIN_CMD_MID),
        /* 39 */  TO_CPD(SED_CMD_MID),
        /* 40 */  TO_CPD(VC_CMD_MID),
        /* 41 */  TO_CPD(ZUSUP_CMD_MID),
        /* 42 */  TO_CPD(ASPD4525_CMD_MID)
    },
    /* MaxSentMsgsPerWakeup */
    1,
    /* MaxRecvdMsgsPerWakeup */
    1,
    /* MaxLoopsPerWakeup */
    200
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
    
