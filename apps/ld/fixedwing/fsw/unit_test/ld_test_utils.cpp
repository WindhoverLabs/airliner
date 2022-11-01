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


extern  LD_ConfigTbl_t LD_ConfigTbl;


extern Ut_CFE_PSP_MEMUTILS_HookTable_t         Ut_CFE_PSP_MEMUTILS_HookTable;
extern Ut_CFE_PSP_MEMUTILS_ReturnCodeTable_t
                  Ut_CFE_PSP_MEMUTILS_ReturnCodeTable[UT_CFE_PSP_MEMUTILS_MAX_INDEX];

extern Ut_CFE_PSP_TIMER_HookTable_t            Ut_CFE_PSP_TIMER_HookTable;
extern Ut_CFE_PSP_TIMER_ReturnCodeTable_t
                  Ut_CFE_PSP_TIMER_ReturnCodeTable[UT_CFE_PSP_TIMER_MAX_INDEX];

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

    Ut_CFE_TBL_AddTable(LD_CONFIG_TABLE_FILENAME, (void *) &LD_ConfigTbl);

    memset(&Ut_CFE_PSP_MEMUTILS_HookTable, 0, sizeof(Ut_CFE_PSP_MEMUTILS_HookTable));
    memset(&Ut_CFE_PSP_MEMUTILS_ReturnCodeTable, 0,
                                 sizeof(Ut_CFE_PSP_MEMUTILS_ReturnCodeTable));

    memset(&Ut_CFE_PSP_TIMER_HookTable, 0, sizeof(Ut_CFE_PSP_TIMER_HookTable));
    memset(&Ut_CFE_PSP_TIMER_ReturnCodeTable, 0,
                                 sizeof(Ut_CFE_PSP_TIMER_ReturnCodeTable));
}

void LD_Test_TearDown(void)
{
    memset(&Ut_CFE_PSP_MEMUTILS_HookTable, 0, sizeof(Ut_CFE_PSP_MEMUTILS_HookTable));
    memset(&Ut_CFE_PSP_MEMUTILS_ReturnCodeTable, 0,
                                  sizeof(Ut_CFE_PSP_MEMUTILS_ReturnCodeTable));

    memset(&Ut_CFE_PSP_TIMER_HookTable, 0, sizeof(Ut_CFE_PSP_TIMER_HookTable));
    memset(&Ut_CFE_PSP_TIMER_ReturnCodeTable, 0,
                                     sizeof(Ut_CFE_PSP_TIMER_ReturnCodeTable));
}
