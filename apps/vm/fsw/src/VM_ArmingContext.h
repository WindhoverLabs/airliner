//
// ex: set ro:
// DO NOT EDIT.
// generated by smc (http://smc.sourceforge.net/)
// from file : VM_Arming.sm
//

#ifndef VM_ARMINGCONTEXT_H
#define VM_ARMINGCONTEXT_H


#define SMC_USES_IOSTREAMS

#include <statemap.h>

// Forward declarations.
class VM_ArmingMap;
class VM_ArmingMap_Init;
class VM_ArmingMap_Standby;
class VM_ArmingMap_Armed;
class VM_ArmingMap_StandbyError;
class VM_ArmingMap_ArmedError;
class VM_ArmingMap_Default;
class VM_ArmingState;
class VM_ArmingContext;
class VM_Arming;

class VM_ArmingState :
    public statemap::State
{
public:

    VM_ArmingState(const char * const name, const int stateId)
    : statemap::State(name, stateId)
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

class VM_ArmingMap
{
public:

    static VM_ArmingMap_Init Init;
    static VM_ArmingMap_Standby Standby;
    static VM_ArmingMap_Armed Armed;
    static VM_ArmingMap_StandbyError StandbyError;
    static VM_ArmingMap_ArmedError ArmedError;
};

class VM_ArmingMap_Default :
    public VM_ArmingState
{
public:

    VM_ArmingMap_Default(const char * const name, const int stateId)
    : VM_ArmingState(name, stateId)
    {};

};

class VM_ArmingMap_Init :
    public VM_ArmingMap_Default
{
public:
    VM_ArmingMap_Init(const char * const name, const int stateId)
    : VM_ArmingMap_Default(name, stateId)
    {};

    virtual void InitComplete(VM_ArmingContext& context);
    virtual void Reset(VM_ArmingContext& context);
};

class VM_ArmingMap_Standby :
    public VM_ArmingMap_Default
{
public:
    VM_ArmingMap_Standby(const char * const name, const int stateId)
    : VM_ArmingMap_Default(name, stateId)
    {};

    virtual void Entry(VM_ArmingContext&);
    virtual void Arm(VM_ArmingContext& context);
    virtual void Error(VM_ArmingContext& context);
    virtual void Reset(VM_ArmingContext& context);
};

class VM_ArmingMap_Armed :
    public VM_ArmingMap_Default
{
public:
    VM_ArmingMap_Armed(const char * const name, const int stateId)
    : VM_ArmingMap_Default(name, stateId)
    {};

    virtual void Entry(VM_ArmingContext&);
    virtual void Exit(VM_ArmingContext&);
    virtual void Disarm(VM_ArmingContext& context);
    virtual void Error(VM_ArmingContext& context);
    virtual void Reset(VM_ArmingContext& context);
};

class VM_ArmingMap_StandbyError :
    public VM_ArmingMap_Default
{
public:
    VM_ArmingMap_StandbyError(const char * const name, const int stateId)
    : VM_ArmingMap_Default(name, stateId)
    {};

    virtual void Entry(VM_ArmingContext&);
};

class VM_ArmingMap_ArmedError :
    public VM_ArmingMap_Default
{
public:
    VM_ArmingMap_ArmedError(const char * const name, const int stateId)
    : VM_ArmingMap_Default(name, stateId)
    {};

    virtual void Entry(VM_ArmingContext&);
};

class VM_ArmingContext :
    public statemap::FSMContext
{
public:

    explicit VM_ArmingContext(VM_Arming& owner)
    : FSMContext(VM_ArmingMap::Init),
      _owner(owner)
    {};

    VM_ArmingContext(VM_Arming& owner, const statemap::State& state)
    : FSMContext(state),
      _owner(owner)
    {};

    virtual void enterStartState()
    {
        getState().Entry(*this);
        return;
    }

    inline VM_Arming& getOwner()
    {
        return (_owner);
    };

    inline VM_ArmingState& getState()
    {
        if (_state == NULL)
        {
            throw statemap::StateUndefinedException();
        }

        return dynamic_cast<VM_ArmingState&>(*_state);
    };

    inline void Arm()
    {
        getState().Arm(*this);
    };

    inline void Disarm()
    {
        getState().Disarm(*this);
    };

    inline void Error()
    {
        getState().Error(*this);
    };

    inline void InitComplete()
    {
        getState().InitComplete(*this);
    };

    inline void Reset()
    {
        getState().Reset(*this);
    };

private:
    VM_Arming& _owner;
};


#endif // VM_ARMINGCONTEXT_H

//
// Local variables:
//  buffer-read-only: t
// End:
//