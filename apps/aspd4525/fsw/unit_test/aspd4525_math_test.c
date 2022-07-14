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

#define ASPD4525_MATH_PRESSURE_ACCURACY_LIMIT_PASCALS (((0.47)/2)+FLT_EPSILON)

void Test_ASPD4525_MATH_GetDeltaPressure(void)
{
    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        uint32 pressureDataCounts = 0x2000;
        float expected = 0.0;
        float result;

        configTable.fPressureMaximum_PSI = 0.5;
        configTable.fPressureMinimum_PSI = -0.5;

        result = ASPD4525_MATH_GetDeltaPressure((ASPD4525_ConfigTblEntry_t*)0, pressureDataCounts);
        sprintf(message, "DeltaPressure should be zero because bad config, %f\n", result);
        UtAssert_DoubleCmpAbs(result, expected, FLT_EPSILON, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        uint32 pressureDataCounts = 0x2000;
        float expected = 0.0;
        float result;

        configTable.fPressureMaximum_PSI = 0.5;
        configTable.fPressureMinimum_PSI = -0.5;

        result = ASPD4525_MATH_GetDeltaPressure(&configTable, pressureDataCounts);
        sprintf(message, "DeltaPressure should be zero, %f\n", result);
        UtAssert_DoubleCmpAbs(result, expected, ASPD4525_MATH_PRESSURE_ACCURACY_LIMIT_PASCALS, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        uint32 pressureDataCounts = 0x1fff;
        float expected = 0.0;
        float result;

        configTable.fPressureMaximum_PSI = 0.5;
        configTable.fPressureMinimum_PSI = -0.5;

        result = ASPD4525_MATH_GetDeltaPressure(&configTable, pressureDataCounts);
        sprintf(message, "DeltaPressure should be zero again, %f\n", result);
        UtAssert_DoubleCmpAbs(result, expected, ASPD4525_MATH_PRESSURE_ACCURACY_LIMIT_PASCALS, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        uint32 pressureDataCounts = 0x3ccc;
        float expected = 3447.38;
        float result;

        configTable.fPressureMaximum_PSI = 0.5;
        configTable.fPressureMinimum_PSI = -0.5;

        result = ASPD4525_MATH_GetDeltaPressure(&configTable, pressureDataCounts);
        sprintf(message, "DeltaPressure should be maximum, %f\n", result);
        UtAssert_DoubleCmpAbs(result, expected, ASPD4525_MATH_PRESSURE_ACCURACY_LIMIT_PASCALS, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        uint32 pressureDataCounts = 0x0333;
        float expected = 3447.38;
        float result;

        configTable.fPressureMaximum_PSI = 0.5;
        configTable.fPressureMinimum_PSI = -0.5;

        result = ASPD4525_MATH_GetDeltaPressure(&configTable, pressureDataCounts);
        sprintf(message, "DeltaPressure should be minimum, %f\n", result);
        UtAssert_DoubleCmpAbs(result, expected, ASPD4525_MATH_PRESSURE_ACCURACY_LIMIT_PASCALS, message);
    }
}

#define ASPD4525_MATH_TEMPERATURE_ACCURACY_LIMIT_CELSIUS  ((200.0/2046.0)+FLT_EPSILON)

void Test_ASPD4525_MATH_GetTemperature(void)
{
    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        uint32 temperatureCounts = 0x0000;
        float expected = 0.0;
        float result;

        configTable.fTemperatureMaximum_Celcius = 150.0;
        configTable.fTemperatureMinimum_Celcius = -50.0;

        result = ASPD4525_MATH_GetTemperature((ASPD4525_ConfigTblEntry_t*) 0, temperatureCounts);
        sprintf(message, "Temperature should be 0 because bad config table pointer, %f\n", result);
        UtAssert_DoubleCmpAbs(result, expected, FLT_EPSILON, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        uint32 temperatureCounts = 0x0000;
        float expected = -50.0;
        float result;

        configTable.fTemperatureMaximum_Celcius = 150.0;
        configTable.fTemperatureMinimum_Celcius = -50.0;

        result = ASPD4525_MATH_GetTemperature(&configTable, temperatureCounts);
        sprintf(message, "Temperature should be %f, %f\n",expected, result);
        UtAssert_DoubleCmpAbs(result, expected, ASPD4525_MATH_TEMPERATURE_ACCURACY_LIMIT_CELSIUS, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        uint32 temperatureCounts = 0x01ff;
        float expected = 0.0;
        float result;

        configTable.fTemperatureMaximum_Celcius = 150.0;
        configTable.fTemperatureMinimum_Celcius = -50.0;

        result = ASPD4525_MATH_GetTemperature(&configTable, temperatureCounts);
        sprintf(message, "Temperature should be %f, %f\n",expected, result);
        UtAssert_DoubleCmpAbs(result, expected, ASPD4525_MATH_TEMPERATURE_ACCURACY_LIMIT_CELSIUS, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        uint32 temperatureCounts = 0x0266;
        float expected = 10.0;
        float result;

        configTable.fTemperatureMaximum_Celcius = 150.0;
        configTable.fTemperatureMinimum_Celcius = -50.0;

        result = ASPD4525_MATH_GetTemperature(&configTable, temperatureCounts);
        sprintf(message, "Temperature should be %f, %f\n",expected, result);
        UtAssert_DoubleCmpAbs(result, expected, ASPD4525_MATH_TEMPERATURE_ACCURACY_LIMIT_CELSIUS, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        uint32 temperatureCounts = 0x02ff;
        float expected = 25.0;
        float result;

        configTable.fTemperatureMaximum_Celcius = 150.0;
        configTable.fTemperatureMinimum_Celcius = -50.0;

        result = ASPD4525_MATH_GetTemperature(&configTable, temperatureCounts);
        sprintf(message, "Temperature should be %f, %f\n",expected, result);
        UtAssert_DoubleCmpAbs(result, expected, ASPD4525_MATH_TEMPERATURE_ACCURACY_LIMIT_CELSIUS, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        uint32 temperatureCounts = 0x03ff;
        float expected = 50.0;
        float result;

        configTable.fTemperatureMaximum_Celcius = 150.0;
        configTable.fTemperatureMinimum_Celcius = -50.0;

        result = ASPD4525_MATH_GetTemperature(&configTable, temperatureCounts);
        sprintf(message, "Temperature should be %f, %f\n",expected, result);
        UtAssert_DoubleCmpAbs(result, expected, ASPD4525_MATH_TEMPERATURE_ACCURACY_LIMIT_CELSIUS, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        uint32 temperatureCounts = 0x0565;
        float expected = 85.0;
        float result;

        configTable.fTemperatureMaximum_Celcius = 150.0;
        configTable.fTemperatureMinimum_Celcius = -50.0;

        result = ASPD4525_MATH_GetTemperature(&configTable, temperatureCounts);
        sprintf(message, "Temperature should be %f, %f\n",expected, result);
        UtAssert_DoubleCmpAbs(result, expected, ASPD4525_MATH_TEMPERATURE_ACCURACY_LIMIT_CELSIUS, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        uint32 temperatureCounts = 0x07ff;
        float expected = 150.0;
        float result;

        configTable.fTemperatureMaximum_Celcius = 150.0;
        configTable.fTemperatureMinimum_Celcius = -50.0;

        result = ASPD4525_MATH_GetTemperature(&configTable, temperatureCounts);
        sprintf(message, "Temperature should be %f, %f\n",expected, result);
        UtAssert_DoubleCmpAbs(result, expected, ASPD4525_MATH_TEMPERATURE_ACCURACY_LIMIT_CELSIUS, message);
    }
}

void Test_ASPD4525_MATH_CalibrateAirSpeedPressures(void)
{
    {
        ASPD4525_ConfigTblEntry_t configTable; 
        ASPD4525_LabCalibArgCmd_t labCalibArgCmd;
        char message[100];
        float result;
        float expectedMax, expectedMin;
        labCalibArgCmd.uPCountLow=0x2000;
        labCalibArgCmd.uPCountHigh=0x3000;
        labCalibArgCmd.fVelocityLow_SI=0.0;
        labCalibArgCmd.fVelocityHigh_SI=50.0;

        expectedMax = configTable.fPressureMaximum_PSI;
        expectedMin = configTable.fPressureMinimum_PSI;

        ASPD4525_MATH_CalibrateAirSpeedPressures((ASPD4525_ConfigTblEntry_t*)0, &labCalibArgCmd);
        result = configTable.fPressureMaximum_PSI;
        sprintf(message, "Pressure Max should be %f, %.9f\n",expectedMax, result);
        UtAssert_DoubleCmpAbs(result, expectedMax, FLT_EPSILON, message);

        result = configTable.fPressureMinimum_PSI;
        sprintf(message, "Pressure Min should be %f, %.9f\n",expectedMin, result);
        UtAssert_DoubleCmpAbs(result, expectedMin, FLT_EPSILON, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable; 
        ASPD4525_LabCalibArgCmd_t labCalibArgCmd;
        char message[100];
        float result;
        float expectedMax, expectedMin;
        labCalibArgCmd.uPCountLow=0x2000;
        labCalibArgCmd.uPCountHigh=0x3000;
        labCalibArgCmd.fVelocityLow_SI=0.0;
        labCalibArgCmd.fVelocityHigh_SI=50.0;

        expectedMax = configTable.fPressureMaximum_PSI;
        expectedMin = configTable.fPressureMinimum_PSI;

        ASPD4525_MATH_CalibrateAirSpeedPressures(&configTable, (ASPD4525_LabCalibArgCmd_t*)0);
        result = configTable.fPressureMaximum_PSI;
        sprintf(message, "Pressure Max should be %f, %.9f\n",expectedMax, result);
        UtAssert_DoubleCmpAbs(result, expectedMax, FLT_EPSILON, message);

        result = configTable.fPressureMinimum_PSI;
        sprintf(message, "Pressure Min should be %f, %.9f\n",expectedMin, result);
        UtAssert_DoubleCmpAbs(result, expectedMin, FLT_EPSILON, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable; 
        ASPD4525_LabCalibArgCmd_t labCalibArgCmd;
        char message[100];
        float result;
        float expected;
        labCalibArgCmd.uPCountLow=0x2000;
        labCalibArgCmd.uPCountHigh=0x3000;
        labCalibArgCmd.fVelocityLow_SI=0.0;
        labCalibArgCmd.fVelocityHigh_SI=50.0;

        ASPD4525_MATH_CalibrateAirSpeedPressures(&configTable, &labCalibArgCmd);
        result = configTable.fPressureMaximum_PSI;
        expected = 0.399708629;
        sprintf(message, "Pressure Max should be %f, %.9f\n",expected, result);
        UtAssert_DoubleCmpAbs(result, expected, FLT_EPSILON, message);

        result = configTable.fPressureMinimum_PSI;
        expected = -0.399762839;
        sprintf(message, "Pressure Min should be %f, %.9f\n",expected, result);
        UtAssert_DoubleCmpAbs(result, expected, FLT_EPSILON, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable; 
        ASPD4525_LabCalibArgCmd_t labCalibArgCmd;
        char message[100];
        float result;
        float expected;
        labCalibArgCmd.uPCountLow=0x1e00;
        labCalibArgCmd.uPCountHigh=0x3333;
        labCalibArgCmd.fVelocityLow_SI=0.0;
        labCalibArgCmd.fVelocityHigh_SI=50.0;

        ASPD4525_MATH_CalibrateAirSpeedPressures(&configTable, &labCalibArgCmd);
        result = configTable.fPressureMaximum_PSI;
        expected = 0.322630584;
        sprintf(message, "Pressure Max should be %f, %.9f\n",expected, result);
        UtAssert_DoubleCmpAbs(result, expected, FLT_EPSILON, message);

        result = configTable.fPressureMinimum_PSI;
        expected = -0.280766368;
        sprintf(message, "Pressure Min should be %f, %.9f\n",expected, result);
        UtAssert_DoubleCmpAbs(result, expected, FLT_EPSILON, message);
    }

}

void ASPD4525_MATH_Test_AddTestCases(void)
{
    UtTest_Add(Test_ASPD4525_MATH_GetDeltaPressure, ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_MATH_GetDeltaPressure");

    UtTest_Add(Test_ASPD4525_MATH_GetTemperature, ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_MATH_GetTemperature");

    UtTest_Add(Test_ASPD4525_MATH_CalibrateAirSpeedPressures, ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_MATH_CalibrateAirSpeedPressures");

}

