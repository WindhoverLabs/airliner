#include "vm_Main.h"
#include "vm_events.h"
#include "vm_app.h"
#include <math.h>
#include "px4lib.h"

VM_Main::VM_Main(VM &inApp) :
    FSM(*this),
    App(inApp)
{

}


VM_Main::~VM_Main()
{

}


uint32 VM_Main::GetCurrentStateID()
{
	return FSM.getState().getId();
}


void VM_Main::EnteredManual()
{
	App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_MANUAL;

    CFE_EVS_SendEvent(VM_MAIN_ENTERED_MANUAL_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::Manual");
}


void VM_Main::EnteredAltitudeControl()
{
	App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_ALTCTL;

    CFE_EVS_SendEvent(VM_MAIN_ENTERED_ALTITUDE_CONTROL_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::AltitudeControl");
}


void VM_Main::EnteredPositionControl()
{
	App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_POSCTL;

    CFE_EVS_SendEvent(VM_MAIN_ENTERED_POSITION_CONTROL_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::PositionControl");
}


void VM_Main::EnteredAutoMission()
{
	App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_AUTO_MISSION;

    CFE_EVS_SendEvent(VM_MAIN_ENTERED_AUTO_MISSION_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::AutoMission");
}


void VM_Main::EnteredAutoLoiter()
{
	App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_AUTO_LOITER;

    CFE_EVS_SendEvent(VM_MAIN_ENTERED_AUTO_LOITER_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::AutoLoiter");
}


void VM_Main::EnteredAutoLand()
{
	App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_AUTO_LAND;

    CFE_EVS_SendEvent(VM_MAIN_ENTERED_AUTO_LAND_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::EnteredAutoLand");
}


void VM_Main::EnteredAutoTakeoff()
{
//	//PX4_VehicleCommandMsg_t cmd;
//
//
//	//CFE_SB_InitMsg(&cmd, PX4_VEHICLE_COMMAND_MID, sizeof(cmd), TRUE);
//
//	App.VehicleCommandMsg.Timestamp = PX4LIB_GetPX4TimeUs();
//	App.VehicleCommandMsg.Param1 = NAN;
//	App.VehicleCommandMsg.Param2 = NAN;
//	App.VehicleCommandMsg.Param3 = NAN;
//	App.VehicleCommandMsg.Param4 = NAN;
//	App.VehicleCommandMsg.Param5 = NAN;
//	App.VehicleCommandMsg.Param6 = NAN;
//	App.VehicleCommandMsg.Param7 = NAN;
//	App.VehicleCommandMsg.Command = PX4_VehicleCmd_t::PX4_VEHICLE_CMD_NAV_TAKEOFF;
//	App.VehicleCommandMsg.TargetSystem = 1;
//	App.VehicleCommandMsg.TargetComponent = 1;
//	App.VehicleCommandMsg.SourceSystem = 0;
//	App.VehicleCommandMsg.SourceComponent = 0;
//	App.VehicleCommandMsg.Confirmation = 0;
//	OS_printf("command Prepared\n");
//	App.SendVehicleCommandMsg();
//    //CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&cmd);
//    //CFE_SB_SendMsg((CFE_SB_Msg_t*)&cmd);
//
//
//    //App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_AUTO_TAKEOFF;
//    OS_printf("SET NAVSTATE:\n");
//
//
//
//
//	/* Update the VehicleStatus message */
//
//    App.VehicleControlModeMsg.ControlVelocityEnabled = true;
//    App.VehicleControlModeMsg.ControlPositionEnabled = true;
////	/* Update the CommanderState message */
////	VehicleManagerStateMsg.Timestamp = timestamp;
////
////	/* Update the VehicleControlMode message */
////	VehicleControlModeMsg.Timestamp = timestamp;
////
////
//
//
//    App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_AUTO_TAKEOFF;
//    App.VehicleStatusMsg.NavState = PX4_NavigationState_t::PX4_NAVIGATION_STATE_AUTO_TAKEOFF;

    CFE_EVS_SendEvent(VM_MAIN_ENTERED_AUTO_TAKEOFF_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::AutoTakeoff");
}


void VM_Main::EnteredAutoReturnToLaunch()
{
	App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_AUTO_RTL;

    CFE_EVS_SendEvent(VM_MAIN_ENTERED_AUTO_RTL_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::AutoReturnToLaunch");
}


void VM_Main::EnteredAutoFollowTarget()
{
	App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_AUTO_FOLLOW_TARGET;

    CFE_EVS_SendEvent(VM_MAIN_ENTERED_AUTO_FOLLOW_TARGET_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::AutoFollowTarget");
}


void VM_Main::EnteredAutoPrecland()
{
	App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_AUTO_PRECLAND;

    CFE_EVS_SendEvent(VM_MAIN_ENTERED_AUTO_PRECLAND_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::AutoPrecland");
}


void VM_Main::EnteredAcrobatic()
{
	App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_ACRO;

    CFE_EVS_SendEvent(VM_MAIN_ENTERED_ACROBATIC_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::Acrobatic");
}


void VM_Main::EnteredOffboard()
{
	App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_OFFBOARD;

    CFE_EVS_SendEvent(VM_MAIN_ENTERED_OFFBOARD_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::Offboard");
}


void VM_Main::EnteredRattitude()
{
	App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_RATTITUDE;

    CFE_EVS_SendEvent(VM_MAIN_ENTERED_RATTITUDE_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::Rattitude");
}


void VM_Main::EnteredStabilize()
{
	App.VehicleManagerStateMsg.MainState = PX4_COMMANDER_MAIN_STATE_STAB;

    CFE_EVS_SendEvent(VM_MAIN_ENTERED_STABILIZE_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::Stabilize");
}


void VM_Main::DoAction()
{
	if(strcmp(FSM.getState().getName(),"VM_MainMap::Manual") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_MainMap::AltitudeControl") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_MainMap::PositionControl") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_MainMap::AutoMission") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_MainMap::AutoLoiter") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_MainMap::AutoReturnToLaunch") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_MainMap::Acrobatic") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_MainMap::Offboard") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_MainMap::Stabilize") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_MainMap::Rattitude") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_MainMap::AutoTakeoff") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_MainMap::AutoLand") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_MainMap::AutoFollowTarget") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_MainMap::AutoPrecland") == 0)
	{
		/* TODO */
	}
	else
	{
	    CFE_EVS_SendEvent(VM_IN_UNKNOWN_STATE_ERR_EID, CFE_EVS_ERROR,
	    		"VM_MainMap is in unknown state (%u, '%s')", FSM.getState().getId(), FSM.getState().getName());
	}
}


boolean VM_Main::IsConditionLocalPositionValid()
{
	return App.ConditionLocalPositionValid;
}


boolean VM_Main::IsVehicleArmed()
{
	return App.IsVehicleArmed();
}
