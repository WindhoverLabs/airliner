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

#ifndef VC_DEV_IO_I2C_H
#define VC_DEV_IO_I2C_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "px4_msgs.h"
#include "px4lib.h"
#include "../vc_custom_shared.h"

/************************************************************************
** Local Defines
*************************************************************************/

#define MT9V117_BUS_FREQUENCY_IN_KHZ 400
#define MT9V117_TRANSFER_TIMEOUT_IN_USEC 500

#define MT9V117_REG_ID 0x00
#define MT9V117_REG_RESET 0x001A
#define MT9V117_REG_ACCESS_CTL 0x0982
#define MT9V117_PHYSICAL_ADDRESS_ACCESS 0x098A
#define MT9V117_LOGICAL_ADDRESS_ACCESS 0x098E
#define MT9V117_REG_PAD_SLEW 0x0030
#define MT9V117_REG_CMD 0x0040
#define MT9V117_REG_AE_TRACK_JUMP_DIVISOR 0xa812
#define MT9V117_REG_CAM_AET_SKIP_FRAMES 0xc868

#define MT9V117_WHOAMI 0x2282
#define MT9V117_RESET 0x0001
#define HOST_CMD_OK 0x8000
#define HOST_CMD_0 0x0001
#define HOST_CMD_1 0x0002
#define HOST_CMD_2 0x0004
#define MT9V117_AE_RULE_AVERAGE 0x00
#define MT9V117_AE_RULE_WEIGHTED 0x01

#define MT9V117_VAR_AE_RULE 0x9
#define MT9V117_VAR_AE_TRACK 0xA
#define MT9V117_VAR_AWB 0xB
#define MT9V117_VAR_CAM_CTRL 0x12
#define MT9V117_VAR_LOW_LIGHT 0xF
#define MT9V117_VAR_SYSMGR 0x17
#define MT9V117_VAR_PATCHLDR 0x18

#define MT9V117_OUTPUT_FORMAT_RGB_565       0x0
#define MT9V117_OUTPUT_FORMAT_RGB_555       0x1000
#define MT9V117_OUTPUT_FORMAT_RGB_444X      0x2000
#define MT9V117_OUTPUT_FORMAT_RGB_X444      0x3000
#define MT9V117_OUTPUT_FORMAT_BAYER_10      0x0
#define MT9V117_OUTPUT_FORMAT_YUV           0x0
#define MT9V117_OUTPUT_FORMAT_RGB           0x100
#define MT9V117_OUTPUT_FORMAT_BAYER         0x200
#define MT9V117_OUTPUT_FORMAT_BT656_ENABLE  0x08
#define MT9V117_OUTPUT_FORMAT_MONO_ENABLE   0x04
#define MT9V117_OUTPUT_FORMAT_SWAP_BYTES    0x02
#define MT9V117_OUTPUT_FORMAT_SWAP_RED_BLUE 0x01

/************************************************************************
** Function Prototypes
*************************************************************************/

/**
 * @brief Custom function to initialize MT9V117 device data.
 * @return 0 if successful -1 if failed
 */
int32 MT9V117_Init_Device(void);
/**
 * @brief Open I2C Driver.
 * @return 0 if successful -1 if failed
 */
int32 I2C_Start(void);
/**
 * @brief Writes data to specified files.
 * @return 0 if successful -1 if failed
 */
int32 Write_ToFile(const char * path, int value);
/**
 * @brief Performs simple read operation.
 * @return 0 if successful -1 if failed
 */
int32 Read_FromReg(uint8 *out_buffer, size_t length);
/**
 * @brief Performs 16-bit read operation.
 * @return 0 if successful -1 if failed
 */
int32 Read_16_FromReg(uint16 address, uint16 *out_buffer, size_t length);
/**
 * @brief Performs 16-bit write operation.
 * @return 0 if successful -1 if failed
 */
int32 Write_16_ToReg(uint16 address, uint16 *in_buffer, size_t length);
/**
 * @brief Performs 16-bit read, given address and value pointer.
 * @return 0 if successful -1 if failed
 */
int32 Read16(uint16 add, uint16 *val);
/**
 * @brief Performs 8-bit write, given address and value.
 * @return 0 if successful -1 if failed
 */
int32 Write8(uint16 add, uint8 val);
/**
 * @brief Performs 16-bit write, given address and value.
 * @return 0 if successful -1 if failed
 */
int32 Write16(uint16 add, uint16 val);
/**
 * @brief Performs 32-bit write, given address and value.
 * @return 0 if successful -1 if failed
 */
int32 Write32(uint16 add, uint32 val);
/**
 * @brief Verifies WHOAMI register value.
 * @return 0 if successful -1 if failed
 */
int32 MT9V117_Probe(void);
/**
 * @brief Soft resets MT9V117 device.
 * @return 0 if successful -1 if failed
 */
int32 MT9V117_SoftReset(void);
/**
 * @brief Applies MT9V117 device patch.
 * @return 0 if successful negative integer if failed
 */
int32 MT9V117_WritePatch(void);
/**
 * @brief Applies settings to MT9V117 device.
 * @return 0 if successful negative integer if failed
 */
int32 MT9V117_WriteSettings(void);
/**
 * @brief Configure MT9V117 device.
 * @return 0 if successful negative integer if failed
 */
int32 MT9V117_ConfigureSensor(void);
/**
 * @brief Check if patch has been applied properly for MT9V117 device.
 * @return 0 if successful negative integer if failed
 */
int32 MT9V117_Check_ConfigChange(uint16 new_state);
/**
 * @brief Set frame/image format for MT9V117 device.
 * @return 0 if successful negative integer if failed
 */
int32 MT9V117_SetFormat(void);

static uint16 swap16(uint16 val) { return (val >> 8) | (val << 8); }

static uint32 swap32(uint32 val) { return (val >> 24) | ((val >> 8) & 0x0000FF00) | ((val << 8) & 0x00FF0000) | (val << 24); }

static uint16 to_register(uint16 address, uint16 offset) {return (0x8000 | (address << 10) | offset);}

#endif /* VC_DEV_IO_I2C_H */

/************************/
/*  End of File Comment */
/************************/

