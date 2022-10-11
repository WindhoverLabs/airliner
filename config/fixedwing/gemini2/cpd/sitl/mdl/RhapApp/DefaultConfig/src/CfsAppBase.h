/*********************************************************************
	Rhapsody	: 9.0.1 
	Login		: mbenson
	Component	: RhapAppComponent 
	Configuration 	: DefaultConfig
	Model Element	: CfsAppBase
//!	Generated Date	: Tue, 11, Oct 2022  
	File Path	: RhapApp/DefaultConfig/src/CfsAppBase.h
*********************************************************************/

#ifndef CfsAppBase_H
#define CfsAppBase_H

//## auto_generated
#include <oxf/oxf.h>
//## auto_generated
#include "SampleApp.h"
//## auto_generated
#include "cfe.h"
//## package core::SampleApp

//## class CfsAppBase
class CfsAppBase {
public :

    //## type EventID
    enum EventID {
        RESERVED_EID = 0,
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
        MSGLEN_ERR_EID,
        BASE_EVENT_COUNT
    };
    
    //## type TableRegistration
    struct TableRegistration {
        CFE_TBL_Handle_t* HandlePtr;		//## attribute HandlePtr
        const char* Name;		//## attribute Name
        uint32 Size;		//## attribute Size
        int OptionFlags;		//## attribute OptionFlags
        CFE_TBL_CallbackFuncPtr_t ValidationFuncPtr;		//## attribute ValidationFuncPtr
        void ** TablePtr;		//## attribute TablePtr
        const char* FileName;		//## attribute FileName
    };
    
    ////    Constructors and destructors    ////
    
    //## auto_generated
    CfsAppBase();
    
    //## auto_generated
    virtual ~CfsAppBase();
    
    ////    Operations    ////
    
    //## operation AppMain()
    void AppMain();
    
    //## operation Execute()
    virtual void Execute() = 0;
    
    //## operation Init0()
    int32* Init0();
    
    //## operation Init1()
    void Init1();
    
    //## operation getEventFilters(CFE_EVS_BinFilter_t,uint32)
    virtual void getEventFilters(CFE_EVS_BinFilter_t*& events, uint32& count);
    
    //## operation getTableRegistrations(TableRegistration,uint32)
    void getTableRegistrations(TableRegistration*& tables, uint32& count);
    
    //## operation startBehavior()
    virtual bool startBehavior();
    
    ////    Additional operations    ////
    
    //## auto_generated
    uint32 getRunStatus() const;
    
    //## auto_generated
    void setRunStatus(uint32 p_RunStatus);
    
    ////    Attributes    ////

protected :

    uint32 RunStatus;		//## attribute RunStatus
};

#endif
/*********************************************************************
	File Path	: RhapApp/DefaultConfig/src/CfsAppBase.h
*********************************************************************/
