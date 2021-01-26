#include "math/Stats2F.hpp"

Stats2F::Stats2F():
	m_Sum(),
	m_SumSq(),
	m_Count(0)
{
    
}
Stats2F::~Stats2F()
{

}

void Stats2F::update(math::Vector2F u)
{
    m_Sum = m_Sum + u;
	m_SumSq = m_SumSq + u.EMult(u);
	m_Count = m_Count + 1;
}

