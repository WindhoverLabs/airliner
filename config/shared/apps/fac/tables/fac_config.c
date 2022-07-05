#include "cfe_tbl_filedef.h"
#include "fac_tbldefs.h"

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
FAC_ConfigTbl_t FAC_ConfigTbl =
{
   /**
    * Attitude Roll Time Constant
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
    * Attitude pitch time constant
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
    * Pitch rate proportional gain.
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
   0.08f,

   /**
    * Pitch rate integrator gain.
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
   0.02f,

   /**
    * Maximum positive / up pitch rate.
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
    * Maximum negative / down pitch rate.
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
    * Pitch rate integrator limit
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
    * Roll rate proportional Gain
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
   0.05f,

   /**
    * Roll rate integrator Gain
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
   0.01f,

   /**
    * Roll integrator anti-windup
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
    * Maximum roll rate
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
    * Yaw rate proportional gain
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
    * Yaw rate integrator gain
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
   0.0f,

   /**
    * Yaw rate integrator limit
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
    * Maximum yaw rate
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
    * Roll control to yaw control feedforward gain.
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
    * Enable wheel steering controller
    *
    * @boolean
    * @group FW Attitude Control
    */
   0,

   /**
    * Wheel steering rate proportional gain
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
    * Wheel steering rate integrator gain
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
    * Wheel steering rate integrator limit
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
    * Maximum wheel steering rate
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
    * Roll rate feed forward
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
   0.5f,

   /**
    * Pitch rate feed forward
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
   0.5f,

   /**
    * Yaw rate feed forward
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
    * Wheel steering rate feed forward
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
    * Minimal speed for yaw coordination
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
   1000.0f,

   /**
    * Method used for yaw coordination
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
    * Roll setpoint offset
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
    * Pitch setpoint offset
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
    * Max manual roll
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
   45.0f,

   /**
    * Max manual pitch
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
   45.0f,

   /**
    * Scale factor for flaps
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
    * Scale factor for flaperons
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
    * Disable airspeed sensor
    *
    * For small wings or VTOL without airspeed sensor this parameter can be used to
    * enable flying without an airspeed reading
    *
    * @boolean
    * @group FW Attitude Control
    */
   0,

   /**
    * Manual roll scale
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
    * Manual pitch scale
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
    * Manual yaw scale
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
    * Whether to scale throttle by battery power level
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
    * Acro body x max rate.
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
    * Acro body y max rate.
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
    * Acro body z max rate.
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
    * Threshold for Rattitude mode
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
   0.8f
};

CFE_TBL_FILEDEF(FAC_ConfigTbl, FAC.CONFIG_TBL, FAC dflt config table, fac_config.tbl)
