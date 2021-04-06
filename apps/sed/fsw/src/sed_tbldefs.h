/****************************************************************************
*
*   Copyright (c) 2020 Windhover Labs, L.L.C. All rights reserved.
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

#ifndef SED_TBLDEFS_H
#define SED_TBLDEFS_H

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
#include "sed_platform_cfg.h"

/************************************************************************
** Local Defines
*************************************************************************/

/**
 * \brief Defines the table identification name used for the
 * configuration table registration.
 */
#define SED_CONFIG_TABLENAME ("CONFIG_TBL")

/** \brief Definition for a single config table entry */
typedef struct
{
    /** \brief Acceleration X-axis calibration scale. */
    float AccXScale;
    /** \brief Acceleration Y-axis calibration scale. */
    float AccYScale;
    /** \brief Acceleration Z-axis calibration scale. */
    float AccZScale;
    /** \brief Acceleration X-axis calibration offset. */
    float AccXOffset;
    /** \brief Acceleration Y-axis calibration offset. */
    float AccYOffset;
    /** \brief Acceleration Z-axis calibration offset. */
    float AccZOffset;
    /** \brief Gyro X-axis calibration scale. */
    float GyroXScale;
    /** \brief Gyro Y-axis calibration scale. */
    float GyroYScale;
    /** \brief Gyro Z-axis calibration scale. */
    float GyroZScale;
    /** \brief Gyro X-axis calibration offset. */
    float GyroXOffset;
    /** \brief Gyro Y-axis calibration offset. */
    float GyroYOffset;
    /** \brief Gyro Z-axis calibration offset. */
    float GyroZOffset;
    /** \brief Room temperature offset. */
    float TempOffset;
    /** \brief Temperature sensitivity. */
    float TempSensitivity;
    /** \brief Accel unit value */
    float AccUnit;
    /** \brief Gyro unit value */
    float GyroUnit;
    /** \brief Accel divider value */
    float AccDivider;
    /** \brief Gyro divider value */
    float GyroDivider;
    /** \brief Gyro divider value */
    float AccScale;
    /** \brief Gyro divider value */
    float GyroScale;

} SED_ConfigTbl_t;


#ifdef __cplusplus
}
#endif

#endif /* SED_TBLDEFS_H */

/************************/
/*  End of File Comment */
/************************/
