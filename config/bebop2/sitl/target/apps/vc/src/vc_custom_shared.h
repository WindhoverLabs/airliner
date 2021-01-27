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

#ifndef VC_CUSTOM_SHARED_H
#define VC_CUSTOM_SHARED_H
/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "vc_app.h"
#include "vc_custom.h"
#include "vc_dev_io_udp/vc_dev_io_udp.h"

/************************************************************************
** Local Defines
*************************************************************************/
/* start streaming command */
#define VC_STARTSTREAMING_CC  (2)
/* stop streaming command */
#define VC_STOPSTREAMING_CC   (3)


/************************************************************************
** Structure Declarations
*************************************************************************/
/** 
**  \brief Start Streaming Command
*/
typedef struct 
{
    uint8                  CmdHeader[CFE_SB_CMD_HDR_SIZE];
    /*! Address to start streaming to */
    char                   Address[VC_ADDRESS_LENGTH];
    /*! Port to start streaming to */
    uint16                 Port;
} VC_StartStreamCmd_t;


typedef enum {

/** \brief <tt> 'VC - ' </tt>
**  \event <tt> 'VC - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when a transmit resource encounters an 
**  socket error.
**
*/
    VC_SOCKET_ERR_EID = VC_EVT_CNT,
    
/** \brief <tt> 'VC - ' </tt>
**  \event <tt> 'VC - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when a device resource encounters an 
**  error.
**
*/
    VC_DEVICE_ERR_EID,
    
/** \brief <tt> 'VC - ' </tt>
**  \event <tt> 'VC - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when a start streaming command is 
**  received with a bad address command parameter.
**
*/
    VC_ADDR_ERR_EID,
    
/** \brief <tt> 'VC - ' </tt>
**  \event <tt> 'VC - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when a start streaming command is 
**  received with a NULL address command parameter.
**
*/
    VC_ADDR_NUL_ERR_EID,
    
/** \brief Number of custom events 
**
**  \par Limits:
**       int32
*/
    VC_CUSTOM_EVT_CNT
} VC_CustomEventIds_t;


/************************************************************************
** External Global Variables
*************************************************************************/

extern VC_AppData_t VC_AppData;

/************************************************************************
** Function Prototypes
*************************************************************************/

/************************************************************************/
/** \brief Determines if the maximum of event filters has been reached.
**
**  \par Description
**       This function checks if an index has reached the maximum
**       number of events.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]    ind    The current index to check.
**                             
**
**  \returns    boolean
**
*************************************************************************/
boolean VC_Custom_Max_Events_Not_Reached(int32 ind);


#endif /* VC_CUSTOM_SHARED_H */
