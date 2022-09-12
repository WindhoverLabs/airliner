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

#include "fpc_cmds_test.hpp"
#include "fpc_test_utils.hpp"

#include "fpc_msg.h"

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
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_cfe_time_stubs.h"


uint32    ProcessNewCmds_Result = 0xffffffff;

/**************************************************************************
 * Tests for FPC ProcessNewCmds()
 **************************************************************************/
/**
 * Test FPC ProcessNewCmds, InvalidCmd
 */
void Test_FPC_ProcessNewCmds_InvalidCmd(void)
{
    FPC  oFPC{};

    int32           CmdPipe;
    FPC_NoArgCmd_t  InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, PX4_AIRSPEED_MID, sizeof(FPC_NoArgCmd_t), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    FPC_Test_PrintCmdMsg((void*)&InMsg, sizeof(FPC_NoArgCmd_t));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FPC_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFPC.AppMain();

    /* Verify results */
    if ((Ut_CFE_EVS_GetEventQueueDepth() == 2) && (oFPC.HkTlm.usCmdErrCnt == 1))
    {
        UtAssert_True(TRUE, "ProcessNewCmds, InvalidCmd");
    }
    else
    {
        UtAssert_True(FALSE, "ProcessNewCmds, InvalidCmd");
    }
}

/**
 * Test FPC ProcessNewCmds, InvalidCmdCode
 */
void Test_FPC_ProcessNewCmds_InvalidCmdCode(void)
{
    FPC  oFPC{};

    int32              CmdPipe;
    FPC_NoArgCmd_t     InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, FPC_CMD_MID, sizeof(FPC_NoArgCmd_t), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)255);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    FPC_Test_PrintCmdMsg((void*)&InMsg, sizeof(FPC_NoArgCmd_t));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FPC_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFPC.AppMain();

    /* Verify results */
    if ((Ut_CFE_EVS_GetEventQueueDepth() == 2) && (oFPC.HkTlm.usCmdErrCnt == 1))
    {
        UtAssert_True(TRUE, "ProcessNewCmds, InvalidCmdCode");
    }
    else
    {
        UtAssert_True(FALSE, "ProcessNewCmds, InvalidCmdCode");
    }
}

/**
 * Test FPC ProcessNewCmds, CmdPipeError
 */
void Test_FPC_ProcessNewCmds_CmdPipeError(void)
{
    FPC  oFPC{};

    int32              SchPipe;
    FPC_NoArgCmd_t     InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    SchPipe = Ut_CFE_SB_CreatePipe("FPC_SCH_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, FPC_WAKEUP_MID, sizeof(FPC_NoArgCmd_t), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)0);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)SchPipe);

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_BAD_ARGUMENT, 2);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Execute the function being tested */
    oFPC.AppMain();
}


/**
 * Test FPC ProcessNewCmds, SendEventHook
 */
int32 Test_FPC_ProcessNewCmds_SendEventHook
               (uint16 EventID, uint16 EventType, const char *EventText, ...)
{
    va_list  Ptr;
    char     Buf[256];
    char     *pCmdStr1 = NULL;
    char     *pCmdStr2 = NULL;
    char     *pCmdStr3 = NULL;

    va_start(Ptr, EventText);
    vsnprintf(Buf, (size_t)CFE_EVS_MAX_MESSAGE_LENGTH, EventText, Ptr);
    va_end(Ptr);

    printf("###ProcessNewCmds_SendEventHook:\n");
    printf("%s\n", Buf);

    pCmdStr1 = strstr(Buf, "NOOP");
    pCmdStr2 = strstr(Buf, "RESET");
    pCmdStr3 = strstr(Buf, "FPC_DO_GO_AROUND");

    printf("pCmdStr1: %p, pCmdStr2: %p, pCmdStr3: %p\n", pCmdStr1, pCmdStr2, pCmdStr3);

    if (pCmdStr1 != NULL)
    {
        ProcessNewCmds_Result = FPC_NOOP_CC;
    }

    if (pCmdStr2 != NULL)
    {
        ProcessNewCmds_Result = FPC_RESET_CC;
    }

    if (pCmdStr3 != NULL)
    {
        ProcessNewCmds_Result = FPC_DO_GO_AROUND_CC;
    }

    return 0;
}


/**
 * Test FPC ProcessNewCmds, Noop
 */
void Test_FPC_ProcessNewCmds_Noop(void)
{
    FPC  oFPC{};

    int32              CmdPipe;
    FPC_NoArgCmd_t     InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, FPC_CMD_MID, sizeof(FPC_NoArgCmd_t), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)FPC_NOOP_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    FPC_Test_PrintCmdMsg((void*)&InMsg, sizeof(FPC_NoArgCmd_t));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FPC_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    ProcessNewCmds_Result = 0xffffffff;
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    oFPC.AppMain();

    /* Verify results */
    if ((oFPC.HkTlm.usCmdCnt == 1) && (ProcessNewCmds_Result == FPC_NOOP_CC))
    {
        UtAssert_True(TRUE, "ProcessNewCmds, Noop");
    }
    else
    {
        UtAssert_True(FALSE, "ProcessNewCmds, Noop");
    }
}

/**
 * Test FPC ProcessNewCmds, Reset
 */
void Test_FPC_ProcessNewCmds_Reset(void)
{
    FPC  oFPC{};

    int32              CmdPipe;
    FPC_NoArgCmd_t     InMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&InMsg, FPC_CMD_MID, sizeof(FPC_NoArgCmd_t), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&InMsg, (uint16)FPC_RESET_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&InMsg, (CFE_SB_PipeId_t)CmdPipe);

    FPC_Test_PrintCmdMsg((void*)&InMsg, sizeof(FPC_NoArgCmd_t));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FPC_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    ProcessNewCmds_Result = 0xffffffff;
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    oFPC.AppMain();

    /* Verify results */
    if ((oFPC.HkTlm.usCmdCnt == 0) && (oFPC.HkTlm.usCmdErrCnt == 0)
         && (ProcessNewCmds_Result == FPC_RESET_CC))
    {
        UtAssert_True(TRUE, "ProcessNewCmds, Reset");
    }
    else
    {
        UtAssert_True(FALSE, "ProcessNewCmds, Reset");
    }
}


/**
 * Test FPC ProcessNewCmds, DoGoAround
 */
void Test_FPC_ProcessNewCmds_DoGoAround(void)
{
#if 1
    FPC  oFPC{};
#endif

    int32              CmdPipe;
    FPC_NoArgCmd_t     CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */


    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_DO_GO_AROUND_CC);
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&CmdMsg);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

/*
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FPC_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 4);
*/

    ProcessNewCmds_Result = 0xffffffff;
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    oFPC.InitEvent();
    oFPC.InitPipe();
    oFPC.InitData();

    oFPC.m_VehicleControlModeMsg.Timestamp = FPC_Test_GetTimeUs();
    oFPC.m_VehicleControlModeMsg.ExternalManualOverrideOk = FALSE;
    oFPC.m_VehicleControlModeMsg.SystemHilEnabled = FALSE;
    oFPC.m_VehicleControlModeMsg.ControlManualEnabled = FALSE;
    oFPC.m_VehicleControlModeMsg.ControlAutoEnabled = TRUE;
    oFPC.m_VehicleControlModeMsg.ControlOffboardEnabled = FALSE;
    oFPC.m_VehicleControlModeMsg.ControlRatesEnabled = TRUE;
    oFPC.m_VehicleControlModeMsg.ControlAttitudeEnabled = TRUE;
    oFPC.m_VehicleControlModeMsg.ControlRattitudeEnabled = TRUE;
    oFPC.m_VehicleControlModeMsg.ControlForceEnabled = FALSE;
    oFPC.m_VehicleControlModeMsg.ControlAccelerationEnabled = FALSE;
    oFPC.m_VehicleControlModeMsg.ControlVelocityEnabled = FALSE;
    oFPC.m_VehicleControlModeMsg.ControlPositionEnabled = FALSE;
    oFPC.m_VehicleControlModeMsg.ControlAltitudeEnabled = FALSE;
    oFPC.m_VehicleControlModeMsg.ControlClimbRateEnabled = FALSE;
    oFPC.m_VehicleControlModeMsg.ControlTerminationEnabled = FALSE;
    oFPC.m_VehicleControlModeMsg.ControlFixedHdgEnabled = FALSE;

    oFPC.m_PositionSetpointTripletMsg.Timestamp = FPC_Test_GetTimeUs();
    oFPC.m_PositionSetpointTripletMsg.Previous.Timestamp = FPC_Test_GetTimeUs();
    oFPC.m_PositionSetpointTripletMsg.Previous.Lat = 47.397741928975;
    oFPC.m_PositionSetpointTripletMsg.Previous.Lon = 8.545593979817;
    oFPC.m_PositionSetpointTripletMsg.Previous.X = 0.0f;
    oFPC.m_PositionSetpointTripletMsg.Previous.Y = 0.0f;
    oFPC.m_PositionSetpointTripletMsg.Previous.Z = 0.0f;
    oFPC.m_PositionSetpointTripletMsg.Previous.VX = 0.0f;
    oFPC.m_PositionSetpointTripletMsg.Previous.VY = 0.0f;
    oFPC.m_PositionSetpointTripletMsg.Previous.VZ = 0.0f;
    oFPC.m_PositionSetpointTripletMsg.Previous.Alt = 490.7512f;
    oFPC.m_PositionSetpointTripletMsg.Previous.Yaw = 1.547718f;
    oFPC.m_PositionSetpointTripletMsg.Previous.Yawspeed = 0.0f;
    oFPC.m_PositionSetpointTripletMsg.Previous.LoiterRadius = 50.0f;
    oFPC.m_PositionSetpointTripletMsg.Previous.PitchMin = 0.0f;
    oFPC.m_PositionSetpointTripletMsg.Previous.AX = 0.0f;
    oFPC.m_PositionSetpointTripletMsg.Previous.AY = 0.0f;
    oFPC.m_PositionSetpointTripletMsg.Previous.AZ = 0.0f;
    oFPC.m_PositionSetpointTripletMsg.Previous.AcceptanceRadius = 0.0f;
    oFPC.m_PositionSetpointTripletMsg.Previous.CruisingSpeed = -1.0f;
    oFPC.m_PositionSetpointTripletMsg.Previous.CruisingThrottle = -1.0f;
    oFPC.m_PositionSetpointTripletMsg.Previous.Valid = TRUE;
    oFPC.m_PositionSetpointTripletMsg.Previous.Type = PX4_SETPOINT_TYPE_LOITER;
    oFPC.m_PositionSetpointTripletMsg.Previous.PositionValid = FALSE;
    oFPC.m_PositionSetpointTripletMsg.Previous.VelocityValid = FALSE;
    oFPC.m_PositionSetpointTripletMsg.Previous.VelocityFrame = 0;
    oFPC.m_PositionSetpointTripletMsg.Previous.AltValid = TRUE;
    oFPC.m_PositionSetpointTripletMsg.Previous.YawValid = TRUE;
    oFPC.m_PositionSetpointTripletMsg.Previous.DisableMcYawControl = FALSE;
    oFPC.m_PositionSetpointTripletMsg.Previous.YawspeedValid = FALSE;
    oFPC.m_PositionSetpointTripletMsg.Previous.LoiterDirection = 1;
    oFPC.m_PositionSetpointTripletMsg.Previous.AccelerationValid = FALSE;
    oFPC.m_PositionSetpointTripletMsg.Previous.AccelerationIsForce = FALSE;

    oFPC.m_PositionSetpointTripletMsg.Current.Timestamp = FPC_Test_GetTimeUs();
    oFPC.m_PositionSetpointTripletMsg.Current.Lat = 47.397741928975;
    oFPC.m_PositionSetpointTripletMsg.Current.Lon = 8.545593979817;
    oFPC.m_PositionSetpointTripletMsg.Current.X = 0.0f;
    oFPC.m_PositionSetpointTripletMsg.Current.Y = 0.0f;
    oFPC.m_PositionSetpointTripletMsg.Current.Z = 0.0f;
    oFPC.m_PositionSetpointTripletMsg.Current.VX = 0.0f;
    oFPC.m_PositionSetpointTripletMsg.Current.VY = 0.0f;
    oFPC.m_PositionSetpointTripletMsg.Current.VZ = 0.0f;
    oFPC.m_PositionSetpointTripletMsg.Current.Alt = 490.7512f;
    oFPC.m_PositionSetpointTripletMsg.Current.Yaw = 1.547718f;
    oFPC.m_PositionSetpointTripletMsg.Current.Yawspeed = 0.0f;
    oFPC.m_PositionSetpointTripletMsg.Current.LoiterRadius = 50.0f;
    oFPC.m_PositionSetpointTripletMsg.Current.PitchMin = 0.0f;
    oFPC.m_PositionSetpointTripletMsg.Current.AX = 0.0f;
    oFPC.m_PositionSetpointTripletMsg.Current.AY = 0.0f;
    oFPC.m_PositionSetpointTripletMsg.Current.AZ = 0.0f;
    oFPC.m_PositionSetpointTripletMsg.Current.AcceptanceRadius = 0.0f;
    oFPC.m_PositionSetpointTripletMsg.Current.CruisingSpeed = -1.0f;
    oFPC.m_PositionSetpointTripletMsg.Current.CruisingThrottle = -1.0f;
    oFPC.m_PositionSetpointTripletMsg.Current.Valid = TRUE;
    oFPC.m_PositionSetpointTripletMsg.Current.Type = PX4_SETPOINT_TYPE_LAND;
    oFPC.m_PositionSetpointTripletMsg.Current.PositionValid = FALSE;
    oFPC.m_PositionSetpointTripletMsg.Current.VelocityValid = FALSE;
    oFPC.m_PositionSetpointTripletMsg.Current.VelocityFrame = 0;
    oFPC.m_PositionSetpointTripletMsg.Current.AltValid = TRUE;
    oFPC.m_PositionSetpointTripletMsg.Current.YawValid = TRUE;
    oFPC.m_PositionSetpointTripletMsg.Current.DisableMcYawControl = FALSE;
    oFPC.m_PositionSetpointTripletMsg.Current.YawspeedValid = FALSE;
    oFPC.m_PositionSetpointTripletMsg.Current.LoiterDirection = 1;
    oFPC.m_PositionSetpointTripletMsg.Current.AccelerationValid = FALSE;
    oFPC.m_PositionSetpointTripletMsg.Current.AccelerationIsForce = FALSE;

    oFPC.ProcessNewCmds();

    /* Verify results */
    if ((oFPC.HkTlm.usCmdCnt == 1) && (ProcessNewCmds_Result == FPC_DO_GO_AROUND_CC))
    {
        UtAssert_True(TRUE, "ProcessNewCmds, DoGoAround");
    }
    else
    {
        UtAssert_True(FALSE, "ProcessNewCmds, DoGoAround");
    }
}

#if 0
void Test_FPC_ProcessNewCmds_DoGoAround(void)
{
#if 0
    FPC  oFPC{};
#endif

    int32                             CmdPipe;
    int32                             DataPipe;
    FPC_NoArgCmd_t                    CmdMsg;
    PX4_VehicleControlModeMsg_t       VCMode;
    PX4_PositionSetpointTripletMsg_t  PSpTriple;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */

    DataPipe = Ut_CFE_SB_CreatePipe("FPC_DATA_PIPE");

    CFE_SB_InitMsg ((void*)&VCMode, PX4_VEHICLE_CONTROL_MODE_MID, sizeof(VCMode), TRUE);
    VCMode.Timestamp = FPC_Test_GetTimeUs();
    VCMode.ExternalManualOverrideOk = FALSE;
    VCMode.SystemHilEnabled = FALSE;
    VCMode.ControlManualEnabled = FALSE;
    VCMode.ControlAutoEnabled = TRUE;
    VCMode.ControlOffboardEnabled = FALSE;                       // fix this
    VCMode.ControlRatesEnabled = TRUE;
    VCMode.ControlAttitudeEnabled = TRUE;
    VCMode.ControlRattitudeEnabled = TRUE;
    VCMode.ControlForceEnabled = FALSE;
    VCMode.ControlAccelerationEnabled = FALSE;
    VCMode.ControlVelocityEnabled = FALSE;
    VCMode.ControlPositionEnabled = FALSE;
    VCMode.ControlAltitudeEnabled = FALSE;
    VCMode.ControlClimbRateEnabled = FALSE;
    VCMode.ControlTerminationEnabled = FALSE;
    VCMode.ControlFixedHdgEnabled = FALSE;
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&VCMode);
    Ut_CFE_SB_AddMsgToPipe((void*)&VCMode, (CFE_SB_PipeId_t)DataPipe);

    CFE_SB_InitMsg ((void*)&PSpTriple, PX4_POSITION_SETPOINT_TRIPLET_MID, sizeof(PSpTriple), TRUE);
    PSpTriple.Timestamp = FPC_Test_GetTimeUs();
    PSpTriple.Previous.Timestamp = FPC_Test_GetTimeUs();
    PSpTriple.Previous.Lat = 47.397741928975;
    PSpTriple.Previous.Lon = 8.545593979817;
    PSpTriple.Previous.X = 0.0f;                                  // fix this
    PSpTriple.Previous.Y = 0.0f;                                  // fix this
    PSpTriple.Previous.Z = 0.0f;                                  // fix this
    PSpTriple.Previous.VX = 0.0f;                                 // fix this
    PSpTriple.Previous.VY = 0.0f;                                 // fix this
    PSpTriple.Previous.VZ = 0.0f;                                 // fix this
    PSpTriple.Previous.Alt = 490.7512f;                                // fix this
    PSpTriple.Previous.Yaw = 1.547718f;                                // fix this
    PSpTriple.Previous.Yawspeed = 0.0f;                           // fix this
    PSpTriple.Previous.LoiterRadius = 50.0f;                       // fix this
    PSpTriple.Previous.PitchMin = 0.0f;                           // fix this
    PSpTriple.Previous.AX = 0.0f;                                 // fix this
    PSpTriple.Previous.AY = 0.0f;                                 // fix this
    PSpTriple.Previous.AZ = 0.0f;                                 // fix this
    PSpTriple.Previous.AcceptanceRadius = 0.0f;                   // fix this
    PSpTriple.Previous.CruisingSpeed = -1.0f;                      // fix this
    PSpTriple.Previous.CruisingThrottle = -1.0f;                   // fix this
    PSpTriple.Previous.Valid = TRUE;                                // fix this
    PSpTriple.Previous.Type = PX4_SETPOINT_TYPE_LOITER;
    PSpTriple.Previous.PositionValid = FALSE;
    PSpTriple.Previous.VelocityValid = FALSE;
    PSpTriple.Previous.VelocityFrame = 0;
    PSpTriple.Previous.AltValid = TRUE;
    PSpTriple.Previous.YawValid = TRUE;
    PSpTriple.Previous.DisableMcYawControl = FALSE;
    PSpTriple.Previous.YawspeedValid = FALSE;
    PSpTriple.Previous.LoiterDirection = 1;
    PSpTriple.Previous.AccelerationValid = FALSE;
    PSpTriple.Previous.AccelerationIsForce = FALSE;

    PSpTriple.Current.Timestamp = FPC_Test_GetTimeUs();
    PSpTriple.Current.Lat = 47.397741928975;
    PSpTriple.Current.Lon = 8.545593979817;
    PSpTriple.Current.X = 0.0f;                                   // fix this
    PSpTriple.Current.Y = 0.0f;                                   // fix this
    PSpTriple.Current.Z = 0.0f;                                   // fix this
    PSpTriple.Current.VX = 0.0f;                                  // fix this
    PSpTriple.Current.VY = 0.0f;                                  // fix this
    PSpTriple.Current.VZ = 0.0f;                                  // fix this
    PSpTriple.Current.Alt = 490.7512f;                                 // fix this
    PSpTriple.Current.Yaw = 1.547718f;                                 // fix this
    PSpTriple.Current.Yawspeed = 0.0f;                            // fix this
    PSpTriple.Current.LoiterRadius = 50.0f;                        // fix this
    PSpTriple.Current.PitchMin = 0.0f;                            // fix this
    PSpTriple.Current.AX = 0.0f;                                  // fix this
    PSpTriple.Current.AY = 0.0f;                                  // fix this
    PSpTriple.Current.AZ = 0.0f;                                  // fix this
    PSpTriple.Current.AcceptanceRadius = 0.0f;                    // fix this
    PSpTriple.Current.CruisingSpeed = -1.0f;                       // fix this
    PSpTriple.Current.CruisingThrottle = -1.0f;                    // fix this
    PSpTriple.Current.Valid = TRUE;
    PSpTriple.Current.Type = PX4_SETPOINT_TYPE_LAND;
    PSpTriple.Current.PositionValid = FALSE;
    PSpTriple.Current.VelocityValid = FALSE;
    PSpTriple.Current.VelocityFrame = 0;
    PSpTriple.Current.AltValid = TRUE;
    PSpTriple.Current.YawValid = TRUE;
    PSpTriple.Current.DisableMcYawControl = FALSE;
    PSpTriple.Current.YawspeedValid = FALSE;
    PSpTriple.Current.LoiterDirection = 1;
    PSpTriple.Current.AccelerationValid = FALSE;
    PSpTriple.Current.AccelerationIsForce = FALSE;

    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_DO_GO_AROUND_CC);
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *)&CmdMsg);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FPC_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 4);

    ProcessNewCmds_Result = 0xffffffff;
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    oFPC.AppMain();

    /* Verify results */
    if ((oFPC.HkTlm.usCmdCnt == 1) && (ProcessNewCmds_Result == FPC_DO_GO_AROUND_CC))
    {
        UtAssert_True(TRUE, "ProcessNewCmds, DoGoAround");
    }
    else
    {
        UtAssert_True(FALSE, "ProcessNewCmds, DoGoAround");
    }
}
#endif


/**
 * Test FPC VerifyCmdLength(), Fail CmdLength
 */
void Test_FPC_VerifyCmdLength_Fail_CmdLength(void)
{
#if 1
    FPC  oFPC{};
#endif

    boolean           bResult = TRUE;
    boolean           bExpected = FALSE;
    FPC_NoArgCmd_t    CmdMsg;

    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg) + 5, TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_NOOP_CC);

    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    /* Execute the function being tested */
    bResult = oFPC.VerifyCmdLength((CFE_SB_MsgPtr_t)&CmdMsg, sizeof(CmdMsg) + 10);

    /* Verify results */
    UtAssert_True (((bResult == bExpected) && (oFPC.HkTlm.usCmdErrCnt == 1)),
                   "VerifyCmdLength, Fail CmdLength");
}


void FPC_Cmds_Test_AddTestCases(void)
{
#if 0
    UtTest_Add(Test_FPC_ProcessNewCmds_InvalidCmd, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_ProcessNewCmds_InvalidCmd");
    UtTest_Add(Test_FPC_ProcessNewCmds_InvalidCmdCode, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_ProcessNewCmds_InvalidCmdCode");
    UtTest_Add(Test_FPC_ProcessNewCmds_CmdPipeError, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_ProcessNewCmds_CmdPipeError");
    UtTest_Add(Test_FPC_ProcessNewCmds_Noop, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_ProcessNewCmds_Noop");
    UtTest_Add(Test_FPC_ProcessNewCmds_Reset, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_ProcessNewCmds_Reset");
#endif
    UtTest_Add(Test_FPC_ProcessNewCmds_DoGoAround, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_ProcessNewCmds_DoGoAround");
    UtTest_Add(Test_FPC_VerifyCmdLength_Fail_CmdLength, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_VerifyCmdLength_Fail_CmdLength");
} /* end FPC_Cmds_Test_AddTestCases */
