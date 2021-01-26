    
#ifndef MAVLINK_APP_H
#define MAVLINK_APP_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"

#include "mavlink_platform_cfg.h"
#include "mavlink_mission_cfg.h"
#include "mavlink_private_ids.h"
#include "mavlink_private_types.h"
#include "mavlink_perfids.h"
#include "mavlink_msgids.h"
#include "mavlink_msg.h"
#include "mavlink_events.h"
#include "mavlink_config_utils.h"
#include "mavlink_cds_utils.h"
#include "mavlink_custom.h"
#include "mavlink.h"
#include "prm_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Local Defines
*************************************************************************/
#define MAVLINK_TIMEOUT_MSEC             	(1000)
#define MAVLINK_LISTENER_TASK_NAME  				"MAVLINK_LISTENER"
#define MAVLINK_PASSTHRU_LISTENER_TASK_NAME  		"MAVLINK_PASSTHRU_LISTENER"
#define MAVLINK_LISTENER_TASK_STACK_SIZE			16000
#define MAVLINK_LISTENER_TASK_PRIORITY				100
#define MAVLINK_CFG_TBL_MUTEX_NAME 					"MAVLINK_CFG_TBL_MUTEX"
#define MAVLINK_TIME_TBL_MUTEX_NAME 				"MAVLINK_TIME_TBL_MUTEX"

/************************************************************************
** Local Structure Definitions
*************************************************************************/
/**
**  \brief MAVLINK Operational Data Structure
*/
typedef struct
{
    /** \brief CFE Event Table */
    CFE_EVS_BinFilter_t  EventTbl[MAVLINK_EVT_CNT];

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

    /** \brief Param Table Handle */
    CFE_TBL_Handle_t  ActionMapHdl;

    /** \brief Param Table Pointer */
    MAVLINK_ActionMapTbl_t*  ActionMapPtr;

    /* Critical Data Storage (CDS) table-related */

    /** \brief CDS Table Handle */
    CFE_ES_CDSHandle_t  CdsTblHdl;

    /** \brief CDS Table data */
    MAVLINK_CdsTbl_t  CdsTbl;

    /** \brief Mavlink cmd ingest flag */
    boolean IngestActive;

    /** \brief Mavlink listeners alive flag */
    boolean ListenersAlive;

    /** \brief Buffer for child task cmd ingest */
    uint8           IngestBuffer[MAVLINK_MAX_PACKET_LEN];

    /** \brief Buffer for child task pass thru ingest */
	uint8           PassThruIngestBuffer[MAVLINK_MAX_PACKET_LEN];

    /** \brief ID of listener child task */
	uint32          ListenerTaskID;

    /** \brief ID of listener child task */
	uint32          PassThruListenerTaskID;

    /* Inputs/Outputs */

    /** \brief Input Data from I/O or other apps */
    MAVLINK_InData_t   InData;

    /** \brief Output Data published at the end of cycle */
    MAVLINK_OutData_t  OutData;

    /** \brief Housekeeping Telemetry for downlink */
    MAVLINK_HkTlm_t  HkTlm;

    boolean ParamsInitialized;

    /** \brief Mutex for Action Map table */
	uint32          ActionMapMutex;

	/** \brief  */
	uint32          WakeupCount;

} MAVLINK_AppData_t;

typedef struct
{
	char name[PRMLIB_MSG_PARAM_NAME_LEN];
    float value;
    uint8 type;
    uint8 vehicle_id;
    uint8 component_id;
} MAVLINK_ParamData_t;

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
/** \brief CFS Mavlink Task (MAVLINK) application entry point
**
**  \par Description
**       CFS Mavlink Task application entry point.  This function
**       performs app initialization, then waits for the cFE ES Startup
**       Sync, then executes the RPR main processing loop.
**
**  \par Assumptions, External Events, and Notes:
**       If there is an unrecoverable failure during initialization the
**       main loop is never executed and the application will exit.
**
*************************************************************************/
void  MAVLINK_AppMain(void);

/************************************************************************/
/** \brief Initialize the CFS Mavlink (MAVLINK) application
**
**  \par Description
**       Mavlink application initialization routine. This
**       function performs all the required startup steps to
**       initialize (or restore from CDS) MAVLINK data structures and get
**       the application registered with the cFE services so it can
**       begin to receive command messages and send events.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
**  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
**  \retstmt Return codes from #MAVLINK_InitEvent               \endcode
**  \retstmt Return codes from #MAVLINK_InitPipe                \endcode
**  \retstmt Return codes from #MAVLINK_InitData                \endcode
**  \retstmt Return codes from #MAVLINK_InitConfigTbl           \endcode
**  \retstmt Return codes from #MAVLINK_InitCdsTbl              \endcode
**  \retstmt Return codes from #OS_TaskInstallDeleteHandler \endcode
**  \endreturns
**
*************************************************************************/
int32  MAVLINK_InitApp(void);

/************************************************************************/
/** \brief Initialize Event Services and Event tables
**
**  \par Description
**       This function performs the steps required to setup
**       cFE Event Services for use by the MAVLINK application.
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
int32  MAVLINK_InitEvent(void);

/************************************************************************/
/** \brief Initialize global variables used by MAVLINK application
**
**  \par Description
**       This function performs the steps required to initialize
**       the MAVLINK application data.
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
int32  MAVLINK_InitData(void);

/************************************************************************/
/** \brief Initialize message pipes
**
**  \par Description
**       This function performs the steps required to setup
**       initialize the cFE Software Bus message pipes and subscribe to
**       messages for the MAVLINK application.
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
int32  MAVLINK_InitPipe(void);

/************************************************************************/
/** \brief Receive and process messages
**
**  \par Description
**       This function receives and processes messages
**       for the MAVLINK application
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
int32  MAVLINK_RcvMsg(int32 iBlocking);

/************************************************************************/
/** \brief Mavlink Task incoming data processing
**
**  \par Description
**       This function processes incoming data subscribed
**       by MAVLINK application
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  MAVLINK_ProcessNewData(void);

/************************************************************************/
/** \brief Mavlink Task incoming command processing
**
**  \par Description
**       This function processes incoming commands subscribed
**       by MAVLINK application
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  MAVLINK_ProcessNewCmds(void);

/************************************************************************/
/** \brief Mavlink Task application commands
**
**  \par Description
**       This function processes command messages
**       specific to the MAVLINK application
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   MsgPtr       A #CFE_SB_Msg_t pointer that
**                             references the software bus message
**
*************************************************************************/
void  MAVLINK_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr);

/************************************************************************/
/** \brief Sends MAVLINK housekeeping message
**
**  \par Description
**       This function sends the housekeeping message
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  MAVLINK_ReportHousekeeping(void);

/************************************************************************/
/** \brief Sends MAVLINK output data
**
**  \par Description
**       This function publishes the MAVLINK application output data.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  MAVLINK_SendOutData(void);

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
boolean  MAVLINK_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr, uint16 usExpectedLen);

int32 MAVLINK_InitChildTasks(void);
void MAVLINK_PassThruListenerTaskMain(void);
void MAVLINK_ListenerTaskMain(void);
void MAVLINK_CleanupCallback();
void MAVLINK_EnableConnection(void);
void MAVLINK_DisableConnection(void);
void MAVLINK_MessageRouter(mavlink_message_t msg);
void MAVLINK_ProcessHeartbeat(mavlink_heartbeat_t heartbeat);
void MAVLINK_SendHeartbeat(void);
void MAVLINK_SendParamsToGCS(void);
void MAVLINK_SendParamToGCS(MAVLINK_ParamData_t param_data, uint16 param_index, uint16 param_count);
MAVLINK_MsgAction_t MAVLINK_GetMessageAction(mavlink_message_t msg);
int32 MAVLINK_HandleRequestParams();
int32 MAVLINK_HandleSetParam(mavlink_param_set_t param);
int32 MAVLINK_HandleRequestParamRead(mavlink_param_request_read_t paramMsg);
int32 MAVLINK_HandleRequestMission();
int32 MAVLINK_HandleCommandLong(mavlink_command_long_t msg);
void MAVLINK_ProcessHeartbeat(mavlink_heartbeat_t heartbeat);
void MAVLINK_ProcessNewParamCmds(CFE_SB_Msg_t* MsgPtr);
void MAVLINK_MessagePassThru(mavlink_message_t msg);

int32 MAVLINK_InitCustom(void);
int32 MAVLINK_ReadPassThru(char* buffer, uint32* size);
int32 MAVLINK_SendPassThru(const char* buffer, uint32 size);
int32 MAVLINK_ReadMessage(char* buffer, uint32* size);
int32 MAVLINK_SendMessage(const char* buffer, uint32 size);

#ifdef __cplusplus
}
#endif 

#endif /* MAVLINK_APP_H */

/************************/
/*  End of File Comment */
/************************/
