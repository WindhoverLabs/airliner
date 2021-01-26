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

#define MULTIROTOR_MIXER_MAX_ROTOR_COUNT 8

typedef enum
{
    MIXER_QUAD_X,
    MIXER_QUAD_H,
    MIXER_QUAD_PLUS,
    MIXER_QUAD_V,
    MIXER_QUAD_WIDE,
    MIXER_QUAD_S250AQ,
    MIXER_QUAD_DEADCAT,
    MIXER_HEX_X,
    MIXER_HEX_PLUS,
    MIXER_HEX_COX,
    MIXER_HEX_T,
    MIXER_OCTA_X,
    MIXER_OCTA_PLUS,
    MIXER_OCTA_COX,
    MIXER_OCTA_COX_WIDE,
    MIXER_TWIN_ENGINE,
    MIXER_TRI_Y,
    MIXER_MAX_GEOMETRY
} MultirotorGeometry_t;

typedef struct {
    float   RollScale; /**< scales roll for this rotor */
    float   PitchScale;/**< scales pitch for this rotor */
    float   YawScale;  /**< scales yaw for this rotor */
    float   OutScale;  /**< scales total out for this rotor */
} MultirotorMixer_RotorConfig_t;

typedef struct
{
    float  RollScale;
    float  PitchScale;
    float  YawScale;
    float  IdleSpeed;
    float  DeltaOutMax;
    uint32 RotorCount;
	MultirotorGeometry_t Geometry;
    MultirotorMixer_RotorConfig_t RotorConfig[MULTIROTOR_MIXER_MAX_ROTOR_COUNT];
} MultirotorMixer_ConfigTable_t;


typedef MultirotorMixer_ConfigTable_t *MultirotorMixer_ConfigTablePtr_t;


#endif
