#ifndef HIGHPASS_H
#define HIGHPASS_H

#include "cfe.h"

class HighPass
{
public:
	/************************************************************************/
	/** \brief High Pass Constructor
	 **
	 **  \par Description
	 **       This function initializes the object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 *************************************************************************/
	HighPass();

	/************************************************************************/
	/** \brief High Pass Destructor
	 **
	 **  \par Description
	 **       This function destructs the object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 *************************************************************************/
	~HighPass();

	/************************************************************************/
	/** \brief Update High Pass
	 **
	 **  \par Description
	 **       This function
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

	/** \brief Previous filter input value */
	float m_PrevInput;

	/** \brief Previous filter output value */
	float m_PrevOutput;
};

#endif
