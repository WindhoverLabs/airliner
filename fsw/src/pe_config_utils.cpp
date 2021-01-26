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

#include "pe_app.h"
#include <math.h>

/************************************************************************
** Function Definitions
*************************************************************************/


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Config Table                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PE::InitConfigTbl()
{
    int32  iStatus=0;

    /* Register Config table */
    iStatus = CFE_TBL_Register(&ConfigTblHdl,
        PE_CONFIG_TABLENAME,
        (sizeof(PE_ConfigTbl_t)),
        CFE_TBL_OPT_DEFAULT,
        PE::ValidateConfigTbl);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, a critical table could return another nominal code.  If this table is
         * made critical this logic would have to change. */
        (void) CFE_EVS_SendEvent(PE_CFGTBL_REG_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to register config table (0x%08lX)",
                                 iStatus);
        goto PE_InitConfigTbl_Exit_Tag;
    }

    /* Load Config table file */
    iStatus = CFE_TBL_Load(ConfigTblHdl,
                           CFE_TBL_SRC_FILE,
                           PE_CONFIG_TABLE_FILENAME);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, CFE_SUCCESS is for a successful full table load.  If a partial table
           load is desired then this logic would have to change. */
        (void) CFE_EVS_SendEvent(PE_CFGTBL_LOAD_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to load Config Table (0x%08lX)",
                                 iStatus);
        goto PE_InitConfigTbl_Exit_Tag;
    }

    iStatus = AcquireConfigPointers();

PE_InitConfigTbl_Exit_Tag:
    return iStatus;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate PE Configuration Table                                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PE::ValidateConfigTbl(void* ConfigTblPtr)
{
    int32  iStatus = CFE_SUCCESS;
    char Param[PE_PARAM_NAME_MAX_LEN];
    PE_ConfigTbl_t* PE_ConfigTblPtr = (PE_ConfigTbl_t*)(ConfigTblPtr);

    if (PE_ConfigTblPtr->VXY_PUB_THRESH < VXY_PUB_THRESH_MIN ||
        PE_ConfigTblPtr->VXY_PUB_THRESH > VXY_PUB_THRESH_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "VXY_PUB_THRESH");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }

    if (PE_ConfigTblPtr->Z_PUB_THRESH < Z_PUB_THRESH_MIN ||
        PE_ConfigTblPtr->Z_PUB_THRESH > Z_PUB_THRESH_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "Z_PUB_THRESH");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->ACCEL_XY_STDDEV < ACCEL_XY_STDDEV_MIN ||
        PE_ConfigTblPtr->ACCEL_XY_STDDEV > ACCEL_XY_STDDEV_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "ACCEL_XY_STDDEV");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->ACCEL_Z_STDDEV < ACCEL_Z_STDDEV_MIN ||
        PE_ConfigTblPtr->ACCEL_Z_STDDEV > ACCEL_Z_STDDEV_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "ACCEL_Z_STDDEV");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->BARO_FUSE < BARO_FUSE_MIN ||
        PE_ConfigTblPtr->BARO_FUSE > BARO_FUSE_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "BARO_FUSE");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->BARO_STDDEV < BARO_STDDEV_MIN ||
        PE_ConfigTblPtr->BARO_STDDEV > BARO_STDDEV_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "BARO_STDDEV");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->GPS_FUSE < GPS_FUSE_MIN ||
        PE_ConfigTblPtr->GPS_FUSE > GPS_FUSE_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "GPS_FUSE");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->GPS_DELAY < GPS_DELAY_MIN ||
        PE_ConfigTblPtr->GPS_DELAY > GPS_DELAY_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "GPS_DELAY");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->GPS_XY_STDDEV < GPS_XY_STDDEV_MIN ||
        PE_ConfigTblPtr->GPS_XY_STDDEV > GPS_XY_STDDEV_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "GPS_XY_STDDEV");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->GPS_Z_STDDEV < GPS_Z_STDDEV_MIN ||
        PE_ConfigTblPtr->GPS_Z_STDDEV > GPS_Z_STDDEV_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "GPS_Z_STDDEV");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->GPS_VXY_STDDEV < GPS_VXY_STDDEV_MIN ||
        PE_ConfigTblPtr->GPS_VXY_STDDEV > GPS_VXY_STDDEV_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "GPS_VXY_STDDEV");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->GPS_VZ_STDDEV < GPS_VZ_STDDEV_MIN ||
        PE_ConfigTblPtr->GPS_VZ_STDDEV > GPS_VZ_STDDEV_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "GPS_VZ_STDDEV");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->GPS_EPH_MAX < GPS_EPH_MAX_MIN ||
        PE_ConfigTblPtr->GPS_EPH_MAX > GPS_EPH_MAX_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "GPS_EPH_MAX");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->GPS_EPV_MAX < GPS_EPV_MAX_MIN ||
        PE_ConfigTblPtr->GPS_EPV_MAX > GPS_EPV_MAX_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "GPS_EPV_MAX");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->LAND_FUSE < LAND_FUSE_MIN ||
        PE_ConfigTblPtr->LAND_FUSE > LAND_FUSE_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "LAND_FUSE");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->LAND_Z_STDDEV < LAND_Z_STDDEV_MIN ||
        PE_ConfigTblPtr->LAND_Z_STDDEV > LAND_Z_STDDEV_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "LAND_Z_STDDEV");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->LAND_VXY_STDDEV < LAND_VXY_STDDEV_MIN ||
        PE_ConfigTblPtr->LAND_VXY_STDDEV > LAND_VXY_STDDEV_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "LAND_VXY_STDDEV");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->PN_P_NOISE_DENSITY < PN_P_NOISE_DENSITY_MIN ||
        PE_ConfigTblPtr->PN_P_NOISE_DENSITY > PN_P_NOISE_DENSITY_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "PN_P_NOISE_DENSITY");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->PN_V_NOISE_DENSITY < PN_V_NOISE_DENSITY_MIN ||
        PE_ConfigTblPtr->PN_V_NOISE_DENSITY > PN_V_NOISE_DENSITY_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "PN_V_NOISE_DENSITY");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->PN_B_NOISE_DENSITY < PN_B_NOISE_DENSITY_MIN ||
        PE_ConfigTblPtr->PN_B_NOISE_DENSITY > PN_B_NOISE_DENSITY_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "PN_B_NOISE_DENSITY");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->PN_T_NOISE_DENSITY < PN_T_NOISE_DENSITY_MIN ||
        PE_ConfigTblPtr->PN_T_NOISE_DENSITY > PN_T_NOISE_DENSITY_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "PN_T_NOISE_DENSITY");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->T_MAX_GRADE < T_MAX_GRADE_MIN ||
        PE_ConfigTblPtr->T_MAX_GRADE > T_MAX_GRADE_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "T_MAX_GRADE");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->FAKE_ORIGIN < FAKE_ORIGIN_MIN ||
        PE_ConfigTblPtr->FAKE_ORIGIN > FAKE_ORIGIN_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "FAKE_ORIGIN");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->INIT_ORIGIN_LAT < INIT_ORIGIN_LAT_MIN ||
        PE_ConfigTblPtr->INIT_ORIGIN_LAT > INIT_ORIGIN_LAT_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "INIT_ORIGIN_LAT");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->INIT_ORIGIN_LON < INIT_ORIGIN_LON_MIN ||
        PE_ConfigTblPtr->INIT_ORIGIN_LON > INIT_ORIGIN_LON_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "INIT_ORIGIN_LON");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->DIST_FUSE < DIST_FUSE_MIN ||
        PE_ConfigTblPtr->DIST_FUSE > DIST_FUSE_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "DIST_FUSE");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->DIST_STDDEV < DIST_STDDEV_MIN ||
        PE_ConfigTblPtr->DIST_STDDEV > DIST_STDDEV_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "DIST_STDDEV");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->DIST_OFF_Z < DIST_OFF_Z_MIN ||
        PE_ConfigTblPtr->DIST_OFF_Z > DIST_OFF_Z_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "DIST_OFF_Z");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->FLOW_FUSE < FLOW_FUSE_MIN ||
        PE_ConfigTblPtr->FLOW_FUSE > FLOW_FUSE_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "FLOW_FUSE");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->FLOW_SCALE < FLOW_SCALE_MIN ||
        PE_ConfigTblPtr->FLOW_SCALE > FLOW_SCALE_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "FLOW_SCALE");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->FLOW_R < FLOW_R_MIN ||
        PE_ConfigTblPtr->FLOW_R > FLOW_R_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "FLOW_R");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->FLOW_RR < FLOW_RR_MIN ||
        PE_ConfigTblPtr->FLOW_RR > FLOW_RR_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "FLOW_RR");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->FLOW_QUALITY_MIN < FLOW_QUALITY_MIN_MIN ||
        PE_ConfigTblPtr->FLOW_QUALITY_MIN > FLOW_QUALITY_MIN_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "FLOW_QUALITY_MIN");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
    if (PE_ConfigTblPtr->FLOW_MIN_AGL < FLOW_MIN_AGL_MIN ||
        PE_ConfigTblPtr->FLOW_MIN_AGL > FLOW_MIN_AGL_MAX)
    {
        snprintf(Param, PE_PARAM_NAME_MAX_LEN, "FLOW_MIN_AGL");
        iStatus = -1;
        goto PE_ValidateConfigTbl_Exit_Tag;
    }
    
PE_ValidateConfigTbl_Exit_Tag:

    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(PE_TBL_VALIDATE_ERR_EID, CFE_EVS_ERROR,
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
int32 PE::AcquireConfigPointers(void)
{
    int32 iStatus = CFE_SUCCESS;

    /*
    ** Release the table
    */
    (void) CFE_TBL_ReleaseAddress(ConfigTblHdl);

    /*
    ** Manage the table
    */
    iStatus = CFE_TBL_Manage(ConfigTblHdl);
    if ((iStatus != CFE_SUCCESS) && (iStatus != CFE_TBL_INFO_UPDATED))
    {
        (void) CFE_EVS_SendEvent(PE_CFGTBL_MANAGE_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to manage PE Config table (0x%08lX)",
                                 iStatus);
        goto PE_AcquireConfigPointers_Exit_Tag;
    }

    /*
    ** Get a pointer to the table
    */
    iStatus = CFE_TBL_GetAddress((void**)&ConfigTblPtr, ConfigTblHdl);
    if (iStatus == CFE_TBL_INFO_UPDATED)
    {
        m_ParamsUpdated = true;
        UpdateLocalParams();
        iStatus = CFE_SUCCESS;
    }
    else if(iStatus != CFE_SUCCESS)
    {
        ConfigTblPtr = 0;
        (void) CFE_EVS_SendEvent(PE_CFGTBL_GETADDR_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to get Config table's address (0x%08lX)",
                                 iStatus);
    }

PE_AcquireConfigPointers_Exit_Tag:
    return iStatus;
}


#ifdef __cplusplus
}
#endif

/************************/
/*  End of File Comment */
/************************/
    
