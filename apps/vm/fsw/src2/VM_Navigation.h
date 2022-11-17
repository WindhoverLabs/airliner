#include "VM_Navigation_sm.h"

class VM_Navigation
{
    private:
        VM_NavigationContext _fsm;


    public:
        VM_Navigation() : _fsm(*this) {
        }

        VM_NavigationContext& getContext() {
            return _fsm;
        }

        void trAltitudeControl() {
            _fsm.trAltitudeControl();
        }

        void trPositionControl() {
            _fsm.trPositionControl();
        }

        void trAutoLoiter() {
            _fsm.trAutoLoiter();
        }

        void trAutoReturnToLaunch() {
            _fsm.trAutoReturnToLaunch();
        }

        void trAcrobatic() {
            _fsm.trAcrobatic();
        }

        void trStabilize() {
            _fsm.trStabilize();
        }

        void trRattitude() {
            _fsm.trRattitude();
        }

        void trAutoTakeoff() {
            _fsm.trAutoTakeoff();
        }

        void trAutoLand() {
            _fsm.trAutoLand();
        }

        void Reset() {
            _fsm.Reset();
        }

        void trManual() {
            _fsm.trManual();
        }

        void trInitComplete() {
            _fsm.trInitComplete();
        }

};
