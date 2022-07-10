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

#ifndef FAC_MISSION_CFG_H
#define FAC_MISSION_CFG_H


/*
** FAC Mission Configuration Parameter Definitions
*/

/* TODO:  Add mission configuration parameter definitions here, if necessary. */

#define FAC_SB_TIMEOUT         CFE_SB_PEND_FOREVER  /* Can be a value in milliseconds */

#define FAC_HK_TLM_MSG         (344)      /* FAC_HK_TLM_MID: 0x0B58  */
#define FAC_OUT_DATA_MSG       (345)      /* FAC_OUT_DATA_MID: 0x0B59  */
#define FAC_CMD_MSG            (346)      /* FAC_CMD_MID: 0x1B5A   */
#define FAC_SEND_HK_MSG        (347)      /* FAC_SEND_HK_MID: 0x1B5B  */
#define FAC_WAKEUP_MSG         (348)      /* FAC_WAKEUP_MID: 0x1B5C  */


#endif /* FAC_MISSION_CFG_H */

/************************/
/*  End of File Comment */
/************************/
