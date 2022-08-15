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

#include "fac_app.hpp"

/************************************************************************
** Function Definitions
*************************************************************************/


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Config Table                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 FAC::InitConfigTbl()
{
    int32  iStatus = 0;

    /* Register Param table */
    iStatus = CFE_TBL_Register(&ParamTblHdl,
                               FAC_PARAM_TABLENAME,
                               (sizeof(FAC_ParamTbl_t)),
                               CFE_TBL_OPT_DEFAULT,
                               FAC::ValidateParamTbl);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, a critical table could return another nominal code.  If this table is
         * made critical this logic would have to change. */
        CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to register param table (0x%08X)",
                                 (unsigned int)iStatus);
        goto FAC_InitConfigTbl_Exit_Tag;
    }

    /* Load Config table file */
    iStatus = CFE_TBL_Load(ParamTblHdl,
                           CFE_TBL_SRC_FILE,
                           FAC_PARAM_TABLE_FILENAME);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, CFE_SUCCESS is for a successful full table load.  If a partial table
           load is desired then this logic would have to change. */
        CFE_EVS_SendEvent(FAC_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to load Config Table (0x%08X)",
                                 (unsigned int)iStatus);
        goto FAC_InitConfigTbl_Exit_Tag;
    }

    iStatus = AcquireConfigPointers();

FAC_InitConfigTbl_Exit_Tag:
    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate PWM Configuration Table                                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 FAC::ValidateParamTbl(void* ConfigTblPtr)
{
    int32  iStatus = CFE_SUCCESS;
    char Param[FAC_PARAM_NAME_MAX_LEN];
    FAC_ParamTbl_t* FAC_ParamTblPtr = (FAC_ParamTbl_t*)(ConfigTblPtr);

    if (FAC_ParamTblPtr->FW_R_TC < FW_R_TC_MIN ||
        FAC_ParamTblPtr->FW_R_TC > FW_R_TC_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_R_TC");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (FAC_ParamTblPtr->FW_P_TC <FW_P_TC_MIN ||
        FAC_ParamTblPtr->FW_P_TC > FW_P_TC_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_P_TC");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (FAC_ParamTblPtr->FW_PR_P < FW_PR_P_MIN ||
        FAC_ParamTblPtr->FW_PR_P > FW_PR_P_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_PR_P");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (FAC_ParamTblPtr->FW_PR_I < FW_PR_I_MIN ||
        FAC_ParamTblPtr->FW_PR_I > FW_PR_I_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_PR_I");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (FAC_ParamTblPtr->FW_P_RMAX_POS < FW_P_RMAX_POS_MIN ||
        FAC_ParamTblPtr->FW_P_RMAX_POS > FW_P_RMAX_POS_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_P_RMAX_POS");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (FAC_ParamTblPtr->FW_P_RMAX_NEG < FW_P_RMAX_NEG_MIN ||
        FAC_ParamTblPtr->FW_P_RMAX_NEG > FW_P_RMAX_NEG_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_P_RMAX_NEG");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (FAC_ParamTblPtr->FW_PR_IMAX < FW_PR_IMAX_MIN ||
        FAC_ParamTblPtr->FW_PR_IMAX > FW_PR_IMAX_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_PR_IMAX");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (FAC_ParamTblPtr->FW_RR_P < FW_RR_P_MIN ||
        FAC_ParamTblPtr->FW_RR_P > FW_RR_P_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_RR_P");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (FAC_ParamTblPtr->FW_RR_I < FW_RR_I_MIN ||
        FAC_ParamTblPtr->FW_RR_I > FW_RR_I_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_RR_I");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (FAC_ParamTblPtr->FW_RR_IMAX < FW_RR_IMAX_MIN ||
        FAC_ParamTblPtr->FW_RR_IMAX > FW_RR_IMAX_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_RR_IMAX");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (FAC_ParamTblPtr->FW_R_RMAX < FW_R_RMAX_MIN ||
        FAC_ParamTblPtr->FW_R_RMAX > FW_R_RMAX_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_R_RMAX");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (FAC_ParamTblPtr->FW_YR_P < FW_YR_P_MIN ||
        FAC_ParamTblPtr->FW_YR_P > FW_YR_P_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_YR_P");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (FAC_ParamTblPtr->FW_YR_I < FW_YR_I_MIN ||
        FAC_ParamTblPtr->FW_YR_I > FW_YR_I_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_YR_I");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (FAC_ParamTblPtr->FW_YR_IMAX < FW_YR_IMAX_MIN ||
        FAC_ParamTblPtr->FW_YR_IMAX > FW_YR_IMAX_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_YR_IMAX");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (FAC_ParamTblPtr->FW_Y_RMAX < FW_Y_RMAX_MIN ||
        FAC_ParamTblPtr->FW_Y_RMAX > FW_Y_RMAX_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_Y_RMAX");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (FAC_ParamTblPtr->FW_RLL_TO_YAW_FF < FW_RLL_TO_YAW_FF_MIN ||
        FAC_ParamTblPtr->FW_RLL_TO_YAW_FF > FW_RLL_TO_YAW_FF_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_RLL_TO_YAW_FF");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }

    if (FAC_ParamTblPtr->FW_WR_P < FW_WR_P_MIN ||
        FAC_ParamTblPtr->FW_WR_P > FW_WR_P_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_WR_P");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }

    if (FAC_ParamTblPtr->FW_WR_I < FW_WR_I_MIN ||
        FAC_ParamTblPtr->FW_WR_I > FW_WR_I_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_WR_I");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }

    if (FAC_ParamTblPtr->FW_WR_IMAX < FW_WR_IMAX_MIN ||
        FAC_ParamTblPtr->FW_WR_IMAX > FW_WR_IMAX_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_WR_IMAX");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }

    if (FAC_ParamTblPtr->FW_W_RMAX < FW_W_RMAX_MIN ||
        FAC_ParamTblPtr->FW_W_RMAX > FW_W_RMAX_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_W_RMAX");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }

    if (FAC_ParamTblPtr->FW_RR_FF < FW_RR_FF_MIN ||
        FAC_ParamTblPtr->FW_RR_FF > FW_RR_FF_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_RR_FF");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }

    if (FAC_ParamTblPtr->FW_PR_FF < FW_PR_FF_MIN ||
        FAC_ParamTblPtr->FW_PR_FF > FW_PR_FF_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_PR_FF");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }

    if (FAC_ParamTblPtr->FW_PR_FF < FW_PR_FF_MIN ||
        FAC_ParamTblPtr->FW_PR_FF > FW_PR_FF_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_PR_FF");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }

    if (FAC_ParamTblPtr->FW_WR_FF < FW_WR_FF_MIN ||
        FAC_ParamTblPtr->FW_WR_FF > FW_WR_FF_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_WR_FF");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }

    if (FAC_ParamTblPtr->FW_YCO_VMIN < FW_YCO_VMIN_MIN ||
        FAC_ParamTblPtr->FW_YCO_VMIN > FW_YCO_VMIN_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_YCO_VMIN");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (FAC_ParamTblPtr->FW_YCO_METHOD < FW_YCO_METHOD_MIN ||
        FAC_ParamTblPtr->FW_YCO_METHOD > FW_YCO_METHOD_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_YCO_METHOD");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }

    if (FAC_ParamTblPtr->FW_RSP_OFF < FW_RSP_OFF_MIN ||
        FAC_ParamTblPtr->FW_RSP_OFF > FW_RSP_OFF_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_RSP_OFF");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }

    if (FAC_ParamTblPtr->FW_PSP_OFF < FW_PSP_OFF_MIN ||
        FAC_ParamTblPtr->FW_PSP_OFF > FW_PSP_OFF_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_PSP_OFF");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }

    if (FAC_ParamTblPtr->FW_MAN_R_MAX < FW_MAN_R_MAX_MIN ||
        FAC_ParamTblPtr->FW_MAN_R_MAX > FW_MAN_R_MAX_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_MAN_R_MAX");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }

    if (FAC_ParamTblPtr->FW_MAN_P_MAX < FW_MAN_P_MAX_MIN ||
        FAC_ParamTblPtr->FW_MAN_P_MAX > FW_MAN_P_MAX_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_MAN_P_MAX");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }

    if (FAC_ParamTblPtr->FW_FLAPS_SCL < FW_FLAPS_SCL_MIN ||
        FAC_ParamTblPtr->FW_FLAPS_SCL > FW_FLAPS_SCL_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_FLAPS_SCL");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (FAC_ParamTblPtr->FW_FLAPERON_SCL < FW_FLAPERON_SCL_MIN ||
        FAC_ParamTblPtr->FW_FLAPERON_SCL > FW_FLAPERON_SCL_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_FLAPERON_SCL");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }    

    if (FAC_ParamTblPtr->FW_MAN_R_SC < FW_MAN_R_SC_MIN ||
        FAC_ParamTblPtr->FW_MAN_R_SC > FW_MAN_R_SC_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_MAN_R_SC");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }

    if (FAC_ParamTblPtr->FW_MAN_P_SC < FW_MAN_P_SC_MIN)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_MAN_P_SC");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }

    if (FAC_ParamTblPtr->FW_MAN_Y_SC < FW_MAN_Y_SC_MIN)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_MAN_Y_SC");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (FAC_ParamTblPtr->FW_ACRO_X_MAX < FW_ACRO_X_MAX_MIN ||
        FAC_ParamTblPtr->FW_ACRO_X_MAX > FW_ACRO_X_MAX_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_ACRO_X_MAX");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (FAC_ParamTblPtr->FW_ACRO_Y_MAX < FW_ACRO_Y_MAX_MIN ||
        FAC_ParamTblPtr->FW_ACRO_Y_MAX > FW_ACRO_Y_MAX_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_ACRO_Y_MAX");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (FAC_ParamTblPtr->FW_ACRO_Z_MAX < FW_ACRO_Z_MAX_MIN ||
        FAC_ParamTblPtr->FW_ACRO_Z_MAX > FW_ACRO_Z_MAX_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_ACRO_Z_MAX");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }
    
    if (FAC_ParamTblPtr->FW_RATT_TH < FW_RATT_TH_MIN ||
        FAC_ParamTblPtr->FW_RATT_TH > FW_RATT_TH_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_RATT_TH");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }

    if (FAC_ParamTblPtr->FW_AIRSPD_MIN < FW_AIRSPD_MIN_MIN ||
        FAC_ParamTblPtr->FW_AIRSPD_MIN > FW_AIRSPD_MIN_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_AIRSPD_MIN");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }

    if (FAC_ParamTblPtr->FW_AIRSPD_MAX < FW_AIRSPD_MAX_MIN ||
        FAC_ParamTblPtr->FW_AIRSPD_MAX > FW_AIRSPD_MAX_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_AIRSPD_MAX");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }

    if (FAC_ParamTblPtr->FW_AIRSPD_TRIM < FW_AIRSPD_TRIM_MIN ||
        FAC_ParamTblPtr->FW_AIRSPD_TRIM > FW_AIRSPD_TRIM_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "FW_AIRSPD_TRIM");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }

    if (FAC_ParamTblPtr->TRIM_ROLL < TRIM_ROLL_MIN ||
        FAC_ParamTblPtr->TRIM_ROLL > TRIM_ROLL_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "TRIM_ROLL");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }

    if (FAC_ParamTblPtr->TRIM_PITCH < TRIM_PITCH_MIN ||
        FAC_ParamTblPtr->TRIM_PITCH > TRIM_PITCH_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "TRIM_PITCH");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }

    if (FAC_ParamTblPtr->TRIM_YAW < TRIM_YAW_MIN ||
        FAC_ParamTblPtr->TRIM_YAW > TRIM_YAW_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "TRIM_YAW");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }

    if (FAC_ParamTblPtr->VT_TYPE < VT_TYPE_MIN ||
        FAC_ParamTblPtr->VT_TYPE > VT_TYPE_MAX)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "VT_TYPE");
        iStatus = -1;
        goto FAC_ValidateConfigTbl_Exit_Tag;
    }

FAC_ValidateConfigTbl_Exit_Tag:

    if (iStatus != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(FAC_TBL_VALIDATE_ERR_EID, CFE_EVS_ERROR,
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
int32 FAC::AcquireConfigPointers(void)
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
        CFE_EVS_SendEvent(FAC_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to manage PWM Config table (0x%08X)",
                                 (unsigned int)iStatus);
        goto FAC_AcquireConfigPointers_Exit_Tag;
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
        CFE_EVS_SendEvent(FAC_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to get Param table's address (0x%08X)",
                                 (unsigned int)iStatus);
    }

FAC_AcquireConfigPointers_Exit_Tag:
    return (iStatus);

}


#ifdef __cplusplus
}
#endif

/************************/
/*  End of File Comment */
/************************/
    
