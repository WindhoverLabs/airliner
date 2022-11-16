/*********************************/
/* Please do not edit this class */
/* Content change without notify */
/*********************************/

#include "VM_Arming.h"

void state_Init(VM_Arming *aVM_Arming) {
    printf("Please select transition:\n");
    printf("1. InitComplete\n");
    printf("2. Reset\n");
    printf("0. quit\n");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            aVM_Arming->InitComplete();
            break;
        case 2:
            aVM_Arming->Reset();
            break;
        case 0:
            exit(0);
    }
}

void state_Standby(VM_Arming *aVM_Arming) {
    printf("Please select transition:\n");
    printf("1. Arm\n");
    printf("2. Error\n");
    printf("3. Reset\n");
    printf("0. quit\n");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            aVM_Arming->Arm();
            break;
        case 2:
            aVM_Arming->Error();
            break;
        case 3:
            aVM_Arming->Reset();
            break;
        case 0:
            exit(0);
    }
}

void state_Armed(VM_Arming *aVM_Arming) {
    printf("Please select transition:\n");
    printf("1. Disarm\n");
    printf("2. Error\n");
    printf("3. Reset\n");
    printf("0. quit\n");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            aVM_Arming->Disarm();
            break;
        case 2:
            aVM_Arming->Error();
            break;
        case 3:
            aVM_Arming->Reset();
            break;
        case 0:
            exit(0);
    }
}

int main(int argc, char **argv) {
    VM_Arming lVM_Arming;
    while (true) {
        printf("Current state: %s\n", lVM_Arming.getContext().getState().getName());
        if (&lVM_Arming.getContext().getState() == &VM_ArmingFSM::Init) {
            state_Init(&lVM_Arming);
        }
        else if (&lVM_Arming.getContext().getState() == &VM_ArmingFSM::Standby) {
            state_Standby(&lVM_Arming);
        }
        else if (&lVM_Arming.getContext().getState() == &VM_ArmingFSM::Armed) {
            state_Armed(&lVM_Arming);
        }
    }
}
