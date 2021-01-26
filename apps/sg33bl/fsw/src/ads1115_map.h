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
#ifndef ADS1115_MAP_H
#define ADS1115_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

/** \brief ADS1115 register map. */
typedef enum {
    /** \brief Conversion register. */
    ADS1115_REG_CONVERSION              = 0x00,
    /** \brief Config register. */
    ADS1115_REG_CONFIG                  = 0x01,
    /** \brief Lo threshold register. */
    ADS1115_REG_LO_THRESHOLD            = 0x02,
    /** \brief Hi threshold register. */
    ADS1115_REG_HI_THRESHOLD            = 0x03
} ADS1115_Register_Map_t;


/** \brief Configuration register bits. */
typedef enum {
    /** \brief Comparator queue bit 0. */
    ADS1115_CONFIG_BITS_COMP_QUE0       = 1,
    /** \brief Comparator queue bit 1. */
    ADS1115_CONFIG_BITS_COMP_QUE1       = 2,
    /** \brief Latching comparator bit. */
    ADS1115_CONFIG_BITS_COMP_LAT        = 3,
    /** \brief Comparator polarity bit. */
    ADS1115_CONFIG_BITS_COMP_POL        = 4,
    /** \brief Comparator mode bit. */
    ADS1115_CONFIG_BITS_COMP_MODE       = 5,
    /** \brief Data rate bit 0. */
    ADS1115_CONFIG_BITS_COMP_DR0        = 6,
    /** \brief Data rate bit 1. */
    ADS1115_CONFIG_BITS_COMP_DR1        = 7,
    /** \brief Data rate bit 2. */
    ADS1115_CONFIG_BITS_COMP_DR2        = 8,
    /** \brief Device operating mode bit. */
    ADS1115_CONFIG_BITS_MODE            = 9,
    /** \brief Programmable gain amplifier config bit 0. */
    ADS1115_CONFIG_BITS_PGA0            = 10,
    /** \brief Programmable gain amplifier config bit 1. */
    ADS1115_CONFIG_BITS_PGA1            = 11,
    /** \brief Programmable gain amplifier config bit 2. */
    ADS1115_CONFIG_BITS_PGA2            = 12,
    /** \brief Input multiplexer config bit 0. */
    ADS1115_CONFIG_BITS_MUX0            = 13,
    /** \brief Input multiplexer config bit 1. */
    ADS1115_CONFIG_BITS_MUX1            = 14,
    /** \brief Input multiplexer config bit 2. */
    ADS1115_CONFIG_BITS_MUX2            = 15,
    /** \brief Operational status/single-shot conversion start bit. */
    ADS1115_CONFIG_BITS_OS              = 16
} ADS1115_Bits_Config_t;


/** \brief Input multiplexer configuration bits. */
typedef enum {
    /** \brief AINP = AIN0 and AINN = AIN1 (default). */
    ADS1115_MUX_BITS_P0_N1              = 0x00,
    /** \brief AINP = AIN0 and AINN = AIN3. */
    ADS1115_MUX_BITS_P0_N3              = 0x01,
    /** \brief AINP = AIN1 and AINN = AIN3. */
    ADS1115_MUX_BITS_P1_N3              = 0x02,
    /** \brief AINP = AIN2 and AINN = AIN3. */
    ADS1115_MUX_BITS_P2_N3              = 0x03,
    /** \brief AINP = AIN0 and AINN = GND. */
    ADS1115_MUX_BITS_P0_NG              = 0x04,
    /** \brief AINP = AIN1 and AINN = GND. */
    ADS1115_MUX_BITS_P1_NG              = 0x05,
    /** \brief AINP = AIN2 and AINN = GND. */
    ADS1115_MUX_BITS_P2_NG              = 0x06,
    /** \brief AINP = AIN3 and AINN = GND. */
    ADS1115_MUX_BITS_P3_NG              = 0x07
} ADS1115_Bits_Mux_t;


/** \brief Programmable gain amplifier configuration bits. */
typedef enum {
    /** \brief FS = ±6.144V. */
    ADS1115_PGA_BITS_6P144              = 0x00,
    /** \brief FS = ±4.096V. */
    ADS1115_PGA_BITS_4P096              = 0x01,
    /** \brief FS = ±2.048V (default). */
    ADS1115_PGA_BITS_2P048              = 0x02,
    /** \brief FS = ±1.024V. */
    ADS1115_PGA_BITS_1P024              = 0x03,
    /** \brief FS = ±0.512V. */
    ADS1115_PGA_BITS_0P512              = 0x04,
    /** \brief FS = ±0.256V. */
    ADS1115_PGA_BITS_0P256              = 0x05,
    /** \brief FS = ±0.256V. */
    ADS1115_PGA_BITS_0P256B             = 0x06,
    /** \brief FS = ±0.256V. */
    ADS1115_PGA_BITS_0P256C             = 0x07
} ADS1115_Bits_Pga_t;


/** \brief Device operating mode configuration bits. */
typedef enum {
    /** \brief Continuous conversion mode. */
    ADS1115_MODE_BITS_CONTINUOUS        = 0x00,
    /** \brief Power-down single-shot mode (default). */
    ADS1115_MODE_BITS_SINGLE            = 0x01
} ADS1115_Bits_Mode_t;


/** \brief Data rates configuration bits. */
typedef enum {
    /** \brief 8SPS. */
    ADS1115_DATA_RATE_BITS_8            = 0x00,
    /** \brief 16SPS. */
    ADS1115_DATA_RATE_BITS_16           = 0x01,
    /** \brief 32SPS. */
    ADS1115_DATA_RATE_BITS_32           = 0x02,
    /** \brief 64SPS. */
    ADS1115_DATA_RATE_BITS_64           = 0x03,
    /** \brief 128SPS (default). */
    ADS1115_DATA_RATE_BITS_128          = 0x04,
    /** \brief 250SPS. */
    ADS1115_DATA_RATE_BITS_250          = 0x05,
    /** \brief 475SPS. */
    ADS1115_DATA_RATE_BITS_475          = 0x06,
    /** \brief 860SPS. */
    ADS1115_DATA_RATE_BITS_860          = 0x07
} ADS1115_Bits_Data_Rate_t;


/** \brief Comparator mode bits. */
typedef enum {
    /** \brief Traditional comparator with hysteresis (default). */
    ADS1115_COMP_MODE_BITS_HYSTERESIS   = 0x00,
    /** \brief Window comparator. */
    ADS1115_COMP_MODE_BITS_WINDOW       = 0x01
} ADS1115_Bits_Comp_Mode_t;


/** \brief Comparator polarity bits. */
typedef enum {
    /** \brief Active low (default). */
    ADS1115_COMP_POL_ACTIVE_LOW         = 0x00,
    /** \brief Active high. */
    ADS1115_COMP_POL_ACTIVE_HIGH        = 0x01
} ADS1115_Bits_Comp_Polarity_t;


/** \brief Latching comparator bits. */
typedef enum {
    /** \brief Non-latching comparator (default). */
    ADS1115_COMP_LAT_BITS_NON_LATCHING  = 0x00,
    /** \brief Latching comparator. */
    ADS1115_COMP_LAT_BITS_LATCHING      = 0x01
} ADS1115_Bits_Comp_Latching_t;


/** \brief Comparator queue and disable bits. */
typedef enum {
    /** \brief Assert after one conversion. */
    ADS1115_COMP_QUE_BITS_ASSERT1       = 0x00,
    /** \brief Assert after two conversions. */
    ADS1115_COMP_QUE_BITS_ASSERT2       = 0x01,
    /** \brief Assert after four conversions. */
    ADS1115_COMP_QUE_BITS_ASSERT4       = 0x02,
    /** \brief Disable comparator (default). */
    ADS1115_COMP_QUE_BITS_DISABLE       = 0x03
} ADS1115_Bits_Comp_Queue_t;


/** \brief Operational status bits. */
typedef enum {
    /** \brief Conversion active. */
    ADS1115_OP_STATUS_ACTIVE            = 0x00,
    /** \brief Conversion inactive. */
    ADS1115_OP_STATUS_INACTIVE          = 0x01
} ADS1115_Bits_Op_Status_t;


/* mV multipliers. */
/** \brief mV constant gain multiplier 6P144. */
#define ADS1115_MV_MULT_6P144           (0.187500f)
/** \brief mV constant gain multiplier 4P096. */
#define ADS1115_MV_MULT_4P096           (0.125000f)
/** \brief mV constant gain multiplier 2P048 (default). */
#define ADS1115_MV_MULT_2P048           (0.062500f)
/** \brief mV constant gain multiplier 1P024. */
#define ADS1115_MV_MULT_1P024           (0.031250f)
/** \brief mV constant gain multiplier 0P512. */
#define ADS1115_MV_MULT_0P512           (0.015625f)
/** \brief mV constant gain multiplier 0P256. */
#define ADS1115_MV_MULT_0P256           (0.007813f)
/** \brief mV constant gain multiplier 0P256B. */
#define ADS1115_MV_MULT_0P256B          (0.007813f)
/** \brief mV constant gain multiplier 0P256C. */
#define ADS1115_MV_MULT_0P256C          (0.007813f)

/* ADS1115 addresses. */
/** \brief Address pin low (GND) (default). */
#define ADS1115_ADDRESS_GND             (0x48)
/** \brief Address pin high (VCC). */
#define ADS1115_ADDRESS_VDD             (0x49)
/** \brief Address pin tied to SDA pin. */
#define ADS1115_ADDRESS_SDA             (0x4A)
/** \brief Address pin tied to SCL pin. */
#define ADS1115_ADDRESS_SCL             (0x4B)

/* Configuration bit lengths. */
/** \brief Config bit length mux. */
#define ADS1115_BIT_LENGTH_MUX          (3)
/** \brief Config bit length pga. */
#define ADS1115_BIT_LENGTH_PGA          (3)
/** \brief Config bit length data ready. */
#define ADS1115_BIT_LENGTH_DR           (3)
/** \brief Config bit length comparator queue. */
#define ADS1115_BIT_LENGTH_COMP_QUE     (2)


#ifdef __cplusplus
}
#endif 

#endif /* ADS1115_MAP_H */
