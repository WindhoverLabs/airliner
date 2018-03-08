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

#ifndef VM_TBLDEFS_H
#define VM_TBLDEFS_H

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
#include "vm_platform_cfg.h"

/************************************************************************
** Local Defines
*************************************************************************/

/**
 * \brief Defines the table identification name used for the
 * configuration table registration.
 */
#define VM_CONFIG_TABLENAME ("CONFIG_TBL")


/** \brief Definition for a single config table entry */
typedef struct
{
	int		SYS_AUTOSTART;
	int 	COM_RC_IN_MODE;
	int 	COM_ARM_SWISBTN;
	int 	COM_ARM_WO_GPS;
	int 	COM_ARM_MIS_REQ;
	int 	COM_RC_ARM_HYST;
	int 	MAV_TYPE;
	int 	MAV_SYS_ID;
	int 	MAV_COMP_ID;
	int 	CBRK_SUPPLY_CHK;
	int 	CBRK_USB_CHK;
	int 	CBRK_AIRSPD_CHK;
	int 	CBRK_ENGINEFAIL;
	int 	CBRK_GPSFAIL;
	int 	CBRK_FLIGHTTERM;
	int 	CBRK_VELPOSERR;
	int 	NAV_DLL_ACT;
	int 	NAV_RCL_ACT;
	int 	COM_DL_LOSS_T;
	float 	COM_RC_LOSS_T;
	float 	COM_RC_STICK_OV;
	int 	COM_RC_OVERRIDE;
	int 	COM_DL_REG_T;
	float 	COM_EF_THROT;
	float 	COM_EF_C2T;
	float 	COM_EF_TIME;
	int 	GF_ACTION;
	int		COM_DISARM_LAND;
	int 	COM_LOW_BAT_ACT;
	float 	COM_OF_LOSS_T;
	int 	COM_OBL_ACT;
	int 	COM_OBL_RC_ACT;
	float 	COM_HOME_H_T;
	float 	COM_HOME_V_T;
	int 	COM_FLTMODE1;
	int 	COM_FLTMODE2;
	int 	COM_FLTMODE3;
	int 	COM_FLTMODE4;
	int 	COM_FLTMODE5;
	int 	COM_FLTMODE6;
	float 	COM_ARM_EKF_POS;
	float 	COM_ARM_EKF_VEL;
	float 	COM_ARM_EKF_HGT;
	float 	COM_ARM_EKF_YAW;
	float 	COM_ARM_EKF_AB;
	float 	COM_ARM_EKF_GB;
	float 	COM_ARM_IMU_ACC;
	float 	COM_ARM_IMU_GYR;
	int 	COM_POSCTL_NAVL;
} VM_ConfigTbl_t;


#ifdef __cplusplus
}
#endif

#endif /* VM_TBLDEFS_H */

/************************/
/*  End of File Comment */
/************************/
