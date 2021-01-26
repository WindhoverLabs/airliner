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

#ifndef MAC_APP_H
#define MAC_APP_H

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
#include "mac_platform_cfg.h"
#include "mac_mission_cfg.h"
#include "mac_perfids.h"
#include "mac_msgids.h"
#include "mac_msg.h"
#include "mac_events.h"
#include "mac_tbldefs.h"
#include "px4_msgs.h"
#include <math/Vector3F.hpp>


/************************************************************************
 ** Local Defines
 *************************************************************************/
#define TPA_RATE_LOWER_LIMIT                (0.05f)
#define MIN_TAKEOFF_THRUST                  (0.2f)
#define ATTITUDE_TC_DEFAULT                 (0.2f)
#define MANUAL_THROTTLE_MAX_MULTICOPTER     (0.9f)
#define MAX_GYRO_COUNT                      (3)
#define MICRO_SEC_TO_SEC_DIV                (1000000.0f)
#define TWO_MILLISECONDS                    (0.002f)
#define TWENTY_MILLISECONDS                 (0.02f)


/** \brief Pipe depth for the data pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define MAC_DATA_PIPE_DEPTH            (11)

/** \brief Pipe name for the Scheduler pipe
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define MAC_DATA_PIPE_NAME             ("MAC_DATA_PIPE")

/************************************************************************
 ** Local Structure Definitions
 *************************************************************************/
typedef enum
{
    MAC_AXIS_INDEX_ROLL     = 0,
    MAC_AXIS_INDEX_PITCH    = 1,
    MAC_AXIS_INDEX_YAW      = 2,
    MAC_AXIS_COUNT          = 3
} MAC_AXIS_Index_t;

typedef struct
{
    /** \brief  Actuator arming status */
    PX4_ActuatorArmedMsg_t                Armed;

    /** \brief Battery status */
    PX4_BatteryStatusMsg_t                BatteryStatus;

    /** \brief Control State */
    PX4_ControlStateMsg_t                 ControlState;

    /** \brief Manual control setpoint */
    PX4_ManualControlSetpointMsg_t        ManualControlSp;

    /** \brief Motor limits */
    PX4_MultirotorMotorLimitsMsg_t        MotorLimits;

    /** \brief Sensor thermal corrections */
    PX4_SensorCorrectionMsg_t             SensorCorrection;

    /** \brief Sensor gyro */
    PX4_SensorGyroMsg_t                   SensorGyro;

    /** \brief Vehicle attitude setpont */
    PX4_VehicleAttitudeSetpointMsg_t      VAttSp;

    /** \brief Vehicle control mode */
    PX4_VehicleControlModeMsg_t           VControlMode;

    /** \brief Vehicle rates setpoint */
    PX4_VehicleRatesSetpointMsg_t         VRatesSp;

    /** \brief Vehicle Status */
    PX4_VehicleStatusMsg_t                VehicleStatus;
    
} MAC_CurrentValueTable_t;

typedef struct
{
    /** \brief P gain for angular error */
    math::Vector3F att_p;

    /** \brief P gain for angular rate error */
    math::Vector3F rate_p;

    /** \brief I gain for angular rate error */
    math::Vector3F rate_i;

    /** \brief Integrator state limit for rate loop */
    math::Vector3F rate_int_lim;

    /** \brief D gain for angular rate error */
    math::Vector3F rate_d;

    /** \brief Feedforward gain for desired rates */
    math::Vector3F rate_ff;

    /** \brief Yaw control feed-forward */
    float yaw_ff;

    /** \brief Throttle PID Attenuation breakpoint P */
    float tpa_breakpoint_p;

    /** \brief Throttle PID Attenuation breakpoint I */
    float tpa_breakpoint_i;

    /** \brief Throttle PID Attenuation breakpoint D */
    float tpa_breakpoint_d;

    /** \brief Throttle PID Attenuation slope P */
    float tpa_rate_p;

    /** \brief Throttle PID Attenuation slope I */
    float tpa_rate_i;

    /** \brief Throttle PID Attenuation slope D */
    float tpa_rate_d;

    /** \brief Max roll rate in Degrees */
    float roll_rate_max;

    /** \brief Max pitch rate in Degrees */
    float pitch_rate_max;

    /** \brief Max yaw rate in Degrees */
    float yaw_rate_max;

    /** \brief Max auto yaw rate in Degrees */
    float yaw_auto_max;

    /** \brief Attitude rate limits in stabilized modes */
    math::Vector3F mc_rate_max;

    /** \brief Attitude rate limits in auto modes */
    math::Vector3F auto_rate_max;

    /** \brief Max attitude rates in acro mode */
    math::Vector3F acro_rate_max;

    /** \brief Threshold for Rattitude mode */
    float rattitude_thres;

    /** \brief Battery power level scaler */
    boolean bat_scale_en;

    /** \brief Board rotation */
    int board_rotation;

    /** \brief Physical board offset from center of vehicle */
    float board_offset[3];
    
} MAC_Params_t;


/**
 **  \brief MAC Operational Data Structure
 */
class MAC
{
public:
    MAC();
    ~MAC();

    /** \brief CFE Event Table */
    CFE_EVS_BinFilter_t EventTbl[MAC_EVT_CNT];

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

    /** \brief Param Table Handle */
    CFE_TBL_Handle_t ParamTblHdl;

    /** \brief Param Table Pointer */
    MAC_ParamTbl_t* ParamTblPtr;

    /** \brief Output Data published at the end of cycle */
    PX4_ActuatorControlsMsg_t m_ActuatorControls0;

    /** \brief Housekeeping Telemetry for downlink */
    MAC_HkTlm_t HkTlm;

    /** \brief Current value table */
    MAC_CurrentValueTable_t CVT;

    /** \brief Previous cycle rates */
    math::Vector3F m_AngularRatesPrevious;

    /** \brief Previous cycle rates setpoint */
    math::Vector3F m_AngularRatesSetpointPrevious;

    /** \brief Current cycle rates setpoint */
    math::Vector3F m_AngularRatesSetpoint;

    /** \brief Current cycle rates integral error */
    math::Vector3F m_AngularRatesIntegralError;

    /** \brief Current cycle calculated rates */
    math::Vector3F m_AttControl;

    /** \brief Currently used gyro device */
    int32 m_SelectedGyro;

    /** \brief thrust setpoint */
    float m_ThrustSp;

    /** \brief Locally stored copy of params */
    MAC_Params_t m_Params;

    /************************************************************************/
    /** \brief CFS PWM Motor Controller Task (MAC) application entry point
     **
     **  \par Description
     **       CFS PWM Motor Controller Task application entry point.  This function
     **       performs app initialization, then waits for the cFE ES Startup
     **       Sync, then executes the RPR main processing loop.
     **
     **  \par Assumptions, External Events, and Notes:
     **       If there is an unrecoverable failure during initialization the
     **       main loop is never executed and the application will exit.
     **
     *************************************************************************/
    void AppMain(void);

    /************************************************************************/
    /** \brief Initialize the CFS PWM Motor Controller (MAC) application
     **
     **  \par Description
     **       PWM Motor Controller application initialization routine. This
     **       function performs all the required startup steps to
     **       initialize (or restore from CDS) MAC data structures and get
     **       the application registered with the cFE services so it can
     **       begin to receive command messages and send events.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
     **  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
     **  \retstmt Return codes from #MAC_InitEvent               \endcode
     **  \retstmt Return codes from #MAC_InitPipe                \endcode
     **  \retstmt Return codes from #MAC_InitData                \endcode
     **  \retstmt Return codes from #MAC_InitConfigTbl           \endcode
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
     **       cFE Event Services for use by the MAC application.
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
    /** \brief Initialize global variables used by MAC application
     **
     **  \par Description
     **       This function performs the steps required to initialize
     **       the MAC application data.
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
    int32 InitData(void);

    /************************************************************************/
    /** \brief Initialize message pipes
     **
     **  \par Description
     **       This function performs the steps required to setup
     **       initialize the cFE Software Bus message pipes and subscribe to
     **       messages for the MAC application.
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
     **       for the MAC application from the SCH pipe.  This function
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
    /** \brief PWM Motor Controller Task incoming data processing
     **
     **  \par Description
     **       This function processes incoming data subscribed
     **       by MAC application
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    osalbool ProcessIncomingData(void);

    /************************************************************************/
    /** \brief PWM Motor Controller Task incoming command processing
     **
     **  \par Description
     **       This function processes incoming commands subscribed
     **       by MAC application
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void ProcessNewCmds(void);

    /************************************************************************/
    /** \brief PWM Motor Controller Task application commands
     **
     **  \par Description
     **       This function processes command messages
     **       specific to the MAC application
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
    /** \brief Sends MAC housekeeping message
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
    /** \brief Sends the Actuator Output message.
     **
     **  \par Description
     **       This function publishes the actuator output message containing
     **       the commanding values to the motors.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendActuatorControls(void);

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
    /** \brief Initialize the MAC configuration tables.
    **
    **  \par Description
    **       This function initializes MAC's configuration tables.  This
    **       includes the PWM and the Mixer configuration table.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
    **  \retstmt Return codes from #CFE_TBL_Register          \endcode
    **  \retstmt Return codes from #CFE_TBL_Load              \endcode
    **  \retstmt Return codes from #MAC_AcquireConfigPointers \endcode
    **  \endreturns
    **
    *************************************************************************/
    int32  InitConfigTbl(void);

    /************************************************************************/
    /** \brief Obtain MAC configuration tables data pointers.
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
    /** \brief Validate MAC PWM configuration table
    **
    **  \par Description
    **       This function validates MAC's PWM configuration table
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
    static int32  ValidateParamTbl(void*);

    /************************************************************************/
    /** \brief Run Attitude Controller
    **
    **  \par Description
    **       This function cycles the attitude controller
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void RunController(void);

    /************************************************************************/
    /** \brief Update Params 
    **
    **  \par Description
    **       This function copies params from the table into local data
    **       structures required for use 
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void UpdateParams(void);

    /************************************************************************/
    /** \brief Control Attitude
    **
    **  \par Description
    **       This function calculates the attitude setpoint
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in]   dt    #float of time delta
    **
    *************************************************************************/
    void ControlAttitude(float dt);

    /************************************************************************/
    /** \brief 
    **
    **  \par Description
    **       This function calculates the rates required to meet the attitude
    **       setpoint
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in]   dt    #float of time delta
    **
    *************************************************************************/
    void ControlAttitudeRates(float dt);

    /************************************************************************/
    /** \brief 
    **
    **  \par Description
    **       This function adjusts the rates based on the current throttle value
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in]   tpa_breakpoint    #float Parameter for breakpoint
    **  \param [in]   tpa_rate          #float Parameter for rate
    **
    **  \returns
    **  #math::Vector3F of adjusted rates
    **  \endreturns
    *************************************************************************/
    math::Vector3F PidAttenuations(float tpa_breakpoint, float tpa_rate);

};

extern MAC oMAC;

#ifdef __cplusplus
}
#endif 

#endif /* MAC_APP_H */

/************************/
/*  End of File Comment */
/************************/
