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
    CFE_EVS_SendEvent(VM_NAVSM_ENTERED_MANUAL_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::Manual");
}


void VM_Navigation::EnteredAltitudeControl()
{
    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_ALTCTL_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::");
}


void VM_Navigation::EnteredPositionControl()
{
    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_POSCTL_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::PositionControl");
}


void VM_Navigation::EnteredAutoMission()
{
    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_AUTOMISSION_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::AutoMission");
}


void VM_Navigation::EnteredAutoLoiter()
{
    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_AUTOLOITER_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::AutoLoiter");
}


void VM_Navigation::EnteredAutoReturnToLaunch()
{
    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_RTL_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::AutoRTL");
}


void VM_Navigation::EnteredAutoRCRecover()
{
    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_AUTORCRECOVER_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::AutoRcRecover");
}


void VM_Navigation::EnteredAutoRtgs()
{
    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_AUTORTGS_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::AutoRtgs");
}


void VM_Navigation::EnteredAutoLandEngineFail()
{
    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_LAND_ENGINE_FAIL_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::LandEngineFail");
}


void VM_Navigation::EnteredAutoLandGpsFail()
{
    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_LAND_GPS_FAIL_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::LandGpsFail");
}


void VM_Navigation::EnteredAcrobatic()
{
    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_ACRO_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::Acrobatic");
}


void VM_Navigation::EnteredDescend()
{
    CFE_EVS_SendEvent(VM_NAVSN_ENTERED_DESCEND_INFO_EID, CFE_EVS_INFORMATION,
    		"Navigation::Descend");
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
	else
	{
	    CFE_EVS_SendEvent(VM_IN_UNKNOWN_STATE_ERR_EID, CFE_EVS_ERROR,
	    		"VM_NavigationMap is in unknown state (%u, '%s')", FSM.getState().getId(), FSM.getState().getName());
	}
}
