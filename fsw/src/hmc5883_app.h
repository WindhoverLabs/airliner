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

#ifndef HMC5883_APP_H
#define HMC5883_APP_H

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
#include "hmc5883_platform_cfg.h"
#include "hmc5883_mission_cfg.h"
#include "hmc5883_perfids.h"
#include "hmc5883_msgids.h"
#include "hmc5883_msg.h"
#include "hmc5883_events.h"
#include "hmc5883_tbldefs.h"
#include "px4_msgs.h"

/************************************************************************
 ** Local Defines
 *************************************************************************/
/** \brief Gain setting 1090 (default).
**
**  \par Description:
**       Gain setting Recommended Sensor Field Range +- 1.3 Ga,
**       Gain (LSb/Gauss) 1090, Digital Resoluation (mG/LSb) 0.92.
*/
#define HMC5883_BITS_CONFIG_B_RANGE_1GA3          (0x01 << 5)

/** \brief Gain setting 820.
**
**  \par Description:
**       Gain setting Recommended Sensor Field Range +- 1.9 Ga,
**       Gain (LSb/Gauss) 820, Digital Resoluation (mG/LSb) 1.22.
*/
#define HMC5883_BITS_CONFIG_B_RANGE_1GA9          (0x02 << 5)

/** \brief Gain setting 660.
**
**  \par Description:
**       Gain setting Recommended Sensor Field Range +- 2.5 Ga,
**       Gain (LSb/Gauss) 660, Digital Resoluation (mG/LSb) 1.52.
*/
#define HMC5883_BITS_CONFIG_B_RANGE_2GA5          (0x03 << 5)

/** \brief Configuration register A default value.
**
**  \par Description:
**       Data output rate default.
*/
#define HMC5883_BITS_CONFIG_A_DEFAULT             (0x10)

/** \brief Enabled temperature compensation (HMC5983).
**
**  \par Limits:
**       None.
*/
#define HMC5983_TEMP_SENSOR_ENABLE                (1 << 7)

/** \brief Enable positive bias configuration for X, Y, and Z axes.
** 
**  \par Description:
**       In this configuration, a positive current is forced across the 
**       resistive load for all three axes.
*/
#define HMC5883_POS_BIAS_ENABLE                   (0x01)

/** \brief Enable negative bias configuration for X, Y, and Z axes.
** 
**  \par Description:
**       In this configuration, a negative current is forced across the 
**       resistive load for all three axes.
*/
#define HMC5883_NEG_BIAS_ENABLE                   (0x02)


/************************************************************************
 ** Local Structure Definitions
 *************************************************************************/
/**
 * \brief application status
 */
typedef enum
{
    /*! App status uninitialized */
    HMC5883_UNINITIALIZED = 0,
    /*! App status uninitialized */
    HMC5883_INITIALIZED   = 1
} HMC5883_Status_t;


/**
 * \brief application parameters
 */
typedef struct
{
    /*! Param X-axis calibration scale */
    float x_scale;
    /*! Param Y-axis calibration scale */
    float y_scale;
    /*! Param Z-axis calibration scale */
    float z_scale;
    /*! Param X-axis calibration offset */
    float x_offset;
    /*! Param Y-axis calibration offset */
    float y_offset;
    /*! Param Z-axis calibration offset */
    float z_offset;
} HMC5883_Params_t;


/**
 **  \brief HMC5883 Application Class
 */
class HMC5883
{
public:
    HMC5883();
    ~HMC5883();

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
    HMC5883_ConfigTbl_t* ConfigTblPtr;
    
    /** \brief Output Data published at the end of cycle */
    PX4_SensorMagMsg_t SensorMagMsg;
    
    /** \brief params from the config table */
    HMC5883_Params_t m_Params;
    
    /** \brief Housekeeping Telemetry for downlink */
    HMC5883_HkTlm_t HkTlm;
    
    /** \brief Diagnostic data for downlink */
    HMC5883_DiagPacket_t Diag;

    /************************************************************************/
    /** \brief HMC5883 Driver Application (HMC5883) application entry point
     **
     **  \par Description
     **       HMC5883 Driver Application Task application entry point.  This function
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
    /** \brief Initialize the HMC5883 Driver Application (HMC5883) application
     **
     **  \par Description
     **       HMC5883 Driver Application application initialization routine. This
     **       function performs all the required startup steps to
     **       initialize (or restore from CDS) HMC5883 data structures and get
     **       the application registered with the cFE services so it can
     **       begin to receive command messages and send events.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
     **  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
     **  \retstmt Return codes from #HMC5883_InitEvent               \endcode
     **  \retstmt Return codes from #HMC5883_InitPipe                \endcode
     **  \retstmt Return codes from #HMC5883_InitData                \endcode
     **  \retstmt Return codes from #HMC5883_InitConfigTbl           \endcode
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
     **       cFE Event Services for use by the HMC5883 application.
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
    /** \brief Initialize global variables used by HMC5883 application
     **
     **  \par Description
     **       This function performs the steps required to initialize
     **       the HMC5883 application data.
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
     **       messages for the HMC5883 application.
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
     **       for the HMC5883 application from the SCH pipe.  This function
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
    /** \brief HMC5883 Driver Application Task incoming command processing
     **
     **  \par Description
     **       This function processes incoming commands subscribed
     **       by HMC5883 application
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void ProcessCmdPipe(void);

    /************************************************************************/
    /** \brief HMC5883 Driver Application Task application commands
     **
     **  \par Description
     **       This function processes command messages
     **       specific to the HMC5883 application
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
    /** \brief Sends HMC5883 housekeeping message
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
    /** \brief Sends the SensorMagMsg message.
     **
     **  \par Description
     **       This function publishes the SensorMagMsg message containing
     **       <TODO>
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendSensorMagMsg(void);
    
    /************************************************************************/
    /** \brief Sends a diagnostic message.
     **
     **  \par Description
     **       This function publishes the diagnostic message.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendDiag(void);

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
    
    /************************************************************************/
    /** \brief Read from the HMC5883.
     **
     **  \par Description
     **       This function populates values in the associated PX4 
     **       message.
     **       <TODO>
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void ReadDevice(void);

    /************************************************************************/
    /** \brief Run the self calibration routine.
    **
    **  \par Description
    **       This function uses the device built in self test to create a
    **       bias for calibration.
    **
    **  \param [out]    Calibration    The calibration result.
    **
    **  \returns    TRUE for success, FALSE for failure.
    **
    *************************************************************************/
    boolean SelfCalibrate(HMC5883_CalibrationMsg_t *Calibration);

    /************************************************************************/
    /** \brief Validate scale values.
    **
    **  \par Description
    **       Sanity check scale values.
    **
    **  \returns    TRUE for valid, FALSE for invalid.
    **
    *************************************************************************/
    boolean CheckScale(float X, float Y, float Z);
    
    /************************************************************************/
    /** \brief Validate offset values.
    **
    **  \par Description
    **       Sanity check offset values.
    **
    **  \returns    TRUE for valid, FALSE for invalid.
    **
    *************************************************************************/
    boolean CheckOffset(float X, float Y, float Z);
    
private:
    /************************************************************************/
    /** \brief Initialize the HMC5883 configuration tables.
    **
    **  \par Description
    **       This function initializes HMC5883's configuration tables.  This
    **       includes <TODO>.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
    **  \retstmt Return codes from #CFE_TBL_Register          \endcode
    **  \retstmt Return codes from #CFE_TBL_Load              \endcode
    **  \retstmt Return codes from #HMC5883_AcquireConfigPointers \endcode
    **  \endreturns
    **
    *************************************************************************/
    int32  InitConfigTbl(void);

    /************************************************************************/
    /** \brief Obtain HMC5883 configuration tables data pointers.
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

public:
    /************************************************************************/
    /** \brief Validate HMC5883 configuration table
    **
    **  \par Description
    **       This function validates HMC5883's configuration table
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
    
    /************************************************************************/
    /** \brief Update parameters from the configuration table.
     **
     **  \par Description
     **       This function updates the current paramamters from the 
     **       currently loaded configuration table.
     **
     *************************************************************************/
    void UpdateParamsFromTable(void);
    
    /************************************************************************/
    /** \brief Set sensor calibation values
    **
    **  \par Description
    **       Updates the sensor calibration table with passed values.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in]   CalibrationMsgPtr   A pointer to the calibration messsage
    **
    **  \returns
    **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
    **  \endreturns
    **
    *************************************************************************/
    int32 UpdateCalibrationValues(HMC5883_SetCalibrationCmd_t* CalibrationMsgPtr);
};


/************************************************************************/
/** \brief Cleanup prior to exit
**
**  \par Description
**       This function handles any necessary cleanup prior
**       to application exit.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void HMC5883_CleanupCallback(void);

#ifdef __cplusplus
}
#endif 

#endif /* HMC5883_APP_H */

/************************/
/*  End of File Comment */
/************************/
