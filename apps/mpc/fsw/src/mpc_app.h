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

#ifndef MPC_APP_H
#define MPC_APP_H

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

#include "mpc_platform_cfg.h"
#include "mpc_mission_cfg.h"
#include "mpc_perfids.h"
#include "mpc_msgids.h"
#include "mpc_msg.h"
#include "mpc_events.h"
#include "mpc_tbldefs.h"
#include "px4_msgs.h"
#include "math/Matrix3F3.hpp"
#include "geo/geo.h"
#include "math/Derivative.hpp"
#include "math/Vector2F.hpp"
#include "systemlib/hysteresis.h"
#include "math/filters/LowPassFilter2p.hpp"

/************************************************************************
 ** Local Defines
 *************************************************************************/


/** \brief Pipe depth for the data pipe
*/
#define MPC_DATA_PIPE_DEPTH            (8)

/** \brief Pipe name for the data pipe */
#define MPC_DATA_PIPE_NAME             ("MPC_DATA_PIPE")



/************************************************************************
 ** Local Structure Definitions
 *************************************************************************/

/**
 **  \brief MPC Application Class
 */
class MPC
{
public:
    MPC();
    ~MPC();

    /**\brief Scheduling Pipe ID */
    CFE_SB_PipeId_t SchPipeId;

    /** \brief Command Pipe ID */
    CFE_SB_PipeId_t CmdPipeId;

    /** \brief Data Pipe ID */
    CFE_SB_PipeId_t DataPipeId;


    /* Task-related */

    /** \brief Task Run Status */
    uint32 uiRunStatus;

    /* Config table-related */

    /** \brief Config Table Handle */
    CFE_TBL_Handle_t ConfigTblHdl;

    /** \brief Config Table Pointer */
    MPC_ConfigTbl_t* ConfigTblPtr;


    /* Output Messages */
    /** \brief Housekeeping Telemetry for downlink */
    MPC_HkTlm_t HkTlm;

    /** \brief Diagnositc Telemetry for downlink */
    MPC_DiagPacket_t DiagTlm;

    /** \brief Output Data published at the end of cycle */
    PX4_VehicleAttitudeSetpointMsg_t      m_VehicleAttitudeSetpointMsg;
    PX4_VehicleLocalPositionSetpointMsg_t m_VehicleLocalPositionSetpointMsg;

    /* Input Messages */
    PX4_ControlStateMsg_t                 m_ControlStateMsg;
    PX4_ManualControlSetpointMsg_t        m_ManualControlSetpointMsg;
    PX4_HomePositionMsg_t                 m_HomePositionMsg;
    PX4_VehicleControlModeMsg_t           m_VehicleControlModeMsg;
    PX4_PositionSetpointTripletMsg_t      m_PositionSetpointTripletMsg;
    PX4_VehicleStatusMsg_t                m_VehicleStatusMsg;
    PX4_VehicleLandDetectedMsg_t          m_VehicleLandDetectedMsg;
    PX4_VehicleLocalPositionMsg_t         m_VehicleLocalPositionMsg;


    /* Reset counters */
    /** \brief When set to a non-zero integer, m_PositionSetpoint[2] will get set to m_VehicleLocalPositionMsg.Z */
    uint8 m_ResetCounterZ;

    /** \brief When set to a non-zero integer, m_PositionSetpoint[0] and m_PositionSetpoint[1] will get set to
     *         m_VehicleLocalPositionMsg.X and m_VehicleLocalPositionMsg.Y respectively. */
    uint8 m_ResetCounterXy;

    /** \brief When set to a non-zero integer, m_VehicleAttitudeSetpointMsg.YawBody will get set to the Euler
     *         yaw component of the m_ControlStateMsg.QuatResetCounter quaternion. */
    uint8 m_HeadingResetCounter;


    /* Control variables used for altitude, position, and yaw hold */
    /** \brief Vehicle local position. */
    math::Vector3F m_Position;

    /** \brief Vehicle local position setpoint. */
    math::Vector3F m_PositionSetpoint;

    /** \brief Vehicle local position velocity. */
    math::Vector3F m_Velocity;

    /** \brief Vehicle local position velocity on the previous step. */
    math::Vector3F m_VelocityPrevious;

    /** \brief Vehicle local position velocity setpoint. */
    math::Vector3F m_VelocitySetpoint;

    /** \brief Vehicle local position velocity setpoint on the previous step. */
    math::Vector3F m_VelocitySetpointPrevious;

    /** \brief Derivative of the current vehicle local position velocity. */
    math::Vector3F m_VelocityErrD;

    /** \brief Current setpoint of the triplet. */
    math::Vector3F m_CurrentPositionSetpoint;  /**< current setpoint of the triplets */

    /** \brief Previous setpoint position of the triplet. */
    math::Vector3F m_PreviousPositionSetpoint;

    /** \brief Rotation matrix setpoint used to determine the Vehicle Attitude Setpoint quaternion. */
    math::Matrix3F3 m_RSetpoint;

    /** \brief Rotation matrix from attitude in quaternions. */
    math::Matrix3F3 m_Rotation;

    /** \brief Thrust vector used to determine the Vehicle Local Position Setpoint acceleration. */
    math::Vector3F m_ThrustInt;

    /** \brief Velocity in Z that agrees with the position rate. */
    float m_DerivativeZ;

    /** \brief Yaw Euler angle. */
    float m_Yaw;

    /** \brief Current yaw offset in manual mode. */
    float m_ManYawOffset;


    /* State variables */
    /** \brief Briefly set to TRUE when MPC initially enters automatic mode, causing the state machine
     *         to execute specific behavior on state entry.  This flag is reset back to FALSE immediately
     *         after the entry function has executed. */
    osalbool m_ModeAuto;

    /** \brief When set to TRUE, MPC will hold position, unless the pilot has applied horizontal control
     *         stick deflections beyond the deadband.  */
    osalbool m_PositionHoldEngaged;

    /** \brief When set to TRUE, MPC will hold position, unless the pilot has applied vertical control
     *         stick deflections beyond the deadband.  */
    osalbool m_AltitudeHoldEngaged;

    /** \brief When set to TRUE, MPC will hold position.  */
    osalbool m_RunPosControl;

    /** \brief When set to TRUE, MPC will hold altitude.  */
    osalbool m_RunAltControl;

    /** \brief When set to TRUE, MPC will set horizontal components of m_PositionSetpoint to
     *         m_Position. */
    osalbool m_ResetPositionSetpoint;

    /** \brief When set to TRUE, MPC will set vertical components of m_PositionSetpoint to
     *         m_Position. */
    osalbool m_ResetAltitudeSetpoint;

    /** \brief When set to TRUE, MPC will reset both the vertical and horizontal components of
     *         m_PositionSetpoint to m_Position. */
    osalbool m_DoResetAltPos;

    /** \brief Set by the #m_VehicleLandDetectedMsg.Landed message.  Indicates the vehicle has
     *         landed.   */
    osalbool m_WasLanded;

    /** \brief Resets thrust (vertical component) integrals to 0.0 when set to TRUE. */
    osalbool m_ResetIntZ;

    /** \brief Resets the integrals of the horizontal components to 0.0 when set to TRUE. */
    osalbool m_ResetIntXY;

    /** \brief Resets yaw setpoint to current position when set to TRUE. */
    osalbool m_ResetYawSetpoint;

    /** \brief When m_VehicleControlModeMsg.ControlOffboardEnabled is set and this is FALSE,
     *         the horizontal component of m_PositionSetpoint is set to the horizontal
     *         component of m_Position. */
    osalbool m_HoldOffboardXY;

    /** \brief When m_VehicleControlModeMsg.ControlOffboardEnabled is set and this is FALSE,
     *         the vertical component of m_PositionSetpoint is set to the vertical
     *         component of m_Position. */
    osalbool m_HoldOffboardZ;

    /** \brief Indicates that the vehicle is currently in takeoff (automatic or manual), and
     *         applying a special velocity setpoint limitation for smooth takeoff */
    osalbool m_InTakeoff;          /**< flag for smooth velocity setpoint takeoff ramp */

    /** \brief Indicates with the latitude and longitude have been updated (are "fresh").  When
     *         set to TRUE, sets the horizontal components of the Current Position Setpoint
     *         equal to #m_Position. */
    osalbool m_TripletLatLonFinite;

    /** \brief Set to #m_VehicleLocalPositionMsg.RefTimestamp.  When a new Vehicle Local Position
     *         message has been received, MPC detects the new RefTimestamp, causing it to run the
     *         #UpdateRef function. */
    uint64 m_RefTimestamp;

    /** \brief Local projection reference. */
    struct map_projection_reference_s m_RefPos;

    /** \brief Vehicle local position reference altitude. */
    float m_RefAlt;

    /** \brief TRUE when the reference altitude is defined in a global reference frame. */
    osalbool m_RefAltIsGlobal;

    /** \brief Home yaw angle present when vehicle was taking off (euler). */
    float m_YawTakeoff;

    /* Velocity controller PIDs */
    /** \brief Position Setpoint PID Proportional (gain) value. */
    math::Vector3F m_PosP;

    /** \brief PID Proportional (gain) value.  Used to calculate the thrust component of the
     *         Position Setpoint. */
    math::Vector3F m_VelP;

    /** \brief PID Integral value.  Used to calculate the thrust component of the
     *         Position Setpoint. */
    math::Vector3F m_VelI;

    /** \brief PID Differential value.  Used to calculate the thrust component of the
     *         Position Setpoint. */
    math::Vector3F m_VelD;

    /** \brief Derivative of the velocity X component.  Used to calculate the thrust component of the
     *         Position Setpoint. */
    Derivative m_VelXDeriv;

    /** \brief Derivative of the velocity Y component.  Used to calculate the thrust component of the
     *         Position Setpoint. */
    Derivative m_VelYDeriv;

    /** \brief Derivative of the velocity Z component.  Used to calculate the thrust component of the
     *         Position Setpoint. */
    Derivative m_VelZDeriv;


    /* Limit variables */
    /** \brief Horizontal acceleration limit applied in manual mode. */
    float m_AccelerationStateLimitXY;

    /** \brief Vertical acceleration limit applied in manual mode. */
    float m_AccelerationStateLimitZ;

    /** \brief Horizontal jerk limit in manual mode dependent on stick input. */
    float m_ManualJerkLimitXY;

    /** \brief Vertical jerk limit in manual mode. */
    float m_ManualJerkLimitZ;

    /** \brief Equal to XY_VEL_MAX except in auto mode when close to target.  MPC will reduce this value
     *         when close to the target in preparation for a smooth stop. */
    float m_VelMaxXy;

    /** \brief Velocity limit value which gets ramped up during takeoff. */
    float m_TakeoffVelLimit;


    /* Stick input variables */
    /** \brief Manual pitch input low pass filter. */
    math::LowPassFilter2p m_FilterManualPitch;

    /** \brief Manual roll input low pass filter. */
    math::LowPassFilter2p m_FilterManualRoll;

    /** \brief The previous value of the stick input.  For manual controlled mode to detect
     *         direction change. */
    math::Vector2F m_StickInputXyPrev;

    /** \brief Defines what the user intends to do derived from horizontal component of the stick input. */
    ManualStickInput m_UserIntentionXY;

    /** \brief Defines what the user intends to do derived from vertical component of the stick input. */
    ManualStickInput m_UserIntentionZ;

    /** \brief Used for braking. */
    systemlib::Hysteresis m_ManualDirectionChangeHysteresis;

    /************************************************************************/
    /** \brief Multicopter Position Control (MPC) application entry point
     **
     **  \par Description
     **       Multicopter Position Control Task application entry point.  This function
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
    /** \brief Initialize the Multicopter m_Position Control (MPC) application
     **
     **  \par Description
     **       Multicopter Position Control application initialization routine. This
     **       function performs all the required startup steps to
     **       initialize (or restore from CDS) MPC data structures and get
     **       the application registered with the cFE services so it can
     **       begin to receive command messages and send events.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
     **  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
     **  \retstmt Return codes from #MPC_InitEvent               \endcode
     **  \retstmt Return codes from #MPC_InitPipe                \endcode
     **  \retstmt Return codes from #MPC_InitData                \endcode
     **  \retstmt Return codes from #MPC_InitConfigTbl           \endcode
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
     **       cFE Event Services for use by the MPC application.
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
    /** \brief Initialize global variables used by MPC application
     **
     **  \par Description
     **       This function performs the steps required to initialize
     **       the MPC application data.
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
     **       messages for the MPC application.
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
     **       for the MPC application from the SCH pipe.  This function
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
    /** \brief Multicopter Position Control Task incoming data processing
     **
     **  \par Description
     **       This function processes incoming data subscribed
     **       by MPC application
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  \retcode #TRUE   \retdesc \copydoc TRUE    \endcode
     **  \retcode #FALSE  \retdesc \copydoc FALSE   \endcode
     **  \retstmt Boolean TRUE indicates messages were successfully received
     **           with no errors.  \endcode
     **  \endreturns
     *************************************************************************/
    osalbool ProcessDataPipe(void);

    /************************************************************************/
    /** \brief Multicopter Position Control Task incoming command processing
     **
     **  \par Description
     **       This function processes incoming commands subscribed
     **       by MPC application
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void ProcessCmdPipe(void);

    /************************************************************************/
    /** \brief Multicopter Position Control Task application commands
     **
     **  \par Description
     **       This function processes command messages
     **       specific to the MPC application
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
    /** \brief Sends MPC housekeeping message
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
    /** \brief Sends MPC Diagnostic message
     **
     **  \par Description
     **       This function sends the diagnostic message
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void ReportDiagnostic(void);

    /************************************************************************/
    /** \brief Sends the VehicleAttitudeSetpointMsg message.
     **
     **  \par Description
     **       This function publishes the VehicleAttitudeSetpointMsg message containing
     **       <TODO>
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendVehicleAttitudeSetpointMsg(void);

    /************************************************************************/
    /** \brief Sends the VehicleLocalPosition message.
     **
     **  \par Description
     **       This function publishes the VehicleLocalPosition message containing
     **       <TODO>
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendVehicleLocalPositionSetpointMsg(void);

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
     **
     **  \returns
     **  \retcode #TRUE   \retdesc \copydoc TRUE    \endcode
     **  \retcode #FALSE  \retdesc \copydoc FALSE   \endcode
     **  \retstmt Boolean TRUE if the message length matches expectations,
     **           FALSE if it does not.  \endcode
     **  \endreturns
     **  \returns
     **
     *************************************************************************/
    osalbool VerifyCmdLength(CFE_SB_Msg_t* MsgPtr, uint16 usExpectedLen);

    /************************************************************************/
    /** \brief Initialize the MPC configuration tables.
    **
    **  \par Description
    **       This function initializes MPC's configuration tables.  This
    **       includes <TODO>.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
    **  \retstmt Return codes from #CFE_TBL_Register          \endcode
    **  \retstmt Return codes from #CFE_TBL_Load              \endcode
    **  \retstmt Return codes from #MPC_AcquireConfigPointers \endcode
    **  \endreturns
    **
    *************************************************************************/
    int32  InitConfigTbl(void);

    /************************************************************************/
    /** \brief Obtain MPC configuration tables data pointers.
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

    /************************************************************************/
    /** \brief Process Control State Message
    **
    **  \par Description
    **       This function calculates yaw and Euler angles from the current
    **       control state message.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void ProcessControlStateMsg(void);

    /************************************************************************/
    /** \brief Process Vehicle Local Position message
    **
    **  \par Description
    **       This function performs Position Setpoint resets, as directed by
    **       the #VehicleLocalPositionMsg message.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void ProcessVehicleLocalPositionMsg(void);

    /************************************************************************/
    /** \brief Process Position Setpoint Triplet message
    **
    **  \par Description
    **       This function determines if the setpoint positions are valid and
    **       sets the Valid flag to FALSE when they are invalid.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void ProcessPositionSetpointTripletMsg(void);

    /************************************************************************/
    /** \brief The main cyclic Execution function.
    **
    **  \par Description
    **       This is the function called in the main cyclic loop of the MPC
    **       application.  This is triggered by the #MPC_WAKEUP_MID function
    **       and performs the bulk of the MPC functionality.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void Execute(void);

    /************************************************************************/
    /** \brief Update the reference position
    **
    **  \par Description
    **       This function updates the reference position.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void UpdateRef(void);

    /************************************************************************/
    /** \brief Update velocity derivative.
    **
    **  \par Description
    **       This function updates the velocity and the velocity derivative
    **       independent of the current flight mode.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void UpdateVelocityDerivative(float dt);

    /************************************************************************/
    /** \brief Execute the control functions.
    **
    **  \par Description
    **       This function either executes the #ControlManual or the
    **       #ControlNonManual function, depending on the current flight mode.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void DoControl(float dt);

    /************************************************************************/
    /** \brief Generate attitude setpoint
    **
    **  \par Description
    **       This function generates the #m_VehicleAttitudeSetpointMsg
    **       message.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void GenerateAttitudeSetpoint(float dt);

    /************************************************************************/
    /** \brief Control Manual
    **
    **  \par Description
    **       This function performs the tasks required to control manual
    **       flight.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void ControlManual(float dt);

    /************************************************************************/
    /** \brief Control Non-Manual
    **
    **  \par Description
    **       This function performs the tasks required to control non-manual
    **       (automatic) flight.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void ControlNonManual(float dt);

    /************************************************************************/
    /** \brief Throttle Curve
    **
    **  \par Description
    **       This function maps #m_ManualControlSetpointMsg.Z (throttle) to
    **       0.0 to 1.0, with 0.5 mapping to the hover throttle.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  The mapped throttle value
    **  \endreturns
    **
    *************************************************************************/
    float ThrottleCurve(float ctl, float ctr);

    /************************************************************************/
    /** \brief Reset Altitude Setpoint
    **
    **  \par Description
    **       When #m_ResetAltitudeSetpoint is set to TRUE, this function will
    **       reset #m_PositionSetpoint[2] (altitude) to #m_Position[2].
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void ResetAltSetpoint(void);

    /************************************************************************/
    /** \brief Reset Position Setpoint
    **
    **  \par Description
    **       When m_ResetPositionSetpoint is set to TRUE, this function will
    **       reset the horizontal compoments of #m_PositionSetpoint to
    **       #m_Position.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void ResetPosSetpoint(void);

    /************************************************************************/
    /** \brief Control Position
    **
    **  \par Description
    **       If #m_VehicleControlModeMsg.ControlClimbRateEnabled,
    **       #m_VehicleControlModeMsg.ControlVelocityEnabled, or
    **       #m_VehicleControlModeMsg.ControlAccelerationEnabled are set to
    **       TRUE, this function will call the #CalculateThrustSetpoint
    **       function.  Otherwise, this will set #m_ResetIntZ to TRUE and
    **       reset the thrust integral to to 0.0.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void ControlPosition(float dt);

    /************************************************************************/
    /** \brief Control Offboard
    **
    **  \par Description
    **       This function is called when
    **       #m_VehicleControlModeMsg.ControlOffboardEnabled is set to TRUE,
    **       allowing the vehicle to be controlled by
    **       #m_PositionSetpointTripletMsg received from an external source.
    **
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void ControlOffboard(float dt);

    /************************************************************************/
    /** \brief Control Auto
    **
    **  \par Description
    **       This function performs all tasks required for MPC provided
    **       automatic control.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void ControlAuto(float dt);

    /************************************************************************/
    /** \brief Calculate Velocity Setpoint
    **
    **  \par Description
    **       This function calculates the #m_VelocitySetpoint vector.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void CalculateVelocitySetpoint(float dt);

    /************************************************************************/
    /** \brief Calculate Thrust Setpoint
    **
    **  \par Description
    **       This function calculates the ThrustSp vector and generates the
    **       #m_VehicleAttitudeSetpointMsg message.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void CalculateThrustSetpoint(float dt);

    /************************************************************************/
    /** \brief Get horizontal cruising speed.
    **
    **  \par Description
    **       This function returns the horizontal cruising speed, from
    **       either #m_PositionSetpointTripletMsg.Current.CruisingSpeed when
    **       greater than 0, or from the #XY_CRUISE configuration parameter.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  The horizontal cruising speed.
    **  \endreturns
    **
    *************************************************************************/
    float GetCruisingSpeedXY(void);

    /************************************************************************/
    /** \brief Update internal parameters from table.
    **
    **  \par Description
    **       This function function will update some internal parameters
    **       from the newly loaded CFE table.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void UpdateParamsFromTable(void);

    /************************************************************************/
    /** \brief Apply an altitude limit
    **
    **  \par Description
    **       This function applies an altitude limit, when present.  The
    **       altitude limit is defined in the #m_VehicleLandDetectedMsg.AltMax
    **       message.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void LimitAltitude(void);

    /************************************************************************/
    /** \brief In Auto Takeoff
    **
    **  \par Description
    **       This function returns TRUE if we are in an auto takeoff mode.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  \retcode #TRUE   \retdesc \copydoc TRUE    \endcode
    **  \retcode #FALSE  \retdesc \copydoc FALSE   \endcode
    **  \retstmt Boolean TRUE indicates vehicle is in auto takeoff
    **           mode  \endcode
    **  \endreturns
    **
    *************************************************************************/
    osalbool InAutoTakeoff(void);

    /************************************************************************/
    /** \brief Apply Velocity Setpoint Slew Rate
    **
    **  \par Description
    **       This function applies a slew rate to reach the target velocity
    **       setpoint.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void ApplyVelocitySetpointSlewRate(float dt);

    /************************************************************************/
    /** \brief Get velocity close
    **
    **  \par Description
    **       Get velocity close to current that depends on angle between
    **       prev-current and current-next line.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  Close velocity
    **  \endreturns
    **
    *************************************************************************/
    float GetVelClose(const math::Vector2F &UnitPrevToCurrent, const math::Vector2F &UnitCurrentToNext);

    /************************************************************************/
    /** \brief Set manual vertical acceleration
    **
    **  \par Description
    **       In manual altitude control, set the vertical acceleration based
    **       on whether the user is accelerating or braking.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void SetManualAccelerationZ(float &max_acceleration, const float stick_z, const float dt);

    /************************************************************************/
    /** \brief Set manual horizontal acceleration
    **
    **  \par Description
    **       In manual altitude control, set the horizontal acceleration based
    **       on whether the user is accelerating or braking.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void SetManualAccelerationXY(math::Vector2F &stick_xy, const float dt);

    /************************************************************************/
    /** \brief Pilot is commanding a manual takeoff
    **
    **  \par Description
    **       This function returns true if the manual control setpoint Z
    **       (throttle) is greater than 65%.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  \retcode #TRUE   \retdesc \copydoc TRUE    \endcode
    **  \retcode #FALSE  \retdesc \copydoc FALSE   \endcode
    **  \retstmt Boolean TRUE indicates pilot is commanding a manual
    **           takeoff  \endcode
    **  \endreturns
    **
    *************************************************************************/
    osalbool ManualWantsTakeoff(void);

    /************************************************************************/
    /** \brief Update XY PIDs.
    **
    **  \par Description
    **       This function updates the horizontal (XY) position and velocity
    **       PIDs, both member functions and configuration table.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  \retcode #TRUE   \retdesc \copydoc TRUE    \endcode
    **  \retcode #FALSE  \retdesc \copydoc FALSE   \endcode
    **  \retstmt Boolean TRUE indicates the command was valid and the change
    **           was successful. \endcode
    **  \endreturns
    **
    *************************************************************************/
    osalbool UpdateXyPids(MPC_SetPidCmd_t* PidMsg);

    /************************************************************************/
    /** \brief Update Z PIDs.
    **
    **  \par Description
    **       This function updates the vertical (Z) position and velocity
    **       PIDs, both member functions and configuration table.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  \retcode #TRUE   \retdesc \copydoc TRUE    \endcode
    **  \retcode #FALSE  \retdesc \copydoc FALSE   \endcode
    **  \retstmt Boolean TRUE indicates the command was valid and the change
    **           was successful. \endcode
    **  \endreturns
    **
    *************************************************************************/
    osalbool UpdateZPids(MPC_SetPidCmd_t* PidMsg);

    /************************************************************************/
    /** \brief Update the hold dead zone (HOLD_DZ)
    **
    **  \par Description
    **       This function updates the hold dead zone (HOLD_DZ) in the
    **       configuration table.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  \retcode #TRUE   \retdesc \copydoc TRUE    \endcode
    **  \retcode #FALSE  \retdesc \copydoc FALSE   \endcode
    **  \retstmt Boolean TRUE indicates the command was valid and the change
    **           was successful. \endcode
    **  \endreturns
    **
    *************************************************************************/
    osalbool UpdateHoldDz(MPC_SetDzCmd_t* DzMsg);

    /************************************************************************/
    /** \brief Set manual stick EXPO (XY_MAN_EXPO and Z_MAN_EXPO)
    **
    **  \par Description
    **       This function sets the #XY_MAN_EXPO and #Z_MAN_EXPO parameters
    **       in the configuration table.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  \retcode #TRUE   \retdesc \copydoc TRUE    \endcode
    **  \retcode #FALSE  \retdesc \copydoc FALSE   \endcode
    **  \retstmt Boolean TRUE indicates the command was valid and the change
    **           was successful. \endcode
    **  \endreturns
    **
    *************************************************************************/    
    osalbool UpdateStickExpo(MPC_SetStickExpoCmd_t* ExpoMsg);

    /************************************************************************/
    /** \brief Update the takeoff ramp time (TKO_RAMP_T)
    **
    **  \par Description
    **       This function
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  \retcode #TRUE   \retdesc \copydoc TRUE    \endcode
    **  \retcode #FALSE  \retdesc \copydoc FALSE   \endcode
    **  \retstmt Boolean TRUE indicates the command was valid and the change
    **           was successful. \endcode
    **  \endreturns
    **
    *************************************************************************/    
    osalbool UpdateTakeoffRampTime(MPC_SetTkoRampCmd_t* TkoRampMsg);

    /************************************************************************/
    /** \brief Validate MPC configuration table
    **
    **  \par Description
    **       This function validates MPC's configuration table
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

    /************************************************************************/
    /** \brief Validate the XY_P parameter.
    **
    **  \par Description
    **       This function validates the XY_P parameter
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in]   value    The value to be validated.
    **
    **  \returns
    **  \retcode #TRUE   \retdesc \copydoc TRUE    \endcode
    **  \retcode #FALSE  \retdesc \copydoc FALSE   \endcode
    **  \retstmt Boolean TRUE indicates the value passed is valid. \endcode
    **  \endreturns
    **
    *************************************************************************/
    static osalbool Validate_XY_P(float value);

    /************************************************************************/
    /** \brief Validate the XY_VEL_P parameter.
    **
    **  \par Description
    **       This function validates the XY_VEL_P parameter
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in]   value    The value to be validated.
    **
    **  \returns
    **  \retcode #TRUE   \retdesc \copydoc TRUE    \endcode
    **  \retcode #FALSE  \retdesc \copydoc FALSE   \endcode
    **  \retstmt Boolean TRUE indicates the value passed is valid. \endcode
    **  \endreturns
    **
    *************************************************************************/
    static osalbool Validate_XY_VEL_P(float value);

    /************************************************************************/
    /** \brief Validate the XY_VEL_I parameter.
    **
    **  \par Description
    **       This function validates the XY_VEL_I parameter
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in]   value    The value to be validated.
    **
    **  \returns
    **  \retcode #TRUE   \retdesc \copydoc TRUE    \endcode
    **  \retcode #FALSE  \retdesc \copydoc FALSE   \endcode
    **  \retstmt Boolean TRUE indicates the value passed is valid. \endcode
    **  \endreturns
    **
    *************************************************************************/
    static osalbool Validate_XY_VEL_I(float value);

    /************************************************************************/
    /** \brief Validate the XY_VEL_D parameter.
    **
    **  \par Description
    **       This function validates the XY_VEL_D parameter
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in]   value    The value to be validated.
    **
    **  \returns
    **  \retcode #TRUE   \retdesc \copydoc TRUE    \endcode
    **  \retcode #FALSE  \retdesc \copydoc FALSE   \endcode
    **  \retstmt Boolean TRUE indicates the value passed is valid. \endcode
    **  \endreturns
    **
    *************************************************************************/
    static osalbool Validate_XY_VEL_D(float value);

    /************************************************************************/
    /** \brief Validate the Z_P parameter.
    **
    **  \par Description
    **       This function validates the Z_P parameter
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in]   value    The value to be validated.
    **
    **  \returns
    **  \retcode #TRUE   \retdesc \copydoc TRUE    \endcode
    **  \retcode #FALSE  \retdesc \copydoc FALSE   \endcode
    **  \retstmt Boolean TRUE indicates the value passed is valid. \endcode
    **  \endreturns
    **
    *************************************************************************/
    static osalbool Validate_Z_P(float value);

    /************************************************************************/
    /** \brief Validate the Z_VEL_P parameter.
    **
    **  \par Description
    **       This function validates the Z_VEL_P parameter
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in]   value    The value to be validated.
    **
    **  \returns
    **  \retcode #TRUE   \retdesc \copydoc TRUE    \endcode
    **  \retcode #FALSE  \retdesc \copydoc FALSE   \endcode
    **  \retstmt Boolean TRUE indicates the value passed is valid. \endcode
    **  \endreturns
    **
    *************************************************************************/
    static osalbool Validate_Z_VEL_P(float value);

    /************************************************************************/
    /** \brief Validate the Z_VEL_I parameter.
    **
    **  \par Description
    **       This function validates the Z_VEL_I parameter
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in]   value    The value to be validated.
    **
    **  \returns
    **  \retcode #TRUE   \retdesc \copydoc TRUE    \endcode
    **  \retcode #FALSE  \retdesc \copydoc FALSE   \endcode
    **  \retstmt Boolean TRUE indicates the value passed is valid. \endcode
    **  \endreturns
    **
    *************************************************************************/
    static osalbool Validate_Z_VEL_I(float value);

    /************************************************************************/
    /** \brief Validate the Z_VEL_D parameter.
    **
    **  \par Description
    **       This function validates the Z_VEL_D parameter
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in]   value    The value to be validated.
    **
    **  \returns
    **  \retcode #TRUE   \retdesc \copydoc TRUE    \endcode
    **  \retcode #FALSE  \retdesc \copydoc FALSE   \endcode
    **  \retstmt Boolean TRUE indicates the value passed is valid. \endcode
    **  \endreturns
    **
    *************************************************************************/
    static osalbool Validate_Z_VEL_D(float value);

    /************************************************************************/
    /** \brief Validate the HOLD_DZ parameter.
    **
    **  \par Description
    **       This function validates the HOLD_DZ parameter
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in]   value    The value to be validated.
    **
    **  \returns
    **  \retcode #TRUE   \retdesc \copydoc TRUE    \endcode
    **  \retcode #FALSE  \retdesc \copydoc FALSE   \endcode
    **  \retstmt Boolean TRUE indicates the value passed is valid. \endcode
    **  \endreturns
    **
    *************************************************************************/
    static osalbool Validate_HOLD_DZ(float value);

    /************************************************************************/
    /** \brief Validate the XY_MAN_EXPO parameter.
    **
    **  \par Description
    **       This function validates the XY_MAN_EXPO parameter
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in]   value    The value to be validated.
    **
    **  \returns
    **  \retcode #TRUE   \retdesc \copydoc TRUE    \endcode
    **  \retcode #FALSE  \retdesc \copydoc FALSE   \endcode
    **  \retstmt Boolean TRUE indicates the value passed is valid. \endcode
    **  \endreturns
    **
    *************************************************************************/
    static osalbool Validate_XY_MAN_EXPO(float value);

    /************************************************************************/
    /** \brief Validate the Z_MAN_EXPO parameter.
    **
    **  \par Description
    **       This function validates the Z_MAN_EXPO parameter
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in]   value    The value to be validated.
    **
    **  \returns
    **  \retcode #TRUE   \retdesc \copydoc TRUE    \endcode
    **  \retcode #FALSE  \retdesc \copydoc FALSE   \endcode
    **  \retstmt Boolean TRUE indicates the value passed is valid. \endcode
    **  \endreturns
    **
    *************************************************************************/
    static osalbool Validate_Z_MAN_EXPO(float value);

    /************************************************************************/
    /** \brief Validate the TKO_RAMP_T parameter.
    **
    **  \par Description
    **       This function validates the TKO_RAMP_T parameter
    **
    **  \par Assumptions, External Events, and Notes:
    **       The #MPC_INVLD_PARAM_ERR_EID error event message will be
    **         sent if the function fails validation.
    **
    **  \param [in]   value    The value to be validated.
    **
    **  \returns
    **  \retcode #TRUE   \retdesc \copydoc TRUE    \endcode
    **  \retcode #FALSE  \retdesc \copydoc FALSE   \endcode
    **  \retstmt Boolean TRUE indicates the value passed is valid. \endcode
    **  \endreturns
    **
    *************************************************************************/
    static osalbool Validate_TKO_RAMP_T(float value);
};

#ifdef __cplusplus
}
#endif 

#endif /* MPC_APP_H */

/************************/
/*  End of File Comment */
/************************/
