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

#ifndef ULR_APP_H
#define ULR_APP_H

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
#include "ulr_platform_cfg.h"
#include "ulr_mission_cfg.h"
#include "ulr_perfids.h"
#include "ulr_msgids.h"
#include "ulr_msg.h"
#include "ulr_events.h"
#include "px4_msgs.h"
#include "math/filters/LowPassFilter2p.hpp"


/************************************************************************
 ** Local Defines
 *************************************************************************/

/** \brief This is the size of the buffer exchanged by the platform
 *         independent and custom layer. */
#define ULR_BUF_LEN                     (6)


/************************************************************************
 ** Local Structure Definitions
 *************************************************************************/

/** \brief These are the states of the stream style message parser.  The
 *         state machine design makes the parser resilient to drop out and
 *         noise. */
typedef enum
{
    ULR_PARSER_STATE_UNINITIALIZED           = 0,
    ULR_PARSER_STATE_WAITING_FOR_HEADER      = 1,
    ULR_PARSER_STATE_WAITING_FOR_VERSION_ID  = 2,
    ULR_PARSER_STATE_WAITING_FOR_ALT_BYTE_1  = 3,
    ULR_PARSER_STATE_WAITING_FOR_ALT_BYTE_2  = 4,
    ULR_PARSER_STATE_WAITING_FOR_SNR         = 5,
    ULR_PARSER_STATE_WAITING_FOR_CHECKSUM    = 6
} ULR_ParserState_t;


/** \brief This is the structure passed from the custom layer to the platform
 *         independent layer. */
typedef struct
{
    uint8  VersionID;
    uint8  AltitudeH;
    uint8  AltitudeL;
    uint8  SNR;
    uint8  Checksum;
} ULR_UartMessage_t;


/** \brief Listener task C style entry point. */
extern "C" void ULR_ListenerTaskMain();


/**
 **  \brief ULR Application Class
 */
class ULR
{
public:
    /** \brief Default constructor. */
    ULR();

    /** \brief Destructor. */
    ~ULR();

    /**\brief Scheduling Pipe ID */
    CFE_SB_PipeId_t SchPipeId;

    /** \brief Command Pipe ID */
    CFE_SB_PipeId_t CmdPipeId;

    /* Task-related */
    /** \brief This variable contains the current state of the message
     *        parser. */
    ULR_ParserState_t ParserState;

    /** \brief This is the buffer passed from the custom layer to the platform
     *        independent layer.  Having this as a member attribute allows us
     *        to possibly debug certain errors by using MM to read the
     *        contents of this variable.  We can't do that with variables
     *        allocated on the stack. */
    uint8             ParserBuffer[ULR_BUF_LEN];

    /** \brief This is the fully assembled message.  Again, having this as a
     *         member attribute allows us to possibly debug certain errors by
     *         using MM to read the contents of this variable.  We can't do
     *         that with variables allocated on the stack. */
    ULR_UartMessage_t UartMessage;

    /** \brief Task Run Status */
    uint32 uiRunStatus;

    /** \brief Output Data published at the end of cycle */
    PX4_DistanceSensorMsg_t DistanceSensor;

    /** \brief Housekeeping Telemetry for downlink */
    ULR_HkTlm_t HkTlm;

    /** \brief ID of listener child task */
    uint32 ListenerTaskID;

    /** \brief ID of the mutex used to protect data shared between the parent
     *         and the child task. */
    uint32 Mutex;
    
    /** \brief Low pass filter. */
    math::LowPassFilter2p   HeightFilter;

    /** \brief This flag is used by an internal function to signal whether the
     *         child task should continue looping or terminate.
     */
    osalbool ChildContinueFlag;

    /************************************************************************/
    /** \brief Aerotenna uLanding Radar (ULR) application entry point
     **
     **  \par Description
     **       Aerotenna uLanding Radar Task application entry point.  This function
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
    /** \brief Listener task entry point
     **
     **  \par Description
     **       This function waits for device messages from the custom layer.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void ListenerTaskMain(void);

    /************************************************************************/
    /** \brief Listener task entry point
     **
     **  \par Description
     **       This function initializes the listener task.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    int32 InitListenerTask(void);

    /************************************************************************/
    /** \brief Initialize the Aerotenna uLanding Radar (ULR) application
     **
     **  \par Description
     **       Aerotenna uLanding Radar application initialization routine. This
     **       function performs all the required startup steps to
     **       initialize (or restore from CDS) ULR data structures and get
     **       the application registered with the cFE services so it can
     **       begin to receive command messages and send events.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
     **  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
     **  \retstmt Return codes from #ULR_InitEvent               \endcode
     **  \retstmt Return codes from #ULR_InitPipe                \endcode
     **  \retstmt Return codes from #ULR_InitData                \endcode
     **  \retstmt Return codes from #ULR_InitConfigTbl           \endcode
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
     **       cFE Event Services for use by the ULR application.
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
    /** \brief Initialize global variables used by ULR application
     **
     **  \par Description
     **       This function performs the steps required to initialize
     **       the ULR application data.
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
     **       messages for the ULR application.
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
     **       for the ULR application from the SCH pipe.  This function
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
    /** \brief Aerotenna uLanding Radar Task incoming command processing
     **
     **  \par Description
     **       This function processes incoming commands subscribed
     **       by ULR application
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void ProcessCmdPipe(void);

    /************************************************************************/
    /** \brief Aerotenna uLanding Radar Task application commands
     **
     **  \par Description
     **       This function processes command messages
     **       specific to the ULR application
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
    /** \brief Sends ULR housekeeping message
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
    /** \brief Sends the DistanceSensor message.
     **
     **  \par Description
     **       This function publishes the DistanceSensor message.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendDistanceSensor(void);

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
    /** \brief Initialize device
     **
     **  \par Description
     **       This function is defined in the platform specific package and
     **       initializes the device, whatever it is, to allow the application
     **       to read the sensor.
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
    /** \brief Report the distance to the ground.
     **
     **  \par Description
     **       This function publishes a most current distance measurement onto
     **       the software bus.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void     ReportDistance(void);

    /************************************************************************/
    /** \brief Determines if child task should continue.
     **
     **  \par Description
     **       This function determines if the child task should continue or
     **       if it should terminate.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  TRUE if the child task should continue, FALSE if it should not.
     **  \endreturns
     **
     *************************************************************************/
    osalbool ChildContinueExec(void);

    /************************************************************************/
    /** \brief Signal the child task to terminate.
     **
     **  \par Description
     **       This function is called by the parent task to signal the child
     **       task to terminate execution.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void     StopChild(void);

    /************************************************************************/
    /** \brief Get the distance measurements.
     **
     **  \par Description
     **       This function measures the distance, applies limits, and publishes
     **       the results.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  Returns 0 if successful.  Returns a negative number if unsuccessful.
     **  \endreturns
     **
     *************************************************************************/
    int32  GetDistance(void);

    /************************************************************************/
    /** \brief Read a measurement from the device.
     **
     **  \par Description
     **       This function retrieves a measurement from the device.  This
     **       function is defined in the platform specific package.  It
     **       provides the platform specific interface to the device, and
     **       applies any platform specific calibrations to return a
     **       measurement in <TODO: Get units>.
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
    int32 ReadDevice(uint8 *Buffer, uint32 *Size);

    /************************************************************************/
    /** \brief Close the uLanding Radar device.
     **
     **  \par Description
     **       This function is called when shutdown to close the device.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void  CloseDevice(void);

public:
    /************************************************************************/
    /** \brief Validate ULR configuration table
    **
    **  \par Description
    **       This function validates ULR's configuration table
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
    /** \brief Verifies device message checksum
     **
     **  \par Description
     **       This function verifies the message from the device by checking
     **       the actual reported checksum against the expected echecksum..
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  TRUE if the actual and expected checksums match, FALSE if they do
     **  not.
     **  \endreturns
     **
     *************************************************************************/
    osalbool IsChecksumOk(void);
};

#ifdef __cplusplus
}
#endif 

#endif /* ULR_APP_H */

/************************/
/*  End of File Comment */
/************************/
