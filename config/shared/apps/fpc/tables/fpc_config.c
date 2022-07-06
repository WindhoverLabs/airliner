
/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe_tbl_filedef.h"
#include "fpc_tbldefs.h"
#include "fpc_mission_cfg.h"

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
static OS_USED CFE_TBL_FileDef_t CFE_TBL_FileDef =
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
/* Default FPC config table data */
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
    20.0f,

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
    0.75f,

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
    5.0f,

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
    8.0f,

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
    2.0f,

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
    5.0f,

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
    5.0f,


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
    10.0f,


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
    0.05f,

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
    0.8f,

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
    0.02f,

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
    0.5f,

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
    0.1f,

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
    7.0f,

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
    3.0f,


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
     15.0f,


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
     1.0f,

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
    10.0,

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
    15.0f,

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
    20.0f,

    /**
     * ARSP_MODE
     * Airspeed disabled.
     *
     * @group Fixedwing Position Control
     */
    FALSE,


    /**
     * P_LIM_MIN_RADIANS
     * Negative pitch limit
     *
     * The minimum negative pitch the controller will output.
     *
     * @unit deg
     * @min -60.0
     * @max 0.0
     * @decimal 1
     * @increment 0.5
     * @group FW L1 Control
     */
    DEG_TO_RADIANS(-45.0f),

    /**
     * P_LIM_MAX_RADIANS
     *
     * The maximum positive pitch the controller will output.
     *
     * @unit deg
     * @min 0.0
     * @max 60.0
     * @decimal 1
     * @increment 0.5
     * @group FW L1 Control
     */
    DEG_TO_RADIANS(45.0f),

    /**
     * R_LIM_RADIANS
     * Controller roll limit
     *
     * The maximum roll the controller will output.
     *
     * @unit deg
     * @min 35.0
     * @max 65.0
     * @decimal 1
     * @increment 0.5
     * @group FW L1 Control
     */
    DEG_TO_RADIANS(50.0f),

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
    1.0f,

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
    0.15f,


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
    0.6f,

    /**
     * THR_SLEW_MAX
     *
     * Maximum slew rate for the commanded throttle
     *
     * @min 0.0
     * @max 1.0
     * @group FW L1 Control
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
     * MAN_R_MAX_RADIANS
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
    DEG_TO_RADIANS(45.0f),

    /**
     * MAN_P_MAX_RADIANS
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
    DEG_TO_RADIANS(45.0f),

    /**
     * RSP_OFF_RADIANS
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
    DEG_TO_RADIANS(0.0f),

    /**
     * PSP_OFF_RADIANS
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
    DEG_TO_RADIANS(0.0f),

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
    1.0f,

    /**
     * Landing slope angle in degrees
     *
     * @unit deg
     * @min 1.0
     * @max 15.0
     * @decimal 1
     * @increment 0.5
     * @group FW L1 Control
     */
    DEG_TO_RADIANS(5.0f),

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
    10.0f,

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
    8.0f,

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
    -1.0f,

    /**
      LND_HHDIST
     * Landing heading hold horizontal distance.
     * Set to 0 to disable heading hold.
     *
     * @unit m
     * @min 0
     * @max 30.0
     * @decimal 1
     * @increment 0.5
     * @group FW L1 Control
     */
    15.0f,

    /**
     * Flare, minimum pitch
     *
     * Minimum pitch during flare, a positive sign means nose up
     * Applied once FW_LND_TLALT is reached
     *
     * @unit deg
     * @min 0
     * @max 15.0
     * @decimal 1
     * @increment 0.5
     * @group FW L1 Control
     */
    DEG_TO_RADIANS(2.5f),

    /**
     * LND_FL_PMAX_RADIANS
     * Flare, maximum pitch
     *
     * Maximum pitch during flare, a positive sign means nose up
     * Applied once FW_LND_TLALT is reached
     *
     * @unit deg
     * @min 0
     * @max 45.0
     * @decimal 1
     * @increment 0.5
     * @group FW L1 Control
     */
    DEG_TO_RADIANS(15.0f),

    /**
     * LND_USETER
     * Use terrain estimate during landing
     *
     * @boolean
     * @group FW L1 Control
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
    1.3f,


    {
        /**
         * @file launchdetection_params.c
         *
         * Parameters for launchdetection
         *
         * @author Thomas Gubler <thomasgubler@gmail.com>

        /*
         * Catapult launch detection parameters, accessible via MAVLink
         *

        /**LAUN_ALL_ON
         * Launch detection
         *
         * @boolean
         * @group FW Launch detection
         */
         0,

        /**
         * LAUN_CAT_A
         * Catapult accelerometer threshold.
         *
         * LAUN_CAT_A for LAUN_CAT_T serves as threshold to trigger launch detection.
         *
         * @unit m/s/s
         * @min 0
         * @decimal 1
         * @increment 0.5
         * @group FW Launch detection
         */
         30,

        /**
         * Catapult time threshold.
         * LAUN_CAT_T
         * LAUN_CAT_A for LAUN_CAT_T serves as threshold to trigger launch detection.
         *
         * @unit s
         * @min 0.0
         * @max 5.0
         * @decimal 2
         * @increment 0.05
         * @group FW Launch detection
         */
         0.05f,

        /**
         * Motor delay
         * LAUN_CAT_MDEL
         * Delay between starting attitude control and powering up the throttle (giving throttle control to the controller)
         * Before this timespan is up the throttle will be set to FW_THR_IDLE, set to 0 to deactivate
         *
         * @unit s
         * @min 0.0
         * @max 10.0
         * @decimal 1
         * @increment 0.5
         * @group FW Launch detection
         */
         0,

        /**
         * LAUN_CAT_PMAX_RADIANS
         * Maximum pitch before the throttle is powered up (during motor delay phase)
         *
         * This is an extra limit for the maximum pitch which is imposed in the phase before the throttle turns on.
         * This allows to limit the maximum pitch angle during a bungee launch (make the launch less steep).
         *
         * @unit deg
         * @min 0.0
         * @max 45.0
         * @decimal 1
         * @increment 0.5
         * @group FW Launch detection
         */
        DEG_TO_RADIANS(30)
    },
    {
        /**
         * RWTO_TKOFF
         * Runway takeoff with landing gear
         *
         * @boolean
         * @group Runway Takeoff
         */
        FALSE,

        /**
         * RWTO_HDG
         * Specifies which heading should be held during runnway takeoff.
         *
         * 0: airframe heading, 1: heading towards takeoff waypoint
         *
         * @value 0 Airframe
         * @value 1 Waypoint
         * @min 0
         * @max 1
         * @group Runway Takeoff
         */
        0,
        /**
         * NAV_ALT
         * Altitude AGL at which we have enough ground clearance to allow some roll.
         * Until RWTO_NAV_ALT is reached the plane is held level and only
         * rudder is used to keep the heading (see RWTO_HDG). This should be below
         * FW_CLMBOUT_DIFF if FW_CLMBOUT_DIFF > 0.
         *
         * @unit m
         * @min 0.0
         * @max 100.0
         * @decimal 1
         * @increment 1
         * @group Runway Takeoff
         */
        5.0,
        /**
         * MAX_THR
         * Max throttle during runway takeoff.
         * (Can be used to test taxi on runway)
         *
         * @unit norm
         * @min 0.0
         * @max 1.0
         * @decimal 2
         * @increment 0.01
         * @group Runway Takeoff
         */
        1.0,
        /**
         * PSP
         * Pitch setpoint during taxi / before takeoff airspeed is reached.
         * A taildragger with stearable wheel might need to pitch up
         * a little to keep it's wheel on the ground before airspeed
         * to takeoff is reached.
         *
         * @unit deg
         * @min 0.0
         * @max 20.0
         * @decimal 1
         * @increment 0.5
         * @group Runway Takeoff
         */
        0.0,
        /**
         * MAX_PITCH
         * Max pitch during takeoff.
         * Fixed-wing settings are used if set to 0. Note that there is also a minimum
         * pitch of 10 degrees during takeoff, so this must be larger if set.
         *
         * @unit deg
         * @min 0.0
         * @max 60.0
         * @decimal 1
         * @increment 0.5
         * @group Runway Takeoff
         */
        20.0,
        /**
         * Max roll during climbout.
         * Roll is limited during climbout to ensure enough lift and prevents aggressive
         * navigation before we're on a safe height.
         *
         * @unit deg
         * @min 0.0
         * @max 60.0
         * @decimal 1
         * @increment 0.5
         * @group Runway Takeoff
         */
         25,
        /**
         * AIRSPD_SCL
         * Min. airspeed scaling factor for takeoff.
         * Pitch up will be commanded when the following airspeed is reached:
         * FW_AIRSPD_MIN * RWTO_AIRSPD_SCL
         *
         * @unit norm
         * @min 0.0
         * @max 2.0
         * @decimal 2
         * @increment 0.01
         * @group Runway Takeoff
         */
         1.3
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

