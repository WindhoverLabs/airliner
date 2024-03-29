
/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe_tbl_filedef.h"
#include "fpc_tbldefs.h"
#include "fpc_mission_cfg.h"

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Definitions
*************************************************************************/
#define FPC_CONFIG_TABLENAME ("CONFIG_TBL")
/**
** \brief The cFE FPC config table definition.
**
** Content format: ObjName[64], TblName[38], Desc[32], TgtFileName[20], ObjSize
**    ObjName - variable name of config table, e.g., FPC_ConfigDefTbl[]
**    TblName - app's table name, e.g., FPC.CONFIG_TBL, where FPC is the same app name
**              used in cfe_es_startup.scr, and FPC_defConfigTbl is the same table
**              name passed in to CFE_TBL_Register()
**    Desc - description of table in string format
**    TgtFileName[20] - table file name, compiled as .tbl file extension
**    ObjSize - size of the entire table
**
*/
static OS_USED CFE_TBL_FileDef_t CFE_TBL_FileDef =
{
    "FPC_ConfigTbl", "FPC.CONFIG_TBL", "FPC default config table",
    "fpc_config.tbl", (sizeof(FPC_ConfigTbl_t))
};

/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/

/**
**  \brief Default FPC config table data
*/
/* Default FPC config table data */
FPC_ConfigTbl_t FPC_ConfigTbl =
{
    /**
     * L1_PERIOD
     *
     * This is the L1 distance and defines the tracking
     * point ahead of the aircraft its following.
     * A value of 18-25 seconds works for most aircraft. Shorten
     * slowly during tuning until response is sharp without oscillation.
     *
     * @unit s
     * @min 12.0
     * @max 50.0
     * @decimal 1
     * @increment 0.5
     * @group FW L1 Control
     */
    20.0f,

    /**
     * L1_DAMPING
     *
     * Damping factor for L1 control.
     *
     * @min 0.6
     * @max 0.9
     * @decimal 2
     * @increment 0.05
     * @group FW L1 Control
     */
    0.75f,

    /**
     * T_TIME_CONST
     * TECS time constant
     *
     * This is the time constant of the TECS control algorithm (in seconds).
     * Smaller values make it faster to respond, larger values make it slower
     * to respond.
     *
     * @unit s
     * @min 1.0
     * @max 10.0
     * @decimal 1
     * @increment 0.5
     * @group FW TECS
     */
    5.0f,

    /**
     * T_THRO_CONST
     * TECS Throttle time constant
     *
     * This is the time constant of the TECS throttle control algorithm (in seconds).
     * Smaller values make it faster to respond, larger values make it slower
     * to respond.
     *
     * @unit s
     * @min 1.0
     * @max 10.0
     * @decimal 1
     * @increment 0.5
     * @group FW TECS
     */
    8.0f,

    /**
     * T_SINK_MIN
     * Minimum descent rate
     *
     * This is the sink rate of the aircraft with the throttle
     * set to THR_MIN and flown at the same airspeed as used
     * to measure FW_T_CLMB_MAX.
     *
     * @unit m/s
     * @min 1.0
     * @max 5.0
     * @decimal 1
     * @increment 0.5
     * @group FW TECS
     */
    2.0f,

    /**
     * T_SINK_MAX
     * Maximum descent rate
     *
     * This sets the maximum descent rate that the controller will use.
     * If this value is too large, the aircraft can over-speed on descent.
     * This should be set to a value that can be achieved without
     * exceeding the lower pitch angle limit and without over-speeding
     * the aircraft.
     *
     * @unit m/s
     * @min 2.0
     * @max 15.0
     * @decimal 1
     * @increment 0.5
     * @group FW TECS
     */
    5.0f,

    /**
     * T_CLMB_MAX
     * Maximum climb rate
     *
     * This is the best climb rate that the aircraft can achieve with
     * the throttle set to THR_MAX and the airspeed set to the
     * default value. For electric aircraft make sure this number can be
     * achieved towards the end of flight when the battery voltage has reduced.
     * The setting of this parameter can be checked by commanding a positive
     * altitude change of 100m in loiter, RTL or guided mode. If the throttle
     * required to climb is close to THR_MAX and the aircraft is maintaining
     * airspeed, then this parameter is set correctly. If the airspeed starts
     * to reduce, then the parameter is set to high, and if the throttle
     * demand required to climb and maintain speed is noticeably less than
     * FW_THR_MAX, then either FW_T_CLMB_MAX should be increased or
     * FW_THR_MAX reduced.
     *
     * @unit m/s
     * @min 1.0
     * @max 15.0
     * @decimal 1
     * @increment 0.5
     * @group FW TECS
     */
    4.0f,


    /**
     * CLMBOUT_DIFF
     * Climbout Altitude difference
     *
     * If the altitude error exceeds this parameter, the system will climb out
     * with maximum throttle and minimum airspeed until it is closer than this
     * distance to the desired altitude. Mostly used for takeoff waypoints / modes.
     * Set to 0 to disable climbout mode (not recommended).
     *
     * @unit m
     * @min 0.0
     * @max 150.0
     * @decimal 1
     * @increment 0.5
     * @group FW L1 Control
     */
    10.0f,

    /**
     * T_HRATE_P
     * Height rate proportional factor
     *
     * @min 0.0
     * @max 1.0
     * @decimal 2
     * @increment 0.05
     * @group FW TECS
     */

    0.05f,

    /**
      * T_HRATE_FF
     * Height rate feed forward
     *
     * @min 0.0
     * @max 1.0
     * @decimal 2
     * @increment 0.05
     * @group FW TECS
     */

    0.8f,

    /**
      * T_HGT_NOISE_FILTER_COEFF
     * Height noise filter (1st order IIR) coefficient
     *
     * @min 0.0
     * @max 1.0
     * @decimal 2
     * @increment 0.05
     * @group FW TECS
     */

    0.1f,


    /**
     * T_SRATE_P
     * Speed rate P factor
     *
     * @min 0.0
     * @max 2.0
     * @decimal 2
     * @increment 0.01
     * @group FW TECS
     */

    0.02f,

    /**
     * T_THR_DAMP
     * Throttle damping factor
     *
     * This is the damping gain for the throttle demand loop.
     * Increase to add damping to correct for oscillations in speed and height.
     *
     * @min 0.0
     * @max 2.0
     * @decimal 1
     * @increment 0.1
     * @group FW TECS
     */

    0.5f,

    /**
      T_INTEG_GAIN
     * Integrator gain
     *
     * This is the integrator gain on the control loop.
     * Increasing this gain increases the speed at which speed
     * and height offsets are trimmed out, but reduces damping and
     * increases overshoot.
     *
     * @min 0.0
     * @max 2.0
     * @decimal 2
     * @increment 0.05
     * @group FW TECS
     */

    0.1f,

    /**
     T_VERT_ACC
     * Maximum vertical acceleration
     *
     * This is the maximum vertical acceleration (in m/s/s)
     * either up or down that the controller will use to correct speed
     * or height errors. The default value of 7 m/s/s (equivalent to +- 0.7 g)
     * allows for reasonably aggressive pitch changes if required to recover
     * from under-speed conditions.
     *
     * @unit m/s/s
     * @min 1.0
     * @max 10.0
     * @decimal 1
     * @increment 0.5
     * @group FW TECS
     */

    7.0f,

    /**
     * T_HGT_OMEGA
     * Complementary filter "omega" parameter for height
     *
     * This is the cross-over frequency (in radians/second) of the complementary
     * filter used to fuse vertical acceleration and barometric height to obtain
     * an estimate of height rate and height. Increasing this frequency weights
     * the solution more towards use of the barometer, whilst reducing it weights
     * the solution more towards use of the accelerometer data.
     *
     * @unit rad/s
     * @min 1.0
     * @max 10.0
     * @decimal 1
     * @increment 0.5
     * @group FW TECS
     */

    3.0f,

    /**
      * T_SPD_OMEGA
     * Complementary filter "omega" parameter for speed
     *
     * This is the cross-over frequency (in radians/second) of the complementary
     * filter used to fuse longitudinal acceleration and airspeed to obtain an
     * improved airspeed estimate. Increasing this frequency weights the solution
     * more towards use of the airspeed sensor, whilst reducing it weights the
     * solution more towards use of the accelerometer data.
     *
     * @unit rad/s
     * @min 1.0
     * @max 10.0
     * @decimal 1
     * @increment 0.5
     * @group FW TECS
     */

    2.0f,

    /**
     * T_RLL2THR
     * Roll -> Throttle feedforward
     *
     * Increasing this gain turn increases the amount of throttle that will
     * be used to compensate for the additional drag created by turning.
     * Ideally this should be set to  approximately 10 x the extra sink rate
     * in m/s created by a 45 degree bank turn. Increase this gain if
     * the aircraft initially loses energy in turns and reduce if the
     * aircraft initially gains energy in turns. Efficient high aspect-ratio
     * aircraft (eg powered sailplanes) can use a lower value, whereas
     * inefficient low aspect-ratio models (eg delta wings) can use a higher value.
     *
     * @min 0.0
     * @max 20.0
     * @decimal 1
     * @increment 0.5
     * @group FW TECS
     */

     15.0f,

    /**
     * T_SPDWEIGHT
     * Speed <--> Altitude priority
     *
     * This parameter adjusts the amount of weighting that the pitch control
     * applies to speed vs height errors. Setting it to 0.0 will cause the
     * pitch control to control height and ignore speed errors. This will
     * normally improve height accuracy but give larger airspeed errors.
     * Setting it to 2.0 will cause the pitch control loop to control speed
     * and ignore height errors. This will normally reduce airspeed errors,
     * but give larger height errors. The default value of 1.0 allows the pitch
     * control to simultaneously control height and speed.
     * Note to Glider Pilots - set this parameter to 2.0 (The glider will
     * adjust its pitch angle to maintain airspeed, ignoring changes in height).
     *
     * @min 0.0
     * @max 2.0
     * @decimal 1
     * @increment 1.0
     * @group FW TECS
     */

     1.0f,

    /**
     * T_PTCH_DAMP
     * Pitch damping factor
     *
     * This is the damping gain for the pitch demand loop. Increase to add
     * damping to correct for oscillations in height. The default value of 0.0
     * will work well provided the pitch to servo controller has been tuned
     * properly.
     *
     * @min 0.0
     * @max 2.0
     * @decimal 1
     * @increment 0.1
     * @group FW TECS
     */

    0.0f,

    /**
     * T_STE_RATE_ERROR_FILTER_COEFF
     * Specific Total Energy Rate Error Filter (1st order IIR) coefficient
     *
     * @min 0.0
     * @max 1.0
     * @decimal 1
     * @increment 0.1
     * @group FW TECS
     */

    0.2f,

    /**
     * AIRSPD_MIN
     * Minimum Airspeed
     *
     * If the airspeed falls below this value, the TECS controller will try to
     * increase airspeed more aggressively.
     *
     * @unit m/s
     * @min 0.0
     * @max 40
     * @decimal 1
     * @increment 0.5
     * @group FW TECS
     */
    10.0f,

    /**
     * AIRSPD_TRIM
     * Cruise Airspeed
     *
     * The fixed wing controller tries to fly at this airspeed.
     *
     * @unit m/s
     * @min 0.0
     * @max 40
     * @decimal 1
     * @increment 0.5
     * @group FW TECS
     */
    21.0f,

    /**
     * AIRSPD_MAX
     * Maximum Airspeed
     *
     * If the airspeed is above this value, the TECS controller will try to decrease
     * airspeed more aggressively.
     *
     * @unit m/s
     * @min 0.0
     * @max 40
     * @decimal 1
     * @increment 0.5
     * @group FW TECS
     */
    30.0f,

    /**
     * ARSP_MODE
     * Airspeed disabled.
     *
     * @group Fixedwing Position Control
     */
    FALSE,


    /**
     * P_LIM_MIN_RADIANS
     * Negative pitch limit
     *
     * The minimum negative pitch the controller will output.
     *
     * @unit deg
     * @min -60.0
     * @max 0.0
     * @decimal 1
     * @increment 0.5
     * @group FW L1 Control
     */
    DEG_TO_RADIANS(-20.0f),

    /**
     * P_LIM_MAX_RADIANS
     *
     * The maximum positive pitch the controller will output.
     *
     * @unit deg
     * @min 0.0
     * @max 60.0
     * @decimal 1
     * @increment 0.5
     * @group FW L1 Control
     */
    DEG_TO_RADIANS(20.0f),

    /**
     * R_LIM_RADIANS
     * Controller roll limit
     *
     * The maximum roll the controller will output.
     *
     * @unit deg
     * @min 35.0
     * @max 65.0
     * @decimal 1
     * @increment 0.5
     * @group FW L1 Control
     */
    DEG_TO_RADIANS(30.0f),

    /**
     * THR_MIN
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    0.0f,

    /**
     * THR_MAX
     * Minimum thrust in auto thrust control
     *
     * It's recommended to set it > 0 to avoid free fall with zero thrust.
     *
     * @unit norm
     * @min 0.05
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group Fixedwing Position Control
     */
    1.0f,

    /**
     * THR_IDLE
     * Idle throttle
     *
     * This is the minimum throttle while on the ground
     *
     * For aircraft with internal combustion engine this parameter should be set
     * above desired idle rpm.
     *
     * @unit norm
     * @min 0.0
     * @max 0.4
     * @decimal 2
     * @increment 0.01
     * @group FW L1 Control
     */
    0.0f,


    /**
     * THR_CRUISE
     * Cruise throttle
     *
     * This is the throttle setting required to achieve the desired cruise speed. Most airframes have a value of 0.5-0.7.
     *
     * @unit norm
     * @min 0.0
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group FW L1 Control
     */
    0.6f,

    /**
     * THR_SLEW_MAX
     *
     * Maximum slew rate for the commanded throttle
     *
     * @min 0.0
     * @max 1.0
     * @group FW L1 Control
     */
    0.0f,

    /**
     * THR_ALT_SCL
     * Scale throttle by pressure change
     *
     * Automatically adjust throttle to account for decreased air density at higher altitudes.
     * Start with a scale factor of 1.0 and adjust for different propulsion systems.
     *
     * When flying without airspeed sensor this will help to keep a constant performance over large altitude ranges.
     *
     * The default value of 0 will disable scaling.
     *
     * @min 0.0
     * @max 2.0
     * @decimal 1
     * @increment 0.1
     * @group FW L1 Control
     */
    0.0f,

    /**
     * MAN_R_MAX_RADIANS
     * Max manual roll
     *
     * Max roll for manual control in attitude stabilized mode
     *
     * @unit deg
     * @min 0.0
     * @max 90.0
     * @decimal 1
     * @increment 0.5
     * @group FW Attitude Control
     */

    DEG_TO_RADIANS(45.0f),

    /**
     * MAN_P_MAX_RADIANS
     * Max manual pitch
     *
     * Max pitch for manual control in attitude stabilized mode
     *
     * @unit deg
     * @min 0.0
     * @max 90.0
     * @decimal 1
     * @increment 0.5
     * @group FW Attitude Control
     */

    DEG_TO_RADIANS(45.0f),

    /**
     * RSP_OFF_RADIANS
     * Roll setpoint offset
     *
     * An airframe specific offset of the roll setpoint in degrees, the value is
     * added to the roll setpoint and should correspond to the typical cruise speed
     * of the airframe.
     *
     * @unit deg
     * @min -90.0
     * @max 90.0
     * @decimal 1
     * @increment 0.5
     * @group FW Attitude Control
     */
    DEG_TO_RADIANS(0.0f),

    /**
     * PSP_OFF_RADIANS
     * Pitch setpoint offset
     *
     * An airframe specific offset of the pitch setpoint in degrees, the value is
     * added to the pitch setpoint and should correspond to the typical cruise
     * speed of the airframe.
     *
     * @unit deg
     * @min -90.0
     * @max 90.0
     * @decimal 1
     * @increment 0.5
     * @group FW Attitude Control
     */
    DEG_TO_RADIANS(0.0f),

    /**
     * THR_LND_MAX
     * Throttle limit value before flare
     *
     * This throttle value will be set as throttle limit at FW_LND_TLALT,
     * before aircraft will flare.
     *
     * @unit norm
     * @min 0.0
     * @max 1.0
     * @decimal 2
     * @increment 0.01
     * @group FW L1 Control
     */

    1.0f,

    /**
     * LND_ANG_RADIANS
     * Landing slope angle in degrees
     *
     * @unit deg
     * @min 1.0
     * @max 15.0
     * @decimal 1
     * @increment 0.5
     * @group FW L1 Control
     */
    DEG_TO_RADIANS(3.0f),

    /**
     * LND_HVIRT
     * This is "h1" here:https://github.com/WindhoverLabs/airliner/blob/integration/apps/fpc/docs/images/fw_landing.png
     * @group Fixedwing Position Control
     */
    10.0f,

    /**
     * LND_FLALT
     * Landing flare altitude (relative to landing altitude)
     *
     * NOTE: max(FW_LND_FLALT, FW_LND_FL_TIME * |z-velocity|) is taken as the flare altitude
     *
     * @unit m
     * @min 0.0
     * @decimal 1
     * @increment 0.5
     * @group FW L1 Control
     */
    3.0f,

    /**
     * LND_TLALT
     * Landing throttle limit altitude (relative landing altitude)
     *
     * Default of -1.0 lets the system default to applying throttle
     * limiting at 2/3 of the flare altitude.
     *
     * @unit m
     * @min -1.0
     * @max 30.0
     * @decimal 1
     * @increment 0.5
     * @group FW L1 Control
     */
    -1.0f,

    /**
      LND_HHDIST
     * Landing heading hold horizontal distance.
     * Set to 0 to disable heading hold.
     *
     * @unit m
     * @min 0
     * @max 30.0
     * @decimal 1
     * @increment 0.5
     * @group FW L1 Control
     */
    0.5f,

    /**
     * LND_FL_PMIN_RADIANS
     * Flare, minimum pitch
     *
     * Minimum pitch during flare, a positive sign means nose up
     * Applied once FW_LND_TLALT is reached
     *
     * @unit deg
     * @min 0
     * @max 15.0
     * @decimal 1
     * @increment 0.5
     * @group FW L1 Control
     */
    DEG_TO_RADIANS(2.5f),

    /**
     * LND_FL_PMAX_RADIANS
     * Flare, maximum pitch
     *
     * Maximum pitch during flare, a positive sign means nose up
     * Applied once FW_LND_TLALT is reached
     *
     * @unit deg
     * @min 0
     * @max 45.0
     * @decimal 1
     * @increment 0.5
     * @group FW L1 Control
     */
    DEG_TO_RADIANS(10.0f),

    /**
     * LND_USETER
     * Use terrain estimate during landing
     *
     * @boolean
     * @group FW L1 Control
     */
    0.0f,

    /**
     * LND_AIRSPD_SC
     * Min. airspeed scaling factor for landing
     *
     * Multiplying this factor with the minimum airspeed of the plane
     * gives the target airspeed the landing approach.
     * FW_AIRSPD_MIN * FW_LND_AIRSPD_SC
     *
     * @unit norm
     * @min 1.0
     * @max 1.5
     * @decimal 2
     * @increment 0.01
     * @group FW L1 Control
     */
    1.3f,


    {
        /**
         * @file launchdetection_params.c
         *
         * Parameters for launchdetection
         *
         * @author Thomas Gubler <thomasgubler@gmail.com>

        /*
         * Catapult launch detection parameters, accessible via MAVLink
         *

        /**LAUN_ALL_ON
         * Launch detection
         *
         * @boolean
         * @group FW Launch detection
         */
         0,

        /**
         * LAUN_CAT_A
         * Catapult accelerometer threshold.
         *
         * LAUN_CAT_A for LAUN_CAT_T serves as threshold to trigger launch detection.
         *
         * @unit m/s/s
         * @min 0
         * @decimal 1
         * @increment 0.5
         * @group FW Launch detection
         */
         30.0f,

        /**
         * Catapult time threshold.
         * LAUN_CAT_T
         * LAUN_CAT_A for LAUN_CAT_T serves as threshold to trigger launch detection.
         *
         * @unit s
         * @min 0.0
         * @max 5.0
         * @decimal 2
         * @increment 0.05
         * @group FW Launch detection
         */
         0.05f,

        /**
         * Motor delay
         * LAUN_CAT_MDEL
         * Delay between starting attitude control and powering up the throttle (giving throttle control to the controller)
         * Before this timespan is up the throttle will be set to FW_THR_IDLE, set to 0 to deactivate
         *
         * @unit s
         * @min 0.0
         * @max 10.0
         * @decimal 1
         * @increment 0.5
         * @group FW Launch detection
         */
         0,

        /**
         * LAUN_CAT_PMAX_RADIANS
         * Maximum pitch before the throttle is powered up (during motor delay phase)
         *
         * This is an extra limit for the maximum pitch which is imposed in the phase before the throttle turns on.
         * This allows to limit the maximum pitch angle during a bungee launch (make the launch less steep).
         *
         * @unit deg
         * @min 0.0
         * @max 45.0
         * @decimal 1
         * @increment 0.5
         * @group FW Launch detection
         */
        DEG_TO_RADIANS(30)
    },
    {
        /**
         * RWTO_TKOFF
         * Runway takeoff with landing gear
         *
         * @boolean
         * @group Runway Takeoff
         */
        TRUE,

        /**
         * RWTO_HDG
         * Specifies which heading should be held during runnway takeoff.
         *
         * 0: airframe heading, 1: heading towards takeoff waypoint
         *
         * @value 0 Airframe
         * @value 1 Waypoint
         * @min 0
         * @max 1
         * @group Runway Takeoff
         */
        0,
        /**
         * NAV_ALT
         * Altitude AGL at which we have enough ground clearance to allow some roll.
         * Until RWTO_NAV_ALT is reached the plane is held level and only
         * rudder is used to keep the heading (see RWTO_HDG). This should be below
         * FW_CLMBOUT_DIFF if FW_CLMBOUT_DIFF > 0.
         *
         * @unit m
         * @min 0.0
         * @max 100.0
         * @decimal 1
         * @increment 1
         * @group Runway Takeoff
         */
        5.0,
        /**
         * MAX_THR
         * Max throttle during runway takeoff.
         * (Can be used to test taxi on runway)
         *
         * @unit norm
         * @min 0.0
         * @max 1.0
         * @decimal 2
         * @increment 0.01
         * @group Runway Takeoff
         */
        1.0,
        /**
         * PSP_RADIANS
         * Pitch setpoint during taxi / before takeoff airspeed is reached.
         * A taildragger with stearable wheel might need to pitch up
         * a little to keep it's wheel on the ground before airspeed
         * to takeoff is reached.
         *
         * @unit deg
         * @min 0.0
         * @max 20.0
         * @decimal 1
         * @increment 0.5
         * @group Runway Takeoff
         */
        DEG_TO_RADIANS(0.0),
        /**
         * MAX_PITCH_RADIANS
         * Max pitch during takeoff.
         * Fixed-wing settings are used if set to 0. Note that there is also a minimum
         * pitch of 10 degrees during takeoff, so this must be larger if set.
         *
         * @unit deg
         * @min 0.0
         * @max 60.0
         * @decimal 1
         * @increment 0.5
         * @group Runway Takeoff
         */
        DEG_TO_RADIANS(20.0),
        /**
         MAX_ROLL_RADIANS
         * Max roll during climbout.
         * Roll is limited during climbout to ensure enough lift and prevents aggressive
         * navigation before we're on a safe height.
         *
         * @unit deg
         * @min 0.0
         * @max 60.0
         * @decimal 1
         * @increment 0.5
         * @group Runway Takeoff
         */
         DEG_TO_RADIANS(25),
        /**
         * AIRSPD_SCL
         * Min. airspeed scaling factor for takeoff.
         * Pitch up will be commanded when the following airspeed is reached:
         * FW_AIRSPD_MIN * RWTO_AIRSPD_SCL
         *
         * @unit norm
         * @min 0.0
         * @max 2.0
         * @decimal 2
         * @increment 0.01
         * @group Runway Takeoff
         */
         1.3,

        /**
         * RWTO_AIRSPD_MIN
         * Minimum Airspeed
         *
         * If the airspeed falls below this value, the TECS controller will try to
         * increase airspeed more aggressively.
         *
         * @unit m/s
         * @min 0.0
         * @max 40
         * @decimal 1
         * @increment 0.5
         * @group FW TECS
         */
        13.0,

        /**
         * RWTO_CLMBOUT_DIFF
         * Climbout Altitude difference
         *
         * If the altitude error exceeds this parameter, the system will climb out
         * with maximum throttle and minimum airspeed until it is closer than this
         * distance to the desired altitude. Mostly used for takeoff waypoints / modes.
         * Set to 0 to disable climbout mode (not recommended).
         *
         * @unit m
         * @min 0.0
         * @max 150.0
         * @decimal 1
         * @increment 0.5
         * @group FW L1 Control
         */
        10.0f
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
