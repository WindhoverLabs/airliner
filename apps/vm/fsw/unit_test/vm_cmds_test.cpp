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

#include <unistd.h>


CFE_SB_MsgId_t  SendConfHook_MsgId = 0;


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

    /* Verify results */
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

    /* Verify results */
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

    /* Verify results */
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

    /* Verify results */
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
    VM             oVMut;

    int32          CmdPipe;
    uint64         timestamp;
    VM_NoArgCmd_t  CmdMsg;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, VM_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)VM_VEHICLE_ARM_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    VM_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
                        (void *)&Test_VM_ProcessCmdPipe_GetPSPTimeHook);

    /* Execute the function being tested */
    /* Emulate AppMain() */
    oVMut.InitApp();
    oVMut.ArmingSM.FSM.InitComplete();
    oVMut.NavigationSM.FSM.trInitComplete();
    oVMut.Initialization();
    oVMut.SetHomePosition();

    /* Emulate VM_WAKEUP_MID handler */
    oVMut.VehicleGlobalPositionMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.BatteryStatusMsg.Warning = PX4_BATTERY_WARNING_NONE;
    oVMut.SafetyMsg.SafetySwitchAvailable = FALSE;

    oVMut.Execute();

    timestamp = VM_Test_GetTimeUs();
    oVMut.ActuatorArmedMsg.Timestamp = timestamp;
    oVMut.VehicleStatusMsg.Timestamp = timestamp;
    oVMut.VehicleManagerStateMsg.Timestamp = timestamp;
    oVMut.VehicleControlModeMsg.Timestamp = timestamp;

    oVMut.ArmingSM.DoAction();
    oVMut.NavigationSM.DoAction();

    /* Process VM_SEND_HK_MID */
    oVMut.RcvSchPipeMsg(VM_SCH_PIPE_PEND_TIME);

    sprintf(expectedEvent, "%s", "Arming::Armed");

    /* Verify results */
    UtAssert_True(oVMut.HkTlm.usCmdCnt == 1, "ProcessCmdPipe, VehicleArm");
    UtAssert_EventSent(VM_ARMING_ENTERED_ARMED_STATE_INFO_EID, CFE_EVS_INFORMATION,
             expectedEvent, "ProcessCmdPipe, VehicleArm EventSent");
}


/**
 * Test VM ProcessCmdPipe, VehicleDisarm
 */
void Test_VM_ProcessCmdPipe_VehicleDisarm(void)
{
    VM             oVMut;

    int32          CmdPipe;
    uint64         timestamp;
    VM_NoArgCmd_t  CmdMsgArm;
    VM_NoArgCmd_t  CmdMsgDisarm;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsgArm, VM_CMD_MID, sizeof(CmdMsgArm), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsgArm, (uint16)VM_VEHICLE_ARM_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsgArm, (CFE_SB_PipeId_t)CmdPipe);
    VM_Test_PrintCmdMsg((void*)&CmdMsgArm, sizeof(CmdMsgArm));

    CFE_SB_InitMsg ((void*)&CmdMsgDisarm, VM_CMD_MID, sizeof(CmdMsgDisarm), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsgDisarm, (uint16)VM_VEHICLE_DISARM_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsgDisarm, (CFE_SB_PipeId_t)CmdPipe);
    VM_Test_PrintCmdMsg((void*)&CmdMsgDisarm, sizeof(CmdMsgDisarm));

    Ut_CFE_PSP_TIMER_SetFunctionHook(UT_CFE_PSP_TIMER_GETTIME_INDEX,
                        (void *)&Test_VM_ProcessCmdPipe_GetPSPTimeHook);

    /* Emulate AppMain() */
    oVMut.InitApp();
    oVMut.ArmingSM.FSM.InitComplete();
    oVMut.NavigationSM.FSM.trInitComplete();
    oVMut.Initialization();
    oVMut.SetHomePosition();

    /* Emulate VM_WAKEUP_MID handler */
    oVMut.VehicleGlobalPositionMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.BatteryStatusMsg.Warning = PX4_BATTERY_WARNING_NONE;
    oVMut.SafetyMsg.SafetySwitchAvailable = FALSE;

    oVMut.Execute();

    timestamp = VM_Test_GetTimeUs();
    oVMut.ActuatorArmedMsg.Timestamp = timestamp;
    oVMut.VehicleStatusMsg.Timestamp = timestamp;
    oVMut.VehicleManagerStateMsg.Timestamp = timestamp;
    oVMut.VehicleControlModeMsg.Timestamp = timestamp;

    oVMut.ArmingSM.DoAction();
    oVMut.NavigationSM.DoAction();

    /* Process Command Pipe */
    oVMut.ProcessCmdPipe();

    sprintf(expectedEvent, "%s", "Arming::Standby");

    /* Verify results */
    UtAssert_True(oVMut.HkTlm.usCmdCnt == 2, "ProcessCmdPipe, VehicleDisarm");
    UtAssert_EventSent(VM_ARMING_ENTERED_STANDBY_STATE_INFO_EID, CFE_EVS_INFORMATION,
             expectedEvent, "ProcessCmdPipe, VehicleDisarm EventSent");
}


/**
 * Test VM ProcessCmdPipe, SetNavManual
 */
void Test_VM_ProcessCmdPipe_SetNavManual(void)
{
    VM             oVMut;

    int32          CmdPipe;
    uint64         timestamp;
    VM_NoArgCmd_t  CmdMsgArm;
    VM_NoArgCmd_t  CmdMsgAlt;
    VM_NoArgCmd_t  CmdMsgMan;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);

    CFE_SB_InitMsg ((void*)&CmdMsgArm, VM_CMD_MID, sizeof(CmdMsgArm), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsgArm, (uint16)VM_VEHICLE_ARM_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsgArm, (CFE_SB_PipeId_t)CmdPipe);
    VM_Test_PrintCmdMsg((void*)&CmdMsgArm, sizeof(CmdMsgArm));

    CFE_SB_InitMsg ((void*)&CmdMsgAlt, VM_CMD_MID, sizeof(CmdMsgAlt), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsgAlt, (uint16)VM_SET_NAV_ALTCTL_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsgAlt, (CFE_SB_PipeId_t)CmdPipe);
    VM_Test_PrintCmdMsg((void*)&CmdMsgAlt, sizeof(CmdMsgAlt));

    CFE_SB_InitMsg ((void*)&CmdMsgMan, VM_CMD_MID, sizeof(CmdMsgMan), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsgMan, (uint16)VM_SET_NAV_MANUAL_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsgMan, (CFE_SB_PipeId_t)CmdPipe);
    VM_Test_PrintCmdMsg((void*)&CmdMsgMan, sizeof(CmdMsgMan));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    /* Execute the function being tested */
    /* Emulate AppMain() */
    oVMut.InitApp();
    oVMut.ArmingSM.FSM.InitComplete();
    oVMut.NavigationSM.FSM.trInitComplete();
    oVMut.Initialization();
    oVMut.SetHomePosition();

    /* Emulate VM_WAKEUP_MID handler */
    oVMut.VehicleGlobalPositionMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.BatteryStatusMsg.Warning = PX4_BATTERY_WARNING_NONE;
    oVMut.SafetyMsg.SafetySwitchAvailable = FALSE;
    oVMut.VehicleLocalPositionMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.VehicleLocalPositionMsg.Z_Valid = TRUE;
    oVMut.VehicleLocalPositionMsg.V_Z_Valid = TRUE;

    oVMut.Execute();

    timestamp = VM_Test_GetTimeUs();
    oVMut.ActuatorArmedMsg.Timestamp = timestamp;
    oVMut.VehicleStatusMsg.Timestamp = timestamp;
    oVMut.VehicleManagerStateMsg.Timestamp = timestamp;
    oVMut.VehicleControlModeMsg.Timestamp = timestamp;

    oVMut.ArmingSM.DoAction();
    oVMut.NavigationSM.DoAction();

    /* Process VM_SEND_HK_MID */
    oVMut.RcvSchPipeMsg(VM_SCH_PIPE_PEND_TIME);

    sprintf(expectedEvent, "%s", "Navigation::Manual");

    /* Verify results */
    UtAssert_True(oVMut.HkTlm.usCmdCnt == 3, "ProcessCmdPipe, SetNavManual");
    UtAssert_EventSent(VM_NAVSM_ENTERED_MANUAL_INFO_EID, CFE_EVS_INFORMATION,
             expectedEvent, "ProcessCmdPipe, SetNavManual EventSent");
}


/**
 * Test VM ProcessCmdPipe, SetNavAltctl
 */
void Test_VM_ProcessCmdPipe_SetNavAltctl(void)
{
    VM             oVMut;

    int32          CmdPipe;
    uint64         timestamp;
    VM_NoArgCmd_t  CmdMsgArm;
    VM_NoArgCmd_t  CmdMsgAlt;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);

    CFE_SB_InitMsg ((void*)&CmdMsgArm, VM_CMD_MID, sizeof(CmdMsgArm), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsgArm, (uint16)VM_VEHICLE_ARM_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsgArm, (CFE_SB_PipeId_t)CmdPipe);
    VM_Test_PrintCmdMsg((void*)&CmdMsgArm, sizeof(CmdMsgArm));

    CFE_SB_InitMsg ((void*)&CmdMsgAlt, VM_CMD_MID, sizeof(CmdMsgAlt), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsgAlt, (uint16)VM_SET_NAV_ALTCTL_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsgAlt, (CFE_SB_PipeId_t)CmdPipe);
    VM_Test_PrintCmdMsg((void*)&CmdMsgAlt, sizeof(CmdMsgAlt));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    /* Execute the function being tested */
    /* Emulate AppMain() */
    oVMut.InitApp();
    oVMut.ArmingSM.FSM.InitComplete();
    oVMut.NavigationSM.FSM.trInitComplete();
    oVMut.Initialization();
    oVMut.SetHomePosition();

    /* Emulate VM_WAKEUP_MID handler */
    oVMut.VehicleGlobalPositionMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.BatteryStatusMsg.Warning = PX4_BATTERY_WARNING_NONE;
    oVMut.SafetyMsg.SafetySwitchAvailable = FALSE;
    oVMut.VehicleLocalPositionMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.VehicleLocalPositionMsg.Z_Valid = TRUE;
    oVMut.VehicleLocalPositionMsg.V_Z_Valid = TRUE;

    oVMut.Execute();

    timestamp = VM_Test_GetTimeUs();
    oVMut.ActuatorArmedMsg.Timestamp = timestamp;
    oVMut.VehicleStatusMsg.Timestamp = timestamp;
    oVMut.VehicleManagerStateMsg.Timestamp = timestamp;
    oVMut.VehicleControlModeMsg.Timestamp = timestamp;

    oVMut.ArmingSM.DoAction();
    oVMut.NavigationSM.DoAction();

    /* Process VM_SEND_HK_MID */
    oVMut.RcvSchPipeMsg(VM_SCH_PIPE_PEND_TIME);

    sprintf(expectedEvent, "%s", "Navigation::AltitudeControl");

    /* Verify results */
    UtAssert_True(oVMut.HkTlm.usCmdCnt == 2, "ProcessCmdPipe, SetNavAltctl");
    UtAssert_EventSent(VM_NAVSN_ENTERED_ALTCTL_INFO_EID, CFE_EVS_INFORMATION,
             expectedEvent, "ProcessCmdPipe, SetNavAltctl EventSent");
}


/**
 * Test VM ProcessCmdPipe, SetNavPosctl
 */
void Test_VM_ProcessCmdPipe_SetNavPosctl(void)
{
    VM             oVMut;

    int32          CmdPipe;
    uint64         timestamp;
    VM_NoArgCmd_t  CmdMsgArm;
    VM_NoArgCmd_t  CmdMsgPos;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);

    CFE_SB_InitMsg ((void*)&CmdMsgArm, VM_CMD_MID, sizeof(CmdMsgArm), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsgArm, (uint16)VM_VEHICLE_ARM_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsgArm, (CFE_SB_PipeId_t)CmdPipe);
    VM_Test_PrintCmdMsg((void*)&CmdMsgArm, sizeof(CmdMsgArm));

    CFE_SB_InitMsg ((void*)&CmdMsgPos, VM_CMD_MID, sizeof(CmdMsgPos), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsgPos, (uint16)VM_SET_NAV_POSCTL_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsgPos, (CFE_SB_PipeId_t)CmdPipe);
    VM_Test_PrintCmdMsg((void*)&CmdMsgPos, sizeof(CmdMsgPos));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    /* Execute the function being tested */
    /* Emulate AppMain() */
    oVMut.InitApp();
    oVMut.ArmingSM.FSM.InitComplete();
    oVMut.NavigationSM.FSM.trInitComplete();
    oVMut.Initialization();
    oVMut.SetHomePosition();

    /* Emulate VM_WAKEUP_MID handler */
    oVMut.VehicleGlobalPositionMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.BatteryStatusMsg.Warning = PX4_BATTERY_WARNING_NONE;
    oVMut.SafetyMsg.SafetySwitchAvailable = FALSE;
    oVMut.VehicleLocalPositionMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.VehicleLocalPositionMsg.XY_Valid = TRUE;
    oVMut.VehicleLocalPositionMsg.V_XY_Valid = TRUE;
    oVMut.VehicleLocalPositionMsg.Z_Valid = TRUE;
    oVMut.VehicleLocalPositionMsg.V_Z_Valid = TRUE;

    oVMut.Execute();

    timestamp = VM_Test_GetTimeUs();
    oVMut.ActuatorArmedMsg.Timestamp = timestamp;
    oVMut.VehicleStatusMsg.Timestamp = timestamp;
    oVMut.VehicleManagerStateMsg.Timestamp = timestamp;
    oVMut.VehicleControlModeMsg.Timestamp = timestamp;

    oVMut.ArmingSM.DoAction();
    oVMut.NavigationSM.DoAction();

    /* Process VM_SEND_HK_MID */
    oVMut.RcvSchPipeMsg(VM_SCH_PIPE_PEND_TIME);

    sprintf(expectedEvent, "%s", "Navigation::PositionControl");

    /* Verify results */
    UtAssert_True(oVMut.HkTlm.usCmdCnt == 2, "ProcessCmdPipe, SetNavPosctl");
    UtAssert_EventSent(VM_NAVSN_ENTERED_POSCTL_INFO_EID, CFE_EVS_INFORMATION,
             expectedEvent, "ProcessCmdPipe, SetNavPosctl EventSent");
}


/**
 * Test VM ProcessCmdPipe, SetNavAutoLoiter
 */
void Test_VM_ProcessCmdPipe_SetNavAutoLoiter(void)
{
    VM             oVMut;

    int32          CmdPipe;
    uint64         timestamp;
    VM_NoArgCmd_t  CmdMsgArm;
    VM_NoArgCmd_t  CmdMsgLoiter;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);

    CFE_SB_InitMsg ((void*)&CmdMsgArm, VM_CMD_MID, sizeof(CmdMsgArm), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsgArm, (uint16)VM_VEHICLE_ARM_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsgArm, (CFE_SB_PipeId_t)CmdPipe);
    VM_Test_PrintCmdMsg((void*)&CmdMsgArm, sizeof(CmdMsgArm));

    CFE_SB_InitMsg ((void*)&CmdMsgLoiter, VM_CMD_MID, sizeof(CmdMsgLoiter), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsgLoiter, (uint16)VM_SET_NAV_AUTO_LOITER_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsgLoiter, (CFE_SB_PipeId_t)CmdPipe);
    VM_Test_PrintCmdMsg((void*)&CmdMsgLoiter, sizeof(CmdMsgLoiter));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    /* Execute the function being tested */
    /* Emulate AppMain() */
    oVMut.InitApp();
    oVMut.ArmingSM.FSM.InitComplete();
    oVMut.NavigationSM.FSM.trInitComplete();
    oVMut.Initialization();
    oVMut.SetHomePosition();

    /* Emulate VM_WAKEUP_MID handler */
    oVMut.VehicleGlobalPositionMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.BatteryStatusMsg.Warning = PX4_BATTERY_WARNING_NONE;
    oVMut.SafetyMsg.SafetySwitchAvailable = FALSE;
    oVMut.SensorCombinedMsg.MagTimestamp = VM_Test_GetTimeUs();
    oVMut.SensorCombinedMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.SensorCombinedMsg.AccTimestamp = VM_Test_GetTimeUs();
    oVMut.VehicleAttitudeMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.VehicleLocalPositionMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.VehicleLandDetectedMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.VehicleGpsPositionMsg.Timestamp = VM_Test_GetTimeUs();

    oVMut.Execute();

    timestamp = VM_Test_GetTimeUs();
    oVMut.ActuatorArmedMsg.Timestamp = timestamp;
    oVMut.VehicleStatusMsg.Timestamp = timestamp;
    oVMut.VehicleManagerStateMsg.Timestamp = timestamp;
    oVMut.VehicleControlModeMsg.Timestamp = timestamp;

    oVMut.ArmingSM.DoAction();
    oVMut.NavigationSM.DoAction();

    /* Process VM_SEND_HK_MID */
    oVMut.RcvSchPipeMsg(VM_SCH_PIPE_PEND_TIME);

    sprintf(expectedEvent, "%s", "Navigation::AutoLoiter");

    /* Verify results */
    UtAssert_True(oVMut.HkTlm.usCmdCnt == 2, "ProcessCmdPipe, SetNavAutoLoiter");
    UtAssert_EventSent(VM_NAVSN_ENTERED_AUTOLOITER_INFO_EID, CFE_EVS_INFORMATION,
             expectedEvent, "ProcessCmdPipe, SetNavAutoLoiter EventSent");
}


/**
 * Test VM ProcessCmdPipe, SetNavAutoRtl
 */
void Test_VM_ProcessCmdPipe_SetNavAutoRtl(void)
{
    VM             oVMut;

    int32          CmdPipe;
    uint64         timestamp;
    VM_NoArgCmd_t  CmdMsgArm;
    VM_NoArgCmd_t  CmdMsgRtl;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);

    CFE_SB_InitMsg ((void*)&CmdMsgArm, VM_CMD_MID, sizeof(CmdMsgArm), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsgArm, (uint16)VM_VEHICLE_ARM_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsgArm, (CFE_SB_PipeId_t)CmdPipe);
    VM_Test_PrintCmdMsg((void*)&CmdMsgArm, sizeof(CmdMsgArm));

    CFE_SB_InitMsg ((void*)&CmdMsgRtl, VM_CMD_MID, sizeof(CmdMsgRtl), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsgRtl, (uint16)VM_SET_NAV_AUTO_RTL_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsgRtl, (CFE_SB_PipeId_t)CmdPipe);
    VM_Test_PrintCmdMsg((void*)&CmdMsgRtl, sizeof(CmdMsgRtl));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    /* Execute the function being tested */
    /* Emulate AppMain() */
    oVMut.InitApp();
    oVMut.ArmingSM.FSM.InitComplete();
    oVMut.NavigationSM.FSM.trInitComplete();
    oVMut.Initialization();
    oVMut.SetHomePosition();

    /* Emulate VM_WAKEUP_MID handler */
    oVMut.VehicleGlobalPositionMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.BatteryStatusMsg.Warning = PX4_BATTERY_WARNING_NONE;
    oVMut.SafetyMsg.SafetySwitchAvailable = FALSE;
    oVMut.SensorCombinedMsg.MagTimestamp = VM_Test_GetTimeUs();
    oVMut.SensorCombinedMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.SensorCombinedMsg.AccTimestamp = VM_Test_GetTimeUs();
    oVMut.VehicleAttitudeMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.VehicleLocalPositionMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.VehicleLandDetectedMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.VehicleGpsPositionMsg.Timestamp = VM_Test_GetTimeUs();

    oVMut.Execute();

    timestamp = VM_Test_GetTimeUs();
    oVMut.ActuatorArmedMsg.Timestamp = timestamp;
    oVMut.VehicleStatusMsg.Timestamp = timestamp;
    oVMut.VehicleManagerStateMsg.Timestamp = timestamp;
    oVMut.VehicleControlModeMsg.Timestamp = timestamp;

    oVMut.ArmingSM.DoAction();
    oVMut.NavigationSM.DoAction();

    /* Process VM_SEND_HK_MID */
    oVMut.RcvSchPipeMsg(VM_SCH_PIPE_PEND_TIME);

    sprintf(expectedEvent, "%s", "Navigation::AutoRTL");

    /* Verify results */
    UtAssert_True(oVMut.HkTlm.usCmdCnt == 2, "ProcessCmdPipe, SetNavAutoRtl");
    UtAssert_EventSent(VM_NAVSN_ENTERED_RTL_INFO_EID, CFE_EVS_INFORMATION,
             expectedEvent, "ProcessCmdPipe, SetNavAutoRtl EventSent");
}


/**
 * Test VM ProcessCmdPipe, SetNavAcro
 */
void Test_VM_ProcessCmdPipe_SetNavAcro(void)
{
    VM             oVMut;

    int32          CmdPipe;
    uint64         timestamp;
    VM_NoArgCmd_t  CmdMsgArm;
    VM_NoArgCmd_t  CmdMsgAcro;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);

    CFE_SB_InitMsg ((void*)&CmdMsgArm, VM_CMD_MID, sizeof(CmdMsgArm), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsgArm, (uint16)VM_VEHICLE_ARM_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsgArm, (CFE_SB_PipeId_t)CmdPipe);
    VM_Test_PrintCmdMsg((void*)&CmdMsgArm, sizeof(CmdMsgArm));

    CFE_SB_InitMsg ((void*)&CmdMsgAcro, VM_CMD_MID, sizeof(CmdMsgAcro), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsgAcro, (uint16)VM_SET_NAV_ACRO_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsgAcro, (CFE_SB_PipeId_t)CmdPipe);
    VM_Test_PrintCmdMsg((void*)&CmdMsgAcro, sizeof(CmdMsgAcro));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    /* Execute the function being tested */
    /* Emulate AppMain() */
    oVMut.InitApp();
    oVMut.ArmingSM.FSM.InitComplete();
    oVMut.NavigationSM.FSM.trInitComplete();
    oVMut.Initialization();
    oVMut.SetHomePosition();

    /* Emulate VM_WAKEUP_MID handler */
    oVMut.VehicleGlobalPositionMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.BatteryStatusMsg.Warning = PX4_BATTERY_WARNING_NONE;
    oVMut.SafetyMsg.SafetySwitchAvailable = FALSE;
    oVMut.SensorCombinedMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.SensorCombinedMsg.MagInvalid = FALSE;
    oVMut.SensorCombinedMsg.AccInvalid = FALSE;

    oVMut.Execute();

    timestamp = VM_Test_GetTimeUs();
    oVMut.ActuatorArmedMsg.Timestamp = timestamp;
    oVMut.VehicleStatusMsg.Timestamp = timestamp;
    oVMut.VehicleManagerStateMsg.Timestamp = timestamp;
    oVMut.VehicleControlModeMsg.Timestamp = timestamp;

    oVMut.ArmingSM.DoAction();
    oVMut.NavigationSM.DoAction();

    /* Process VM_SEND_HK_MID */
    oVMut.RcvSchPipeMsg(VM_SCH_PIPE_PEND_TIME);

    sprintf(expectedEvent, "%s", "Navigation::Acrobatic");

    /* Verify results */
    UtAssert_True(oVMut.HkTlm.usCmdCnt == 2, "ProcessCmdPipe, SetNavAcro");
    UtAssert_EventSent(VM_NAVSN_ENTERED_ACRO_INFO_EID, CFE_EVS_INFORMATION,
             expectedEvent, "ProcessCmdPipe, SetNavAcro EventSent");
}


/**
 * Test VM ProcessCmdPipe, SetNavStabilize
 */
void Test_VM_ProcessCmdPipe_SetNavStabilize(void)
{
    VM             oVMut;

    int32          CmdPipe;
    uint64         timestamp;
    VM_NoArgCmd_t  CmdMsgArm;
    VM_NoArgCmd_t  CmdMsgStab;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);

    CFE_SB_InitMsg ((void*)&CmdMsgArm, VM_CMD_MID, sizeof(CmdMsgArm), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsgArm, (uint16)VM_VEHICLE_ARM_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsgArm, (CFE_SB_PipeId_t)CmdPipe);
    VM_Test_PrintCmdMsg((void*)&CmdMsgArm, sizeof(CmdMsgArm));

    CFE_SB_InitMsg ((void*)&CmdMsgStab, VM_CMD_MID, sizeof(CmdMsgStab), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsgStab, (uint16)VM_SET_NAV_STABILIZE_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsgStab, (CFE_SB_PipeId_t)CmdPipe);
    VM_Test_PrintCmdMsg((void*)&CmdMsgStab, sizeof(CmdMsgStab));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    /* Execute the function being tested */
    /* Emulate AppMain() */
    oVMut.InitApp();
    oVMut.ArmingSM.FSM.InitComplete();
    oVMut.NavigationSM.FSM.trInitComplete();
    oVMut.Initialization();
    oVMut.SetHomePosition();

    /* Emulate VM_WAKEUP_MID handler */
    oVMut.VehicleGlobalPositionMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.BatteryStatusMsg.Warning = PX4_BATTERY_WARNING_NONE;
    oVMut.SafetyMsg.SafetySwitchAvailable = FALSE;
    oVMut.SensorCombinedMsg.MagTimestamp = VM_Test_GetTimeUs();
    oVMut.SensorCombinedMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.SensorCombinedMsg.AccTimestamp = VM_Test_GetTimeUs();
    oVMut.VehicleAttitudeMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.VehicleLocalPositionMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.VehicleLandDetectedMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.VehicleGpsPositionMsg.Timestamp = VM_Test_GetTimeUs();

    oVMut.Execute();

    timestamp = VM_Test_GetTimeUs();
    oVMut.ActuatorArmedMsg.Timestamp = timestamp;
    oVMut.VehicleStatusMsg.Timestamp = timestamp;
    oVMut.VehicleManagerStateMsg.Timestamp = timestamp;
    oVMut.VehicleControlModeMsg.Timestamp = timestamp;

    oVMut.ArmingSM.DoAction();
    oVMut.NavigationSM.DoAction();

    /* Process VM_SEND_HK_MID */
    oVMut.RcvSchPipeMsg(VM_SCH_PIPE_PEND_TIME);

    sprintf(expectedEvent, "%s", "Navigation::Stabilize");

    /* Verify results */
    UtAssert_True(oVMut.HkTlm.usCmdCnt == 2, "ProcessCmdPipe, SetNavStabilize");
    UtAssert_EventSent(VM_NAVSN_ENTERED_STABILIZE_INFO_EID, CFE_EVS_INFORMATION,
             expectedEvent, "ProcessCmdPipe, SetNavStabilize EventSent");
}


/**
 * Test VM ProcessCmdPipe, SetNavRattitude
 */
void Test_VM_ProcessCmdPipe_SetNavRattitude(void)
{
    VM             oVMut;

    int32          CmdPipe;
    uint64         timestamp;
    VM_NoArgCmd_t  CmdMsgArm;
    VM_NoArgCmd_t  CmdMsgRatt;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);

    CFE_SB_InitMsg ((void*)&CmdMsgArm, VM_CMD_MID, sizeof(CmdMsgArm), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsgArm, (uint16)VM_VEHICLE_ARM_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsgArm, (CFE_SB_PipeId_t)CmdPipe);
    VM_Test_PrintCmdMsg((void*)&CmdMsgArm, sizeof(CmdMsgArm));

    CFE_SB_InitMsg ((void*)&CmdMsgRatt, VM_CMD_MID, sizeof(CmdMsgRatt), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsgRatt, (uint16)VM_SET_NAV_RATTITUDE_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsgRatt, (CFE_SB_PipeId_t)CmdPipe);
    VM_Test_PrintCmdMsg((void*)&CmdMsgRatt, sizeof(CmdMsgRatt));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    /* Execute the function being tested */
    /* Emulate AppMain() */
    oVMut.InitApp();
    oVMut.ArmingSM.FSM.InitComplete();
    oVMut.NavigationSM.FSM.trInitComplete();
    oVMut.Initialization();
    oVMut.SetHomePosition();

    /* Emulate VM_WAKEUP_MID handler */
    oVMut.VehicleGlobalPositionMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.BatteryStatusMsg.Warning = PX4_BATTERY_WARNING_NONE;
    oVMut.SafetyMsg.SafetySwitchAvailable = FALSE;
    oVMut.SensorCombinedMsg.MagTimestamp = VM_Test_GetTimeUs();
    oVMut.SensorCombinedMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.SensorCombinedMsg.AccTimestamp = VM_Test_GetTimeUs();
    oVMut.VehicleAttitudeMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.VehicleLocalPositionMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.VehicleLandDetectedMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.VehicleGpsPositionMsg.Timestamp = VM_Test_GetTimeUs();

    oVMut.Execute();

    timestamp = VM_Test_GetTimeUs();
    oVMut.ActuatorArmedMsg.Timestamp = timestamp;
    oVMut.VehicleStatusMsg.Timestamp = timestamp;
    oVMut.VehicleManagerStateMsg.Timestamp = timestamp;
    oVMut.VehicleControlModeMsg.Timestamp = timestamp;

    oVMut.ArmingSM.DoAction();
    oVMut.NavigationSM.DoAction();

    /* Process VM_SEND_HK_MID */
    oVMut.RcvSchPipeMsg(VM_SCH_PIPE_PEND_TIME);

    sprintf(expectedEvent, "%s", "Navigation::Rattitude");

    /* Verify results */
    UtAssert_True(oVMut.HkTlm.usCmdCnt == 2, "ProcessCmdPipe, SetNavRattitude");
    UtAssert_EventSent(VM_NAVSN_ENTERED_RATTITUDE_INFO_EID, CFE_EVS_INFORMATION,
             expectedEvent, "ProcessCmdPipe, SetNavRattitude EventSent");
}


/**
 * Test VM ProcessCmdPipe, SetNavAutoTakeoff
 */
void Test_VM_ProcessCmdPipe_SetNavAutoTakeoff(void)
{
    VM             oVMut;

    int32          CmdPipe;
    uint64         timestamp;
    VM_NoArgCmd_t  CmdMsgArm;
    VM_NoArgCmd_t  CmdMsgTakeoff;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);

    CFE_SB_InitMsg ((void*)&CmdMsgArm, VM_CMD_MID, sizeof(CmdMsgArm), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsgArm, (uint16)VM_VEHICLE_ARM_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsgArm, (CFE_SB_PipeId_t)CmdPipe);
    VM_Test_PrintCmdMsg((void*)&CmdMsgArm, sizeof(CmdMsgArm));

    CFE_SB_InitMsg ((void*)&CmdMsgTakeoff, VM_CMD_MID, sizeof(CmdMsgTakeoff), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsgTakeoff, (uint16)VM_SET_NAV_AUTO_TAKEOFF_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsgTakeoff, (CFE_SB_PipeId_t)CmdPipe);
    VM_Test_PrintCmdMsg((void*)&CmdMsgTakeoff, sizeof(CmdMsgTakeoff));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    /* Execute the function being tested */
    /* Emulate AppMain() */
    oVMut.InitApp();
    oVMut.ArmingSM.FSM.InitComplete();
    oVMut.NavigationSM.FSM.trInitComplete();
    oVMut.Initialization();
    oVMut.SetHomePosition();

    /* Emulate VM_WAKEUP_MID handler */
    oVMut.VehicleGlobalPositionMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.BatteryStatusMsg.Warning = PX4_BATTERY_WARNING_NONE;
    oVMut.SafetyMsg.SafetySwitchAvailable = FALSE;
    oVMut.SensorCombinedMsg.MagTimestamp = VM_Test_GetTimeUs();
    oVMut.SensorCombinedMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.SensorCombinedMsg.AccTimestamp = VM_Test_GetTimeUs();
    oVMut.VehicleAttitudeMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.VehicleLocalPositionMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.VehicleLandDetectedMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.VehicleGpsPositionMsg.Timestamp = VM_Test_GetTimeUs();

    oVMut.Execute();

    timestamp = VM_Test_GetTimeUs();
    oVMut.ActuatorArmedMsg.Timestamp = timestamp;
    oVMut.VehicleStatusMsg.Timestamp = timestamp;
    oVMut.VehicleManagerStateMsg.Timestamp = timestamp;
    oVMut.VehicleControlModeMsg.Timestamp = timestamp;

    oVMut.ArmingSM.DoAction();
    oVMut.NavigationSM.DoAction();

    /* Process VM_SEND_HK_MID */
    oVMut.RcvSchPipeMsg(VM_SCH_PIPE_PEND_TIME);

    sprintf(expectedEvent, "%s", "Navigation::AutoTakeoff");

    /* Verify results */
    UtAssert_True(oVMut.HkTlm.usCmdCnt == 2, "ProcessCmdPipe, SetNavAutoTakeoff");
    UtAssert_EventSent(VM_NAVSN_ENTERED_AUTO_TAKEOFF_INFO_EID, CFE_EVS_INFORMATION,
             expectedEvent, "ProcessCmdPipe, SetNavAutoTakeoff EventSent");
}


/**
 * Test VM ProcessCmdPipe, SetNavAutoLand
 */
void Test_VM_ProcessCmdPipe_SetNavAutoLand(void)
{
    VM             oVMut;

    int32          CmdPipe;
    uint64         timestamp;
    VM_NoArgCmd_t  CmdMsgArm;
    VM_NoArgCmd_t  CmdMsgLand;
    char           expectedEvent[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);

    CFE_SB_InitMsg ((void*)&CmdMsgArm, VM_CMD_MID, sizeof(CmdMsgArm), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsgArm, (uint16)VM_VEHICLE_ARM_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsgArm, (CFE_SB_PipeId_t)CmdPipe);
    VM_Test_PrintCmdMsg((void*)&CmdMsgArm, sizeof(CmdMsgArm));

    CFE_SB_InitMsg ((void*)&CmdMsgLand, VM_CMD_MID, sizeof(CmdMsgLand), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsgLand, (uint16)VM_SET_NAV_AUTO_LAND_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsgLand, (CFE_SB_PipeId_t)CmdPipe);
    VM_Test_PrintCmdMsg((void*)&CmdMsgLand, sizeof(CmdMsgLand));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    /* Execute the function being tested */
    /* Emulate AppMain() */
    oVMut.InitApp();
    oVMut.ArmingSM.FSM.InitComplete();
    oVMut.NavigationSM.FSM.trInitComplete();
    oVMut.Initialization();
    oVMut.SetHomePosition();

    /* Emulate VM_WAKEUP_MID handler */
    oVMut.VehicleGlobalPositionMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.BatteryStatusMsg.Warning = PX4_BATTERY_WARNING_NONE;
    oVMut.SafetyMsg.SafetySwitchAvailable = FALSE;
    oVMut.SensorCombinedMsg.MagTimestamp = VM_Test_GetTimeUs();
    oVMut.SensorCombinedMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.SensorCombinedMsg.AccTimestamp = VM_Test_GetTimeUs();
    oVMut.VehicleAttitudeMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.VehicleLocalPositionMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.VehicleLandDetectedMsg.Timestamp = VM_Test_GetTimeUs();
    oVMut.VehicleGpsPositionMsg.Timestamp = VM_Test_GetTimeUs();

    oVMut.Execute();

    timestamp = VM_Test_GetTimeUs();
    oVMut.ActuatorArmedMsg.Timestamp = timestamp;
    oVMut.VehicleStatusMsg.Timestamp = timestamp;
    oVMut.VehicleManagerStateMsg.Timestamp = timestamp;
    oVMut.VehicleControlModeMsg.Timestamp = timestamp;

    oVMut.ArmingSM.DoAction();
    oVMut.NavigationSM.DoAction();

    /* Process VM_SEND_HK_MID */
    oVMut.RcvSchPipeMsg(VM_SCH_PIPE_PEND_TIME);

    sprintf(expectedEvent, "%s", "Navigation::AutoLand");

    /* Verify results */
    UtAssert_True(oVMut.HkTlm.usCmdCnt == 2, "ProcessCmdPipe, SetNavAutoLand");
    UtAssert_EventSent(VM_NAVSN_ENTERED_AUTO_LAND_INFO_EID, CFE_EVS_INFORMATION,
             expectedEvent, "ProcessCmdPipe, SetNavAutoLand EventSent");
}


/**
 * Test VM ProcessCmdPipe, SendConfigurationHook
 */
int32 Test_VM_ProcessCmdPipe_SendConfigurationHook(CFE_SB_Msg_t *MsgPtr)
{
    unsigned char       *pBuff = NULL;
    uint16              msgLen = 0;
    int                 i = 0;
    CFE_SB_MsgId_t      MsgId = 0;
    time_t              localTime;
    struct tm           *loc_time;
    CFE_TIME_SysTime_t  TimeFromMsg;
    VM_ConfigTlm_t      ConfigTlm;

    msgLen = CFE_SB_GetTotalMsgLength(MsgPtr);
    MsgId = CFE_SB_GetMsgId(MsgPtr);

    pBuff = (unsigned char *)MsgPtr;
    printf("###SendConfigurationHook(msgLen %u)", msgLen);
    for (i = 0; i < msgLen; i++)
    {
        printf("0x%02x ", *pBuff);
        pBuff++;
    }
    printf("\n");

    TimeFromMsg = CFE_SB_GetMsgTime(MsgPtr);
    localTime = VM_Test_GetTimeFromMsg(TimeFromMsg);
    loc_time = localtime(&localTime);
    printf("TimeFromMessage: %s", asctime(loc_time));

    switch(MsgId)
    {
        case VM_CONFIG_TLM_MID:     // check this
        {
            SendConfHook_MsgId = VM_CONFIG_TLM_MID;
            printf("Sent VM_CONFIG_TLM_MID\n");
            memcpy((void *)&ConfigTlm, (void *)MsgPtr, sizeof(ConfigTlm));
            // Display
            break;
        }
        default:
        {
            SendConfHook_MsgId = MsgId;
            printf("Sent MID: %u\n", MsgId);
            break;
        }
    }
}


/**
 * Test VM ProcessCmdPipe, SendConfiguration
 */
void Test_VM_ProcessCmdPipe_SendConfiguration(void)
{
    int32          CmdPipe;
    VM_NoArgCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe(VM_CMD_PIPE_NAME);
    CFE_SB_InitMsg ((void*)&CmdMsg, VM_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)VM_SEND_CONFIGURATION_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
    VM_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, VM_SEND_HK_MID, 1);

    SendConfHook_MsgId = 0;
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX,
                          (void*)&Test_VM_ProcessCmdPipe_SendConfigurationHook);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oVM.AppMain();

    /* Verify results */
    UtAssert_True((oVM.HkTlm.usCmdCnt == 1) &&
                  (SendConfHook_MsgId == VM_CONFIG_TLM_MID),
                  "ProcessCmdPipe, SendConfiguration");
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
#if 0
    UtTest_Add(Test_VM_ProcessCmdPipe_SendConfiguration,
               VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_ProcessCmdPipe_SendConfiguration");
#endif

    UtTest_Add(Test_VM_VerifyCmdLength_Fail_CmdLength,
               VM_Test_Setup, VM_Test_TearDown,
               "Test_VM_VerifyCmdLength_Fail_CmdLength");
} /* end VM_Cmds_Test_AddTestCases */
