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

#include <float.h>


boolean   ProcessNewCmds_Result = FALSE;
char      ProcessNewCmds_Str[64];

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
    FPC_NoArgCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, PX4_AIRSPEED_MID, sizeof(CmdMsg), TRUE);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

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
    FPC_NoArgCmd_t     CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)255);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

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
    FPC_NoArgCmd_t     CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    SchPipe = Ut_CFE_SB_CreatePipe("FPC_SCH_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_WAKEUP_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)0);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)SchPipe);

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
    char     *pCmdStr = NULL;

    va_start(Ptr, EventText);
    vsnprintf(Buf, (size_t)CFE_EVS_MAX_MESSAGE_LENGTH, EventText, Ptr);
    va_end(Ptr);

    printf("###ProcessNewCmds_SendEventHook:\n");
    printf("%s\n", Buf);

    pCmdStr = strstr(Buf, ProcessNewCmds_Str);

    if (pCmdStr != NULL)
    {
        ProcessNewCmds_Result = TRUE;
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
    FPC_NoArgCmd_t     CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_NOOP_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FPC_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "NOOP");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    oFPC.AppMain();

    /* Verify results */
    if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
         && (ProcessNewCmds_Result == TRUE))
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
    FPC_NoArgCmd_t     CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_RESET_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, FPC_WAKEUP_MID, 1);

    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "RESET");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    oFPC.AppMain();

    /* Verify results */
    if ((oFPC.HkTlm.usCmdCnt == 0) && (oFPC.HkTlm.usCmdErrCnt == 0)
         && (ProcessNewCmds_Result == TRUE))
    {
        UtAssert_True(TRUE, "ProcessNewCmds, Reset");
    }
    else
    {
        UtAssert_True(FALSE, "ProcessNewCmds, Reset");
    }
}


/**
 * Test FPC ProcessNewCmds, DoGoAround_Data
 */
void Test_FPC_ProcessNewCmds_DoGoAround_Data(FPC *pFPC)
{
    pFPC->m_VehicleControlModeMsg.Timestamp = FPC_Test_GetTimeUs();
    pFPC->m_VehicleControlModeMsg.ExternalManualOverrideOk = FALSE;
    pFPC->m_VehicleControlModeMsg.SystemHilEnabled = FALSE;
    pFPC->m_VehicleControlModeMsg.ControlManualEnabled = FALSE;
    pFPC->m_VehicleControlModeMsg.ControlAutoEnabled = TRUE;
    pFPC->m_VehicleControlModeMsg.ControlOffboardEnabled = FALSE;
    pFPC->m_VehicleControlModeMsg.ControlRatesEnabled = TRUE;
    pFPC->m_VehicleControlModeMsg.ControlAttitudeEnabled = TRUE;
    pFPC->m_VehicleControlModeMsg.ControlRattitudeEnabled = TRUE;
    pFPC->m_VehicleControlModeMsg.ControlForceEnabled = FALSE;
    pFPC->m_VehicleControlModeMsg.ControlAccelerationEnabled = FALSE;
    pFPC->m_VehicleControlModeMsg.ControlVelocityEnabled = FALSE;
    pFPC->m_VehicleControlModeMsg.ControlPositionEnabled = FALSE;
    pFPC->m_VehicleControlModeMsg.ControlAltitudeEnabled = FALSE;
    pFPC->m_VehicleControlModeMsg.ControlClimbRateEnabled = FALSE;
    pFPC->m_VehicleControlModeMsg.ControlTerminationEnabled = FALSE;
    pFPC->m_VehicleControlModeMsg.ControlFixedHdgEnabled = FALSE;

    pFPC->m_PositionSetpointTripletMsg.Timestamp = FPC_Test_GetTimeUs();
    pFPC->m_PositionSetpointTripletMsg.Previous.Timestamp = FPC_Test_GetTimeUs();
    pFPC->m_PositionSetpointTripletMsg.Previous.Lat = 47.397741928975;
    pFPC->m_PositionSetpointTripletMsg.Previous.Lon = 8.545593979817;
    pFPC->m_PositionSetpointTripletMsg.Previous.X = 0.0f;
    pFPC->m_PositionSetpointTripletMsg.Previous.Y = 0.0f;
    pFPC->m_PositionSetpointTripletMsg.Previous.Z = 0.0f;
    pFPC->m_PositionSetpointTripletMsg.Previous.VX = 0.0f;
    pFPC->m_PositionSetpointTripletMsg.Previous.VY = 0.0f;
    pFPC->m_PositionSetpointTripletMsg.Previous.VZ = 0.0f;
    pFPC->m_PositionSetpointTripletMsg.Previous.Alt = 490.7512f;
    pFPC->m_PositionSetpointTripletMsg.Previous.Yaw = 1.547718f;
    pFPC->m_PositionSetpointTripletMsg.Previous.Yawspeed = 0.0f;
    pFPC->m_PositionSetpointTripletMsg.Previous.LoiterRadius = 50.0f;
    pFPC->m_PositionSetpointTripletMsg.Previous.PitchMin = 0.0f;
    pFPC->m_PositionSetpointTripletMsg.Previous.AX = 0.0f;
    pFPC->m_PositionSetpointTripletMsg.Previous.AY = 0.0f;
    pFPC->m_PositionSetpointTripletMsg.Previous.AZ = 0.0f;
    pFPC->m_PositionSetpointTripletMsg.Previous.AcceptanceRadius = 0.0f;
    pFPC->m_PositionSetpointTripletMsg.Previous.CruisingSpeed = -1.0f;
    pFPC->m_PositionSetpointTripletMsg.Previous.CruisingThrottle = -1.0f;
    pFPC->m_PositionSetpointTripletMsg.Previous.Valid = TRUE;
    pFPC->m_PositionSetpointTripletMsg.Previous.Type = PX4_SETPOINT_TYPE_LOITER;
    pFPC->m_PositionSetpointTripletMsg.Previous.PositionValid = FALSE;
    pFPC->m_PositionSetpointTripletMsg.Previous.VelocityValid = FALSE;
    pFPC->m_PositionSetpointTripletMsg.Previous.VelocityFrame = 0;
    pFPC->m_PositionSetpointTripletMsg.Previous.AltValid = TRUE;
    pFPC->m_PositionSetpointTripletMsg.Previous.YawValid = TRUE;
    pFPC->m_PositionSetpointTripletMsg.Previous.DisableMcYawControl = FALSE;
    pFPC->m_PositionSetpointTripletMsg.Previous.YawspeedValid = FALSE;
    pFPC->m_PositionSetpointTripletMsg.Previous.LoiterDirection = 1;
    pFPC->m_PositionSetpointTripletMsg.Previous.AccelerationValid = FALSE;
    pFPC->m_PositionSetpointTripletMsg.Previous.AccelerationIsForce = FALSE;

    pFPC->m_PositionSetpointTripletMsg.Current.Timestamp = FPC_Test_GetTimeUs();
    pFPC->m_PositionSetpointTripletMsg.Current.Lat = 47.397741928975;
    pFPC->m_PositionSetpointTripletMsg.Current.Lon = 8.545593979817;
    pFPC->m_PositionSetpointTripletMsg.Current.X = 0.0f;
    pFPC->m_PositionSetpointTripletMsg.Current.Y = 0.0f;
    pFPC->m_PositionSetpointTripletMsg.Current.Z = 0.0f;
    pFPC->m_PositionSetpointTripletMsg.Current.VX = 0.0f;
    pFPC->m_PositionSetpointTripletMsg.Current.VY = 0.0f;
    pFPC->m_PositionSetpointTripletMsg.Current.VZ = 0.0f;
    pFPC->m_PositionSetpointTripletMsg.Current.Alt = 490.7512f;
    pFPC->m_PositionSetpointTripletMsg.Current.Yaw = 1.547718f;
    pFPC->m_PositionSetpointTripletMsg.Current.Yawspeed = 0.0f;
    pFPC->m_PositionSetpointTripletMsg.Current.LoiterRadius = 50.0f;
    pFPC->m_PositionSetpointTripletMsg.Current.PitchMin = 0.0f;
    pFPC->m_PositionSetpointTripletMsg.Current.AX = 0.0f;
    pFPC->m_PositionSetpointTripletMsg.Current.AY = 0.0f;
    pFPC->m_PositionSetpointTripletMsg.Current.AZ = 0.0f;
    pFPC->m_PositionSetpointTripletMsg.Current.AcceptanceRadius = 0.0f;
    pFPC->m_PositionSetpointTripletMsg.Current.CruisingSpeed = -1.0f;
    pFPC->m_PositionSetpointTripletMsg.Current.CruisingThrottle = -1.0f;
    pFPC->m_PositionSetpointTripletMsg.Current.Valid = TRUE;
    pFPC->m_PositionSetpointTripletMsg.Current.Type = PX4_SETPOINT_TYPE_LAND;
    pFPC->m_PositionSetpointTripletMsg.Current.PositionValid = FALSE;
    pFPC->m_PositionSetpointTripletMsg.Current.VelocityValid = FALSE;
    pFPC->m_PositionSetpointTripletMsg.Current.VelocityFrame = 0;
    pFPC->m_PositionSetpointTripletMsg.Current.AltValid = TRUE;
    pFPC->m_PositionSetpointTripletMsg.Current.YawValid = TRUE;
    pFPC->m_PositionSetpointTripletMsg.Current.DisableMcYawControl = FALSE;
    pFPC->m_PositionSetpointTripletMsg.Current.YawspeedValid = FALSE;
    pFPC->m_PositionSetpointTripletMsg.Current.LoiterDirection = 1;
    pFPC->m_PositionSetpointTripletMsg.Current.AccelerationValid = FALSE;
    pFPC->m_PositionSetpointTripletMsg.Current.AccelerationIsForce = FALSE;
}


/**
 * Test FPC ProcessNewCmds, DoGoAround
 */
void Test_FPC_ProcessNewCmds_DoGoAround(void)
{
    FPC  oFPC{};
    FPC  *pFPC = &oFPC;

    int32              iStatus = CFE_SUCCESS;
    int32              CmdPipe;
    FPC_NoArgCmd_t     CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */

    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_DO_GO_AROUND_CC);
    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);

    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "DO_GO_AROUND");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                Test_FPC_ProcessNewCmds_DoGoAround_Data(pFPC);
                oFPC.ProcessNewCmds();
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, DoGoAround");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
            && (ProcessNewCmds_Result == TRUE))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, DoGoAround");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, DoGoAround");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_L1_PERIOD_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_L1_PERIOD_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_L1_PERIOD_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "L1_PERIOD");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->L1_PERIOD + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_L1_PERIOD_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->L1_PERIOD - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_L1_PERIOD_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_L1_PERIOD_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_L1_DAMPING_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_L1_DAMPING_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_L1_DAMPING_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "L1_DAMPING");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->L1_DAMPING + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_L1_DAMPING_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->L1_DAMPING - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_L1_DAMPING_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_L1_DAMPING_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_T_TIME_CONST_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_T_TIME_CONST_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_T_TIME_CONST_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "T_TIME_CONST");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->T_TIME_CONST + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_TIME_CONST_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->T_TIME_CONST - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_T_TIME_CONST_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_TIME_CONST_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_T_THRO_CONST_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_T_THRO_CONST_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_T_THRO_CONST_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "T_THRO_CONST");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->T_THRO_CONST + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_THRO_CONST_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->T_THRO_CONST - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_T_THRO_CONST_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_THRO_CONST_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_T_SINK_MIN_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_T_SINK_MIN_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_T_SINK_MIN_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "T_SINK_MIN");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->T_SINK_MIN + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_SINK_MIN_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->T_SINK_MIN - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_T_SINK_MIN_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_SINK_MIN_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_T_SINK_MAX_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_T_SINK_MAX_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_T_SINK_MAX_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "T_SINK_MAX");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->T_SINK_MAX + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_SINK_MAX_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->T_SINK_MAX - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_T_SINK_MAX_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_SINK_MAX_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_T_CLMB_MAX_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_T_CLMB_MAX_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_T_CLMB_MAX_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "T_CLMB_MAX");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->T_CLMB_MAX + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_CLMB_MAX_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->T_CLMB_MAX - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_T_CLMB_MAX_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_CLMB_MAX_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_CLMBOUT_DIFF_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_CLMBOUT_DIFF_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_CLMBOUT_DIFF_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "CLMBOUT_DIFF");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->CLMBOUT_DIFF + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_CLMBOUT_DIFF_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->CLMBOUT_DIFF - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_CLMBOUT_DIFF_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_CLMBOUT_DIFF_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_T_HRATE_P_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_T_HRATE_P_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_T_HRATE_P_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "T_HRATE_P");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->T_HRATE_P + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_HRATE_P_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->T_HRATE_P - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_T_HRATE_P_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_HRATE_P_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_T_HRATE_FF_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_T_HRATE_FF_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_T_HRATE_FF_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "T_HRATE_FF");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->T_HRATE_FF + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_HRATE_FF_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->T_HRATE_FF - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_T_HRATE_FF_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_HRATE_FF_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_T_SRATE_P_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_T_SRATE_P_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_T_SRATE_P_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "T_SRATE_P");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->T_SRATE_P + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_SRATE_P_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->T_SRATE_P - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_T_SRATE_P_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_SRATE_P_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_T_THR_DAMP_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_T_THR_DAMP_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_T_THR_DAMP_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "T_THR_DAMP");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->T_THR_DAMP + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_THR_DAMP_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->T_THR_DAMP - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_T_THR_DAMP_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_THR_DAMP_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_T_INTEG_GAIN_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_T_INTEG_GAIN_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_T_INTEG_GAIN_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "T_INTEG_GAIN");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->T_INTEG_GAIN + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_INTEG_GAIN_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->T_INTEG_GAIN - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_T_INTEG_GAIN_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_INTEG_GAIN_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_T_VERT_ACC_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_T_VERT_ACC_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_T_VERT_ACC_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "T_VERT_ACC");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->T_VERT_ACC + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_VERT_ACC_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->T_VERT_ACC - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_T_VERT_ACC_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_VERT_ACC_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_T_HGT_OMEGA_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_T_HGT_OMEGA_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_T_HGT_OMEGA_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "T_HGT_OMEGA");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->T_HGT_OMEGA + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_HGT_OMEGA_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->T_HGT_OMEGA - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_T_HGT_OMEGA_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_HGT_OMEGA_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_T_SPD_OMEGA_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_T_SPD_OMEGA_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_T_SPD_OMEGA_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "T_SPD_OMEGA");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->T_SPD_OMEGA + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_SPD_OMEGA_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->T_SPD_OMEGA - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_T_SPD_OMEGA_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_SPD_OMEGA_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_T_RLL2THR_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_T_RLL2THR_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_T_RLL2THR_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "T_RLL2THR");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->T_RLL2THR + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_RLL2THR_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->T_RLL2THR - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_T_RLL2THR_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_RLL2THR_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_T_SPDWEIGHT_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_T_SPDWEIGHT_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_T_SPDWEIGHT_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "T_SPDWEIGHT");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->T_SPDWEIGHT + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_SPDWEIGHT_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->T_SPDWEIGHT - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_T_SPDWEIGHT_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_SPDWEIGHT_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_T_PTCH_DAMP_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_T_PTCH_DAMP_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_T_PTCH_DAMP_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "T_PTCH_DAMP");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->T_PTCH_DAMP + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_PTCH_DAMP_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->T_PTCH_DAMP - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_T_PTCH_DAMP_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_T_PTCH_DAMP_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_AIRSPD_MIN_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_AIRSPD_MIN_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_AIRSPD_MIN_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "AIRSPD_MIN");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->AIRSPD_MIN + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_AIRSPD_MIN_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->AIRSPD_MIN - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_AIRSPD_MIN_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_AIRSPD_MIN_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_AIRSPD_TRIM_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_AIRSPD_TRIM_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_AIRSPD_TRIM_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "AIRSPD_TRIM");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->AIRSPD_TRIM + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_AIRSPD_TRIM_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->AIRSPD_TRIM - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_AIRSPD_TRIM_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_AIRSPD_TRIM_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_AIRSPD_MAX_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_AIRSPD_MAX_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_AIRSPD_MAX_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "AIRSPD_MAX");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->AIRSPD_MAX + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_AIRSPD_MAX_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->AIRSPD_MAX - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_AIRSPD_MAX_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_AIRSPD_MAX_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_ARSP_MODE_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_ARSP_MODE_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamBooleanCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_ARSP_MODE_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "ARSP_MODE");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = !oFPC.ConfigTblPtr->ARSP_MODE;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_ARSP_MODE_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->ARSP_MODE - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_ARSP_MODE_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_ARSP_MODE_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_P_LIM_MIN_RADIANS_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_P_LIM_MIN_RADIANS_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_P_LIM_MIN_RADIANS_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "P_LIM_MIN_RADIANS");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->P_LIM_MIN_RADIANS + 0.1f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_P_LIM_MIN_RADIANS_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->P_LIM_MIN_RADIANS - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_P_LIM_MIN_RADIANS_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_P_LIM_MIN_RADIANS_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_P_LIM_MAX_RADIANS_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_P_LIM_MAX_RADIANS_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_P_LIM_MAX_RADIANS_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "P_LIM_MAX_RADIANS");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->P_LIM_MAX_RADIANS + 0.1f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_P_LIM_MAX_RADIANS_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->P_LIM_MAX_RADIANS - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_P_LIM_MAX_RADIANS_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_P_LIM_MAX_RADIANS_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_R_LIM_RADIANS_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_R_LIM_RADIANS_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_R_LIM_RADIANS_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "R_LIM_RADIANS");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->R_LIM_RADIANS + 0.1f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_R_LIM_RADIANS_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->R_LIM_RADIANS - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_R_LIM_RADIANS_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_R_LIM_RADIANS_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_THR_MIN_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_THR_MIN_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_THR_MIN_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "THR_MIN");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->THR_MIN + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_THR_MIN_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->THR_MIN - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_THR_MIN_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_THR_MIN_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_THR_MAX_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_THR_MAX_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_THR_MAX_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "THR_MAX");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->THR_MAX + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_THR_MAX_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->THR_MAX - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_THR_MAX_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_THR_MAX_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_THR_IDLE_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_THR_IDLE_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_THR_IDLE_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "THR_IDLE");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->THR_IDLE + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_THR_IDLE_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->THR_IDLE - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_THR_IDLE_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_THR_IDLE_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_THR_CRUISE_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_THR_CRUISE_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_THR_CRUISE_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "THR_CRUISE");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->THR_CRUISE + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_THR_CRUISE_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->THR_CRUISE - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_THR_CRUISE_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_THR_CRUISE_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_THR_SLEW_MAX_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_THR_SLEW_MAX_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_THR_SLEW_MAX_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "THR_SLEW_MAX");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->THR_SLEW_MAX + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_THR_SLEW_MAX_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->THR_SLEW_MAX - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_THR_SLEW_MAX_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_THR_SLEW_MAX_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_THR_ALT_SCL_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_THR_ALT_SCL_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_THR_ALT_SCL_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "THR_ALT_SCL");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->THR_ALT_SCL + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_THR_ALT_SCL_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->THR_ALT_SCL - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_THR_ALT_SCL_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_THR_ALT_SCL_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_MAN_R_MAX_RADIANS_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_MAN_R_MAX_RADIANS_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_MAN_R_MAX_RADIANS_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "MAN_R_MAX_RADIANS");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->MAN_R_MAX_RADIANS + 0.1f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_MAN_R_MAX_RADIANS_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->MAN_R_MAX_RADIANS - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_MAN_R_MAX_RADIANS_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_MAN_R_MAX_RADIANS_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_MAN_P_MAX_RADIANS_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_MAN_P_MAX_RADIANS_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_MAN_P_MAX_RADIANS_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "MAN_P_MAX_RADIANS");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->MAN_P_MAX_RADIANS + 0.1f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_MAN_P_MAX_RADIANS_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->MAN_P_MAX_RADIANS - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_MAN_P_MAX_RADIANS_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_MAN_P_MAX_RADIANS_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_RSP_OFF_RADIANS_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_RSP_OFF_RADIANS_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_RSP_OFF_RADIANS_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "RSP_OFF_RADIANS");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->RSP_OFF_RADIANS + 0.1f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_RSP_OFF_RADIANS_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->RSP_OFF_RADIANS - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_RSP_OFF_RADIANS_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_RSP_OFF_RADIANS_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_PSP_OFF_RADIANS_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_PSP_OFF_RADIANS_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_PSP_OFF_RADIANS_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "PSP_OFF_RADIANS");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->PSP_OFF_RADIANS + 0.1f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_PSP_OFF_RADIANS_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->PSP_OFF_RADIANS - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_PSP_OFF_RADIANS_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_PSP_OFF_RADIANS_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_THR_LND_MAX_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_THR_LND_MAX_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_THR_LND_MAX_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "THR_LND_MAX");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->THR_LND_MAX + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_THR_LND_MAX_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->THR_LND_MAX - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_THR_LND_MAX_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_THR_LND_MAX_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_LND_ANG_RADIANS_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_LND_ANG_RADIANS_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_LND_ANG_RADIANS_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "LND_ANG_RADIANS");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->LND_ANG_RADIANS + 0.1f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LND_ANG_RADIANS_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->LND_ANG_RADIANS - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_LND_ANG_RADIANS_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LND_ANG_RADIANS_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_LND_HVIRT_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_LND_HVIRT_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_LND_HVIRT_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "LND_HVIRT");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->LND_HVIRT + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LND_HVIRT_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->LND_HVIRT - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_LND_HVIRT_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LND_HVIRT_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_LND_FLALT_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_LND_FLALT_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_LND_FLALT_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "LND_FLALT");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->LND_FLALT + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LND_FLALT_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->LND_FLALT - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_LND_FLALT_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LND_FLALT_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_LND_TLALT_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_LND_TLALT_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_LND_TLALT_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "LND_TLALT");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->LND_TLALT + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LND_TLALT_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->LND_TLALT - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_LND_TLALT_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LND_TLALT_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_LND_HHDIST_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_LND_HHDIST_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_LND_HHDIST_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "LND_HHDIST");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->LND_HHDIST + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LND_HHDIST_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->LND_HHDIST - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_LND_HHDIST_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LND_HHDIST_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_LND_FL_PMIN_RADIANS_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_LND_FL_PMIN_RADIANS_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_LND_FL_PMIN_RADIANS_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "LND_FL_PMIN_RADIANS");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->LND_FL_PMIN_RADIANS + 0.1f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LND_FL_PMIN_RADIANS_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->LND_FL_PMIN_RADIANS - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_LND_FL_PMIN_RADIANS_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LND_FL_PMIN_RADIANS_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_LND_FL_PMAX_RADIANS_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_LND_FL_PMAX_RADIANS_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_LND_FL_PMAX_RADIANS_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "LND_FL_PMAX_RADIANS");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->LND_FL_PMAX_RADIANS + 0.1f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LND_FL_PMAX_RADIANS_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->LND_FL_PMAX_RADIANS - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_LND_FL_PMAX_RADIANS_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LND_FL_PMAX_RADIANS_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_LND_USETER_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_LND_USETER_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamInt32Cmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_LND_USETER_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "LND_USETER");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->LND_USETER + 1;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LND_USETER_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->LND_USETER - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_LND_USETER_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LND_USETER_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_LND_AIRSPD_SC_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_LND_AIRSPD_SC_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_LND_AIRSPD_SC_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "LND_AIRSPD_SC");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->LND_AIRSPD_SC + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LND_AIRSPD_SC_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->LND_AIRSPD_SC - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_LND_AIRSPD_SC_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LND_AIRSPD_SC_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_LAUN_ALL_ON_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_LAUN_ALL_ON_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamBooleanCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_LAUN_ALL_ON_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "LAUN_ALL_ON");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = !oFPC.ConfigTblPtr->FPC_Launch_Detection.LAUN_ALL_ON;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LAUN_ALL_ON_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->FPC_Launch_Detection.LAUN_ALL_ON - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_LAUN_ALL_ON_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LAUN_ALL_ON_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_LAUN_CAT_A_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_LAUN_CAT_A_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_LAUN_CAT_A_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "LAUN_CAT_A");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->FPC_Launch_Detection.LAUN_CAT_A + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LAUN_CAT_A_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->FPC_Launch_Detection.LAUN_CAT_A - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_LAUN_CAT_A_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LAUN_CAT_A_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_LAUN_CAT_T_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_LAUN_CAT_T_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_LAUN_CAT_T_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "LAUN_CAT_T");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->FPC_Launch_Detection.LAUN_CAT_T + 0.1f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LAUN_CAT_T_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->FPC_Launch_Detection.LAUN_CAT_T - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_LAUN_CAT_T_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LAUN_CAT_T_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_LAUN_CAT_MDEL_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_LAUN_CAT_MDEL_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_LAUN_CAT_MDEL_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "LAUN_CAT_MDEL");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->FPC_Launch_Detection.LAUN_CAT_MDEL + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LAUN_CAT_MDEL_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->FPC_Launch_Detection.LAUN_CAT_MDEL - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_LAUN_CAT_MDEL_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LAUN_CAT_MDEL_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_LAUN_CAT_PMAX_RADIANS_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_LAUN_CAT_PMAX_RADIANS_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_LAUN_CAT_PMAX_RADIANS_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "LAUN_CAT_PMAX_RADIANS");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->FPC_Launch_Detection.LAUN_CAT_PMAX_RADIANS + 0.1f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LAUN_CAT_PMAX_RADIANS_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
        && (ProcessNewCmds_Result == TRUE)
        && (fabs(oFPC.ConfigTblPtr->FPC_Launch_Detection.LAUN_CAT_PMAX_RADIANS - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_LAUN_CAT_PMAX_RADIANS_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_LAUN_CAT_PMAX_RADIANS_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_RWTO_TKOFF_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_RWTO_TKOFF_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamBooleanCmd_t  CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_RWTO_TKOFF_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "RWTO_TKOFF");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = !oFPC.ConfigTblPtr->FPC_Runway_Takeoff.RWTO_TKOFF;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_RWTO_TKOFF_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
         && (ProcessNewCmds_Result == TRUE)
         && (fabs(oFPC.ConfigTblPtr->FPC_Runway_Takeoff.RWTO_TKOFF - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_RWTO_TKOFF_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_RWTO_TKOFF_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_RWTO_HDG_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_RWTO_HDG_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamInt32Cmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_RWTO_HDG_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "RWTO_HDG");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->FPC_Runway_Takeoff.RWTO_HDG + 1;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_RWTO_HDG_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
         && (ProcessNewCmds_Result == TRUE)
         && (fabs(oFPC.ConfigTblPtr->FPC_Runway_Takeoff.RWTO_HDG - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_RWTO_HDG_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_RWTO_HDG_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_NAV_ALT_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_NAV_ALT_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_NAV_ALT_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "NAV_ALT");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->FPC_Runway_Takeoff.NAV_ALT + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_NAV_ALT_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
         && (ProcessNewCmds_Result == TRUE)
         && (fabs(oFPC.ConfigTblPtr->FPC_Runway_Takeoff.NAV_ALT - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_NAV_ALT_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_NAV_ALT_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_MAX_THR_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_MAX_THR_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_MAX_THR_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "MAX_THR");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->FPC_Runway_Takeoff.MAX_THR + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_MAX_THR_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
         && (ProcessNewCmds_Result == TRUE)
         && (fabs(oFPC.ConfigTblPtr->FPC_Runway_Takeoff.MAX_THR - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_MAX_THR_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_MAX_THR_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_PSP_RADIANS_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_PSP_RADIANS_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_PSP_RADIANS_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "PSP_RADIANS");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->FPC_Runway_Takeoff.PSP_RADIANS + 0.1f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_PSP_RADIANS_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
         && (ProcessNewCmds_Result == TRUE)
         && (fabs(oFPC.ConfigTblPtr->FPC_Runway_Takeoff.PSP_RADIANS - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_PSP_RADIANS_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_PSP_RADIANS_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_MAX_PITCH_RADIANS_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_MAX_PITCH_RADIANS_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_MAX_PITCH_RADIANS_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "MAX_PITCH_RADIANS");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->FPC_Runway_Takeoff.MAX_PITCH_RADIANS + 0.1f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_MAX_PITCH_RADIANS_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
         && (ProcessNewCmds_Result == TRUE)
         && (fabs(oFPC.ConfigTblPtr->FPC_Runway_Takeoff.MAX_PITCH_RADIANS - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_MAX_PITCH_RADIANS_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_MAX_PITCH_RADIANS_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_MAX_ROLL_RADIANS_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_MAX_ROLL_RADIANS_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_MAX_ROLL_RADIANS_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "MAX_ROLL_RADIANS");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->FPC_Runway_Takeoff.MAX_ROLL_RADIANS + 0.1f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_MAX_ROLL_RADIANS_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
           && (ProcessNewCmds_Result == TRUE)
           && (fabs(oFPC.ConfigTblPtr->FPC_Runway_Takeoff.MAX_ROLL_RADIANS - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_MAX_ROLL_RADIANS_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_MAX_ROLL_RADIANS_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_AIRSPD_SCL_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_AIRSPD_SCL_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_AIRSPD_SCL_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "AIRSPD_SCL");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->FPC_Runway_Takeoff.AIRSPD_SCL + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_AIRSPD_SCL_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
           && (ProcessNewCmds_Result == TRUE)
           && (fabs(oFPC.ConfigTblPtr->FPC_Runway_Takeoff.AIRSPD_SCL - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_AIRSPD_SCL_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_AIRSPD_SCL_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_RUNWAY_AIRSPD_MIN_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_RUNWAY_AIRSPD_MIN_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_RUNWAY_AIRSPD_MIN_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "RWTO_AIRSPD_MIN");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->FPC_Runway_Takeoff.RWTO_AIRSPD_MIN + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_RUNWAY_AIRSPD_MIN_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
           && (ProcessNewCmds_Result == TRUE)
           && (fabs(oFPC.ConfigTblPtr->FPC_Runway_Takeoff.RWTO_AIRSPD_MIN - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_RUNWAY_AIRSPD_MIN_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_RUNWAY_AIRSPD_MIN_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, UPDATE_RUNWAY_CLMBOUT_DIFF_CC
 */
void Test_FPC_ProcessNewCmds_UPDATE_RUNWAY_CLMBOUT_DIFF_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_UPDATE_RUNWAY_CLMBOUT_DIFF_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "RWTO_CLMBOUT_DIFF");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                iStatus = oFPC.InitConfigTbl();
                if (iStatus == CFE_SUCCESS)
                {
                    CmdMsg.param = oFPC.ConfigTblPtr->FPC_Runway_Takeoff.RWTO_CLMBOUT_DIFF + 1.0f;
                    Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                    FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                    oFPC.ProcessNewCmds();
                }
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_RUNWAY_CLMBOUT_DIFF_CC");
    }
    else
    {
       /* Verify results */
       if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
         && (ProcessNewCmds_Result == TRUE)
         && (fabs(oFPC.ConfigTblPtr->FPC_Runway_Takeoff.RWTO_CLMBOUT_DIFF - CmdMsg.param) < FLT_EPSILON))
       {
           UtAssert_True(TRUE, "ProcessNewCmds, UPDATE_RUNWAY_CLMBOUT_DIFF_CC");
       }
       else
       {
           UtAssert_True(FALSE, "ProcessNewCmds, UPDATE_RUNWAY_CLMBOUT_DIFF_CC");
       }
    }
}


/**
 * Test FPC ProcessNewCmds, OVERRIDE_ALTITUDE_CC
 */
void Test_FPC_ProcessNewCmds_OVERRIDE_ALTITUDE_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_OVERRIDE_ALTITUDE_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "_hold_alt");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                CmdMsg.param = oFPC._hold_alt + 1.0f;
                Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                oFPC.ProcessNewCmds();
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, OVERRIDE_ALTITUDE_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
           && (ProcessNewCmds_Result == TRUE)
           && (fabs(oFPC._hold_alt - CmdMsg.param) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, OVERRIDE_ALTITUDE_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, OVERRIDE_ALTITUDE_CC");
        }
    }
}


/**
 * Test FPC ProcessNewCmds, OVERRIDE_HEADING_CC
 */
void Test_FPC_ProcessNewCmds_OVERRIDE_HEADING_CC(void)
{
    FPC  oFPC{};

    int32                        iStatus = CFE_SUCCESS;
    int32                        CmdPipe;
    FPC_UpdateParamFloatCmd_t    CmdMsg;

    /* The following will emulate the behavior of receiving a message,
       and gives it data to process. */
    CmdPipe = Ut_CFE_SB_CreatePipe("FPC_CMD_PIPE");
    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)&CmdMsg, (uint16)FPC_OVERRIDE_HEADING_CC);

    ProcessNewCmds_Result = FALSE;
    strcpy(ProcessNewCmds_Str, "_hdg_hold_yaw");
    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FPC_ProcessNewCmds_SendEventHook);

    /* Execute the function being tested */
    iStatus = oFPC.InitEvent();
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = oFPC.InitPipe();
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = oFPC.InitData();
            if (iStatus == CFE_SUCCESS)
            {
                CmdMsg.param = 45.0f;
                Ut_CFE_SB_AddMsgToPipe((void*)&CmdMsg, (CFE_SB_PipeId_t)CmdPipe);
                FPC_Test_PrintCmdMsg((void*)&CmdMsg, sizeof(CmdMsg));

                oFPC.ProcessNewCmds();
            }
        }
    }

    if (iStatus != CFE_SUCCESS)
    {
        UtAssert_True(FALSE, "ProcessNewCmds, OVERRIDE_HEADING_CC");
    }
    else
    {
        /* Verify results */
        if ((oFPC.HkTlm.usCmdCnt == 1) && (oFPC.HkTlm.usCmdErrCnt == 0)
           && (ProcessNewCmds_Result == TRUE)
           && (fabs(oFPC._hdg_hold_yaw - DEG_TO_RADIANS(CmdMsg.param)) < FLT_EPSILON))
        {
            UtAssert_True(TRUE, "ProcessNewCmds, OVERRIDE_HEADING_CC");
        }
        else
        {
            UtAssert_True(FALSE, "ProcessNewCmds, OVERRIDE_HEADING_CC");
        }
    }
}


/**
 * Test FPC VerifyCmdLength(), Fail CmdLength
 */
void Test_FPC_VerifyCmdLength_Fail_CmdLength(void)
{
    FPC  oFPC{};

    boolean           bResult = TRUE;
    boolean           bExpected = FALSE;
    FPC_NoArgCmd_t    CmdMsg;

    CFE_SB_InitMsg ((void*)&CmdMsg, FPC_CMD_MID, sizeof(CmdMsg), TRUE);
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
    UtTest_Add(Test_FPC_ProcessNewCmds_DoGoAround, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_ProcessNewCmds_DoGoAround");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_L1_PERIOD_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_L1_PERIOD_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_L1_DAMPING_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_L1_DAMPING_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_T_TIME_CONST_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_T_TIME_CONST_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_T_THRO_CONST_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_T_THRO_CONST_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_T_SINK_MIN_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_T_SINK_MIN_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_T_SINK_MAX_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_T_SINK_MAX_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_T_CLMB_MAX_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_T_CLMB_MAX_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_CLMBOUT_DIFF_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_CLMBOUT_DIFF_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_T_HRATE_P_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_T_HRATE_P_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_T_HRATE_FF_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_T_HRATE_FF_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_T_SRATE_P_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_T_SRATE_P_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_T_THR_DAMP_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_T_THR_DAMP_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_T_INTEG_GAIN_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_T_INTEG_GAIN_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_T_VERT_ACC_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_T_VERT_ACC_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_T_HGT_OMEGA_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_T_HGT_OMEGA_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_T_SPD_OMEGA_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_T_SPD_OMEGA_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_T_RLL2THR_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_T_RLL2THR_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_T_SPDWEIGHT_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_T_SPDWEIGHT_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_T_PTCH_DAMP_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_T_PTCH_DAMP_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_AIRSPD_MIN_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_AIRSPD_MIN_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_AIRSPD_TRIM_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_AIRSPD_TRIM_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_AIRSPD_MAX_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_AIRSPD_MAX_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_ARSP_MODE_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_ARSP_MODE_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_P_LIM_MIN_RADIANS_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_P_LIM_MIN_RADIANS_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_P_LIM_MAX_RADIANS_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_P_LIM_MAX_RADIANS_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_R_LIM_RADIANS_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_R_LIM_RADIANS_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_THR_MIN_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_THR_MIN_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_THR_MAX_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_THR_MAX_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_THR_IDLE_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_THR_IDLE_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_THR_CRUISE_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_THR_CRUISE_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_THR_SLEW_MAX_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_THR_SLEW_MAX_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_THR_ALT_SCL_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_THR_ALT_SCL_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_MAN_R_MAX_RADIANS_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_MAN_R_MAX_RADIANS_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_MAN_P_MAX_RADIANS_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_MAN_P_MAX_RADIANS_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_RSP_OFF_RADIANS_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_RSP_OFF_RADIANS_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_PSP_OFF_RADIANS_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_PSP_OFF_RADIANS_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_THR_LND_MAX_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_THR_LND_MAX_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_LND_ANG_RADIANS_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_LND_ANG_RADIANS_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_LND_HVIRT_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_LND_HVIRT_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_LND_FLALT_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_LND_FLALT_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_LND_TLALT_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_LND_TLALT_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_LND_HHDIST_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_LND_HHDIST_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_LND_FL_PMIN_RADIANS_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_LND_FL_PMIN_RADIANS_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_LND_FL_PMAX_RADIANS_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_LND_FL_PMAX_RADIANS_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_LND_USETER_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_LND_USETER_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_LND_AIRSPD_SC_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_LND_AIRSPD_SC_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_LAUN_ALL_ON_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_LAUN_ALL_ON_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_LAUN_CAT_A_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_LAUN_CAT_A_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_LAUN_CAT_T_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_LAUN_CAT_T_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_LAUN_CAT_MDEL_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_LAUN_CAT_MDEL_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_LAUN_CAT_PMAX_RADIANS_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_LAUN_CAT_PMAX_RADIANS_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_RWTO_TKOFF_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_RWTO_TKOFF_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_RWTO_HDG_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_RWTO_HDG_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_NAV_ALT_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_NAV_ALT_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_MAX_THR_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_MAX_THR_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_PSP_RADIANS_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_PSP_RADIANS_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_MAX_PITCH_RADIANS_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_MAX_PITCH_RADIANS_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_MAX_ROLL_RADIANS_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_MAX_ROLL_RADIANS_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_AIRSPD_SCL_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_AIRSPD_SCL_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_RUNWAY_AIRSPD_MIN_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_RUNWAY_AIRSPD_MIN_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_UPDATE_RUNWAY_CLMBOUT_DIFF_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_UPDATE_RUNWAY_CLMBOUT_DIFF_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_OVERRIDE_ALTITUDE_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_OVERRIDE_ALTITUDE_CC");
    UtTest_Add(Test_FPC_ProcessNewCmds_OVERRIDE_HEADING_CC, FPC_Test_Setup,
               FPC_Test_TearDown, "Test_FPC_ProcessNewCmds_OVERRIDE_HEADING_CC");
    UtTest_Add(Test_FPC_VerifyCmdLength_Fail_CmdLength, FPC_Test_Setup, FPC_Test_TearDown,
               "Test_FPC_VerifyCmdLength_Fail_CmdLength");
} /* end FPC_Cmds_Test_AddTestCases */
