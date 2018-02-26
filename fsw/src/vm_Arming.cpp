#include "vm_Arming.h"
#include "vm_events.h"

VM_Arming::VM_Arming() :
    _fsm(*this)
{

}


VM_Arming::~VM_Arming()
{

}


uint32 VM_Arming::GetCurrentStateID()
{
	return _fsm.getState().getId();
}


void VM_Arming::DisplayCurrentState()
{
	//getIndex
	//VM_ArmingState *currentState = _fsm.getState();
	OS_printf("Current state = %s\n", _fsm.getState().getName());
}


void VM_Arming::Arm()
{
	_fsm.Arm();
}


void VM_Arming::Disarm()
{
	_fsm.Disarm();
}


void VM_Arming::InitComplete()
{
	_fsm.InitComplete();
}


void VM_Arming::EnteredStandby()
{
    CFE_EVS_SendEvent(VM_ENTERED_STANDBY_STATE_INFO_EID, CFE_EVS_INFORMATION,
    		"Arming::Standby");
}


void VM_Arming::EnteredArmed()
{
    CFE_EVS_SendEvent(VM_ENTERED_ARMED_STATE_INFO_EID, CFE_EVS_INFORMATION,
    		"Arming::Armed");
}


void VM_Arming::EnteredStandbyError()
{
    CFE_EVS_SendEvent(VM_ENTERED_STANDBY_ERROR_STATE_INFO_EID, CFE_EVS_INFORMATION,
    		"Arming::StandbyError");
}


void VM_Arming::EnteredArmedError()
{
    CFE_EVS_SendEvent(VM_ENTERED_ARMED_ERROR_STATE_INFO_EID, CFE_EVS_INFORMATION,
    		"Arming::ArmedError");
}

