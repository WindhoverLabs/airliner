/************************************************************************
** Includes
*************************************************************************/
#include "ads1115.h"
#include "ads1115_custom.h"
#include <unistd.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Default constructor.                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
ADS1115::ADS1115()
{
    /* Set default values as initial values. */
    mConfig.mux = ADS1115_MUX_BITS_P0_N1;
    mConfig.pga = ADS1115_PGA_BITS_2P048;
    mConfig.mode = ADS1115_MODE_BITS_SINGLE;
    mConfig.rate = ADS1115_DATA_RATE_BITS_128;
    mConfig.compMode = ADS1115_COMP_MODE_BITS_HYSTERESIS;
    mConfig.compPolarity = ADS1115_COMP_POL_ACTIVE_LOW;
    mConfig.compLatching = ADS1115_COMP_LAT_BITS_NON_LATCHING;
    mConfig.compQueueMode = ADS1115_COMP_QUE_BITS_DISABLE;
    mConfig.mvMultiplier = ADS1115_MV_MULT_2P048;
    ADS1115_Custom_InitData();
    ADS1115_Custom_Init();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Destructor constructor.                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
ADS1115::~ADS1115()
{
    ADS1115_Custom_Uninit();
}


/* Configuration setters. */
boolean ADS1115::setMux(const ADS1115_Bits_Mux_t mux)
{
    boolean returnBool = FALSE;
    uint16 value = 0;

    /* Get the existing value. */
    returnBool = ADS1115_readBlock(ADS1115_REG_CONFIG, &value, sizeof(value));
    if(returnBool == FALSE)
    {
        goto end_of_function;
    }

    value = (value << 8) | (value >> 8);

    /* Set the new value of just the desired bits. */
    value = setBits16(ADS1115_CONFIG_BITS_MUX2 - 1, ADS1115_BIT_LENGTH_MUX, value, mux);

    value = (value << 8) | (value >> 8);

    /* Write the new value. */
    returnBool = ADS1115_writeBlock(ADS1115_REG_CONFIG, &value, sizeof(value));
    if(returnBool == FALSE)
    {
        goto end_of_function;
    }

    mConfig.mux = mux;

end_of_function:
    return returnBool;
}


boolean ADS1115::setGain(const ADS1115_Bits_Pga_t pga)
{
    boolean returnBool = FALSE;
    uint16 value = 0;

    /* Get the existing value. */
    returnBool = ADS1115_readBlock(ADS1115_REG_CONFIG, &value, sizeof(value));
    if(returnBool == FALSE)
    {
        goto end_of_function;
    }

    value = (value << 8) | (value >> 8);

    /* Set the new value of just the desired bits. */
    value = setBits16(ADS1115_CONFIG_BITS_PGA2 - 1, ADS1115_BIT_LENGTH_PGA, value, pga);

    value = (value << 8) | (value >> 8);

    /* Write the new value. */
    returnBool = ADS1115_writeBlock(ADS1115_REG_CONFIG, &value, sizeof(value));
    if(returnBool == FALSE)
    {
        goto end_of_function;
    }

    mConfig.pga = pga;

    switch (pga) 
    {
        case ADS1115_PGA_BITS_6P144:
        {
            mConfig.mvMultiplier = ADS1115_MV_MULT_6P144;
            break;
        }
        case ADS1115_PGA_BITS_4P096:
        {
            mConfig.mvMultiplier = ADS1115_MV_MULT_4P096;
            break;
        }
        case ADS1115_PGA_BITS_2P048:
        {
            mConfig.mvMultiplier = ADS1115_MV_MULT_2P048;
            break;
        }
        case ADS1115_PGA_BITS_1P024:
        {
            mConfig.mvMultiplier = ADS1115_MV_MULT_1P024;
            break;
        }
        case ADS1115_PGA_BITS_0P512:
        {
            mConfig.mvMultiplier = ADS1115_MV_MULT_0P512;
            break;
        }
        case ADS1115_PGA_BITS_0P256:
            /* Fall through. */
        case ADS1115_PGA_BITS_0P256B:
            /* Fall through. */
        case ADS1115_PGA_BITS_0P256C:
        {
            mConfig.mvMultiplier = ADS1115_MV_MULT_0P256;
            break;
        }
    }

end_of_function:
    return returnBool;
}


boolean ADS1115::setMode(const ADS1115_Bits_Mode_t mode)
{
    boolean returnBool = FALSE;
    uint16 value = 0;

    /* Get the existing value. */
    returnBool = ADS1115_readBlock(ADS1115_REG_CONFIG, &value, sizeof(value));
    if(returnBool == FALSE)
    {
        goto end_of_function;
    }

    value = (value << 8) | (value >> 8);

    /* Set the new value of just the desired bit. */
    if(mode)
    {
        value |= 1 << (ADS1115_CONFIG_BITS_MODE - 1);
    }
    else
    {
        value &= ~(1 << (ADS1115_CONFIG_BITS_MODE - 1));
    }

    value = (value << 8) | (value >> 8);

    /* Write the new value. */
    returnBool = ADS1115_writeBlock(ADS1115_REG_CONFIG, &value, sizeof(value));
    if(returnBool == FALSE)
    {
        goto end_of_function;
    }

    mConfig.mode = mode;

end_of_function:
    return returnBool;
}


boolean ADS1115::setRate(const ADS1115_Bits_Data_Rate_t rate)
{
    boolean returnBool = FALSE;
    uint16 value = 0;

    /* Get the existing value. */
    returnBool = ADS1115_readBlock(ADS1115_REG_CONFIG, &value, sizeof(value));
    if(returnBool == FALSE)
    {
        goto end_of_function;
    }

    value = (value << 8) | (value >> 8);

    /* Set the new value of just the desired bits. */
    value = setBits16(ADS1115_CONFIG_BITS_COMP_DR2 - 1, ADS1115_BIT_LENGTH_DR, value, rate);

    value = (value << 8) | (value >> 8);

    /* Write the new value. */
    returnBool = ADS1115_writeBlock(ADS1115_REG_CONFIG, &value, sizeof(value));
    if(returnBool == FALSE)
    {
        goto end_of_function;
    }

    mConfig.rate = rate;

end_of_function:
    return returnBool;
}


boolean ADS1115::setComparatorMode(const ADS1115_Bits_Comp_Mode_t compMode)
{
    boolean returnBool = FALSE;
    uint16 value = 0;

    /* Get the existing value. */
    returnBool = ADS1115_readBlock(ADS1115_REG_CONFIG, &value, sizeof(value));
    if(returnBool == FALSE)
    {
        goto end_of_function;
    }

    value = (value << 8) | (value >> 8);

    /* Set the new value of just the desired bit. */
    if(compMode)
    {
        value |= 1 << (ADS1115_CONFIG_BITS_COMP_MODE - 1);
    }
    else
    {
        value &= ~(1 << (ADS1115_CONFIG_BITS_COMP_MODE - 1));
    }

    value = (value << 8) | (value >> 8);

    /* Write the new value. */
    returnBool = ADS1115_writeBlock(ADS1115_REG_CONFIG, &value, sizeof(value));
    if(returnBool == FALSE)
    {
        goto end_of_function;
    }

    mConfig.compMode = compMode;

end_of_function:
    return returnBool;
}


boolean ADS1115::setComparatorPolarity(const ADS1115_Bits_Comp_Polarity_t compPolarity)
{
    boolean returnBool = FALSE;
    uint16 value = 0;

    /* Get the existing value. */
    returnBool = ADS1115_readBlock(ADS1115_REG_CONFIG, &value, sizeof(value));
    if(returnBool == FALSE)
    {
        goto end_of_function;
    }

    value = (value << 8) | (value >> 8);

    /* Set the new value of just the desired bit. */
    if(compPolarity)
    {
        value |= 1 << (ADS1115_CONFIG_BITS_COMP_POL - 1);
    }
    else
    {
        value &= ~(1 << (ADS1115_CONFIG_BITS_COMP_POL - 1));
    }

    value = (value << 8) | (value >> 8);

    /* Write the new value. */
    returnBool = ADS1115_writeBlock(ADS1115_REG_CONFIG, &value, sizeof(value));
    if(returnBool == FALSE)
    {
        goto end_of_function;
    }

    mConfig.compPolarity = compPolarity;

end_of_function:
    return returnBool;
}


boolean ADS1115::setComparatorLatchEnabled(const ADS1115_Bits_Comp_Latching_t compLatching)
{
    boolean returnBool = FALSE;
    uint16 value = 0;

    /* Get the existing value. */
    returnBool = ADS1115_readBlock(ADS1115_REG_CONFIG, &value, sizeof(value));
    if(returnBool == FALSE)
    {
        goto end_of_function;
    }

    value = (value << 8) | (value >> 8);

    /* Set the new value of just the desired bit. */
    if(compLatching)
    {
        value |= 1 << (ADS1115_CONFIG_BITS_COMP_LAT - 1);
    }
    else
    {
        value &= ~(1 << (ADS1115_CONFIG_BITS_COMP_LAT - 1));
    }

    value = (value << 8) | (value >> 8);

    /* Write the new value. */
    returnBool = ADS1115_writeBlock(ADS1115_REG_CONFIG, &value, sizeof(value));
    if(returnBool == FALSE)
    {
        goto end_of_function;
    }

    mConfig.compLatching = compLatching;

end_of_function:
    return returnBool;
}


boolean ADS1115::setComparatorQueueMode(const ADS1115_Bits_Comp_Queue_t compQueueMode)
{
    boolean returnBool = FALSE;
    uint16 value = 0;

    /* Get the existing value. */
    returnBool = ADS1115_readBlock(ADS1115_REG_CONFIG, &value, sizeof(value));
    if(returnBool == FALSE)
    {
        goto end_of_function;
    }

    value = (value << 8) | (value >> 8);

    /* Set the new value of just the desired bits. */
    value = setBits16(ADS1115_CONFIG_BITS_COMP_QUE1 - 1, ADS1115_BIT_LENGTH_COMP_QUE, value, compQueueMode);

    value = (value << 8) | (value >> 8);

    /* Write the new value. */
    returnBool = ADS1115_writeBlock(ADS1115_REG_CONFIG, &value, sizeof(value));
    if(returnBool == FALSE)
    {
        goto end_of_function;
    }

    mConfig.compQueueMode = compQueueMode;

end_of_function:
    return returnBool;
}


boolean setOperationalStatus(ADS1115_Bits_Op_Status_t status)
{
    boolean returnBool = FALSE;
    uint16 value = 0;

    /* Get the existing value. */
    returnBool = ADS1115_readBlock(ADS1115_REG_CONFIG, &value, sizeof(value));
    if(returnBool == FALSE)
    {
        goto end_of_function;
    }

    value = (value << 8) | (value >> 8);

    /* Set the new value of just the desired bit. */
    if(status)
    {
        value |= 1 << (ADS1115_CONFIG_BITS_OS - 1);
    }
    else
    {
        value &= ~(1 << (ADS1115_CONFIG_BITS_OS - 1));
    }

    value = (value << 8) | (value >> 8);

    /* Write the new value. */
    returnBool = ADS1115_writeBlock(ADS1115_REG_CONFIG, &value, sizeof(value));
    if(returnBool == FALSE)
    {
        goto end_of_function;
    }

end_of_function:
    return returnBool;
}


boolean ADS1115::setLoThresholdReg(uint16 value)
{
    value = (value << 8) | (value >> 8);
    return ADS1115_writeBlock(ADS1115_REG_LO_THRESHOLD, &value, sizeof(value));
}


boolean ADS1115::setHiThresholdReg(uint16 value)
{
    value = (value << 8) | (value >> 8);
    return ADS1115_writeBlock(ADS1115_REG_HI_THRESHOLD, &value, sizeof(value));
}


/* Configuration getters. */
boolean ADS1115::getMux(ADS1115_Bits_Mux_t *mux)
{
    boolean returnBool = FALSE;
    uint16 value = 0;

    /* Get the existing value. */
    returnBool = ADS1115_readBlock(ADS1115_REG_CONFIG, &value, sizeof(value));
    if(returnBool == FALSE)
    {
        goto end_of_function;
    }

    value = (value << 8) | (value >> 8);

    *mux = static_cast<ADS1115_Bits_Mux_t>(getBits16(ADS1115_CONFIG_BITS_MUX2 - 1, 
            ADS1115_BIT_LENGTH_MUX, value));

end_of_function:
    return returnBool;
}


boolean ADS1115::getGain(ADS1115_Bits_Pga_t *pga)
{
    boolean returnBool = FALSE;
    uint16 value = 0;

    /* Get the existing value. */
    returnBool = ADS1115_readBlock(ADS1115_REG_CONFIG, &value, sizeof(value));
    if(returnBool == FALSE)
    {
        goto end_of_function;
    }

    value = (value << 8) | (value >> 8);

    /* Set the new value of just the desired bits. */
    *pga = static_cast<ADS1115_Bits_Pga_t>(getBits16(ADS1115_CONFIG_BITS_PGA2 - 1, 
            ADS1115_BIT_LENGTH_PGA, value));

end_of_function:
    return returnBool;
}


boolean ADS1115::getMode(ADS1115_Bits_Mode_t *mode)
{
    boolean returnBool = FALSE;
    uint16 value = 0;

    /* Get the existing value. */
    returnBool = ADS1115_readBlock(ADS1115_REG_CONFIG, &value, sizeof(value));
    if(returnBool == FALSE)
    {
        goto end_of_function;
    }

    value = (value << 8) | (value >> 8);

    *mode = static_cast<ADS1115_Bits_Mode_t>(value & (1 << ADS1115_CONFIG_BITS_MODE));

end_of_function:
    return returnBool;
}


boolean ADS1115::getRate(ADS1115_Bits_Data_Rate_t *rate)
{
    boolean returnBool = FALSE;
    uint16 value = 0;

    /* Get the existing value. */
    returnBool = ADS1115_readBlock(ADS1115_REG_CONFIG, &value, sizeof(value));
    if(returnBool == FALSE)
    {
        goto end_of_function;
    }

    value = (value << 8) | (value >> 8);

    *rate = static_cast<ADS1115_Bits_Data_Rate_t>(getBits16(ADS1115_CONFIG_BITS_COMP_DR2 - 1, 
            ADS1115_BIT_LENGTH_DR, value));

end_of_function:
    return returnBool;

}


boolean ADS1115::getComparatorMode(ADS1115_Bits_Comp_Mode_t *compMode)
{
    boolean returnBool = FALSE;
    uint16 value = 0;

    /* Get the existing value. */
    returnBool = ADS1115_readBlock(ADS1115_REG_CONFIG, &value, sizeof(value));
    if(returnBool == FALSE)
    {
        goto end_of_function;
    }

    value = (value << 8) | (value >> 8);

    *compMode = static_cast<ADS1115_Bits_Comp_Mode_t>(value & (1 << ADS1115_CONFIG_BITS_COMP_MODE));

end_of_function:
    return returnBool;
}


boolean ADS1115::getComparatorPolarity(ADS1115_Bits_Comp_Polarity_t *compPolarity)
{
    boolean returnBool = FALSE;
    uint16 value = 0;

    /* Get the existing value. */
    returnBool = ADS1115_readBlock(ADS1115_REG_CONFIG, &value, sizeof(value));
    if(returnBool == FALSE)
    {
        goto end_of_function;
    }

    value = (value << 8) | (value >> 8);

    *compPolarity = static_cast<ADS1115_Bits_Comp_Polarity_t>(value & (1 << ADS1115_CONFIG_BITS_COMP_POL));

end_of_function:
    return returnBool;
}


boolean ADS1115::getComparatorLatchEnabled(ADS1115_Bits_Comp_Latching_t *compLatching)
{
    boolean returnBool = FALSE;
    uint16 value = 0;

    /* Get the existing value. */
    returnBool = ADS1115_readBlock(ADS1115_REG_CONFIG, &value, sizeof(value));
    if(returnBool == FALSE)
    {
        goto end_of_function;
    }

    value = (value << 8) | (value >> 8);

    *compLatching = static_cast<ADS1115_Bits_Comp_Latching_t>(value & (1 << ADS1115_CONFIG_BITS_COMP_LAT));

end_of_function:
    return returnBool;
}


boolean ADS1115::getComparatorQueueMode(ADS1115_Bits_Comp_Queue_t *compQueueMode)
{
    boolean returnBool = FALSE;
    uint16 value = 0;

    /* Get the existing value. */
    returnBool = ADS1115_readBlock(ADS1115_REG_CONFIG, &value, sizeof(value));
    if(returnBool == FALSE)
    {
        goto end_of_function;
    }

    value = (value << 8) | (value >> 8);

    *compQueueMode = static_cast<ADS1115_Bits_Comp_Queue_t>(getBits16(ADS1115_CONFIG_BITS_COMP_QUE1 - 1, 
            ADS1115_BIT_LENGTH_COMP_QUE, value));

end_of_function:
    return returnBool;
}


boolean ADS1115::getOperationalStatus(ADS1115_Bits_Op_Status_t *status)
{
    boolean returnBool = FALSE;
    uint16 value = 0;

    /* Get the existing value. */
    returnBool = ADS1115_readBlock(ADS1115_REG_CONFIG, &value, sizeof(value));
    if(returnBool == FALSE)
    {
        goto end_of_function;
    }

    value = (value << 8) | (value >> 8);

    *status = static_cast<ADS1115_Bits_Op_Status_t>(value & (1 << ADS1115_CONFIG_BITS_OS));

end_of_function:
    return returnBool;
}


uint16 ADS1115::setBits16(uint8 bitStart, uint8 length, uint16 originalValue, uint16 data)
{
    uint16 word = originalValue;
    uint16 mask = ((1 << length) - 1) << (bitStart - length + 1);
    data <<= (bitStart - length + 1);
    data &= mask;
    word &= ~(mask);
    word |= data;
    return word;
}


uint16 ADS1115::getBits16(uint8 bitStart, uint8 length, uint16 originalValue)
{
    uint16 word = originalValue;
    uint16 mask = ((1 << length) - 1) << (bitStart - length + 1);
    word &= mask;
    word >>= (bitStart - length + 1);
    return word;
}


boolean ADS1115::getConfiguration(ADS1115_Configuration_t *config)
{
    boolean returnBool = FALSE;
    uint16 value = 0;

    returnBool = getConfigReg(&value);
    if(FALSE == returnBool)
    {
        goto end_of_function;
    }

    value = (value << 8) | (value >> 8);

    config->mux = static_cast<ADS1115_Bits_Mux_t>
            (getBits16(ADS1115_CONFIG_BITS_MUX2 - 1, 
            ADS1115_BIT_LENGTH_MUX, value));

    config->pga = static_cast<ADS1115_Bits_Pga_t>
            (getBits16(ADS1115_CONFIG_BITS_PGA2 - 1, 
            ADS1115_BIT_LENGTH_PGA, value));

    config->mode = static_cast<ADS1115_Bits_Mode_t>
            (value & (1 << ADS1115_CONFIG_BITS_MODE));
    
    config->rate = static_cast<ADS1115_Bits_Data_Rate_t>
            (getBits16(ADS1115_CONFIG_BITS_COMP_DR2 - 1, 
            ADS1115_BIT_LENGTH_DR, value));
            
    config->compMode = static_cast<ADS1115_Bits_Comp_Mode_t>
            (value & (1 << ADS1115_CONFIG_BITS_COMP_MODE));
    
    config->compPolarity = static_cast<ADS1115_Bits_Comp_Polarity_t>
            (value & (1 << ADS1115_CONFIG_BITS_COMP_POL));
    
    config->compLatching = static_cast<ADS1115_Bits_Comp_Latching_t>
            (value & (1 << ADS1115_CONFIG_BITS_COMP_LAT));

    config->compQueueMode = static_cast<ADS1115_Bits_Comp_Queue_t>
            (getBits16(ADS1115_CONFIG_BITS_COMP_QUE1 - 1, 
            ADS1115_BIT_LENGTH_COMP_QUE, value));

    switch (config->pga) 
    {
        case ADS1115_PGA_BITS_6P144:
        {
            config->mvMultiplier = ADS1115_MV_MULT_6P144;
            break;
        }
        case ADS1115_PGA_BITS_4P096:
        {
            config->mvMultiplier = ADS1115_MV_MULT_4P096;
            break;
        }
        case ADS1115_PGA_BITS_2P048:
        {
            config->mvMultiplier = ADS1115_MV_MULT_2P048;
            break;
        }
        case ADS1115_PGA_BITS_1P024:
        {
            config->mvMultiplier = ADS1115_MV_MULT_1P024;
            break;
        }
        case ADS1115_PGA_BITS_0P512:
        {
            config->mvMultiplier = ADS1115_MV_MULT_0P512;
            break;
        }
        case ADS1115_PGA_BITS_0P256:
            /* Fall through. */
        case ADS1115_PGA_BITS_0P256B:
            /* Fall through. */
        case ADS1115_PGA_BITS_0P256C:
        {
            config->mvMultiplier = ADS1115_MV_MULT_0P256;
            break;
        }
    }

end_of_function:
    return returnBool;
}


boolean ADS1115::setConfiguration(const ADS1115_Configuration_t config)
{
    boolean returnBool = FALSE;
    uint16 value = 0;

    value = setBits16(ADS1115_CONFIG_BITS_MUX2 - 1, ADS1115_BIT_LENGTH_MUX, value, config.mux);

    value = setBits16(ADS1115_CONFIG_BITS_PGA2 - 1, ADS1115_BIT_LENGTH_PGA, value, config.pga);

    /* Set the new value of just the desired bit. */
    if(config.mode)
    {
        value |= 1 << (ADS1115_CONFIG_BITS_MODE - 1);
    }

    value = setBits16(ADS1115_CONFIG_BITS_COMP_DR2 - 1, ADS1115_BIT_LENGTH_DR, value, config.rate);

    if(config.compMode)
    {
        value |= 1 << (ADS1115_CONFIG_BITS_COMP_MODE - 1);
    }

    if(config.compPolarity)
    {
        value |= 1 << (ADS1115_CONFIG_BITS_COMP_POL - 1);
    }

    if(config.compLatching)
    {
        value |= 1 << (ADS1115_CONFIG_BITS_COMP_LAT - 1);
    }

    value = setBits16(ADS1115_CONFIG_BITS_COMP_QUE1 - 1, ADS1115_BIT_LENGTH_COMP_QUE, value, config.compQueueMode);

    value = (value << 8) | (value >> 8);

    returnBool = ADS1115_writeBlock(ADS1115_REG_CONFIG, &value, sizeof(value));

end_of_function:
    return returnBool;
}


boolean ADS1115::getConversionReg(int16 *value)
{
    boolean returnBool = FALSE;
    uint16 conversion = 0;
    
    returnBool = ADS1115_readBlock(ADS1115_REG_CONVERSION, &conversion, sizeof(conversion));

    *value = (conversion << 8) | (conversion >> 8);

    return returnBool;
}


boolean ADS1115::getConfigReg(uint16 *value)
{
    boolean returnBool  = FALSE;
    uint16 temp;

    returnBool = ADS1115_readBlock(ADS1115_REG_CONFIG, &temp, sizeof(temp));
    
    *value = (temp << 8) | (temp >> 8);
    
    return returnBool;
}


boolean ADS1115::getLoThresholdReg(uint16 *value)
{
    boolean returnBool  = FALSE;
    uint16 temp;

    returnBool = ADS1115_readBlock(ADS1115_REG_LO_THRESHOLD, &temp, sizeof(temp));

    *value = (temp << 8) | (temp >> 8);

    return returnBool;
}


boolean ADS1115::getHiThresholdReg(uint16 *value)
{
    boolean returnBool  = FALSE;
    uint16 temp;

    returnBool = ADS1115_readBlock(ADS1115_REG_HI_THRESHOLD, &temp, sizeof(temp));

    *value = (temp << 8) | (temp >> 8);

    return returnBool;
}


/************************/
/*  End of File Comment */
/************************/
