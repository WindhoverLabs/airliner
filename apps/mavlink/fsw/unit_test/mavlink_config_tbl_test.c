
#include "mavlink_config_tbl_test.h"
#include "mavlink_test_utils.h"

#include "mavlink_msg.h"

#include "uttest.h"
#include "ut_osapi_stubs.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_es_hooks.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_cfe_time_stubs.h"


void MAVLINK_Config_Tbl_Test_Case1(void)
{

}


void MAVLINK_Config_Tbl_Test_AddTestCases(void)
{
    UtTest_Add(MAVLINK_Config_Tbl_Test_Case1, MAVLINK_Test_Setup, MAVLINK_Test_TearDown, "MAVLINK_Config_Tbl_Test_Case1");
}


