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

#ifndef SENS_APP_H
#define SENS_APP_H

/************************************************************************
 ** Pragmas
 *************************************************************************/

/************************************************************************
 ** Includes
 *************************************************************************/

#include <math/filters/LowPassFilter2p.hpp>

#ifdef __cplusplus
extern "C" {
#endif

#include "cfe.h"

#include "sens_platform_cfg.h"
#include "sens_mission_cfg.h"
#include "sens_perfids.h"
#include "sens_msgids.h"
#include "sens_msg.h"
#include "sens_events.h"
#include "sens_tbldefs.h"
#include "px4_msgs.h"

/************************************************************************
 ** Local Defines
 *************************************************************************/

/************************************************************************
 ** Local Structure Definitions
 *************************************************************************/
typedef struct
{
    PX4_SensorAccelMsg_t          SensorAccelMsg;
    uint64                        LastAccelTime;
    PX4_SensorBaroMsg_t           SensorBaroMsg;
    uint64                        LastBaroTime;
    PX4_SensorGyroMsg_t           SensorGyroMsg;
    uint64                        LastGyroTime;
    PX4_SensorMagMsg_t            SensorMagMsg;
    uint64                        LastMagTime;
//    PX4_ActuatorControlsMsg_t     ActuatorControls0Msg;
//    uint64                        LastActuatorControls0Time;
    PX4_InputRcMsg_t              InputRcMsg;
    uint64                        LastInputRcTime;
    PX4_DifferentialPressureMsg_t DifferentialPressureMsg;
    uint64                        LastDifferentialPressureTime;
    PX4_VehicleControlModeMsg_t   VehicleControlModeMsg;
    uint64                        LastVehicleControlModeTime;
} SENS_CurrentValueTable_t;


/**
 **  \brief SENS Application Class
 */
class SENS
{
public:
    SENS();
    ~SENS();

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
    SENS_ConfigTbl_t* ConfigTblPtr;

    /** \brief Output Data published at the end of cycle */
    PX4_ActuatorControlsMsg_t      ActuatorControls3Msg;
    PX4_BatteryStatusMsg_t         BatteryStatusMsg;
    PX4_AirspeedMsg_t              AirspeedMsg;
    PX4_RcChannelsMsg_t            RcChannelsMsg;
    PX4_ManualControlSetpointMsg_t ManualControlSetpointMsg;
    //PX4_SensorPreflightMsg_t     SensorPreflightMsg;
    PX4_SensorCombinedMsg_t        SensorCombinedMsg;

    bool Armed;

    /** \brief Housekeeping Telemetry for downlink */
    SENS_HkTlm_t HkTlm;

    /** \brief Current Value Table */
    SENS_CurrentValueTable_t CVT;

    math::LowPassFilter2p FilterRoll;
    math::LowPassFilter2p FilterPitch;
    math::LowPassFilter2p FilterYaw;
    math::LowPassFilter2p FilterThrottle;

    /************************************************************************/
    /** \brief Sensors (SENS) application entry point
     **
     **  \par Description
     **       Sensors Task application entry point.  This function
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
    /** \brief Initialize the Sensors (SENS) application
     **
     **  \par Description
     **       Sensors application initialization routine. This
     **       function performs all the required startup steps to
     **       initialize (or restore from CDS) SENS data structures and get
     **       the application registered with the cFE services so it can
     **       begin to receive command messages and send events.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
     **  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
     **  \retstmt Return codes from #SENS_InitEvent               \endcode
     **  \retstmt Return codes from #SENS_InitPipe                \endcode
     **  \retstmt Return codes from #SENS_InitData                \endcode
     **  \retstmt Return codes from #SENS_InitConfigTbl           \endcode
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
     **       cFE Event Services for use by the SENS application.
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
    /** \brief Initialize global variables used by SENS application
     **
     **  \par Description
     **       This function performs the steps required to initialize
     **       the SENS application data.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void InitData(void);

    /************************************************************************/
    /** \brief Initialize message pipesCyclicProcessing
     **
     **  \par Description
     **       This function performs the steps required to setup
     **       initialize the cFE Software Bus message pipes and subscribe to
     **       messages for the SENS application.
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
     **       for the SENS application from the SCH pipe.  This function
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
    /** \brief Sensors Task incoming command processing
     **
     **  \par Description
     **       This function processes incoming commands subscribed
     **       by SENS application
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void ProcessCmdPipe(void);

    /************************************************************************/
    /** \brief Sensors Task application commands
     **
     **  \par Description
     **       This function processes command messages
     **       specific to the SENS application
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
    /** \brief Sends SENS housekeeping message
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
    /** \brief Sends the ActuatorControls3Msg message.
     **
     **  \par Description
     **       This function publishes the ActuatorControls3Msg message containing
     **       <TODO>
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendActuatorControls3Msg(void);
    /************************************************************************/
    /** \brief Sends the BatteryStatusMsg message.
     **
     **  \par Description
     **       This function publishes the BatteryStatusMsg message containing
     **       <TODO>
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendBatteryStatusMsg(void);
    /************************************************************************/
    /** \brief Sends the AirspeedMsg message.
     **
     **  \par Description
     **       This function publishes the AirspeedMsg message containing
     **       <TODO>
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendAirspeedMsg(void);
    /************************************************************************/
    /** \brief Sends the RcChannelsMsg message.
     **
     **  \par Description
     **       This function publishes the RcChannelsMsg message containing
     **       <TODO>
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendRcChannelsMsg(void);
    /************************************************************************/
    /** \brief Sends the ManualControlSetpointMsg message.
     **
     **  \par Description
     **       This function publishes the ManualControlSetpointMsg message containing
     **       <TODO>
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendManualControlSetpointMsg(void);
//    /************************************************************************/
//    /** \brief Sends the SensorPreflightMsg message.
//     **
//     **  \par Description
//     **       This function publishes the SensorPreflightMsg message containing
//     **       <TODO>
//     **
//     **  \par Assumptions, External Events, and Notes:
//     **       None
//     **
//     *************************************************************************/
//    void SendSensorPreflightMsg(void);
    /************************************************************************/
    /** \brief Sends the SensorCombinedMsg message.
     **
     **  \par Description
     **       This function publishes the SensorCombinedMsg message containing
     **       <TODO>
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendSensorCombinedMsg(void);
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

    void UpdateRcFunctions(void);

    float GetRcValue(uint8 func, float min_value, float max_value);

private:
    /************************************************************************/
    /** \brief Initialize the SENS configuration tables.
    **
    **  \par Description
    **       This function initializes SENS's configuration tables.  This
    **       includes <TODO>.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
    **  \retstmt Return codes from #CFE_TBL_Register          \endcode
    **  \retstmt Return codes from #CFE_TBL_Load              \endcode
    **  \retstmt Return codes from #SENS_AcquireConfigPointers \endcode
    **  \endreturns
    **
    *************************************************************************/
    int32  InitConfigTbl(void);

    /************************************************************************/
    /** \brief Obtain SENS configuration tables data pointers.
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

    void ProcessRCInput(void);
    void CombineSensorInput(void);
    void CyclicProcessing(void);
    PX4_SwitchPos_t GetRcSw3PosPosition(uint8 func, float on_th, bool on_inv, float mid_th, bool mid_inv);
    PX4_SwitchPos_t GetRcSw2PosPosition(uint8 func, float on_th, bool on_inv);

public:
    /************************************************************************/
    /** \brief Validate SENS configuration table
    **
    **  \par Description
    **       This function validates SENS's configuration table
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

#endif /* SENS_APP_H */

/************************/
/*  End of File Comment */
/************************/
