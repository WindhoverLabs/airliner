/****************************************************************************
 *
 *   Copyright (C) 2012, 2014 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file geo.h
 *
 * Definition of geo / math functions to perform geodesic calculations
 *
 * @author Thomas Gubler <thomasgubler@student.ethz.ch>
 * @author Julian Oes <joes@student.ethz.ch>
 * @author Lorenz Meier <lm@inf.ethz.ch>
 * @author Anton Babushkin <anton.babushkin@me.com>
 * Additional functions - @author Doug Weibel <douglas.weibel@colorado.edu>
 */

#ifndef GEO_H
#define GEO_H

#ifdef __cplusplus
extern "C" {
#endif


#include "cfe.h"
#include <stdbool.h>
#include "geo_lookup/geo_mag_declination.h"

#define CONSTANTS_ONE_G						    9.80665f		/* m/s^2		*/
#define CONSTANTS_AIR_DENSITY_SEA_LEVEL_15C		1.225f			/* kg/m^3		*/
#define CONSTANTS_AIR_GAS_CONST				    287.1f 			/* J/(kg * K)		*/
#define CONSTANTS_ABSOLUTE_NULL_CELSIUS			-273.15f		/* °C			*/
#define CONSTANTS_RADIUS_OF_EARTH			    (6371000)			/* meters (m)		*/

// XXX remove
struct crosstrack_error_s {
	bool past_end;		// Flag indicating we are past the end of the line/arc segment
	float distance;		// Distance in meters to closest point on line/arc
	float bearing;		// Bearing in radians to closest point on line/arc
} ;

/**
**  \brief Geo map projection struct
*/
struct map_projection_reference_s {
	/** \brief Latitude in radians */
	double lat_rad;

	/** \brief Longitude in radians */
	double lon_rad;

	/** \brief Sin of latitude */
	double sin_lat;

	/** \brief Cosine of latitude */
	double cos_lat;

	/** \brief Initialized flag */
	bool init_done;

	/** \brief Timestamp of current data */
	uint64 timestamp;
};

struct globallocal_converter_reference_s {
	float alt;
	bool init_done;
};

/************************************************************************/
/** \brief Projection Initialized
 **
 **  \par Description
 **       This function checks if global projection was initialized.
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **  \returns
 **  True if map was initialized before, false else
 **  \endreturns
 **
 *************************************************************************/
bool map_projection_global_initialized(void);

/************************************************************************/
/** \brief Projection Reference Initialized
 **
 **  \par Description
 **       This function checks if projection given as argument was initialized.
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **	 \param [in]   ref      The #map_projection_reference_s to check
 **
 **  \returns
 **  True if map was initialized before, false else
 **  \endreturns
 **
 *************************************************************************/
bool map_projection_initialized(const struct map_projection_reference_s *ref);

/************************************************************************/
/** \brief Get Timestamp
 **
 **  \par Description
 **       This function gets the timestamp of the global map projection.
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **  \returns
 **  The timestamp of the map_projection
 **  \endreturns
 **
 *************************************************************************/
uint64 map_projection_global_timestamp(void);

/************************************************************************/
/** \brief Get Reference Timestamp
 **
 **  \par Description
 **       This function gets the timestamp of the global map projection.
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **	 \param [in]   ref      The #map_projection_reference_s to check
 **
 **  \returns
 **  The timestamp of the map_projection
 **  \endreturns
 **
 *************************************************************************/
uint64 map_projection_timestamp(const struct map_projection_reference_s *ref);

/************************************************************************/
/** \brief Get Lat Lon
 **
 **  \par Description
 **       This function writes the reference values of the global projection
 **       to ref_lat and ref_lon
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **	 \param [in]   ref_lat_rad     Latitude
 **	 \param [in]   ref_lon_rad     Longitude
 **
 **  \returns
 **  0 if map_projection_init was called before, -1 else
 **  \endreturns
 **
 *************************************************************************/
int map_projection_global_reference(double *ref_lat_rad, double *ref_lon_rad);

/************************************************************************/
/** \brief Get Reference Lat Lon
 **
 **  \par Description
 **       This function writes the reference values of the projection
 **       given by the argument to ref_lat and ref_lon
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **	 \param [in]   ref		       #map_projection_reference_s to get lat lon
 **	 \param [in]   ref_lat_rad     Latitude
 **	 \param [in]   ref_lon_rad     Longitude
 **
 **  \returns
 **  0 if map_projection_init was called before, -1 else
 **  \endreturns
 **
 *************************************************************************/
int map_projection_reference(const struct map_projection_reference_s *ref, double *ref_lat_rad,
				      double *ref_lon_rad);

/************************************************************************/
/** \brief Initializes Global Map Transformation.
 **
 **  \par Description
 **       This function initializes the transformation between the geographic
 **       coordinate system and the azimuthal equidistant plane
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **	 \param [in]   lat_0    	 Latitude in degrees
 **	 \param [in]   lon_0     	 Longitude in degrees
 **	 \param [in]   timestamp     Timestamp
 **
 **  \returns
 **  0
 **  \endreturns
 **
 *************************************************************************/
int map_projection_global_init(double lat_0, double lon_0, uint64 timestamp);

/************************************************************************/
/** \brief Initializes Reference Map Transformation.
 **
 **  \par Description
 **       This function initializes the transformation between the geographic
 **       coordinate system and the azimuthal equidistant plane
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **	 \param [in]   ref    		 #map_projection_reference_s to init
 **	 \param [in]   lat_0    	 Latitude in degrees
 **	 \param [in]   lon_0     	 Longitude in degrees
 **	 \param [in]   timestamp     Timestamp
 **
 **  \returns
 **  0
 **  \endreturns
 **
 *************************************************************************/
int map_projection_init_timestamped(struct map_projection_reference_s *ref,
		double lat_0, double lon_0, uint64 timestamp);

/************************************************************************/
/** \brief Initializes Reference Map Transformation.
 **
 **  \par Description
 **       This function initializes the transformation between the geographic
 **       coordinate system and the azimuthal equidistant plane
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **	 \param [in]   ref    		 #map_projection_reference_s to init
 **	 \param [in]   lat_0    	 Latitude in degrees
 **	 \param [in]   lon_0     	 Longitude in degrees
 **	 \param [in]   timestamp     Timestamp
 **
 **  \returns
 **  0
 **  \endreturns
 **
 *************************************************************************/
int map_projection_init(struct map_projection_reference_s *ref, double lat_0, double lon_0, double time);

/************************************************************************/
/** \brief Project Global Map Transformation.
 **
 **  \par Description
 **       This function Transforms a point in the geographic coordinate
 **       system to the local azimuthal equidistant plane using the
 **       global projection
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **	 \param [in]   x    	 North
 **	 \param [in]   y    	 East
 **	 \param [in]   lat    	 Latitude in degrees
 **	 \param [in]   lon     	 Longitude in degrees
 **
 **  \returns
 **  0 if map_projection_init was called before, -1 else
 **  \endreturns
 **
 *************************************************************************/
int map_projection_global_project(double lat, double lon, float *x, float *y);

/************************************************************************/
/** \brief Project Reference Map Transformation.
 **
 **  \par Description
 **       This function Transforms a point in the geographic coordinate
 **       system to the local azimuthal equidistant plane using the
 **       global projection
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **	 \param [in]   ref    	 #map_projection_reference_s to project
 **	 \param [in]   x    	 North
 **	 \param [in]   y    	 East
 **	 \param [in]   lat    	 Latitude in degrees
 **	 \param [in]   lon     	 Longitude in degrees
 **
 **  \returns
 **  0 if map_projection_init was called before, -1 else
 **  \endreturns
 **
 *************************************************************************/
int map_projection_project(const struct map_projection_reference_s *ref, double lat, double lon, float *x,
				    float *y);

/************************************************************************/
/** \brief Reproject Global Map Transformation.
 **
 **  \par Description
 **       This function transforms a point in the local azimuthal
 **       equidistant plane to the geographic coordinate system
 **       using the global projection
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **	 \param [in]   x    	 North
 **	 \param [in]   y    	 East
 **	 \param [in]   lat    	 Latitude in degrees
 **	 \param [in]   lon     	 Longitude in degrees
 **
 **  \returns
 **  0 if map_projection_init was called before, -1 else
 **  \endreturns
 **
 *************************************************************************/
int map_projection_global_reproject(float x, float y, double *lat, double *lon);

/************************************************************************/
/** \brief Reproject Reference Map Transformation.
 **
 **  \par Description
 **       This function transforms a point in the local azimuthal
 **       equidistant plane to the geographic coordinate system
 **       using the global projection
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **	 \param [in]   x    	 North
 **	 \param [in]   y    	 East
 **	 \param [in]   lat    	 Latitude in degrees
 **	 \param [in]   lon     	 Longitude in degrees
 **
 **  \returns
 **  0 if map_projection_init was called before, -1 else
 **  \endreturns
 **
 *************************************************************************/
int map_projection_reproject(const struct map_projection_reference_s *ref, float x, float y, double *lat,
				      double *lon);

/************************************************************************/
/** \brief Get Reference Position
 **
 **  \par Description
 **       This function get reference position of the global map projection
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **	 \param [in/out]   lat_0    	 Latitude
 **	 \param [in/out]   lon_0     	 Longitude
 **
 **  \returns
 **  0 if map_projection_init was called before, -1 else
 **  \endreturns
 **
 *************************************************************************/
int map_projection_global_getref(double *lat_0, double *lon_0);

/************************************************************************/
/** \brief Initialize Global Local Converter
 **
 **  \par Description
 **       This function initializes the global mapping between global
 **       position (spherical) and local position (NED).
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **	 \param [in]   lat_0    	 Latitude
 **	 \param [in]   lon_0     	 Longitude
 **	 \param [in]   alt_0     	 Altitude
 **	 \param [in]   timestamp     Timestamp
 **
 **  \returns
 **  0 if map_projection_init was called before, -1 else
 **  \endreturns
 **
 *************************************************************************/
int globallocalconverter_init(double lat_0, double lon_0, float alt_0, uint64 timestamp);

/************************************************************************/
/** \brief Check Global Local Converter Initialized
 **
 **  \par Description
 **       This function checks if globallocalconverter was initialized
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **  \returns
 **  True if map was initialized before, false else
 **  \endreturns
 **
 *************************************************************************/
bool globallocalconverter_initialized(void);

/************************************************************************/
/** \brief Convert Global To Local
 **
 **  \par Description
 **       This function convert from global position coordinates to local
 **        position coordinates using the global reference
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **	 \param [in]       lat    	 Latitude
 **	 \param [in]   	   lon     	 Longitude
 **	 \param [in]   	   alt     	 Altitude
 **	 \param [in/out]   x     	 North
 **	 \param [in/out]   y     	 East
 **	 \param [in/out]   z     	 Down
 **
 **  \returns
 **  0 if map_projection_init was called before, -1 else
 **  \endreturns
 **
 *************************************************************************/
int globallocalconverter_tolocal(double lat, double lon, float alt, float *x, float *y, float *z);

/************************************************************************/
/** \brief Convert Local To Global
 **
 **  \par Description
 **       This function converts from local position coordinates to global
 **       position coordinates using the global reference
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **	 \param [in]  		 x     	 North
 **	 \param [in]   		 y     	 East
 **	 \param [in] 		 z     	 Down
 **	 \param [in/out]     lat     Latitude
 **	 \param [in/out]   	 lon     Longitude
 **	 \param [in/out]   	 alt     Altitude
 **
 **  \returns
 **  0 if map_projection_init was called before, -1 else
 **  \endreturns
 **
 *************************************************************************/
int globallocalconverter_toglobal(float x, float y, float z,  double *lat, double *lon, float *alt);

/************************************************************************/
/** \brief Get Position Reference
 **
 **  \par Description
 **       This function gets reference position of the global to
 **       local converter
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **	 \param [in/out]     lat_0     Latitude
 **	 \param [in/out]   	 lon_0     Longitude
 **	 \param [in/out]   	 alt_0     Altitude
 **
 **  \returns
 **  0 if map_projection_init was called before, -1 else
 **  \endreturns
 **
 *************************************************************************/
int globallocalconverter_getref(double *lat_0, double *lon_0, float *alt_0);

/************************************************************************/
/** \brief Get Waypoint Distance
 **
 **  \par Description
 **       This function returns the distance to the next waypoint in meters.
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **	 \param [in]     lat_now      Latitude now
 **	 \param [in]   	 lon_now      Longitude now
 **	 \param [in]     lat_next     Latitude next
 **	 \param [in]   	 lon_next     Longitude next
 **
 **  \returns
 **  Distance in meters
 **  \endreturns
 **
 *************************************************************************/
float get_distance_to_next_waypoint(double lat_now, double lon_now, double lat_next, double lon_next);

/************************************************************************/
/** \brief Create Waypoint
 **
 **  \par Description
 **       This function creates a new waypoint C on the line of two given
 **        waypoints (A, B) at certain distance from waypoint A
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **	 \param [in]     	lat_A     	 	Latitude A
 **	 \param [in]   	 	lon_A     		Longitude A
 **	 \param [in]     	lat_B     		Latitude B
 **	 \param [in]   		lon_B     		Longitude B
 **	 \param [in]   		dist     		Distance
 **	 \param [in/out]    lat_target      Latitude C
 **	 \param [in/out]   	lon_target      Longitude C
 **
 *************************************************************************/
void create_waypoint_from_line_and_dist(double lat_A, double lon_A, double lat_B, double lon_B, float dist,
		double *lat_target, double *lon_target);

/************************************************************************/
/** \brief Create Waypoint From Heading
 **
 **  \par Description
 **       This function creates a waypoint from given waypoint, distance
 **       and bearing see http://www.movable-type.co.uk/scripts/latlong.html
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **	 \param [in]     	lat_A     	 	Latitude A
 **	 \param [in]   	 	lon_A     		Longitude A
 **	 \param [in]     	bearing    		Bearin in radians
 **	 \param [in]   		dist     		Distance
 **	 \param [in/out]    lat_target      Latitude C
 **	 \param [in/out]   	lon_target      Longitude C
 **
 *************************************************************************/
void waypoint_from_heading_and_distance(double lat_start, double lon_start, float bearing, float dist,
		double *lat_target, double *lon_target);

/************************************************************************/
/** \brief Get Waypoint Bearing
 **
 **  \par Description
 **       This function returns the bearing to the next waypoint in radians.
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **	 \param [in]     lat_now      Latitude now
 **	 \param [in]   	 lon_now      Longitude now
 **	 \param [in]     lat_next     Latitude next
 **	 \param [in]   	 lon_next     Longitude next
 **
 **  \returns
 **  Bearing in radians
 **  \endreturns
 **
 *************************************************************************/
float get_bearing_to_next_waypoint(double lat_now, double lon_now, double lat_next, double lon_next);

/************************************************************************/
/** \brief Calculate Global Distance
 **
 **  \par Description
 **       This function Calculate distance in global frame
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **	 \param [in]  	     lat_now      Latitude now
 **	 \param [in]   		 lon_now      Longitude now
 **	 \param [in]   		 alt_now      Altitude now
 **	 \param [in] 	     lat_next     Latitude next
 **	 \param [in]   		 lon_next     Longitude next
 **	 \param [in]   		 alt_next     Altitude next
 **	 \param [in/out]   	 dist_xy      Horizontal distance
 **	 \param [in/out]   	 dist_z       Vertical distance
 **
 **  \returns
 **  Euclidian distance
 **  \endreturns
 **
 *************************************************************************/
float get_distance_to_point_global_wgs84(double lat_now, double lon_now, float alt_now,
		double lat_next, double lon_next, float alt_next,
		float *dist_xy, float *dist_z);

/************************************************************************/
/** \brief Wrap Bearing PI
 **
 **  \par Description
 **       This function wraps the bearing to PI
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **	 \param [in]     bearing      Bearing
 **
 **  \returns
 **  Wrapped bearing
 **  \endreturns
 **
 *************************************************************************/
float _wrap_pi(float bearing);

/************************************************************************/
/** \brief Wrap Bearing 2PI
 **
 **  \par Description
 **       This function wraps the bearing to 2PI
 **
 **  \par Assumptions, External Events, and Notes:
 **       None
 **
 **	 \param [in]     bearing      Bearing
 **
 **  \returns
 **  Wrapped bearing
 **  \endreturns
 **
 *************************************************************************/
float _wrap_2pi(float bearing);

#ifdef __cplusplus
}
#endif

#endif
