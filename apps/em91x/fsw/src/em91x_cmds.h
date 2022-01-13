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
#ifndef EM91X_CMDS_H
#define EM91X_CMDS_H


/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"


/************************************************************************/
/** \brief Process noop command
**
**  \par Description
**       Processes a noop ground command.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \sa #EM91X_NOOP_CC
**
*************************************************************************/
void EM91X_NoopCmd(void);


/************************************************************************/
/** \brief Process reset counters command
**
**  \par Description
**       Processes a reset counters ground command which will reset
**       the command error and command execution counters to zero.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \sa #EM91X_RESET_CC
**
*************************************************************************/
void EM91X_ResetCtrsCmd(void);


/* TODO */
int32 EM91X_SetPasswordCmd(EM91X_PasswordCmd_t *MessagePtr);
int32 EM91X_EnterPasswordCmd(EM91X_PasswordCmd_t *MessagePtr);
int32 EM91X_AntSelCmd(EM91X_AntSelCmd_t *MsgPtr);



#endif /* _cf_cmds_ */

/************************/
/*  End of File Comment */
/************************/
