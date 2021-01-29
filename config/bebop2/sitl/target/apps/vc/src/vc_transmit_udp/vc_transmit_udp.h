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

#ifndef VC_TRANSMIT_UDP_H
#define VC_TRANSMIT_UDP_H

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "px4_msgs.h"
#include "msg_ids.h"
#include "vc_transmit.h"
#include "../vc_custom_shared.h"
#include "vc_perfids.h"
#include <arpa/inet.h>
#include "vc_app.h"

/************************************************************************
** Structure Declarations
*************************************************************************/

/**
 * Channel modes
 */
typedef enum
{
    /*! Channel state unused  */
    VC_CHANNEL_UNUSED       = 0,
    /*! Channel state disabled */
    VC_CHANNEL_DISABLED     = 1,
    /*! Channel state enabled */
    VC_CHANNEL_ENABLED      = 2
} VC_ChannelMode_t;


/**
 * Transmit struct handle for user defined source and/or destination
 * configuration information and initialized resource reference.
 */
typedef struct
{
    /*! Channel mode */
    VC_ChannelMode_t    Mode;
    /*! Channel identifier */
    uint8               ChannelID;
    /*! Destination port */
    uint16              DestPort;
    /*! Local port to bind */
    uint16              MyPort;
    /*! Destination IP address */
    char                DestIP[INET_ADDRSTRLEN];
    /*! Local IP address to allow interface choices */
    char                MyIP[INET_ADDRSTRLEN];
    /*! Socket file descriptor for the channel */
    int                 SocketFd;
} VC_Transmit_Handle_t;

/**
 * Transmit handles
 */
typedef struct
{
    VC_Transmit_Handle_t Channel[VC_MAX_OUTPUT_CHANNELS];
} VC_AppCustomData_t;


/** \brief Optical flow frame message  */
PX4_OpticalFlowFrameMsg_t OpticalFlowFrameMsg;
/************************************************************************
** External Global Variables
*************************************************************************/

extern VC_AppCustomData_t VC_AppCustomData;

/************************************************************************
** Function Prototypes
*************************************************************************/

/**
 * @brief Custom function to initialize custom transmit data structure.
 * @return CFE_SUCCESS
 */
int32 VC_CustomTransmit_InitData(void);


/**
 * @brief Initialize all enabled transmit resources
 * @return 0 for success -1 for failure
 */
int32 VC_Init_CustomTransmitters(void);


/**
 * @brief Initialize a transmit channel
 * @return 0 for success -1 for failure
 */
int32 VC_EnableChannel(uint8 ChannelID);


/**
 * @brief Cleanup all transmit resources 
 * @return 0 for success -1 for failure
 */
int32 VC_CleanupCustom(void);


/**
 * @brief Disable a transmit channel
 * @param ChannelID the channel to disable
 * @return 0 for success -1 for failure
 */
int32 VC_DisableChannel(uint8 ChannelID);


#endif
