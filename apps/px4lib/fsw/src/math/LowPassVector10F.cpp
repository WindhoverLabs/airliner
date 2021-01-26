#include "math/LowPassVector10F.hpp"

#define M_PI_F			3.14159265f

LowPassVector10F::LowPassVector10F() :
	m_Initialized(false),
    m_State{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}
{
    
}

LowPassVector10F::~LowPassVector10F()
{

}

math::Vector10F LowPassVector10F::Update(math::Vector10F input, float dt, float cutoff)
{
	float b = 0.0f;
	float a = 0.0f;
	math::Vector10F output;
    output.Zero();

	if (m_Initialized)
	{
        b = 2.0f * float(M_PI_F) * cutoff * dt;
        a = b / (1.0f + b);
    	output = input * a + m_State * (1.0f - a);
        m_State = output;
	}
	else
	{
		/* If this is the first call to update we have no valid derivative
		 * and so we use the assumption the input value is not changing much,
		 * which is the best we can do here.
		 */
        m_Initialized = true;
	}

	return output;
}

float LowPassVector10F::operator [] (uint32 i) const
{
	if(i < 10)
	{
		return m_State[i];
	}

	return NAN;
};
