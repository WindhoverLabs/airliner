/*==============================================================================
 Copyright (c) 2015, Windhover Labs
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 * Neither the name of TlmOut nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef __cplusplus
extern "C"
{
#endif

/*
 ** Pragmas
 */

/*
 ** Include Files
 */
#define _GNU_SOURCE

#include <math.h>
#include "px4br_mission_cfg.h"
#include "px4br_app.h"
#include "px4br_cmds.h"
#include "px4br_events.h"
#include "px4br_cmd_codes.h"



/*
 ** External Global Variables
 */
extern PX4BR_AppData_t PX4BR_AppData;



/*
 ** Local Function Prototypes
 */

boolean PX4BR_ResetStatus(void);
boolean PX4BR_SendVehicleCommand(PX4_VehicleCommandCmd_t *cmdMsg);



/*
 ** Local Function Definitions
 */

void PX4BR_CmdExec(CFE_SB_MsgPtr_t msg)
{
    uint16 CommandCode;
    boolean bValid = TRUE;

    /* Get Command Code */
    CommandCode = CFE_SB_GetCmdCode(msg);

    switch (CommandCode)
    {
        case PX4BR_NOP_CC:
            /* This is a NoOp command.  Just raise an event. */
            CFE_EVS_SendEvent(
                    PX4BR_NOOP_INF_EID,
                    CFE_EVS_INFORMATION,
                    "No-op Command Rcvd");
            break;

        case PX4BR_RESET_STATUS_CC:
            /* Reset status command. */
            bValid = PX4BR_ResetStatus();
            break;

        case PX4BR_VEHICLE_LAUNCH_CC:
        {
        	PX4_VehicleCommandMsg_t cmd;

        	CFE_SB_InitMsg(&cmd, PX4_VEHICLE_COMMAND_MID, sizeof(PX4_VehicleCommandMsg_t), TRUE);

        	cmd.Timestamp.Seconds = 0;
        	cmd.Timestamp.Subseconds = 0;
        	cmd.Param1 = NAN;
        	cmd.Param2 = NAN;
        	cmd.Param3 = NAN;
        	cmd.Param4 = NAN;
        	cmd.Param5 = NAN;
        	cmd.Param6 = NAN;
        	cmd.Param7 = NAN;
        	cmd.Command = PX4_VEHICLE_CMD_NAV_TAKEOFF;
        	cmd.TargetSystem = 1;
        	cmd.TargetComponent = 1;
        	cmd.SourceSystem = 0;
        	cmd.SourceComponent = 0;
        	cmd.Confirmation = 0;

			PX4BR_RouteMessageToPX4(&cmd);
            break;
        }

        case PX4BR_VEHICLE_LAND_CC:
        {
        	PX4_VehicleCommandMsg_t cmd;

        	CFE_SB_InitMsg(&cmd, PX4_VEHICLE_COMMAND_MID, sizeof(PX4_VehicleCommandMsg_t), TRUE);

        	cmd.Timestamp.Seconds = 0;
        	cmd.Timestamp.Subseconds = 0;
        	cmd.Param1 = NAN;
        	cmd.Param2 = NAN;
        	cmd.Param3 = NAN;
        	cmd.Param4 = NAN;
        	cmd.Param5 = NAN;
        	cmd.Param6 = NAN;
        	cmd.Param7 = NAN;
        	cmd.Command = PX4_VEHICLE_CMD_NAV_LAND;
        	cmd.TargetSystem = 1;
        	cmd.TargetComponent = 1;
        	cmd.SourceSystem = 0;
        	cmd.SourceComponent = 0;
        	cmd.Confirmation = 0;

			PX4BR_RouteMessageToPX4(&cmd);
            break;
        }

        case PX4BR_VEHICLE_ARM_CC:
        {
        	PX4_VehicleCommandMsg_t cmd;

        	CFE_SB_InitMsg(&cmd, PX4_VEHICLE_COMMAND_MID, sizeof(PX4_VehicleCommandMsg_t), TRUE);

        	cmd.Timestamp.Seconds = 0;
        	cmd.Timestamp.Subseconds = 0;
        	cmd.Param1 = 1.0f;
        	cmd.Param2 = NAN;
        	cmd.Param3 = NAN;
        	cmd.Param4 = NAN;
        	cmd.Param5 = NAN;
        	cmd.Param6 = NAN;
        	cmd.Param7 = NAN;
        	cmd.Command = PX4_VEHICLE_CMD_COMPONENT_ARM_DISARM;
        	cmd.TargetSystem = 1;
        	cmd.TargetComponent = 1;
        	cmd.SourceSystem = 0;
        	cmd.SourceComponent = 0;
        	cmd.Confirmation = 0;

			PX4BR_RouteMessageToPX4(&cmd);
            break;
        }

        case PX4BR_VEHICLE_DISARM_CC:
        {
        	PX4_VehicleCommandMsg_t cmd;

        	CFE_SB_InitMsg(&cmd, PX4_VEHICLE_COMMAND_MID, sizeof(PX4_VehicleCommandMsg_t), TRUE);

        	cmd.Timestamp.Seconds = 0;
        	cmd.Timestamp.Subseconds = 0;
        	cmd.Param1 = 0.0f;
        	cmd.Param2 = NAN;
        	cmd.Param3 = NAN;
        	cmd.Param4 = NAN;
        	cmd.Param5 = NAN;
        	cmd.Param6 = NAN;
        	cmd.Param7 = NAN;
        	cmd.Command = PX4_VEHICLE_CMD_COMPONENT_ARM_DISARM;
        	cmd.TargetSystem = 1;
        	cmd.TargetComponent = 1;
        	cmd.SourceSystem = 0;
        	cmd.SourceComponent = 0;
        	cmd.Confirmation = 0;

			PX4BR_RouteMessageToPX4(&cmd);
            break;
        }

        case PX4BR_VEHICLE_RTL_CC:
        {
        	PX4_VehicleCommandMsg_t cmd;

        	CFE_SB_InitMsg(&cmd, PX4_VEHICLE_COMMAND_MID, sizeof(PX4_VehicleCommandMsg_t), TRUE);

        	cmd.Timestamp.Seconds = 0;
        	cmd.Timestamp.Subseconds = 0;
        	cmd.Param1 = 0.0f;
        	cmd.Param2 = NAN;
        	cmd.Param3 = NAN;
        	cmd.Param4 = NAN;
        	cmd.Param5 = NAN;
        	cmd.Param6 = NAN;
        	cmd.Param7 = NAN;
        	cmd.Command = PX4_VEHICLE_CMD_COMPONENT_ARM_DISARM;
        	cmd.TargetSystem = 1;
        	cmd.TargetComponent = 1;
        	cmd.SourceSystem = 0;
        	cmd.SourceComponent = 0;
        	cmd.Confirmation = 0;

			PX4BR_RouteMessageToPX4(&cmd);
            break;
        }

        case PX4BR_VEHICLE_INVERTED_FLIGHT_CC:
        {
        	PX4_VehicleCommandMsg_t cmd;

        	CFE_SB_InitMsg(&cmd, PX4_VEHICLE_COMMAND_MID, sizeof(PX4_VehicleCommandMsg_t), TRUE);

        	cmd.Timestamp.Seconds = 0;
        	cmd.Timestamp.Subseconds = 0;
        	cmd.Param1 = 0.0f;
        	cmd.Param2 = NAN;
        	cmd.Param3 = NAN;
        	cmd.Param4 = NAN;
        	cmd.Param5 = NAN;
        	cmd.Param6 = NAN;
        	cmd.Param7 = NAN;
        	cmd.Command = PX4_VEHICLE_CMD_COMPONENT_ARM_DISARM;
        	cmd.TargetSystem = 1;
        	cmd.TargetComponent = 1;
        	cmd.SourceSystem = 0;
        	cmd.SourceComponent = 0;
        	cmd.Confirmation = 0;

			PX4BR_RouteMessageToPX4(&cmd);
            break;
        }

        case PX4BR_VEHICLE_SET_MODE_CC:
        {
        	PX4_VehicleCommandMsg_t cmd;
        	PX4_VehicleModeChangeCmd_t *cmdIn = (PX4_VehicleModeChangeCmd_t*)msg;

        	CFE_SB_InitMsg(&cmd, PX4_VEHICLE_COMMAND_MID, sizeof(PX4_VehicleCommandMsg_t), TRUE);

        	cmd.Timestamp.Seconds = 0;
        	cmd.Timestamp.Subseconds = 0;
        	cmd.Param1 = (float)cmdIn->BaseMode;
        	cmd.Param2 = (float)cmdIn->CustomMainMode;
        	cmd.Param3 = (float)cmdIn->CustomSubMode;
        	cmd.Param4 = NAN;
        	cmd.Param5 = NAN;
        	cmd.Param6 = NAN;
        	cmd.Param7 = NAN;
        	cmd.Command = PX4_VEHICLE_CMD_DO_SET_MODE;
        	cmd.TargetSystem = 1;
        	cmd.TargetComponent = 1;
        	cmd.SourceSystem = 0;
        	cmd.SourceComponent = 0;
        	cmd.Confirmation = 0;

			PX4BR_RouteMessageToPX4(&cmd);
            break;
        }

        case PX4BR_MANUAL_CONTROL_SETPOINT_CC:
        {
        	PX4_ManualControlSetpointMsg_t cmdOut;
        	PX4_ManualControlSetpointCmd_t *cmdIn = (PX4_ManualControlSetpointCmd_t*)msg;

        	CFE_SB_InitMsg(&cmdOut, PX4_MANUAL_CONTROL_SETPOINT_MID, sizeof(PX4_ManualControlSetpointMsg_t), TRUE);

        	cmdOut.Timestamp.Seconds = 0;
        	cmdOut.Timestamp.Subseconds = 0;
        	if(cmdIn->InputMask & PX4_MANUAL_INPUT_MASK_X)
        	{
        		cmdOut.X = cmdIn->X / 1000.0;
        	}
        	else
        	{
        		cmdOut.X = NAN;
        	}

        	if(cmdIn->InputMask & PX4_MANUAL_INPUT_MASK_Y)
        	{
        		cmdOut.Y = cmdIn->Y / 1000.0;
        	}
        	else
        	{
        		cmdOut.Y = NAN;
        	}

        	if(cmdIn->InputMask & PX4_MANUAL_INPUT_MASK_Z)
        	{
        		cmdOut.Z = cmdIn->Z / 1000.0;
        	}
        	else
        	{
        		cmdOut.Z = NAN;
        	}

        	if(cmdIn->InputMask & PX4_MANUAL_INPUT_MASK_RC_FAILSAFE)
        	{
        		cmdOut.R = cmdIn->RcFailsafe / 1000.0;
        	}
        	else
        	{
        		cmdOut.R = NAN;
        	}

        	if(cmdIn->InputMask & PX4_MANUAL_INPUT_MASK_FLAPS)
        	{
        		cmdOut.Flaps = cmdIn->Flaps / 1000.0;
        	}
        	else
        	{
        		cmdOut.Flaps = NAN;
        	}

        	if(cmdIn->InputMask & PX4_MANUAL_INPUT_MASK_AUX1)
        	{
        		cmdOut.Aux1 = cmdIn->Aux1 / 1000.0;
        	}
        	else
        	{
        		cmdOut.Aux1 = NAN;
        	}

        	if(cmdIn->InputMask & PX4_MANUAL_INPUT_MASK_AUX2)
        	{
        		cmdOut.Aux2 = cmdIn->Aux2 / 1000.0;
        	}
        	else
        	{
        		cmdOut.Aux2 = NAN;
        	}

        	if(cmdIn->InputMask & PX4_MANUAL_INPUT_MASK_AUX3)
        	{
        		cmdOut.Aux3 = cmdIn->Aux3 / 1000.0;
        	}
        	else
        	{
        		cmdOut.Aux3 = NAN;
        	}

        	if(cmdIn->InputMask & PX4_MANUAL_INPUT_MASK_AUX4)
        	{
        		cmdOut.Aux4 = cmdIn->Aux4 / 1000.0;
        	}
        	else
        	{
        		cmdOut.Aux4 = NAN;
        	}

        	if(cmdIn->InputMask & PX4_MANUAL_INPUT_MASK_AUX5)
        	{
        		cmdOut.Aux5 = cmdIn->Aux5 / 1000.0;
        	}
        	else
        	{
        		cmdOut.Aux5 = NAN;
        	}

        	cmdOut.ModeSwitch = cmdIn->ModeSwitch;
        	cmdOut.ReturnSwitch = cmdIn->ReturnSwitch;
        	cmdOut.RattitudeSwitch = cmdIn->RattitudeSwitch;
        	cmdOut.PosctlSwitch = cmdIn->PosctlSwitch;
        	cmdOut.LoiterSwitch = cmdIn->LoiterSwitch;
        	cmdOut.AcroSwitch = cmdIn->AcroSwitch;
        	cmdOut.OffboardSwitch = cmdIn->OffboardSwitch;
        	cmdOut.KillSwitch = cmdIn->KillSwitch;
        	cmdOut.TransitionSwitch = cmdIn->TransitionSwitch;
        	cmdOut.ModeSlot = cmdIn->ModeSlot;

			PX4BR_RouteMessageToPX4(&cmdOut);
            break;
        }

        default:
            /* Unknown command code.  Raise an event. */
            CFE_EVS_SendEvent(
                    PX4BR_FNCODE_ERR_EID,
                    CFE_EVS_ERROR,
                    "Invalid Function Code Rcvd In Ground Command 0x%x",
                    CommandCode);
            bValid = FALSE;
    }

    if (bValid)
    {
        /* The command was valid and successful.  Increment command
         * counter.
         */
        ++PX4BR_AppData.HkMsg.CmdCount;
    }
    else
    {
        /* The command was invalid or failed.  Increment command error
         * counter.
         */
        ++PX4BR_AppData.HkMsg.CmdErrCount;
    }
}



boolean PX4BR_ResetStatus()
{
    /* Clear all counters and report successful command execution. */
    PX4BR_AppData.HkMsg.CmdCount = -1;
    PX4BR_AppData.HkMsg.CmdErrCount = 0;

    return TRUE;
}



boolean PX4BR_SendVehicleCommand(PX4_VehicleCommandCmd_t *cmdMsg)
{
	OS_printf("VehicleCommandCmd_t\n");
	OS_printf("  Param1=%f\n", cmdMsg->Param1);
	OS_printf("  Command=%u\n", cmdMsg->Command);
	OS_printf("  Param2=%u\n", cmdMsg->Param2);
	OS_printf("**********\n");

	PX4_DisplayBuffer((const char*)cmdMsg, sizeof(PX4_VehicleCommandCmd_t));

	return TRUE;
}



#ifdef __cplusplus
}
#endif
