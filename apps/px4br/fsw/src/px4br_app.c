/*==============================================================================
 Copyright (c) 2015, Windhover Labs
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 * Neither the name of TlmOut nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef __cplusplus
extern "C"
{
#endif

/*
 ** Include Files
 */

#include <fcntl.h>
#include "cfe.h"
#include "px4br_mission_cfg.h"
#include "px4br_app.h"
#include "px4br_perfids.h"
#include "px4br_msg.h"
#include "px4br_platform_cfg.h"
#include "px4br_events.h"
#include "px4br_version.h"
#include <termio.h>
#include <stdlib.h>
#include <netinet/tcp.h>
#include <px4_msgs.h>
#include "px4br_routes.h"

/*
 ** Local Variables
 */

PX4BR_AppData_t PX4BR_AppData;

extern PX4BR_Route_t PX4BR_Route[];


/*
 ** Local Function Prototypes
 */

void  PX4BR_AppMain(void);
int32 PX4BR_RecvMsg(void);
void  PX4BR_Cleanup(void);
int32 PX4BR_AppInit(void);
void  PX4BR_InitEvents(void);
int32 PX4BR_GetPX4FileData(void);
int32 PX4BR_ParseFileEntry(char *FileEntry, uint32 LineNum);
int32 PX4BR_InitPeers(void);
PX4BR_Peer_t* PX4BR_GetInitializingPeer(void);
int32 PX4BR_SetInitializingPeer(PX4BR_Peer_t *inPeer);
boolean PX4BR_IsPeerConnected(PX4BR_Peer_t* peer);
int32 PX4BR_InitPeerSocket(PX4BR_Peer_t *inPeer);
int32 PX4BR_InitSubscriptions(void);
void PX4BR_RouteMessageToSB(const char *inMsgName, const char *inMsgContent, uint32 inMsgContentSize);
void PX4BR_RouteMessageToPX4(CFE_SB_MsgPtr_t sbMsg);
void PX4BR_SendHk(void);
int32 PX4BR_GetRouteByMsgName(const char *inMsgName, PX4BR_Route_t **inOutRoute);
int32 PX4BR_GetRouteByMsgID(CFE_SB_MsgId_t inMsgID, PX4BR_Route_t **inOutRoute);

/*
 * Local Variables
 */

/* Event Filter Table */

CFE_EVS_BinFilter_t PX4BR_EventFilters[] = {
    { PX4BR_INIT_INF_EID, CFE_EVS_NO_FILTER },
    { PX4BR_PIPE_ERR_EID, CFE_EVS_NO_FILTER },
    { PX4BR_RCVMSG_ERR_EID, CFE_EVS_NO_FILTER },
    { PX4BR_MSGID_ERR_EID, CFE_EVS_NO_FILTER },
    { PX4BR_FNCODE_ERR_EID, CFE_EVS_NO_FILTER },
    { PX4BR_CMD_ARG_INVALID_EID, CFE_EVS_NO_FILTER },
    { PX4BR_NOOP_INF_EID, CFE_EVS_NO_FILTER },
    { PX4BR_ENGAGE_TEST_MODE_EID, CFE_EVS_NO_FILTER },
    { PX4BR_HK_CREATE_CHDTASK_ERR_EID, CFE_EVS_NO_FILTER },
    { PX4BR_CMD_INVALID_MODE_EID, CFE_EVS_NO_FILTER },
    { PX4BR_CMD_RUN_FOR_TIME_EID, CFE_EVS_NO_FILTER },
    { PX4BR_RUNTIME_QUEUE_ACQ_ERR_EID, CFE_EVS_NO_FILTER },
    { PX4BR_STEP_FRAME_INF_EID, CFE_EVS_NO_FILTER },
    { PX4BR_HK_TASK_INIT_FAILED_EID, CFE_EVS_NO_FILTER }
};

/*
 * Function Definitions
 */

void PX4BR_AppMain()
{
    int32 Status = CFE_SUCCESS;

    CFE_ES_PerfLogEntry(PX4BR_MAIN_TASK_PERF_ID);

    /* Initialize the ETA app*/
    Status = PX4BR_AppInit();
    if (Status != CFE_SUCCESS)
    {
        PX4BR_AppData.RunStatus = CFE_ES_APP_ERROR;
    }

    /* Application Main Loop */
    while (CFE_ES_RunLoop(&PX4BR_AppData.RunStatus) == TRUE)
    {
        /* Call RecvMsg to pend on sch wakeup msg */
        PX4BR_RecvMsg();
    }

    /*  Exit the Application */
    CFE_ES_ExitApp(PX4BR_AppData.RunStatus);
}


int32 PX4BR_RecvMsg()
{
    int32 Status = CFE_SUCCESS;
    CFE_SB_MsgPtr_t 	msgPtr = 0;

    /*  Performance Log Exit Stamp */
    CFE_ES_PerfLogExit(PX4BR_MAIN_TASK_PERF_ID);

    /* Wait for SCH software bus Wakeup or HK message */
    Status = CFE_SB_RcvMsg(&msgPtr, PX4BR_AppData.SchPipe,
            CFE_SB_PEND_FOREVER);
    /* Message received now process message */
    if (Status == CFE_SUCCESS)
    {
        CFE_SB_MsgId_t msgId;

        /* Read the message ID from the message. */
        msgId = CFE_SB_GetMsgId(msgPtr);
        switch (msgId)
        {
            case PX4BR_SEND_HK_MID:
            {
                /* This is a command message. */
                PX4BR_SendHk();
                break;
            }

            case PX4BR_CMD_MID:
            {
                /* This is a command message. */
                PX4BR_CmdExec(msgPtr);
                break;
            }

            case PX4BR_PROC_CMD_MID:
            {
            	break;
            }

            default:
            {
                /* Unknown message. This shouldn't happen. */
                CFE_EVS_SendEvent(PX4BR_MSGID_ERR_EID, CFE_EVS_ERROR,
                        "Msg Pipe Invalid Msg ID Rcvd 0x%x", msgId);
                break;
            }
        }
    }
    else
    {
        CFE_EVS_SendEvent(PX4BR_RCVMSG_ERR_EID, CFE_EVS_ERROR,
                "CFE_SB_RcvMsg failed.  %i", Status);
    }

    /* Performance Log Exit Stamp */
    CFE_ES_PerfLogEntry(PX4BR_MAIN_TASK_PERF_ID);

    return Status;

}



void PX4BR_Cleanup()
{
}



int32 PX4BR_AppInit()
{
    int32 Status = CFE_SUCCESS;

    PX4BR_AppData.RunStatus = CFE_ES_APP_RUN;

    /* Register the ETA app */
    CFE_ES_RegisterApp();

    /* Register event filter table */
    Status = CFE_EVS_Register(PX4BR_EventFilters,
            sizeof(PX4BR_EventFilters)/sizeof(CFE_EVS_BinFilter_t),
            CFE_EVS_BINARY_FILTER);
    if (Status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("PX4BR: Error Registering Events");
        goto end_of_function;
    }

    /* Initialize housekeeping packet (clear user data area) */
    CFE_SB_InitMsg(
    		&PX4BR_AppData.HkMsg,
			PX4BR_HK_TLM_MID,
			sizeof(PX4BR_AppData.HkMsg),
			TRUE);

    /* Create scheduler pipe */
    Status = CFE_SB_CreatePipe(&PX4BR_AppData.SchPipe, PX4BR_SCH_PIPE_DEPTH,
            PX4BR_SCH_PIPE_NAME);
    if (Status == CFE_SUCCESS)
    {
        CFE_SB_Subscribe(PX4BR_SEND_HK_MID, PX4BR_AppData.SchPipe);
        CFE_SB_Subscribe(PX4BR_PROC_CMD_MID, PX4BR_AppData.SchPipe);
        CFE_SB_Subscribe(PX4BR_CMD_MID, PX4BR_AppData.SchPipe);
    }
    else
    {
        CFE_EVS_SendEvent(PX4BR_PIPE_ERR_EID, CFE_EVS_ERROR,
                "Can't create sch pipe status %i", Status);
        goto end_of_function;
    }

    /* Create command pipe */
    Status = CFE_SB_CreatePipe(&PX4BR_AppData.CmdPipe, PX4BR_CMD_PIPE_DEPTH,
            PX4BR_CMD_PIPE_NAME);
    if (Status == CFE_SUCCESS)
    {
    }
    else
    {
        CFE_EVS_SendEvent(PX4BR_PIPE_ERR_EID, CFE_EVS_ERROR,
                "Can't create cmd pipe status %i", Status);
        goto end_of_function;
    }

    /* Create router pipe */
    Status = CFE_SB_CreatePipe(&PX4BR_AppData.RouterPipe, PX4BR_ROUTER_PIPE_DEPTH,
            PX4BR_ROUTER_PIPE_NAME);
    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(PX4BR_PIPE_ERR_EID, CFE_EVS_ERROR,
                "Can't create router pipe status %i", Status);
        goto end_of_function;
    }

    /* Install the delete handler */
    OS_TaskInstallDeleteHandler((void *) (&PX4BR_Cleanup));

    /* Initialize internal data. */
    PX4BR_AppData.HkMsg.RuntimeMode = PX4BR_RUNTIME_MODE_NORMAL;

    /* Initialize HK child task. */
    //Status = PX4BR_InitHKTask();
    //if (Status == CFE_SUCCESS)
    //{
    //    CFE_EVS_SendEvent(PX4BR_HK_TASK_INIT_FAILED_EID, CFE_EVS_ERROR,
    //            "Can't initialize HK task.  status 0x%08x", Status);
    //    goto end_of_function;
    //}

    Status = PX4BR_GetPX4FileData();
    if(Status != PX4BR_OK)
    {
        CFE_EVS_SendEvent(PX4BR_FILE_ERR_EID, CFE_EVS_ERROR,
                "PX4BR APP Will Terminate, Peer File Not Found or Data Invalid!");
        goto end_of_function;
    }/* end if */

    Status = PX4BR_InitPeers();
    if(Status != PX4BR_OK)
    {
        CFE_EVS_SendEvent(PX4BR_FILE_ERR_EID, CFE_EVS_ERROR,
                "InitPeer failed.");
        goto end_of_function;
    }/* end if */

    Status = CFE_ES_GetAppID(&PX4BR_AppData.AppID);
    if(Status != PX4BR_OK)
    {
        CFE_EVS_SendEvent(PX4BR_FILE_ERR_EID, CFE_EVS_ERROR,
                "Failed to get App ID.");
        goto end_of_function;
    }/* end if */

    Status = CFE_ES_GetAppName(PX4BR_AppData.AppName, PX4BR_AppData.AppID, sizeof(PX4BR_AppData.AppName));
    if(Status != PX4BR_OK)
    {
        CFE_EVS_SendEvent(PX4BR_FILE_ERR_EID, CFE_EVS_ERROR,
                "CFE_ES_GetAppName failed.");
        goto end_of_function;
    }/* end if */

    Status = PX4BR_InitSubscriptions();
    if(Status != PX4BR_OK)
    {
        CFE_EVS_SendEvent(PX4BR_FILE_ERR_EID, CFE_EVS_ERROR,
                "Init Subscriptions failed.");
        goto end_of_function;
    }/* end if */

    /* Tell the world we're alive and ready for business. */
    CFE_EVS_SendEvent(PX4BR_INIT_INF_EID, CFE_EVS_INFORMATION,
            "PX4 Bridge Initialized. Version %d.%d.%d.%d",
            PX4BR_MAJOR_VERSION,
            PX4BR_MINOR_VERSION,
            PX4BR_REVISION,
            PX4BR_MISSION_REV);

end_of_function:
    return Status;

}



void PX4BR_SendHk(void)
{
	CFE_SB_TimeStampMsg((CFE_SB_MsgPtr_t)&PX4BR_AppData.HkMsg);
	CFE_SB_SendMsg((CFE_SB_MsgPtr_t)&PX4BR_AppData.HkMsg);
}



void PX4BR_PeerListenerTaskMain(void)
{
    int32 Status = CFE_SUCCESS;

    /* Register the child task. */
    Status = CFE_ES_RegisterChildTask();
    if (Status != CFE_SUCCESS)
    {
        /* Task registration failed.  Raise an event and terminate the
         * thread. */
        CFE_EVS_SendEvent(PX4BR_HK_CREATE_CHDTASK_ERR_EID,
                CFE_EVS_ERROR,
                "Listener child task failed.  CFE_ES_RegisterChildTask returned: 0x%08X",
                Status);
    }
    else
    {
    	/* Get the data struct that contains everything we need to know to do
    	 * our job.
    	 */
    	PX4BR_Peer_t *peer = PX4BR_GetInitializingPeer();
    	if(peer == 0)
    	{
    		/* This should never happen, so something went really bad.  Kill
    		 * the application so we can clear memory and restart.
    		 */
    		/* TODO - Add event. */
    		PX4BR_AppData.RunStatus = CFE_ES_APP_ERROR;
    	}
    	else
    	{
            /* This is the main loop.  Keep looping until the main application
             * signals that its going to terminate.
             */
    		while(PX4BR_AppData.RunStatus == CFE_ES_APP_RUN)
    		{
            	/* Wait until we're connected. */
    			if(PX4BR_IsPeerConnected(peer) == FALSE)
    			{
    				/* We aren't connected yet.  Just sleep for a second and
    				 * wait for it to connect.
    				 */
    				OS_TaskDelay(1000);
    			}
    			else
    			{
    				/* We must be connected.  Go do that voodoo that you do. */

    				int n = 0;
    				unsigned char sizeField[2];
    				uint16 msgSize = 0;

    				/* First read the 2 byte size */
    				n = read(peer->Socket, sizeField, PX4BR_SIZE_FIELD_LENGTH);
    				if(n == 0)
    				{
    					/* TODO - Add event */
    					OS_printf("PX4BR:  Socket disconnected.\n");
    					peer->IsConnected = FALSE;
    					close(peer->Socket);
    					PX4BR_InitPeerSocket(peer);
    				}
    				else if(n < 0)
    				{
    					/* TODO - Add event */
    					OS_printf("PX4BR:  Socket read failed.  errno=%d '%s'\n", errno, strerror(errno));
    					peer->IsConnected = FALSE;
    					close(peer->Socket);
    					PX4BR_InitPeerSocket(peer);
    				}
    				else
    				{
    					char msgName[PX4BR_NAME_FIELD_LENGTH];

    					/* Correct the size field ABI */
    					msgSize = (sizeField[0] << 8);
    					msgSize |= (sizeField[1]);

    					/* Now read the 50 byte message name */
    					n = read(peer->Socket, msgName, PX4BR_NAME_FIELD_LENGTH);
        				if(n == 0)
        				{
        					/* TODO - Add event */
        					OS_printf("PX4BR:  Socket disconnected.\n");
        					peer->IsConnected = FALSE;
        					close(peer->Socket);
        					PX4BR_InitPeerSocket(peer);
        				}
        				else if(n < 0)
        				{
        					/* TODO - Add event */
        					OS_printf("PX4BR:  Socket read failed.  errno=%d '%s'\n", errno, strerror(errno));
        					peer->IsConnected = FALSE;
        					close(peer->Socket);
        					PX4BR_InitPeerSocket(peer);
        				}
        				else
        				{
    						char msgContent[PX4BR_MAX_MSG_SIZE];
    						uint32 rcvdBytes = 0;

    						/* Now read the n byte message content */
    						do
    						{
    							n = read(peer->Socket, &msgContent[rcvdBytes], msgSize-rcvdBytes);
    							rcvdBytes += n;
    							if(n <= 0)
    								break;
    						} while((n > 0) && (rcvdBytes < msgSize));

            				if(n == 0)
            				{
            					/* TODO - Add event */
            					OS_printf("PX4BR:  Socket disconnected.\n");
            					peer->IsConnected = FALSE;
            					close(peer->Socket);
            					PX4BR_InitPeerSocket(peer);
            				}
            				else if(n < 0)
            				{
            					/* TODO - Add event */
            					OS_printf("PX4BR:  Socket read failed.  errno=%d '%s'\n", errno, strerror(errno));
            					peer->IsConnected = FALSE;
            					close(peer->Socket);
            					PX4BR_InitPeerSocket(peer);
            				}
            				else
            				{
            					PX4BR_RouteMessageToSB(msgName, msgContent, msgSize);
            				}
    					}
    				}
    			}
        	}
    	}
    }
}



void PX4BR_PeerDataOutTaskMain(void)
{
    int32 status = CFE_SUCCESS;
    PX4BR_Peer_t *peer = 0;

    /* Register the child task. */
    status = CFE_ES_RegisterChildTask();
    if(status != CFE_SUCCESS)
    {
        /* Task registration failed.  Raise an event and terminate the
         * thread. */
        CFE_EVS_SendEvent(PX4BR_HK_CREATE_CHDTASK_ERR_EID,
                CFE_EVS_ERROR,
                "DataOut child task failed.  CFE_ES_RegisterChildTask returned: 0x%08X",
                status);
        PX4BR_AppData.RunStatus = CFE_ES_APP_ERROR;
    }
    else
    {
    	/* Get the data struct that contains everything we need to know to do
    	 * our job.
    	 */
    	peer = PX4BR_GetInitializingPeer();
    	if(peer == 0)
    	{
    		/* This should never happen, so something went really bad.  Kill
    		 * the application so we can clear memory and restart.
    		 */
    		/* TODO - Add event. */
    		PX4BR_AppData.RunStatus = CFE_ES_APP_ERROR;
    	}
    }

    /* This is the main loop.  Keep looping until the main application
     * signals that its going to terminate.
     */
    while(PX4BR_AppData.RunStatus == CFE_ES_APP_RUN)
    {
    	int rc = 0;

		/* We're going to let this be the main thread controlling the state of
		 * the socket.  So, lets go ahead and connect that socket.  Wait until
		 * the socket is connected to proceed.  None of the other threads will
		 * do anything meaningful until the socket is connected.
		 */
		while((PX4BR_IsPeerConnected(peer) == FALSE) && (PX4BR_AppData.RunStatus == CFE_ES_APP_RUN))
		{
			errno = 0;
			rc = connect(peer->Socket,(struct sockaddr*)&peer->ServAddr, sizeof(peer->ServAddr));
			if(errno == ECONNREFUSED)
			{
				/* The other side isn't up yet.  Wait. */
				OS_printf("PX4BR:  Waiting to connect.\n");
			}
			else if(rc < 0)
			{
				/* TODO - Add event. */
				OS_printf("PX4BR:  Failed to connect socket. errno=%u '%s'\n", errno, strerror(errno));
				OS_printf("%u\n", errno);
			}
			else
			{
				OS_printf("PX4BR:  Socket connected.\n");
				peer->IsConnected = TRUE;
			}
			OS_TaskDelay(1000);
		}

		if(PX4BR_IsPeerConnected(peer))
		{
		    int32 Status = CFE_SUCCESS;
		    CFE_SB_MsgPtr_t 	msgPtr = 0;

		    /* Wait for a message to route. */
		    status = CFE_SB_RcvMsg(&msgPtr, PX4BR_AppData.RouterPipe,
		            PX4BR_ROUTER_PIPE_TIMEOUT);
		    /* Message received now process message */
		    if (status == CFE_SUCCESS)
		    {
		    	CFE_SB_SenderId_t *sender;
		    	status = CFE_SB_GetLastSenderId(&sender, PX4BR_AppData.RouterPipe);
		    	if(status == CFE_SUCCESS)
		    	{
		    	    if(strncmp(sender->AppName, PX4BR_AppData.AppName, strlen(PX4BR_AppData.AppName)) != 0)
		    	    {
						CFE_SB_MsgId_t msgID;
						PX4BR_Route_t *route = 0;

						/* Read the message ID from the message. */
						msgID = CFE_SB_GetMsgId(msgPtr);
						PX4BR_GetRouteByMsgID(msgID, &route);
						if(route == 0)
						{
							OS_printf("PX4BR:  Received SB message with no route (2).  '0x%04x'\n", msgID);
						}
						else
						{
							PX4BR_RouteMessageToPX4(msgPtr);
						}
		    	    }
		    	}
		    }
		    else if(status == CFE_SB_TIME_OUT)
		    {
		    	/* TODO:  Add keepalive */
		    }
		    else
		    {
		        CFE_EVS_SendEvent(PX4BR_RCVMSG_ERR_EID, CFE_EVS_ERROR,
		                "CFE_SB_RcvMsg failed.  %i", Status);
		    }
			//int n = 0;
			//char buffer[PX4BR_MAX_MSG_SIZE] = "Hello PX4, from CFS.";

			//n = write(peer->Socket, buffer, strnlen(buffer,PX4BR_MAX_MSG_SIZE));
			//if(n < 0)
			//{
			//	/* TODO - Add event */
			//	OS_printf("PX4BR:  Socket write failed.  errno=%d '%s'", errno, strerror(errno));
			//    peer->IsConnected = FALSE;
			//    close(peer->Socket);
			//	PX4BR_InitPeerSocket(peer);
			//}
			//OS_TaskDelay(1000);
		}
	}
}



boolean PX4BR_IsPeerConnected(PX4BR_Peer_t* peer)
{
	return peer->IsConnected;
}



PX4BR_Peer_t* PX4BR_GetInitializingPeer()
{
	PX4BR_Peer_t*  peer = 0;

	if(PX4BR_AppData.InitializingPeerCount > 0)
	{
		PX4BR_AppData.InitializingPeerCount--;
		peer = PX4BR_AppData.InitializingPeer;
	}

	return peer;
}



int32 PX4BR_GetPX4FileData(void)
{
    static char PX4BR_PeerData[PX4BR_PEER_FILE_LINE_SIZE]; /* A buffer for a line in a file */
    int BuffLen; /* Length of the current buffer */
    int PeerFile = 0;
    char c;
    uint32 FileOpened = FALSE;
    uint32 LineNum = 0;

    /* First check for the file in RAM */
    PeerFile = OS_open(PX4BR_VOL_PEER_FILENAME, O_RDONLY, 0);
    if (PeerFile != OS_ERROR)
    {
        CFE_EVS_SendEvent(PX4BR_FILE_OPENED_EID, CFE_EVS_INFORMATION,
                "%s:Opened SBN Peer Data file %s", CFE_CPU_NAME, PX4BR_VOL_PEER_FILENAME);
        FileOpened = TRUE;
    }
    else
    {
        CFE_EVS_SendEvent(PX4BR_VOL_FAILED_EID, CFE_EVS_INFORMATION,
                "%s:Peer file %s failed to open", CFE_CPU_NAME,
				PX4BR_VOL_PEER_FILENAME);
        FileOpened = FALSE;
    }

    /* If ram file failed to open, try to open non vol file */
    if (FileOpened == FALSE)
    {
        PeerFile = OS_open(PX4BR_NONVOL_PEER_FILENAME, O_RDONLY, 0);

        if (PeerFile != OS_ERROR)
        {
            CFE_EVS_SendEvent(PX4BR_FILE_OPENED_EID, CFE_EVS_INFORMATION,
                    "%s:Opened SBN Peer Data file %s", CFE_CPU_NAME, PX4BR_NONVOL_PEER_FILENAME);
            FileOpened = TRUE;
        }
        else
        {
            CFE_EVS_SendEvent(PX4BR_NONVOL_FAILED_EID, CFE_EVS_ERROR,
                    "%s:Peer file %s failed to open", CFE_CPU_NAME,
					PX4BR_NONVOL_PEER_FILENAME);
            FileOpened = FALSE;
        }

    }/* end if */

    /*
     ** If no file was opened, SBN must terminate
     */
    if (FileOpened == FALSE)
    {
        return PX4BR_ERROR;
    }/* end if */

    memset(PX4BR_PeerData, 0x0, PX4BR_PEER_FILE_LINE_SIZE);
    BuffLen = 0;

    /*
     ** Parse the lines from the file
     */
    while (1)
    {
        OS_read(PeerFile, &c, 1);

        if (c == '!')
            break;

        if (c == '\n' || c == ' ' || c == '\t')
        {
            /*
             ** Skip all white space in the file
             */
            ;
        }
        else if (c == ',')
        {
            /*
             ** replace the field delimiter with a space
             ** This is used for the sscanf string parsing
             */
        	PX4BR_PeerData[BuffLen] = ' ';
            if (BuffLen < (PX4BR_PEER_FILE_LINE_SIZE - 1))
                BuffLen++;
        }
        else if (c != ';')
        {
            /*
             ** Regular data gets copied in
             */
        	PX4BR_PeerData[BuffLen] = c;
            if (BuffLen < (PX4BR_PEER_FILE_LINE_SIZE - 1))
                BuffLen++;
        }
        else
        {
            /*
             ** Send the line to the file parser
             */
            if (PX4BR_ParseFileEntry(PX4BR_PeerData, LineNum) == PX4BR_ERROR)
                return PX4BR_ERROR;
            LineNum++;
            memset(PX4BR_PeerData, 0x0, PX4BR_PEER_FILE_LINE_SIZE);
            BuffLen = 0;
        }

    }/* end while */

    OS_close(PeerFile);

    return PX4BR_OK;
}



int32 PX4BR_ParseFileEntry(char *FileEntry, uint32 LineNum)
{
  char    Name[PX4BR_MAX_PEERNAME_LENGTH];
  char    Addr[16];
  int     Port;
  int     ScanfStatus;

   /*
   ** Using sscanf to parse the string.
   ** Currently no error handling
   */
   OS_printf(FileEntry);
   OS_printf("\n");

   ScanfStatus = sscanf(FileEntry,"%s %s %d",Name, Addr, &Port);

   /* Fixme - 1) sscanf needs to be made safe. Use discrete sub functions to safely parse the file
              2) Different protocol id's will have different line parameters
              3) Need check for my cpu name not found
   */

   /*
   ** Check to see if the correct number of items were parsed
   */
   if (ScanfStatus != PX4BR_ITEMS_PER_FILE_LINE) {
     CFE_EVS_SendEvent(PX4BR_INV_LINE_EID,CFE_EVS_ERROR,
                        "%s:Invalid PX4BR ata file line,exp %d items,found %d",
						PX4BR_ITEMS_PER_FILE_LINE, ScanfStatus);
     return PX4BR_ERROR;
   }/* end if */

   if(LineNum < PX4BR_MAX_NETWORK_PEERS)
   {
	   struct hostent *server = gethostbyname(Addr);
	   if(server == 0)
	   {
		   /* TODO - Add event. */
		   printf("PX4BR:  Failed to resolve host. errno=%u", errno);
		   return PX4BR_ERROR;
	   }
	   bcopy(  (char*)server->h_addr,
			   (char*)&PX4BR_AppData.Peer[LineNum].ServAddr.sin_addr.s_addr,
			   server->h_length);
	   bzero((char *) &PX4BR_AppData.Peer[LineNum].ServAddr, sizeof(PX4BR_AppData.Peer[LineNum].ServAddr));
	   PX4BR_AppData.Peer[LineNum].ServAddr.sin_port = htons(Port);
       PX4BR_AppData.Peer[LineNum].State = PX4BR_PEER_STATE_UNINITIALIZED;

       /* Add "!" in case this is the last line read from file */
       /* Declaration of SBN.FileData[SBN_MAX_NETWORK_PEERS + 1] ensures */
       /* writing to array element [LineNum + 1] is safe */
       strncpy((char *)&PX4BR_AppData.Peer[LineNum + 1].Name,"!", PX4BR_MAX_PEERNAME_LENGTH);
   }

   return PX4BR_OK;

}/* end SBN_ParseFileEntry */



int32 PX4BR_InitPeers(void)
{
	uint32 i = 0;
	int32 Status = CFE_SUCCESS;

	/* Clear the initializing peer variables.  These are used as a Blackboard
	 * pattern.  The newly created child tasks will get their respective
	 * Peer object from here.  We clear it now to reflect no initializing peer.
	 * When a peer is created, the address to the newly populated Peer
	 * object is set in "InitializingPeer", and the InitializingPeerCount is set to 3.
	 * Each of the 3 child tasks then pulls the InitializingPeer address, and
	 * caches it for internal use.  It then decrements the InitializingPeerCount
	 * by 1.  Once the count is back down to 0, we know all the threads have
	 * retrieved and cached the address, so we can move on to the next
	 * initialization, if there is one.  Only one peer can be in the
	 * initializing state at a time, to ensure the initializing peer always
	 * gets the correct reference.
	 */
	PX4BR_AppData.InitializingPeer = 0;
	PX4BR_AppData.InitializingPeerCount = 0;

	for(i=0; i < PX4BR_MAX_NETWORK_PEERS; ++i)
	{
		if(PX4BR_AppData.Peer[i].State == PX4BR_PEER_STATE_UNINITIALIZED)
		{
			PX4BR_Peer_t *peer = &PX4BR_AppData.Peer[i];
			peer->IsConnected = FALSE;

			/* This is a valid peer that needs to be initialized. */
			PX4BR_InitPeerSocket(peer);

			/* Now create the 3 worker threads.*/
			char *peerName = PX4BR_AppData.Peer[i].Name;
			char taskNameListener[OS_MAX_API_NAME];
			char taskNameDataOut[OS_MAX_API_NAME];

			strncpy(taskNameListener, PX4BR_LISTENER_TASK_NAME_TEMPLATE, sizeof(taskNameListener));
			strncpy(taskNameDataOut, PX4BR_DATAOUT_TASK_NAME_TEMPLATE, sizeof(taskNameDataOut));
			strncat(taskNameListener, peerName, sizeof(taskNameListener) - strnlen(peerName, sizeof(peerName)) - 1);
			strncat(taskNameDataOut, peerName, sizeof(taskNameDataOut) - strnlen(peerName, sizeof(peerName)) - 1);

			Status = PX4BR_SetInitializingPeer(peer);
			if(Status != PX4BR_OK)
			{
				goto end_of_function;
			}

			Status = CFE_ES_CreateChildTask(&PX4BR_AppData.Peer[i].ListenerTaskID,
					taskNameListener,
					PX4BR_PeerListenerTaskMain,
					0,
					PX4BR_LISTENER_TASK_STACK_SIZE,
					PX4BR_LISTENER_TASK_PRIORITY,
					0);
			if (Status != CFE_SUCCESS)
			{
				/* Failed to create the task.  Raise an event. */
				CFE_EVS_SendEvent(PX4BR_HK_CREATE_CHDTASK_ERR_EID,
						CFE_EVS_ERROR,
						"listener child task failed.  CFE_ES_CreateChildTask returned: 0x%08X",
						Status);
				goto end_of_function;
			}

			Status = CFE_ES_CreateChildTask(&PX4BR_AppData.Peer[i].DataOutTaskID,
					taskNameDataOut,
					PX4BR_PeerDataOutTaskMain,
					0,
					PX4BR_DATAOUT_TASK_STACK_SIZE,
					PX4BR_DATAOUT_TASK_PRIORITY,
					0);
			if (Status != CFE_SUCCESS)
			{
				/* Failed to create the task.  Raise an event. */
				CFE_EVS_SendEvent(PX4BR_HK_CREATE_CHDTASK_ERR_EID,
						CFE_EVS_ERROR,
						"DataOut child task failed.  CFE_ES_CreateChildTask returned: 0x%08X",
						Status);
				goto end_of_function;
			}
		}
	}

end_of_function:
	return Status;
}



int32 PX4BR_SetInitializingPeer(PX4BR_Peer_t *inPeer)
{
	int32 status = PX4BR_OK;

	if(PX4BR_AppData.InitializingPeerCount != 0)
	{
		status = PX4BR_ERROR;
		/* TODO - Add event. */
		OS_printf("PX4BR:  Attempted to set initializing peer while peer still initializing.\n");
		goto end_of_function;
	}
	PX4BR_AppData.InitializingPeer = inPeer;
	PX4BR_AppData.InitializingPeerCount = 2;

end_of_function:
	return status;
}


int32 PX4BR_InitPeerSocket(PX4BR_Peer_t *inPeer)
{
	int32 Status = CFE_SUCCESS;
	int optval = 0;
	int optlen = sizeof(optval);

	inPeer->Socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(inPeer->Socket < 0)
	{
		/* TODO - Add event. */
		OS_printf("PX4BR:  Socket creation failed.\n");
		Status = PX4BR_ERROR;
		goto end_of_function;
	}
	inPeer->ServAddr.sin_family = AF_INET;

	/* Enable TCP keep alive so we know if/when its disconnected. */
	optval = 1;
	optlen = sizeof(optval);
	if(setsockopt(inPeer->Socket, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0)
	{
		/* TODO - Add event. */
		OS_printf("PX4BR:  Failed to enable TCP keepalive.\n");
		Status = PX4BR_ERROR;
		goto end_of_function;
	}

	/* Set the keep alive interval. */
	optval = PX4BR_TCP_KEEPALIVE_INTERVAL;
	optlen = sizeof(optval);
	if(setsockopt(inPeer->Socket, SOL_SOCKET, TCP_KEEPINTVL, &optval, optlen) < 0)
	{
		/* TODO - Add event. */
		OS_printf("PX4BR:  Failed to enable TCP keepalive.\n");
		Status = PX4BR_ERROR;
		goto end_of_function;
	}

end_of_function:
	return Status;
}



void PX4BR_RouteMessageToSB(const char *inMsgName, const char *inMsgContent, uint32 inMsgContentSize)
{
	int32 iStatus = PX4BR_OK;
	PX4BR_Route_t *route = 0;

	iStatus = PX4BR_GetRouteByMsgName(inMsgName, &route);
	if(iStatus != PX4BR_OK)
	{
		//OS_printf("PX4BR:  Received an unknown message.  '%s'\n", inMsgName);
	}
	else
	{
		/* TODO - Add protection against overflow. */
		char outMsg[1500];
		CFE_SB_InitMsg(outMsg,
		       route->MsgID,
			   1500,
			   FALSE);
		int32 decSize = route->DecodeFunc(inMsgContent, inMsgContentSize, outMsg);
		if(decSize > 1500)
		{
			OS_printf("PX4BR just had a buffer overflow.  Fix it.\n");
			return;
		}

		if(decSize > 0)
		{
			/* We successfully decoded it.  Now publish it. */
			CFE_SB_SetTotalMsgLength((CFE_SB_MsgPtr_t)outMsg, decSize);
			CFE_SB_TimeStampMsg((CFE_SB_MsgPtr_t)outMsg);
			CFE_SB_SendMsg((CFE_SB_MsgPtr_t)outMsg);
			route->InCount++;
			if(route->InCount == 1)
			{
				OS_printf("PX4BR:  Received '%s'\n", inMsgName);
			}
			if(route->InCount == 0)
			{
				/* Skip the 0 to avoid roll over issues. */
				route->InCount++;
			}
			return;
		}
	}
}



void PX4BR_RouteMessageToPX4(CFE_SB_MsgPtr_t sbMsg)
{
    CFE_SB_MsgId_t msgID = 0;
    PX4BR_Route_t *route = 0;

    /* Read the message ID from the message. */
    msgID = CFE_SB_GetMsgId(sbMsg);
    PX4BR_GetRouteByMsgID(msgID, &route);
    if(route == 0)
    {
    	OS_printf("PX4BR:  Received SB message with no route.\n");
    }
    else
    {
    	int n = 0;
    	uint32 i = 0;
    	char buffer[PX4BR_MAX_MSG_SIZE];
		uint16 contentSize = CFE_SB_GetTotalMsgLength(sbMsg);
		int32 encSize = route->EncodeFunc(sbMsg, &buffer[PX4BR_NAME_FIELD_LENGTH + PX4BR_SIZE_FIELD_LENGTH], PX4BR_MAX_MSG_SIZE-(PX4BR_NAME_FIELD_LENGTH + PX4BR_SIZE_FIELD_LENGTH));
    	uint32 totalSize = encSize + PX4BR_NAME_FIELD_LENGTH + PX4BR_SIZE_FIELD_LENGTH;

    	if(totalSize > PX4BR_MAX_MSG_SIZE)
    	{
    		OS_printf("PX4BR:  Message '%s' does not fit in transmission packet.  Dropping message.", route->OrbName);
    		return;
    	}

    	buffer[0] = ((encSize & 0xff00) >> 8);
    	buffer[1] = (encSize & 0x00ff);
    	memcpy(&buffer[PX4BR_SIZE_FIELD_LENGTH], route->OrbName, PX4BR_NAME_FIELD_LENGTH);

		for(i = 0; i < PX4BR_MAX_NETWORK_PEERS; ++i)
		{
			if(PX4BR_AppData.Peer[i].IsConnected == TRUE)
			{
				n = write(PX4BR_AppData.Peer[i].Socket, buffer, totalSize);
				if(n < 0)
				{
					/* TODO - Add event */
					OS_printf("PX4BR:  Socket write failed.  errno=%d '%s'", errno, strerror(errno));
					PX4BR_AppData.Peer[i].IsConnected = FALSE;
					close(PX4BR_AppData.Peer[i].Socket);
					PX4BR_InitPeerSocket(&PX4BR_AppData.Peer[i]);
				}
				else
				{
					route->OutCount++;
					if(route->OutCount == 1)
					{
						OS_printf("PX4BR:  Sent '%s'\n", route->OrbName);
					}
					if(route->OutCount == 0)
					{
						/* Skip the 0 to avoid roll over issues. */
						route->OutCount++;
					}
				}
			}
		}
    }
}



int32 PX4BR_GetRouteByMsgName(const char *inMsgName, PX4BR_Route_t **inOutRoute)
{
	uint32 i = 0;
	int32 iStatus = PX4BR_ERROR;

	for(i = 0; i < PX4BR_MAX_ROUTE_COUNT; ++i)
	{
		/* First check to see if this route even has a name at all. */
		if(PX4BR_Route[i].OrbName[0] != 0)
		{
			/* It does.  Now check to see if its the name we're looking for. */
			if(strncmp(PX4BR_Route[i].OrbName, inMsgName, PX4BR_NAME_FIELD_LENGTH) == 0)
			{
				*inOutRoute = &PX4BR_Route[i];
				iStatus = PX4BR_OK;
				goto end_of_function;
			}
		}
	}

end_of_function:
	return iStatus;

}



int32 PX4BR_GetRouteByMsgID(CFE_SB_MsgId_t inMsgID, PX4BR_Route_t **inOutRoute)
{
	uint32 i = 0;
	int32 iStatus = PX4BR_ERROR;

	for(i = 0; i < PX4BR_MAX_ROUTE_COUNT; ++i)
	{
		/* Is this the route we're looking for? */
		if(inMsgID == PX4BR_Route[i].MsgID)
		{
			/* It is.  Return it. */
			*inOutRoute = &PX4BR_Route[i];
			iStatus = PX4BR_OK;
			goto end_of_function;
		}
	}

end_of_function:
	return iStatus;

}



int32 PX4BR_InitSubscriptions(void)
{
	uint32 i = 0;
	int32 iStatus = PX4BR_OK;

	for(i = 0; i < PX4BR_MAX_ROUTE_COUNT; ++i)
	{
		/* First check to see if this route even has a name at all. */
		if(PX4BR_Route[i].OrbName[0] != 0)
		{
			CFE_SB_Qos_t qos = {0,0};
			iStatus = CFE_SB_SubscribeEx(
					PX4BR_Route[i].MsgID,
			        PX4BR_AppData.RouterPipe,
					qos,
					PX4BR_Route[i].MsgDepth);
			if(iStatus != CFE_SUCCESS)
			{
				OS_printf("PX4BR:  CFE_SB_SubscribeEx() failed to subscribe to PX4 message 0x%04x.\n", PX4BR_Route[i].MsgID);
				iStatus = PX4BR_ERROR;
				goto end_of_function;
			}
		}
	}

end_of_function:
	return iStatus;
}


#ifdef __cplusplus
}
#endif
