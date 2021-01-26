#include <math/BlockDelay.hpp>

using namespace delay;


BlockDelay10FLEN10::BlockDelay10FLEN10()
{
};


BlockDelay10FLEN10::~BlockDelay10FLEN10()
{
};


void BlockDelay10FLEN10::Initialize(void)
{
    int i = 0;
    m_Index = 0;
    m_Delay = -1;
    
    /* Init all the matrices */
    for(i = 0; i < LEN; i++)
    {
        m_Storage[i].Zero();
    }
    
}


math::Vector10F BlockDelay10FLEN10::Update(const math::Vector10F &input)
{
    /* store current value */
    m_Storage[m_Index] = input;
    
    /* delay starts at zero, then increases to LEN */
    m_Delay += 1;
    
    if (m_Delay > (LEN - 1)) 
    {
        m_Delay = LEN - 1;
    }
    
    /* compute position of delayed value */
    int j = m_Index - m_Delay;

    if (j < 0) 
    {
        j += LEN;
    }
    
    /* increment storage position */
    m_Index += 1;

    if (m_Index > (LEN - 1))
    {
        m_Index  = 0;
    }

    /* get delayed value */
    return m_Storage[j];
}


math::Vector10F BlockDelay10FLEN10::Get(uint8 delay)
{
    int j = m_Index - delay;

    if (j < 0) 
    { 
        j += LEN; 
    }

    return m_Storage[j];
}




BlockDelayUINT64LEN10::BlockDelayUINT64LEN10()
{
};


BlockDelayUINT64LEN10::~BlockDelayUINT64LEN10()
{
};


void BlockDelayUINT64LEN10::Initialize(void)
{
    int i = 0;
    m_Index = 0;
    m_Delay = -1;
    
    /* Init all the matrices */
    for(i = 0; i < LEN; i++)
    {
        m_Storage[i] = 0;
    }
    
}


uint64 BlockDelayUINT64LEN10::Update(const uint64 &input)
{
    /* store current value */
    m_Storage[m_Index] = input;
    
    /* delay starts at zero, then increases to LEN */
    m_Delay += 1;
    
    if (m_Delay > (LEN - 1)) 
    {
        m_Delay = LEN - 1;
    }
    
    /* compute position of delayed value */
    int j = m_Index - m_Delay;

    if (j < 0) 
    {
        j += LEN;
    }
    
    /* increment storage position */
    m_Index += 1;

    if (m_Index > (LEN - 1))
    {
        m_Index  = 0;
    }

    /* get delayed value */
    return m_Storage[j];
}


uint64 BlockDelayUINT64LEN10::Get(uint8 delay)
{
    int j = m_Index - delay;

    if (j < 0) 
    { 
        j += LEN; 
    }

    return m_Storage[j];
}
