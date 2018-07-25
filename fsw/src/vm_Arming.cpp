#include "vm_Arming.h"
#include "vm_events.h"
#include "vm_app.h"
#include <math.h>

#include "px4lib.h"

VM_Arming::VM_Arming(VM &inApp) :
        FSM(*this), App(inApp) {

}

VM_Arming::~VM_Arming() {

}

uint32 VM_Arming::GetCurrentStateID() {
return FSM.getState().getId();
}

void VM_Arming::Init(void) {
App.ActuatorArmedMsg.Timestamp = 0;
App.ActuatorArmedMsg.Armed = false;
App.ActuatorArmedMsg.Prearmed = false;
App.ActuatorArmedMsg.ReadyToArm = false;
App.ActuatorArmedMsg.Lockdown = false;
App.ActuatorArmedMsg.ManualLockdown = false;
App.ActuatorArmedMsg.ForceFailsafe = false;
App.ActuatorArmedMsg.InEscCalibrationMode = false;
App.VehicleStatusMsg.ArmingState = PX4_ARMING_STATE_INIT;

}

void VM_Arming::EnteredStandby() {
App.ActuatorArmedMsg.Armed = false;
App.ActuatorArmedMsg.Prearmed = true;
App.ActuatorArmedMsg.ReadyToArm = true;
App.VehicleStatusMsg.ArmingState = PX4_ARMING_STATE_STANDBY;

CFE_EVS_SendEvent(VM_ARMING_ENTERED_STANDBY_STATE_INFO_EID, CFE_EVS_INFORMATION,
        "Arming::Standby");
}

void VM_Arming::EnteredArmed() {
App.ActuatorArmedMsg.Prearmed = false;
App.ActuatorArmedMsg.ReadyToArm = false;
App.VehicleStatusMsg.ArmingState = PX4_ARMING_STATE_ARMED;
App.ActuatorArmedMsg.Armed = true;
App.VehicleControlModeMsg.Armed = true;
App.SendActuatorArmedMsg();
App.SetHomePosition();

CFE_EVS_SendEvent(VM_ARMING_ENTERED_ARMED_STATE_INFO_EID, CFE_EVS_INFORMATION,
        "Arming::Armed");
}

void VM_Arming::ExitedArmed() {
App.ActuatorArmedMsg.Armed = false;
App.VehicleControlModeMsg.Armed = false;
App.SendActuatorArmedMsg();
}

void VM_Arming::EnteredStandbyError() {
App.VehicleStatusMsg.ArmingState = PX4_ARMING_STATE_STANDBY_ERROR;
App.SendActuatorArmedMsg();
CFE_EVS_SendEvent(VM_ARMING_ENTERED_STANDBY_ERROR_STATE_INFO_EID,
CFE_EVS_INFORMATION, "Arming::StandbyError");
}

void VM_Arming::EnteredArmedError() {
App.VehicleStatusMsg.ArmingState = PX4_ARMING_STATE_ARMED_ERROR;
App.SendActuatorArmedMsg();
CFE_EVS_SendEvent(VM_ARMING_ENTERED_ARMED_ERROR_STATE_INFO_EID,
CFE_EVS_INFORMATION, "Arming::ArmedError");
}

void VM_Arming::DoAction() {
if (strcmp(FSM.getState().getName(),"VM_ArmingMap::Init") == 0) {
    /* Keep sending this message */
    App.SendActuatorArmedMsg();
} else if (strcmp(FSM.getState().getName(),"VM_ArmingMap::Standby") == 0) {
    /* Keep sending this message */
    App.SendActuatorArmedMsg();

} else if (strcmp(FSM.getState().getName(),"VM_ArmingMap::Armed") == 0) {
    /* TODO */

} else if (strcmp(FSM.getState().getName(),"VM_ArmingMap::StandbyError") == 0) {
    /* TODO */
} else {
CFE_EVS_SendEvent(VM_IN_UNKNOWN_STATE_ERR_EID, CFE_EVS_ERROR,
        "VM_ArmingMap is in unknown state (%u, '%s')", FSM.getState().getId(), FSM.getState().getName());
}

}

boolean VM_Arming::PreFlightCheckCleared() {
boolean battery_ok = true;
boolean safety_off = true;
boolean sensors_ok = true;


/* Warn about global position */
if (App.VehicleGlobalPositionMsg.Timestamp == 0) {
    (void) CFE_EVS_SendEvent(VM_NOPE_ERR_EID, CFE_EVS_ERROR,
                            "WARN! - Position estimator not initialized");
}


/* Battery warning check */
if (App.BatteryStatusMsg.Warning
    >= PX4_BatteryWarningSeverity_t::PX4_BATTERY_WARNING_LOW) {
CFE_EVS_SendEvent(VM_PRE_ARM_BAT_CHECK_INFO_EID, CFE_EVS_ERROR,
        "Low battery, cannot arm");
battery_ok = false;
}

/* Safety check */
if (App.SafetyMsg.SafetySwitchAvailable && !App.SafetyMsg.SafetyOff) {
CFE_EVS_SendEvent(VM_PRE_ARM_SAFETY_CHECK_INFO_EID, CFE_EVS_ERROR,
        "Safety is ON, cannot arm");
safety_off = false;
}

/* Sensors check */
if (!App.status_flags.condition_system_sensors_initialized) {
CFE_EVS_SendEvent(VM_PRE_ARM_SENSORS_CHECK_INFO_EID, CFE_EVS_ERROR,
        "Sensors not setup correctly");
sensors_ok = false;
}

boolean flight_ok = (battery_ok && safety_off && sensors_ok);

return flight_ok;
}

