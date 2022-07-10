#include "bat_app.h"
#include <unistd.h>


int32 BAT::InitDevice(void)
{
	return 0;
}


void BAT::CloseDevice(void)
{
}


int32 BAT::ReadDevice(float &Voltage, float &Current)
{
	Voltage = 28.0f;
	Current = 0.0f;

	usleep(1000);

	return 0;
}
