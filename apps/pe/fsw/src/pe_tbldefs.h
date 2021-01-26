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

#ifndef PE_TBLDEFS_H
#define PE_TBLDEFS_H

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
#include "pe_platform_cfg.h"

/************************************************************************
** Local Defines
*************************************************************************/
#define PE_PARAM_NAME_MAX_LEN     (32)
#define VXY_PUB_THRESH_MIN        (0.01f)
#define VXY_PUB_THRESH_MAX        (1.0f)
#define Z_PUB_THRESH_MIN          (0.3f)
#define Z_PUB_THRESH_MAX          (5.0f)
#define ACCEL_XY_STDDEV_MIN       (0.00001f)
#define ACCEL_XY_STDDEV_MAX       (2.0f)
#define ACCEL_Z_STDDEV_MIN        (0.00001f)
#define ACCEL_Z_STDDEV_MAX        (2.0f)
#define BARO_FUSE_MIN             (0)
#define BARO_FUSE_MAX             (1)
#define BARO_STDDEV_MIN           (0.01f)
#define BARO_STDDEV_MAX           (100.0f)
#define GPS_FUSE_MIN              (0)
#define GPS_FUSE_MAX              (1)
#define GPS_DELAY_MIN             (0.0f)
#define GPS_DELAY_MAX             (0.4f)
#define GPS_XY_STDDEV_MIN         (0.01f)
#define GPS_XY_STDDEV_MAX         (5.0f)
#define GPS_Z_STDDEV_MIN          (0.01f)
#define GPS_Z_STDDEV_MAX          (200.0f)
#define GPS_VXY_STDDEV_MIN        (0.01f)
#define GPS_VXY_STDDEV_MAX        (2)
#define GPS_VZ_STDDEV_MIN         (0.01f)
#define GPS_VZ_STDDEV_MAX         (2)
#define GPS_EPH_MAX_MIN           (1.0f)
#define GPS_EPH_MAX_MAX           (5.0f)
#define GPS_EPV_MAX_MIN           (1.0f)
#define GPS_EPV_MAX_MAX           (5.0f)
#define LAND_FUSE_MIN             (0)
#define LAND_FUSE_MAX             (1)
#define LAND_Z_STDDEV_MIN         (0.001f)
#define LAND_Z_STDDEV_MAX         (10.0f)
#define LAND_VXY_STDDEV_MIN       (0.01f)
#define LAND_VXY_STDDEV_MAX       (10.0f)
#define PN_P_NOISE_DENSITY_MIN    (0.0f)
#define PN_P_NOISE_DENSITY_MAX    (1.0f)
#define PN_V_NOISE_DENSITY_MIN    (0.0f)
#define PN_V_NOISE_DENSITY_MAX    (1.0f)
#define PN_B_NOISE_DENSITY_MIN    (0.0f)
#define PN_B_NOISE_DENSITY_MAX    (1.0f)
#define PN_T_NOISE_DENSITY_MIN    (0.0f)
#define PN_T_NOISE_DENSITY_MAX    (1.0f)
#define T_MAX_GRADE_MIN           (0)
#define T_MAX_GRADE_MAX           (100)
#define FAKE_ORIGIN_MIN           (0)
#define FAKE_ORIGIN_MAX           (1)
#define INIT_ORIGIN_LAT_MIN       (-90.0f)
#define INIT_ORIGIN_LAT_MAX       (90.0f)
#define INIT_ORIGIN_LON_MIN       (-180.0f)
#define INIT_ORIGIN_LON_MAX       (180.0f)
#define DIST_FUSE_MIN             (0)
#define DIST_FUSE_MAX             (1)
#define DIST_STDDEV_MIN           (0.01f)
#define DIST_STDDEV_MAX           (1)
#define DIST_OFF_Z_MIN            (-1)
#define DIST_OFF_Z_MAX            (1)
#define FLOW_FUSE_MIN             (0)
#define FLOW_FUSE_MAX             (1)
#define FLOW_SCALE_MIN            (0.1f)
#define FLOW_SCALE_MAX            (10.0f)
#define FLOW_R_MIN                (0.1f)
#define FLOW_R_MAX                (10.0f)
#define FLOW_RR_MIN               (0.0f)
#define FLOW_RR_MAX               (10.0f)
#define FLOW_QUALITY_MIN_MIN      (0)
#define FLOW_QUALITY_MIN_MAX      (255)
#define FLOW_MIN_AGL_MIN          (0.0f)
#define FLOW_MIN_AGL_MAX          (10.0f)

/**
 * \brief Defines the table identification name used for the
 * configuration table registration.
 */
#define PE_CONFIG_TABLENAME ("CONFIG_TBL")


/** \brief Params table */
typedef struct
{
    float  VXY_PUB_THRESH;

    float  Z_PUB_THRESH;

    float  ACCEL_XY_STDDEV;

    float  ACCEL_Z_STDDEV;

    boolean BARO_FUSE;

    float  BARO_STDDEV;

    boolean GPS_FUSE;

    float  GPS_DELAY;

    float  GPS_XY_STDDEV;

    float  GPS_Z_STDDEV;

    float  GPS_VXY_STDDEV;

    float  GPS_VZ_STDDEV;

    float  GPS_EPH_MAX;

    float  GPS_EPV_MAX;

    boolean LAND_FUSE;

    float  LAND_Z_STDDEV;

    float  LAND_VXY_STDDEV;

    float  PN_P_NOISE_DENSITY;

    float  PN_V_NOISE_DENSITY;

    float  PN_B_NOISE_DENSITY;

    float  PN_T_NOISE_DENSITY;

    float  T_MAX_GRADE;

    int32  FAKE_ORIGIN;

    float  INIT_ORIGIN_LAT;

    float  INIT_ORIGIN_LON;

    boolean DIST_FUSE;

    float DIST_STDDEV;

    float DIST_OFF_Z;

    boolean FLOW_FUSE;

    float FLOW_SCALE;

    float FLOW_R;

    float FLOW_RR;

    uint8 FLOW_QUALITY_MIN;

    float FLOW_MIN_AGL;

} PE_ConfigTbl_t;


#ifdef __cplusplus
}
#endif

#endif /* PE_TBLDEFS_H */

/************************/
/*  End of File Comment */
/************************/
