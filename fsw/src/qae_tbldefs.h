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
#ifndef QAE_TBLDEFS_H
#define QAE_TBLDEFS_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "qae_platform_cfg.h"

/************************************************************************
** Local Defines
*************************************************************************/

/**
 * \brief Defines the table identification name used for the
 * configuration table registration.
 */
#define QAE_CONFIG_TABLENAME ("CONFIG_TBL")

#define ATT_W_ACC_MIN           (0.0f)
#define ATT_W_ACC_MAX           (1.0f)
#define ATT_W_MAG_MIN           (0.0f)
#define ATT_W_MAG_MAX           (1.0f)
#define ATT_W_GYRO_BIAS_MIN     (0.0f)
#define ATT_W_GYRO_BIAS_MAX     (1.0f)
#define ATT_MAG_DECL_MIN        (0.0f)
#define ATT_MAG_DECL_MAX        (180.0f)
#define ATT_MAG_DECL_A_MIN      (0)
#define ATT_MAG_DECL_A_MAX      (1)
#define ATT_ACC_COMP_MIN        (0)
#define ATT_ACC_COMP_MAX        (1)
#define ATT_BIAS_MAX_MIN        (0.0f)
#define ATT_BIAS_MAX_MAX        (2.0f)

/** \brief Definition for a single config table entry */
typedef struct
{
    /** \brief Complimentary filter accelerometer weight.
     * 
     *  \par Limits:
     *       Min > Max (incr.) #ATT_W_ACC_MIN > #ATT_W_ACC_MAX, default 0.2.
     */
    float    ATT_W_ACC;
    
    /** \brief Complimentary filter magnetometer weight.
     * 
     *  \par Limits:
     *       Min > Max (incr.) #ATT_W_MAG_MIN > #ATT_W_MAG_MAX, default 0.1.
     */
    float    ATT_W_MAG;
    
    /** \brief Complimentary filter gyroscope bias weight.
     * 
     *  \par Limits:
     *       Min > Max (incr.) #ATT_W_GYRO_BIAS_MIN > #ATT_W_GYRO_BIAS_MAX, default 0.1.
     */
    float    ATT_W_GYRO_BIAS;
    
    /** \brief Magnetic declination, in degrees.
     * 
     *  \par Limits:
     *       Min > Max (incr.) #ATT_MAG_DECL_MIN > #ATT_MAG_DECL_MAX, default 0.0.
     */
    float    ATT_MAG_DECL;
    
    /** \brief Automatic GPS based declination compensation.
     * 
     *  \par Limits:
     *       TRUE or FALSE, default 1 (TRUE).
     */
    boolean  ATT_MAG_DECL_A;
    
    /** \brief Acceleration compensation based on GPS velocity.
     * 
     *  \par Limits:
     *       TRUE or FALSE, default 1 (TRUE).
     */
    boolean  ATT_ACC_COMP;
    
    /** \brief Gyro bias limit in rad/s.
     * 
     *  \par Limits:
     *       Min > Max (incr.) #ATT_BIAS_MAX_MIN > #ATT_BIAS_MAX_MAX, default 0.05.
     */
    float    ATT_BIAS_MAX;
} QAE_ConfigTbl_t;


#ifdef __cplusplus
}
#endif

#endif /* QAE_TBLDEFS_H */

/************************/
/*  End of File Comment */
/************************/
