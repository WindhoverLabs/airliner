#ifndef DERIVATIVE_H
#define DERIVATIVE_H

#include "cfe.h"
#include <math/filters/LowPassFilter2p.hpp>

class Derivative
{
public:

	/************************************************************************/
	/** \brief Derivative Constructor
	 **
	 **  \par Description
	 **       This function initializes the object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 *************************************************************************/
	Derivative();

	/************************************************************************/
	/** \brief Derivative Destructor
	 **
	 **  \par Description
	 **       This function destructs the object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 *************************************************************************/
	~Derivative();

	/************************************************************************/
	/** \brief Update Derivative
	 **
	 **  \par Description
	 **       This function updates the derivative value
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   value      A #float specifying the input value
	 **  \param [in]   dt         A #float specifying delta time in us
	 **  \param [in]   cutoff     A #float specifying cutoff frequency
	 **
	 *************************************************************************/
	float Update(float value, float dt, float cutoff);

private:

	/** \brief Initialization flag */
	bool m_Initialized;

	/** \brief Previous value of derivative */
	float m_PreviousValue;

	/** \brief Low Pass Filter */
    math::LowPassFilter2p m_LowPass;
};

#endif
