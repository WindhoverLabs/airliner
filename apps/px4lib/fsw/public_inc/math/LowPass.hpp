#ifndef LOWPASS_H
#define LOWPASS_H

#include "cfe.h"

class LowPass
{
public:
	/************************************************************************/
	/** \brief Low Pass Constructor
	 **
	 **  \par Description
	 **       This function initializes the object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 *************************************************************************/
	LowPass();

	/************************************************************************/
	/** \brief Low Pass Destructor
	 **
	 **  \par Description
	 **       This function destructs the object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 *************************************************************************/
	~LowPass();

	/************************************************************************/
	/** \brief Update Low Pass
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

	/** \brief Initialization flag */
	bool m_Initialized;

	/** \brief State value */
	float m_State;
};

#endif
