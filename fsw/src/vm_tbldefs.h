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
{
    /** \vmcfg COM_RC_IN_MODE
     *
     *  \brief RC control input mode
     *
     *  \par Limits:
     *       Min > Max (incr.) 0 > #COM_RC_IN_MODE_MAX.
     */
    uint32 COM_RC_IN_MODE;

    /** \vmcfg COM_ARM_SWISBTN
     *
     *  \brief Arm switch is only a button
     *
     *  \par Limits:
     *       Min > Max (incr.) 0 > 1, default 0.
     */
    uint32 COM_ARM_SWISBTN;

    /** \vmcfg COM_RC_ARM_HYST
     *
     *  \brief RC input arm/disarm command duration
     *
     *  \par Limits:
     *       Min > Max (incr.) #COM_RC_ARM_HYST_MIN > #COM_RC_ARM_HYST_MAX.
     */
    uint32 COM_RC_ARM_HYST;

    /** \vmcfg MAV_SYS_ID
     *
     *  \brief System ID
     *
     *  \par Limits:
     *       Min > Max (incr.) #MAV_SYS_ID_MIN > #MAV_SYS_ID_MAX.
     */
    uint32 MAV_SYS_ID;

    /** \vmcfg MAV_COMP_ID
     *
     *  \brief Component ID
     *
     *  \par Limits:
     *       Min > Max (incr.) #MAV_COMP_ID_MIN > #MAV_COMP_ID_MAX.
     */
    uint32 MAV_COMP_ID;

    /** \vmcfg COM_RC_LOSS_T
     *
     *  \brief RC loss time threshold
     *
     *  \par Limits:
     *       Min > Max (incr.) 0.0 > #COM_RC_LOSS_T_MAX.
     */
    float COM_RC_LOSS_T;

    /** \vmcfg COM_LOW_BAT_ACT
     *
     *  \brief Battery failsafe mode
     *
     *  \par Limits:
     *       Min > Max (incr.) 0 > #COM_LOW_BAT_ACT
     */
    uint32 COM_LOW_BAT_ACT;

    /** \vmcfg COM_HOME_H_T
     *
     *  \brief Home set horizontal threshold
     *
     *  \par Limits:
     *       Min > Max (incr.) #COM_HOME_H_T_MIN > #COM_HOME_H_T_MAX
     */
    float COM_HOME_H_T;

    /** \vmcfg COM_HOME_V_T
     *
     *  \brief Home set vertical threshold
     *
     *  \par Limits:
     *       Min > Max (incr.) #COM_HOME_V_T_MIN > #COM_HOME_V_T_MAX
     */
    float COM_HOME_V_T;

    /** \vmcfg HOME_POS_ALT_PADDING
     *
     *  \brief Padding for home position altitude message, to avoid hover above ground.
     *
     *  \par Limits:
     *       None.
     */
    float HOME_POS_ALT_PADDING;

}VM_ConfigTbl_t;

#ifdef __cplusplus
}
#endif

#endif /* VM_TBLDEFS_H */

/************************/
/*  End of File Comment */
/************************/
