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

#ifndef LD_TBLDEFS_H
#define LD_TBLDEFS_H

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
#include "ld_platform_cfg.h"

/************************************************************************
 ** Local Defines
 *************************************************************************/
#define LD_PARAM_NAME_MAX_LEN     (32)
#define LD_FFALL_THR_MIN                 (0.1f)
#define LD_FFALL_THR_MAX                 (10.0f)
#define LD_FFALL_TTRI_MIN                (0.02f)
#define LD_FFALL_TTRI_MAX                (5.0f)
#define LD_MAN_DWNTHR_MIN                (0.0f)
#define LD_MAN_DWNTHR_MAX                (1.0f)
#define LD_ALT_MAX_MIN                   (-1.0f)
#define LD_ALT_MAX_MAX                   (10000)
#define LD_LOW_T_THR_MIN                 (0.1f)
#define LD_LOW_T_THR_MAX                 (0.9f)
#define LD_MAN_MIN_THR_MIN               (0.0f)
#define LD_MAN_MIN_THR_MAX               (1.0f)
#define LD_POS_STK_UP_THRES_MIN          (0.5f)
#define LD_POS_STK_UP_THRES_MAX          (1.0f)
#define LD_POS_STK_DW_THRES_MIN          (0.0f)
#define LD_POS_STK_DW_THRES_MAX          (0.5f)
#define LD_LANDSPEED_MIN                 (0.0f)
#define LD_LANDSPEED_MAX                 (10.0f)
#define LD_MIN_THR_NO_ALT_TIMEOUT_MIN    (0000001)
#define LD_MIN_THR_NO_ALT_TIMEOUT_MAX    (9000000)

/**
 * \brief Defines the table identification name used for the
 * configuration table registration.
 */
#define LD_CONFIG_TABLENAME ("CONFIG_TBL")

/** \brief Definition for a single config table entry */
typedef struct
{
    /** \ldcfg LD_Z_VEL_MAX
     *
     *  \brief Multicopter max climb rate.
     *
     *  \par Limits:
     *      default 0.5.
     */
    float LD_Z_VEL_MAX;

    /** \ldcfg LD_XY_VEL_MAX
     *
     *  \brief Multicopter max horizontal velocity.
     *
     *  \par Limits:
     *      default 1.5.
     */
    float LD_XY_VEL_MAX;

    /** \ldcfg LD_ROT_MAX
     *
     *  \brief Multicopter max rotation.
     *
     *  \par Limits:
     *      default 20.0.
     */
    float LD_ROT_MAX;

    /** \ldcfg LD_FFALL_THR
     *
     *  \brief Multicopter specific force threshold.
     *
     *  \par Limits:
     *      Min > Max (incr.) 0.1 > 10 , default 2.0.
     */
    float LD_FFALL_THR;

    /** \ldcfg LD_FFALL_TTRI
     *
     *  \brief Multicopter free-fall trigger time.
     *
     *  \par Limits:
     *      Min > Max (incr.) 0.02 > 5 , default 0.3.
     */
    float LD_FFALL_TTRI;

    /** \ldcfg LD_MAN_DWNTHR
     *
     *  \brief Multicopter Flight stick down threshold for landing.
     *
     *  \par Limits:
     *      default 0.15.
     */
    float LD_MAN_DWNTHR;

    /** \ldcfg LD_ALT_MAX
     *
     *  \brief Multicopter maximum altitude.
     *
     *  \par Limits:
     *      default 10000.0.
     */
    float LD_ALT_MAX;

    /** \ldcfg LD_LOW_T_THR
     *
     *  \brief Multicopter low throttle threshold
     *
     *  \par Limits:
     *      default 0.3
     */
    float LD_LOW_T_THR;

    /** \ldcfg LD_MAN_MIN_THR
     *
     *  \brief Multicopter minimum throttle in manual mode.
     *
     *  \par Limits:
     *      default 0.08.
     */
    float LD_MAN_MIN_THR;

    /** \ldcfg LD_POS_STK_UP_THRES
     *
     *  \brief Multicopter takeoff stick up threshold in position control mode.
     *
     *  \par Limits:
     *      default 0.65.
     */
    float LD_POS_STK_UP_THRES;

    /** \ldcfg LD_POS_STK_DW_THRES
     *
     *  \brief Multicopter takeoff stick down threshold in position control mode.
     *
     *  \par Limits:
     *      default 0.15.
     */
    float LD_POS_STK_DW_THRES;

    /** \ldcfg LD_LANDSPEED
     *
     *  \brief Landing descend rate.
     *
     *  \par Limits:
     *      default 0.5f.
     */
    float LD_LANDSPEED;
    
    /** \ldcfg LD_MIN_THR_NO_ALT_TIMEOUT
     *
     *  \brief Timeout for detecting land when using min thrust with no altitude lock.
     *
     *  \par Limits:
     *      default 8000000.
     */
    uint64 LD_MIN_THR_NO_ALT_TIMEOUT;
    
}LD_ConfigTbl_t;

#ifdef __cplusplus
}
#endif

#endif /* LD_TBLDEFS_H */

/************************/
/*  End of File Comment */
/************************/
