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

#ifndef ADS1115_H
#define ADS1115_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
 ** Pragmas
 *************************************************************************/

/************************************************************************
 ** Includes
 *************************************************************************/
#include "cfe.h"
#include "ads1115_map.h"

/************************************************************************
 ** Local Defines
 *************************************************************************/

/************************************************************************
 ** Structure Definitions
 *************************************************************************/
typedef struct
{
    ADS1115_Bits_Mux_t mux;
    ADS1115_Bits_Pga_t pga;
    ADS1115_Bits_Mode_t mode;
    ADS1115_Bits_Data_Rate_t rate;
    ADS1115_Bits_Comp_Mode_t compMode;
    ADS1115_Bits_Comp_Polarity_t compPolarity;
    ADS1115_Bits_Comp_Latching_t compLatching;
    ADS1115_Bits_Comp_Queue_t compQueueMode;
    float mvMultiplier;
} ADS1115_Configuration_t;


/**
 **  \brief ADS1115 Class
 */
class ADS1115
{
public:
    ADS1115();
    ~ADS1115();

    /* The current device configuration. */
    ADS1115_Configuration_t mConfig;
    /* Configuration setters. */
    boolean setMux(const ADS1115_Bits_Mux_t mux);
    boolean setGain(const ADS1115_Bits_Pga_t pga);
    boolean setMode(const ADS1115_Bits_Mode_t mode);
    boolean setRate(const ADS1115_Bits_Data_Rate_t rate);
    boolean setComparatorMode(const ADS1115_Bits_Comp_Mode_t compMode);
    boolean setComparatorPolarity(const ADS1115_Bits_Comp_Polarity_t compPolarity);
    boolean setComparatorLatchEnabled(const ADS1115_Bits_Comp_Latching_t compLatching);
    boolean setComparatorQueueMode(const ADS1115_Bits_Comp_Queue_t compQueueMode);
    boolean setOperationalStatus(ADS1115_Bits_Op_Status_t status);
    /* Configuration getters. */
    boolean getMux(ADS1115_Bits_Mux_t *mux);
    boolean getGain(ADS1115_Bits_Pga_t *pga);
    boolean getMode(ADS1115_Bits_Mode_t *mode);
    boolean getRate(ADS1115_Bits_Data_Rate_t *rate);
    boolean getComparatorMode(ADS1115_Bits_Comp_Mode_t *compMode);
    boolean getComparatorPolarity(ADS1115_Bits_Comp_Polarity_t *compPolarity);
    boolean getComparatorLatchEnabled(ADS1115_Bits_Comp_Latching_t *compLatching);
    boolean getComparatorQueueMode(ADS1115_Bits_Comp_Queue_t *compQueueMode);
    boolean getOperationalStatus(ADS1115_Bits_Op_Status_t *status);
    /* Total configuration set/get. */
    boolean getConfiguration(ADS1115_Configuration_t *config);
    boolean setConfiguration(const ADS1115_Configuration_t config);
    /* Read registers. */
    boolean getConversionReg(int16 *value);
    boolean getConfigReg(uint16 *value);
    boolean getLoThresholdReg(uint16 *value);
    boolean getHiThresholdReg(uint16 *value);
    /* Set registers. */
    boolean setLoThresholdReg(uint16 value);
    boolean setHiThresholdReg(uint16 value);
    /* Utilities. */
    uint16 setBits16(uint8 bitStart, uint8 length, uint16 originalValue, uint16 data);
    uint16 getBits16(uint8 bitStart, uint8 length, uint16 originalValue);
protected:

private:
};

#ifdef __cplusplus
}
#endif 

#endif /* ADS1115_H */

/************************/
/*  End of File Comment */
/************************/
