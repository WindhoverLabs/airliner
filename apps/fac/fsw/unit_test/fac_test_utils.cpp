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

#include "fac_test_utils.hpp"

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

#include <time.h>

extern FAC cpyFAC;
extern FAC_ParamTbl_t FAC_ParamTbl;

/*
 * Config table for testing
 */
extern FAC_ParamTbl_t FAC_ParamTbl;

FAC_ParamTbl_t FAC_ParamTblNominal =
{
    5.0,		/* MC_ROLL_P 	   */
    0.1,		/* MC_ROLLRATE_P   */
    0.05,		/* MC_ROLLRATE_I   */
    0.30,		/* MC_RR_INT_LIM   */
    0.001,		/* MC_ROLLRATE_D   */
    0.0,		/* MC_ROLLRATE_FF  */
    5.0,		/* MC_PITCH_P      */
    0.1,		/* MC_PITCHRATE_P  */
    0.05,		/* MC_PITCHRATE_I  */
    0.30,		/* MC_PR_INT_LIM   */
    0.001,		/* MC_PITCHRATE_D  */
    0.0,		/* MC_PITCHRATE_FF */
    1.0,		/* MC_TPA_BREAK_P  */
    1.0,		/* MC_TPA_BREAK_I  */
    1.0,		/* MC_TPA_BREAK_D  */
    0.0,		/* MC_TPA_RATE_P   */
    0.0,		/* MC_TPA_RATE_I   */
    0.0,		/* MC_TPA_RATE_D   */
    2.8,		/* MC_YAW_P        */
    0.08,		/* MC_YAWRATE_P	   */
    0.1,		/* MC_YAWRATE_I    */
    0.30,       /* MC_YR_INT_LIM   */
    0.0,        /* MC_YAWRATE_D    */
    0.0,        /* MC_YAWRATE_FF   */
    220.0,      /* MC_ROLLRATE_MAX */
    220.0,      /* MC_PITCHRATE_MAX */
    200.0,      /* MC_YAWRATE_MAX   */
    45.0,       /* MC_YAWRAUTO_MAX  */
    360.0,      /* MC_ACRO_R_MAX    */
    360.0,		/* MC_ACRO_P_MAX    */
    360.0,      /* MC_ACRO_Y_MAX    */
    0.8,        /* MC_RATT_TH	    */
    0.2,        /* MC_ROLL_TC		*/
    0.2,		/* MC_PITCH_TC      */
    0,			/* MC_BAT_SCALE_EN  */
    0,			/* SENS_BOARD_ROT   */
    0.0,		/* SENS_BOARD_X_OFF */
    0.0,		/* SENS_BOARD_Y_OFF */
    0.0 		/* SENS_BOARD_Z_OFF */
};

/*
 * Function Definitions
 */

void FAC_Test_Setup(void)
{
    /* initialize test environment to default state for every test */

    CFE_PSP_MemCpy(&oFAC, &cpyFAC, sizeof(FAC));

    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_TIME_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();

    Ut_CFE_TBL_AddTable(FAC_PARAM_TABLE_FILENAME, (void *) &FAC_ParamTbl);
}

void FAC_Test_TearDown(void)
{
}
