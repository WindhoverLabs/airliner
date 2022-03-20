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

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include <string.h>

#include "cfe.h"

#include "rfd900x_app.h"
#include "rfd900x_msg.h"
#include "rfd900x_version.h"
#include "sedlib.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/
RFD900X_AppData_t  RFD900X_AppData;

/************************************************************************
** Local Variables
*************************************************************************/

#define UART_NOOP_CC              (0)
#define UART_RESET_CC             (1)
#define UART_QUEUE_DATA_FOR_TX_CC (2)
#define UART_FLUSH_CC             (3)
#define UART_CONFIGURE_CC         (4)

/* SLIP special character codes
 */
#define SLIP_END        	 0xC0    /* Indicates end of packet */
#define SLIP_ESC             0xDB    /* Indicates byte stuffing */
#define SLIP_ESC_END         0xDC    /* ESC ESC_END means END data byte */
#define SLIP_ESC_ESC         0xDD    /* ESC ESC_ESC means ESC data byte */





/************************************************************************
** Local Function Definitions
*************************************************************************/
void RFD900X_SendMessage(void);
int32 RFD900X_InitSockets(void);
int32 RFD900X_BindTxSocket(void);
int32 RFD900X_BindRxSocket(void);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 RFD900X_InitEvent()
{
    int32  iStatus=CFE_SUCCESS;
    int32  ind = 0;

    /* Initialize the event filter table.
     * Note: 0 is the CFE_EVS_NO_FILTER mask and event 0 is reserved (not used) */
    memset((void*)RFD900X_AppData.EventTbl, 0x00, sizeof(RFD900X_AppData.EventTbl));

    /* TODO: Choose the events you want to filter.  CFE_EVS_MAX_EVENT_FILTERS
     * limits the number of filters per app.  An explicit CFE_EVS_NO_FILTER 
     * (the default) has been provided as an example. */
    RFD900X_AppData.EventTbl[  ind].EventID = RFD900X_RESERVED_EID;
    RFD900X_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    RFD900X_AppData.EventTbl[  ind].EventID = RFD900X_INF_EID;
    RFD900X_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    RFD900X_AppData.EventTbl[  ind].EventID = RFD900X_CONFIG_TABLE_ERR_EID;
    RFD900X_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    RFD900X_AppData.EventTbl[  ind].EventID = RFD900X_CDS_ERR_EID;
    RFD900X_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    RFD900X_AppData.EventTbl[  ind].EventID = RFD900X_PIPE_ERR_EID;
    RFD900X_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    RFD900X_AppData.EventTbl[  ind].EventID = RFD900X_MSGID_ERR_EID;
    RFD900X_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    RFD900X_AppData.EventTbl[  ind].EventID = RFD900X_MSGLEN_ERR_EID;
    RFD900X_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(RFD900X_AppData.EventTbl,
                               RFD900X_EVT_CNT, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("RFD900X - Failed to register with EVS (0x%08X)\n", (unsigned int)iStatus);
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 RFD900X_InitPipe()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&RFD900X_AppData.SchPipeId,
                                 RFD900X_SCH_PIPE_DEPTH,
                                 RFD900X_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(RFD900X_WAKEUP_MID, RFD900X_AppData.SchPipeId, CFE_SB_Default_Qos, RFD900X_SCH_PIPE_WAKEUP_RESERVED);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(RFD900X_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "Sch Pipe failed to subscribe to RFD900X_WAKEUP_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto RFD900X_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(RFD900X_SEND_HK_MID, RFD900X_AppData.SchPipeId, CFE_SB_Default_Qos, RFD900X_SCH_PIPE_SEND_HK_RESERVED);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(RFD900X_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "CMD Pipe failed to subscribe to RFD900X_SEND_HK_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto RFD900X_InitPipe_Exit_Tag;
        }

    }
    else
    {
        (void) CFE_EVS_SendEvent(RFD900X_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create SCH pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto RFD900X_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&RFD900X_AppData.CmdPipeId,
                                 RFD900X_CMD_PIPE_DEPTH,
                                 RFD900X_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(RFD900X_CMD_MID, RFD900X_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(RFD900X_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "CMD Pipe failed to subscribe to RFD900X_CMD_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto RFD900X_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(RFD900X_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create CMD pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto RFD900X_InitPipe_Exit_Tag;
    }

    /* Init data pipe and subscribe to messages on the data pipe */
    iStatus = CFE_SB_CreatePipe(&RFD900X_AppData.DataPipeId,
                                 RFD900X_DATA_PIPE_DEPTH,
                                 RFD900X_DATA_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* TODO:  Add CFE_SB_Subscribe() calls for other apps' output data here.
        **
        ** Examples:
        **     CFE_SB_Subscribe(GNCEXEC_OUT_DATA_MID, RFD900X_AppData.DataPipeId);
        */
    }
    else
    {
        (void) CFE_EVS_SendEvent(RFD900X_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create Data pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto RFD900X_InitPipe_Exit_Tag;
    }

RFD900X_InitPipe_Exit_Tag:
    return (iStatus);
}
    

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 RFD900X_InitData()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init input data */
    memset((void*)&RFD900X_AppData.InData,
    		0x00,
			sizeof(RFD900X_AppData.InData));

    /* Init output data */
    CFE_SB_InitMsg(&RFD900X_AppData.OutData,
                   RFD900X_OUT_DATA_MID,
				   sizeof(RFD900X_AppData.OutData),
				   TRUE);

    /* Init housekeeping packet */
    CFE_SB_InitMsg(&RFD900X_AppData.HkTlm,
                   RFD900X_HK_TLM_MID,
				   sizeof(RFD900X_AppData.HkTlm),
				   TRUE);


    CFE_SB_InitMsg(&RFD900X_AppData.UART_StatusTlm,
    		        RFD900X_UART_TLM_MID,
					sizeof(RFD900X_AppData.UART_StatusTlm),
					TRUE);

	RFD900X_AppData.UART_StatusTlm.Version = 1;

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* RFD900X initialization                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 RFD900X_InitApp()
{
    int32  iStatus   = CFE_SUCCESS;
    int8   hasEvents = 0;

    iStatus = RFD900X_InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("RFD900X - Failed to init events (0x%08X)\n", (unsigned int)iStatus);
        goto RFD900X_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = RFD900X_InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(RFD900X_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init pipes (0x%08X)",
                                 (unsigned int)iStatus);
        goto RFD900X_InitApp_Exit_Tag;
    }

    iStatus = RFD900X_InitData();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(RFD900X_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init data (0x%08X)",
                                 (unsigned int)iStatus);
        goto RFD900X_InitApp_Exit_Tag;
    }

    iStatus = RFD900X_InitConfigTbl();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(RFD900X_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init config tables (0x%08X)",
                                 (unsigned int)iStatus);
        goto RFD900X_InitApp_Exit_Tag;
    }

    iStatus = RFD900X_InitCdsTbl();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(RFD900X_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init CDS table (0x%08X)",
                                 (unsigned int)iStatus);
        goto RFD900X_InitApp_Exit_Tag;
    }

    iStatus = RFD900X_InitSockets();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(RFD900X_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to initialize socket (0x%08X)",
                                 (unsigned int)iStatus);
        goto RFD900X_InitApp_Exit_Tag;
    }

    iStatus = SEDLIB_GetPipe(
    		"UART1_CMD",
			sizeof(UART_QueueDataCmd_t),
			&RFD900X_AppData.TxMsgPortHandle);
    if(iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(RFD900X_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to initialize UART TxMsgPort. (0x%08lX)",
                                 iStatus);
    }

    iStatus = SEDLIB_GetPipe(
    		"UART1_STATUS",
			sizeof(UART_StatusTlm_t),
			&RFD900X_AppData.RxMsgPortHandle);
    if(iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(RFD900X_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to initialize UART RxMsgPort. (0x%08lX)",
                                 iStatus);
    }

RFD900X_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(RFD900X_INIT_INF_EID, CFE_EVS_INFORMATION,
                                 "Initialized.  Version %d.%d.%d.%d",
                                 RFD900X_MAJOR_VERSION,
                                 RFD900X_MINOR_VERSION,
                                 RFD900X_REVISION,
                                 RFD900X_MISSION_REV);
    }
    else
    {
        if (hasEvents == 1)
        {
            (void) CFE_EVS_SendEvent(RFD900X_INIT_ERR_EID, CFE_EVS_ERROR, "Application failed to initialize");
        }
        else
        {
            (void) CFE_ES_WriteToSysLog("RFD900X - Application failed to initialize\n");
        }
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 RFD900X_RcvMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(RFD900X_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, RFD900X_AppData.SchPipeId, iBlocking);

    RFD900X_ProcessNewData();

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(RFD900X_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
	    {
            case RFD900X_WAKEUP_MID:
                RFD900X_ProcessNewCmds();
                RFD900X_ProcessNewData();

                /* TODO:  Add more code here to handle other things when app wakes up */

                /* The last thing to do at the end of this Wakeup cycle should be to
                 * automatically publish new output. */
                RFD900X_SendOutData();
                break;

            case RFD900X_SEND_HK_MID:
                RFD900X_ReportHousekeeping();
                break;

            default:
                (void) CFE_EVS_SendEvent(RFD900X_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid SCH msgId (0x%04X)", (unsigned short)MsgId);
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
        /* TODO: This is an example of exiting on an error (either CFE_SB_BAD_ARGUMENT, or
         * CFE_SB_PIPE_RD_ERROR).
         */
        (void) CFE_EVS_SendEvent(RFD900X_PIPE_ERR_EID, CFE_EVS_ERROR,
			  "SB pipe read error (0x%08X), app will exit", (unsigned int)iStatus);
        RFD900X_AppData.uiRunStatus= CFE_ES_APP_ERROR;
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Data                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void RFD900X_ProcessNewData()
{
	SEDLIB_ReturnCode_t rc;
    uint32 bytesRemaining = 0;
    int32 recvSize;
    SEDLIB_MsgReadStatus_t readStatus;
    SEDLIB_MsgWriteStatus_t writeStatus;

	/* Process UART commands */
    rc = SEDLIB_ReadMsg(RFD900X_AppData.TxMsgPortHandle, (CFE_SB_MsgPtr_t)&RFD900X_AppData.UART_QueueDataCmd);

    /* Did we just receive fresh data? */
    if(SEDLIB_MSG_FRESH_OK == rc)
    {
        /* Yes we did.  Get the command code. */
    	RFD900X_AppData.UART_StatusTlm.LastCmdCode = CFE_SB_GetCmdCode((CFE_SB_MsgPtr_t)&RFD900X_AppData.UART_QueueDataCmd);

        /* Act on the command code. */
        switch(RFD900X_AppData.UART_StatusTlm.LastCmdCode)
        {
            case UART_NOOP_CC:
            {
                break;
            }

            case UART_RESET_CC:
            {
                break;
            }

            case UART_QUEUE_DATA_FOR_TX_CC:
            {
            	/* Loop through each byte to queue data or send a message out when the
                 * end symbol was found. */
            	for(uint32 i = 0; i < RFD900X_AppData.UART_QueueDataCmd.BytesInBuffer; ++i)
            	{
            		/* Queue the next byte. */
            		RFD900X_AppData.TxInBuffer[RFD900X_AppData.TxInBufferCursor] =
            				RFD900X_AppData.UART_QueueDataCmd.Buffer[i];

            		/* Was this byte the SLIP_END symbol? */
            		if(SLIP_END == RFD900X_AppData.TxInBuffer[RFD900X_AppData.TxInBufferCursor])
            		{
            		    /* Yes it was.  Send the message out, reset the cursor, and
            		     * keep going. */
            			RFD900X_SendMessage();
            			RFD900X_AppData.TxInBufferCursor = 0;
            		}
            		else
            		{
                		RFD900X_AppData.TxInBufferCursor++;
            		}
            	}

        		RFD900X_SendMessage();
        		RFD900X_AppData.TxInBufferCursor = 0;

                break;
            }

            case UART_FLUSH_CC:
            {
                break;
            }

            case UART_CONFIGURE_CC:
            {
                break;
            }
        }
    }

    /* Is the receiver ready to receive a new response? */
    rc = SEDLIB_GetMessageStatus(RFD900X_AppData.RxMsgPortHandle, &readStatus, &writeStatus);
    if(readStatus == SEDLIB_MSG_READ_ACKNOWLEDGED)
    {
		/* Process UART responses.  First see if we received any data. */
		bytesRemaining = sizeof(RFD900X_AppData.RxBuffer) - RFD900X_AppData.BytesInRxBuffer;
		recvSize = recv(RFD900X_AppData.RxSocket,
						(char *)&RFD900X_AppData.RxBuffer[RFD900X_AppData.BytesInRxBuffer],
						(size_t)bytesRemaining, 0);
		/* Did we receive any data? */
		if(recvSize > 0)
		{
			uint32 bytesToCopy;

			RFD900X_AppData.BytesInRxBuffer += recvSize;

			/* Yes we did.  Put as much of it in the status response as will fit. */
			if(RFD900X_AppData.BytesInRxBuffer > sizeof(RFD900X_AppData.UART_StatusTlm.RxBuffer))
			{
				bytesToCopy = sizeof(RFD900X_AppData.UART_StatusTlm.RxBuffer);
			}
			else
			{
				bytesToCopy = RFD900X_AppData.BytesInRxBuffer;
			}

			memcpy(RFD900X_AppData.UART_StatusTlm.RxBuffer, RFD900X_AppData.RxBuffer, bytesToCopy);

			RFD900X_AppData.UART_StatusTlm.BytesInBuffer = bytesToCopy;
			RFD900X_AppData.UART_StatusTlm.RxFrameID++;

			rc = SEDLIB_SendMsg(RFD900X_AppData.RxMsgPortHandle, (CFE_SB_MsgPtr_t)&RFD900X_AppData.UART_StatusTlm);
            if(rc != SEDLIB_OK)
            {
                (void) CFE_EVS_SendEvent(RFD900X_SENDMSG_ERR_EID,
                		                 CFE_EVS_ERROR,
                                         "SEDLIB_SendMsg error %i", rc);
            }

			/* We're going to assume that the message went out.  Nothing we can
			 * do if it didn't.  Is there any data left in the RxBuffer? */
            if(RFD900X_AppData.BytesInRxBuffer >  bytesToCopy)
            {
            	/* Yes, there is still data left.  Shift the data to the left. */
            	for(uint32 i = bytesToCopy; i < RFD900X_AppData.BytesInRxBuffer; ++i)
            	{
            		RFD900X_AppData.RxBuffer[i - bytesToCopy] = RFD900X_AppData.RxBuffer[i];
            	}

            	/* No set the new bytes in buffer count. */
            	RFD900X_AppData.BytesInRxBuffer = RFD900X_AppData.BytesInRxBuffer - bytesToCopy;
            }
		}
    }
}



void RFD900X_SendMessage(void)
{
	ssize_t sentSize;
    struct sockaddr_in s_addr;
    int    status = 0;
    uint32 outCursor = 0;

//    /* Send the message out */
//    printf("%u   ", __LINE__);
//    for(uint32 i = 0; i < RFD900X_AppData.TxInBufferCursor; ++i)
//    {
//        printf("%02x ", RFD900X_AppData.TxInBuffer[i]);
//    }
//    printf("\n");

    for(uint32 i = 0; i < RFD900X_AppData.TxInBufferCursor; ++i)
    {
    	const uint8 inByte = (const uint8) RFD900X_AppData.TxInBuffer[i];

    	switch(inByte)
    	{
    	    case SLIP_ESC:
    	    {
    	    	++i;

    	    	const uint8 inByte2 = (const uint8) RFD900X_AppData.TxInBuffer[i];

    	    	switch(inByte2)
    	    	{
    	    	    case SLIP_ESC_END:
    	    	    {
    	    	    	RFD900X_AppData.TxOutBuffer[outCursor] = SLIP_END;
    	    	    	outCursor++;
    	    	    	break;
    	    	    }

    	    	    case SLIP_ESC_ESC:
    	    	    {
    	    	    	RFD900X_AppData.TxOutBuffer[outCursor] = SLIP_ESC;
    	    	    	outCursor++;
    	    	    	break;
    	    	    }

    	    	    default:
    	    	    {
    	    	    	RFD900X_AppData.TxOutBuffer[outCursor] = inByte2;
    	    	    	outCursor++;
    	    	    	break;
    	    	    }
    	    	}

    	    	break;
    	    }

    	    default:
    	    {
    	    	RFD900X_AppData.TxOutBuffer[outCursor] = inByte;
    	    	outCursor++;
    	    }
    	}
    }

    if(outCursor > 0)
    {
		bzero((char *) &s_addr, sizeof(s_addr));
		s_addr.sin_family      = AF_INET;

		/* Send message via UDP socket */
		s_addr.sin_addr.s_addr = inet_addr(RFD900X_AppData.ConfigTblPtr->Address);
		s_addr.sin_port = htons(RFD900X_AppData.ConfigTblPtr->TxPort);

		status = sendto(RFD900X_AppData.TxSocket,
						(char *)RFD900X_AppData.TxOutBuffer,
						outCursor,
						0,
						(struct sockaddr *) &s_addr,
						sizeof(s_addr));
		if (status < 0)
		{
			CFE_EVS_SendEvent(RFD900X_TX_SEND_ERR_EID,
							  CFE_EVS_ERROR,
							  "send errno %d.", errno);
		}
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void RFD900X_ProcessNewCmds()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr=NULL;
    CFE_SB_MsgId_t  CmdMsgId;

    /* Process command messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, RFD900X_AppData.CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case RFD900X_CMD_MID:
                    RFD900X_ProcessNewAppCmds(CmdMsgPtr);
                    break;

                /* TODO:  Add code to process other subscribed commands here
                **
                ** Example:
                **     case CFE_TIME_DATA_CMD_MID:
                **         RFD900X_ProcessTimeDataCmd(CmdMsgPtr);
                **         break;
                */

                default:
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with this
                     *  pipe, but not handled in this switch-case.) */
                    RFD900X_AppData.HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(RFD900X_MSGID_ERR_EID, CFE_EVS_ERROR,
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
            (void) CFE_EVS_SendEvent(RFD900X_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08X)", (unsigned int)iStatus);
            RFD900X_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process RFD900X Commands                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void RFD900X_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case RFD900X_NOOP_CC:
                RFD900X_AppData.HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(RFD900X_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "Recvd NOOP cmd (%u), Version %d.%d.%d.%d",
                                  (unsigned int)uiCmdCode,
                                  RFD900X_MAJOR_VERSION,
                                  RFD900X_MINOR_VERSION,
                                  RFD900X_REVISION,
                                  RFD900X_MISSION_REV);
                break;

            case RFD900X_RESET_CC:
                RFD900X_AppData.HkTlm.usCmdCnt = 0;
                RFD900X_AppData.HkTlm.usCmdErrCnt = 0;
                (void) CFE_EVS_SendEvent(RFD900X_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "Recvd RESET cmd (%u)", (unsigned int)uiCmdCode);
                break;

            /* TODO:  Add code to process the rest of the RFD900X commands here */

            default:
                RFD900X_AppData.HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(RFD900X_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid cmdId (%u)", (unsigned int)uiCmdCode);
                break;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send RFD900X Housekeeping                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void RFD900X_ReportHousekeeping()
{
    /* TODO:  Add code to update housekeeping data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&RFD900X_AppData.HkTlm);
    int32 iStatus = CFE_SB_SendMsg((CFE_SB_Msg_t*)&RFD900X_AppData.HkTlm);
    if (iStatus != CFE_SUCCESS)
    {
        /* TODO: Decide what to do if the send message fails. */
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Publish Output Data                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void RFD900X_SendOutData()
{
    /* TODO:  Add code to update output data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&RFD900X_AppData.OutData);
    int32 iStatus = CFE_SB_SendMsg((CFE_SB_Msg_t*)&RFD900X_AppData.OutData);
    if (iStatus != CFE_SUCCESS)
    {
        /* TODO: Decide what to do if the send message fails. */
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Command Length                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

boolean RFD900X_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
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

            (void) CFE_EVS_SendEvent(RFD900X_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            RFD900X_AppData.HkTlm.usCmdErrCnt++;
        }
    }

    return (bResult);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* RFD900X application entry point and main process loop               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void RFD900X_AppMain()
{
    /* Register the application with Executive Services */
    RFD900X_AppData.uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("RFD900X - Failed to register the app (0x%08X)\n", (unsigned int)iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(RFD900X_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = RFD900X_InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(RFD900X_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(RFD900X_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(RFD900X_MAIN_TASK_PERF_ID);
    }
    else
    {
        RFD900X_AppData.uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&RFD900X_AppData.uiRunStatus) == TRUE)
    {
        int32 iStatus = RFD900X_RcvMsg(RFD900X_SCH_PIPE_PEND_TIME);
        if (iStatus != CFE_SUCCESS)
        {
            /* TODO: Decide what to do for other return values in RFD900X_RcvMsg(). */
        }
        /* TODO: This is only a suggestion for when to update and save CDS table.
        ** Depends on the nature of the application, the frequency of update
        ** and save can be more or less independently.
        */
        RFD900X_UpdateCdsTbl();
        RFD900X_SaveCdsTbl();

        iStatus = RFD900X_AcquireConfigPointers();
        if(iStatus != CFE_SUCCESS)
        {
            /* We apparently tried to load a new table but failed.  Terminate the application. */
            RFD900X_AppData.uiRunStatus = CFE_ES_APP_ERROR;
        }
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(RFD900X_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(RFD900X_AppData.uiRunStatus);
}



int32 RFD900X_InitSockets(void)
{
	int32  returnCode = CFE_SUCCESS;
    int    reuseaddr = 1;
    struct sockaddr_in servaddr;
    int    status;

    /* Initialize the RX and TX sockets */
    RFD900X_AppData.RxSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(RFD900X_AppData.RxSocket < 0)
    {
        CFE_EVS_SendEvent(RFD900X_SOCKET_ERR_EID, CFE_EVS_ERROR,
                "RX socket errno %i", errno);
        returnCode = -1;
        goto end_of_function;
    }

    RFD900X_AppData.TxSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(RFD900X_AppData.TxSocket < 0)
    {
        CFE_EVS_SendEvent(RFD900X_SOCKET_ERR_EID, CFE_EVS_ERROR,
                "TX socket errno %i", errno);
        returnCode = -1;
        goto end_of_function;
    }

    /* Set the Reuse Address flag for each socket.  If we don't set this flag,
     * the socket will lock the port on termination and the kernel won't
     * unlock it until it times out after a minute or so.
     */
    setsockopt(RFD900X_AppData.RxSocket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
    setsockopt(RFD900X_AppData.TxSocket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));

    returnCode = RFD900X_BindTxSocket();
    if(returnCode != CFE_SUCCESS)
    {
    	goto end_of_function;
    }

    returnCode = RFD900X_BindRxSocket();

end_of_function:

    return returnCode;

}



int32 RFD900X_BindTxSocket(void)
{
	int32  returnCode = CFE_SUCCESS;
    struct sockaddr_in servaddr;
    int    status;

	/* Bind the TX socket. */
    bzero((void*)&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = 0;
    status = bind(RFD900X_AppData.TxSocket,
                  (struct sockaddr *)&servaddr,
				  sizeof(servaddr));
    if(status < 0)
    {
        CFE_EVS_SendEvent(RFD900X_SOCKET_ERR_EID, CFE_EVS_ERROR,
                "TX bind errno %i", errno);
        returnCode = -1;
    }

    return returnCode;
}



int32 RFD900X_BindRxSocket(void)
{
	int32  returnCode = CFE_SUCCESS;
    struct sockaddr_in servaddr;
    int    status;

	/* Bind the RX socket. */
    bzero((void*)&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(RFD900X_AppData.ConfigTblPtr->RxPort);
    status = bind(RFD900X_AppData.RxSocket,
                  (struct sockaddr *)&servaddr,
				  sizeof(servaddr));
    if(status < 0)
    {
        CFE_EVS_SendEvent(RFD900X_SOCKET_ERR_EID, CFE_EVS_ERROR,
                "RX bind errno %i", errno);
        returnCode = -1;
        goto end_of_function;
    }

    /* Put the socket in non-blocking mode: */
    status = fcntl(RFD900X_AppData.RxSocket, F_SETFL, fcntl(RFD900X_AppData.RxSocket, F_GETFL) | O_NONBLOCK);
    if(status)
    {
        CFE_EVS_SendEvent(RFD900X_SOCKET_ERR_EID, CFE_EVS_ERROR,
                "RX fcntl errno %i", errno);
        returnCode = -1;
    }

end_of_function:

    return returnCode;
}



/************************/
/*  End of File Comment */
/************************/
