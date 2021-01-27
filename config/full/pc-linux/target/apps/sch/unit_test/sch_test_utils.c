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
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_time_stubs.h"

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

/* ut_ostimer_stubs */

Ut_OSTIMER_HookTable_t          Ut_OSTIMER_HookTable;
Ut_OSTIMER_ReturnCodeTable_t    Ut_OSTIMER_ReturnCodeTable[UT_OSTIMER_MAX_INDEX];

void Ut_OSTIMER_Reset(void)
{
    memset(&Ut_OSTIMER_HookTable, 0, sizeof(Ut_OSTIMER_HookTable));
    memset(&Ut_OSTIMER_ReturnCodeTable, 0, sizeof(Ut_OSTIMER_ReturnCodeTable));
}

void Ut_OSTIMER_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if          (Index == UT_OSTIMER_CREATE_INDEX)    { Ut_OSTIMER_HookTable.OS_TimerCreate = FunPtr; }
    else if     (Index == UT_OSTIMER_SET_INDEX)       { Ut_OSTIMER_HookTable.OS_TimerSet = FunPtr; }
    else
    {
        printf("Unsupported OSTIMER Index In SetFunctionHook Call %lu", Index);
        UtAssert_True(FALSE, "Unsupported OSTIMER Index In SetFunctionHook Call");
    }
}

void Ut_OSTIMER_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < UT_OSTIMER_MAX_INDEX)
    {
        Ut_OSTIMER_ReturnCodeTable[Index].Value = RtnVal;
        Ut_OSTIMER_ReturnCodeTable[Index].Count = CallCnt;
    }
    else
    {
        printf("Unsupported OSTIMER Index In SetReturnCode Call %lu\n", Index);
        UtAssert_True(FALSE, "Unsupported OSTIMER Index In SetReturnCode Call");
    }
}

boolean Ut_OSTIMER_UseReturnCode(uint32 Index)
{
    if (Ut_OSTIMER_ReturnCodeTable[Index].Count > 0)
    {
        Ut_OSTIMER_ReturnCodeTable[Index].Count--;
        if (Ut_OSTIMER_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }
    else if (Ut_OSTIMER_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero == TRUE)
    {
        return(TRUE);
    }
    
    return(FALSE);
}

void Ut_OSTIMER_ContinueReturnCodeAfterCountZero(uint32 Index)
{
    Ut_OSTIMER_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero = TRUE;
}

int32 OS_TimerCreate(uint32 *timer_id, const char *timer_name, uint32 *clock_accuracy, OS_TimerCallback_t  callback_ptr)
{
    /* Check for specified return */
    if (Ut_OSTIMER_UseReturnCode(UT_OSTIMER_CREATE_INDEX))
        return Ut_OSTIMER_ReturnCodeTable[UT_OSTIMER_CREATE_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_OSTIMER_HookTable.OS_TimerCreate)
        return(Ut_OSTIMER_HookTable.OS_TimerCreate(timer_id, timer_name, clock_accuracy, callback_ptr));

    return CFE_SUCCESS;
}

int32 OS_TimerSet(uint32 timer_id, uint32 start_time, uint32 interval_time)
{
    /* Check for specified return */
    if (Ut_OSTIMER_UseReturnCode(UT_OSTIMER_SET_INDEX))
        return Ut_OSTIMER_ReturnCodeTable[UT_OSTIMER_SET_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_OSTIMER_HookTable.OS_TimerSet)
        return(Ut_OSTIMER_HookTable.OS_TimerSet(timer_id, start_time, interval_time));

    return CFE_SUCCESS;
}

/* end ut_ostimer_stubs */



void OS_RtmEndFrame(void)
{
    //if(OS_RUNTIME_MODE == 1)
    //{
        //OS_RtmData.State = OS_RTM_STATE_NONREALTIME_HOLD;
        //pthread_cond_broadcast(&OS_RtmData.CV);
    //}
    //else
    //{
        //OS_RtmData.CurrentFrameCount++;
        //OS_RtmData.CurrentMinorFrame = OS_RtmData.CurrentFrameCount % OS_RtmData.MinorFramesPerMajorFrame;
    //}
}

OS_RunTimeModeEnum_t OS_RtmGetRunMode(void)
{
    return OS_RUNTIME_MODE_REALTIME;
}

void OS_RtmSetMajorAndMinorFrame(uint32 MajorFrameLength, uint32 MinorFramesPerMajorFrame)
{

}

uint32 OS_RtmGetCurrentMinorFrame(void)
{
    return 0;
    //MINOR_FRAME;
}


int32 OS_RtmEngageStepMode(void)
{
    return 0;
}


/************************/
/*  End of File Comment */
/************************/
