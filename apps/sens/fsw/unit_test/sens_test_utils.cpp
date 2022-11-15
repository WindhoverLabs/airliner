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

#include "sens_test_utils.hpp"

#include "cfe.h"
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

extern Ut_CFE_PSP_TIMER_HookTable_t               Ut_CFE_PSP_TIMER_HookTable;
extern Ut_CFE_PSP_TIMER_ReturnCodeTable_t
                Ut_CFE_PSP_TIMER_ReturnCodeTable[UT_CFE_PSP_TIMER_MAX_INDEX];

/*
 * Config table for testing
 */
SENS_ConfigTbl_t SENS_ConfigTblUnitTest = {
    /* Expected RC mapping:
    * CH 1: Roll
    * CH 2: Pitch
    * CH 3: Throttle
    * CH 4: Yaw
    */

    /* RC%d_MIN  (0..17)    */
    982.0f, // Throttle min
    982.0f, // Roll min
    982.0f, // Pitch min
    982.0f, // Yaw min
    982.0f,
    982.0f,
    982.0f,
    982.0f,
    982.0f,
    982.0f,
    982.0f,
    982.0f,
    982.0f,
    982.0f,
    982.0f,
    982.0f,
    982.0f,
    982.0f,
    /* RC%d_TRIM (0..17)    */
    982.0f,  // Throttle trim - set this equal to min
    1493.0f, // Roll trim
    1493.0f, // Pitch trim
    1493.0f, // Yaw trim
    1493.0f,
    1493.0f,
    1493.0f,
    1493.0f,
    1493.0f,
    1493.0f,
    1493.0f,
    1493.0f,
    1493.0f,
    1493.0f,
    1493.0f,
    1493.0f,
    1493.0f,
    1493.0f,
    /* RC%d_MAX  (0..17)    */
    2006.0f, // Throttle max
    2006.0f, // Roll max
    2006.0f, // Pitch max
    2006.0f, // Yaw max
    2006.0f,
    2006.0f,
    2006.0f,
    2006.0f,
    2006.0f,
    2006.0f,
    2006.0f,
    2006.0f,
    2006.0f,
    2006.0f,
    2006.0f,
    2006.0f,
    2006.0f,
    2006.0f,
    /* RC%d_REV  (0..17)    */
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    /* RC%d_DZ   (0..17)    */
    10.0f,
    10.0f,
    10.0f,
    10.0f,
    10.0f,
    10.0f,
    10.0f,
    10.0f,
    10.0f,
    10.0f,
    10.0f,
    10.0f,
    10.0f,
    10.0f,
    10.0f,
    10.0f,
    10.0f,
    10.0f,

    2,  /* RC_MAP_ROLL          */
    3,  /* RC_MAP_PITCH         */
    4,  /* RC_MAP_YAW           */
    1,  /* RC_MAP_THROTTLE      */
    0,  /* RC_MAP_FAILSAFE      */

    8,   /* RC_MAP_MODE_SW       */
    12,  /* RC_MAP_RETURN_SW     */
    10,  /* RC_MAP_RATT_SW       */
    0,   /* RC_MAP_POSCTL_SW     */
    0,   /* RC_MAP_LOITER_SW     */
    0,   /* RC_MAP_ACRO_SW       */
    0,   /* RC_MAP_OFFB_SW       */
    7,   /* RC_MAP_KILL_SW       */
    6,   /* RC_MAP_ARM_SW        */
    11,  /* RC_MAP_TRANS_SW      */
    5,   /* RC_MAP_GEAR_SW       */
    0,   /* RC_MAP_STAB_SW       */
    0,   /* RC_MAP_MAN_SW        */
    0,   /* RC_MAP_FLAPS         */
    0,   /* RC_MAP_ALTCTL_SW     */

    0,  /* RC_MAP_AUX1          */
    0,  /* RC_MAP_AUX2          */
    0,  /* RC_MAP_AUX3          */
    0,  /* RC_MAP_AUX4          */
    0,  /* RC_MAP_AUX5          */

    0,  /* RC_MAP_PARAM%d (1)   */
    0,  /* RC_MAP_PARAM%d (2)   */
    0,  /* RC_MAP_PARAM%d (3)   */

    0,  /* RC_MAP_FLTMODE       */

    0,         /* RC_FAILS_THR         */
    0.2313f,   /* RC_ASSIST_TH         */
    0.9770f,   /* RC_AUTO_TH           */
    0.5f,      /* RC_RATT_TH           */
    0.6185f,   /* RC_POSCTL_TH         */
    0.5f,      /* RC_RETURN_TH         */
    0.5f,      /* RC_LOITER_TH         */
    0.5f,      /* RC_ACRO_TH           */
    0.5f,      /* RC_OFFB_TH           */
    0.5f,      /* RC_KILLSWITCH_TH     */
    0.5f,      /* RC_ARMSWITCH_TH      */
    0.25f,     /* RC_TRANS_TH          */
    0.25f,     /* RC_GEAR_TH           */
    0.5f,      /* RC_STAB_TH           */
    0.5f,      /* RC_MAN_TH            */
    0.5f,      /* RC_ALTCTL_TH         */

    50.0f,  /* RC_FLT_SMP_RATE      */
    10.0f,  /* RC_FLT_CUTOFF        */

    1       /* ModeSwitchControl    */
};

/*
 * Function Definitions
 */

void SENS_Test_Setup(void)
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

    Ut_CFE_TBL_AddTable(SENS_CONFIG_TABLE_FILENAME, (void *) &SENS_ConfigTbl);

    memset(&Ut_CFE_PSP_MEMUTILS_HookTable, 0,
            sizeof(Ut_CFE_PSP_MEMUTILS_HookTable));
    memset(&Ut_CFE_PSP_MEMUTILS_ReturnCodeTable, 0,
            sizeof(Ut_CFE_PSP_MEMUTILS_ReturnCodeTable));

    memset(&Ut_CFE_PSP_TIMER_HookTable, 0,
            sizeof(Ut_CFE_PSP_TIMER_HookTable));
    memset(&Ut_CFE_PSP_TIMER_ReturnCodeTable, 0,
            sizeof(Ut_CFE_PSP_TIMER_ReturnCodeTable));
}

void SENS_Test_TearDown(void) {
    memset(&Ut_CFE_PSP_MEMUTILS_HookTable, 0,
            sizeof(Ut_CFE_PSP_MEMUTILS_HookTable));
    memset(&Ut_CFE_PSP_MEMUTILS_ReturnCodeTable, 0,
            sizeof(Ut_CFE_PSP_MEMUTILS_ReturnCodeTable));

    memset(&Ut_CFE_PSP_TIMER_HookTable, 0,
            sizeof(Ut_CFE_PSP_TIMER_HookTable));
    memset(&Ut_CFE_PSP_TIMER_ReturnCodeTable, 0,
            sizeof(Ut_CFE_PSP_TIMER_ReturnCodeTable));
}

void SENS_Test_PrintCmdMsg(void *pMsg, uint32 size)
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

uint64 SENS_Test_GetTimeUs(void)
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

time_t SENS_Test_GetTimeFromTimestamp(uint64 timestamp)
{
    time_t  local_time;

    local_time = (time_t)(timestamp / 1000000);

    return local_time;
}

time_t SENS_Test_GetTimeFromMsg(CFE_TIME_SysTime_t cfe_time)
{
    time_t   local_time;

    local_time = (time_t)cfe_time.Seconds;

    return local_time;
}
