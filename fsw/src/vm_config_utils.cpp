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

#include "vm_app.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>

/************************************************************************
 ** Function Definitions
 *************************************************************************/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Config Table                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 VM::InitConfigTbl()
{
    int32 iStatus=0;

    /* Register Config table */
    iStatus = CFE_TBL_Register(&ConfigTblHdl,
            VM_CONFIG_TABLENAME,
            (sizeof(VM_ConfigTbl_t)),
            CFE_TBL_OPT_DEFAULT,
            VM::ValidateConfigTbl);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, a critical table could return another nominal code.  If this table is
         * made critical this logic would have to change. */
        (void) CFE_EVS_SendEvent(VM_CFGTBL_REG_ERR_EID, CFE_EVS_ERROR,
                "Failed to register config table (0x%08lX)",
                iStatus);
        goto VM_InitConfigTbl_Exit_Tag;
    }

    /* Load Config table file */
    iStatus = CFE_TBL_Load(ConfigTblHdl,
            CFE_TBL_SRC_FILE,
            VM_CONFIG_TABLE_FILENAME);
    if (iStatus != CFE_SUCCESS)
    {
        /* Note, CFE_SUCCESS is for a successful full table load.  If a partial table
         load is desired then this logic would have to change. */
        (void) CFE_EVS_SendEvent(VM_CFGTBL_LOAD_ERR_EID, CFE_EVS_ERROR,
                "Failed to load Config Table (0x%08lX)",
                iStatus);
        goto VM_InitConfigTbl_Exit_Tag;
    }

    iStatus = AcquireConfigPointers();

    VM_InitConfigTbl_Exit_Tag:
    return iStatus;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate VM Configuration Table                                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 VM::ValidateConfigTbl(void* ConfigTblPtr)
{
    int32 iStatus=0;
    VM_ConfigTbl_t* VM_ConfigTblPtr = (VM_ConfigTbl_t*)(ConfigTblPtr);

    /* TODO:  Add validation code here. */

    VM_ValidateConfigTbl_Exit_Tag:
    return iStatus;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Acquire Config Pointers                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 VM::AcquireConfigPointers(void)
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
        (void) CFE_EVS_SendEvent(VM_CFGTBL_MANAGE_ERR_EID, CFE_EVS_ERROR,
                "Failed to manage VM Config table (0x%08lX)",
                iStatus);
        goto VM_AcquireConfigPointers_Exit_Tag;
    }

    /*
     ** Get a pointer to the table
     */
    iStatus = CFE_TBL_GetAddress((void**)&ConfigTblPtr, ConfigTblHdl);
    if (iStatus == CFE_TBL_INFO_UPDATED)
    {
        iStatus = CFE_SUCCESS;
    }
    else if(iStatus != CFE_SUCCESS)
    {
        ConfigTblPtr = 0;
        (void) CFE_EVS_SendEvent(VM_CFGTBL_GETADDR_ERR_EID, CFE_EVS_ERROR,
                "Failed to get Config table's address (0x%08lX)",
                iStatus);
    }

    VM_AcquireConfigPointers_Exit_Tag:
    return iStatus;
}

#ifdef __cplusplus
}
#endif

/************************/
/*  End of File Comment */
/************************/

