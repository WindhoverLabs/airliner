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

#ifndef FLOW_MSG_H
#define FLOW_MSG_H

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
** FLOW Command Codes
*************************************************************************/

/** \flowcmd Noop 
**  
**  \par Description
**       Implements the Noop command that demonstrates the FLOW task is alive
**
**  \flowcmdmnemonic \FLOW_NOOP
**
**  \par Command Structure
**       #FLOW_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \FLOW_CMDACPTCNT - command counter will increment
**       - The #FLOW_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \FLOW_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #FLOW_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #FLOW_RESET_CC
**  \sa #FLOW_SET_HFOV_CC
**  \sa #FLOW_SET_TILE_SIZE_CC
**  \sa #FLOW_SET_BLOCKS_CC
**  \sa #FLOW_SET_OP_RATE_CC
**  \sa #FLOW_SET_IMG_WIDTH_CC
**  \sa #FLOW_SET_IMG_HEIGHT_CC
**  \sa #FLOW_SET_SEARCH_SIZE_CC
**  \sa #FLOW_SET_FEATURE_THRESHOLD_CC
**  \sa #FLOW_SET_VALUE_THRESHOLD_CC
**  \sa #FLOW_SET_MEAN_COUNT_THRESHOLD_CC
**  \sa #FLOW_SEND_DIAG_CC
*/
#define FLOW_NOOP_CC                 (0)

/** \flowcmd Reset Counters
**  
**  \par Description
**       Resets the FLOW housekeeping counters
**
**  \flowcmdmnemonic \FLOW_TLMRST
**
**  \par Command Structure
**       #FLOW_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \FLOW_CMDACTPCNT       - command counter will be cleared
**       - \b \c \FLOW_CMDRJCTCNT       - command error counter will be cleared
**       - The #FLOW_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \FLOW_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #FLOW_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #FLOW_NOOP_CC
**  \sa #FLOW_SET_HFOV_CC
**  \sa #FLOW_SET_TILE_SIZE_CC
**  \sa #FLOW_SET_BLOCKS_CC
**  \sa #FLOW_SET_OP_RATE_CC
**  \sa #FLOW_SET_IMG_WIDTH_CC
**  \sa #FLOW_SET_IMG_HEIGHT_CC
**  \sa #FLOW_SET_SEARCH_SIZE_CC
**  \sa #FLOW_SET_FEATURE_THRESHOLD_CC
**  \sa #FLOW_SET_VALUE_THRESHOLD_CC
**  \sa #FLOW_SET_MEAN_COUNT_THRESHOLD_CC
**  \sa #FLOW_SEND_DIAG_CC
*/
#define FLOW_RESET_CC                			(1)

/** \flowcmd Horizontal field of view
**
**  \par Description
**       Sets horizontal field of view
**
**  \flowcmdmnemonic \FLOW_TLMRST
**
**  \par Command Structure
**       #FLOW_SetFloat_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \FLOW_CMDACTPCNT       - command counter will be cleared
**       - \b \c \FLOW_CMDRJCTCNT       - command error counter will be cleared
**       - The #FLOW_CMD_ACCEPTED_EID debug event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c \FLOW_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #FLOW_CMD_REJECTED_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #FLOW_NOOP_CC
**  \sa #FLOW_RESET_CC
**  \sa #FLOW_SET_TILE_SIZE_CC
**  \sa #FLOW_SET_BLOCKS_CC
**  \sa #FLOW_SET_OP_RATE_CC
**  \sa #FLOW_SET_IMG_WIDTH_CC
**  \sa #FLOW_SET_IMG_HEIGHT_CC
**  \sa #FLOW_SET_SEARCH_SIZE_CC
**  \sa #FLOW_SET_FEATURE_THRESHOLD_CC
**  \sa #FLOW_SET_VALUE_THRESHOLD_CC
**  \sa #FLOW_SET_MEAN_COUNT_THRESHOLD_CC
**  \sa #FLOW_SEND_DIAG_CC
*/
#define FLOW_SET_HFOV_CC                		(2)

/** \flowcmd Sliding window tile size
**
**  \par Description
**       Sets sliding window tile size
**
**  \flowcmdmnemonic \FLOW_TLMRST
**
**  \par Command Structure
**       #FLOW_SetInt32_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \FLOW_CMDACTPCNT       - command counter will be cleared
**       - \b \c \FLOW_CMDRJCTCNT       - command error counter will be cleared
**       - The #FLOW_CMD_ACCEPTED_EID debug event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c \FLOW_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #FLOW_CMD_REJECTED_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #FLOW_NOOP_CC
**  \sa #FLOW_RESET_CC
**  \sa #FLOW_SET_HFOV_CC
**  \sa #FLOW_SET_BLOCKS_CC
**  \sa #FLOW_SET_OP_RATE_CC
**  \sa #FLOW_SET_IMG_WIDTH_CC
**  \sa #FLOW_SET_IMG_HEIGHT_CC
**  \sa #FLOW_SET_SEARCH_SIZE_CC
**  \sa #FLOW_SET_FEATURE_THRESHOLD_CC
**  \sa #FLOW_SET_VALUE_THRESHOLD_CC
**  \sa #FLOW_SET_MEAN_COUNT_THRESHOLD_CC
**  \sa #FLOW_SEND_DIAG_CC
*/
#define FLOW_SET_TILE_SIZE_CC            		(3)

/** \flowcmd Number of blocks of sliding window
**
**  \par Description
**       Sets number of blocks of sliding window
**
**  \flowcmdmnemonic \FLOW_TLMRST
**
**  \par Command Structure
**       #FLOW_SetInt32_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \FLOW_CMDACTPCNT       - command counter will be cleared
**       - \b \c \FLOW_CMDRJCTCNT       - command error counter will be cleared
**       - The #FLOW_CMD_ACCEPTED_EID debug event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c \FLOW_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #FLOW_CMD_REJECTED_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #FLOW_NOOP_CC
**  \sa #FLOW_RESET_CC
**  \sa #FLOW_SET_HFOV_CC
**  \sa #FLOW_SET_TILE_SIZE_CC
**  \sa #FLOW_SET_OP_RATE_CC
**  \sa #FLOW_SET_IMG_WIDTH_CC
**  \sa #FLOW_SET_IMG_HEIGHT_CC
**  \sa #FLOW_SET_SEARCH_SIZE_CC
**  \sa #FLOW_SET_FEATURE_THRESHOLD_CC
**  \sa #FLOW_SET_VALUE_THRESHOLD_CC
**  \sa #FLOW_SET_MEAN_COUNT_THRESHOLD_CC
**  \sa #FLOW_SEND_DIAG_CC
*/
#define FLOW_SET_BLOCKS_CC               		(4)

/** \flowcmd Output rate
**
**  \par Description
**       Sets output rate
**
**  \flowcmdmnemonic \FLOW_TLMRST
**
**  \par Command Structure
**       #FLOW_SetInt32_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \FLOW_CMDACTPCNT       - command counter will be cleared
**       - \b \c \FLOW_CMDRJCTCNT       - command error counter will be cleared
**       - The #FLOW_CMD_ACCEPTED_EID debug event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c \FLOW_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #FLOW_CMD_REJECTED_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #FLOW_NOOP_CC
**  \sa #FLOW_RESET_CC
**  \sa #FLOW_SET_HFOV_CC
**  \sa #FLOW_SET_TILE_SIZE_CC
**  \sa #FLOW_SET_BLOCKS_CC
**  \sa #FLOW_SET_IMG_WIDTH_CC
**  \sa #FLOW_SET_IMG_HEIGHT_CC
**  \sa #FLOW_SET_SEARCH_SIZE_CC
**  \sa #FLOW_SET_FEATURE_THRESHOLD_CC
**  \sa #FLOW_SET_VALUE_THRESHOLD_CC
**  \sa #FLOW_SET_MEAN_COUNT_THRESHOLD_CC
**  \sa #FLOW_SEND_DIAG_CC
*/
#define FLOW_SET_OP_RATE_CC              		(5)

/** \flowcmd Image width
**  \par Description
**       Sets image width
**
**  \flowcmdmnemonic \FLOW_TLMRST
**
**  \par Command Structure
**       #FLOW_SetInt32_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \FLOW_CMDACTPCNT       - command counter will be cleared
**       - \b \c \FLOW_CMDRJCTCNT       - command error counter will be cleared
**       - The #FLOW_CMD_ACCEPTED_EID debug event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c \FLOW_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #FLOW_CMD_REJECTED_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #FLOW_NOOP_CC
**  \sa #FLOW_RESET_CC
**  \sa #FLOW_SET_HFOV_CC
**  \sa #FLOW_SET_TILE_SIZE_CC
**  \sa #FLOW_SET_BLOCKS_CC
**  \sa #FLOW_SET_OP_RATE_CC
**  \sa #FLOW_SET_SEARCH_SIZE_CC
**  \sa #FLOW_SET_FEATURE_THRESHOLD_CC
**  \sa #FLOW_SET_VALUE_THRESHOLD_CC
**  \sa #FLOW_SET_MEAN_COUNT_THRESHOLD_CC
**  \sa #FLOW_SEND_DIAG_CC
*/
#define FLOW_SET_IMG_WIDTH_CC            		(6)

/** \flowcmd Image height
**
**  \par Description
**       Sets image height
**
**  \flowcmdmnemonic \FLOW_TLMRST
**
**  \par Command Structure
**       #FLOW_SetInt32_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \FLOW_CMDACTPCNT       - command counter will be cleared
**       - \b \c \FLOW_CMDRJCTCNT       - command error counter will be cleared
**       - The #FLOW_CMD_ACCEPTED_EID debug event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c \FLOW_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #FLOW_CMD_REJECTED_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #FLOW_NOOP_CC
**  \sa #FLOW_RESET_CC
**  \sa #FLOW_SET_HFOV_CC
**  \sa #FLOW_SET_TILE_SIZE_CC
**  \sa #FLOW_SET_BLOCKS_CC
**  \sa #FLOW_SET_OP_RATE_CC
**  \sa #FLOW_SET_SEARCH_SIZE_CC
**  \sa #FLOW_SET_FEATURE_THRESHOLD_CC
**  \sa #FLOW_SET_VALUE_THRESHOLD_CC
**  \sa #FLOW_SET_MEAN_COUNT_THRESHOLD_CC
**  \sa #FLOW_SEND_DIAG_CC
*/
#define FLOW_SET_IMG_HEIGHT_CC           		(7)

/** \flowcmd Image search size
**
**  \par Description
**       Sets image search size
**
**  \flowcmdmnemonic \FLOW_TLMRST
**
**  \par Command Structure
**       #FLOW_SetInt32_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \FLOW_CMDACTPCNT       - command counter will be cleared
**       - \b \c \FLOW_CMDRJCTCNT       - command error counter will be cleared
**       - The #FLOW_CMD_ACCEPTED_EID debug event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c \FLOW_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #FLOW_CMD_REJECTED_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #FLOW_NOOP_CC
**  \sa #FLOW_RESET_CC
**  \sa #FLOW_SET_HFOV_CC
**  \sa #FLOW_SET_TILE_SIZE_CC
**  \sa #FLOW_SET_BLOCKS_CC
**  \sa #FLOW_SET_OP_RATE_CC
**  \sa #FLOW_SET_IMG_WIDTH_CC
**  \sa #FLOW_SET_IMG_HEIGHT_CC
**  \sa #FLOW_SET_FEATURE_THRESHOLD_CC
**  \sa #FLOW_SET_VALUE_THRESHOLD_CC
**  \sa #FLOW_SET_MEAN_COUNT_THRESHOLD_CC
**  \sa #FLOW_SEND_DIAG_CC
*/
#define FLOW_SET_SEARCH_SIZE_CC          		(8)

/** \flowcmd Default feature threshold
**
**  \par Description
**       Sets default feature threshold
**
**  \flowcmdmnemonic \FLOW_TLMRST
**
**  \par Command Structure
**       #FLOW_SetInt32_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \FLOW_CMDACTPCNT       - command counter will be cleared
**       - \b \c \FLOW_CMDRJCTCNT       - command error counter will be cleared
**       - The #FLOW_CMD_ACCEPTED_EID debug event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c \FLOW_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #FLOW_CMD_REJECTED_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #FLOW_NOOP_CC
**  \sa #FLOW_RESET_CC
**  \sa #FLOW_SET_HFOV_CC
**  \sa #FLOW_SET_TILE_SIZE_CC
**  \sa #FLOW_SET_BLOCKS_CC
**  \sa #FLOW_SET_OP_RATE_CC
**  \sa #FLOW_SET_IMG_WIDTH_CC
**  \sa #FLOW_SET_IMG_HEIGHT_CC
**  \sa #FLOW_SET_SEARCH_SIZE_CC
**  \sa #FLOW_SET_VALUE_THRESHOLD_CC
**  \sa #FLOW_SET_MEAN_COUNT_THRESHOLD_CC
**  \sa #FLOW_SEND_DIAG_CC
*/
#define FLOW_SET_FEATURE_THRESHOLD_CC    		(9)

/** \flowcmd Default value threshold
**
**  \par Description
**       Sets default value threshold
**
**  \flowcmdmnemonic \FLOW_TLMRST
**
**  \par Command Structure
**       #FLOW_SetInt32_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \FLOW_CMDACTPCNT       - command counter will be cleared
**       - \b \c \FLOW_CMDRJCTCNT       - command error counter will be cleared
**       - The #FLOW_CMD_ACCEPTED_EID debug event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c \FLOW_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #FLOW_CMD_REJECTED_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #FLOW_NOOP_CC
**  \sa #FLOW_RESET_CC
**  \sa #FLOW_SET_HFOV_CC
**  \sa #FLOW_SET_TILE_SIZE_CC
**  \sa #FLOW_SET_BLOCKS_CC
**  \sa #FLOW_SET_OP_RATE_CC
**  \sa #FLOW_SET_IMG_WIDTH_CC
**  \sa #FLOW_SET_IMG_HEIGHT_CC
**  \sa #FLOW_SET_SEARCH_SIZE_CC
**  \sa #FLOW_SET_FEATURE_THRESHOLD_CC
**  \sa #FLOW_SET_MEAN_COUNT_THRESHOLD_CC
**  \sa #FLOW_SEND_DIAG_CC
*/
#define FLOW_SET_VALUE_THRESHOLD_CC      		(10)

/** \flowcmd Mean count threshold
**
**  \par Description
**       Sets mean count threshold
**
**  \flowcmdmnemonic \FLOW_TLMRST
**
**  \par Command Structure
**       #FLOW_SetInt32_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \FLOW_CMDACTPCNT       - command counter will be cleared
**       - \b \c \FLOW_CMDRJCTCNT       - command error counter will be cleared
**       - The #FLOW_CMD_ACCEPTED_EID debug event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c \FLOW_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #FLOW_CMD_REJECTED_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #FLOW_NOOP_CC
**  \sa #FLOW_RESET_CC
**  \sa #FLOW_SET_HFOV_CC
**  \sa #FLOW_SET_TILE_SIZE_CC
**  \sa #FLOW_SET_BLOCKS_CC
**  \sa #FLOW_SET_OP_RATE_CC
**  \sa #FLOW_SET_IMG_WIDTH_CC
**  \sa #FLOW_SET_IMG_HEIGHT_CC
**  \sa #FLOW_SET_SEARCH_SIZE_CC
**  \sa #FLOW_SET_FEATURE_THRESHOLD_CC
**  \sa #FLOW_SET_VALUE_THRESHOLD_CC
**  \sa #FLOW_SEND_DIAG_CC
*/
#define FLOW_SET_MEAN_COUNT_THRESHOLD_CC     	(11)

/** \flowcmd Send diagnostic message
**
**  \par Description
**       Sets send diagnostic message
**
**  \flowcmdmnemonic \FLOW_TLMRST
**
**  \par Command Structure
**       #FLOW_SetInt32_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \FLOW_CMDACTPCNT       - command counter will be cleared
**       - \b \c \FLOW_CMDRJCTCNT       - command error counter will be cleared
**       - The #FLOW_CMD_ACCEPTED_EID debug event message will be
**         generated when the command is executed
**
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
**
**  \par Evidence of failure may be found in the following telemetry:
**       - \b \c \FLOW_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #FLOW_CMD_REJECTED_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #FLOW_NOOP_CC
**  \sa #FLOW_RESET_CC
**  \sa #FLOW_SET_HFOV_CC
**  \sa #FLOW_SET_TILE_SIZE_CC
**  \sa #FLOW_SET_BLOCKS_CC
**  \sa #FLOW_SET_OP_RATE_CC
**  \sa #FLOW_SET_IMG_WIDTH_CC
**  \sa #FLOW_SET_IMG_HEIGHT_CC
**  \sa #FLOW_SET_SEARCH_SIZE_CC
**  \sa #FLOW_SET_FEATURE_THRESHOLD_CC
**  \sa #FLOW_SET_VALUE_THRESHOLD_CC
**  \sa #FLOW_SET_MEAN_COUNT_THRESHOLD_CC
*/
#define FLOW_SEND_DIAG_CC     					(12)

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** 
**  \brief No Arguments Command
**  For command details see #FLOW_NOOP_CC, #FLOW_RESET_CC
**  Also see #FLOW_SEND_HK_MID
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} FLOW_NoArgCmd_t;

/** 
**  \brief FLOW application housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \flowtlmmnemonic \FLOW_CMDACPTCNT
        \brief Count of accepted commands */
    uint8              usCmdCnt;   

    /** \flowtlmmnemonic \FLOW_CMDRJCTCNT
        \brief Count of failed commands */
    uint8              usCmdErrCnt; 

    /** \brief Timestamp */
	uint64 Timestamp;

	/** \brief Pixel flow across X axis */
	float PixelFlowX;

	/** \brief Pixel flow across Y axis */
	float PixelFlowY;

	/** \brief Angular flow across X axis */
	float AngularFlowX;

	/** \brief Angular flow across Y axis */
	float AngularFlowY;

	/** \brief Rate Integral of Gyro along X axis */
	float GyroXRateIntegral;

	/** \brief Rate Integral of Gyro along Y axis */
	float GyroYRateIntegral;

	/** \brief Rate Integral of Gyro along Z axis */
	float GyroZRateIntegral;

	/** \brief Quality */
	int32 Quality;

	/** \brief Instantaneous accumulated flow across X axis */
	float FlowSum_X;

	/** \brief Instantaneous accumulated flow across Y axis */
	float FlowSum_Y;

	/** \brief Instantaneous accumulated flow quality */
	int32 FlowQualitySum;

	/** \brief Valid frame comparison count */
	int32 ValidFrameCount;

	/** \brief Instantaneous time between two consecutive frames */
	int32 DeltaTime;

	/** \brief Flow initialization status */
    boolean FlowInitalized;

    /** \brief Last appeared gyro message timestamp */
    uint64 LastGyroMsgAppeared;

    /** \brief Last appeared frame message timestamp */
    uint32 TimeLastPublished;

} FLOW_HkTlm_t;

/**
**  \brief FLOW diagnostic data
*/
typedef struct
{
    uint8           TlmHeader[CFE_SB_TLM_HDR_SIZE];

	/** \brief Horizontal field of view */
	float HFOV;

	/** \brief Focal length X of downward facing camera.  */
    float FocalLength_X;

    /** \brief Focal length Y of downward facing camera.  */
    float FocalLength_Y;

	/** \brief Sliding window tile size */
	int32 TileSize;

	/** \brief Number of blocks of sliding window */
	int32 Blocks;

	/** \brief Output rate */
	int32 OutputRate;

	/** \brief Image width and height */
	int32 ImgWidth;

	int32 ImgHeight;

	/** \brief Image search size */
	int32 SearchSize;

	/** \brief Default feature threshold */
	int32 FeatureThreshold;

	/** \brief Default value threshold */
	int32 ValueThreshold;

	/** \brief Mean count threshold */
	int32 MeancountThreshold;

} FLOW_DiagPacket_t;


/**
**  \brief FLOW command argument data
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    float  value;
} FLOW_SetFloat_t;

typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    int32  value;
} FLOW_SetInt32_t;

#ifdef __cplusplus
}
#endif

#endif /* FLOW_MSG_H */

/************************/
/*  End of File Comment */
/************************/
