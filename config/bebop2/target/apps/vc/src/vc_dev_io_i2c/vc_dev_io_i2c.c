/****************************************************************************
*
*   Copyright (c) 2017 Windhover Labs, L.L.C. All rights reserved.
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
* 3. Neither the name Windhover Labs nor the names of its
*    contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
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
*****************************************************************************/

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "vc_dev_io_i2c.h"
#include "vc_mt9v117_patch.h"
#include "cfe.h"
#include <string.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <linux/v4l2-subdev.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Definitions
*************************************************************************/

int32 MT9V117_Init_Device(void)
{
	int32 returnCode = 0;

	/* Initialize i2c file descriptor */
	I2C_FD = -1;

	/* Set device clock frequency and reset pwm */
	returnCode += Write_ToFile("/sys/class/pwm/pwm_9/period_ns", 23);
	returnCode += Write_ToFile("/sys/class/pwm/pwm_9/run", 0);
	returnCode += Write_ToFile("/sys/class/pwm/pwm_9/run", 1);

	if(returnCode < 0)
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"Failed to set PWM clock frequency");
		returnCode = -1;
		goto end_of_function;
	}

	/* Reset gpio pin at 129 */
	returnCode += Write_ToFile("/sys/class/gpio/gpio129/value", 0);
	returnCode += Write_ToFile("/sys/class/gpio/gpio129/value", 1);

	if(returnCode < 0)
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"Failed to reset GPIO129 pin");
		returnCode = -1;
		goto end_of_function;
	}

	usleep(50000);

	/* Start i2c device */
	returnCode += I2C_Start();
	if(returnCode < 0)
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"Failed to start I2C device");
		returnCode = -1;
		goto end_of_function;
	}

	/* Set slave*/
	if(ioctl(I2C_FD,I2C_SLAVE,MT9V117_SLAVE_ADDRESS) != 0){
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"I2C slave configuration failed");
		returnCode = -1;
		goto end_of_function;
	}

	/* Initalize mt9v117 */
	returnCode += MT9V117_Probe();
	if (returnCode < 0)
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"MT9V117 image sensor not detected");
		returnCode = -1;
		goto end_of_function;
	}

	returnCode += MT9V117_SoftReset();
	if (returnCode < 0)
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"MT9V117 image sensor soft reset failed");
		returnCode = -1;
		goto end_of_function;
	}

	usleep(50000);

	returnCode += MT9V117_WritePatch();
	if (returnCode < 0)
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"MT9V117 image sensor write patch failed");
		returnCode = -1;
		goto end_of_function;
	}

	returnCode += MT9V117_WriteSettings();
	if (returnCode < 0)
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"MT9V117 image sensor write settings failed");
		returnCode = -1;
		goto end_of_function;
	}

	returnCode += MT9V117_ConfigureSensor();
	if (returnCode < 0)
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"MT9V117 image sensor configuration failed");
		returnCode = -1;
		goto end_of_function;
	}

	/* set format */
	returnCode += MT9V117_SetFormat();
	if (returnCode < 0)
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"MT9V117 set format failed");
		returnCode = -1;
		goto end_of_function;
	}

	end_of_function:
		return(returnCode);
}

int32 I2C_Start()
{
	int32 returnCode = 0;

	I2C_FD = open(DEVICE_I2C_PATH_A, O_RDWR);

	if (I2C_FD < 0) {
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
						"Failed to open file at %s", DEVICE_I2C_PATH_A);
		returnCode = -1;
	}

	return (returnCode);
}

int32 Write_ToFile(const char * path, int value)
{
	int32 returnCode = 0;

	char filename[sizeof(path) + 40] = {0};
	snprintf(filename, sizeof(filename), "%s", path);

	FILE * fd = fopen(filename,"w");

	if (fd == nullptr)
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"Unable to open file: %s", filename);
		returnCode = -1;
		goto end_of_function;
	}

	if (fprintf(fd, "%d", value) < 0)
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"Unable to write to file: %s", filename);
		returnCode = -1;
	}

	if (fclose(fd) != 0)
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"Unable to close file: %s", filename);
		returnCode = -1;
		goto end_of_function;
	}

	end_of_function:
	    return (returnCode);
}

int32 Read_FromReg(uint8 *out_buffer, size_t length)
{
	int32 returnCode      = 0;

	if (I2C_FD < 0)
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"Failed to open file descriptor");
		returnCode = -1;
		goto end_of_function;
	}

	ssize_t bytes_read = 0;
	bytes_read = read(I2C_FD, out_buffer, length);

	if (bytes_read != (ssize_t)length)
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"Read register reports a read of %zd bytes, but attempted to set %zd bytes", bytes_read, length);
		returnCode = -1;
	}

	end_of_function:
		return(returnCode);
}

int32 Read_16_FromReg(uint16 address, uint16 *out_buffer, size_t length)
{
	int32 returnCode = 0;

	if (I2C_FD < 0)
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"Failed to open file descriptor at %u", address);
		returnCode = -1;
		goto end_of_function;
	}

	returnCode = Write_16_ToReg(address, nullptr, 0);

	if (returnCode < 0)
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"Failed to write at %u", address);
		returnCode  = -1;
		goto end_of_function;
	}

	returnCode = Read_FromReg((uint8 *)out_buffer, length);

	if (returnCode < 0)
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"Failed to read at %u", address);
		returnCode  = -1;
	}

	end_of_function:
		return(returnCode);

}

int32 Write_16_ToReg(uint16 address, uint16 *in_buffer, size_t length)
{
	int32 returnCode = 0;
	int total_retries = 0;
	unsigned retry_count = 0;
	uint8 write_buffer[length + 2];

	if (I2C_FD < 0)
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"Failed to open file descriptor at %u", address);
		returnCode = -1;
		goto end_of_function;
	}

	if (in_buffer)
	{
		memcpy(&write_buffer[2], in_buffer, length);
	}

	/* Save the address of the register to read from in the write buffer for the combined write. */
	write_buffer[0] = (uint8)(address & 0xFF);
	write_buffer[1] = (uint8)(address >> 8);

	/*
	 * Verify that the length of the caller's buffer does not exceed the local stack
	 * buffer with one additional byte for the register ID.
	 */
	if (length + 2 > MAX_LEN_TRANSMIT_BUFFER_IN_BYTES)
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"Caller's buffer exceeds max len");
		returnCode  = -1;
		goto end_of_function;
	}

	do
	{
		ssize_t bytes_written = write(I2C_FD, (char *) write_buffer, length + 2);

		if (bytes_written != (ssize_t)length + 2)
		{
			(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
					"I2C write failed. Reported %zd bytes written\n",bytes_written);
			returnCode  = -1;
			goto end_of_function;
		}
	}
	while (retry_count++ < total_retries);


	end_of_function:
		return (returnCode);
}

int32 Read16(uint16 add, uint16 *val)
{
	int32 returnCode = 0;
	uint16 read_val;
	returnCode = Read_16_FromReg(swap16(add), &read_val, 2);
	*val = swap16(read_val);
	return (returnCode);
}

int32  write8(uint16 add, uint8 val)
{
	int32 returnCode = 0;
	uint8 tx = val;
	returnCode = Write_16_ToReg(swap16(add), (uint16 *) &tx, 1);
	return (returnCode);
}

int32  Write16(uint16 add, uint16 val)
{
	int32 returnCode = 0;
	uint16 buf = swap16(val);
	returnCode = Write_16_ToReg(swap16(add), &buf, 2);
	return (returnCode);
}

int32  Write32(uint16 add, uint32 val)
{
	int32 returnCode = 0;
	uint32 buf = swap32(val);
	returnCode = Write_16_ToReg(swap16(add), (uint16 *)&buf, 4);
	return (returnCode);
}

int32 MT9V117_Probe()
{
	int32 returnCode = 0;
	uint16 id = 0;

	returnCode = Read16(MT9V117_REG_ID, &id);

	if (returnCode < 0)
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"Unable to read whoami reg");
		returnCode = -1;
		goto end_of_function;
	}

	if (id != MT9V117_WHOAMI)
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"Wrong WHOAMI %x (expected %x)", id, MT9V117_WHOAMI);
		returnCode = -1;
	}

	end_of_function:
		return(returnCode);
}

int32 MT9V117_SoftReset()
{
	int32 returnCode = 0;

	uint16 tx = MT9V117_RESET;
	returnCode = Write16(MT9V117_REG_RESET, tx);

	if (returnCode < 0)
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"Unable to soft reset for %s", DEVICE_NAME_A);
		returnCode = -1;
		goto end_of_function;
	}

	returnCode = Write16(MT9V117_REG_RESET, 0);

	if (returnCode < 0)
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"Unable to restart after soft reset for %s", DEVICE_NAME_A);
		returnCode = -1;
		goto end_of_function;
	}
	usleep(50000);

	end_of_function:
			return(returnCode);
}

int32 MT9V117_WritePatch()
{
	int32 returnCode = 0;

	/* Errata item 2 */
	returnCode += Write16(0x301a, 0x10d0);
	returnCode += Write16(0x31c0, 0x1404);
	returnCode += Write16(0x3ed8, 0x879c);
	returnCode += Write16(0x3042, 0x20e1);
	returnCode += Write16(0x30d4, 0x8020);
	returnCode += Write16(0x30c0, 0x0026);
	returnCode += Write16(0x301a, 0x10d4);

	/* Errata item 6 */
	returnCode += Write16(to_register(MT9V117_VAR_AE_TRACK, 0x0002), 0x00d3);
	returnCode += Write16(to_register(MT9V117_VAR_CAM_CTRL, 0x0078), 0x00a0);
	returnCode += Write16(to_register(MT9V117_VAR_CAM_CTRL, 0x0076), 0x0140);

	/* Errata item 8 */
	returnCode += Write16(to_register(MT9V117_VAR_LOW_LIGHT, 0x0004), 0x00fc);
	returnCode += Write16(to_register(MT9V117_VAR_LOW_LIGHT, 0x0038), 0x007f);
	returnCode += Write16(to_register(MT9V117_VAR_LOW_LIGHT, 0x003a), 0x007f);
	returnCode += Write16(to_register(MT9V117_VAR_LOW_LIGHT, 0x003c), 0x007f);
	returnCode += Write16(to_register(MT9V117_VAR_LOW_LIGHT, 0x0004), 0x00f4);

	/* Patch 0403:
	 * Critical sensor optimization
	 */
	returnCode += Write16(MT9V117_REG_ACCESS_CTL, 0x0001);
	returnCode += Write16(MT9V117_PHYSICAL_ADDRESS_ACCESS, 0x7000);

	/* Write patch lines */
	size_t i;
	for (i = 0; i < MT9V117_PATCH_LINE_NUM; ++i)
	{
		int bytes_written = write(I2C_FD, (uint8 *) MT9V117_patch[i].data, MT9V117_patch[i].size);

		if (bytes_written != MT9V117_patch[i].size)
		{
			(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
					"Patch line transmission failed for %s", DEVICE_NAME_A);
			returnCode += -1;
		}
	}
	returnCode += Write16(MT9V117_LOGICAL_ADDRESS_ACCESS, 0x0000);

	returnCode += Write16(to_register(MT9V117_VAR_PATCHLDR, 0x00), 0x05d8);
	returnCode += Write16(to_register(MT9V117_VAR_PATCHLDR, 0x02), 0x0403);
	returnCode += Write32(to_register(MT9V117_VAR_PATCHLDR, 0x04), 0x00430104);
	returnCode += Write16(MT9V117_REG_CMD, (HOST_CMD_OK | HOST_CMD_0));

	returnCode += MT9V117_Check_ConfigChange(HOST_CMD_0);

	return(returnCode);
}

int32 MT9V117_WriteSettings()
{
	int32 returnCode = 0;

	returnCode += Write32(to_register(MT9V117_VAR_AWB, 0x40), 50000);
	returnCode += Write16(to_register(MT9V117_VAR_AE_RULE, 0x04), MT9V117_AE_RULE_AVERAGE);

	uint16 pad_slew;
	returnCode = Read16(MT9V117_REG_PAD_SLEW, &pad_slew);

	if (returnCode == 0)
	{
		uint16 new_pad_slew = pad_slew | 0x0600 | 0x0001;
		returnCode += Write16(MT9V117_REG_PAD_SLEW, new_pad_slew);

	}
	else
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"Write Settings Read error for %s", DEVICE_NAME_A);
		returnCode  += -1;
	}

	return(returnCode);
}

int32 MT9V117_ConfigureSensor()
{
	int32 returnCode = 0;

	returnCode += Write16(to_register(MT9V117_VAR_CAM_CTRL, 0x02), 16);  /* X address startoffset */
	returnCode += Write16(to_register(MT9V117_VAR_CAM_CTRL, 0x06), 663); /* X address end */
	returnCode += Write16(to_register(MT9V117_VAR_CAM_CTRL, 0x00), 8);   /* Y address start */
	returnCode += Write16(to_register(MT9V117_VAR_CAM_CTRL, 0x04), 501); /* Y address end */
	returnCode += Write16(to_register(MT9V117_VAR_CAM_CTRL, 0x14), 243); /* cpipe last row */
	returnCode += Write16(to_register(MT9V117_VAR_CAM_CTRL, 0x0E), 283); /* frame length line */
	returnCode += Write16(to_register(MT9V117_VAR_CAM_CTRL, 0x28), 4);   /* read mode: y skip enabled */
	returnCode += Write16(to_register(MT9V117_VAR_CAM_CTRL, 0x1A), 1);   /* max FDZONE 60 */
	returnCode += Write16(to_register(MT9V117_VAR_CAM_CTRL, 0x1E), 1);   /* target FDZONE 60 */

	returnCode += write8(MT9V117_REG_AE_TRACK_JUMP_DIVISOR, 0x03);
	returnCode += write8(MT9V117_REG_CAM_AET_SKIP_FRAMES, 0x02);

	returnCode += Write16(to_register(MT9V117_VAR_CAM_CTRL, 0x54), 320); /* frame width */
	returnCode += Write16(to_register(MT9V117_VAR_CAM_CTRL, 0x56), 240); /* frame height */

	/* Set gain metric for 90 fps */
	returnCode += Write16(to_register(MT9V117_VAR_CAM_CTRL, 0x116), 0x03e8); /* low light start gain metric */
	returnCode += Write16(to_register(MT9V117_VAR_CAM_CTRL, 0x118), 0x1770); /* low light stop gain metric */

	/* set crop window */
	returnCode += Write16(to_register(MT9V117_VAR_CAM_CTRL, 0x48), 0); /* crop window X offset */
	returnCode += Write16(to_register(MT9V117_VAR_CAM_CTRL, 0x4A), 0); /* crop window Y offset */
	returnCode += Write16(to_register(MT9V117_VAR_CAM_CTRL, 0x4C), 640); /* crop window width */
	returnCode += Write16(to_register(MT9V117_VAR_CAM_CTRL, 0x4E), 240); /* crop window height */

	/* Enable auto-stats mode */
	returnCode += write8(to_register(MT9V117_VAR_CAM_CTRL, 0x50), 3); /* crop mode */
	returnCode += Write16(to_register(MT9V117_VAR_CAM_CTRL, 0xF0), 319); /* AWB HG window X end */
	returnCode += Write16(to_register(MT9V117_VAR_CAM_CTRL, 0xF2), 239); /* AWB HG window Y end */
	returnCode += Write16(to_register(MT9V117_VAR_CAM_CTRL, 0xF4), 2); /* AE initial window X start */
	returnCode += Write16(to_register(MT9V117_VAR_CAM_CTRL, 0xF6), 2); /* AE initial window Y start */
	returnCode += Write16(to_register(MT9V117_VAR_CAM_CTRL, 0xF8), 65); /* AE initial window X end */
	returnCode += Write16(to_register(MT9V117_VAR_CAM_CTRL, 0xFA), 49); /* AE initial window Y end */

	uint16 itu656;

	if (Read16(to_register(MT9V117_VAR_CAM_CTRL, 0x58), &itu656) == 0)
	{
		uint16 new_itu656 = itu656 | MT9V117_OUTPUT_FORMAT_BT656_ENABLE;
		returnCode += Write16(to_register(MT9V117_VAR_CAM_CTRL, 0x58), new_itu656); /* output format */

	}
	else
	{
		returnCode += -1;
	}

	returnCode += write8(to_register(MT9V117_VAR_SYSMGR, 0x00), 0x28); /* next state */

	returnCode += Write16(MT9V117_REG_CMD, HOST_CMD_OK | HOST_CMD_1);

	returnCode += MT9V117_Check_ConfigChange(HOST_CMD_1);

	return(returnCode);
}

int32 MT9V117_Check_ConfigChange(uint16 new_state)
{
	int32 returnCode = 0;
	uint16 retries = 10;
	uint16 state = 0;

	int i = 0;
	for (i = 0; i < retries; ++i)
	{
		returnCode = Read16(MT9V117_REG_CMD, &state);

		if (returnCode == 0 && (state & new_state) == 0)
		{
			break;
		}
		/* 15ms */
		usleep(15000);
	}

	if (i >= retries)
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"Timeout: Config change failed for %s", DEVICE_NAME_A);
		returnCode = -1;
		goto end_of_function;
	}

	if ((state & HOST_CMD_OK) == 0)
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"Config change failed for %s", DEVICE_NAME_A);
		returnCode = -2;
	}

	end_of_function:
		return(returnCode);
}

int32 MT9V117_SetFormat()
{
	int32 returnCode = 0;
	struct v4l2_subdev_format fmt;

	int fd;
	fd = open(DEVICE_PATH_A, O_RDWR,0);

	if (fd < 0)
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"Failed to open file descriptor for path %s", DEVICE_PATH_A);
		returnCode = -1;
		goto end_of_function;
	}

	memset(&fmt, 0, sizeof(fmt));
	fmt.pad = 0;
	fmt.which = V4L2_SUBDEV_FORMAT_ACTIVE;
	fmt.format.width = 320;
	fmt.format.height = 240;
	fmt.format.code = V4L2_MBUS_FMT_UYVY8_2X8;
	fmt.format.field = V4L2_FIELD_NONE;
	fmt.format.colorspace = 1;

	returnCode = ioctl(fd, VIDIOC_SUBDEV_S_FMT, &fmt);
	close(fd);

	if (returnCode < 0)
	{
		(void) CFE_EVS_SendEvent(VC_DEVICE_ERR_EID, CFE_EVS_ERROR,
				"Set format failed for %s", DEVICE_NAME_A);
		returnCode = -1;
	}

	end_of_function:
	    return (returnCode);
}






