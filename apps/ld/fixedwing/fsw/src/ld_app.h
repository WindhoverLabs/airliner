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

#ifndef LD_APP_H
#define LD_APP_H

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
#include "ld_platform_cfg.h"
#include "ld_mission_cfg.h"
#include "ld_perfids.h"
#include "ld_msgids.h"
#include "ld_msg.h"
#include "ld_events.h"
#include "ld_tbldefs.h"
#include "px4_msgs.h"
#include "px4lib.h"
#include "ld_state_history.h"

/************************************************************************
 ** Local Defines
 *************************************************************************/
#define LD_MAX_EVENT_FILTERS (64)
#define LD_ARMING_THRESH_FACTOR (2.5f)
#define LD_25_PERCENT (0.25f)
#define LD_50_PERCENT (0.5f)
#define LD_75_PERCENT (0.75f)
#define LD_LOCAL_POSITION_TIMEOUT (500000)
#define ONE_PERCENT_THRUST (0.01f)

/************************************************************************
 ** Local Structure Definitions
 *************************************************************************/
/**
 * \brief message current value table
 */
typedef struct
{
    /** \brief The actuator armed message */
    PX4_ActuatorArmedMsg_t ActuatorArmedMsg;

    /** \brief The air speed message */
    PX4_AirspeedMsg_t AirspeedMsg;

    /** \brief The actuator control message */
    PX4_ActuatorControlsMsg_t ActuatorControls0Msg;

    /** \brief The control state message */
    PX4_ControlStateMsg_t ControlStateMsg;

    /** \brief The battery status message */
    PX4_BatteryStatusMsg_t BatteryStatusMsg;

    /** \brief The vehicle attitude message */
    PX4_VehicleAttitudeMsg_t VehicleAttitudeMsg;

    /** \brief The manual control message */
    PX4_ManualControlSetpointMsg_t ManualControlSetpointMsg;

    /** \brief The vehicle local position message */
    PX4_VehicleLocalPositionMsg_t VehicleLocalPositionMsg;

    /** @brief The vehicle sensor combined message */
    PX4_SensorCombinedMsg_t VehicleSensorCombinedMsg;

    /** \brief The vehicle control mode message */
    PX4_VehicleControlModeMsg_t VehicleControlModeMsg;

} LD_CurrentValueTable_t;

/**
 * \brief Land detection states
 */
enum LandDetectionState
{
    /**! Vehicle is in flying state */
    FLYING = 0,

    /**! Vehicle is in flying landed state */
    LANDED = 1,

    /**! Vehicle is in flying free fall state */
    FREEFALL = 2,

    /**! Vehicle is in flying ground contact state */
    GROUND_CONTACT = 3
};

/**
 **  \brief LD Application Class
 */
class LD
{
public:
    LD();
    ~LD();

    /**\brief Scheduling Pipe ID */
    CFE_SB_PipeId_t SchPipeId;

    /** \brief Command Pipe ID */
    CFE_SB_PipeId_t CmdPipeId;

    /** \brief Command Pipe ID */
    CFE_SB_PipeId_t DataPipeId;

    /* Task-related */
    /** \brief Task Run Status */
    uint32 uiRunStatus;

    /* Config table-related */
    /** \brief Config Table Handle */
    CFE_TBL_Handle_t ConfigTblHdl;

    /** \brief Config Table Pointer */
    LD_ConfigTbl_t* ConfigTblPtr;

    /** \brief Output Data published at the end of cycle */
    /** \brief The vehicle land detected message */
    PX4_VehicleLandDetectedMsg_t VehicleLandDetectedMsg = {};

    /* Application data */
    /** \brief Housekeeping Telemetry for downlink */
    LD_HkTlm_t HkTlm;

    /** \brief Current Value Table */
    LD_CurrentValueTable_t CVT = {};

    /** \brief The land detection state variable */
    LandDetectionState state = {};

    /** \brief The previous land detection message */
    PX4_VehicleLandDetectedMsg_t PreviousLandDetectedMsg;

    /** \brief The free fall state history variable */
    StateHistory freefall_history = {FALSE};

    /** \brief The landed state history variable */
    StateHistory landed_history = {TRUE};

    /** \brief The ground contact state history variable */
    StateHistory ground_contact_history = {TRUE};

    /** \brief The arming time variable */
    uint64 arming_time = 0;

    /** \brief The minimum thrust on start variable */
    uint64 min_thrust_start = 0;

    /** \brief The message publish counter variable (acts like a flag for one time event) */
    int publish_counter = 0;

    /** \brief The maximum altitude value variable */
    float altitude_max;

    /** \brief The total flight time variable */
    uint64 total_flight_time = 0;

    /** \brief The take off time variable */
    uint64 takeoff_time = 0;

    /** \brief Run main land detector loop at this rate in Hz. */
    static constexpr uint32 LAND_DETECTOR_UPDATE_RATE_HZ = 50;

    /** \brief Time in us that landing conditions have to hold before triggering a land. */
    static constexpr uint64 LAND_DETECTOR_TRIGGER_TIME_US = 300000;

    /** \brief Time in us that ground contact condition have to hold before triggering contact ground */
    static constexpr uint64 GROUND_CONTACT_TRIGGER_TIME_US = 350000;

    /** \brief Time interval in us in which wider acceptance thresholds are used after arming. */
    static constexpr uint64 LAND_DETECTOR_ARM_PHASE_TIME_US = 2000000;

    LD_Diag_t DiagTlm;

    /************************************************************************/
    /** \brief Landing Detector (LD) application entry point
     **
     **  \par Description
     **       Landing Detector Task application entry point.  This function
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
    /** \brief Initialize the Landing Detector (LD) application
     **
     **  \par Description
     **       Landing Detector application initialization routine. This
     **       function performs all the required startup steps to
     **       initialize (or restore from CDS) LD data structures and get
     **       the application registered with the cFE services so it can
     **       begin to receive command messages and send events.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
     **  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
     **  \retstmt Return codes from #LD_InitEvent               \endcode
     **  \retstmt Return codes from #LD_InitPipe                \endcode
     **  \retstmt Return codes from #LD_InitData                \endcode
     **  \retstmt Return codes from #LD_InitConfigTbl           \endcode
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
     **       cFE Event Services for use by the LD application.
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
    /** \brief Initialize global variables used by LD application
     **
     **  \par Description
     **       This function performs the steps required to initialize
     **       the LD application data.
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
     **       messages for the LD application.
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
     **       for the LD application from the SCH pipe.  This function
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
    /** \brief Receive and process messages from the data pipe.
     **
     **  \par Description
     **       This function receives and processes messages
     **       for the LD application from the Data pipe.  This function
     **       will pend for the type defined by CFE_SB_POLL, allowing
     **       it to wait for messages, i.e. wakeup messages on Data.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
     **  \retstmt Return codes from #CFE_SB_RcvMsg            \endcode
     **  \endreturns
     **
     *************************************************************************/
    int32 RcvDataPipeMsg();

    /************************************************************************/
    /** \brief Landing Detector Task incoming command processing
     **
     **  \par Description
     **       This function processes incoming commands subscribed
     **       by LD application
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void ProcessCmdPipe(void);

    /************************************************************************/
    /** \brief Landing Detector Task application commands
     **
     **  \par Description
     **       This function processes command messages
     **       specific to the LD application
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
    /** \brief Sends LD housekeeping message
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
    /** \brief Sends the VehicleLandDetectedMsg message.
     **
     **  \par Description
     **       This function publishes the VehicleLandDetectedMsg message containing
     **       <TODO>
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendVehicleLandDetectedMsg(void);

    /************************************************************************/
    /** \brief Sends the Diagnotic message.
     **
     **  \par Description
     **       This function publishes the diag message to the SB
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/    
    void SendDiag(void);
    
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

private:
    /************************************************************************/
    /** \brief Initialize the LD configuration tables.
     **
     **  \par Description
     **       This function initializes LD's configuration tables.  This
     **       includes <TODO>.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
     **  \retstmt Return codes from #CFE_TBL_Register          \endcode
     **  \retstmt Return codes from #CFE_TBL_Load              \endcode
     **  \retstmt Return codes from #LD_AcquireConfigPointers \endcode
     **  \endreturns
     **
     *************************************************************************/
    int32 InitConfigTbl(void);
    
    /************************************************************************/
    /** \brief Obtain LD configuration tables data pointers.
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
    
    /************************************************************************/
    /** \brief Detect if vehicle is in free fall state.
     **
     **  \par Description
     **       Computes net acceleration acting on the vehicle. if the value
     **       is close to the free fall threshold then true is returned
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  True id net acceleration acting on the vehicle's body is less than
     **  free fall threshold value.
     **  \endreturns
     **
     *************************************************************************/
    osalbool DetectFreeFall(void);
    
    /************************************************************************/
    /** \brief Detect if vehicle is in free fall state.
     **
     **  \par Description
     **       Computes net acceleration acting on the vehicle. if the value
     **       is close to the free fall threshold then true is returned
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  True id net acceleration acting on the vehicle's body is less than
     **  free fall threshold value.
     **  \endreturns
     **
     *************************************************************************/
    osalbool DetectGroundContactState(void);
    
    /************************************************************************/
    /** \brief Detect if vehicle is in free fall state.
     **
     **  \par Description
     **       Computes net acceleration acting on the vehicle. if the value
     **       is close to the free fall threshold then true is returned
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  True id net acceleration acting on the vehicle's body is less than
     **  free fall threshold value.
     **  \endreturns
     **
     *************************************************************************/
    osalbool DetectLandedState(void);
    
    /************************************************************************/
    /** \brief Detect if vehicle is in free fall state.
     **
     **  \par Description
     **       Computes net acceleration acting on the vehicle. if the value
     **       is close to the free fall threshold then true is returned
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  True id net acceleration acting on the vehicle's body is less than
     **  free fall threshold value.
     **  \endreturns
     **
     *************************************************************************/
    float TakeoffThrottle(void);
    
    /************************************************************************/
    /** \brief Detect if vehicle is in free fall state.
     **
     **  \par Description
     **       Computes net acceleration acting on the vehicle. if the value
     **       is close to the free fall threshold then true is returned
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  True id net acceleration acting on the vehicle's body is less than
     **  free fall threshold value.
     **  \endreturns
     **
     *************************************************************************/
    float MaxAltitude(void);

    /************************************************************************/
    /** \brief Detect if vehicle is in free fall state.
     **
     **  \par Description
     **       Computes net acceleration acting on the vehicle. if the value
     **       is close to the free fall threshold then true is returned
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  True id net acceleration acting on the vehicle's body is less than
     **  free fall threshold value.
     **  \endreturns
     **
     *************************************************************************/
    osalbool AltitudeLock(void);
    
    /************************************************************************/
    /** \brief Detect if vehicle is in free fall state.
     **
     **  \par Description
     **       Computes net acceleration acting on the vehicle. if the value
     **       is close to the free fall threshold then true is returned
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  True id net acceleration acting on the vehicle's body is less than
     **  free fall threshold value.
     **  \endreturns
     **
     *************************************************************************/
    osalbool PositionLock(void);
    
    /************************************************************************/
    /** \brief Detect if vehicle is in free fall state.
     **
     **  \par Description
     **       Computes net acceleration acting on the vehicle. if the value
     **       is close to the free fall threshold then true is returned
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  True id net acceleration acting on the vehicle's body is less than
     **  free fall threshold value.
     **  \endreturns
     **
     *************************************************************************/
    osalbool ManualControlPresent(void);

    /************************************************************************/
    /** \brief Detect if vehicle is in free fall state.
     **
     **  \par Description
     **       Computes net acceleration acting on the vehicle. if the value
     **       is close to the free fall threshold then true is returned
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  True id net acceleration acting on the vehicle's body is less than
     **  free fall threshold value.
     **  \endreturns
     **
     *************************************************************************/
    osalbool MinimalThrust(void);
    
    /************************************************************************/
    /** \brief Detect if vehicle is in free fall state.
     **
     **  \par Description
     **       Computes net acceleration acting on the vehicle. if the value
     **       is close to the free fall threshold then true is returned
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  True id net acceleration acting on the vehicle's body is less than
     **  free fall threshold value.
     **  \endreturns
     **
     *************************************************************************/
    void UpdateState(void);
    
    /************************************************************************/
    /** \brief Land Detector Task
     **
     **  \par Description
     **       Executes every time a new message is available, determines if
     **       vehicle is in one of three states, writes this information to
     **       VehicleLandDetectedMsg and publishes it.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void Execute(void);
    
    /************************************************************************/
    /** \brief Detect Change In State.
     **
     **  \par Description
     **       Reads VehicleLandDetectedMsg and determines if state of the
     **       vehicle has changed.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void DetectAndSendStateChangeEvent(void);


    float _velocity_xy_filtered{0.0f};
	float _velocity_z_filtered{0.0f};
	float _airspeed_filtered{0.0f};
	float _accel_horz_lp{0.0f};


public:
    /************************************************************************/
    /** \brief Validate LD configuration table
     **
     **  \par Description
     **       This function validates LD's configuration table
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

#endif /* LD_APP_H */

/************************/
/*  End of File Comment */
/************************/
