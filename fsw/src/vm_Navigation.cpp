#include "vm_Navigation.h"
#include "vm_events.h"
#include "vm_app.h"

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
	App.VehicleStatusMsg.NavState = PX4_NAVIGATION_STATE_MANUAL;

    CFE_EVS_SendEvent(VM_NAVSM_ENTERED_MANUAL_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::Manual");
}


void VM_Navigation::EnteredAltitudeControl()
{
	App.VehicleStatusMsg.NavState = PX4_NAVIGATION_STATE_ALTCTL;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_ALTCTL_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::AltitudeControl");
}


void VM_Navigation::EnteredPositionControl()
{
	App.VehicleStatusMsg.NavState = PX4_NAVIGATION_STATE_POSCTL;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_POSCTL_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::PositionControl");
}


void VM_Navigation::EnteredAutoMission()
{
	App.VehicleStatusMsg.NavState = PX4_NAVIGATION_STATE_AUTO_MISSION;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_AUTOMISSION_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::AutoMission");
}


void VM_Navigation::EnteredAutoLoiter()
{
	App.VehicleStatusMsg.NavState = PX4_NAVIGATION_STATE_AUTO_LOITER;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_AUTOLOITER_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::AutoLoiter");
}


void VM_Navigation::EnteredAutoReturnToLaunch()
{
	App.VehicleStatusMsg.NavState = PX4_NAVIGATION_STATE_AUTO_RTL;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_RTL_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::AutoRTL");
}


void VM_Navigation::EnteredAutoRCRecover()
{
	App.VehicleStatusMsg.NavState = PX4_NAVIGATION_STATE_AUTO_RCRECOVER;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_AUTORCRECOVER_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::AutoRcRecover");
}


void VM_Navigation::EnteredAutoRtgs()
{
	App.VehicleStatusMsg.NavState = PX4_NAVIGATION_STATE_AUTO_RTGS;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_AUTORTGS_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::AutoRtgs");
}


void VM_Navigation::EnteredAutoLandEngineFail()
{
	App.VehicleStatusMsg.NavState = PX4_NAVIGATION_STATE_AUTO_LANDENGFAIL;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_LAND_ENGINE_FAIL_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::LandEngineFail");
}


void VM_Navigation::EnteredAutoLandGpsFail()
{
	App.VehicleStatusMsg.NavState = PX4_NAVIGATION_STATE_AUTO_LANDGPSFAIL;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_LAND_GPS_FAIL_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::LandGpsFail");
}


void VM_Navigation::EnteredAcrobatic()
{
	App.VehicleStatusMsg.NavState = PX4_NAVIGATION_STATE_ACRO;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_ACRO_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::Acrobatic");
}


void VM_Navigation::EnteredDescend()
{
	App.VehicleStatusMsg.NavState = PX4_NAVIGATION_STATE_DESCEND;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_DESCEND_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::Descend");
}


void VM_Navigation::EnteredTermination()
{
	App.VehicleStatusMsg.NavState = PX4_NAVIGATION_STATE_TERMINATION;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_TERMINATION_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::Termination");
}


void VM_Navigation::EnteredOffboard()
{
	App.VehicleStatusMsg.NavState = PX4_NAVIGATION_STATE_OFFBOARD;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_OFFBOARD_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::Offboard");
}


void VM_Navigation::EnteredStabilize()
{
	App.VehicleStatusMsg.NavState = PX4_NAVIGATION_STATE_STAB;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_STABILIZE_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::Stabilize");
}


void VM_Navigation::EnteredRattitude()
{
	App.VehicleStatusMsg.NavState = PX4_NAVIGATION_STATE_RATTITUDE;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_RATTITUDE_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::Rattitude");
}


void VM_Navigation::EnteredAutoTakeoff()
{
	App.VehicleStatusMsg.NavState = PX4_NAVIGATION_STATE_AUTO_TAKEOFF;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_AUTO_TAKEOFF_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::AutoTakeoff");
}


void VM_Navigation::EnteredAutoLand()
{
	App.VehicleStatusMsg.NavState = PX4_NAVIGATION_STATE_AUTO_LAND;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_AUTO_LAND_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::AutoLand");
}


void VM_Navigation::EnteredAutoFollowTarget()
{
	App.VehicleStatusMsg.NavState = PX4_NAVIGATION_STATE_AUTO_FOLLOW_TARGET;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_AUTO_FOLLOW_TARGET_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::AutoFollowTarget");
}


void VM_Navigation::EnteredAutoPrecland()
{
	/* TODO:  What is the enumeration for this state? */
	App.VehicleStatusMsg.NavState = (PX4_NavigationState_t)999;

    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_AUTO_PRECLAND_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::AutoPrecland");
}




void VM_Navigation::DoAction()
{
	if(strcmp(FSM.getState().getName(),"VM_NavigationMap::Manual") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::AltitudeControl") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::PositionControl") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::AutoMission") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::AutoLoiter") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::AutoReturnToLaunch") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::AutoRCRecover") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::AutoRtgs") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::AutoLandEngineFail") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::AutoLandGpsFail") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::Acrobatic") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::Descend") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::Termination") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::Offboard") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::Stabilize") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::Rattitude") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::AutoTakeoff") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::AutoLand") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::AutoFollowTarget") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_NavigationMap::AutoPrecland") == 0)
	{
		/* TODO */
	}
	else
	{
	    CFE_EVS_SendEvent(VM_IN_UNKNOWN_STATE_ERR_EID, CFE_EVS_ERROR,
	    		"VM_NavigationMap is in unknown state (%u, '%s')", FSM.getState().getId(), FSM.getState().getName());
	}
}
