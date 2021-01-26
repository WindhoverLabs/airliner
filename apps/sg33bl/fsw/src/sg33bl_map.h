/****************************************************************************
*
*   Copyright (c) 2019 Windhover Labs, L.L.C. All rights reserved.
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

#ifndef SG33BL_MAP_H
#define SG33BL_MAP_H

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    /* Product information. */
    /** \brief Read the version product number. */
    SG33BL_REG_PRODUCT_NO           = 0x00,
    /** \brief Read the version. */
    SG33BL_REG_PRODUCT_VERSION      = 0x02,
    /** \brief Read the firmware version. */
    SG33BL_REG_FIRMWARE_VERSION     = 0x04,
    /** \brief Read the sub-serial number. */
    SG33BL_REG_SERIAL_NO_SUB        = 0x06,
    /** \brief Read the main serial number. */
    SG33BL_REG_SERIAL_NO_MAIN       = 0x08,
    /* Status. */
    /** \brief Read the system status flag. */
    SG33BL_REG_STATUS_FLAG          = 0x0A,
    /** \brief Read the current position. */
    SG33BL_REG_POSITION             = 0x0C,
    /** \brief Read the velocity. */
    SG33BL_REG_VELOCITY             = 0x0E,
    /** \brief Read the torque. */
    SG33BL_REG_TORQUE               = 0x10,
    /** \brief Read the input voltage (V). */
    SG33BL_REG_VOLTAGE              = 0x12,
    /** \brief Read the MCU temperature (C). */
    SG33BL_REG_MCU_TEMP             = 0x14,
    /** \brief Read the motor temperature (C). */
    SG33BL_REG_MOTOR_TEMP           = 0x16,
    /** \brief Read the accumulated operating time (minute). */
    SG33BL_REG_WORKING_TIME         = 0x1A,
    /** \brief Read the current humidity (%). */
    SG33BL_REG_HUMIDITY             = 0x3C,
    /** \brief Read the current maximum humidity (%). */
    SG33BL_REG_HUMIDITY_MAX         = 0x40,
    /** \brief Read the current minimum humidity (%). */
    SG33BL_REG_HUMIDITY_MIN         = 0x42,
    /* Action. */
    /** \brief Set new position. */
    SG33BL_REG_POSITION_NEW         = 0x1E,
    /** \brief Set new velocity. */
    SG33BL_REG_VELOCITY_NEW         = 0x20,
    /** \brief Set new torque. */
    SG33BL_REG_TORQUE_NEW           = 0x22,
    /** \brief Set 360 rotation. */
    SG33BL_REG_360DEG_TURN_NEW      = 0x24,
    /* Config. */
    /** \brief Set the ID. */
    SG33BL_REG_ID                   = 0x32,
    /** \brief Set the baud rate (bps). */
    SG33BL_REG_BAUD_RATE            = 0x34,
    /** \brief Set the delay time until return packet is transmitted. */
    SG33BL_REG_NORMAL_RETURN_DELAY  = 0x3A,
    /** \brief Set the power management control. */
    SG33BL_REG_POWER_CONFIG         = 0x46,
    /** \brief Set the emergency stop control bit. */
    SG33BL_REG_EMERGENCY_STOP       = 0x48,
    /** \brief Set the action control bit. */
    SG33BL_REG_ACTION_MODE          = 0x4A,
    /** \brief Set the position slope. */
    SG33BL_REG_POSITION_SLOPE       = 0x4C,
    /** \brief Set the dead-band. */
    SG33BL_REG_DEAD_BAND            = 0x4E,
    /** \brief Set the maximum velocity. */
    SG33BL_REG_VELOCITY_MAX         = 0x54,
    /** \brief Set the maximum torque. */
    SG33BL_REG_TORQUE_MAX           = 0x56,
    /** \brief Set the maximum operating voltage. */
    SG33BL_REG_VOLTAGE_MAX          = 0x58,
    /** \brief Set the minimum operating voltage. */
    SG33BL_REG_VOLTAGE_MIN          = 0x5A,
    /** \brief Set the maximum operating temperature. */
    SG33BL_REG_TEMP_MAX             = 0x5C,
    /** \brief Set the minimum operating temperature. */
    SG33BL_REG_TEMP_MIN             = 0x5E,
    /** \brief Restore data to factory default. */
    SG33BL_REG_FACTORY_DEFAULT      = 0x6E,
    /** \brief Save changed data in memory. */
    SG33BL_REG_CONFIG_SAVE          = 0x70,
    /** \brief Set the end position. */
    SG33BL_REG_POS_END              = 0x94,
    /** \brief Set the start position. */
    SG33BL_REG_POS_START            = 0x96,
    /** \brief Set the neutral position. */
    SG33BL_REG_POS_NEUTRAL          = 0xC2
} SG33BL_Register_Map_t;


/* Bit mask. */
/* Status bits emergency stop. */
#define SG33BL_BITS_STATUS_MAX_VOLT (0x4000)
#define SG33BL_BITS_STATUS_MIN_VOLT (0x2000)
#define SG33BL_BITS_STATUS_MAX_TEMP (0x800)
#define SG33BL_BITS_STATUS_MIN_TEMP (0x400)

/* Power config bits. */
#define SG33BL_BITS_EMG_STOP_ON     (0x200)
#define SG33BL_BITS_TORQUE_ON       (0x2)
#define SG33BL_BITS_RESET           (0x1)

/* Action mode bits. */
#define SG33BL_BITS_CONT_ROTATION   (0x80)
#define SG33BL_BITS_VEL_TIME_MODE   (0x2)
#define SG33BL_BITS_ACCEL_MODE      (0x1)

/* Baud rate settings. */
#define SG33BL_SET_B9600            (0)
#define SG33BL_SET_B14400           (1)
#define SG33BL_SET_B19200           (2)
#define SG33BL_SET_B38400           (3)
#define SG33BL_SET_B57600           (4)
#define SG33BL_SET_B115200          (5)
#define SG33BL_SET_B230400          (6)

/* Factory default settings. */
#define SG33BL_SET_FACTORY_DEFAULT  (0x1515)

/* Save configuration settings. Don't use. */
#define SG33BL_SET_SAVE_CONFIG      (0xFFFF)

/* Header flags. */
#define SG33BL_HEADER_WRITE         (0x96)
#define SG33BL_HEADER_RESPONSE      (0x69)
#define SG33BL_HEADER_ID_NONE       (0x00)
#define SG33BL_HEADER_ID_BROADCAST  (0xFF)

/* Default baud rate. */
#define SG33BL_BAUD_DEFAULT         (115200)

/* Typical data return delay per spec sheet (us). */
#define SG33BL_DATA_RET_DELAY_TYP   (20000)

/* TODO Boot up time. */
#define SG33BL_STARTUP_TIME         (1000000)

#ifdef __cplusplus
}
#endif 

#endif /* SG33BL_MAP_H */
