#ifndef _H_VM_ARMING_SM
#define _H_VM_ARMING_SM

#define SMC_USES_IOSTREAMS

#include "statemap.h"

// Forward declarations.
class VM_ArmingFSM;
class VM_ArmingFSM_Init;
class VM_ArmingFSM_Standby;
class VM_ArmingFSM_Armed;
class VM_ArmingFSM_StandbyError;
class VM_ArmingFSM_ArmedError;
class VM_ArmingFSM_Default;
class VM_ArmingState;
class VM_ArmingContext;
class VM_Arming;

class VM_ArmingState :
    public State
{
public:

    VM_ArmingState(const char *name, int stateId)
    : State(name, stateId)
    {};

    virtual void Entry(VM_ArmingContext&) {};
    virtual void Exit(VM_ArmingContext&) {};

    virtual void Arm(VM_ArmingContext& context);
    virtual void Disarm(VM_ArmingContext& context);
    virtual void Error(VM_ArmingContext& context);
    virtual void InitComplete(VM_ArmingContext& context);
    virtual void Reset(VM_ArmingContext& context);

protected:

    virtual void Default(VM_ArmingContext& context);
};

class VM_ArmingFSM
{
public:

    static VM_ArmingFSM_Init Init;
    static VM_ArmingFSM_Standby Standby;
    static VM_ArmingFSM_Armed Armed;
    static VM_ArmingFSM_StandbyError StandbyError;
    static VM_ArmingFSM_ArmedError ArmedError;
};

class VM_ArmingFSM_Default :
    public VM_ArmingState
{
public:

    VM_ArmingFSM_Default(const char *name, int stateId)
    : VM_ArmingState(name, stateId)
    {};

};

class VM_ArmingFSM_Init :
    public VM_ArmingFSM_Default
{
public:
    VM_ArmingFSM_Init(const char *name, int stateId)
    : VM_ArmingFSM_Default(name, stateId)
    {};

    void InitComplete(VM_ArmingContext& context);
    void Reset(VM_ArmingContext& context);
};

class VM_ArmingFSM_Standby :
    public VM_ArmingFSM_Default
{
public:
    VM_ArmingFSM_Standby(const char *name, int stateId)
    : VM_ArmingFSM_Default(name, stateId)
    {};

    void Arm(VM_ArmingContext& context);
    void Error(VM_ArmingContext& context);
    void Reset(VM_ArmingContext& context);
};

class VM_ArmingFSM_Armed :
    public VM_ArmingFSM_Default
{
public:
    VM_ArmingFSM_Armed(const char *name, int stateId)
    : VM_ArmingFSM_Default(name, stateId)
    {};

    void Disarm(VM_ArmingContext& context);
    void Error(VM_ArmingContext& context);
    void Reset(VM_ArmingContext& context);
};

class VM_ArmingFSM_StandbyError :
    public VM_ArmingFSM_Default
{
public:
    VM_ArmingFSM_StandbyError(const char *name, int stateId)
    : VM_ArmingFSM_Default(name, stateId)
    {};

};

class VM_ArmingFSM_ArmedError :
    public VM_ArmingFSM_Default
{
public:
    VM_ArmingFSM_ArmedError(const char *name, int stateId)
    : VM_ArmingFSM_Default(name, stateId)
    {};

};

class VM_ArmingContext :
    public FSMContext
{
public:

    VM_ArmingContext(VM_Arming& owner)
    : _owner(owner)
    {
        setState(VM_ArmingFSM::Init);
        VM_ArmingFSM::Init.Entry(*this);
    };

    VM_Arming& getOwner() const
    {
        return (_owner);
    };

    VM_ArmingState& getState() const
    {
        if (_state == NULL)
        {
            throw StateUndefinedException();
        }

        return (dynamic_cast<VM_ArmingState&>(*_state));
    };

    void Arm()
    {
        (getState()).Arm(*this);
    };

    void Disarm()
    {
        (getState()).Disarm(*this);
    };

    void Error()
    {
        (getState()).Error(*this);
    };

    void InitComplete()
    {
        (getState()).InitComplete(*this);
    };

    void Reset()
    {
        (getState()).Reset(*this);
    };

private:

    VM_Arming& _owner;
};

#endif
