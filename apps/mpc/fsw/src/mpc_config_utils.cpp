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

#ifdef __cplusplus
extern "C" {
#endif

#include "mpc_app.h"
#include <math.h>

/************************************************************************
** Function Definitions
*************************************************************************/


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Config Table                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MPC::InitConfigTbl()
{
    int32  iStatus=0;

    /* Register Config table */
    iStatus = CFE_TBL_Register(&ConfigTblHdl,
        MPC_CONFIG_TABLENAME,
        (sizeof(MPC_ConfigTbl_t)),
        CFE_TBL_OPT_DEFAULT,
        MPC::ValidateConfigTbl);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, a critical table could return another nominal code.  If this table is
         * made critical this logic would have to change. */
        (void) CFE_EVS_SendEvent(MPC_CFGTBL_REG_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to register config table (0x%08lX)",
                                 iStatus);
        goto MPC_InitConfigTbl_Exit_Tag;
    }

    /* Load Config table file */
    iStatus = CFE_TBL_Load(ConfigTblHdl,
                           CFE_TBL_SRC_FILE,
                           MPC_CONFIG_TABLE_FILENAME);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, CFE_SUCCESS is for a successful full table load.  If a partial table
           load is desired then this logic would have to change. */
        (void) CFE_EVS_SendEvent(MPC_CFGTBL_LOAD_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to load Config Table (0x%08lX)",
                                 iStatus);
        goto MPC_InitConfigTbl_Exit_Tag;
    }

    iStatus = AcquireConfigPointers();

MPC_InitConfigTbl_Exit_Tag:
    return iStatus;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Acquire Config Pointers                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MPC::AcquireConfigPointers(void)
{
    int32 iStatus = CFE_SUCCESS;

    /*
    ** Release the table
    */
    /* TODO: This return value can indicate success, error, or that the info has been 
     * updated.  We ignore this return value in favor of checking CFE_TBL_Manage(), but
     * be sure this is the behavior you want. */
    (void) CFE_TBL_ReleaseAddress(ConfigTblHdl);

    /*
    ** Manage the table
    */
    iStatus = CFE_TBL_Manage(ConfigTblHdl);
    if ((iStatus != CFE_SUCCESS) && (iStatus != CFE_TBL_INFO_UPDATED))
    {
        (void) CFE_EVS_SendEvent(MPC_CFGTBL_MANAGE_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to manage MPC Config table (0x%08lX)",
                                 iStatus);
        goto MPC_AcquireConfigPointers_Exit_Tag;
    }

    /*
    ** Get a pointer to the table
    */
    iStatus = CFE_TBL_GetAddress((void**)&ConfigTblPtr, ConfigTblHdl);
    if (iStatus == CFE_TBL_INFO_UPDATED)
    {
        iStatus = CFE_SUCCESS;
        UpdateParamsFromTable();
    }
    else if(iStatus != CFE_SUCCESS)
    {
        ConfigTblPtr = 0;
        (void) CFE_EVS_SendEvent(MPC_CFGTBL_GETADDR_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to get Config table's address (0x%08lX)",
                                 iStatus);
    }

MPC_AcquireConfigPointers_Exit_Tag:
    return iStatus;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate MPC Configuration Table                                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MPC::ValidateConfigTbl(void* ConfigTblPtr)
{
    int32  iStatus=0;
    MPC_ConfigTbl_t* MPC_ConfigTblPtr = (MPC_ConfigTbl_t*)(ConfigTblPtr);

    if(!(MPC_ConfigTblPtr->THR_MIN >= THR_MIN_MIN && MPC_ConfigTblPtr->THR_MIN <= THR_MIN_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->THR_HOVER >= THR_HOVER_MIN && MPC_ConfigTblPtr->THR_HOVER <= THR_HOVER_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->THR_MAX >= THR_MAX_MIN && MPC_ConfigTblPtr->THR_MAX <= THR_MAX_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->MANTHR_MIN >= MANTHR_MIN_MIN && MPC_ConfigTblPtr->MANTHR_MIN <= MANTHR_MIN_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->MANTHR_MAX >= MANTHR_MAX_MIN && MPC_ConfigTblPtr->MANTHR_MAX <= MANTHR_MAX_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(MPC::Validate_Z_P(MPC_ConfigTblPtr->Z_P) == FALSE)
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(MPC::Validate_Z_VEL_P(MPC_ConfigTblPtr->Z_VEL_P) == FALSE)
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(MPC::Validate_Z_VEL_I(MPC_ConfigTblPtr->Z_VEL_I) == FALSE)
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(MPC::Validate_Z_VEL_D(MPC_ConfigTblPtr->Z_VEL_D) == FALSE)
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->Z_VEL_MAX_UP >= Z_VEL_MAX_UP_MIN && MPC_ConfigTblPtr->Z_VEL_MAX_UP <= Z_VEL_MAX_UP_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->Z_VEL_MAX_DN >= Z_VEL_MAX_DN_MIN && MPC_ConfigTblPtr->Z_VEL_MAX_DN <= Z_VEL_MAX_DN_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->Z_FF >= Z_FF_MIN && MPC_ConfigTblPtr->Z_FF <= Z_FF_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(MPC::Validate_XY_P(MPC_ConfigTblPtr->XY_P) == FALSE)
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(MPC::Validate_XY_VEL_P(MPC_ConfigTblPtr->XY_VEL_P) == FALSE)
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(MPC::Validate_XY_VEL_I(MPC_ConfigTblPtr->XY_VEL_I) == FALSE)
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(MPC::Validate_XY_VEL_D(MPC_ConfigTblPtr->XY_VEL_D) == FALSE)
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->XY_CRUISE >= XY_CRUISE_MIN && MPC_ConfigTblPtr->XY_CRUISE <= XY_CRUISE_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->TARGET_THRE >= TARGET_THRE_MIN && MPC_ConfigTblPtr->TARGET_THRE <= TARGET_THRE_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->XY_VEL_MAX >= XY_VEL_MAX_MIN && MPC_ConfigTblPtr->XY_VEL_MAX <= XY_VEL_MAX_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->XY_FF >= XY_FF_MIN && MPC_ConfigTblPtr->XY_FF <= XY_FF_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->TILTMAX_AIR >= TILTMAX_AIR_MIN && MPC_ConfigTblPtr->TILTMAX_AIR <= TILTMAX_AIR_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->TILTMAX_LND >= TILTMAX_LND_MIN && MPC_ConfigTblPtr->TILTMAX_LND <= TILTMAX_LND_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->LAND_SPEED >= LAND_SPEED_MIN && MPC_ConfigTblPtr->LAND_SPEED <= LAND_SPEED_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->TKO_SPEED >= TKO_SPEED_MIN && MPC_ConfigTblPtr->TKO_SPEED <= TKO_SPEED_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->MAN_TILT_MAX >= MAN_TILT_MAX_MIN && MPC_ConfigTblPtr->MAN_TILT_MAX <= MAN_TILT_MAX_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->MAN_Y_MAX >= MAN_Y_MAX_MIN && MPC_ConfigTblPtr->MAN_Y_MAX <= MAN_Y_MAX_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(MPC::Validate_HOLD_DZ(MPC_ConfigTblPtr->HOLD_DZ) == FALSE)
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->HOLD_MAX_XY >= HOLD_MAX_XY_MIN && MPC_ConfigTblPtr->HOLD_MAX_XY <= HOLD_MAX_XY_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->HOLD_MAX_Z >= HOLD_MAX_Z_MIN && MPC_ConfigTblPtr->HOLD_MAX_Z <= HOLD_MAX_Z_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->VELD_LP >= VELD_LP_MIN && MPC_ConfigTblPtr->VELD_LP <= VELD_LP_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->ACC_HOR_MAX >= ACC_HOR_MAX_MIN && MPC_ConfigTblPtr->ACC_HOR_MAX <= ACC_HOR_MAX_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->ACC_UP_MAX >= ACC_UP_MAX_MIN && MPC_ConfigTblPtr->ACC_UP_MAX <= ACC_UP_MAX_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->ACC_DOWN_MAX >= ACC_DOWN_MAX_MIN && MPC_ConfigTblPtr->ACC_DOWN_MAX <= ACC_DOWN_MAX_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->ALT_MODE >= ALT_MODE_MIN && MPC_ConfigTblPtr->ALT_MODE <= ALT_MODE_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(MPC::Validate_XY_MAN_EXPO(MPC_ConfigTblPtr->XY_MAN_EXPO) == FALSE)
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(MPC::Validate_Z_MAN_EXPO(MPC_ConfigTblPtr->Z_MAN_EXPO) == FALSE)
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->LAND_ALT1 >= LAND_ALT1_MIN && MPC_ConfigTblPtr->LAND_ALT1 <= LAND_ALT1_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->LAND_ALT2 >= LAND_ALT2_MIN && MPC_ConfigTblPtr->LAND_ALT2 <= LAND_ALT2_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(MPC::Validate_TKO_RAMP_T(MPC_ConfigTblPtr->TKO_RAMP_T) == FALSE)
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->MC_YAWRATE_MAX >= MC_YAWRATE_MAX_MIN && MPC_ConfigTblPtr->MC_YAWRATE_MAX <= MC_YAWRATE_MAX_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->MC_YAW_P >= MC_YAW_P_MIN && MPC_ConfigTblPtr->MC_YAW_P <= MC_YAW_P_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->MPC_CRUISE_90 >= MPC_CRUISE_90_MIN && MPC_ConfigTblPtr->MPC_CRUISE_90 <= MPC_CRUISE_90_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->MPC_JERK_MAX >= MPC_JERK_MAX_MIN && MPC_ConfigTblPtr->MPC_JERK_MAX <= MPC_JERK_MAX_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->MPC_JERK_MIN >= MPC_JERK_MIN_MIN && MPC_ConfigTblPtr->MPC_JERK_MIN <= MPC_JERK_MIN_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->MPC_DEC_HOR_SLOW >= MPC_DEC_HOR_SLOW_MIN && MPC_ConfigTblPtr->MPC_DEC_HOR_SLOW <= MPC_DEC_HOR_SLOW_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->MPC_VEL_MANUAL >= MPC_VEL_MANUAL_MIN && MPC_ConfigTblPtr->MPC_VEL_MANUAL <= MPC_VEL_MANUAL_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->NAV_ACC_RAD >= NAV_ACC_RAD_MIN && MPC_ConfigTblPtr->NAV_ACC_RAD <= NAV_ACC_RAD_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    if(!(MPC_ConfigTblPtr->NAV_MIS_YAW_ERR >= NAV_MIS_YAW_ERR_MIN && MPC_ConfigTblPtr->NAV_MIS_YAW_ERR <= NAV_MIS_YAW_ERR_MAX))
    {
    	printf("%s %u\n", __FUNCTION__, __LINE__);
    	iStatus = -1;
    }

    return iStatus;
}


#ifdef __cplusplus
}
#endif

/************************/
/*  End of File Comment */
/************************/
    
