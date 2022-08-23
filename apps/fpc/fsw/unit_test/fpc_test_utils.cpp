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
#include <iostream>

extern FPC  cpyFPC;

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
 * Config table for Invalid status
 */
FPC_ConfigTbl_t FPC_ConfigTblInvalid = {
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
    5.0f,                         /* AIRSPD_TRIM */
    4.5f,                        /* AIRSPD_MAX */
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

    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_TIME_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();

#if 1
    Ut_CFE_TBL_AddTable(FPC_CONFIG_TABLE_FILENAME, (void *) &FPC_ConfigTbl);
#else
    Ut_CFE_TBL_AddTable(FPC_CONFIG_TABLE_FILENAME, (void *) &FPC_ConfigTblNominal);
#endif

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
               + (double)pTbl->FPC_Runway_Takeoff.PSP
               + (double)pTbl->FPC_Runway_Takeoff.MAX_PITCH
               + (double)pTbl->FPC_Runway_Takeoff.MAX_ROLL
               + (double)pTbl->FPC_Runway_Takeoff.AIRSPD_SCL
               + (double)pTbl->FPC_Runway_Takeoff.AIRSPD_MIN
               + (double)pTbl->FPC_Runway_Takeoff.CLMBOUT_DIFF;

    printf("FPC_Test_GetChecksum:\n");
    std::cout.precision(17);
    std::cout << "Params Checksum(with Max precision): "
              << checksum << "\n";

    std::cout.precision(7);
    std::cout << "Param Table Values:\n";
    std::cout << "L1_PERIOD: " << pTbl->L1_PERIOD
              << ", L1_DAMPING: " << pTbl->L1_DAMPING << "\n";
    std::cout << "T_TIME_CONST: " << pTbl->T_TIME_CONST
              << ", T_THRO_CONST: " << pTbl->T_THRO_CONST << "\n";
    std::cout << "T_SINK_MIN: " << pTbl->T_SINK_MIN
              << ", T_SINK_MAX: " << pTbl->T_SINK_MAX << "\n";
    std::cout << "T_CLMB_MAX: " << pTbl->T_CLMB_MAX
              << ", CLMBOUT_DIFF: " << pTbl->CLMBOUT_DIFF << "\n";
    std::cout << "T_HRATE_P: " << pTbl->T_HRATE_P
              << ", T_HRATE_FF: " << pTbl->T_HRATE_FF << "\n";
    std::cout << "T_SRATE_P: " << pTbl->T_SRATE_P
              << ", T_THR_DAMP: " << pTbl->T_THR_DAMP << "\n";
    std::cout << "T_INTEG_GAIN: " << pTbl->T_INTEG_GAIN
              << ", T_VERT_ACC: " << pTbl->T_VERT_ACC << "\n";
    std::cout << "T_HGT_OMEGA: " << pTbl->T_HGT_OMEGA
              << ", T_SPD_OMEGA: " << pTbl->T_SPD_OMEGA << "\n";
    std::cout << "T_RLL2THR: " << pTbl->T_RLL2THR
              << ", T_SPDWEIGHT: " << pTbl->T_SPDWEIGHT << "\n";
    std::cout << "T_PTCH_DAMP: " << pTbl->T_PTCH_DAMP
              << ", AIRSPD_MIN: " << pTbl->AIRSPD_MIN << "\n";
    std::cout << "AIRSPD_TRIM: " << pTbl->AIRSPD_TRIM
              << ", AIRSPD_MAX: " << pTbl->AIRSPD_MAX << "\n";
    std::cout << "ARSP_MODE: " << pTbl->ARSP_MODE << "\n";
    std::cout << "P_LIM_MIN_RADIANS: " << pTbl->P_LIM_MIN_RADIANS
              << ", P_LIM_MAX_RADIANS: " << pTbl->P_LIM_MAX_RADIANS << "\n";
    std::cout << "R_LIM_RADIANS: " << pTbl->R_LIM_RADIANS
              << ", THR_MIN: " << pTbl->THR_MIN << "\n";
    std::cout << "THR_MAX: " << pTbl->THR_MAX
              << ", THR_IDLE: " << pTbl->THR_IDLE << "\n";
    std::cout << "THR_CRUISE: " << pTbl->THR_CRUISE
              << ", THR_SLEW_MAX: " << pTbl->THR_SLEW_MAX << "\n";
    std::cout << "THR_ALT_SCL: " << pTbl->THR_ALT_SCL
              << ", MAN_R_MAX_RADIANS: " << pTbl->MAN_R_MAX_RADIANS << "\n";
    std::cout << "MAN_P_MAX_RADIANS: " << pTbl->MAN_P_MAX_RADIANS
              << ", RSP_OFF_RADIANS: " << pTbl->RSP_OFF_RADIANS << "\n";
    std::cout << "PSP_OFF_RADIANS: " << pTbl->PSP_OFF_RADIANS
              << ", THR_LND_MAX: " << pTbl->THR_LND_MAX << "\n";
    std::cout << "LND_ANG_RADIANS: " << pTbl->LND_ANG_RADIANS
              << ", LND_HVIRT: " << pTbl->LND_HVIRT << "\n";
    std::cout << "LND_FLALT: " << pTbl->LND_FLALT
              << ", LND_TLALT: " << pTbl->LND_TLALT << "\n";
    std::cout << "LND_HHDIST: " << pTbl->LND_HHDIST
              << ", LND_FL_PMIN_RADIANS: " << pTbl->LND_FL_PMIN_RADIANS << "\n";
    std::cout << "LND_FL_PMAX_RADIANS: " << pTbl->LND_FL_PMAX_RADIANS
              << ", LND_USETER: " << pTbl->LND_USETER << "\n";
    std::cout << "LND_AIRSPD_SC: " << pTbl->LND_AIRSPD_SC << "\n";

    std::cout << "LAUN_ALL_ON: " << pTbl->FPC_Launch_Detection.LAUN_ALL_ON
              << ", LAUN_CAT_A: " << pTbl->FPC_Launch_Detection.LAUN_CAT_A << "\n";
    std::cout << "LAUN_CAT_T: " << pTbl->FPC_Launch_Detection.LAUN_CAT_T
              << ", LAUN_CAT_MDEL: " << pTbl->FPC_Launch_Detection.LAUN_CAT_MDEL << "\n";
    std::cout << "LAUN_CAT_PMAX_RADIANS: " << pTbl->FPC_Launch_Detection.LAUN_CAT_PMAX_RADIANS
              << "\n";

    std::cout << "RWTO_TKOFF: " << pTbl->FPC_Runway_Takeoff.RWTO_TKOFF << "\n";
    std::cout << "RWTO_HDG: " << pTbl->FPC_Runway_Takeoff.RWTO_HDG << "\n";
    std::cout << "NAV_ALT: " << pTbl->FPC_Runway_Takeoff.NAV_ALT << "\n";
    std::cout << "MAX_THR: " << pTbl->FPC_Runway_Takeoff.MAX_THR << "\n";
    std::cout << "PSP: " << pTbl->FPC_Runway_Takeoff.PSP << "\n";
    std::cout << "MAX_PITCH: " << pTbl->FPC_Runway_Takeoff.MAX_PITCH << "\n";
    std::cout << "MAX_ROLL: " << pTbl->FPC_Runway_Takeoff.MAX_ROLL << "\n";
    std::cout << "AIRSPD_SCL: " << pTbl->FPC_Runway_Takeoff.AIRSPD_SCL << "\n";
    std::cout << "AIRSPD_MIN: " << pTbl->FPC_Runway_Takeoff.AIRSPD_MIN << "\n";
    std::cout << "CLMBOUT_DIFF: " << pTbl->FPC_Runway_Takeoff.CLMBOUT_DIFF << "\n\n";

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
