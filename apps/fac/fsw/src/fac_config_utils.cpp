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
/* Validate Configuration Table                                    */
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

    if (InvalidFW_R_TC(FAC_ParamTblPtr->FW_R_TC))
    {
        err_code |= 0x1;
        strcat(Err_Msg, "FW_R_TC | ");
    }
    
    if (InvalidFW_P_TC(FAC_ParamTblPtr->FW_P_TC))
    {
        err_code |= 0x2;
        strcat(Err_Msg, "FW_P_TC | ");
    }
    
    if (InvalidFW_PR_P(FAC_ParamTblPtr->FW_PR_P))
    {
        err_code |= 0x4;
        strcat(Err_Msg, "FW_PR_P |");
    }
    
    if (InvalidFW_PR_I(FAC_ParamTblPtr->FW_PR_I))
    {
        err_code |= 0x8;
        strcat(Err_Msg, "FW_PR_I | ");
    }
    
    if (InvalidFW_P_RMAX_POS(FAC_ParamTblPtr->FW_P_RMAX_POS))
    {
        err_code |= 0x10;
        strcat(Err_Msg, "FW_P_RMAX_POS | ");
    }
    
    if (InvalidFW_P_RMAX_NEG(FAC_ParamTblPtr->FW_P_RMAX_NEG))
    {
        err_code |= 0x20;
        strcat(Err_Msg, "FW_P_RMAX_NEG | ");
    }
    
    if (InvalidFW_PR_IMAX(FAC_ParamTblPtr->FW_PR_IMAX))
    {
        err_code |= 0x40;
        strcat(Err_Msg, "FW_PR_IMAX | ");
    }
    
    if (InvalidFW_RR_P(FAC_ParamTblPtr->FW_RR_P))
    {
        err_code |= 0x80;
        strcat(Err_Msg, "FW_RR_P | ");
    }
    
    if (InvalidFW_RR_I(FAC_ParamTblPtr->FW_RR_I))
    {
        err_code |= 0x100;
        strcat(Err_Msg, "FW_RR_I | ");
    }
    
    if (InvalidFW_RR_IMAX(FAC_ParamTblPtr->FW_RR_IMAX))
    {
        err_code |= 0x200;
        strcat(Err_Msg, "FW_RR_IMAX | ");
    }
    
    if (InvalidFW_R_RMAX(FAC_ParamTblPtr->FW_R_RMAX))
    {
        err_code |= 0x400;
        strcat(Err_Msg, "FW_R_RMAX | ");
    }
    
    if (InvalidFW_YR_P(FAC_ParamTblPtr->FW_YR_P))
    {
        err_code |= 0x800;
        strcat(Err_Msg, "FW_YR_P | ");
    }
    
    if (InvalidFW_YR_I(FAC_ParamTblPtr->FW_YR_I))
    {
        err_code |= 0x1000;
        strcat(Err_Msg, "FW_YR_I | ");
    }
    
    if (InvalidFW_YR_IMAX(FAC_ParamTblPtr->FW_YR_IMAX))
    {
        err_code |= 0x2000;
        strcat(Err_Msg, "FW_YR_IMAX | ");
    }
    
    if (InvalidFW_Y_RMAX(FAC_ParamTblPtr->FW_Y_RMAX))
    {
        err_code |= 0x4000;
        strcat(Err_Msg, "FW_Y_RMAX | ");
    }
    
    if (InvalidFW_RLL_TO_YAW_FF(FAC_ParamTblPtr->FW_RLL_TO_YAW_FF))
    {
        err_code |= 0x8000;
        strcat(Err_Msg, "FW_RLL_TO_YAW_FF | ");
    }

    if (InvalidFW_WR_P(FAC_ParamTblPtr->FW_WR_P))
    {
        err_code |= 0x10000;
        strcat(Err_Msg, "FW_WR_P | ");
    }

    if (InvalidFW_WR_I(FAC_ParamTblPtr->FW_WR_I))
    {
        err_code |= 0x20000;
        strcat(Err_Msg, "FW_WR_I | ");
    }

    if (InvalidFW_WR_IMAX(FAC_ParamTblPtr->FW_WR_IMAX))
    {
        err_code |= 0x40000;
        strcat(Err_Msg, "FW_WR_IMAX | ");
    }

    if (InvalidFW_W_RMAX(FAC_ParamTblPtr->FW_W_RMAX))
    {
        err_code |= 0x80000;
        strcat(Err_Msg, "FW_W_RMAX | ");
    }

    if (InvalidFW_RR_FF(FAC_ParamTblPtr->FW_RR_FF))
    {
        err_code |= 0x100000;
        strcat(Err_Msg, "FW_RR_FF | ");
    }

    if (InvalidFW_PR_FF(FAC_ParamTblPtr->FW_PR_FF))
    {
        err_code |= 0x200000;
        strcat(Err_Msg, "FW_PR_FF | ");
    }

    if (InvalidFW_YR_FF(FAC_ParamTblPtr->FW_YR_FF))
    {
        err_code |= 0x400000;
        strcat(Err_Msg, "FW_YR_FF | ");
    }

    if (InvalidFW_WR_FF(FAC_ParamTblPtr->FW_WR_FF))
    {
        err_code |= 0x800000;
        strcat(Err_Msg, "FW_WR_FF | ");
    }

    if (InvalidFW_YCO_VMIN(FAC_ParamTblPtr->FW_YCO_VMIN))
    {
        err_code |= 0x1000000;
        strcat(Err_Msg, "FW_YCO_VMIN | ");
    }
    
    if (InvalidFW_YCO_METHOD(FAC_ParamTblPtr->FW_YCO_METHOD))
    {
        err_code |= 0x2000000;
        strcat(Err_Msg, "FW_YCO_METHOD | ");
    }

    if (InvalidFW_RSP_OFF(FAC_ParamTblPtr->FW_RSP_OFF))
    {
        err_code |= 0x4000000;
        strcat(Err_Msg, "FW_RSP_OFF | ");
    }

    if (InvalidFW_PSP_OFF(FAC_ParamTblPtr->FW_PSP_OFF))
    {
        err_code |= 0x8000000;
        strcat(Err_Msg, "FW_PSP_OFF | ");
    }

    if (InvalidFW_MAN_R_MAX(FAC_ParamTblPtr->FW_MAN_R_MAX))
    {
        err_code |= 0x10000000;
        strcat(Err_Msg, "FW_MAN_R_MAX | ");
    }

    if (InvalidFW_MAN_P_MAX(FAC_ParamTblPtr->FW_MAN_P_MAX))
    {
        err_code |= 0x20000000;
        strcat(Err_Msg, "FW_MAN_P_MAX | ");
    }

    if (InvalidFW_FLAPS_SCL(FAC_ParamTblPtr->FW_FLAPS_SCL))
    {
        err_code |= 0x40000000;
        strcat(Err_Msg, "FW_FLAPS_SCL | ");
    }
    
    if (InvalidFW_FLAPERON_SCL(FAC_ParamTblPtr->FW_FLAPERON_SCL))
    {
        err_code |= 0x80000000;
        strcat(Err_Msg, "FW_FLAPERON_SCL | ");
    }    

    if (InvalidFW_MAN_R_SC(FAC_ParamTblPtr->FW_MAN_R_SC))
    {
        err_code |= 0x100000000;
        strcat(Err_Msg, "FW_MAN_R_SC | ");
    }

    if (InvalidFW_MAN_P_SC(FAC_ParamTblPtr->FW_MAN_P_SC))
    {
        err_code |= 0x200000000;
        strcat(Err_Msg, "FW_MAN_P_SC | ");
    }

    if (InvalidFW_MAN_Y_SC(FAC_ParamTblPtr->FW_MAN_Y_SC))
    {
        err_code |= 0x400000000;
        strcat(Err_Msg, "FW_MAN_Y_SC | ");
    }
    
    if (InvalidFW_ACRO_X_MAX(FAC_ParamTblPtr->FW_ACRO_X_MAX))
    {
        err_code |= 0x800000000;
        strcat(Err_Msg, "FW_ACRO_X_MAX | ");
    }
    
    if (InvalidFW_ACRO_Y_MAX(FAC_ParamTblPtr->FW_ACRO_Y_MAX))
    {
        err_code |= 0x1000000000;
        strcat(Err_Msg, "FW_ACRO_Y_MAX | ");
    }
    
    if (InvalidFW_ACRO_Z_MAX(FAC_ParamTblPtr->FW_ACRO_Z_MAX))
    {
        err_code |= 0x2000000000;
        strcat(Err_Msg, "FW_ACRO_Z_MAX | ");
    }
    
    if (InvalidFW_RATT_TH(FAC_ParamTblPtr->FW_RATT_TH))
    {
        err_code |= 0x4000000000;
        strcat(Err_Msg, "FW_RATT_TH | ");
    }

    if (InvalidFW_AIRSPD_MIN(FAC_ParamTblPtr->FW_AIRSPD_MIN))
    {
        err_code |= 0x8000000000;
        strcat(Err_Msg, "FW_AIRSPD_MIN | ");
    }

    if (InvalidFW_AIRSPD_MAX(FAC_ParamTblPtr->FW_AIRSPD_MAX))
    {
        err_code |= 0x10000000000;
        strcat(Err_Msg, "FW_AIRSPD_MAX | ");
    }

    if (InvalidFW_AIRSPD_TRIM(FAC_ParamTblPtr->FW_AIRSPD_TRIM))
    {
        err_code |= 0x20000000000;
        strcat(Err_Msg, "FW_AIRSPD_TRIM | ");
    }

    if (InvalidTRIM_ROLL(FAC_ParamTblPtr->TRIM_ROLL))
    {
        err_code |= 0x40000000000;
        strcat(Err_Msg, "TRIM_ROLL | ");
    }

    if (InvalidTRIM_PITCH(FAC_ParamTblPtr->TRIM_PITCH))
    {
        err_code |= 0x80000000000;
        strcat(Err_Msg, "TRIM_PITCH | ");
    }

    if (InvalidTRIM_YAW(FAC_ParamTblPtr->TRIM_YAW))
    {
        err_code |= 0x100000000000;
        strcat(Err_Msg, "TRIM_YAW | ");
    }

    if (InvalidVT_TYPE(FAC_ParamTblPtr->VT_TYPE))
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


void FAC::HandleTableUpdate(void)
{
    int32 Status = 0;

    Status = CFE_TBL_Modified(ParamTblHdl);
    if(Status != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(FAC_TBL_MODIFIED_ERROR_EID, CFE_EVS_ERROR,
                "CFE_TBL_Modified failed with code (%d)", Status);
    }
    UpdateParams();
}


osalbool FAC::InvalidFW_R_TC(float param)
{
    return (param < FW_R_TC_MIN || param > FW_R_TC_MAX);
}


osalbool FAC::InvalidFW_P_TC(float param)
{
    return (param <FW_P_TC_MIN || param > FW_P_TC_MAX);
}


osalbool FAC::InvalidFW_PR_P(float param)
{
    return (param < FW_PR_P_MIN || param > FW_PR_P_MAX);
}


osalbool FAC::InvalidFW_PR_I(float param)
{
    return (param < FW_PR_I_MIN || param > FW_PR_I_MAX);
}


osalbool FAC::InvalidFW_P_RMAX_POS(float param)
{
    return (param < FW_P_RMAX_POS_MIN || param > FW_P_RMAX_POS_MAX);
}


osalbool FAC::InvalidFW_P_RMAX_NEG(float param)
{
    return (param < FW_P_RMAX_NEG_MIN || param > FW_P_RMAX_NEG_MAX);
}


osalbool FAC::InvalidFW_PR_IMAX(float param)
{
    return (param < FW_PR_IMAX_MIN || param > FW_PR_IMAX_MAX);
}


osalbool FAC::InvalidFW_RR_P(float param)
{
    return (param < FW_RR_P_MIN || param > FW_RR_P_MAX);
}


osalbool FAC::InvalidFW_RR_I(float param)
{
    return (param < FW_RR_I_MIN || param > FW_RR_I_MAX);
}


osalbool FAC::InvalidFW_RR_IMAX(float param)
{
    return (param < FW_RR_IMAX_MIN || param > FW_RR_IMAX_MAX);
}


osalbool FAC::InvalidFW_R_RMAX(float param)
{
    return (param < FW_R_RMAX_MIN || param > FW_R_RMAX_MAX);
}


osalbool FAC::InvalidFW_YR_P(float param)
{
    return (param < FW_YR_P_MIN || param > FW_YR_P_MAX);
}


osalbool FAC::InvalidFW_YR_I(float param)
{
    return (param < FW_YR_I_MIN || param > FW_YR_I_MAX);
}


osalbool FAC::InvalidFW_YR_IMAX(float param)
{
    return (param < FW_YR_IMAX_MIN || param > FW_YR_IMAX_MAX);
}


osalbool FAC::InvalidFW_Y_RMAX(float param)
{
    return (param < FW_Y_RMAX_MIN || param > FW_Y_RMAX_MAX);
}


osalbool FAC::InvalidFW_RLL_TO_YAW_FF(float param)
{
    return (param < FW_RLL_TO_YAW_FF_MIN);
}


osalbool FAC::InvalidFW_W_EN(int32 param)
{
    return FALSE;
}


osalbool FAC::InvalidFW_WR_P(float param)
{
    return (param < FW_WR_P_MIN || param > FW_WR_P_MAX);
}


osalbool FAC::InvalidFW_WR_I(float param)
{
    return (param < FW_WR_I_MIN || param > FW_WR_I_MAX);
}


osalbool FAC::InvalidFW_WR_IMAX(float param)
{
    return (param < FW_WR_IMAX_MIN || param > FW_WR_IMAX_MAX);
}


osalbool FAC::InvalidFW_W_RMAX(float param)
{
    return (param < FW_W_RMAX_MIN || param > FW_W_RMAX_MAX);
}


osalbool FAC::InvalidFW_RR_FF(float param)
{
    return (param < FW_RR_FF_MIN || param > FW_RR_FF_MAX);
}


osalbool FAC::InvalidFW_PR_FF(float param)
{
    return (param < FW_PR_FF_MIN || param > FW_PR_FF_MAX);
}


osalbool FAC::InvalidFW_YR_FF(float param)
{
    return (param < FW_YR_FF_MIN || param > FW_YR_FF_MAX);
}


osalbool FAC::InvalidFW_WR_FF(float param)
{
    return (param < FW_WR_FF_MIN || param > FW_WR_FF_MAX);
}


osalbool FAC::InvalidFW_YCO_VMIN(float param)
{
    return (param < FW_YCO_VMIN_MIN || param > FW_YCO_VMIN_MAX);
}


osalbool FAC::InvalidFW_YCO_METHOD(int32 param)
{
    return (param < FW_YCO_METHOD_MIN || param > FW_YCO_METHOD_MAX);
}


osalbool FAC::InvalidFW_RSP_OFF(float param)
{
    return (param < FW_RSP_OFF_MIN || param > FW_RSP_OFF_MAX);
}


osalbool FAC::InvalidFW_PSP_OFF(float param)
{
    return (param < FW_PSP_OFF_MIN || param > FW_PSP_OFF_MAX);
}


osalbool FAC::InvalidFW_MAN_R_MAX(float param)
{
    return (param < FW_MAN_R_MAX_MIN || param > FW_MAN_R_MAX_MAX);
}


osalbool FAC::InvalidFW_MAN_P_MAX(float param)
{
    return (param < FW_MAN_P_MAX_MIN || param > FW_MAN_P_MAX_MAX);
}


osalbool FAC::InvalidFW_FLAPS_SCL(float param)
{
    return (param < FW_FLAPS_SCL_MIN || param > FW_FLAPS_SCL_MAX);
}


osalbool FAC::InvalidFW_FLAPERON_SCL(float param)
{
    return (param < FW_FLAPERON_SCL_MIN || param > FW_FLAPERON_SCL_MAX);
}


osalbool FAC::InvalidFW_ARSP_MODE(int32 param)
{
    return FALSE;
}


osalbool FAC::InvalidFW_MAN_R_SC(float param)
{
    return (param < FW_MAN_R_SC_MIN || param > FW_MAN_R_SC_MAX);
}


osalbool FAC::InvalidFW_MAN_P_SC(float param)
{
    return (param < FW_MAN_P_SC_MIN);
}


osalbool FAC::InvalidFW_MAN_Y_SC(float param)
{
    return (param < FW_MAN_Y_SC_MIN);
}


osalbool FAC::InvalidFW_BAT_SCALE_EN(int32 param)
{
    return FALSE;
}


osalbool FAC::InvalidFW_ACRO_X_MAX(float param)
{
    return (param < FW_ACRO_X_MAX_MIN || param > FW_ACRO_X_MAX_MAX);
}


osalbool FAC::InvalidFW_ACRO_Y_MAX(float param)
{
    return (param < FW_ACRO_Y_MAX_MIN || param > FW_ACRO_Y_MAX_MAX);
}


osalbool FAC::InvalidFW_ACRO_Z_MAX(float param)
{
    return (param < FW_ACRO_Z_MAX_MIN || param > FW_ACRO_Z_MAX_MAX);
}


osalbool FAC::InvalidFW_RATT_TH(float param)
{
    return (param < FW_RATT_TH_MIN || param > FW_RATT_TH_MAX);
}


osalbool FAC::InvalidFW_AIRSPD_MIN(float param)
{
    return (param < FW_AIRSPD_MIN_MIN || param > FW_AIRSPD_MIN_MAX);
}


osalbool FAC::InvalidFW_AIRSPD_MAX(float param)
{
    return (param < FW_AIRSPD_MAX_MIN || param > FW_AIRSPD_MAX_MAX);
}


osalbool FAC::InvalidFW_AIRSPD_TRIM(float param)
{
    return (param < FW_AIRSPD_TRIM_MIN || param > FW_AIRSPD_TRIM_MAX);
}


osalbool FAC::InvalidTRIM_ROLL(float param)
{
    return (param < TRIM_ROLL_MIN || param > TRIM_ROLL_MAX);
}


osalbool FAC::InvalidTRIM_PITCH(float param)
{
    return (param < TRIM_PITCH_MIN || param > TRIM_PITCH_MAX);
}


osalbool FAC::InvalidTRIM_YAW(float param)
{
    return (param < TRIM_YAW_MIN || param > TRIM_YAW_MAX);
}


osalbool FAC::InvalidVT_TYPE(uint32 param)
{
    return (param < VT_TYPE_MIN || param > VT_TYPE_MAX);
}

#ifdef __cplusplus
}
#endif

/************************/
/*  End of File Comment */
/************************/
    
