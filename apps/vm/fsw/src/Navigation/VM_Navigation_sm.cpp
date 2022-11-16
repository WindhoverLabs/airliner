#include "VM_Navigation.h"
#include "VM_Navigation_sm.h"


// Static class declarations.
VM_NavigationFSM_Init VM_NavigationFSM::Init("VM_NavigationFSM::Init", 0);
VM_NavigationFSM_Manual VM_NavigationFSM::Manual("VM_NavigationFSM::Manual", 1);
VM_NavigationFSM_AltitudeControl VM_NavigationFSM::AltitudeControl("VM_NavigationFSM::AltitudeControl", 2);
VM_NavigationFSM_PositionControl VM_NavigationFSM::PositionControl("VM_NavigationFSM::PositionControl", 3);
VM_NavigationFSM_AutoLoiter VM_NavigationFSM::AutoLoiter("VM_NavigationFSM::AutoLoiter", 4);
VM_NavigationFSM_AutoReturnToLaunch VM_NavigationFSM::AutoReturnToLaunch("VM_NavigationFSM::AutoReturnToLaunch", 5);
VM_NavigationFSM_Acrobatic VM_NavigationFSM::Acrobatic("VM_NavigationFSM::Acrobatic", 6);
VM_NavigationFSM_Stabilize VM_NavigationFSM::Stabilize("VM_NavigationFSM::Stabilize", 7);
VM_NavigationFSM_Rattitude VM_NavigationFSM::Rattitude("VM_NavigationFSM::Rattitude", 8);
VM_NavigationFSM_AutoTakeoff VM_NavigationFSM::AutoTakeoff("VM_NavigationFSM::AutoTakeoff", 9);
VM_NavigationFSM_AutoLand VM_NavigationFSM::AutoLand("VM_NavigationFSM::AutoLand", 10);

void VM_NavigationState::Reset(VM_NavigationContext& context)
{
    Default(context);
    return;
}

void VM_NavigationState::trAcrobatic(VM_NavigationContext& context)
{
    Default(context);
    return;
}

void VM_NavigationState::trAltitudeControl(VM_NavigationContext& context)
{
    Default(context);
    return;
}

void VM_NavigationState::trAutoLand(VM_NavigationContext& context)
{
    Default(context);
    return;
}

void VM_NavigationState::trAutoLoiter(VM_NavigationContext& context)
{
    Default(context);
    return;
}

void VM_NavigationState::trAutoReturnToLaunch(VM_NavigationContext& context)
{
    Default(context);
    return;
}

void VM_NavigationState::trAutoTakeoff(VM_NavigationContext& context)
{
    Default(context);
    return;
}

void VM_NavigationState::trInitComplete(VM_NavigationContext& context)
{
    Default(context);
    return;
}

void VM_NavigationState::trManual(VM_NavigationContext& context)
{
    Default(context);
    return;
}

void VM_NavigationState::trPositionControl(VM_NavigationContext& context)
{
    Default(context);
    return;
}

void VM_NavigationState::trRattitude(VM_NavigationContext& context)
{
    Default(context);
    return;
}

void VM_NavigationState::trStabilize(VM_NavigationContext& context)
{
    Default(context);
    return;
}

void VM_NavigationState::Default(VM_NavigationContext& context)
{
    throw (
        TransitionUndefinedException(
            context.getState().getName(),
            context.getTransition()));

    return;
}

void VM_NavigationFSM_Init::Reset(VM_NavigationContext& context)
{


    return;
}

void VM_NavigationFSM_Init::trInitComplete(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Manual);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Manual::Reset(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Init);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Manual::trAcrobatic(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Acrobatic);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Manual::trAltitudeControl(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AltitudeControl);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Manual::trAutoLand(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoLand);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Manual::trAutoLoiter(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoLoiter);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Manual::trAutoReturnToLaunch(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoReturnToLaunch);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Manual::trAutoTakeoff(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoTakeoff);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Manual::trPositionControl(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::PositionControl);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Manual::trRattitude(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Rattitude);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Manual::trStabilize(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Stabilize);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AltitudeControl::Reset(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Init);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AltitudeControl::trAcrobatic(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Acrobatic);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AltitudeControl::trAutoLand(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoLand);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AltitudeControl::trAutoLoiter(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoLoiter);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AltitudeControl::trAutoReturnToLaunch(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoReturnToLaunch);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AltitudeControl::trAutoTakeoff(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoTakeoff);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AltitudeControl::trManual(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Manual);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AltitudeControl::trPositionControl(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::PositionControl);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AltitudeControl::trRattitude(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Rattitude);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AltitudeControl::trStabilize(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Stabilize);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_PositionControl::Reset(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Init);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_PositionControl::trAcrobatic(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Acrobatic);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_PositionControl::trAltitudeControl(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AltitudeControl);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_PositionControl::trAutoLand(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoLand);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_PositionControl::trAutoLoiter(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoLoiter);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_PositionControl::trAutoReturnToLaunch(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoReturnToLaunch);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_PositionControl::trAutoTakeoff(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoTakeoff);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_PositionControl::trManual(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Manual);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_PositionControl::trRattitude(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Rattitude);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_PositionControl::trStabilize(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Stabilize);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoLoiter::Reset(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Init);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoLoiter::trAcrobatic(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Acrobatic);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoLoiter::trAltitudeControl(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AltitudeControl);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoLoiter::trAutoLand(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoLand);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoLoiter::trAutoReturnToLaunch(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoReturnToLaunch);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoLoiter::trAutoTakeoff(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoTakeoff);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoLoiter::trManual(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Manual);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoLoiter::trPositionControl(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::PositionControl);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoLoiter::trRattitude(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Rattitude);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoLoiter::trStabilize(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Stabilize);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoReturnToLaunch::Reset(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Init);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoReturnToLaunch::trAcrobatic(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Acrobatic);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoReturnToLaunch::trAltitudeControl(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AltitudeControl);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoReturnToLaunch::trAutoLand(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoLand);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoReturnToLaunch::trAutoLoiter(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoLoiter);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoReturnToLaunch::trAutoTakeoff(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoTakeoff);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoReturnToLaunch::trManual(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Manual);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoReturnToLaunch::trPositionControl(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::PositionControl);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoReturnToLaunch::trRattitude(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Rattitude);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoReturnToLaunch::trStabilize(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Stabilize);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Acrobatic::Reset(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Init);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Acrobatic::trAltitudeControl(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AltitudeControl);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Acrobatic::trAutoLand(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoLand);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Acrobatic::trAutoLoiter(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoLoiter);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Acrobatic::trAutoReturnToLaunch(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoReturnToLaunch);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Acrobatic::trAutoTakeoff(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoTakeoff);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Acrobatic::trManual(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Manual);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Acrobatic::trPositionControl(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::PositionControl);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Acrobatic::trRattitude(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Rattitude);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Acrobatic::trStabilize(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Stabilize);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Stabilize::Reset(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Init);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Stabilize::trAcrobatic(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Acrobatic);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Stabilize::trAltitudeControl(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AltitudeControl);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Stabilize::trAutoLand(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoLand);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Stabilize::trAutoLoiter(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoLoiter);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Stabilize::trAutoReturnToLaunch(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoReturnToLaunch);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Stabilize::trAutoTakeoff(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoTakeoff);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Stabilize::trManual(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Manual);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Stabilize::trPositionControl(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::PositionControl);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Stabilize::trRattitude(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Rattitude);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Rattitude::Reset(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Init);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Rattitude::trAcrobatic(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Acrobatic);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Rattitude::trAltitudeControl(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AltitudeControl);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Rattitude::trAutoLand(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoLand);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Rattitude::trAutoLoiter(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoLoiter);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Rattitude::trAutoReturnToLaunch(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoReturnToLaunch);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Rattitude::trAutoTakeoff(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoTakeoff);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Rattitude::trManual(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Manual);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Rattitude::trPositionControl(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::PositionControl);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_Rattitude::trStabilize(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Stabilize);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoTakeoff::Reset(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Init);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoTakeoff::trAcrobatic(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Acrobatic);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoTakeoff::trAltitudeControl(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AltitudeControl);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoTakeoff::trAutoLand(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoLand);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoTakeoff::trAutoLoiter(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoLoiter);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoTakeoff::trAutoReturnToLaunch(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoReturnToLaunch);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoTakeoff::trManual(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Manual);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoTakeoff::trPositionControl(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::PositionControl);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoTakeoff::trRattitude(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Rattitude);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoTakeoff::trStabilize(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Stabilize);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoLand::Reset(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Init);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoLand::trAcrobatic(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Acrobatic);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoLand::trAltitudeControl(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AltitudeControl);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoLand::trAutoLoiter(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoLoiter);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoLand::trAutoReturnToLaunch(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoReturnToLaunch);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoLand::trAutoTakeoff(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::AutoTakeoff);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoLand::trManual(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Manual);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoLand::trPositionControl(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::PositionControl);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoLand::trRattitude(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Rattitude);
    (context.getState()).Entry(context);

    return;
}

void VM_NavigationFSM_AutoLand::trStabilize(VM_NavigationContext& context)
{

    (context.getState()).Exit(context);
    context.setState(VM_NavigationFSM::Stabilize);
    (context.getState()).Entry(context);

    return;
}
