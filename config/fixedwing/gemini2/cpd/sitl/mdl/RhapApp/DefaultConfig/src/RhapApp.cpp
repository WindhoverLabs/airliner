/********************************************************************
	Rhapsody	: 9.0.1 
	Login		: mbenson
	Component	: RhapAppComponent 
	Configuration 	: DefaultConfig
	Model Element	: RhapApp
//!	Generated Date	: Tue, 11, Oct 2022  
	File Path	: RhapApp/DefaultConfig/src/RhapApp.cpp
*********************************************************************/

//## auto_generated
#include <oxf/omthread.h>
//## auto_generated
#include "RhapApp.h"
//## package core::SampleApp

//## class RhapApp
RhapApp::RhapApp(IOxfActive* theActiveContext) {
    setActiveContext(theActiveContext, false);
    initStatechart();
}

RhapApp::~RhapApp() {
    cancelTimeouts();
}

void RhapApp::Execute() {
    //#[ operation Execute()
    printf("Execute()*************************************\n");
    printf("Execute()*************************************\n");
    printf("Execute()*************************************\n");
    printf("Execute()*************************************\n");
    printf("Execute()*************************************\n");
    printf("Execute()*************************************\n");
    printf("Execute()*************************************\n");
    printf("Execute()*************************************\n");
    printf("Execute()*************************************\n");
    printf("Execute()*************************************\n");
    printf("Execute()*************************************\n");
    printf("Execute()*************************************\n");
    printf("Execute()*************************************\n");
    printf("Execute()*************************************\n");
    printf("Execute()*************************************\n");
    printf("Execute()*************************************\n");
    printf("Execute()*************************************\n");
    printf("Execute()*************************************\n");
    printf("Execute()*************************************\n");
    printf("Execute()*************************************\n");
    printf("Execute()*************************************\n");
    printf("Execute()*************************************\n");
    printf("Execute()*************************************\n");
    printf("Execute()*************************************\n");
    //#]
}

void RhapApp::getEventFilters(CFE_EVS_BinFilter_t*& events, uint32& count) {
    //#[ operation getEventFilters(CFE_EVS_BinFilter_t,uint32)
    CFE_EVS_BinFilter_t myEvents[] = {
        RESERVED_EID,         CFE_EVS_NO_FILTER,
        INF_EID,              CFE_EVS_NO_FILTER,
        CONFIG_TABLE_ERR_EID, CFE_EVS_NO_FILTER,
        CDS_ERR_EID,          CFE_EVS_NO_FILTER,
        PIPE_ERR_EID,         CFE_EVS_NO_FILTER,
        MSGID_ERR_EID,        CFE_EVS_NO_FILTER,
        MSGLEN_ERR_EID,       CFE_EVS_NO_FILTER
    };
    
    events = myEvents;
    count = sizeof(myEvents)/sizeof(CFE_EVS_BinFilter_t);
    //#]
}

bool RhapApp::startBehavior() {
    bool done = false;
    done = OMReactive::startBehavior();
    return done;
}

void RhapApp::initStatechart() {
    rootState_subState = OMNonState;
    rootState_active = OMNonState;
    rootState_timeout = NULL;
}

void RhapApp::cancelTimeouts() {
    cancel(rootState_timeout);
}

bool RhapApp::cancelTimeout(const IOxfTimeout* arg) {
    bool res = false;
    if(rootState_timeout == arg)
        {
            rootState_timeout = NULL;
            res = true;
        }
    return res;
}

void RhapApp::rootState_entDef() {
    {
        rootState_subState = RED;
        rootState_active = RED;
        //#[ state RED.(Entry) 
        printf("RED\n");
        //#]
        rootState_timeout = scheduleTimeout(5000, NULL);
    }
}

IOxfReactive::TakeEventStatus RhapApp::rootState_processEvent() {
    IOxfReactive::TakeEventStatus res = eventNotConsumed;
    switch (rootState_active) {
        // State RED
        case RED:
        {
            if(IS_EVENT_TYPE_OF(OMTimeoutEventId))
                {
                    if(getCurrentEvent() == rootState_timeout)
                        {
                            cancel(rootState_timeout);
                            rootState_subState = GREEN;
                            rootState_active = GREEN;
                            //#[ state GREEN.(Entry) 
                            printf("GREEN\n");
                            //#]
                            rootState_timeout = scheduleTimeout(5000, NULL);
                            res = eventConsumed;
                        }
                }
            
        }
        break;
        // State GREEN
        case GREEN:
        {
            if(IS_EVENT_TYPE_OF(OMTimeoutEventId))
                {
                    if(getCurrentEvent() == rootState_timeout)
                        {
                            cancel(rootState_timeout);
                            rootState_subState = YELLOW;
                            rootState_active = YELLOW;
                            //#[ state YELLOW.(Entry) 
                            printf("YELLOW\n");
                            //#]
                            rootState_timeout = scheduleTimeout(1000, NULL);
                            res = eventConsumed;
                        }
                }
            
        }
        break;
        // State YELLOW
        case YELLOW:
        {
            if(IS_EVENT_TYPE_OF(OMTimeoutEventId))
                {
                    if(getCurrentEvent() == rootState_timeout)
                        {
                            cancel(rootState_timeout);
                            rootState_subState = RED;
                            rootState_active = RED;
                            //#[ state RED.(Entry) 
                            printf("RED\n");
                            //#]
                            rootState_timeout = scheduleTimeout(5000, NULL);
                            res = eventConsumed;
                        }
                }
            
        }
        break;
        default:
            break;
    }
    return res;
}

/*********************************************************************
	File Path	: RhapApp/DefaultConfig/src/RhapApp.cpp
*********************************************************************/
