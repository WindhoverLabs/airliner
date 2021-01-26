
#include "uttest.h"

#include "mavlink_app_test.h"
#include "mavlink_cmds_test.h"
#include "mavlink_config_tbl_test.h"
#include "mavlink_cds_tbl_test.h"

int main(void)
{   
    MAVLINK_App_Test_AddTestCases();
    MAVLINK_Cmds_Test_AddTestCases();
    MAVLINK_Config_Tbl_Test_AddTestCases();
    MAVLINK_CDS_Tbl_Test_AddTestCases();

    return(UtTest_Run());

}

