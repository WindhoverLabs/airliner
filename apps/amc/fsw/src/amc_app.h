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

#ifndef AMC_APP_H
#define AMC_APP_H


#include <mixer/MultirotorMixer.h>
#include <mixer/SimpleMixer.h>

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

#include "amc_platform_cfg.h"
#include "amc_mission_cfg.h"
#include "amc_perfids.h"
#include "amc_msgids.h"
#include "amc_msg.h"
#include "amc_events.h"
#include "amc_tbldefs.h"
#include "px4_msgs.h"
#include <pwm_limit/pwm_limit.h>



/************************************************************************
 ** Local Defines
 *************************************************************************/
#define AMC_DATA_PIPE_DEPTH    (2)
#define AMC_DATA_PIPE_NAME     "AMC_DATA_PIPE"
#define AMC_ARM_DEBUG_TIMEOUT  (2500)

/************************************************************************
 ** Local Structure Definitions
 *************************************************************************/

typedef struct
{
    PX4_ActuatorArmedMsg_t    ActuatorArmed;
    PX4_ActuatorControlsMsg_t ActuatorControls0;
    PX4_ActuatorControlsMsg_t ActuatorControls1;
    PX4_ActuatorControlsMsg_t ActuatorControls2;
    PX4_ActuatorControlsMsg_t ActuatorControls3;
} AMC_CurrentValueTable_t;


extern "C" void AMC_AppMain(void);


/**
 **  \brief AMC Application Class
 */
class AMC
{
public:
    AMC(void);
    ~AMC();

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

    /** \brief Mixer Config Table Handle */
    CFE_TBL_Handle_t MixerConfigTblHdl;

    /** \brief Config Table Pointer */
    AMC_ConfigTbl_t* ConfigTblPtr;

    /** \brief Mixer Config Table Pointer */
    AMC_Mixer_ConfigTable_t* MixerConfigTblPtr;

    /** \brief Output Data published at the end of cycle */
    PX4_ActuatorOutputsMsg_t ActuatorOutputs;

    /** \brief Housekeeping Telemetry for downlink */
    AMC_HkTlm_t HkTlm;

    AMC_CurrentValueTable_t CVT;

    MultirotorMixer MultirotorMixerObject[AMC_MULTIROTOR_MIXER_MAX_MIXERS];
    SimpleMixer     SimpleMixerObject[AMC_SIMPLE_MIXER_MAX_MIXERS];

    //MIXER_Data_t  MixerData;
    PwmLimit_Data_t PwmLimit;

    int32 ConfigMutex;

    /************************************************************************/
    /** \brief CFS PWM Motor Controller Task (AMC) application entry point
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
    /** \brief Initialize the CFS PWM Motor Controller (AMC) application
     **
     **  \par Description
     **       PWM Motor Controller application initialization routine. This
     **       function performs all the required startup steps to
     **       initialize (or restore from CDS) AMC data structures and get
     **       the application registered with the cFE services so it can
     **       begin to receive command messages and send events.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
     **  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
     **  \retstmt Return codes from #AMC_InitEvent               \endcode
     **  \retstmt Return codes from #AMC_InitPipe                \endcode
     **  \retstmt Return codes from #AMC_InitData                \endcode
     **  \retstmt Return codes from #AMC_InitConfigTbl           \endcode
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
     **       cFE Event Services for use by the AMC application.
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
    /** \brief Initialize global variables used by AMC application
     **
     **  \par Description
     **       This function performs the steps required to initialize
     **       the AMC application data.
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
     **       messages for the AMC application.
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
    int32 InitPipes(void);

    /************************************************************************/
    /** \brief Initialize Scheduler pipes
     **
     **  \par Description
     **       This function performs the steps required to setup
     **       initialize the cFE Software Bus message pipes and subscribe to
     **       messages for the AMC application.
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
    int32 InitSchPipe(void);

    /************************************************************************/
    /** \brief Initialize Command pipes
     **
     **  \par Description
     **       This function performs the steps required to setup
     **       initialize the cFE Software Bus message pipes and subscribe to
     **       messages for the AMC application.
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
    int32 InitCmdPipe(void);

    /************************************************************************/
    /** \brief Initialize Data pipes
     **
     **  \par Description
     **       This function performs the steps required to setup
     **       initialize the cFE Software Bus message pipes and subscribe to
     **       messages for the AMC application.
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
    int32 InitDataPipe(void);

    /************************************************************************/
    /** \brief Receive and process messages from the scheduler pipe.
     **
     **  \par Description
     **       This function receives and processes messages
     **       for the AMC application from the SCH pipe.  This function
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
    /** \brief PWM Motor Controller Task incoming command processing
     **
     **  \par Description
     **       This function processes incoming commands subscribed
     **       by AMC application
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void ProcessCmdPipe(void);

    /************************************************************************/
    /** \brief PWM Motor Controller Task incoming data processing
     **
     **  \par Description
     **       This function processes incoming data messages subscribed
     **       by AMC application
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void ProcessDataPipe(void);

    /************************************************************************/
    /** \brief PWM Motor Controller Task application commands
     **
     **  \par Description
     **       This function processes command messages
     **       specific to the AMC application
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
    /** \brief Sends AMC housekeeping message
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
    void SendActuatorOutputs(void);

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
    /** \brief Set the actual motor outputs.
     **
     **  \par Description
     **       This function takes an array of uint16 representing motor
     **       speeds for each of the motors in the array, and sets the motor
     **       to that specific speed.  The range of inputs is configuration
     **       specific.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \param [in]   *PWM          A pointer to an array of uint16.  The
     **                              array size is defined by
     **                              AMC_MAX_MOTOR_OUTPUTS.
     **                              references the software bus message
     **
     *************************************************************************/
    void   SetMotorOutputs(const uint16 *PWM);

    /************************************************************************/
    /** \brief Initialize device
     **
     **  \par Description
     **       This function is defined in the platform specific package and
     **       initializes the device, whatever it is, to allow the application
     **       to set motor speeds.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  Returns 0 if successful.  Returns a negative number if unsuccessful.
     **  Meaning of actual return value is platform specific.
     **  \endreturns
     **
     *************************************************************************/
    int32  InitDevice(void);

    /************************************************************************/
    /** \brief Update motors
     **
     **  \par Description
     **       This function mixes the yaw, pitch, roll, and throttle actuators
     **       and sets each specific motor to result in the commanded attitude
     **       change.  Exactly to which proportion each motor affects each
     **       actuator position is defined in the #MixerConfigTblHdl table.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void   UpdateMotors(void);

    /************************************************************************/
    /** \brief Stop motors
     **
     **  \par Description
     **       This function commands the motors to immediately stop.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void   StopMotors(void);

    /************************************************************************/
    /** \brief Initialize the AMC configuration tables.
    **
    **  \par Description
    **       This function initializes AMC's configuration tables.  This
    **       includes the PWM and the Mixer configuration table.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
    **  \retstmt Return codes from #CFE_TBL_Register          \endcode
    **  \retstmt Return codes from #CFE_TBL_Load              \endcode
    **  \retstmt Return codes from #AMC_AcquireConfigPointers \endcode
    **  \endreturns
    **
    *************************************************************************/
    int32  InitConfigTbl(void);

    /************************************************************************/
    /** \brief Obtain AMC configuration tables data pointers.
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
    /** \brief Validate AMC PWM configuration table
    **
    **  \par Description
    **       This function validates AMC's PWM configuration table
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
    static int32  ValidateCfgTbl(void*);

    /************************************************************************/
    /** \brief Validate AMC Mixer configuration table
    **
    **  \par Description
    **       This function validates AMC's Mixer configuration table
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
    static int32  ValidateMixerCfgTbl(void*);

    /************************************************************************/
    /** \brief Return the specified control value
    **
    **  \par Description
    **       This function is called by the Mixer object and returns the
    **       value of the specified control.  This specific function only
    **       supports ControlGroup 0 and will return an error otherwise.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
     **
     **  \param [in]   ControlGroup    Index of the control group.
     **  \param [in]   ControlIndex    Index of the control.
    **
    **  \returns
    **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
    **  \retcode -1  \retdesc Invalid input  \endcode
    **  \endreturns
    **
    *************************************************************************/
    static int32  ControlCallback(
        uint8 ControlGroup,
        uint8 ControlIndex,
        float &Control);

    void DisplayInputs(void);

    static void CleanupCallback(void);
};

#ifdef __cplusplus
}
#endif

#endif /* AMC_APP_H */

/************************/
/*  End of File Comment */
/************************/
