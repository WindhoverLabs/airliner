#include "ulr_app.h"
#include <unistd.h>
#include "px4_msgs.h"
#include "simlib.h"

#define ULR_CUSTOM_BUF_LEN 6

int32 ULR::InitDevice(void)
{
	return 0;
}


int32 ULR::ReadDevice(uint8 *Buffer, uint32 *Size)
{
	uint16 MinDistance = 0;
	uint16 MaxDistance = 0;
	uint16 CurrentDistance = 0;
	PX4_DistanceSensorType_t SensorType = PX4_DISTANCE_SENSOR_RADAR;
	uint8  SensorID = 0;
	PX4_SensorOrientation_t  SensorOrientation = PX4_SENSOR_ORIENTATION_NONE;
	uint8  Covariance = 0;
	int32 Status = CFE_SUCCESS;
    uint16 distanceInCm = 0;
    uint32 i = 0;

	OS_TaskDelay(4);

	if((Buffer == 0) || (Size == 0))
	{
		Status = -1;
		goto end_of_function;
	}

	if(*Size < ULR_CUSTOM_BUF_LEN)
	{
		Status = -1;
		goto end_of_function;
	}

	SIMLIB_GetDistanceSensor(
		&MinDistance,
		&MaxDistance,
		&CurrentDistance,
		&SensorType,
		&SensorID,
		&SensorOrientation,
		&Covariance);

//	OS_printf("***************************\n");
//	OS_printf("Minimum = %u\n", MinDistance);
//	OS_printf("Maximum = %u\n", MaxDistance);
//	OS_printf("Current = %u\n", CurrentDistance);
//	OS_printf("SensorType = %u\n", SensorType);
//	OS_printf("SensorID = %u\n", SensorID);
//	OS_printf("SensorOrientation = %u\n", SensorOrientation);
//	OS_printf("Covariance = %u\n", Covariance);

	/* Message sync symbol. */
	Buffer[0] = 0xfe;

	/* Version ID. */
    Buffer[1] = 0x01;

    /* Altitude byte 1. */
    if(CurrentDistance > MaxDistance)
    {
    	CurrentDistance = MaxDistance;
    }
    if(CurrentDistance < MinDistance)
    {
    	CurrentDistance = MinDistance;
    }
    Buffer[2] = (uint8)(CurrentDistance & 0x00ff);
    Buffer[3] = (uint8)((CurrentDistance & 0xff00) >> 8);

    /* Signal to Noise Ratio.  Just map the SNR to the ratio of distance between Min and Max distance. */
	if((MaxDistance - MinDistance) == 0)
	{
		/* Set SNR to 0 to avoid divide by zero error. */
		Buffer[4] = 0;
	}
	else
	{
		Buffer[4] = (CurrentDistance - MinDistance) / (MaxDistance - MinDistance);
	}

    /* Checksum. */
    Buffer[5] = 0;

    Buffer[5] = (Buffer[1] + Buffer[2] + Buffer[3] + Buffer[4]) & 0xFF;

    *Size = ULR_CUSTOM_BUF_LEN;

end_of_function:
	return Status;
}
