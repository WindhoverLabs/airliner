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

#include "aspd4525_test_utils.h"
#include "ut_aspd4525_custom_stubs.h"

#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_psp_timer_stubs.h"
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


extern Ut_CFE_PSP_TIMER_HookTable_t            Ut_CFE_PSP_TIMER_HookTable;
extern Ut_CFE_PSP_TIMER_ReturnCodeTable_t
             Ut_CFE_PSP_TIMER_ReturnCodeTable[UT_CFE_PSP_TIMER_MAX_INDEX];


/*
 * Config table for testing
 */
ASPD4525_ConfigTblEntry_t
          ASPD4525_ConfigTblUnitTest[ASPD4525_CONFIG_TABLE_MAX_ENTRIES] = {
    /* Entry 1 */
    {
        .fPressureMinimum_PSI = -0.99,
        .fPressureMaximum_PSI = 1.029,
        .fTemperatureMinimum_Celcius = -50.0,
        .fTemperatureMaximum_Celcius = 150.0,
        .fAirGasConstantR_SI = 8.3144598,
        .fGravitationalAccereleration_SI = 9.80665,
        .fAirMolarMass_SI = 0.0289644,
        .fAltitudeMeters_bs =
                 {0.0, 11000.0, 20000.0, 32000.0, 47000.0, 51000.0, 71000.0},
        .fRho_bs =
             {1.2250, 0.36391, 0.08803, 0.01322, 0.00143, 0.00086, 0.000064},
        .fTemp_bs = {288.15, 216.65, 216.65, 228.65, 270.65, 270.65, 214.65},
        .fLapseRate_bs = {-0.0065, 0.0, 0.001, 0.0028, 0.0, -0.0028, -0.002},
        .uEquationNo_bs = {1,2,1,1,2,1,1},
        .uAirDensityCalculationMode = ASPD4525_CONFIG_AIRDENSITY_ALTITUDE_MODE
    }
};


/*
 * Function Definitions
 */
void ASPD4525_Test_Setup(void)
{
    /* initialize test environment to default state for every test */

    CFE_PSP_MemSet(&ASPD4525_AppData, 0x00, sizeof(ASPD4525_AppData_t));

    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_TIME_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();
    Ut_ASPD4525_Custom_Reset();

#if 0
    Ut_CFE_TBL_AddTable(ASPD4525_CONFIG_TABLE_FILENAME,
                        (void *)&ASPD4525_ConfigTbl);
#else
    Ut_CFE_TBL_AddTable(ASPD4525_CONFIG_TABLE_FILENAME,
                        (void *)&ASPD4525_ConfigTblUnitTest);
#endif

    memset(&Ut_CFE_PSP_TIMER_HookTable, 0,
            sizeof(Ut_CFE_PSP_TIMER_HookTable));
    memset(&Ut_CFE_PSP_TIMER_ReturnCodeTable, 0,
            sizeof(Ut_CFE_PSP_TIMER_ReturnCodeTable));
}

void ASPD4525_Test_TearDown(void) {
    CFE_PSP_MemSet(&ASPD4525_AppData, 0x00, sizeof(ASPD4525_AppData_t));

    memset(&Ut_CFE_PSP_TIMER_HookTable, 0,
            sizeof(Ut_CFE_PSP_TIMER_HookTable));
    memset(&Ut_CFE_PSP_TIMER_ReturnCodeTable, 0,
            sizeof(Ut_CFE_PSP_TIMER_ReturnCodeTable));
}

void ASPD4525_Test_PrintCmdMsg(void *pMsg, uint32 size)
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

time_t ASPD4525_Test_GetTimeFromMsg(CFE_TIME_SysTime_t cfe_time)
{
    time_t   local_time;

    local_time = (time_t)cfe_time.Seconds;

    return local_time;
}

time_t ASPD4525_Test_GetTimeFromTimestamp(uint64 timestamp)
{
    time_t  local_time;

    local_time = (time_t)(timestamp / 1000000);

    return local_time;
}
