/*************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "cfe_psp.h"
#include "osapi.h"
#include "hs_app.h"
#include "hs_cmds.h"
#include "hs_msg.h"
#include "hs_custom.h"
#include "hs_events.h"
#include "hs_monitors.h"
#include "hs_perfids.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Idle Task Main Process Loop                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void HS_IdleTask(void)
{
    OS_time_t PSPTime = {0,0};

    HS_CustomData.IdleTaskRunStatus = CFE_ES_RegisterChildTask();

    while (HS_CustomData.IdleTaskRunStatus == CFE_SUCCESS)
    {
    	OS_TaskDelay(1000);
    }

    /*
    ** If the run status is externally set to something else
    */
    return;

} /* End of HS_IdleTask() */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize The Idle Task                                        */
/*                                                                 */
/* NOTE: For complete prolog information, see 'hs_custom.h'        */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 HS_CustomInit(void)
{
    int32 Status = CFE_SUCCESS;

    /*
    ** Spawn the Idle Task
    */
    Status = CFE_ES_CreateChildTask(&HS_CustomData.IdleTaskID,      
                                     HS_IDLE_TASK_NAME,       
                                     HS_IdleTask,         
                                     HS_IDLE_TASK_STACK_PTR,  
                                     HS_IDLE_TASK_STACK_SIZE, 
                                     HS_IDLE_TASK_PRIORITY,   
                                     HS_IDLE_TASK_FLAGS);     
     
    if(Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(HS_CR_CHILD_TASK_ERR_EID, CFE_EVS_ERROR,
                          "Error Creating Child Task for CPU Utilization Monitoring,RC=0x%08X",
                          (unsigned int)Status);
        return (Status);
    }

    return(Status);

} /* end HS_CustomInit */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Terminate The Idle Task                                         */
/*                                                                 */
/* NOTE: For complete prolog information, see 'hs_custom.h'        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HS_CustomCleanup(void)
{

} /* end HS_CustomCleanup */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Callback function that marks the Idle time                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HS_MarkIdleCallback(void)
{
    /*
    ** Capture the CPU Utilization (at a consistant time)
    */

    return;

} /* End of HS_MarkIdleCallback() */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Performs Utilization Monitoring and reporting                   */
/*                                                                 */
/* NOTE: For complete prolog information, see 'hs_custom.h'        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HS_CustomMonitorUtilization(void)
{
    HS_MonitorUtilization();
} /* End of HS_CustomMonitorUtilization() */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Processes any additional commands                               */
/*                                                                 */
/* NOTE: For complete prolog information, see 'hs_custom.h'        */
/*                                                                 */
/* This function MUST return CFE_SUCCESS if any command is found   */
/* and must NOT return CFE_SUCCESS if no command was found         */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 HS_CustomCommands(CFE_SB_MsgPtr_t MessagePtr)
{
    int32 Status = CFE_SUCCESS;

    uint16          CommandCode = 0;

    CommandCode = CFE_SB_GetCmdCode(MessagePtr);
    switch (CommandCode)
    {
        case HS_REPORT_DIAG_CC:
            HS_UtilDiagReport();
            break;

        case HS_SET_UTIL_PARAMS_CC:
            HS_SetUtilParamsCmd(MessagePtr);
            break;

        case HS_SET_UTIL_DIAG_CC:
            HS_SetUtilDiagCmd(MessagePtr);
            break;

        default:
            Status = !CFE_SUCCESS;
            break;

    } /* end CommandCode switch */

    return Status;

} /* End of HS_CustomCommands() */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Report Utilization Diagnostics Information                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void HS_UtilDiagReport(void)
{

} /* end HS_UtilDiagReport */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Report Utilization                                              */
/*                                                                 */
/* NOTE: For complete prolog information, see 'hs_custom.h'        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 HS_CustomGetUtil(void)
{
   int32 currentUtil = 0;

   currentUtil = (int32) uiGetCpuUsage();

   return currentUtil;
}



void HS_SetUtilParamsCmd(CFE_SB_MsgPtr_t MessagePtr)
{
    uint16            ExpectedLength = sizeof(HS_SetUtilParamsCmd_t);
    HS_SetUtilParamsCmd_t  *CmdPtr;

    /*
    ** Verify message packet length
    */
    if(HS_VerifyMsgLength(MessagePtr, ExpectedLength))
    {
        CmdPtr = ((HS_SetUtilParamsCmd_t *)MessagePtr);

        if((CmdPtr->Mult1 != 0) &&
           (CmdPtr->Mult2 != 0) &&
           (CmdPtr->Div != 0))
        {
            HS_CustomData.UtilMult1 = CmdPtr->Mult1;
            HS_CustomData.UtilMult2 = CmdPtr->Mult2;
            HS_CustomData.UtilDiv   = CmdPtr->Div;
            HS_AppData.CmdCount++;
            CFE_EVS_SendEvent (HS_SET_UTIL_PARAMS_DBG_EID, CFE_EVS_DEBUG,
                               "Utilization Parms set: Mult1: %d Div: %d Mult2: %d", 
                               (int)HS_CustomData.UtilMult1, (int)HS_CustomData.UtilDiv, (int)HS_CustomData.UtilMult2);
        }
        else
        {
            HS_AppData.CmdErrCount++;
            CFE_EVS_SendEvent (HS_SET_UTIL_PARAMS_ERR_EID, CFE_EVS_ERROR,
                               "Utilization Parms Error: No parameter may be 0: Mult1: %d Div: %d Mult2: %d", 
                               (int)CmdPtr->Mult1, (int)CmdPtr->Div, (int)CmdPtr->Mult2);
        }
    }

    return;

} /* end HS_SetUtilParamsCmd */



void HS_SetUtilDiagCmd(CFE_SB_MsgPtr_t MessagePtr)
{
    uint16            ExpectedLength = sizeof(HS_SetUtilDiagCmd_t);
    HS_SetUtilDiagCmd_t  *CmdPtr;

    /*
    ** Verify message packet length
    */
    if(HS_VerifyMsgLength(MessagePtr, ExpectedLength))
    {
        HS_AppData.CmdCount++;
        CmdPtr = ((HS_SetUtilDiagCmd_t *)MessagePtr);

        HS_CustomData.UtilMask  = CmdPtr->Mask;
        CFE_EVS_SendEvent (HS_SET_UTIL_DIAG_DBG_EID, CFE_EVS_DEBUG,
                           "Utilization Diagnostics Mask has been set to %08X", 
                           (unsigned int)HS_CustomData.UtilMask);
    }

    return;

} /* end HS_SetUtilDiagCmd */



/************************/
/*  End of File Comment */
/************************/
