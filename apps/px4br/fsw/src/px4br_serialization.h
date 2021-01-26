/*
 * px4br_serialization.h
 *
 *  Created on: Oct 22, 2016
 *      Author: mbenson
 */

#ifndef SRC_PX4BR_SERIALIZATION_H_
#define SRC_PX4BR_SERIALIZATION_H_

#include "cfe.h"
#include "px4_msgs.h"

/* Serialization Functions */
uint32 PX4BR_ActuatorArmed_Enc(const PX4_ActuatorArmedMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_ActuatorControls_Enc(const PX4_ActuatorControlsMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_ActuatorDirect_Enc(const PX4_ActuatorDirectMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_ActuatorOutputs_Enc(const PX4_ActuatorOutputsMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_AdcReport_Enc(const PX4_AdcReportMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_Airspeed_Enc(const PX4_AirspeedMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_AttPosMoCap_Enc(const PX4_AttPosMoCapMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_BatteryStatus_Enc(const PX4_BatteryStatusMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_CameraTrigger_Enc(const PX4_CameraTriggerMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_CommanderState_Enc(const PX4_CommanderStateMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_ControlState_Enc(const PX4_ControlStateMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_CpuLoad_Enc(const PX4_CpuLoadMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_DebugKeyValue_Enc(const PX4_DebugKeyValueMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_DifferentialPressure_Enc(const PX4_DifferentialPressureMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_DistanceSensor_Enc(const PX4_DistanceSensorMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_FwPosCtrlStatus_Enc(const PX4_FwPosCtrlStatusMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_FwVirtualAttitudeSetpoint_Enc(const PX4_FwVirtualAttitudeSetpointMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_FwVirtualRatesSetpoint_Enc(const PX4_FwVirtualRatesSetpointMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_Ekf2Innovations_Enc(const PX4_Ekf2InnovationsMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_Ekf2Replay_Enc(const PX4_Ekf2ReplayMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_EscReport_Enc(const PX4_EscReportMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_EscStatus_Enc(const PX4_EscStatusMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_EstimatorStatus_Enc(const PX4_EstimatorStatusMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_Fence_Enc(const PX4_FenceMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_FenceVertex_Enc(const PX4_FenceVertexMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_FilteredBottomFlow_Enc(const PX4_FilteredBottomFlowMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_FollowTarget_Enc(const PX4_FollowTargetMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_GeofenceResult_Enc(const PX4_GeofenceResultMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_GpsDump_Enc(const PX4_GpsDumpMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_GpsInjectData_Enc(const PX4_GpsInjectDataMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_HilSensor_Enc(const PX4_HilSensorMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_HomePosition_Enc(const PX4_HomePositionMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_InputRc_Enc(const PX4_InputRcMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_LogMessage_Enc(const PX4_LogMessageMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_ManualControlSetpoint_Enc(const PX4_ManualControlSetpointMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_MavlinkLog_Enc(const PX4_MavlinkLogMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_McAttCtrlStatus_Enc(const PX4_McAttCtrlStatusMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_McVirtualAttitudeSetpoint_Enc(const PX4_McVirtualAttitudeSetpointMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_McVirtualRatesSetpoint_Enc(const PX4_McVirtualRatesSetpointMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_Mission_Enc(const PX4_MissionMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_MissionResult_Enc(const PX4_MissionResultMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_MultirotorMotorLimits_Enc(const PX4_MultirotorMotorLimitsMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_OffboardControlMode_Enc(const PX4_OffboardControlModeMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_OpticalFlow_Enc(const PX4_OpticalFlowMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_OutputPwm_Enc(const PX4_OutputPwmMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_ParameterUpdate_Enc(const PX4_ParameterUpdateMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_PositionSetpoint_Enc(const PX4_PositionSetpointMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_PositionSetpointTriplet_Enc(const PX4_PositionSetpointTripletMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_PwmInput_Enc(const PX4_PwmInputMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_QShellReq_Enc(const PX4_QShellReqMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_RcChannels_Enc(const PX4_RcChannelsMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_RcParameterMap_Enc(const PX4_RcParameterMapMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_Safety_Enc(const PX4_SafetyMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_SatelliteInfo_Enc(const PX4_SatelliteInfoMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_SensorAccel_Enc(const PX4_SensorAccelMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_SensorBaro_Enc(const PX4_SensorBaroMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_SensorCombined_Enc(const PX4_SensorCombinedMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_SensorGyro_Enc(const PX4_SensorGyroMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_SensorMag_Enc(const PX4_SensorMagMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_ServorailStatus_Enc(const PX4_ServorailStatusMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_SubsystemInfo_Enc(const PX4_SubsystemInfoMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_SystemPower_Enc(const PX4_SystemPowerMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_TecsStatus_Enc(const PX4_TecsStatusMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_TelemetryStatus_Enc(const PX4_TelemetryStatusMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_TestMotor_Enc(const PX4_TestMotorMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_TimeOffset_Enc(const PX4_TimeOffsetMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_TransponderReport_Enc(const PX4_TransponderReportMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_UavCanParameterRequest_Enc(const PX4_UavCanParameterRequestMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_UavCanParameterValue_Enc(const PX4_UavCanParameterValueMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_VehicleAttitude_Enc(const PX4_VehicleAttitudeMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_VehicleAttitudeSetpoint_Enc(const PX4_VehicleAttitudeSetpointMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_VehicleCommandAck_Enc(const PX4_VehicleCommandAckMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_VehicleCommand_Enc(const PX4_VehicleCommandMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_VehicleControlMode_Enc(const PX4_VehicleControlModeMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_VehicleForceSetpoint_Enc(const PX4_VehicleForceSetpointMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_VehicleGlobalPosition_Enc(const PX4_VehicleGlobalPositionMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_VehicleGlobalVelocitySetpoint_Enc(const PX4_VehicleGlobalVelocitySetpointMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_VehicleGpsPosition_Enc(const PX4_VehicleGpsPositionMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_VehicleLandDetected_Enc(const PX4_VehicleLandDetectedMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_VehicleLocalPosition_Enc(const PX4_VehicleLocalPositionMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_VehicleLocalPositionSetpoint_Enc(const PX4_VehicleLocalPositionSetpointMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_VehicleRatesSetpoint_Enc(const PX4_VehicleRatesSetpointMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_VehicleStatus_Enc(const PX4_VehicleStatusMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_VisionPositionEstimate_Enc(const PX4_VisionPositionEstimateMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_VtolVehicleStatus_Enc(const PX4_VtolVehicleStatusMsg_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 PX4BR_WindEstimate_Enc(const PX4_WindEstimateMsg_t *inObject, char *inOutBuffer, uint32 inSize);

/* Deserialization Functions */
uint32 PX4BR_ActuatorArmed_Dec(const char *inBuffer, uint32 inSize, PX4_ActuatorArmedMsg_t *inOutObject);
uint32 PX4BR_ActuatorControls_Dec(const char *inBuffer, uint32 inSize, PX4_ActuatorControlsMsg_t *inOutObject);
uint32 PX4BR_ActuatorDirect_Dec(const char *inBuffer, uint32 inSize, PX4_ActuatorDirectMsg_t *inOutObject);
uint32 PX4BR_ActuatorOutputs_Dec(const char *inBuffer, uint32 inSize, PX4_ActuatorOutputsMsg_t *inOutObject);
uint32 PX4BR_AdcReport_Dec(const char *inBuffer, uint32 inSize, PX4_AdcReportMsg_t *inOutObject);
uint32 PX4BR_Airspeed_Dec(const char *inBuffer, uint32 inSize, PX4_AirspeedMsg_t *inOutObject);
uint32 PX4BR_AttPosMoCap_Dec(const char *inBuffer, uint32 inSize, PX4_AttPosMoCapMsg_t *inOutObject);
uint32 PX4BR_BatteryStatus_Dec(const char *inBuffer, uint32 inSize, PX4_BatteryStatusMsg_t *inOutObject);
uint32 PX4BR_CameraTrigger_Dec(const char *inBuffer, uint32 inSize, PX4_CameraTriggerMsg_t *inOutObject);
uint32 PX4BR_CommanderState_Dec(const char *inBuffer, uint32 inSize, PX4_CommanderStateMsg_t *inOutObject);
uint32 PX4BR_ControlState_Dec(const char *inBuffer, uint32 inSize, PX4_ControlStateMsg_t *inOutObject);
uint32 PX4BR_CpuLoad_Dec(const char *inBuffer, uint32 inSize, PX4_CpuLoadMsg_t *inOutObject);
uint32 PX4BR_DebugKeyValue_Dec(const char *inBuffer, uint32 inSize, PX4_DebugKeyValueMsg_t *inOutObject);
uint32 PX4BR_DifferentialPressure_Dec(const char *inBuffer, uint32 inSize, PX4_DifferentialPressureMsg_t *inOutObject);
uint32 PX4BR_DistanceSensor_Dec(const char *inBuffer, uint32 inSize, PX4_DistanceSensorMsg_t *inOutObject);
uint32 PX4BR_FwPosCtrlStatus_Dec(const char *inBuffer, uint32 inSize, PX4_FwPosCtrlStatusMsg_t *inOutObject);
uint32 PX4BR_FwVirtualAttitudeSetpoint_Dec(const char *inBuffer, uint32 inSize, PX4_FwVirtualAttitudeSetpointMsg_t *inOutObject);
uint32 PX4BR_FwVirtualRatesSetpoint_Dec(const char *inBuffer, uint32 inSize, PX4_FwVirtualRatesSetpointMsg_t *inOutObject);
uint32 PX4BR_Ekf2Innovations_Dec(const char *inBuffer, uint32 inSize, PX4_Ekf2InnovationsMsg_t *inOutObject);
uint32 PX4BR_Ekf2Replay_Dec(const char *inBuffer, uint32 inSize, PX4_Ekf2ReplayMsg_t *inOutObject);
uint32 PX4BR_EscReport_Dec(const char *inBuffer, uint32 inSize, PX4_EscReportMsg_t *inOutObject);
uint32 PX4BR_EscStatus_Dec(const char *inBuffer, uint32 inSize, PX4_EscStatusMsg_t *inOutObject);
uint32 PX4BR_EstimatorStatus_Dec(const char *inBuffer, uint32 inSize, PX4_EstimatorStatusMsg_t *inOutObject);
uint32 PX4BR_Fence_Dec(const char *inBuffer, uint32 inSize, PX4_FenceMsg_t *inOutObject);
uint32 PX4BR_FenceVertex_Dec(const char *inBuffer, uint32 inSize, PX4_FenceVertexMsg_t *inOutObject);
uint32 PX4BR_FilteredBottomFlow_Dec(const char *inBuffer, uint32 inSize, PX4_FilteredBottomFlowMsg_t *inOutObject);
uint32 PX4BR_FollowTarget_Dec(const char *inBuffer, uint32 inSize, PX4_FollowTargetMsg_t *inOutObject);
uint32 PX4BR_GeofenceResult_Dec(const char *inBuffer, uint32 inSize, PX4_GeofenceResultMsg_t *inOutObject);
uint32 PX4BR_GpsDump_Dec(const char *inBuffer, uint32 inSize, PX4_GpsDumpMsg_t *inOutObject);
uint32 PX4BR_GpsInjectData_Dec(const char *inBuffer, uint32 inSize, PX4_GpsInjectDataMsg_t *inOutObject);
uint32 PX4BR_HilSensor_Dec(const char *inBuffer, uint32 inSize, PX4_HilSensorMsg_t *inOutObject);
uint32 PX4BR_HomePosition_Dec(const char *inBuffer, uint32 inSize, PX4_HomePositionMsg_t *inOutObject);
uint32 PX4BR_InputRc_Dec(const char *inBuffer, uint32 inSize, PX4_InputRcMsg_t *inOutObject);
uint32 PX4BR_LogMessage_Dec(const char *inBuffer, uint32 inSize, PX4_LogMessageMsg_t *inOutObject);
uint32 PX4BR_ManualControlSetpoint_Dec(const char *inBuffer, uint32 inSize, PX4_ManualControlSetpointMsg_t *inOutObject);
uint32 PX4BR_MavlinkLog_Dec(const char *inBuffer, uint32 inSize, PX4_MavlinkLogMsg_t *inOutObject);
uint32 PX4BR_McAttCtrlStatus_Dec(const char *inBuffer, uint32 inSize, PX4_McAttCtrlStatusMsg_t *inOutObject);
uint32 PX4BR_McVirtualAttitudeSetpoint_Dec(const char *inBuffer, uint32 inSize, PX4_McVirtualAttitudeSetpointMsg_t *inOutObject);
uint32 PX4BR_McVirtualRatesSetpoint_Dec(const char *inBuffer, uint32 inSize, PX4_McVirtualRatesSetpointMsg_t *inOutObject);
uint32 PX4BR_Mission_Dec(const char *inBuffer, uint32 inSize, PX4_MissionMsg_t *inOutObject);
uint32 PX4BR_MissionResult_Dec(const char *inBuffer, uint32 inSize, PX4_MissionResultMsg_t *inOutObject);
uint32 PX4BR_MultirotorMotorLimits_Dec(const char *inBuffer, uint32 inSize, PX4_MultirotorMotorLimitsMsg_t *inOutObject);
uint32 PX4BR_OffboardControlMode_Dec(const char *inBuffer, uint32 inSize, PX4_OffboardControlModeMsg_t *inOutObject);
uint32 PX4BR_OpticalFlow_Dec(const char *inBuffer, uint32 inSize, PX4_OpticalFlowMsg_t *inOutObject);
uint32 PX4BR_OutputPwm_Dec(const char *inBuffer, uint32 inSize, PX4_OutputPwmMsg_t *inOutObject);
uint32 PX4BR_ParameterUpdate_Dec(const char *inBuffer, uint32 inSize, PX4_ParameterUpdateMsg_t *inOutObject);
uint32 PX4BR_PositionSetpoint_Dec(const char *inBuffer, uint32 inSize, PX4_PositionSetpointMsg_t *inOutObject);
uint32 PX4BR_PositionSetpointTriplet_Dec(const char *inBuffer, uint32 inSize, PX4_PositionSetpointTripletMsg_t *inOutObject);
uint32 PX4BR_PwmInput_Dec(const char *inBuffer, uint32 inSize, PX4_PwmInputMsg_t *inOutObject);
uint32 PX4BR_QShellReq_Dec(const char *inBuffer, uint32 inSize, PX4_QShellReqMsg_t *inOutObject);
uint32 PX4BR_RcChannels_Dec(const char *inBuffer, uint32 inSize, PX4_RcChannelsMsg_t *inOutObject);
uint32 PX4BR_RcParameterMap_Dec(const char *inBuffer, uint32 inSize, PX4_RcParameterMapMsg_t *inOutObject);
uint32 PX4BR_Safety_Dec(const char *inBuffer, uint32 inSize, PX4_SafetyMsg_t *inOutObject);
uint32 PX4BR_SatelliteInfo_Dec(const char *inBuffer, uint32 inSize, PX4_SatelliteInfoMsg_t *inOutObject);
uint32 PX4BR_SensorAccel_Dec(const char *inBuffer, uint32 inSize, PX4_SensorAccelMsg_t *inOutObject);
uint32 PX4BR_SensorBaro_Dec(const char *inBuffer, uint32 inSize, PX4_SensorBaroMsg_t *inOutObject);
uint32 PX4BR_SensorCombined_Dec(const char *inBuffer, uint32 inSize, PX4_SensorCombinedMsg_t *inOutObject);
uint32 PX4BR_SensorGyro_Dec(const char *inBuffer, uint32 inSize, PX4_SensorGyroMsg_t *inOutObject);
uint32 PX4BR_SensorMag_Dec(const char *inBuffer, uint32 inSize, PX4_SensorMagMsg_t *inOutObject);
uint32 PX4BR_ServorailStatus_Dec(const char *inBuffer, uint32 inSize, PX4_ServorailStatusMsg_t *inOutObject);
uint32 PX4BR_SubsystemInfo_Dec(const char *inBuffer, uint32 inSize, PX4_SubsystemInfoMsg_t *inOutObject);
uint32 PX4BR_SystemPower_Dec(const char *inBuffer, uint32 inSize, PX4_SystemPowerMsg_t *inOutObject);
uint32 PX4BR_TecsStatus_Dec(const char *inBuffer, uint32 inSize, PX4_TecsStatusMsg_t *inOutObject);
uint32 PX4BR_TelemetryStatus_Dec(const char *inBuffer, uint32 inSize, PX4_TelemetryStatusMsg_t *inOutObject);
uint32 PX4BR_TestMotor_Dec(const char *inBuffer, uint32 inSize, PX4_TestMotorMsg_t *inOutObject);
uint32 PX4BR_TimeOffset_Dec(const char *inBuffer, uint32 inSize, PX4_TimeOffsetMsg_t *inOutObject);
uint32 PX4BR_TransponderReport_Dec(const char *inBuffer, uint32 inSize, PX4_TransponderReportMsg_t *inOutObject);
uint32 PX4BR_UavCanParameterRequest_Dec(const char *inBuffer, uint32 inSize, PX4_UavCanParameterRequestMsg_t *inOutObject);
uint32 PX4BR_UavCanParameterValue_Dec(const char *inBuffer, uint32 inSize, PX4_UavCanParameterValueMsg_t *inOutObject);
uint32 PX4BR_VehicleAttitude_Dec(const char *inBuffer, uint32 inSize, PX4_VehicleAttitudeMsg_t *inOutObject);
uint32 PX4BR_VehicleAttitudeSetpoint_Dec(const char *inBuffer, uint32 inSize, PX4_VehicleAttitudeSetpointMsg_t *inOutObject);
uint32 PX4BR_VehicleCommandAck_Dec(const char *inBuffer, uint32 inSize, PX4_VehicleCommandAckMsg_t *inOutObject);
uint32 PX4BR_VehicleCommand_Dec(const char *inBuffer, uint32 inSize, PX4_VehicleCommandMsg_t *inOutObject);
uint32 PX4BR_VehicleControlMode_Dec(const char *inBuffer, uint32 inSize, PX4_VehicleControlModeMsg_t *inOutObject);
uint32 PX4BR_VehicleForceSetpoint_Dec(const char *inBuffer, uint32 inSize, PX4_VehicleForceSetpointMsg_t *inOutObject);
uint32 PX4BR_VehicleGlobalPosition_Dec(const char *inBuffer, uint32 inSize, PX4_VehicleGlobalPositionMsg_t *inOutObject);
uint32 PX4BR_VehicleGlobalVelocitySetpoint_Dec(const char *inBuffer, uint32 inSize, PX4_VehicleGlobalVelocitySetpointMsg_t *inOutObject);
uint32 PX4BR_VehicleGpsPosition_Dec(const char *inBuffer, uint32 inSize, PX4_VehicleGpsPositionMsg_t *inOutObject);
uint32 PX4BR_VehicleLandDetected_Dec(const char *inBuffer, uint32 inSize, PX4_VehicleLandDetectedMsg_t *inOutObject);
uint32 PX4BR_VehicleLocalPosition_Dec(const char *inBuffer, uint32 inSize, PX4_VehicleLocalPositionMsg_t *inOutObject);
uint32 PX4BR_VehicleLocalPositionSetpoint_Dec(const char *inBuffer, uint32 inSize, PX4_VehicleLocalPositionSetpointMsg_t *inOutObject);
uint32 PX4BR_VehicleRatesSetpoint_Dec(const char *inBuffer, uint32 inSize, PX4_VehicleRatesSetpointMsg_t *inOutObject);
uint32 PX4BR_VehicleStatus_Dec(const char *inBuffer, uint32 inSize, PX4_VehicleStatusMsg_t *inOutObject);
uint32 PX4BR_VisionPositionEstimate_Dec(const char *inBuffer, uint32 inSize, PX4_VisionPositionEstimateMsg_t *inOutObject);
uint32 PX4BR_VtolVehicleStatus_Dec(const char *inBuffer, uint32 inSize, PX4_VtolVehicleStatusMsg_t *inOutObject);
uint32 PX4BR_WindEstimate_Dec(const char *inBuffer, uint32 inSize, PX4_WindEstimateMsg_t *inOutObject);


#endif /* SRC_PX4BR_SERIALIZATION_H_ */
