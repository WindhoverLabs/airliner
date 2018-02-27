#include "vm_Main.h"
#include "vm_events.h"
#include "vm_app.h"

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
    CFE_EVS_SendEvent(VM_MAIN_ENTERED_MANUAL_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::Manual");
}


void VM_Main::EnteredAltitudeControl()
{
    CFE_EVS_SendEvent(VM_MAIN_ENTERED_ALTITUDE_CONTROL_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::AltitudeControl");
}


void VM_Main::EnteredPositionControl()
{
    CFE_EVS_SendEvent(VM_MAIN_ENTERED_POSITION_CONTROL_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::PositionControl");
}


void VM_Main::EnteredAutoMission()
{
    CFE_EVS_SendEvent(VM_MAIN_ENTERED_AUTO_MISSION_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::AutoMission");
}


void VM_Main::EnteredAutoLoiter()
{
    CFE_EVS_SendEvent(VM_MAIN_ENTERED_AUTO_LOITER_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::AutoLoiter");
}


void VM_Main::EnteredAutoLand()
{
    CFE_EVS_SendEvent(VM_MAIN_ENTERED_AUTO_LAND_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::EnteredAutoLand");
}


void VM_Main::EnteredAutoTakeoff()
{
    CFE_EVS_SendEvent(VM_MAIN_ENTERED_AUTO_TAKEOFF_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::AutoTakeoff");
}


void VM_Main::EnteredAutoReturnToLaunch()
{
    CFE_EVS_SendEvent(VM_MAIN_ENTERED_AUTO_RTL_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::AutoReturnToLaunch");
}


void VM_Main::EnteredAutoFollowTarget()
{
    CFE_EVS_SendEvent(VM_MAIN_ENTERED_AUTO_FOLLOW_TARGET_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::AutoFollowTarget");
}


void VM_Main::EnteredAutoPrecland()
{
    CFE_EVS_SendEvent(VM_MAIN_ENTERED_AUTO_PRECLAND_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::AutoPrecland");
}


void VM_Main::EnteredAcrobatic()
{
    CFE_EVS_SendEvent(VM_MAIN_ENTERED_ACROBATIC_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::Acrobatic");
}


void VM_Main::EnteredOffboard()
{
    CFE_EVS_SendEvent(VM_MAIN_ENTERED_OFFBOARD_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::Offboard");
}


void VM_Main::EnteredRattitude()
{
    CFE_EVS_SendEvent(VM_MAIN_ENTERED_RATTITUDE_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::Rattitude");
}


void VM_Main::EnteredStabilize()
{
    CFE_EVS_SendEvent(VM_MAIN_ENTERED_STABILIZE_INFO_EID, CFE_EVS_INFORMATION,
    		"Main::Stabilize");
}


