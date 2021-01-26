#include "math/Stats6F.hpp"

Stats6F::Stats6F():
	m_Sum(),
	m_SumSq(),
	m_Count(0)
{
    
}
Stats6F::~Stats6F()
{

}

void Stats6F::update(math::Vector6F u)
{
    m_Sum = m_Sum + u;
	m_SumSq = m_SumSq + u.EMult(u);
	m_Count = m_Count + 1;
}

