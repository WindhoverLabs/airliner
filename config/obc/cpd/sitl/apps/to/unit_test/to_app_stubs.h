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

#ifndef TO_APP_STUBS_H
#define TO_APP_STUBS_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "to_custom_test_utils.h"

/************************************************************************
** Structure Declarations
*************************************************************************/
typedef struct
{
    int32 TO_Channel_OpenChannel_Return; 
    uint8 TO_Channel_State_Return;
    uint8 TO_Channel_State_Return1;
    uint8 TO_Channel_State_CallCount;
} TO_App_Returns_t;

/************************************************************************
** External Global Variables
*************************************************************************/
extern TO_App_Returns_t TO_App_Return;

/************************************************************************
** Function Prototypes (Stubs)
*************************************************************************/
int32 TO_Channel_OpenChannel(uint32 index, const char *ChannelName,
        const char *ConfigTableName, const char *ConfigTableFileName,
        TO_ChannelTbl_t *BackupTblPtr, const char *DumpTableName,
        const uint32 CfCntSemMax, const char *CfCntSemName);

void  TO_Channel_LockByIndex(uint16 index);

void  TO_Channel_UnlockByIndex(uint16 index);

uint8 TO_Channel_State(uint16 index);


#ifdef __cplusplus
}
#endif

#endif /* TO_APP_STUBS_H */
