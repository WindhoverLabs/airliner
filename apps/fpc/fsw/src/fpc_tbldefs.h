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
    float LAUN_CAT_PMAX;

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
//    PARAM_DEFINE_INT32(RWTO_TKOFF, 0);

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
//    PARAM_DEFINE_INT32(RWTO_HDG, 0);

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
//    PARAM_DEFINE_FLOAT(RWTO_NAV_ALT, 5.0);

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
//    PARAM_DEFINE_FLOAT(RWTO_MAX_THR, 1.0);

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
    float PSP;
//    PARAM_DEFINE_FLOAT(RWTO_PSP, 0.0);

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
     float MAX_PITCH;
//    PARAM_DEFINE_FLOAT(RWTO_MAX_PITCH, 20.0);

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
     float MAX_ROLL;
//    PARAM_DEFINE_FLOAT(RWTO_MAX_ROLL, 25.0);

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
//    PARAM_DEFINE_FLOAT(RWTO_AIRSPD_SCL, 1.3);
}FPC_Runway_Takeoff_t;

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


    int32 ARSP_MODE;

    float P_LIM_MIN;
    float P_LIM_MAX;
    float R_LIM;

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
    float MAN_R_MAX;
    /** \brief Pitch max in degrees.
     *
     *  \par
     *
     *  \par Limits:
     */
    float MAN_P_MAX;

    /** \brief Roll Speed offset in degrees.
     *
     *  \par
     *
     *  \par Limits:
     */
    float RSP_OFF;

    /** \brief Pitch Speed offset in degrees.
     *
     *  \par
     *
     *  \par Limits:
     */
    float PSP_OFF;

    float THR_LND_MAX;

    float LND_ANG;

    float LND_HVIRT;

    float LND_FLALT;

    float LND_TLALT;
    float LND_HHDIST;

    /** \brief in degrees.
     *
     *  \par
     *
     *  \par Limits:
     */
    float LND_FL_PMIN;


    /** \brief in degrees.
     *
     *  \par
     *
     *  \par Limits:
     */
    float LND_FL_PMAX;

    int32 LND_USETER;

    float LND_AIRSPD_SC;

    FPC_Launch_Detection_t FPC_Launch_Detection;

} FPC_ConfigTbl_t;



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
