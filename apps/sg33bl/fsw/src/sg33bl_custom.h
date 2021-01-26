/****************************************************************************
*
*   Copyright (c) 2019 Windhover Labs, L.L.C. All rights reserved.
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

#ifndef SG33BL_CUSTOM_H
#define SG33BL_CUSTOM_H

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

/************************************************************************
** Defines
*************************************************************************/

/************************************************************************
** Structure Definitions
*************************************************************************/

/**
 * \brief SG33BL device status
 */
typedef enum
{
    /*! SG33BL status uninitialized */
    SG33BL_CUSTOM_UNINITIALIZED  = 0,
    /*! SG33BL status initialized */
    SG33BL_CUSTOM_INITIALIZED    = 1
} SG33BL_Custom_Status_t;


typedef enum
{
    SG33BL_PARSER_STATE_WAITING_FOR_UNKNOWN  = 0,
    SG33BL_PARSER_STATE_WAITING_FOR_HEADER   = 1,
    SG33BL_PARSER_STATE_WAITING_FOR_ID       = 2,
    SG33BL_PARSER_STATE_WAITING_FOR_ADDR     = 3,
    SG33BL_PARSER_STATE_WAITING_FOR_LEN      = 4,
    SG33BL_PARSER_STATE_WAITING_FOR_DATA1    = 5,
    SG33BL_PARSER_STATE_WAITING_FOR_DATA2    = 6,
    SG33BL_PARSER_STATE_WAITING_FOR_CHECKSUM = 7
} SG33BL_Custom_ParserState_t;


typedef struct
{
    /*! Device file descriptor */
    int                           DeviceFd;
    /*! The current device status */
    SG33BL_Custom_Status_t        Status;
    /*! The current baud */
    uint32                        Baud;
    SG33BL_Custom_ParserState_t   ParserState;
} SG33BL_AppCustomData_t;


#pragma pack(push, 1)
typedef struct
{
  uint8 header;
  uint8 id;
  uint8 addr;
  uint8 length;
} SG33BL_Frame_Start_t;
#pragma pack(pop)


#pragma pack(push, 1)
typedef struct 
{
  SG33BL_Frame_Start_t frame_start;
  uint16 data;
  uint8 checksum;
} SG33BL_Packet_t;
#pragma pack(pop)


#pragma pack(push, 1)
typedef struct 
{
  SG33BL_Frame_Start_t frame_start;
  uint8 checksum;
} SG33BL_Normal_Read_t;
#pragma pack(pop)

/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Function Prototypes
*************************************************************************/

/************************************************************************/
/** \brief Custom function to initialize custom device data structure. 
**
**  \par Description
**       This function is called on app startup, reload, restart etc
**       to initialize non-zero data.
**
*************************************************************************/
void SG33BL_Custom_InitData(void);

/************************************************************************/
/** \brief Custom function to initialize custom device(s).
**
**  \par Description
**       This function is called at initialization and allows the
**       custom layer to provide specific functionality to initialize
**       internal objects.
**
**  \returns
**  TRUE if successful, FALSE otherwise.
**  \endreturns
**
*************************************************************************/
boolean SG33BL_Custom_Init(void);


/************************************************************************/
/** \brief Custom function to uninitialize custom device(s).
**
**  \par Description
**       This function is called in preparation for loading a new
**       configuration, allowing the custom layer to do whatever it
**       needs with the current configuration before reconfiguration,
**       if anything. Also, called in cleanup to close and uninitialize
**       device resources.
**
**  \returns
**  TRUE if successful, FALSE otherwise.
**  \endreturns
**
*************************************************************************/
boolean SG33BL_Custom_Deinit(void);

/************************************************************************/
/** \brief Custom function to initialize custom events. 
**
**  \par Description
**       This function is called in init event before CFE_EVS_Register
**       to add custom events to the event filter table.
**
**  \par Assumptions, External Events, and Notes:
**       This function must be defined, but not all custom
**       layers will do anything in this function.
**
**  \returns
**       The number of events written to the filter table and -1 for 
**       failure i.e. CFE_EVS_MAX_EVENT_FILTERS reached.
**
*************************************************************************/
int32 SG33BL_Custom_Init_EventFilters(int32 ind, CFE_EVS_BinFilter_t *EventTbl);

boolean SG33BL_Custom_Write(uint8 Addr, uint16 Value);
boolean SG33BL_Custom_Read(uint8 Addr, uint16 *Value);


#ifdef __cplusplus
}
#endif 

#endif /* SG33BL_CUSTOM_H */
