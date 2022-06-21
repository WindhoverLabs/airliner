
/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe_tbl_filedef.h"
#include "aspd4525_tbldefs.h"
#include "aspd4525_config.h"

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Definitions
*************************************************************************/

/**
** \brief The cFE ASPD4525 config table definition.
**
** Content format: ObjName[64], TblName[38], Desc[32], TgtFileName[20], ObjSize
**    ObjName - variable name of config table, e.g., ASPD4525_ConfigDefTbl[]
**    TblName - app's table name, e.g., ASPD4525.CONFIG_TBL, where ASPD4525 is the same app name
**              used in cfe_es_startup.scr, and ASPD4525_defConfigTbl is the same table
**              name passed in to CFE_TBL_Register()
**    Desc - description of table in string format
**    TgtFileName[20] - table file name, compiled as .tbl file extension
**    ObjSize - size of the entire table
**
*/
static OS_USED CFE_TBL_FileDef_t CFE_TBL_FileDef =
{
    "ASPD4525_ConfigTbl", "ASPD4525.CONFIG_TBL", "ASPD4525 default config table",
    "aspd4525_config.tbl", (sizeof(ASPD4525_ConfigTblEntry_t) * ASPD4525_CONFIG_TABLE_MAX_ENTRIES)
};

/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/

/**
**  \brief Default ASPD4525 config table data
*/
ASPD4525_ConfigTblEntry_t ASPD4525_ConfigTbl[ASPD4525_CONFIG_TABLE_MAX_ENTRIES] =
{
    /* Entry 1 */
    {
        .fPressureMinimum_PSI = 0.0,
        .fPressureMaximum_PSI = 1.0,
        .fTemperatureMinimum_Celcius = -50.0,
        .fTemperatureMaximum_Celcius = 150.0,
        .fAirGasConstantR_SI = 8.3144598,
        .fGravitationalAccereleration_SI = 9.80665,
        .fAirMolarMass_SI = 0.0289644,
        .fAltitudeMeters_bs = {0.0, 11000.0, 20000.0, 32000.0, 47000.0, 51000.0, 71000.0},
        .fRho_bs = {1.2250, 0.36391, 0.08803, 0.01322, 0.00143, 0.00086, 0.000064},
        .fTemp_bs = {288.15, 216.65, 216.65, 228.65, 270.65, 270.65, 214.65},
        .fLapseRate_bs = {-0.0065, 0.0, 0.001, 0.0028, 0.0, -0.0028, -0.002},
        .uEquationNo_bs = {1,2,1,1,2,1,1},
        .uAirDensityCalculationMode = ASPD4525_CONFIG_AIRDENSITY_ALTITUDE_MODE
    }
};

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Prototypes
*************************************************************************/

/************************************************************************
** Function Definitions
*************************************************************************/

/************************/
/*  End of File Comment */
/************************/
    
