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
        CF_SPACE_TO_GND_PDU_MID,
        /* Mids[2] */
        CF_INCOMING_PDU_MID,
        /* Mids[3] */
        SBND_HK_TLM_MID,
        /* Mids[4] */
        0x0,
        /* Mids[5] */
        0x0,
        /* Mids[6] */
        0x0,
        /* Mids[7] */
        0x0,
        /* Mids[8] */
        0x0,
        /* Mids[9] */
        0x0,
        /* Mids[10] */
        0x0,
        /* Mids[11] */
        0x0,
        /* Mids[12] */
        0x0,
        /* Mids[13] */
        0x0,
        /* Mids[14] */
        0x0,
        /* Mids[15] */
        0x0,
        /* Mids[16] */
        0x0,
        /* Mids[17] */
        0x0,
        /* Mids[18] */
        0x0,
        /* Mids[19] */
        0x0,
        /* Mids[20] */
        0x0,
        /* Mids[21] */
        0x0,
        /* Mids[22] */
        0x0,
        /* Mids[23] */
        0x0,
        /* Mids[24] */
        0x0,
        /* Mids[25] */
        0x0,
        /* Mids[26] */
        0x0,
        /* Mids[27] */
        0x0,
        /* Mids[28] */
        0x0,
        /* Mids[29] */
        0x0,
        /* Mids[30] */
        0x0,
        /* Mids[31] */
        0x0,
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
    
