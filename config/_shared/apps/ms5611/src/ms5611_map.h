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

#ifndef MS5611_MAP_H
#define MS5611_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

/* SPI Commands */
/** \brief MS5611 command reset */
#define MS5611_SPI_CMD_RESET                (0x1e)

/** \brief MS5611 command convert D1 (OSR=256) */
#define MS5611_SPI_CMD_CONVERT_D1_MASK      (0x40)

/** \brief MS5611 command convert D2 (OSR=256) */
#define MS5611_SPI_CMD_CONVERT_D2_MASK      (0x50)

/** \brief MS5611 command mask convert OSR 256 */
#define MS5611_SPI_CMD_OSR_256_MASK         (0x00)

/** \brief MS5611 command mask convert OSR 512 */
#define MS5611_SPI_CMD_OSR_512_MASK         (0x02)

/** \brief MS5611 command mask convert OSR 1024 */
#define MS5611_SPI_CMD_OSR_1024_MASK        (0x04)

/** \brief MS5611 command mask convert OSR 2048 */
#define MS5611_SPI_CMD_OSR_2048_MASK        (0x06)

/** \brief MS5611 command mask convert OSR 4096 */
#define MS5611_SPI_CMD_OSR_4096_MASK        (0x08)

/** \brief MS5611 command ADC read */
#define MS5611_SPI_CMD_ADC_READ             (0x00)

/** \brief MS5611 command mask PROM read */
#define MS5611_SPI_CMD_PROM_READ_MASK       (0xa0)

/** \brief MS5611 command PROM read shift */
#define MS5611_SPI_CMD_PROM_ADDR_SHIFT      (1)

/** \brief MS5611 command convert D1 with OSR mask */
#define MS5611_SPI_CMD_CONVERT_D1           (MS5611_SPI_CMD_CONVERT_D1_MASK + MS5611_SPI_CMD_OSR_1024_MASK)

/** \brief MS5611 command convert D2 with OSR mask */
#define MS5611_SPI_CMD_CONVERT_D2           (MS5611_SPI_CMD_CONVERT_D2_MASK + MS5611_SPI_CMD_OSR_1024_MASK)

#ifdef __cplusplus
}
#endif 

#endif /* MS5611_MAP_H */
