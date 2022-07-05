/****************************************************************************
 *
 *   Copyright (c) 2017 Windhover Labs, L.L.C. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name Windhover Labs nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/

#ifndef FAC_TBLDEFS_H
#define FAC_TBLDEFS_H

#include "cfe.h"


#define FAC_CONFIG_TABLENAME          ("CONFIG_TBL")


typedef struct
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
   float FW_R_TC;

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
   float FW_P_TC;

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
   float FW_PR_P;

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
   float FW_PR_I;

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
   float FW_P_RMAX_POS;

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
   float FW_P_RMAX_NEG;

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
   float FW_PR_IMAX;

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
   float FW_RR_P;

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
   float FW_RR_I;

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
   float FW_RR_IMAX;

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
   float FW_R_RMAX;

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
   float FW_YR_P;

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
   float FW_YR_I;

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
   float FW_YR_IMAX;

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
   float FW_Y_RMAX;

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
   float FW_RLL_TO_YAW_FF;

   /**
    * Enable wheel steering controller
    *
    * @boolean
    * @group FW Attitude Control
    */
   int32 FW_W_EN;

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
   float FW_WR_P;

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
    float FW_WR_I;

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
   float FW_WR_IMAX;

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
   float FW_W_RMAX;

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
   float FW_RR_FF;

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
   float FW_PR_FF;

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
   float FW_YR_FF;

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
   float FW_WR_FF;

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
   float FW_YCO_VMIN;

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
   int32 FW_YCO_METHOD;

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
   float FW_RSP_OFF;

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
   float FW_PSP_OFF;

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
   float FW_MAN_R_MAX;

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
   float FW_MAN_P_MAX;

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
   float FW_FLAPS_SCL;

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
   float FW_FLAPERON_SCL;

   /**
    * Disable airspeed sensor
    *
    * For small wings or VTOL without airspeed sensor this parameter can be used to
    * enable flying without an airspeed reading
    *
    * @boolean
    * @group FW Attitude Control
    */
   int32 FW_ARSP_MODE;

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
   float FW_MAN_R_SC;

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
   float FW_MAN_P_SC;

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
   float FW_MAN_Y_SC;

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
   int32 FW_BAT_SCALE_EN;

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
   float FW_ACRO_X_MAX;

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
   float FW_ACRO_Y_MAX;

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
   float FW_ACRO_Z_MAX;

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
   float FW_RATT_TH;

} FAC_ConfigTbl_t;


#endif /* FAC_TBLDEFS_H */
