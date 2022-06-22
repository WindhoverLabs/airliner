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

#ifndef MIXER_TABLES_H
#define MIXER_TABLES_H

#include "cfe.h"
#include "amc_platform_cfg.h"

#define AMC_MULTIROTOR_MIXER_MAX_ROTOR_COUNT  (8)

typedef enum
{
	AMC_UNUSED,
	AMC_ENABLED
} AMC_EntryState_t;

typedef enum
{
	AMC_MULTIROTOR_GEOMETRY_UNKNOWN,
	AMC_MULTIROTOR_GEOMETRY_QUAD_X,
	AMC_MULTIROTOR_GEOMETRY_QUAD_H,
	AMC_MULTIROTOR_GEOMETRY_QUAD_PLUS,
	AMC_MULTIROTOR_GEOMETRY_QUAD_V,
	AMC_MULTIROTOR_GEOMETRY_QUAD_WIDE,
	AMC_MULTIROTOR_GEOMETRY_QUAD_S250AQ,
	AMC_MULTIROTOR_GEOMETRY_QUAD_DEADCAT,
	AMC_MULTIROTOR_GEOMETRY_HEX_X,
	AMC_MULTIROTOR_GEOMETRY_HEX_PLUS,
	AMC_MULTIROTOR_GEOMETRY_HEX_COX,
	AMC_MULTIROTOR_GEOMETRY_HEX_T,
	AMC_MULTIROTOR_GEOMETRY_OCTA_X,
	AMC_MULTIROTOR_GEOMETRY_OCTA_PLUS,
	AMC_MULTIROTOR_GEOMETRY_OCTA_COX,
	AMC_MULTIROTOR_GEOMETRY_OCTA_COX_WIDE,
	AMC_MULTIROTOR_GEOMETRY_TWIN_ENGINE,
	AMC_MULTIROTOR_GEOMETRY_TRI_Y,
	AMC_MULTIROTOR_GEOMETRY_MAX_GEOMETRY
} AMC_MultirotorGeometry_t;

typedef struct {
    float   RollScale; /**< scales roll for this rotor */
    float   PitchScale;/**< scales pitch for this rotor */
    float   YawScale;  /**< scales yaw for this rotor */
    float   OutScale;  /**< scales total out for this rotor */
} AMC_MultirotorMixer_RotorConfig_t;

typedef struct
{
    uint32 RotorCount;
    float  RollScale;
    float  PitchScale;
    float  YawScale;
    float  IdleSpeed;
    float  DeltaOutMax;
    AMC_MultirotorGeometry_t Geometry;
    AMC_MultirotorMixer_RotorConfig_t RotorConfig[AMC_MULTIROTOR_MIXER_MAX_ROTOR_COUNT];
} AMC_MultirotorMixer_Config_t;


/** simple channel scaler */
typedef struct {
	float NegativeScale;
	float PositiveScale;
	float Offset;
	float MinOutput;
	float MaxOutput;
} AMC_Mixer_Scaler_t;

/** mixer input */
typedef struct {
	uint8 ControlGroup;	/**< group from which the input reads */
	uint8 ControlIndex;	/**< index within the control group */
	AMC_Mixer_Scaler_t Scaler;		/**< scaling applied to the input before use */
} AMC_SimpleMixer_Control_t;

typedef struct
{
	uint8  ControlCount;	/**< number of inputs */
	AMC_Mixer_Scaler_t    OutputScaler;	/**< scaling for the output */
	AMC_SimpleMixer_Control_t   Controls[AMC_SIMPLE_MIXER_MAX_CONTROLS];	/**< actual size of the array is set by control_count */
} AMC_SimpleMixer_Config_t;

typedef struct
{
	AMC_MultirotorMixer_Config_t Multirotor[AMC_MULTIROTOR_MIXER_MAX_MIXERS];
	AMC_SimpleMixer_Config_t     Simple[AMC_SIMPLE_MIXER_MAX_MIXERS];
} AMC_Mixer_ConfigTable_t;



#endif
