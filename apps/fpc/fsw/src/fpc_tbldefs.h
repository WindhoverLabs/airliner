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
    
#ifndef FPC_TBLDEFS_H
#define FPC_TBLDEFS_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Local Defines
*************************************************************************/
/**
 * \brief Defines the number of entries in the table
 */
#define FPC_CONFIG_TABLE_MAX_ENTRIES  (1)

/**
 * \brief Defines the table identification name used for table registration.
 */
#define FPC_CONFIG_TABLENAME          ("CONFIG_TBL")

/**
 * \brief Defines the table file name used for table registration.
 */
#define FPC_CDS_TABLENAME  ("fpc_CdsTbl")

/************************************************************************
** Local Structure Declarations
*************************************************************************/


/** \brief Definition for a single config table entry */
typedef struct
{
    /** \brief Minimum thrust in auto thrust control.
     *
     *  \par It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     *  \par Limits:
     *       Min > Max (incr.) #THR_MIN_MIN > #THR_MIN_MAX, default 0.2.
     */
    float THR_MIN;

    /** \brief Hover thrust.
     *
     *  \par Vertical thrust required to hover.  This value is mapped to center
     *       stick for manual throttle control.  With this value set to the
     *       thrust required to hover, transition from manual to ALTCTL mode
     *       while hovering will occur with the throttle stick near center,
     *       which is then interpreted as (near) zero demand for vertical
     *       speed.
     *
     *  \par Limits:
     *       Min > Max (incr.) #THR_HOVER_MIN > #THR_HOVER_MAX, default 0.2.
     */
    float THR_HOVER;

    /** \brief Maximum thrust in auto thrust control.
     *
     *  \par Limit max allowed thrust. Setting a value of one can put
     *       the system into actuator saturation as no spread between
     *       the motors is possible any more. A value of 0.8 - 0.9
     *       is recommended.
     *
     *  \par Limits:
     *       Min > Max (incr.) #THR_MAX_MIN > #THR_MAX_MAX.
     */
    float THR_MAX;

    /** \brief Minimum manual thrust.
     *
     *  \par Minimum vertical thrust. It's recommended to set it > 0 to avoid
     *       free fall with zero thrust.
     *
     *  \par Limits:
     *       Min > Max (incr.) #MANTHR_MIN_MIN > #MANTHR_MIN_MAX.
     */
    float MANTHR_MIN;

    /** \brief Maximum manual thrust.
     *
     *  \par Limit max allowed thrust. Setting a value of one can put
     *       the system into actuator saturation as no spread between
     *       the motors is possible any more. A value of 0.8 - 0.9
     *       is recommended.
     *
     *  \par Limits:
     *       Min > Max (incr.) #MANTHR_MAX_MIN > #MANTHR_MAX_MAX.
     */
    float MANTHR_MAX;

    /** \brief Proportional gain for vertical position error
     *
     *  \par Limits:
     *       Min > Max (incr.) #Z_P_MIN > #Z_P_MAX.
     */
    float Z_P;

    /** \brief Proportional gain for vertical velocity error
     *
     *  \par Limits:
     *       Min > Max (incr.) #Z_VEL_P_MIN > #Z_VEL_P_MAX.
     */
    float Z_VEL_P;

    /** \brief Integral gain for vertical velocity error
     *
     *  \par Non zero value allows hovering thrust estimation on stabilized or
     *       autonomous takeoff.
     *
     *  \par Limits:
     *       Min > Max (incr.) #Z_VEL_I_MIN > #Z_VEL_I_MAX.
     */
    float Z_VEL_I;

    /** \brief Differential gain for vertical velocity error.
     *
     *  \par Limits:
     *       Min > Max (incr.) #Z_VEL_D_MIN > #Z_VEL_D_MAX.
     */
    float Z_VEL_D;

    /** \brief Maximum vertical ascent velocity
     *
     *  \par Maximum vertical velocity in AUTO mode and endpoint for
     *       stabilized modes (ALTCTRL, POSCTRL).
     *
     *  \par Limits:
     *       Min > Max (incr.) #Z_VEL_D_MIN > #Z_VEL_D_MAX.
     *
     *  \par Units: m/s
     */
    float Z_VEL_MAX_UP;

    /** \brief Maximum vertical descent velocity
     *
     *  \par Maximum vertical velocity in AUTO mode and endpoint for
     *       stabilized modes (ALTCTRL, POSCTRL).
     *
     *  \par Limits:
     *       Min > Max (incr.) #Z_VEL_MAX_DN_MIN > #Z_VEL_MAX_DN_MAX.
     *
     *  \par Units: m/s
     */
    float Z_VEL_MAX_DN;

    /** \brief Vertical velocity feed forward
     *
     *  \par Feed forward weight for altitude control in stabilized modes
     *       (ALTCTRL, POSCTRL). 0 will give slow response and no overshot,
     *       1 - fast response and big overshot.
     *
     *  \par Limits:
     *       Min > Max (incr.) #Z_FF_MIN > #Z_FF_MAX.
     */
    float Z_FF;

    /** \brief Proportional gain for horizontal position error.
     *
     *  \par Limits:
     *       Min > Max (incr.) #XY_P_MIN > #XY_P_MAX.
     */
    float XY_P;

    /** \brief Proportional gain for horizontal velocity error.
     *
     *  \par Limits:
     *       Min > Max (incr.) #XY_VEL_P_MIN > #XY_VEL_P_MAX.
     */
    float XY_VEL_P;

    /** \brief Non-zero value allows to resist wind.
     *
     *  \par Limits:
     *       Min > Max (incr.) #XY_VEL_I_MIN > #XY_VEL_I_MAX.
     */
    float XY_VEL_I;

    /** \brief Differential gain for horizontal velocity error. Small values
     *         help reduce fast oscillations. If value is too big
     *         oscillations will appear again.
     *
     *  \par Limits:
     *       Min > Max (incr.) #XY_VEL_D_MIN > #XY_VEL_D_MAX.
     */
    float XY_VEL_D;

    /** \brief Nominal horizontal velocity in mission.
     *
     *  \par Limits:
     *       Min > Max (incr.) #XY_CRUISE_MIN > #XY_CRUISE_MAX.
     *
     *  \par Units:  m/s
     */
    float XY_CRUISE;

    /** \brief Distance Threshold Horizontal Auto
     *
     *  \par The distance defines at which point the vehicle
     *       has to slow down to reach target if no direct
     *       passing to the next target is desired.
     *
     *  \par Limits:
     *       Min > Max (incr.) #TARGET_THRE_MIN > #TARGET_THRE_MAX.
     *
     *  \par Units:  m
     */
    float TARGET_THRE;

    /** \brief Maximum horizontal velocity
     *
     *  \par Maximum horizontal velocity in AUTO mode. If higher speeds
     *       are commanded in a mission they will be capped to this velocity.
     *
     *  \par Limits:
     *       Min > Max (incr.) #XY_VEL_MIN > #XY_VEL_MAX.
     *
     *  \par Units:  m/s
     */
    float XY_VEL_MAX;

    /** \brief Horizontal velocity feed forward
     *
     *  \par Feed forward weight for position control in position control mode (POSCTRL).
     *       0 will give slow response and no overshot, 1 - fast response and big overshot.
     *
     *  \par Limits:
     *       Min > Max (incr.) #XY_FF_MIN > #XY_FF_MAX.
     */
    float XY_FF;

    /** \brief Maximum tilt angle in air.
     *
     *  \par Limits maximum tilt in AUTO and POSCTRL modes during flight.
     *
     *  \par Limits:
     *       Min > Max (incr.) #TILTMAX_AIR_MIN > #TILTMAX_AIR_MAX.
     *
     *  \par Units: degrees
     */
    float TILTMAX_AIR;

    /** \brief Maximum tilt angle during landing.
     *
     *  \par Limits maximum tilt angle on landing.
     *
     *  \par Limits:
     *       Min > Max (incr.) #TILTMAX_LND_MIN > #TILTMAX_LND_MAX.
     *
     *  \par Units: degrees
     */
    float TILTMAX_LND;

    /** \brief Landing descend rate
     *
     *  \par Limits:
     *       Min > Max (incr.) #LAND_SPEED_MIN > #LAND_SPEED_MAX.
     *
     *  \par Units: m
     */
    float LAND_SPEED;

    /** \brief Takeoff climb rate
     *
     *  \par Limits:
     *       Min > Max (incr.) #TKO_SPEED_MIN > #TKO_SPEED_MAX.
     *
     *  \par Units: m/s
     */
    float TKO_SPEED;

    /** \brief Maximal tilt angle in manual or altitude mode
     *
     *  \par Limits:
     *       Min > Max (incr.) #MAN_TILT_MAX_MIN > #MAN_TILT_MAX_MAX.
     *
     *  \par Units: deg
     */
    float MAN_TILT_MAX;

    /** \brief Max manual yaw rate
     *
     *  \par Limits:
     *       Min > Max (incr.) #MAN_Y_MAX_MIN > #MAN_Y_MAX_MAX.
     *
     *  \par Units: deg/s
     */
    float MAN_Y_MAX;

    /** \brief Deadzone of sticks where position hold is enabled
     *
     *  \par Limits:
     *       Min > Max (incr.) #MAN_Y_MAX_MIN > #MAN_Y_MAX_MAX.
     *
     *  \par Units: deg/s
     */
    float HOLD_DZ;

    /** \brief Maximum horizontal velocity for which position hold
     *         is enabled (use 0 to disable check).
     *
     *  \par Limits:
     *       Min > Max (incr.) #MAN_XY_MAX_MIN > #MAN_XY_MAX_MAX.
     *
     *  \par Units: m/s
     */
    float HOLD_MAX_XY;

    /** \brief Maximum vertical velocity for which position hold is enabled
     *         (use 0 to disable check)
     *
     *  \par Limits:
     *       Min > Max (incr.) #HOLD_MAX_Z_MIN > #HOLD_MAX_Z_MAX.
     *
     *  \par Units: m/s
     */
    float HOLD_MAX_Z;

    /** \brief Low pass filter cut freq. for numerical velocity derivative
     *
     *  \par Limits:
     *       Min > Max (incr.) #VELD_LP_MIN > #VELD_LP_MAX.
     *
     *  \par Units: Hz
     */
    float VELD_LP;

    /** \brief Maximum horizonal acceleration in velocity controlled modes
     *
     *  \par Limits:
     *       Min > Max (incr.) #ACC_HOR_MAX_MIN > #ACC_HOR_MAX_MAX.
     *
     *  \par Units: m/s/s
     */
    float ACC_HOR_MAX;

    /** \brief Maximum vertical acceleration in velocity controlled modes upward
     *
     *  \par Limits:
     *       Min > Max (incr.) #ACC_UP_MAX_MIN > #ACC_UP_MAX_MAX.
     *
     *  \par Units: m/s/s
     */
    float ACC_UP_MAX;

    /** \brief Maximum vertical acceleration in velocity controlled modes down
     *
     *  \par Limits:
     *       Min > Max (incr.) #ACC_DOWN_MAX_MIN > #ACC_DOWN_MAX_MAX.
     *
     *  \par Units: m/s/s
     */
    float ACC_DOWN_MAX;

    /** \brief Altitude control mode, note mode 1 only tested with LPE
     *
     *  \par Limits:
     *       Min > Max (incr.) #ALT_MODE_MIN > #ALT_MODE_MAX.
     *
     *  \par 0: Altitude following, 1: Terrain following
     */
    uint32 ALT_MODE;

    /** \brief Manual control stick exponential curve sensitivity attenuation
     *         with small velocity setpoints.
     *
     *  \par Limits:
     *       Min > Max (incr.) #XY_MAN_EXPO_MIN > #XY_MAN_EXPO_MAX.
     *
     *  \par 0: Purely linear input curve, 1: Purely cubic input curve
     */
    float XY_MAN_EXPO;

    /** \brief Manual control stick vertical exponential curve
     *
     *  \par The higher the value the less sensitivity the stick has
     *       around zero while still reaching the maximum value with full
     *       stick deflection.
     *
     *  \par Limits:
     *       Min > Max (incr.) #Z_MAN_EXPO_MIN > #Z_MAN_EXPO_MAX.
     *
     *  \par 0: Purely linear input curve, 1: Purely cubic input curve
     */
    float Z_MAN_EXPO;

    /** \brief Altitude for step 1 of slow landing (descend)
     *
     *  \par Below this altitude descending velocity gets limited
     *       to a value between #Z_VEL_MAX and #LAND_SPEED
     *       to enable a smooth descent experience.
     *       Value needs to be higher than #LAND_ALT2
     *
     *  \par Limits:
     *       Min > Max (incr.) #LAND_ALT1_MIN > #LAND_ALT1_MAX.
     *
     *  \par Units: m
     */
    uint32 LAND_ALT1;

    /** \brief Altitude for step 2 of slow landing (landing)
     *
     *  \par Below this altitude descending velocity gets limited to
     *  #LAND_SPEED.  Value needs to be lower than #LAND_ALT1.
     *
     *  \par Limits:
     *       Min > Max (incr.) #LAND_ALT2_MIN > #LAND_ALT2_MAX.
     *
     *  \par Units: m
     */
    uint32 LAND_ALT2;

    /** \brief Position control smooth takeoff ramp time constant
     *
     *  \par Increasing this value will make automatic and manual takeoff
     *       slower.  If it's too slow the drone might scratch the ground
     *       and tip over.
     *
     *  \par Limits:
     *       Min > Max (incr.) #TKO_RAMP_T_MIN > #TKO_RAMP_T_MAX.
     */
    float TKO_RAMP_T;

    /** \brief Max yaw rate
     *
     *  \par Limits:
     *       Min > Max (incr.) #MC_YAWRATE_MAX_MIN > #MC_YAWRATE_MAX_MAX.
     *
     *  \par Units: deg/s
     */
    float MC_YAWRATE_MAX;

    /** \brief Yaw P gain
     *
     *  \par Yaw proportional gain, i.e. desired angular speed in rad/s for error 1 rad.
     *
     *  \par Limits:
     *       Min > Max (incr.) #MC_YAW_P_MIN > #MC_YAW_P_MAX.
     *
     *  \par Units: 1/s
     */
    float MC_YAW_P;

    int32 VT_OPT_RECOV_EN;

    /** \brief Cruising speed at 90
     *
     *  \par Cruise speed when angle prev-current/current-next setpoint
     *       is 90 degrees. It should be lower than #XY_CRUISE.
     *
     *  \par Applies only in AUTO modes (includes also RTL / hold / etc.)
     *
     *  \par Limits:
     *       Min > Max (incr.) #FPC_CRUISE_90_MIN > #FPC_CRUISE_90_MAX.
     *
     *  \par Units: m/s
     */
    float FPC_CRUISE_90;

    /** \brief Maximum jerk in manual controlled mode for BRAKING to zero.
     *
     *  \par If this value is below FPC_JERK_MIN, the acceleration limit in xy
     *   and z is FPC_ACC_HOR_MAX and FPC_ACC_UP_MAX respectively
     *   instantaneously when the user demands brake (=zero stick input).
     *   Otherwise the acceleration limit increases from current acceleration
     *   limit towards FPC_ACC_HOR_MAX/FPC_ACC_UP_MAX with jerk limit
     *
     *  \par Limits:
     *       Min > Max (incr.) #FPC_JERK_MAX_MIN > #FPC_JERK_MAX_MAX.
     *
     *  \par Units: m/s/s/s
     */
    float FPC_JERK_MAX;

    /** \brief Minimum jerk in manual controlled mode for BRAKING to zero
     *
     *  \par Limits:
     *       Min > Max (incr.) #FPC_JERK_MIN_MIN > #FPC_JERK_MIN_MAX.
     *
     *  \par Units: m/s/s/s
     */
    float FPC_JERK_MIN;

    /** \brief Slow horizontal manual deceleration for manual mode
     *
     *  \par Limits:
     *       Min > Max (incr.) #FPC_DEC_HOR_SLOW_MIN > #FPC_DEC_HOR_SLOW_MAX.
     *
     *  \par Units: m/s/s
     */
    float FPC_DEC_HOR_SLOW;

    /** \brief Maximum horizontal velocity setpoint for manual controlled mode
     *
     *  \par If velocity setpoint larger than FPC_XY_VEL_MAX is set, then
     *       the setpoint will be capped to FPC_XY_VEL_MAX
     *
     *  \par Limits:
     *       Min > Max (incr.) #FPC_VEL_MANUAL_MIN > #FPC_VEL_MANUAL_MAX.
     *
     *  \par Units: m/s
     */
    float FPC_VEL_MANUAL;

    /** \brief Acceptance Radius
     *
     *  \par Default acceptance radius, overridden by acceptance radius of
     *       waypoint if set. For fixed wing the L1 turning distance is used
     *       for horizontal acceptance.
     *
     *  \par Limits:
     *       Min > Max (incr.) #NAV_ACC_RAD_MIN > #NAV_ACC_RAD_MAX.
     *
     *  \par Units: m
     */
    float NAV_ACC_RAD;

    /** NAV_MIS_YAW_ERR
     *
     *  \brief Max yaw error in degrees needed for waypoint heading acceptance.
     *
     *  \par Limits:
     *       Min > Max (incr.) NAV_MIS_YAW_ERR_MIN > NAV_MIS_YAW_ERR_MAX , default 12.0.
     *
     *  \par Units: deg
     */
    float NAV_MIS_YAW_ERR;


} FPC_ConfigTbl_t;



/** \brief Definition for a single config table entry */
typedef struct
{
    int32  iParam;

    /* TODO:  Add type declaration for config parameters here.
    **
    ** Examples:
    **    int8/char            cParam;
    **    int8/char            cParams[16];
    **    uint8/unsigned char  ucParam;
    **    uint8/unsigned char  ucParams[16];
    **
    **    int16   sParam;
    **    int16   sParams[8];
    **    uint16  usParam;
    **    uint16  usParams[8];
    **
    **    int32   iParam;
    **    int32   iParams[5];
    **    uint32  uiParam;
    **    uint32  uiParams[5];
    **
    **    float  fParam;
    **    float  fParams[3];
    **
    **    double  dParam;
    **    double  dParams[3];
    */
} FPC_ConfigTblEntry_t;


/** \brief Definition for Critical Data Storage (CDS) table entry */
typedef struct
{
    int32  iParam;

    /* TODO:  Add type declaration for CDS data here. */
} FPC_CdsTbl_t;

/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Prototypes
*************************************************************************/


#ifdef __cplusplus
}
#endif

#endif /* FPC_TBLDEFS_H */

/************************/
/*  End of File Comment */
/************************/
