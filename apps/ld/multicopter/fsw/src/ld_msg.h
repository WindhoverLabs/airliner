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

#ifndef LD_MSG_H
#define LD_MSG_H

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

/************************************************************************
 ** Local Defines
 *************************************************************************/

/************************************************************************
 ** LD Command Codes
 *************************************************************************/

/** \ldcmd Noop 
 **
 **  \par Description
 **       Implements the Noop command that demonstrates the LD task is alive
 **
 **  \ldcmdmnemonic \LD_NOOP
 **
 **  \par Command Structure
 **       #LD_NoArgCmd_t
 **
 **  \par Command Verification
 **       Successful execution of this command may be verified with
 **       the following telemetry:
 **       - \b \c \LD_CMDACPTCNT - command counter will increment
 **       - The #LD_CMD_INF_EID informational event message will be
 **         generated when the command is received
 **
 **  \par Error Conditions
 **       This command may fail for the following reason(s):
 **       - Command packet length not as expected
 **
 **  \par Evidence of failure may be found in the following telemetry:
 **       - \b \c \LD_CMDRJCTCNT - command error counter will increment
 **       - Error specific event message #LD_MSGID_ERR_EID
 **
 **  \par Criticality
 **       None
 **
 **  \sa #LD_RESET_CC
 */
#define LD_NOOP_CC                 (0)

/** \ldcmd Reset Counters
 **
 **  \par Description
 **       Resets the LD housekeeping counters
 **
 **  \ldcmdmnemonic \LD_TLMRST
 **
 **  \par Command Structure
 **       #LD_NoArgCmd_t
 **
 **  \par Command Verification
 **       Successful execution of this command may be verified with
 **       the following telemetry:
 **       - \b \c \LD_CMDACTPCNT       - command counter will be cleared
 **       - \b \c \LD_CMDRJCTCNT       - command error counter will be cleared
 **       - The #LD_CMD_INF_EID debug event message will be
 **         generated when the command is executed
 **
 **  \par Error Conditions
 **       This command may fail for the following reason(s):
 **       - Command packet length not as expected
 **
 **  \par Evidence of failure may be found in the following telemetry:
 **       - \b \c \LD_CMDRJCTCNT - command error counter will increment
 **       - Error specific event message #LD_MSGID_ERR_EID
 **
 **  \par Criticality
 **       None
 **
 **  \sa #LD_NOOP_CC
 */
#define LD_RESET_CC                (1)

#define LD_MODE_AUTO_CC            (2)

#define LD_MODE_MANUAL_CC          (3)

#define LD_MODE_MIXED_CC           (4)

/************************************************************************
 ** Local Structure Declarations
 *************************************************************************/

/** 
 **  \brief No Arguments Command
 **  For command details see #LD_NOOP_CC, #LD_RESET_CC
 **  Also see #LD_SEND_HK_MID
 */
typedef struct
{
    uint8 ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
}LD_NoArgCmd_t;

/** \ldtlm Housekeeping data
 **
 **  \par Description
 **       LD application housekeeping data
 **
 **  \par Criticality
 **       None
 */
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8 TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \ldtlmmnemonic \LD_CMDACPTCNT
     \brief Count of accepted commands */
    uint8 usCmdCnt;

    /** \ldtlmmnemonic \LD_CMDRJCTCNT
     \brief Count of failed commands */
    uint8 usCmdErrCnt;

    uint8 state;

    uint8 mode;

}LD_HkTlm_t;


/**
 * \brief 
 */
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8 TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /**\brief   */
    boolean GC_MinThrust;

    /**\brief   */
    boolean GC_AltitudeLock;

    /**\brief   */
    boolean GC_PositionLock;

    /**\brief   */
    boolean GC_InDescent;

    /**\brief   */
    boolean GC_HitGround;

    /**\brief   */
    boolean GC_HorMovement;

    /**\brief   */
    boolean GC_VertMovement;

    /**\brief   */
    boolean GC_ManualControlIdlingOrAutomatic;

    /**\brief   */
    boolean GroundContact;

    /**\brief   */
    boolean LD_GC_history_state;
    
    /**\brief   */
    boolean LD_MinThrust;

    /**\brief   */
    boolean LD_Rotation;
    
    /**\brief   */
    boolean LD_HorMovement;
    
    /**\brief   */
    boolean LD_PositionLock;

    /**\brief   */
    boolean Landed;

} LD_Diag_t;

#ifdef __cplusplus
}
#endif

#endif /* LD_MSG_H */

/************************/
/*  End of File Comment */
/************************/
