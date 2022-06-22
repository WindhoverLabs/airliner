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
extern "C"
{
#endif

#include "amc_app.h"
#include <math.h>

/************************************************************************
 ** Function Definitions
 *************************************************************************/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Config Table                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 AMC::InitConfigTbl(void)
{
    int32 iStatus=0;

    /* Register Config table */
    iStatus = CFE_TBL_Register(&ConfigTblHdl,
            AMC_CONFIG_TABLENAME,
            (sizeof(AMC_ConfigTbl_t)),
            CFE_TBL_OPT_DEFAULT,
            AMC::ValidateCfgTbl);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, a critical table could return another nominal code.  If this
         * table is made critical this logic would have to change.
         */
        CFE_EVS_SendEvent(AMC_CFGTBL_REG_ERR_EID, CFE_EVS_ERROR,
                "Failed to register config table (0x%08X)",
                (unsigned int)iStatus);
        goto AMC_InitConfigTbl_Exit_Tag;
    }

    /* Register Mixer Config table */
    iStatus = CFE_TBL_Register(&MixerConfigTblHdl,
            AMC_MIXER_CONFIG_TABLENAME,
            (sizeof(AMC_Mixer_ConfigTable_t)),
            CFE_TBL_OPT_DEFAULT,
            0);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, a critical table could return another nominal code.  If this
         * table is made critical this logic would have to change.
         */
        CFE_EVS_SendEvent(AMC_CFGTBL_REG_ERR_EID, CFE_EVS_ERROR,
                "Failed to register mixer table (0x%08X)",
                (unsigned int)iStatus);
        goto AMC_InitConfigTbl_Exit_Tag;
    }

    /* Load Config table file */
    iStatus = CFE_TBL_Load(ConfigTblHdl,
            CFE_TBL_SRC_FILE,
            AMC_CONFIG_TABLE_FILENAME);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, CFE_SUCCESS is for a successful full table load.  If a
         * partial table load is desired then this logic would have to
         * change.
         */
        CFE_EVS_SendEvent(AMC_CFGTBL_LOAD_ERR_EID, CFE_EVS_ERROR,
                "Failed to load Config Table (0x%08X)",
                (unsigned int)iStatus);
        goto AMC_InitConfigTbl_Exit_Tag;
    }

    /* Load Config table file */
    iStatus = CFE_TBL_Load(MixerConfigTblHdl,
            CFE_TBL_SRC_FILE,
            AMC_MIXER_CONFIG_TABLE_FILENAME);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, CFE_SUCCESS is for a successful full table load.  If a
         * partial table load is desired then this logic would have to
         * change.
         */
        CFE_EVS_SendEvent(AMC_CFGTBL_LOAD_ERR_EID, CFE_EVS_ERROR,
                "Failed to load Mixer Table (0x%08X)",
                (unsigned int)iStatus);
        goto AMC_InitConfigTbl_Exit_Tag;
    }

    iStatus = AcquireConfigPointers();

AMC_InitConfigTbl_Exit_Tag:
    return iStatus;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate Configuration Table                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 AMC::ValidateCfgTbl(void* ConfigTblPtr)
{
    int32 iStatus=0;
    AMC_ConfigTbl_t* AMC_ConfigTblPtr =
            (AMC_ConfigTbl_t*)(ConfigTblPtr);

    if(AMC_ConfigTblPtr->PwmMin < AMC_ConfigTblPtr->PwmDisarmed)
    {
        CFE_EVS_SendEvent(AMC_CFGTBL_MIN_LT_DISARMED_ERR_EID,
                CFE_EVS_ERROR,
                "Tbl Vldt: Min (%u) less than Disarmed (%u) speed.",
                (unsigned int)AMC_ConfigTblPtr->PwmMin,
                (unsigned int)AMC_ConfigTblPtr->PwmDisarmed);
        iStatus = -1;
        goto AMC_ValidateCfgTbl_Exit_Tag;
    }

    if(AMC_ConfigTblPtr->PwmMax < AMC_ConfigTblPtr->PwmMin)
    {
        CFE_EVS_SendEvent(AMC_CFGTBL_MAX_LT_MIN_ERR_EID, CFE_EVS_ERROR,
                "Tbl Vldt: Max (%u) less than Min (%u) speed.",
                (unsigned int)AMC_ConfigTblPtr->PwmMax,
                (unsigned int)AMC_ConfigTblPtr->PwmMin);
        iStatus = -1;
        goto AMC_ValidateCfgTbl_Exit_Tag;
    }

AMC_ValidateCfgTbl_Exit_Tag:
    return iStatus;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Acquire Config Pointers                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 AMC::AcquireConfigPointers(void)
{
    int32 iStatus = CFE_SUCCESS;

    /*
     ** Release the table
     */
    /* TODO: This return value can indicate success, error, or that the info
     * has been updated.  We ignore this return value in favor of checking
     * CFE_TBL_Manage(), but be sure this is the behavior you want.
     */
    CFE_TBL_ReleaseAddress(ConfigTblHdl);

    CFE_TBL_ReleaseAddress(MixerConfigTblHdl);

    /*
     ** Manage the table
     */
    iStatus = CFE_TBL_Manage(ConfigTblHdl);
    if ((iStatus != CFE_SUCCESS) && (iStatus != CFE_TBL_INFO_UPDATED))
    {
        CFE_EVS_SendEvent(AMC_CFGTBL_MANAGE_ERR_EID, CFE_EVS_ERROR,
                "Failed to manage Config table (0x%08X)",
                (unsigned int)iStatus);
        goto AMC_AcquireConfigPointers_Exit_Tag;
    }

    /*
     ** Manage the table
     */
    iStatus = CFE_TBL_Manage(MixerConfigTblHdl);
    if ((iStatus != CFE_SUCCESS) && (iStatus != CFE_TBL_INFO_UPDATED))
    {
        CFE_EVS_SendEvent(AMC_CFGTBL_MANAGE_ERR_EID, CFE_EVS_ERROR,
                "Failed to manage Mixer Config table (0x%08X)",
                (unsigned int)iStatus);
        goto AMC_AcquireConfigPointers_Exit_Tag;
    }

    /*
     ** Get a pointer to the table
     */
    iStatus = CFE_TBL_GetAddress((void**)&ConfigTblPtr,
            ConfigTblHdl);
    if (iStatus == CFE_TBL_INFO_UPDATED)
    {
        iStatus = CFE_SUCCESS;
    }
    else if(iStatus != CFE_SUCCESS)
    {
        ConfigTblPtr = 0;
        CFE_EVS_SendEvent(AMC_CFGTBL_GETADDR_ERR_EID, CFE_EVS_ERROR,
                "Failed to get Config table's address (0x%08X)",
                (unsigned int)iStatus);
    }

    /*
     ** Get a pointer to the table
     */
    iStatus = CFE_TBL_GetAddress((void**)&MixerConfigTblPtr,
    		MixerConfigTblHdl);
    if (iStatus == CFE_TBL_INFO_UPDATED)
    {
        iStatus = CFE_SUCCESS;
    }
    else if(iStatus != CFE_SUCCESS)
    {
    	MixerConfigTblPtr = 0;
        CFE_EVS_SendEvent(AMC_CFGTBL_GETADDR_ERR_EID, CFE_EVS_ERROR,
                "Failed to get Mixer Config table's address (0x%08X)",
                (unsigned int)iStatus);
    }

	for(uint32 i = 0; i < AMC_MULTIROTOR_MIXER_MAX_MIXERS; ++i)
	{
	    MultirotorMixerObject[i].SetConfig(&MixerConfigTblPtr->Multirotor[i]);
	}

	for(uint32 i = 0; i < AMC_SIMPLE_MIXER_MAX_MIXERS; ++i)
	{
	    SimpleMixerObject[i].SetConfig(&MixerConfigTblPtr->Simple[i]);
	}

AMC_AcquireConfigPointers_Exit_Tag:
    return iStatus;

}

#ifdef __cplusplus
}
#endif

/************************/
/*  End of File Comment */
/************************/

