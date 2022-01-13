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

#include "em91x_cmds.h"



/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/************************************************************************
** External Global Variables
*************************************************************************/
extern EM91X_AppData_t  EM91X_AppData;

/************************************************************************
** Global Variables
*************************************************************************/

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Function Prototypes
*************************************************************************/

/************************************************************************
** Function Definitions
*************************************************************************/



/******************************************************************************
**  Function:  EM91X_NoopCmd()
**
**  Purpose:
**      This function
**
**  Arguments:
**
**
**  Return:
**
*/
void EM91X_NoopCmd(void)
{
    CFE_EVS_SendEvent(EM91X_CMD_INF_EID, CFE_EVS_INFORMATION,
            "EM91X No-op command, Version %d.%d.%d.%d",
			EM91X_MAJOR_VERSION,
			EM91X_MINOR_VERSION,
			EM91X_REVISION,
			EM91X_MISSION_REV);

} /* end of EM91X_NoopCmd() */


/******************************************************************************
**  Function:  EM91X_ResetCtrsCmd()
**
**  Purpose:
**      This function
**
**  Arguments:
**
**
**  Return:
**
*/
void EM91X_ResetCtrsCmd(void)
{
    EM91X_AppData.HktLM.ErrCounter = 0;
    EM91X_AppData.HktLM.CmdCounter = 0;
} /* end of EM91X_ResetCtrsCmd() */



int32 EM91X_SetPasswordCmd(EM91X_PasswordCmd_t *MessagePtr)
{
    int32 status = EM91X_SUCCESS;

    /* TODO */

    return status;

} /* end of EM91X_SetPasswordCmd() */



int32 EM91X_EnterPasswordCmd(EM91X_PasswordCmd_t *MessagePtr)
{
    int32 status = EM91X_SUCCESS;

    /* TODO */

    return status;

} /* end of EM91X_EnterPasswordCmd() */



int32 EM91X_AntSelCmd(EM91X_AntSelCmd_t *MsgPtr)
{
    int32 status = EM91X_SUCCESS;

    /* TODO */

    return status;

}
