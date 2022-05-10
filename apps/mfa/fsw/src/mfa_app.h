/****************************************************************************
*
*   Copyright (c) 2022 Windhover Labs, L.L.C. All rights reserved.
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

#ifndef MFA_APP_H
#define MFA_APP_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "cfe.h"
#include "mfa_tbldefs.h"
#include "mfa_msg.h"


#define MFA_MAJOR_VERSION    (1)
#define MFA_MINOR_VERSION    (0)
#define MFA_PATCH_VERSION    (0)

typedef struct
{
	CFE_TBL_Handle_t  ConfigTblHdl;
	MFA_ConfigTbl_t* ConfigTblPtr;
    MFA_HKTlm_t      HkTlm;
    CFE_SB_PipeId_t SchPipeId;
} MFA_APPData_t;

typedef struct
{
    uint8  TlmHeader[CFE_SB_CMD_HDR_SIZE];
}MFA_NoArgCmd_t;

void MFA_AppMain(void);
uint32 MFA_AppInit(void);
uint32 MFA_InitTables();
uint32 MFA_InitEvents();
uint32 MFA_InitPipes();
#ifdef __cplusplus
}
#endif 

#endif /* MFA_APP_H */

/************************/
/*  End of File Comment */
/************************/
