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


extern LD cpyLD;

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
    CFE_PSP_MemCpy((void *)&oLD, (void *)&cpyLD, sizeof(LD));

    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_TIME_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();

    Ut_CFE_TBL_AddTable(LD_CONFIG_TABLE_FILENAME, (void *)&LD_ConfigTbl);

    memset(&Ut_CFE_PSP_MEMUTILS_HookTable, 0,
                                   sizeof(Ut_CFE_PSP_MEMUTILS_HookTable));
    memset(&Ut_CFE_PSP_MEMUTILS_ReturnCodeTable, 0,
                             sizeof(Ut_CFE_PSP_MEMUTILS_ReturnCodeTable));

    memset(&Ut_CFE_PSP_TIMER_HookTable, 0,
                                      sizeof(Ut_CFE_PSP_TIMER_HookTable));
    memset(&Ut_CFE_PSP_TIMER_ReturnCodeTable, 0,
                                sizeof(Ut_CFE_PSP_TIMER_ReturnCodeTable));
}

void LD_Test_SetupUT(void)
{
    /* initialize test environment to default state for every test */
    CFE_PSP_MemCpy((void *)&oLD, (void *)&cpyLD, sizeof(LD));

    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_TIME_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();

    Ut_CFE_TBL_AddTable(LD_CONFIG_TABLE_FILENAME,
                                          (void *)&LD_ConfigTblUnitTest);

    memset(&Ut_CFE_PSP_MEMUTILS_HookTable, 0,
                                   sizeof(Ut_CFE_PSP_MEMUTILS_HookTable));
    memset(&Ut_CFE_PSP_MEMUTILS_ReturnCodeTable, 0,
                             sizeof(Ut_CFE_PSP_MEMUTILS_ReturnCodeTable));

    memset(&Ut_CFE_PSP_TIMER_HookTable, 0,
                                      sizeof(Ut_CFE_PSP_TIMER_HookTable));
    memset(&Ut_CFE_PSP_TIMER_ReturnCodeTable, 0,
                                sizeof(Ut_CFE_PSP_TIMER_ReturnCodeTable));
}

void LD_Test_SetupUT_Invalid(void)
{
    /* initialize test environment to default state for every test */
    CFE_PSP_MemCpy((void *)&oLD, (void *)&cpyLD, sizeof(LD));

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

void GetActuatorArmedMsg(PX4_ActuatorArmedMsg_t *pMsg)
{
    pMsg->Timestamp = LD_Test_GetTimeUs();
    pMsg->Armed = FALSE;
    pMsg->Prearmed = FALSE;
    pMsg->ReadyToArm = TRUE;
    pMsg->Lockdown = FALSE;
    pMsg->ManualLockdown = FALSE;
    pMsg->ForceFailsafe = TRUE;
    pMsg->InEscCalibrationMode = FALSE;

    return;
}

void GetVehicleLocalPositionMsg(PX4_VehicleLocalPositionMsg_t *pMsg)
{
    pMsg->Timestamp = LD_Test_GetTimeUs();
    pMsg->RefTimestamp = LD_Test_GetTimeUs();
    pMsg->RefLat = 47.397741928975;
    pMsg->RefLon = 8.545593979817;
    pMsg->SurfaceBottomTimestamp = LD_Test_GetTimeUs();
    pMsg->X = 0.0000413358f;
    pMsg->Y = -0.00021784112f;
    pMsg->Z = 0.0003718244f;
    pMsg->Delta_XY[0] = 0.0f;
    pMsg->Delta_XY[1] = 0.0f;
    pMsg->Delta_Z = 0.0f;
    pMsg->VX = -0.027511f;
    pMsg->VY = 0.006788f;
    pMsg->VZ = -0.051438f;
    pMsg->Delta_VXY[0] = 0.0f;
    pMsg->Delta_VXY[1] = 0.0f;
    pMsg->Delta_VZ = 0.0f;
    pMsg->AX = 0.0f;
    pMsg->AY = 0.0f;
    pMsg->AZ = 0.0f;
    pMsg->Yaw = 0.49531016f;
    pMsg->RefAlt = 75.10329f;
    pMsg->DistBottom = -0.0003718244f;
    pMsg->DistBottomRate = 0.051438f;
    pMsg->EpH = 3.0f;
    pMsg->EpV = 0.03878543f;
    pMsg->EvH = 0.0f;
    pMsg->EvV = 0.0f;
    pMsg->EstimatorType = 0;
    pMsg->XY_Valid = FALSE;
    pMsg->Z_Valid = TRUE;
    pMsg->V_XY_Valid = FALSE;
    pMsg->V_Z_Valid = TRUE;
    pMsg->XY_ResetCounter = 0;
    pMsg->Z_ResetCounter = 0;
    pMsg->VXY_ResetCounter = 0;
    pMsg->VZ_ResetCounter = 0;
    pMsg->XY_Global = FALSE;
    pMsg->Z_Global = TRUE;
    pMsg->DistBottomValid = FALSE;

    return;
}

void GetAirspeedMsg(PX4_AirspeedMsg_t *pMsg)
{
    pMsg->Timestamp = LD_Test_GetTimeUs();
    pMsg->IndicatedAirspeed = 11.772184f;        /* m/s     */
    pMsg->TrueAirspeed = 11.777838f;             /* m/s     */
    pMsg->TrueAirspeedUnfiltered = 11.777838f;   /* m/s     */
    pMsg->AirTemperature = 15.0f;                /* Celsius */
    pMsg->Confidence = 0.0f;

    return;
}

void GetBatteryStatusMsg(PX4_BatteryStatusMsg_t *pMsg)
{
    pMsg->Timestamp = LD_Test_GetTimeUs();
    pMsg->Voltage = 12.0f;                    /* V   */
    pMsg->VoltageFiltered = 12.0f;            /* V   */
    pMsg->Current = 5.0f;                     /* A   */
    pMsg->CurrentFiltered = 5.0f;             /* A   */
    pMsg->Discharged = 1000.0f;               /* mAh */
    pMsg->Remaining = 0.15f;
    pMsg->Scale = 1.0f;
    pMsg->CellCount = 3;
    pMsg->Connected = TRUE;
    pMsg->Warning = PX4_BATTERY_WARNING_LOW;

    return;
}

void GetManualControlSetpointMsg(PX4_ManualControlSetpointMsg_t *pMsg)
{
    pMsg->Timestamp = LD_Test_GetTimeUs();
    pMsg->X = -0.32135728f;
    pMsg->Y = 0.03976143f;
    pMsg->Z = 0.00197238f;
    pMsg->R = 0.0f;
#if 0
    pMsg->Flaps = 0.0f;
    pMsg->Aux1 = 0.0f;
    pMsg->Aux2 = 0.0f;
    pMsg->Aux3 = 0.0f;
    pMsg->Aux4 = 0.0f;
    pMsg->Aux5 = 0.0f;
    pMsg->ModeSwitch = PX4_SWITCH_POS_NONE;
    pMsg->ReturnSwitch = PX4_SWITCH_POS_NONE;
    pMsg->RattitudeSwitch = PX4_SWITCH_POS_NONE;
    pMsg->PosctlSwitch = PX4_SWITCH_POS_NONE;
    pMsg->LoiterSwitch = PX4_SWITCH_POS_NONE;
    pMsg->AcroSwitch = PX4_SWITCH_POS_NONE;
    pMsg->OffboardSwitch = PX4_SWITCH_POS_NONE;
    pMsg->KillSwitch = PX4_SWITCH_POS_NONE;
    pMsg->TransitionSwitch = PX4_SWITCH_POS_NONE;
    pMsg->GearSwitch = PX4_SWITCH_POS_NONE;
    pMsg->ArmSwitch = PX4_SWITCH_POS_NONE;
    pMsg->StabSwitch = PX4_SWITCH_POS_NONE;
    pMsg->ManSwitch = PX4_SWITCH_POS_NONE;
    pMsg->ModeSlot = PX4_MODE_SLOT_NONE;
    pMsg->DataSource = PX4_MANUAL_CONTROL_SOURCE_RC;
    pMsg->AltctlSwitch = PX4_SWITCH_POS_NONE;
#endif

    return;
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
