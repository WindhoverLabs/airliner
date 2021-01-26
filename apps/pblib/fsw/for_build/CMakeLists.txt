 #############################################################################
 #
 #   Copyright (c) 2017 Windhover Labs, L.L.C. All rights reserved.
 #
 # Redistribution and use in source and binary forms, with or without
 # modification, are permitted provided that the following conditions
 # are met:
 #
 # 1. Redistributions of source code must retain the above copyright
 #    notice, this list of conditions and the following disclaimer.
 # 2. Redistributions in binary form must reproduce the above copyright
 #    notice, this list of conditions and the following disclaimer in
 #    the documentation and/or other materials provided with the
 #    distribution.
 # 3. Neither the name Windhover Labs nor the names of its contributors 
 #    may be used to endorse or promote products derived from this software
 #    without specific prior written permission.
 #
 # THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 # "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 # LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 # FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 # COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 # INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 # BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 # OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 # AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 # LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 # ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 # POSSIBILITY OF SUCH DAMAGE.
 #
 #############################################################################
 
set(PROTOBUF_MSGS_DIR ${CMAKE_CURRENT_BINARY_DIR}/msgs/protobuf)

buildliner_add_app_def(pblib
    FILE PBLIB
    NANOPB
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/pb_lib.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/pb_version.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../public_inc/pb_lib.h
      	${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/cfe_serialization.h
      	${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/cfe_serialization.c
      	${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/px4_serialization.h
      	${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/px4_serialization.c
      	${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/sens_serialization.h
      	${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/sens_serialization.c
      	${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/gps_serialization.h
      	${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/gps_serialization.c
        ${PROTOBUF_MSGS_DIR}/CFE_ES_HkPacket_t.pb.h
        ${PROTOBUF_MSGS_DIR}/CFE_ES_HkPacket_t.pb.c
        ${PROTOBUF_MSGS_DIR}/CFE_ES_SetMaxPRCountCmd_t.pb.h
        ${PROTOBUF_MSGS_DIR}/CFE_ES_SetMaxPRCountCmd_t.pb.c
        ${PROTOBUF_MSGS_DIR}/sens_hk.pb.h
        ${PROTOBUF_MSGS_DIR}/sens_hk.pb.c
        ${PROTOBUF_MSGS_DIR}/gps_hk.pb.h
        ${PROTOBUF_MSGS_DIR}/gps_hk.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_DistanceSensorMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_DistanceSensorMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_VehicleGlobalPositionMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_VehicleGlobalPositionMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_SensorCombinedMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_SensorCombinedMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_EstimatorStatusMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_EstimatorStatusMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_McAttCtrlStatusMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_McAttCtrlStatusMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_InputRcMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_InputRcMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_VehicleAttitudeMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_VehicleAttitudeMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_VehicleAttitudeSetpointMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_VehicleAttitudeSetpointMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_VehicleCommandMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_VehicleCommandMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_LedControlMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_LedControlMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_AirspeedMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_AirspeedMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_SafetyMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_SafetyMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_ActuatorControlsMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_ActuatorControlsMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_BatteryStatusMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_BatteryStatusMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_VehicleStatusMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_VehicleStatusMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_PositionSetpointTripletMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_PositionSetpointTripletMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_VehicleGpsPositionMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_VehicleGpsPositionMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_VehicleLocalPositionSetpointMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_VehicleLocalPositionSetpointMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_HomePositionMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_HomePositionMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_VehicleLocalPositionMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_VehicleLocalPositionMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_TelemetryStatusMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_TelemetryStatusMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_SaturationStatus_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_SaturationStatus_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_VehicleRatesSetpointMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_VehicleRatesSetpointMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_VehicleControlModeMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_VehicleControlModeMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_VehicleLandDetectedMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_VehicleLandDetectedMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_MissionMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_MissionMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_MissionResultMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_MissionResultMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_RcChannelsMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_RcChannelsMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_GpsInjectDataMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_GpsInjectDataMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_ControlStateMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_ControlStateMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_SubsystemInfoMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_SubsystemInfoMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_ManualControlSetpointMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_ManualControlSetpointMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_SensorBaroMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_SensorBaroMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_SensorAccelMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_SensorAccelMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_SensorMagMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_SensorMagMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_DifferentialPressureMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_DifferentialPressureMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_SensorGyroMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_SensorGyroMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_ActuatorArmedMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_ActuatorArmedMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_ActuatorOutputsMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_ActuatorOutputsMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_SensorCorrectionMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_SensorCorrectionMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_CommanderStateMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_CommanderStateMsg_t.pb.c
        ${PROTOBUF_MSGS_DIR}/PX4_SatelliteInfoMsg_t.pb.h
        ${PROTOBUF_MSGS_DIR}/PX4_SatelliteInfoMsg_t.pb.c
        ${PROJECT_SOURCE_DIR}/tools/nanopb/pb_common.c
        ${PROJECT_SOURCE_DIR}/tools/nanopb/pb_common.h
        ${PROJECT_SOURCE_DIR}/tools/nanopb/pb_decode.c
        ${PROJECT_SOURCE_DIR}/tools/nanopb/pb_decode.h
        ${PROJECT_SOURCE_DIR}/tools/nanopb/pb_encode.c
        ${PROJECT_SOURCE_DIR}/tools/nanopb/pb_encode.h
        ${PROJECT_SOURCE_DIR}/tools/nanopb/pb.h

    INCLUDES
        ${PROTOBUF_MSGS_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
    	${CMAKE_CURRENT_SOURCE_DIR}/../public_inc/
    	${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/

    PROTOBUF_DEFS
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/CFE_ES_HkPacket_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/CFE_ES_SetMaxPRCountCmd_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/sens_hk.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/gps_hk.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_DistanceSensorMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_VehicleGlobalPositionMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_SensorCombinedMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_EstimatorStatusMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_McAttCtrlStatusMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_InputRcMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_VehicleAttitudeMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_VehicleAttitudeSetpointMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_VehicleCommandMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_LedControlMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_AirspeedMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_SafetyMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_ActuatorControlsMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_BatteryStatusMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_VehicleStatusMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_PositionSetpointTripletMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_VehicleGpsPositionMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_VehicleLocalPositionSetpointMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_HomePositionMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_VehicleLocalPositionMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_TelemetryStatusMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_SaturationStatus_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_VehicleRatesSetpointMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_VehicleControlModeMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_VehicleLandDetectedMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_MissionMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_MissionResultMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_RcChannelsMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_GpsInjectDataMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_ControlStateMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_SubsystemInfoMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_ManualControlSetpointMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_SensorBaroMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_SensorAccelMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_SensorMagMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_DifferentialPressureMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_SensorGyroMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_ActuatorArmedMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_ActuatorOutputsMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_SensorCorrectionMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_CommanderStateMsg_t.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/PX4_SatelliteInfoMsg_t.proto

    PROTOBUF_MSGS_DIR        
        ${PROTOBUF_MSGS_DIR}

    # This application has a public include header.  Add it so all the other applications can access it.
    PUBLIC_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../public_inc/

    DESIGN_DOCS ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in
    
    REFERENCE_CONFIG
        ${PROJECT_SOURCE_DIR}/apps/pblib/fsw/mission_inc
        ${PROJECT_SOURCE_DIR}/apps/pblib/fsw/platform_inc
)

buildliner_add_app_unit_test(pblib-ut
    UTASSERT
    NANOPB
    NO_HELGRIND
    NO_MASSIF

    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/pb_lib.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/pb_version.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../public_inc/pb_lib.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/pbl_app_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/pbl_app_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/pbl_test_utils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/pbl_test_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/pbl_testrunner.c

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
		${CMAKE_CURRENT_SOURCE_DIR}/../public_inc/
		${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/
        
    VALGRIND_SUPPRESSION_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/valgrind.supp
)

set(PROTOBUF_MSGS_DIR ${PROTOBUF_MSGS_DIR} PARENT_SCOPE)
