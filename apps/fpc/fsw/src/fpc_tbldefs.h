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


    int32_t ARSP_MODE;

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

    int32_t LND_USETER;

    float LND_AIRSPD_SC;
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
