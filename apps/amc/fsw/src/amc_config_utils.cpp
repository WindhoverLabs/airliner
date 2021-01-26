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
    iStatus = CFE_TBL_Register(&PwmConfigTblHdl,
            AMC_PWM_CONFIG_TABLENAME,
            (sizeof(AMC_PwmConfigTbl_t)),
            CFE_TBL_OPT_DEFAULT,
            AMC::ValidatePwmCfgTbl);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, a critical table could return another nominal code.  If this
         * table is made critical this logic would have to change.
         */
        CFE_EVS_SendEvent(AMC_PWM_CFGTBL_REG_ERR_EID, CFE_EVS_ERROR,
                "Failed to register PWM table (0x%08X)",
                (unsigned int)iStatus);
        goto AMC_InitConfigTbl_Exit_Tag;
    }

    /* Register Mixer table */
    iStatus = CFE_TBL_Register(&MixerConfigTblHdl,
            AMC_MIXER_CONFIG_TABLENAME,
            (sizeof(MultirotorMixer_ConfigTable_t)),
            CFE_TBL_OPT_DEFAULT,
            AMC::ValidateMixerCfgTbl);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, a critical table could return another nominal code.  If this
         * table is made critical this logic would have to change.
         */
        CFE_EVS_SendEvent(AMC_MIXER_CFGTBL_REG_ERR_EID, CFE_EVS_ERROR,
                "Failed to register Mixer table (0x%08X)",
                (unsigned int)iStatus);
        goto AMC_InitConfigTbl_Exit_Tag;
    }

    /* Load Config table file */
    iStatus = CFE_TBL_Load(PwmConfigTblHdl,
            CFE_TBL_SRC_FILE,
            AMC_PWM_CONFIG_TABLE_FILENAME);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, CFE_SUCCESS is for a successful full table load.  If a
         * partial table load is desired then this logic would have to
         * change.
         */
        CFE_EVS_SendEvent(AMC_PWM_CFGTBL_LOAD_ERR_EID, CFE_EVS_ERROR,
                "Failed to load PWM Config Table (0x%08X)",
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
        CFE_EVS_SendEvent(AMC_MIXER_CFGTBL_LOAD_ERR_EID, CFE_EVS_ERROR,
                "Failed to load Mixer Config Table (0x%08X)",
                (unsigned int)iStatus);
        goto AMC_InitConfigTbl_Exit_Tag;
    }

    iStatus = AcquireConfigPointers();

AMC_InitConfigTbl_Exit_Tag:
    return iStatus;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate PWM Configuration Table                                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 AMC::ValidatePwmCfgTbl(void* ConfigTblPtr)
{
    uint32 i = 0;
    int32 iStatus=0;
    AMC_PwmConfigTbl_t* AMC_PwmConfigTblPtr =
            (AMC_PwmConfigTbl_t*)(ConfigTblPtr);

    if(AMC_PwmConfigTblPtr->PwmMin < AMC_PwmConfigTblPtr->PwmDisarmed)
    {
        CFE_EVS_SendEvent(AMC_PWM_CFGTBL_MIN_LT_DISARMED_ERR_EID,
                CFE_EVS_ERROR,
                "PWM Tbl Vldt: Min (%u) less than Disarmed (%u) speed.",
                (unsigned int)AMC_PwmConfigTblPtr->PwmMin,
                (unsigned int)AMC_PwmConfigTblPtr->PwmDisarmed);
        iStatus = -1;
        goto AMC_ValidatePwmCfgTbl_Exit_Tag;
    }

    if(AMC_PwmConfigTblPtr->PwmMax < AMC_PwmConfigTblPtr->PwmMin)
    {
        CFE_EVS_SendEvent(AMC_PWM_CFGTBL_MAX_LT_MIN_ERR_EID, CFE_EVS_ERROR,
                "PWM Tbl Vldt: Max (%u) less than Min (%u) speed.",
                (unsigned int)AMC_PwmConfigTblPtr->PwmMax,
                (unsigned int)AMC_PwmConfigTblPtr->PwmMin);
        iStatus = -1;
        goto AMC_ValidatePwmCfgTbl_Exit_Tag;
    }

AMC_ValidatePwmCfgTbl_Exit_Tag:
    return iStatus;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate Mixer Configuration Table                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 AMC::ValidateMixerCfgTbl(void* ConfigTblPtr)
{
    uint32 i = 0;
    int32 iStatus=0;
    MultirotorMixer_ConfigTable_t* TblPtr =
            (MultirotorMixer_ConfigTable_t*)(ConfigTblPtr);
    boolean rollScaleNotFinite = FALSE;
    boolean pitchScaleNotFinite = FALSE;
    boolean yawScaleNotFinite = FALSE;
    boolean idleSpeedNotFinite = FALSE;
    boolean deltaOutMaxNotFinite = FALSE;
    boolean invalidGeometry = FALSE;
    boolean invalidRotorCount = FALSE;
    boolean outScaleNotFinite = FALSE;

    rollScaleNotFinite = !isfinite(TblPtr->RollScale);
    pitchScaleNotFinite = !isfinite(TblPtr->PitchScale);
    yawScaleNotFinite = !isfinite(TblPtr->YawScale);
    idleSpeedNotFinite = !isfinite(TblPtr->IdleSpeed);
    deltaOutMaxNotFinite = !isfinite(TblPtr->DeltaOutMax);
    if(TblPtr->Geometry >= MIXER_MAX_GEOMETRY)
    {
        invalidGeometry = TRUE;
    }

    if((TblPtr->RotorCount < 1) ||
            (TblPtr->RotorCount < AMC_MAX_MOTOR_OUTPUTS))
    {
        invalidRotorCount = TRUE;
    }

    if(rollScaleNotFinite || pitchScaleNotFinite || yawScaleNotFinite
            || idleSpeedNotFinite || invalidGeometry )
    {
        CFE_EVS_SendEvent(AMC_MIXER_INVALID_MIXER_ERR_EID, CFE_EVS_ERROR,
                "Mixer cfg tbl invld.  R[%u] P[%u] Y[%u] I[%u] G[%u] \
                D[%u] RC[%u]",
                rollScaleNotFinite, pitchScaleNotFinite, yawScaleNotFinite,
                idleSpeedNotFinite, invalidGeometry, deltaOutMaxNotFinite,
                invalidRotorCount);
        iStatus = -1;
        goto AMC_ValidateMixerCfgTbl_Exit_Tag;
    }

    for(i = 0; i < TblPtr->RotorCount; ++i)
    {
        rollScaleNotFinite = !isfinite(TblPtr->RotorConfig[i].RollScale);
        pitchScaleNotFinite = !isfinite(TblPtr->RotorConfig[i].PitchScale);
        yawScaleNotFinite = !isfinite(TblPtr->RotorConfig[i].YawScale);
        outScaleNotFinite = !isfinite(TblPtr->RotorConfig[i].OutScale);

        if(rollScaleNotFinite || pitchScaleNotFinite || yawScaleNotFinite
                || outScaleNotFinite )
        {
            CFE_EVS_SendEvent(AMC_MIXER_INVLD_ROTOR_CONFIG_ERR_EID,
                    CFE_EVS_ERROR, "Mixer cfg tbl invld rotor[%u] config.  \
                            R[%u] P[%u] Y[%u] O[%u]",
                    (unsigned int)i, rollScaleNotFinite, pitchScaleNotFinite,
                    yawScaleNotFinite,
                    outScaleNotFinite);
            iStatus = -1;
            goto AMC_ValidateMixerCfgTbl_Exit_Tag;
        }
    }

    for(i = TblPtr->RotorCount; i < AMC_MAX_MOTOR_OUTPUTS; ++i)
    {
        boolean unusedRollScaleNotZero = FALSE;
        boolean unusedPitchScaleNotZero = FALSE;
        boolean unusedYawScaleNotZero = FALSE;
        boolean unusedOutScaleNotZero = FALSE;

        if((uint32)TblPtr->RotorConfig[i].RollScale != 0)
        {
            unusedRollScaleNotZero = TRUE;
        }

        if((uint32)TblPtr->RotorConfig[i].PitchScale != 0)
        {
            unusedPitchScaleNotZero = TRUE;
        }

        if((uint32)TblPtr->RotorConfig[i].YawScale != 0)
        {
            unusedYawScaleNotZero = TRUE;
        }

        if((uint32)TblPtr->RotorConfig[i].OutScale != 0)
        {
            unusedOutScaleNotZero = TRUE;
        }

        if(unusedRollScaleNotZero || unusedPitchScaleNotZero ||
                unusedYawScaleNotZero || unusedOutScaleNotZero)
        {
            CFE_EVS_SendEvent(AMC_MIXER_TBLVAL_NONZER_DATA_ERR_EID,
                    CFE_EVS_ERROR,
                    "Mixer cfg tbl contains unused nonzero data.");
            iStatus = -1;
            goto AMC_ValidateMixerCfgTbl_Exit_Tag;
        }
    }

AMC_ValidateMixerCfgTbl_Exit_Tag:
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
    CFE_TBL_ReleaseAddress(PwmConfigTblHdl);

    CFE_TBL_ReleaseAddress(MixerConfigTblHdl);

    /*
     ** Manage the table
     */
    iStatus = CFE_TBL_Manage(PwmConfigTblHdl);
    if ((iStatus != CFE_SUCCESS) && (iStatus != CFE_TBL_INFO_UPDATED))
    {
        CFE_EVS_SendEvent(AMC_PWM_CFGTBL_MANAGE_ERR_EID, CFE_EVS_ERROR,
                "Failed to manage PWM Config table (0x%08X)",
                (unsigned int)iStatus);
        goto AMC_AcquireConfigPointers_Exit_Tag;
    }

    iStatus = CFE_TBL_Manage(MixerConfigTblHdl);
    if ((iStatus != CFE_SUCCESS) && (iStatus != CFE_TBL_INFO_UPDATED))
    {
        CFE_EVS_SendEvent(AMC_MIXER_CFGTBL_MANAGE_ERR_EID, CFE_EVS_ERROR,
                "Failed to manage Mixer Config table (0x%08X)",
                (unsigned int)iStatus);
        goto AMC_AcquireConfigPointers_Exit_Tag;
    }

    /*
     ** Get a pointer to the table
     */
    iStatus = CFE_TBL_GetAddress((void**)&PwmConfigTblPtr,
            PwmConfigTblHdl);
    if (iStatus == CFE_TBL_INFO_UPDATED)
    {
        iStatus = CFE_SUCCESS;
    }
    else if(iStatus != CFE_SUCCESS)
    {
        PwmConfigTblPtr = 0;
        CFE_EVS_SendEvent(AMC_PWM_CFGTBL_GETADDR_ERR_EID, CFE_EVS_ERROR,
                "Failed to get PWM Config table's address (0x%08X)",
                (unsigned int)iStatus);
    }

    iStatus = CFE_TBL_GetAddress((void**)&MixerConfigTblPtr,
            MixerConfigTblHdl);
    if (iStatus == CFE_TBL_INFO_UPDATED)
    {
        iStatus = CFE_SUCCESS;
    }
    else if(iStatus != CFE_SUCCESS)
    {
        MixerConfigTblPtr = 0;
        CFE_EVS_SendEvent(AMC_MIXER_CFGTBL_GETADDR_ERR_EID, CFE_EVS_ERROR,
                "Failed to get Mixer Config table's address (0x%08X)",
                (unsigned int)iStatus);
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

