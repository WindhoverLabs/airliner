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
    
#ifndef HES_APP_H
#define HES_APP_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"

#include "hes_platform_cfg.h"
#include "hes_mission_cfg.h"
#include "hes_perfids.h"
#include "hes_msgids.h"
#include "hes_msg.h"
#include "hes_events.h"
#include "hes_config_utils.h"
#include "hes_tbldefs.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Local Defines
*************************************************************************/
#define HES_VEL_FILTER_COEFF            (0.01)
#define HES_AIRSPEED_FILTER_COEFF       (0.05)//(0.01)
#define HES_GROUNDSPEED_FILTER_COEFF    (0.01)
#define HES_EULER_FILTER_COEFF          (0.25)//(0.01)

#define HES_MPS_TO_KNOTS(mps)           ((1.943844)*(mps))
#define HES_MPS_TO_KPH(mps)             ((3.6)*(mps))
#define HES_MPS_TO_MPH(mps)             ((2.236936)*(mps))

#define HES_METERS_TO_FEET(meters)      ((3.28084)*(meters))
#define HES_FEET_TO_YARDS(feet)         ((feet)/3)

/************************************************************************
** Local Structure Definitions
*************************************************************************/
typedef struct
{
    /** \brief Airspeed */
    PX4_AirspeedMsg_t                     Airspeed;

    /** \brief Battery status */
    PX4_BatteryStatusMsg_t                BatteryStatus;

    /** \brief Vehicle attitude */
    PX4_VehicleAttitudeMsg_t              VAtt;

    /** \brief Vehicle global position */
    PX4_VehicleGlobalPositionMsg_t        VGlobalPosition;

    /** \brief Vehicle land detected */
    PX4_VehicleLandDetectedMsg_t          VLandDetected;

    /** \brief Vehicle Status */
    PX4_VehicleStatusMsg_t                VehicleStatus;

    /** \brief Setpoint Triplets */
    PX4_PositionSetpointTripletMsg_t      PositionSpTriplet;

    float filtered_vel_n;
    float filtered_vel_e;
    float filtered_vel_d;
    float filtered_airspeed;
    float filtered_groundspeed;
    // float filtered_speed;
    float filtered_euler[3];

    int32 filtered_euler_phase[3];
    
} HES_CurrentValueTable_t;

/**
**  \brief HES Operational Data Structure
*/
typedef struct
{
    /** \brief CFE Event Table */
    CFE_EVS_BinFilter_t  EventTbl[HES_EVT_CNT];

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
    HES_ConfigTblEntry_t*  ConfigTblPtr;

    /* Inputs/Outputs */
    /** \brief Current value table */
    HES_CurrentValueTable_t CVT;

    /** \brief Housekeeping Telemetry for downlink */
    HES_HkTlm_t  HkTlm;

} HES_AppData_t;

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
/** \brief CFS Hawk Eye Server Task (HES) application entry point
**
**  \par Description
**       CFS Hawk Eye Server Task application entry point.  This function
**       performs app initialization, then waits for the cFE ES Startup
**       Sync, then executes the RPR main processing loop.
**
**  \par Assumptions, External Events, and Notes:
**       If there is an unrecoverable failure during initialization the
**       main loop is never executed and the application will exit.
**
*************************************************************************/
void  HES_AppMain(void);

/************************************************************************/
/** \brief Initialize the CFS Hawk Eye Server (HES) application
**
**  \par Description
**       Hawk Eye Server application initialization routine. This
**       function performs all the required startup steps to
**       initialize (or restore from CDS) HES data structures and get
**       the application registered with the cFE services so it can
**       begin to receive command messages and send events.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
**  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
**  \retstmt Return codes from #HES_InitEvent               \endcode
**  \retstmt Return codes from #HES_InitPipe                \endcode
**  \retstmt Return codes from #HES_InitData                \endcode
**  \retstmt Return codes from #HES_InitConfigTbl           \endcode
**  \retstmt Return codes from #HES_InitCdsTbl              \endcode
**  \retstmt Return codes from #OS_TaskInstallDeleteHandler \endcode
**  \endreturns
**
*************************************************************************/
int32  HES_InitApp(void);

/************************************************************************/
/** \brief Initialize Event Services and Event tables
**
**  \par Description
**       This function performs the steps required to setup
**       cFE Event Services for use by the HES application.
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
int32  HES_InitEvent(void);

/************************************************************************/
/** \brief Initialize global variables used by HES application
**
**  \par Description
**       This function performs the steps required to initialize
**       the HES application data.
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
int32  HES_InitData(void);

/************************************************************************/
/** \brief Initialize message pipes
**
**  \par Description
**       This function performs the steps required to setup
**       initialize the cFE Software Bus message pipes and subscribe to
**       messages for the HES application.
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
int32  HES_InitPipe(void);

/************************************************************************/
/** \brief Receive and process messages
**
**  \par Description
**       This function receives and processes messages
**       for the HES application
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
int32  HES_RcvMsg(int32 iBlocking);

/************************************************************************/
/** \brief Hawk Eye Server Task incoming data processing
**
**  \par Description
**       This function processes incoming data subscribed
**       by HES application
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  HES_ProcessNewData(void);

/************************************************************************/
/** \brief Hawk Eye Server Task incoming command processing
**
**  \par Description
**       This function processes incoming commands subscribed
**       by HES application
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  HES_ProcessNewCmds(void);

/************************************************************************/
/** \brief Hawk Eye Server Task application commands
**
**  \par Description
**       This function processes command messages
**       specific to the HES application
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   MsgPtr       A #CFE_SB_Msg_t pointer that
**                             references the software bus message
**
*************************************************************************/
void  HES_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr);

/************************************************************************/
/** \brief Sends HES housekeeping message
**
**  \par Description
**       This function sends the housekeeping message
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  HES_ReportHousekeeping(void);

/************************************************************************/
/** \brief Sends HES output data
**
**  \par Description
**       This function publishes the HES application output data.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  HES_SendOutData(void);

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
boolean  HES_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr, uint16 usExpectedLen);

#ifdef __cplusplus
}
#endif 

#endif /* HES_APP_H */

/************************/
/*  End of File Comment */
/************************/
