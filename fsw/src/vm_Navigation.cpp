#include "vm_Navigation.h"
#include "vm_events.h"
#include "vm_app.h"

typedef enum {
    VM_NAVSM_MANUAL_STATE,
    VM_NAVSM_ALTCTL,
    VM_NAVSM_POSCTL,
	VM_NAVSM_AUTO_RTL,
    VM_NAVSM_AUTO_LOITER,
    VM_NAVSM_ACRO,
    VM_NAVSM_STAB,
    VM_NAVSM_RATTITUDE,
    VM_NAVSM_AUTO_TAKEOFF,
    VM_NAVSM_AUTO_LAND,
} VM_NavSM_StateType;


VM_Navigation::VM_Navigation(VM &inApp) :
    FSM(*this),
    App(inApp)
{

}


VM_Navigation::~VM_Navigation()
{

}


uint32 VM_Navigation::GetCurrentStateID()
{
	return FSM.getState().getId();
}


void VM_Navigation::EnteredManual()
{
	App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_MANUAL;
	App.VehicleStatusMsg.NavState = PX4_NavigationState_t::PX4_NAVIGATION_STATE_MANUAL;

    CFE_EVS_SendEvent(VM_NAVSM_ENTERED_MANUAL_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::Manual");
    App.SendVehicleStatusMsg();
}


void VM_Navigation::EnteredAltitudeControl()
{
	App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_ALTCTL;
	App.VehicleStatusMsg.NavState = PX4_NavigationState_t::PX4_NAVIGATION_STATE_ALTCTL;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_ALTCTL_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::AltitudeControl");
    App.SendVehicleStatusMsg();
}


void VM_Navigation::EnteredPositionControl()
{
	App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_POSCTL;
	App.VehicleStatusMsg.NavState = PX4_NavigationState_t::PX4_NAVIGATION_STATE_POSCTL;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_POSCTL_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::PositionControl");
    App.SendVehicleStatusMsg();
}

void VM_Navigation::EnteredAutoLoiter()
{
	App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_AUTO_LOITER;
	App.VehicleStatusMsg.NavState = PX4_NavigationState_t::PX4_NAVIGATION_STATE_AUTO_LOITER;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_AUTOLOITER_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::AutoLoiter");
    App.SendVehicleStatusMsg();
}


void VM_Navigation::EnteredAutoReturnToLaunch()
{
	App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_AUTO_RTL;
	App.VehicleStatusMsg.NavState = PX4_NavigationState_t::PX4_NAVIGATION_STATE_AUTO_RTL;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_RTL_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::AutoRTL");
    App.SendVehicleStatusMsg();
}

void VM_Navigation::EnteredAcrobatic()
{
	App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_ACRO;
	App.VehicleStatusMsg.NavState = PX4_NAVIGATION_STATE_ACRO;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_ACRO_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::Acrobatic");
    App.SendVehicleStatusMsg();
}


void VM_Navigation::EnteredStabilize()
{
	App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_STAB;
	App.VehicleStatusMsg.NavState = PX4_NAVIGATION_STATE_STAB;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_STABILIZE_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::Stabilize");
    App.SendVehicleStatusMsg();
}


void VM_Navigation::EnteredRattitude()
{
	App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_RATTITUDE;
	App.VehicleStatusMsg.NavState = PX4_NAVIGATION_STATE_RATTITUDE;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_RATTITUDE_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::Rattitude");
    App.SendVehicleStatusMsg();
}


void VM_Navigation::EnteredAutoTakeoff()
{
	App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_AUTO_TAKEOFF;
	App.VehicleStatusMsg.NavState = PX4_NavigationState_t::PX4_NAVIGATION_STATE_AUTO_TAKEOFF;

    //App.SetHomePosition();
    //App.SendHomePositionMsg();

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_AUTO_TAKEOFF_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::AutoTakeoff");
    App.SendVehicleStatusMsg();
}


void VM_Navigation::EnteredAutoLand()
{
	App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_AUTO_LAND;
	App.VehicleStatusMsg.NavState = PX4_NavigationState_t::PX4_NAVIGATION_STATE_AUTO_LAND;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_AUTO_LAND_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::AutoLand");
    App.SendVehicleStatusMsg();
}





void VM_Navigation::DoAction()
{
	/* TODO - Replace these next two lines with the correct code. */
	App.VehicleControlModeMsg.SystemHilEnabled = false;
	App.VehicleControlModeMsg.ExternalManualOverrideOk = false;

	App.VehicleControlModeMsg.ControlOffboardEnabled = false;

	App.VehicleControlModeMsg.ControlManualEnabled = false;
	App.VehicleControlModeMsg.ControlAutoEnabled = true;
	App.VehicleControlModeMsg.ControlRatesEnabled = IsStabilizationRequired();
	App.VehicleControlModeMsg.ControlAttitudeEnabled = IsStabilizationRequired();
	App.VehicleControlModeMsg.ControlRattitudeEnabled = false;
	App.VehicleControlModeMsg.ControlAltitudeEnabled = true;
	App.VehicleControlModeMsg.ControlClimbRateEnabled = true;
	App.VehicleControlModeMsg.ControlPositionEnabled = true;
	App.VehicleControlModeMsg.ControlVelocityEnabled = true;
	App.VehicleControlModeMsg.ControlAccelerationEnabled = false;
	App.VehicleControlModeMsg.ControlTerminationEnabled = false;

	switch(FSM.getState().getId())
	{
		case VM_NavSM_StateType::VM_NAVSM_MANUAL_STATE:
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

		case VM_NavSM_StateType::VM_NAVSM_ALTCTL:
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

		case VM_NavSM_StateType::VM_NAVSM_POSCTL:
			App.VehicleControlModeMsg.ControlManualEnabled = true;
			App.VehicleControlModeMsg.ControlAutoEnabled = false;
			App.VehicleControlModeMsg.ControlRatesEnabled = true;
			App.VehicleControlModeMsg.ControlAttitudeEnabled = true;
			App.VehicleControlModeMsg.ControlRattitudeEnabled = false;
			App.VehicleControlModeMsg.ControlAltitudeEnabled = true;
			App.VehicleControlModeMsg.ControlClimbRateEnabled = true;
			/* TODO - Replace the next 2 lines with the correct code. */
			App.VehicleControlModeMsg.ControlPositionEnabled = true;
			App.VehicleControlModeMsg.ControlVelocityEnabled = true;
			App.VehicleControlModeMsg.ControlAccelerationEnabled = false;
			App.VehicleControlModeMsg.ControlTerminationEnabled = false;
			break;

		/* Fallthru */
		case VM_NavSM_StateType::VM_NAVSM_AUTO_RTL:

		/* Continue the fallthru */

		case VM_NavSM_StateType::VM_NAVSM_AUTO_LAND:
		case VM_NavSM_StateType::VM_NAVSM_AUTO_LOITER:
		case VM_NavSM_StateType::VM_NAVSM_AUTO_TAKEOFF:
			App.VehicleControlModeMsg.ControlManualEnabled = false;
			App.VehicleControlModeMsg.ControlAutoEnabled = true;
			App.VehicleControlModeMsg.ControlRatesEnabled = true;
			App.VehicleControlModeMsg.ControlAttitudeEnabled = true;
			App.VehicleControlModeMsg.ControlRattitudeEnabled = false;
			App.VehicleControlModeMsg.ControlAltitudeEnabled = true;
			App.VehicleControlModeMsg.ControlClimbRateEnabled = true;
			/* TODO - Replace the next 2 lines with the correct code. */
			App.VehicleControlModeMsg.ControlPositionEnabled = true;
			App.VehicleControlModeMsg.ControlVelocityEnabled = true;
			App.VehicleControlModeMsg.ControlAccelerationEnabled = false;
			App.VehicleControlModeMsg.ControlTerminationEnabled = false;
			break;



		case VM_NavSM_StateType::VM_NAVSM_ACRO:
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



		case VM_NavSM_StateType::VM_NAVSM_STAB:
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

		case VM_NavSM_StateType::VM_NAVSM_RATTITUDE:
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



		default:
		    CFE_EVS_SendEvent(VM_IN_UNKNOWN_STATE_ERR_EID, CFE_EVS_ERROR,
		    		"VM_NavigationMap is in unknown state (%u, '%s')", FSM.getState().getId(), FSM.getState().getName());
	}

}




/* TODO:  I'll finish this later. */
/*
void VM_Navigation::IsStateMachineSane()
{
	bool isSane = true;
	if(strcmp(FSM.getState().getName(),"VM_NavigationMap::Manual") == 0)
	{
		if(FSM.getState().getId() != VM_NavSM_StateType::VM_NAVSM_MANUAL_STATE)
		{
			isSane = false;
		}
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::AltitudeControl") == 0)
	{
		if(FSM.getState().getId() != VM_NavSM_StateType::VM_NAVSM_MANUAL_STATE)
		{
			isSane = false;
		}
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::PositionControl") == 0)
	{
		if(FSM.getState().getId() != VM_NavSM_StateType::VM_NAVSM_MANUAL_STATE)
		{
			isSane = false;
		}
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::AutoMission") == 0)
	{
		if(FSM.getState().getId() != VM_NavSM_StateType::VM_NAVSM_MANUAL_STATE)
		{
			isSane = false;
		}
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::AutoLoiter") == 0)
	{
		if(FSM.getState().getId() != VM_NavSM_StateType::VM_NAVSM_MANUAL_STATE)
		{
			isSane = false;
		}
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::AutoReturnToLaunch") == 0)
	{
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::AutoRCRecover") == 0)
	{
		if(FSM.getState().getId() != VM_NavSM_StateType::VM_NAVSM_MANUAL_STATE)
		{
			isSane = false;
		}
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::AutoRtgs") == 0)
	{
		if(FSM.getState().getId() != VM_NavSM_StateType::VM_NAVSM_MANUAL_STATE)
		{
			isSane = false;
		}
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::AutoLandEngineFail") == 0)
	{
		if(FSM.getState().getId() != VM_NavSM_StateType::VM_NAVSM_MANUAL_STATE)
		{
			isSane = false;
		}
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::AutoLandGpsFail") == 0)
	{
		if(FSM.getState().getId() != VM_NavSM_StateType::VM_NAVSM_MANUAL_STATE)
		{
			isSane = false;
		}
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::Acrobatic") == 0)
	{
		if(FSM.getState().getId() != VM_NavSM_StateType::VM_NAVSM_MANUAL_STATE)
		{
			isSane = false;
		}
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::Descend") == 0)
	{
		if(FSM.getState().getId() != VM_NavSM_StateType::VM_NAVSM_MANUAL_STATE)
		{
			isSane = false;
		}
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::Termination") == 0)
	{
		if(FSM.getState().getId() != VM_NavSM_StateType::VM_NAVSM_MANUAL_STATE)
		{
			isSane = false;
		}
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::Offboard") == 0)
	{
		if(FSM.getState().getId() != VM_NavSM_StateType::VM_NAVSM_MANUAL_STATE)
		{
			isSane = false;
		}
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::Stabilize") == 0)
	{
		if(FSM.getState().getId() != VM_NavSM_StateType::VM_NAVSM_MANUAL_STATE)
		{
			isSane = false;
		}
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::Rattitude") == 0)
	{
		if(FSM.getState().getId() != VM_NavSM_StateType::VM_NAVSM_MANUAL_STATE)
		{
			isSane = false;
		}
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::AutoTakeoff") == 0)
	{
		if(FSM.getState().getId() != VM_NavSM_StateType::VM_NAVSM_MANUAL_STATE)
		{
			isSane = false;
		}
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::AutoLand") == 0)
	{
		if(FSM.getState().getId() != VM_NavSM_StateType::VM_NAVSM_MANUAL_STATE)
		{
			isSane = false;
		}
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::AutoFollowTarget") == 0)
	{
		if(FSM.getState().getId() != VM_NavSM_StateType::VM_NAVSM_MANUAL_STATE)
		{
			isSane = false;
		}
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::AutoPrecland") == 0)
	{
		if(FSM.getState().getId() != VM_NavSM_StateType::VM_NAVSM_MANUAL_STATE)
		{
			isSane = false;
		}
	}
	else
	{
	    CFE_EVS_SendEvent(VM_IN_UNKNOWN_STATE_ERR_EID, CFE_EVS_ERROR,
	    		"VM_NavigationMap is in unknown state (%u, '%s')", FSM.getState().getId(), FSM.getState().getName());
	}
}
*/


boolean VM_Navigation::IsAllMessagesReady(){

	boolean validity = false;

	boolean SensorMagMsgReady = (App.SensorMagMsg.Timestamp > 0);
	boolean SensorGyroMsgReady = (App.SensorGyroMsg.Timestamp > 0);
	boolean SensorAccelMsgReady = (App.SensorAccelMsg.Timestamp > 0);
	boolean SensorCombinedMsgReady = (App.SensorCombinedMsg.Timestamp > 0);
	boolean VehicleAttitudeMsgReady = (App.VehicleAttitudeMsg.Timestamp > 0);
	boolean VehicleLocalPositionMsg = (App.VehicleLocalPositionMsg.Timestamp > 0);
	boolean VehicleLandDetectedMsgReady = (App.VehicleLandDetectedMsg.Timestamp > 0);
	boolean VehicleGlobalPositionMsgReady = (App.VehicleGlobalPositionMsg.Timestamp > 0);
	boolean VehicleGpsPositionMsgReady = (App.VehicleGpsPositionMsg.Timestamp > 0);

	if (SensorMagMsgReady
			&& SensorGyroMsgReady
			&& SensorAccelMsgReady
			&& SensorCombinedMsgReady
			&& VehicleAttitudeMsgReady
			&& VehicleLocalPositionMsg
			&& VehicleLandDetectedMsgReady
			&& VehicleGlobalPositionMsgReady
			&& VehicleGpsPositionMsgReady){
		validity = true;

	}

	return validity;

}

boolean VM_Navigation::IsStabilizationRequired(void)
{
	/* TODO */
	return true;
}

boolean VM_Navigation::IsTransitionAltCtlValid(void){

	boolean validity = false;
	PX4_NavigationState_t Current_NavState = App.VehicleStatusMsg.NavState;

	/* Altitude Hold Requirement Validation */
	if(App.VehicleLocalPositionMsg.Timestamp > 0 && App.VehicleLocalPositionMsg.Z_Valid && App.VehicleLocalPositionMsg.V_Z_Valid){

		validity = true;
	}

	return validity;
}

boolean VM_Navigation::IsTransitionPosCtlValid(void){

	boolean validity = false;
	PX4_NavigationState_t Current_NavState = App.VehicleStatusMsg.NavState;

	/* Altitude Hold Requirement Validation */
	if(App.VehicleLocalPositionMsg.Timestamp > 0
	   && App.VehicleLocalPositionMsg.XY_Valid
	   && App.VehicleLocalPositionMsg.V_XY_Valid
	   && App.VehicleLocalPositionMsg.Z_Valid
	   && App.VehicleLocalPositionMsg.V_Z_Valid){

		validity = true;
	}

	return validity;
}

boolean VM_Navigation::IsTransitionAcrobaticValid(void){

	boolean validity = false;
	PX4_NavigationState_t Current_NavState = App.VehicleStatusMsg.NavState;

	/* Altitude Hold Requirement Validation */
	if(App.SensorCombinedMsg.Timestamp > 0
	   && (App.SensorCombinedMsg.MagTimestampRelative!= PX4_RELATIVE_TIMESTAMP_INVALID)
	   && (App.SensorCombinedMsg.AccRelTimeInvalid != PX4_RELATIVE_TIMESTAMP_INVALID)){

		validity = true;
	}

	return validity;
}


boolean VM_Navigation::IsTransitionAutoLoiterValid(void){
	boolean validity = IsAllMessagesReady();
	return validity;

}

boolean VM_Navigation::IsTransitionRtlValid(void){
	boolean validity = IsAllMessagesReady();
	return validity;
}

boolean VM_Navigation::IsTransitionStabilizeValid(void){
	boolean validity = IsAllMessagesReady();
	return validity;
}

boolean VM_Navigation::IsTransitionRattitudeValid(void){
	boolean validity = IsAllMessagesReady();
	return validity;
}

boolean VM_Navigation::IsTransitionAutoTakeoffValid(void){
	boolean validity = IsAllMessagesReady();
	return validity;
}

boolean VM_Navigation::IsTransitionAutoLandValid(void){
	boolean validity = IsAllMessagesReady();
	return validity;
}



boolean VM_Navigation::IsConditionLocalPositionValid()
{
	return App.ConditionLocalPositionValid;
}


boolean VM_Navigation::IsVehicleArmed()
{
	return App.IsVehicleArmed();
}
