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

#ifndef PE_APP_H
#define PE_APP_H

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
#include "pe_platform_cfg.h"
#include "pe_mission_cfg.h"
#include "pe_perfids.h"
#include "pe_msgids.h"
#include "pe_msg.h"
#include "pe_events.h"
#include "pe_tbldefs.h"
#include "px4_msgs.h"
#include "px4lib.h"
#include "math/BlockDelay.hpp"
#include "math/Vector1F.hpp"
#include "math/Vector2F.hpp"
#include "math/Vector3F.hpp"
#include "math/Vector6F.hpp"
#include "math/Vector10F.hpp"
#include "math/Matrix1F1.hpp"
#include "math/Matrix1F2.hpp"
#include "math/Matrix1F3.hpp"
#include "math/Matrix1F6.hpp"
#include "math/Matrix1F10.hpp"
#include "math/Matrix2F2.hpp"
#include "math/Matrix2F10.hpp"
#include "math/Matrix3F3.hpp"
#include "math/Matrix6F10.hpp"
#include "math/Matrix6F6.hpp"
#include "math/Matrix10F2.hpp"
#include "math/Matrix10F3.hpp"
#include "math/Matrix10F10.hpp"
#include "math/LowPass.hpp"
#include "math/HighPass.hpp"
#include "math/LowPassVector10F.hpp"
#include "math/Stats1F.hpp"
#include "math/Stats2F.hpp"
#include "math/Stats6F.hpp"
#include "math/Quaternion.hpp"
#include "math/Euler.hpp"
#include "math/Dcm.hpp"
#include "geo/geo.h"
#include <poll.h>
#include <math.h>


/************************************************************************
 ** Local Defines
 *************************************************************************/

#define PE_BETA_TABLE_SIZE    (7)
/** \brief PE params mutex name. */
#define PE_PARAMS_MUTEX                "PE_PARAMS_MUTEX"

/** \brief Pipe depth for the data pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define PE_DATA_PIPE_DEPTH            (9)

/** \brief Pipe name for the Scheduler pipe
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define PE_DATA_PIPE_NAME             ("PE_DATA_PIPE")

/************************************************************************
 ** Local Structure Definitions
 *************************************************************************/
enum {
    X_x  = 0,
    X_y  = 1,
    X_z  = 2,
    X_vx = 3,
    X_vy = 4,
    X_vz = 5,
    X_bx = 6,
    X_by = 7,
    X_bz = 8,
    X_tz = 9,
    n_x  = 10
};

enum {
    U_ax = 0,
    U_ay = 1,
    U_az = 2,
    n_u  = 3
};

enum {
    Y_baro_z = 0,
    n_y_baro = 1
};

enum {
    Y_dist_z = 0,
    n_y_dist = 1
};

enum {
    Y_gps_x  = 0,
    Y_gps_y  = 1,
    Y_gps_z  = 2,
    Y_gps_vx = 3,
    Y_gps_vy = 4,
    Y_gps_vz = 5,
    n_y_gps  = 6
};

enum {
    Y_land_vx  = 0,
    Y_land_vy  = 1,
    Y_land_agl = 2,
    n_y_land   = 3
};

enum {
    Y_flow_vx = 0,
    Y_flow_vy = 1,
    n_y_flow = 2
};

/* Enums for other sensors would go here */


/**
 **  \brief PE Application Class
 */
class PE
{
public:
    PE();
    ~PE();

    /** \brief Constants */
    const float  DELAY_MAX = 0.5f;
    const float  HIST_STEP = 0.05f;
    const float  BIAS_MAX = 1e-1f;
    const size_t HIST_LEN = 10;
    const size_t N_DIST_SUBS = 4;
    const uint32 EST_STDDEV_XY_VALID = 2.0f;
    const uint32 EST_STDDEV_Z_VALID = 2.0f;
    const uint32 EST_STDDEV_TZ_VALID = 2.0f;
    const float  P_MAX = 1.0e6;
    const float  LAND_RATE = 10.0f;
    const float  DIST_RATE = 125.0f;
    const float  LOW_PASS_CUTOFF = 5.0f;
    const float  EPH_THRESH = 3.0f;
    const float  EPV_THRESH = 3.0f;
    const float  BETA_TABLE[PE_BETA_TABLE_SIZE] = {0.0f,
                                                   8.82050518214f, 
                                                   12.094592431f, 
                                                   13.9876612368f, 
                                                   16.0875642296f, 
                                                   17.8797700658f, 
                                                   19.6465647819f};

    /**\brief Scheduling Pipe ID */
    CFE_SB_PipeId_t SchPipeId;

    /** \brief Command Pipe ID */
    CFE_SB_PipeId_t CmdPipeId;

    /** \brief Data Pipe ID */
    CFE_SB_PipeId_t DataPipeId;

    /* Task-related */

    /** \brief Task Run Status */
    uint32 uiRunStatus;

    /* Config table-related */

    /** \brief Config Table Handle */
    CFE_TBL_Handle_t ConfigTblHdl;

    /** \brief Config Table Pointer */
    PE_ConfigTbl_t* ConfigTblPtr;

    uint32 ConfigMutex;

    /** \brief Ingest Data */
    PX4_VehicleGpsPositionMsg_t m_VehicleGpsPositionMsg;
    PX4_VehicleStatusMsg_t m_VehicleStatusMsg;
    PX4_VehicleLandDetectedMsg_t m_VehicleLandDetectedMsg;
    PX4_ActuatorArmedMsg_t m_ActuatorArmedMsg;
    PX4_VehicleAttitudeMsg_t m_VehicleAttitudeMsg;
    PX4_SensorCombinedMsg_t m_SensorCombinedMsg;
    PX4_VehicleAttitudeSetpointMsg_t m_VehicleAttitudeSetpointMsg;
    PX4_DistanceSensorMsg_t m_DistanceSensor;
    PX4_OpticalFlowMsg_t m_OpticalFlowMsg;

    /** \brief Output Data published at the end of cycle */
    PX4_VehicleLocalPositionMsg_t m_VehicleLocalPositionMsg;
    PX4_EstimatorStatusMsg_t m_EstimatorStatusMsg;
    PX4_VehicleGlobalPositionMsg_t m_VehicleGlobalPositionMsg;

    /* Sensor stats */
    Stats1F m_BaroStats;
    Stats1F m_DistStats;
    Stats1F m_FlowQStats;
    Stats6F m_GpsStats;
    uint16 m_LandCount;

    /* Validity */
    osalbool m_XyEstValid;
    osalbool m_ZEstValid;
    osalbool m_TzEstValid;

    /* Map */
    struct map_projection_reference_s m_MapRef;

    /* Low pass filter */
    LowPassVector10F m_XLowPass;
    LowPass m_AglLowPass;

    /* High pass filter */
    HighPass m_FlowGyroXHighPass;
    HighPass m_FlowGyroYHighPass;

    /* Delay blocks */
    delay::BlockDelay10FLEN10   m_XDelay;
    delay::BlockDelayUINT64LEN10 m_TDelay;

    /* Timestamps */
    uint64 m_Timestamp;
    uint64 m_Timestamp_Hist;
    uint64 m_TimestampLastBaro;
    uint64 m_TimeLastBaro;
    uint64 m_TimeLastGps;
    uint64 m_TimeLastDist;
    uint64 m_TimeLastLand;
    uint64 m_TimeLastFlow;

    /* Timeouts */
    osalbool   m_BaroTimeout;
    osalbool   m_GpsTimeout;
    osalbool   m_LandTimeout;
    osalbool   m_DistTimeout;
    osalbool   m_FlowTimeout;

    /* Faults */
    osalbool   m_BaroFault;
    osalbool   m_GpsFault;
    osalbool   m_LandFault;
    osalbool   m_DistFault;
    osalbool   m_FlowFault;

    /* Fuse Flags */
    osalbool   m_BaroFuse;
    osalbool   m_GpsFuse;
    osalbool   m_LandFuse;
    osalbool   m_DistFuse;
    osalbool   m_FlowFuse;

    /* Reference altitudes */
    float m_AltOrigin;
    float m_BaroAltOrigin;
    float m_GpsAltOrigin;
    float m_DistAltOrigin;

    /* Status */
    osalbool m_ReceivedGps;
    osalbool m_LastArmedState;
    osalbool m_EstimatorLocalInitialized;
    osalbool m_EstimatorGlobalInitialized;
    osalbool m_BaroInitialized;
    osalbool m_GpsInitialized;
    osalbool m_LandInitialized;
    osalbool m_DistInitialized;
    osalbool m_FlowInitialized;
    osalbool m_AltOriginInitialized;
    osalbool m_ParamsUpdated;

    /* State space */
    math::Vector10F     m_StateVec; // state vector
    math::Vector3F      m_InputVec; // input vector
    math::Matrix10F10   m_StateCov; // state covariance matrix

    math::Matrix3F3     m_RotationMat;
    math::Vector3F      m_Euler;

    math::Matrix10F10   m_DynamicsMat; // dynamics matrix
    math::Matrix10F3    m_InputMat; // input matrix
    math::Matrix3F3     m_InputCov; // input covariance
    math::Matrix10F10   m_NoiseCov; // process noise covariance

    /* Sensor specific data structs */
    struct Baro
    {
        math::Vector1F y;
        math::Matrix1F10 C;
        math::Matrix1F1 R;
        math::Matrix1F1 S_I;
        math::Vector1F r;
        float beta;
        math::Matrix10F1 K;
        math::Matrix10F1 temp;
        math::Vector10F dx;
    } m_Baro;

    struct GPS
    {
        math::Vector6F y_global;
        double  lat;
        double  lon;
        float  alt;
        float px;
        float py;
        float pz;
        math::Vector6F y;
        math::Matrix6F10 C;
        math::Matrix6F6 R;
        float var_xy;
        float var_z;
        float var_vxy;
        float var_vz;
        float gps_s_stddev;
        uint8 i_hist;
        math::Vector10F x0;
        math::Vector6F r;
        math::Matrix6F6 S_I;
        math::Matrix1F6 rTranspose;
        float beta;
        float beta_thresh;
        math::Matrix10F6 K;
        math::Vector10F dx;
        
    } m_GPS;

    struct Land
    {
        math::Vector3F y;
        math::Matrix3F10 C;
        math::Matrix3F3 R;
        math::Matrix3F3 S_I;
        math::Vector3F r;
        float beta;
        float beta_thresh;
        math::Matrix10F3 K;
        math::Vector10F dx;
        
    } m_Land;

    struct Dist
    {
        math::Vector1F y;
        math::Matrix1F10 C;
        math::Matrix1F1 R;
        math::Matrix1F1 S_I;
        math::Vector1F r;
        float beta;
        math::Matrix10F1 K;
        math::Matrix10F1 temp;
        math::Vector10F dx;
    } m_Dist;
    
    struct Flow
    {
        math::Vector2F y;
        math::Matrix2F10 C;
        math::Matrix2F2 R;
        math::Matrix2F2 S_I;
        math::Vector2F r;
        float beta;
        math::Matrix10F2 K;
        math::Matrix10F2 temp;
        math::Vector10F dx;
    } m_Flow;

    struct Predict
    {
        math::Quaternion q;
        math::Vector3F a;
        math::Vector10F k1;
        math::Vector10F k2;
        math::Vector10F k3;
        math::Vector10F k4;
        math::Vector10F dx;
        float bx;
        float by;
        float bz;
        math::Matrix10F10 dP;
    } m_Predict;

    /** \brief Housekeeping Telemetry for downlink */
    PE_HkTlm_t HkTlm;

    /************************************************************************/
    /** \brief Position Estimator (PE) application entry point
     **
     **  \par Description
     **       Position Estimator Task application entry point.  This function
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
    /** \brief Initialize the Position Estimator (PE) application
     **
     **  \par Description
     **       Position Estimator application initialization routine. This
     **       function performs all the required startup steps to
     **       initialize (or restore from CDS) PE data structures and get
     **       the application registered with the cFE services so it can
     **       begin to receive command messages and send events.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
     **  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
     **  \retstmt Return codes from #PE_InitEvent               \endcode
     **  \retstmt Return codes from #PE_InitPipe                \endcode
     **  \retstmt Return codes from #PE_InitData                \endcode
     **  \retstmt Return codes from #PE_InitConfigTbl           \endcode
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
     **       cFE Event Services for use by the PE application.
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
    /** \brief Initialize global variables used by PE application
     **
     **  \par Description
     **       This function performs the steps required to initialize
     **       the PE application data.
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
     **       messages for the PE application.
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
     **       for the PE application from the SCH pipe.  This function
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
    /** \brief Position Estimator Task incoming command processing
     **
     **  \par Description
     **       This function processes incoming commands subscribed
     **       by PE application
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void ProcessCmdPipe(void);

    /************************************************************************/
    /** \brief Position Estimator Task incoming data processing
     **
     **  \par Description
     **       This function processes incoming data messages subscribed
     **       by PE application
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    osalbool ProcessDataPipe(void);

    /************************************************************************/
    /** \brief Position Estimator Task application commands
     **
     **  \par Description
     **       This function processes command messages
     **       specific to the PE application
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
    /** \brief Sends PE housekeeping message
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
    /** \brief Sends the VehicleLocalPositionMsg message.
     **
     **  \par Description
     **       This function publishes the VehicleLocalPositionMsg message
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendVehicleLocalPositionMsg(void);

    /************************************************************************/
    /** \brief Sends the VehicleGlobalPositionMsg message.
     **
     **  \par Description
     **       This function publishes the VehicleGlobalPositionMsg message
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendVehicleGlobalPositionMsg(void);

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
    osalbool VerifyCmdLength(CFE_SB_Msg_t* MsgPtr, uint16 usExpectedLen);

    /************************************************************************/
    /** \brief Validate PE configuration table
    **
    **  \par Description
    **       This function validates PE's configuration table
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

private:
    /************************************************************************/
    /** \brief Initialize the PE configuration tables.
    **
    **  \par Description
    **       This function initializes PE's configuration tables.  This
    **       includes local parameters.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
    **  \retstmt Return codes from #CFE_TBL_Register          \endcode
    **  \retstmt Return codes from #CFE_TBL_Load              \endcode
    **  \retstmt Return codes from #PE_AcquireConfigPointers \endcode
    **  \endreturns
    **
    *************************************************************************/
    int32  InitConfigTbl(void);

    /************************************************************************/
    /** \brief Obtain PE configuration tables data pointers.
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



private:
    /************************************************************************/
    /** \brief Initialize State Covariance
    **
    **  \par Description
    **       This function initializes state covariance to default values.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void initStateCov(void);

    /************************************************************************/
    /** \brief Initialize State Space
    **
    **  \par Description
    **       This function initializes state space to default values.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void InitStateSpace(void);

    /************************************************************************/
    /** \brief Update State Space
    **
    **  \par Description
    **       This function updates state space to current values.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void updateStateSpace(void);

    /************************************************************************/
    /** \brief Update State Space Parameters
    **
    **  \par Description
    **       This function updates correct state space indexes to parameter values
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void updateStateSpaceParams(void);

public:
    /************************************************************************/
    /** \brief Baro Measure
    **
    **  \par Description
    **       This function reads the current baro message
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in/out]   y    A #Vector1F to store baro measurement
    **
    **  \returns
    **  \retcode #CFE_SUCCESS \endcode
    **  \endreturns
    **
    *************************************************************************/
    int32  baroMeasure(math::Vector1F &y);

    /************************************************************************/
    /** \brief Baro Correct
    **
    **  \par Description
    **       This function corrects the baro measurement
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void baroCorrect(void);

    /************************************************************************/
    /** \brief Baro Initialize
    **
    **  \par Description
    **       This function initializes the baro
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void baroInit(void);

    /************************************************************************/
    /** \brief Check Baro Timeout
    **
    **  \par Description
    **       This function checks if the baro message has timed out
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void baroCheckTimeout(void);

    /************************************************************************/
    /** \brief GPS Measure
    **
    **  \par Description
    **       This function reads the current baro message
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in/out]   y    A #Vector6F to store baro measurement
    **
    **  \returns
    **  \retcode #CFE_SUCCESS \endcode
    **  \endreturns
    **
    *************************************************************************/
    int  gpsMeasure(math::Vector6F &y);

    /************************************************************************/
    /** \brief GPS Correct
    **
    **  \par Description
    **       This function corrects the GPS measurement
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void gpsCorrect(void);

    /************************************************************************/
    /** \brief GPS Initialize
    **
    **  \par Description
    **       This function initializes the GPS
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void gpsInit(void);

    /************************************************************************/
    /** \brief Check GPS Timeout
    **
    **  \par Description
    **       This function checks if the GPS message has timed out
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void gpsCheckTimeout(void);

    /************************************************************************/
    /** \brief Land Detector Measure
    **
    **  \par Description
    **       This function reads the current land detector message
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in/out]   y    A #Vector6F to store baro measurement
    **
    **  \returns
    **  \retcode #CFE_SUCCESS \endcode
    **  \endreturns
    **
    *************************************************************************/
    int  landMeasure(math::Vector3F &y);

    /************************************************************************/
    /** \brief Land Detector Correct
    **
    **  \par Description
    **       This function corrects the land detector measurement
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void landCorrect(void);

    /************************************************************************/
    /** \brief Land Detector Initialize
    **
    **  \par Description
    **       This function initializes the land detector
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void landInit(void);

    /************************************************************************/
    /** \brief Check Land Detector Timeout
    **
    **  \par Description
    **       This function checks if the land detector message has timed out
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void landCheckTimeout(void);

    /************************************************************************/
    /** \brief Check Landed
    **
    **  \par Description
    **       This function checks if the vehicle is landed to determine whether
    **       to integrate land parameters into prediction
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    osalbool landed(void);

    /************************************************************************/
    /** \brief Dist Measure
    **
    **  \par Description
    **       This function reads the current dist message
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in/out]   y    A #Vector1F to store dist measurement
    **
    **  \returns
    **  \retcode #CFE_SUCCESS \endcode
    **  \endreturns
    **
    *************************************************************************/
    int32  distMeasure(math::Vector1F &y);

    /************************************************************************/
    /** \brief Dist Correct
    **
    **  \par Description
    **       This function corrects the dist measurement
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void distCorrect(void);

    /************************************************************************/
    /** \brief Dist Initialize
    **
    **  \par Description
    **       This function initializes the dist
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void distInit(void);

    /************************************************************************/
    /** \brief Check Dist Timeout
    **
    **  \par Description
    **       This function checks if the dist message has timed out
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void distCheckTimeout(void);
    
    /************************************************************************/
    /** \brief Flow Measure
    **
    **  \par Description
    **       This function reads the current flow message
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in/out]   y    A #Vector1F to store flow measurement
    **
    **  \returns
    **  \retcode #CFE_SUCCESS \endcode
    **  \endreturns
    **
    *************************************************************************/
    int32  flowMeasure(math::Vector2F &y);

    /************************************************************************/
    /** \brief Flow Correct
    **
    **  \par Description
    **       This function corrects the flow measurement
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void flowCorrect(void);

    /************************************************************************/
    /** \brief Flow Initialize
    **
    **  \par Description
    **       This function initializes the flow
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void flowInit(void);

    /************************************************************************/
    /** \brief Check Flow Timeout
    **
    **  \par Description
    **       This function checks if the flow message has timed out
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void flowCheckTimeout(void);

    /************************************************************************/
    /** \brief Check Timeouts
    **
    **  \par Description
    **       This function calls all the sensor timeout functions.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void CheckTimeouts(void);

    /************************************************************************/
    /** \brief Get Delay Periods
    **
    **  \par Description
    **       This function checks if data is delayed
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  \retcode #CFE_SUCCESS if no delay \endcode
    **  -1 otherwise
    **  \endreturns
    **
    *************************************************************************/
    int getDelayPeriods(float delay, uint8 *periods);

    /************************************************************************/
    /** \brief Update Local Params
    **
    **  \par Description
    **       This function updates the local application struct with up to
    **       date values from the parameter table.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void UpdateLocalParams(void);

    /************************************************************************/
    /** \brief Update State
    **
    **  \par Description
    **       This function is the cyclic operation. It updates the state
    **       covariance, performs error checking, and reports data validity.
    **       Runs at 125hz.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void Update(void);

    /************************************************************************/
    /** \brief Predict
    **
    **  \par Description
    **       This function predicts the next state and is called in the
    **       Update function. Utilizes rk4 algorithm described at:
    **       https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in]   dt    Delta time
    **
    *************************************************************************/
    void Predict(float dt);

    /************************************************************************/
    /** \brief Dynamics
    **
    **  \par Description
    **       This function performs linear algebra operations utilized by the
    **       prediction code.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in]   x    A #Vector10F input
    **  \param [in]   u    A #Vector3F input
    **
    *************************************************************************/
    math::Vector10F dynamics(const math::Vector10F &x, const math::Vector3F &u);

    /************************************************************************/
    /** \brief Check Initialized
    **
    **  \par Description
    **       This function
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  TRUE if initialized. FALSE otherwise.
    **  \endreturns
    **
    *************************************************************************/
    osalbool Initialized(void);

};

#ifdef __cplusplus
}
#endif 

#endif /* PE_APP_H */

/************************/
/*  End of File Comment */
/************************/
