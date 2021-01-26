/****************************************************************************
 *
 *   Copyright (c) 2016-2017 Windhover Labs, L.L.C. All rights reserved.
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
 * 3. Neither the name Windhover Labs nor the names of its contributors 
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
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
 ****************************************************************************/

#ifndef PC_LIB_H
#define PC_LIB_H

/*************************************************************************
** Includes
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cfe.h"

/*************************************************************************
** Macro Definitions
*************************************************************************/
#define PBLIB_REG_MAX_ENTRY  (100)
#define PBLIB_REGTBL_MUTEX_NAME ("PBLIB_REGTBL_MUTEX")

/************************************************************************
** Local Structure Definitions
*************************************************************************/

/**
**  \brief Registered Serialization Data
*/
typedef struct
{
	/** \brief Message ID of serialized message */
	CFE_SB_MsgId_t 	msgId;

	/** \brief Name of serialized message data type */
	char			msgName[CFE_SB_MAX_SB_MSG_SIZE];

	/** \brief Command code for serialized message */
    uint16	 		cmdCode;
} PBLIB_RegData_t;

/**
**  \brief PB Lib Data
*/
typedef struct
{
	/** \brief Registration array for serialization functions */
	PBLIB_RegData_t RegisteredFuncs[PBLIB_REG_MAX_ENTRY];

	/** \brief Mutex for registration array */
	uint32          RegTblMutex;
}PBLIB_AppData_t;

typedef uint32 (*PBLib_DecodeFuncPtr_t)(char *, uint32, const void *);
typedef uint32 (*PBLib_EncodeFuncPtr_t)(const void *, char *, uint32);

/*************************************************************************
** Local Function Prototypes
*************************************************************************/

/************************************************************************/
/** \brief Initialize the Protobuf Library (PBLIB)
**
**  \par Description
**       Protobuf Library initialization routine. This function performs all
**       the required startup steps to initialize PBL's registration
**       array.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
**  \retstmt Return codes from #OS_MutSemCreate \endcode
**  \endreturns
**
*************************************************************************/
int32 PBLIB_LibInit(void);

/************************************************************************/
/** \brief Register Command Message
**
**  \par Description
**       This function adds an entry to the serialization registration
**       array.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   msgID        A #CFE_SB_MsgId_t that specifies the
**  						   message ID if of the command
**
**  \param [in]   cmdCode      A #uint16 that specifies the command code
**
**  \param [in]   msgName      A #char pointer of the message name
**
**  \returns
**  #int32 Returns 0 if successful, -1 if MsgID is invalid, or
**  -2 if array is full
**  \endreturns
*************************************************************************/
int32 PBLIB_RegisterCmdMessage(CFE_SB_MsgId_t msgId, uint16 cmdCode, char *msgName);

/************************************************************************/
/** \brief Register Telemetry Message
**
**  \par Description
**       This function adds an entry to the serialization registration
**       array.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   msgID        A #CFE_SB_MsgId_t that specifies the
**  						   message ID if of the command
**
**  \param [in]   msgName      A #char pointer of the message name
**
**  \returns
**  #int32 Returns 0 if successful, -1 if MsgID is invalid, or
**  -2 if array is full
**  \endreturns
*************************************************************************/
int32 PBLIB_RegisterTlmMessage(CFE_SB_MsgId_t msgId, char *msgName);

/************************************************************************/
/** \brief Deregister Command  Message
**
**  \par Description
**       This function removes an entry from the serialization
**       registration array.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   msgID        A #CFE_SB_MsgId_t that specifies the
**  						   message ID if of the command
**
**  \param [in]   cmdCode      A #uint16 that specifies the command code
**
**  \returns
**  #int32 Returns 0 if successful, -1 if MsgID is invalid, or
**  -2 if MsgID with cmdCode not found
**  \endreturns
*************************************************************************/
int32 PBLIB_DeregisterCmdMessage(CFE_SB_MsgId_t msgId, uint16 cmdCode);

/************************************************************************/
/** \brief Deregister Telemetry Message
**
**  \par Description
**       This function removes an entry from the serialization
**       registration array.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   msgID        A #CFE_SB_MsgId_t that specifies the
**  						   message ID if of the command
**
**  \returns
**  #int32 Returns 0 if successful, -1 if MsgID is invalid, or
**  -2 if MsgID with cmdCode not found
**  \endreturns
*************************************************************************/
int32 PBLIB_DeregisterTlmMessage(CFE_SB_MsgId_t msgId);

/************************************************************************/
/** \brief Get Command Serialization Function
**
**  \par Description
**       This function looks up the address of a serialization function
**       registered with PBL.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   msgID        A #CFE_SB_MsgId_t that specifies the
**  						   message ID if of the command
**
**  \param [in]   cmdCode      A #uint16 that specifies the command code
**
**  \returns
**  #PBLib_EncodeFuncPtr_t Pointer to the serialization function address
**  or 0 if not found.
**  \endreturns
*************************************************************************/
PBLib_EncodeFuncPtr_t PBLIB_GetCmdSerializationFunc(CFE_SB_MsgId_t msgId, uint16 cmdCode);

/************************************************************************/
/** \brief Get Telemetry Serialization Function
**
**  \par Description
**       This function looks up the address of a serialization function
**       registered with PBL.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   msgID        A #CFE_SB_MsgId_t that specifies the
**  						   message ID if of the command
**
**  \returns
**  #PBLib_EncodeFuncPtr_t Pointer to the serialization function address
**  or 0 if not found.
**  \endreturns
*************************************************************************/
PBLib_EncodeFuncPtr_t PBLIB_GetTlmSerializationFunc(CFE_SB_MsgId_t msgId);

/************************************************************************/
/** \brief Get Command Deserialization Function
**
**  \par Description
**       This function looks up the address of a deserialization function
**       registered with PBL.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   msgID        A #CFE_SB_MsgId_t that specifies the
**  						   message ID if of the command
**
**  \param [in]   cmdCode      A #uint16 that specifies the command code
**
**  \returns
**  #PBLib_EncodeFuncPtr_t Pointer to the deserialization function address
**  or 0 if not found.
**  \endreturns
*************************************************************************/
PBLib_DecodeFuncPtr_t PBLIB_GetCmdDeserializationFunc(CFE_SB_MsgId_t msgId, uint16 cmdCode);

/************************************************************************/
/** \brief Get Telemetry Deserialization Function
**
**  \par Description
**       This function looks up the address of a deserialization function
**       registered with PBL.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   msgID        A #CFE_SB_MsgId_t that specifies the
**  						   message ID if of the command
**
**  \returns
**  #PBLib_EncodeFuncPtr_t Pointer to the deserialization function address
**  or 0 if not found.
**  \endreturns
*************************************************************************/
PBLib_DecodeFuncPtr_t PBLIB_GetTlmDeserializationFunc(CFE_SB_MsgId_t msgId);

/************************/
/*  End of File Comment */
/************************/
#endif
