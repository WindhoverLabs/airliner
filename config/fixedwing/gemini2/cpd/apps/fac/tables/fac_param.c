
/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe_tbl_filedef.h"
#include "fac_tbldefs.h"

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Definitions
*************************************************************************/

/**
** \brief The cFE FAC config table definition.
**
** Content format: ObjName[64], TblName[38], Desc[32], TgtFileName[20], ObjSize
**    ObjName - variable name of config table, e.g., FAC_ConfigDefTbl[]
**    TblName - app's table name, e.g., FAC.CONFIG_TBL, where FAC is the same app name
**              used in cfe_es_startup.scr, and FAC_defConfigTbl is the same table
**              name passed in to CFE_TBL_Register()
**    Desc - description of table in string format
**    TgtFileName[20] - table file name, compiled as .tbl file extension
**    ObjSize - size of the entire table
**
*/

/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/

/**
**  \brief Default FAC config table data
*/
FAC_ParamTbl_t FAC_ParamTbl =
{
	/**
	 * Attitude Roll Time Constant (FW_R_TC)
	 *
	 * This defines the latency between a roll step input and the achieved setpoint
	 * (inverse to a P gain). Half a second is a good start value and fits for
	 * most average systems. Smaller systems may require smaller values, but as
	 * this will wear out servos faster, the value should only be decreased as
	 * needed.
	 *
	 * @unit s
	 * @min 0.4
	 * @max 1.0
	 * @decimal 2
	 * @increment 0.05
	 * @group FW Attitude Control
	 */
     0.4f,

	/**
	 * Attitude pitch time constant (FW_P_TC)
	 *
	 * This defines the latency between a pitch step input and the achieved setpoint
	 * (inverse to a P gain). Half a second is a good start value and fits for
	 * most average systems. Smaller systems may require smaller values, but as
	 * this will wear out servos faster, the value should only be decreased as
	 * needed.
	 *
	 * @unit s
	 * @min 0.2
	 * @max 1.0
	 * @decimal 2
	 * @increment 0.05
	 * @group FW Attitude Control
	 */
     0.4f,

	/**
	 * Pitch rate proportional gain (FW_PR_P).
	 *
	 * This defines how much the elevator input will be commanded depending on the
	 * current body angular rate error.
	 *
	 * @unit %/rad/s
	 * @min 0.005
	 * @max 1.0
	 * @decimal 3
	 * @increment 0.005
	 * @group FW Attitude Control
	 */
     0.04f,

	/**
	 * Pitch rate integrator gain (FW_PR_I).
	 *
	 * This gain defines how much control response will result out of a steady
	 * state error. It trims any constant error.
	 *
	 * @unit %/rad
	 * @min 0.005
	 * @max 0.5
	 * @decimal 3
	 * @increment 0.005
	 * @group FW Attitude Control
	 */
     0.01f,

	/**
	 * Maximum positive / up pitch rate (FW_P_RMAX_POS).
	 *
	 * This limits the maximum pitch up angular rate the controller will output (in
	 * degrees per second). Setting a value of zero disables the limit.
	 *
	 * @unit deg/s
	 * @min 0.0
	 * @max 90.0
	 * @decimal 1
	 * @increment 0.5
	 * @group FW Attitude Control
	 */
     60.0f,

	/**
	 * Maximum negative / down pitch rate (FW_P_RMAX_NEG).
	 *
	 * This limits the maximum pitch down up angular rate the controller will
	 * output (in degrees per second). Setting a value of zero disables the limit.
	 *
	 * @unit deg/s
	 * @min 0.0
	 * @max 90.0
	 * @decimal 1
	 * @increment 0.5
	 * @group FW Attitude Control
	 */
     60.0f,

	/**
	 * Pitch rate integrator limit (FW_PR_IMAX).
	 *
	 * The portion of the integrator part in the control surface deflection is
	 * limited to this value
	 *
	 * @min 0.0
	 * @max 1.0
	 * @decimal 2
	 * @increment 0.05
	 * @group FW Attitude Control
	 */
     0.4f,

	/**
	 * Roll rate proportional Gain (FW_RR_P).
	 *
	 * This defines how much the aileron input will be commanded depending on the
	 * current body angular rate error.
	 *
	 * @unit %/rad/s
	 * @min 0.005
	 * @max 1.0
	 * @decimal 3
	 * @increment 0.005
	 * @group FW Attitude Control
	 */
     0.20f,

	/**
	 * Roll rate integrator Gain (FW_RR_I).
	 *
	 * This gain defines how much control response will result out of a steady
	 * state error. It trims any constant error.
	 *
	 * @unit %/rad
	 * @min 0.005
	 * @max 0.2
	 * @decimal 3
	 * @increment 0.005
	 * @group FW Attitude Control
	 */
     0.02f,

	/**
	 * Roll integrator anti-windup (FW_RR_IMAX).
	 *
	 * The portion of the integrator part in the control surface deflection is limited to this value.
	 *
	 * @min 0.0
	 * @max 1.0
	 * @decimal 2
	 * @increment 0.05
	 * @group FW Attitude Control
	 */
     0.2f,

	/**
	 * Maximum roll rate (FW_R_RMAX).
	 *
	 * This limits the maximum roll rate the controller will output (in degrees per
	 * second). Setting a value of zero disables the limit.
	 *
	 * @unit deg/s
	 * @min 0.0
	 * @max 90.0
	 * @decimal 1
	 * @increment 0.5
	 * @group FW Attitude Control
	 */
     70.0f,

	/**
	 * Yaw rate proportional gain (FW_YR_P).
	 *
	 * This defines how much the rudder input will be commanded depending on the
	 * current body angular rate error.
	 *
	 * @unit %/rad/s
	 * @min 0.005
	 * @max 1.0
	 * @decimal 3
	 * @increment 0.005
	 * @group FW Attitude Control
	 */
     0.05f,

	/**
	 * Yaw rate integrator gain (FW_YR_I).
	 *
	 * This gain defines how much control response will result out of a steady
	 * state error. It trims any constant error.
	 *
	 * @unit %/rad
	 * @min 0.0
	 * @max 50.0
	 * @decimal 1
	 * @increment 0.5
	 * @group FW Attitude Control
	 */
     0.02f,

	/**
	 * Yaw rate integrator limit (FW_YR_IMAX).
	 *
	 * The portion of the integrator part in the control surface deflection is
	 * limited to this value
	 *
	 * @min 0.0
	 * @max 1.0
	 * @decimal 2
	 * @increment 0.05
	 * @group FW Attitude Control
	 */
     0.2f,

	/**
	 * Maximum yaw rate (FW_Y_RMAX).
	 *
	 * This limits the maximum yaw rate the controller will output (in degrees per
	 * second). Setting a value of zero disables the limit.
	 *
	 * @unit deg/s
	 * @min 0.0
	 * @max 90.0
	 * @decimal 1
	 * @increment 0.5
	 * @group FW Attitude Control
	 */
     0.0f,

	/**
	 * Roll control to yaw control feedforward gain (FW_RLL_TO_YAW_FF).
	 *
	 * This gain can be used to counteract the "adverse yaw" effect for fixed wings.
	 * When the plane enters a roll it will tend to yaw the nose out of the turn.
	 * This gain enables the use of a yaw actuator (rudder, airbrakes, ...) to counteract
	 * this effect.
	 *
	 * @min 0.0
	 * @decimal 1
	 * @increment 0.01
	 * @group FW Attitude Control
	 */
     0.0f,

	/**
	 * Enable wheel steering controller (FW_W_EN).
	 *
	 * @boolean
	 * @group FW Attitude Control
	 */
     1,


	/**
	 * Wheel steering rate proportional gain (FW_WR_P).
	 *
	 * This defines how much the wheel steering input will be commanded depending on the
	 * current body angular rate error.
	 *
	 * @unit %/rad/s
	 * @min 0.005
	 * @max 1.0
	 * @decimal 3
	 * @increment 0.005
	 * @group FW Attitude Control
	 */
     0.5f,

	/**
	 * Wheel steering rate integrator gain (FW_WR_I).
	 *
	 * This gain defines how much control response will result out of a steady
	 * state error. It trims any constant error.
	 *
	 * @unit %/rad
	 * @min 0.005
	 * @max 0.5
	 * @decimal 3
	 * @increment 0.005
	 * @group FW Attitude Control
	 */
     0.1f,

	/**
	 * Wheel steering rate integrator limit (FW_WR_IMAX).
	 *
	 * The portion of the integrator part in the control surface deflection is
	 * limited to this value
	 *
	 * @min 0.0
	 * @max 1.0
	 * @decimal 2
	 * @increment 0.05
	 * @group FW Attitude Control
	 */
     1.0f,

	/**
	 * Maximum wheel steering rate (FW_W_RMAX).
	 *
	 * This limits the maximum wheel steering rate the controller will output (in degrees per
	 * second). Setting a value of zero disables the limit.
	 *
	 * @unit deg/s
	 * @min 0.0
	 * @max 90.0
	 * @decimal 1
	 * @increment 0.5
	 * @group FW Attitude Control
	 */
     0.0f,

	/**
	 * Roll rate feed forward (FW_RR_FF).
	 *
	 * Direct feed forward from rate setpoint to control surface output. Use this
	 * to obtain a tigher response of the controller without introducing
	 * noise amplification.
	 *
	 * @unit %/rad/s
	 * @min 0.0
	 * @max 10.0
	 * @decimal 2
	 * @increment 0.05
	 * @group FW Attitude Control
	 */
     2.0f,

	/**
	 * Pitch rate feed forward (FW_PR_FF).
	 *
	 * Direct feed forward from rate setpoint to control surface output
	 *
	 * @unit %/rad/s
	 * @min 0.0
	 * @max 10.0
	 * @decimal 2
	 * @increment 0.05
	 * @group FW Attitude Control
	 */
     0.1f,

	/**
	 * Yaw rate feed forward (FW_YR_FF).
	 *
	 * Direct feed forward from rate setpoint to control surface output
	 *
	 * @unit %/rad/s
	 * @min 0.0
	 * @max 10.0
	 * @decimal 2
	 * @increment 0.05
	 * @group FW Attitude Control
	 */
     0.3f,

	/**
	 * Wheel steering rate feed forward (FW_WR_FF).
	 *
	 * Direct feed forward from rate setpoint to control surface output
	 *
	 * @unit %/rad/s
	 * @min 0.0
	 * @max 10.0
	 * @decimal 2
	 * @increment 0.05
	 * @group FW Attitude Control
	 */
     0.2f,

	/**
	 * Minimal speed for yaw coordination (FW_YCO_VMIN).
	 *
	 * For airspeeds above this value, the yaw rate is calculated for a coordinated
	 * turn. Set to a very high value to disable.
	 *
	 * @unit m/s
	 * @min 0.0
	 * @max 1000.0
	 * @decimal 1
	 * @increment 0.5
	 * @group FW Attitude Control
	 */
     10.0f,

	/**
	 * Method used for yaw coordination (FW_YCO_METHOD).
	 *
	 * The param value sets the method used to calculate the yaw rate
	 * 0: open-loop zero lateral acceleration based on kinematic constraints
	 * 1: closed-loop: try to reduce lateral acceleration to 0 by measuring the acceleration
	 *
	 * @min 0
	 * @max 1
	 * @value 0 open-loop
	 * @value 1 closed-loop
	 * @group FW Attitude Control
	 */
     0,

	/**
	 * Roll setpoint offset (FW_RSP_OFF).
	 *
	 * An airframe specific offset of the roll setpoint in degrees, the value is
	 * added to the roll setpoint and should correspond to the typical cruise speed
	 * of the airframe.
	 *
	 * @unit deg
	 * @min -90.0
	 * @max 90.0
	 * @decimal 1
	 * @increment 0.5
	 * @group FW Attitude Control
	 */
     0.0f,

	/**
	 * Pitch setpoint offset (FW_PSP_OFF).
	 *
	 * An airframe specific offset of the pitch setpoint in degrees, the value is
	 * added to the pitch setpoint and should correspond to the typical cruise
	 * speed of the airframe.
	 *
	 * @unit deg
	 * @min -90.0
	 * @max 90.0
	 * @decimal 1
	 * @increment 0.5
	 * @group FW Attitude Control
	 */
     0.0f,

	/**
	 * Max manual roll (FW_MAN_R_MAX).
	 *
	 * Max roll for manual control in attitude stabilized mode
	 *
	 * @unit deg
	 * @min 0.0
	 * @max 90.0
	 * @decimal 1
	 * @increment 0.5
	 * @group FW Attitude Control
	 */
     30.0f,

	/**
	 * Max manual pitch (FW_MAN_P_MAX).
	 *
	 * Max pitch for manual control in attitude stabilized mode
	 *
	 * @unit deg
	 * @min 0.0
	 * @max 90.0
	 * @decimal 1
	 * @increment 0.5
	 * @group FW Attitude Control
	 */
     20.0f,

	/**
	 * Scale factor for flaps (FW_FLAPS_SCL).
	 *
	 * @unit norm
	 * @min 0.0
	 * @max 1.0
	 * @decimal 2
	 * @increment 0.01
	 * @group FW Attitude Control
	 */
     0.0f,

	/**
	 * Scale factor for flaperons (FW_FLAPERON_SCL).
	 *
	 * @unit norm
	 * @min 0.0
	 * @max 1.0
	 * @decimal 2
	 * @increment 0.01
	 * @group FW Attitude Control
	 */
     0.0f,

	/**
	 * Disable airspeed sensor (FW_ARSP_MODE).
	 *
	 * For small wings or VTOL without airspeed sensor this parameter can be used to
	 * enable flying without an airspeed reading
	 *
	 * @boolean
	 * @group FW Attitude Control
	 */
     0,

	/**
	 * Manual roll scale (FW_MAN_R_SC).
	 *
	 * Scale factor applied to the desired roll actuator command in full manual mode. This parameter allows
	 * to adjust the throws of the control surfaces.
	 *
	 * @unit norm
	 * @min 0.0
	 * @max 1.0
	 * @decimal 2
	 * @increment 0.01
	 * @group FW Attitude Control
	 */
     1.0f,

	/**
	 * Manual pitch scale (FW_MAN_P_SC).
	 *
	 * Scale factor applied to the desired pitch actuator command in full manual mode. This parameter allows
	 * to adjust the throws of the control surfaces.
	 *
	 * @unit norm
	 * @min 0.0
	 * @decimal 2
	 * @increment 0.01
	 * @group FW Attitude Control
	 */
     1.0f,

	/**
	 * Manual yaw scale (FW_MAN_Y_SC).
	 *
	 * Scale factor applied to the desired yaw actuator command in full manual mode. This parameter allows
	 * to adjust the throws of the control surfaces.
	 *
	 * @unit norm
	 * @min 0.0
	 * @decimal 2
	 * @increment 0.01
	 * @group FW Attitude Control
	 */
     1.0f,

	/**
	 * Whether to scale throttle by battery power level (FW_BAT_SCALE_EN).
	 *
	 * This compensates for voltage drop of the battery over time by attempting to
	 * normalize performance across the operating range of the battery. The fixed wing
	 * should constantly behave as if it was fully charged with reduced max thrust
	 * at lower battery percentages. i.e. if cruise speed is at 0.5 throttle at 100% battery,
	 * it will still be 0.5 at 60% battery.
	 *
	 * @boolean
	 * @group FW Attitude Control
	 */
     0,

	/**
	 * Acro body x max rate (FW_ACRO_X_MAX).
	 *
	 * This is the rate the controller is trying to achieve if the user applies full roll
	 * stick input in acro mode.
	 *
	 * @min 45
	 * @max 720
	 * @unit degrees
	 * @group FW Attitude Control
	 */
     90.0f,

	/**
	 * Acro body y max rate (FW_ACRO_Y_MAX).
	 *
	 * This is the body y rate the controller is trying to achieve if the user applies full pitch
	 * stick input in acro mode.
	 *
	 * @min 45
	 * @max 720
	 * @unit degrees
	 * @group FW Attitude Control
	 */
     90.0f,

	/**
	 * Acro body z max rate (FW_ACRO_Z_MAX).
	 *
	 * This is the body z rate the controller is trying to achieve if the user applies full yaw
	 * stick input in acro mode.
	 *
	 * @min 10
	 * @max 180
	 * @unit degrees
	 * @group FW Attitude Control
	 */
     45.0f,

	/**
	 * Threshold for Rattitude mode (FW_RATT_TH).
	 *
	 * Manual input needed in order to override attitude control rate setpoints
	 * and instead pass manual stick inputs as rate setpoints
	 *
	 * @min 0.0
	 * @max 1.0
	 * @decimal 2
	 * @increment 0.01
	 * @group FW Attitude Control
	 */
     0.8f,

	/**
	 * Minimum Airspeed (FW_AIRSPD_MIN).
	 *
	 * If the airspeed falls below this value, the TECS controller will try to
	 * increase airspeed more aggressively.
	 *
	 * @unit m/s
	 * @min 0.0
	 * @max 40
	 * @decimal 1
	 * @increment 0.5
	 * @group FW TECS
	 */
     13.0f,

	/**
	 * Maximum Airspeed (FW_AIRSPD_MAX).
	 *
	 * If the airspeed is above this value, the TECS controller will try to decrease
	 * airspeed more aggressively.
	 *
	 * @unit m/s
	 * @min 0.0
	 * @max 40
	 * @decimal 1
	 * @increment 0.5
	 * @group FW TECS
	 */
     30.0f,

	/**
	 * Cruise Airspeed (FW_AIRSPD_TRIM).
	 *
	 * The fixed wing controller tries to fly at this airspeed.
	 *
	 * @unit m/s
	 * @min 0.0
	 * @max 40
	 * @decimal 1
	 * @increment 0.5
	 * @group FW TECS
	 */
     21.0f,

	/**
	 * Roll trim (TRIM_ROLL).
	 *
	 * The trim value is the actuator control value the system needs
	 * for straight and level flight. It can be calibrated by
	 * flying manually straight and level using the RC trims and
	 * copying them using the GCS.
	 *
	 * @group Radio Calibration
	 * @min -0.25
	 * @max 0.25
	 * @decimal 2
	 * @increment 0.01
	 */
     0.0f,

	/**
	 * Pitch trim (TRIM_PITCH).
	 *
	 * The trim value is the actuator control value the system needs
	 * for straight and level flight. It can be calibrated by
	 * flying manually straight and level using the RC trims and
	 * copying them using the GCS.
	 *
	 * @group Radio Calibration
	 * @min -0.25
	 * @max 0.25
	 * @decimal 2
	 * @increment 0.01
	 */
     0.0f,

	/**
	 * Yaw trim (TRIM_YAW).
	 *
	 * The trim value is the actuator control value the system needs
	 * for straight and level flight. It can be calibrated by
	 * flying manually straight and level using the RC trims and
	 * copying them using the GCS.
	 *
	 * @group Radio Calibration
	 * @min -0.25
	 * @max 0.25
	 * @decimal 2
	 * @increment 0.01
	 */
     0.0f,

	/**
	 * VTOL Type (Tailsitter=0, Tiltrotor=1, Standard=2) (VT_TYPE).
	 *
	 * @value 0 Tailsitter
	 * @value 1 Tiltrotor
	 * @value 2 Standard
	 * @min 0
	 * @max 2
	 * @decimal 0
	 * @group VTOL Attitude Control
	 */
     0
};

CFE_TBL_FILEDEF(FAC_ParamTbl, FAC.PARAM_TBL, FAC dflt param table, fac_param.tbl)

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
