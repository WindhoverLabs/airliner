#include "pb_lib.h"
#include "uttest.h"

#include "pbl_app_test.h"

int main(void)
{   
    PBLIB_App_Test_AddTestCases();

    return(UtTest_Run());

}

