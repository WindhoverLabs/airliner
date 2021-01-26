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

#include <math/BlockDelay.hpp>
#include "pe_block_delay_stubs.hpp"

using namespace delay;


Block_Delay_Returns_t delay::Block_Delay_Returns;


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


/* Mocked return */
math::Vector10F BlockDelay10FLEN10::Get(uint8 delay)
{
    /* Return the mocked vector */
    return Block_Delay_Returns.vectorReturn;
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


/* Mocked return */
uint64 BlockDelayUINT64LEN10::Get(uint8 delay)
{
    return Block_Delay_Returns.arrayReturn;
}


