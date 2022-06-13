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
    
#ifndef ASPD4525_APP_H
#define ASPD4525_APP_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"

#include "aspd4525_platform_cfg.h"
#include "aspd4525_mission_cfg.h"
#include "aspd4525_private_ids.h"
#include "aspd4525_private_types.h"
#include "aspd4525_perfids.h"
#include "aspd4525_msgids.h"
#include "aspd4525_msg.h"
#include "aspd4525_events.h"
#include "aspd4525_config_utils.h"
#include "aspd4525_cds_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Local Defines
*************************************************************************/
#define ASPD4525_TIMEOUT_MSEC             	(1000)

/************************************************************************
** Local Structure Definitions
*************************************************************************/
/**
**  \brief ASPD4525 Operational Data Structure
*/
typedef struct
{
    /** \brief CFE Event Table */
    CFE_EVS_BinFilter_t  EventTbl[ASPD4525_EVT_CNT];

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
    ASPD4525_ConfigTblEntry_t*  ConfigTblPtr;

    /* Critical Data Storage (CDS) table-related */

    /** \brief CDS Table Handle */
    CFE_ES_CDSHandle_t  CdsTblHdl;

    /** \brief CDS Table data */
    ASPD4525_CdsTbl_t  CdsTbl;

    /* Inputs/Outputs */

    /** \brief Input Data from I/O or other apps */
    ASPD4525_InData_t   InData;

    /** \brief Output Data published at the end of cycle */
    ASPD4525_OutData_t  OutData;

    /** \brief Housekeeping Telemetry for downlink */
    ASPD4525_HkTlm_t  HkTlm;

    int16 sPressureDiffCount;
    int16 sIemperatureCount;
    uint8 ucStatus;  /* 0 - ok, 1 - Busy, 2 - Stale, (3 or -1) - Error*/

    float fPressureDiff; /* in Pascals */
    float fTemperature;  /* in Celsius */

} ASPD4525_AppData_t;

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
/** \brief CFS ASPD-4525 Driver App Task (ASPD4525) application entry point
**
**  \par Description
**       CFS ASPD-4525 Driver App Task application entry point.  This function
**       performs app initialization, then waits for the cFE ES Startup
**       Sync, then executes the RPR main processing loop.
**
**  \par Assumptions, External Events, and Notes:
**       If there is an unrecoverable failure during initialization the
**       main loop is never executed and the application will exit.
**
*************************************************************************/
void  ASPD4525_AppMain(void);

/************************************************************************/
/** \brief Initialize the CFS ASPD-4525 Driver App (ASPD4525) application
**
**  \par Description
**       ASPD-4525 Driver App application initialization routine. This
**       function performs all the required startup steps to
**       initialize (or restore from CDS) ASPD4525 data structures and get
**       the application registered with the cFE services so it can
**       begin to receive command messages and send events.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
**  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
**  \retstmt Return codes from #ASPD4525_InitEvent               \endcode
**  \retstmt Return codes from #ASPD4525_InitPipe                \endcode
**  \retstmt Return codes from #ASPD4525_InitData                \endcode
**  \retstmt Return codes from #ASPD4525_InitConfigTbl           \endcode
**  \retstmt Return codes from #ASPD4525_InitCdsTbl              \endcode
**  \retstmt Return codes from #OS_TaskInstallDeleteHandler \endcode
**  \endreturns
**
*************************************************************************/
int32  ASPD4525_InitApp(void);

/************************************************************************/
/** \brief Initialize Event Services and Event tables
**
**  \par Description
**       This function performs the steps required to setup
**       cFE Event Services for use by the ASPD4525 application.
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
int32  ASPD4525_InitEvent(void);

/************************************************************************/
/** \brief Initialize global variables used by ASPD4525 application
**
**  \par Description
**       This function performs the steps required to initialize
**       the ASPD4525 application data.
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
int32  ASPD4525_InitData(void);

/************************************************************************/
/** \brief Initialize message pipes
**
**  \par Description
**       This function performs the steps required to setup
**       initialize the cFE Software Bus message pipes and subscribe to
**       messages for the ASPD4525 application.
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
int32  ASPD4525_InitPipe(void);

/************************************************************************/
/** \brief Receive and process messages
**
**  \par Description
**       This function receives and processes messages
**       for the ASPD4525 application
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
int32  ASPD4525_RcvMsg(int32 iBlocking);

/************************************************************************/
/** \brief ASPD-4525 Driver App Task incoming data processing
**
**  \par Description
**       This function processes incoming data subscribed
**       by ASPD4525 application
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  ASPD4525_ProcessNewData(void);

/************************************************************************/
/** \brief ASPD-4525 Driver App Task incoming command processing
**
**  \par Description
**       This function processes incoming commands subscribed
**       by ASPD4525 application
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  ASPD4525_ProcessNewCmds(void);

/************************************************************************/
/** \brief ASPD-4525 Driver App Task application commands
**
**  \par Description
**       This function processes command messages
**       specific to the ASPD4525 application
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   MsgPtr       A #CFE_SB_Msg_t pointer that
**                             references the software bus message
**
*************************************************************************/
void  ASPD4525_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr);

/************************************************************************/
/** \brief Sends ASPD4525 housekeeping message
**
**  \par Description
**       This function sends the housekeeping message
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  ASPD4525_ReportHousekeeping(void);

/************************************************************************/
/** \brief Sends ASPD4525 output data
**
**  \par Description
**       This function publishes the ASPD4525 application output data.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  ASPD4525_SendOutData(void);

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
boolean  ASPD4525_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr, uint16 usExpectedLen);


void ASPD4525_ReadDevice(void);

#ifdef __cplusplus
}
#endif 

#endif /* ASPD4525_APP_H */

/************************/
/*  End of File Comment */
/************************/
