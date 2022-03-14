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
    
#ifndef SIMLINK_APP_H
#define SIMLINK_APP_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"

#include "simlink_platform_cfg.h"
#include "simlink_mission_cfg.h"
#include "simlink_private_ids.h"
#include "simlink_private_types.h"
#include "simlink_perfids.h"
#include "simlink_msgids.h"
#include "simlink_msg.h"
#include "simlink_events.h"
#include "simlink_config_utils.h"
#include "simlink_cds_utils.h"
#include "simlink.h"
#include "cvt_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Local Defines
*************************************************************************/
#define SIMLINK_TIMEOUT_MSEC             	(1000)

/************************************************************************
** Local Structure Definitions
*************************************************************************/
/**
**  \brief SIMLINK Operational Data Structure
*/
typedef struct
{
    /** \brief CFE Event Table */
    CFE_EVS_BinFilter_t  EventTbl[SIMLINK_EVT_CNT];

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
    SIMLINK_ConfigTblEntry_t*  ConfigTblPtr;

    /* Critical Data Storage (CDS) table-related */

    /** \brief CDS Table Handle */
    CFE_ES_CDSHandle_t  CdsTblHdl;

    /** \brief CDS Table data */
    SIMLINK_CdsTbl_t  CdsTbl;

    /* Inputs/Outputs */

    /** \brief Input Data from I/O or other apps */
    SIMLINK_InData_t   InData;

    /** \brief Output Data published at the end of cycle */
    SIMLINK_OutData_t  OutData;

    /** \brief Housekeeping Telemetry for downlink */
    SIMLINK_HkTlm_t    HkTlm;

    /* TODO */
    uint32             ListenerTaskID;
    osalbool           ChildContinueFlag;
    int                Socket;

    CVT_ContainerID_t   GpsContainer[SIMLINK_GPS_DEVICE_COUNT];
    CVT_ContainerID_t   GyroContainer[SIMLINK_GYRO_DEVICE_COUNT];
    CVT_ContainerID_t   AccelContainer[SIMLINK_ACCEL_DEVICE_COUNT];
    CVT_ContainerID_t   MagContainer[SIMLINK_MAG_DEVICE_COUNT];
    CVT_ContainerID_t   BaroContainer[SIMLINK_BARO_DEVICE_COUNT];

    SIMLINK_GPS_Msg_t   GpsMsg[SIMLINK_GPS_DEVICE_COUNT];
    SIMLINK_Gyro_Msg_t  GyroMsg[SIMLINK_GYRO_DEVICE_COUNT];
    SIMLINK_Accel_Msg_t AccelMsg[SIMLINK_ACCEL_DEVICE_COUNT];
    SIMLINK_Mag_Msg_t   MagMsg[SIMLINK_MAG_DEVICE_COUNT];
    SIMLINK_Baro_Msg_t  BaroMsg[SIMLINK_BARO_DEVICE_COUNT];

} SIMLINK_AppData_t;

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
/** \brief CFS UBlox M8N Simlink Task (SIMLINK) application entry point
**
**  \par Description
**       CFS UBlox M8N Simlink Task application entry point.  This function
**       performs app initialization, then waits for the cFE ES Startup
**       Sync, then executes the RPR main processing loop.
**
**  \par Assumptions, External Events, and Notes:
**       If there is an unrecoverable failure during initialization the
**       main loop is never executed and the application will exit.
**
*************************************************************************/
void  SIMLINK_AppMain(void);

/************************************************************************/
/** \brief Initialize the CFS UBlox M8N Simlink (SIMLINK) application
**
**  \par Description
**       UBlox M8N Simlink application initialization routine. This
**       function performs all the required startup steps to
**       initialize (or restore from CDS) SIMLINK data structures and get
**       the application registered with the cFE services so it can
**       begin to receive command messages and send events.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
**  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
**  \retstmt Return codes from #SIMLINK_InitEvent               \endcode
**  \retstmt Return codes from #SIMLINK_InitPipe                \endcode
**  \retstmt Return codes from #SIMLINK_InitData                \endcode
**  \retstmt Return codes from #SIMLINK_InitConfigTbl           \endcode
**  \retstmt Return codes from #SIMLINK_InitCdsTbl              \endcode
**  \retstmt Return codes from #OS_TaskInstallDeleteHandler \endcode
**  \endreturns
**
*************************************************************************/
int32  SIMLINK_InitApp(void);

/************************************************************************/
/** \brief Initialize Event Services and Event tables
**
**  \par Description
**       This function performs the steps required to setup
**       cFE Event Services for use by the SIMLINK application.
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
int32  SIMLINK_InitEvent(void);

/************************************************************************/
/** \brief Initialize global variables used by SIMLINK application
**
**  \par Description
**       This function performs the steps required to initialize
**       the SIMLINK application data.
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
int32  SIMLINK_InitData(void);

/************************************************************************/
/** \brief Initialize message pipes
**
**  \par Description
**       This function performs the steps required to setup
**       initialize the cFE Software Bus message pipes and subscribe to
**       messages for the SIMLINK application.
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
int32  SIMLINK_InitPipe(void);

/************************************************************************/
/** \brief Receive and process messages
**
**  \par Description
**       This function receives and processes messages
**       for the SIMLINK application
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
int32  SIMLINK_RcvMsg(int32 iBlocking);

/************************************************************************/
/** \brief UBlox M8N Simlink Task incoming data processing
**
**  \par Description
**       This function processes incoming data subscribed
**       by SIMLINK application
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  SIMLINK_ProcessNewData(void);

/************************************************************************/
/** \brief UBlox M8N Simlink Task incoming command processing
**
**  \par Description
**       This function processes incoming commands subscribed
**       by SIMLINK application
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  SIMLINK_ProcessNewCmds(void);

/************************************************************************/
/** \brief UBlox M8N Simlink Task application commands
**
**  \par Description
**       This function processes command messages
**       specific to the SIMLINK application
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   MsgPtr       A #CFE_SB_Msg_t pointer that
**                             references the software bus message
**
*************************************************************************/
void  SIMLINK_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr);

/************************************************************************/
/** \brief Sends SIMLINK housekeeping message
**
**  \par Description
**       This function sends the housekeeping message
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  SIMLINK_ReportHousekeeping(void);

/************************************************************************/
/** \brief Sends SIMLINK output data
**
**  \par Description
**       This function publishes the SIMLINK application output data.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  SIMLINK_SendOutData(void);

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
boolean  SIMLINK_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr, uint16 usExpectedLen);

#ifdef __cplusplus
}
#endif 

#endif /* SIMLINK_APP_H */

/************************/
/*  End of File Comment */
/************************/
