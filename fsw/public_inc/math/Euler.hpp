#ifndef EULER_HPP
#define EULER_HPP

#include "cfe.h"
#include "Vector3F.hpp"
#include "Quaternion.hpp"
#include "Matrix3F3.hpp"


namespace math
{

class Euler : public Vector3F
{

public:

	/************************************************************************/
	/** \brief Euler Matrix Constructor
	 **
	 **  \par Description
	 **       This function initializes the object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 *************************************************************************/
	Euler();

	/************************************************************************/
	/** \brief Euler Matrix Constructor
	 **
	 **  \par Description
	 **       This function initializes the object with passed values.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   dcm      A #Matrix3F3 generate the Euler numbers from
	 **
	 *************************************************************************/
	Euler(Matrix3F3 dcm);

	/************************************************************************/
	/** \brief Euler Matrix Destructor
	 **
	 **  \par Description
	 **       This function destructs the object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 *************************************************************************/
	~Euler();


protected:

};

}

#endif
