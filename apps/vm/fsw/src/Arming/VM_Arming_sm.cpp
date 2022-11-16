#include "VM_Arming.h"
#include "VM_Arming_sm.h"


// Static class declarations.
VM_ArmingFSM_Init VM_ArmingFSM::Init("VM_ArmingFSM::Init", 0);
VM_ArmingFSM_Standby VM_ArmingFSM::Standby("VM_ArmingFSM::Standby", 1);
VM_ArmingFSM_Armed VM_ArmingFSM::Armed("VM_ArmingFSM::Armed", 2);
VM_ArmingFSM_StandbyError VM_ArmingFSM::StandbyError("VM_ArmingFSM::StandbyError", 3);
VM_ArmingFSM_ArmedError VM_ArmingFSM::ArmedError("VM_ArmingFSM::ArmedError", 4);

void VM_ArmingState::Arm(VM_ArmingContext& context)
{
    Default(context);
    return;
}

void VM_ArmingState::Disarm(VM_ArmingContext& context)
{
    Default(context);
    return;
}

void VM_ArmingState::Error(VM_ArmingContext& context)
{
    Default(context);
    return;
}

void VM_ArmingState::InitComplete(VM_ArmingContext& context)
{
    Default(context);
    return;
}

void VM_ArmingState::Reset(VM_ArmingContext& context)
{
    Default(context);
    return;
}

void VM_ArmingState::Default(VM_ArmingContext& context)
{
    throw (
        TransitionUndefinedException(
            context.getState().getName(),
            context.getTransition()));

    return;
}

void VM_ArmingFSM_Init::InitComplete(VM_ArmingContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_ArmingFSM::Standby);
    (context.getState()).Entry(context);

    return;
}

void VM_ArmingFSM_Init::Reset(VM_ArmingContext& context)
{


    return;
}

void VM_ArmingFSM_Standby::Arm(VM_ArmingContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_ArmingFSM::Armed);
    (context.getState()).Entry(context);

    return;
}

void VM_ArmingFSM_Standby::Error(VM_ArmingContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_ArmingFSM::StandbyError);
    (context.getState()).Entry(context);

    return;
}

void VM_ArmingFSM_Standby::Reset(VM_ArmingContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_ArmingFSM::Init);
    (context.getState()).Entry(context);

    return;
}

void VM_ArmingFSM_Armed::Disarm(VM_ArmingContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_ArmingFSM::Standby);
    (context.getState()).Entry(context);

    return;
}

void VM_ArmingFSM_Armed::Error(VM_ArmingContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_ArmingFSM::ArmedError);
    (context.getState()).Entry(context);

    return;
}

void VM_ArmingFSM_Armed::Reset(VM_ArmingContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_ArmingFSM::Init);
    (context.getState()).Entry(context);

    return;
}
