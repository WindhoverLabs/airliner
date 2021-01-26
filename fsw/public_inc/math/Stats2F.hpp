#ifndef STATS2F_H
#define STATS2F_H

#include "cfe.h"
#include "math/Vector2F.hpp"

class Stats2F
{
public:
	/************************************************************************/
	/** \brief Stats Constructor
	 **
	 **  \par Description
	 **       This function initializes the object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 *************************************************************************/
	Stats2F();

	/************************************************************************/
	/** \brief Stats Destructor
	 **
	 **  \par Description
	 **       This function destructs the object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 *************************************************************************/
	~Stats2F();

	/************************************************************************/
	/** \brief Stats Update
	 **
	 **  \par Description
	 **       This function updates the object with the passed value.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   u      A #Vector2F input
	 **
	 *************************************************************************/
	void update(math::Vector2F u);

	/************************************************************************/
	/** \brief Stats Reset
	 **
	 **  \par Description
	 **       This function resets the object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 *************************************************************************/
    void reset()
	{
		m_Sum.Zero();
		m_SumSq.Zero();
		m_Count = 0;
	}

	/************************************************************************/
	/** \brief Get Count
	 **
	 **  \par Description
	 **       This function returns the number of samples.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \returns
	 **  Number of samples
	 **  \endreturns
	 **
	 *************************************************************************/
	size_t getCount() { return m_Count; }

	/************************************************************************/
	/** \brief Get Mean
	 **
	 **  \par Description
	 **       This function returns the mean of the samples.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \returns
	 **  Mean of samples
	 **  \endreturns
	 **
	 *************************************************************************/
	math::Vector2F getMean() { return m_Sum / m_Count; }

	/************************************************************************/
	/** \brief Get Variance
	 **
	 **  \par Description
	 **       This function returns the variance of the samples.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \returns
	 **  Variance of samples
	 **  \endreturns
	 **
	 *************************************************************************/
	math::Vector2F getVar()
	{
		return (m_SumSq - m_Sum.EMult(m_Sum) / m_Count) / m_Count;
	}

	/************************************************************************/
	/** \brief Get Standard Deviation
	 **
	 **  \par Description
	 **       This function returns the standard deviation of the samples.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \returns
	 **  Variance of samples
	 **  \endreturns
	 **
	 *************************************************************************/
	math::Vector2F getStdDev()
	{
		return getVar().pow(0.5);
	}

private:

	/** \brief Sum of samples */
	math::Vector2F m_Sum;

	/** \brief Sum squared of samples */
	math::Vector2F m_SumSq;

	/** \brief Count of samples */
	size_t m_Count;

};

#endif
