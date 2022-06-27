#ifndef EULER_HPP
#define EULER_HPP

#include "cfe.h"
#include "Vector3F.hpp"
#include "Quaternion.hpp"
#include "Matrix3F3.hpp"
#include "Dcm.hpp"

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

    /**
     * Constructor from quaternion instance.
     *
     * Instance is set from a quaternion representing transformation
     * from frame 2 to frame 1.
     * This instance will hold the angles defining the 3-2-1 intrinsic
     * Tait-Bryan rotation sequence from frame 1 to frame 2.
     *
     * @param q quaternion
    */
    Euler(Quaternion q);

    /**
     * Constructor from euler angles
     *
     * Instance is initialized from an 3-2-1 intrinsic Tait-Bryan
     * rotation sequence representing transformation from frame 1
     * to frame 2.
     *
     * @param phi_ rotation angle about X axis
     * @param theta_ rotation angle about Y axis
     * @param psi_ rotation angle about Z axis
     */
    Euler(float phi_, float theta_, float psi_);
    inline float Phi() const
    {
        return  data[0];
    }
    inline float Theta() const
    {
        return data[1];
    }
    inline float Psi() const
    {
        return data[2];
    }


protected:

};

}

#endif
