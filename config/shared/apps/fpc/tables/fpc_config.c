/*
** Pragmas
*/

/*
** Include Files
*/
#include "cfe_tbl_filedef.h"
#include "fpc_tbldefs.h"

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

    "FPC_ConfigTbl", "FPC.CONFIG_TBL", "FPC default config table",
    "fpc_config.tbl", (sizeof(FPC_ConfigTbl_t))
};

/*
** External Global Variables
*/

/*
** Global Variables
*/

/* Default FPC config table data */
FPC_ConfigTbl_t FPC_ConfigTbl =
{
	/**
         * FPC_THR_MIN
	 * Minimum thrust in auto thrust control
	 *
	 * It's recommended to set it > 0 to avoid free fall with zero thrust.
	 *
	 * @unit norm
	 * @min 0.05
	 * @max 1.0
	 * @decimal 2
	 * @increment 0.01
	 * @group Multicopter Position Control
	 */
	0.12f,

	/**
         * FPC_THR_HOVER
	 * Hover thrust
	 *
	 * Vertical thrust required to hover.
	 * This value is mapped to center stick for manual throttle control.
	 * With this value set to the thrust required to hover, transition
	 * from manual to ALTCTL mode while hovering will occur with the
	 * throttle stick near center, which is then interpreted as (near)
	 * zero demand for vertical speed.
	 *
	 * @unit norm
	 * @min 0.2
	 * @max 0.8
	 * @decimal 2
	 * @increment 0.01
	 * @group Multicopter Position Control
	 */
	0.5f,

	/**
         * FPC_THR_MAX
	 * Maximum thrust in auto thrust control
	 *
	 * Limit max allowed thrust. Setting a value of one can put
	 * the system into actuator saturation as no spread between
	 * the motors is possible any more. A value of 0.8 - 0.9
	 * is recommended.
	 *
	 * @unit norm
	 * @min 0.0
	 * @max 0.95
	 * @decimal 2
	 * @increment 0.01
	 * @group Multicopter Position Control
	 */
	0.9f,

	/**
         * FPC_MANTHR_MIN
	 * Minimum manual thrust
	 *
	 * Minimum vertical thrust. It's recommended to set it > 0 to avoid free fall with zero thrust.
	 *
	 * @unit norm
	 * @min 0.0
	 * @max 1.0
	 * @decimal 2
	 * @increment 0.01
	 * @group Multicopter Position Control
	 */
	0.08f,

	/**
         * FPC_MANTHR_MAX
	 * Maximum manual thrust
	 *
	 * Limit max allowed thrust. Setting a value of one can put
	 * the system into actuator saturation as no spread between
	 * the motors is possible any more. A value of 0.8 - 0.9
	 * is recommended.
	 *
	 * @unit norm
	 * @min 0.0
	 * @max 1.0
	 * @decimal 2
	 * @increment 0.01
	 * @group Multicopter Position Control
	 */
	0.9f,

	/**
         * FPC_Z_P
	 * Proportional gain for vertical position error
	 *
	 * @min 0.0
	 * @max 1.5
	 * @decimal 2
	 * @group Multicopter Position Control
	 */
	1.0f,

	/**
         * FPC_Z_VEL_P
	 * Proportional gain for vertical velocity error
	 *
	 * @min 0.1
	 * @max 0.4
	 * @decimal 2
	 * @group Multicopter Position Control
	 */
	0.2f,

	/**
         * FPC_Z_VEL_I
	 * Integral gain for vertical velocity error
	 *
	 * Non zero value allows hovering thrust estimation on stabilized or autonomous takeoff.
	 *
	 * @min 0.01
	 * @max 0.1
	 * @decimal 3
	 * @group Multicopter Position Control
	 */
	0.02f,

	/**
         * FPC_Z_VEL_D
	 * Differential gain for vertical velocity error
	 *
	 * @min 0.0
	 * @max 0.1
	 * @decimal 3
	 * @group Multicopter Position Control
	 */
	0.0f,

	/**
         * FPC_Z_VEL_MAX_UP
	 * Maximum vertical ascent velocity
	 *
	 * Maximum vertical velocity in AUTO mode and endpoint for stabilized modes (ALTCTRL, POSCTRL).
	 *
	 * @unit m/s
	 * @min 0.5
	 * @max 8.0
	 * @decimal 1
	 * @group Multicopter Position Control
	 */
	2.5f,

	/**
         * FPC_Z_VEL_MAX_DN
	 * Maximum vertical descent velocity
	 *
	 * Maximum vertical velocity in AUTO mode and endpoint for stabilized modes (ALTCTRL, POSCTRL).
	 *
	 * @unit m/s
	 * @min 0.5
	 * @max 4.0
	 * @group Multicopter Position Control
	 */
	1.0f,

	/**
         * FPC_Z_FF
	 * Vertical velocity feed forward
	 *
	 * Feed forward weight for altitude control in stabilized modes (ALTCTRL, POSCTRL). 0 will give slow responce and no overshot, 1 - fast responce and big overshot.
	 *
	 * @min 0.0
	 * @max 1.0
	 * @decimal 2
	 * @group Multicopter Position Control
	 */
	0.5f,

	/**
         * FPC_XY_P
	 * Proportional gain for horizontal position error
	 *
	 * @min 0.0
	 * @max 2.0
	 * @decimal 2
	 * @group Multicopter Position Control
	 */
	0.95f,

	/**
         * FPC_XY_VEL_P
	 * Proportional gain for horizontal velocity error
	 *
	 * @min 0.06
	 * @max 0.15
	 * @decimal 2
	 * @group Multicopter Position Control
	 */
	0.09,

	/**
         * FPC_XY_VEL_I
	 * Integral gain for horizontal velocity error
	 *
	 * Non-zero value allows to resist wind.
	 *
	 * @min 0.0
	 * @max 0.1
	 * @decimal 3
	 * @group Multicopter Position Control
	 */
	0.02f,

	/**
         * FPC_XY_VEL_D
	 * Differential gain for horizontal velocity error. Small values help reduce fast oscillations. If value is too big oscillations will appear again.
	 *
	 * @min 0.005
	 * @max 0.1
	 * @decimal 3
	 * @group Multicopter Position Control
	 */
	0.01f,

	/**
         * FPC_XY_CRUISE
	 * Nominal horizontal velocity in mission
	 *
	 * Normal horizontal velocity in AUTO modes (includes
	 * also RTL / hold / etc.) and endpoint for
	 * position stabilized mode (POSCTRL).
	 *
	 * @unit m/s
	 * @min 3.0
	 * @max 20.0
	 * @increment 1
	 * @decimal 2
	 * @group Multicopter Position Control
	 */
	5.0f,

	/**
         * FPC_TARGET_THRE
	 * Distance Threshold Horizontal Auto
	 *
	 * The distance defines at which point the vehicle
	 * has to slow down to reach target if no direct
	 * passing to the next target is desired
	 *
	 * @unit m
	 * @min 1.0
	 * @max 50.0
	 * @increment 1
	 * @decimal 2
	 * @group Multicopter Position Control
	 */
	15.0f,

	/**
         * FPC_XY_VEL_MAX
	 * Maximum horizontal velocity
	 *
	 * Maximum horizontal velocity in AUTO mode. If higher speeds
	 * are commanded in a mission they will be capped to this velocity.
     * NOTE: POSCTL uses the min of this and FPC_VEL_MAN_MAX as its max
	 *
	 * @unit m/s
	 * @min 0.0
	 * @max 20.0
	 * @increment 1
	 * @decimal 2
	 * @group Multicopter Position Control
	 */
	7.0f,

	/**
         * FPC_XY_FF
	 * Horizontal velocity feed forward
	 *
	 * Feed forward weight for position control in position control mode (POSCTRL). 0 will give slow responce and no overshot, 1 - fast responce and big overshot.
	 *
	 * @min 0.0
	 * @max 1.0
	 * @decimal 2
	 * @group Multicopter Position Control
	 */
	0.5f,

	/**
         * FPC_TILTMAX_AIR
	 * Maximum tilt angle in air
	 *
	 * Limits maximum tilt in AUTO and POSCTRL modes during flight.
	 *
	 * @unit deg
	 * @min 0.0
	 * @max 90.0
	 * @decimal 1
	 * @group Multicopter Position Control
	 */
	30.0f,

	/**
         * FPC_TILTMAX_LND
	 * Maximum tilt during landing
	 *
	 * Limits maximum tilt angle on landing.
	 *
	 * @unit deg
	 * @min 0.0
	 * @max 90.0
	 * @decimal 1
	 * @group Multicopter Position Control
	 */
	10.0f,

	/**
         * FPC_LAND_SPEED
	 * Landing descend rate
	 *
	 * @unit m/s
	 * @min 0.2
	 * @decimal 1
	 * @group Multicopter Position Control
	 */
	0.3f,

	/**
         * FPC_TKO_SPEED
	 * Takeoff climb rate
	 *
	 * @unit m/s
	 * @min 1
	 * @max 5
	 * @decimal 2
	 * @group Multicopter Position Control
	 */
	1.5f,

	/**
         * FPC_MAN_TILT_MAX
	 * Maximal tilt angle in manual or altitude mode
	 *
	 * @unit deg
	 * @min 0.0
	 * @max 90.0
	 * @decimal 1
	 * @group Multicopter Position Control
	 */
	30.0f,

	/**
         * FPC_MAN_Y_MAX
	 * Max manual yaw rate
	 *
	 * @unit deg/s
	 * @min 0.0
	 * @max 400
	 * @decimal 1
	 * @group Multicopter Position Control
	 */
	200.0f,

	/**
         * FPC_HOLD_DZ
	 * Deadzone of sticks where position hold is enabled
	 *
	 * @min 0.0
	 * @max 1.0
	 * @decimal 2
	 * @group Multicopter Position Control
	 */
	0.1f,

	/**
         * FPC_HOLD_MAX_XY
	 * Maximum horizontal velocity for which position hold is enabled (use 0 to disable check)
	 *
	 * @unit m/s
	 * @min 0.0
	 * @max 3.0
	 * @decimal 2
	 * @group Multicopter Position Control
	 */
	0.8f,

	/**
         * FPC_HOLD_MAX_Z
	 * Maximum vertical velocity for which position hold is enabled (use 0 to disable check)
	 *
	 * @unit m/s
	 * @min 0.0
	 * @max 3.0
	 * @decimal 2
	 * @group Multicopter Position Control
	 */
	0.6f,

	/**
         * FPC_VELD_LP
	 * Low pass filter cut freq. for numerical velocity derivative
	 *
	 * @unit Hz
	 * @min 0.0
	 * @max 10
	 * @decimal 2
	 * @group Multicopter Position Control
	 */
	5.0f,

	/**
         * FPC_ACC_HOR_MAX
	 * Maximum horizonal acceleration in velocity controlled modes
	 *
	 * @unit m/s/s
	 * @min 2.0
	 * @max 15.0
	 * @increment 1
	 * @decimal 2
	 * @group Multicopter Position Control
	 */
	10.0f,

	/**
         * FPC_ACC_UP_MAX
	 * Maximum vertical acceleration in velocity controlled modes upward
	 *
	 * @unit m/s/s
	 * @min 2.0
	 * @max 15.0
	 * @increment 1
	 * @decimal 2
	 * @group Multicopter Position Control
	 */
	10.0f,

	/**
         * FPC_ACC_DOWN_MAX
	 * Maximum vertical acceleration in velocity controlled modes down
	 *
	 * @unit m/s/s
	 * @min 2.0
	 * @max 15.0
	 * @increment 1
	 * @decimal 2
	 * @group Multicopter Position Control
	 */
	10.0f,

	/**
         * FPC_ALT_MODE
	 * Altitude control mode, note mode 1 only tested with LPE
	 *
	 * @min 0
	 * @max 1
	 * @value 0 Altitude following
	 * @value 1 Terrain following
	 * @group Multicopter Position Control
	 */
	0,

	/**
         * FPC_XY_MAN_EXPO
	 * Manual control stick exponential curve sensitivity attenuation with small velocity setpoints
	 *
	 * The higher the value the less sensitivity the stick has around zero
	 * while still reaching the maximum value with full stick deflection.
	 *
	 * 0 Purely linear input curve (default)
	 * 1 Purely cubic input curve
	 *
	 * @min 0
	 * @max 1
	 * @decimal 2
	 * @group Multicopter Position Control
	 */
	1.0,

	/**
         * FPC_Z_MAN_EXPO
	 * Manual control stick vertical exponential curve
	 *
	 * The higher the value the less sensitivity the stick has around zero
	 * while still reaching the maximum value with full stick deflection.
	 *
	 * 0 Purely linear input curve (default)
	 * 1 Purely cubic input curve
	 *
	 * @min 0
	 * @max 1
	 * @decimal 2
	 * @group Multicopter Position Control
	 */
	1.0,

	/**
         * FPC_LAND_ALT1
	 * Altitude for 1. step of slow landing (descend)
	 *
	 * Below this altitude descending velocity gets limited
         * to a value between "FPC_Z_VEL_MAX" and "FPC_LAND_SPEED"
	 * to enable a smooth descent experience
         * Value needs to be higher than "FPC_LAND_ALT2"
	 *
	 * @unit m
	 * @min 0
	 * @max 122
	 * @decimal 1
	 * @group Multicopter Position Control
	 */
	6,

	/**
         * FPC_LAND_ALT2
	 * Altitude for 2. step of slow landing (landing)
	 *
         * Below this altitude descending velocity gets limited to "FPC_LAND_SPEED"
         * Value needs to be lower than "FPC_LAND_ALT1"
	 *
	 * @unit m
	 * @min 0
	 * @max 122
	 * @decimal 1
	 * @group Multicopter Position Control
	 */
	3,

	/**
         * FPC_TKO_RAMP_T
	 * Position control smooth takeoff ramp time constant
	 *
	 * Increasing this value will make automatic and manual takeoff slower.
	 * If it's too slow the drone might scratch the ground and tip over.
	 *
	 * @min 0.1
	 * @max 1
	 * @group Multicopter Position Control
	 */
	0.1f,

    /**
     * MC_YAWRATE_MAX
     */
	200.0f,

	/**
	 * MC_YAW_P
	 */
	2.8f,

	/**
	 * VT_OPT_RECOV_EN
	 */
	0,

    /**
     * FPC_CRUISE_90
     * Cruise speed when angle prev-current/current-next setpoint
     * is 90 degrees. It should be lower than FPC_XY_CRUISE.
     *
     * Applies only in AUTO modes (includes
     * also RTL / hold / etc.)
     *
     * @unit m/s
     * @min 1.0
     * @increment 1
     * @decimal 2
     * @group Multicopter Position Control
     */
    3.0f,

    /**
     * FPC_JERK_MAX
     * Maximum jerk in manual controlled mode for BRAKING to zero.
     * If this value is below FPC_JERK_MIN, the acceleration limit in xy and z
     * is FPC_ACC_HOR_MAX and FPC_ACC_UP_MAX respectively instantaneously when the
     * user demands brake (=zero stick input).
     * Otherwise the acceleration limit increases from current acceleration limit
     * towards FPC_ACC_HOR_MAX/FPC_ACC_UP_MAX with jerk limit
     *
     * @unit m/s/s/s
     * @min 0.0
     * @max 15.0
     * @increment 1
     * @decimal 2
     * @group Multicopter Position Control
     */
    10.0f,

    /**
     * FPC_JERK_MIN
     * Minimum jerk in manual controlled mode for BRAKING to zero
     *
     * @unit m/s/s/s
     * @min 0.5
     * @max 10.0
     * @increment 1
     * @decimal 2
     * @group Multicopter Position Control
     */
    0.0f,

    /**
     * FPC_DEC_HOR_SLOW
     * Slow horizontal manual deceleration for manual mode
     *
     * @unit m/s/s
     * @min 0.5
     * @max 10.0
     * @increment 1
     * @decimal 2
     * @group Multicopter Position Control
     */
    3.0f,

    /**
     * FPC_VEL_MANUAL
     * Maximum horizontal velocity setpoint for manual controlled mode
     * If velocity setpoint larger than FPC_XY_VEL_MAX is set, then
     * the setpoint will be capped to FPC_XY_VEL_MAX
     *
     * @unit m/s
     * @min 3.0
     * @max 20.0
     * @increment 1
     * @decimal 2
     * @group Multicopter Position Control
     */
    7.0f,

    /**
     * NAV_ACC_RAD
     *
     * Default acceptance radius, overridden by acceptance radius of waypoint if set.
     * For fixed wing the L1 turning distance is used for horizontal acceptance.
     *
     * @unit m
     * @min 0.05
     * @max 200.0
     * @decimal 1
     * @increment 0.5
     * @group Mission
     */
    2.0f,

    /** NAV_MIS_YAW_ERR
     *
     *  \brief Max yaw error in degrees needed for waypoint heading acceptance.
     *
     *  \par Limits:
     *       Min > Max (incr.) 0 > 90 , default 12.0.
     */
    12.0f

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
** End of file FPC_config.c
**=====================================================================================*/
    
