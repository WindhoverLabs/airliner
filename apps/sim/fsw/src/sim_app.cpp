/************************************************************************
** Includes
*************************************************************************/
#include <string.h>

#include "cfe.h"

#include "sim_app.h"
#include "sim_msg.h"
#include "sim_version.h"
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include "mavlink.h"
#include "simlib.h"
#include "math/Vector3F.hpp"
//#include "integrator.h"


#include "px4lib.h"
#include "px4lib_msgids.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Instantiate the application object.                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
SIM oSIM;



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Default constructor.                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
SIM::SIM() :
	_accel_int(1000000 / GYROSIM_ACCEL_DEFAULT_RATE, true),
	_gyro_int(1000000 / GYROSIM_GYRO_DEFAULT_RATE, true)
{

}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Destructor constructor.                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
SIM::~SIM()
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables.                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 SIM::InitEvent()
{
    int32  iStatus=CFE_SUCCESS;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(0, 0, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("SIM - Failed to register with EVS (0x%08lX)\n", iStatus);
    }

    return iStatus;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 SIM::InitPipe()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&SchPipeId,
    		SIM_SCH_PIPE_DEPTH,
			SIM_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(SIM_WAKEUP_MID, SchPipeId, CFE_SB_Default_Qos, SIM_WAKEUP_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(SIM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
            		"Sch Pipe failed to subscribe to SIM_WAKEUP_MID. (0x%08lX)",
                    iStatus);
            goto SIM_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(SIM_SEND_HK_MID, SchPipeId, CFE_SB_Default_Qos, SIM_SEND_HK_MID_MAX_MSG_COUNT);
        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(SIM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
					 "CMD Pipe failed to subscribe to SIM_SEND_HK_MID. (0x%08X)",
					 (unsigned int)iStatus);
            goto SIM_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(SIM_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
			 "Failed to create SCH pipe (0x%08lX)",
			 iStatus);
        goto SIM_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&CmdPipeId,
    		SIM_CMD_PIPE_DEPTH,
			SIM_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(SIM_CMD_MID, CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(SIM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
				 "CMD Pipe failed to subscribe to SIM_CMD_MID. (0x%08lX)",
				 iStatus);
            goto SIM_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(SIM_PIPE_INIT_ERR_EID, CFE_EVS_ERROR,
			 "Failed to create CMD pipe (0x%08lX)",
			 iStatus);
        goto SIM_InitPipe_Exit_Tag;
    }

SIM_InitPipe_Exit_Tag:
    return iStatus;
}
    

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SIM::InitData()
{
    /* Init housekeeping message. */
    CFE_SB_InitMsg(&HkTlm,
    		SIM_HK_TLM_MID, sizeof(HkTlm), TRUE);

#ifdef SIM_PUBLISH_GPS
    /* Init output message GPS position */
    CFE_SB_InitMsg(&VehicleGps,
            PX4_VEHICLE_GPS_POSITION_MID, sizeof(PX4_VehicleGpsPositionMsg_t), TRUE);
#endif

#ifdef SIM_PUBLISH_ACCEL
    /* Init output message accelerometer */
    CFE_SB_InitMsg(&SensorAccel,
            PX4_SENSOR_ACCEL_MID, sizeof(PX4_SensorAccelMsg_t), TRUE);
#endif

#ifdef SIM_PUBLISH_MAG
    /* Init output message magnetometer */
    CFE_SB_InitMsg(&SensorMag,
            PX4_SENSOR_MAG_MID, sizeof(PX4_SensorMagMsg_t), TRUE);
#endif

#ifdef SIM_PUBLISH_GYRO
    /* Init output message gyroscope */
    CFE_SB_InitMsg(&SensorGyro,
            PX4_SENSOR_GYRO_MID, sizeof(PX4_SensorGyroMsg_t), TRUE);
#endif

#ifdef SIM_PUBLISH_MAG
    /* Init output message baro */
    CFE_SB_InitMsg(&SensorBaro,
            PX4_SENSOR_BARO_MID, sizeof(PX4_SensorBaroMsg_t), TRUE);
#endif

#ifdef SIM_PUBLISH_DISTANCE_SENSOR
    /* Init output messages */
    CFE_SB_InitMsg(&DistanceSensor,
        PX4_DISTANCE_SENSOR_MID, sizeof(PX4_DistanceSensorMsg_t), TRUE);
#endif

#ifdef SIM_PUBLISH_OPTICAL_FLOW
    /* Init output messages */
    CFE_SB_InitMsg(&OpticalFlow,
        PX4_OPTICAL_FLOW_MID, sizeof(PX4_OpticalFlowMsg_t), TRUE);
#endif

}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* SIM initialization                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 SIM::InitApp()
{
    int32  iStatus   = CFE_SUCCESS;
    int8   hasEvents = 0;

    iStatus = InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("SIM - Failed to init events (0x%08lX)\n", iStatus);
        goto SIM_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        goto SIM_InitApp_Exit_Tag;
    }

    InitData();

    iStatus = InitConfigTbl();
    if (iStatus != CFE_SUCCESS)
    {
        goto SIM_InitApp_Exit_Tag;
    }

    iStatus = ListenerInit();
    if (iStatus != CFE_SUCCESS)
    {
        goto SIM_InitApp_Exit_Tag;
    }

SIM_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(SIM_INIT_INF_EID, CFE_EVS_INFORMATION,
                                 "Initialized.  Version %d.%d.%d",
								 SIM_MAJOR_VERSION,
								 SIM_MINOR_VERSION,
								 SIM_REVISION);
    }
    else
    {
        if (hasEvents == 1)
        {
            (void) CFE_ES_WriteToSysLog("SIM - Application failed to initialize\n");
        }
    }

    return iStatus;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 SIM::RcvSchPipeMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(SIM_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(SIM_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
        {
            case SIM_WAKEUP_MID:
                /* TODO:  Do something here. */
                break;

            case SIM_SEND_HK_MID:
                ProcessCmdPipe();
                ReportHousekeeping();
                break;

            default:
                (void) CFE_EVS_SendEvent(SIM_MSGID_ERR_EID, CFE_EVS_ERROR,
                     "Recvd invalid SCH msgId (0x%04X)", MsgId);
        }
    }
    else if (iStatus == CFE_SB_NO_MESSAGE)
    {
        /* TODO: If there's no incoming message, you can do something here, or 
         * nothing.  Note, this section is dead code only if the iBlocking arg
         * is CFE_SB_PEND_FOREVER. */
        iStatus = CFE_SUCCESS;
    }
    else if (iStatus == CFE_SB_TIME_OUT)
    {
        /* TODO: If there's no incoming message within a specified time (via the
         * iBlocking arg, you can do something here, or nothing.  
         * Note, this section is dead code only if the iBlocking arg
         * is CFE_SB_PEND_FOREVER. */
        iStatus = CFE_SUCCESS;
    }
    else
    {
        (void) CFE_EVS_SendEvent(SIM_RCVMSG_ERR_EID, CFE_EVS_ERROR,
			  "SCH pipe read error (0x%08lX).", iStatus);
    }

    return iStatus;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SIM::ProcessCmdPipe()
{
    int32 iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr=NULL;
    CFE_SB_MsgId_t  CmdMsgId;

    /* Process command messages until the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case SIM_CMD_MID:
                    ProcessAppCmds(CmdMsgPtr);
                    break;

                default:
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with this
                     *  pipe, but not handled in this switch-case.) */
                    HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(SIM_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "Recvd invalid CMD msgId (0x%04X)", (unsigned short)CmdMsgId);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            (void) CFE_EVS_SendEvent(SIM_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08lX)", iStatus);
            break;
        }
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process SIM Commands                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SIM::ProcessAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case SIM_NOOP_CC:
                HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(SIM_CMD_NOOP_EID, CFE_EVS_INFORMATION,
					"Recvd NOOP. Version %d.%d.%d.%d",
					SIM_MAJOR_VERSION,
					SIM_MINOR_VERSION,
					SIM_REVISION,
					SIM_MISSION_REV);
                break;

            case SIM_RESET_CC:
                HkTlm.usCmdCnt = 0;
                HkTlm.usCmdErrCnt = 0;
                break;

            default:
                HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(SIM_CC_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid command code (%u)", (unsigned int)uiCmdCode);
                break;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send SIM Housekeeping                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SIM::ReportHousekeeping()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&HkTlm);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Publish Output Data                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Command Length                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean SIM::VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
                           uint16 usExpectedLen)
{
    boolean bResult  = TRUE;
    uint16  usMsgLen = 0;

    if (MsgPtr != NULL)
    {
        usMsgLen = CFE_SB_GetTotalMsgLength(MsgPtr);

        if (usExpectedLen != usMsgLen)
        {
            bResult = FALSE;
            CFE_SB_MsgId_t MsgId = CFE_SB_GetMsgId(MsgPtr);
            uint16 usCmdCode = CFE_SB_GetCmdCode(MsgPtr);

            (void) CFE_EVS_SendEvent(SIM_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            HkTlm.usCmdErrCnt++;
        }
    }

    return bResult;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* SIM Application C style main entry point.                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
extern "C" void SIM_AppMain()
{
    oSIM.AppMain();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* SIM Application C++ style main entry point.                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SIM::AppMain()
{
    /* Register the application with Executive Services */
    uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("SIM - Failed to register the app (0x%08lX)\n", iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(SIM_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(SIM_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(SIM_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(SIM_MAIN_TASK_PERF_ID);
    }
    else
    {
        uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&uiRunStatus) == TRUE)
    {
        RcvSchPipeMsg(SIM_SCH_PIPE_PEND_TIME);

        iStatus = AcquireConfigPointers();
        if(iStatus != CFE_SUCCESS)
        {
            /* We apparently tried to load a new table but failed.  Terminate the application. */
            uiRunStatus = CFE_ES_APP_ERROR;
        }
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(SIM_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(uiRunStatus);
}


int32 SIM::ListenerInit()
{
	int32 Status = CFE_SUCCESS;
	int32 TaskID = 0;
    int   reuseaddr = 1;
	struct sockaddr_in address;

    if((Socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
    	//TODO:  Add event
    	//OS_printf("OSAL:  Failed to create sim socket.  errno: %i\n", errno);
    	Status = -1;
    	goto end_of_function;
    }

    setsockopt(Socket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));

    bzero((char *) &address, sizeof(address));
    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = htonl (INADDR_ANY);
    address.sin_port        = htons(SIM_PORT);

	if ( (connect(Socket, (struct sockaddr *) &address, sizeof(address)) < 0) )
	{
    	//TODO:  Add event
    	//OS_printf("OSAL:  Failed to bind sim socket.  errno: %i\n", errno);
		Status = -1;
		goto end_of_function;
	}

	SIMLIB_SetSocket(Socket);

    Status = OS_MutSemCreate(&MutexID, SIM_MUTEX_NAME, 0);
	if (Status != CFE_SUCCESS)
	{
		goto end_of_function;
	}

	ChildContinueFlag = true;

	Status = CFE_ES_CreateChildTask(&ListenerTaskID,
				SIM_LISTENER_TASK_NAME,
				SIM::ListenerTask_c,
				NULL,
				SIM_LISTENER_TASK_STACK_SIZE,
				SIM_LISTENER_TASK_PRIORITY,
				SIM_LISTENER_TASK_FLAGS);

end_of_function:
    return Status;

}


bool  SIM::ChildContinueExec(void)
{
	bool result;

    OS_MutSemTake(MutexID);
    result = ChildContinueFlag;
    OS_MutSemGive(MutexID);

	return result;
}


void SIM::StopChild(void)
{
    OS_MutSemTake(MutexID);
    ChildContinueFlag = false;
    OS_MutSemGive(MutexID);
}


void SIM::ListenerTask_c(void)
{
	oSIM.ListenerTask();
}


void SIM::SetRates(void)
{
	mavlink_message_t msg;
	uint8 buffer[MAVLINK_MAX_PACKET_LEN];
	mavlink_command_long_t cmdLongMsg;
	uint32 length = 0;

	//send MAV_CMD_SET_MESSAGE_INTERVAL for HIL_STATE_QUATERNION ground truth
	cmdLongMsg.command = MAV_CMD_SET_MESSAGE_INTERVAL;
	cmdLongMsg.param1 = MAVLINK_MSG_ID_HIL_STATE_QUATERNION;
	cmdLongMsg.param2 = 5e3;

	mavlink_msg_command_long_encode(1, 1, &msg, &cmdLongMsg);
	length = mavlink_msg_to_send_buffer(buffer, &msg);

	if(Socket != 0)
	{
		send(Socket, (char *)buffer, length, 0);
	}
}

void SIM::ListenerTask(void)
{
#ifdef SIM_PUBLISH_GYRO
    math::Vector3F gval;
    math::Vector3F gval_integrated;
#endif
#ifdef SIM_PUBLISH_ACCEL
    math::Vector3F aval;
    math::Vector3F aval_integrated;
#endif
	char buffer[SIM_MAX_MESSAGE_SIZE] = {};
	int32 size = SIM_MAX_MESSAGE_SIZE;

	CFE_ES_RegisterChildTask();

    SetRates();

	while(ChildContinueExec())
	{
		size = read(Socket,
						   (char *)buffer,
						   (size_t)size);
		if(size <= 0)
		{
	    	//TODO:  Add event
			OS_TaskDelay(1000);
		}
		else
		{
			mavlink_message_t msg;
			mavlink_status_t status;
			int32 i = 0;
			//char temp;

			for (i = 0; i < size; ++i)
			{
				//temp = buffer[i];
				if (mavlink_parse_char(MAVLINK_COMM_0, buffer[i], &msg, &status))
				{
					switch(msg.msgid)
					{
						case MAVLINK_MSG_ID_HIL_RC_INPUTS_RAW:
						{
							OS_printf("MAVLINK_MSG_ID_HIL_RC_INPUTS_RAW\n");
							mavlink_hil_rc_inputs_raw_t 		decodedMsg;
							mavlink_msg_hil_rc_inputs_raw_decode(&msg, &decodedMsg);
							break;
						}

						case MAVLINK_MSG_ID_HIL_SENSOR:
						{
							mavlink_hil_sensor_t 				decodedMsg;
							mavlink_msg_hil_sensor_decode(&msg, &decodedMsg);

#ifdef SIM_CHECK_UPDATED_FIELDS
							if(decodedMsg.fields_updated & 0x00000007)
							{
#endif
#ifdef SIM_PUBLISH_ACCEL
                                //SensorAccel.Scaling = NEW_SCALE_G_DIGIT * CONSTANTS_ONE_G;
                                SensorAccel.Scaling = 0;
                                SensorAccel.Range_m_s2 = 0;
                                SensorAccel.Timestamp = PX4LIB_GetPX4TimeUs();
                                //SensorAccel.XRaw = (int16)((decodedMsg.xacc / MG2MS2) / SensorAccel.Scaling);
                                //SensorAccel.YRaw = (int16)((decodedMsg.yacc / MG2MS2) / SensorAccel.Scaling);
                                //SensorAccel.ZRaw = (int16)((decodedMsg.zacc / MG2MS2) / SensorAccel.Scaling);
                                SensorAccel.XRaw = 0;
                                SensorAccel.YRaw = 0;
                                SensorAccel.ZRaw = 0;
                                SensorAccel.X = decodedMsg.xacc;
                                SensorAccel.Y = decodedMsg.yacc;
                                SensorAccel.Z = decodedMsg.zacc;
                                /* Accel Integrate */
                                aval[0] = SensorAccel.X;
                                aval[1] = SensorAccel.Y;
                                aval[2] = SensorAccel.Z;
                                aval_integrated[0] = 0.0f;
                                aval_integrated[1] = 0.0f;
                                aval_integrated[2] = 0.0f;
                                _accel_int.put(SensorAccel.Timestamp, aval, aval_integrated, SensorAccel.IntegralDt);
                                SensorAccel.XIntegral = aval_integrated[0];
                                SensorAccel.YIntegral = aval_integrated[1];
                                SensorAccel.ZIntegral = aval_integrated[2];
                                /* fake device id */
                                SensorAccel.DeviceID = 6789478;
                                CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&SensorAccel);
                                CFE_SB_SendMsg((CFE_SB_Msg_t*)&SensorAccel);
#else

								SIMLIB_SetAccel(decodedMsg.xacc, decodedMsg.yacc, decodedMsg.zacc);
#endif
#ifdef SIM_CHECK_UPDATED_FIELDS
							}
#endif

#ifdef SIM_CHECK_UPDATED_FIELDS
							if(decodedMsg.fields_updated & 0x00000038)
							{
#endif
#ifdef SIM_PUBLISH_GYRO
                                SensorGyro.Scaling = 0;
                                SensorGyro.Range = 0;
                                SensorGyro.Timestamp = PX4LIB_GetPX4TimeUs();
                                //SensorGyro.XRaw = (int16)(decodedMsg.xgyro * 1000.0f);
                                //SensorGyro.YRaw = (int16)(decodedMsg.ygyro * 1000.0f);
                                //SensorGyro.ZRaw = (int16)(decodedMsg.zgyro * 1000.0f);
                                SensorGyro.XRaw = 0;
                                SensorGyro.YRaw = 0;
                                SensorGyro.ZRaw = 0;
                                SensorGyro.X = decodedMsg.xgyro;
                                SensorGyro.Y = decodedMsg.ygyro;
                                SensorGyro.Z = decodedMsg.zgyro;
                                gval[0] = SensorGyro.X;
                                gval[1] = SensorGyro.Y;
                                gval[2] = SensorGyro.Z;
                                gval_integrated[0] = 0.0f;
                                gval_integrated[1] = 0.0f;
                                gval_integrated[2] = 0.0f;
                                _gyro_int.put(SensorGyro.Timestamp, gval, gval_integrated, SensorGyro.IntegralDt);
                                SensorGyro.XIntegral = gval_integrated[0];
                                SensorGyro.YIntegral = gval_integrated[1];
                                SensorGyro.ZIntegral = gval_integrated[2];
                                /* fake device ID */
                                SensorGyro.DeviceID = 3467548;
                                CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&SensorGyro);
                                CFE_SB_SendMsg((CFE_SB_Msg_t*)&SensorGyro);
#else
								SIMLIB_SetGyro(decodedMsg.xgyro, decodedMsg.ygyro, decodedMsg.zgyro);
#endif
#ifdef SIM_CHECK_UPDATED_FIELDS
							}
#endif

#ifdef SIM_CHECK_UPDATED_FIELDS
							if(decodedMsg.fields_updated & 0x000001c0)
							{
#endif
#ifdef SIM_PUBLISH_MAG
                                SensorMag.Timestamp = PX4LIB_GetPX4TimeUs();
                                SensorMag.Scaling = 0;
                                SensorMag.Range = 0;
                                //SensorMag.XRaw = (int16)((decodedMsg.xmag * 1000.0f) / NEW_SCALE_GA_DIGIT);
                                //SensorMag.YRaw = (int16)((decodedMsg.ymag * 1000.0f) / NEW_SCALE_GA_DIGIT);
                                //SensorMag.ZRaw = (int16)((decodedMsg.zmag * 1000.0f) / NEW_SCALE_GA_DIGIT);
                                //SensorMag.XRaw = (int16)((decodedMsg.xmag * 1000.0f));
                                //SensorMag.YRaw = (int16)((decodedMsg.ymag * 1000.0f));
                                //SensorMag.ZRaw = (int16)((decodedMsg.zmag * 1000.0f));
                                SensorMag.XRaw = 0;
                                SensorMag.YRaw = 0;
                                SensorMag.ZRaw = 0;
                                SensorMag.X = decodedMsg.xmag;
                                SensorMag.Y = decodedMsg.ymag;
                                SensorMag.Z = decodedMsg.zmag;
                                CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&SensorMag);
                                CFE_SB_SendMsg((CFE_SB_Msg_t*)&SensorMag);
#else
								SIMLIB_SetMag(decodedMsg.xmag, decodedMsg.ymag, decodedMsg.zmag);

#endif

#ifdef SIM_CHECK_UPDATED_FIELDS
							}
#endif


#ifdef SIM_CHECK_UPDATED_FIELDS
							if(decodedMsg.fields_updated & 0x00000400)
							{
#endif
#ifdef SIM_PUBLISH_BARO
                                SensorBaro.Timestamp = PX4LIB_GetPX4TimeUs();
                                SensorBaro.Pressure = decodedMsg.abs_pressure;
#else
								//SIMLIB_SetPressure(decodedMsg.abs_pressure, decodedMsg.diff_pressure);
#endif
#ifdef SIM_CHECK_UPDATED_FIELDS
						    }
#endif

#ifdef SIM_CHECK_UPDATED_FIELDS
							if(decodedMsg.fields_updated & 0x00001a00)
                            {
#endif
#ifdef SIM_PUBLISH_BARO       
                                SensorBaro.Timestamp = PX4LIB_GetPX4TimeUs();
                                SensorBaro.Altitude = decodedMsg.pressure_alt;
                                /* fake device ID */
                                //SensorBaro.DeviceID = 478459;
                                CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&SensorBaro);
                                CFE_SB_SendMsg((CFE_SB_Msg_t*)&SensorBaro);
#else
								SIMLIB_SetPressureAltitude(decodedMsg.pressure_alt);

#endif
#ifdef SIM_CHECK_UPDATED_FIELDS
						    }
#endif

                            /* TODO sitl gazebo mavlink plugin needs to be updated
                             * to set bit 12, TRUE set for now... */
#ifdef SIM_CHECK_UPDATED_FIELDS
							if(decodedMsg.fields_updated & 0x00000800)
							{
#endif
#ifdef SIM_PUBLISH_BARO
                                SensorBaro.Temperature = decodedMsg.temperature;
#endif
#ifdef SIM_PUBLISH_ACCEL
                                SensorAccel.Temperature = decodedMsg.temperature;
                                SensorAccel.TemperatureRaw = (int16)((SensorAccel.Temperature - 21.0f) * 333.87f);
#endif
#ifdef SIM_PUBLISH_MAG
                                SensorMag.Temperature = decodedMsg.temperature;
#endif
#ifdef SIM_PUBLISH_GYRO
                                SensorGyro.Temperature = decodedMsg.temperature;
                                SensorGyro.TemperatureRaw = (int16)((SensorGyro.Temperature -21.0f) * 333.87f);
#endif
								SIMLIB_SetTemp(decodedMsg.temperature);
#ifdef SIM_CHECK_UPDATED_FIELDS
							}
#endif

							break;
						}

						case MAVLINK_MSG_ID_HIL_GPS:
						{
							mavlink_hil_gps_t 					decodedMsg;
							mavlink_msg_hil_gps_decode(&msg, &decodedMsg);
#ifdef SIM_PUBLISH_GPS
                            VehicleGps.Timestamp      = PX4LIB_GetPX4TimeUs();
                            VehicleGps.Lat            = decodedMsg.lat;
                            VehicleGps.Lon            = decodedMsg.lon;
                            VehicleGps.Alt            = decodedMsg.alt;
                            VehicleGps.EpH            = (float)decodedMsg.eph * 1e-2f;
                            VehicleGps.EpV            = (float)decodedMsg.epv * 1e-2f;
                            VehicleGps.Vel_m_s        = (float)decodedMsg.vel / 100.0f;
                            VehicleGps.Vel_n_m_s      = (float)decodedMsg.vn / 100.0f;
                            VehicleGps.Vel_e_m_s      = (float)decodedMsg.ve / 100.0f;
                            VehicleGps.Vel_d_m_s      = (float)decodedMsg.vd / 100.0f;
                            VehicleGps.COG            = (float)decodedMsg.cog * 3.1415f / (100.0f * 180.0f);
                            VehicleGps.FixType        = decodedMsg.fix_type;
                            VehicleGps.SatellitesUsed = decodedMsg.satellites_visible;
                            CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&VehicleGps);
                            CFE_SB_SendMsg((CFE_SB_Msg_t*)&VehicleGps);
#else
							SIMLIB_SetGPS(
								(PX4_GpsFixType_t)decodedMsg.fix_type,
								decodedMsg.lat,
								decodedMsg.lon,
								decodedMsg.alt,
								decodedMsg.eph,
								decodedMsg.epv,
								decodedMsg.vel,
								decodedMsg.vn,
								decodedMsg.ve,
								decodedMsg.vd,
								decodedMsg.cog,
								decodedMsg.satellites_visible);
#endif
							break;
						}

						case MAVLINK_MSG_ID_HIL_OPTICAL_FLOW:
						{
							mavlink_hil_optical_flow_t 			decodedMsg;
							mavlink_msg_hil_optical_flow_decode(&msg, &decodedMsg);
							
#ifdef SIM_PUBLISH_OPTICAL_FLOW
                        	OpticalFlow.Timestamp                       = PX4LIB_GetPX4TimeUs();
	                        OpticalFlow.PixelFlowXIntegral              = decodedMsg.integrated_x;
	                        OpticalFlow.PixelFlowYIntegral              = decodedMsg.integrated_y; 
	                        OpticalFlow.GyroXRateIntegral               = decodedMsg.integrated_xgyro;
	                        OpticalFlow.GyroYRateIntegral               = decodedMsg.integrated_ygyro;
	                        OpticalFlow.GyroZRateIntegral               = decodedMsg.integrated_zgyro;
	                        OpticalFlow.GroundDistance                  = decodedMsg.distance;
	                        OpticalFlow.IntegrationTimespan             = decodedMsg.integration_time_us;
	                        OpticalFlow.TimeSinceLastSonarUpdate        = decodedMsg.time_delta_distance_us;
	                        OpticalFlow.GyroTemperature                 = decodedMsg.temperature;
	                        OpticalFlow.SensorID                        = decodedMsg.sensor_id;
	                        OpticalFlow.Quality                         = decodedMsg.quality;
	                        
	                        CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&OpticalFlow);
                            CFE_SB_SendMsg((CFE_SB_Msg_t*)&OpticalFlow);
#endif
							break;
						}

						case MAVLINK_MSG_ID_HIL_STATE_QUATERNION:
						{
							mavlink_hil_state_quaternion_t 		decodedMsg;
							mavlink_msg_hil_state_quaternion_decode(&msg, &decodedMsg);
							break;
						}

						case MAVLINK_MSG_ID_VISION_POSITION_ESTIMATE:
						{
							mavlink_vision_position_estimate_t 	decodedMsg;
							mavlink_msg_vision_position_estimate_decode(&msg, &decodedMsg);
							break;
						}

						case MAVLINK_MSG_ID_DISTANCE_SENSOR:
						{
							PX4_DistanceSensorType_t sensorType;
							PX4_SensorOrientation_t  sensorOrientation;
							mavlink_distance_sensor_t 	decodedMsg;
							mavlink_msg_distance_sensor_decode(&msg, &decodedMsg);

							sensorType = (PX4_DistanceSensorType_t) decodedMsg.type;
							sensorOrientation = (PX4_SensorOrientation_t) decodedMsg.orientation;
#ifdef SIM_PUBLISH_DISTANCE_SENSOR
                            DistanceSensor.Timestamp = PX4LIB_GetPX4TimeUs();
                            DistanceSensor.MinDistance = decodedMsg.min_distance / 100.0f;
                            DistanceSensor.MaxDistance = decodedMsg.max_distance / 100.0f;
                            DistanceSensor.CurrentDistance = decodedMsg.current_distance / 100.0f;
                            DistanceSensor.Type = sensorType;
                            DistanceSensor.ID = decodedMsg.id;
                            DistanceSensor.Orientation = sensorOrientation;
                            DistanceSensor.Covariance = decodedMsg.covariance / 100.0f;
                            CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&DistanceSensor);
                            CFE_SB_SendMsg((CFE_SB_Msg_t*)&DistanceSensor);
#else
							SIMLIB_SetDistanceSensor(
									decodedMsg.min_distance,
									decodedMsg.max_distance,
									decodedMsg.current_distance,
									sensorType,
									decodedMsg.id,
									sensorOrientation,
									decodedMsg.covariance);
#endif
							break;
						}

						case MAVLINK_MSG_ID_HEARTBEAT:
							SIMLIB_SendHeartbeat();
							break;

						case MAVLINK_MSG_ID_SYSTEM_TIME:
							break;

						case MAVLINK_MSG_ID_SET_MODE:
							break;

						default:
							OS_printf("Received packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\n", msg.sysid, msg.compid, msg.len, msg.msgid);
							break;
					}
				}
			}
		}
	}

	CFE_ES_ExitChildTask();
}


/************************/
/*  End of File Comment */
/************************/
