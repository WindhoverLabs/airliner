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

#include "vc_custom_shared.h"
#include "vc_cmds.h"
#include <string.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process New Custom Commands.  Raise an error event if the       */
/* command is unknown.                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VC_ProcessNewCustomCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uint16 inSize = CFE_SB_GetTotalMsgLength(MsgPtr);
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);

        switch (uiCmdCode)
        {
            case VC_STARTSTREAMING_CC: 
            {
                VC_StartStreamingCmd(MsgPtr); 
                break;
            }
            case VC_STOPSTREAMING_CC:
            {
                VC_StopStreamingCmd(MsgPtr);
                break;
            }
            default:
            {
                VC_AppData.HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(VC_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid cmdId (%u)", (unsigned int)uiCmdCode);
                break;
            }
        }
    }
}


void VC_StartStreamingCmd(CFE_SB_Msg_t* MsgPtr)
{
    VC_StartStreamCmd_t *CmdPtr = 0;
    
    uint16 ExpectedLength = sizeof(VC_StartStreamCmd_t);

    /* Verify command packet length */
    if (VC_VerifyCmdLength(MsgPtr, ExpectedLength))
    {
        (void) CFE_EVS_SendEvent(VC_CMD_INF_EID, CFE_EVS_INFORMATION, "Start Streaming command received");

        if (VC_AppData.AppState == VC_INITIALIZED)
        {
            CmdPtr = ((VC_StartStreamCmd_t *) MsgPtr);
        
            /* 
            ** NUL terminate the very end of the address string as a
            ** safety measure
            */
            CmdPtr->Address[VC_ADDRESS_LENGTH - 1] = '\0';

            /* 
            ** Check if the address string is a nul string
            */ 
            if(strlen(CmdPtr->Address) == 0)
            {
                VC_AppData.HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(VC_ADDR_NUL_ERR_EID, CFE_EVS_ERROR,
                                "NUL (empty) string specified for address");
            }
            /* Check if the address is valid */
            else if (FALSE == VC_Address_Verification(CmdPtr->Address))
            {
                VC_AppData.HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(VC_ADDR_ERR_EID, CFE_EVS_ERROR,
                                "Invalid string specified for address");
            }
            /* Update the configuration */
            else if (FALSE == VC_Update_Destination(CmdPtr->Address, CmdPtr->Port))
            {   
                VC_AppData.HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(VC_INIT_ERR_EID, CFE_EVS_ERROR,
                                "Destination update failed in cmd start streaming");
            }
            ///* Call VC_Transmit_Uninit() */
            //else if (FALSE == VC_Transmit_Uninit())
            //{
                //VC_AppData.HkTlm.usCmdErrCnt++;
                //(void) CFE_EVS_SendEvent(VC_UNINIT_ERR_EID, CFE_EVS_ERROR,
                                //"VC_Transmit_Uninit failed in cmd start streaming");
            //}
            ///* Call VC_Transmit_Init() */
            //else if (FALSE == VC_Transmit_Init())
            //{
                //VC_AppData.HkTlm.usCmdErrCnt++;
                //(void) CFE_EVS_SendEvent(VC_INIT_ERR_EID, CFE_EVS_ERROR,
                                //"VC_Transmit_Init failed in cmd start streaming");
            //}
            /* Call VC_Devices_Start() */
            else if (FALSE == VC_Devices_Start())
            {
                VC_AppData.HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(VC_INIT_ERR_EID, CFE_EVS_ERROR,
                                "VC_Devices_Start failed in cmd start streaming");
            }
            /* Success! */
            else
            {
                VC_AppData.AppState = VC_STREAMING;
                VC_AppData.HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(VC_CMD_INF_EID, CFE_EVS_INFORMATION, 
                    "VC started streaming to %s:%u", CmdPtr->Address, CmdPtr->Port);
            }
        }
        else
        {
            VC_AppData.HkTlm.usCmdErrCnt++;
            (void) CFE_EVS_SendEvent(VC_CMD_ERR_EID, CFE_EVS_ERROR, "VC is already streaming");
        }
    }
    return;
} /* End of VC_StartStreamingCmd() */


void VC_StopStreamingCmd(CFE_SB_Msg_t* MsgPtr)
{
    uint16 ExpectedLength = sizeof(VC_NoArgCmd_t);

    /* Verify command packet length */
    if (VC_VerifyCmdLength(MsgPtr, ExpectedLength))
    {
        (void) CFE_EVS_SendEvent(VC_CMD_INF_EID, CFE_EVS_INFORMATION, "Stop Streaming command received");

        if (VC_AppData.AppState == VC_STREAMING)
        {
            if(FALSE == VC_Devices_Stop())
            {
                VC_AppData.HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(VC_UNINIT_ERR_EID, CFE_EVS_ERROR,
                                "VC_Devices_Stop() failed");
            }
            else
            {
                VC_AppData.AppState = VC_INITIALIZED;
                VC_AppData.HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(VC_CMD_INF_EID ,CFE_EVS_INFORMATION, 
                    "VC stopped streaming");
            }
        }
        else
        {
            VC_AppData.HkTlm.usCmdErrCnt++;
            (void) CFE_EVS_SendEvent(VC_CMD_ERR_EID, CFE_EVS_ERROR, "VC is already not streaming");
        }
    }
    return;
} /* End of VC_StopStreamingCmd() */


boolean VC_Custom_Max_Events_Not_Reached(int32 ind)
{
    boolean returnBool = FALSE;
    if ((ind < VC_MAX_EVENT_FILTERS) && (ind > 0))
    {
        returnBool = TRUE;
    }

    return (returnBool);
}


int32 VC_Custom_Init_EventFilters(int32 ind, CFE_EVS_BinFilter_t *EventTbl)
{
    int32 customEventCount = ind;
    
    /* Null check */
    if(0 == EventTbl)
    {
        customEventCount = VC_CUSTOM_ERROR_CODE;
        goto end_of_function;
    }

    if(TRUE == VC_Custom_Max_Events_Not_Reached(customEventCount))
    {
        EventTbl[  customEventCount].EventID = VC_SOCKET_ERR_EID;
        EventTbl[customEventCount++].Mask    = CFE_EVS_FIRST_16_STOP;
    }
    else
    {
        customEventCount = VC_CUSTOM_ERROR_CODE;
        goto end_of_function;
    }
    
    if(TRUE == VC_Custom_Max_Events_Not_Reached(customEventCount))
    {
        EventTbl[  customEventCount].EventID = VC_DEVICE_ERR_EID;
        EventTbl[customEventCount].Mask    = CFE_EVS_FIRST_16_STOP;
    }
    else
    {
        customEventCount = VC_CUSTOM_ERROR_CODE;
        goto end_of_function;
    }

end_of_function:

    return (customEventCount);
}


