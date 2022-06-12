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
    
#ifndef FPC_APP_H
#define FPC_APP_H

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

#include "fpc_platform_cfg.h"
#include "fpc_mission_cfg.h"
#include "fpc_private_ids.h"
#include "fpc_private_types.h"
#include "fpc_perfids.h"
#include "fpc_msgids.h"
#include "fpc_msg.h"
#include "fpc_events.h"
#include "fpc_tbldefs.h"
#include "fpc_config_utils.h"
#include "fpc_landing_slope.h"

#include "px4_msgs.h"

#include "math/Matrix3F3.hpp"
#include "math/Quaternion.hpp"
#include "math/Euler.hpp"
#include "math/Dcm.hpp"
#include "math/Expo.hpp"
#include "math/Functions.hpp"


/************************************************************************
** Local Defines
*************************************************************************/
#define FPC_TIMEOUT_MSEC             	(1000)


/************************************************************************
** Local Structure Definitions
*************************************************************************/
/**
**  \brief FPC Operational Data Structure
*/

class FPC
{
public:
    FPC();
   ~FPC();

    /** \brief CFE Event Table */
    CFE_EVS_BinFilter_t  EventTbl[FPC_EVT_CNT];

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
    FPC_ConfigTbl_t*  ConfigTblPtr;

    /* Critical Data Storage (CDS) table-related */

    /** \brief CDS Table Handle */
    CFE_ES_CDSHandle_t  CdsTblHdl;

    /** \brief CDS Table data */
    FPC_CdsTbl_t  CdsTbl;

    /* Inputs/Outputs */

    /** \brief Input Data from I/O or other apps */
    FPC_InData_t   InData;

    /** \brief Output Data published at the end of cycle */
    FPC_OutData_t  OutData;

    /** \brief Housekeeping Telemetry for downlink */
    FPC_HkTlm_t  HkTlm;

    /** \brief Output Data published at the end of cycle */

    PX4_Position_Control_Status_t         m_PositionControlStatusMsg;
    /* Input Messages */
    PX4_ControlStateMsg_t                 m_ControlStateMsg;
    PX4_ManualControlSetpointMsg_t        m_ManualControlSetpointMsg;
    PX4_HomePositionMsg_t                 m_HomePositionMsg;
    PX4_VehicleControlModeMsg_t           m_VehicleControlModeMsg;
    PX4_PositionSetpointTripletMsg_t      m_PositionSetpointTripletMsg;
    PX4_VehicleStatusMsg_t                m_VehicleStatusMsg;
    PX4_VehicleLandDetectedMsg_t          m_VehicleLandDetectedMsg;
    PX4_VehicleLocalPositionMsg_t         m_VehicleLocalPositionMsg;

    Landingslope                          m_LandingSlope;
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
    /** \brief CFS Fixedwing Position Control Task (FPC) application entry point
    **
    **  \par Description
    **       CFS Fixedwing Position Control Task application entry point.  This function
    **       performs app initialization, then waits for the cFE ES Startup
    **       Sync, then executes the RPR main processing loop.
    **
    **  \par Assumptions, External Events, and Notes:
    **       If there is an unrecoverable failure during initialization the
    **       main loop is never executed and the application will exit.
    **
    *************************************************************************/
    void  AppMain(void);

    /************************************************************************/
    /** \brief Initialize the CFS Fixedwing Position Control (FPC) application
    **
    **  \par Description
    **       Fixedwing Position Control application initialization routine. This
    **       function performs all the required startup steps to
    **       initialize (or restore from CDS) FPC data structures and get
    **       the application registered with the cFE services so it can
    **       begin to receive command messages and send events.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
    **  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
    **  \retstmt Return codes from #FPC_InitEvent               \endcode
    **  \retstmt Return codes from #FPC_InitPipe                \endcode
    **  \retstmt Return codes from #FPC_InitData                \endcode
    **  \retstmt Return codes from #FPC_InitConfigTbl           \endcode
    **  \retstmt Return codes from #FPC_InitCdsTbl              \endcode
    **  \retstmt Return codes from #OS_TaskInstallDeleteHandler \endcode
    **  \endreturns
    **
    *************************************************************************/
    int32  InitApp(void);

    /************************************************************************/
    /** \brief Initialize Event Services and Event tables
    **
    **  \par Description
    **       This function performs the steps required to setup
    **       cFE Event Services for use by the FPC application.
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
    int32  InitEvent(void);

    /************************************************************************/
    /** \brief Initialize global variables used by FPC application
    **
    **  \par Description
    **       This function performs the steps required to initialize
    **       the FPC application data.
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
    int32  InitData(void);

    /************************************************************************/
    /** \brief Initialize message pipes
    **
    **  \par Description
    **       This function performs the steps required to setup
    **       initialize the cFE Software Bus message pipes and subscribe to
    **       messages for the FPC application.
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
    int32  InitPipe(void);

    /************************************************************************/
    /** \brief Receive and process messages
    **
    **  \par Description
    **       This function receives and processes messages
    **       for the FPC application
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
    int32  RcvMsg(int32 iBlocking);

    /************************************************************************/
    /** \brief Fixedwing Position Control Task incoming data processing
    **
    **  \par Description
    **       This function processes incoming data subscribed
    **       by FPC application
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void  ProcessNewData(void);

    /************************************************************************/
    /** \brief Fixedwing Position Control Task incoming command processing
    **
    **  \par Description
    **       This function processes incoming commands subscribed
    **       by FPC application
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void  ProcessNewCmds(void);

    /************************************************************************/
    /** \brief Fixedwing Position Control Task application commands
    **
    **  \par Description
    **       This function processes command messages
    **       specific to the FPC application
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in]   MsgPtr       A #CFE_SB_Msg_t pointer that
    **                             references the software bus message
    **
    *************************************************************************/
    void  ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr);

    /************************************************************************/
    /** \brief Sends FPC housekeeping message
    **
    **  \par Description
    **       This function sends the housekeeping message
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void  ReportHousekeeping(void);

    /************************************************************************/
    /** \brief Sends FPC output data
    **
    **  \par Description
    **       This function publishes the FPC application output data.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void  SendOutData(void);

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
    boolean  VerifyCmdLength(CFE_SB_Msg_t* MsgPtr, uint16 usExpectedLen);

    int32 InitConfigTbl(void);
    static int32 ValidateConfigTbl(void* ConfigTblPtr);
    void ProcessNewConfigTbl(void);
    int32 AcquireConfigPointers(void);


    /************************************************************************/
    /** \brief Init FPC CDS tables
    **
    **  \par Description
    **       This function initializes FPC's CDS tables
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
    **  \retstmt Return codes from #CFE_ES_RegisterCDS       \endcode
    **  \retstmt Return codes from #CFE_ES_CopyToCDS         \endcode
    **  \endreturns
    **
    *************************************************************************/
    int32  InitCdsTbl(void);

    /************************************************************************/
    /** \brief Update FPC CDS tables
    **
    **  \par Description
    **       This function updates FPC's CDS tables
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void   UpdateCdsTbl(void);

    /************************************************************************/
    /** \brief Save FPC CDS tables
    **
    **  \par Description
    **       This function saves FPC's CDS tables
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void   SaveCdsTbl(void);

    void UpdateParamsFromTable(void);

    void SendPositionControlStatusMsg(void);


};

#ifdef __cplusplus
}
#endif 

#endif /* FPC_APP_H */

/************************/
/*  End of File Comment */
/************************/
