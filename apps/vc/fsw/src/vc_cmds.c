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

/************************************************************************
** Includes
*************************************************************************/
#include "vc_cmds.h"
#include "vc_app.h"
#include <string.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* NO-OP Command                                                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VC_NoopCmd(CFE_SB_Msg_t* MsgPtr)
{
    uint16 ExpectedLength = sizeof(VC_NoArgCmd_t);

    /* Verify command packet length */
    if (VC_VerifyCmdLength(MsgPtr, ExpectedLength))
    {
        VC_AppData.HkTlm.usCmdCnt++;

        (void) CFE_EVS_SendEvent(VC_NOOP_INF_EID, CFE_EVS_INFORMATION,
                            "Recvd NOOP cmd, Version %d.%d.%d.%d",
                            VC_MAJOR_VERSION,
                            VC_MINOR_VERSION,
                            VC_REVISION,
                            VC_MISSION_REV);
    }

    return;

} /* End of VC_NoopCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Reset Command                                                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VC_ResetCmd(CFE_SB_Msg_t* MsgPtr)
{
    uint16 ExpectedLength = sizeof(VC_NoArgCmd_t); 

    /* Verify command packet length */
    if (VC_VerifyCmdLength(MsgPtr, ExpectedLength))
    {
        VC_AppData.HkTlm.usCmdCnt = 0;
        VC_AppData.HkTlm.usCmdErrCnt = 0;
        (void) CFE_EVS_SendEvent(VC_RESET_INF_EID, CFE_EVS_INFORMATION,
                            "Recvd RESET cmd");
                
    }

    return;

} /* End of VC_ResetCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Report Housekeeping                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VC_ReportHousekeeping(CFE_SB_Msg_t* MsgPtr)
{
    uint16 ExpectedLength = sizeof(VC_NoArgCmd_t); 
    
    /* Verify command packet length */
    if (VC_VerifyCmdLength(MsgPtr, ExpectedLength))
    {
        VC_AppData.HkTlm.State = VC_AppData.AppState; 
        
        /* Send housekeeping telemetry packet */
        CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&VC_AppData.HkTlm);
        CFE_SB_SendMsg((CFE_SB_Msg_t*)&VC_AppData.HkTlm);
    }

    return;

} /* End of VC_HousekeepingCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Command Length                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean VC_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
                           uint16 usExpectedLen)
{
    boolean bResult  = TRUE;
    uint16  usMsgLen = 0;

    if (MsgPtr != NULL)
    {
        usMsgLen = CFE_SB_GetTotalMsgLength(MsgPtr);

        if (usExpectedLen != usMsgLen)
        {
            bResult = FALSE;
            CFE_SB_MsgId_t MsgId = CFE_SB_GetMsgId(MsgPtr);
            uint16 usCmdCode = CFE_SB_GetCmdCode(MsgPtr);

            (void) CFE_EVS_SendEvent(VC_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            VC_AppData.HkTlm.usCmdErrCnt++;
        }
    }

    return (bResult);
} /* End of VC_VerifyCmdLength() */

/************************/
/*  End of File Comment */
