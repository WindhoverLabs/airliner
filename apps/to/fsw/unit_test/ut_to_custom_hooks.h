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

#include "cfe.h"

#include "to_channel.h"

int32  Ut_TO_Custom_Init(void);
int32  Ut_TO_Custom_InitEvent(int32 ind);
int32  Ut_TO_OutputChannel_CustomBuildup(uint32 index);
int32  Ut_TO_OutputChannel_CustomTeardown(uint32 index);
void   Ut_TO_OutputChannel_CustomCleanupAll(void);
void   Ut_TO_OutputChannel_ProcessNewCustomCmds(CFE_SB_Msg_t* MsgPtr);
uint8  Ut_TO_OutputChannel_Status(uint32 index);
uint32 Ut_TO_GetCustomVersion(void);
void   Ut_TO_PrintCustomVersion(void);
void   Ut_TO_ResetCustomChannelCounters(void);
void   Ut_TO_UpdateCustomDataFromTable(uint16 ChannelID,
                                  osalbool sendEvent,
                                  osalbool abandonCurrentMsg);
void   Ut_TO_OutputChannel_SendTelemetry(uint32 index);
