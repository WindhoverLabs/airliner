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

#ifndef FAC_APP_H
#define FAC_APP_H

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
#include "fac_platform_cfg.h"
#include "fac_mission_cfg.h"
#include "fac_perfids.h"
#include "fac_msgids.h"
#include "fac_msg.h"
#include "fac_events.h"
#include "fac_tbldefs.h"
#include "px4_msgs.h"
#include <math/Vector3F.hpp>
#include "ECL_RollController.hpp"
#include "ECL_PitchController.hpp"
#include "ECL_YawController.hpp"
#include "ECL_WheelController.hpp"


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

enum vtol_type {
	TAILSITTER = 0,
	TILTROTOR,
	STANDARD
};


/** \brief Pipe name for the Data pipe
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define FAC_DATA_PIPE_NAME             ("FAC_DATA_PIPE")


/** \brief Pipe depth for the data pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define FAC_DATA_PIPE_DEPTH            (8)


/************************************************************************
 ** Local Structure Definitions
 *************************************************************************/
typedef enum
{
    FAC_AXIS_INDEX_ROLL     = 0,
    FAC_AXIS_INDEX_PITCH    = 1,
    FAC_AXIS_INDEX_YAW      = 2,
    FAC_AXIS_COUNT          = 3
} FAC_AXIS_Index_t;

typedef struct
{
    /** \brief Airspeed */
    PX4_AirspeedMsg_t                     Airspeed;

    /** \brief Battery status */
    PX4_BatteryStatusMsg_t                BatteryStatus;

    /** \brief Manual control setpoint */
    PX4_ManualControlSetpointMsg_t        ManualControlSp;

    /** \brief Vehicle attitude */
    PX4_VehicleAttitudeMsg_t              VAtt;

    /** \brief Vehicle attitude setpont */
    PX4_VehicleAttitudeSetpointMsg_t      VAttSp;

    /** \brief Vehicle control mode */
    PX4_VehicleControlModeMsg_t           VControlMode;

    /** \brief Vehicle global position */
    PX4_VehicleGlobalPositionMsg_t        VGlobalPosition;

    /** \brief Vehicle land detected */
    PX4_VehicleLandDetectedMsg_t          VLandDetected;

    /** \brief Vehicle Status */
    PX4_VehicleStatusMsg_t                VehicleStatus;
    
} FAC_CurrentValueTable_t;



/**
 **  \brief FAC Operational Data Structure
 */
class FAC
{
public:
    FAC();
    ~FAC();

    /** \brief CFE Event Table */
    CFE_EVS_BinFilter_t EventTbl[FAC_EVT_CNT];

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
    FAC_ParamTbl_t* ParamTblPtr;

    /** \brief Output Data published at the end of cycle */
    PX4_ActuatorControlsMsg_t m_ActuatorControls0;

    PX4_ActuatorControlsMsg_t m_ActuatorControls2;

    PX4_VehicleRatesSetpointMsg_t m_VehicleRatesSetpoint;

    /** \brief Housekeeping Telemetry for downlink */
    FAC_HkTlm_t HkTlm;

    /** \brief Current value table */
    FAC_CurrentValueTable_t CVT;

	ECL_RollController			_roll_ctrl;
	ECL_PitchController			_pitch_ctrl;
	ECL_YawController			_yaw_ctrl;
	ECL_WheelController			_wheel_ctrl;

	float _flaps_applied;
	float _flaperons_applied;


    /************************************************************************/
    /** \brief CFS PWM Motor Controller Task (FAC) application entry point
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
    /** \brief Initialize the CFS PWM Motor Controller (FAC) application
     **
     **  \par Description
     **       PWM Motor Controller application initialization routine. This
     **       function performs all the required startup steps to
     **       initialize (or restore from CDS) FAC data structures and get
     **       the application registered with the cFE services so it can
     **       begin to receive command messages and send events.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
     **  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
     **  \retstmt Return codes from #FAC_InitEvent               \endcode
     **  \retstmt Return codes from #FAC_InitPipe                \endcode
     **  \retstmt Return codes from #FAC_InitData                \endcode
     **  \retstmt Return codes from #FAC_InitConfigTbl           \endcode
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
     **       cFE Event Services for use by the FAC application.
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
    /** \brief Initialize global variables used by FAC application
     **
     **  \par Description
     **       This function performs the steps required to initialize
     **       the FAC application data.
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
     **       messages for the FAC application.
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
     **       for the FAC application from the SCH pipe.  This function
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
     **       by FAC application
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
     **       by FAC application
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
     **       specific to the FAC application
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
    /** \brief Sends FAC housekeeping message
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
    /** \brief Resets FAC housekeeping data
     **
     **  \par Description
     **       This function resets the housekeeping data
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void ResetHousekeeping(void);

    /************************************************************************/
    /** \brief Sends the Output message.
     **
     **  \par Description
     **       This function publishes the FAC output messages containing
     **       the attitude control values.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
     void SendOutputData(CFE_SB_Msg_t*);

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
    /** \brief Initialize the FAC configuration tables.
    **
    **  \par Description
    **       This function initializes FAC's configuration tables.  This
    **       includes the PWM and the Mixer configuration table.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
    **  \retstmt Return codes from #CFE_TBL_Register          \endcode
    **  \retstmt Return codes from #CFE_TBL_Load              \endcode
    **  \retstmt Return codes from #FAC_AcquireConfigPointers \endcode
    **  \endreturns
    **
    *************************************************************************/
    int32  InitConfigTbl(void);

    /************************************************************************/
    /** \brief Obtain FAC configuration tables data pointers.
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
    /** \brief Inform table services that a table has been updated.
    **
    **  \par Description
    **       This function calls CFE_TBL_Modified.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void  HandleTableUpdate(void);

    /************************************************************************/
    /** \brief Validate FAC PWM configuration table
    **
    **  \par Description
    **       This function validates FAC's PWM configuration table
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

    static osalbool InvalidFW_R_TC(float param);
    static osalbool InvalidFW_P_TC(float param);
    static osalbool InvalidFW_PR_P(float param);
    static osalbool InvalidFW_PR_I(float param);
    static osalbool InvalidFW_P_RMAX_POS(float param);
    static osalbool InvalidFW_P_RMAX_NEG(float param);
    static osalbool InvalidFW_PR_IMAX(float param);
    static osalbool InvalidFW_RR_P(float param);
    static osalbool InvalidFW_RR_I(float param);
    static osalbool InvalidFW_RR_IMAX(float param);
    static osalbool InvalidFW_R_RMAX(float param);
    static osalbool InvalidFW_YR_P(float param);
    static osalbool InvalidFW_YR_I(float param);
    static osalbool InvalidFW_YR_IMAX(float param);
    static osalbool InvalidFW_Y_RMAX(float param);
    static osalbool InvalidFW_RLL_TO_YAW_FF(float param);
    static osalbool InvalidFW_W_EN(int32 param);
    static osalbool InvalidFW_WR_P(float param);
    static osalbool InvalidFW_WR_I(float param);
    static osalbool InvalidFW_WR_IMAX(float param);
    static osalbool InvalidFW_W_RMAX(float param);
    static osalbool InvalidFW_RR_FF(float param);
    static osalbool InvalidFW_PR_FF(float param);
    static osalbool InvalidFW_YR_FF(float param);
    static osalbool InvalidFW_WR_FF(float param);
    static osalbool InvalidFW_YCO_VMIN(float param);
    static osalbool InvalidFW_YCO_METHOD(int32 param);
    static osalbool InvalidFW_RSP_OFF(float param);
    static osalbool InvalidFW_PSP_OFF(float param);
    static osalbool InvalidFW_MAN_R_MAX(float param);
    static osalbool InvalidFW_MAN_P_MAX(float param);
    static osalbool InvalidFW_FLAPS_SCL(float param);
    static osalbool InvalidFW_FLAPERON_SCL(float param);
    static osalbool InvalidFW_ARSP_MODE(int32 param);
    static osalbool InvalidFW_MAN_R_SC(float param);
    static osalbool InvalidFW_MAN_P_SC(float param);
    static osalbool InvalidFW_MAN_Y_SC(float param);
    static osalbool InvalidFW_BAT_SCALE_EN(int32 param);
    static osalbool InvalidFW_ACRO_X_MAX(float param);
    static osalbool InvalidFW_ACRO_Y_MAX(float param);
    static osalbool InvalidFW_ACRO_Z_MAX(float param);
    static osalbool InvalidFW_RATT_TH(float param);
    static osalbool InvalidFW_AIRSPD_MIN(float param);
    static osalbool InvalidFW_AIRSPD_MAX(float param);
    static osalbool InvalidFW_AIRSPD_TRIM(float param);
    static osalbool InvalidTRIM_ROLL(float param);
    static osalbool InvalidTRIM_PITCH(float param);
    static osalbool InvalidTRIM_YAW(float param);
    static osalbool InvalidVT_TYPE(uint32 param);

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

};

extern FAC oFAC;

#ifdef __cplusplus
}
#endif 

#endif /* FAC_APP_H */

/************************/
/*  End of File Comment */
/************************/
