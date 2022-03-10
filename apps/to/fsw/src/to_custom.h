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

#ifndef TO_CUSTOM_H
#define TO_CUSTOM_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/

#include "cfe.h"

/************************************************************************
** Local Defines
*************************************************************************/

/** \brief TO_Custom_InitEvent passed index with filled event table */
#define TO_CUSTOM_INITEVENT_TOO_MANY_FILTERS                   (-8)

/** \brief TO_Custom_InitEvent passed index with filled event table */
#define TO_CUSTOM_INITEVENT_INDEX_OCCUPIED                     (-9)

/** \brief TO_Custom_InitEvent passed bad index */
#define TO_CUSTOM_INITEVENT_BAD_IND_ERR                        (-10)

/** \brief Send message out the socket error */
#define TO_CUSTOM_RETURN_CODE_TLM_SENDTO_ERR                   (-11)

/** \brief Open Channel Ground error */
#define TO_CUSTOM_RETURN_CODE_TLM_OPEN_GROUND_CHANNEL_ERR      (-12)

/** \brief Open Channel Ground error */
#define TO_CUSTOM_RETURN_CODE_TLM_OPEN_ONBOARD_CHANNEL_ERR     (-13)

/** \brief Channel destination address is NULL  */
#define TO_CUSTOM_RETURN_CODE_TLM_DEST_ADDR_NULL               (-14)

/** \brief Channel ID is invalid */
#define TO_CUSTOM_RETURN_CODE_TLM_CHANNEL_ID_INVALID           (-15)

/** \brief Socket error */
#define TO_CUSTOM_RETURN_CODE_TLM_SOCKET_ERR                   (-16)

/** \brief Socket bind error */
#define TO_CUSTOM_RETURN_CODE_TLM_SOCKET_BIND_ERR              (-17)

/** \brief Channel not closed or disabled  */
#define TO_CUSTOM_RETURN_CODE_TLM_CHANNEL_NOT_DISABLED         (-18)



/************************************************************************
** Local Structure Definitions
*************************************************************************/

/************************************************************************
** External Global Variables
*************************************************************************/


/************************************************************************/
/** \brief Custom function to initialize all output channels.
**
**  \par Description
**       This function is called once at initialization and allows the
**       custom layer to provide specific functionality to initialize
**       internal objects.
**
**  \par Assumptions, External Events, and Notes:
**       This function must be defined, but not all custom
**       layers will do anything in this function.
**
*************************************************************************/
int32 TO_Custom_Init(void);

/************************************************************************/
/** \brief Custom function to initialize custom event filters.
**
**  \par Description
**       This function is called once at initialization and sets the
**       event filters for custom layer EID's if necessary. Passed the index
**       of the first empty entry of TO_AppData.EventTbl. Should be called in
**       TO_InitEvent().
**
**  \par Assumptions, External Events, and Notes:
**       This function must be defined, but not all custom
**       layers will do anything in this function. ind assumed to be
**       between 0 and CFE_MAX_EVENT_FILTERS, and point to an unused
**       location in TO_AppData.EventTbl.
**
**  \param[in,out] ind         Pointer to a value the will be used for
**                              size of the EventTbl array to register
**                              TO event messages for filtering 
**
*************************************************************************/
int32 TO_Custom_InitEvent(int32 *ind);

/************************************************************************/
/** \brief Custom function to build up all output channels.
**
**  \par Description
**       This function is called after a new configuration table has
**       been loaded, allowing the custom layer to do whatever it needs
**       to do after a reconfiguration, if anything.
**
**  \par Assumptions, External Events, and Notes:
**       This function must be defined, but not all custom
**       layers will do anything in this function.
**
**  \param [in]   index
**                Index of the channel
**
*************************************************************************/
int32 TO_OutputChannel_CustomBuildup(uint32 index);



/************************************************************************/
/** \brief Custom function in to tear down all output channels.
**
**  \par Description
**       This function is called in preparation for loading a new
**       configuration table, allowing the custom layer to do whatever it
**       needs with the current configuration before reconfiguration,
**       if anything.
**
**  \par Assumptions, External Events, and Notes:
**       This function must be defined, but not all custom
**       layers will do anything in this function.
**
**  \param [in]   index
**                Index of the channel
**
*************************************************************************/
int32 TO_OutputChannel_CustomTeardown(uint32 index);



/************************************************************************/
/** \brief Custom function to cleanup all output channels before
**         shutdown.
**
**  \par Description
**       This function is called in preparation for application shutdown.
**       This allows the custom layer to do whatever it needs to do, if
**       anything.
**
**  \par Assumptions, External Events, and Notes:
**       This function must be defined, but not all custom
**       layers will do anything in this function.
**
*************************************************************************/
void  TO_OutputChannel_CustomCleanupAll(void);



/************************************************************************/
/** \brief Custom function to process new commands.
**
**  \par Description
**       This function is called when the sealed layer receives a command
**       it doesn't know about.  This allows the custom layer to receive
**       commands without requiring the sealed layer to know about it.
**
**  \par Assumptions, External Events, and Notes:
**       This function must be defined, though not all custom layers will
**       have commands to process.  This function must at least raise an
**       event when an unknown command is received.
**
**  \param [in]   msgPtr        A #CFE_SB_Msg_t pointer that
**                              references the software bus message
**
*************************************************************************/
void TO_OutputChannel_ProcessNewCustomCmds(CFE_SB_Msg_t *msgPtr);



/************************************************************************/
/** \brief Custom function to return channel mode status.
**
**  \par Description
**       This function returns the status of a channel from the custom
**       layer.
**
**  \par Assumptions, External Events, and Notes:
**       This function must be defined, though not all custom layers will
**       have commands to process.  This function must at least raise an
**       event when an unknown command is received.
**
**  \param [in]   index        The channel to return the status of
**
*************************************************************************/
uint8 TO_OutputChannel_Status(uint32 index);

/**
 * \brief Get the custom version number.
 */
uint32 TO_GetCustomVersion(void);

/**
 * \brief Print custom version (long form).
 */
void TO_PrintCustomVersion(void);


void TO_OutputChannel_SendTelemetry(uint32 index);


/************************************************************************/
/** \brief Resets channel HK counters
**
*************************************************************************/
void TO_ResetCustomChannelCounters(void);

#endif
