/****************************************************************************
*
*   Copyright (c) 2017 Windhover Labs, L.L.C. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
* 3. Neither the name Windhover Labs nor the names of its
*    contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/

#ifndef VM_APP_H
#define VM_APP_H

#include "vm_Arming.h"
#include "vm_Navigation.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
 ** Pragmas
 *************************************************************************/

/************************************************************************
 ** Includes
 *************************************************************************/
#include "cfe.h"

#include "vm_platform_cfg.h"
#include "vm_mission_cfg.h"
#include "vm_perfids.h"
#include "vm_msgids.h"
#include "vm_msg.h"
#include "vm_events.h"
#include "vm_tbldefs.h"
#include "px4_msgs.h"
#include "px4lib.h"

#include "math/Quaternion.hpp"
#include "math/Euler.hpp"
#include "math/Dcm.hpp"

#include "math/Matrix3F3.hpp"
#include "math/Vector3F.hpp"

#include "vm_state_history.h"

/************************************************************************
 ** Local Defines
 *************************************************************************/
#define HOTPLUG_SENS_TIMEOUT		(8 * 1000 * 1000) 									/**< Decouple update interval and hysteresis counters, all depends on intervals */
#define COMMANDER_MONITORING_INTERVAL 10000
#define COMMANDER_MONITORING_LOOPSPERMSEC (1/(COMMANDER_MONITORING_INTERVAL/1000.0f))
#define OFFBOARD_TIMEOUT		500000
#define FAILSAFE_DEFAULT_TIMEOUT	(3 * 1000 * 1000)									/**< hysteresis time - the failsafe will trigger after 3 seconds in this state */
#define POSITION_TIMEOUT		(1 * 1000 * 1000)
#define STICK_ON_OFF_LIMIT 0.9f

/************************************************************************
 ** Local Structure Definitions
 *************************************************************************/
/**
 * \brief Status flags
 */
typedef struct  {
	bool condition_system_sensors_initialized;
	bool condition_system_returned_to_home;
	bool condition_home_position_valid;					// indicates a valid home position (a valid home position is not always a valid launch)
	bool usb_connected;                                	// status of the USB power supply
	bool rc_signal_found_once;
	bool rc_signal_lost_cmd;                       		// true if RC lost mode is commanded
	bool rc_input_blocked;                              // set if RC input should be ignored temporarily
}VM_StatusFlags;


/**
 * \brief parameter table
 */
typedef struct
{
	int autostart_id ;                           // 6001; //SYS_AUTOSTART (for hil)
	int rc_in_off ;                           // 2; //COM_RC_IN_MODE
	int arm_switch_is_button ;                           // 0;//COM_ARM_SWISBTN
	int arm_without_gps ;                           // 1;//COM_ARM_WO_GPS
	int arm_mission_required ;                           // 0;//COM_ARM_MIS_REQ
	int rc_arm_hyst ;                           // 1000;//COM_RC_ARM_HYST
	int mav_type ;                           // 13;//MAV_TYPE
	int system_id ;                           // 1;//MAV_SYS_ID
	int component_id ;                           // 1;//MAV_COMP_ID
	int cbrk_supply_chk ;                           // 0;//CBRK_SUPPLY_CHK
	int cbrk_usb_chk ;                           // 0;//CBRK_USB_CHK
	int cbrk_airspd_chk ;                           // 0;//CBRK_AIRSPD_CHK
	int cbrk_enginefail_chk ;                           // 284953;//CBRK_ENGINEFAIL
	int cbrk_gpsdail_chk ;                           // 0;//CBRK_GPSFAIL
	int cbrk_flightterm_chk ;                           // 121212;//CBRK_FLIGHTTERM
	int cbrk_velposerr_chk ;                           // 0;//CBRK_VELPOSERR
	int nav_dll_act ;                           // 2;//	NAV_DLL_ACT
	int nav_rcl_act ;                           // 2;//	NAV_RCL_ACT
	int dl_loss_t ;                           // 10;//	COM_DL_LOSS_T
	float rc_loss_t ;                           // 0.5;//	COM_RC_LOSS_T
	float rc_stick_ovrde ;                           // 12.0;//	COM_RC_STICK_OV
	int rc_ovrde ;                           // 0;//	COM_RC_OVERRIDE
	int dl_reg_t ;                           // 0;//	COM_DL_REG_T
	float ef_throt ;                           // 0.5;//	COM_EF_THROT
	float ef_c2t ;                           // 0.5;//	COM_EF_C2T
	float ef_time ;                           // 10.0;//	COM_EF_TIME
	int gf_action ;                           // 1;//	GF_ACTION
	int disarm_land ;                           // 3;//	COM_DISARM_LAND
	int low_bat_act ;                           // 1;//	COM_LOW_BAT_ACT
	float of_loss_t ;                           // 0.0;//	COM_OF_LOSS_T
	int obl_act ;                           // 0;//	COM_OBL_ACT
	int obl_rcl_act ;                           // 0;//	COM_OBL_RC_ACT
	float home_h_t ;                           // 5.0;//	COM_HOME_H_T
	float home_v_t ;                           // 10.0;//	COM_HOME_V_T
	int flt_mode_1 ;                           // -1;//	COM_FLTMODE1
	int flt_mode_2 ;                           // -1;//	COM_FLTMODE2
	int flt_mode_3 ;                           // -1;//	COM_FLTMODE3
	int flt_mode_4 ;                           // -1;//	COM_FLTMODE4
	int flt_mode_5 ;                           // -1;//	COM_FLTMODE5
	int flt_mode_6 ;                           // -1;//	COM_FLTMODE6
	float arm_ekf_pos ;                           // 0.5;//	COM_ARM_EKF_POS
	float arm_ekf_vel ;                           // 0.5;//	COM_ARM_EKF_VEL
	float arm_ekf_hgt ;                           // 1.0;//	COM_ARM_EKF_HGT
	float arm_ekf_yaw ;                           // 0.5;//	COM_ARM_EKF_YAW
	float arm_ekf_ab ;                           // 0.0050;//	COM_ARM_EKF_AB
	float arm_ekf_gb ;                           // 0.0009;//	COM_ARM_EKF_GB
	float arm_imu_acc ;                           // 0.7;//	COM_ARM_IMU_ACC
	float arm_imu_gyr ;                           // 0.2;//	COM_ARM_IMU_GYR
	int posctl_navl ;                           // 0;//	COM_POSCTL_NAVL
} VM_Params_t;

/**
 * \brief Navigation modes
 */
typedef struct{
	boolean inPosCtl;
	boolean inRtl;
	boolean inLoiter;
	boolean inManual;
	boolean intakeoff;
}VM_Modes;

/**
 **  \brief VM Application Class
 */
class VM
{
public:
    VM();
    ~VM();

    /**\brief Scheduling Pipe ID */
    CFE_SB_PipeId_t SchPipeId;

    /** \brief Command Pipe ID */
    CFE_SB_PipeId_t CmdPipeId;

    /* Task-related */

    /** \brief Task Run Status */
    uint32 uiRunStatus;

    /* Config table-related */

    /** \brief Config Table Handle */
    CFE_TBL_Handle_t ConfigTblHdl;

    /** \brief Config Table Pointer */
    VM_ConfigTbl_t* ConfigTblPtr;

    /** \brief Input Data subscribed at the beginning of cycle */
    PX4_SensorMagMsg_t SensorMagMsg;
    PX4_SensorGyroMsg_t SensorGyroMsg;
    PX4_BatteryStatusMsg_t BatteryStatusMsg;
//  PX4_SensorPreflightMsg_t SensorPreflightMsg;
    PX4_TelemetryStatusMsg_t TelemetryStatusMsg;
    PX4_SubsystemInfoMsg_t SubsystemInfoMsg;
//  PX4_VehicleGpsPositionMsg_t VehicleGpsPositionMsg;
    PX4_VehicleAttitudeMsg_t VehicleAttitudeMsg;
//  PX4_VehicleGlobalPositionMsg_t VehicleGlobalPositionMsg;
    PX4_VehicleLocalPositionMsg_t VehicleLocalPositionMsg;
    PX4_VehicleLandDetectedMsg_t VehicleLandDetectedMsg;
    PX4_GeofenceResultMsg_t GeofenceResultMsg;
    PX4_MissionResultMsg_t MissionResultMsg;
    PX4_ManualControlSetpointMsg_t ManualControlSetpointMsg;
    PX4_PositionSetpointTripletMsg_t PositionSetpointTripletMsg;
    PX4_OffboardControlModeMsg_t OffboardControlModeMsg;
    PX4_SensorAccelMsg_t SensorAccelMsg;
    PX4_SafetyMsg_t SafetyMsg;
    PX4_SensorCorrectionMsg_t SensorCorrectionMsg;
    PX4_SensorCombinedMsg_t SensorCombinedMsg;
    PX4_VehicleCommandMsg_t VehicleCommandMsg;

    /** \brief Output Data published at the end of cycle */
    PX4_ActuatorArmedMsg_t ActuatorArmedMsg;
    PX4_VehicleGlobalPositionMsg_t VehicleGlobalPositionMsg;
    PX4_VehicleGpsPositionMsg_t VehicleGpsPositionMsg;
    PX4_HomePositionMsg_t HomePositionMsg;
    PX4_CommanderStateMsg_t VehicleManagerStateMsg;
    PX4_MissionMsg_t MissionMsg;
    PX4_LedControlMsg_t LedControlMsg;
    PX4_VehicleStatusMsg_t VehicleStatusMsg;
    PX4_VehicleControlModeMsg_t VehicleControlModeMsg;

    VM_Arming     ArmingSM;
    VM_Navigation NavigationSM;

    VM_Params_t vm_params;

    /** \brief Housekeeping Telemetry for downlink */
    VM_HkTlm_t HkTlm;

    /** \brief True if home position is not set and local variables are not initialization */
    boolean NotInitialized = true;

    /** \brief Timestamps vn boot */
    uint64 VmBootTimestamp = 0;

    VM_StatusFlags status_flags = {};
    boolean ConditionLocalPositionValid;

    boolean vh_prev_landed = true;
    boolean vh_prev_in_flight = false;

	unsigned stick_off_counter = 0;
	unsigned stick_on_counter = 0;
	unsigned last_sp_man_arm_switch = 0;


	bool low_battery_voltage_actions_done = false;
	bool critical_battery_voltage_actions_done = false;
	bool emergency_battery_voltage_actions_done = false;

	uint64 rc_signal_lost_timestamp;


	bool arming_state_changed = false;

	VM_Modes previous_modes{0};





    /************************************************************************/
    /** \brief Vehicle Manager (VM) application entry point
     **
     **  \par Description
     **       Vehicle Manager Task application entry point.  This function
     **       performs app initialization, then waits for the cFE ES Startup
     **       Sync, then executes the main processing loop.
     **
     **  \par Assumptions, External Events, and Notes:
     **       If there is an unrecoverable failure during initialization the
     **       main loop is never executed and the application will exit.
     **
     *************************************************************************/
    void AppMain(void);

    /************************************************************************/
    /** \brief Initialize the Vehicle Manager (VM) application
     **
     **  \par Description
     **       Vehicle Manager application initialization routine. This
     **       function performs all the required startup steps to
     **       initialize (or restore from CDS) VM data structures and get
     **       the application registered with the cFE services so it can
     **       begin to receive command messages and send events.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
     **  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
     **  \retstmt Return codes from #VM_InitEvent               \endcode
     **  \retstmt Return codes from #VM_InitPipe                \endcode
     **  \retstmt Return codes from #VM_InitData                \endcode
     **  \retstmt Return codes from #VM_InitConfigTbl           \endcode
     **  \retstmt Return codes from #OS_TaskInstallDeleteHandler \endcode
     **  \endreturns
     **
     *************************************************************************/
    int32 InitApp(void);

    /************************************************************************/
    /** \brief Initialize Event Services and Event tables
     **
     **  \par Description
     **       This function performs the steps required to setup
     **       cFE Event Services for use by the VM application.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
     **  \retstmt Return codes from #CFE_EVS_Register  \endcode
     **  \endreturns
     **
     *************************************************************************/
    int32 InitEvent(void);

    /************************************************************************/
    /** \brief Initialize global variables used by VM application
     **
     **  \par Description
     **       This function performs the steps required to initialize
     **       the VM application data.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void InitData(void);

    /************************************************************************/
    /** \brief Initialize message pipes
     **
     **  \par Description
     **       This function performs the steps required to setup
     **       initialize the cFE Software Bus message pipes and subscribe to
     **       messages for the VM application.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
     **  \retstmt Return codes from #CFE_SB_CreatePipe        \endcode
     **  \retstmt Return codes from #CFE_SB_SubscribeEx       \endcode
     **  \retstmt Return codes from #CFE_SB_Subscribe         \endcode
     **  \endreturns
     **
     *************************************************************************/
    int32 InitPipe(void);

    /************************************************************************/
    /** \brief Receive and process messages from the scheduler pipe.
     **
     **  \par Description
     **       This function receives and processes messages
     **       for the VM application from the SCH pipe.  This function
     **       will pend for the type defined by iBlocking, allowing
     **       it to wait for messages, i.e. wakeup messages from scheduler.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \param [in]   iBlocking    A #CFE_SB_PEND_FOREVER, #CFE_SB_POLL or
     **                             millisecond timeout
     **
     **  \returns
     **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
     **  \retstmt Return codes from #CFE_SB_RcvMsg            \endcode
     **  \endreturns
     **
     *************************************************************************/
    int32 RcvSchPipeMsg(int32 iBlocking);

    /************************************************************************/
    /** \brief Vehicle Manager Task incoming command processing
     **
     **  \par Description
     **       This function processes incoming commands subscribed
     **       by VM application
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void ProcessCmdPipe(void);

    /************************************************************************/
    /** \brief Vehicle Manager Task application commands
     **
     **  \par Description
     **       This function processes command messages
     **       specific to the VM application
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \param [in]   MsgPtr       A #CFE_SB_Msg_t pointer that
     **                             references the software bus message
     **
     *************************************************************************/
    void ProcessAppCmds(CFE_SB_Msg_t* MsgPtr);

    /************************************************************************/
    /** \brief Sends VM housekeeping message
     **
     **  \par Description
     **       This function sends the housekeeping message
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void ReportHousekeeping(void);
    /************************************************************************/
    /** \brief Sends the ActuatorArmedMsg message.
     **
     **  \par Description
     **       This function publishes the ActuatorArmedMsg message containing
     **       <TODO>
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendActuatorArmedMsg(void);
    /************************************************************************/
    /** \brief Sends the HomePositionMsg message.
     **
     **  \par Description
     **       This function publishes the HomePositionMsg message containing
     **       <TODO>
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendHomePositionMsg(void);
    /************************************************************************/
    /** \brief Sends the VehicleManagerStateMsg message.
     **
     **  \par Description
     **       This function publishes the VehicleManagerStateMsg message containing
     **       <TODO>
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendVehicleManagerStateMsg(void);
    /************************************************************************/
    /** \brief Sends the MissionMsg message.
     **
     **  \par Description
     **       This function publishes the MissionMsg message containing
     **       <TODO>
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendMissionMsg(void);
    /************************************************************************/
    /** \brief Sends the LedControlMsg message.
     **
     **  \par Description
     **       This function publishes the LedControlMsg message containing
     **       <TODO>
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendLedControlMsg(void);
    /************************************************************************/
    /** \brief Sends the VehicleStatusMsg message.
     **
     **  \par Description
     **       This function publishes the VehicleStatusMsg message containing
     **       <TODO>
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendVehicleStatusMsg(void);

    /************************************************************************/
    /** \brief Sends the VehicleControlModeMsg message.
     **
     **  \par Description
     **       This function publishes the VehicleControlModeMsg message
     **       containing <TODO>
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendVehicleControlModeMsg(void);
    void SendVehicleGlobalPositionMsg(void);
    void SendVehicleGpsPositionMsg(void);
    void SendVehicleCommandMsg(void);

    /************************************************************************/
    /** \brief Verify Command Length
     **
     **  \par Description
     **       This function verifies the command message length.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \param [in]   MsgPtr        A #CFE_SB_Msg_t pointer that
     **                              references the software bus message
     **  \param [in]   usExpectedLen The expected length of the message
     **
     **  \returns
     **  TRUE if the message length matches expectations, FALSE if it does not.
     **  \endreturns
     **
     *************************************************************************/
    boolean VerifyCmdLength(CFE_SB_Msg_t* MsgPtr, uint16 usExpectedLen);

    boolean IsVehicleArmed(void);
    void SetHomePosition(void);
    void RcModes(void);
    uint64 TimeElapsed(uint64 *);
    uint64 TimeNow(void);
    void Initialization();
    void FlightSessionInit();
    void UpdateParamsFromTable();
    void Execute();


private:
    /************************************************************************/
    /** \brief Initialize the VM configuration tables.
    **
    **  \par Description
    **       This function initializes VM's configuration tables.  This
    **       includes <TODO>.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
    **  \retstmt Return codes from #CFE_TBL_Register          \endcode
    **  \retstmt Return codes from #CFE_TBL_Load              \endcode
    **  \retstmt Return codes from #VM_AcquireConfigPointers \endcode
    **  \endreturns
    **
    *************************************************************************/
    int32  InitConfigTbl(void);

    /************************************************************************/
    /** \brief Obtain VM configuration tables data pointers.
    **
    **  \par Description
    **       This function manages the configuration tables
    **       and obtains a pointer to their data.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
    **  \endreturns
    **
    *************************************************************************/
    int32  AcquireConfigPointers(void);

public:
    /************************************************************************/
    /** \brief Validate VM configuration table
    **
    **  \par Description
    **       This function validates VM's configuration table
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in]   ConfigTblPtr    A pointer to the table to validate.
    **
    **  \returns
    **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
    **  \endreturns
    **
    *************************************************************************/
    static int32  ValidateConfigTbl(void*);
};

#ifdef __cplusplus
}
#endif 

#endif /* VM_APP_H */

/************************/
/*  End of File Comment */
/************************/
