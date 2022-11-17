#ifndef VM_HKTLM_T_H
#define VM_HKTLM_T_H

struct VM_HkTlm_t {

public:
	/**
	 * \brief cFE SB Tlm Msg Hdr
	 */
	uint8 TlmHeader[CFE_SB_TLM_HDR_SIZE];
	/**
	 * \vmtlmmnemonic \VM_CMDACPTCNT
	 *      \brief Count of accepted commands.  Cleared by Reset Command.
	 */
	uint8 usCmdCnt;
	/**
	 * \vmtlmmnemonic \VM_CMDRJCTCNT
	 *      \brief Count of failed commands.  Cleared by Reset Command.
	 */
	uint8 usCmdErrCnt;
	/**
	 * \vmtlmmnemonic \VM_ARMSTATE
	 *      \brief Current arm state
	 */
	uint32 ArmingState;
	/**
	 * \vmtlmmnemonic \VM_NAVSTATE
	 *      \brief Current navigation state
	 */
	uint32 NavState;
	/**
	 * \brief Number of times the main loop has woken up.
	 */
	uint32 WakeupCount;
	/**
	 * \brief Number of times the Battery Status message has been received.
	 */
	uint32 BatteryStatusMsgCount;
	/**
	 * \brief Number of times the Telemetry Status message has been received.
	 */
	uint32 TelemetryStatusMsgCount;
	/**
	 * \brief Number of times the Subsystem Info message has been received.
	 */
	uint32 SubsystemInfoMsgCount;
	/**
	 * \brief Number of times the Vehicle Attitude message has been received.
	 */
	uint32 VehicleAttitudeMsgCount;
	/**
	 * \brief Number of times the Vehicle Local Position message has been received.
	 */
	uint32 VehicleLocalPositionMsgCount;
	/**
	 * \brief Number of times the Vehicle Land Detected message has been received.
	 */
	uint32 VehicleLandDetectedMsgCount;
	/**
	 * \brief Number of times the Mission Result message has been received.
	 */
	uint32 MissionResultMsgCount;
	/**
	 * \brief Number of times the Manual Control Setpoint message has been received.
	 */
	uint32 ManualControlSetpointMsgCount;
	/**
	 * \brief Number of times the Position Setpoint Triplet message has been received.
	 */
	uint32 PositionSetpointTripletMsgCount;
	/**
	 * \brief Number of times the Safety message has been received.
	 */
	uint32 SafetyMsgCount;
	/**
	 * \brief Number of times the Sensor Correction message has been received.
	 */
	uint32 SensorCorrectionMsgCount;
	/**
	 * \brief Number of times the Sensor Combined message has been received.
	 */
	uint32 SensorCombinedMsgCount;
	/**
	 * \brief Number of times the Vehicle Command message has been received.
	 */
	uint32 VehicleCommandMsgCount;
	/**
	 * \brief Number of times the Vehicle Global Position message has been received.
	 */
	uint32 VehicleGlobalPositionMsgCount;
	/**
	 * \brief Number of times the Vehicle GPS Position message has been received.
	 */
	uint32 VehicleGpsPositionMsgCount;
	/**
	 * \brief Number of times the Vehicle Control Mode message has been received.
	 */
	uint32 VehicleControlModeMsgCount;
	/**
	 * \brief True if home position is not set and local variables are not initialization
	 */
	osalbool NotInitialized;
	/**
	 * \brief Timestamps vm at boot
	 */
	uint64 BootTimestamp;
	/**
	 * \brief True if local position is valid
	 */
	osalbool LocalPositionIsValid;
	/**
	 * \brief True if previously landed
	 */
	osalbool PrevLanded;
	/**
	 * \brief True if previously in flight
	 */
	osalbool PrevInFlight;
	/**
	 * \brief Records a count when vehicle is disarmed with stick
	 */
	uint32 StickOffCounter;
	/**
	 * \brief Records a count when vehicle is armed with stick
	 */
	uint32 StickOnCounter;
	/**
	 * \brief Arming switch in manual control setpoint message
	 */
	uint32 LastSpManArmSwitch;
	/**
	 * \brief True when vehicle's battery is low and a contingency action is implemented
	 */
	osalbool LowBatteryVoltageActionsDone;
	/**
	 * \brief True when vehicle's battery is critical and a contingency action is implemented
	 */
	osalbool CriticalBatteryVoltageActionsDone;
	/**
	 * \brief True when vehicle's battery is dangerously low and a contingency action is implemented
	 */
	osalbool EmergencyBatteryVoltageActionsDone;
	/**
	 * \brief Timestamps the moment rc signal is lost
	 */
	uint64 RCSignalLostTimestamp;
	/**
	 * \brief True when arming status changes with the vehicle
	 */
	osalbool ArmingStateChanged;
	/**
	 * \brief status flag variable
	 */
	VM_StatusFlags StatusFlags;
	/**
	 * \brief An instance rc navigation mode
	 */
	VM_Modes PreviousModes;
};

#endif
