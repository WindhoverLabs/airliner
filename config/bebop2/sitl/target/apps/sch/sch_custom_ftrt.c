/*************************************************************************
**
** Include section
**
**************************************************************************/

#include "cfe.h"
#include "sch_platform_cfg.h"

#include "cfe_time_msg.h"
#include "cfe_psp.h"
#include "sch_app.h"
#include "osapi.h"


/*************************************************************************
**
** Macro definitions
**
**************************************************************************/

/*
** Timer Characteristics
*/
#define SCH_TIMER_NAME   "SCH_MINOR_TIMER"

/*************************************************************************
** Local function prototypes
**************************************************************************/

/************************************************************************/
/** \brief Computes a minor slot number from a MET subseconds zero point
**  
**  \par Description
**       This function determines the current slot (minor frame) number if
**       one were to assume that slot zero started when the MET microseconds
**       are equal to zero. 
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \returns
**  \retstmt Returns slot index from zero to (#SCH_TOTAL_SLOTS-1) \endcode
**  \endreturns
**
*************************************************************************/
uint32 SCH_GetMETSlotNumber(void);

/************************************************************************/
/** \brief Performs Major Frame Synchronization
**  
**  \par Description
**       This function is called by cFE TIME services when a Major Frame
**       synchronization signal is received.  It then synchronizes the
**       minor frame (slot) processing of the Schedule Definition Table. 
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
*************************************************************************/
void  SCH_MajorFrameCallback(void);

/************************************************************************/
/** \brief Performs Minor Frame time step
**  
**  \par Description
**       This function is called by an OSAL timer when the minor frame
**       timing reference sends a signal.  The Scheduler Application uses
**       this to drive the Application's processing of each minor frame. 
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
*************************************************************************/
void  SCH_MinorFrameCallback(uint32 TimerId);



/*************************************************************************
**
** Function definitions
**
**************************************************************************/

/*******************************************************************
**
** SCH_CustomEarlyInit
**
** NOTE: For complete prolog information, see 'sch_custom.h'
**
** This function MUST update SCH_AppData.ClockAccuracy to the
** resolution of the minor frame timer.
********************************************************************/

int32 SCH_CustomEarlyInit(void)
{
    int32             Status = CFE_SUCCESS;
    
    Status = OS_TimerCreate(&SCH_AppData.TimerId,
                                 SCH_TIMER_NAME,
                                 &SCH_AppData.ClockAccuracy,
                                 SCH_MinorFrameCallback);

    return Status;

} /* End of CustomEarlyInit() */


/*******************************************************************
**
** SCH_CustomLateInit
**
** NOTE: For complete prolog information, see 'sch_custom.h'
**
** This function MUST perform any startup synchronization required,
** and MUST finish setting up the major and minor frame timers. 
********************************************************************/

int32 SCH_CustomLateInit(void)
{
    int32  Status    = CFE_SUCCESS;

    CFE_ES_WaitForStartupSync(SCH_STARTUP_SYNC_TIMEOUT);

    OS_RtmSetMajorAndMinorFrame(SCH_MICROS_PER_MAJOR_FRAME, SCH_TOTAL_SLOTS);
    OS_RtmEngageStepMode();

    Status = OS_TimerSet(SCH_AppData.TimerId, SCH_NORMAL_SLOT_PERIOD, SCH_NORMAL_SLOT_PERIOD);

    /* Step one minor frame to get the machine going. */
    OS_BinSemGive(SCH_AppData.TimeSemaphore);

    return Status;
} /* End of SCH_CustomLateInit() */


/*******************************************************************
**
** SCH_CustomGetCurrentSlotNumber
**
** NOTE: For complete prolog information, see 'sch_custom.h'
********************************************************************/

uint32 SCH_CustomGetCurrentSlotNumber(void)
{
	uint32 minorFrame = OS_RtmGetCurrentMinorFrame();

	return minorFrame;
} /* End of SH_CustomGetCurrentSlotNumber() */

/*******************************************************************
**
** SCH_CustomCleanup
**
** NOTE: For complete prolog information, see 'sch_custom.h'
********************************************************************/

void SCH_CustomCleanup(void)
{
//    /* unregister the TIME callback for the major frame */
//    CFE_TIME_UnregisterSynchCallback((CFE_TIME_SynchCallbackPtr_t)&SCH_MajorFrameCallback);

} /* End of SH_CustomCleanup() */


void SCH_MinorFrameCallback(uint32 TimerId)
{
	OS_BinSemGive(SCH_AppData.TimeSemaphore);
} /* End of SCH_MinorFrameCallback() */


/************************/
/*  End of File Comment */
/************************/

