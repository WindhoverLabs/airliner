#ifndef FPC_LANDING_SLOPE_H
#define FPC_LANDING_SLOPE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>

#include "math/Matrix3F3.hpp"
#include "math/Quaternion.hpp"
#include "math/Euler.hpp"
#include "math/Dcm.hpp"
#include "math/Expo.hpp"
#include "math/Functions.hpp"

/**
 * @brief The Landingslope class
 */
class Landingslope
{
private:
    /* see Documentation/fw_landing.png for a plot of the landing slope */
    float m_Landing_Slope_Angle_Rad
    {   0.0f}; /**< phi in the plot */
    float m_Flare_Relative_Alt
    {   0.0f}; /**< h_flare,rel in the plot */
    float m_Motor_Lim_Relative_Alt
    {   0.0f};
    float m_H1_Virt
    {   0.0f}; /**< H1 in the plot */
    float m_H0
    {   0.0f}; /**< h_flare,rel + H1 in the plot */
    float m_D1
    {   0.0f}; /**< d1 in the plot */
    float m_Flare_Constant
    {   0.0f};
    float m_Flare_Length
    {   0.0f}; /**< d1 + delta d in the plot */
    float m_Horizontal_Slope_Displacement
    {   0.0f}; /**< delta d in the plot */

    void calculateSlopeValues();

public:

    /**
     *
     * @return relative altitude of point on landing slope at distance to landing waypoint=wp_landing_distance
     */
    float getLandingSlopeRelativeAltitude(float wp_landing_distance);

    /**
     *
     * @return relative altitude of point on landing slope at distance to landing waypoint=wp_landing_distance
     * Performs check if aircraft is in front of waypoint to avoid climbout
     */
    float getLandingSlopeRelativeAltitudeSave(float wp_landing_distance, float bearing_lastwp_currwp,
            float bearing_airplane_currwp);

    /**
     *
     * @return Relative altitude of point on landing slope at distance to landing waypoint=wp_landing_distance
     */
    float getLandingSlopeRelativeAltitude(float wp_landing_distance, float horizontal_slope_displacement,
            float landing_slope_angle_rad)
    {
        // flare_relative_alt is negative
        return (wp_landing_distance - horizontal_slope_displacement) * tanf(landing_slope_angle_rad);
    }

    /**
*
    * @return Absolute altitude of point on landing slope at distance to landing waypoint=wp_landing_distance
    */
    float getLandingSlopeAbsoluteAltitude(float wp_landing_distance, float wp_landing_altitude,
            float horizontal_slope_displacement, float landing_slope_angle_rad)
    {
        return getLandingSlopeRelativeAltitude(wp_landing_distance, horizontal_slope_displacement,
                landing_slope_angle_rad) + wp_landing_altitude;
    }

    /**
     *
     * @return distance to landing waypoint of point on landing slope at altitude=slope_altitude
     */
    float getLandingSlopeWPDistance(float slope_altitude, float wp_landing_altitude,
            float horizontal_slope_displacement, float landing_slope_angle_rad)
    {
        return (slope_altitude - wp_landing_altitude) / tanf(landing_slope_angle_rad) + horizontal_slope_displacement;

    }

    float getFlareCurveRelativeAltitudeSave(float wp_landing_distance, float bearing_lastwp_currwp,
            float bearing_airplane_currwp);

    void update(float landing_slope_angle_rad_new,
            float flare_relative_alt_new,
            float motor_lim_relative_alt_new,
            float H1_virt_new);

    float landing_slope_angle_rad()
    {   return m_Landing_Slope_Angle_Rad;}
    float flare_relative_alt()
    {   return m_Flare_Relative_Alt;}
    float motor_lim_relative_alt()
    {   return m_Motor_Lim_Relative_Alt;}
    float flare_length()
    {   return m_Flare_Length;}
    float horizontal_slope_displacement()
    {   return m_Horizontal_Slope_Displacement;}

};

#ifdef __cplusplus
}
#endif

#endif /* FPC_LANDING_SLOPE_H */
