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


#ifndef _VC_APP_H
#define _VC_APP_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "vc_msg.h"
#include "vc_events.h"
#include "vc_mission_cfg.h"
#include "vc_platform_cfg.h"
#include "vc_version.h"
#include "vc_transmit.h"
#include "vc_dev_io.h"
#include "vc_custom.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Macro Definitions
*************************************************************************/
#define VC_MAX_EVENT_FILTERS (32)

/************************************************************************
** Type Definitions
*************************************************************************/

/**
 * \brief Device status
 */
typedef enum
{
    /*! VC app status uninitialized */
    VC_UNINITIALIZED = 0,
    /*! VC app status uninitialized */
    VC_INITIALIZED   = 1,
    /*! VC app status streaming */
    VC_STREAMING     = 2
} VC_Status_t;


/** 
 *  \brief VC global data structure
 */
typedef struct
{
    /** \brief Housekeeping Telemetry for downlink */
    VC_HkTlm_t  HkTlm;
    /**\brief Scheduling Pipe ID */
    CFE_SB_PipeId_t  SchPipeId;
    /** \brief Command Pipe ID */
    CFE_SB_PipeId_t  CmdPipeId;
    /* Run Status variable used in the main processing loop */
    /** \brief Task Run Status */
    uint32                uiRunStatus;
    /** \brief VC App State */
    VC_Status_t           AppState;
    
} VC_AppData_t;


/************************************************************************
** Function Prototypes
*************************************************************************/

/************************************************************************/
/** \brief CFS Video Control (VC) application entry point
**
**  \par Description
**       CFS Video control application entry point.  This function
**       performs app initialization, custom transmit and device 
**       initialization, and starts streaming in a child task.
**
**  \par Assumptions, External Events, and Notes:
**       If there is an unrecoverable failure during initialization the
**       main loop is never executed and the application will exit. An
**       error includes all initialization but excludes "start 
**       streaming". If streaming fails to start VC will not exit and 
**       will indicate an initialized state.
**
**
*************************************************************************/
void VC_AppMain(void);


/************************************************************************/
/** \brief Initialize the CFS Video Control (VC) application
**
**  \par Description
**       Video Control application initialization routine. This
**       function performs all the required startup steps to
**       initialize VC data structures, custom device resources,
**       and custom transmit resources, and start streaming video.
**       Init also registers the applications with the cFE services so 
**       it can begin to receive command messages and send events.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
**  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
**  \retstmt Return codes from #VC_InitEvent                \endcode
**  \retstmt Return codes from #VC_InitPipe                 \endcode
**  \retstmt Return codes from #VC_InitData                 \endcode
**  \retstmt Return codes from #OS_TaskInstallDeleteHandler \endcode
**  \endreturns
**
*************************************************************************/
int32 VC_InitApp(void);


/************************************************************************/
/** \brief Initialize message pipes
**
**  \par Description
**       This function performs the steps required to setup
**       initialize the cFE Software Bus message pipes and subscribe to
**       messages for the VC application.
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
int32 VC_InitPipe(void);


/************************************************************************/
/** \brief Video Control (VC) critical cleanup prior to exit.
**
**  \par Description
**       This function handles any critical cleanup prior
**       to application exit.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void VC_CleanupCallback(void);


/************************************************************************/
/** \brief Video Control (VC) cleanup prior to exit
**
**  \par Description
**       This function handles any necessary cleanup prior
**       to application exit.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void VC_CleanupExit(void);


/************************************************************************/
/** \brief Initialize Event Services and Event tables
**
**  \par Description
**       This function performs the steps required to setup
**       cFE Event Services for use by the VC application.
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
int32  VC_InitEvent(void);


/************************************************************************/
/** \brief Initialize global variables used by VC application
**
**  \par Description
**       This function performs the steps required to initialize
**       the VC application data.
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
int32  VC_InitData(void);


/************************************************************************/
/** \brief Receive and process software bus messages
**
**  \par Description
**       This function processes incoming SB messages and routes
**       messages to the appropriete helper function to process commands
**       or report housekeeping etc.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   iBlocking    timeOut CFE_SB_PEND, CFE_SB_POLL or 
**                             millisecond timeout
**
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \endreturns
** 
*************************************************************************/
int32 VC_RcvMsg(int32 iBlocking);


/************************************************************************/
/** \brief Video Control Task incoming command processing
**
**  \par Description
**       This function processes incoming commands subscribed
**       by VC application
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  VC_ProcessNewCmds(void);


/************************************************************************/
/** \brief Video Control Task application commands
**
**  \par Description
**       This function processes command messages
**       specific to the VC application
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   MsgPtr       A #CFE_SB_Msg_t pointer that
**                             references the software bus message
**
*************************************************************************/
void  VC_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr);


/************************************************************************
** External Global Variables
*************************************************************************/
extern VC_AppData_t     VC_AppData;


#ifdef __cplusplus
}
#endif 

#endif /* VC_APP_H */

/************************/
/*  End of File Comment */
/************************/
