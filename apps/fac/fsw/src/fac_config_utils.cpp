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

#include <string.h>

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
    uint32 err_code = 0x0;
    FAC_ParamTbl_t* FAC_ParamTblPtr = (FAC_ParamTbl_t*)(ConfigTblPtr);
    char Param[FAC_PARAM_NAME_MAX_LEN];
    char Err_Msg[2048];

    CFE_PSP_MemSet((void*)Param, 0x00, sizeof(Param));
    CFE_PSP_MemSet((void*)Err_Msg, 0x00, sizeof(Err_Msg));

    if (FAC_ParamTblPtr->FW_R_TC < FW_R_TC_MIN ||
        FAC_ParamTblPtr->FW_R_TC > FW_R_TC_MAX)
    {
        err_code |= 0x1;
        strcat(Err_Msg, "FW_R_TC | ");
    }
    
    if (FAC_ParamTblPtr->FW_P_TC <FW_P_TC_MIN ||
        FAC_ParamTblPtr->FW_P_TC > FW_P_TC_MAX)
    {
        err_code |= 0x2;
        strcat(Err_Msg, "FW_P_TC | ");
    }
    
    if (FAC_ParamTblPtr->FW_PR_P < FW_PR_P_MIN ||
        FAC_ParamTblPtr->FW_PR_P > FW_PR_P_MAX)
    {
        err_code |= 0x4;
        strcat(Err_Msg, "FW_PR_P |");
    }
    
    if (FAC_ParamTblPtr->FW_PR_I < FW_PR_I_MIN ||
        FAC_ParamTblPtr->FW_PR_I > FW_PR_I_MAX)
    {
        err_code |= 0x8;
        strcat(Err_Msg, "FW_PR_I | ");
    }
    
    if (FAC_ParamTblPtr->FW_P_RMAX_POS < FW_P_RMAX_POS_MIN ||
        FAC_ParamTblPtr->FW_P_RMAX_POS > FW_P_RMAX_POS_MAX)
    {
        err_code |= 0x10;
        strcat(Err_Msg, "FW_P_RMAX_POS | ");
    }
    
    if (FAC_ParamTblPtr->FW_P_RMAX_NEG < FW_P_RMAX_NEG_MIN ||
        FAC_ParamTblPtr->FW_P_RMAX_NEG > FW_P_RMAX_NEG_MAX)
    {
        err_code |= 0x20;
        strcat(Err_Msg, "FW_P_RMAX_NEG | ");
    }
    
    if (FAC_ParamTblPtr->FW_PR_IMAX < FW_PR_IMAX_MIN ||
        FAC_ParamTblPtr->FW_PR_IMAX > FW_PR_IMAX_MAX)
    {
        err_code |= 0x40;
        strcat(Err_Msg, "FW_PR_IMAX | ");
    }
    
    if (FAC_ParamTblPtr->FW_RR_P < FW_RR_P_MIN ||
        FAC_ParamTblPtr->FW_RR_P > FW_RR_P_MAX)
    {
        err_code |= 0x80;
        strcat(Err_Msg, "FW_RR_P | ");
    }
    
    if (FAC_ParamTblPtr->FW_RR_I < FW_RR_I_MIN ||
        FAC_ParamTblPtr->FW_RR_I > FW_RR_I_MAX)
    {
        err_code |= 0x100;
        strcat(Err_Msg, "FW_RR_I | ");
    }
    
    if (FAC_ParamTblPtr->FW_RR_IMAX < FW_RR_IMAX_MIN ||
        FAC_ParamTblPtr->FW_RR_IMAX > FW_RR_IMAX_MAX)
    {
        err_code |= 0x200;
        strcat(Err_Msg, "FW_RR_IMAX | ");
    }
    
    if (FAC_ParamTblPtr->FW_R_RMAX < FW_R_RMAX_MIN ||
        FAC_ParamTblPtr->FW_R_RMAX > FW_R_RMAX_MAX)
    {
        err_code |= 0x400;
        strcat(Err_Msg, "FW_R_RMAX | ");
    }
    
    if (FAC_ParamTblPtr->FW_YR_P < FW_YR_P_MIN ||
        FAC_ParamTblPtr->FW_YR_P > FW_YR_P_MAX)
    {
        err_code |= 0x800;
        strcat(Err_Msg, "FW_YR_P | ");
    }
    
    if (FAC_ParamTblPtr->FW_YR_I < FW_YR_I_MIN ||
        FAC_ParamTblPtr->FW_YR_I > FW_YR_I_MAX)
    {
        err_code |= 0x1000;
        strcat(Err_Msg, "FW_YR_I | ");
    }
    
    if (FAC_ParamTblPtr->FW_YR_IMAX < FW_YR_IMAX_MIN ||
        FAC_ParamTblPtr->FW_YR_IMAX > FW_YR_IMAX_MAX)
    {
        err_code |= 0x2000;
        strcat(Err_Msg, "FW_YR_IMAX | ");
    }
    
    if (FAC_ParamTblPtr->FW_Y_RMAX < FW_Y_RMAX_MIN ||
        FAC_ParamTblPtr->FW_Y_RMAX > FW_Y_RMAX_MAX)
    {
        err_code |= 0x4000;
        strcat(Err_Msg, "FW_Y_RMAX | ");
    }
    
    if (FAC_ParamTblPtr->FW_RLL_TO_YAW_FF < FW_RLL_TO_YAW_FF_MIN ||
        FAC_ParamTblPtr->FW_RLL_TO_YAW_FF > FW_RLL_TO_YAW_FF_MAX)
    {
        err_code |= 0x8000;
        strcat(Err_Msg, "FW_RLL_TO_YAW_FF | ");
    }

    if (FAC_ParamTblPtr->FW_WR_P < FW_WR_P_MIN ||
        FAC_ParamTblPtr->FW_WR_P > FW_WR_P_MAX)
    {
        err_code |= 0x10000;
        strcat(Err_Msg, "FW_WR_P | ");
    }

    if (FAC_ParamTblPtr->FW_WR_I < FW_WR_I_MIN ||
        FAC_ParamTblPtr->FW_WR_I > FW_WR_I_MAX)
    {
        err_code |= 0x20000;
        strcat(Err_Msg, "FW_WR_I | ");
    }

    if (FAC_ParamTblPtr->FW_WR_IMAX < FW_WR_IMAX_MIN ||
        FAC_ParamTblPtr->FW_WR_IMAX > FW_WR_IMAX_MAX)
    {
        err_code |= 0x40000;
        strcat(Err_Msg, "FW_WR_IMAX | ");
    }

    if (FAC_ParamTblPtr->FW_W_RMAX < FW_W_RMAX_MIN ||
        FAC_ParamTblPtr->FW_W_RMAX > FW_W_RMAX_MAX)
    {
        err_code |= 0x80000;
        strcat(Err_Msg, "FW_W_RMAX | ");
    }

    if (FAC_ParamTblPtr->FW_RR_FF < FW_RR_FF_MIN ||
        FAC_ParamTblPtr->FW_RR_FF > FW_RR_FF_MAX)
    {
        err_code |= 0x100000;
        strcat(Err_Msg, "FW_RR_FF | ");
    }

    if (FAC_ParamTblPtr->FW_PR_FF < FW_PR_FF_MIN ||
        FAC_ParamTblPtr->FW_PR_FF > FW_PR_FF_MAX)
    {
        err_code |= 0x200000;
        strcat(Err_Msg, "FW_PR_FF | ");
    }

    if (FAC_ParamTblPtr->FW_YR_FF < FW_YR_FF_MIN ||
        FAC_ParamTblPtr->FW_YR_FF > FW_YR_FF_MAX)
    {
        err_code |= 0x400000;
        strcat(Err_Msg, "FW_YR_FF | ");
    }

    if (FAC_ParamTblPtr->FW_WR_FF < FW_WR_FF_MIN ||
        FAC_ParamTblPtr->FW_WR_FF > FW_WR_FF_MAX)
    {
        err_code |= 0x800000;
        strcat(Err_Msg, "FW_WR_FF | ");
    }

    if (FAC_ParamTblPtr->FW_YCO_VMIN < FW_YCO_VMIN_MIN ||
        FAC_ParamTblPtr->FW_YCO_VMIN > FW_YCO_VMIN_MAX)
    {
        err_code |= 0x1000000;
        strcat(Err_Msg, "FW_YCO_VMIN | ");
    }
    
    if (FAC_ParamTblPtr->FW_YCO_METHOD < FW_YCO_METHOD_MIN ||
        FAC_ParamTblPtr->FW_YCO_METHOD > FW_YCO_METHOD_MAX)
    {
        err_code |= 0x2000000;
        strcat(Err_Msg, "FW_YCO_METHOD | ");
    }

    if (FAC_ParamTblPtr->FW_RSP_OFF < FW_RSP_OFF_MIN ||
        FAC_ParamTblPtr->FW_RSP_OFF > FW_RSP_OFF_MAX)
    {
        err_code |= 0x4000000;
        strcat(Err_Msg, "FW_RSP_OFF | ");
    }

    if (FAC_ParamTblPtr->FW_PSP_OFF < FW_PSP_OFF_MIN ||
        FAC_ParamTblPtr->FW_PSP_OFF > FW_PSP_OFF_MAX)
    {
        err_code |= 0x8000000;
        strcat(Err_Msg, "FW_PSP_OFF | ");
    }

    if (FAC_ParamTblPtr->FW_MAN_R_MAX < FW_MAN_R_MAX_MIN ||
        FAC_ParamTblPtr->FW_MAN_R_MAX > FW_MAN_R_MAX_MAX)
    {
        err_code |= 0x10000000;
        strcat(Err_Msg, "FW_MAN_R_MAX | ");
    }

    if (FAC_ParamTblPtr->FW_MAN_P_MAX < FW_MAN_P_MAX_MIN ||
        FAC_ParamTblPtr->FW_MAN_P_MAX > FW_MAN_P_MAX_MAX)
    {
        err_code |= 0x20000000;
        strcat(Err_Msg, "FW_MAN_P_MAX | ");
    }

    if (FAC_ParamTblPtr->FW_FLAPS_SCL < FW_FLAPS_SCL_MIN ||
        FAC_ParamTblPtr->FW_FLAPS_SCL > FW_FLAPS_SCL_MAX)
    {
        err_code |= 0x40000000;
        strcat(Err_Msg, "FW_FLAPS_SCL | ");
    }
    
    if (FAC_ParamTblPtr->FW_FLAPERON_SCL < FW_FLAPERON_SCL_MIN ||
        FAC_ParamTblPtr->FW_FLAPERON_SCL > FW_FLAPERON_SCL_MAX)
    {
        err_code |= 0x80000000;
        strcat(Err_Msg, "FW_FLAPERON_SCL | ");
    }    

    if (FAC_ParamTblPtr->FW_MAN_R_SC < FW_MAN_R_SC_MIN ||
        FAC_ParamTblPtr->FW_MAN_R_SC > FW_MAN_R_SC_MAX)
    {
        err_code |= 0x100000000;
        strcat(Err_Msg, "FW_MAN_R_SC | ");
    }

    if (FAC_ParamTblPtr->FW_MAN_P_SC < FW_MAN_P_SC_MIN)
    {
        err_code |= 0x200000000;
        strcat(Err_Msg, "FW_MAN_P_SC | ");
    }

    if (FAC_ParamTblPtr->FW_MAN_Y_SC < FW_MAN_Y_SC_MIN)
    {
        err_code |= 0x400000000;
        strcat(Err_Msg, "FW_MAN_Y_SC | ");
    }
    
    if (FAC_ParamTblPtr->FW_ACRO_X_MAX < FW_ACRO_X_MAX_MIN ||
        FAC_ParamTblPtr->FW_ACRO_X_MAX > FW_ACRO_X_MAX_MAX)
    {
        err_code |= 0x800000000;
        strcat(Err_Msg, "FW_ACRO_X_MAX | ");
    }
    
    if (FAC_ParamTblPtr->FW_ACRO_Y_MAX < FW_ACRO_Y_MAX_MIN ||
        FAC_ParamTblPtr->FW_ACRO_Y_MAX > FW_ACRO_Y_MAX_MAX)
    {
        err_code |= 0x1000000000;
        strcat(Err_Msg, "FW_ACRO_Y_MAX | ");
    }
    
    if (FAC_ParamTblPtr->FW_ACRO_Z_MAX < FW_ACRO_Z_MAX_MIN ||
        FAC_ParamTblPtr->FW_ACRO_Z_MAX > FW_ACRO_Z_MAX_MAX)
    {
        err_code |= 0x2000000000;
        strcat(Err_Msg, "FW_ACRO_Z_MAX | ");
    }
    
    if (FAC_ParamTblPtr->FW_RATT_TH < FW_RATT_TH_MIN ||
        FAC_ParamTblPtr->FW_RATT_TH > FW_RATT_TH_MAX)
    {
        err_code |= 0x4000000000;
        strcat(Err_Msg, "FW_RATT_TH | ");
    }

    if (FAC_ParamTblPtr->FW_AIRSPD_MIN < FW_AIRSPD_MIN_MIN ||
        FAC_ParamTblPtr->FW_AIRSPD_MIN > FW_AIRSPD_MIN_MAX)
    {
        err_code |= 0x8000000000;
        strcat(Err_Msg, "FW_AIRSPD_MIN | ");
    }

    if (FAC_ParamTblPtr->FW_AIRSPD_MAX < FW_AIRSPD_MAX_MIN ||
        FAC_ParamTblPtr->FW_AIRSPD_MAX > FW_AIRSPD_MAX_MAX)
    {
        err_code |= 0x10000000000;
        strcat(Err_Msg, "FW_AIRSPD_MAX | ");
    }

    if (FAC_ParamTblPtr->FW_AIRSPD_TRIM < FW_AIRSPD_TRIM_MIN ||
        FAC_ParamTblPtr->FW_AIRSPD_TRIM > FW_AIRSPD_TRIM_MAX)
    {
        err_code |= 0x20000000000;
        strcat(Err_Msg, "FW_AIRSPD_TRIM | ");
    }

    if (FAC_ParamTblPtr->TRIM_ROLL < TRIM_ROLL_MIN ||
        FAC_ParamTblPtr->TRIM_ROLL > TRIM_ROLL_MAX)
    {
        err_code |= 0x40000000000;
        strcat(Err_Msg, "TRIM_ROLL | ");
    }

    if (FAC_ParamTblPtr->TRIM_PITCH < TRIM_PITCH_MIN ||
        FAC_ParamTblPtr->TRIM_PITCH > TRIM_PITCH_MAX)
    {
        err_code |= 0x80000000000;
        strcat(Err_Msg, "TRIM_PITCH | ");
    }

    if (FAC_ParamTblPtr->TRIM_YAW < TRIM_YAW_MIN ||
        FAC_ParamTblPtr->TRIM_YAW > TRIM_YAW_MAX)
    {
        err_code |= 0x100000000000;
        strcat(Err_Msg, "TRIM_YAW | ");
    }

    if (FAC_ParamTblPtr->VT_TYPE < VT_TYPE_MIN ||
        FAC_ParamTblPtr->VT_TYPE > VT_TYPE_MAX)
    {
        err_code |= 0x200000000000;
        strcat(Err_Msg, "VT_TYPE");
    }

    if (err_code != 0x0)
    {
        snprintf(Param, FAC_PARAM_NAME_MAX_LEN, "%s", Err_Msg);
        iStatus = -1;
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
    
