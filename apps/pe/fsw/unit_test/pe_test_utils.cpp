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

#include "cfe.h"
#include "pe_test_utils.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_tbl_hooks.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_cfe_time_stubs.h"
#include "ut_osapi_stubs.h"
#include "ut_osfileapi_stubs.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_evs_stubs.h"
#include "pe_stubs.h"
#include "pe_block_delay_stubs.hpp"

#include <time.h>

extern PE_ConfigTbl_t PE_ConfigTbl;

/*
 * Config table for testing
 */
/* Default ULR config table data */
//PE_ConfigTbl_t PE_ConfigTbl =
//{
//	/* TODO:  Define table. */
//	//VXY_PUB_THRESH
//    .3f,
//	//Z_PUB_THRESH
//    1.0f,
//	//ACCEL_XY_STDDEV
//	.012f,
//	//ACCEL_Z_STDDEV
//    .02f,
//	//BARO_FUSE
//    1,
//	//BARO_STDDEV
//    3.0f,
//	//GPS_FUSE
//    1,
//	//GPS_DELAY
//    .29f,
//	//GPS_XY_STDDEV
//    1.0f,
//	//GPS_Z_STDDEV
//    3.0f,
//	//GPS_VXY_STDDEV
//    .25f,
//	//GPS_VZ_STDDEV
//    .25f,
//	//GPS_EPH_MAX
//    3.0f,
//	//GPS_EPV_MAX
//    5.0f,
//	//LAND_FUSE
//    1,
//	//LAND_Z_STDDEV
//    .03f,
//	//LAND_VXY_STDDEV
//    .05f,
//	//PN_P_NOISE_DENSITY
//    .1f,
//	//PN_V_NOISE_DENSITY
//    .1f,
//	//PN_B_NOISE_DENSITY
//    .001f,
//	//PN_T_NOISE_DENSITY
//    .001f,
//	//T_MAX_GRADE
//	0.0f,
//	//FAKE_ORIGIN
//    0,
//	//INIT_ORIGIN_LAT
//    29.562144f,
//	//INIT_ORIGIN_LON
//    -95.144554,
//    //DIST_FUSE
//    TRUE,
//    //DIST_STDDEV
//    .05f,
//    //DIST_OFF_Z
//    0.0f,
//    //FLOW_FUSE
//    0,
//    //FLOW_SCALE
//	1.3f,
//    //FLOW_R
//	7.0f,
//    //FLOW_RR
//	7.0f,
//    //FLOW_QUALITY_MIN
//	150,
//    //FLOW_MIN_AGL
//	0.5f
//};

/*
 * Function Definitions
 */

void PE_Test_Setup(void)
{
    /* initialize test environment to default state for every test */

    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_TIME_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();

    /* Clear mocked return values */
    memset(&Geo_Project_Returns, 0, sizeof(Geo_Project_Returns));
    memset(&delay::Block_Delay_Returns, 0, sizeof(delay::Block_Delay_Returns));

    Ut_CFE_TBL_AddTable(PE_CONFIG_TABLE_FILENAME, (void *) &PE_ConfigTbl);
}

void PE_Test_TearDown(void) 
{

}
