/*==============================================================================
 Copyright (c) 2015, Windhover Labs
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 * Neither the name of TlmOut nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _PX4BR_MSG_H_
#define _PX4BR_MSG_H_

/*
 ** Pragmas
 */

/*
 ** Include Files
 */

#include "cfe.h"
#include "px4br_mission_cfg.h"
#include "px4_msgs.h"

/*
 ** Local Defines
 */

/*
 ** Local Structure Declarations
 */

typedef enum
{
    PX4BR_RUNTIME_MODE_NORMAL     = 0,
    PX4BR_RUNTIME_MODE_TEST       = 1
} PX4BR_RuntimeMode_t;

typedef enum
{
    PX4BR_SWITCH_POS_NONE		  = 0,
    PX4BR_SWITCH_POS_ON		  	  = 1,
    PX4BR_SWITCH_POS_MIDDLE		  = 2,
    PX4BR_SWITCH_POS_OFF		  = 3
} PX4BR_SwitchPosType_t;

typedef enum
{
    PX4BR_MODE_SLOT_TYPE_NONE	  = -1,
    PX4BR_MODE_SLOT_TYPE_SLOT_1	  = 0,
    PX4BR_MODE_SLOT_TYPE_SLOT_2	  = 1,
    PX4BR_MODE_SLOT_TYPE_SLOT_3	  = 2,
    PX4BR_MODE_SLOT_TYPE_SLOT_4	  = 3,
    PX4BR_MODE_SLOT_TYPE_SLOT_5	  = 4,
    PX4BR_MODE_SLOT_TYPE_SLOT_6	  = 5
} PX4BR_ModeSlotType_t;

typedef struct
{
    uint8               CmdHeader[CFE_SB_CMD_HDR_SIZE];
    PX4BR_RuntimeMode_t   RuntimeMode;
} PX4BR_SetRuntimeModeMsg_t;



typedef struct
{
    uint8               TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint16              CmdCount;
    uint16              CmdErrCount;
    PX4BR_RuntimeMode_t RuntimeMode;
} PX4BR_HkPacket_t;



typedef struct
{
    uint8 CmdHeader[CFE_SB_CMD_HDR_SIZE];
	float Param1;
	float Param2;
	float Param3;
	float Param4;
	double Param5;
	double Param6;
	float Param7;
	PX4_VehicleCmd_t Command;
	uint32 TargetSystem;
	uint32 TargetComponent;
	uint32 SourceSystem;
	uint32 SourceComponent;
	uint8 Confirmation;
} PX4_VehicleCommandCmd_t;



typedef struct
{
    uint8 CmdHeader[CFE_SB_CMD_HDR_SIZE];
	uint8 BaseMode;
	uint8 CustomMainMode;
	uint8 CustomSubMode;
} PX4_VehicleModeChangeCmd_t;



typedef struct
{
    uint8 CmdHeader[CFE_SB_CMD_HDR_SIZE];
	int32 X;
	int32 Y;
	int32 Z;
	int32 RcFailsafe;
	int32 Flaps;
	int32 Aux1;
	int32 Aux2;
	int32 Aux3;
	int32 Aux4;
	int32 Aux5;
	uint32 InputMask;
	PX4BR_SwitchPosType_t ModeSwitch;
	PX4BR_SwitchPosType_t ReturnSwitch;
	PX4BR_SwitchPosType_t RattitudeSwitch;
	PX4BR_SwitchPosType_t PosctlSwitch;
	PX4BR_SwitchPosType_t LoiterSwitch;
	PX4BR_SwitchPosType_t AcroSwitch;
	PX4BR_SwitchPosType_t OffboardSwitch;
	PX4BR_SwitchPosType_t KillSwitch;
	PX4BR_SwitchPosType_t TransitionSwitch;
	PX4BR_ModeSlotType_t  ModeSlot;
} PX4_ManualControlSetpointCmd_t;


typedef enum
{
	PX4_MANUAL_INPUT_MASK_X				= 0x00000001,
	PX4_MANUAL_INPUT_MASK_Y				= 0x00000002,
	PX4_MANUAL_INPUT_MASK_Z				= 0x00000004,
	PX4_MANUAL_INPUT_MASK_RC_FAILSAFE	= 0x00000008,
	PX4_MANUAL_INPUT_MASK_FLAPS			= 0x00000010,
	PX4_MANUAL_INPUT_MASK_AUX1			= 0x00000020,
	PX4_MANUAL_INPUT_MASK_AUX2			= 0x00000040,
	PX4_MANUAL_INPUT_MASK_AUX3			= 0x00000080,
	PX4_MANUAL_INPUT_MASK_AUX4			= 0x00000100,
	PX4_MANUAL_INPUT_MASK_AUX5			= 0x00000200
} PX4_ManualInputMaskBit_t;



/*
 ** External Global Variables
 */

/*
 ** Global Variables
 */

/*
 ** Local Variables
 */

/*
 ** Local Function Prototypes
 */

#endif /* _PX4BR_MSG_H_ */

/* ---------- end of file eta_msg.h ----------*/
