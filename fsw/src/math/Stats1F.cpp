#include "math/Stats1F.hpp"

Stats1F::Stats1F():
	m_Sum(),
	m_SumSq(),
	m_Count(0)
{
    
}
Stats1F::~Stats1F()
{

}

void Stats1F::update(math::Vector1F u)
{
    m_Sum = m_Sum + u;
	m_SumSq = m_SumSq + u.EMult(u);
	m_Count = m_Count + 1;
}

