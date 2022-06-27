
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
//Insert error since for now we will be using the table from config/shared
Error

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
FPC_ConfigTbl_t FPC_ConfigTbl =
{
    /**
     * L1_PERIOD
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * L1_DAMPING
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * T_TIME_CONST
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * T_THRO_CONST
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * T_SINK_MIN
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * T_SINK_MAX
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * T_CLMB_MAX
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,


    /**
     * CLMBOUT_DIFF
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,


    /**
     * T_HRATE_P
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * T_HRATE_FF
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * T_SRATE_P
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * T_THR_DAMP
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * T_INTEG_GAIN
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * T_VERT_ACC
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * T_HGT_OMEGA
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * T_SPD_OMEGA
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,


    /**
     * T_RLL2THR
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,


    /**
     * T_SPDWEIGHT
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * T_PTCH_DAMP
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * AIRSPD_MIN
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    5.0,

    /**
     * AIRSPD_TRIM
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    10.0f,

    /**
     * AIRSPD_MAX
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    10.0f,

    /**
     * ARSP_MODE
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0,


    /**
     * P_LIM_MIN
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * P_LIM_MAX
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * R_LIM
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * THR_MIN
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * THR_MAX
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * THR_IDLE
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,


    /**
     * THR_CRUISE
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * THR_SLEW_MAX
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * THR_ALT_SCL
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * MAN_R_MAX
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * MAN_P_MAX
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * RSP_OFF
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * PSP_OFF
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * THR_LND_MAX
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * LND_ANG
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * LND_HVIRT
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * LND_FLALT
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * LND_TLALT
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * LND_HHDIST
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * LND_FL_PMIN
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * LND_FL_PMAX
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * LND_USETER
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * LND_AIRSPD_SC
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

   .FPC_Launch_Detection =
    {
        .LAUN_ALL_ON = {FALSE},

    }
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
    
