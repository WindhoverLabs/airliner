#include "vm_Arming.h"
#include "vm_events.h"
#include "vm_app.h"
#include "px4lib.h"

VM_Arming::VM_Arming(VM &inApp) :
    FSM(*this),
	App(inApp)
{

}


VM_Arming::~VM_Arming()
{

}


uint32 VM_Arming::GetCurrentStateID()
{
	return FSM.getState().getId();
}


void VM_Arming::EnteredInit(void)
{
    App.ActuatorArmedMsg.Timestamp = 0;
    App.ActuatorArmedMsg.Armed = false;
    App.ActuatorArmedMsg.Prearmed = false;
    App.ActuatorArmedMsg.ReadyToArm = false;
    App.ActuatorArmedMsg.Lockdown = false;
    App.ActuatorArmedMsg.ManualLockdown = false;
    App.ActuatorArmedMsg.ForceFailsafe = false;
    App.ActuatorArmedMsg.InEscCalibrationMode = false;
}


void VM_Arming::EnteredStandby()
{
    App.ActuatorArmedMsg.Armed = false;
    App.ActuatorArmedMsg.Prearmed = true;
    App.ActuatorArmedMsg.ReadyToArm = true;

    CFE_EVS_SendEvent(VM_ARMING_ENTERED_STANDBY_STATE_INFO_EID, CFE_EVS_INFORMATION,
    		"Arming::Standby");
}


void VM_Arming::EnteredArmed()
{
    App.ActuatorArmedMsg.Armed = true;

    CFE_EVS_SendEvent(VM_ARMING_ENTERED_ARMED_STATE_INFO_EID, CFE_EVS_INFORMATION,
    		"Arming::Armed");
}


void VM_Arming::ExitedArmed()
{
    App.ActuatorArmedMsg.Armed = false;
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


void VM_Arming::DoAction()
{
	if(strcmp(FSM.getState().getName(),"VM_ArmingMap::Init") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_ArmingMap::Standby") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_ArmingMap::Armed") == 0)
	{
		/* TODO */
	}
	else if(strcmp(FSM.getState().getName(),"VM_ArmingMap::StandbyError") == 0)
	{
		/* TODO */
	}
	else
	{
	    CFE_EVS_SendEvent(VM_IN_UNKNOWN_STATE_ERR_EID, CFE_EVS_ERROR,
	    		"VM_ArmingMap is in unknown state (%u, '%s')", FSM.getState().getId(), FSM.getState().getName());
	}

	App.ActuatorArmedMsg.Timestamp = PX4LIB_GetPX4TimeUs();
	App.SendActuatorArmedMsg();
}
