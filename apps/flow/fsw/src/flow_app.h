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

#ifndef FLOW_APP_H
#define FLOW_APP_H

/************************************************************************
 ** Pragmas
 *************************************************************************/

/************************************************************************
 ** Includes
 *************************************************************************/
#include "cfe.h"

#include "flow_platform_cfg.h"
#include "flow_mission_cfg.h"
#include "flow_perfids.h"
#include "flow_msgids.h"
#include "flow_msg.h"
#include "flow_events.h"
#include "flow_tbldefs.h"
#include "px4_msgs.h"
#include "px4lib.h"


/************************************************************************
 ** Local Defines
 *************************************************************************/

/** \brief The length of event filter array */
#define FLOW_MAX_EVENT_FILTER 						(16)
/** \brief The SB pend behavior type for the Scheduler pipe */
#define FLOW_SCH_PIPE_PEND_TIME         			(CFE_SB_PEND_FOREVER)
/** \brief Minimum time difference (in us) for integration between two gyroscope messages */
#define FLOW_MIN_INT_TIME_DIFF         				(1000)
/** \brief One second is 1000000 us */
#define FLOW_US_IN_SEC         						(1000000.0f)
/** \brief Default sensor id */
#define FLOW_SENSOR_ID         						(1)
/** \brief HOV maximum value */
#define FLOW_HOV_MAX 								(2*M_PI)
/** \brief HOV minimum value */
#define FLOW_HOV_MIN 								(0)
/** \brief TILE SIZE maximum value */
#define FLOW_TILE_SIZE_MAX 							(10)
/** \brief TILE SIZE minimum value */
#define FLOW_TILE_SIZE_MIN 							(2)
/** \brief BLOCKS maximum value */
#define FLOW_BLOCKS_MAX								(8)
/** \brief BLOCKS minimum value */
#define FLOW_BLOCKS_MIN								(2)
/** \brief OUTTPUT RATE maximum value */
#define FLOW_OUTPUT_RATE_MAX 						(10)
/** \brief OUTTPUT RATE minimum value */
#define FLOW_OUTPUT_RATE_MIN 						(2)
/** \brief IMAGE WIDTH maximum value */
#define FLOW_IMAGE_WIDTH_MAX 						(64)
/** \brief IMAGE WIDTH minimum value */
#define FLOW_IMAGE_WIDTH_MIN 						(32)
/** \brief IMAGE HEIGHT maximum value */
#define FLOW_IMAGE_HEIGHT_MAX 						(64)
/** \brief IMAGE HEIGHT minimum value */
#define FLOW_IMAGE_HEIGHT_MIN 						(32)
/** \brief SEARCH SIZE maximum value */
#define FLOW_SEARCH_SIZE_MAX 						(10)
/** \brief SEARCH SIZE minimum value */
#define FLOW_SEARCH_SIZE_MIN 						(2)
/** \brief FEATURE THRESHOLD maximum value */
#define FLOW_FEATURE_THRESHOLD_MAX 					(50)
/** \brief FEATURE THRESHOLD minimum value */
#define FLOW_FEATURE_THRESHOLD_MIN 					(10)
/** \brief VLAUE THRESHOLD maximum value */
#define FLOW_VALUE_THRESHOLD_MAX 					(5000)
/** \brief VLAUE THRESHOLD minimum value */
#define FLOW_VALUE_THRESHOLD_MIN 					(10)
/** \brief MEAN COUNT THRESHOLD maximum value */
#define FLOW_MEAN_COUNT_THRESHOLD_MAX 				(20)
/** \brief MEAN COUNT THRESHOLD minimum value */
#define FLOW_MEAN_COUNT_THRESHOLD_MIN 				(2)
/************************************************************************
 ** Local Structure Definitions
 *************************************************************************/
/**
 **  \brief Current image and timestamp
 */
typedef struct
{
    PX4_OpticalFlowFrameMsg_t OpticalFlowFrameMsg;
    PX4_SensorGyroMsg_t SensorGyroMsg;
} FLOW_CurrentValueTable_t;

/**
 **  \brief Previous image and timestamp is stored
 */
typedef struct
{
	uint64 Timestamp;
	uint8 Frame[PX4_OPTICAL_FLOW_FRAME_SIZE];
} FLOW_PreviousFrame;


/**
 **  \brief FLOW Application Class
 */
class FLOW
{
public:
    FLOW();
    ~FLOW();

    /**\brief Scheduling Pipe ID */
    CFE_SB_PipeId_t SchPipeId;

    /**\brief Gyro Pipe ID */
	CFE_SB_PipeId_t GyroPipeId;

    /** \brief Command Pipe ID */
    CFE_SB_PipeId_t CmdPipeId;

    /* Task-related */
    /** \brief Task Run Status */
    uint32 uiRunStatus;

    /* Config table-related */
    /** \brief Config Table Handle */
    CFE_TBL_Handle_t ConfigTblHdl;

    /** \brief Config Table Pointer */
    FLOW_ConfigTbl_t* ConfigTblPtr;

    /** \brief Output Data published at the end of cycle */
    PX4_OpticalFlowMsg_t OpticalFlowMsg;

    /** \brief Store previous frame */
    FLOW_PreviousFrame PreviousFrame;

    /** \brief Housekeeping Telemetry for downlink */
    FLOW_HkTlm_t HkTlm;

    /** \brief Diagnostic data for downlink */
    FLOW_DiagPacket_t DiagTlm;

    /** \brief Current Value Table */
    FLOW_CurrentValueTable_t CVT;
    /************************************************************************/
    /** \brief Optical Flow (FLOW) application entry point
     **
     **  \par Description
     **       Optical Flow Task application entry point.  This function
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
    /** \brief Initialize the Optical Flow (FLOW) application
     **
     **  \par Description
     **       Optical Flow application initialization routine. This
     **       function performs all the required startup steps to
     **       initialize (or restore from CDS) FLOW data structures and get
     **       the application registered with the cFE services so it can
     **       begin to receive command messages and send events.
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     **  \returns
     **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
     **  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
     **  \retstmt Return codes from #FLOW_InitEvent               \endcode
     **  \retstmt Return codes from #FLOW_InitPipe                \endcode
     **  \retstmt Return codes from #FLOW_InitData                \endcode
     **  \retstmt Return codes from #FLOW_InitConfigTbl           \endcode
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
     **       cFE Event Services for use by the FLOW application.
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
    /** \brief Initialize global variables used by FLOW application
     **
     **  \par Description
     **       This function performs the steps required to initialize
     **       the FLOW application data.
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
     **       messages for the FLOW application.
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
     **       for the FLOW application from the SCH pipe.  This function
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
    /** \brief Optical Flow Task incoming command processing
     **
     **  \par Description
     **       This function processes incoming commands subscribed
     **       by FLOW application
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void ProcessCmdPipe(void);

    /************************************************************************/
    /** \brief Optical Flow Task application commands
     **
     **  \par Description
     **       This function processes command messages
     **       specific to the FLOW application
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
    /** \brief Sends FLOW housekeeping message
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
    /** \brief Sends the OpticalFlowMsg message.
     **
     **  \par Description
     **       This function publishes the OpticalFlowMsg message containing
     **       <TODO>
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendOpticalFlowMsg(void);
    /************************************************************************/
    /** \brief Sends the Diagnostic message.
     **
     **  \par Description
     **       This function publishes the OpticalFlowMsg message containing
     **       <TODO>
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendDiagMsg(void);
    /************************************************************************/
    /** \brief Sends the DistanceSensorMsg message.
     **
     **  \par Description
     **       This function publishes the DistanceSensorMsg message containing
     **       <TODO>
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void SendDistanceSensorMsg(void);
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
    /** \brief Initialize gyroscope message pipe
     **
     **  \par Description
     **       This function performs the steps required to setup
     **       initialize the cFE Software Bus message pipes and subscribe to
     **       gyroscope message for the FLOW application.
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
    int32 InitGyroPipe(void);

    /************************************************************************/
    /** \brief Process Incoming Gyro Messages
     **
     **  \par Description
     **       This function processes gyro messages
     **       specific to the FLOW application
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void ProcessGyroPipe(void);

    /************************************************************************/
    /** \brief Perform USAD8 Operation
     **
     **  \par Description
     **       Performs unsigned sum of absolute differences on
     **       given two values
     **
	 **  \param [in]   val1        Numerical value 1
     **  \param [in]   val2        Numerical value 2
     **
     **  \returns
     **  Unsigned sum of absolute differences
     **  \endreturns
     **
     *************************************************************************/
    uint32 USAD8(uint32 val1, uint32 val2);

    /************************************************************************/
    /** \brief Perform USADA8 Operation
     **
     **  \par Description
     **       Performs accumulate unsigned sum of absolute differences on
     **       given values
     **
	 **  \param [in]   val1        Numerical value 1
     **  \param [in]   val2        Numerical value 2
     **  \param [in]   val3        Numerical value 3
     **
     **  \returns
     **  Accumulate unsigned sum of absolute differences
     **  \endreturns
     **
     *************************************************************************/
    uint32 USADA8(uint32 val1, uint32 val2, uint32 val3);

    /************************************************************************/
    /** \brief Perform UHADD8 Operation
     **
     **  \par Description
     **       Performs unsigned halving add 8-bit on
     **       given values
     **
	 **  \param [in]   val1        Numerical value 1
     **  \param [in]   val2        Numerical value 2
     **
     **  \returns
     **  Unsigned halving add 8-bit
     **  \endreturns
     **
     *************************************************************************/
	uint32 UHADD8(uint32 val1, uint32 val2);

    /************************************************************************/
    /** \brief Compute 8X8 SAD
     **
     **  \par Description
     **       Calculates SAD between two 8x8 tiles
     **
	 **  \param [in]   image1        	Pointer to image 1
     **  \param [in]   image2        	Pointer to image 2
     **  \param [in]   off1X        	Offset 1 along X axis
     **  \param [in]   off1Y        	Offset 1 along Y axis
     **  \param [in]   off2X        	Offset 2 along X axis
     **  \param [in]   off2Y        	Offset 2 along Y axis
     **  \param [in]   row_size       	Row size
     **
     **  \returns
     **  Unsigned halving add 8-bit
     **  \endreturns
     **
     *************************************************************************/
	uint32 ComputeSad8X8(const uint8 * image1,const uint8 *image2, uint16 off1X, uint16 off1Y, uint16 off2X, uint16 off2Y, uint16 row_size);

    /************************************************************************/
    /** \brief Compute Sub-pixel
     **
     **  \par Description
     **       Calculates mean flow for each sub pixel
     **
	 **  \param [in]   image1        	Pointer to image 1
     **  \param [in]   image2        	Pointer to image 2
     **  \param [in]   off1X        	Offset 1 along X axis
     **  \param [in]   off1Y        	Offset 1 along Y axis
     **  \param [in]   off2X        	Offset 2 along X axis
     **  \param [in]   off2Y        	Offset 2 along Y axis
     **  \param [in]   acc       		pointer to acc
     **  \param [in]   row_size       	Row size
     **
     **  \returns
     **  Unsigned halving add 8-bit
     **  \endreturns
     **
     *************************************************************************/
	uint32 ComputeSubpixel(const uint8 *image1, const uint8 *image2, uint16 off1X, uint16 off1Y,uint16 off2X, uint16 off2Y, uint32 *acc, uint16 row_size);

    /************************************************************************/
    /** \brief Compute Difference
     **
     **  \par Description
     **       For a given image use offset to identify 4x4 patterns
     **       and calculate inter-row and inter-column differences
     **
	 **  \param [in]   image        	Pointer to Image
     **  \param [in]   offX        		Offset along X axis
     **  \param [in]   offY        		Offset along Y axis
     **  \param [in]   row_size       	Row size
     **
     **  \returns
     **  Unsigned halving add 8-bit
     **  \endreturns
     **
     *************************************************************************/
	uint32 ComputeDifference(const uint8 *image, uint16 offX, uint16 offY, uint16 row_size);

    /************************************************************************/
    /** \brief Initialize Limit Rate
     **
     **  \par Description
     **       Initializes member variables associated with limiting
     **       rate of optical flow.
     **
     *************************************************************************/
	void InitializeLimitRate(void);

    /************************************************************************/
    /** \brief Limit Optical Flow Rate
     **
     **  \par Description
     **       Adjusts the rate optical flow message is published
     **       by setting the quality to -1 when a off-sync flow
     **       calculation is performed.
     **
	 **  \param [in]   flow_quality        	Flow quality
     **  \param [in]   frame_time_us        Timestamp of the frame in us
     **
     **  \returns
     **  Optical flow quality
     **  \endreturns
     **
     *************************************************************************/
	uint32 LimitRate(int32 flow_quality, const uint32 frame_time_us);

    /************************************************************************/
    /** \brief Compute Flow
     **
     **  \par Description
     **       Calculate instantaneous optical flow between previous
     **       and current image.
     **
	 **  \param [in]   image1        	Pointer to previous image
     **  \param [in]   image2        	Pointer to current image

     **
     **  \returns
     **  Optical flow quality
     **  \endreturns
     **
     *************************************************************************/
	uint32 ComputeFlow(const uint8 *image1, const uint8 *image2);

    /************************************************************************/
    /** \brief Calculate Optical FLow
     **
     **  \par Description
     **       Calculates optical flow, accumulated gyroscope value, copies onto
     **       and publishes OpticalFlowMsg message.
     **
     **  \returns
     **  Optical flow quality
     **  \endreturns
     **
     *************************************************************************/
	uint32 CalculateOpticalFlow(void);

    /************************************************************************/
    /** \brief Initializes flow application's dependent member variables
     **
     **  \par Assumptions, External Events, and Notes:
     **       None
     **
     *************************************************************************/
    void InitFlowApp(void);

private:
    /************************************************************************/
    /** \brief Initialize the FLOW configuration tables.
    **
    **  \par Description
    **       This function initializes FLOW's configuration tables.  This
    **       includes <TODO>.
    **
    **  \par Assumptions, External Events, and Notes:
    **       None
    **
    **  \returns
    **  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
    **  \retstmt Return codes from #CFE_TBL_Register          \endcode
    **  \retstmt Return codes from #CFE_TBL_Load              \endcode
    **  \retstmt Return codes from #FLOW_AcquireConfigPointers \endcode
    **  \endreturns
    **
    *************************************************************************/
    int32  InitConfigTbl(void);

    /************************************************************************/
    /** \brief Obtain FLOW configuration tables data pointers.
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
    /** \brief Validate FLOW configuration table
    **
    **  \par Description
    **       This function validates FLOW's configuration table
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
};


#endif /* FLOW_APP_H */

/************************/
/*  End of File Comment */
/************************/
