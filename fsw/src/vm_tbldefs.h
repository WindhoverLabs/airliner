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

#ifndef VM_TBLDEFS_H
#define VM_TBLDEFS_H

/************************************************************************
 ** Pragmas
 *************************************************************************/

/************************************************************************
 ** Includes
 *************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#include "cfe.h"
#include "vm_platform_cfg.h"

/************************************************************************
 ** Local Defines
 *************************************************************************/

/**
 * \brief Defines the table identification name used for the
 * configuration table registration.
 */
#define VM_CONFIG_TABLENAME ("CONFIG_TBL")

/** \brief Definition for a single config table entry */
typedef struct
{ /** \vmcfg SYS_AUTOSTART
 *
 *  \brief Auto-start script index
 *
 *  \par Limits:
 *       Min > Max (incr.) 0 > 99999, default 0.
 */
    int SYS_AUTOSTART;
    /** \vmcfg COM_RC_IN_MODE
     *
     *  \brief RC control input mode
     *
     *  \par Limits:
     *       Min > Max (incr.) 0 > 2, default 0.
     */
    int COM_RC_IN_MODE;
    /** \vmcfg COM_ARM_SWISBTN
     *
     *  \brief Arm switch is only a button
     *
     *  \par Limits:
     *       Min > Max (incr.) 0 > 1, default 0.
     */
    int COM_ARM_SWISBTN;
    /** \vmcfg COM_ARM_WO_GPS
     *
     *  \brief Allow arming without GPS
     *
     *  \par Limits:
     *       default 1.
     */
    int COM_ARM_WO_GPS;
    /** \vmcfg COM_ARM_MIS_REQ
     *
     *  \brief Require valid mission to arm
     *
     *  \par Limits:
     *       default 0.
     */
    int COM_ARM_MIS_REQ;
    /** \vmcfg COM_RC_ARM_HYST
     *
     *  \brief RC input arm/disarm command duration
     *
     *  \par Limits:
     *       Min > Max (incr.) 100 > 1500, default 1000.
     */
    int COM_RC_ARM_HYST;
    /** \vmcfg MAV_TYPE
     *
     *  \brief Airframe type \n
     *          0: Generic micro air vehicle \n
     *          1: Fixed wing aircraft \n
     *          2: Quadrotor \n
     *          3: Coaxial helicopter \n
     *          4: Normal helicopter with tail rotor \n
     *          5: Ground installation \n
     *          6: Operator control unit / ground control station \n
     *          7: Airship, controlled \n
     *          8: Free balloon, uncontrolled \n
     *          9: Rocket \n
     *          10: Ground rover \n
     *          11: Surface vessel, boat, ship \n
     *          12: Submarine \n
     *          13: Hexarotor \n
     *          14: Octorotor \n
     *          15: Tricopter \n
     *          16: Flapping wing \n
     *          17: Kite \n
     *          18: Onboard companion controller \n
     *          19: Two-rotor VTOL using control surfaces in vertical operation in addition. Tailsitter. \n
     *          20: Quad-rotor VTOL using a V-shaped quad config in vertical operation. Tailsitter. \n
     *          21: Tiltrotor VTOL \n
     *          22: VTOL reserved 2 \n
     *          23: VTOL reserved 3 \n
     *          24: VTOL reserved 4 \n
     *          25: VTOL reserved 5 \n
     *          26: Onboard gimbal \n
     *          27: Onboard ADSB peripheral
     *  \par Limits:
     *       Min > Max (incr.) 1 > 27, default 2.
     */
    int MAV_TYPE;
    /** \vmcfg MAV_SYS_ID
     *
     *  \brief System ID
     *
     *  \par Limits:
     *       Min > Max (incr.) 1 > 250, default 1.
     */
    int MAV_SYS_ID;
    /** \vmcfg MAV_COMP_ID
     *
     *  \brief Component ID
     *
     *  \par Limits:
     *       Min > Max (incr.) 1 > 250, default 1.
     */
    int MAV_COMP_ID;
    /** \vmcfg CBRK_SUPPLY_CHK
     *
     *  \brief Circuit breaker for power supply check
     *
     *  \par Limits:
     *       Min > Max (incr.) 0 > 894281, default 0.
     */
    int CBRK_SUPPLY_CHK;
    /** \vmcfg CBRK_USB_CHK
     *
     *  \brief Circuit breaker for USB link check
     *
     *  \par Limits:
     *       Min > Max (incr.) 0 > 197848, default 0.
     */
    int CBRK_USB_CHK;
    /** \vmcfg CBRK_AIRSPD_CHK
     *
     *  \brief Circuit breaker for airspeed sensor
     *
     *  \par Limits:
     *       Min > Max (incr.) 0 > 162128, default 0.
     */
    int CBRK_AIRSPD_CHK;
    /** \vmcfg CBRK_ENGINEFAIL
     *
     *  \brief Circuit breaker for engine failure detection
     *
     *  \par Limits:
     *       Min > Max (incr.) 0 > 284953, default 0.
     */
    int CBRK_ENGINEFAIL;
    /** \vmcfg CBRK_GPSFAIL
     *
     *  \brief Circuit breaker for GPS failure detection
     *
     *  \par Limits:
     *       Min > Max (incr.) 0 > 240024, default 0.
     */
    int CBRK_GPSFAIL;
    /** \vmcfg CBRK_FLIGHTTERM
     *
     *  \brief Circuit breaker for flight termination
     *
     *  \par Limits:
     *       Min > Max (incr.) 0 > 121212, default 121212.
     */
    int CBRK_FLIGHTTERM;
    /** \vmcfg CBRK_VELPOSERR
     *
     *  \brief Circuit breaker for position error check
     *
     *  \par Limits:
     *       Min > Max (incr.) 0 > 201607, default 0.
     */
    int CBRK_VELPOSERR;
    /** \vmcfg NAV_DLL_ACT
     *
     *  \brief Set data link loss failsafe mode
     *
     *  \par Limits:
     *       default 0.
     */
    int NAV_DLL_ACT;
    /** \vmcfg NAV_RCL_ACT
     *
     *  \brief Set RC loss failsafe mode \n
     *          0: Disabled \n
     *          1: Loiter \n
     *          2: Return to Land \n
     *          3: Land at current position \n
     *          4: RC Auto Recovery (CASA Outback Challenge rules) \n
     *          5: Terminate \n
     *          6: Lockdown
     *  \par Limits:
     *       default 2.
     */
    int NAV_RCL_ACT;
    /** \vmcfg COM_DL_LOSS_T
     *
     *  \brief Datalink loss time threshold
     *
     *  \par Limits:
     *       Min > Max (incr.) 5 > 300 , default 10.
     */
    int COM_DL_LOSS_T;
    /** \vmcfg COM_RC_LOSS_T
     *
     *  \brief RC loss time threshold
     *
     *  \par Limits:
     *       Min > Max (incr.) 0 > 35, default 0.5.
     */
    float COM_RC_LOSS_T;
    /** \vmcfg COM_RC_STICK_OV
     *
     *  \brief RC stick override threshold
     *
     *  \par Limits:
     *       Min > Max (incr.) 5 > 40, default 12.0.
     */
    float COM_RC_STICK_OV;
    /** \vmcfg COM_RC_OVERRIDE
     *
     *  \brief Enable RC stick override of auto modes
     *
     *  \par Limits:
     *       default 0.
     */
    int COM_RC_OVERRIDE;
    /** \vmcfg COM_DL_REG_T
     *
     *  \brief Datalink regain time threshold
     *
     *  \par Limits:
     *       Min > Max (incr.) 0 > 3 , default 0.
     */
    int COM_DL_REG_T;
    /** \vmcfg COM_EF_THROT
     *
     *  \brief Engine Failure Throttle Threshold
     *
     *  \par Limits:
     *       Min > Max (incr.) 0.0 > 1.0 , default 0.5.
     */
    float COM_EF_THROT;
    /** \vmcfg COM_EF_C2T
     *
     *  \brief Engine Failure Current/Throttle Threshold
     *
     *  \par Limits:
     *       Min > Max (incr.) 0.0 > 50.0, default 5.0.
     */
    float COM_EF_C2T;
    /** \vmcfg COM_EF_TIME
     *
     *  \brief Engine Failure Time Threshold
     *
     *  \par Limits:
     *       Min > Max (incr.) 0.0 > 60.0, default 10.0.
     */
    float COM_EF_TIME;
    /** \vmcfg GF_ACTION
     *
     *  \brief Geofence violation action
     *
     *  \par Limits:
     *       Min > Max (incr.) 0 > 4, default 1.
     */
    int GF_ACTION;
    /** \vmcfg COM_DISARM_LAND
     *
     *  \brief Time-out for auto disarm after landing
     *
     *  \par Limits:
     *       Min > Max (incr.) 0 > 20, default 0.
     */
    int COM_DISARM_LAND;
    /** \vmcfg COM_LOW_BAT_ACT
     *
     *  \brief Battery failsafe mode
     *
     *  \par Limits:
     *       default 0.
     */
    int COM_LOW_BAT_ACT;
    /** \vmcfg COM_OF_LOSS_T
     *
     *  \brief Time-out to wait when offboard connection is lost before triggering offboard lost action
     *
     *  \par Limits:
     *       Min > Max (incr.) 0 > 60, default 0.0.
     */
    float COM_OF_LOSS_T;
    /** \vmcfg COM_OBL_ACT
     *
     *  \brief Set offboard loss failsafe mode \n
     *          0: Land at current position \n
     *          1: Loiter \n
     *          2: Return to Land
     *  \par Limits:
     *       default 0.
     */
    int COM_OBL_ACT;
    /** \vmcfg COM_OBL_RC_ACT
     *
     *  \brief Set offboard loss failsafe mode when RC is available \n
     *          0: Position control \n
     *          1: Altitude control \n
     *          2: Manual \n
     *          3: Return to Land \n
     *          4: Land at current position \n
     *          5: Loiter
     *  \par Limits:
     *       default 0.
     */
    int COM_OBL_RC_ACT;
    /** \vmcfg COM_HOME_H_T
     *
     *  \brief Home set horizontal threshold
     *
     *  \par Limits:
     *       Min > Max (incr.) 2 > 15, default 5.0.
     */
    float COM_HOME_H_T;
    /** \vmcfg COM_HOME_V_T
     *
     *  \brief Home set vertical threshold
     *
     *  \par Limits:
     *       Min > Max (incr.) 5 > 25, default 10.0.
     */
    float COM_HOME_V_T;
    /** \vmcfg COM_FLTMODE1
     *
     *  \brief First flightmode slot (1000-1160) \n
     *          -1: Unassigned \n
     *          0: Manual \n
     *          1: Altitude \n
     *          2: Position \n
     *          3: Mission \n
     *          4: Hold \n
     *          5: Return \n
     *          6: Acro \n
     *          7: Offboard \n
     *          8: Stabilized \n
     *          9: Rattitude \n
     *          10: Takeoff \n
     *          11: Land \n
     *          12: Follow Me
     *  \par Limits:
     *       default -1.
     */
    int COM_FLTMODE1;
    /** \vmcfg COM_FLTMODE2
     *
     *  \brief Second flightmode slot (1160-1320) \n
     *          -1: Unassigned \n
     *          0: Manual \n
     *          1: Altitude \n
     *          2: Position \n
     *          3: Mission \n
     *          4: Hold \n
     *          5: Return \n
     *          6: Acro \n
     *          7: Offboard \n
     *          8: Stabilized \n
     *          9: Rattitude \n
     *          10: Takeoff \n
     *          11: Land \n
     *          12: Follow Me
     *  \par Limits:
     *       default -1.
     */
    int COM_FLTMODE2;
    /** \vmcfg COM_FLTMODE3
     *
     *  \brief Third flightmode slot (1320-1480) \n
     *          -1: Unassigned \n
     *          0: Manual \n
     *          1: Altitude \n
     *          2: Position \n
     *          3: Mission \n
     *          4: Hold \n
     *          5: Return \n
     *          6: Acro \n
     *          7: Offboard \n
     *          8: Stabilized \n
     *          9: Rattitude \n
     *          10: Takeoff \n
     *          11: Land \n
     *          12: Follow Me
     *  \par Limits:
     *       default -1.
     */
    int COM_FLTMODE3;
    /** \vmcfg COM_FLTMODE4
     *
     *  \brief Fourth flightmode slot (1480-1640) \n
     *          -1: Unassigned \n
     *          0: Manual \n
     *          1: Altitude \n
     *          2: Position \n
     *          3: Mission \n
     *          4: Hold \n
     *          5: Return \n
     *          6: Acro \n
     *          7: Offboard \n
     *          8: Stabilized \n
     *          9: Rattitude \n
     *          10: Takeoff \n
     *          11: Land \n
     *          12: Follow Me
     *  \par Limits:
     *       default -1.
     */
    int COM_FLTMODE4;
    /** \vmcfg COM_FLTMODE5
     *
     *  \brief Fifth flightmode slot (1640-1800) \n
     *          -1: Unassigned \n
     *          0: Manual \n
     *          1: Altitude \n
     *          2: Position \n
     *          3: Mission \n
     *          4: Hold \n
     *          5: Return \n
     *          6: Acro \n
     *          7: Offboard \n
     *          8: Stabilized \n
     *          9: Rattitude \n
     *          10: Takeoff \n
     *          11: Land \n
     *          12: Follow Me
     *  \par Limits:
     *       default -1.
     */
    int COM_FLTMODE5;
    /** \vmcfg COM_FLTMODE6
     *
     *  \brief Sixth flightmode slot (1800-2000) \n
     *          -1: Unassigned \n
     *          0: Manual \n
     *          1: Altitude \n
     *          2: Position \n
     *          3: Mission \n
     *          4: Hold \n
     *          5: Return \n
     *          6: Acro \n
     *          7: Offboard \n
     *          8: Stabilized \n
     *          9: Rattitude \n
     *          10: Takeoff \n
     *          11: Land \n
     *          12: Follow Me
     *  \par Limits:
     *       default -1.
     */
    int COM_FLTMODE6;
    /** \vmcfg COM_ARM_EKF_POS
     *
     *  \brief Maximum EKF position innovation test ratio that will allow arming
     *
     *  \par Limits:
     *       Min > Max (incr.) 0.1 > 1.0, default 0.5.
     */
    float COM_ARM_EKF_POS;
    /** \vmcfg COM_ARM_EKF_VEL
     *
     *  \brief Maximum EKF velocity innovation test ratio that will allow arming
     *
     *  \par Limits:
     *       Min > Max (incr.) 0.1 > 1.0, default 0.5.
     */
    float COM_ARM_EKF_VEL;
    /** \vmcfg COM_ARM_EKF_HGT
     *
     *  \brief Maximum EKF height innovation test ratio that will allow arming
     *
     *  \par Limits:
     *       Min > Max (incr.) 0.1 > 1.0, default 1.0.
     */
    float COM_ARM_EKF_HGT;
    /** \vmcfg COM_ARM_EKF_YAW
     *
     *  \brief Maximum EKF yaw innovation test ratio that will allow arming
     *
     *  \par Limits:
     *       Min > Max (incr.) 0.1 > 1.0, default 0.5.
     */
    float COM_ARM_EKF_YAW;
    /** \vmcfg COM_ARM_EKF_AB
     *
     *  \brief Maximum value of EKF gyro delta angle bias estimate that will allow arming
     *
     *  \par Limits:
     *       Min > Max (incr.) 0.001 > 0.01, default 2.4e-3 .
     */
    float COM_ARM_EKF_AB;
    /** \vmcfg COM_ARM_EKF_GB
     *
     *  \brief Maximum value of EKF gyro delta angle bias estimate that will allow arming
     *
     *  \par Limits:
     *       Min > Max (incr.) 0.0001 > 0.0017, default 8.7e-4 .
     */
    float COM_ARM_EKF_GB;
    /** \vmcfg COM_ARM_IMU_ACC
     *
     *  \brief Maximum accelerometer inconsistency between IMU units that will allow arming
     *
     *  \par Limits:
     *       Min > Max (incr.) 0.1 > 1.0, default 0.7.
     */
    float COM_ARM_IMU_ACC;
    /** \vmcfg COM_ARM_IMU_GYR
     *
     *  \brief Maximum rate gyro inconsistency between IMU units that will allow arming
     *
     *  \par Limits:
     *       Min > Max (incr.) 0.02 > 0.3, default 0.25.
     */
    float COM_ARM_IMU_GYR;
    /** \vmcfg COM_POSCTL_NAVL
     *
     *  \brief Position control navigation loss response
     *
     *  \par Limits:
     *       default 0.
     */
    int COM_POSCTL_NAVL;
    /** \vmcfg HOME_POS_ALT_PADDING
     *
     *  \brief Padding for home position altitude message, to avoid hover above ground.
     *
     *  \par Limits:
     *       default 1.
     */
    int HOME_POS_ALT_PADDING;

}VM_ConfigTbl_t;

#ifdef __cplusplus
}
#endif

#endif /* VM_TBLDEFS_H */

/************************/
/*  End of File Comment */
/************************/
