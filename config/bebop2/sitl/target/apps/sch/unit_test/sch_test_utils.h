 /*************************************************************************
 ** File:
 **   $Id: sch_test_utils.h 1.2 2017/06/21 15:28:58EDT mdeschu Exp  $
 **
 ** Purpose: 
 **   This file contains the function prototypes and global variables for the unit test utilities for the SCH application.
 **
 ** References:
 **   Flight Software Branch C Coding Standard Version 1.2
 **   CFS Development Standards Document
 ** Notes:
 **
 *************************************************************************/

#ifndef SCH_TEST_UTILS_H
#define SCH_TEST_UTILS_H

/*
 * Includes
 */

#include "sch_app.h"
#include "ut_cfe_evs_hooks.h"
// FIXME: these need to be implemented/updated in UT-Assert (by copying from sch_test_utils.c/.h):
//#include "ut_cfe_evs_stubs.h"
//#include "ut_cfe_time_stubs.h"
//#include "ut_ostimer_stubs.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_sb_hooks.h"
#include "ut_cfe_time_hooks.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_tbl_hooks.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_osapi_stubs.h"
#include "ut_osfileapi_stubs.h"
#include "ut_cfe_es_stubs.h"
#include <time.h>

/*
 * Function Definitions
 */

void SCH_Test_Setup(void);
void SCH_Test_TearDown(void);

/*
 * Additional UT-Assert Stub Functions and Required Data Structures
 *
 * Note: This code needs to be moved into the UT-Assert library.  We are including it here for now because the 
 * next release of the UT-Assert library is not expected to happen in the near future.
 */

/* ut_ostimer_stubs */

typedef enum 
{
    UT_OSTIMER_CREATE_INDEX,
    UT_OSTIMER_SET_INDEX,
    UT_OSTIMER_MAX_INDEX
} Ut_OSTIMER_INDEX_t;

typedef struct
{
    int32 (*OS_TimerCreate)(uint32 *timer_id, const char *timer_name, uint32 *clock_accuracy, OS_TimerCallback_t  callback_ptr);
    int32 (*OS_TimerSet)(uint32 timer_id, uint32 start_time, uint32 interval_time);
} Ut_OSTIMER_HookTable_t;

typedef struct
{
    int32   Value;
    uint32  Count;
    boolean ContinueReturnCodeAfterCountZero;
} Ut_OSTIMER_ReturnCodeTable_t;

void Ut_OSTIMER_Reset(void);
void Ut_OSTIMER_SetFunctionHook(uint32 Index, void *FunPtr);
void Ut_OSTIMER_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);
void Ut_OSTIMER_ContinueReturnCodeAfterCountZero(uint32 Index);

/* end ut_ostimer_stubs */

#endif

/************************/
/*  End of File Comment */
/************************/
