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

#ifndef VC_TRANSMIT_H
#define VC_TRANSMIT_H

/************************************************************************
** Function Prototypes
*************************************************************************/

/************************************************************************/
/** \brief Custom function to initialize custom transmit data structure. 
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
int32 VC_Transmit_InitData(void);


/************************************************************************/
/** \brief Custom function to initialize all output channels.
**
**  \par Description
**       This function is called once at initialization and allows the
**       custom layer to provide specific functionality to initialize
**       internal objects. Subsequent calls are made when a new 
**       destination is initialized.
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
boolean VC_Transmit_Init(void);


/************************************************************************/
/** \brief Custom function to uninitialize all output channels.
**
**  \par Description
**       This function is called in preparation for loading a new
**       configuration, allowing the custom layer to do whatever it
**       needs with the current configuration before reconfiguration,
**       if anything. Also, called in cleanup to close and uninitialize
**       transmit resources.
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
boolean VC_Transmit_Uninit(void);


/************************************************************************/
/** \brief Custom function to uninitialize all output channels.
**
**  \par Description
**       This function is called in preparation for loading a new
**       configuration, allowing the custom layer to do whatever it
**       needs with the current configuration before reconfiguration,
**       if anything. Also, called in cleanup to close and uninitialize
**       transmit resources.
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
boolean VC_Transmit_Uninit(void);


/************************************************************************/
/** \brief Custom function to perform any critical cleanup after a 
**         failuse.
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
void VC_Transmit_Critical_Cleanup(void);


/************************************************************************/
/** \brief Custom function to verify a transmit address.
**
**  \par Description
**       This function is called to validate an address.
**
**  \par Assumptions, External Events, and Notes:
**       This function must be defined, but not all custom
**       layers will do anything in this function.
**
**  \param [in] Address the pointer to the address to verify.
**
**  \returns
**  TRUE if successful, FALSE otherwise.
**  \endreturns
**
*************************************************************************/
boolean VC_Address_Verification(const char *Address);


/************************************************************************/
/** \brief Custom function to update the transmit destination.
**
**  \par Description
**       This function is called to validate an address.
**
**  \par Assumptions, External Events, and Notes:
**       Uninit must be called before the destination can be updated.
**
**  \param [in] Address the pointer to the address of destination.
**  \param [in] Port the port number to send to.
**
**  \returns
**  TRUE if successful, FALSE otherwise.
**  \endreturns
**
*************************************************************************/
boolean VC_Update_Destination(const char *Address, uint16 Port);


/************************************************************************/
/** \brief Transmit data using a previously initialized channel.
**
**  \par Description
**       This function transmits data using a custom transmit resource.
**
**  \par Assumptions, External Events, and Notes:
**       None.
**
**  \param [in] Buffer points to a buffer containing the data to be sent.
**  \param [in] Size specifies the size of the data in bytes.
**
**  \returns
**  On success, returns the number of bytes sent. On error, -1 is
 *  returned
**  \endreturns
**
*************************************************************************/
int32 VC_SendData(uint32 ChannelID, const char* Buffer, uint32 Size);

#endif

/************************/
/*  End of File Comment */
/************************/
