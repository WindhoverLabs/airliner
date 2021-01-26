#include <math/Derivative.hpp>


Derivative::Derivative() :
	m_Initialized(false),
	m_PreviousValue(0.0f),
	m_LowPass(0.0f, 0.0f)
{

}



Derivative::~Derivative()
{

}



float Derivative::Update(float input, float dt, float cutoff)
{
	float output = 0.0f;

	m_LowPass.set_cutoff_frequency(1.0/dt, cutoff);

	if (m_Initialized)
	{
		output = m_LowPass.apply((input - m_PreviousValue) / dt);
	}
	else
	{
		/* If this is the first call to update we have no valid derivative
		 * and so we use the assumption the input value is not changing much,
		 * which is the best we can do here.
		 */
		m_LowPass.apply(0.0f);
		output = 0.0f;
		m_Initialized = true;
	}

	m_PreviousValue = input;

	return output;
}
