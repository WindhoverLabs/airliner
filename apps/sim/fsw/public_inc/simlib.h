#ifndef SIMLIB_H
#define SIMLIB_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "px4_msgs.h"

/************************************************************************
** Type Definitions
*************************************************************************/


/*************************************************************************
** Exported Functions
*************************************************************************/


int32 SIMLIB_GetAccel(float *X, float *Y, float *Z);

int32 SIMLIB_SetAccel(float X, float Y, float Z);

int32 SIMLIB_GetGyro(float *X, float *Y, float *Z);

int32 SIMLIB_SetGyro(float X, float Y, float Z);

int32 SIMLIB_GetMag(float *X, float *Y, float *Z);

int32 SIMLIB_SetMag(float X, float Y, float Z);

int32 SIMLIB_GetTemp(float *Temperature);

int32 SIMLIB_SetTemp(float Temperature);

int32 SIMLIB_GetPressure(float *Absolute, float *Differential);

int32 SIMLIB_SetPressure(float Absolute, float Differential);

int32 SIMLIB_GetPressureAltitude(float *Altitude);

int32 SIMLIB_SetPressureAltitude(float Altitude);

int32 SIMLIB_GetGPS(
		PX4_GpsFixType_t *FixType,
		int32            *Latitude,
		int32            *Longitude,
		int32            *Altitude,
		uint16           *EPH,
		uint16           *EPV,
		uint16           *Velocity,
		int16            *VN,
		int16            *VE,
		int16            *VD,
		uint16           *COG,
		uint8            *SatellitesVisible);

int32 SIMLIB_SetGPS(
		PX4_GpsFixType_t FixType,
		int32            Latitude,
		int32            Longitude,
		int32            Altitude,
		uint16           EPH,
		uint16           EPV,
		uint16           Velocity,
		int16            VN,
		int16            VE,
		int16            VD,
		uint16           COG,
		uint8            SatellitesVisible);

int32 SIMLIB_GetRCInputs(
		uint16 *Channels,
		uint16 *ChannelCount,
		uint8  *RSSI);

int32 SIMLIB_SetRCInputs(
		const uint16 *Channels,
		uint16 ChannelCount,
		uint8  RSSI);

int32 SIMLIB_GetActuatorControls(
		float  *Control,
		uint32 *ControlCount,
		uint8  *Mode);

int32 SIMLIB_SetActuatorControls(
		const float *Control,
		uint32 ControlCount,
		uint8  Mode);
        
int32 SIMLIB_SetLandingGearControls(
		const float *Control,
		uint32 LeftGearIndex, uint32 RightGearIndex);

int32 SIMLIB_GetDistanceSensor(
		uint16 *Minimum,
		uint16 *Maximum,
		uint16 *Current,
		PX4_DistanceSensorType_t *Type,
		uint8  *ID,
		PX4_SensorOrientation_t  *Orientation,
		uint8  *Covariance);

int32 SIMLIB_SetDistanceSensor(
		uint16 Minimum,
		uint16 Maximum,
		uint16 Current,
		PX4_DistanceSensorType_t Type,
		uint8  ID,
		PX4_SensorOrientation_t  Orientation,
		uint8  Covariance);

void SIMLIB_SetSocket(int Socket, int Port, char *Address);


int32 SIMLIB_SendHeartbeat(void);


#ifdef __cplusplus
}
#endif

#endif /* SIMLIB_H */

/************************/
/*  End of File Comment */
/************************/
