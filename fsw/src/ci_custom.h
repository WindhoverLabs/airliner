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

#ifndef CI_CUSTOM_H
#define CI_CUSTOM_H

#include "cfe.h"

/************************************************************************/
/** \brief Initialize Custom Layer
**
**  \par Description
**       This function initializes the CI custom layer interface.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**  CFE_SUCCESS if successfully intitialized, -1 if not.
**  \endreturns
**
*************************************************************************/
int32 CI_InitCustom(void);

/************************************************************************/
/** \brief Read Message
**
**  \par Description
**       This function reads a message from the custom layer interface.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in/out]   buffer        Buffer to fill with read contents
**                                 
**  \param [in/out]   size          Read contents size
**
*************************************************************************/
void CI_ReadMessage(uint8* buffer, uint32* size);

/************************************************************************/
/** \brief Read Serialized Message
**
**  \par Description
**       This function reads a message from the serialized custom layer 
**       interface.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in/out]   buffer        Buffer to fill with read contents
**                                 
**  \param [in/out]   size          Read contents size
**
*************************************************************************/
void CI_ReadSerializedMessage(char* buffer, uint32* size);

/************************************************************************/
/** \brief Cleanup Custom
**
**  \par Description
**       This function closes interfaces in the custom layer.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void CI_CleanupCustom(void);

#endif
