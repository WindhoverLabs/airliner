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

#ifndef VM_MSGIDS_H
#define VM_MSGIDS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 ** \name VM Command Message IDs */
/** \{ */
#define VM_CMD_MID                          (0x1c70)      /**< \brief Msg ID for commands to VM. */
#define VM_SEND_HK_MID                      (0x1c6e)      /**< \brief Msg ID to request VM to send a housekeeping message. */
#define VM_WAKEUP_MID                       (0x1c6f)      /**< \brief Msg ID to wake up VM and perform its primary function. */
/** \} */

/**
 ** \name VM Telemetery Message ID */
/** \{ */
#define VM_HK_TLM_MID                       (0x0c6d)      /**< \brief Msg ID of the VM housekeeping message. */
#define VM_CONFIG_TLM_MID                   (0x0000)      /**< \brief Msg ID of the VM configuration message. */
/** \} */

#ifdef __cplusplus
}
#endif

#endif /* VM_MSGIDS_H */

/************************/
/*  End of File Comment */
/************************/
