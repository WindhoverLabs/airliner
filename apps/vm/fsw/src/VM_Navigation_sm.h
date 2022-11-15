#ifndef _H_VM_NAVIGATION_SM
#define _H_VM_NAVIGATION_SM

#define SMC_USES_IOSTREAMS

#include "statemap.h"

// Forward declarations.
class VM_NavigationFSM;
class VM_NavigationFSM_Init;
class VM_NavigationFSM_Manual;
class VM_NavigationFSM_AltitudeControl;
class VM_NavigationFSM_PositionControl;
class VM_NavigationFSM_AutoLoiter;
class VM_NavigationFSM_AutoReturnToLaunch;
class VM_NavigationFSM_Acrobatic;
class VM_NavigationFSM_Stabilize;
class VM_NavigationFSM_Rattitude;
class VM_NavigationFSM_AutoTakeoff;
class VM_NavigationFSM_AutoLand;
class VM_NavigationFSM_Default;
class VM_NavigationState;
class VM_NavigationContext;
class VM_Navigation;

class VM_NavigationState :
    public State
{
public:

    VM_NavigationState(const char *name, int stateId)
    : State(name, stateId)
    {};

    virtual void Entry(VM_NavigationContext&) {};
    virtual void Exit(VM_NavigationContext&) {};

    virtual void Reset(VM_NavigationContext& context);
    virtual void trAcrobatic(VM_NavigationContext& context);
    virtual void trAltitudeControl(VM_NavigationContext& context);
    virtual void trAutoLand(VM_NavigationContext& context);
    virtual void trAutoLoiter(VM_NavigationContext& context);
    virtual void trAutoReturnToLaunch(VM_NavigationContext& context);
    virtual void trAutoTakeoff(VM_NavigationContext& context);
    virtual void trInitComplete(VM_NavigationContext& context);
    virtual void trManual(VM_NavigationContext& context);
    virtual void trPositionControl(VM_NavigationContext& context);
    virtual void trRattitude(VM_NavigationContext& context);
    virtual void trStabilize(VM_NavigationContext& context);

protected:

    virtual void Default(VM_NavigationContext& context);
};

class VM_NavigationFSM
{
public:

    static VM_NavigationFSM_Init Init;
    static VM_NavigationFSM_Manual Manual;
    static VM_NavigationFSM_AltitudeControl AltitudeControl;
    static VM_NavigationFSM_PositionControl PositionControl;
    static VM_NavigationFSM_AutoLoiter AutoLoiter;
    static VM_NavigationFSM_AutoReturnToLaunch AutoReturnToLaunch;
    static VM_NavigationFSM_Acrobatic Acrobatic;
    static VM_NavigationFSM_Stabilize Stabilize;
    static VM_NavigationFSM_Rattitude Rattitude;
    static VM_NavigationFSM_AutoTakeoff AutoTakeoff;
    static VM_NavigationFSM_AutoLand AutoLand;
};

class VM_NavigationFSM_Default :
    public VM_NavigationState
{
public:

    VM_NavigationFSM_Default(const char *name, int stateId)
    : VM_NavigationState(name, stateId)
    {};

};

class VM_NavigationFSM_Init :
    public VM_NavigationFSM_Default
{
public:
    VM_NavigationFSM_Init(const char *name, int stateId)
    : VM_NavigationFSM_Default(name, stateId)
    {};

    void Reset(VM_NavigationContext& context);
    void trInitComplete(VM_NavigationContext& context);
};

class VM_NavigationFSM_Manual :
    public VM_NavigationFSM_Default
{
public:
    VM_NavigationFSM_Manual(const char *name, int stateId)
    : VM_NavigationFSM_Default(name, stateId)
    {};

    void Reset(VM_NavigationContext& context);
    void trAcrobatic(VM_NavigationContext& context);
    void trAltitudeControl(VM_NavigationContext& context);
    void trAutoLand(VM_NavigationContext& context);
    void trAutoLoiter(VM_NavigationContext& context);
    void trAutoReturnToLaunch(VM_NavigationContext& context);
    void trAutoTakeoff(VM_NavigationContext& context);
    void trPositionControl(VM_NavigationContext& context);
    void trRattitude(VM_NavigationContext& context);
    void trStabilize(VM_NavigationContext& context);
};

class VM_NavigationFSM_AltitudeControl :
    public VM_NavigationFSM_Default
{
public:
    VM_NavigationFSM_AltitudeControl(const char *name, int stateId)
    : VM_NavigationFSM_Default(name, stateId)
    {};

    void Reset(VM_NavigationContext& context);
    void trAcrobatic(VM_NavigationContext& context);
    void trAutoLand(VM_NavigationContext& context);
    void trAutoLoiter(VM_NavigationContext& context);
    void trAutoReturnToLaunch(VM_NavigationContext& context);
    void trAutoTakeoff(VM_NavigationContext& context);
    void trManual(VM_NavigationContext& context);
    void trPositionControl(VM_NavigationContext& context);
    void trRattitude(VM_NavigationContext& context);
    void trStabilize(VM_NavigationContext& context);
};

class VM_NavigationFSM_PositionControl :
    public VM_NavigationFSM_Default
{
public:
    VM_NavigationFSM_PositionControl(const char *name, int stateId)
    : VM_NavigationFSM_Default(name, stateId)
    {};

    void Reset(VM_NavigationContext& context);
    void trAcrobatic(VM_NavigationContext& context);
    void trAltitudeControl(VM_NavigationContext& context);
    void trAutoLand(VM_NavigationContext& context);
    void trAutoLoiter(VM_NavigationContext& context);
    void trAutoReturnToLaunch(VM_NavigationContext& context);
    void trAutoTakeoff(VM_NavigationContext& context);
    void trManual(VM_NavigationContext& context);
    void trRattitude(VM_NavigationContext& context);
    void trStabilize(VM_NavigationContext& context);
};

class VM_NavigationFSM_AutoLoiter :
    public VM_NavigationFSM_Default
{
public:
    VM_NavigationFSM_AutoLoiter(const char *name, int stateId)
    : VM_NavigationFSM_Default(name, stateId)
    {};

    void Reset(VM_NavigationContext& context);
    void trAcrobatic(VM_NavigationContext& context);
    void trAltitudeControl(VM_NavigationContext& context);
    void trAutoLand(VM_NavigationContext& context);
    void trAutoReturnToLaunch(VM_NavigationContext& context);
    void trAutoTakeoff(VM_NavigationContext& context);
    void trManual(VM_NavigationContext& context);
    void trPositionControl(VM_NavigationContext& context);
    void trRattitude(VM_NavigationContext& context);
    void trStabilize(VM_NavigationContext& context);
};

class VM_NavigationFSM_AutoReturnToLaunch :
    public VM_NavigationFSM_Default
{
public:
    VM_NavigationFSM_AutoReturnToLaunch(const char *name, int stateId)
    : VM_NavigationFSM_Default(name, stateId)
    {};

    void Reset(VM_NavigationContext& context);
    void trAcrobatic(VM_NavigationContext& context);
    void trAltitudeControl(VM_NavigationContext& context);
    void trAutoLand(VM_NavigationContext& context);
    void trAutoLoiter(VM_NavigationContext& context);
    void trAutoTakeoff(VM_NavigationContext& context);
    void trManual(VM_NavigationContext& context);
    void trPositionControl(VM_NavigationContext& context);
    void trRattitude(VM_NavigationContext& context);
    void trStabilize(VM_NavigationContext& context);
};

class VM_NavigationFSM_Acrobatic :
    public VM_NavigationFSM_Default
{
public:
    VM_NavigationFSM_Acrobatic(const char *name, int stateId)
    : VM_NavigationFSM_Default(name, stateId)
    {};

    void Reset(VM_NavigationContext& context);
    void trAltitudeControl(VM_NavigationContext& context);
    void trAutoLand(VM_NavigationContext& context);
    void trAutoLoiter(VM_NavigationContext& context);
    void trAutoReturnToLaunch(VM_NavigationContext& context);
    void trAutoTakeoff(VM_NavigationContext& context);
    void trManual(VM_NavigationContext& context);
    void trPositionControl(VM_NavigationContext& context);
    void trRattitude(VM_NavigationContext& context);
    void trStabilize(VM_NavigationContext& context);
};

class VM_NavigationFSM_Stabilize :
    public VM_NavigationFSM_Default
{
public:
    VM_NavigationFSM_Stabilize(const char *name, int stateId)
    : VM_NavigationFSM_Default(name, stateId)
    {};

    void Reset(VM_NavigationContext& context);
    void trAcrobatic(VM_NavigationContext& context);
    void trAltitudeControl(VM_NavigationContext& context);
    void trAutoLand(VM_NavigationContext& context);
    void trAutoLoiter(VM_NavigationContext& context);
    void trAutoReturnToLaunch(VM_NavigationContext& context);
    void trAutoTakeoff(VM_NavigationContext& context);
    void trManual(VM_NavigationContext& context);
    void trPositionControl(VM_NavigationContext& context);
    void trRattitude(VM_NavigationContext& context);
};

class VM_NavigationFSM_Rattitude :
    public VM_NavigationFSM_Default
{
public:
    VM_NavigationFSM_Rattitude(const char *name, int stateId)
    : VM_NavigationFSM_Default(name, stateId)
    {};

    void Reset(VM_NavigationContext& context);
    void trAcrobatic(VM_NavigationContext& context);
    void trAltitudeControl(VM_NavigationContext& context);
    void trAutoLand(VM_NavigationContext& context);
    void trAutoLoiter(VM_NavigationContext& context);
    void trAutoReturnToLaunch(VM_NavigationContext& context);
    void trAutoTakeoff(VM_NavigationContext& context);
    void trManual(VM_NavigationContext& context);
    void trPositionControl(VM_NavigationContext& context);
    void trStabilize(VM_NavigationContext& context);
};

class VM_NavigationFSM_AutoTakeoff :
    public VM_NavigationFSM_Default
{
public:
    VM_NavigationFSM_AutoTakeoff(const char *name, int stateId)
    : VM_NavigationFSM_Default(name, stateId)
    {};

    void Reset(VM_NavigationContext& context);
    void trAcrobatic(VM_NavigationContext& context);
    void trAltitudeControl(VM_NavigationContext& context);
    void trAutoLand(VM_NavigationContext& context);
    void trAutoLoiter(VM_NavigationContext& context);
    void trAutoReturnToLaunch(VM_NavigationContext& context);
    void trManual(VM_NavigationContext& context);
    void trPositionControl(VM_NavigationContext& context);
    void trRattitude(VM_NavigationContext& context);
    void trStabilize(VM_NavigationContext& context);
};

class VM_NavigationFSM_AutoLand :
    public VM_NavigationFSM_Default
{
public:
    VM_NavigationFSM_AutoLand(const char *name, int stateId)
    : VM_NavigationFSM_Default(name, stateId)
    {};

    void Reset(VM_NavigationContext& context);
    void trAcrobatic(VM_NavigationContext& context);
    void trAltitudeControl(VM_NavigationContext& context);
    void trAutoLoiter(VM_NavigationContext& context);
    void trAutoReturnToLaunch(VM_NavigationContext& context);
    void trAutoTakeoff(VM_NavigationContext& context);
    void trManual(VM_NavigationContext& context);
    void trPositionControl(VM_NavigationContext& context);
    void trRattitude(VM_NavigationContext& context);
    void trStabilize(VM_NavigationContext& context);
};

class VM_NavigationContext :
    public FSMContext
{
public:

    VM_NavigationContext(VM_Navigation& owner)
    : _owner(owner)
    {
        setState(VM_NavigationFSM::Init);
        VM_NavigationFSM::Init.Entry(*this);
    };

    VM_Navigation& getOwner() const
    {
        return (_owner);
    };

    VM_NavigationState& getState() const
    {
        if (_state == NULL)
        {
            throw StateUndefinedException();
        }

        return (dynamic_cast<VM_NavigationState&>(*_state));
    };

    void Reset()
    {
        (getState()).Reset(*this);
    };

    void trAcrobatic()
    {
        (getState()).trAcrobatic(*this);
    };

    void trAltitudeControl()
    {
        (getState()).trAltitudeControl(*this);
    };

    void trAutoLand()
    {
        (getState()).trAutoLand(*this);
    };

    void trAutoLoiter()
    {
        (getState()).trAutoLoiter(*this);
    };

    void trAutoReturnToLaunch()
    {
        (getState()).trAutoReturnToLaunch(*this);
    };

    void trAutoTakeoff()
    {
        (getState()).trAutoTakeoff(*this);
    };

    void trInitComplete()
    {
        (getState()).trInitComplete(*this);
    };

    void trManual()
    {
        (getState()).trManual(*this);
    };

    void trPositionControl()
    {
        (getState()).trPositionControl(*this);
    };

    void trRattitude()
    {
        (getState()).trRattitude(*this);
    };

    void trStabilize()
    {
        (getState()).trStabilize(*this);
    };

private:

    VM_Navigation& _owner;
};

#endif
