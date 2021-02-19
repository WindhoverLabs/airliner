/*************************************************************************
** Includes
*************************************************************************/
#include "simlib.h"
#include "sim_version.h"
#include "px4_msgs.h"
#include "sim_mission_cfg.h"
#include "sim_platform_cfg.h"
#include "mavlink.h"
#include <arpa/inet.h>
#include "px4lib_msgids.h"

/*************************************************************************
** Macro Definitions
*************************************************************************/

#define SIMLIB_OK            (0)
#define SIMLIB_OK_FRESH      (1)
#define SIMLIB_OK_STALE      (2)
#define SIMLIB_OK_NO_VALUE   (3)
#define SIMLIB_INVALID_PARAM (-1)

typedef enum
{
	SIMLIB_DATA_STATE_NO_DATA = SIMLIB_OK_NO_VALUE,
	SIMLIB_DATA_STATE_FRESH = SIMLIB_OK_FRESH,
	SIMLIB_DATA_STATE_STALE = SIMLIB_OK_STALE,
} SIMLIB_DataState_t;

typedef struct
{
	float X;
	float Y;
	float Z;
	SIMLIB_DataState_t DataState;
} SIMLIB_AccelData_t;

typedef struct
{
	float X;
	float Y;
	float Z;
	SIMLIB_DataState_t DataState;
} SIMLIB_GyroData_t;

typedef struct
{
	float X;
	float Y;
	float Z;
	SIMLIB_DataState_t DataState;
} SIMLIB_MagData_t;

typedef struct
{
	float Temperature;
	SIMLIB_DataState_t DataState;
} SIMLIB_TemperatureData_t;

typedef struct
{
	float Absolute;
	float Differential;
	SIMLIB_DataState_t DataState;
} SIMLIB_DiffPressureData_t;

typedef struct
{
	float Altitude;
	SIMLIB_DataState_t DataState;
} SIMLIB_PressureAltitudeData_t;

typedef struct
{
	PX4_GpsFixType_t FixType;
	int32            Latitude;
	int32            Longitude;
	int32            Altitude;
	uint16           EPH;
	uint16           EPV;
	uint16           Velocity;
	int16            VN;
	int16            VE;
	int16            VD;
	uint16           COG;
	uint8            SatellitesVisible;
	SIMLIB_DataState_t DataState;
} SIMLIB_GPSData_t;

typedef struct
{
	uint16 Channel[12];
	uint8  RSSI;
	SIMLIB_DataState_t DataState;
} SIMLIB_RCInputData_t;

typedef struct
{
	float  Control[16];
	uint8  Mode;
	SIMLIB_DataState_t DataState;
} SIMLIB_ActuatorControlsData_t;

typedef struct
{
	uint16 Minimum;
	uint16 Maximum;
	uint16 Current;
	uint16 Covariance;
	PX4_DistanceSensorType_t Type;
	uint8  ID;
	PX4_SensorOrientation_t  Orientation;
	SIMLIB_DataState_t DataState;
} SIMLIB_DistanceSensorData_t;


typedef struct
{
	uint32 MutexID;
	SIMLIB_AccelData_t AccelData;
	SIMLIB_GyroData_t GyroData;
	SIMLIB_MagData_t MagData;
	SIMLIB_TemperatureData_t TemperatureData;
	SIMLIB_DiffPressureData_t DiffPressureData;
	SIMLIB_PressureAltitudeData_t PressureAltitudeData;
	SIMLIB_GPSData_t GPSData;
	SIMLIB_RCInputData_t RCInputData;
	SIMLIB_ActuatorControlsData_t ActuatorControlsData;
	SIMLIB_DistanceSensorData_t  DistanceSensorData;
    int Socket;
    int SendPort;
    char SendAddress[255];
} SIMLIB_LibData_t;

SIMLIB_LibData_t SIMLIB_LibData;

/*************************************************************************
** Private Function Prototypes
*************************************************************************/
int32 SIM_LibInit(void);


int32 SIM_LibInit(void)
{
	int32 iStatus = CFE_SUCCESS;

	iStatus = OS_MutSemCreate(&SIMLIB_LibData.MutexID, SIMLIB_MUTEX_SEM_NAME, 0);
	if(iStatus != CFE_SUCCESS)
	{
	    OS_printf ("SIM failed to create mutsem.  %li\n", iStatus);
	}
	else
	{
		SIMLIB_LibData.AccelData.DataState = SIMLIB_DATA_STATE_NO_DATA;
		SIMLIB_LibData.GyroData.DataState = SIMLIB_DATA_STATE_NO_DATA;
		SIMLIB_LibData.MagData.DataState = SIMLIB_DATA_STATE_NO_DATA;
		SIMLIB_LibData.TemperatureData.DataState = SIMLIB_DATA_STATE_NO_DATA;
		SIMLIB_LibData.DiffPressureData.DataState = SIMLIB_DATA_STATE_NO_DATA;
		SIMLIB_LibData.PressureAltitudeData.DataState = SIMLIB_DATA_STATE_NO_DATA;
		SIMLIB_LibData.GPSData.DataState = SIMLIB_DATA_STATE_NO_DATA;
		SIMLIB_LibData.RCInputData.DataState = SIMLIB_DATA_STATE_NO_DATA;
		SIMLIB_LibData.ActuatorControlsData.DataState = SIMLIB_DATA_STATE_NO_DATA;

		OS_printf ("SIM Interface Initialized.  Version %d.%d.%d.%d\n",
                SIM_MAJOR_VERSION,
                SIM_MINOR_VERSION,
                SIM_REVISION,
                SIM_MISSION_REV);
	}

    return iStatus;
 
}/* End SIM_LibInit */


void SIMLIB_SetSocket(int Socket)
{
	SIMLIB_LibData.Socket = Socket;
}

int32 SIMLIB_GetAccel(float *X, float *Y, float *Z)
{
	int32 iStatus = SIMLIB_OK_FRESH;

	if((X == 0) || (Y == 0) || (Z == 0))
	{
		iStatus = SIMLIB_INVALID_PARAM;
		goto end_of_function;
	}

	OS_MutSemTake(SIMLIB_LibData.MutexID);

	*X = SIMLIB_LibData.AccelData.X;
	*Y = SIMLIB_LibData.AccelData.Y;
	*Z = SIMLIB_LibData.AccelData.Z;

	iStatus = SIMLIB_LibData.AccelData.DataState;
	if(iStatus == SIMLIB_OK_FRESH)
	{
		SIMLIB_LibData.AccelData.DataState = SIMLIB_OK_STALE;
	}

	OS_MutSemGive(SIMLIB_LibData.MutexID);

end_of_function:
    return iStatus;
}

int32 SIMLIB_SetAccel(float X, float Y, float Z)
{
	OS_MutSemTake(SIMLIB_LibData.MutexID);

	SIMLIB_LibData.AccelData.X = X;
	SIMLIB_LibData.AccelData.Y = Y;
	SIMLIB_LibData.AccelData.Z = Z;
	SIMLIB_LibData.AccelData.DataState = SIMLIB_OK_FRESH;

	OS_MutSemGive(SIMLIB_LibData.MutexID);

	return SIMLIB_OK;
}


int32 SIMLIB_GetGyro(float *X, float *Y, float *Z)
{
	int32 iStatus = SIMLIB_OK_FRESH;

	if((X == 0) || (Y == 0) || (Z == 0))
	{
		iStatus = SIMLIB_INVALID_PARAM;
		goto end_of_function;
	}

	OS_MutSemTake(SIMLIB_LibData.MutexID);

	*X = SIMLIB_LibData.GyroData.X;
	*Y = SIMLIB_LibData.GyroData.Y;
	*Z = SIMLIB_LibData.GyroData.Z;

	iStatus = SIMLIB_LibData.GyroData.DataState;
	if(iStatus == SIMLIB_OK_FRESH)
	{
		SIMLIB_LibData.GyroData.DataState = SIMLIB_OK_STALE;
	}

	OS_MutSemGive(SIMLIB_LibData.MutexID);

end_of_function:
    return iStatus;
}

int32 SIMLIB_SetGyro(float X, float Y, float Z)
{
	OS_MutSemTake(SIMLIB_LibData.MutexID);

	SIMLIB_LibData.GyroData.X = X;
	SIMLIB_LibData.GyroData.Y = Y;
	SIMLIB_LibData.GyroData.Z = Z;
	SIMLIB_LibData.GyroData.DataState = SIMLIB_OK_FRESH;

	OS_MutSemGive(SIMLIB_LibData.MutexID);

	return SIMLIB_OK;
}


int32 SIMLIB_GetMag(float *X, float *Y, float *Z)
{
	int32 iStatus = SIMLIB_OK_FRESH;

	if((X == 0) || (Y == 0) || (Z == 0))
	{
		iStatus = SIMLIB_INVALID_PARAM;
		goto end_of_function;
	}

	OS_MutSemTake(SIMLIB_LibData.MutexID);

	*X = SIMLIB_LibData.MagData.X;
	*Y = SIMLIB_LibData.MagData.Y;
	*Z = SIMLIB_LibData.MagData.Z;

	iStatus = SIMLIB_LibData.MagData.DataState;
	if(iStatus == SIMLIB_OK_FRESH)
	{
		SIMLIB_LibData.MagData.DataState = SIMLIB_OK_STALE;
	}

	OS_MutSemGive(SIMLIB_LibData.MutexID);

end_of_function:
    return iStatus;
}

int32 SIMLIB_SetMag(float X, float Y, float Z)
{
	OS_MutSemTake(SIMLIB_LibData.MutexID);

	SIMLIB_LibData.MagData.X = X;
	SIMLIB_LibData.MagData.Y = Y;
	SIMLIB_LibData.MagData.Z = Z;
	SIMLIB_LibData.MagData.DataState = SIMLIB_OK_FRESH;

	OS_MutSemGive(SIMLIB_LibData.MutexID);

	return SIMLIB_OK;
}


int32 SIMLIB_GetTemp(float *Temperature)
{
	int32 iStatus = SIMLIB_OK_NO_VALUE;

	if(Temperature == 0)
	{
		iStatus = SIMLIB_INVALID_PARAM;
		goto end_of_function;
	}

	OS_MutSemTake(SIMLIB_LibData.MutexID);

	*Temperature = SIMLIB_LibData.TemperatureData.Temperature;

	iStatus = SIMLIB_LibData.TemperatureData.DataState;
	if(iStatus == SIMLIB_OK_FRESH)
	{
		SIMLIB_LibData.TemperatureData.DataState = SIMLIB_OK_STALE;
	}

	OS_MutSemGive(SIMLIB_LibData.MutexID);

end_of_function:
    return iStatus;
}

int32 SIMLIB_SetTemp(float Temperature)
{
	OS_MutSemTake(SIMLIB_LibData.MutexID);

	SIMLIB_LibData.TemperatureData.Temperature = Temperature;
	SIMLIB_LibData.TemperatureData.DataState = SIMLIB_OK_FRESH;

	OS_MutSemGive(SIMLIB_LibData.MutexID);

	return SIMLIB_OK;
}


int32 SIMLIB_GetPressure(float *Absolute, float *Differential)
{
	int32 iStatus = SIMLIB_OK_NO_VALUE;

	if((Absolute == 0) || (Differential == 0))
	{
		iStatus = SIMLIB_INVALID_PARAM;
		goto end_of_function;
	}

	OS_MutSemTake(SIMLIB_LibData.MutexID);

	*Absolute = SIMLIB_LibData.DiffPressureData.Absolute;
	*Differential = SIMLIB_LibData.DiffPressureData.Differential;

	iStatus = SIMLIB_LibData.DiffPressureData.DataState;
	if(iStatus == SIMLIB_OK_FRESH)
	{
		SIMLIB_LibData.DiffPressureData.DataState = SIMLIB_OK_STALE;
	}

	OS_MutSemGive(SIMLIB_LibData.MutexID);

end_of_function:
    return iStatus;
}

int32 SIMLIB_SetPressure(float Absolute, float Differential)
{
	OS_MutSemTake(SIMLIB_LibData.MutexID);

	SIMLIB_LibData.DiffPressureData.Absolute = Absolute;
	SIMLIB_LibData.DiffPressureData.Differential = Differential;
	SIMLIB_LibData.DiffPressureData.DataState = SIMLIB_OK_FRESH;

	OS_MutSemGive(SIMLIB_LibData.MutexID);

	return SIMLIB_OK;
}


int32 SIMLIB_GetPressureAltitude(float *Altitude)
{
	int32 iStatus = SIMLIB_OK_NO_VALUE;

	if(Altitude == 0)
	{
		iStatus = SIMLIB_INVALID_PARAM;
		goto end_of_function;
	}

	OS_MutSemTake(SIMLIB_LibData.MutexID);

	*Altitude = SIMLIB_LibData.PressureAltitudeData.Altitude;

	iStatus = SIMLIB_LibData.PressureAltitudeData.DataState;
	if(iStatus == SIMLIB_OK_FRESH)
	{
		SIMLIB_LibData.PressureAltitudeData.DataState = SIMLIB_OK_STALE;
	}

	OS_MutSemGive(SIMLIB_LibData.MutexID);

end_of_function:
    return iStatus;
}

int32 SIMLIB_SetPressureAltitude(float Altitude)
{
	OS_MutSemTake(SIMLIB_LibData.MutexID);

	SIMLIB_LibData.PressureAltitudeData.Altitude = Altitude;
	SIMLIB_LibData.PressureAltitudeData.DataState = SIMLIB_OK_FRESH;

	OS_MutSemGive(SIMLIB_LibData.MutexID);

	return SIMLIB_OK;
}


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
		uint8            *SatellitesVisible)
{
	int32 iStatus = SIMLIB_OK_NO_VALUE;

	if(
			(FixType == 0) ||
			(Latitude == 0) ||
			(Longitude == 0) ||
			(Altitude == 0) ||
			(EPH == 0) ||
			(EPV == 0) ||
			(Velocity == 0) ||
			(VN == 0) ||
			(VE == 0) ||
			(VD == 0) ||
			(COG == 0) ||
			(SatellitesVisible == 0))
	{
		iStatus = SIMLIB_INVALID_PARAM;
		goto end_of_function;
	}

	OS_MutSemTake(SIMLIB_LibData.MutexID);

	*FixType = SIMLIB_LibData.GPSData.FixType;
	*Latitude = SIMLIB_LibData.GPSData.Latitude;
	*Longitude = SIMLIB_LibData.GPSData.Longitude;
	*Altitude = SIMLIB_LibData.GPSData.Altitude;
	*EPH = SIMLIB_LibData.GPSData.EPH;
	*EPV = SIMLIB_LibData.GPSData.EPV;
	*Velocity = SIMLIB_LibData.GPSData.Velocity;
	*VN = SIMLIB_LibData.GPSData.VN;
	*VE = SIMLIB_LibData.GPSData.VE;
	*VD = SIMLIB_LibData.GPSData.VD;
	*COG = SIMLIB_LibData.GPSData.COG;
	*SatellitesVisible = SIMLIB_LibData.GPSData.SatellitesVisible;

	iStatus = SIMLIB_LibData.GPSData.DataState;
	if(iStatus == SIMLIB_OK_FRESH)
	{
		SIMLIB_LibData.GPSData.DataState = SIMLIB_OK_STALE;
	}

	OS_MutSemGive(SIMLIB_LibData.MutexID);

end_of_function:
    return iStatus;
}

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
		uint8            SatellitesVisible)
{
	OS_MutSemTake(SIMLIB_LibData.MutexID);

    SIMLIB_LibData.GPSData.FixType = FixType;
    SIMLIB_LibData.GPSData.Latitude = Latitude;
    SIMLIB_LibData.GPSData.Longitude = Longitude;
    SIMLIB_LibData.GPSData.Altitude = Altitude;
    SIMLIB_LibData.GPSData.EPH = EPH;
    SIMLIB_LibData.GPSData.EPV = EPV;
    SIMLIB_LibData.GPSData.Velocity = Velocity;
    SIMLIB_LibData.GPSData.VN = VN;
    SIMLIB_LibData.GPSData.VE = VE;
    SIMLIB_LibData.GPSData.VD = VD;
    SIMLIB_LibData.GPSData.COG = COG;
    SIMLIB_LibData.GPSData.SatellitesVisible = SatellitesVisible;
	SIMLIB_LibData.PressureAltitudeData.DataState = SIMLIB_OK_FRESH;

	OS_MutSemGive(SIMLIB_LibData.MutexID);

	return SIMLIB_OK;
}


int32 SIMLIB_GetRCInputs(
		uint16 *Channels,
		uint16 *ChannelCount,
		uint8  *RSSI)
{
	int32 iStatus = SIMLIB_OK_NO_VALUE;
    uint32 i = 0;

	if((Channels == 0) || (ChannelCount == 0))
	{
		iStatus = SIMLIB_INVALID_PARAM;
		goto end_of_function;
	}

	if(*ChannelCount > 12)
	{
		*ChannelCount = 12;
	}

	OS_MutSemTake(SIMLIB_LibData.MutexID);

	for(i=0; i < *ChannelCount; ++i)
	{
		Channels[i] = SIMLIB_LibData.RCInputData.Channel[i];
	}
	*RSSI = SIMLIB_LibData.RCInputData.RSSI;

	iStatus = SIMLIB_LibData.RCInputData.DataState;
	if(iStatus == SIMLIB_OK_FRESH)
	{
		SIMLIB_LibData.RCInputData.DataState = SIMLIB_OK_STALE;
	}

	OS_MutSemGive(SIMLIB_LibData.MutexID);

end_of_function:
    return iStatus;
}

int32 SIMLIB_SetRCInputs(
		const uint16 *Channels,
		uint16 ChannelCount,
		uint8  RSSI)
{
	int32 iStatus = SIMLIB_OK_NO_VALUE;
    uint32 i = 0;

	if(Channels == 0)
	{
		iStatus = SIMLIB_INVALID_PARAM;
		goto end_of_function;
	}

	if(ChannelCount > 12)
	{
		iStatus = SIMLIB_INVALID_PARAM;
		goto end_of_function;
	}

	OS_MutSemTake(SIMLIB_LibData.MutexID);

	for(i=0; i < ChannelCount; ++i)
	{
		SIMLIB_LibData.RCInputData.Channel[i] = Channels[i];
	}
	SIMLIB_LibData.RCInputData.RSSI = RSSI;
	SIMLIB_LibData.RCInputData.DataState = SIMLIB_OK_FRESH;

	OS_MutSemGive(SIMLIB_LibData.MutexID);

	iStatus = SIMLIB_OK;

end_of_function:
    return iStatus;
}


int32 SIMLIB_GetActuatorControls(
		float  *Control,
		uint32 *ControlCount,
		uint8  *Mode)
{
	int32 iStatus = SIMLIB_OK_NO_VALUE;
	uint32 i = 0;

	if((Control == 0) || (ControlCount == 0))
	{
		iStatus = SIMLIB_INVALID_PARAM;
		goto end_of_function;
	}

	if(*ControlCount > 16)
	{
		*ControlCount = 16;
	}

	OS_MutSemTake(SIMLIB_LibData.MutexID);

	for(i=0; i < *ControlCount; ++i)
	{
		Control[i] = SIMLIB_LibData.ActuatorControlsData.Control[i];
	}
	*Mode = SIMLIB_LibData.ActuatorControlsData.Mode;

	iStatus = SIMLIB_LibData.ActuatorControlsData.DataState;
	if(iStatus == SIMLIB_OK_FRESH)
	{
		SIMLIB_LibData.ActuatorControlsData.DataState = SIMLIB_OK_STALE;
	}

	OS_MutSemGive(SIMLIB_LibData.MutexID);

end_of_function:
    return iStatus;
}

int32 SIMLIB_SetActuatorControls(
		const float *Control,
		uint32 ControlCount,
		uint8  Mode)
{
	int32 iStatus = SIMLIB_OK_NO_VALUE;
	uint32 i = 0;
	mavlink_message_t msg = {};
	uint8 buffer[MAVLINK_MAX_PACKET_LEN];
	mavlink_hil_actuator_controls_t actuatorControlsMsg = {};
	uint32 length = 0;
    //struct sockaddr_in si_sim;

    //si_sim.sin_family = AF_INET;
    //si_sim.sin_port = htons(36985);
    //si_sim.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(Control == 0)
	{
		iStatus = SIMLIB_INVALID_PARAM;
		goto end_of_function;
	}

	if(ControlCount > 16)
	{
		iStatus = SIMLIB_INVALID_PARAM;
		goto end_of_function;
	}

	OS_MutSemTake(SIMLIB_LibData.MutexID);

    /* Copy just the control count into SIMLIB data */
	for(i=0; i < ControlCount; ++i)
	{
		SIMLIB_LibData.ActuatorControlsData.Control[i] = Control[i];
	}
	SIMLIB_LibData.ActuatorControlsData.Mode = Mode;
	SIMLIB_LibData.ActuatorControlsData.DataState = SIMLIB_OK_FRESH;

    /* Copy the full contents from SIMLIB data */
	for(i=0; i < 16; ++i)
	{
		actuatorControlsMsg.controls[i] = SIMLIB_LibData.ActuatorControlsData.Control[i];
	}
	OS_MutSemGive(SIMLIB_LibData.MutexID);

	actuatorControlsMsg.time_usec = 0;
	actuatorControlsMsg.flags = 0;
	actuatorControlsMsg.mode = 129;

	mavlink_msg_hil_actuator_controls_encode(1, 1, &msg, &actuatorControlsMsg);
	length = mavlink_msg_to_send_buffer(buffer, &msg);

	if(SIMLIB_LibData.Socket != 0)
	{
		send(SIMLIB_LibData.Socket, (char *)buffer, length, 0);
	}

	iStatus = SIMLIB_OK;

end_of_function:
    return iStatus;
}


int32 SIMLIB_SetLandingGearControls(
		const float *Control,
		uint32 LeftGearIndex, uint32 RightGearIndex)
{
	int32 iStatus = SIMLIB_OK_NO_VALUE;
	uint32 i = 0;

	if(Control == 0)
	{
		iStatus = SIMLIB_INVALID_PARAM;
		goto end_of_function;
	}

	OS_MutSemTake(SIMLIB_LibData.MutexID);

    SIMLIB_LibData.ActuatorControlsData.Control[LeftGearIndex] = Control[LeftGearIndex];
    SIMLIB_LibData.ActuatorControlsData.Control[RightGearIndex] = Control[RightGearIndex];

	OS_MutSemGive(SIMLIB_LibData.MutexID);

	iStatus = SIMLIB_OK;

end_of_function:
    return iStatus;
}


int32 SIMLIB_GetDistanceSensor(
		uint16 *Minimum,
		uint16 *Maximum,
		uint16 *Current,
		PX4_DistanceSensorType_t *Type,
		uint8  *ID,
		PX4_SensorOrientation_t  *Orientation,
		uint8  *Covariance)
{
	int32 iStatus = SIMLIB_OK_NO_VALUE;

	if(
			(Minimum == 0) ||
			(Maximum == 0) ||
			(Current == 0) ||
			(Type == 0) ||
			(ID == 0) ||
			(Orientation == 0) ||
			(Covariance == 0))
	{
		iStatus = SIMLIB_INVALID_PARAM;
		goto end_of_function;
	}

	OS_MutSemTake(SIMLIB_LibData.MutexID);

	*Minimum = SIMLIB_LibData.DistanceSensorData.Minimum;
	*Maximum = SIMLIB_LibData.DistanceSensorData.Maximum;
	*Current = SIMLIB_LibData.DistanceSensorData.Current;
	*Type = SIMLIB_LibData.DistanceSensorData.Type;
	*ID = SIMLIB_LibData.DistanceSensorData.ID;
	*Orientation = SIMLIB_LibData.DistanceSensorData.Orientation;
	*Covariance = SIMLIB_LibData.DistanceSensorData.Covariance;

	iStatus = SIMLIB_LibData.DistanceSensorData.DataState;
	if(iStatus == SIMLIB_OK_FRESH)
	{
		SIMLIB_LibData.DistanceSensorData.DataState = SIMLIB_OK_STALE;
	}

	OS_MutSemGive(SIMLIB_LibData.MutexID);

end_of_function:
    return iStatus;
}

int32 SIMLIB_SetDistanceSensor(
		uint16 Minimum,
		uint16 Maximum,
		uint16 Current,
		PX4_DistanceSensorType_t Type,
		uint8  ID,
		PX4_SensorOrientation_t  Orientation,
		uint8  Covariance)
{
	OS_MutSemTake(SIMLIB_LibData.MutexID);

	SIMLIB_LibData.DistanceSensorData.Minimum = Minimum;
	SIMLIB_LibData.DistanceSensorData.Maximum = Maximum;
	SIMLIB_LibData.DistanceSensorData.Current = Current;
	SIMLIB_LibData.DistanceSensorData.Type = Type;
	SIMLIB_LibData.DistanceSensorData.ID = ID;
	SIMLIB_LibData.DistanceSensorData.Orientation = Orientation;
	SIMLIB_LibData.DistanceSensorData.Covariance = Covariance;
	SIMLIB_LibData.DistanceSensorData.DataState = SIMLIB_OK_FRESH;

	OS_MutSemGive(SIMLIB_LibData.MutexID);

	return SIMLIB_OK;
}


int32 SIMLIB_SendHeartbeat(void)
{
	int32 iStatus = SIMLIB_OK_NO_VALUE;
	uint32 i = 0;
	mavlink_message_t msg = {};
	uint8 buffer[MAVLINK_MAX_PACKET_LEN];
	mavlink_heartbeat_t heartbeatMsg = {};
	uint32 length = 0;

	heartbeatMsg.type = MAV_TYPE_GENERIC;
	heartbeatMsg.autopilot = MAV_AUTOPILOT_GENERIC;
	heartbeatMsg.base_mode = 0; //MAV_MODE_FLAG_DECODE_POSITION_SAFETY + MAV_MODE_FLAG_DECODE_POSITION_CUSTOM_MODE;
	heartbeatMsg.custom_mode = 0;
	heartbeatMsg.system_status = MAV_STATE_ACTIVE;
	heartbeatMsg.mavlink_version = 1;

	mavlink_msg_heartbeat_encode(1, 1, &msg, &heartbeatMsg);
	length = mavlink_msg_to_send_buffer(buffer, &msg);

	if(SIMLIB_LibData.Socket != 0)
	{
		send(SIMLIB_LibData.Socket, (char *)buffer, length, 0);
	}

	iStatus = SIMLIB_OK;

end_of_function:
    return iStatus;
}


/************************/
/*  End of File Comment */
/************************/
