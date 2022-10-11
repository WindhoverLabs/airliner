/********************************************************************
	Rhapsody	: 9.0.1 
	Login		: mbenson
	Component	: RhapAppComponent 
	Configuration 	: DefaultConfig
	Model Element	: DefaultConfig
//!	Generated Date	: Tue, 11, Oct 2022  
	File Path	: RhapApp/DefaultConfig/src/MainRhapAppComponent.cpp
*********************************************************************/

//## auto_generated
#include "MainRhapAppComponent.h"
int vxmain(int argc, char* argv[]) {
    int status = 0;
    if(OXF::initialize())
        {
            //#[ configuration RhapAppComponent::DefaultConfig 
            //#]
            OXF::start();
            status = 0;
        }
    else
        {
            status = 1;
        }
    return status;
}

/*********************************************************************
	File Path	: RhapApp/DefaultConfig/src/MainRhapAppComponent.cpp
*********************************************************************/
