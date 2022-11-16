/*********************************/
/* Please do not edit this class */
/* Content change without notify */
/*********************************/

#include "VM_Navigation.h"

void state_Init(VM_Navigation *aVM_Navigation) {
    printf("Please select transition:\n");
    printf("1. trInitComplete\n");
    printf("2. Reset\n");
    printf("0. quit\n");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            aVM_Navigation->trInitComplete();
            break;
        case 2:
            aVM_Navigation->Reset();
            break;
        case 0:
            exit(0);
    }
}

void state_Manual(VM_Navigation *aVM_Navigation) {
    printf("Please select transition:\n");
    printf("1. trAltitudeControl\n");
    printf("2. trPositionControl\n");
    printf("3. trAutoLoiter\n");
    printf("4. trAutoReturnToLaunch\n");
    printf("5. trAcrobatic\n");
    printf("6. trStabilize\n");
    printf("7. trRattitude\n");
    printf("8. trAutoTakeoff\n");
    printf("9. trAutoLand\n");
    printf("10. Reset\n");
    printf("0. quit\n");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            aVM_Navigation->trAltitudeControl();
            break;
        case 2:
            aVM_Navigation->trPositionControl();
            break;
        case 3:
            aVM_Navigation->trAutoLoiter();
            break;
        case 4:
            aVM_Navigation->trAutoReturnToLaunch();
            break;
        case 5:
            aVM_Navigation->trAcrobatic();
            break;
        case 6:
            aVM_Navigation->trStabilize();
            break;
        case 7:
            aVM_Navigation->trRattitude();
            break;
        case 8:
            aVM_Navigation->trAutoTakeoff();
            break;
        case 9:
            aVM_Navigation->trAutoLand();
            break;
        case 10:
            aVM_Navigation->Reset();
            break;
        case 0:
            exit(0);
    }
}

void state_AltitudeControl(VM_Navigation *aVM_Navigation) {
    printf("Please select transition:\n");
    printf("1. trManual\n");
    printf("2. trPositionControl\n");
    printf("3. trAutoLoiter\n");
    printf("4. trAutoReturnToLaunch\n");
    printf("5. trAcrobatic\n");
    printf("6. trStabilize\n");
    printf("7. trRattitude\n");
    printf("8. trAutoTakeoff\n");
    printf("9. trAutoLand\n");
    printf("10. Reset\n");
    printf("0. quit\n");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            aVM_Navigation->trManual();
            break;
        case 2:
            aVM_Navigation->trPositionControl();
            break;
        case 3:
            aVM_Navigation->trAutoLoiter();
            break;
        case 4:
            aVM_Navigation->trAutoReturnToLaunch();
            break;
        case 5:
            aVM_Navigation->trAcrobatic();
            break;
        case 6:
            aVM_Navigation->trStabilize();
            break;
        case 7:
            aVM_Navigation->trRattitude();
            break;
        case 8:
            aVM_Navigation->trAutoTakeoff();
            break;
        case 9:
            aVM_Navigation->trAutoLand();
            break;
        case 10:
            aVM_Navigation->Reset();
            break;
        case 0:
            exit(0);
    }
}

void state_PositionControl(VM_Navigation *aVM_Navigation) {
    printf("Please select transition:\n");
    printf("1. trManual\n");
    printf("2. trAltitudeControl\n");
    printf("3. trAutoLoiter\n");
    printf("4. trAutoReturnToLaunch\n");
    printf("5. trAcrobatic\n");
    printf("6. trStabilize\n");
    printf("7. trRattitude\n");
    printf("8. trAutoTakeoff\n");
    printf("9. trAutoLand\n");
    printf("10. Reset\n");
    printf("0. quit\n");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            aVM_Navigation->trManual();
            break;
        case 2:
            aVM_Navigation->trAltitudeControl();
            break;
        case 3:
            aVM_Navigation->trAutoLoiter();
            break;
        case 4:
            aVM_Navigation->trAutoReturnToLaunch();
            break;
        case 5:
            aVM_Navigation->trAcrobatic();
            break;
        case 6:
            aVM_Navigation->trStabilize();
            break;
        case 7:
            aVM_Navigation->trRattitude();
            break;
        case 8:
            aVM_Navigation->trAutoTakeoff();
            break;
        case 9:
            aVM_Navigation->trAutoLand();
            break;
        case 10:
            aVM_Navigation->Reset();
            break;
        case 0:
            exit(0);
    }
}

void state_AutoLoiter(VM_Navigation *aVM_Navigation) {
    printf("Please select transition:\n");
    printf("1. trManual\n");
    printf("2. trAltitudeControl\n");
    printf("3. trPositionControl\n");
    printf("4. trAutoReturnToLaunch\n");
    printf("5. trAcrobatic\n");
    printf("6. trStabilize\n");
    printf("7. trRattitude\n");
    printf("8. trAutoTakeoff\n");
    printf("9. trAutoLand\n");
    printf("10. Reset\n");
    printf("0. quit\n");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            aVM_Navigation->trManual();
            break;
        case 2:
            aVM_Navigation->trAltitudeControl();
            break;
        case 3:
            aVM_Navigation->trPositionControl();
            break;
        case 4:
            aVM_Navigation->trAutoReturnToLaunch();
            break;
        case 5:
            aVM_Navigation->trAcrobatic();
            break;
        case 6:
            aVM_Navigation->trStabilize();
            break;
        case 7:
            aVM_Navigation->trRattitude();
            break;
        case 8:
            aVM_Navigation->trAutoTakeoff();
            break;
        case 9:
            aVM_Navigation->trAutoLand();
            break;
        case 10:
            aVM_Navigation->Reset();
            break;
        case 0:
            exit(0);
    }
}

void state_AutoReturnToLaunch(VM_Navigation *aVM_Navigation) {
    printf("Please select transition:\n");
    printf("1. trManual\n");
    printf("2. trAltitudeControl\n");
    printf("3. trPositionControl\n");
    printf("4. trAutoLoiter\n");
    printf("5. trAcrobatic\n");
    printf("6. trStabilize\n");
    printf("7. trRattitude\n");
    printf("8. trAutoTakeoff\n");
    printf("9. trAutoLand\n");
    printf("10. Reset\n");
    printf("0. quit\n");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            aVM_Navigation->trManual();
            break;
        case 2:
            aVM_Navigation->trAltitudeControl();
            break;
        case 3:
            aVM_Navigation->trPositionControl();
            break;
        case 4:
            aVM_Navigation->trAutoLoiter();
            break;
        case 5:
            aVM_Navigation->trAcrobatic();
            break;
        case 6:
            aVM_Navigation->trStabilize();
            break;
        case 7:
            aVM_Navigation->trRattitude();
            break;
        case 8:
            aVM_Navigation->trAutoTakeoff();
            break;
        case 9:
            aVM_Navigation->trAutoLand();
            break;
        case 10:
            aVM_Navigation->Reset();
            break;
        case 0:
            exit(0);
    }
}

void state_Acrobatic(VM_Navigation *aVM_Navigation) {
    printf("Please select transition:\n");
    printf("1. trManual\n");
    printf("2. trAltitudeControl\n");
    printf("3. trPositionControl\n");
    printf("4. trAutoLoiter\n");
    printf("5. trAutoReturnToLaunch\n");
    printf("6. trStabilize\n");
    printf("7. trRattitude\n");
    printf("8. trAutoTakeoff\n");
    printf("9. trAutoLand\n");
    printf("10. Reset\n");
    printf("0. quit\n");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            aVM_Navigation->trManual();
            break;
        case 2:
            aVM_Navigation->trAltitudeControl();
            break;
        case 3:
            aVM_Navigation->trPositionControl();
            break;
        case 4:
            aVM_Navigation->trAutoLoiter();
            break;
        case 5:
            aVM_Navigation->trAutoReturnToLaunch();
            break;
        case 6:
            aVM_Navigation->trStabilize();
            break;
        case 7:
            aVM_Navigation->trRattitude();
            break;
        case 8:
            aVM_Navigation->trAutoTakeoff();
            break;
        case 9:
            aVM_Navigation->trAutoLand();
            break;
        case 10:
            aVM_Navigation->Reset();
            break;
        case 0:
            exit(0);
    }
}

void state_Stabilize(VM_Navigation *aVM_Navigation) {
    printf("Please select transition:\n");
    printf("1. trManual\n");
    printf("2. trAltitudeControl\n");
    printf("3. trPositionControl\n");
    printf("4. trAutoLoiter\n");
    printf("5. trAutoReturnToLaunch\n");
    printf("6. trAcrobatic\n");
    printf("7. trRattitude\n");
    printf("8. trAutoTakeoff\n");
    printf("9. trAutoLand\n");
    printf("10. Reset\n");
    printf("0. quit\n");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            aVM_Navigation->trManual();
            break;
        case 2:
            aVM_Navigation->trAltitudeControl();
            break;
        case 3:
            aVM_Navigation->trPositionControl();
            break;
        case 4:
            aVM_Navigation->trAutoLoiter();
            break;
        case 5:
            aVM_Navigation->trAutoReturnToLaunch();
            break;
        case 6:
            aVM_Navigation->trAcrobatic();
            break;
        case 7:
            aVM_Navigation->trRattitude();
            break;
        case 8:
            aVM_Navigation->trAutoTakeoff();
            break;
        case 9:
            aVM_Navigation->trAutoLand();
            break;
        case 10:
            aVM_Navigation->Reset();
            break;
        case 0:
            exit(0);
    }
}

void state_Rattitude(VM_Navigation *aVM_Navigation) {
    printf("Please select transition:\n");
    printf("1. trManual\n");
    printf("2. trAltitudeControl\n");
    printf("3. trPositionControl\n");
    printf("4. trAutoLoiter\n");
    printf("5. trAutoReturnToLaunch\n");
    printf("6. trAcrobatic\n");
    printf("7. trStabilize\n");
    printf("8. trAutoTakeoff\n");
    printf("9. trAutoLand\n");
    printf("10. Reset\n");
    printf("0. quit\n");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            aVM_Navigation->trManual();
            break;
        case 2:
            aVM_Navigation->trAltitudeControl();
            break;
        case 3:
            aVM_Navigation->trPositionControl();
            break;
        case 4:
            aVM_Navigation->trAutoLoiter();
            break;
        case 5:
            aVM_Navigation->trAutoReturnToLaunch();
            break;
        case 6:
            aVM_Navigation->trAcrobatic();
            break;
        case 7:
            aVM_Navigation->trStabilize();
            break;
        case 8:
            aVM_Navigation->trAutoTakeoff();
            break;
        case 9:
            aVM_Navigation->trAutoLand();
            break;
        case 10:
            aVM_Navigation->Reset();
            break;
        case 0:
            exit(0);
    }
}

void state_AutoTakeoff(VM_Navigation *aVM_Navigation) {
    printf("Please select transition:\n");
    printf("1. trManual\n");
    printf("2. trAltitudeControl\n");
    printf("3. trPositionControl\n");
    printf("4. trAutoLoiter\n");
    printf("5. trAutoReturnToLaunch\n");
    printf("6. trAcrobatic\n");
    printf("7. trStabilize\n");
    printf("8. trRattitude\n");
    printf("9. trAutoLand\n");
    printf("10. Reset\n");
    printf("0. quit\n");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            aVM_Navigation->trManual();
            break;
        case 2:
            aVM_Navigation->trAltitudeControl();
            break;
        case 3:
            aVM_Navigation->trPositionControl();
            break;
        case 4:
            aVM_Navigation->trAutoLoiter();
            break;
        case 5:
            aVM_Navigation->trAutoReturnToLaunch();
            break;
        case 6:
            aVM_Navigation->trAcrobatic();
            break;
        case 7:
            aVM_Navigation->trStabilize();
            break;
        case 8:
            aVM_Navigation->trRattitude();
            break;
        case 9:
            aVM_Navigation->trAutoLand();
            break;
        case 10:
            aVM_Navigation->Reset();
            break;
        case 0:
            exit(0);
    }
}

void state_AutoLand(VM_Navigation *aVM_Navigation) {
    printf("Please select transition:\n");
    printf("1. trManual\n");
    printf("2. trAltitudeControl\n");
    printf("3. trPositionControl\n");
    printf("4. trAutoLoiter\n");
    printf("5. trAutoReturnToLaunch\n");
    printf("6. trAcrobatic\n");
    printf("7. trStabilize\n");
    printf("8. trRattitude\n");
    printf("9. trAutoTakeoff\n");
    printf("10. Reset\n");
    printf("0. quit\n");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            aVM_Navigation->trManual();
            break;
        case 2:
            aVM_Navigation->trAltitudeControl();
            break;
        case 3:
            aVM_Navigation->trPositionControl();
            break;
        case 4:
            aVM_Navigation->trAutoLoiter();
            break;
        case 5:
            aVM_Navigation->trAutoReturnToLaunch();
            break;
        case 6:
            aVM_Navigation->trAcrobatic();
            break;
        case 7:
            aVM_Navigation->trStabilize();
            break;
        case 8:
            aVM_Navigation->trRattitude();
            break;
        case 9:
            aVM_Navigation->trAutoTakeoff();
            break;
        case 10:
            aVM_Navigation->Reset();
            break;
        case 0:
            exit(0);
    }
}

int main(int argc, char **argv) {
    VM_Navigation lVM_Navigation;
    while (true) {
        printf("Current state: %s\n", lVM_Navigation.getContext().getState().getName());
        if (&lVM_Navigation.getContext().getState() == &VM_NavigationFSM::Init) {
            state_Init(&lVM_Navigation);
        }
        else if (&lVM_Navigation.getContext().getState() == &VM_NavigationFSM::Manual) {
            state_Manual(&lVM_Navigation);
        }
        else if (&lVM_Navigation.getContext().getState() == &VM_NavigationFSM::AltitudeControl) {
            state_AltitudeControl(&lVM_Navigation);
        }
        else if (&lVM_Navigation.getContext().getState() == &VM_NavigationFSM::PositionControl) {
            state_PositionControl(&lVM_Navigation);
        }
        else if (&lVM_Navigation.getContext().getState() == &VM_NavigationFSM::AutoLoiter) {
            state_AutoLoiter(&lVM_Navigation);
        }
        else if (&lVM_Navigation.getContext().getState() == &VM_NavigationFSM::AutoReturnToLaunch) {
            state_AutoReturnToLaunch(&lVM_Navigation);
        }
        else if (&lVM_Navigation.getContext().getState() == &VM_NavigationFSM::Acrobatic) {
            state_Acrobatic(&lVM_Navigation);
        }
        else if (&lVM_Navigation.getContext().getState() == &VM_NavigationFSM::Stabilize) {
            state_Stabilize(&lVM_Navigation);
        }
        else if (&lVM_Navigation.getContext().getState() == &VM_NavigationFSM::Rattitude) {
            state_Rattitude(&lVM_Navigation);
        }
        else if (&lVM_Navigation.getContext().getState() == &VM_NavigationFSM::AutoTakeoff) {
            state_AutoTakeoff(&lVM_Navigation);
        }
        else if (&lVM_Navigation.getContext().getState() == &VM_NavigationFSM::AutoLand) {
            state_AutoLand(&lVM_Navigation);
        }
    }
}
