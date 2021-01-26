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

#ifndef SENS_TBLDEFS_H
#define SENS_TBLDEFS_H

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
#include "sens_platform_cfg.h"
#include "px4_msgs.h"

/************************************************************************
** Local Defines
*************************************************************************/

/**
 * \brief Defines the table identification name used for the
 * configuration table registration.
 */
#define SENS_CONFIG_TABLENAME ("CONFIG_TBL")


/** \brief Definition for a single config table entry */
typedef struct
{
	float Min[PX4_RC_INPUT_MAX_CHANNELS];           /* RC%d_MIN */
	float Trim[PX4_RC_INPUT_MAX_CHANNELS];          /* RC%d_TRIM */
	float Max[PX4_RC_INPUT_MAX_CHANNELS];           /* RC%d_MAX */
	float Rev[PX4_RC_INPUT_MAX_CHANNELS];           /* RC%d_REV */
	float DZ[PX4_RC_INPUT_MAX_CHANNELS];            /* RC%d_DZ */

	int32 MapRoll;                                  /* RC_MAP_ROLL          */
	int32 MapPitch;                                 /* RC_MAP_PITCH         */
	int32 MapYaw;                                   /* RC_MAP_YAW           */
	int32 MapThrottle;                              /* RC_MAP_THROTTLE      */
	int32 MapFailsafe;                              /* RC_MAP_FAILSAFE      */

	int32 MapModeSw;                                /* RC_MAP_MODE_SW       */
	int32 MapReturnSw;                              /* RC_MAP_RETURN_SW     */
	int32 MapRattitudeSw;                           /* RC_MAP_RATT_SW       */
	int32 MapPosctlSw;                              /* RC_MAP_POSCTL_SW     */
	int32 MapLoiterSw;                              /* RC_MAP_LOITER_SW     */
	int32 MapAcroSw;                                /* RC_MAP_ACRO_SW       */
	int32 MapOffboardSw;                            /* RC_MAP_OFFB_SW       */
	int32 MapKillSw;                                /* RC_MAP_KILL_SW       */
	int32 MapArmSw;                                 /* RC_MAP_ARM_SW        */
	int32 MapTransSw;                               /* RC_MAP_TRANS_SW      */
	int32 MapGearSw;                                /* RC_MAP_GEAR_SW       */
	int32 MapStabSw;                                /* RC_MAP_STAB_SW       */
	int32 MapManSw;                                 /* RC_MAP_MAN_SW        */
	int32 MapFlaps;                                 /* RC_MAP_FLAPS         */
	int32 MapAltctlSw;                              /* RC_MAP_ALTCTL_SW     */

	int32 MapAux1;                                  /* RC_MAP_AUX1          */
	int32 MapAux2;                                  /* RC_MAP_AUX2          */
	int32 MapAux3;                                  /* RC_MAP_AUX3          */
	int32 MapAux4;                                  /* RC_MAP_AUX4          */
	int32 MapAux5;                                  /* RC_MAP_AUX5          */

	int32 MapParam[PX4_RC_PARAM_MAP_NCHAN];         /* RC_MAP_PARAM%d       */

	int32 MapFlightMode;                            /* RC_MAP_FLTMODE       */

	int32 FailsThr;                                 /* RC_FAILS_THR         */
	float AssistTh;                                 /* RC_ASSIT_TH          */
	float AutoTh;                                   /* RC_AUTO_TH           */
	float RattitudeTh;                              /* RC_RATT_TH           */
	float PosctlTh;                                 /* RC_POSCTL_TH         */
	float ReturnTh;                                 /* RC_RETURN_TH         */
	float LoiterTh;                                 /* RC_LOITER_TH         */
	float AcroTh;                                   /* RC_ACRO_TH           */
	float OffboardTh;                               /* RC_OFFB_TH           */
	float KillswitchTh;                             /* RC_KILLSWITCH_TH     */
	float ArmswitchTh;                              /* RC_ARMSWITCH_TH      */
	float TransTh;                                  /* RC_TRANS_TH          */
	float GearTh;                                   /* RC_GEAR_TH           */
	float StabTh;                                   /* RC_STAB_TH           */
	float ManTh;                                    /* RC_MAN_TH            */
	float AltctlTh;                                 /* RC_ALTCTL_TH         */
	
	float FltSmpRate;                               /* RC_FLT_SMP_RATE      */
	float FltCutoff;                                /* RC_FLT_CUTOFF        */

    /* This is a flag that if enabled, will set mode flags
    *  in the manualcontrolsetpoint message based on the
    *  mode switch position rather than individual switches
    *  for  individual modes */
	boolean ModeSwitchControl;                      /* ModeSwitchControl    */


} SENS_ConfigTbl_t;


#ifdef __cplusplus
}
#endif

#endif /* SENS_TBLDEFS_H */

/************************/
/*  End of File Comment */
/************************/
