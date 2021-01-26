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

#ifndef SIM_APP_H
#define SIM_APP_H

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

#include "sim_platform_cfg.h"
#include "sim_mission_cfg.h"
#include "sim_perfids.h"
#include "sim_msgids.h"
#include "sim_msg.h"
#include "sim_events.h"
#include "sim_tbldefs.h"
#include "math/Integrator.hpp"


#include "px4_msgs.h"
/************************************************************************
 ** Local Defines
 *************************************************************************/
//#define SIM_PUBLISH_GPS
//#define SIM_PUBLISH_ACCEL
//#define SIM_PUBLISH_MAG
//#define SIM_PUBLISH_GYRO
//#define SIM_PUBLISH_BARO
//#define SIM_PUBLISH_DISTANCE_SENSOR
//#define SIM_PUBLISH_OPTICAL_FLOW

#define GYROSIM_ACCEL_DEFAULT_RATE	400
#define GYROSIM_GYRO_DEFAULT_RATE	400
//#define CONSTANTS_ONE_G      9.80665f
//#define MG2MS2              (CONSTANTS_ONE_G / 1000.0f)
//#define NEW_SCALE_G_DIGIT   0.732e-3f
//#define NEW_SCALE_GA_DIGIT  0.479e-3f

/************************************************************************
 ** Local Structure Definitions
 *************************************************************************/


/**
 **  \brief SIM Application Class
 */
class SIM
{
public:
    SIM();
    ~SIM();

    /**\brief Scheduling Pipe ID */
    CFE_SB_PipeId_t SchPipeId;

    /** \brief Command Pipe ID */
    CFE_SB_PipeId_t CmdPipeId;

    /* Task-related */

    /** \brief Task Run Status */
    uint32 uiRunStatus;

    /* Config table-related */

    /** \brief Config Table Handle */
    CFE_TBL_Handle_t ConfigTblHdl;

    /** \brief Config Table Pointer */
    SIM_ConfigTbl_t* ConfigTblPtr;

    /** \brief Housekeeping Telemetry for downlink */
    SIM_HkTlm_t HkTlm;

	Integrator _accel_int;
	Integrator _gyro_int;

    /* All messages */
    PX4_VehicleGpsPositionMsg_t     VehicleGps;
    PX4_SensorAccelMsg_t            SensorAccel;
    PX4_SensorMagMsg_t              SensorMag;
    PX4_SensorGyroMsg_t             SensorGyro;
    PX4_SensorBaroMsg_t             SensorBaro;
    PX4_DistanceSensorMsg_t         DistanceSensor;
    PX4_OpticalFlowMsg_t            OpticalFlow;

    int    Socket;
    int    SendPort;
    char   SendAddress[255];

    void SetRates(void);

    /************************************************************************/
    /** \brief Simulator Interface (SIM) application entry point
     **
     **  \par Description
     **       Simulator Interface Task application entry point.  This function
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
    /** \brief Initialize the Simulator Interface (SIM) application
     **
     **  \par Description
     **       Simulator Interface application initialization routine. This
     **       function performs all the required startup steps to
     **       initialize (or restore from CDS) SIM data structures and get
     **       the application registered with the cFE services so it can
     **       begin to receive command messages and send events.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
     **  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
     **  \retstmt Return codes from #SIM_InitEvent               \endcode
     **  \retstmt Return codes from #SIM_InitPipe                \endcode
     **  \retstmt Return codes from #SIM_InitData                \endcode
     **  \retstmt Return codes from #SIM_InitConfigTbl           \endcode
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
     **       cFE Event Services for use by the SIM application.
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
    /** \brief Initialize global variables used by SIM application
     **
     **  \par Description
     **       This function performs the steps required to initialize
     **       the SIM application data.
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
     **       messages for the SIM application.
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
     **       for the SIM application from the SCH pipe.  This function
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
    /** \brief Simulator Interface Task incoming command processing
     **
     **  \par Description
     **       This function processes incoming commands subscribed
     **       by SIM application
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void ProcessCmdPipe(void);

    /************************************************************************/
    /** \brief Simulator Interface Task application commands
     **
     **  \par Description
     **       This function processes command messages
     **       specific to the SIM application
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
    /** \brief Sends SIM housekeeping message
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

private:
    /************************************************************************/
    /** \brief Initialize the SIM configuration tables.
    **
    **  \par Description
    **       This function initializes SIM's configuration tables.  This
    **       includes <TODO>.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
    **  \retstmt Return codes from #CFE_TBL_Register          \endcode
    **  \retstmt Return codes from #CFE_TBL_Load              \endcode
    **  \retstmt Return codes from #SIM_AcquireConfigPointers \endcode
    **  \endreturns
    **
    *************************************************************************/
    int32  InitConfigTbl(void);

    /************************************************************************/
    /** \brief Obtain SIM configuration tables data pointers.
    **
    **  \par Description
    **       This function manages the configuration tables
    **       and obtains a pointer to their data.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
    **  \endreturns
    **
    *************************************************************************/
    int32  AcquireConfigPointers(void);

    int32 ListenerInit(void);

    boolean ChildContinueFlag;
    uint32 ListenerTaskID;

    bool  ChildContinueExec(void);
    void  StopChild(void);
    uint32 MutexID;


public:
    /************************************************************************/
    /** \brief Validate SIM configuration table
    **
    **  \par Description
    **       This function validates SIM's configuration table
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in]   ConfigTblPtr    A pointer to the table to validate.
    **
    **  \returns
    **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
    **  \endreturns
    **
    *************************************************************************/
    static int32  ValidateConfigTbl(void*);

    static void ListenerTask_c(void);
    void ListenerTask(void);
};

#ifdef __cplusplus
}
#endif 

#endif /* SIM_APP_H */

/************************/
/*  End of File Comment */
/************************/
