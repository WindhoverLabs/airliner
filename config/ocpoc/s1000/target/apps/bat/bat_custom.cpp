#include "bat_app.h"
#include <unistd.h>

#define BAT_VOLTAGE_PATH "/sys/bus/iio/devices/iio:device0/in_voltage8_raw"

/* 28V / 4096 counts = 0.0068359375f V per count */
#define BAT_BATTERY1_V_DIV   (0.0068359375f)


int32 BAT::InitDevice(void)
{
	int32 ret = CFE_SUCCESS;
	FILE *FD;

	FD = fopen(BAT_VOLTAGE_PATH, "r");

	if (FD == NULL)
	{
		ret = -1;
	}
	else
	{
		fclose(FD);
	}

	return ret;
}


void BAT::CloseDevice(void)
{
}


int32 BAT::ReadDevice(float &Voltage, float &Current)
{
	int32 ret = CFE_SUCCESS;
	FILE *FD;

	FD = fopen(BAT_VOLTAGE_PATH, "r");

	if (FD == NULL)
	{
		Voltage = 0.0f;
		Current = 0.0f;
		ret = -1;
	}
	else
	{
		int ret_tmp = fscanf(FD, "%f", (float*)&Voltage);
		if(ret_tmp < 0)
		{
			ret = ret_tmp;
		}

		Voltage *= BAT_BATTERY1_V_DIV;

		fclose(FD);
	}

	usleep(10000);

	Current = 0.0f;

	return ret;
}
