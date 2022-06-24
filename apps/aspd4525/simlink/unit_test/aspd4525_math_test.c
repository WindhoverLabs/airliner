/**
 * @file aspd4525_math_test.c
 * @author Shahed Rahim (srahim@windhoverlabs.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "aspd4525_test_utils.h"
#include "aspd4525_math.h"
#include "aspd4525_msg.h"

#include <float.h>

#include "uttest.h"
#include "ut_osapi_stubs.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_sb_hooks.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_es_hooks.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_cfe_time_stubs.h"

void Test_ASPD4525_MATH_GetTempDataCounts(void)
{
    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        float fTemperature = ASPD4525_MATH_CELSIUS2KELVIN( 0.0 );
        uint16 result;
        uint16 expected = 0;

        configTable.fTemperatureMaximum_Celcius = 150.0;
        configTable.fTemperatureMinimum_Celcius = -50.0;

        result = ASPD4525_MATH_GetTempDataCounts((ASPD4525_ConfigTblEntry_t*) 0, fTemperature);
        sprintf(message, "TemperatureCounts should be %d because bad config table pointer, %d\n", expected, result);
        UtAssert_IntegerCmpAbs(result, expected, 0, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        float fTemperature = ASPD4525_MATH_CELSIUS2KELVIN( -50.0 );
        uint16 result;
        uint16 expected = 0;

        configTable.fTemperatureMaximum_Celcius = 150.0;
        configTable.fTemperatureMinimum_Celcius = -50.0;

        result = ASPD4525_MATH_GetTempDataCounts(&configTable, fTemperature);
        sprintf(message, "TemperatureCounts should be %d, got %d\n", expected, result);
        UtAssert_IntegerCmpAbs(result, expected, 0, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        float fTemperature = ASPD4525_MATH_CELSIUS2KELVIN( 150.0 );
        uint16 result;
        uint16 expected = 0x7ff;

        configTable.fTemperatureMaximum_Celcius = 150.0;
        configTable.fTemperatureMinimum_Celcius = -50.0;

        result = ASPD4525_MATH_GetTempDataCounts(&configTable, fTemperature);
        sprintf(message, "TemperatureCounts should be %d, got %d\n", expected, result);
        UtAssert_IntegerCmpAbs(result, expected, 1, message);
    }
}

void Test_ASPD4525_MATH_GetDiffPressureDataCounts(void)
{
    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        float fDiffPressure = 0.0;
        uint16 result;
        uint16 expected = 0;

        configTable.fPressureMaximum_PSI = 0.5;
        configTable.fPressureMinimum_PSI = -0.5;

        result = ASPD4525_MATH_GetDiffPressureDataCounts((ASPD4525_ConfigTblEntry_t*) 0, fDiffPressure);
        sprintf(message, "DiffPressureCnts should be %d because bad config table pointer, %d\n", expected, result);
        UtAssert_IntegerCmpAbs(result, expected, 0, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        float fDiffPressure = 0.0;
        uint16 result;
        uint16 expected = 0x2000;

        configTable.fPressureMaximum_PSI = 0.5;
        configTable.fPressureMinimum_PSI = -0.5;

        result = ASPD4525_MATH_GetDiffPressureDataCounts(&configTable, fDiffPressure);
        sprintf(message, "DiffPressureCnts should be %d, got %d\n", expected, result);
        UtAssert_IntegerCmpAbs(result, expected, 1, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        float fDiffPressure = -0.5;
        uint16 result;
        uint16 expected = 0x0333;

        configTable.fPressureMaximum_PSI = 0.5;
        configTable.fPressureMinimum_PSI = -0.5;

        result = ASPD4525_MATH_GetDiffPressureDataCounts(&configTable, fDiffPressure);
        sprintf(message, "DiffPressureCnts should be %d, got %d\n", expected, result);
        UtAssert_IntegerCmpAbs(result, expected, 1, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        float fDiffPressure = 0.5;
        uint16 result;
        uint16 expected = 0x3ccb;

        configTable.fPressureMaximum_PSI = 0.5;
        configTable.fPressureMinimum_PSI = -0.5;

        result = ASPD4525_MATH_GetDiffPressureDataCounts(&configTable, fDiffPressure);
        sprintf(message, "DiffPressureCnts should be %d, got %d\n", expected, result);
        UtAssert_IntegerCmpAbs(result, expected, 1, message);
    }

}

void Test_ASPD4525_MATH_ByteFlip(void)
{
    {
        char message[100];
        uint32 data = 0x12345678;
        uint32 expected = 0x78563412;
        uint32 result;
        result = ASPD4525_MATH_ByteFlip(data);
        sprintf(message, "result should be 0x%08lx, got 0x%08lx\n", expected, result);
        UtAssert_IntegerCmpAbs(result, expected, 0, message);
    }
}


void Test_ASPD4525_MATH_PackOutGoingData(void)
{
    {
        char message[100];
        uint16 uDiffPressureDataCounts = 0x2edc;
        uint16 uTempDataCounts = 0x489;
        uint8 status = 2;
        uint32 expected = 0xaedc9120;
        uint32 result;
        result = ASPD4525_MATH_PackOutGoingData(uDiffPressureDataCounts, uTempDataCounts, status);
        sprintf(message, "result should be 0x%08lx, got 0x%08lx\n", expected, result);
        UtAssert_IntegerCmpAbs(result, expected, 0, message);
    }
}

void ASPD4525_MATH_Test_AddTestCases(void)
{
    UtTest_Add(Test_ASPD4525_MATH_GetTempDataCounts, ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_MATH_GetTempDataCounts");

    UtTest_Add(Test_ASPD4525_MATH_GetDiffPressureDataCounts, ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_MATH_GetDiffPressureDataCounts");

    UtTest_Add(Test_ASPD4525_MATH_ByteFlip, ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_MATH_ByteFlip");

    UtTest_Add(Test_ASPD4525_MATH_PackOutGoingData, ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_MATH_PackOutGoingData");
}

