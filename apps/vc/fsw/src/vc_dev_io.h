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


#ifndef VC_DEV_IO_H
#define VC_DEV_IO_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/

/************************************************************************
** Defines
*************************************************************************/

/************************************************************************
** Structure Definitions
*************************************************************************/

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
**  \par Assumptions, External Events, and Notes:
**       This function must be defined, but not all custom
**       layers will do anything in this function.
**
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt Return codes from #CFE_EVS_Register  \endcode
**  \endreturns
**
*************************************************************************/
int32 VC_Devices_InitData(void);

/************************************************************************/
/** \brief Custom function to start an initialized custom device. 
**
**  \par Description
**       This function is called after initialization to start a 
**       configured device action. It must also be called before 
**       re-initialization i.e. configuration changes.
**
**  \par Assumptions, External Events, and Notes:
**       This function must be defined, but not all custom
**       layers will do anything in this function.
**
**  \returns
**  TRUE if successful, FALSE otherwise.
**  \endreturns
**
*************************************************************************/
boolean VC_Devices_Start(void);


/************************************************************************/
/** \brief Custom function to stop an initialized custom device. 
**
**  \par Description
**       This function is called to stop a configured device action.
**       It may also be called after reconfiguration.
**
**  \par Assumptions, External Events, and Notes:
**       This function must be defined, but not all custom
**       layers will do anything in this function.
**
**  \returns
**  TRUE if successful, FALSE otherwise.
**  \endreturns
**
*************************************************************************/
boolean VC_Devices_Stop(void);


/************************************************************************/
/** \brief Custom function to initialize all custom device input 
**         channels.
**
**  \par Description
**       This function is called at initialization and allows the
**       custom layer to provide specific functionality to initialize
**       internal objects.
**
**  \par Assumptions, External Events, and Notes:
**       This function must be defined, but not all custom
**       layers will do anything in this function.
**
**  \returns
**  TRUE if successful, FALSE otherwise.
**  \endreturns
**
*************************************************************************/
boolean VC_Devices_Init(void);


/************************************************************************/
/** \brief Custom function to uninitialize all custom device input
**         channels.
**
**  \par Description
**       This function is called in preparation for loading a new
**       configuration, allowing the custom layer to do whatever it
**       needs with the current configuration before reconfiguration,
**       if anything. Also, called before application exit.
**
**  \par Assumptions, External Events, and Notes:
**       This function must be defined, but not all custom
**       layers will do anything in this function.
**
**  \returns
**  TRUE if successful, FALSE otherwise.
**  \endreturns
**
*************************************************************************/
boolean VC_Devices_Uninit(void);


/************************************************************************/
/** \brief Custom function to perform any critical cleanup after a 
**         failure.
**
**  \par Description
**       This function is called in cleanup to close and uninitialize
**       device resources.
**
**  \par Assumptions, External Events, and Notes:
**       This function must be defined, but not all custom
**       layers will do anything in this function.
**
*************************************************************************/
void VC_Devices_Critical_Cleanup(void);

#endif

