#include "vm_Arming.h"
#include "vm_events.h"

VM_Arming::VM_Arming() :
    FSM(*this)
{

}


VM_Arming::~VM_Arming()
{

}


uint32 VM_Arming::GetCurrentStateID()
{
	return FSM.getState().getId();
}


void VM_Arming::EnteredStandby()
{
    CFE_EVS_SendEvent(VM_ARMING_ENTERED_STANDBY_STATE_INFO_EID, CFE_EVS_INFORMATION,
    		"Arming::Standby");
}


void VM_Arming::EnteredArmed()
{
    CFE_EVS_SendEvent(VM_ARMING_ENTERED_ARMED_STATE_INFO_EID, CFE_EVS_INFORMATION,
    		"Arming::Armed");
}


void VM_Arming::EnteredStandbyError()
{
    CFE_EVS_SendEvent(VM_ARMING_ENTERED_STANDBY_ERROR_STATE_INFO_EID, CFE_EVS_INFORMATION,
    		"Arming::StandbyError");
}


void VM_Arming::EnteredArmedError()
{
    CFE_EVS_SendEvent(VM_ARMING_ENTERED_ARMED_ERROR_STATE_INFO_EID, CFE_EVS_INFORMATION,
    		"Arming::ArmedError");
}

