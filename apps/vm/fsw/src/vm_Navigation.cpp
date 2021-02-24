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

#include "vm_Navigation.h"
#include "vm_events.h"
#include "vm_app.h"
#include "px4lib_msgids.h"
#include "cfs_utils.h"

typedef enum
{
    VM_NAVSM_MANUAL_STATE = 0,
    VM_NAVSM_ALTCTL       = 1,
    VM_NAVSM_POSCTL       = 2,
    VM_NAVSM_AUTO_RTL     = 3,
    VM_NAVSM_AUTO_LOITER  = 4,
    VM_NAVSM_ACRO         = 5,
    VM_NAVSM_STAB         = 6,
    VM_NAVSM_RATTITUDE    = 7,
    VM_NAVSM_AUTO_TAKEOFF = 8,
    VM_NAVSM_AUTO_LAND    = 9
} VM_NavSM_StateType;



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  VM_Navigation non-default constructor                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
VM_Navigation::VM_Navigation(VM &inApp) :
        FSM(*this), App(inApp)
{

}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  VM_Navigation destructor                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
VM_Navigation::~VM_Navigation()
{

}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  GetCurrentStateID Function                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
uint32 VM_Navigation::GetCurrentStateID()
{
	uint32 id = FSM.getState().getId();

	return id;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  EnteredManual Function                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM_Navigation::EnteredManual()
{
    App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_MANUAL;
    App.VehicleStatusMsg.NavState = PX4_NavigationState_t::PX4_NAVIGATION_STATE_MANUAL;

    CFE_EVS_SendEvent(VM_NAVSM_ENTERED_MANUAL_INFO_EID, CFE_EVS_INFORMATION, "Navigation::Manual");
    App.SendVehicleStatusMsg();
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  EnteredAltitudeControl Function                                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM_Navigation::EnteredAltitudeControl()
{
    App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_ALTCTL;
    App.VehicleStatusMsg.NavState = PX4_NavigationState_t::PX4_NAVIGATION_STATE_ALTCTL;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_ALTCTL_INFO_EID, CFE_EVS_INFORMATION, "Navigation::AltitudeControl");
    App.SendVehicleStatusMsg();
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  EnteredPositionControl Function                                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM_Navigation::EnteredPositionControl()
{
    App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_POSCTL;
    App.VehicleStatusMsg.NavState = PX4_NavigationState_t::PX4_NAVIGATION_STATE_POSCTL;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_POSCTL_INFO_EID, CFE_EVS_INFORMATION,
            "Navigation::PositionControl");
    App.SendVehicleStatusMsg();
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  EnteredAutoLoiter Function                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM_Navigation::EnteredAutoLoiter()
{
    App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_AUTO_LOITER;
    App.VehicleStatusMsg.NavState = PX4_NavigationState_t::PX4_NAVIGATION_STATE_AUTO_LOITER;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_AUTOLOITER_INFO_EID, CFE_EVS_INFORMATION,
            "Navigation::AutoLoiter");
    App.SendVehicleStatusMsg();
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  EnteredAutoReturnToLaunch Function                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM_Navigation::EnteredAutoReturnToLaunch()
{
    App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_AUTO_RTL;
    App.VehicleStatusMsg.NavState = PX4_NavigationState_t::PX4_NAVIGATION_STATE_AUTO_RTL;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_RTL_INFO_EID, CFE_EVS_INFORMATION,
        "Navigation::AutoRTL");
    App.SendVehicleStatusMsg();
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  EnteredAcrobatic Function                                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM_Navigation::EnteredAcrobatic()
{
    App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_ACRO;
    App.VehicleStatusMsg.NavState = PX4_NAVIGATION_STATE_ACRO;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_ACRO_INFO_EID, CFE_EVS_INFORMATION,
        "Navigation::Acrobatic");
    App.SendVehicleStatusMsg();
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  EnteredStabilize Function                                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM_Navigation::EnteredStabilize()
{
    App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_STAB;
    App.VehicleStatusMsg.NavState = PX4_NAVIGATION_STATE_STAB;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_STABILIZE_INFO_EID, CFE_EVS_INFORMATION,
        "Navigation::Stabilize");
    App.SendVehicleStatusMsg();
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  EnteredRattitude Function                                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM_Navigation::EnteredRattitude()
{
    App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_RATTITUDE;
    App.VehicleStatusMsg.NavState = PX4_NAVIGATION_STATE_RATTITUDE;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_RATTITUDE_INFO_EID, CFE_EVS_INFORMATION,
        "Navigation::Rattitude");
    App.SendVehicleStatusMsg();
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  EnteredAutoTakeoff Function                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM_Navigation::EnteredAutoTakeoff()
{
    App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_AUTO_TAKEOFF;
    App.VehicleStatusMsg.NavState = PX4_NavigationState_t::PX4_NAVIGATION_STATE_AUTO_TAKEOFF;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_AUTO_TAKEOFF_INFO_EID, CFE_EVS_INFORMATION,
        "Navigation::AutoTakeoff");
    App.SendVehicleStatusMsg();
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  EnteredAutoLand Function                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM_Navigation::EnteredAutoLand()
{
    App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_AUTO_LAND;
    App.VehicleStatusMsg.NavState = PX4_NavigationState_t::PX4_NAVIGATION_STATE_AUTO_LAND;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_AUTO_LAND_INFO_EID, CFE_EVS_INFORMATION,
        "Navigation::AutoLand");
    App.SendVehicleStatusMsg();
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  DoAction Function                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VM_Navigation::DoAction()
{
    /* Set common fields in vehicle control mode message */
    App.VehicleControlModeMsg.Armed = App.VehicleStatusMsg.ArmingState;
    App.VehicleControlModeMsg.SystemHilEnabled = false;
    App.VehicleControlModeMsg.ExternalManualOverrideOk = false;
    App.VehicleControlModeMsg.ControlOffboardEnabled = false;

    switch (FSM.getState().getId())
    {
        case VM_NavSM_StateType::VM_NAVSM_MANUAL_STATE:
        {
            App.VehicleControlModeMsg.ControlManualEnabled = true;
            App.VehicleControlModeMsg.ControlAutoEnabled = false;
            App.VehicleControlModeMsg.ControlRatesEnabled = IsStabilizationRequired();
            App.VehicleControlModeMsg.ControlAttitudeEnabled = IsStabilizationRequired();
            App.VehicleControlModeMsg.ControlRattitudeEnabled = false;
            App.VehicleControlModeMsg.ControlAltitudeEnabled = false;
            App.VehicleControlModeMsg.ControlClimbRateEnabled = false;
            App.VehicleControlModeMsg.ControlPositionEnabled = false;
            App.VehicleControlModeMsg.ControlVelocityEnabled = false;
            App.VehicleControlModeMsg.ControlAccelerationEnabled = false;
            App.VehicleControlModeMsg.ControlTerminationEnabled = false;
            break;
        }

        case VM_NavSM_StateType::VM_NAVSM_ALTCTL:
        {
            App.VehicleControlModeMsg.ControlManualEnabled = true;
            App.VehicleControlModeMsg.ControlAutoEnabled = false;
            App.VehicleControlModeMsg.ControlRatesEnabled = true;
            App.VehicleControlModeMsg.ControlAttitudeEnabled = true;
            App.VehicleControlModeMsg.ControlRattitudeEnabled = false;
            App.VehicleControlModeMsg.ControlAltitudeEnabled = true;
            App.VehicleControlModeMsg.ControlClimbRateEnabled = true;
            App.VehicleControlModeMsg.ControlPositionEnabled = false;
            App.VehicleControlModeMsg.ControlVelocityEnabled = false;
            App.VehicleControlModeMsg.ControlAccelerationEnabled = false;
            App.VehicleControlModeMsg.ControlTerminationEnabled = false;
            break;
        }

        case VM_NavSM_StateType::VM_NAVSM_POSCTL:
        {
            App.VehicleControlModeMsg.ControlManualEnabled = true;
            App.VehicleControlModeMsg.ControlAutoEnabled = false;
            App.VehicleControlModeMsg.ControlRatesEnabled = true;
            App.VehicleControlModeMsg.ControlAttitudeEnabled = true;
            App.VehicleControlModeMsg.ControlRattitudeEnabled = false;
            App.VehicleControlModeMsg.ControlAltitudeEnabled = true;
            App.VehicleControlModeMsg.ControlClimbRateEnabled = true;
            App.VehicleControlModeMsg.ControlPositionEnabled = !App.VehicleStatusMsg.InTransitionMode;
            App.VehicleControlModeMsg.ControlVelocityEnabled = !App.VehicleStatusMsg.InTransitionMode;
            App.VehicleControlModeMsg.ControlAccelerationEnabled = false;
            App.VehicleControlModeMsg.ControlTerminationEnabled = false;
            break;
        }

        /* Fall through */
        case VM_NavSM_StateType::VM_NAVSM_AUTO_RTL:
        case VM_NavSM_StateType::VM_NAVSM_AUTO_LAND:
        case VM_NavSM_StateType::VM_NAVSM_AUTO_LOITER:
        case VM_NavSM_StateType::VM_NAVSM_AUTO_TAKEOFF:
        {
            App.VehicleControlModeMsg.ControlManualEnabled = false;
            App.VehicleControlModeMsg.ControlAutoEnabled = true;
            App.VehicleControlModeMsg.ControlRatesEnabled = true;
            App.VehicleControlModeMsg.ControlAttitudeEnabled = true;
            App.VehicleControlModeMsg.ControlRattitudeEnabled = false;
            App.VehicleControlModeMsg.ControlAltitudeEnabled = true;
            App.VehicleControlModeMsg.ControlClimbRateEnabled = true;
            App.VehicleControlModeMsg.ControlPositionEnabled = !App.VehicleStatusMsg.InTransitionMode;
            App.VehicleControlModeMsg.ControlVelocityEnabled = !App.VehicleStatusMsg.InTransitionMode;
            App.VehicleControlModeMsg.ControlAccelerationEnabled = false;
            App.VehicleControlModeMsg.ControlTerminationEnabled = false;
            break;
        }

        case VM_NavSM_StateType::VM_NAVSM_ACRO:
        {
            App.VehicleControlModeMsg.ControlManualEnabled = true;
            App.VehicleControlModeMsg.ControlAutoEnabled = false;
            App.VehicleControlModeMsg.ControlRatesEnabled = true;
            App.VehicleControlModeMsg.ControlAttitudeEnabled = false;
            App.VehicleControlModeMsg.ControlRattitudeEnabled = false;
            App.VehicleControlModeMsg.ControlAltitudeEnabled = false;
            App.VehicleControlModeMsg.ControlClimbRateEnabled = false;
            App.VehicleControlModeMsg.ControlPositionEnabled = false;
            App.VehicleControlModeMsg.ControlVelocityEnabled = false;
            App.VehicleControlModeMsg.ControlAccelerationEnabled = false;
            App.VehicleControlModeMsg.ControlTerminationEnabled = false;
            break;
        }

        case VM_NavSM_StateType::VM_NAVSM_STAB:
        {
            App.VehicleControlModeMsg.ControlManualEnabled = true;
            App.VehicleControlModeMsg.ControlAutoEnabled = false;
            App.VehicleControlModeMsg.ControlRatesEnabled = true;
            App.VehicleControlModeMsg.ControlAttitudeEnabled = true;
            App.VehicleControlModeMsg.ControlRattitudeEnabled = false;
            App.VehicleControlModeMsg.ControlAltitudeEnabled = false;
            App.VehicleControlModeMsg.ControlClimbRateEnabled = false;
            App.VehicleControlModeMsg.ControlPositionEnabled = false;
            App.VehicleControlModeMsg.ControlVelocityEnabled = false;
            App.VehicleControlModeMsg.ControlAccelerationEnabled = false;
            App.VehicleControlModeMsg.ControlTerminationEnabled = false;
            App.VehicleControlModeMsg.ExternalManualOverrideOk = false;
            break;
        }

        case VM_NavSM_StateType::VM_NAVSM_RATTITUDE:
        {
            App.VehicleControlModeMsg.ControlManualEnabled = true;
            App.VehicleControlModeMsg.ControlAutoEnabled = false;
            App.VehicleControlModeMsg.ControlRatesEnabled = true;
            App.VehicleControlModeMsg.ControlAttitudeEnabled = true;
            App.VehicleControlModeMsg.ControlRattitudeEnabled = true;
            App.VehicleControlModeMsg.ControlAltitudeEnabled = false;
            App.VehicleControlModeMsg.ControlClimbRateEnabled = false;
            App.VehicleControlModeMsg.ControlPositionEnabled = false;
            App.VehicleControlModeMsg.ControlVelocityEnabled = false;
            App.VehicleControlModeMsg.ControlAccelerationEnabled = false;
            App.VehicleControlModeMsg.ControlTerminationEnabled = false;
            break;
        }

        default:
        {
            CFE_EVS_SendEvent(VM_IN_UNKNOWN_STATE_ERR_EID, CFE_EVS_ERROR,
                    "VM_NavigationMap is in unknown state (%u, '%s')",
                    FSM.getState().getId(),
                    FSM.getState().getName());
            break;
        }
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  AllMessagesReceivedAtLeastOnce Function                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool VM_Navigation::AllMessagesReceivedAtLeastOnce()
{
    osalbool validity = false;

    osalbool SensorMagMsgReady = !CFE_SB_IsMsgTimeZero((CFE_SB_MsgPtr_t)&App.SensorMagMsg);
    osalbool SensorGyroMsgReady = !CFE_SB_IsMsgTimeZero((CFE_SB_MsgPtr_t)&App.SensorGyroMsg);
    osalbool SensorAccelMsgReady = !CFE_SB_IsMsgTimeZero((CFE_SB_MsgPtr_t)&App.SensorAccelMsg);
    osalbool SensorCombinedMsgReady = (App.SensorCombinedMsg.Timestamp > 0);
    osalbool VehicleAttitudeMsgReady = (App.VehicleAttitudeMsg.Timestamp > 0);
    osalbool VehicleLocalPositionMsgReady = (App.VehicleLocalPositionMsg.Timestamp > 0);
    osalbool VehicleLandDetectedMsgReady = (App.VehicleLandDetectedMsg.Timestamp > 0);
    osalbool VehicleGlobalPositionMsgReady = (App.VehicleGlobalPositionMsg.Timestamp > 0);
    osalbool VehicleGpsPositionMsgReady = (App.VehicleGpsPositionMsg.Timestamp > 0);

    if (SensorMagMsgReady && SensorGyroMsgReady && SensorAccelMsgReady
        && SensorCombinedMsgReady && VehicleAttitudeMsgReady
        && VehicleLocalPositionMsgReady && VehicleLandDetectedMsgReady
        && VehicleGlobalPositionMsgReady && VehicleGpsPositionMsgReady)
    {
        validity = true;
    }

    if (!validity)
    {
        /* Send event */
        CFE_EVS_SendEvent(VM_SEN_NOT_READY_INFO_EID, CFE_EVS_INFORMATION,
            "Sensors not ready (SM=%u SG=%u SA=%u SC=%u VA=%u VLP=%u VLD=%u VGP=%u VGPSP=%u)\n",
			SensorMagMsgReady,
			SensorGyroMsgReady,
			SensorAccelMsgReady,
			SensorCombinedMsgReady,
			VehicleAttitudeMsgReady,
			VehicleLocalPositionMsgReady,
			VehicleLandDetectedMsgReady,
			VehicleGlobalPositionMsgReady,
			VehicleGpsPositionMsgReady);
    }

    return validity;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  IsStabilizationRequired Function                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool VM_Navigation::IsStabilizationRequired(void)
{
    /* Being a rotary wing qualifies for stabilization */
    return true;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  IsTransitionAltCtlValid Function                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool VM_Navigation::IsTransitionAltCtlValid(void)
{
    osalbool validity = false;
    PX4_NavigationState_t Current_NavState = App.VehicleStatusMsg.NavState;

    /* Altitude Hold Requirement Validation */
    if (App.VehicleLocalPositionMsg.Timestamp > 0
        && App.VehicleLocalPositionMsg.Z_Valid
        && App.VehicleLocalPositionMsg.V_Z_Valid)
    {
        validity = true;
    }

    if (!validity)
    {
        /* Send event */
        CFE_EVS_SendEvent(VM_REQ_ALT_CTL_ERR_EID, CFE_EVS_ERROR,
            "Altitude hold mode requirement failed");
    }

    return validity;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  IsTransitionPosCtlValid Function                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool VM_Navigation::IsTransitionPosCtlValid(void)
{
    osalbool validity = false;
    PX4_NavigationState_t Current_NavState = App.VehicleStatusMsg.NavState;

    /* Position Hold Requirement Validation */
    if (App.VehicleLocalPositionMsg.Timestamp > 0
        && App.VehicleLocalPositionMsg.XY_Valid
        && App.VehicleLocalPositionMsg.V_XY_Valid
        && App.VehicleLocalPositionMsg.Z_Valid
        && App.VehicleLocalPositionMsg.V_Z_Valid)
    {
        validity = true;
    }

    if (!validity)
    {
        /* Send event */
        CFE_EVS_SendEvent(VM_REQ_POS_CTL_ERR_EID, CFE_EVS_ERROR,
                "Position hold mode requirement failed");
    }

    return validity;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  IsTransitionAcrobaticValid Function                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool VM_Navigation::IsTransitionAcrobaticValid(void)
{
    osalbool validity = true;
    PX4_NavigationState_t Current_NavState = App.VehicleStatusMsg.NavState;

    /* Altitude Hold Requirement Validation */
    osalbool SensorCombinedMsgReady = !CFE_SB_IsMsgTimeZero((CFE_SB_MsgPtr_t)&App.SensorCombinedMsg);
    osalbool SensorMagMsgReady = !CFE_SB_IsMsgTimeZero((CFE_SB_MsgPtr_t)&App.SensorMagMsg);

    if(CFE_SB_IsMsgTimeZero((CFE_SB_MsgPtr_t)&App.SensorCombinedMsg))
    {
        CFE_EVS_SendEvent(VM_REQ_ACRO_ERR_EID, CFE_EVS_ERROR,
            "Acrobatic mode requirement failed. SensorCombinedMsg not received.");
        validity = false;
    }

    if(App.SensorCombinedMsg.MagInvalid)
    {
        CFE_EVS_SendEvent(VM_REQ_ACRO_ERR_EID, CFE_EVS_ERROR,
            "Acrobatic mode requirement failed. Mag invalid.");
        validity = false;
    }

    if(App.SensorCombinedMsg.AccInvalid)
    {
        CFE_EVS_SendEvent(VM_REQ_ACRO_ERR_EID, CFE_EVS_ERROR,
            "Acrobatic mode requirement failed. Acc invalid.");
        validity = false;
    }

    return validity;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  IsTransitionAutoLoiterValid Function                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool VM_Navigation::IsTransitionAutoLoiterValid(void)
{
    osalbool validity = AllMessagesReceivedAtLeastOnce();
    return validity;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  IsTransitionRtlValid Function                                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool VM_Navigation::IsTransitionRtlValid(void)
{
    osalbool validity = AllMessagesReceivedAtLeastOnce();
    return validity;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  IsTransitionStabilizeValid Function                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool VM_Navigation::IsTransitionStabilizeValid(void)
{
    osalbool validity = AllMessagesReceivedAtLeastOnce();
    return validity;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  IsTransitionRattitudeValid Function                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool VM_Navigation::IsTransitionRattitudeValid(void)
{
    osalbool validity = AllMessagesReceivedAtLeastOnce();
    return validity;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  IsTransitionAutoTakeoffValid Function                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool VM_Navigation::IsTransitionAutoTakeoffValid(void)
{
    osalbool validity = AllMessagesReceivedAtLeastOnce();
    return validity;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  IsTransitionAutoLandValid Function                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool VM_Navigation::IsTransitionAutoLandValid(void)
{
    osalbool validity = AllMessagesReceivedAtLeastOnce();
    return validity;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  IsLocalPositionIsValid Function                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool VM_Navigation::IsLocalPositionIsValid()
{
    return App.HkTlm.LocalPositionIsValid;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  IsVehicleArmed Function                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool VM_Navigation::IsVehicleArmed()
{
    return App.IsVehicleArmed();
}
