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

/************************************************************************
 ** Local Structure Declarations
 *************************************************************************/
typedef struct
{
    /**
     * @file launchdetection_params.c
     *
     * Parameters for launchdetection
     *
     * @author Thomas Gubler <thomasgubler@gmail.com>
     */

    /*
     * Catapult launch detection parameters, accessible via MAVLink
     *
     */

    /**
     * Launch detection
     *
     * @boolean
     * @group FW Launch detection
     */
    boolean LAUN_ALL_ON;

    /**
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
    float LAUN_CAT_A;

    /**
     * Catapult time threshold.
     *
     * LAUN_CAT_A for LAUN_CAT_T serves as threshold to trigger launch detection.
     *
     * @unit s
     * @min 0.0
     * @max 5.0
     * @decimal 2
     * @increment 0.05
     * @group FW Launch detection
     */
    float LAUN_CAT_T;

    /**
     * Motor delay
     *
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
    float LAUN_CAT_MDEL;

    /**
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
    float LAUN_CAT_PMAX_RADIANS;

} FPC_Launch_Detection_t;

typedef struct
{
    /**
     * @file runway_takeoff_params.c
     *
     * Parameters for runway takeoff
     *
     * @author Andreas Antener <andreas@uaventure.com>
     */

    /**
     * Runway takeoff with landing gear
     *
     * @boolean
     * @group Runway Takeoff
     */
    boolean RWTO_TKOFF;
    /**
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
    int32 RWTO_HDG;
    /**
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
    float NAV_ALT;
    /**
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
    float MAX_THR;
    /**
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
    float PSP_RADIANS;
    /**
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
    float MAX_PITCH_RADIANS;
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
    float MAX_ROLL_RADIANS;
    /**
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
    float AIRSPD_SCL;

    /**
     * AIRSPD_MIN
     * Minimum Airspeed
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
    float RWTO_AIRSPD_MIN;

    /**
     * CLMBOUT_DIFF
     * Climbout Altitude difference
     *
     * If the altitude error exceeds this parameter, the system will climb out
     * with maximum throttle and minimum airspeed until it is closer than this
     * distance to the desired altitude. Mostly used for takeoff waypoints / modes.
     * Set to 0 to disable climbout mode (not recommended).
     *
     * @unit m
     * @min 0.0
     * @max 150.0
     * @decimal 1
     * @increment 0.5
     * @group FW L1 Control
     */

    float RWTO_CLMBOUT_DIFF;
} FPC_Runway_Takeoff_t;

/** \brief Definition for a single config table entry
 * This table originated from the params in
 * https://github.com/WindhoverLabs/PX4-Autopilot/blob/main/src/modules/fw_pos_control_l1/FixedwingPositionControl.cpp
 *
 * Any params related to vtol were ignored.
 */
typedef struct
{
    float L1_PERIOD;
    float L1_DAMPING;

    float T_TIME_CONST;
    float T_THRO_CONST;
    float T_SINK_MIN;
    float T_SINK_MAX;
    float T_CLMB_MAX;
    float CLMBOUT_DIFF;
    float T_HRATE_P;
    float T_HRATE_FF;
    float T_HGT_NOISE_FILTER_COEFF;
    float T_SRATE_P;
    float T_THR_DAMP;
    float T_INTEG_GAIN;
    float T_VERT_ACC;
    float T_HGT_OMEGA;
    float T_SPD_OMEGA;
    float T_RLL2THR;
    float T_SPDWEIGHT;
    float T_PTCH_DAMP;

    float AIRSPD_MIN;
    float AIRSPD_TRIM;
    float AIRSPD_MAX;

    boolean ARSP_MODE;

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
    float P_LIM_MIN_RADIANS;

    /**
     * Positive pitch limit
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
    float P_LIM_MAX_RADIANS;

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
    float R_LIM_RADIANS;

    float THR_MIN;
    float THR_MAX;
    float THR_IDLE;
    float THR_CRUISE;
    float THR_SLEW_MAX;
    float THR_ALT_SCL;

    /** \brief Roll max in degrees.
     *
     *  \par
     *
     *  \par Limits:
     */
    float MAN_R_MAX_RADIANS;
    /** \brief Pitch max in degrees.
     *
     *  \par
     *
     *  \par Limits:
     */
    float MAN_P_MAX_RADIANS;

    /** \brief Roll Speed offset in degrees.
     *
     *  \par
     *
     *  \par Limits:
     */
    float RSP_OFF_RADIANS;

    /** \brief Pitch Speed offset in degrees.
     *
     *  \par
     *
     *  \par Limits:
     */
    float PSP_OFF_RADIANS;

    float THR_LND_MAX;

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
    float LND_ANG_RADIANS;

    float LND_HVIRT;

    float LND_FLALT;

    float LND_TLALT;
    float LND_HHDIST;

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
    float LND_FL_PMIN_RADIANS;

    /**
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
    float LND_FL_PMAX_RADIANS;

    int32 LND_USETER;

    float LND_AIRSPD_SC;

    FPC_Launch_Detection_t FPC_Launch_Detection;

    FPC_Runway_Takeoff_t FPC_Runway_Takeoff;

} FPC_ConfigTbl_t;

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
