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

#ifndef AMC_TBLDEFS_H
#define AMC_TBLDEFS_H

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
#include "amc_platform_cfg.h"

/************************************************************************
** Local Defines
*************************************************************************/

/**
 * \brief Defines the table identification name used for the
 * configuration table registration.
 */
#define AMC_CONFIG_TABLENAME ("CONFIG_TBL")

typedef enum
{
	AMC_PWM_DISARM_BEHAVIOR_SAFE   = 0,
	AMC_PWM_DISARM_BEHAVIOR_IGNORE = 1
} AMC_PwmDisarmBehavior_t;

/**
 * \brief Defines the table identification name used for the Mixer
 * configuration table registration.
 */
#define AMC_MIXER_CONFIG_TABLENAME ("MIXERCFG_TBL")

/** \brief Definition for a single config table entry */
typedef struct
{
    /** \brief Motor safe value for this device. */
    uint32                   PwmSafe;

    /** \brief Motor minimum value for this device.  Keep this high enough to overcome stiction but low enough not to cause lift or thrust.*/
    uint32                   PwmMin;

    /** \brief Motor maximum value for this device. */
    uint32                   PwmMax;

    AMC_PwmDisarmBehavior_t  DisarmBehavior;

    /** \brief Initial value at startup.  This is applied at startup, if Disarmed behavior is set to ignore.. */
    uint32                   PwmInitial;
} AMC_PwmChannel_t;

typedef struct
{
	AMC_PwmChannel_t 	Channel[AMC_MAX_MOTOR_OUTPUTS];
} AMC_ConfigTbl_t;


#ifdef __cplusplus
}
#endif

#endif /* AMC_TBLDEFS_H */

/************************/
/*  End of File Comment */
/************************/
