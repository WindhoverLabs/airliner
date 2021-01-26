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

#include "mac_app.hpp"

/************************************************************************
** Function Definitions
*************************************************************************/


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Config Table                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MAC::InitConfigTbl()
{
    int32  iStatus = 0;

    /* Register Param table */
    iStatus = CFE_TBL_Register(&ParamTblHdl,
                               MAC_PARAM_TABLENAME,
                               (sizeof(MAC_ParamTbl_t)),
                               CFE_TBL_OPT_DEFAULT,
                               MAC::ValidateParamTbl);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, a critical table could return another nominal code.  If this table is
         * made critical this logic would have to change. */
        (void) CFE_EVS_SendEvent(MAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to register param table (0x%08X)",
                                 (unsigned int)iStatus);
        goto MAC_InitConfigTbl_Exit_Tag;
    }

    /* Load Config table file */
    iStatus = CFE_TBL_Load(ParamTblHdl,
                           CFE_TBL_SRC_FILE,
                           MAC_PARAM_TABLE_FILENAME);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, CFE_SUCCESS is for a successful full table load.  If a partial table
           load is desired then this logic would have to change. */
        (void) CFE_EVS_SendEvent(MAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to load Config Table (0x%08X)",
                                 (unsigned int)iStatus);
        goto MAC_InitConfigTbl_Exit_Tag;
    }

    iStatus = AcquireConfigPointers();

MAC_InitConfigTbl_Exit_Tag:
    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate PWM Configuration Table                                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MAC::ValidateParamTbl(void* ConfigTblPtr)
{
    int32  iStatus = CFE_SUCCESS;
    char Param[MAC_PARAM_NAME_MAX_LEN];
    MAC_ParamTbl_t* MAC_ParamTblPtr = (MAC_ParamTbl_t*)(ConfigTblPtr);

    if (MAC_ParamTblPtr->MC_ROLL_P < MC_ROLL_P_MIN ||
        MAC_ParamTblPtr->MC_ROLL_P > MC_ROLL_P_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_ROLL_P");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (MAC_ParamTblPtr->MC_ROLLRATE_P < MC_ROLLRATE_P_MIN ||
        MAC_ParamTblPtr->MC_ROLLRATE_P > MC_ROLLRATE_P_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_ROLLRATE_P");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (MAC_ParamTblPtr->MC_ROLLRATE_I < MC_ROLLRATE_I_MIN ||
        MAC_ParamTblPtr->MC_ROLLRATE_I > MC_ROLLRATE_I_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_ROLLRATE_I");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (MAC_ParamTblPtr->MC_ROLLRATE_D < MC_ROLLRATE_D_MIN ||
        MAC_ParamTblPtr->MC_ROLLRATE_D > MC_ROLLRATE_D_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_ROLLRATE_D");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (MAC_ParamTblPtr->MC_RR_INT_LIM < MC_RR_INT_LIM_MIN ||
        MAC_ParamTblPtr->MC_RR_INT_LIM > MC_RR_INT_LIM_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_RR_INT_LIM");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (MAC_ParamTblPtr->MC_ROLLRATE_FF < MC_ROLLRATE_FF_MIN ||
        MAC_ParamTblPtr->MC_ROLLRATE_FF > MC_ROLLRATE_FF_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_ROLLRATE_FF");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (MAC_ParamTblPtr->MC_PITCH_P < MC_PITCH_P_MIN ||
        MAC_ParamTblPtr->MC_PITCH_P > MC_PITCH_P_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_PITCH_P");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (MAC_ParamTblPtr->MC_PITCHRATE_P < MC_PITCHRATE_P_MIN ||
        MAC_ParamTblPtr->MC_PITCHRATE_P > MC_PITCHRATE_P_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_PITCHRATE_P");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (MAC_ParamTblPtr->MC_PITCHRATE_I < MC_PITCHRATE_I_MIN ||
        MAC_ParamTblPtr->MC_PITCHRATE_I > MC_PITCHRATE_I_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_PITCHRATE_I");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (MAC_ParamTblPtr->MC_PR_INT_LIM < MC_PR_INT_LIM_MIN ||
        MAC_ParamTblPtr->MC_PR_INT_LIM > MC_PR_INT_LIM_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_PR_INT_LIM");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (MAC_ParamTblPtr->MC_PITCHRATE_D < MC_PITCHRATE_D_MIN ||
        MAC_ParamTblPtr->MC_PITCHRATE_D > MC_PITCHRATE_D_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_PITCHRATE_D");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (MAC_ParamTblPtr->MC_PITCHRATE_FF < MC_PITCHRATE_FF_MIN ||
        MAC_ParamTblPtr->MC_PITCHRATE_FF > MC_PITCHRATE_FF_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_PITCHRATE_FF");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (MAC_ParamTblPtr->MC_TPA_BREAK_P < MC_TPA_BREAK_P_MIN ||
        MAC_ParamTblPtr->MC_TPA_BREAK_P > MC_TPA_BREAK_P_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_TPA_BREAK_P");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (MAC_ParamTblPtr->MC_TPA_BREAK_I < MC_TPA_BREAK_I_MIN ||
        MAC_ParamTblPtr->MC_TPA_BREAK_I > MC_TPA_BREAK_I_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_TPA_BREAK_I");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (MAC_ParamTblPtr->MC_TPA_BREAK_D < MC_TPA_BREAK_D_MIN ||
        MAC_ParamTblPtr->MC_TPA_BREAK_D > MC_TPA_BREAK_D_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_TPA_BREAK_D");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (MAC_ParamTblPtr->MC_TPA_RATE_P < MC_TPA_RATE_P_MIN ||
        MAC_ParamTblPtr->MC_TPA_RATE_P > MC_TPA_RATE_P_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_TPA_RATE_P");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }

    if (MAC_ParamTblPtr->MC_TPA_RATE_I < MC_TPA_RATE_I_MIN ||
        MAC_ParamTblPtr->MC_TPA_RATE_I > MC_TPA_RATE_I_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_TPA_RATE_I");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }

    if (MAC_ParamTblPtr->MC_TPA_RATE_D < MC_TPA_RATE_D_MIN ||
        MAC_ParamTblPtr->MC_TPA_RATE_D > MC_TPA_RATE_D_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_TPA_RATE_D");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }

    if (MAC_ParamTblPtr->MC_YAW_P < MC_YAW_P_MIN ||
        MAC_ParamTblPtr->MC_YAW_P > MC_YAW_P_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_YAW_P");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }

    if (MAC_ParamTblPtr->MC_YAWRATE_P < MC_YAWRATE_P_MIN ||
        MAC_ParamTblPtr->MC_YAWRATE_P > MC_YAWRATE_P_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_YAWRATE_P");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }

    if (MAC_ParamTblPtr->MC_YAWRATE_I < MC_YAWRATE_I_MIN ||
        MAC_ParamTblPtr->MC_YAWRATE_I > MC_YAWRATE_I_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_YAWRATE_I");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }

    if (MAC_ParamTblPtr->MC_YR_INT_LIM < MC_YR_INT_LIM_MIN ||
        MAC_ParamTblPtr->MC_YR_INT_LIM > MC_YR_INT_LIM_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_YR_INT_LIM");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }

    if (MAC_ParamTblPtr->MC_YAWRATE_D < MC_YAWRATE_D_MIN ||
        MAC_ParamTblPtr->MC_YAWRATE_D > MC_YAWRATE_D_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_YAWRATE_D");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }

    if (MAC_ParamTblPtr->MC_YAWRATE_FF < MC_YAWRATE_FF_MIN ||
        MAC_ParamTblPtr->MC_YAWRATE_FF > MC_YAWRATE_FF_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_YAWRATE_FF");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }

    if (MAC_ParamTblPtr->MC_ROLLRATE_MAX < MC_ROLLRATE_MAX_MIN ||
        MAC_ParamTblPtr->MC_ROLLRATE_MAX > MC_ROLLRATE_MAX_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_ROLLRATE_MAX");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }

    if (MAC_ParamTblPtr->MC_PITCHRATE_MAX < MC_PITCHRATE_MAX_MIN ||
        MAC_ParamTblPtr->MC_PITCHRATE_MAX > MC_PITCHRATE_MAX_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_PITCHRATE_MAX");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }

    if (MAC_ParamTblPtr->MC_YAWRATE_MAX < MC_YAWRATE_MAX_MIN ||
        MAC_ParamTblPtr->MC_YAWRATE_MAX > MC_YAWRATE_MAX_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_YAWRATE_MAX");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (MAC_ParamTblPtr->MC_YAWRAUTO_MAX < MC_YAWRAUTO_MAX_MIN ||
        MAC_ParamTblPtr->MC_YAWRAUTO_MAX > MC_YAWRAUTO_MAX_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_YAWRAUTO_MAX");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }

    if (MAC_ParamTblPtr->MC_ACRO_R_MAX < MC_ACRO_R_MAX_MIN ||
        MAC_ParamTblPtr->MC_ACRO_R_MAX > MC_ACRO_R_MAX_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_ACRO_R_MAX");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }

    if (MAC_ParamTblPtr->MC_ACRO_P_MAX < MC_ACRO_P_MAX_MIN ||
        MAC_ParamTblPtr->MC_ACRO_P_MAX > MC_ACRO_P_MAX_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_ACRO_P_MAX");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }

    if (MAC_ParamTblPtr->MC_ACRO_Y_MAX < MC_ACRO_Y_MAX_MIN ||
        MAC_ParamTblPtr->MC_ACRO_Y_MAX > MC_ACRO_Y_MAX_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_ACRO_Y_MAX");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }

    if (MAC_ParamTblPtr->MC_RATT_TH < MC_RATT_TH_MIN ||
        MAC_ParamTblPtr->MC_RATT_TH > MC_RATT_TH_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_RATT_TH");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }

    if (MAC_ParamTblPtr->MC_ROLL_TC < MC_ROLL_TC_MIN ||
        MAC_ParamTblPtr->MC_ROLL_TC > MC_ROLL_TC_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_ROLL_TC");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (MAC_ParamTblPtr->MC_PITCH_TC < MC_PITCH_TC_MIN ||
        MAC_ParamTblPtr->MC_PITCH_TC > MC_PITCH_TC_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_PITCH_TC");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }    

    if (MAC_ParamTblPtr->MC_BAT_SCALE_EN < MC_BAT_SCALE_EN_MIN ||
        MAC_ParamTblPtr->MC_BAT_SCALE_EN > MC_BAT_SCALE_EN_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "MC_BAT_SCALE_EN");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (MAC_ParamTblPtr->SENS_BOARD_ROT < SENS_BOARD_ROT_MIN ||
        MAC_ParamTblPtr->SENS_BOARD_ROT > SENS_BOARD_ROT_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "SENS_BOARD_ROT");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (MAC_ParamTblPtr->SENS_BOARD_X_OFF < SENS_BOARD_X_OFF_MIN ||
        MAC_ParamTblPtr->SENS_BOARD_X_OFF > SENS_BOARD_X_OFF_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "SENS_BOARD_X_OFF");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (MAC_ParamTblPtr->SENS_BOARD_Y_OFF < SENS_BOARD_Y_OFF_MIN ||
        MAC_ParamTblPtr->SENS_BOARD_Y_OFF > SENS_BOARD_Y_OFF_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "SENS_BOARD_Y_OFF");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (MAC_ParamTblPtr->SENS_BOARD_Z_OFF < SENS_BOARD_Z_OFF_MIN ||
        MAC_ParamTblPtr->SENS_BOARD_Z_OFF > SENS_BOARD_Z_OFF_MAX)
    {
        snprintf(Param, MAC_PARAM_NAME_MAX_LEN, "SENS_BOARD_Z_OFF");
        iStatus = -1;
        goto MAC_ValidateConfigTbl_Exit_Tag;
    }

MAC_ValidateConfigTbl_Exit_Tag:

    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(MAC_TBL_VALIDATE_ERR_EID, CFE_EVS_ERROR,
                                 "Table validation failed for parameter (%s)",
                                 Param);
    }
    
    return iStatus;  
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Acquire Config Pointers                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 MAC::AcquireConfigPointers(void)
{
    int32 iStatus = CFE_SUCCESS;

    /*
    ** Release the table
    */
    (void) CFE_TBL_ReleaseAddress(ParamTblHdl);

    /*
    ** Manage the table
    */
    iStatus = CFE_TBL_Manage(ParamTblHdl);
    if ((iStatus != CFE_SUCCESS) && (iStatus != CFE_TBL_INFO_UPDATED))
    {
        (void) CFE_EVS_SendEvent(MAC_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to manage PWM Config table (0x%08X)",
                                 (unsigned int)iStatus);
        goto MAC_AcquireConfigPointers_Exit_Tag;
    }

    /*
    ** Get a pointer to the table
    */
    iStatus = CFE_TBL_GetAddress((void**)&ParamTblPtr,
                                 ParamTblHdl);
    if (iStatus == CFE_TBL_INFO_UPDATED)
    {
        UpdateParams();
        iStatus = CFE_SUCCESS;
    }
    else if(iStatus != CFE_SUCCESS)
    {
        ParamTblPtr = 0;
        (void) CFE_EVS_SendEvent(MAC_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to get Param table's address (0x%08X)",
                                 (unsigned int)iStatus);
    }

MAC_AcquireConfigPointers_Exit_Tag:
    return (iStatus);

}


#ifdef __cplusplus
}
#endif

/************************/
/*  End of File Comment */
/************************/
    
