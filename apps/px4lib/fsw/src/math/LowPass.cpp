#include "math/LowPass.hpp"

#define M_PI_F			3.14159265f

LowPass::LowPass() :
	m_Initialized(false),
	m_State(0.0f)
{

}
LowPass::~LowPass()
{

}

float LowPass::Update(float input, float dt, float cutoff)
{
	float b = 0.0f;
	float a = 0.0f;
	float output = 0.0f;

	if (m_Initialized)
	{
        b = 2.0f * float(M_PI_F) * cutoff * dt;
        a = b / (1.0f + b);
    	output = a * input + (1.0f - a) * m_State;
        m_State = output;
	}
	else
	{
		/* If this is the first call to update we have no valid derivative
		 * and so we use the assumption the input value is not changing much,
		 * which is the best we can do here.
		 */
        output = 0.0f;
        m_Initialized = true;
	}

	return output;
}
