 /*************************************************************************
 ** File:
 **   $Id: sch_test_utils.c 1.2 2017/06/21 15:28:59EDT mdeschu Exp  $
 **
 ** Purpose: 
 **   This file contains unit test utilities for the sch application.
 **
 ** References:
 **   Flight Software Branch C Coding Standard Version 1.2
 **   CFS Development Standards Document
 ** Notes:
 **
 *************************************************************************/

#include "sch_test_utils.h"
#include "sch_tbldefs.h"

SCH_MessageEntry_t    MessageTable[SCH_MAX_MESSAGES];
SCH_ScheduleEntry_t   ScheduleTable[SCH_TABLE_ENTRIES];

/*
 * Function Definitions
 */

void SCH_Test_Setup(void)
{
    /* initialize test environment to default state for every test */

    CFE_PSP_MemSet(&SCH_AppData, 0, sizeof(SCH_AppData_t));

    SCH_AppData.MessageTable  = &MessageTable[0];
    SCH_AppData.ScheduleTable = &ScheduleTable[0];

    CFE_PSP_MemSet(SCH_AppData.MessageTable, 0, sizeof(SCH_MessageEntry_t)*SCH_MAX_MESSAGES);
    CFE_PSP_MemSet(SCH_AppData.ScheduleTable, 0, sizeof(SCH_ScheduleEntry_t)*SCH_TABLE_ENTRIES);

    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_TIME_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();
    Ut_OSTIMER_Reset();
} /* end SCH_Test_Setup */

void SCH_Test_TearDown(void)
{
    /* cleanup test environment */
} /* end SCH_Test_TearDown */

Ut_CFE_EVS_HookTable_t          Ut_CFE_EVS_HookTable;
Ut_CFE_EVS_ReturnCodeTable_t    Ut_CFE_EVS_ReturnCodeTable[UT_CFE_EVS_MAX_INDEX];

/************************/
/*  End of File Comment */
/************************/
