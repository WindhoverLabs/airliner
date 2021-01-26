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

#ifndef RCIN_APP_H
#define RCIN_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
 ** Includes
 *************************************************************************/
#include "cfe.h"
#include "rcin_platform_cfg.h"
#include "rcin_mission_cfg.h"
#include "rcin_perfids.h"
#include "rcin_msgids.h"
#include "rcin_msg.h"
#include "rcin_events.h"
#include "px4_msgs.h"

/************************************************************************
 ** Local Defines
 *************************************************************************/

/**\brief RCIN specific error code */
#define RCIN_ERROR (-1)

/**\brief RCIN threshold for failed reads */
#define RCIN_STRIKE_COUNT_THRES (10)

/************************************************************************
 ** Local Structure Definitions
 *************************************************************************/
 
/**
 * \brief Application status
 */
typedef enum
{
    /*! App status uninitialized */
    RCIN_UNINITIALIZED = 0,
    /*! App status uninitialized */
    RCIN_INITIALIZED   = 1,
    /*! App status not publishing */
    RCIN_NOTPUBLISHING = 2,
    /*! App status publishing */
    RCIN_PUBLISHING    = 3
} RCIN_Status_t;


/**
 **  \brief RCIN Application Class
 */
class RCIN
{
public:
    RCIN();
    ~RCIN();

    /**\brief Scheduling Pipe ID */
    CFE_SB_PipeId_t SchPipeId;

    /** \brief Command Pipe ID */
    CFE_SB_PipeId_t CmdPipeId;

    /* Task-related */
    /** \brief Task Run Status */
    uint32 uiRunStatus;

    /** \brief Output Data published at the end of cycle */
    PX4_InputRcMsg_t InputRcMsg;

    /** \brief Counter for invalid or failed reads from custom layer */
    uint8 StrikeCount;

    /** \brief Housekeeping Telemetry for downlink */
    RCIN_HkTlm_t HkTlm;
    
    /************************************************************************/
    /** \brief Radio Control Input (RCIN) application entry point
     **
     **  \par Description
     **       Radio Control Input Task application entry point.  This function
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
    /** \brief Initialize the Radio Control Input (RCIN) application
     **
     **  \par Description
     **       Radio Control Input application initialization routine. This
     **       function performs all the required startup steps to
     **       initialize (or restore from CDS) RCIN data structures and get
     **       the application registered with the cFE services so it can
     **       begin to receive command messages and send events.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
     **  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
     **  \retstmt Return codes from #RCIN_InitEvent               \endcode
     **  \retstmt Return codes from #RCIN_InitPipe                \endcode
     **  \retstmt Return codes from #RCIN_InitData                \endcode
     **  \retstmt Return codes from #RCIN_InitConfigTbl           \endcode
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
     **       cFE Event Services for use by the RCIN application.
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
    /** \brief Initialize global variables used by RCIN application
     **
     **  \par Description
     **       This function performs the steps required to initialize
     **       the RCIN application data.
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
     **       messages for the RCIN application.
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
     **       for the RCIN application from the SCH pipe.  This function
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
    /** \brief Radio Control Input Task incoming command processing
     **
     **  \par Description
     **       This function processes incoming commands subscribed
     **       by RCIN application
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void ProcessCmdPipe(void);

    /************************************************************************/
    /** \brief Radio Control Input Task application commands
     **
     **  \par Description
     **       This function processes command messages
     **       specific to the RCIN application
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
    /** \brief Sends RCIN housekeeping message
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
    /** \brief Sends the InputRcMsg message.
     **
     **  \par Description
     **       This function publishes the InputRcMsg message.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendInputRcMsg(void);

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
    /** \brief Read RC device
     **
     **  \par Description
     **       This function calls the custom layer to read from the device and 
     **       updates the InputRC message with current values.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void ReadDevice(void);
};


/************************************************************************/
/** \brief Cleanup prior to exit
**
**  \par Description
**       This function handles any necessary cleanup prior
**       to application exit.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void RCIN_CleanupCallback(void);


#ifdef __cplusplus
}
#endif 

#endif /* RCIN_APP_H */

/************************/
/*  End of File Comment */
/************************/
