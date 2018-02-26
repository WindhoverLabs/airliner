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

/************************************************************************
 ** Local Defines
 *************************************************************************/

/************************************************************************
 ** Local Structure Definitions
 *************************************************************************/
typedef struct
{
    PX4_SensorMagMsg_t SensorMagMsg;
    PX4_SensorGyroMsg_t SensorGyroMsg;
    PX4_SystemPowerMsg_t SystemPowerMsg;
//    PX4_SensorPreflightMsg_t SensorPreflightMsg;
    PX4_TelemetryStatusMsg_t TelemetryStatusMsg;
    PX4_SubsystemInfoMsg_t SubsystemInfoMsg;
    PX4_VehicleGpsPositionMsg_t VehicleGpsPositionMsg;
    PX4_VehicleAttitudeMsg_t VehicleAttitudeMsg;
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
    PX4_VehicleStatusMsg_t VehicleStatusMsg;
    PX4_SensorCombinedMsg_t SensorCombinedMsg;
} VM_CurrentValueTable_t;


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
    /** \brief Output Data published at the end of cycle */
    PX4_ActuatorArmedMsg_t ActuatorArmedMsg;
    PX4_HomePositionMsg_t HomePositionMsg;
    PX4_CommanderStateMsg_t VehicleManagerStateMsg;
    PX4_MissionMsg_t MissionMsg;
    PX4_LedControlMsg_t LedControlMsg;
    PX4_VehicleStatusMsg_t VehicleStatusMsg;
//    PX4_VehicleRoiMsg_t VehicleRoiMsg;

    VM_Arming  ArmingSM;

    /** \brief Housekeeping Telemetry for downlink */
    VM_HkTlm_t HkTlm;
    /** \brief Current Value Table */
    VM_CurrentValueTable_t CVT;
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

//    /************************************************************************/
//    /** \brief Sends the VehicleRoiMsg message.
//     **
//     **  \par Description
//     **       This function publishes the VehicleRoiMsg message containing
//     **       <TODO>
//     **
//     **  \par Assumptions, External Events, and Notes:
//     **       None
//     **
//     *************************************************************************/
//    void SendVehicleRoiMsg(void);

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
