/*
** Pragmas
*/

/*
** Include Files
*/
#include "cfe_tbl_filedef.h"
#include "qae_tbldefs.h"

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

    "QAE_ConfigTbl", "QAE.CONFIG_TBL", "QAE default config table",
    "qae_config.tbl", (sizeof(QAE_ConfigTbl_t))
};

/*
** External Global Variables
*/

/*
** Global Variables
*/

/* Default QAE config table data */
QAE_ConfigTbl_t QAE_ConfigTbl =
{
    /** \brief Complimentary filter accelerometer weight.
     * 
     *  \par Limits:
     *       Min > Max (incr.) 0 > 1, default 0.2.
     */
    0.2f,
    /** \brief Complimentary filter magnetometer weight.
     * 
     *  \par Limits:
     *       Min > Max (incr.) 0 > 1, default 0.1.
     */
    0.1f,
    /** \brief Complimentary filter gyroscope bias weight.
     * 
     *  \par Limits:
     *       Min > Max (incr.) 0 > 1, default 0.1.
     */
    0.1f,
    /** \brief Magnetic declination, in degrees.
     * 
     *  \par Limits:
     *       Min > Max (incr.) 0 > 180, default 0.0.
     */
    0.0f,
    /** \brief Automatic GPS based declination compensation.
     * 
     *  \par Limits:
     *       TRUE or FALSE, default 1 (TRUE).
     */
    1,
    /** \brief Acceleration compensation based on GPS velocity.
     * 
     *  \par Limits:
     *       TRUE or FALSE, default 1 (TRUE).
     */
    1,
    /** \brief Gyro bias limit in rad/s.
     * 
     *  \par Limits:
     *       Min > Max (incr.) 0 > 2, default 0.05.
     */
    0.05f
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
** End of file qae_config.c
**=====================================================================================*/
    
