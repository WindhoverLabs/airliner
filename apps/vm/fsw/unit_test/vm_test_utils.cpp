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

#include "vm_test_utils.hpp"

#include "cfe.h"
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


extern VM cpyVM;

extern Ut_CFE_PSP_MEMUTILS_HookTable_t         Ut_CFE_PSP_MEMUTILS_HookTable;
extern Ut_CFE_PSP_MEMUTILS_ReturnCodeTable_t
                Ut_CFE_PSP_MEMUTILS_ReturnCodeTable[UT_CFE_PSP_MEMUTILS_MAX_INDEX];

extern Ut_CFE_PSP_TIMER_HookTable_t            Ut_CFE_PSP_TIMER_HookTable;
extern Ut_CFE_PSP_TIMER_ReturnCodeTable_t
                  Ut_CFE_PSP_TIMER_ReturnCodeTable[UT_CFE_PSP_TIMER_MAX_INDEX];

/*
 * Config table for UnitTest
 */
VM_ConfigTbl_t VM_ConfigTblUnitTest =
{
    2,                          /* COM_RC_IN_MODE       */
    0,                          /* COM_ARM_SWISBTN      */
    1000,                       /* COM_RC_ARM_HYST      */
    1,                          /* MAV_SYS_ID           */
    1,                          /* MAV_COMP_ID          */
    2,                          /* COM_RC_LOSS_T        */
    1,                          /* COM_LOW_BAT_ACT      */
    5.0,                        /* COM_HOME_H_T         */
    10.0,                       /* COM_HOME_V_T         */
    0.0,                        /* HOME_POS_ALT_PADDING */
    VM_VEHICLE_TYPE_FIXED_WING  /* VEHICLE_TYPE         */
};

/*
 * Invalid Config table
 */
VM_ConfigTbl_t VM_ConfigTblInvalid =
{
    COM_RC_IN_MODE_MAX + 1,     /* COM_RC_IN_MODE       */
    2,                          /* COM_ARM_SWISBTN      */
    COM_RC_ARM_HYST_MAX + 1,    /* COM_RC_ARM_HYST      */
    MAV_SYS_ID_MAX + 1,         /* MAV_SYS_ID           */
    MAV_COMP_ID_MAX + 1,        /* MAV_COMP_ID          */
    COM_RC_LOSS_T_MAX + 1.0,    /* COM_RC_LOSS_T        */
    COM_LOW_BAT_ACT_MAX + 1,    /* COM_LOW_BAT_ACT      */
    COM_HOME_H_T_MAX + 1.0,     /* COM_HOME_H_T         */
    COM_HOME_V_T_MAX + 1.0,     /* COM_HOME_V_T         */
    0.0,                        /* HOME_POS_ALT_PADDING */
    VM_VEHICLE_TYPE_FIXED_WING  /* VEHICLE_TYPE         */
};


/*
 * Function Definitions
 */

void VM_Test_Setup(void)
{
    /* initialize test environment to default state for every test */
    CFE_PSP_MemCpy((void*)&oVM, (void*)&cpyVM, sizeof(VM));

    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_TIME_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();

#if 0
    Ut_CFE_TBL_AddTable(VM_CONFIG_TABLE_FILENAME, (void *) &VM_ConfigTbl);
#else
    Ut_CFE_TBL_AddTable(VM_CONFIG_TABLE_FILENAME, (void *) &VM_ConfigTblUnitTest);
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

void VM_Test_Setup_InvalidConfigTbl(void)
{
    /* initialize test environment to default state for every test */
    CFE_PSP_MemCpy((void*)&oVM, (void*)&cpyVM, sizeof(VM));

    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_TIME_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();

    Ut_CFE_TBL_AddTable(VM_CONFIG_TABLE_FILENAME, (void *) &VM_ConfigTblInvalid);

    memset(&Ut_CFE_PSP_MEMUTILS_HookTable, 0,
            sizeof(Ut_CFE_PSP_MEMUTILS_HookTable));
    memset(&Ut_CFE_PSP_MEMUTILS_ReturnCodeTable, 0,
            sizeof(Ut_CFE_PSP_MEMUTILS_ReturnCodeTable));

    memset(&Ut_CFE_PSP_TIMER_HookTable, 0,
            sizeof(Ut_CFE_PSP_TIMER_HookTable));
    memset(&Ut_CFE_PSP_TIMER_ReturnCodeTable, 0,
            sizeof(Ut_CFE_PSP_TIMER_ReturnCodeTable));
}

void VM_Test_TearDown(void) {
    memset(&Ut_CFE_PSP_MEMUTILS_HookTable, 0,
            sizeof(Ut_CFE_PSP_MEMUTILS_HookTable));
    memset(&Ut_CFE_PSP_MEMUTILS_ReturnCodeTable, 0,
            sizeof(Ut_CFE_PSP_MEMUTILS_ReturnCodeTable));

    memset(&Ut_CFE_PSP_TIMER_HookTable, 0,
            sizeof(Ut_CFE_PSP_TIMER_HookTable));
    memset(&Ut_CFE_PSP_TIMER_ReturnCodeTable, 0,
            sizeof(Ut_CFE_PSP_TIMER_ReturnCodeTable));
}

void VM_Test_PrintCmdMsg(void *pMsg, uint32 size)
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


uint64 VM_Test_GetTimeUs(void)
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

time_t VM_Test_GetTimeFromTimestamp(uint64 timestamp)
{
    time_t  local_time;

    local_time = (time_t)(timestamp / 1000000);

    return local_time;
}

time_t VM_Test_GetTimeFromMsg(CFE_TIME_SysTime_t cfe_time)
{
    time_t   local_time;

    local_time = (time_t)cfe_time.Seconds;

    return local_time;
}
