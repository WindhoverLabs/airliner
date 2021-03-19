#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>


#define RCOUT_ZYNQ_PWM_BASE (0x43c00000)
#define FREQUENCY_PWM       (400)
#define TICK_PER_S          (50000000)
#define TICK_PER_US         (50)
#define DEVICE_PATH         "/dev/mem"
#define MAX_MOTOR_OUTPUTS   (8)
#define PWM_DISARMED		(900)


/* The following struct is used by the SharedMemCmd_t struct and overlayed
 * over the ocpoc PPM registers to control the PWM hardware.
 */
typedef struct
{
    unsigned int Period;
    unsigned int  Hi;
} PeriodHi_t;


/* The following struct is overlayed over the ocpoc PPM registers to control
 * the PWM hardware.
 */
typedef struct
{
    PeriodHi_t PeriodHi[MAX_MOTOR_OUTPUTS];
} SharedMemCmd_t;

volatile SharedMemCmd_t *SharedMemCmd;


void StopMotors(void);
unsigned int Freq2tick(unsigned short FreqHz);
void SetMotorOutputs(const unsigned short *PWM);


int main( int argc, const char* argv[] )
{
    unsigned int i = 0;
    int returnVal = 0;
    int fd = 0;

	printf("Stopping all motors.\n");

	/* Get the handle to the shared memory. */
    fd = open(DEVICE_PATH, O_RDWR | O_SYNC);
    SharedMemCmd = (SharedMemCmd_t *) mmap(0, 0x1000,
            PROT_READ | PROT_WRITE, MAP_SHARED, fd,
            RCOUT_ZYNQ_PWM_BASE);
    close(fd);

    if (SharedMemCmd <= 0)
    {
        returnVal = errno;
        goto end_of_function;
    }

    /* Note: this appears to be required actuators to initialize. */
    for (i = 0; i < MAX_MOTOR_OUTPUTS; ++i)
    {
        SharedMemCmd->PeriodHi[i].Period =
                Freq2tick(FREQUENCY_PWM);
        SharedMemCmd->PeriodHi[i].Hi     =
                Freq2tick(FREQUENCY_PWM) / 2;
    }

    /* Stop all the motors. */
    StopMotors();

end_of_function:
	return returnVal;
}


void StopMotors(void)
{
    unsigned short disarmed_pwm[MAX_MOTOR_OUTPUTS];

    for (unsigned int i = 0; i < MAX_MOTOR_OUTPUTS; ++i)
    {
        disarmed_pwm[i] = PWM_DISARMED;
    }

    SetMotorOutputs(disarmed_pwm);
}


unsigned int Freq2tick(unsigned short FreqHz)
{
	unsigned int duty = TICK_PER_S / (unsigned long)FreqHz;

    return duty;
}



void SetMotorOutputs(const unsigned short *PWM)
{
    unsigned int i = 0;

    if(SharedMemCmd != 0)
    {
		/* Convert this to duty_cycle in ns */
		for (i = 0; i < MAX_MOTOR_OUTPUTS; ++i)
		{
			SharedMemCmd->PeriodHi[i].Hi = TICK_PER_US * PWM[i];
		}
    }
}
