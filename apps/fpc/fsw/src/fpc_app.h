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
    
#ifndef FPC_APP_H
#define FPC_APP_H

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

#include "px4_msgs.h"

#include "math/Matrix3F3.hpp"
#include "math/Quaternion.hpp"
#include "math/Euler.hpp"
#include "math/Dcm.hpp"
#include "math/Expo.hpp"
#include "math/Functions.hpp"
#include "math/Vector2F.hpp"


#include "fpc_platform_cfg.h"
#include "fpc_mission_cfg.h"
#include "fpc_private_ids.h"
#include "fpc_private_types.h"
#include "fpc_perfids.h"
#include "fpc_msgids.h"
#include "fpc_msg.h"
#include "fpc_events.h"
#include "fpc_tbldefs.h"
#include "fpc_config_utils.h"
#include "fpc_landing_slope.h"
#include "fpc_tecs.h"
#include "fpc_launch_detector.h"
#include "fpc_runway_takeoff.h"
#include "fpc_ecl_l1_pos_controller.hpp"


/************************************************************************
** Local Defines
*************************************************************************/
#define FPC_TIMEOUT_MSEC             	(1000)


/************************************************************************
** Local Structure Definitions
*************************************************************************/
/**
**  \brief FPC Operational Data Structure
*/

class FPC
{
public:
    FPC();
   ~FPC();

    /** \brief CFE Event Table */
    CFE_EVS_BinFilter_t  EventTbl[FPC_EVT_CNT];

    /**\brief Scheduling Pipe ID */
    CFE_SB_PipeId_t  SchPipeId;

    /** \brief Command Pipe ID */
    CFE_SB_PipeId_t  CmdPipeId;

    /** \brief Data Pipe ID */
    CFE_SB_PipeId_t  DataPipeId;

    /* Task-related */

    /** \brief Task Run Status */
    uint32  uiRunStatus;

    /* Config table-related */

    /** \brief Config Table Handle */
    CFE_TBL_Handle_t  ConfigTblHdl;

    /** \brief Config Table Pointer */
    FPC_ConfigTbl_t*  ConfigTblPtr;

    /* Inputs/Outputs */

    /** \brief Input Data from I/O or other apps */
    FPC_InData_t   InData;

    /** \brief Output Data published at the end of cycle */
    FPC_OutData_t  OutData;

    /** \brief Housekeeping Telemetry for downlink */
    FPC_HkTlm_t  HkTlm;

    /** \brief Output Data published at the end of cycle */
    PX4_Position_Control_Status_t         m_PositionControlStatusMsg;
    PX4_TecsStatusMsg_t                   m_PX4_TecsStatusMsg;
    PX4_VehicleAttitudeSetpointMsg_t      m_VehicleAttitudeSetpointMsg;

    /* Input Messages */
    PX4_ManualControlSetpointMsg_t        m_ManualControlSetpointMsg;
    PX4_VehicleControlModeMsg_t           m_VehicleControlModeMsg;
    PX4_PositionSetpointTripletMsg_t      m_PositionSetpointTripletMsg;
    PX4_VehicleStatusMsg_t                m_VehicleStatusMsg;
    PX4_VehicleLandDetectedMsg_t          m_VehicleLandDetectedMsg;
    PX4_VehicleLocalPositionMsg_t         m_VehicleLocalPositionMsg;
    PX4_VehicleGlobalPositionMsg_t        m_VehicleGlobalPositionMsg;
    PX4_AirspeedMsg_t                     m_AirspeedMsg;
    PX4_VehicleAttitudeMsg_t              m_VehicleAttitudeMsg;
    PX4_SensorCombinedMsg_t               m_SensorCombinedMsg;
    PX4_SensorBaroMsg_t                   m_SensorBaroMsg;

    Landingslope                          m_LandingSlope;
    /************************************************************************
    ** External Global Variables
    *************************************************************************/

    /************************************************************************
    ** Global Variables
    *************************************************************************/

    /************************************************************************
    ** Local Variables
    *************************************************************************/

    static constexpr float HDG_HOLD_DIST_NEXT =
        3000.0f; // initial distance of waypoint in front of plane in heading hold mode
    static constexpr float HDG_HOLD_REACHED_DIST =
        1000.0f; // distance (plane to waypoint in front) at which waypoints are reset in heading hold mode
    static constexpr float HDG_HOLD_SET_BACK_DIST = 100.0f; // distance by which previous waypoint is set behind the plane
    static constexpr float HDG_HOLD_YAWRATE_THRESH = 0.15f;	// max yawrate at which plane locks yaw for heading hold mode
    static constexpr float HDG_HOLD_MAN_INPUT_THRESH =
        0.01f; // max manual roll/yaw input from user which does not change the locked heading

    static constexpr uint64 T_ALT_TIMEOUT = 1; // time after which we abort landing if terrain estimate is not valid

    static constexpr float THROTTLE_THRESH =
        0.05f;	///< max throttle from user which will not lead to motors spinning up in altitude controlled modes
    static constexpr float MANUAL_THROTTLE_CLIMBOUT_THRESH =
        0.85f; ///< a throttle / pitch input above this value leads to the system switching to climbout mode
    static constexpr float ALTHOLD_EPV_RESET_THRESH = 5.0f;

    static constexpr float MSL_PRESSURE_MILLIBAR = 1013.25f; ///< standard atmospheric pressure in millibar




    // estimator reset counters
    //Remove these implicit constructors and set the value from the main entry point.
    //If these are zero, you can memset. If they are non-zero, do not use memset.
    int8 m_Pos_Reset_Counter{0};				/* captures the number of times the estimator has reset the horizontal position*/
    int8 m_Alt_Reset_Counter{0};				/* captures the number of times the estimator has reset the altitude state*/



    float	m_Hold_Alt{0.0f};				///< hold altitude for altitude mode */
    float	m_Takeoff_ground_alt{0.0f};			///< ground altitude at which plane was launched */
    float	m_Hdg_Hold_Yaw{0.0f};				///< hold heading for velocity mode */
    osalbool	m_Hdg_Hold_Enabled{FALSE};			///< heading hold enabled */
    osalbool	m_Yaw_Lock_Engaged{FALSE};			///< yaw is locked for heading hold */
    float	m_Althold_Epv{0.0f};				///< the position estimate accuracy when engaging alt hold */
    osalbool	m_Was_In_Deadband{FALSE};			///< wether the last stick input was in althold deadband */

    /* throttle and airspeed states */
    osalbool    _airspeed_valid{FALSE};				///< flag if a valid airspeed estimate exists
    uint64  _airspeed_last_received{0};			///< last time airspeed was received. Used to detect timeouts.
    float   _airspeed{0.0f};
    float   _eas2tas{1.0f};

    math::Matrix3F3 _R_nb;				///< current attitude
    float _roll{0.0f};
    float _pitch{0.0f};
    float _yaw{0.0f};

    float _groundspeed_undershoot{0.0f};			///< ground speed error to min. speed in m/s


    uint64 _control_position_last_called{0};		///< last call of control_position  */

    float	_hold_alt{0.0f};				///< hold altitude for altitude mode */
    float	_takeoff_ground_alt{0.0f};			///< ground altitude at which plane was launched */
    float	_hdg_hold_yaw{0.0f};				///< hold heading for velocity mode */
    boolean	_hdg_hold_enabled{FALSE};			///< heading hold enabled */
    boolean	_yaw_lock_engaged{FALSE};			///< yaw is locked for heading hold */
    float	_althold_epv{0.0f};				///< the position estimate accuracy when engaging alt hold */
    boolean	_was_in_deadband{FALSE};			///< wether the last stick input was in althold deadband */


    boolean _was_in_air{FALSE};				///< indicated wether the plane was in the air in the previous interation*/
    uint64 _time_went_in_air{0};			///< time at which the plane went in the air */

    ECL_L1_Pos_Controller	_l1_control;

    /* Landing */
    boolean _land_noreturn_horizontal{FALSE};
    boolean _land_noreturn_vertical{FALSE};
    boolean _land_stayonground{FALSE};
    boolean _land_motor_lim{FALSE};
    boolean _land_onslope{FALSE};

    float _flare_height{0.0f};				///< estimated height to ground at which flare started */
    float _flare_curve_alt_rel_last{0.0f};
    float _target_bearing{0.0f};

    float _t_alt_prev_valid{0};				///< last terrain estimate which was valid */
    uint64 _time_last_t_alt{0};			///< time at which we had last valid terrain alt */


    /* Takeoff launch detection and runway */
    launchdetection::LaunchDetector _launchDetector;
    launchdetection::LaunchDetectionResult _launch_detection_state{launchdetection::LaunchDetectionResult::LAUNCHDETECTION_RES_NONE};
    uint64 _launch_detection_notify{0};

    runwaytakeoff::RunwayTakeoff _runway_takeoff;

    PX4_PositionSetpoint_t _hdg_hold_prev_wp {};		///< position where heading hold started */
    PX4_PositionSetpoint_t _hdg_hold_curr_wp {};		///< position to which heading hold flies */


    TECS    _tecs;
    uint64 _last_tecs_update{0};

    boolean _reinitialize_tecs{TRUE};				///< indicates if the TECS states should be reinitialized (used for VTOL)
    boolean _is_tecs_running{FALSE};

    uint64 _time_started_landing{0};			///< time at which landing started */

    boolean _last_manual{FALSE};				///< true if the last iteration was in manual mode (used to determine when a reset is needed)


    enum FW_POSCTRL_MODE {
        FW_POSCTRL_MODE_AUTO,
        FW_POSCTRL_MODE_POSITION,
        FW_POSCTRL_MODE_ALTITUDE,
        FW_POSCTRL_MODE_OTHER
    } ControlModeCurrent{FW_POSCTRL_MODE_OTHER};		///< used to check the mode in the last control loop iteration. Use to check if the last iteration was in the same mode.


    /************************************************************************
    ** Local Function Prototypes
    *************************************************************************/

    /************************************************************************/
    /** \brief CFS Fixedwing Position Control Task (FPC) application entry point
    **
    **  \par Description
    **       CFS Fixedwing Position Control Task application entry point.  This function
    **       performs app initialization, then waits for the cFE ES Startup
    **       Sync, then executes the RPR main processing loop.
    **
    **  \par Assumptions, External Events, and Notes:
    **       If there is an unrecoverable failure during initialization the
    **       main loop is never executed and the application will exit.
    **
    *************************************************************************/
    void  AppMain(void);

    /************************************************************************/
    /** \brief Initialize the CFS Fixedwing Position Control (FPC) application
    **
    **  \par Description
    **       Fixedwing Position Control application initialization routine. This
    **       function performs all the required startup steps to
    **       initialize (or restore from CDS) FPC data structures and get
    **       the application registered with the cFE services so it can
    **       begin to receive command messages and send events.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
    **  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
    **  \retstmt Return codes from #FPC_InitEvent               \endcode
    **  \retstmt Return codes from #FPC_InitPipe                \endcode
    **  \retstmt Return codes from #FPC_InitData                \endcode
    **  \retstmt Return codes from #FPC_InitConfigTbl           \endcode
    **  \retstmt Return codes from #OS_TaskInstallDeleteHandler \endcode
    **  \endreturns
    **
    *************************************************************************/
    int32  InitApp(void);

    /************************************************************************/
    /** \brief Initialize Event Services and Event tables
    **
    **  \par Description
    **       This function performs the steps required to setup
    **       cFE Event Services for use by the FPC application.
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
    int32  InitEvent(void);

    /************************************************************************/
    /** \brief Initialize global variables used by FPC application
    **
    **  \par Description
    **       This function performs the steps required to initialize
    **       the FPC application data.
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
    int32  InitData(void);

    /************************************************************************/
    /** \brief Initialize message pipes
    **
    **  \par Description
    **       This function performs the steps required to setup
    **       initialize the cFE Software Bus message pipes and subscribe to
    **       messages for the FPC application.
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
    int32  InitPipe(void);

    /************************************************************************/
    /** \brief Receive and process messages
    **
    **  \par Description
    **       This function receives and processes messages
    **       for the FPC application
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
    int32  RcvMsg(int32 iBlocking);

    /************************************************************************/
    /** \brief Fixedwing Position Control Task incoming data processing
    **
    **  \par Description
    **       This function processes incoming data subscribed
    **       by FPC application
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void  ProcessNewData(void);

    /************************************************************************/
    /** \brief Fixedwing Position Control Task incoming command processing
    **
    **  \par Description
    **       This function processes incoming commands subscribed
    **       by FPC application
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void  ProcessNewCmds(void);

    /************************************************************************/
    /** \brief Fixedwing Position Control Task application commands
    **
    **  \par Description
    **       This function processes command messages
    **       specific to the FPC application
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in]   MsgPtr       A #CFE_SB_Msg_t pointer that
    **                             references the software bus message
    **
    *************************************************************************/
    void  ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr);

    /************************************************************************/
    /** \brief Sends FPC housekeeping message
    **
    **  \par Description
    **       This function sends the housekeeping message
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void  ReportHousekeeping(void);

    /************************************************************************/
    /** \brief Sends FPC output data
    **
    **  \par Description
    **       This function publishes the FPC application output data.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void  SendOutData(void);

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
    osalbool  VerifyCmdLength(CFE_SB_Msg_t* MsgPtr, uint16 usExpectedLen);

    int32 InitConfigTbl(void);
    static int32 ValidateConfigTbl(void* ConfigTblPtr);
    void ProcessNewConfigTbl(void);
    int32 AcquireConfigPointers(void);

    void   UpdateParamsFromTable(void);

    void   SendPositionControlStatusMsg(void);
    void   SendTecsStatusMsg(void);
    void   SendVehicleAttitudeSetpointMsg(void);

    void    UpdateAirspeed(void);
    void    UpdateVehicleAttitude(void);
    boolean ControlPosition(const math::Vector2F &curr_pos, const math::Vector2F &ground_speed,
            const PX4_PositionSetpoint_t &pos_sp_prev, const PX4_PositionSetpoint_t &pos_sp_curr);

    void CalculateGndSpeedUndershoot(const math::Vector2F &curr_pos,
                                     const math::Vector2F &ground_speed,
                                     const PX4_PositionSetpoint_t &pos_sp_prev,
                                     const PX4_PositionSetpoint_t &pos_sp_curr);


    float CalculateTargetAirspeed(float airspeed_demand);

    void TecsUpdatePitchThrottle(float alt_sp, float airspeed_sp,
            float pitch_min_rad, float pitch_max_rad,
            float throttle_min, float throttle_max, float throttle_cruise,
            bool climbout_mode, float climbout_pitch_min_rad,
            PX4_TecsMode_t mode = PX4_TECS_MODE_NORMAL);

    boolean InTakeoffSituation();

    float GetTerrainAltitudeTakeoff(float takeoff_alt, const PX4_VehicleGlobalPositionMsg_t &global_pos);
    void ResetLandingState();
    float GetDemandedAirspeed();

    float GetTecsPitch();

    bool UpdateDesiredAltitude(float dt);
    void DoTakeoffHelp(float *hold_altitude, float *pitch_limit_min);
    void GetWaypointHeadingDistance(float heading, PX4_PositionSetpoint_t &waypoint_prev, PX4_PositionSetpoint_t &waypoint_next, bool flag_init);
    float GetTecsThrust();

    void   Execute(void);

};

#ifdef __cplusplus
}
#endif 

#endif /* FPC_APP_H */

/************************/
/*  End of File Comment */
/************************/
