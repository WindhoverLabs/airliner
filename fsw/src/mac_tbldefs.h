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

#ifndef MAC_TBLDEFS_H
#define MAC_TBLDEFS_H

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
#include "mac_platform_cfg.h"

/************************************************************************
** Local Defines
*************************************************************************/
#define MAC_PARAM_NAME_MAX_LEN   (32)

#define MC_ROLL_P_MIN            (0.0f)
#define MC_ROLL_P_MAX            (12.0f)
#define MC_ROLLRATE_P_MIN        (0.0f)
#define MC_ROLLRATE_P_MAX        (0.5f)
#define MC_ROLLRATE_I_MIN        (0.0f)
#define MC_ROLLRATE_I_MAX        (1.0f) // Note: Not explicitly defined max
#define MC_ROLLRATE_D_MIN        (0.0f)
#define MC_ROLLRATE_D_MAX        (0.01f)
#define MC_RR_INT_LIM_MIN        (0.0f)
#define MC_RR_INT_LIM_MAX        (1.0f) // Note: Not explicitly defined max
#define MC_ROLLRATE_FF_MIN       (0.0f)
#define MC_ROLLRATE_FF_MAX       (10.0f) // Note: Not explicitly defined max
#define MC_PITCH_P_MIN           (0.0f)
#define MC_PITCH_P_MAX           (12.0f)
#define MC_PITCHRATE_P_MIN       (0.0f)
#define MC_PITCHRATE_P_MAX       (0.6f)
#define MC_PITCHRATE_I_MIN       (0.0f)
#define MC_PITCHRATE_I_MAX       (1.0f) // Note: Not explicitly defined max
#define MC_PR_INT_LIM_MIN        (0.0f)
#define MC_PR_INT_LIM_MAX        (1.0f) // Note: Not explicitly defined max
#define MC_PITCHRATE_D_MIN       (0.0f)
#define MC_PITCHRATE_D_MAX       (0.1f) // Note: Not explicitly defined max
#define MC_PITCHRATE_FF_MIN      (0.0f)
#define MC_PITCHRATE_FF_MAX      (10.0f) // Note: Not explicitly defined max
#define MC_TPA_BREAK_P_MIN       (0.0f)
#define MC_TPA_BREAK_P_MAX       (1.0f)
#define MC_TPA_BREAK_I_MIN       (0.0f)
#define MC_TPA_BREAK_I_MAX       (1.0f)
#define MC_TPA_BREAK_D_MIN       (0.0f)
#define MC_TPA_BREAK_D_MAX       (1.0f)
#define MC_TPA_RATE_P_MIN        (0.0f)
#define MC_TPA_RATE_P_MAX        (1.0f)
#define MC_TPA_RATE_I_MIN        (0.0f)
#define MC_TPA_RATE_I_MAX        (1.0f)
#define MC_TPA_RATE_D_MIN        (0.0f)
#define MC_TPA_RATE_D_MAX        (1.0f)
#define MC_YAW_P_MIN             (0.0f)
#define MC_YAW_P_MAX             (5.0f)
#define MC_YAWRATE_P_MIN         (0.0f)
#define MC_YAWRATE_P_MAX         (0.6f)
#define MC_YAWRATE_I_MIN         (0.0)
#define MC_YAWRATE_I_MAX         (1.0f) // Note: Not explicitly defined max
#define MC_YR_INT_LIM_MIN        (0.0f)
#define MC_YR_INT_LIM_MAX        (1.0f) // Note: Not explicitly defined max
#define MC_YAWRATE_D_MIN         (0.0f)
#define MC_YAWRATE_D_MAX         (1.0f) // Note: Not explicitly defined max
#define MC_YAWRATE_FF_MIN        (0.0f)
#define MC_YAWRATE_FF_MAX        (1.0f) // Note: Not explicitly defined max
#define MC_ROLLRATE_MAX_MIN      (0.0f)
#define MC_ROLLRATE_MAX_MAX      (1800.0f)
#define MC_PITCHRATE_MAX_MIN     (0.0f)
#define MC_PITCHRATE_MAX_MAX     (1800.0f)
#define MC_YAWRATE_MAX_MIN       (0.0f)
#define MC_YAWRATE_MAX_MAX       (1800.0f)
#define MC_YAWRAUTO_MAX_MIN      (0.0f)
#define MC_YAWRAUTO_MAX_MAX      (360.0f)
#define MC_ACRO_R_MAX_MIN        (0.0f)
#define MC_ACRO_R_MAX_MAX        (1800.0f)
#define MC_ACRO_P_MAX_MIN        (0.0f)
#define MC_ACRO_P_MAX_MAX        (1800.0f)
#define MC_ACRO_Y_MAX_MIN        (0.0f)
#define MC_ACRO_Y_MAX_MAX        (1800.0f)
#define MC_RATT_TH_MIN           (0.0f)
#define MC_RATT_TH_MAX           (1.0f)
#define MC_ROLL_TC_MIN           (0.15f)
#define MC_ROLL_TC_MAX           (0.25f)
#define MC_PITCH_TC_MIN          (0.15f)
#define MC_PITCH_TC_MAX          (0.25f)
#define MC_BAT_SCALE_EN_MIN      (0)
#define MC_BAT_SCALE_EN_MAX      (1)
#define SENS_BOARD_ROT_MIN       (0)
#define SENS_BOARD_ROT_MAX       (34)
#define SENS_BOARD_X_OFF_MIN     (0.0f)
#define SENS_BOARD_X_OFF_MAX     (10.0f) // Note: Not explicitly defined max
#define SENS_BOARD_Y_OFF_MIN     (0.0f)
#define SENS_BOARD_Y_OFF_MAX     (10.0f) // Note: Not explicitly defined max
#define SENS_BOARD_Z_OFF_MIN     (0.0f)
#define SENS_BOARD_Z_OFF_MAX     (10.0f) // Note: Not explicitly defined max

/**
 * \brief Defines the table identification name used for table registration.
 */
#define MAC_PARAM_TABLENAME ("PARAM_TBL")

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** \brief Definition for a single config table entry */
typedef struct
{
    /** \maccfg 
     *
     *  \brief Roll P gain
     *
     *  \note Default: 6.5
     */
    float MC_ROLL_P;
    
    /** \maccfg 
     *
     *  \brief Roll rate P gain
     *
     *  \note Default: 0.15
     */
    float MC_ROLLRATE_P;
    
    /** \maccfg 
     *
     *  \brief Roll rate I gain
     *
     *  \note Default: 0.05
     */
    float MC_ROLLRATE_I;
    
    /** \maccfg 
     *
     *  \brief Roll rate integrator limit
     *
     *  \note Default: 0.30
     */
    float MC_RR_INT_LIM;
    
    /** \maccfg 
     *
     *  \brief Roll rate D gain
     *
     *  \note Default: 0.003
     */
    float MC_ROLLRATE_D;
    
    /** \maccfg 
     *
     *  \brief Roll rate feedforward
     *
     *  \note Default: 0.0
     */
    float MC_ROLLRATE_FF;
    
    /** \maccfg 
     *
     *  \brief Pitch P gain
     *
     *  \note Default: 6.5
     */
    float MC_PITCH_P;
    
    /** \maccfg 
     *
     *  \brief Pitch rate P gain
     *
     *  \note Default: 0.15
     */
    float MC_PITCHRATE_P;
    
    /** \maccfg 
     *
     *  \brief Pitch rate I gain
     *
     *  \note Default: 0.05
     */
    float MC_PITCHRATE_I;
    
    /** \maccfg 
     *
     *  \brief Pitch rate integrator limit
     *
     *  \note Default: 0.30
     */
    float MC_PR_INT_LIM;
    
    /** \maccfg 
     *
     *  \brief Pitch rate D gain
     *
     *  \note Default: 0.003
     */
    float MC_PITCHRATE_D;
    
    /** \maccfg 
     *
     *  \brief Pitch rate feedforward
     *
     *  \note Default: 0.0
     */
    float MC_PITCHRATE_FF;
    
    /** \maccfg 
     *
     *  \brief TPA P Breakpoint
     *
     *  \note Default: 1.0
     */
    float MC_TPA_BREAK_P;
   
    /** \maccfg 
     *
     *  \brief TPA I Breakpoint
     *
     *  \note Default: 1.0
     */
    float MC_TPA_BREAK_I;
    
    /** \maccfg 
     *
     *  \brief TPA D Breakpoint
     *
     *  \note Default: 1.0
     */
    float MC_TPA_BREAK_D;
    
    /** \maccfg 
     *
     *  \brief TPA Rate P
     *
     *  \note Default: 0.0
     */
    float MC_TPA_RATE_P;
    
    /** \maccfg 
     *
     *  \brief TPA Rate I
     *
     *  \note Default: 0.0
     */
    float MC_TPA_RATE_I;
    
    /** \maccfg 
     *
     *  \brief TPA Rate D
     *
     *  \note Default: 0.0
     */
    float MC_TPA_RATE_D;

    /** \maccfg 
     *
     *  \brief Yaw P gain
     *
     *  \note Default: 2.8
     */
    float MC_YAW_P;
    
    /** \maccfg 
     *
     *  \brief Yaw rate P gain
     *
     *  \note Default: 0.2
     */
    float MC_YAWRATE_P;
    
    /** \maccfg 
     *
     *  \brief Yaw rate I gain
     *
     *  \note Default: 0.1
     */
    float MC_YAWRATE_I;
    
    /** \maccfg 
     *
     *  \brief Yaw rate integrator limit
     *
     *  \note Default: 0.30
     */
    float MC_YR_INT_LIM;
    
    /** \maccfg 
     *
     *  \brief Yaw rate D gain
     *
     *  \note Default: 0.0
     */
    float MC_YAWRATE_D;
    
    /** \maccfg 
     *
     *  \brief Yaw rate feedforward
     *
     *  \note Default: 0.0
     */
    float MC_YAWRATE_FF;
    
    /** \maccfg 
     *
     *  \brief Max roll rate
     *
     *  \note Default: 220.0
     */
    float MC_ROLLRATE_MAX;
    
    /** \maccfg 
     *
     *  \brief Max pitch rate
     *
     *  \note Default: 220.0
     */
    float MC_PITCHRATE_MAX;
    
    /** \maccfg 
     *
     *  \brief Max yaw rate
     *
     *  \note Default: 200.0	
     */
    float MC_YAWRATE_MAX;
    
    /** \maccfg 
     *
     *  \brief Max yaw rate in auto mode
     *
     *  \note Default: 45.0
     */
    float MC_YAWRAUTO_MAX;
    
    /** \maccfg 
     *
     *  \brief Max acro roll rate
     *
     *  \note Default: 720.0
     */
    float MC_ACRO_R_MAX;
    
    /** \maccfg 
     *
     *  \brief Max acro pitch rate
     *
     *  \note Default: 720.0
     */
    float MC_ACRO_P_MAX;
    
    /** \maccfg 
     *
     *  \brief Max acro yaw rate
     *
     *  \note Default: 540.0
     */
    float MC_ACRO_Y_MAX;
    
    /** \maccfg 
     *
     *  \brief Threshold for Rattitude mode
     *
     *  \note Default: 0.8
     */
    float MC_RATT_TH;
    
    /** \maccfg 
     *
     *  \brief Attitude Roll Time Constant
     *
     *  \note Default: 0.2
     */
    float MC_ROLL_TC;
    
    /** \maccfg 
     *
     *  \brief Attitude pitch time constant
     *
     *  \note Default: 0.2
     */
    float MC_PITCH_TC;
    
    /** \maccfg 
     *
     *  \brief Battery power level scaler
     *
     *  \note Default: 0
     */
    int32 MC_BAT_SCALE_EN;
    
    /** \maccfg 
     *
     *  \brief Board rotation
     *
     *  \note Default: 0
     */
    int32 SENS_BOARD_ROT;
    
    /** \maccfg 
     *
     *  \brief Board rotation X (Roll) offset
     *
     *  \note Default: 0.0
     */
    float SENS_BOARD_X_OFF;
    
    /** \maccfg 
     *
     *  \brief Board rotation Y (Pitch) offset
     *
     *  \note Default: 0.0
     */
    float SENS_BOARD_Y_OFF;
    
    /** \maccfg 
     *
     *  \brief Board rotation Z (YAW) offset
     *
     *  \note Default: 0.0
     */
    float SENS_BOARD_Z_OFF;
    
} MAC_ParamTbl_t;

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

#endif /* MAC_TBLDEFS_H */

/************************/
/*  End of File Comment */
/************************/
