#ifndef SIMLINK_H
#define SIMLINK_H

#include "cfe.h"
#include "simlink_platform_cfg.h"

typedef enum
{
	SIMLINK_GPS_NONE0_FIX                            = 0,
	SIMLINK_GPS_NONE1_FIX                            = 1,
	SIMLINK_GPS_2D_FIX                               = 2,
	SIMLINK_GPS_3D_FIX                               = 3,
	SIMLINK_GPS_DGPS_FIX                             = 4,
	SIMLINK_GPS_RTK_FIX                              = 5
} SIMLINK_GpsFixType_t;

typedef struct
{
    uint64   TimeUsec;              /*< [us] Timestamp (UNIX Epoch time or time since system boot). The receiving end can infer timestamp format (since 1.1.1970 or since system boot) by checking for the magnitude of the number.*/
    int32    Latitude;              /*< [degE7] Latitude (WGS84)*/
    int32    Longitude;             /*< [degE7] Longitude (WGS84)*/
    int32    Altitude;              /*< [mm] Altitude (MSL). Positive for up.*/
    uint16   EpH;                   /*<  GPS HDOP horizontal dilution of position (unitless). If unknown, set to: UINT16_MAX*/
    uint16   EpV;                   /*<  GPS VDOP vertical dilution of position (unitless). If unknown, set to: UINT16_MAX*/
    uint16   Speed;                 /*< [cm/s] GPS ground speed. If unknown, set to: 65535*/
    int16    VelocityNorth;         /*< [cm/s] GPS velocity in north direction in earth-fixed NED frame*/
    int16    VelocityEast;          /*< [cm/s] GPS velocity in east direction in earth-fixed NED frame*/
    int16    VelocityDown;          /*< [cm/s] GPS velocity in down direction in earth-fixed NED frame*/
    uint16   COG;                   /*< [cdeg] Course over ground (NOT heading, but direction of movement), 0.0..359.99 degrees. If unknown, set to: 65535*/
    SIMLINK_GpsFixType_t FixType;   /*<  0-1: no fix, 2: 2D fix, 3: 3D fix. Some applications will not use the value of this field unless it is at least two, so always correctly fill in the fix.*/
    uint8    SatellitesVisible;     /*<  Number of satellites visible. If unknown, set to 255*/
    uint16   Yaw;                   /*< [cdeg] Yaw of vehicle relative to Earth's North, zero means not available, use 36000 for north*/
} SIMLINK_GPS_Msg_t;

typedef struct
{
	 uint64 TimeUsec; /*< [us] Timestamp (UNIX Epoch time or time since system boot). The receiving end can infer timestamp format (since 1.1.1970 or since system boot) by checking for the magnitude of the number.*/
     float  X;
     float  Y;
     float  Z;
} SIMLINK_Gyro_Msg_t;

typedef struct
{
	 uint64 TimeUsec; /*< [us] Timestamp (UNIX Epoch time or time since system boot). The receiving end can infer timestamp format (since 1.1.1970 or since system boot) by checking for the magnitude of the number.*/
     float  X;
     float  Y;
     float  Z;
} SIMLINK_Accel_Msg_t;

typedef struct
{
	 uint64 TimeUsec; /*< [us] Timestamp (UNIX Epoch time or time since system boot). The receiving end can infer timestamp format (since 1.1.1970 or since system boot) by checking for the magnitude of the number.*/
     float  X;
     float  Y;
     float  Z;
} SIMLINK_Mag_Msg_t;

typedef struct
{
	 uint64 TimeUsec; /*< [us] Timestamp (UNIX Epoch time or time since system boot). The receiving end can infer timestamp format (since 1.1.1970 or since system boot) by checking for the magnitude of the number.*/
     float  Pressure;
     float  Temperature;
     float  BarometricAltitude;
} SIMLINK_Baro_Msg_t;

typedef struct
{
	 uint64  TimeUsec; /*< [us] Timestamp (UNIX Epoch time or time since system boot). The receiving end can infer timestamp format (since 1.1.1970 or since system boot) by checking for the magnitude of the number.*/
	 float   Channel[SIMLINK_PWM_CHANNEL_COUNT];
} SIMLINK_PWM_Msg_t;



#endif
