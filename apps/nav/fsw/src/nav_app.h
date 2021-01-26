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

#ifndef NAV_APP_H
#define NAV_APP_H

#ifdef __cplusplus
extern "C"
{
#endif

/************************************************************************
 ** Pragmas
 *************************************************************************/

/************************************************************************
 ** Includes
 *************************************************************************/
#include <math.h>
#include <float.h>
#include "cfe.h"
#include "nav_platform_cfg.h"
#include "nav_mission_cfg.h"
#include "nav_perfids.h"
#include "nav_msgids.h"
#include "nav_msg.h"
#include "nav_events.h"
#include "nav_tbldefs.h"
#include "px4_msgs.h"
#include "px4lib.h"
#include "geo/geo.h"

/************************************************************************
 ** Local Defines
 *************************************************************************/
#define NAV_EPSILON_POSITION    (0.001f)
#define M_PI_2_F                (1.57079632f)
#define DELAY_SIGMA             (0.01f)
#define NAV_LAT_SHORT_FORM      (1000)
#define NAV_LON_SHORT_FORM      (1000)
#define CONVERT_DECIMAL_DEGREES (1e7f)

/************************************************************************
 ** Local Structure Definitions
 *************************************************************************/
/**
 * \brief message current value table
 */
typedef struct
{
    /** \brief The home position message */
    PX4_HomePositionMsg_t HomePositionMsg;
    /** \brief The mission message */
    PX4_MissionMsg_t MissionMsg;
    /** \brief The position message from GPS */
    PX4_VehicleGpsPositionMsg_t VehicleGpsPositionMsg;
    /** \brief The global position message */
    PX4_VehicleGlobalPositionMsg_t VehicleGlobalPosition;
    /** \brief The vehicle status message */
    PX4_VehicleStatusMsg_t VehicleStatusMsg;
    /** \brief The land detection message */
    PX4_VehicleLandDetectedMsg_t VehicleLandDetectedMsg;
    /** \brief The vehicle local position message */
    PX4_VehicleLocalPositionMsg_t VehicleLocalPositionMsg;
    /** \brief The vehicle command message */
    PX4_VehicleCommandMsg_t VehicleCommandMsg;
}NAV_CurrentValueTable_t;

/**
 * \brief mission planning structure
 */
typedef struct
{
    /** \brief The latitude in degrees */
    double Lat;
    /** \brief The longitude in degrees */
    double Lon;
    /** \brief The time that the MAV should stay inside the radius before advancing in seconds */
    float TimeInside;
    /** \brief The minimal pitch angle for fixed wing takeoff waypoints */
    float PitchMin;
    /** \brief The default radius in which the mission is accepted as reached in meters */
    float AcceptanceRadius;
    /** \brief lThe oiter radius in meters, 0 for a VTOL to hover, negative for counter-clockwise */
    float LoiterRadius;
    /** \brief The in radians NED -PI..+PI, NAN means don't change yaw */
    float Yaw;
    /** \brief The latitude padding */
    float LatFloatPadding;
    /** \brief The longitude padding */
    float LonFloatPadding;
    /** \brief The altitude in meters    (AMSL) */
    float Altitude;
    /** \brief The array to store mission command values for MAV_FRAME_MISSION */
    float Params[7];
    /** \brief The navigation command */
    uint16 NavCmd;
    /** \brief The index where the do jump will go to */
    int16 DoJumpMissionIndex;
    /** \brief The how many times do jump needs to be done */
    uint16 DoJumpRepeatCount;
    /** \brief The count how many times the jump has been done */
    uint16 DoJumpCurrentCount;
    /** \brief The mission frame */
    uint16 Frame;
    /** \brief How the mission item was generated */
    uint16 Origin;
    /** \brief The exit xtrack location: 0 for center of loiter wp, 1 for exit location */
    uint16 LoiterExitXTrack;
    /** \brief The heading needs to be reached */
    uint16 ForceHeading;
    /** \brief True if altitude is relative from start point */
    uint16 AltitudeIsRelative;
    /** \brief True if next waypoint should follow after this one */
    uint16 AutoContinue;
    /** \brief Disables multi-copter yaw with this flag */
    uint16 DisableMcYaw;
}NAV_MissionItem_t;

/**
 * \brief mission origin
 */
typedef enum
{
    /**! Mavlink originated mission item */
    ORIGIN_MAVLINK = 0,
    /*! Onboard originated mission item */
    ORIGIN_ONBOARD = 1
}NAV_Origin_t;

/**
 * \brief Return to Launch states
 */
typedef enum
{
    /*! The vehicle is in idle state during RTL navigation */
    RTL_STATE_NONE              = 0,
    /*! The vehicle is in climb state during RTL navigation */
    RTL_STATE_CLIMB             = 1,
    /*! The vehicle is in Return to home latitude and longitude state during RTL navigation */
    RTL_STATE_RETURN            = 2,
    /*!  VTOL to MC transition state during RTL navigation */
    RTL_STATE_TRANSITION_TO_MC  = 3,
    /*! The vehicle is in descend state during RTL navigation */
    RTL_STATE_DESCEND           = 4,
    /*! The vehicle is in loiter state during RTL navigation */
    RTL_STATE_LOITER            = 5,
    /*! The vehicle is in land state during RTL navigation */
    RTL_STATE_LAND              = 6,
    /*! The vehicle is in landed state during RTL navigation */
    RTL_STATE_LANDED            = 7,
}RTLState;

/**
 **  \brief NAV Application Class
 */
class NAV
{
public:
    NAV();
    ~NAV();

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
    NAV_ConfigTbl_t* ConfigTblPtr;

    /** \brief Output Data published at the end of cycle */
    /** \brief The mission result message */
    PX4_MissionResultMsg_t MissionResultMsg = {};
    /** \brief The position set point triplet message */
    PX4_PositionSetpointTripletMsg_t PositionSetpointTripletMsg = {};
    /** \brief A temporary triplet type message to store take off triplet */
    PX4_PositionSetpointTripletMsg_t TakeoffTripletMsg = {};
    /** \brief A temporary triplet type message to store reposition triplet  */
    PX4_PositionSetpointTripletMsg_t RepositionTripletMsg = {};

    /* Application data */
    /** \brief Housekeeping Telemetry for downlink */
    NAV_HkTlm_t HkTlm;
    /** \brief Current Value Table */
    NAV_CurrentValueTable_t CVT = {};

    /** \brief The mission item variable */
    NAV_MissionItem_t MissionItem;
    /** \brief RTL state initialization */
    RTLState RtlState;
    /** \brief This variable stores previously encountered navigation state */
    PX4_VehicleStatusMsg_t PreviousState;
    /** \brief This variable stores previously encountered navigation command */
    PX4_VehicleCommandMsg_t PreviousCommand;
    /** \brief Flag is set to true if a previously unseen command is encountered */
    osalbool NewCommandArrived;
    /** \brief Will allow to loiter at setpoint */
    osalbool CanLoiterAtSetpoint;
    /** \brief True if loiter position is set */
    osalbool LoiterPositionSet;
    /** \brief True if waypoint position is reached */
    osalbool WaypointPositionReached;
    /** \brief True if waypoint yaw is reached */
    osalbool WaypointYawReached;
    /** \brief Default time first time inside orbit in mission item */
    uint64 TimeFirstInsideOrbit;
    /** \brief Default action start in mission item */
    uint64 ActionStart;
    /** \brief Default time for waypoint reached in mission item */
    uint64 TimeWpReached;
    /** \brief Default mission cruising speed in mission item */
    float MissionCruisingSpeed;
    /** \brief Default mission throttle in mission item */
    float MissionThrottle;
    /** \brief True if vehicle status message is updated once by navigation states */
    osalbool VehicleStatusUpdateOnce;
    /** \brief True if mission result message is updated by navigation states */
    osalbool MissionResultUpdated;
    /** \brief True if position setpoint triplet message is updated by navigation states */
    osalbool PositionSetpointTripletUpdated;
    float ForceDescentTarget;
    osalbool ForceDescentExecuting;
    osalbool ForceDescentCompleted;

    /************************************************************************/
    /** \brief Navigator (NAV) application entry point
     **
     **  \par Description
     **       Navigator Task application entry point.  This function
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
    /** \brief Initialize the Navigator (NAV) application
     **
     **  \par Description
     **       Navigator application initialization routine. This
     **       function performs all the required startup steps to
     **       initialize (or restore from CDS) NAV data structures and get
     **       the application registered with the cFE services so it can
     **       begin to receive command messages and send events.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
     **  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
     **  \retstmt Return codes from #NAV_InitEvent               \endcode
     **  \retstmt Return codes from #NAV_InitPipe                \endcode
     **  \retstmt Return codes from #NAV_InitData                \endcode
     **  \retstmt Return codes from #NAV_InitConfigTbl           \endcode
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
     **       cFE Event Services for use by the NAV application.
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
    /** \brief Initialize global variables used by NAV application
     **
     **  \par Description
     **       This function performs the steps required to initialize
     **       the NAV application data.
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
     **       messages for the NAV application.
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
     **       for the NAV application from the SCH pipe.  This function
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
    /** \brief Navigator Task incoming command processing
     **
     **  \par Description
     **       This function processes incoming commands subscribed
     **       by NAV application
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void ProcessCmdPipe(void);

    /************************************************************************/
    /** \brief Navigator Task application commands
     **
     **  \par Description
     **       This function processes command messages
     **       specific to the NAV application
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
    /** \brief Sends NAV housekeeping message
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
    /** \brief Sends the MissionResultMsg message.
     **
     **  \par Description
     **       This function publishes the MissionResultMsg message containing
     **       <TODO>
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendMissionResultMsg(void);
    
    /************************************************************************/
    /** \brief Sends the PositionSetpointTripletMsg message.
     **
     **  \par Description
     **       This function publishes the PositionSetpointTripletMsg message containing
     **       <TODO>
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendPositionSetpointTripletMsg(void);
    
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
    osalbool VerifyCmdLength(CFE_SB_Msg_t* MsgPtr, uint16 usExpectedLen);
    
    /************************************************************************/
    /** \brief Navigation Task
     **
     **  \par Description
     **       Executes every time a new message is available, verifies message
     **       validity and updates messages and publishes them to software bus.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  0 at the end of task.
     **  \endreturns
     **
     *************************************************************************/
    int32 Execute(void);
    
    /************************************************************************/
    /** \brief Vehicle Takeoff
     **
     **  \par Description
     **       Sets position set point triplet for vehicle takeoff.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None

     **
     *************************************************************************/
    void Takeoff(void);
    
    /************************************************************************/
    /** \brief Takeoff Routine
     **
     **  \par Description
     **       This routine call is persistent until vehicle is in takeoff state,
     **       if mission is reached or vehicle lands, performs state transition.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None

     **
     *************************************************************************/
    void TakeoffActive(void);
    
    /************************************************************************/
    /** \brief Vehicle Land
     **
     **  \par Description
     **       Sets position set point triplet for vehicle land.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void Land(void);
    
    /************************************************************************/
    /** \brief Land Routine
     **
     **  \par Description
     **       This routine call is persistent until vehicle is in land state,
     **       if mission is reached or vehicle lands, performs state transition.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void LandActive(void);
    
    /************************************************************************/
    /** \brief Vehicle Loiter
     **
     **  \par Description
     **       Sets position set point triplet for vehicle loiter.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void Loiter(void);
    
    /************************************************************************/
    /** \brief Loiter Routine
     **
     **  \par Description
     **       This routine call is persistent until vehicle is in loiter state,
     **       if mission is reached or vehicle lands, performs state transition.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void LoiterActive(void);
    
    /************************************************************************/
    /** \brief Loiter Reposition routine
     **
     **  \par Description
     **       When necessary, while loitering performs re-positioning.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void LoiterReposition(void);
    
    /************************************************************************/
    /** \brief Set Loiter Position Setpoint
     **
     **  \par Description
     **       Sets position set point triplet.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void LoiterSetPosition(void);
    
    /************************************************************************/
    /** \brief Vehicle return to launch
     **
     **  \par Description
     **       Sets position set point triplet for vehicle rtl.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void Rtl(void);
    
    /************************************************************************/
    /** \brief Set RTL Position Setpoint
     **
     **  \par Description
     **       Sets position setpoint triplet based on RTL state received.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SetRtlItem(void);
    
    /************************************************************************/
    /** \brief RTL State Transitions
     **
     **  \par Description
     **       Transits from one state to another to complete return to launch.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void AdvanceRtl(void);
    
    /************************************************************************/
    /** \brief RTL Routine
     **
     **  \par Description
     **       This routine call is persistent until vehicle is in RTL state,
     **       calls AdvanceRtl function for state transition.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void RtlActive(void);
    
    /************************************************************************/
    /** \brief Detect State change
     **
     **  \par Description
     **       Detects if the navigation state is changed.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  TRUE if state changes, FALSE if it does not.
     **  \endreturns
     **
     *************************************************************************/
    osalbool StateChangeDetect(void);
    
    /************************************************************************/
    /** \brief Store Previous Commands
     **
     **  \par Description
     **       A local application variable stores previously received
     **       vehicle command in the last execution cycle.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void CacheCommandEventHist(void);
    
    /************************************************************************/
    /** \brief Verify Mission Item Reach
     **
     **  \par Description
     **       A navigation state invokes a navigation action and this
     **       function checks if the end of this action is reached.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  TRUE if the mission item is reached, FALSE if it does not.
     **  \endreturns
     **
     *************************************************************************/
    osalbool IsMissionItemReached(void);
    
    /************************************************************************/
    /** \brief Marks Mission Result Message as Fail
     **
     **  \par Description
     **       Sets MissionFaliure field in mission result message to false,
     **       creates a critical severity event.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \param [in]   Reason        A reason to why the mission failed
     **
     *************************************************************************/
    void SetMissionFaliure(const char*);
    
    /************************************************************************/
    /** \brief Sets Current Position Setpoint
     **
     **  \par Description
     **       This function verifies the command message length.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \param [in]   PositionSetpointTriplet_ptr        A pointer to position setpoint triplet
     **
     **  \param [in]   MissionItem_ptr                    A pointer to navigation mission item
     **
     *************************************************************************/
    void ConvertMissionItemToCurrentSetpoint(PX4_PositionSetpoint_t *, NAV_MissionItem_t *);
    
    /************************************************************************/
    /** \brief Sets Loiter Setpoint Triplet
     **
     **  \par Description
     **       Prepares position setpoint triplet for loiter state.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \param [in]   MissionItem_ptr                    A pointer to navigation mission item
     **
     *************************************************************************/
    void SetLoiterItem(NAV_MissionItem_t *);
    
    /************************************************************************/
    /** \brief Returns Value Assigned to GetTimeInside
     **
     **  \par Description
     **       Returns Value Assigned to GetTimeInside field in mission item.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \param [in]   MissionItem_ptr                    A pointer to navigation mission item
     **
     **  \returns
     **  Returns Value Assigned to GetTimeInside if vehicle command is takeoff, else return 0.
     **  \endreturns
     **
     *************************************************************************/
    float GetTimeInside(NAV_MissionItem_t *);
    
    /************************************************************************/
    /** \brief Validate Home Position Message
     **
     **  \par Description
     **       Checks home position message timestamp and validates message.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  TRUE if the home position message is valid, FALSE if it does not.
     **  \endreturns
     **
     *************************************************************************/
    osalbool HomePositionValid(void);
    
    /************************************************************************/
    /** \brief Return Default Accepted Radius
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  Return default accepted radius.
     **  \endreturns
     **
     *************************************************************************/
    float GetDefaultAcceptedRadius(void);
    
    /************************************************************************/
    /** \brief Sets Accepted Radius
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \param [in]   AcceptedRadius        A float value which represents accepted radius.
     **
     *************************************************************************/
    void SetAcceptedRadius(float);
    
    /************************************************************************/
    /** \brief Return Altitude Accepted Radius
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  Return altitude accepted radius.
     **  \endreturns
     **
     *************************************************************************/
    float GetAltitudeAcceptedRadius(void);
    
    /************************************************************************/
    /** \brief Return Cruising Throttle
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  Return cruising throttle.
     **  \endreturns
     **
     *************************************************************************/
    float GetCruisingThrottle(void);
    
    /************************************************************************/
    /** \brief Return Cruising Speed
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  Return cruising speed.
     **  \endreturns
     **
     *************************************************************************/
    float GetCruisingSpeed(void);
    
    /************************************************************************/
    /** \brief Returns Address of Mission Result Message
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  Returns Address of Mission Result Message.
     **  \endreturns
     **
     *************************************************************************/
    PX4_MissionResultMsg_t* GetMissionResultMsg()
    {
        return &MissionResultMsg;
    }
    
    /************************************************************************/
    /** \brief Returns Address of Position Setpoint Triplet Message
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  Returns Address of Position Setpoint Triplet Message.
     **  \endreturns
     **
     *************************************************************************/
    PX4_PositionSetpointTripletMsg_t* GetPositionSetpointTripletMsg()
    {
        return &PositionSetpointTripletMsg;
    }
    
    /************************************************************************/
    /** \brief Returns Address of Take off Triplet Message
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  Returns Address of Take off Triplet Message.
     **  \endreturns
     **
     *************************************************************************/
    PX4_PositionSetpointTripletMsg_t* GetTakeoffTripletMsg()
    {
        return &TakeoffTripletMsg;
    }
    
    /************************************************************************/
    /** \brief Returns Address of Reposition Triplet Message
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  Returns Address of Reposition Triplet Message.
     **  \endreturns
     **
     *************************************************************************/
    PX4_PositionSetpointTripletMsg_t* GetRepositionTripletMsg()
    {
        return &RepositionTripletMsg;
    }
    
    /************************************************************************/
    /** \brief Returns Address of Home Position Message
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  Returns Address of Home Position Message.
     **  \endreturns
     **
     *************************************************************************/
    PX4_HomePositionMsg_t* GetHomePosition()
    {
        return &CVT.HomePositionMsg;
    }
    
    /************************************************************************/
    /** \brief Returns Address of Mission Message
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  Returns Address of Mission Message.
     **  \endreturns
     **
     *************************************************************************/
    PX4_MissionMsg_t* GetMissionMsg()
    {
        return &CVT.MissionMsg;
    }
    
    /************************************************************************/
    /** \brief Returns Address of Vehicle Global Position Message
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  Returns Address of Vehicle Global Position Message.
     **  \endreturns
     **
     *************************************************************************/
    PX4_VehicleGlobalPositionMsg_t* GetVehicleGlobalPositionMsg()
    {
        return &CVT.VehicleGlobalPosition;
    }
    
    /************************************************************************/
    /** \brief Returns Address of Vehicle Status Message
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  Returns Address of Vehicle Status Message.
     **  \endreturns
     **
     *************************************************************************/
    PX4_VehicleStatusMsg_t* GetVehicleStatusMsg()
    {
        return &CVT.VehicleStatusMsg;
    }
    
    /************************************************************************/
    /** \brief Returns Address of Vehicle Land Detected Message
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  Returns Address of Vehicle Land Detected Message.
     **  \endreturns
     **
     *************************************************************************/
    PX4_VehicleLandDetectedMsg_t* GetVehicleLandDetectedMsg()
    {
        return &CVT.VehicleLandDetectedMsg;
    }
    
    /************************************************************************/
    /** \brief Returns Address of Vehicle Local Position Message
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  Returns Address of Vehicle Local Position Message.
     **  \endreturns
     **
     *************************************************************************/
    PX4_VehicleLocalPositionMsg_t* GetVehicleLocalPositionMsg()
    {
        return &CVT.VehicleLocalPositionMsg;
    }
    
    /************************************************************************/
    /** \brief Returns Address of Vehicle Command Message
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  Returns Address of Vehicle Command Message.
     **  \endreturns
     **
     *************************************************************************/
    PX4_VehicleCommandMsg_t* GetVehicleCommandMsg()
    {
        return &CVT.VehicleCommandMsg;
    }
    
    /************************************************************************/
    /** \brief Returns False to function call
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  Returns false since mission not planned.
     **  \endreturns
     **
     *************************************************************************/
    osalbool IsPlannedMission()
    {
        return false;
    }
    
    /************************************************************************/
    /** \brief Sets CanLoiterAtSetpoint
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \param [in]   can_loiter        A application specific osalbool variable.
     **
     *************************************************************************/
    void SetCanLoiterAtSetpoint(osalbool can_loiter)
    {
        CanLoiterAtSetpoint = can_loiter;
    }
    
    /************************************************************************/
    /** \brief Sets PositionSetpointTripletUpdated to True
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SetPositionSetpointTripletUpdated()
    {
        PositionSetpointTripletUpdated = true;
    }

private:
    /************************************************************************/
    /** \brief Initialize the NAV configuration tables.
     **
     **  \par Description
     **       This function initializes NAV's configuration tables.  This
     **       includes <TODO>.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
     **  \retstmt Return codes from #CFE_TBL_Register          \endcode
     **  \retstmt Return codes from #CFE_TBL_Load              \endcode
     **  \retstmt Return codes from #NAV_AcquireConfigPointers \endcode
     **  \endreturns
     **
     *************************************************************************/
    int32 InitConfigTbl(void);

    /************************************************************************/
    /** \brief Obtain NAV configuration tables data pointers.
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
    /** \brief Validate NAV configuration table
     **
     **  \par Description
     **       This function validates NAV's configuration table
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

#endif /* NAV_APP_H */

/************************/
/*  End of File Comment */
/************************/
