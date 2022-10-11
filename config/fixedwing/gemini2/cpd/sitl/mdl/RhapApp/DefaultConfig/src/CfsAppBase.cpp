/********************************************************************
	Rhapsody	: 9.0.1 
	Login		: mbenson
	Component	: RhapAppComponent 
	Configuration 	: DefaultConfig
	Model Element	: CfsAppBase
//!	Generated Date	: Tue, 11, Oct 2022  
	File Path	: RhapApp/DefaultConfig/src/CfsAppBase.cpp
*********************************************************************/

//## auto_generated
#include "CfsAppBase.h"
//## package core::SampleApp

//## class CfsAppBase
CfsAppBase::CfsAppBase() {
}

CfsAppBase::~CfsAppBase() {
}

void CfsAppBase::AppMain() {
    //#[ operation AppMain()
    RunStatus = CFE_ES_APP_RUN;
    CFE_EVS_BinFilter_t *myEvents = 0;
    TableRegistration *myTables = 0;
    uint32 myEventCount = 0;
    uint32 myTableCount = 0;
    uint32 combinedEventCount = 0;
    int32 status;
    CFE_EVS_BinFilter_t baseEvents[] = {
        RESERVED_EID,         CFE_EVS_NO_FILTER,
        INF_EID,              CFE_EVS_NO_FILTER,
        CONFIG_TABLE_ERR_EID, CFE_EVS_NO_FILTER,
        CDS_ERR_EID,          CFE_EVS_NO_FILTER,
        PIPE_ERR_EID,         CFE_EVS_NO_FILTER,
        MSGID_ERR_EID,        CFE_EVS_NO_FILTER,
        MSGLEN_ERR_EID,       CFE_EVS_NO_FILTER
    };
    CFE_EVS_BinFilter_t combinedEvents[CFE_EVS_MAX_EVENT_FILTERS];
    uint32 baseEventCount = sizeof(baseEvents)/sizeof(CFE_EVS_BinFilter_t);
    
    /* Register the application with Executive Services */
    (void) CFE_ES_RegisterApp();
    if(status != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("Failed to register the app (0x%08X)\n", (unsigned int)status);
    }
    
    /* Add the base events to the combined event list */
    for(uint32 i = 1; i < baseEventCount; ++i)
    {
        combinedEvents[i].EventID = baseEvents[i].EventID;
        combinedEvents[i].Mask = baseEvents[i].Mask;
        combinedEventCount++;
    }
    
    /* Get the events from the super class. */
    getEventFilters(myEvents, myEventCount);
    if((0 == myEvents) || (0 == myEventCount))
    {
        /* TODO */
    }
    else
    {
    	/* Add the events from the super class to the combined event list */
    	for(uint32 i = baseEventCount; i < myEventCount; ++i)
    	{
    	    combinedEvents[i].EventID = myEvents[i].EventID;
    	    combinedEvents[i].Mask = myEvents[i].Mask;
            combinedEventCount++;
    	}
    }
    
    /* Now register the combined events. */
    status = CFE_EVS_Register(combinedEvents, combinedEventCount, CFE_EVS_BINARY_FILTER);
    if (status != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("Failed to register with EVS (0x%08X)\n", (unsigned int)status);
    }
    
    /* Get the tables from the super class. */
    getTableRegistrations(myTables, myTableCount);
    if((0 == myTables) || (0 == myTableCount))
    {
        /* TODO */
    }
    else
    {
    
    }
    
    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(0);
    
    startBehavior();
    
    while(1)
    {
        OS_TaskDelay(10000);
    }
    
    CFE_ES_ExitApp(0);
    //#]
}

int32* CfsAppBase::Init0() {
    //#[ operation Init0()
    //#]
}

void CfsAppBase::Init1() {
    //#[ operation Init1()
    //#]
}

void CfsAppBase::getEventFilters(CFE_EVS_BinFilter_t*& events, uint32& count) {
    //#[ operation getEventFilters(CFE_EVS_BinFilter_t,uint32)
    events = 0;
    count = 0;
    //#]
}

void CfsAppBase::getTableRegistrations(CfsAppBase::TableRegistration*& tables, uint32& count) {
    //#[ operation getTableRegistrations(TableRegistration,uint32)
    tables = 0;
    count = 0;
    //#]
}

bool CfsAppBase::startBehavior() {
    //#[ operation startBehavior()
    return FALSE;
    //#]
}

uint32 CfsAppBase::getRunStatus() const {
    return RunStatus;
}

void CfsAppBase::setRunStatus(uint32 p_RunStatus) {
    RunStatus = p_RunStatus;
}

/*********************************************************************
	File Path	: RhapApp/DefaultConfig/src/CfsAppBase.cpp
*********************************************************************/
