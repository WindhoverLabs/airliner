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

#define ASPD4525_MATH_PRESSURE_ACCURACY_LIMIT_PASCALS (((0.47)/2)+FLT_EPSILON)

void Test_ASPD4525_MATH_GetDeltaPressure(void)
{
    ASPD4525_ConfigTblEntry_t configTable;

    configTable.fPressureMaximum_PSI = 1.0;
    configTable.fPressureMinimum_PSI = 0.0;

    uint32 pressureDataCounts = 0x2000;
    float expected = 0.0;
    float result = ASPD4525_MATH_GetDeltaPressure((ASPD4525_ConfigTblEntry_t*)0, pressureDataCounts);
    char message[100];
    sprintf(message, "DeltaPressure should be zero because bad config, %f\n", result);
    UtAssert_DoubleCmpAbs(result, expected, FLT_EPSILON, message);

    result = ASPD4525_MATH_GetDeltaPressure(&configTable, pressureDataCounts);
    sprintf(message, "DeltaPressure should be zero, %f\n", result);
    UtAssert_DoubleCmpAbs(result, expected, ASPD4525_MATH_PRESSURE_ACCURACY_LIMIT_PASCALS, message);

    pressureDataCounts = 0x1fff;
    expected = 0.0;
    result = ASPD4525_MATH_GetDeltaPressure(&configTable, pressureDataCounts);
    sprintf(message, "DeltaPressure should be zero again, %f\n", result);
    UtAssert_DoubleCmpAbs(result, expected, ASPD4525_MATH_PRESSURE_ACCURACY_LIMIT_PASCALS, message);

    pressureDataCounts = 0x3ccc;
    expected = 3447.38;
    result = ASPD4525_MATH_GetDeltaPressure(&configTable, pressureDataCounts);
    sprintf(message, "DeltaPressure should be maximum, %f\n", result);
    UtAssert_DoubleCmpAbs(result, expected, ASPD4525_MATH_PRESSURE_ACCURACY_LIMIT_PASCALS, message);


    pressureDataCounts = 0x0333;
    expected = 3447.38;
    result = ASPD4525_MATH_GetDeltaPressure(&configTable, pressureDataCounts);
    sprintf(message, "DeltaPressure should be minimum, %f\n", result);
    UtAssert_DoubleCmpAbs(result, expected, ASPD4525_MATH_PRESSURE_ACCURACY_LIMIT_PASCALS, message);

}

#define ASPD4525_MATH_TEMPERATURE_ACCURACY_LIMIT_CELSIUS  ((200.0/2046.0)+FLT_EPSILON)

void Test_ASPD4525_MATH_GetTemperature(void)
{
    ASPD4525_ConfigTblEntry_t configTable;

    configTable.fTemperatureMaximum_Celcius = 150.0;
    configTable.fTemperatureMinimum_Celcius = -50.0;

    uint32 temperatureCounts = 0x0000;
    float expected = 0.0;
    float result = ASPD4525_MATH_GetTemperature((ASPD4525_ConfigTblEntry_t*) 0, temperatureCounts);
    char message[100];
    sprintf(message, "Temperature should be 0 because bad config table pointer, %f\n", result);
    UtAssert_DoubleCmpAbs(result, expected, FLT_EPSILON, message);

    expected = -50.0;
    result = ASPD4525_MATH_GetTemperature(&configTable, temperatureCounts);
    sprintf(message, "Temperature should be %f, %f\n",expected, result);
    UtAssert_DoubleCmpAbs(result, expected, ASPD4525_MATH_TEMPERATURE_ACCURACY_LIMIT_CELSIUS, message);

    temperatureCounts = 0x01ff;
    expected = 0.0;
    result = ASPD4525_MATH_GetTemperature(&configTable, temperatureCounts);
    sprintf(message, "Temperature should be %f, %f\n",expected, result);
    UtAssert_DoubleCmpAbs(result, expected, ASPD4525_MATH_TEMPERATURE_ACCURACY_LIMIT_CELSIUS, message);

    temperatureCounts = 0x0266;
    expected = 10.0;
    result = ASPD4525_MATH_GetTemperature(&configTable, temperatureCounts);
    sprintf(message, "Temperature should be %f, %f\n",expected, result);
    UtAssert_DoubleCmpAbs(result, expected, ASPD4525_MATH_TEMPERATURE_ACCURACY_LIMIT_CELSIUS, message);

    temperatureCounts = 0x02ff;
    expected = 25.0;
    result = ASPD4525_MATH_GetTemperature(&configTable, temperatureCounts);
    sprintf(message, "Temperature should be %f, %f\n",expected, result);
    UtAssert_DoubleCmpAbs(result, expected, ASPD4525_MATH_TEMPERATURE_ACCURACY_LIMIT_CELSIUS, message);

    temperatureCounts = 0x03ff;
    expected = 50.0;
    result = ASPD4525_MATH_GetTemperature(&configTable, temperatureCounts);
    sprintf(message, "Temperature should be %f, %f\n",expected, result);
    UtAssert_DoubleCmpAbs(result, expected, ASPD4525_MATH_TEMPERATURE_ACCURACY_LIMIT_CELSIUS, message);

    temperatureCounts = 0x0565;
    expected = 85.0;
    result = ASPD4525_MATH_GetTemperature(&configTable, temperatureCounts);
    sprintf(message, "Temperature should be %f, %f\n",expected, result);
    UtAssert_DoubleCmpAbs(result, expected, ASPD4525_MATH_TEMPERATURE_ACCURACY_LIMIT_CELSIUS, message);
}

void ASPD4525_MATH_Test_AddTestCases(void)
{
    UtTest_Add(Test_ASPD4525_MATH_GetDeltaPressure, ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_MATH_GetDeltaPressure");

    UtTest_Add(Test_ASPD4525_MATH_GetTemperature, ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_MATH_GetTemperature");
}

