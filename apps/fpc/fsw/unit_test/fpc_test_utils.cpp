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

#include "fpc_test_utils.h"

#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_psp_timer_stubs.h"
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

extern FPC  cpyFPC;

//extern FPC_ConfigTbl_t FPC_ConfigTbl;

extern Ut_CFE_PSP_MEMUTILS_HookTable_t         Ut_CFE_PSP_MEMUTILS_HookTable;
extern Ut_CFE_PSP_MEMUTILS_ReturnCodeTable_t
                  Ut_CFE_PSP_MEMUTILS_ReturnCodeTable[UT_CFE_PSP_MEMUTILS_MAX_INDEX];

extern Ut_CFE_PSP_TIMER_HookTable_t            Ut_CFE_PSP_TIMER_HookTable;
extern Ut_CFE_PSP_TIMER_ReturnCodeTable_t
                  Ut_CFE_PSP_TIMER_ReturnCodeTable[UT_CFE_PSP_TIMER_MAX_INDEX];


/*
 * Config table for Nominal status
 */
FPC_ConfigTbl_t FPC_ConfigTblNominal = {
    20.0f,                        /* L1_PERIOD */
    0.75f,                        /* L1_DAMPING */
    5.0f,                         /* T_TIME_CONST */
    8.0f,                         /* T_THRO_CONST */
    0.7f,                         /* T_SINK_MIN */
    5.0f,                         /* T_SINK_MAX */
    5.0f,                         /* T_CLMB_MAX */
    10.0f,                        /* CLMBOUT_DIFF */
    0.05f,                        /* T_HRATE_P */
    0.8f,                         /* T_HRATE_FF */
    0.02f,                        /* T_SRATE_P */
    0.5f,                         /* T_THR_DAMP */
    0.1f,                         /* T_INTEG_GAIN */
    7.0f,                         /* T_VERT_ACC */
    0.0f,                         /* T_HGT_OMEGA */
    3.0f,                         /* T_SPD_OMEGA */
    15.0f,                        /* T_RLL2THR */
    1.0f,                         /* T_SPDWEIGHT */
    0.0f,                         /* T_PTCH_DAMP */
    0.6f,                         /* AIRSPD_MIN */
    1.0f,                         /* AIRSPD_TRIM */
    10.0f,                        /* AIRSPD_MAX */
    FALSE,                        /* ARSP_MODE */
    DEG_TO_RADIANS(10.0f),        /* P_LIM_MIN_RADIANS */
    DEG_TO_RADIANS(10.0f),        /* P_LIM_MAX_RADIANS */
    DEG_TO_RADIANS(50.0f),        /* R_LIM_RADIANS */
    0.2f,                         /* THR_MIN */
    1.0f,                         /* THR_MAX */
    0.15f,                        /* THR_IDLE */
    0.6f,                         /* THR_CRUISE */
    0.0f,                         /* THR_SLEW_MAX */
    0.0f,                         /* THR_ALT_SCL */
    DEG_TO_RADIANS(45.0f),        /* MAN_R_MAX_RADIANS */
    DEG_TO_RADIANS(10.0f),        /* MAN_P_MAX_RADIANS */
    DEG_TO_RADIANS(0.0f),         /* RSP_OFF_RADIANS */
    DEG_TO_RADIANS(0.0f),         /* PSP_OFF_RADIANS */
    1.0f,                         /* THR_LND_MAX */
    DEG_TO_RADIANS(5.0f),         /* Landing slope angle in degrees */
    10.0f,                        /* LND_HVIRT */
    8.0f,                         /* LND_FLALT */
    -1.0f,                        /* LND_TLALT */
    15.0f,                        /* LND_HHDIST */
    DEG_TO_RADIANS(2.5f),         /* Flare, minimum pitch */
    DEG_TO_RADIANS(15.0f),        /* LND_FL_PMAX_RADIANS */
    0,                            /* LND_USETER */
    1.3f,                         /* LND_AIRSPD_SC */
    {
        /* @file launchdetection_params.c
         * Catapult launch detection parameters, accessible via MAVLink */

         0,                       /* LAUN_ALL_ON */
         30.0f,                   /* LAUN_CAT_A */
         0.05f,                   /* Catapult time threshold. */
         0.0f,                    /* Motor delay */
         DEG_TO_RADIANS(30)       /* LAUN_CAT_PMAX_RADIANS */
    },
    {
        TRUE,                     /* RWTO_TKOFF */
        0,                        /* RWTO_HDG */
        100.0f,                   /* NAV_ALT */
        1.0f,                     /* MAX_THR */
        0.0f,                     /* PSP */
        10.0f,                    /* MAX_PITCH */
        25.0f,                    /* Max roll during climbout. */
        1.2f,                     /* AIRSPD_SCL */
        2.0f,                     /* AIRSPD_MIN */
        10.0f                     /* CLMBOUT_DIFF */
    }
};


/*
 * Function Definitions
 */

void FPC_Test_Setup(void)
{
    /* initialize test environment to default state for every test */

    CFE_PSP_MemCpy((void*)&oFPC, (void*)&cpyFPC, sizeof(FPC));

    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_TIME_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();

#if 0
    Ut_CFE_TBL_AddTable(FPC_CONFIG_TABLE_FILENAME, (void *) &FPC_ConfigTbl);
#else
    Ut_CFE_TBL_AddTable(FPC_CONFIG_TABLE_FILENAME, (void *) &FPC_ConfigTblNominal);
#endif

    memset(&Ut_CFE_PSP_MEMUTILS_HookTable, 0, sizeof(Ut_CFE_PSP_MEMUTILS_HookTable));
    memset(&Ut_CFE_PSP_MEMUTILS_ReturnCodeTable, 0, sizeof(Ut_CFE_PSP_MEMUTILS_ReturnCodeTable));

    memset(&Ut_CFE_PSP_TIMER_HookTable, 0, sizeof(Ut_CFE_PSP_TIMER_HookTable));
    memset(&Ut_CFE_PSP_TIMER_ReturnCodeTable, 0, sizeof(Ut_CFE_PSP_TIMER_ReturnCodeTable));
}

void FPC_Test_TearDown(void) {
    memset(&Ut_CFE_PSP_MEMUTILS_HookTable, 0, sizeof(Ut_CFE_PSP_MEMUTILS_HookTable));
    memset(&Ut_CFE_PSP_MEMUTILS_ReturnCodeTable, 0, sizeof(Ut_CFE_PSP_MEMUTILS_ReturnCodeTable));

    memset(&Ut_CFE_PSP_TIMER_HookTable, 0, sizeof(Ut_CFE_PSP_TIMER_HookTable));
    memset(&Ut_CFE_PSP_TIMER_ReturnCodeTable, 0, sizeof(Ut_CFE_PSP_TIMER_ReturnCodeTable));
}

time_t FPC_Test_GetTimeFromTimestamp(uint64 timestamp)
{
    time_t  local_time;

    local_time = (time_t)(timestamp / 1000000);

    return local_time;
}

time_t FPC_Test_GetTimeFromMsg(CFE_TIME_SysTime_t cfe_time)
{
    time_t   local_time;

    local_time = (time_t)cfe_time.Seconds;

    return local_time;
}
