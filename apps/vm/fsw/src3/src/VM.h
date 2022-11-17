#ifndef VM_H
#define VM_H

/**
 * *  \brief VM Application Class
 */
class VM {

public:
	/**
	 * \brief Scheduling Pipe ID
	 */
	CFE_SB_PipeId_t SchPipeId;
	/**
	 * \brief Command Pipe ID
	 */
	CFE_SB_PipeId_t CmdPipeId;
	/**
	 * \brief Data Pipe ID
	 */
	CFE_SB_PipeId_t DataPipeId;
	/**
	 * \brief Task Run Status
	 */
	uint32 uiRunStatus;
	/**
	 * \brief Config Table Handle
	 */
	CFE_TBL_Handle_t ConfigTblHdl;
	/**
	 * \brief Battery status message
	 */
	PX4_BatteryStatusMsg_t BatteryStatusMsg;
	/**
	 * \brief Telemetry status message
	 */
	PX4_TelemetryStatusMsg_t TelemetryStatusMsg;
	/**
	 * \brief Subsystem information message
	 */
	PX4_SubsystemInfoMsg_t SubsystemInfoMsg;
	/**
	 * \brief Vehicle attitude message
	 */
	PX4_VehicleAttitudeMsg_t VehicleAttitudeMsg;
	/**
	 * \brief Vehicle local position messsage
	 */
	PX4_VehicleLocalPositionMsg_t VehicleLocalPositionMsg;
	/**
	 * \brief Vehicle land detected message
	 */
	PX4_VehicleLandDetectedMsg_t VehicleLandDetectedMsg;
	/**
	 * \brief Mission result message
	 */
	PX4_MissionResultMsg_t MissionResultMsg;
	/**
	 * \brief Manual control setpoint message
	 */
	PX4_ManualControlSetpointMsg_t ManualControlSetpointMsg;
	/**
	 * \brief position setpoint triplet message
	 */
	PX4_PositionSetpointTripletMsg_t PositionSetpointTripletMsg;
	/**
	 * \brief Safety message
	 */
	PX4_SafetyMsg_t SafetyMsg;
	/**
	 * \brief Sensor correction message
	 */
	PX4_SensorCorrectionMsg_t SensorCorrectionMsg;
	/**
	 * \brief Sensor combined message
	 */
	PX4_SensorCombinedMsg_t SensorCombinedMsg;
	/**
	 * \brief Vehicle commanding message
	 */
	PX4_VehicleCommandMsg_t VehicleCommandMsg;
	/**
	 * \brief Vehicle global position message
	 */
	PX4_VehicleGlobalPositionMsg_t VehicleGlobalPositionMsg;
	/**
	 * \brief Vehicle gps position message
	 */
	PX4_VehicleGpsPositionMsg_t VehicleGpsPositionMsg;
	/**
	 * \brief Actuator armed message
	 */
	PX4_ActuatorArmedMsg_t ActuatorArmedMsg;
	/**
	 * \brief Home position message
	 */
	PX4_HomePositionMsg_t HomePositionMsg;
	/**
	 * \brief Vehicle manager state message
	 */
	PX4_CommanderStateMsg_t VehicleManagerStateMsg;
	/**
	 * \brief Mission message
	 */
	PX4_MissionMsg_t MissionMsg;
	/**
	 * \brief Led control message
	 */
	PX4_LedControlMsg_t LedControlMsg;
	/**
	 * \brief Vehicle status message
	 */
	PX4_VehicleStatusMsg_t VehicleStatusMsg;
	/**
	 * \brief Vehicle control mode message
	 */
	PX4_VehicleControlModeMsg_t VehicleControlModeMsg;
	/**
	 * \brief Arming state machine handle
	 */
	VM_Arming ArmingSM;
	/**
	 * \brief Navigation state machine handle
	 */
	VM_Navigation NavigationSM;
	/**
	 * \brief Housekeeping Telemetry for downlink
	 */
	VM_HkTlm_t HkTlm;
	/**
	 * \brief Configuration Telemetry for downlink
	 */
	VM_ConfigTlm_t ConfigTlm;
	/**
	 * \brief Config Table Pointer
	 */
	VM_ConfigTbl_t* ConfigTblPtr;

	/**
	 * \brief Default constructor.
	 */
	VM();

	/**
	 * \brief Destructor
	 */
	~VM();

	/**
	 * \brief Vehicle Manager (VM) application entry point
	 * *
	 * *  \par Description
	 * *       Vehicle Manager Task application entry point.  This function
	 * *       performs app initialization, then waits for the cFE ES Startup
	 * *       Sync, then executes the main processing loop.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       If there is an unrecoverable failure during initialization the
	 * *       main loop is never executed and the application will exit.
	 * *
	 * ***********************************************************************
	 */
	void AppMain(void unnamed_1);

	/**
	 * \brief Initialize the Vehicle Manager (VM) application
	 * *
	 * *  \par Description
	 * *       Vehicle Manager application initialization routine. This
	 * *       function performs all the required startup steps to
	 * *       initialize (or restore from CDS) VM data structures and get
	 * *       the application registered with the cFE services so it can
	 * *       begin to receive command messages and send events.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * *  \returns
	 * *  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
	 * *  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
	 * *  \retstmt Return codes from #VM_InitEvent               \endcode
	 * *  \retstmt Return codes from #VM_InitPipe                \endcode
	 * *  \retstmt Return codes from #VM_InitData                \endcode
	 * *  \retstmt Return codes from #VM_InitConfigTbl           \endcode
	 * *  \retstmt Return codes from #OS_TaskInstallDeleteHandler \endcode
	 * *  \endreturns
	 * *
	 * ***********************************************************************
	 */
	int32 InitApp(void unnamed_1);

	/**
	 * \brief Initialize named parameters
	 * *
	 * *  \par Description
	 * *       This function registers named parameters and prepares the object
	 * *       to respond to updates in named parameters.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * *  \returns
	 * *  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
	 * *  \retstmt Return codes from #ParamsConsumer::InitParams  \endcode
	 * *  \endreturns
	 * *
	 * ***********************************************************************
	 */
	int32 InitParams(void unnamed_1);

	/**
	 * \brief Initialize Event Services and Event tables
	 * *
	 * *  \par Description
	 * *       This function performs the steps required to setup
	 * *       cFE Event Services for use by the VM application.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * *  \returns
	 * *  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
	 * *  \retstmt Return codes from #CFE_EVS_Register  \endcode
	 * *  \endreturns
	 * *
	 * ***********************************************************************
	 */
	int32 InitEvent(void unnamed_1);

	/**
	 * \brief Initialize global variables used by VM application
	 * *
	 * *  \par Description
	 * *       This function performs the steps required to initialize
	 * *       the VM application data.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * ***********************************************************************
	 */
	void InitData(void unnamed_1);

	/**
	 * \brief Initialize message pipes
	 * *
	 * *  \par Description
	 * *       This function performs the steps required to setup
	 * *       initialize the cFE Software Bus message pipes and subscribe to
	 * *       messages for the VM application.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * *  \returns
	 * *  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
	 * *  \retstmt Return codes from #CFE_SB_CreatePipe        \endcode
	 * *  \retstmt Return codes from #CFE_SB_SubscribeEx       \endcode
	 * *  \retstmt Return codes from #CFE_SB_Subscribe         \endcode
	 * *  \endreturns
	 * *
	 * ***********************************************************************
	 */
	int32 InitPipe(void unnamed_1);

	/**
	 * \brief Receive and process messages from the scheduler pipe.
	 * *
	 * *  \par Description
	 * *       This function receives and processes messages
	 * *       for the VM application from the SCH pipe.  This function
	 * *       will pend for the type defined by iBlocking, allowing
	 * *       it to wait for messages, i.e. wakeup messages from scheduler.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * *  \param [in]   iBlocking    A #CFE_SB_PEND_FOREVER, #CFE_SB_POLL or
	 * *                             millisecond timeout
	 * *
	 * *  \returns
	 * *  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
	 * *  \retstmt Return codes from #CFE_SB_RcvMsg            \endcode
	 * *  \endreturns
	 * *
	 * ***********************************************************************
	 */
	int32 RcvSchPipeMsg(int32 iBlocking);

	/**
	 * \brief Process incoming data messages
	 * *
	 * *  \par Description
	 * *       This function processes incoming data messages subscribed
	 * *       by VM application
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * ***********************************************************************
	 */
	void ProcessDataPipe(void unnamed_1);

	/**
	 * \brief Vehicle Manager Task incoming command processing
	 * *
	 * *  \par Description
	 * *       This function processes incoming commands subscribed
	 * *       by VM application
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * ***********************************************************************
	 */
	void ProcessCmdPipe(void unnamed_1);

	/**
	 * \brief Vehicle Manager Task application commands
	 * *
	 * *  \par Description
	 * *       This function processes command messages
	 * *       specific to the VM application
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * *  \param [in]   MsgPtr       A #CFE_SB_Msg_t pointer that
	 * *                             references the software bus message
	 * *
	 * ***********************************************************************
	 */
	void ProcessAppCmds(CFE_SB_Msg_t* MsgPtr);

	/**
	 * \brief Sends VM housekeeping message
	 * *
	 * *  \par Description
	 * *       This function sends the housekeeping message
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * ***********************************************************************
	 */
	void ReportHousekeeping(void unnamed_1);

	/**
	 * \brief Sends the ActuatorArmedMsg message.
	 * *
	 * *  \par Description
	 * *       This function publishes the ActuatorArmedMsg message containing
	 * *       <TODO>
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * ***********************************************************************
	 */
	void SendActuatorArmedMsg(void unnamed_1);

	/**
	 * \brief Sends the HomePositionMsg message.
	 * *
	 * *  \par Description
	 * *       This function publishes the HomePositionMsg message containing
	 * *       <TODO>
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * ***********************************************************************
	 */
	void SendHomePositionMsg(void unnamed_1);

	/**
	 * \brief Sends the VehicleManagerStateMsg message.
	 * *
	 * *  \par Description
	 * *       This function publishes the VehicleManagerStateMsg message containing
	 * *       <TODO>
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * ***********************************************************************
	 */
	void SendVehicleManagerStateMsg(void unnamed_1);

	/**
	 * \brief Sends the MissionMsg message.
	 * *
	 * *  \par Description
	 * *       This function publishes the MissionMsg message containing
	 * *       <TODO>
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * ***********************************************************************
	 */
	void SendMissionMsg(void unnamed_1);

	/**
	 * \brief Sends the LedControlMsg message.
	 * *
	 * *  \par Description
	 * *       This function publishes the LedControlMsg message containing
	 * *       <TODO>
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * ***********************************************************************
	 */
	void SendLedControlMsg(void unnamed_1);

	/**
	 * \brief Sends the VehicleStatusMsg message.
	 * *
	 * *  \par Description
	 * *       This function publishes the VehicleStatusMsg message containing
	 * *       <TODO>
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * ***********************************************************************
	 */
	void SendVehicleStatusMsg(void unnamed_1);

	/**
	 * \brief Sends the VehicleControlModeMsg message.
	 * *
	 * *  \par Description
	 * *       This function publishes the VehicleControlModeMsg message
	 * *       containing <TODO>
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * ***********************************************************************
	 */
	void SendVehicleControlModeMsg(void unnamed_1);

	/**
	 * \brief Sends the VehicleCommandMsg message.
	 * *
	 * *  \par Description
	 * *       This function publishes the VehicleCommandMsg message
	 * *       containing <TODO>
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * ***********************************************************************
	 */
	void SendVehicleCommandMsg(void unnamed_1);

	/**
	 * \brief Verify Command Length
	 * *
	 * *  \par Description
	 * *       This function verifies the command message length.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * *  \param [in]   MsgPtr        A #CFE_SB_Msg_t pointer that
	 * *                              references the software bus message
	 * *  \param [in]   usExpectedLen The expected length of the message
	 * *
	 * *  \returns
	 * *  TRUE if the message length matches expectations, FALSE if it does not.
	 * *  \endreturns
	 * *
	 * ***********************************************************************
	 */
	boolean VerifyCmdLength(CFE_SB_Msg_t* MsgPtr, uint16 usExpectedLen);

	/**
	 * \brief Time Elapsed
	 * *
	 * *  \par Description
	 * *       Returns elaped time.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * *  \param [in]   TimePtr    A pointer to then time.
	 * *
	 * *  \returns
	 * *   Time in unsigned integer format.
	 * *  \endreturns
	 * *
	 * ***********************************************************************
	 */
	uint64 TimeElapsed(uint64* TimePtr);

	/**
	 * \brief Time
	 * *
	 * *  \par Description
	 * *       Returns current time.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *  \returns
	 * *   Time in unsigned integer format.
	 * *  \endreturns
	 * *
	 * ***********************************************************************
	 */
	uint64 TimeNow(void unnamed_1);

	/**
	 * \brief Vehicle Armed
	 * *
	 * *  \par Description
	 * *       True is Vehicle is armed
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *  \returns
	 * *   TRUE if the armed attribute in actuator armed message is true, FALSE if it is not.
	 * *  \endreturns
	 * *
	 * ***********************************************************************
	 */
	osalbool IsVehicleArmed(void unnamed_1);

	/**
	 * \brief Set Home Position
	 * *
	 * *  \par Description
	 * *       Sets home position at initialization, arming and launch events
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * ***********************************************************************
	 */
	void FlightSessionInit(void unnamed_1);

	/**
	 * \brief Set Home Position
	 * *
	 * *  \par Description
	 * *       Sets home position at initialization, arming and launch events
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * ***********************************************************************
	 */
	void SetHomePosition(void unnamed_1);

	/**
	 * \brief RC Control Navigation Modes
	 * *
	 * *  \par Description
	 * *       Procedure to how rc will switch navigation modes such as position
	 * *       hold, auto loiter, auto rtl and manual using manual control setpoint
	 * *       message. State transitions are invoked by this method.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * ***********************************************************************
	 */
	void RcModes(void unnamed_1);

	/**
	 * \brief Vehicle Manager Initialization Task
	 * *
	 * *  \par Description
	 * *       Messages are initialized to their defaults and housekeeping.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * ***********************************************************************
	 */
	void Initialization(void unnamed_1);

	/**
	 * \brief Vehicle Manager Maintenance Task
	 * *
	 * *  \par Description
	 * *       Executes every time a new message is available, verifies message
	 * *       validity and updates messages and publishes them to software bus.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * ***********************************************************************
	 */
	void Execute(void unnamed_1);

	/**
	 * \brief TODO
	 * *
	 * *  \par Description
	 * *       Executes every time a new message is available, verifies message
	 * *       validity and updates messages and publishes them to software bus.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * ***********************************************************************
	 */
	const char* GetNavStateAsString(uint32 unnamed_1);

	/**
	 * \brief Updates application params from param table
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * ***********************************************************************
	 */
	void UpdateParamsFromTable(void unnamed_1);

private:
	/**
	 * \brief Initialize the VM configuration tables.
	 * *
	 * *  \par Description
	 * *       This function initializes VM's configuration tables.  This
	 * *       includes <TODO>.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * *  \returns
	 * *  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
	 * *  \retstmt Return codes from #CFE_TBL_Register          \endcode
	 * *  \retstmt Return codes from #CFE_TBL_Load              \endcode
	 * *  \retstmt Return codes from #VM_AcquireConfigPointers \endcode
	 * *  \endreturns
	 * *
	 * ***********************************************************************
	 */
	int32 InitConfigTbl(void unnamed_1);

	/**
	 * \brief Obtain VM configuration tables data pointers.
	 * *
	 * *  \par Description
	 * *       This function manages the configuration tables
	 * *       and obtains a pointer to their data.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * *  \returns
	 * *  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
	 * *  \endreturns
	 * *
	 * ***********************************************************************
	 */
	int32 AcquireConfigPointers(void unnamed_1);

	/**
	 * \brief Send the configuration message.
	 * *
	 * *  \par Description
	 * *       This function publishes a message containing the current
	 * *       configuration.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * *  \returns
	 * *   None
	 * *  \endreturns
	 * *
	 * ***********************************************************************
	 */
	void ReportConfiguration(void unnamed_1);

public:
	/**
	 * \brief Validate VM configuration table
	 * *
	 * *  \par Description
	 * *       This function validates VM's configuration table
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * *  \param [in]   ConfigTblPtr    A pointer to the table to validate.
	 * *
	 * *  \returns
	 * *  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
	 * *  \endreturns
	 * *
	 * ***********************************************************************
	 */
	static int32 ValidateConfigTbl(void* unnamed_1);

	/**
	 * \brief Validate the COM_RC_IN_MODE parameter
	 * *
	 * *  \par Description
	 * *       Validate the RC control input mode (COM_RC_IN_MODE) parameter.
	 * *
	 * *  \par Limits:
	 * *       Min > Max (incr.) 0 > 2, default 0.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       #VM_INVALID_CONFIG_TABLE_ERR_EID
	 * *
	 * *  \param [in]   param    Value of the parameter.
	 * *
	 * *  \returns
	 * *  \retcode false  \retdesc \copydoc false  \endcode
	 * *  \retcode true  \retdesc \copydoc true  \endcode
	 * *  \endreturns
	 * *
	 * ***********************************************************************
	 */
	static osalbool Validate_COM_RC_IN_MODE(uint32 param);

	/**
	 * \brief Validate the COM_ARM_SWISBTN parameter
	 * *
	 * *  \par Description
	 * *       Validate the Arm switch is only a button (COM_ARM_SWISBTN) parameter.
	 * *
	 * *  \par Limits:
	 * *       Min > Max (incr.) 0 > 1, default 0.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       #VM_INVALID_CONFIG_TABLE_ERR_EID
	 * *
	 * *  \param [in]   param    Value of the parameter.
	 * *
	 * *  \returns
	 * *  \retcode false  \retdesc \copydoc false  \endcode
	 * *  \retcode true  \retdesc \copydoc true  \endcode
	 * *  \endreturns
	 * *
	 * ***********************************************************************
	 */
	static osalbool Validate_COM_ARM_SWISBTN(uint32 param);

	/**
	 * \brief Validate the COM_RC_ARM_HYST parameter
	 * *
	 * *  \par Description
	 * *       Validate the RC input arm/disarm command duration (COM_RC_ARM_HYST) parameter.
	 * *
	 * *  \par Limits:
	 * *       Min > Max (incr.) 100 > 1500, default 1000.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       #VM_INVALID_CONFIG_TABLE_ERR_EID
	 * *
	 * *  \param [in]   param    Value of the parameter.
	 * *
	 * *  \returns
	 * *  \retcode false  \retdesc \copydoc false  \endcode
	 * *  \retcode true  \retdesc \copydoc true  \endcode
	 * *  \endreturns
	 * *
	 * ***********************************************************************
	 */
	static osalbool Validate_COM_RC_ARM_HYST(uint32 param);

	/**
	 * \brief Validate the MAV_SYS_ID parameter
	 * *
	 * *  \par Description
	 * *       Validate the System ID (MAV_SYS_ID) parameter.
	 * *
	 * *  \par Limits:
	 * *       Min > Max (incr.) 1 > 250, default 1.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       #VM_INVALID_CONFIG_TABLE_ERR_EID
	 * *
	 * *  \param [in]   param    Value of the parameter.
	 * *
	 * *  \returns
	 * *  \retcode false  \retdesc \copydoc false  \endcode
	 * *  \retcode true  \retdesc \copydoc true  \endcode
	 * *  \endreturns
	 * *
	 * ***********************************************************************
	 */
	static osalbool Validate_MAV_SYS_ID(uint32 param);

	/**
	 * \brief Validate the MAV_COMP_ID parameter
	 * *
	 * *  \par Description
	 * *       Validate the Component ID (MAV_COMP_ID) parameter.
	 * *
	 * *  \par Limits:
	 * *       Min > Max (incr.) 1 > 250, default 1.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       #VM_INVALID_CONFIG_TABLE_ERR_EID
	 * *
	 * *  \param [in]   param    Value of the parameter.
	 * *
	 * *  \returns
	 * *  \retcode false  \retdesc \copydoc false  \endcode
	 * *  \retcode true  \retdesc \copydoc true  \endcode
	 * *  \endreturns
	 * *
	 * ***********************************************************************
	 */
	static osalbool Validate_MAV_COMP_ID(uint32 param);

	/**
	 * \brief Validate the COM_RC_LOSS_T parameter
	 * *
	 * *  \par Description
	 * *       Validate the RC loss time threshold (COM_RC_LOSS_T) parameter.
	 * *
	 * *  \par Limits:
	 * *       Min > Max (incr.) 0.0 > 35.0, default 0.5.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       #VM_INVALID_CONFIG_TABLE_ERR_EID
	 * *
	 * *  \param [in]   param    Value of the parameter.
	 * *
	 * *  \returns
	 * *  \retcode false  \retdesc \copydoc false  \endcode
	 * *  \retcode true  \retdesc \copydoc true  \endcode
	 * *  \endreturns
	 * *
	 * ***********************************************************************
	 */
	static osalbool Validate_COM_RC_LOSS_T(float param);

	/**
	 * \brief Validate the COM_LOW_BAT_ACT parameter
	 * *
	 * *  \par Description
	 * *       Validate the Battery failsafe mode (COM_LOW_BAT_ACT) parameter.
	 * *
	 * *  \par Limits:
	 * *       Min > Max (incr.) 0 > 3, default 0.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       #VM_INVALID_CONFIG_TABLE_ERR_EID
	 * *
	 * *  \param [in]   param    Value of the parameter.
	 * *
	 * *  \returns
	 * *  \retcode false  \retdesc \copydoc false  \endcode
	 * *  \retcode true  \retdesc \copydoc true  \endcode
	 * *  \endreturns
	 * *
	 * ***********************************************************************
	 */
	static osalbool Validate_COM_LOW_BAT_ACT(uint32 param);

	/**
	 * \brief Validate the COM_HOME_H_T parameter
	 * *
	 * *  \par Description
	 * *       Validate the Home set horizontal threshold (COM_HOME_H_T) parameter.
	 * *
	 * *  \par Limits:
	 * *       Min > Max (incr.) 2.0 > 15.0, default 5.0.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       #VM_INVALID_CONFIG_TABLE_ERR_EID
	 * *
	 * *  \param [in]   param    Value of the parameter.
	 * *
	 * *  \returns
	 * *  \retcode false  \retdesc \copydoc false  \endcode
	 * *  \retcode true  \retdesc \copydoc true  \endcode
	 * *  \endreturns
	 * *
	 * ***********************************************************************
	 */
	static osalbool Validate_COM_HOME_H_T(float param);

	/**
	 * \brief Validate the COM_HOME_V_T parameter
	 * *
	 * *  \par Description
	 * *       Validate the Home set vertical threshold (COM_HOME_V_T) parameter.
	 * *
	 * *  \par Limits:
	 * *       Min > Max (incr.) 5.0 > 25.0, default 10.0.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       #VM_INVALID_CONFIG_TABLE_ERR_EID
	 * *
	 * *  \param [in]   param    Value of the parameter.
	 * *
	 * *  \returns
	 * *  \retcode false  \retdesc \copydoc false  \endcode
	 * *  \retcode true  \retdesc \copydoc true  \endcode
	 * *  \endreturns
	 * *
	 * ***********************************************************************
	 */
	static osalbool Validate_COM_HOME_V_T(float param);

	/**
	 * \brief Validate a #uint32 named parameter
	 * *
	 * *  \par Description
	 * *       This function is called by the #ParamsConsumer base class
	 * *       when before a named parameter is changed by address.  The address
	 * *       passed is the address of the parameter passed in the
	 * *       #ParamsConsumer::InitParams function.  Return #true if the
	 * *       parameter is valid, and #false if invalid.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * *  \returns
	 * *   #true if valid.  #false if invalid
	 * *  \retstmt Return codes from:
	 * *           #VM::Validate_COM_RC_IN_MODE,
	 * *           #VM::Validate_COM_ARM_SWISBTN,
	 * *           #VM::Validate_COM_RC_ARM_HYST,
	 * *           #VM::Validate_MAV_SYS_ID,
	 * *           #VM::Validate_MAV_COMP_ID,
	 * *           #VM::Validate_COM_LOW_BAT_ACT
	 * *  \endreturns
	 * *
	 * ***********************************************************************
	 */
	osalbool onParamValidate(void* Address, uint32 Value);

	/**
	 * \brief Validate a #float named parameter
	 * *
	 * *  \par Description
	 * *       This function is called by the #ParamsConsumer base class
	 * *       when before a named parameter is changed by address.  The address
	 * *       passed is the address of the parameter passed in the
	 * *       #ParamsConsumer::InitParams function.  Return #true if the
	 * *       parameter is valid, and #false if invalid.
	 * *
	 * *  \par Assumptions, External Events, and Notes:
	 * *       None
	 * *
	 * *  \returns
	 * *   #true if valid.  #false if invalid
	 * *  \retstmt Return codes from:
	 * *           #VM::Validate_COM_RC_LOSS_T,
	 * *           #VM::Validate_COM_HOME_H_T,
	 * *           #VM::Validate_COM_HOME_V_T,
	 * *           #VM::Validate_HOME_POS_ALT_PADDING
	 * *  \endreturns
	 * *
	 * ***********************************************************************
	 */
	osalbool onParamValidate(void* Address, float Value);

	class Arming {


	public:
		void InitComplete();

		void Reset();

		void Arm();

		void Error();

		void Disarm();
	};
	class Navigation {


	public:
		void trInitComplete();

		void Reset();

		void trAltitudeControl();

		void trPositionControl();

		void trAutoLoiter();

		void trAutoReturnToLaunch();

		void trAcrobatic();

		void trStabilize();

		void trRattitude();

		void trAutoTakeoff();

		void trAutoLand();

		void trManual();
	};
};

#endif
