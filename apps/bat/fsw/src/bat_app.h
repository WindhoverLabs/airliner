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

#ifndef BAT_APP_H
#define BAT_APP_H

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

#include "bat_platform_cfg.h"
#include "bat_mission_cfg.h"
#include "bat_perfids.h"
#include "bat_msgids.h"
#include "bat_msg.h"
#include "bat_events.h"
#include "bat_tbldefs.h"
#include "px4_msgs.h"
/************************************************************************
 ** Local Defines
 *************************************************************************/
/** \brief Pipe depth for the data pipe
**
**  \par Limits:
**       minimum of 1, max of CFE_SB_MAX_PIPE_DEPTH.
*/
#define BAT_DATA_PIPE_DEPTH            (2)

/** \brief Pipe name for the Scheduler pipe
**
**  \par Limits:
**       Note, this name must fit in OS_MAX_API_NAME.
*/
#define BAT_DATA_PIPE_NAME             ("BAT_DATA_PIPE")


/************************************************************************
 ** Local Structure Definitions
 *************************************************************************/

#define BAT_LISTENER_TASK_NAME          ("BAT_LISTENER")
#define BAT_LISTENER_TASK_PRIORITY      (103)
#define BAT_MUTEX_NAME                  ("BAT_MUTEX")
#define BAT_CURRENT_FILTER_INIT_VALUE   (-1.0f)
#define BAT_VOLTAGE_FILTER_INIT_VALUE   (-1.0f)
#define BAT_THROTTLE_FILTER_INIT_VALUE  (-1.0f)

extern "C" void BAT_ListenerTaskMain();

typedef struct
{
    PX4_ActuatorControlsMsg_t ActuatorControls0;
    PX4_ActuatorArmedMsg_t    ActuatorArmed;
} BAT_CurrentValueTable_t;

/**
 **  \brief BAT Application Class
 */
class BAT
{
public:
    BAT();
    ~BAT();

    /** \brief CFE Event Table */
    CFE_EVS_BinFilter_t  EventTbl[BAT_EVT_CNT];

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
    BAT_ConfigTbl_t* ConfigTblPtr;

    /** \brief Output Data published at the end of cycle */
    PX4_BatteryStatusMsg_t BatteryStatusMsg;

    /** \brief Housekeeping Telemetry for downlink */
    BAT_HkTlm_t HkTlm;

    /** \brief ID of listener child task */
    uint32 ListenerTaskID;

    /** \brief Mutex for publishing and modifying data */
    uint32 BatteryMutex;

    /** \brief Flag for continueing listener thread execution */
    osalbool m_ChildContinueFlag;

    /** \brief CFE time for comparision */
    CFE_TIME_SysTime_t m_SampleTime;

    /** \brief Remaining voltage estimate */
    float m_RemainingVoltage;
    
    /** \brief Remaining capacity estimate */
    float m_RemainingCapacity;
    
    /** \brief Discharged current estimate */
    float m_Discharged;
    
    /** \brief Filtered throttle value */
    float m_ThrottleFiltered;

    /** \brief Current value table for subscribed messages */
    BAT_CurrentValueTable_t CVT;

    /************************************************************************/
    /** \brief Battery Monitor (BAT) application entry point
     **
     **  \par Description
     **       Battery Monitor Task application entry point.  This function
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
    /** \brief Initialize the Battery Monitor (BAT) application
     **
     **  \par Description
     **       Battery Monitor application initialization routine. This
     **       function performs all the required startup steps to
     **       initialize (or restore from CDS) BAT data structures and get
     **       the application registered with the cFE services so it can
     **       begin to receive command messages and send events.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
     **  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
     **  \retstmt Return codes from #BAT_InitEvent               \endcode
     **  \retstmt Return codes from #BAT_InitPipe                \endcode
     **  \retstmt Return codes from #BAT_InitData                \endcode
     **  \retstmt Return codes from #BAT_InitConfigTbl           \endcode
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
     **       cFE Event Services for use by the BAT application.
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
    /** \brief Initialize global variables used by BAT application
     **
     **  \par Description
     **       This function performs the steps required to initialize
     **       the BAT application data.
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
     **       messages for the BAT application.
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
     **       for the BAT application from the SCH pipe.  This function
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
    /** \brief Battery Monitor Task incoming command processing
     **
     **  \par Description
     **       This function processes incoming commands subscribed
     **       by BAT application
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void ProcessCmdPipe(void);

    /************************************************************************/
    /** \brief Battery Monitor Task incoming data processing
     **
     **  \par Description
     **       This function processes incoming data subscribed
     **       by BAT application
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void ProcessDataPipe(void);

    /************************************************************************/
    /** \brief Battery Monitor Task application commands
     **
     **  \par Description
     **       This function processes command messages
     **       specific to the BAT application
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
    /** \brief Sends BAT housekeeping message
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
    osalbool VerifyCmdLength(CFE_SB_Msg_t* MsgPtr, uint16 usExpectedLen);

private:
    /************************************************************************/
    /** \brief Initialize the BAT configuration tables.
    **
    **  \par Description
    **       This function initializes BAT's configuration table.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
    **  \retstmt Return codes from #CFE_TBL_Register          \endcode
    **  \retstmt Return codes from #CFE_TBL_Load              \endcode
    **  \retstmt Return codes from #BAT_AcquireConfigPointers \endcode
    **  \endreturns
    **
    *************************************************************************/
    int32  InitConfigTbl(void);

    /************************************************************************/
    /** \brief Obtain BAT configuration tables data pointers.
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
    int32 AcquireConfigPointers(void);
    
    /************************************************************************/
    /** \brief Child Continue Execution
    **
    **  \par Description
    **       This function is evaluated in the child task loop to determine
    **       if the child task should continue cycling
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  \retcode #TRUE if yes #FALSE if not  \endcode
    **  \endreturns
    **
    *************************************************************************/
    osalbool  ChildContinueExec(void);
    
    /************************************************************************/
    /** \brief Stop Child Task Execution
    **
    **  \par Description
    **       This function stops execution of the child task by setting the
    **       continue execution flag to false
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void  StopChild(void);
    
    /************************************************************************/
    /** \brief Initialize Listener Task
    **
    **  \par Description
    **       This function intitializes the child task with CFE
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  \retcode #CFE_SUCCESS  if success, error code otherwise  \endcode
    **  \endreturns
    **
    *************************************************************************/
    int32 InitListenerTask(void);
    
    /************************************************************************/
    /** \brief Get Filtered Voltage
    **
    **  \par Description
    **       This function returns the filtered voltage value
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in]   Voltage        #float of current voltage
    **
    **  \returns
    **  Filtered voltage
    **  \endreturns
    **
    *************************************************************************/
    float GetFilteredVoltage(float Voltage);
    
    /************************************************************************/
    /** \brief Get Filtered Current
    **
    **  \par Description
    **       This function returns the filtered current value
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in]   Current        #float of current value
    **
    **  \returns
    **  Filtered Current
    **  \endreturns
    **
    *************************************************************************/
    float GetFilteredCurrent(float Current);
    
    /************************************************************************/
    /** \brief Get Filtered Throttle
    **
    **  \par Description
    **       This function returns the filtered throttle vlaue
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in]   Throttle        #float of current throttle
    **
    **  \returns
    **  Filtered thottle
    **  \endreturns
    **
    *************************************************************************/
    float GetFilteredThrottle(float Throttle);
    
    /************************************************************************/
    /** \brief Get Discharged Amount
    **
    **  \par Description
    **       This function returns amount of mAh discharged from the battery
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in]   Current        #float of current value
    **
    **  \returns
    **  Discharged amount
    **  \endreturns
    **
    *************************************************************************/
    float GetDischarged(float Current);
    
    /************************************************************************/
    /** \brief Get Charge Remaining
    **
    **  \par Description
    **       This function returns the amount of charge remaining.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in]   Voltage                   #float of current voltage
    **  \param [in]   Current                   #float of current value
    **  \param [in]   ThrottleNormalized        #float of current filtered throttle
    **  \param [in]   Armed                     #osalbool of current voltage
    **
    **  \returns
    **  Percentage of battery remaining
    **  \endreturns
    **
    *************************************************************************/
    float GetRemaining(float Voltage, float Current, float ThrottleNormalized, osalbool Armed);
    
    /************************************************************************/
    /** \brief Get Warning Severity
    **
    **  \par Description
    **       This function returns the current severity of battery level
    **       based on configured values
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  #PX4_BatteryWarningSeverity_t for current severity level
    **  \endreturns
    **
    *************************************************************************/
    PX4_BatteryWarningSeverity_t GetWarningSeverity(float Remaining);
    
    /************************************************************************/
    /** \brief Get Voltage Scale
    **
    **  \par Description
    **       This function returns the voltage scale
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  #float of voltage scale
    **  \endreturns
    **
    *************************************************************************/
    float GetScale(void);

    /************************************************************************/
    /** \brief Publish Battery Status
    **
    **  \par Description
    **       This function publishes the battery status message to the CFE SB
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
    **  \endreturns
    **
    *************************************************************************/
    void  PublishBatteryStatus(void);

    /************************************************************************/
    /** \brief Initialize Device
    **
    **  \par Description
    **       This function intializes the custom layer for the battery driver
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  #CFE_SUCCESS if success, error code otherwise \endcode
    **  \endreturns
    **
    *************************************************************************/
    int32 InitDevice(void);
    
    /************************************************************************/
    /** \brief Close Device
    **
    **  \par Description
    **       This function closes the custom layer driver
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void  CloseDevice(void);
    
    /************************************************************************/
    /** \brief Read Device
    **
    **  \par Description
    **       This function reads the battery information from the connected device
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \param [in/out]   Voltage         Pointer to #float of voltage
    **  \param [in/out]   Current         Pointer to #float of current
    **
    **  \returns
    **  #CFE_SUCCESS if success, error code otherwise \endcode
    **  \endreturns
    **
    *************************************************************************/
    int32 ReadDevice(float &Voltage, float &Current);

public:
    /************************************************************************/
    /** \brief Validate BAT configuration table
    **
    **  \par Description
    **       This function validates BAT's configuration table
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
    /** \brief Listener Task Main
    **
    **  \par Description
    **       This is the executing function of the child task
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    *************************************************************************/
    void  ListenerTaskMain(void);
};

#ifdef __cplusplus
}
#endif 

#endif /* BAT_APP_H */

/************************/
/*  End of File Comment */
/************************/
