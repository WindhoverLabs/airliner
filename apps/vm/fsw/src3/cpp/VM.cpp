#include "../src/VM.h"

VM::VM() {
	// TODO - implement VM::VM
	throw "Not yet implemented";
}

VM::~VM() {
	// TODO - implement VM::~VM
	throw "Not yet implemented";
}

void VM::AppMain(void unnamed_1) {
	// TODO - implement VM::AppMain
	throw "Not yet implemented";
}

int32 VM::InitApp(void unnamed_1) {
	// TODO - implement VM::InitApp
	throw "Not yet implemented";
}

int32 VM::InitParams(void unnamed_1) {
	// TODO - implement VM::InitParams
	throw "Not yet implemented";
}

int32 VM::InitEvent(void unnamed_1) {
	// TODO - implement VM::InitEvent
	throw "Not yet implemented";
}

void VM::InitData(void unnamed_1) {
	// TODO - implement VM::InitData
	throw "Not yet implemented";
}

int32 VM::InitPipe(void unnamed_1) {
	// TODO - implement VM::InitPipe
	throw "Not yet implemented";
}

int32 VM::RcvSchPipeMsg(int32 iBlocking) {
	// TODO - implement VM::RcvSchPipeMsg
	throw "Not yet implemented";
}

void VM::ProcessDataPipe(void unnamed_1) {
	// TODO - implement VM::ProcessDataPipe
	throw "Not yet implemented";
}

void VM::ProcessCmdPipe(void unnamed_1) {
	// TODO - implement VM::ProcessCmdPipe
	throw "Not yet implemented";
}

void VM::ProcessAppCmds(CFE_SB_Msg_t* MsgPtr) {
	// TODO - implement VM::ProcessAppCmds
	throw "Not yet implemented";
}

void VM::ReportHousekeeping(void unnamed_1) {
	// TODO - implement VM::ReportHousekeeping
	throw "Not yet implemented";
}

void VM::SendActuatorArmedMsg(void unnamed_1) {
	// TODO - implement VM::SendActuatorArmedMsg
	throw "Not yet implemented";
}

void VM::SendHomePositionMsg(void unnamed_1) {
	// TODO - implement VM::SendHomePositionMsg
	throw "Not yet implemented";
}

void VM::SendVehicleManagerStateMsg(void unnamed_1) {
	// TODO - implement VM::SendVehicleManagerStateMsg
	throw "Not yet implemented";
}

void VM::SendMissionMsg(void unnamed_1) {
	// TODO - implement VM::SendMissionMsg
	throw "Not yet implemented";
}

void VM::SendLedControlMsg(void unnamed_1) {
	// TODO - implement VM::SendLedControlMsg
	throw "Not yet implemented";
}

void VM::SendVehicleStatusMsg(void unnamed_1) {
	// TODO - implement VM::SendVehicleStatusMsg
	throw "Not yet implemented";
}

void VM::SendVehicleControlModeMsg(void unnamed_1) {
	// TODO - implement VM::SendVehicleControlModeMsg
	throw "Not yet implemented";
}

void VM::SendVehicleCommandMsg(void unnamed_1) {
	// TODO - implement VM::SendVehicleCommandMsg
	throw "Not yet implemented";
}

boolean VM::VerifyCmdLength(CFE_SB_Msg_t* MsgPtr, uint16 usExpectedLen) {
	// TODO - implement VM::VerifyCmdLength
	throw "Not yet implemented";
}

uint64 VM::TimeElapsed(uint64* TimePtr) {
	// TODO - implement VM::TimeElapsed
	throw "Not yet implemented";
}

uint64 VM::TimeNow(void unnamed_1) {
	// TODO - implement VM::TimeNow
	throw "Not yet implemented";
}

osalbool VM::IsVehicleArmed(void unnamed_1) {
	// TODO - implement VM::IsVehicleArmed
	throw "Not yet implemented";
}

void VM::FlightSessionInit(void unnamed_1) {
	// TODO - implement VM::FlightSessionInit
	throw "Not yet implemented";
}

void VM::SetHomePosition(void unnamed_1) {
	// TODO - implement VM::SetHomePosition
	throw "Not yet implemented";
}

void VM::RcModes(void unnamed_1) {
	// TODO - implement VM::RcModes
	throw "Not yet implemented";
}

void VM::Initialization(void unnamed_1) {
	// TODO - implement VM::Initialization
	throw "Not yet implemented";
}

void VM::Execute(void unnamed_1) {
	// TODO - implement VM::Execute
	throw "Not yet implemented";
}

const char* VM::GetNavStateAsString(uint32 unnamed_1) {
	// TODO - implement VM::GetNavStateAsString
	throw "Not yet implemented";
}

void VM::UpdateParamsFromTable(void unnamed_1) {
	// TODO - implement VM::UpdateParamsFromTable
	throw "Not yet implemented";
}

int32 VM::InitConfigTbl(void unnamed_1) {
	// TODO - implement VM::InitConfigTbl
	throw "Not yet implemented";
}

int32 VM::AcquireConfigPointers(void unnamed_1) {
	// TODO - implement VM::AcquireConfigPointers
	throw "Not yet implemented";
}

void VM::ReportConfiguration(void unnamed_1) {
	// TODO - implement VM::ReportConfiguration
	throw "Not yet implemented";
}

static int32 VM::ValidateConfigTbl(void* unnamed_1) {
	// TODO - implement VM::ValidateConfigTbl
	throw "Not yet implemented";
}

static osalbool VM::Validate_COM_RC_IN_MODE(uint32 param) {
	// TODO - implement VM::Validate_COM_RC_IN_MODE
	throw "Not yet implemented";
}

static osalbool VM::Validate_COM_ARM_SWISBTN(uint32 param) {
	// TODO - implement VM::Validate_COM_ARM_SWISBTN
	throw "Not yet implemented";
}

static osalbool VM::Validate_COM_RC_ARM_HYST(uint32 param) {
	// TODO - implement VM::Validate_COM_RC_ARM_HYST
	throw "Not yet implemented";
}

static osalbool VM::Validate_MAV_SYS_ID(uint32 param) {
	// TODO - implement VM::Validate_MAV_SYS_ID
	throw "Not yet implemented";
}

static osalbool VM::Validate_MAV_COMP_ID(uint32 param) {
	// TODO - implement VM::Validate_MAV_COMP_ID
	throw "Not yet implemented";
}

static osalbool VM::Validate_COM_RC_LOSS_T(float param) {
	// TODO - implement VM::Validate_COM_RC_LOSS_T
	throw "Not yet implemented";
}

static osalbool VM::Validate_COM_LOW_BAT_ACT(uint32 param) {
	// TODO - implement VM::Validate_COM_LOW_BAT_ACT
	throw "Not yet implemented";
}

static osalbool VM::Validate_COM_HOME_H_T(float param) {
	// TODO - implement VM::Validate_COM_HOME_H_T
	throw "Not yet implemented";
}

static osalbool VM::Validate_COM_HOME_V_T(float param) {
	// TODO - implement VM::Validate_COM_HOME_V_T
	throw "Not yet implemented";
}

osalbool VM::onParamValidate(void* Address, uint32 Value) {
	// TODO - implement VM::onParamValidate
	throw "Not yet implemented";
}

osalbool VM::onParamValidate(void* Address, float Value) {
	// TODO - implement VM::onParamValidate
	throw "Not yet implemented";
}
