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

#include "math/Limits.hpp"

#include <time.h>
#include <float.h>


extern FAC cpyFAC;

extern Ut_CFE_PSP_MEMUTILS_HookTable_t         Ut_CFE_PSP_MEMUTILS_HookTable;
extern Ut_CFE_PSP_MEMUTILS_ReturnCodeTable_t
                  Ut_CFE_PSP_MEMUTILS_ReturnCodeTable[UT_CFE_PSP_MEMUTILS_MAX_INDEX];

extern Ut_CFE_PSP_TIMER_HookTable_t            Ut_CFE_PSP_TIMER_HookTable;
extern Ut_CFE_PSP_TIMER_ReturnCodeTable_t
                  Ut_CFE_PSP_TIMER_ReturnCodeTable[UT_CFE_PSP_TIMER_MAX_INDEX];


/*
 * Config table for Tailsitter status
 */
FAC_ParamTbl_t FAC_ParamTblTailSitter =
{
    0.4f,    /* Attitude Roll Time Constant (FW_R_TC) */
    0.4f,    /* Attitude pitch time constant (FW_P_TC) */
    0.01f,   /* Pitch rate proportional gain (FW_PR_P) */
    0.02f,   /* Pitch rate integrator gain (FW_PR_I) */
    5.0f,    /* Maximum positive / up pitch rate (FW_P_RMAX_POS) */
    20.0f,   /* Maximum negative / down pitch rate (FW_P_RMAX_NEG) */
    0.2f,    /* Pitch rate integrator limit (FW_PR_IMAX) */
    0.01f,   /* Roll rate proportional Gain (FW_RR_P) */
    0.01f,   /* Roll rate integrator Gain (FW_RR_I) */
    0.2f,    /* Roll integrator anti-windup (FW_RR_IMAX) */
    10.0f,   /* Maximum roll rate (FW_R_RMAX) */
    0.02f,   /* Yaw rate proportional gain (FW_YR_P) */
    0.0f,    /* Yaw rate integrator gain (FW_YR_I) */
    0.2f,    /* Yaw rate integrator limit (FW_YR_IMAX) */
    0.0f,    /* Maximum yaw rate (FW_Y_RMAX) */
    0.0f,    /* Roll control to yaw control feedforward gain (FW_RLL_TO_YAW_FF) */
    0,       /* Enable wheel steering controller (FW_W_EN) */
    0.5f,    /* Wheel steering rate proportional gain (FW_WR_P) */
    0.1f,    /* Wheel steering rate integrator gain (FW_WR_I) */
    1.0f,    /* Wheel steering rate integrator limit (FW_WR_IMAX) */
    0.0f,    /* Maximum wheel steering rate (FW_W_RMAX) */
    0.5f,    /* Roll rate feed forward (FW_RR_FF) */
    0.5f,    /* Pitch rate feed forward (FW_PR_FF) */
    0.3f,    /* Yaw rate feed forward (FW_YR_FF) */
    0.2f,    /* Wheel steering rate feed forward (FW_WR_FF) */
    1000.0f, /* Minimal speed for yaw coordination (FW_YCO_VMIN) */
    0,       /* Method used for yaw coordination (FW_YCO_METHOD) */
    0.0f,    /* Roll setpoint offset (FW_RSP_OFF) */
    0.0f,    /* Pitch setpoint offset (FW_PSP_OFF) */
    45.0f,   /* Max manual roll (FW_MAN_R_MAX) */
    10.0f,   /* Max manual pitch (FW_MAN_P_MAX) */
    1.0f,    /* Scale factor for flaps (FW_FLAPS_SCL) */
    0.0f,    /* Scale factor for flaperons (FW_FLAPERON_SCL) */
    0,       /* Disable airspeed sensor (FW_ARSP_MODE) */
    1.0f,    /* Manual roll scale (FW_MAN_R_SC) */
    1.0f,    /* Manual pitch scale (FW_MAN_P_SC) */
    1.0f,    /* Manual yaw scale (FW_MAN_Y_SC) */
    0,       /* Whether to scale throttle by battery power level (FW_BAT_SCALE_EN) */
    90.0f,   /* Acro body x max rate (FW_ACRO_X_MAX) */
    90.0f,   /* Acro body y max rate (FW_ACRO_Y_MAX) */
    45.0f,   /* Acro body z max rate (FW_ACRO_Z_MAX) */
    0.8f,    /* Threshold for Rattitude mode (FW_RATT_TH) */
    10.0f,   /* Minimum Airspeed (FW_AIRSPD_MIN) */
    20.0f,   /* Maximum Airspeed (FW_AIRSPD_MAX) */
    15.0f,   /* Cruise Airspeed (FW_AIRSPD_TRIM) */
    0.0f,    /* Roll trim (TRIM_ROLL) */
    0.0f,    /* Pitch trim (TRIM_PITCH) */
    0.0f,    /* Yaw trim (TRIM_YAW) */
    0        /* VTOL Type (Tailsitter=0, Tiltrotor=1, Standard=2) (VT_TYPE) */
};

/*
 * Config table for not TailSitter status
 */
FAC_ParamTbl_t FAC_ParamTblStandard =
{
    0.4f,    /* Attitude Roll Time Constant (FW_R_TC) */
    0.4f,    /* Attitude pitch time constant (FW_P_TC) */
    0.01f,   /* Pitch rate proportional gain (FW_PR_P) */
    0.02f,   /* Pitch rate integrator gain (FW_PR_I) */
    5.0f,    /* Maximum positive / up pitch rate (FW_P_RMAX_POS) */
    20.0f,   /* Maximum negative / down pitch rate (FW_P_RMAX_NEG) */
    0.2f,    /* Pitch rate integrator limit (FW_PR_IMAX) */
    0.01f,   /* Roll rate proportional Gain (FW_RR_P) */
    0.01f,   /* Roll rate integrator Gain (FW_RR_I) */
    0.2f,    /* Roll integrator anti-windup (FW_RR_IMAX) */
    10.0f,   /* Maximum roll rate (FW_R_RMAX) */
    0.02f,   /* Yaw rate proportional gain (FW_YR_P) */
    0.0f,    /* Yaw rate integrator gain (FW_YR_I) */
    0.2f,    /* Yaw rate integrator limit (FW_YR_IMAX) */
    0.0f,    /* Maximum yaw rate (FW_Y_RMAX) */
    0.0f,    /* Roll control to yaw control feedforward gain (FW_RLL_TO_YAW_FF) */
    0,       /* Enable wheel steering controller (FW_W_EN) */
    0.5f,    /* Wheel steering rate proportional gain (FW_WR_P) */
    0.1f,    /* Wheel steering rate integrator gain (FW_WR_I) */
    1.0f,    /* Wheel steering rate integrator limit (FW_WR_IMAX) */
    0.0f,    /* Maximum wheel steering rate (FW_W_RMAX) */
    0.5f,    /* Roll rate feed forward (FW_RR_FF) */
    0.5f,    /* Pitch rate feed forward (FW_PR_FF) */
    0.3f,    /* Yaw rate feed forward (FW_YR_FF) */
    0.2f,    /* Wheel steering rate feed forward (FW_WR_FF) */
    1000.0f, /* Minimal speed for yaw coordination (FW_YCO_VMIN) */
    0,       /* Method used for yaw coordination (FW_YCO_METHOD) */
    0.0f,    /* Roll setpoint offset (FW_RSP_OFF) */
    0.0f,    /* Pitch setpoint offset (FW_PSP_OFF) */
    45.0f,   /* Max manual roll (FW_MAN_R_MAX) */
    10.0f,   /* Max manual pitch (FW_MAN_P_MAX) */
    1.0f,    /* Scale factor for flaps (FW_FLAPS_SCL) */
    0.0f,    /* Scale factor for flaperons (FW_FLAPERON_SCL) */
    0,       /* Disable airspeed sensor (FW_ARSP_MODE) */
    1.0f,    /* Manual roll scale (FW_MAN_R_SC) */
    1.0f,    /* Manual pitch scale (FW_MAN_P_SC) */
    1.0f,    /* Manual yaw scale (FW_MAN_Y_SC) */
    0,       /* Whether to scale throttle by battery power level (FW_BAT_SCALE_EN) */
    90.0f,   /* Acro body x max rate (FW_ACRO_X_MAX) */
    90.0f,   /* Acro body y max rate (FW_ACRO_Y_MAX) */
    45.0f,   /* Acro body z max rate (FW_ACRO_Z_MAX) */
    0.8f,    /* Threshold for Rattitude mode (FW_RATT_TH) */
    10.0f,   /* Minimum Airspeed (FW_AIRSPD_MIN) */
    20.0f,   /* Maximum Airspeed (FW_AIRSPD_MAX) */
    15.0f,   /* Cruise Airspeed (FW_AIRSPD_TRIM) */
    0.0f,    /* Roll trim (TRIM_ROLL) */
    0.0f,    /* Pitch trim (TRIM_PITCH) */
    0.0f,    /* Yaw trim (TRIM_YAW) */
    2        /* VTOL Type (Tailsitter=0, Tiltrotor=1, Standard=2) (VT_TYPE) */
};

/*
 * Function Definitions
 */

void FAC_Test_Setup(void)
{
    /* initialize test environment to default state for every test */

    CFE_PSP_MemCpy((void*)&oFAC, (void*)&cpyFAC, sizeof(FAC));

    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_TIME_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();

    if (1)
    {
        Ut_CFE_TBL_AddTable(FAC_PARAM_TABLE_FILENAME, (void *) &FAC_ParamTblStandard);
    }
    else
    {
        Ut_CFE_TBL_AddTable(FAC_PARAM_TABLE_FILENAME, (void *) &FAC_ParamTbl);
    }

    memset(&Ut_CFE_PSP_MEMUTILS_HookTable, 0, sizeof(Ut_CFE_PSP_MEMUTILS_HookTable));
    memset(&Ut_CFE_PSP_MEMUTILS_ReturnCodeTable, 0, sizeof(Ut_CFE_PSP_MEMUTILS_ReturnCodeTable));

    memset(&Ut_CFE_PSP_TIMER_HookTable, 0, sizeof(Ut_CFE_PSP_TIMER_HookTable));
    memset(&Ut_CFE_PSP_TIMER_ReturnCodeTable, 0, sizeof(Ut_CFE_PSP_TIMER_ReturnCodeTable));
}

void FAC_Test_Setup_TailSitter(void)
{
    /* initialize test environment to default state for every test */

    CFE_PSP_MemCpy((void*)&oFAC, (void*)&cpyFAC, sizeof(FAC));

    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_TIME_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();

    Ut_CFE_TBL_AddTable(FAC_PARAM_TABLE_FILENAME, (void *) &FAC_ParamTblTailSitter);

    memset(&Ut_CFE_PSP_MEMUTILS_HookTable, 0, sizeof(Ut_CFE_PSP_MEMUTILS_HookTable));
    memset(&Ut_CFE_PSP_MEMUTILS_ReturnCodeTable, 0, sizeof(Ut_CFE_PSP_MEMUTILS_ReturnCodeTable));

    memset(&Ut_CFE_PSP_TIMER_HookTable, 0, sizeof(Ut_CFE_PSP_TIMER_HookTable));
    memset(&Ut_CFE_PSP_TIMER_ReturnCodeTable, 0, sizeof(Ut_CFE_PSP_TIMER_ReturnCodeTable));
}

void FAC_Test_TearDown(void)
{
    memset(&Ut_CFE_PSP_MEMUTILS_HookTable, 0, sizeof(Ut_CFE_PSP_MEMUTILS_HookTable));
    memset(&Ut_CFE_PSP_MEMUTILS_ReturnCodeTable, 0, sizeof(Ut_CFE_PSP_MEMUTILS_ReturnCodeTable));

    memset(&Ut_CFE_PSP_TIMER_HookTable, 0, sizeof(Ut_CFE_PSP_TIMER_HookTable));
    memset(&Ut_CFE_PSP_TIMER_ReturnCodeTable, 0, sizeof(Ut_CFE_PSP_TIMER_ReturnCodeTable));
}

void FAC_Test_PrintCmdMsg(void *pMsg, uint32 size)
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

void FAC_Test_PrintEclValues()
{
    ECL_PitchController *pPitch = &oFAC._pitch_ctrl;
    ECL_RollController  *pRoll = &oFAC._roll_ctrl;
    ECL_YawController   *pYaw = &oFAC._yaw_ctrl;
    ECL_WheelController *pWheel = &oFAC._wheel_ctrl;

    printf("PitchController::_tc: %f\n", pPitch->get_time_constant());
    printf("PitchController::_k_p: %f\n", pPitch->get_k_p());
    printf("PitchController::_k_i: %f\n", pPitch->get_k_i());
    printf("PitchController::_k_ff: %f\n", pPitch->get_k_ff());
    printf("PitchController::_integrator_max: %f\n", pPitch->get_integrator_max());
    printf("PitchController::_max_rate: %f\n", pPitch->get_max_rate_pos());
    printf("PitchController::_max_rate_neg: %f\n\n", pPitch->get_max_rate_neg());

    printf("RollController::_tc: %f\n", pRoll->get_time_constant());
    printf("RollController::_k_p: %f\n", pRoll->get_k_p());
    printf("RollController::_k_i: %f\n", pRoll->get_k_i());
    printf("RollController::_k_ff: %f\n", pRoll->get_k_ff());
    printf("RollController::_integrator_max: %f\n", pRoll->get_integrator_max());
    printf("RollController::_max_rate: %f\n\n", pRoll->get_max_rate());

    printf("YawController::_k_p: %f\n", pYaw->get_k_p());
    printf("YawController::_k_i: %f\n", pYaw->get_k_i());
    printf("YawController::_k_ff: %f\n", pYaw->get_k_ff());
    printf("YawController::_integrator_max: %f\n", pYaw->get_integrator_max());
    printf("YawController::_coordinated_min_speed: %f\n", pYaw->get_coordinated_min_speed());
    printf("YawController::_coordinated_method: %d\n", pYaw->get_coordinated_method());
    printf("YawController::_max_rate: %f\n\n", pYaw->get_max_rate());

    printf("WheelController::_k_p: %f\n", pWheel->get_k_p());
    printf("WheelController::_k_i: %f\n", pWheel->get_k_i());
    printf("WheelController::_k_ff: %f\n", pWheel->get_k_ff());
    printf("WheelController::_integrator_max: %f\n", pWheel->get_integrator_max());
    printf("WheelController::_max_rate: %f\n\n", pWheel->get_max_rate());

    return;
}

uint32 FAC_Test_ValidateEclValues()
{
    uint32 uResult = 0x0;
    FAC_ParamTbl_t *pTbl = oFAC.ParamTblPtr;
    ECL_PitchController *pPitch = &oFAC._pitch_ctrl;
    ECL_RollController  *pRoll = &oFAC._roll_ctrl;
    ECL_YawController   *pYaw = &oFAC._yaw_ctrl;
    ECL_WheelController *pWheel = &oFAC._wheel_ctrl;

    if (fabs(pPitch->get_time_constant() - pTbl->FW_P_TC) > FLT_EPSILON)
    {
        uResult |= 0x01;
    }
    if (fabs(pPitch->get_k_p() - pTbl->FW_PR_P) > FLT_EPSILON)
    {
        uResult |= 0x02;
    }
    if (fabs(pPitch->get_k_i() - pTbl->FW_PR_I) > FLT_EPSILON)
    {
        uResult |= 0x04;
    }
    if (fabs(pPitch->get_k_ff() - pTbl->FW_PR_FF) > FLT_EPSILON)
    {
        uResult |= 0x08;
    }
    if (fabs(pPitch->get_integrator_max() - pTbl->FW_PR_IMAX) > FLT_EPSILON)
    {
        uResult |= 0x10;
    }
    if (fabs(pPitch->get_max_rate_pos() -
             math::radians(pTbl->FW_P_RMAX_POS)) > FLT_EPSILON)
    {
        uResult |= 0x20;
    }
    if (fabs(pPitch->get_max_rate_neg() -
             math::radians(pTbl->FW_P_RMAX_NEG)) > FLT_EPSILON)
    {
        uResult |= 0x40;
    }

    if (fabs(pRoll->get_time_constant() - pTbl->FW_R_TC) > FLT_EPSILON)
    {
        uResult |= 0x100;
    }
    if (fabs(pRoll->get_k_p() - pTbl->FW_RR_P) > FLT_EPSILON)
    {
        uResult |= 0x200;
    }
    if (fabs(pRoll->get_k_i() - pTbl->FW_RR_I) > FLT_EPSILON)
    {
        uResult |= 0x400;
    }
    if (fabs(pRoll->get_k_ff() - pTbl->FW_RR_FF) > FLT_EPSILON)
    {
        uResult |= 0x800;
    }
    if (fabs(pRoll->get_integrator_max() - pTbl->FW_RR_IMAX) > FLT_EPSILON)
    {
        uResult |= 0x1000;
    }
    if (fabs(pRoll->get_max_rate() -
             math::radians(pTbl->FW_R_RMAX)) > FLT_EPSILON)
    {
        uResult |= 0x2000;
    }

    if (fabs(pYaw->get_k_p() - pTbl->FW_YR_P) > FLT_EPSILON)
    {
        uResult |= 0x20000;
    }
    if (fabs(pYaw->get_k_i() - pTbl->FW_YR_I) > FLT_EPSILON)
    {
        uResult |= 0x40000;
    }
    if (fabs(pYaw->get_k_ff() - pTbl->FW_YR_FF) > FLT_EPSILON)
    {
        uResult |= 0x80000;
    }
    if (fabs(pYaw->get_integrator_max() - pTbl->FW_YR_IMAX) > FLT_EPSILON)
    {
        uResult |= 0x100000;
    }
    if (fabs(pYaw->get_coordinated_min_speed() - pTbl->FW_YCO_VMIN) > FLT_EPSILON)
    {
        uResult |= 0x200000;
    }
    if (fabs((float)pYaw->get_coordinated_method() - (float)pTbl->FW_YCO_METHOD) > FLT_EPSILON)
    {
        uResult |= 0x400000;
    }
    if (fabs(pYaw->get_max_rate() - math::radians(pTbl->FW_Y_RMAX)) > FLT_EPSILON)
    {
        uResult |= 0x800000;
    }

    if (fabs(pWheel->get_k_p() - pTbl->FW_WR_P) > FLT_EPSILON)
    {
        uResult |= 0x02000000;
    }
    if (fabs(pWheel->get_k_i() - pTbl->FW_WR_I) > FLT_EPSILON)
    {
        uResult |= 0x04000000;
    }
    if (fabs(pWheel->get_k_ff() - pTbl->FW_WR_FF) > FLT_EPSILON)
    {
        uResult |= 0x08000000;
    }
    if (fabs(pWheel->get_integrator_max() - pTbl->FW_WR_IMAX) > FLT_EPSILON)
    {
        uResult |= 0x10000000;
    }
    if (fabs(pWheel->get_max_rate() - math::radians(pTbl->FW_W_RMAX)) > FLT_EPSILON)
    {
        uResult |= 0x20000000;
    }

    printf("Test_ValidateEclValues: uResult is 0x%08x\n", (unsigned int)uResult);

    return uResult;
}

time_t FAC_Test_GetTimeFromTimestamp(uint64 timestamp)
{
    time_t  local_time;

    local_time = (time_t)(timestamp / 1000000);

    return local_time;
}

time_t FAC_Test_GetTimeFromMsg(CFE_TIME_SysTime_t cfe_time)
{
    time_t   local_time;

    local_time = (time_t)cfe_time.Seconds;

    return local_time;
}
