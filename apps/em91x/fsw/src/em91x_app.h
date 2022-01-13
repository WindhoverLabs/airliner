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
    
#ifndef EM91X_APP_H
#define EM91X_APP_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"

#include "em91x_platform_cfg.h"
#include "em91x_mission_cfg.h"
#include "em91x_private_ids.h"
#include "em91x_private_types.h"
#include "em91x_perfids.h"
#include "em91x_msgids.h"
#include "em91x_msg.h"
#include "em91x_events.h"
#include "em91x_config_utils.h"
#include "em91x_cds_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Local Defines
*************************************************************************/
#define EM91X_TIMEOUT_MSEC             	(1000)

/************************************************************************
** Local Structure Definitions
*************************************************************************/
/**
**  \brief EM91X Operational Data Structure
*/
typedef struct
{
    /** \brief CFE Event Table */
    CFE_EVS_BinFilter_t  EventTbl[EM91X_EVT_CNT];

    /**\brief Scheduling Pipe ID */
    CFE_SB_PipeId_t  SchPipeId;

    /** \brief Command Pipe ID */
    CFE_SB_PipeId_t  CmdPipeId;

    /** \brief Data Pipe ID */
    CFE_SB_PipeId_t  DataPipeId;

    /* Task-related */

    /** \brief Task Run Status */
    uint32  uiRunStatus;

    /* Config table-related */

    /** \brief Config Table Handle */
    CFE_TBL_Handle_t  ConfigTblHdl;

    /** \brief Config Table Pointer */
    EM91X_ConfigTblEntry_t*  ConfigTblPtr;

    /* Critical Data Storage (CDS) table-related */

    /** \brief CDS Table Handle */
    CFE_ES_CDSHandle_t  CdsTblHdl;

    /** \brief CDS Table data */
    EM91X_CdsTbl_t  CdsTbl;

    /* Inputs/Outputs */

    /** \brief Input Data from I/O or other apps */
    EM91X_InData_t   InData;

    /** \brief Output Data published at the end of cycle */
    EM91X_OutData_t  OutData;

    /** \brief Housekeeping Telemetry for downlink */
    EM91X_HkTlm_t  HkTlm;

} EM91X_AppData_t;

/************************************************************************
** External Global Variables
*************************************************************************/

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
/** \brief CFS Sierra Wireless EM9190/EM9191 Task (EM91X) application entry point
**
**  \par Description
**       CFS Sierra Wireless EM9190/EM9191 Task application entry point.  This function
**       performs app initialization, then waits for the cFE ES Startup
**       Sync, then executes the RPR main processing loop.
**
**  \par Assumptions, External Events, and Notes:
**       If there is an unrecoverable failure during initialization the
**       main loop is never executed and the application will exit.
**
*************************************************************************/
void  EM91X_AppMain(void);

/************************************************************************/
/** \brief Initialize the CFS Sierra Wireless EM9190/EM9191 (EM91X) application
**
**  \par Description
**       Sierra Wireless EM9190/EM9191 application initialization routine. This
**       function performs all the required startup steps to
**       initialize (or restore from CDS) EM91X data structures and get
**       the application registered with the cFE services so it can
**       begin to receive command messages and send events.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
**  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
**  \retstmt Return codes from #EM91X_InitEvent               \endcode
**  \retstmt Return codes from #EM91X_InitPipe                \endcode
**  \retstmt Return codes from #EM91X_InitData                \endcode
**  \retstmt Return codes from #EM91X_InitConfigTbl           \endcode
**  \retstmt Return codes from #EM91X_InitCdsTbl              \endcode
**  \retstmt Return codes from #OS_TaskInstallDeleteHandler \endcode
**  \endreturns
**
*************************************************************************/
int32  EM91X_InitApp(void);

/************************************************************************/
/** \brief Initialize Event Services and Event tables
**
**  \par Description
**       This function performs the steps required to setup
**       cFE Event Services for use by the EM91X application.
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
int32  EM91X_InitEvent(void);

/************************************************************************/
/** \brief Initialize global variables used by EM91X application
**
**  \par Description
**       This function performs the steps required to initialize
**       the EM91X application data.
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
int32  EM91X_InitData(void);

/************************************************************************/
/** \brief Initialize message pipes
**
**  \par Description
**       This function performs the steps required to setup
**       initialize the cFE Software Bus message pipes and subscribe to
**       messages for the EM91X application.
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
int32  EM91X_InitPipe(void);

/************************************************************************/
/** \brief Receive and process messages
**
**  \par Description
**       This function receives and processes messages
**       for the EM91X application
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
int32  EM91X_RcvMsg(int32 iBlocking);

/************************************************************************/
/** \brief Sierra Wireless EM9190/EM9191 Task incoming data processing
**
**  \par Description
**       This function processes incoming data subscribed
**       by EM91X application
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  EM91X_ProcessNewData(void);

/************************************************************************/
/** \brief Sierra Wireless EM9190/EM9191 Task incoming command processing
**
**  \par Description
**       This function processes incoming commands subscribed
**       by EM91X application
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  EM91X_ProcessNewCmds(void);

/************************************************************************/
/** \brief Sierra Wireless EM9190/EM9191 Task application commands
**
**  \par Description
**       This function processes command messages
**       specific to the EM91X application
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   MsgPtr       A #CFE_SB_Msg_t pointer that
**                             references the software bus message
**
*************************************************************************/
void  EM91X_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr);

/************************************************************************/
/** \brief Sends EM91X housekeeping message
**
**  \par Description
**       This function sends the housekeeping message
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  EM91X_ReportHousekeeping(void);

/************************************************************************/
/** \brief Sends EM91X output data
**
**  \par Description
**       This function publishes the EM91X application output data.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  EM91X_SendOutData(void);

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
boolean  EM91X_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr, uint16 usExpectedLen);

#ifdef __cplusplus
}
#endif 

#endif /* EM91X_APP_H */

/************************/
/*  End of File Comment */
/************************/
