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


#include <math.h>
#include "vm_app.h"


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
    int32 iStatus = 0;

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
extern "C" int32 VM::ValidateConfigTbl(void* ConfigTblPtr)
{
	int32            status = CFE_SUCCESS;
    osalbool         valid = true;
    VM_ConfigTbl_t  *VM_ConfigTblPtr = (VM_ConfigTbl_t*)(ConfigTblPtr);

    valid = VM::Validate_COM_RC_IN_MODE(VM_ConfigTblPtr->COM_RC_IN_MODE);
    if(valid == false)
    {
    	status = -1;
    }

    valid = VM::Validate_COM_ARM_SWISBTN(VM_ConfigTblPtr->COM_ARM_SWISBTN);
    if(valid == false)
    {
    	status = -1;
    }

    valid = VM::Validate_COM_RC_ARM_HYST(VM_ConfigTblPtr->COM_RC_ARM_HYST);
    if(valid == false)
    {
    	status = -1;
    }

    valid = VM::Validate_MAV_SYS_ID(VM_ConfigTblPtr->MAV_SYS_ID);
    if(valid == false)
    {
    	status = -1;
    }

    valid = VM::Validate_MAV_COMP_ID(VM_ConfigTblPtr->MAV_COMP_ID);
    if(valid == false)
    {
    	status = -1;
    }

    valid = VM::Validate_COM_RC_LOSS_T(VM_ConfigTblPtr->COM_RC_LOSS_T);
    if(valid == false)
    {
    	status = -1;
    }

    valid = VM::Validate_COM_LOW_BAT_ACT(VM_ConfigTblPtr->COM_LOW_BAT_ACT);
    if(valid == false)
    {
    	status = -1;
    }

    valid = VM::Validate_COM_HOME_H_T(VM_ConfigTblPtr->COM_HOME_H_T);
    if(valid == false)
    {
    	status = -1;
    }

    valid = VM::Validate_COM_HOME_V_T(VM_ConfigTblPtr->COM_HOME_V_T);
    if(valid == false)
    {
    	status = -1;
    }

    return status;
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



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate COM_RC_IN_MODE                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool VM::Validate_COM_RC_IN_MODE(uint32 param)
{
	osalbool valid = true;

    if(param > COM_RC_IN_MODE_MAX)
    {
        CFE_EVS_SendEvent(VM_INVALID_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
                "Invalid Config Table.  COM_RC_IN_MODE.  Exp:<0-%u>  Act: %u", COM_RC_IN_MODE_MAX, (unsigned int) param);

        valid = false;
    }

    return valid;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate COM_ARM_SWISBTN                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool VM::Validate_COM_ARM_SWISBTN(uint32 param)
{
	osalbool valid = true;

    if(param > 1)
    {
        CFE_EVS_SendEvent(VM_INVALID_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
                "Invalid Config Table.  COM_ARM_SWISBTN.  Exp:<0-1>  Act: %u", (unsigned int) param);

        valid = false;
    }

    return valid;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate COM_RC_ARM_HYST                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool VM::Validate_COM_RC_ARM_HYST(uint32 param)
{
	osalbool valid = true;

    if((param < COM_RC_ARM_HYST_MIN) || (param > COM_RC_ARM_HYST_MAX))
    {
        CFE_EVS_SendEvent(VM_INVALID_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
                "Invalid Config Table.  COM_RC_ARM_HYST.  Exp:<%u-%u>  Act: %u", COM_RC_ARM_HYST_MIN, COM_RC_ARM_HYST_MAX, (unsigned int) param);

        valid = false;
    }

    return valid;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate MAV_SYS_ID                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool VM::Validate_MAV_SYS_ID(uint32 param)
{
	osalbool valid = true;

    if((param < MAV_SYS_ID_MIN) || (param > MAV_SYS_ID_MAX))
    {
        CFE_EVS_SendEvent(VM_INVALID_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
                "Invalid Config Table.  MAV_SYS_ID.  Exp:<%u-%u>  Act: %u", MAV_SYS_ID_MIN, MAV_SYS_ID_MAX, (unsigned int) param);

        valid = false;
    }

    return valid;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate MAV_COMP_ID                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool VM::Validate_MAV_COMP_ID(uint32 param)
{
	osalbool valid = true;

    if((param < MAV_COMP_ID_MIN) || (param > MAV_COMP_ID_MAX))
    {
        CFE_EVS_SendEvent(VM_INVALID_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
                "Invalid Config Table.  MAV_COMP_ID.  Exp:<%u-%u>  Act: %u", MAV_COMP_ID_MIN, MAV_COMP_ID_MAX, (unsigned int) param);

        valid = false;
    }

    return valid;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate COM_RC_LOSS_T                                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool VM::Validate_COM_RC_LOSS_T(float param)
{
	osalbool valid = true;

    if(param > COM_RC_LOSS_T_MAX)
    {
        CFE_EVS_SendEvent(VM_INVALID_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
                "Invalid Config Table.  COM_RC_LOSS_T.  Exp:<0.0-%f>  Act: %f", COM_RC_LOSS_T_MAX, param);

        valid = false;
    }

    return valid;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate COM_LOW_BAT_ACT                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool VM::Validate_COM_LOW_BAT_ACT(uint32 param)
{
	osalbool valid = true;

    if(param > COM_LOW_BAT_ACT_MAX)
    {
        CFE_EVS_SendEvent(VM_INVALID_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
                "Invalid Config Table.  COM_LOW_BAT_ACT.  Exp:<0-%u>  Act: %u", COM_LOW_BAT_ACT_MAX, (unsigned int) param);

        valid = false;
    }

    return valid;
}




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate COM_HOME_H_T                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool VM::Validate_COM_HOME_H_T(float param)
{
	osalbool valid = true;

    if((param < COM_HOME_H_T_MIN) || (param > COM_HOME_H_T_MAX))
    {
        CFE_EVS_SendEvent(VM_INVALID_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
                "Invalid Config Table.  COM_HOME_H_T.  Exp:<%f-%f>  Act: %f", COM_HOME_H_T_MIN, COM_HOME_H_T_MAX, param);

        valid = false;
    }

    return valid;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Validate COM_HOME_V_T                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
osalbool VM::Validate_COM_HOME_V_T(float param)
{
	osalbool valid = true;

    if((param < COM_HOME_V_T_MIN) || (param > COM_HOME_V_T_MAX))
    {
        CFE_EVS_SendEvent(VM_INVALID_CONFIG_TABLE_ERR_EID, CFE_EVS_ERROR,
                "Invalid Config Table.  COM_HOME_V_T.  Exp:<%f-%f>  Act: %f", COM_HOME_V_T_MIN, COM_HOME_V_T_MAX, param);

        valid = false;
    }

    return valid;
}



/************************/
/*  End of File Comment */
/************************/

