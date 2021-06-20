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

#ifndef TO_APP_H
#define TO_APP_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"

#include "to_platform_cfg.h"
#include "to_mission_cfg.h"
#include "to_perfids.h"
#include "to_msgids.h"
#include "to_msg.h"
#include "to_events.h"
#include "to_config_utils.h"
#include "to_tbldefs.h"
#include "to_message_flow.h"
#include "to_priority_queue.h"
#include "to_scheduler.h"
#include "to_output_queue.h"
#include "to_custom.h"
#include "to_channel.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Local Defines
*************************************************************************/
#define TO_HKTLM_CHANNEL_MUTEX_NAME      ("TO_HK_CH_MUTEX")

/************************************************************************
** Local Structure Definitions
*************************************************************************/
/**
**  \brief TO Operational Data Structure
*/
typedef struct
{
    /* Task-related */
    /** \brief Task Run Status */
    uint32                runStatus;

    /** \brief Housekeeping Telemetry for downlink */
    TO_HkTlm_t            HkTlm;

    /** \brief Mutex for TO AppData */
    uint32                MutexID;

    /** \brief Channel Data */
    TO_ChannelData_t      ChannelData[TO_MAX_CHANNELS];

    /** \brief CFE Event Table */
    CFE_EVS_BinFilter_t   EventTbl[CFE_EVS_MAX_EVENT_FILTERS];
    /* CFE_EVS_BinFilter_t is two uint16's */

    uint16                OutMessageSize;

    /**\brief Scheduling Pipe ID */
    CFE_SB_PipeId_t       SchPipeId;

    /** \brief Command Pipe ID */
    CFE_SB_PipeId_t       CmdPipeId;

} TO_AppData_t;

/************************************************************************
** External Global Variables
*************************************************************************/
extern TO_AppData_t TO_AppData;

/************************************************************************
** Global Variables
*************************************************************************/

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Prototypes
*************************************************************************/

/************************************************************************/
/** \brief CFS Telemetry Output Task (TO) application entry point
**
**  \par Description
**       CFS Telemetry Output Task application entry point.  This function
**       performs app initialization, then waits for the cFE ES Startup
**       Sync, then executes the RPR main processing loop.
**
**  \par Assumptions, External Events, and Notes:
**       If there is an unrecoverable failure during initialization the
**       main loop is never executed and the application will exit.
**
*************************************************************************/
void  TO_AppMain(void);

/************************************************************************/
/** \brief Initialize the CFS Telemetry Output (TO) application
**
**  \par Description
**       Telemetry Output application initialization routine. This
**       function performs all the required startup steps to
**       initialize TO data structures and get
**       the application registered with the cFE services so it can
**       begin to receive command messages and send events.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \return
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
**  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
**  \retstmt Return codes from #TO_InitEvent                \endcode
**  \retstmt Return codes from #TO_InitPipe                 \endcode
**  \retstmt Return codes from #TO_InitData                 \endcode
**  \retstmt Return codes from #TO_InitTables               \endcode
**  \retstmt Return codes from #OS_TaskInstallDeleteHandler \endcode
**  \endreturns
**
*************************************************************************/
int32  TO_InitApp(void);

/************************************************************************/
/** \brief Initialize Event Services and Event tables
**
**  \par Description
**       This function performs the steps required to setup
**       cFE Event Services for use by the TO application.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \return
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt Return codes from #CFE_EVS_Register  \endcode
**  \endreturns
**
*************************************************************************/
int32  TO_InitEvent(void);

/************************************************************************/
/** \brief Initialize global variables used by TO application
**
**  \par Description
**       This function performs the steps required to initialize
**       the TO application data.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \return
**   OSAL error if unsuccessful.
**
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt Return codes from #OS_MutSemCreate  \endcode
**
*************************************************************************/
int32 TO_InitData(void);

/************************************************************************/
/** \brief Initialize message pipes
**
**  \par Description
**       This function performs the steps required to setup
**       initialize the cFE Software Bus message pipes and subscribe to
**       messages for the TO application.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \return
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt Return codes from #CFE_SB_CreatePipe        \endcode
**  \retstmt Return codes from #CFE_SB_SubscribeEx       \endcode
**  \retstmt Return codes from #CFE_SB_Subscribe         \endcode
**  \endreturns
**
*************************************************************************/
int32  TO_InitPipe(void);

/************************************************************************/
/** \brief Receive and process messages
**
**  \par Description
**       This function receives and processes messages
**       for the TO application
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   blocking    A #CFE_SB_PEND_FOREVER, #CFE_SB_POLL or
**                             millisecond timeout
**
**  \return
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt Return codes from #CFE_SB_RcvMsg            \endcode
**  \endreturns
**
*************************************************************************/
int32  TO_RcvMsg(int32 blocking);

/************************************************************************/
/** \brief Telemetry Output Task incoming command processing
**
**  \par Description
**       This function processes incoming commands subscribed
**       by TO application
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  TO_ProcessNewCmds(void);

/************************************************************************/
/** \brief Telemetry Output Task application commands
**
**  \par Description
**       This function processes command messages
**       specific to the TO application
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   msgPtr       A #CFE_SB_Msg_t pointer that
**                             references the software bus message
**
*************************************************************************/
void  TO_ProcessNewAppCmds(CFE_SB_Msg_t *msgPtr);


/************************************************************************/
/** \brief Sends TO housekeeping message
**
**  \par Description
**       This function sends the housekeeping message
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  TO_ReportHousekeeping(void);

/************************************************************************/
/** \brief Verify Command Length
**
**  \par Description
**       This routine will check if the actual length of a software bus
**       command message matches the expected length and send an
**       error event message if a mismatch occurs
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   msgPtr        A #CFE_SB_Msg_t pointer that
**                              references the software bus message

**  \param [in]   expectedLen The expected length of the message
**
**  \return
**  \retstmt Returns TRUE if the length is as expected      \endcode
**  \retstmt Returns FALSE if the length is not as expected \endcode
**
**  \sa #TO_MSG_LEN_ERR_EID
**
*************************************************************************/
osalbool  TO_VerifyCmdLength(CFE_SB_Msg_t *msgPtr, uint16 expectedLen);

/************************************************************************/
/** \brief Sends TO Diagnostic Telemetry
**
**  \par Description
**       This function publishes the TO application diagnostics telemetry.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   ChannelIdx Channel index
**
**  \return
**  TRUE if command is executed without error, FALSE if it does not.
**
*************************************************************************/
osalbool  TO_SendDiag(uint16 ChannelIdx);

void TO_Cleanup(void);

#ifdef __cplusplus
}
#endif 

#endif /* TO_APP_H */

/************************/
/*  End of File Comment */
/************************/
