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
#include "ld_test_utils.hpp"
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


extern Ut_CFE_PSP_MEMUTILS_HookTable_t        Ut_CFE_PSP_MEMUTILS_HookTable;
extern Ut_CFE_PSP_MEMUTILS_ReturnCodeTable_t
          Ut_CFE_PSP_MEMUTILS_ReturnCodeTable[UT_CFE_PSP_MEMUTILS_MAX_INDEX];

extern Ut_CFE_PSP_TIMER_HookTable_t            Ut_CFE_PSP_TIMER_HookTable;
extern Ut_CFE_PSP_TIMER_ReturnCodeTable_t
                Ut_CFE_PSP_TIMER_ReturnCodeTable[UT_CFE_PSP_TIMER_MAX_INDEX];


LD_ConfigTbl_t LD_ConfigTblUnitTest
{
    10.0f,            /* LD_Z_VEL_MAX */
    2.0f,             /* LD_XY_VEL_MAX */
    10000.0f,         /* LD_ALT_MAX */
    0.35f,            /* LD_LOW_T_THR */
    0.08f,            /* LD_MAN_MIN_THR */
    0.65f,            /* LD_POS_STK_UP_THRES */
    0.15f,            /* LD_POS_STK_DW_THRES */
    0.5f,             /* LD_LANDSPEED */
    8.0f,             /* LD_LNDFW_AIRSPD_MAX */
    8000000,          /* LD_MIN_THR_NO_ALT_TIMEOUT */
    LD_OP_MODE_AUTO   /* LD_OP_MODE */
};

LD_ConfigTbl_t LD_ConfigTblUnitTest_Invalid
{
    LD_Z_VEL_MAX_MAX + 1.0f,                /* LD_Z_VEL_MAX */
    LD_XY_VEL_MAX_MAX + 1.0f,               /* LD_XY_VEL_MAX */
    LD_ALT_MAX_MAX + 1.0f,                  /* LD_ALT_MAX */
    LD_LOW_T_THR_MAX + 1.0f,                /* LD_LOW_T_THR */
    LD_MAN_MIN_THR_MAX + 1.0f,              /* LD_MAN_MIN_THR */
    LD_POS_STK_UP_THRES_MAX + 1.0f,         /* LD_POS_STK_UP_THRES */
    LD_POS_STK_DW_THRES_MAX + 1.0f,         /* LD_POS_STK_DW_THRES */
    LD_LANDSPEED_MAX + 1.0f,                /* LD_LANDSPEED */
    8.0f,                                   /* LD_LNDFW_AIRSPD_MAX */
    LD_MIN_THR_NO_ALT_TIMEOUT_MAX + 1,      /* LD_MIN_THR_NO_ALT_TIMEOUT */
    LD_OP_MODE_AUTO                         /* LD_OP_MODE */
};


/*
 * Function Definitions
 */

void LD_Test_Setup(void)
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
    Ut_CFE_TBL_AddTable(LD_CONFIG_TABLE_FILENAME, (void *)&LD_ConfigTbl);
#else
    Ut_CFE_TBL_AddTable(LD_CONFIG_TABLE_FILENAME,
                                          (void *)&LD_ConfigTblUnitTest);
#endif

    memset(&Ut_CFE_PSP_MEMUTILS_HookTable, 0,
                                   sizeof(Ut_CFE_PSP_MEMUTILS_HookTable));
    memset(&Ut_CFE_PSP_MEMUTILS_ReturnCodeTable, 0,
                             sizeof(Ut_CFE_PSP_MEMUTILS_ReturnCodeTable));

    memset(&Ut_CFE_PSP_TIMER_HookTable, 0,
                                      sizeof(Ut_CFE_PSP_TIMER_HookTable));
    memset(&Ut_CFE_PSP_TIMER_ReturnCodeTable, 0,
                                sizeof(Ut_CFE_PSP_TIMER_ReturnCodeTable));
}

void LD_Test_Setup_Invalid(void)
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

    Ut_CFE_TBL_AddTable(LD_CONFIG_TABLE_FILENAME, (void *)
                                           &LD_ConfigTblUnitTest_Invalid);

    memset(&Ut_CFE_PSP_MEMUTILS_HookTable, 0,
                                   sizeof(Ut_CFE_PSP_MEMUTILS_HookTable));
    memset(&Ut_CFE_PSP_MEMUTILS_ReturnCodeTable, 0,
                             sizeof(Ut_CFE_PSP_MEMUTILS_ReturnCodeTable));

    memset(&Ut_CFE_PSP_TIMER_HookTable, 0,
                                      sizeof(Ut_CFE_PSP_TIMER_HookTable));
    memset(&Ut_CFE_PSP_TIMER_ReturnCodeTable, 0,
                                sizeof(Ut_CFE_PSP_TIMER_ReturnCodeTable));
}

void LD_Test_TearDown(void)
{
    memset(&Ut_CFE_PSP_MEMUTILS_HookTable, 0,
                                   sizeof(Ut_CFE_PSP_MEMUTILS_HookTable));
    memset(&Ut_CFE_PSP_MEMUTILS_ReturnCodeTable, 0,
                             sizeof(Ut_CFE_PSP_MEMUTILS_ReturnCodeTable));

    memset(&Ut_CFE_PSP_TIMER_HookTable, 0,
                                      sizeof(Ut_CFE_PSP_TIMER_HookTable));
    memset(&Ut_CFE_PSP_TIMER_ReturnCodeTable, 0,
                                sizeof(Ut_CFE_PSP_TIMER_ReturnCodeTable));
}

void LD_Test_PrintCmdMsg(void *pMsg, uint32 size)
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

double LD_Test_GetConfigDataChecksum(LD_ConfigTbl_t *pTbl)
{
    double checksum = 0.0;

    checksum = (
        (double)pTbl->LD_Z_VEL_MAX + (double)pTbl->LD_XY_VEL_MAX +
        (double)pTbl->LD_ALT_MAX + (double)pTbl->LD_LOW_T_THR +
        (double)pTbl->LD_MAN_MIN_THR + (double)pTbl->LD_POS_STK_UP_THRES +
        (double)pTbl->LD_POS_STK_DW_THRES + (double)pTbl->LD_LANDSPEED +
        (double)pTbl->LD_LNDFW_AIRSPD_MAX +
        (double)pTbl->LD_MIN_THR_NO_ALT_TIMEOUT);

    std::cout.precision(17);
    std::cout << "\nConfigDataChecksum(with Max precision): " <<
                 checksum << "\n";

    std::cout.precision(7);
    std::cout << "\nConfigTable Values:\n";
    std::cout << "LD_Z_VEL_MAX: " << pTbl->LD_Z_VEL_MAX << "\n";
    std::cout << "LD_XY_VEL_MAX: " << pTbl->LD_XY_VEL_MAX << "\n";
    std::cout << "LD_ALT_MAX: " << pTbl->LD_ALT_MAX << "\n";
    std::cout << "LD_LOW_T_THR: " << pTbl->LD_LOW_T_THR << "\n";
    std::cout << "LD_MAN_MIN_THR: " << pTbl->LD_MAN_MIN_THR << "\n";
    std::cout << "LD_POS_STK_UP_THRES: " << pTbl->LD_POS_STK_UP_THRES << "\n";
    std::cout << "LD_POS_STK_DW_THRES: " << pTbl->LD_POS_STK_DW_THRES << "\n";
    std::cout << "LD_LANDSPEED: " << pTbl->LD_LANDSPEED << "\n";
    std::cout << "LD_LNDFW_AIRSPD_MAX: " << pTbl->LD_LNDFW_AIRSPD_MAX << "\n";
    std::cout << "LD_MIN_THR_NO_ALT_TIMEOUT: " <<
                  pTbl->LD_MIN_THR_NO_ALT_TIMEOUT << "\n";
    std::cout << "LD_OP_MODE: " << pTbl->LD_OP_MODE << "\n";

    return checksum;
}

uint64 LD_Test_GetTimeUs(void)
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

time_t LD_Test_GetTimeFromTimestamp(uint64 timestamp)
{
    time_t  local_time;

    local_time = (time_t)(timestamp / 1000000);

    return local_time;
}

time_t LD_Test_GetTimeFromMsg(CFE_TIME_SysTime_t cfe_time)
{
    time_t   local_time;

    local_time = (time_t)cfe_time.Seconds;

    return local_time;
}
