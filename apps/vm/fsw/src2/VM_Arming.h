#include "VM_Arming_sm.h"

class VM_Arming
{
    private:
        VM_ArmingContext _fsm;


    public:
        VM_Arming() : _fsm(*this) {
        }

        VM_ArmingContext& getContext() {
            return _fsm;
        }

        void InitComplete() {
            _fsm.InitComplete();
        }

        void Reset() {
            _fsm.Reset();
        }

        void Arm() {
            _fsm.Arm();
        }

        void Error() {
            _fsm.Error();
        }

        void Disarm() {
            _fsm.Disarm();
        }

};
