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

#ifndef RGBLED_APP_H
#define RGBLED_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
 ** Includes
 *************************************************************************/
#include "cfe.h"
#include "rgbled_platform_cfg.h"
#include "rgbled_mission_cfg.h"
#include "rgbled_perfids.h"
#include "rgbled_msgids.h"
#include "rgbled_msg.h"
#include "rgbled_events.h"
#include "px4_msgs.h"
#include "rgbled_custom.h"

/************************************************************************
 ** Local Structure Definitions
 *************************************************************************/
/**
 * \brief LED device status
 */
typedef enum
{
    /*! RGBLED app status uninitialized */
    RGBLED_UNINITIALIZED = 0,
    /*! RGBLED app status uninitialized */
    RGBLED_INITIALIZED   = 1,
    /*! RGBLED app status on */
    RGBLED_ON            = 2,
    /*! RGBLED app status self-test */
    RGBLED_SELFTEST      = 3
} RGBLED_Status_t;


/**
 * \brief LED colors
 */
typedef enum
{
    RGBLED_COLOR_OFF    = 0,
    RGBLED_COLOR_RED    = 1,
    RGBLED_COLOR_GREEN  = 2,
    RGBLED_COLOR_BLUE   = 3,
    RGBLED_COLOR_AMBER  = 4,
    RGBLED_COLOR_YELLOW = 5,
    RGBLED_COLOR_PURPLE = 6,
    RGBLED_COLOR_CYAN   = 7,
    RGBLED_COLOR_WHITE  = 8
} RGBLED_Colors_t;


/**
 * \brief LED modes (not currently implemented).
 */
typedef enum
{
    RGBLED_MODE_OFF          = 0,
    RGBLED_MODE_ON           = 1,
    RGBLED_MODE_DISABLED     = 2,
    RGBLED_MODE_BLINK_SLOW   = 3,
    RGBLED_MODE_BLINK_NORMAL = 4,
    RGBLED_MODE_BLINK_FAST   = 5,
    RGBLED_MODE_BREATHE      = 6
} RGBLED_Modes_t;

/**
 * \brief LED Message current value table.
 */
typedef struct
{
    PX4_LedControlMsg_t RGBLEDControl;
} RGBLED_CurrentValueTable_t;


/**
 **  \brief RGBLED Application Class
 */
class RGBLED
{
public:
    RGBLED();
    ~RGBLED();

    /**\brief Scheduling Pipe ID */
    CFE_SB_PipeId_t SchPipeId;

    /** \brief Command Pipe ID */
    CFE_SB_PipeId_t CmdPipeId;

    /* Task-related */
    /** \brief Task Run Status */
    uint32 uiRunStatus;
    
    /** \brief The previous state of the app */
    uint8 previousState;

    /** \brief Housekeeping Telemetry for downlink */
    RGBLED_HkTlm_t HkTlm;

    /** \brief Current Value Table */
    RGBLED_CurrentValueTable_t CVT;

    /************************************************************************/
    /** \brief RGBLED Control (RGBLED) application entry point
     **
     **  \par Description
     **       RGBLED Control Task application entry point.  This function
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
    /** \brief Initialize the RGBLED Control (RGBLED) application
     **
     **  \par Description
     **       RGBLED Control application initialization routine. This
     **       function performs all the required startup steps to
     **       initialize (or restore from CDS) RGBLED data structures and get
     **       the application registered with the cFE services so it can
     **       begin to receive command messages and send events.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
     **  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
     **  \retstmt Return codes from #RGBLED_InitEvent               \endcode
     **  \retstmt Return codes from #RGBLED_InitPipe                \endcode
     **  \retstmt Return codes from #RGBLED_InitData                \endcode
     **  \retstmt Return codes from #RGBLED_InitConfigTbl           \endcode
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
     **       cFE Event Services for use by the RGBLED application.
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
    /** \brief Initialize global variables used by RGBLED application
     **
     **  \par Description
     **       This function performs the steps required to initialize
     **       the RGBLED application data.
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
     **       messages for the RGBLED application.
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
     **       for the RGBLED application from the SCH pipe.  This function
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
    /** \brief RGBLED Control Task incoming command processing
     **
     **  \par Description
     **       This function processes incoming commands subscribed
     **       by RGBLED application
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void ProcessCmdPipe(void);

    /************************************************************************/
    /** \brief RGBLED Control Task application commands
     **
     **  \par Description
     **       This function processes command messages
     **       specific to the RGBLED application
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
    /** \brief Sends RGBLED housekeeping message
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
};


/************************************************************************/
/** \brief RGBLED cleanup prior to exit
**
**  \par Description
**       This function handles any necessary cleanup prior
**       to application exit.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void RGBLED_CleanupCallback(void);


#ifdef __cplusplus
}
#endif 

#endif /* RGBLED_APP_H */

/************************/
/*  End of File Comment */
/************************/
