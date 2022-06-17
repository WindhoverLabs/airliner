/**
 * @file aspd4525_atm_test.c
 * @author Shahed Rahim (srahim@windhoverlabs.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "aspd4525_test_utils.h"
#include "aspd4525_atm.h"

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

#define ACCEPTED_FLT_RATIO (0.0001)

void Test_ASPD4525_ATM_GetAirDensity(void)
{
    {
        float expected = 0.0;
        float result = 
            ASPD4525_ATM_GetAirDensity(
                (ASPD4525_ConfigTblEntry_t*)0, 
                25.0,
                101325.0
            );
        char message[100];
        sprintf(message, "AirDensity should be %f because bad config, %f\n", expected, result);
        UtAssert_DoubleCmpAbs(result, expected, FLT_EPSILON, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        float expected = 1.183893;
        float result;
        configTable.fAirGasConstantR_SI = 8.3144598;
        configTable.fAirMolarMass_SI = 0.0289644;

        result = 
            ASPD4525_ATM_GetAirDensity(
                &configTable, 
                25.0,
                101325.0
            );
        sprintf(message, "AirDensity should be %f, got %f\n",expected, result);
        UtAssert_DoubleCmpRel(result, expected, ACCEPTED_FLT_RATIO, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        float expected = 1.225;
        float result;
        configTable.fAirGasConstantR_SI = 8.3144598;
        configTable.fAirMolarMass_SI = 0.0289644;

        result = 
            ASPD4525_ATM_GetAirDensity(
                &configTable, 
                15.0,
                101325.0
            );
        sprintf(message, "AirDensity should be %f, got %f\n",expected, result);
        UtAssert_DoubleCmpRel(result, expected, ACCEPTED_FLT_RATIO, message);
    }
}

void ASPD_ATM_TEST_LoadConfigTable(ASPD4525_ConfigTblEntry_t* configTabP) {
    float fAltitudeMeters_bs[ASPD4525_NUM_ATM_LAYERS] = {0.0, 11000.0, 20000.0, 32000.0, 47000.0, 51000.0, 71000.0};
    float fRho_bs[ASPD4525_NUM_ATM_LAYERS] = {1.2250, 0.36391, 0.08803, 0.01322, 0.00143, 0.00086, 0.000064};
    float fTemp_bs[ASPD4525_NUM_ATM_LAYERS] = {288.15, 216.65, 216.65, 228.65, 270.65, 270.65, 214.65};
    float fLapseRate_bs[ASPD4525_NUM_ATM_LAYERS] = {-0.0065, 0.0, 0.001, 0.0028, 0.0, -0.0028, -0.002};
    uint8 uEquationNo_bs[ASPD4525_NUM_ATM_LAYERS] = {1,2,1,1,2,1,1};

    for (int i=0; i< ASPD4525_NUM_ATM_LAYERS; i++) {
        configTabP->fAltitudeMeters_bs[i] = fAltitudeMeters_bs[i];
        configTabP->fRho_bs[i] = fRho_bs[i];
        configTabP->fTemp_bs[i] = fTemp_bs[i];
        configTabP->fLapseRate_bs[i] = fLapseRate_bs[i];
        configTabP->uEquationNo_bs[i] = uEquationNo_bs[i];
    }
}

void Test_ASPD4525_ATM_GetAirDensityWithAlt(void)
{
    {
        float expected = 0.0;
        float result = 
            ASPD4525_ATM_GetAirDensityWithAlt(
                (ASPD4525_ConfigTblEntry_t*)0, 
                10.0
            );
        char message[100];
        sprintf(message, "AirDensity should be %f because bad config, %f\n", expected, result);
        UtAssert_DoubleCmpAbs(result, expected, FLT_EPSILON, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        float expected = 1.223824;
        float result;
        configTable.fAirGasConstantR_SI = 8.3144598;
        configTable.fAirMolarMass_SI = 0.0289644;
        configTable.fGravitationalAccereleration_SI = 9.80665;

        ASPD_ATM_TEST_LoadConfigTable(&configTable);

        result = 
            ASPD4525_ATM_GetAirDensityWithAlt(
                &configTable, 
                10.0
            );
        sprintf(message, "AirDensity should be %f, got %f\n",expected, result);
        UtAssert_DoubleCmpRel(result, expected, ACCEPTED_FLT_RATIO, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        float expected = 0.310088;
        float result;
        configTable.fAirGasConstantR_SI = 8.3144598;
        configTable.fAirMolarMass_SI = 0.0289644;
        configTable.fGravitationalAccereleration_SI = 9.80665;

        ASPD_ATM_TEST_LoadConfigTable(&configTable);

        result = 
            ASPD4525_ATM_GetAirDensityWithAlt(
                &configTable, 
                12015.0
            );
        sprintf(message, "AirDensity should be %f, got %f\n",expected, result);
        UtAssert_DoubleCmpRel(result, expected, ACCEPTED_FLT_RATIO, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        float expected = 0.063571;
        float result;
        configTable.fAirGasConstantR_SI = 8.3144598;
        configTable.fAirMolarMass_SI = 0.0289644;
        configTable.fGravitationalAccereleration_SI = 9.80665;

        ASPD_ATM_TEST_LoadConfigTable(&configTable);

        result = 
            ASPD4525_ATM_GetAirDensityWithAlt(
                &configTable, 
                22015.0
            );
        sprintf(message, "AirDensity should be %f, got %f\n",expected, result);
        UtAssert_DoubleCmpRel(result, expected, ACCEPTED_FLT_RATIO, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        float expected = 0.009583;
        float result;
        configTable.fAirGasConstantR_SI = 8.3144598;
        configTable.fAirMolarMass_SI = 0.0289644;
        configTable.fGravitationalAccereleration_SI = 9.80665;

        ASPD_ATM_TEST_LoadConfigTable(&configTable);

        result = 
            ASPD4525_ATM_GetAirDensityWithAlt(
                &configTable, 
                34015.0
            );
        sprintf(message, "AirDensity should be %f, got %f\n",expected, result);
        UtAssert_DoubleCmpRel(result, expected, ACCEPTED_FLT_RATIO, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        float expected = 0.001109;
        float result;
        configTable.fAirGasConstantR_SI = 8.3144598;
        configTable.fAirMolarMass_SI = 0.0289644;
        configTable.fGravitationalAccereleration_SI = 9.80665;

        ASPD_ATM_TEST_LoadConfigTable(&configTable);

        result = 
            ASPD4525_ATM_GetAirDensityWithAlt(
                &configTable, 
                49015.0
            );
        sprintf(message, "AirDensity should be %f, got %f\n",expected, result);
        UtAssert_DoubleCmpRel(result, expected, 10*ACCEPTED_FLT_RATIO, message);
    }

        {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        float expected = 0.000679;
        float result;
        configTable.fAirGasConstantR_SI = 8.3144598;
        configTable.fAirMolarMass_SI = 0.0289644;
        configTable.fGravitationalAccereleration_SI = 9.80665;

        ASPD_ATM_TEST_LoadConfigTable(&configTable);

        result = 
            ASPD4525_ATM_GetAirDensityWithAlt(
                &configTable, 
                53015.0
            );
        sprintf(message, "AirDensity should be %f, got %f\n",expected, result);
        UtAssert_DoubleCmpRel(result, expected, 10*ACCEPTED_FLT_RATIO, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        float expected = 0.000035;
        float result;
        configTable.fAirGasConstantR_SI = 8.3144598;
        configTable.fAirMolarMass_SI = 0.0289644;
        configTable.fGravitationalAccereleration_SI = 9.80665;

        ASPD_ATM_TEST_LoadConfigTable(&configTable);

        result = 
            ASPD4525_ATM_GetAirDensityWithAlt(
                &configTable, 
                75015.0
            );
        sprintf(message, "AirDensity should be %f, got %f\n",expected, result);
        UtAssert_DoubleCmpRel(result, expected, 100*ACCEPTED_FLT_RATIO, message);
    }

}

void Test_ASPD4525_ATM_GetAirDensityWithAltTemp(void)
{
    {
        float expected = 0.0;
        float result = 
            ASPD4525_ATM_GetAirDensityWithAltTemp(
                (ASPD4525_ConfigTblEntry_t*)0, 
                10.0,
                25.0
            );
        char message[100];
        sprintf(message, "AirDensity should be %f because bad config, %f\n", expected, result);
        UtAssert_DoubleCmpAbs(result, expected, FLT_EPSILON, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        float expected = 1.225;
        float result;
        configTable.fAirGasConstantR_SI = 8.3144598;
        configTable.fAirMolarMass_SI = 0.0289644;
        configTable.fGravitationalAccereleration_SI = 9.80665;

        ASPD_ATM_TEST_LoadConfigTable(&configTable);

        result = 
            ASPD4525_ATM_GetAirDensityWithAltTemp(
                &configTable, 
                10.0,
                15
            );
        sprintf(message, "AirDensity should be %f, got %f\n",expected, result);
        UtAssert_DoubleCmpRel(result, expected, ACCEPTED_FLT_RATIO, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        float expected = 0.310088;
        float result;
        configTable.fAirGasConstantR_SI = 8.3144598;
        configTable.fAirMolarMass_SI = 0.0289644;
        configTable.fGravitationalAccereleration_SI = 9.80665;

        ASPD_ATM_TEST_LoadConfigTable(&configTable);

        result = 
            ASPD4525_ATM_GetAirDensityWithAltTemp(
                &configTable, 
                12015.0,
                -56.5
            );
        sprintf(message, "AirDensity should be %f, got %f\n",expected, result);
        UtAssert_DoubleCmpRel(result, expected, ACCEPTED_FLT_RATIO, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        float expected = 0.063724;
        float result;
        configTable.fAirGasConstantR_SI = 8.3144598;
        configTable.fAirMolarMass_SI = 0.0289644;
        configTable.fGravitationalAccereleration_SI = 9.80665;

        ASPD_ATM_TEST_LoadConfigTable(&configTable);

        result = 
            ASPD4525_ATM_GetAirDensityWithAltTemp(
                &configTable, 
                22015.0,
                -54.5
            );
        sprintf(message, "AirDensity should be %f, got %f\n",expected, result);
        UtAssert_DoubleCmpRel(result, expected, ACCEPTED_FLT_RATIO, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        float expected = 0.009660;
        float result;
        configTable.fAirGasConstantR_SI = 8.3144598;
        configTable.fAirMolarMass_SI = 0.0289644;
        configTable.fGravitationalAccereleration_SI = 9.80665;

        ASPD_ATM_TEST_LoadConfigTable(&configTable);

        result = 
            ASPD4525_ATM_GetAirDensityWithAltTemp(
                &configTable, 
                34015.0,
                -39.0
            );
        sprintf(message, "AirDensity should be %f, got %f\n",expected, result);
        UtAssert_DoubleCmpRel(result, expected, ACCEPTED_FLT_RATIO, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        float expected = 0.001109;
        float result;
        configTable.fAirGasConstantR_SI = 8.3144598;
        configTable.fAirMolarMass_SI = 0.0289644;
        configTable.fGravitationalAccereleration_SI = 9.80665;

        ASPD_ATM_TEST_LoadConfigTable(&configTable);

        result = 
            ASPD4525_ATM_GetAirDensityWithAltTemp(
                &configTable, 
                49015.0,
                -2.5
            );
        sprintf(message, "AirDensity should be %f, got %f\n",expected, result);
        UtAssert_DoubleCmpRel(result, expected, 10*ACCEPTED_FLT_RATIO, message);
    }

        {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        float expected = 0.000775;
        float result;
        configTable.fAirGasConstantR_SI = 8.3144598;
        configTable.fAirMolarMass_SI = 0.0289644;
        configTable.fGravitationalAccereleration_SI = 9.80665;

        ASPD_ATM_TEST_LoadConfigTable(&configTable);

        result = 
            ASPD4525_ATM_GetAirDensityWithAltTemp(
                &configTable, 
                53015.0,
                -5.0
            );
        sprintf(message, "AirDensity should be %f, got %f\n",expected, result);
        UtAssert_DoubleCmpRel(result, expected, 10*ACCEPTED_FLT_RATIO, message);
    }

    {
        ASPD4525_ConfigTblEntry_t configTable;
        char message[100];
        float expected = 0.000035;
        float result;
        configTable.fAirGasConstantR_SI = 8.3144598;
        configTable.fAirMolarMass_SI = 0.0289644;
        configTable.fGravitationalAccereleration_SI = 9.80665;

        ASPD_ATM_TEST_LoadConfigTable(&configTable);

        result = 
            ASPD4525_ATM_GetAirDensityWithAltTemp(
                &configTable, 
                75015.0,
                -66.5
            );
        sprintf(message, "AirDensity should be %f, got %f\n",expected, result);
        UtAssert_DoubleCmpRel(result, expected, 100*ACCEPTED_FLT_RATIO, message);
    }

}

void Test_ASPD4525_ATM_GetAirSpeed(void)
{
    {
        float fDeltaPressure = 3447.38;
        float fAirDensity = 1.225;
        float expected = 75.022499;
        float result = ASPD4525_ATM_GetAirSpeed(fDeltaPressure, fAirDensity);
        char message[100];
        sprintf(message, "AirSpeed should be %f, got %f\n",expected, result);
        UtAssert_DoubleCmpRel(result, expected, ACCEPTED_FLT_RATIO, message);
    }

    {
        float fDeltaPressure = 15.0;
        float fAirDensity = 0.225;
        float expected = 11.547006;
        float result = ASPD4525_ATM_GetAirSpeed(fDeltaPressure, fAirDensity);
        char message[100];
        sprintf(message, "AirSpeed should be %f, got %f\n",expected, result);
        UtAssert_DoubleCmpRel(result, expected, ACCEPTED_FLT_RATIO, message);
    }

    {
        float fDeltaPressure = 3447.38;
        float fAirDensity = 0.0005;
        float expected = 3713.424072;
        float result = ASPD4525_ATM_GetAirSpeed(fDeltaPressure, fAirDensity);
        char message[100];
        sprintf(message, "AirSpeed should be %f, got %f\n",expected, result);
        UtAssert_DoubleCmpRel(result, expected, ACCEPTED_FLT_RATIO, message);
    }

    {
        float fDeltaPressure = 3447.38;
        float fAirDensity = 0.310088;
        float expected = 149.113541;
        float result = ASPD4525_ATM_GetAirSpeed(fDeltaPressure, fAirDensity);
        char message[100];
        sprintf(message, "AirSpeed should be %f, got %f\n",expected, result);
        UtAssert_DoubleCmpRel(result, expected, ACCEPTED_FLT_RATIO, message);
    }
}

void ASPD4525_ATM_Test_AddTestCases(void)
{
    UtTest_Add(Test_ASPD4525_ATM_GetAirDensity, ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_ATM_GetAirDensity");

    UtTest_Add(Test_ASPD4525_ATM_GetAirDensityWithAlt, ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_ATM_GetAirDensityWithAlt");

    UtTest_Add(Test_ASPD4525_ATM_GetAirDensityWithAltTemp, ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_ATM_GetAirDensityWithAltTemp");
    
    UtTest_Add(Test_ASPD4525_ATM_GetAirSpeed, ASPD4525_Test_Setup, ASPD4525_Test_TearDown,
               "Test_ASPD4525_ATM_GetAirSpeed");
}

