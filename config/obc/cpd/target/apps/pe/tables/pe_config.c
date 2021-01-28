/*
** Pragmas
*/

/*
** Include Files
*/
#include "cfe_tbl_filedef.h"
#include "pe_tbldefs.h"

/*
** Local Defines
*/

/*
** Local Structure Declarations
*/
static OS_USED CFE_TBL_FileDef_t CFE_TBL_FileDef =
{
    /* Content format: ObjName[64], TblName[38], Desc[32], TgtFileName[20], ObjSize
    **    ObjName - variable name of config table, e.g., CI_ConfigDefTbl[]
    **    TblName - app's table name, e.g., CI.CONFIG_TBL, where CI is the same app name
    **              used in cfe_es_startup.scr, and CI_defConfigTbl is the same table
    **              name passed in to CFE_TBL_Register()
    **    Desc - description of table in string format
    **    TgtFileName[20] - table file name, compiled as .tbl file extension
    **    ObjSize - size of the entire table
    */

    "PE_ConfigTbl", "PE.CONFIG_TBL", "PE default config table",
    "pe_config.tbl", (sizeof(PE_ConfigTbl_t))
};

/*
** External Global Variables
*/

/*
** Global Variables
*/

/* Default ULR config table data */
PE_ConfigTbl_t PE_ConfigTbl =
{
	//VXY_PUB_THRESH
    .3f,
	//Z_PUB_THRESH
    1.0f,
	//ACCEL_XY_STDDEV
    .012f,
	//ACCEL_Z_STDDEV
    .02f,
	//BARO_FUSE
    1,
	//BARO_STDDEV
    3.0f,
	//GPS_FUSE
    1,
	//GPS_DELAY
    .29f,
	//GPS_XY_STDDEV
    1.0f,
	//GPS_Z_STDDEV
    3.0f,
	//GPS_VXY_STDDEV
    .25f,
	//GPS_VZ_STDDEV
    .25f,
	//GPS_EPH_MAX
    3.0f,
	//GPS_EPV_MAX
    5.0f,
	//LAND_FUSE
    1,
	//LAND_Z_STDDEV
    .03f,
	//LAND_VXY_STDDEV
    .05f,
	//PN_P_NOISE_DENSITY
    .1f,
	//PN_V_NOISE_DENSITY
    .1f,
	//PN_B_NOISE_DENSITY
    .001f,
	//PN_T_NOISE_DENSITY
    .001f,
	//T_MAX_GRADE
    0.0f,
	//FAKE_ORIGIN
    0,
	//INIT_ORIGIN_LAT
    29.562144f,
	//INIT_ORIGIN_LON
    -95.144554,
    //ULR_FUSE
    FALSE,
    //ULR_STDDEV
    .05f,
    //ULR_OFF_Z
    0.0f,
    // FLOW_FUSE
    FALSE,
    /**
     * Optical flow scale
     *
     * @group Local Position Estimator
     * @unit m
     * @min 0.1
     * @max 10.0
     * @decimal 3
     */
    1.3f,
    /**
     * Optical flow rotation (roll/pitch) noise gain
     *
     * @group Local Position Estimator
     * @unit m/s / (rad)
     * @min 0.1
     * @max 10.0
     * @decimal 3
     */
    7.0f,

    /**
     * Optical flow angular velocity noise gain
     *
     * @group Local Position Estimator
     * @unit m/s / (rad/s)
     * @min 0.0
     * @max 10.0
     * @decimal 3
     */
    7.0f,

    /**
     * Optical flow minimum quality threshold
     *
     * @group Local Position Estimator
     * @min 0
     * @max 255
     * @decimal 0
     */
    150,
    
    /**
     * Optical flow minimum AGL
     *
     * @group Local Position Estimator
     * @min 0.0
     * @max 10.0
     * @decimal 3
     */
    0.5f
};

/*
** Local Variables
*/

/*
** Function Prototypes
*/

/*
** Function Definitions
*/

/*=======================================================================================
** End of file pe_config.c
**=====================================================================================*/
    
