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

#include "fac_config_tbl_test.hpp"
#include "fac_test_utils.hpp"

#include "fac_msg.h"

#include "uttest.h"
#include "ut_osapi_stubs.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_es_hooks.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_cfe_time_stubs.h"

#include <string.h>

bool   bInitConfigTbl_SendEventHook = FALSE;


/**************************************************************************
 * Tests for FAC InitConfigTbl()
 **************************************************************************/
/**
 * Test FAC InitConfigTbl(), fail TBL Register
 */
void Test_FAC_InitConfigTbl_Fail_TblRegister(void)
{
    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_NO_ACCESS;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFAC.InitConfigTbl();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, fail TBL Register");
}


/**
 * Test FAC InitConfigTbl_Fail_ValidateParamTbl()
 */
void Test_FAC_InitConfigTbl_Fail_ValidateParamTbl(void)
{
    /* Set a fail result */
    int32 result = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oFAC.InitConfigTbl();
}


/**
 * Test FAC InitConfigTbl_Fail_ValidateParamTbl_EachParam()
 */
void Test_FAC_InitConfigTbl_Fail_ValidateParamTbl_EachParam(void)
{
    int32    resultInit = CFE_SUCCESS;
    uint32   Validate1 = 0x0;
    uint32   Validate2 = 0x0;

    /* Execute the function being tested */
    resultInit = oFAC.InitConfigTbl();
    if (resultInit == CFE_SUCCESS)
    {
        oFAC.ParamTblPtr->FW_R_TC = 0.3f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x1;
        }
        oFAC.ParamTblPtr->FW_R_TC = 0.4f;

        oFAC.ParamTblPtr->FW_P_TC = 0.1f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x2;
        }
        oFAC.ParamTblPtr->FW_P_TC = 0.4f;

        oFAC.ParamTblPtr->FW_PR_P = 0.004f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x4;
        }
        oFAC.ParamTblPtr->FW_PR_P = 0.01f;

        oFAC.ParamTblPtr->FW_PR_I = 0.004f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x8;
        }
        oFAC.ParamTblPtr->FW_PR_I = 0.02f;

        oFAC.ParamTblPtr->FW_P_RMAX_POS = -5.0f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x10;
        }
        oFAC.ParamTblPtr->FW_P_RMAX_POS = 5.0f;

        oFAC.ParamTblPtr->FW_P_RMAX_NEG = -20.0f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x20;
        }
        oFAC.ParamTblPtr->FW_P_RMAX_NEG = 20.0f;

        oFAC.ParamTblPtr->FW_PR_IMAX = -0.2f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x40;
        }
        oFAC.ParamTblPtr->FW_PR_IMAX = 0.2f;

        oFAC.ParamTblPtr->FW_RR_P = 0.004f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x80;
        }
        oFAC.ParamTblPtr->FW_RR_P = 0.01f;

        oFAC.ParamTblPtr->FW_RR_I = 0.004f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x100;
        }
        oFAC.ParamTblPtr->FW_RR_I = 0.01f;

        oFAC.ParamTblPtr->FW_RR_IMAX = -0.2f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x200;
        }
        oFAC.ParamTblPtr->FW_RR_IMAX = 0.2f;

        oFAC.ParamTblPtr->FW_R_RMAX = -10.0f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x400;
        }
        oFAC.ParamTblPtr->FW_R_RMAX = 10.0f;

        oFAC.ParamTblPtr->FW_YR_P = 0.004f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x800;
        }
        oFAC.ParamTblPtr->FW_YR_P = 0.02f;

        oFAC.ParamTblPtr->FW_YR_I = -0.2f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x1000;
        }
        oFAC.ParamTblPtr->FW_YR_I = 0.0f;

        oFAC.ParamTblPtr->FW_YR_IMAX = -0.1f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x2000;
        }
        oFAC.ParamTblPtr->FW_YR_IMAX = 0.2f;

        oFAC.ParamTblPtr->FW_Y_RMAX = -4.0f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x4000;
        }
        oFAC.ParamTblPtr->FW_Y_RMAX = 0.0f;

        oFAC.ParamTblPtr->FW_RLL_TO_YAW_FF = -0.1f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x8000;
        }
        oFAC.ParamTblPtr->FW_RLL_TO_YAW_FF = 0.0f;

        oFAC.ParamTblPtr->FW_WR_P = 0.004f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x10000;
        }
        oFAC.ParamTblPtr->FW_WR_P = 0.5f;

        oFAC.ParamTblPtr->FW_WR_I = 0.004f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x20000;
        }
        oFAC.ParamTblPtr->FW_WR_I = 0.1f;

        oFAC.ParamTblPtr->FW_WR_IMAX = -0.1f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x40000;
        }
        oFAC.ParamTblPtr->FW_WR_IMAX = 1.0f;

        oFAC.ParamTblPtr->FW_W_RMAX = -1.0f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x80000;
        }
        oFAC.ParamTblPtr->FW_W_RMAX = 0.0f;

        oFAC.ParamTblPtr->FW_RR_FF = -0.5f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x100000;
        }
        oFAC.ParamTblPtr->FW_RR_FF = 0.5f;

        oFAC.ParamTblPtr->FW_PR_FF = 11.5f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x200000;
        }
        oFAC.ParamTblPtr->FW_PR_FF = 0.5f;

        oFAC.ParamTblPtr->FW_YR_FF = 12.0f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x400000;
        }
        oFAC.ParamTblPtr->FW_YR_FF = 0.3f;

        oFAC.ParamTblPtr->FW_WR_FF = 11.0f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x800000;
        }
        oFAC.ParamTblPtr->FW_WR_FF = 0.2f;

        oFAC.ParamTblPtr->FW_YCO_VMIN = 1100.0f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x1000000;
        }
        oFAC.ParamTblPtr->FW_YCO_VMIN = 1000.0f;

        oFAC.ParamTblPtr->FW_YCO_METHOD = -1;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x2000000;
        }
        oFAC.ParamTblPtr->FW_YCO_METHOD = 0;

        oFAC.ParamTblPtr->FW_RSP_OFF = 91.0f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x4000000;
        }
        oFAC.ParamTblPtr->FW_RSP_OFF = 0.0f;

        oFAC.ParamTblPtr->FW_PSP_OFF = 91.0f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x8000000;
        }
        oFAC.ParamTblPtr->FW_PSP_OFF = 0.0f;

        oFAC.ParamTblPtr->FW_MAN_R_MAX = 91.0f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x10000000;
        }
        oFAC.ParamTblPtr->FW_MAN_R_MAX = 45.0f;

        oFAC.ParamTblPtr->FW_MAN_P_MAX = 91.0f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x20000000;
        }
        oFAC.ParamTblPtr->FW_MAN_P_MAX = 10.0f;

        oFAC.ParamTblPtr->FW_FLAPS_SCL = 1.1f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x40000000;
        }
        oFAC.ParamTblPtr->FW_FLAPS_SCL = 1.0f;

        oFAC.ParamTblPtr->FW_FLAPERON_SCL = 1.1f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate1 |= 0x80000000;
        }
        oFAC.ParamTblPtr->FW_FLAPERON_SCL = 0.0f;

        oFAC.ParamTblPtr->FW_MAN_R_SC = 1.1f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate2 |= 0x0;
        }
        oFAC.ParamTblPtr->FW_MAN_R_SC = 1.0f;

        oFAC.ParamTblPtr->FW_MAN_P_SC = -1.0f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate2 |= 0x1;
        }
        oFAC.ParamTblPtr->FW_MAN_P_SC = 1.0f;

        oFAC.ParamTblPtr->FW_MAN_Y_SC = -1.0f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate2 |= 0x2;
        }
        oFAC.ParamTblPtr->FW_MAN_Y_SC = 1.0f;

        oFAC.ParamTblPtr->FW_ACRO_X_MAX = 721.0f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate2 |= 0x4;
        }
        oFAC.ParamTblPtr->FW_ACRO_X_MAX = 90.0f;

        oFAC.ParamTblPtr->FW_ACRO_Y_MAX = 721.0f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate2 |= 0x8;
        }
        oFAC.ParamTblPtr->FW_ACRO_Y_MAX = 90.0f;

        oFAC.ParamTblPtr->FW_ACRO_Z_MAX = 181.0f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate2 |= 0x10;
        }
        oFAC.ParamTblPtr->FW_ACRO_Z_MAX = 45.0f;

        oFAC.ParamTblPtr->FW_RATT_TH = 1.8f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate2 |= 0x20;
        }
        oFAC.ParamTblPtr->FW_RATT_TH = 0.8f;

        oFAC.ParamTblPtr->FW_AIRSPD_MIN = 41.0f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate2 |= 0x40;
        }
        oFAC.ParamTblPtr->FW_AIRSPD_MIN = 10.0f;

        oFAC.ParamTblPtr->FW_AIRSPD_MAX = 41.0f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate2 |= 0x80;
        }
        oFAC.ParamTblPtr->FW_AIRSPD_MAX = 20.0f;

        oFAC.ParamTblPtr->FW_AIRSPD_TRIM = 41.0f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate2 |= 0x100;
        }
        oFAC.ParamTblPtr->FW_AIRSPD_TRIM = 15.0f;

        oFAC.ParamTblPtr->TRIM_ROLL = 0.26f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate2 |= 0x200;
        }
        oFAC.ParamTblPtr->TRIM_ROLL = 0.0f;

        oFAC.ParamTblPtr->TRIM_PITCH = 0.26f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate2 |= 0x400;
        }
        oFAC.ParamTblPtr->TRIM_PITCH = 0.0f;

        oFAC.ParamTblPtr->TRIM_YAW = 0.26f;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate2 |= 0x800;
        }
        oFAC.ParamTblPtr->TRIM_YAW = 0.0f;

        oFAC.ParamTblPtr->VT_TYPE = 3;
        if (oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr) == -1)
        {
            Validate2 |= 0x1000;
        }
        oFAC.ParamTblPtr->VT_TYPE = 0;
    }

    printf("resultInit: %d, Validate1: 0x%08x, Validate2: 0x%08x\n",
            (int)resultInit, (unsigned int)Validate1, (unsigned int)Validate2);

    /* Verify results */
    if ((resultInit == CFE_SUCCESS) && (Validate1 == 0xffffffff) && (Validate2 == 0x1fff))
    {
        UtAssert_True (TRUE, "InitConfigTbl Fail ValidateParamTbl_EachParam");
    }
    else
    {
        UtAssert_True (FALSE, "InitConfigTbl Fail ValidateParamTbl_EachParam");
    }
}


/**
 * Test FAC InitConfigTbl(), SendEventHook
 */
int32 Test_FAC_InitConfigTbl_SendEventHook
            (uint16 EventID, uint16 EventType, const char *EventText, ...)
{
    va_list  Ptr;
    char     Buf[256];
    char     *pSubStr1 = NULL;
    char     *pSubStr2 = NULL;

    va_start(Ptr, EventText);
    vsnprintf(Buf, (size_t)CFE_EVS_MAX_MESSAGE_LENGTH, EventText, Ptr);
    va_end(Ptr);

    printf("###InitConfigTbl_SendEventHook:\n");
    printf("%s\n", Buf);

    pSubStr1 = strstr(Buf, "FW_R_TC");
    pSubStr2 = strstr(Buf, "FW_RLL_TO_YAW_FF");

    printf("pSubStr1: %p, pSubStr2: %p\n", pSubStr1, pSubStr2);

    if ((pSubStr1 != NULL) && (pSubStr2 != NULL))
    {
        bInitConfigTbl_SendEventHook = TRUE;
    }
    else
    {
        bInitConfigTbl_SendEventHook = FALSE;
    }

    return 0;
}


/**
 * Test FAC InitConfigTbl_Fail_ValidateParamTbl_MultipleParam()
 */
void Test_FAC_InitConfigTbl_Fail_ValidateParamTbl_MultipleParam(void)
{
    int32    resultInit = CFE_SUCCESS;

    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX,
               (void*)&Test_FAC_InitConfigTbl_SendEventHook);

    /* Execute the function being tested */
    resultInit = oFAC.InitConfigTbl();
    if (resultInit == CFE_SUCCESS)
    {
        oFAC.ParamTblPtr->FW_R_TC = 0.3f;
        oFAC.ParamTblPtr->FW_RLL_TO_YAW_FF = -0.1f;

        oFAC.ValidateParamTbl((void*)oFAC.ParamTblPtr);
    }

    if ((resultInit == CFE_SUCCESS) && (bInitConfigTbl_SendEventHook == TRUE))
    {
        UtAssert_True (TRUE, "InitConfigTbl Fail ValidateParamTbl_MultipleParam");
    }
    else
    {
        UtAssert_True (FALSE, "InitConfigTbl Fail ValidateParamTbl_MultipleParam");
    }
}


/**
 * Test FAC InitConfigTbl(), ValidateParamTbl_Nominal
 */
void Test_FAC_InitConfigTbl_ValidateParamTbl_Nominal(void)
{
    /* Set a fail result */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oFAC.InitConfigTbl();

    /* Verify results */
    UtAssert_True (result == expected, "InitConfigTbl, ValidateParamTbl Nominal");
}


/**
 * Test FAC InitConfigTbl(), fail TBL Load
 */
void Test_FAC_InitConfigTbl_Fail_TblLoad(void)
{
    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_INFO_UPDATE_PENDING;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFAC.InitConfigTbl();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, fail TBL Load");
}


/**
 * Test FAC InitConfigTbl(), fail TBL Manage
 */
void Test_FAC_InitConfigTbl_Fail_TblManage(void)
{
    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_HANDLE;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MANAGE_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFAC.InitConfigTbl();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, fail TBL Manage");
}


/**
 * Test FAC InitConfigTbl(), fail TBL GetAddress
 */
void Test_FAC_InitConfigTbl_Fail_TblGetAddress(void)
{
    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_NEVER_LOADED;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFAC.InitConfigTbl();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, fail TBL GetAddress");
}


/**
 * Test FAC InitConfigTbl(), fail AcquireConfigPtrs
 */
void Test_FAC_InitConfigTbl_Fail_AcquireConfigPtrs(void)
{
    /* Set a fail result */
    int32 result = CFE_SUCCESS;
    int32 expected = CFE_TBL_ERR_INVALID_HANDLE;

    /* fail the register app */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, expected, 1);

    /* Execute the function being tested */
    result = oFAC.InitConfigTbl();

    /* Verify results */
    UtAssert_True (result == expected,
                   "InitConfigTbl, fail AcquireConfigPtrs");
}


/**
 * Test FAC InitConfigTbl(), Nominal
 */
void Test_FAC_InitConfigTbl_Nominal(void)
{
    /* Set a fail result */
    int32 result = (CFE_SEVERITY_BITMASK & CFE_SEVERITY_ERROR)
                   | CFE_EXECUTIVE_SERVICE | CFE_ES_ERR_APP_REGISTER;
    int32 expected = CFE_SUCCESS;

    /* Execute the function being tested */
    result = oFAC.InitConfigTbl();

    /* Verify results */
    UtAssert_True (result == expected, "InitConfigTbl, Nominal");
}



void FAC_Config_Tbl_Test_AddTestCases(void)
{
    UtTest_Add(Test_FAC_InitConfigTbl_Fail_TblRegister, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitConfigTbl_Fail_TblRegister");
    UtTest_Add(Test_FAC_InitConfigTbl_Fail_ValidateParamTbl, FAC_Test_Setup_ParamInvalid,
               FAC_Test_TearDown, "Test_FAC_InitConfigTbl_Fail_ValidateParamTbl");
    UtTest_Add(Test_FAC_InitConfigTbl_Fail_ValidateParamTbl_EachParam, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_InitConfigTbl_Fail_ValidateParamTbl_EachParam");
    UtTest_Add(Test_FAC_InitConfigTbl_Fail_ValidateParamTbl_MultipleParam, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_InitConfigTbl_Fail_ValidateParamTbl_MultipleParam");
    UtTest_Add(Test_FAC_InitConfigTbl_ValidateParamTbl_Nominal, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_InitConfigTbl_ValidateParamTbl_Nominal");
    UtTest_Add(Test_FAC_InitConfigTbl_Fail_TblLoad, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitConfigTbl_Fail_TblLoad");
    UtTest_Add(Test_FAC_InitConfigTbl_Fail_TblManage, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitConfigTbl_Fail_TblManage");
    UtTest_Add(Test_FAC_InitConfigTbl_Fail_TblGetAddress, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitConfigTbl_Fail_TblGetAddress");
    UtTest_Add(Test_FAC_InitConfigTbl_Fail_AcquireConfigPtrs, FAC_Test_Setup,
               FAC_Test_TearDown, "Test_FAC_InitConfigTbl_Fail_AcquireConfigPtrs");
    UtTest_Add(Test_FAC_InitConfigTbl_Nominal, FAC_Test_Setup, FAC_Test_TearDown,
               "Test_FAC_InitConfigTbl_Nominal");
}
