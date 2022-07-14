#include "cfe.h"
#include "mpu6050_custom.h"
#include "mpu6050_app.h"
#include "simlib.h"
#include <time.h>
#include "px4lib.h"


extern MPU6050 oMPU6050;

void MPU6050_Custom_InitData(void)
{

}


boolean MPU6050_Custom_Init(void)
{
	return TRUE;
}


boolean MPU6050_Custom_Uninit(void)
{
	return TRUE;
}


int32 MPU6050_Custom_Init_EventFilters(int32 ind, CFE_EVS_BinFilter_t *EventTbl)
{
	return 0;
}


boolean MPU6050_SetAccScale(uint8 Scale, float *AccDivider)
{
    boolean returnBool = TRUE;
    uint8 value = 0;

    /* Null pointer check */
    if(0 == AccDivider)
    {
        returnBool = FALSE;
        goto end_of_function;
    }

    switch (Scale)
    {
        case 2:
            *AccDivider = 16384;
            break;

        case 4:
            *AccDivider = 8192;
            break;

        case 8:
            *AccDivider = 4096;
            break;

        case 16:
            *AccDivider = 2048;
            break;

        default:
            returnBool = FALSE;
            goto end_of_function;
    }

end_of_function:
    return returnBool;
}


boolean MPU6050_SetGyroScale(uint32 Scale, float *GyroDivider)
{
    boolean returnBool = TRUE;

    /* Null pointer check */
    if(0 == GyroDivider)
    {
        returnBool = FALSE;
        goto end_of_function;
    }

    switch (Scale)
    {
        case 250:
            *GyroDivider = 131;
            break;

        case 500:
            *GyroDivider = 65.5;
            break;

        case 1000:
            *GyroDivider = 32.8;
            break;

        case 2000:
            *GyroDivider = 16.4;
            break;

        default:
            returnBool = FALSE;
            goto end_of_function;
    }

end_of_function:
    return returnBool;
}


boolean MPU6050_Measure(MPU6050_SampleQueue_t *SampleQueue)
{
    float calGX_f = 0.0f;
	float calGY_f = 0.0f;
	float calGZ_f = 0.0f;
    float calAX_f = 0.0f;
	float calAY_f = 0.0f;
	float calAZ_f = 0.0f;
    float calTemp = 0.0f;

    // Temp
	SIMLIB_GetTemp(&calTemp);

    if(0 == calTemp)
    {
        SampleQueue->Samples[0].Temp = -1083;
    }
    else
    {
        SampleQueue->Samples[0].Temp = ((calTemp + oMPU6050.Diag.Conversion.RoomTempOffset) - 21.0f) * oMPU6050.Diag.Conversion.TempSensitivity;
    }

    // Accel
	SIMLIB_GetAccel(&calAX_f, &calAY_f, &calAZ_f);

    /* Apply inverse rotation */
    MPU6050_Apply_Platform_Rotation(&calAX_f, &calAY_f, &calAZ_f);

    SampleQueue->Samples[0].AX = ((calAX_f / oMPU6050.Diag.Calibration.AccXScale) + oMPU6050.Diag.Calibration.AccXOffset) / (oMPU6050.Diag.Conversion.AccUnit / oMPU6050.Diag.Conversion.AccDivider);
    SampleQueue->Samples[0].AY = ((calAY_f / oMPU6050.Diag.Calibration.AccYScale) + oMPU6050.Diag.Calibration.AccYOffset) / (oMPU6050.Diag.Conversion.AccUnit / oMPU6050.Diag.Conversion.AccDivider);
    SampleQueue->Samples[0].AZ = ((calAZ_f / oMPU6050.Diag.Calibration.AccZScale) + oMPU6050.Diag.Calibration.AccZOffset) / (oMPU6050.Diag.Conversion.AccUnit / oMPU6050.Diag.Conversion.AccDivider);

    // Gyro
	SIMLIB_GetGyro(&calGX_f, &calGY_f, &calGZ_f);
    
    /* Apply inverse rotation */
    MPU6050_Apply_Platform_Rotation(&calGX_f, &calGY_f, &calGZ_f);

    SampleQueue->Samples[0].GX = ((calGX_f / oMPU6050.Diag.Calibration.GyroXScale) + oMPU6050.Diag.Calibration.GyroXOffset) / (oMPU6050.Diag.Conversion.GyroUnit / oMPU6050.Diag.Conversion.GyroDivider);
    SampleQueue->Samples[0].GY = ((calGY_f / oMPU6050.Diag.Calibration.GyroYScale) + oMPU6050.Diag.Calibration.GyroYOffset) / (oMPU6050.Diag.Conversion.GyroUnit / oMPU6050.Diag.Conversion.GyroDivider);
    SampleQueue->Samples[0].GZ = ((calGZ_f / oMPU6050.Diag.Calibration.GyroZScale) + oMPU6050.Diag.Calibration.GyroZOffset) / (oMPU6050.Diag.Conversion.GyroUnit / oMPU6050.Diag.Conversion.GyroDivider);

    SampleQueue->SampleCount = 1;
    
    return TRUE;
}

boolean MPU6050_Read_Gyro(int16 *rawX, int16 *rawY, int16 *rawZ)
{
	float calX_f = 0.0f;
	float calY_f = 0.0f;
	float calZ_f = 0.0f;

	SIMLIB_GetGyro(&calX_f, &calY_f, &calZ_f);
    
    /* Apply inverse rotation */
    MPU6050_Apply_Platform_Rotation(&calX_f, &calY_f, &calZ_f);

    *rawX = ((calX_f / oMPU6050.Diag.Calibration.GyroXScale) + oMPU6050.Diag.Calibration.GyroXOffset) / (oMPU6050.Diag.Conversion.GyroUnit / oMPU6050.Diag.Conversion.GyroDivider);
    *rawY = ((calY_f / oMPU6050.Diag.Calibration.GyroYScale) + oMPU6050.Diag.Calibration.GyroYOffset) / (oMPU6050.Diag.Conversion.GyroUnit / oMPU6050.Diag.Conversion.GyroDivider);
    *rawZ = ((calZ_f / oMPU6050.Diag.Calibration.GyroZScale) + oMPU6050.Diag.Calibration.GyroZOffset) / (oMPU6050.Diag.Conversion.GyroUnit / oMPU6050.Diag.Conversion.GyroDivider);



	return TRUE;
}


boolean MPU6050_Read_Accel(int16 *rawX, int16 *rawY, int16 *rawZ)
{
	float calX_f = 0.0f;
	float calY_f = 0.0f;
	float calZ_f = 0.0f;

	SIMLIB_GetAccel(&calX_f, &calY_f, &calZ_f);

    /* Apply inverse rotation */
    MPU6050_Apply_Platform_Rotation(&calX_f, &calY_f, &calZ_f);

    *rawX = ((calX_f / oMPU6050.Diag.Calibration.AccXScale) + oMPU6050.Diag.Calibration.AccXOffset) / (oMPU6050.Diag.Conversion.AccUnit / oMPU6050.Diag.Conversion.AccDivider);
    *rawY = ((calY_f / oMPU6050.Diag.Calibration.AccYScale) + oMPU6050.Diag.Calibration.AccYOffset) / (oMPU6050.Diag.Conversion.AccUnit / oMPU6050.Diag.Conversion.AccDivider);
    *rawZ = ((calZ_f / oMPU6050.Diag.Calibration.AccZScale) + oMPU6050.Diag.Calibration.AccZOffset) / (oMPU6050.Diag.Conversion.AccUnit / oMPU6050.Diag.Conversion.AccDivider);

	return TRUE;
}

boolean MPU6050_Read_Temp(uint16 *rawTemp)
{
	float calTemp = 0.0f;

	SIMLIB_GetTemp(&calTemp);

    if(0 == calTemp)
    {
        *rawTemp = -1083;
    }
    else
    {
        *rawTemp = ((calTemp + oMPU6050.Diag.Conversion.RoomTempOffset) - 21.0f) * oMPU6050.Diag.Conversion.TempSensitivity;
    }

	return TRUE;
}


boolean MPU6050_Read_ImuStatus(boolean *WOM, boolean *FifoOvflw, boolean *Fsync, boolean *DataReady)
{
    /* Function is unused in platform independent code. */
	return TRUE;
}


boolean MPU6050_Read_MagStatus(boolean *Overrun, boolean *DataReady, boolean *Overflow, boolean *Output16Bit)
{
	/* Function is unused in platform independent code. */
	return TRUE;
}


boolean MPU6050_Read_MagAdj(uint8 *X, uint8 *Y, uint8 *Z)
{
	/* Function is unused in platform independent code. */
	*X = 170;
	*Y = 170;
	*Z = 170;

	return TRUE;
}


boolean MPU6050_Apply_Platform_Rotation(float *X, float *Y, float *Z)
{
    boolean returnBool = TRUE;
    float temp;

    /* Null pointer check */
    if(0 == X || 0 == Y || 0 == Z)
    {
        returnBool = FALSE;
        goto end_of_function;
    }
    /* TODO move to a table */
    /* ROTATION_ROLL_180_YAW_90 */
    temp = *X;
    *X = *Y;
    *Y = temp;
    *Z = -*Z;

end_of_function:

    return returnBool;
}


boolean MPU6050_Read_WhoAmI(uint8 *Value)
{
	*Value = MPU6050_DEVICE_ID;

	return TRUE;
}


boolean MPU6050_Read_MagDeviceID(uint8 *Value)
{
	/* Function is unused in platform independent code. */

	return TRUE;
}


void MPU6050_Get_Rotation(uint8 *Rotation)
{
    
    /* Null pointer check */
    if(0 == Rotation)
    {
        goto end_of_function;
    }
    
    /* TODO move to a table */
    *Rotation = ROTATION_ROLL_180_YAW_90;

end_of_function:
;
}

