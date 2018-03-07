#include "vm_Arming.h"
#include "vm_events.h"
#include "vm_app.h"
#include <math.h>

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


void VM_Arming::Init(void)
{
    App.ActuatorArmedMsg.Timestamp = 0;
    App.ActuatorArmedMsg.Armed = false;
    App.ActuatorArmedMsg.Prearmed = false;
    App.ActuatorArmedMsg.ReadyToArm = false;
    App.ActuatorArmedMsg.Lockdown = false;
    App.ActuatorArmedMsg.ManualLockdown = false;
    App.ActuatorArmedMsg.ForceFailsafe = false;
    App.ActuatorArmedMsg.InEscCalibrationMode = false;
	App.VehicleStatusMsg.ArmingState = PX4_ARMING_STATE_INIT;
    App.VehicleStatusMsg.SystemID = 1;
    App.VehicleStatusMsg.ComponentID = 1;
	//App.SendActuatorArmedMsg();
}


void VM_Arming::EnteredStandby()
{
    App.ActuatorArmedMsg.Armed = false;
    App.ActuatorArmedMsg.Prearmed = true;
    App.ActuatorArmedMsg.ReadyToArm = true;
	App.VehicleStatusMsg.ArmingState = PX4_ARMING_STATE_STANDBY;
	//App.SendActuatorArmedMsg();

    CFE_EVS_SendEvent(VM_ARMING_ENTERED_STANDBY_STATE_INFO_EID, CFE_EVS_INFORMATION,
    		"Arming::Standby");
}


void VM_Arming::EnteredArmed()
{
    App.ActuatorArmedMsg.Prearmed = false;
    App.ActuatorArmedMsg.ReadyToArm = false;
    App.VehicleStatusMsg.ArmingState = PX4_ARMING_STATE_ARMED;
    App.ActuatorArmedMsg.Armed = true;
    App.VehicleControlModeMsg.Armed = true;
	App.SendActuatorArmedMsg();

    CFE_EVS_SendEvent(VM_ARMING_ENTERED_ARMED_STATE_INFO_EID, CFE_EVS_INFORMATION,
    		"Arming::Armed");
}


void VM_Arming::ExitedArmed()
{
    App.ActuatorArmedMsg.Armed = false;
    App.VehicleControlModeMsg.Armed = false;
	App.SendActuatorArmedMsg();
}


void VM_Arming::EnteredStandbyError()
{
	App.VehicleStatusMsg.ArmingState = PX4_ARMING_STATE_STANDBY_ERROR;
	App.SendActuatorArmedMsg();
    CFE_EVS_SendEvent(VM_ARMING_ENTERED_STANDBY_ERROR_STATE_INFO_EID, CFE_EVS_INFORMATION,
    		"Arming::StandbyError");
}


void VM_Arming::EnteredArmedError()
{
	App.VehicleStatusMsg.ArmingState = PX4_ARMING_STATE_ARMED_ERROR;
	App.SendActuatorArmedMsg();
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
	App.SendActuatorArmedMsg();

}

boolean VM_Arming::PreFlightCheckCleared(){
	boolean battery_ok = true;
	boolean safety_off = true;
	boolean power_ok  = true;
	boolean sensors_ok = true;
	float AvionicsPowerRailVoltage = 4.5f;//App.SystemPowerMsg.Voltage5V;

	/* Battery Warning Check */
	if(App.BatteryStatusMsg.Warning >= PX4_BatteryWarningSeverity_t::PX4_BATTERY_WARNING_LOW ){
		OS_printf("LOW BATTERY CANNOT ARM \n");
		battery_ok = false;
	}

	/* Safety Check */
	if(App.SafetyMsg.SafetySwitchAvailable && !App.SafetyMsg.SafetyOff){
		OS_printf("SAFETY ON CANNOT ARM \n");
		safety_off = false;
	}

	/* Power check */
	if(!App.status_flags.condition_power_input_valid){
		OS_printf("Connect power module\n");
		power_ok = false;
	}
	/* Check avionics rail voltages */
	if(AvionicsPowerRailVoltage<4.5f){
		OS_printf("NOT ARMING: Avionics power low: %6.2f Volt\n",AvionicsPowerRailVoltage);
		power_ok = false;
	}
	else if(AvionicsPowerRailVoltage<4.9f){
		OS_printf("CAUTION: Avionics power low: %6.2f Volt\n",AvionicsPowerRailVoltage);

	}else if(AvionicsPowerRailVoltage>5.4f){
		OS_printf("CAUTION: Avionics power high: %6.2f Volt\n",AvionicsPowerRailVoltage);
	}

	if (!App.status_flags.condition_system_sensors_initialized) {
		OS_printf("Sensors not set up correctly \n");
		sensors_ok = false;
	}
/*TODO:QAE HK*/


	return (battery_ok && safety_off && power_ok && sensors_ok);
}


