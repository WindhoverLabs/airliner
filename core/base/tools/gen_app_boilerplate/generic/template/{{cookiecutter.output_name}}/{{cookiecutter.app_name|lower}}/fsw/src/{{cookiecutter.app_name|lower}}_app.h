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
    
#ifndef {{cookiecutter.app_name}}_APP_H
#define {{cookiecutter.app_name}}_APP_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"

#include "{{cookiecutter.app_name|lower}}_platform_cfg.h"
#include "{{cookiecutter.app_name|lower}}_mission_cfg.h"
#include "{{cookiecutter.app_name|lower}}_private_ids.h"
#include "{{cookiecutter.app_name|lower}}_private_types.h"
#include "{{cookiecutter.app_name|lower}}_perfids.h"
#include "{{cookiecutter.app_name|lower}}_msgids.h"
#include "{{cookiecutter.app_name|lower}}_msg.h"
#include "{{cookiecutter.app_name|lower}}_events.h"
#include "{{cookiecutter.app_name|lower}}_config_utils.h"
#include "{{cookiecutter.app_name|lower}}_cds_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Local Defines
*************************************************************************/
#define {{cookiecutter.app_name}}_TIMEOUT_MSEC             	(1000)

/************************************************************************
** Local Structure Definitions
*************************************************************************/
/**
**  \brief {{cookiecutter.app_name}} Operational Data Structure
*/
typedef struct
{
    /** \brief CFE Event Table */
    CFE_EVS_BinFilter_t  EventTbl[{{cookiecutter.app_name}}_EVT_CNT];

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
    {{cookiecutter.app_name}}_ConfigTblEntry_t*  ConfigTblPtr;

    /* Critical Data Storage (CDS) table-related */

    /** \brief CDS Table Handle */
    CFE_ES_CDSHandle_t  CdsTblHdl;

    /** \brief CDS Table data */
    {{cookiecutter.app_name}}_CdsTbl_t  CdsTbl;

    /* Inputs/Outputs */

    /** \brief Input Data from I/O or other apps */
    {{cookiecutter.app_name}}_InData_t   InData;

    /** \brief Output Data published at the end of cycle */
    {{cookiecutter.app_name}}_OutData_t  OutData;

    /** \brief Housekeeping Telemetry for downlink */
    {{cookiecutter.app_name}}_HkTlm_t  HkTlm;

} {{cookiecutter.app_name}}_AppData_t;

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
/** \brief CFS {{cookiecutter.app_full_name}} Task ({{cookiecutter.app_name}}) application entry point
**
**  \par Description
**       CFS {{cookiecutter.app_full_name}} Task application entry point.  This function
**       performs app initialization, then waits for the cFE ES Startup
**       Sync, then executes the RPR main processing loop.
**
**  \par Assumptions, External Events, and Notes:
**       If there is an unrecoverable failure during initialization the
**       main loop is never executed and the application will exit.
**
*************************************************************************/
void  {{cookiecutter.app_name}}_AppMain(void);

/************************************************************************/
/** \brief Initialize the CFS {{cookiecutter.app_full_name}} ({{cookiecutter.app_name}}) application
**
**  \par Description
**       {{cookiecutter.app_full_name}} application initialization routine. This
**       function performs all the required startup steps to
**       initialize (or restore from CDS) {{cookiecutter.app_name}} data structures and get
**       the application registered with the cFE services so it can
**       begin to receive command messages and send events.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
**  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
**  \retstmt Return codes from #{{cookiecutter.app_name}}_InitEvent               \endcode
**  \retstmt Return codes from #{{cookiecutter.app_name}}_InitPipe                \endcode
**  \retstmt Return codes from #{{cookiecutter.app_name}}_InitData                \endcode
**  \retstmt Return codes from #{{cookiecutter.app_name}}_InitConfigTbl           \endcode
**  \retstmt Return codes from #{{cookiecutter.app_name}}_InitCdsTbl              \endcode
**  \retstmt Return codes from #OS_TaskInstallDeleteHandler \endcode
**  \endreturns
**
*************************************************************************/
int32  {{cookiecutter.app_name}}_InitApp(void);

/************************************************************************/
/** \brief Initialize Event Services and Event tables
**
**  \par Description
**       This function performs the steps required to setup
**       cFE Event Services for use by the {{cookiecutter.app_name}} application.
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
int32  {{cookiecutter.app_name}}_InitEvent(void);

/************************************************************************/
/** \brief Initialize global variables used by {{cookiecutter.app_name}} application
**
**  \par Description
**       This function performs the steps required to initialize
**       the {{cookiecutter.app_name}} application data.
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
int32  {{cookiecutter.app_name}}_InitData(void);

/************************************************************************/
/** \brief Initialize message pipes
**
**  \par Description
**       This function performs the steps required to setup
**       initialize the cFE Software Bus message pipes and subscribe to
**       messages for the {{cookiecutter.app_name}} application.
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
int32  {{cookiecutter.app_name}}_InitPipe(void);

/************************************************************************/
/** \brief Receive and process messages
**
**  \par Description
**       This function receives and processes messages
**       for the {{cookiecutter.app_name}} application
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
int32  {{cookiecutter.app_name}}_RcvMsg(int32 iBlocking);

/************************************************************************/
/** \brief {{cookiecutter.app_full_name}} Task incoming data processing
**
**  \par Description
**       This function processes incoming data subscribed
**       by {{cookiecutter.app_name}} application
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  {{cookiecutter.app_name}}_ProcessNewData(void);

/************************************************************************/
/** \brief {{cookiecutter.app_full_name}} Task incoming command processing
**
**  \par Description
**       This function processes incoming commands subscribed
**       by {{cookiecutter.app_name}} application
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  {{cookiecutter.app_name}}_ProcessNewCmds(void);

/************************************************************************/
/** \brief {{cookiecutter.app_full_name}} Task application commands
**
**  \par Description
**       This function processes command messages
**       specific to the {{cookiecutter.app_name}} application
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   MsgPtr       A #CFE_SB_Msg_t pointer that
**                             references the software bus message
**
*************************************************************************/
void  {{cookiecutter.app_name}}_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr);

/************************************************************************/
/** \brief Sends {{cookiecutter.app_name}} housekeeping message
**
**  \par Description
**       This function sends the housekeeping message
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  {{cookiecutter.app_name}}_ReportHousekeeping(void);

/************************************************************************/
/** \brief Sends {{cookiecutter.app_name}} output data
**
**  \par Description
**       This function publishes the {{cookiecutter.app_name}} application output data.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  {{cookiecutter.app_name}}_SendOutData(void);

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
boolean  {{cookiecutter.app_name}}_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr, uint16 usExpectedLen);

#ifdef __cplusplus
}
#endif 

#endif /* {{cookiecutter.app_name}}_APP_H */

/************************/
/*  End of File Comment */
/************************/
