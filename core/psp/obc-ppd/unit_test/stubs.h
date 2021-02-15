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

typedef struct
{
    uint32 CFE_ES_Main_Func;
    uint32 CFE_ES_ProcessCoreException_Func;
    uint32 CFE_TIME_Local1HzISR_Func;
} PSP_UT_CFE_CallCounts_t;

typedef struct
{
    uint32 HostTaskId;
    char   ReasonString[256];
    uint32 *ContextPointer;
    uint32 ContextSize;
} PSP_UT_CFE_ProcessCoreException_Args_t;

typedef struct
{
    uint32 StartType;
    uint32 StartSubtype;
    uint32 ModeId;
    char StartFilePath[256];
} PSP_UT_CFE_ES_Main_Args_t;

typedef struct
{
    PSP_UT_CFE_CallCounts_t CallCounts;
    PSP_UT_CFE_ES_Main_Args_t Main_Args;
    PSP_UT_CFE_ProcessCoreException_Args_t ProcessCoreException_Args;
} PSP_UT_CFE_StubData_t;

typedef struct
{
    PSP_UT_CFE_StubData_t CFE;
} PSP_UT_StubData_t;


extern PSP_UT_StubData_t PSP_UT_StubData;


void PSP_UT_ResetStubData(void);
