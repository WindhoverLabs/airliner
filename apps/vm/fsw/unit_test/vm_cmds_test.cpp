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

#include "vm_cmds_test.hpp"
#include "vm_test_utils.hpp"
#include "vm_version.h"

#include "cfe.h"
#include "vm_msg.h"
#include "uttest.h"
#include "ut_osapi_stubs.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_sb_hooks.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_es_hooks.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_psp_timer_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_cfe_time_stubs.h"


/**
 * Test VM ProcessCmdPipe_GetPSPTimeHook
 */
void Test_VM_ProcessCmdPipe_GetPSPTimeHook(OS_time_t *LocalTime)
{
    int              iStatus;
    struct timespec  time;

    iStatus = clock_gettime(CLOCK_REALTIME, &time);
    if (iStatus == 0)
    {
        LocalTime->seconds = time.tv_sec;
        LocalTime->microsecs = time.tv_nsec / 1000;
    }

    return;
}


/**************************************************************************
 * Tests for VM ProcessCmdPipe()
 **************************************************************************/
/**
 * Test VM ProcessCmdPipe, InvalidCmd
 */
void Test_VM_ProcessCmdPipe_InvalidCmd(void)
{
    int32          CmdPipe;
    VM_NoArgCmd_t  InMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, 0, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    VM_Test_PrintCmdMsg((void*)&InMsg, sizeof(InMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oVM.AppMain();

    sprintf(expectedEvent, "Recvd invalid CMD msgId (0x%04X)", 0);
    UtAssert_EventSent(VM_MSGID_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "ProcessCmdPipe, InvalidCmd");
}


/**
 * Test VM ProcessCmdPipe, InvalidCmdCode
 */
void Test_VM_ProcessCmdPipe_InvalidCmdCode(void)
{
    int32          CmdPipe;
    VM_NoArgCmd_t  InMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, VM_CMD_MID, sizeof(InMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)100);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    VM_Test_PrintCmdMsg((void*)&InMsg, sizeof(InMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oVM.AppMain();

    sprintf(expectedEvent, "Recvd invalid command code (%u)", 100);
    UtAssert_EventSent(VM_CC_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "ProcessCmdPipe, InvalidCmdCode");
}


/**
 * Test VM ProcessCmdPipe, PipeError
 */
void Test_VM_ProcessCmdPipe_PipeError(void)
{
    int32          SchPipe;
    int32          expected = CFE_SB_PIPE_RD_ERR;
    VM_NoArgCmd_t  InMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    SchPipe = Ut_CFE_SB_CreatePipe(VM_SCH_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, VM_SEND_HK_MID, sizeof(InMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)SchPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, expected, 2);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oVM.AppMain();

    sprintf(expectedEvent, "CMD pipe read error (0x%08lX)", expected);

    /* Verify results */
    UtAssert_EventSent(VM_RCVMSG_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "ProcessCmdPipe, PipeError");
}


/**
 * Test VM ProcessCmdPipe, Noop
 */
void Test_VM_ProcessCmdPipe_Noop(void)
{
    int32          CmdPipe;
    VM_NoArgCmd_t  InMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, VM_CMD_MID, sizeof(InMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)VM_NOOP_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    VM_Test_PrintCmdMsg((void*)&InMsg, sizeof(InMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oVM.AppMain();

    sprintf(expectedEvent, "Recvd NOOP. Version %d.%d.%d.%d",
            VM_MAJOR_VERSION, VM_MINOR_VERSION, VM_REVISION, VM_MISSION_REV);

    UtAssert_EventSent(VM_CMD_NOOP_EID, CFE_EVS_INFORMATION, expectedEvent,
                       "ProcessCmdPipe, InvalidCmdCode");
}


/**
 * Test VM ProcessCmdPipe, Reset
 */
void Test_VM_ProcessCmdPipe_Reset(void)
{
    int32          CmdPipe;
    VM_NoArgCmd_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, VM_CMD_MID, sizeof(InMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)VM_RESET_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    VM_Test_PrintCmdMsg((void*)&InMsg, sizeof(InMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oVM.AppMain();

    if ((oVM.HkTlm.usCmdCnt == 0) && (oVM.HkTlm.usCmdErrCnt == 0)
        && (oVM.HkTlm.WakeupCount == 0) && (oVM.HkTlm.BatteryStatusMsgCount == 0)
        && (oVM.HkTlm.TelemetryStatusMsgCount == 0) && (oVM.HkTlm.SubsystemInfoMsgCount == 0)
        && (oVM.HkTlm.VehicleAttitudeMsgCount == 0) && (oVM.HkTlm.VehicleLocalPositionMsgCount == 0)
        && (oVM.HkTlm.VehicleLandDetectedMsgCount == 0) && (oVM.HkTlm.MissionResultMsgCount == 0)
        && (oVM.HkTlm.ManualControlSetpointMsgCount == 0) && (oVM.HkTlm.PositionSetpointTripletMsgCount == 0)
        && (oVM.HkTlm.SafetyMsgCount == 0) && (oVM.HkTlm.SensorCorrectionMsgCount == 0)
        && (oVM.HkTlm.VehicleControlModeMsgCount == 0) && (oVM.HkTlm.SensorCombinedMsgCount == 0)
        && (oVM.HkTlm.VehicleCommandMsgCount == 0) && (oVM.HkTlm.VehicleGlobalPositionMsgCount == 0)
        && (oVM.HkTlm.VehicleGpsPositionMsgCount == 0))
    {
        UtAssert_True(TRUE, "ProcessCmdPipe, Reset");
    }
    else
    {
        UtAssert_True(FALSE, "ProcessCmdPipe, Reset");
    }
}


/**
 * Test VM ProcessCmdPipe, VehicleArm
 */
void Test_VM_ProcessCmdPipe_VehicleArm(void)
{
    int32          CmdPipe;
    VM_NoArgCmd_t  InMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, VM_CMD_MID, sizeof(InMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)VM_VEHICLE_ARM_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    VM_Test_PrintCmdMsg((void*)&InMsg, sizeof(InMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
                        (void *)&Test_VM_ProcessCmdPipe_GetPSPTimeHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oVM.AppMain();

    sprintf(expectedEvent, "%s", "Arming::Armed");

#if 0
    UtAssert_EventSent(VM_ARMING_ENTERED_ARMED_STATE_INFO_EID, CFE_EVS_INFORMATION,
             expectedEvent, "ProcessCmdPipe, VehicleArm");
#endif
}


/**
 * Test VM ProcessCmdPipe, VehicleDisarm
 */
void Test_VM_ProcessCmdPipe_VehicleDisarm(void)
{
    int32          CmdPipe;
    VM_NoArgCmd_t  InMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, VM_CMD_MID, sizeof(InMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)VM_VEHICLE_DISARM_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    VM_Test_PrintCmdMsg((void*)&InMsg, sizeof(InMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oVM.AppMain();

    sprintf(expectedEvent, "%s", "Arming::Standby");

#if 0
    UtAssert_EventSent(VM_ARMING_ENTERED_STANDBY_STATE_INFO_EID, CFE_EVS_INFORMATION,
             expectedEvent, "ProcessCmdPipe, VehicleDisarm");
#endif
}


/**
 * Test VM ProcessCmdPipe, SetNavManual
 */
void Test_VM_ProcessCmdPipe_SetNavManual(void)
{
    int32          CmdPipe;
    VM_NoArgCmd_t  InMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, VM_CMD_MID, sizeof(InMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)VM_SET_NAV_MANUAL_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    VM_Test_PrintCmdMsg((void*)&InMsg, sizeof(InMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oVM.AppMain();

    sprintf(expectedEvent, "%s", "Arming::Standby");
}


/**
 * Test VM ProcessCmdPipe, SetNavAltctl
 */
void Test_VM_ProcessCmdPipe_SetNavAltctl(void)
{
    int32          CmdPipe;
    VM_NoArgCmd_t  InMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, VM_CMD_MID, sizeof(InMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)VM_SET_NAV_ALTCTL_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    VM_Test_PrintCmdMsg((void*)&InMsg, sizeof(InMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oVM.AppMain();
}


/**
 * Test VM ProcessCmdPipe, SetNavPosctl
 */
void Test_VM_ProcessCmdPipe_SetNavPosctl(void)
{
    int32          CmdPipe;
    VM_NoArgCmd_t  InMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, VM_CMD_MID, sizeof(InMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)VM_SET_NAV_POSCTL_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    VM_Test_PrintCmdMsg((void*)&InMsg, sizeof(InMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oVM.AppMain();
}


/**
 * Test VM ProcessCmdPipe, SetNavAutoLoiter
 */
void Test_VM_ProcessCmdPipe_SetNavAutoLoiter(void)
{
    int32          CmdPipe;
    VM_NoArgCmd_t  InMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, VM_CMD_MID, sizeof(InMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)VM_SET_NAV_AUTO_LOITER_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    VM_Test_PrintCmdMsg((void*)&InMsg, sizeof(InMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oVM.AppMain();
}


/**
 * Test VM ProcessCmdPipe, SetNavAutoRtl
 */
void Test_VM_ProcessCmdPipe_SetNavAutoRtl(void)
{
    int32          CmdPipe;
    VM_NoArgCmd_t  InMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, VM_CMD_MID, sizeof(InMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)VM_SET_NAV_AUTO_RTL_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    VM_Test_PrintCmdMsg((void*)&InMsg, sizeof(InMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oVM.AppMain();
}


/**
 * Test VM ProcessCmdPipe, SetNavAcro
 */
void Test_VM_ProcessCmdPipe_SetNavAcro(void)
{
    int32          CmdPipe;
    VM_NoArgCmd_t  InMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, VM_CMD_MID, sizeof(InMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)VM_SET_NAV_ACRO_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    VM_Test_PrintCmdMsg((void*)&InMsg, sizeof(InMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oVM.AppMain();
}


/**
 * Test VM ProcessCmdPipe, SetNavStabilize
 */
void Test_VM_ProcessCmdPipe_SetNavStabilize(void)
{
    int32          CmdPipe;
    VM_NoArgCmd_t  InMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, VM_CMD_MID, sizeof(InMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)VM_SET_NAV_STABILIZE_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    VM_Test_PrintCmdMsg((void*)&InMsg, sizeof(InMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oVM.AppMain();
}


/**
 * Test VM ProcessCmdPipe, SetNavRattitude
 */
void Test_VM_ProcessCmdPipe_SetNavRattitude(void)
{
    int32          CmdPipe;
    VM_NoArgCmd_t  InMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, VM_CMD_MID, sizeof(InMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)VM_SET_NAV_RATTITUDE_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    VM_Test_PrintCmdMsg((void*)&InMsg, sizeof(InMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oVM.AppMain();
}


/**
 * Test VM ProcessCmdPipe, SetNavAutoTakeoff
 */
void Test_VM_ProcessCmdPipe_SetNavAutoTakeoff(void)
{
    int32          CmdPipe;
    VM_NoArgCmd_t  InMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, VM_CMD_MID, sizeof(InMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)VM_SET_NAV_AUTO_TAKEOFF_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    VM_Test_PrintCmdMsg((void*)&InMsg, sizeof(InMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oVM.AppMain();
}


/**
 * Test VM ProcessCmdPipe, SetNavAutoLand
 */
void Test_VM_ProcessCmdPipe_SetNavAutoLand(void)
{
    int32          CmdPipe;
    VM_NoArgCmd_t  InMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, VM_CMD_MID, sizeof(InMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)VM_SET_NAV_AUTO_LAND_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    VM_Test_PrintCmdMsg((void*)&InMsg, sizeof(InMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oVM.AppMain();
}


/**
 * Test VM ProcessCmdPipe, SendConfiguration
 */
void Test_VM_ProcessCmdPipe_SendConfiguration(void)
{
    int32          CmdPipe;
    VM_NoArgCmd_t  InMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&InMsg, VM_CMD_MID, sizeof(InMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)VM_SEND_CONFIGURATION_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    VM_Test_PrintCmdMsg((void*)&InMsg, sizeof(InMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oVM.AppMain();
}


/**************************************************************************
 * Tests for VM VerifyCmdLength()
 **************************************************************************/
/**
 * Test VM VerifyCmdLength(), Fail CmdLength
 */
void Test_VM_VerifyCmdLength_Fail_CmdLength(void)
{
    bool              bResult = TRUE;
    bool              bExpected = FALSE;
    VM_NoArgCmd_t     CmdMsg;
    int               expectedLength = 0;
    char              expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    CFE_SB_InitMsg ((void*)&CmdMsg, VM_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)VM_NOOP_CC);

    VM_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    expectedLength = sizeof(CmdMsg) + 5;
    bResult = oVM.VerifyCmdLength((CFE_SB_MsgPtr_t)&CmdMsg, expectedLength);

    sprintf(expectedEvent, "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
            "msgLen=%d, expectedLen=%d",
            VM_CMD_MID, VM_NOOP_CC, sizeof(CmdMsg), expectedLength);

    /* Verify results */
    UtAssert_True (((bResult == bExpected) && (oVM.HkTlm.usCmdErrCnt == 1)),
                   "VerifyCmdLength, Fail CmdLength");
    UtAssert_EventSent(VM_MSGLEN_ERR_EID, CFE_EVS_ERROR, expectedEvent,
                       "VerifyCmdLength(), Fail CmdLength EventSent");
}



void VM_Cmds_Test_AddTestCases(void)
{
    UtTest_Add(Test_VM_ProcessCmdPipe_InvalidCmd,
               VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_ProcessCmdPipe_InvalidCmd");
    UtTest_Add(Test_VM_ProcessCmdPipe_InvalidCmdCode,
               VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_ProcessCmdPipe_InvalidCmdCode");
    UtTest_Add(Test_VM_ProcessCmdPipe_PipeError,
               VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_ProcessCmdPipe_PipeError");
    UtTest_Add(Test_VM_ProcessCmdPipe_Noop,
               VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_ProcessCmdPipe_Noop");
    UtTest_Add(Test_VM_ProcessCmdPipe_Reset,
               VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_ProcessCmdPipe_Reset");
    UtTest_Add(Test_VM_ProcessCmdPipe_VehicleArm,
               VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_ProcessCmdPipe_VehicleArm");
    UtTest_Add(Test_VM_ProcessCmdPipe_VehicleDisarm,
               VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_ProcessCmdPipe_VehicleDisarm");
    UtTest_Add(Test_VM_ProcessCmdPipe_SetNavManual,
               VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_ProcessCmdPipe_SetNavManual");
    UtTest_Add(Test_VM_ProcessCmdPipe_SetNavAltctl,
               VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_ProcessCmdPipe_SetNavAltctl");
    UtTest_Add(Test_VM_ProcessCmdPipe_SetNavPosctl,
               VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_ProcessCmdPipe_SetNavPosctl");
    UtTest_Add(Test_VM_ProcessCmdPipe_SetNavAutoLoiter,
               VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_ProcessCmdPipe_SetNavAutoLoiter");
    UtTest_Add(Test_VM_ProcessCmdPipe_SetNavAutoRtl,
               VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_ProcessCmdPipe_SetNavAutoRtl");
    UtTest_Add(Test_VM_ProcessCmdPipe_SetNavAcro,
               VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_ProcessCmdPipe_SetNavAcro");
    UtTest_Add(Test_VM_ProcessCmdPipe_SetNavStabilize,
               VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_ProcessCmdPipe_SetNavStabilize");
    UtTest_Add(Test_VM_ProcessCmdPipe_SetNavRattitude,
               VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_ProcessCmdPipe_SetNavRattitude");
    UtTest_Add(Test_VM_ProcessCmdPipe_SetNavAutoTakeoff,
               VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_ProcessCmdPipe_SetNavAutoTakeoff");
    UtTest_Add(Test_VM_ProcessCmdPipe_SetNavAutoLand,
               VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_ProcessCmdPipe_SetNavAutoLand");
    UtTest_Add(Test_VM_ProcessCmdPipe_SendConfiguration,
               VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_ProcessCmdPipe_SendConfiguration");

    UtTest_Add(Test_VM_VerifyCmdLength_Fail_CmdLength,
               VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_VerifyCmdLength_Fail_CmdLength");
} /* end VM_Cmds_Test_AddTestCases */
