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

#include "fpc_test_utils.hpp"

#include <iostream>

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


extern Ut_CFE_PSP_MEMUTILS_HookTable_t         Ut_CFE_PSP_MEMUTILS_HookTable;
extern Ut_CFE_PSP_MEMUTILS_ReturnCodeTable_t
                  Ut_CFE_PSP_MEMUTILS_ReturnCodeTable[UT_CFE_PSP_MEMUTILS_MAX_INDEX];

extern Ut_CFE_PSP_TIMER_HookTable_t            Ut_CFE_PSP_TIMER_HookTable;
extern Ut_CFE_PSP_TIMER_ReturnCodeTable_t
                  Ut_CFE_PSP_TIMER_ReturnCodeTable[UT_CFE_PSP_TIMER_MAX_INDEX];


/*
 * Config table for Nominal status
 */
FPC_ConfigTbl_t FPC_ConfigTblUnitTest = {
    20.0f,                        /* L1_PERIOD */
    0.75f,                        /* L1_DAMPING */
    5.0f,                         /* T_TIME_CONST */
    8.0f,                         /* T_THRO_CONST */
    2.0f,                         /* T_SINK_MIN */
    5.0f,                         /* T_SINK_MAX */
    5.0f,                         /* T_CLMB_MAX */
    10.0f,                        /* CLMBOUT_DIFF */
    0.05f,                        /* T_HRATE_P */
    0.8f,                         /* T_HRATE_FF */
    0.02f,                        /* T_SRATE_P */
    0.5f,                         /* T_THR_DAMP */
    0.1f,                         /* T_INTEG_GAIN */
    7.0f,                         /* T_VERT_ACC */
    3.0f,                         /* T_HGT_OMEGA */
    2.0f,                         /* T_SPD_OMEGA */
    15.0f,                        /* T_RLL2THR */
    1.0f,                         /* T_SPDWEIGHT */
    0.0f,                         /* T_PTCH_DAMP */
    10.0f,                        /* AIRSPD_MIN */
    15.0f,                        /* AIRSPD_TRIM */
    20.0f,                        /* AIRSPD_MAX */
    FALSE,                        /* ARSP_MODE */
    DEG_TO_RADIANS(-45.0f),       /* P_LIM_MIN_RADIANS */
    DEG_TO_RADIANS(45.0f),        /* P_LIM_MAX_RADIANS */
    DEG_TO_RADIANS(50.0f),        /* R_LIM_RADIANS */
    0.0f,                         /* THR_MIN */
    1.0f,                         /* THR_MAX */
    0.15f,                        /* THR_IDLE */
    0.6f,                         /* THR_CRUISE */
    0.0f,                         /* THR_SLEW_MAX */
    0.0f,                         /* THR_ALT_SCL */
    DEG_TO_RADIANS(45.0f),        /* MAN_R_MAX_RADIANS */
    DEG_TO_RADIANS(45.0f),        /* MAN_P_MAX_RADIANS */
    DEG_TO_RADIANS(0.0f),         /* RSP_OFF_RADIANS */
    DEG_TO_RADIANS(0.0f),         /* PSP_OFF_RADIANS */
    1.0f,                         /* THR_LND_MAX */
    DEG_TO_RADIANS(8.0f),         /* LND_ANG_RADIANS */
    10.0f,                        /* LND_HVIRT */
    8.0f,                         /* LND_FLALT */
    -1.0f,                        /* LND_TLALT */
    15.0f,                        /* LND_HHDIST */
    DEG_TO_RADIANS(2.5f),         /* LND_FL_PMIN_RADIANS */
    DEG_TO_RADIANS(15.0f),        /* LND_FL_PMAX_RADIANS */
    0,                            /* LND_USETER */
    1.3f,                         /* LND_AIRSPD_SC */
    {
        /* @file launchdetection_params.c
         * Catapult launch detection parameters, accessible via MAVLink */

         FALSE,                   /* LAUN_ALL_ON */
         30.0f,                   /* LAUN_CAT_A */
         0.05f,                   /* LAUN_CAT_T */
         0.0f,                    /* LAUN_CAT_MDEL */
         DEG_TO_RADIANS(30)       /* LAUN_CAT_PMAX_RADIANS */
    },
    {
        TRUE,                     /* RWTO_TKOFF */
        0,                        /* RWTO_HDG */
        5.0f,                     /* NAV_ALT */
        1.0f,                     /* MAX_THR */
        DEG_TO_RADIANS(0.0),      /* PSP_RADIANS */
        DEG_TO_RADIANS(20.0),     /* MAX_PITCH_RADIANS */
        DEG_TO_RADIANS(25),       /* MAX_ROLL_RADIANS */
        1.3f,                     /* AIRSPD_SCL */
        10.0f,                    /* RWTO_AIRSPD_MIN */
        10.0f                     /* RWTO_CLMBOUT_DIFF */
    }
};

/*
 * Config table for Invalid status
 */
FPC_ConfigTbl_t FPC_ConfigTblInvalid = {
    20.0f,                        /* L1_PERIOD */
    0.75f,                        /* L1_DAMPING */
    5.0f,                         /* T_TIME_CONST */
    8.0f,                         /* T_THRO_CONST */
    2.0f,                         /* T_SINK_MIN */
    5.0f,                         /* T_SINK_MAX */
    5.0f,                         /* T_CLMB_MAX */
    10.0f,                        /* CLMBOUT_DIFF */
    0.05f,                        /* T_HRATE_P */
    0.8f,                         /* T_HRATE_FF */
    0.02f,                        /* T_SRATE_P */
    0.5f,                         /* T_THR_DAMP */
    0.1f,                         /* T_INTEG_GAIN */
    7.0f,                         /* T_VERT_ACC */
    3.0f,                         /* T_HGT_OMEGA */
    2.0f,                         /* T_SPD_OMEGA */
    15.0f,                        /* T_RLL2THR */
    1.0f,                         /* T_SPDWEIGHT */
    0.0f,                         /* T_PTCH_DAMP */
    101.0f,                        /* AIRSPD_MIN */
    15.0f,                        /* AIRSPD_TRIM */
    4.0f,                         /* AIRSPD_MAX */
    FALSE,                        /* ARSP_MODE */
    DEG_TO_RADIANS(-45.0f),       /* P_LIM_MIN_RADIANS */
    DEG_TO_RADIANS(45.0f),        /* P_LIM_MAX_RADIANS */
    DEG_TO_RADIANS(50.0f),        /* R_LIM_RADIANS */
    0.0f,                         /* THR_MIN */
    1.0f,                         /* THR_MAX */
    0.15f,                        /* THR_IDLE */
    0.6f,                         /* THR_CRUISE */
    0.0f,                         /* THR_SLEW_MAX */
    0.0f,                         /* THR_ALT_SCL */
    DEG_TO_RADIANS(45.0f),        /* MAN_R_MAX_RADIANS */
    DEG_TO_RADIANS(45.0f),        /* MAN_P_MAX_RADIANS */
    DEG_TO_RADIANS(0.0f),         /* RSP_OFF_RADIANS */
    DEG_TO_RADIANS(0.0f),         /* PSP_OFF_RADIANS */
    1.0f,                         /* THR_LND_MAX */
    DEG_TO_RADIANS(8.0f),         /* LND_ANG_RADIANS */
    10.0f,                        /* LND_HVIRT */
    8.0f,                         /* LND_FLALT */
    -1.0f,                        /* LND_TLALT */
    15.0f,                        /* LND_HHDIST */
    DEG_TO_RADIANS(2.5f),         /* LND_FL_PMIN_RADIANS */
    DEG_TO_RADIANS(15.0f),        /* LND_FL_PMAX_RADIANS */
    0,                            /* LND_USETER */
    1.3f,                         /* LND_AIRSPD_SC */
    {
        /* @file launchdetection_params.c
         * Catapult launch detection parameters, accessible via MAVLink */

         FALSE,                   /* LAUN_ALL_ON */
         30.0f,                   /* LAUN_CAT_A */
         0.05f,                   /* LAUN_CAT_T */
         0.0f,                    /* LAUN_CAT_MDEL */
         DEG_TO_RADIANS(30)       /* LAUN_CAT_PMAX_RADIANS */
    },
    {
        TRUE,                     /* RWTO_TKOFF */
        0,                        /* RWTO_HDG */
        5.0f,                     /* NAV_ALT */
        1.0f,                     /* MAX_THR */
        DEG_TO_RADIANS(0.0),      /* PSP_RADIANS */
        DEG_TO_RADIANS(20.0),     /* MAX_PITCH_RADIANS */
        DEG_TO_RADIANS(25),       /* MAX_ROLL_RADIANS */
        1.3f,                     /* AIRSPD_SCL */
        10.0f,                    /* RWTO_AIRSPD_MIN */
        10.0f                     /* RWTO_CLMBOUT_DIFF */
    }
};

/*
 * Function Definitions
 */

void FPC_Test_Setup(void)
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

    Ut_CFE_TBL_AddTable(FPC_CONFIG_TABLE_FILENAME, (void *) &FPC_ConfigTbl);

    memset(&Ut_CFE_PSP_MEMUTILS_HookTable, 0, sizeof(Ut_CFE_PSP_MEMUTILS_HookTable));
    memset(&Ut_CFE_PSP_MEMUTILS_ReturnCodeTable, 0, sizeof(Ut_CFE_PSP_MEMUTILS_ReturnCodeTable));

    memset(&Ut_CFE_PSP_TIMER_HookTable, 0, sizeof(Ut_CFE_PSP_TIMER_HookTable));
    memset(&Ut_CFE_PSP_TIMER_ReturnCodeTable, 0, sizeof(Ut_CFE_PSP_TIMER_ReturnCodeTable));
}

void FPC_Test_Setup_ConfigInvalid(void)
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

    Ut_CFE_TBL_AddTable(FPC_CONFIG_TABLE_FILENAME, (void *) &FPC_ConfigTblInvalid);

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

double FPC_Test_GetChecksum(FPC_ConfigTbl_t *pTbl)
{
    double checksum = 0.0;

    checksum = (double)pTbl->L1_PERIOD + (double)pTbl->L1_DAMPING
               + (double)pTbl->T_TIME_CONST + (double)pTbl->T_THRO_CONST
               + (double)pTbl->T_SINK_MIN + (double)pTbl->T_SINK_MAX
               + (double)pTbl->T_CLMB_MAX + (double)pTbl->CLMBOUT_DIFF
               + (double)pTbl->T_HRATE_P + (double)pTbl->T_HRATE_FF
               + (double)pTbl->T_SRATE_P + (double)pTbl->T_THR_DAMP
               + (double)pTbl->T_INTEG_GAIN + (double)pTbl->T_VERT_ACC
               + (double)pTbl->T_HGT_OMEGA + (double)pTbl->T_SPD_OMEGA
               + (double)pTbl->T_RLL2THR + (double)pTbl->T_SPDWEIGHT
               + (double)pTbl->T_PTCH_DAMP + (double)pTbl->AIRSPD_MIN
               + (double)pTbl->AIRSPD_TRIM + (double)pTbl->AIRSPD_MAX
               + (double)pTbl->ARSP_MODE
               + (double)pTbl->P_LIM_MIN_RADIANS + (double)pTbl->P_LIM_MAX_RADIANS
               + (double)pTbl->R_LIM_RADIANS + (double)pTbl->THR_MIN
               + (double)pTbl->THR_MAX + (double)pTbl->THR_IDLE
               + (double)pTbl->THR_CRUISE + (double)pTbl->THR_SLEW_MAX
               + (double)pTbl->THR_ALT_SCL + (double)pTbl->MAN_R_MAX_RADIANS
               + (double)pTbl->MAN_P_MAX_RADIANS + (double)pTbl->RSP_OFF_RADIANS
               + (double)pTbl->PSP_OFF_RADIANS + (double)pTbl->THR_LND_MAX
               + (double)pTbl->LND_ANG_RADIANS + (double)pTbl->LND_HVIRT
               + (double)pTbl->LND_FLALT + (double)pTbl->LND_TLALT
               + (double)pTbl->LND_HHDIST + (double)pTbl->LND_FL_PMIN_RADIANS
               + (double)pTbl->LND_FL_PMAX_RADIANS
               + (double)pTbl->LND_USETER
               + (double)pTbl->LND_AIRSPD_SC;

    checksum = checksum + (double)pTbl->FPC_Launch_Detection.LAUN_ALL_ON
               + (double)pTbl->FPC_Launch_Detection.LAUN_CAT_A
               + (double)pTbl->FPC_Launch_Detection.LAUN_CAT_T
               + (double)pTbl->FPC_Launch_Detection.LAUN_CAT_MDEL
               + (double)pTbl->FPC_Launch_Detection.LAUN_CAT_PMAX_RADIANS;

    checksum = checksum + (double)pTbl->FPC_Runway_Takeoff.RWTO_TKOFF
               + (double)pTbl->FPC_Runway_Takeoff.RWTO_HDG
               + (double)pTbl->FPC_Runway_Takeoff.NAV_ALT
               + (double)pTbl->FPC_Runway_Takeoff.MAX_THR
               + (double)pTbl->FPC_Runway_Takeoff.PSP_RADIANS
               + (double)pTbl->FPC_Runway_Takeoff.MAX_PITCH_RADIANS
               + (double)pTbl->FPC_Runway_Takeoff.MAX_ROLL_RADIANS
               + (double)pTbl->FPC_Runway_Takeoff.AIRSPD_SCL
               + (double)pTbl->FPC_Runway_Takeoff.RWTO_AIRSPD_MIN
               + (double)pTbl->FPC_Runway_Takeoff.RWTO_CLMBOUT_DIFF;

    printf("FPC_Test_GetChecksum:\n");
    printf("Params Checksum(with Max precision): %.17f\n", checksum);
    printf("Param Table Values:\n");
    printf("L1_PERIOD: %f, L1_DAMPING: %f\n", pTbl->L1_PERIOD, pTbl->L1_DAMPING);
    printf("T_TIME_CONST: %f, T_THRO_CONST: %f\n", pTbl->T_TIME_CONST, pTbl->T_THRO_CONST);
    printf("T_SINK_MIN: %f, T_SINK_MAX: %f\n", pTbl->T_SINK_MIN, pTbl->T_SINK_MAX);
    printf("T_CLMB_MAX: %f, CLMBOUT_DIFF: %f\n", pTbl->T_CLMB_MAX, pTbl->CLMBOUT_DIFF);
    printf("T_HRATE_P: %f, T_HRATE_FF: %f\n", pTbl->T_HRATE_P, pTbl->T_HRATE_FF);
    printf("T_SRATE_P: %f, T_THR_DAMP: %f\n", pTbl->T_SRATE_P, pTbl->T_THR_DAMP);
    printf("T_INTEG_GAIN: %f, T_VERT_ACC: %f\n", pTbl->T_INTEG_GAIN, pTbl->T_VERT_ACC);
    printf("T_HGT_OMEGA: %f, T_SPD_OMEGA: %f\n", pTbl->T_HGT_OMEGA, pTbl->T_SPD_OMEGA);
    printf("T_RLL2THR: %f, T_SPDWEIGHT: %f\n", pTbl->T_RLL2THR, pTbl->T_SPDWEIGHT);
    printf("T_PTCH_DAMP: %f, AIRSPD_MIN: %f\n", pTbl->T_PTCH_DAMP, pTbl->AIRSPD_MIN);
    printf("AIRSPD_TRIM: %f, AIRSPD_MAX: %f\n", pTbl->AIRSPD_TRIM, pTbl->AIRSPD_MAX);

    if (pTbl->ARSP_MODE == TRUE)
    {
        printf("ARSP_MODE: TRUE\n");
    }
    else
    {
        printf("ARSP_MODE: FALSE\n");
    }
    printf("P_LIM_MIN_RADIANS: %f, P_LIM_MAX_RADIANS: %f\n",
            pTbl->P_LIM_MIN_RADIANS, pTbl->P_LIM_MAX_RADIANS);
    printf("R_LIM_RADIANS: %f, THR_MIN: %f\n",
            pTbl->R_LIM_RADIANS, pTbl->THR_MIN);
    printf("THR_MAX: %f, THR_IDLE: %f\n", pTbl->THR_MAX, pTbl->THR_IDLE);
    printf("THR_CRUISE: %f, THR_SLEW_MAX: %f\n",
            pTbl->THR_CRUISE, pTbl->THR_SLEW_MAX);
    printf("THR_ALT_SCL: %f, MAN_R_MAX_RADIANS: %f\n",
            pTbl->THR_ALT_SCL, pTbl->MAN_R_MAX_RADIANS);
    printf("MAN_P_MAX_RADIANS: %f, RSP_OFF_RADIANS: %f\n",
            pTbl->MAN_P_MAX_RADIANS, pTbl->RSP_OFF_RADIANS);
    printf("PSP_OFF_RADIANS: %f, THR_LND_MAX: %f\n",
            pTbl->PSP_OFF_RADIANS, pTbl->THR_LND_MAX);
    printf("LND_ANG_RADIANS: %f, LND_HVIRT: %f\n",
            pTbl->LND_ANG_RADIANS, pTbl->LND_HVIRT);
    printf("LND_FLALT: %f, LND_TLALT: %f\n", pTbl->LND_FLALT, pTbl->LND_TLALT);
    printf("LND_HHDIST: %f, LND_FL_PMIN_RADIANS: %f\n",
            pTbl->LND_HHDIST, pTbl->LND_FL_PMIN_RADIANS);
    printf("LND_FL_PMAX_RADIANS: %f, LND_USETER: %ld\n",
            pTbl->LND_FL_PMAX_RADIANS, pTbl->LND_USETER);
    printf("LND_AIRSPD_SC: %f\n", pTbl->LND_AIRSPD_SC);

    if (pTbl->FPC_Launch_Detection.LAUN_ALL_ON == TRUE)
    {
        printf("LAUN_ALL_ON: TRUE, LAUN_CAT_A: %f\n", pTbl->FPC_Launch_Detection.LAUN_CAT_A);
    }
    else
    {
        printf("LAUN_ALL_ON: FALSE, LAUN_CAT_A: %f\n", pTbl->FPC_Launch_Detection.LAUN_CAT_A);
    }
    printf("LAUN_CAT_T: %f, LAUN_CAT_MDEL: %f\n",
            pTbl->FPC_Launch_Detection.LAUN_CAT_T, pTbl->FPC_Launch_Detection.LAUN_CAT_MDEL);
    printf("LAUN_CAT_PMAX_RADIANS: %f\n", pTbl->FPC_Launch_Detection.LAUN_CAT_PMAX_RADIANS);

    if (pTbl->FPC_Runway_Takeoff.RWTO_TKOFF == TRUE)
    {
        printf("RWTO_TKOFF: TRUE\n");
    }
    else
    {
        printf("RWTO_TKOFF: FALSE\n");
    }
    printf("RWTO_HDG: %ld\n", pTbl->FPC_Runway_Takeoff.RWTO_HDG);
    printf("NAV_ALT: %f\n", pTbl->FPC_Runway_Takeoff.NAV_ALT);
    printf("MAX_THR: %f\n", pTbl->FPC_Runway_Takeoff.MAX_THR);
    printf("PSP_RADIANS: %f\n", pTbl->FPC_Runway_Takeoff.PSP_RADIANS);
    printf("MAX_PITCH_RADIANS: %f\n", pTbl->FPC_Runway_Takeoff.MAX_PITCH_RADIANS);
    printf("MAX_ROLL_RADIANS: %f\n", pTbl->FPC_Runway_Takeoff.MAX_ROLL_RADIANS);
    printf("AIRSPD_SCL: %f\n", pTbl->FPC_Runway_Takeoff.AIRSPD_SCL);
    printf("RWTO_AIRSPD_MIN: %f\n", pTbl->FPC_Runway_Takeoff.RWTO_AIRSPD_MIN);
    printf("RWTO_CLMBOUT_DIFF: %f\n", pTbl->FPC_Runway_Takeoff.RWTO_CLMBOUT_DIFF);

    return checksum;
}

void FPC_Test_PrintCmdMsg(void *pMsg, uint32 size)
{
    unsigned char *pBuff;
    int           i = 0;

    pBuff = (unsigned char*)pMsg;
    printf("Emulated Cmd message:");
    for (i = 0; i < size; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff++;
    }
    printf("\n");

    return;
}

uint64 FPC_Test_GetTimeUs(void)
{
    int              iStatus;
    uint64           outTime = 0;
    struct timespec  time;

    iStatus = clock_gettime(CLOCK_REALTIME, &time);
    if (iStatus == 0)
    {
        outTime = static_cast<uint64>(static_cast<uint64>(time.tv_sec)
                  * static_cast<uint64>(1000000))
                  + static_cast<uint64>(static_cast<uint64>(time.tv_nsec)
                    / static_cast<uint64>(1000));
    }

    return outTime;
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
