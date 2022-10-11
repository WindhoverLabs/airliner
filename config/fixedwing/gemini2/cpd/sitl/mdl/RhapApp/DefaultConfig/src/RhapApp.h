/*********************************************************************
	Rhapsody	: 9.0.1 
	Login		: mbenson
	Component	: RhapAppComponent 
	Configuration 	: DefaultConfig
	Model Element	: RhapApp
//!	Generated Date	: Tue, 11, Oct 2022  
	File Path	: RhapApp/DefaultConfig/src/RhapApp.h
*********************************************************************/

#ifndef RhapApp_H
#define RhapApp_H

//## auto_generated
#include <oxf/oxf.h>
//## auto_generated
#include "SampleApp.h"
//## auto_generated
#include <oxf/omreactive.h>
//## auto_generated
#include <oxf/state.h>
//## auto_generated
#include <oxf/event.h>
//## class RhapApp
#include "CfsAppBase.h"
//## package core::SampleApp

//## class RhapApp
class RhapApp : public OMReactive, public CfsAppBase {
public :

    //## type EventID
    enum EventID {
        RESERVED_EID = BASE_EVENT_COUNT,
        INF_EID,
        INIT_INF_EID,
        CONFIG_TABLE_INF_EID,
        CDS_INF_EID,
        ERR_EID,
        INIT_ERR_EID,
        CONFIG_TABLE_ERR_EID,
        CDS_ERR_EID,
        CMD_ERR_EID,
        PIPE_ERR_EID,
        MSGID_ERR_EID,
        MSGLEN_ERR_EID
    };
    
    ////    Constructors and destructors    ////
    
    //## auto_generated
    RhapApp(IOxfActive* theActiveContext = 0);
    
    //## auto_generated
    virtual ~RhapApp();
    
    ////    Operations    ////
    
    //## operation Execute()
    void Execute();
    
    //## operation getEventFilters(CFE_EVS_BinFilter_t,uint32)
    virtual void getEventFilters(CFE_EVS_BinFilter_t*& events, uint32& count);
    
    ////    Additional operations    ////
    
    //## auto_generated
    virtual bool startBehavior();

protected :

    //## auto_generated
    void initStatechart();
    
    //## auto_generated
    void cancelTimeouts();
    
    //## auto_generated
    bool cancelTimeout(const IOxfTimeout* arg);
    
    ////    Framework operations    ////

public :

    // rootState:
    //## statechart_method
    inline bool rootState_IN() const;
    
    //## statechart_method
    virtual void rootState_entDef();
    
    //## statechart_method
    virtual IOxfReactive::TakeEventStatus rootState_processEvent();
    
    // YELLOW:
    //## statechart_method
    inline bool YELLOW_IN() const;
    
    // RED:
    //## statechart_method
    inline bool RED_IN() const;
    
    // GREEN:
    //## statechart_method
    inline bool GREEN_IN() const;
    
    ////    Framework    ////

protected :

//#[ ignore
    enum RhapApp_Enum {
        OMNonState = 0,
        YELLOW = 1,
        RED = 2,
        GREEN = 3
    };
    
    int rootState_subState;
    
    int rootState_active;
    
    IOxfTimeout* rootState_timeout;
//#]
};

inline bool RhapApp::rootState_IN() const {
    return true;
}

inline bool RhapApp::YELLOW_IN() const {
    return rootState_subState == YELLOW;
}

inline bool RhapApp::RED_IN() const {
    return rootState_subState == RED;
}

inline bool RhapApp::GREEN_IN() const {
    return rootState_subState == GREEN;
}

#endif
/*********************************************************************
	File Path	: RhapApp/DefaultConfig/src/RhapApp.h
*********************************************************************/
