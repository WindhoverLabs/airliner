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


#include "sch_config_tbl_test.h"
#include "sch_test_utils.h"

#include "sch_app.h"
#include "sch_cmds.h"
#include "sch_events.h"

#include "uttest.h"
#include "ut_osapi_stubs.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_es_hooks.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_cfe_time_stubs.h"


/**************************************************************************
 * Tests for SCH_TblInit()
 **************************************************************************/
/**
 * SCH_TblInit_Test_RegisterSdtError
 */
void SCH_TblInit_Test_RegisterSdtError(void)
{
    int32  Result = CFE_SUCCESS;
    int32  Expected = CFE_TBL_ERR_NO_ACCESS;
    char   expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, Expected, 1);

    /* Execute the function being tested */
    SCH_EvsInit();
    Result = SCH_TblInit();

    sprintf(expEventText, "Error Registering SDT, RC=0x%08X",
                          (unsigned int)Expected);

    /* Verify results */
    UtAssert_True(SCH_AppData.ScheduleTableHandle == CFE_TBL_BAD_TABLE_HANDLE,
               "SCH_AppData.ScheduleTableHandle == CFE_TBL_BAD_TABLE_HANDLE");
    UtAssert_True(SCH_AppData.MessageTableHandle  == CFE_TBL_BAD_TABLE_HANDLE,
               "SCH_AppData.MessageTableHandle  == CFE_TBL_BAD_TABLE_HANDLE");

    UtAssert_True(Result == Expected, "TblInit_Test_RegisterSdtError");

    UtAssert_EventSent(SCH_SDT_REG_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "TblInit_Test_RegisterSdtError Event Sent");
}


/**
 * SCH_TblInit_Test_RegisterMdtError
 */
void SCH_TblInit_Test_RegisterMdtError(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_TBL_ERR_NO_ACCESS;
    char    expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, Expected, 2);

    /* Execute the function being tested */
    SCH_EvsInit();
    Result = SCH_TblInit();

    sprintf(expEventText, "Error Registering MDT, RC=0x%08X",
                          (unsigned int)Expected);

    /* Verify results */
    UtAssert_True(SCH_AppData.MessageTableHandle  == CFE_TBL_BAD_TABLE_HANDLE,
                "SCH_AppData.MessageTableHandle  == CFE_TBL_BAD_TABLE_HANDLE");

    UtAssert_True(Result == Expected, "TblInit_Test_RegisterMdtError");

    UtAssert_EventSent(SCH_MDT_REG_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "TblInit_Test_RegisterMdtError Event Sent");
}


/**
 * SCH_TblInit_Test_RegisterDeadlineTblError
 */
void SCH_TblInit_Test_RegisterDeadlineTblError(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_TBL_ERR_NO_ACCESS;
    char    expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, Expected, 3);

    /* Execute the function being tested */
    SCH_EvsInit();
    Result = SCH_TblInit();

    sprintf(expEventText, "Error Registering Deadline, RC=0x%08X",
                          (unsigned int)Expected);

    /* Verify results */
    UtAssert_True(Result == Expected, "TblInit_Test_RegisterDeadlineTblError");

    UtAssert_EventSent(SCH_DEADLINE_REG_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "TblInit_Test_RegisterDeadlineTblError Event Sent");
}


/**
 * SCH_TblInit_Test_LoadSdtError
 */
void SCH_TblInit_Test_LoadSdtError(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_TBL_ERR_INVALID_HANDLE;
    char    expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    /* Set to satisfy subsequent condition "Status != CFE_SUCCESS" */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, Expected, 1);

    /* Execute the function being tested */
    SCH_EvsInit();
    Result = SCH_TblInit();

    sprintf(expEventText, "Error (RC=0x%08X) Loading SDT with %s",
            (unsigned int)Expected, SCH_SCHEDULE_FILENAME);

    /* Verify results */
    UtAssert_True(Result == Expected, "TblInit_Test_LoadSdtError");

    UtAssert_EventSent(SCH_SDT_LOAD_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "TblInit_Test_LoadSdtError Event Sent");
}


/**
 * SCH_TblInit_Test_LoadMdtError
 */
void SCH_TblInit_Test_LoadMdtError(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_TBL_ERR_INVALID_HANDLE;
    char    expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, Expected, 2);

    /* Execute the function being tested */
    SCH_EvsInit();
    Result = SCH_TblInit();

    sprintf(expEventText, "Error (RC=0x%08X) Loading MDT with %s",
                       (unsigned int)Expected, SCH_MESSAGE_FILENAME);

    /* Verify results */
    UtAssert_True(SCH_AppData.TableVerifySuccessCount == 1,
                  "SCH_AppData.TableVerifySuccessCount == 1");

    UtAssert_True(Result == Expected, "TblInit_Test_LoadMdtError");

    UtAssert_EventSent(SCH_MDT_LOAD_ERR_EID, CFE_EVS_ERROR,
             expEventText, "TblInit_Test_LoadMdtError Event Sent");
}


/**
 * SCH_TblInit_Test_SdtManageError
 */
void SCH_TblInit_Test_SdtManageError(void)
{
    int32  Result = CFE_SUCCESS;
    int32  Expected = CFE_TBL_ERR_NEVER_LOADED;
    char   expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MANAGE_INDEX, Expected, 1);

    /* Execute the function being tested */
    SCH_EvsInit();
    Result = SCH_TblInit();

    sprintf(expEventText, "Error Acquiring Tbl Ptrs (RC=0x%08X)",
                          (unsigned int)Expected);

    /* Verify results */
    UtAssert_True(Result == Expected, "TblInit_Test_SdtManageError");

    UtAssert_EventSent(SCH_ACQ_PTR_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "TblInit_Test_SdtManageError Event Sent");
}


/**
 * SCH_TblInit_Test_MdtManageError
 */
void SCH_TblInit_Test_MdtManageError(void)
{
    int32  Result = CFE_SUCCESS;
    int32  Expected = CFE_TBL_ERR_NEVER_LOADED;
    char   expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MANAGE_INDEX, Expected, 2);

    /* Execute the function being tested */
    SCH_EvsInit();
    Result = SCH_TblInit();

    sprintf(expEventText, "Error Acquiring Tbl Ptrs (RC=0x%08X)",
                          (unsigned int)Expected);

    /* Verify results */
    UtAssert_True(Result == Expected, "TblInit_Test_MdtManageError");

    UtAssert_EventSent(SCH_ACQ_PTR_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "TblInit_Test_MdtManageError Event Sent");
}


/**
 * SCH_TblInit_Test_DeadlineTblManageError
 */
void SCH_TblInit_Test_DeadlineTblManageError(void)
{
    int32  Result = CFE_SUCCESS;
    int32  Expected = CFE_TBL_ERR_NEVER_LOADED;
    char   expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_MANAGE_INDEX, Expected, 3);

    /* Execute the function being tested */
    SCH_EvsInit();
    Result = SCH_TblInit();

    sprintf(expEventText, "Error Acquiring Tbl Ptrs (RC=0x%08X)",
                          (unsigned int)Expected);

    /* Verify results */
    UtAssert_True(Result == Expected,
                  "SCH_TblInit_Test_DeadlineTblManageError");

    UtAssert_EventSent(SCH_ACQ_PTR_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "TblInit_Test_DeadlineTblManageError Event Sent");
}


/**
 * SCH_TblInit_Test_SdtGetAddressError
 */
void SCH_TblInit_Test_SdtGetAddressError(void)
{
    int32  Result = CFE_SUCCESS;
    int32  Expected = CFE_TBL_ERR_NEVER_LOADED;
    char   expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, Expected, 1);

    /* Execute the function being tested */
    SCH_EvsInit();
    Result = SCH_TblInit();

    sprintf(expEventText, "Error Acquiring Tbl Ptrs (RC=0x%08X)",
                          (unsigned int)Expected);

    /* Verify results */
    UtAssert_True(Result == Expected,
                  "TblInit_Test_SdtGetAddressError");

    UtAssert_EventSent(SCH_ACQ_PTR_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "TblInit_Test_SdtGetAddressError Event Sent");
}


/**
 * SCH_TblInit_Test_MdtGetAddressError
 */
void SCH_TblInit_Test_MdtGetAddressError(void)
{
    int32  Result = CFE_SUCCESS;
    int32  Expected = CFE_TBL_ERR_NEVER_LOADED;
    char   expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, Expected, 2);

    /* Execute the function being tested */
    SCH_EvsInit();
    Result = SCH_TblInit();

    sprintf(expEventText, "Error Acquiring Tbl Ptrs (RC=0x%08X)",
                          (unsigned int)Expected);

    /* Verify results */
    UtAssert_True(Result == Expected,
                  "TblInit_Test_MdtGetAddressError");

    UtAssert_EventSent(SCH_ACQ_PTR_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "TblInit_Test_MdtGetAddressError Event Sent");
}


/**
 * SCH_TblInit_Test_DeadlineTblGetAddressError
 */
void SCH_TblInit_Test_DeadlineTblGetAddressError(void)
{
    int32  Result = CFE_SUCCESS;
    int32  Expected = CFE_TBL_ERR_NEVER_LOADED;
    char   expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, Expected, 3);

    /* Execute the function being tested */
    SCH_EvsInit();
    Result = SCH_TblInit();

    sprintf(expEventText, "Error Acquiring Tbl Ptrs (RC=0x%08X)",
                          (unsigned int)Expected);

    /* Verify results */
    UtAssert_True(Result == Expected,
                  "TblInit_Test_DeadlineTblGetAddressError");

    UtAssert_EventSent(SCH_ACQ_PTR_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "TblInit_Test_DeadlineTblGetAddressError Event Sent");
}


/**
 * SCH_TblInit_Test_AcquirePointersError
 */
void SCH_TblInit_Test_AcquirePointersError(void)
{
    int32  Result = CFE_SUCCESS;
    int32  Expected = CFE_TBL_ERR_NEVER_LOADED;
    char   expEventText[CFE_EVS_MAX_MESSAGE_LENGTH];

    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, Expected, 1);

    /* Execute the function being tested */
    SCH_EvsInit();
    Result = SCH_TblInit();

    sprintf(expEventText, "Error Acquiring Tbl Ptrs (RC=0x%08X)",
                          (unsigned int)Expected);

    /* Verify results */
    UtAssert_True(Result == Expected, "TblInit_Test_AcquirePointersError");

    UtAssert_EventSent(SCH_ACQ_PTR_ERR_EID, CFE_EVS_ERROR, expEventText,
                       "TblInit_Test_AcquirePointersError Event Sent");
}


/**
 * SCH_TblInit_Test_Nominal
 */
void SCH_TblInit_Test_Nominal(void)
{
    int32   Result = CFE_SUCCESS;
    int32   Expected = CFE_SUCCESS;

    /* Execute the function being tested */
    SCH_EvsInit();
    Result = SCH_TblInit();

    /* Verify results */
    UtAssert_True(SCH_AppData.BadTableDataCount       == 0,
                  "SCH_AppData.BadTableDataCount       == 0");
    UtAssert_True(SCH_AppData.TableVerifySuccessCount == 2,
                  "SCH_AppData.TableVerifySuccessCount == 2");
    UtAssert_True(SCH_AppData.TableVerifyFailureCount == 0,
                  "SCH_AppData.TableVerifyFailureCount == 0");
    UtAssert_True(SCH_AppData.TablePassCount          == 0,
                  "SCH_AppData.TablePassCount          == 0");

    UtAssert_True(Result == Expected, "TblInit_Test_Nominal");
}



/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void SCH_Config_Tbl_Test_AddTestCases(void)
{
    UtTest_Add(SCH_TblInit_Test_RegisterSdtError,
               SCH_Test_Setup, SCH_Test_TearDown,
               "SCH_TblInit_Test_RegisterSdtError");
    UtTest_Add(SCH_TblInit_Test_RegisterMdtError,
               SCH_Test_Setup, SCH_Test_TearDown,
               "SCH_TblInit_Test_RegisterMdtError");
    UtTest_Add(SCH_TblInit_Test_RegisterDeadlineTblError,
               SCH_Test_Setup, SCH_Test_TearDown,
               "SCH_TblInit_Test_RegisterDeadlineTblError");
    UtTest_Add(SCH_TblInit_Test_LoadSdtError,
               SCH_Test_Setup, SCH_Test_TearDown,
               "SCH_TblInit_Test_LoadSdtError");
    UtTest_Add(SCH_TblInit_Test_LoadMdtError,
               SCH_Test_Setup, SCH_Test_TearDown,
               "SCH_TblInit_Test_LoadMdtError");
    UtTest_Add(SCH_TblInit_Test_SdtManageError,
               SCH_Test_Setup, SCH_Test_TearDown,
               "SCH_TblInit_Test_SdtManageError");
    UtTest_Add(SCH_TblInit_Test_MdtManageError,
               SCH_Test_Setup, SCH_Test_TearDown,
               "SCH_TblInit_Test_MdtManageError");
    UtTest_Add(SCH_TblInit_Test_DeadlineTblManageError,
               SCH_Test_Setup, SCH_Test_TearDown,
               "SCH_TblInit_Test_DeadlineTblManageError");
    UtTest_Add(SCH_TblInit_Test_SdtGetAddressError,
               SCH_Test_Setup, SCH_Test_TearDown,
               "SCH_TblInit_Test_SdtGetAddressError");
    UtTest_Add(SCH_TblInit_Test_MdtGetAddressError,
               SCH_Test_Setup, SCH_Test_TearDown,
               "SCH_TblInit_Test_MdtGetAddressError");
    UtTest_Add(SCH_TblInit_Test_DeadlineTblGetAddressError,
               SCH_Test_Setup, SCH_Test_TearDown,
               "SCH_TblInit_Test_DeadlineTblGetAddressError");
    UtTest_Add(SCH_TblInit_Test_AcquirePointersError,
               SCH_Test_Setup, SCH_Test_TearDown,
               "SCH_TblInit_Test_AcquirePointersError");
    UtTest_Add(SCH_TblInit_Test_Nominal,
               SCH_Test_Setup, SCH_Test_TearDown,
               "SCH_TblInit_Test_Nominal");
}
