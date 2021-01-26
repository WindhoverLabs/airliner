#ifndef LOWPASSVECTOR10F_H
#define LOWPASSVECTOR10F_H

#include "cfe.h"
#include "math.h"
#include "Vector10F.hpp"
#include "Matrix10F10.hpp"

class LowPassVector10F
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
	LowPassVector10F();

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
	~LowPassVector10F();

	/************************************************************************/
	/** \brief Update Low Pass
	 **
	 **  \par Description
	 **       This function updates the low pass filter with new values.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   value      A #float specifying the input value
	 **  \param [in]   dt         A #float specifying delta time in us
	 **  \param [in]   cutoff     A #float specifying cutoff frequency
	 **
	 *************************************************************************/
	math::Vector10F Update(math::Vector10F input, float dt, float cutoff);

	/************************************************************************/
	/** \brief Index Operator Override
	 **
	 **  \par Description
	 **       This function overrides the [] behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   i      A #uint32 specifying an index
	 **
	 **  \returns
	 **  Value at passed index of the vector or NaN if out of bounds
	 **  \endreturns
	 **
	 *************************************************************************/
    float operator [] (uint32 i) const;

private:
    /** \brief Initialization flag */
	bool m_Initialized;

	/** \brief State value */
	math::Vector10F m_State;
};

#endif
