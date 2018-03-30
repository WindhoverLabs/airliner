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

/************************************************************************
 ** Local Defines
 *************************************************************************/
#define COMMANDER_MONITORING_INTERVAL (10000)
#define COMMANDER_MONITORING_LOOPSPERMSEC (1/(COMMANDER_MONITORING_INTERVAL/1000.0f))
#define STICK_ON_OFF_LIMIT (0.9f)

/************************************************************************
 ** Local Structure Definitions
 *************************************************************************/

/**
 * \brief Vehicle manager status flags
 */
typedef struct {

    /** \brief Indicates if all sensors are initialized */
    boolean condition_system_sensors_initialized;
    /** \brief System in rtl state */
    boolean condition_system_returned_to_home;
    /** \brief Indicates a valid home position (a valid home position is not always a valid launch) */
    boolean condition_home_position_valid;
    /** \brief Satus of the USB power supply */
    boolean usb_connected;
    /** \brief True if RC signal found atleast once */
    boolean rc_signal_found_once;
    /** \brief True if RC lost mode is commanded */
    boolean rc_signal_lost_cmd;
    /** \brief Set if RC input should be ignored temporarily */
    boolean rc_input_blocked;

}VM_StatusFlags;

/**
 * \brief Parameter table
 */
typedef struct
{

    /** \brief Auto-start script index */
    int autostart_id;                                                // 6001
    /** \brief RC control input mode */
    int rc_in_off;                                                      // 2
    /** \brief Arm switch is only a button */
    int arm_switch_is_button;                                           // 0
    /** \brief Allow arming without GPS */
    int arm_without_gps;                                                // 1
    /** \brief Require valid mission to arm */
    int arm_mission_required;                                           // 0
    /** \brief RC input arm/disarm command duration */
    int rc_arm_hyst;                                                 // 1000
    /** \brief Airframe type */
    int mav_type;                                                      // 13
    /** \brief System ID */
    int system_id;                                                      // 1
    /** \brief Component ID */
    int component_id;                                                   // 1
    /** \brief Circuit breaker for power supply check */
    int cbrk_supply_chk;                                                // 0
    /** \brief Circuit breaker for USB link check */
    int cbrk_usb_chk;                                                   // 0
    /** \brief Circuit breaker for airspeed sensor */
    int cbrk_airspd_chk;                                                // 0
    /** \brief Circuit breaker for engine failure detection */
    int cbrk_enginefail_chk;                                       // 284953
    /** \brief Circuit breaker for GPS failure detection */
    int cbrk_gpsdail_chk;                                               // 0
    /** \brief Circuit breaker for flight termination */
    int cbrk_flightterm_chk;                                       // 121212
    /** \brief Circuit breaker for position error check */
    int cbrk_velposerr_chk;                                             // 0
    /** \brief Set data link loss failsafe mode */
    int nav_dll_act;                                                    // 2
    /** \brief Set RC loss failsafe mode */
    int nav_rcl_act;                                                    // 2
    /** \brief Datalink loss time threshold */
    int dl_loss_t;                                                     // 10
    /** \brief RC loss time threshold */
    float rc_loss_t;                                                  // 0.5
    /** \brief RC stick override threshold */
    float rc_stick_ovrde;                                            // 12.0
    /** \brief Enable RC stick override of auto modes */
    int rc_ovrde;                                                       // 0
    /** \brief Datalink regain time threshold */
    int dl_reg_t;                                                       // 0
    /** \brief Engine Failure Throttle Threshold */
    float ef_throt;                                                   // 0.5
    /** \brief Engine Failure Current/Throttle Threshold */
    float ef_c2t;                                                     // 0.5
    /** \brief Engine Failure Time Threshold */
    float ef_time;                                                   // 10.0
    /** \brief Geofence violation action */
    int gf_action;                                                      // 1
    /** \brief Time-out for auto disarm after landing */
    int disarm_land;                                                    // 3
    /** \brief Battery failsafe mode */
    int low_bat_act;                                                    // 1
    /** \brief Time-out to wait when offboard connection is lost before triggering offboard lost action */
    float of_loss_t;                                                  // 0.0
    /** \brief Set offboard loss failsafe mode */
    int obl_act;                                                        // 0
    /** \brief Set offboard loss failsafe mode when RC is available */
    int obl_rcl_act;                                                    // 0
    /** \brief Home set horizontal threshold */
    float home_h_t;                                                   // 5.0
    /** \brief Home set vertical threshold */
    float home_v_t;                                                  // 10.0
    /** \brief First flightmode slot (1000-1160) */
    int flt_mode_1;                                                    // -1
    /** \brief Second flightmode slot (1160-1320) */
    int flt_mode_2;                                                    // -1
    /** \brief Third flightmode slot (1320-1480) */
    int flt_mode_3;                                                    // -1
    /** \brief Fourth flightmode slot (1480-1640) */
    int flt_mode_4;                                                    // -1
    /** \brief Fifth flightmode slot (1640-1800) */
    int flt_mode_5;                                                    // -1
    /** \brief Sixth flightmode slot (1800-2000) */
    int flt_mode_6;                                                    // -1
    /** \brief Maximum EKF position innovation test ratio that will allow arming */
    float arm_ekf_pos;                                               // 0.5;
    /** \brief Maximum EKF velocity innovation test ratio that will allow arming */
    float arm_ekf_vel;                                                // 0.5
    /** \brief Maximum EKF height innovation test ratio that will allow arming */
    float arm_ekf_hgt;                                                // 1.0
    /** \brief Maximum EKF yaw innovation test ratio that will allow arming */
    float arm_ekf_yaw;                                                // 0.5
    /** \brief Maximum value of EKF accelerometer delta velocity bias estimate that will allow arming */
    float arm_ekf_ab;                                              // 0.0050
    /** \brief Maximum value of EKF gyro delta angle bias estimate that will allow arming */
    float arm_ekf_gb;                                              // 0.0009
    /** \brief Maximum accelerometer inconsistency between IMU units that will allow arming */
    float arm_imu_acc;                                                // 0.7
    /** \brief Maximum rate gyro inconsistency between IMU units that will allow arming */
    float arm_imu_gyr;                                                // 0.2
    /** \brief Position control navigation loss response */
    int posctl_navl;                                                    // 0

}VM_Params_t;

/**
 * \brief RC navigation mode switched
 */
typedef struct {

    /** \brief Position control is selected */
    boolean inPosCtl;
    /** \brief Return to launch is selected  */
    boolean inRtl;
    /** \brief Auto loiter is selected  */
    boolean inLoiter;
    /** \brief Manual is selected  */
    boolean inManual;
    /** \brief Takeoff is selected  */
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
    /** \brief Magnetometer message */
    PX4_SensorMagMsg_t SensorMagMsg;
    /** \brief Gyro message */
    PX4_SensorGyroMsg_t SensorGyroMsg;
    /** \brief Battery status message */
    PX4_BatteryStatusMsg_t BatteryStatusMsg;
    /** \brief Telemetry status message */
    PX4_TelemetryStatusMsg_t TelemetryStatusMsg;
    /** \brief Subsystem information message */
    PX4_SubsystemInfoMsg_t SubsystemInfoMsg;
    /** \brief Vehicle attitude message  */
    PX4_VehicleAttitudeMsg_t VehicleAttitudeMsg;
    /** \brief Vehicle local position messsage */
    PX4_VehicleLocalPositionMsg_t VehicleLocalPositionMsg;
    /** \brief Vehicle land detected message */
    PX4_VehicleLandDetectedMsg_t VehicleLandDetectedMsg;
    /** \brief Mission result message */
    PX4_MissionResultMsg_t MissionResultMsg;
    /** \brief Manual control setpoint message */
    PX4_ManualControlSetpointMsg_t ManualControlSetpointMsg;
    /** \brief position setpoint triplet message */
    PX4_PositionSetpointTripletMsg_t PositionSetpointTripletMsg;
    /** \brief Sensor accelerometer message */
    PX4_SensorAccelMsg_t SensorAccelMsg;
    /** \brief Safety message */
    PX4_SafetyMsg_t SafetyMsg;
    /** \brief Sensor correction message */
    PX4_SensorCorrectionMsg_t SensorCorrectionMsg;
    /** \brief Sensor combined message */
    PX4_SensorCombinedMsg_t SensorCombinedMsg;
    /** \brief Vehicle commanding message */
    PX4_VehicleCommandMsg_t VehicleCommandMsg;
    /** \brief Vehicle global position message */
    PX4_VehicleGlobalPositionMsg_t VehicleGlobalPositionMsg;
    /** \brief Vehicle gps position message */
    PX4_VehicleGpsPositionMsg_t VehicleGpsPositionMsg;

    /** \brief Output Data published at the end of cycle */
    /** \brief Actuator armed message  */
    PX4_ActuatorArmedMsg_t ActuatorArmedMsg;
    /** \brief Home position message */
    PX4_HomePositionMsg_t HomePositionMsg;
    /** \brief Vehicle manager state message */
    PX4_CommanderStateMsg_t VehicleManagerStateMsg;
    /** \brief Mission message */
    PX4_MissionMsg_t MissionMsg;
    /** \brief Led control message */
    PX4_LedControlMsg_t LedControlMsg;
    /** \brief Vehicle status message */
    PX4_VehicleStatusMsg_t VehicleStatusMsg;
    /** \brief Vehicle control mode message */
    PX4_VehicleControlModeMsg_t VehicleControlModeMsg;

    /** \brief Arming state machine handle */
    VM_Arming ArmingSM;
    /** \brief Navigation state machine handle */
    VM_Navigation NavigationSM;
    /** \brief param variable */
    VM_Params_t vm_params;

    /** \brief Housekeeping Telemetry for downlink */
    VM_HkTlm_t HkTlm;

    /** \brief True if home position is not set and local variables are not initialization */
    boolean NotInitialized = true;

    /** \brief Timestamps vm at boot */
    uint64 VmBootTimestamp = 0;
    /** \brief status flag variable */
    VM_StatusFlags status_flags = {};
    /** \brief True if local position is valid */
    boolean ConditionLocalPositionValid;
    /** \brief True if previously landed */
    boolean vh_prev_landed = true;
    /** \brief True if previously in flight */
    boolean vh_prev_in_flight = false;
    /** \brief Records a count when vehicle is disarmed with stick  */
    unsigned stick_off_counter = 0;
    /** \brief Records a count when vehicle is armed with stick */
    unsigned stick_on_counter = 0;
    /** \brief Arming switch in manual control setpoint message  */
    unsigned last_sp_man_arm_switch = 0;
    /** \brief True when vehicle's battery is low and a contingency action is implemented */
    boolean low_battery_voltage_actions_done = false;
    /** \brief True when vehicle's battery is critical and a contingency action is implemented */
    boolean critical_battery_voltage_actions_done = false;
    /** \brief True when vehicle's battery is dangerously low and a contingency action is implemented */
    boolean emergency_battery_voltage_actions_done = false;
    /** \brief Timestamps the moment rc signal is lost */
    uint64 rc_signal_lost_timestamp;
    /** \brief True when arming status changes with the vehicle */
    boolean arming_state_changed = false;
    /** \brief An instance rc navigation mode  */
    VM_Modes previous_modes {0};

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
    /************************************************************************/
    /** \brief Sends the VehicleCommandMsg message.
     **
     **  \par Description
     **       This function publishes the VehicleCommandMsg message
     **       containing <TODO>
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
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
    /************************************************************************/
    /** \brief Time Elapsed
     **
     **  \par Description
     **       Returns elaped time.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \param [in]   TimePtr    A pointer to then time.
     **
     **  \returns
     **   Time in unsigned integer format.
     **  \endreturns
     **
     *************************************************************************/
    uint64 TimeElapsed(uint64 * TimePtr);
    /************************************************************************/
    /** \brief Time
     **
     **  \par Description
     **       Returns current time.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **  \returns
     **   Time in unsigned integer format.
     **  \endreturns
     **
     *************************************************************************/
    uint64 TimeNow(void);
    /************************************************************************/
    /** \brief Vehicle Armed
     **
     **  \par Description
     **       True is Vehicle is armed
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **  \returns
     **   TRUE if the armed attribute in actuator armed message is true, FALSE if it is not.
     **  \endreturns
     **
     *************************************************************************/
    boolean IsVehicleArmed(void);
    /************************************************************************/
    /** \brief Set Home Position
     **
     **  \par Description
     **       Sets home position at initialization, arming and launch events
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void FlightSessionInit(void);
    /************************************************************************/
    /** \brief Set Home Position
     **
     **  \par Description
     **       Sets home position at initialization, arming and launch events
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SetHomePosition(void);
    /************************************************************************/
    /** \brief RC Control Navigation Modes
     **
     **  \par Description
     **       Procedure to how rc will switch navigation modes such as position
     **       hold, auto loiter, auto rtl and manual using manual control setpoint
     **       message. State transitions are invoked by this method.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void RcModes(void);
    /************************************************************************/
    /** \brief Vehicle Manager Initialization Task
     **
     **  \par Description
     **       Messages are initialized to their defaults and housekeeping.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void Initialization(void);
    /************************************************************************/
    /** \brief Vehicle Manager Maintenance Task
     **
     **  \par Description
     **       Executes every time a new message is available, verifies message
     **       validity and updates messages and publishes them to software bus.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void Execute(void);
    /************************************************************************/
    /** \brief TODO
     **
     **  \par Description
     **       Executes every time a new message is available, verifies message
     **       validity and updates messages and publishes them to software bus.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    const char* GetNavStateAsString(uint32);
    /************************************************************************/
    /** \brief Updates application params from param table
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void UpdateParamsFromTable(void);

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
    int32 InitConfigTbl(void);

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
    int32 AcquireConfigPointers(void);

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
    static int32 ValidateConfigTbl(void*);
};

#ifdef __cplusplus
}
#endif 

#endif /* VM_APP_H */

/************************/
/*  End of File Comment */
/************************/
