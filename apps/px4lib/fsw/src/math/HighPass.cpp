#include "math/HighPass.hpp"

#define M_PI_F			3.14159265f

HighPass::HighPass() :
	m_PrevInput(0.0f),
	m_PrevOutput(0.0f)
{

}

HighPass::~HighPass()
{

}

float HighPass::Update(float input, float dt, float cutoff)
{
	float b = 0.0f;
	float a = 0.0f;
	float output = 0.0f;

    b = 2.0f * float(M_PI_F) * cutoff * dt;
    a = 1.0f / (1.0f + b);
    
	output = a * (m_PrevOutput + input - m_PrevInput);

    m_PrevInput = input;
    m_PrevOutput = output;

	return output;
}
