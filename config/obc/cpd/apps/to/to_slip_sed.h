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

#ifndef TO_SLIP_SED_H
#define TO_SLIP_SED_H

/************************************************************************
** Includes
*************************************************************************/
#include "to_app.h"
#include "to_custom.h"


/************************************************************************
** Local Defines
*************************************************************************/
#define TO_CUSTOM_RETURN_CODE_NULL_POINTER      (-1)
#define TO_CUSTOM_RETURN_CODE_MESSAGE_TOO_BIG   (-2)



/* TODO:  Add Doxygen markup. */
#define TO_ENABLE_CHANNEL_CC                    (10)
#define TO_DISABLE_CHANNEL_CC                   (11)

#define UART_BUFFER_SIZE                        (1000)
#define UART_CMD_MSG_ID                         (0x1810)


/* SLIP special character codes
 */
#define SLIP_END        	 0xC0    /* Indicates end of packet */
#define SLIP_ESC             0xDB    /* Indicates byte stuffing */
#define SLIP_ESC_END         0xDC    /* ESC ESC_END means END data byte */
#define SLIP_ESC_ESC         0xDD    /* ESC ESC_ESC means ESC data byte */


/************************************************************************
** Local Structure Declarations
*************************************************************************/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint8  ChannelID;
    uint32 MsgPortAddress;
} TO_EnableChannelCmd_t;

typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint8  ChannelID;
} TO_DisableChannelCmd_t;

typedef enum {
    /* TODO:  Add Doxygen markup. */
	TO_TLMOUTENA_INF_EID = TO_EVT_CNT,
    TO_TLMOUTENA_ERR_EID,
    TO_TLMOUTDIS_INF_EID,
    TO_TLMOUTDIS_ERR_EID,
    TO_TLMOUTSEND_ERR_EID,
    TO_TLMOUTSTOP_ERR_EID,
    TO_CUSTOM_EVT_CNT
} TO_CustomEventIds_t;

typedef enum
{
    TO_CHANNEL_UNUSED       = 0,
    TO_CHANNEL_DISABLED     = 1,
    TO_CHANNEL_ENABLED      = 2
} TO_TLM_ChannelMode_t;


/* UART Out message */
#pragma pack(push, 1)
typedef struct
{
    uint8           CmdHeader[CFE_SB_TLM_HDR_SIZE];
    uint32          Version;
	uint32          FrameID;
	uint32          BytesInBuffer;
    uint8           Buffer[UART_BUFFER_SIZE];
} UART_QueueDataCmd_t;
#pragma pack(pop)

typedef enum
{
	TO_SLIP_NOMINAL           = 0,
	TO_SLIP_ENCODING_END      = 1,
	TO_SLIP_ENCODING_ESC_ESC  = 2,
	TO_SLIP_ENCODING_ESC_END  = 3
} TO_SlipEncoderState_t;

typedef struct
{
    TO_TLM_ChannelMode_t            Mode;
    uint32                          MsgPortHandle;
    UART_QueueDataCmd_t             UartQueueDataCmd;
    uint32                          MsgPortAddress;
    uint32                          InputCursor;
    uint32                          BytesQueued;
    TO_SlipEncoderState_t           EncoderState;
    osalbool                        MsgProcessInProgress;
    CFE_SB_MsgPtr_t                 InWorkMsg;
} TO_TlmChannel_t;

typedef struct
{
    TO_TlmChannel_t    Channel[TO_MAX_CHANNELS];
} TO_AppCustomData_t;

/************************************************************************
** External Global Variables
*************************************************************************/

extern TO_AppData_t TO_AppData;

/************************************************************************
** Local Function Definitions
*************************************************************************/

/**
 * The routine to send a buffer out the channel.  This routine
 * abstracts the formatting and output of different channels.
 */
int32 TO_OutputChannel_Send(uint32 ChannelID, const char* Buffer, uint32 Size);

int32 TO_OutputChannel_Enable(uint32 ChannelID);
int32 TO_OutputChannel_Disable(uint32 ChannelID);

/**
 * The UDP Development Channel Task Entry Point
 */
void  TO_OutputChannel_ChannelTask(void);

/**
 * The child task routine for a pulling from the output queue and sending
 * out the channel.
 */
void  TO_OutputChannel_ChannelHandler(uint32 ChannelIndex);

int32 TO_OutputChannel_CustomBuildupAll(uint32 index);
int32 TO_OutputChannel_CustomTeardownAll(uint32 index);


#endif
