
#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include "Vector4F.hpp"
#include "Vector3F.hpp"
#include "Matrix3F3.hpp"
#include "Matrix4F4.hpp"

namespace math
{

class Quaternion : public Vector4F
{
public:
	/************************************************************************/
	/** \brief Quaternion Constructor
	 **
	 **  \par Description
	 **       This function initializes the object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 *************************************************************************/
	Quaternion();

	/************************************************************************/
	/** \brief Quaternion Constructor
	 **
	 **  \par Description
	 **       This function initializes the object with passed values.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   a      A #float specifying a value
	 **  \param [in]   b      A #float specifying a value
	 **  \param [in]   c      A #float specifying a value
	 **  \param [in]   d      A #float specifying a value
	 **
	 *************************************************************************/
	Quaternion(float a, float b, float c, float d);

	/************************************************************************/
	/** \brief Quaternion Constructor
	 **
	 **  \par Description
	 **       This function initializes the object from roll, pitch, and yaw.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   roll       A #float used to initialize the Quaternion
	 **  \param [in]   pitch      A #float used to initialize the Quaternion
	 **  \param [in]   yaw        A #float used to initialize the Quaternion
	 **
	 *************************************************************************/
	Quaternion(float roll, float pitch, float yaw);

	/************************************************************************/
	/** \brief Quaternion Constructor
	 **
	 **  \par Description
	 **       This function initializes the object from a Matrix3F3.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   mat      A #Matrix3F3 used to initialize the Quaternion
	 **
	 *************************************************************************/
	Quaternion(const Matrix3F3 &mat);
	
	/************************************************************************/
	/** \brief Quaternion Constructor
	 **
	 **  \par Description
	 **       This function initializes the object from a axis angle.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   mat      A #Matrix3F3 used to initialize the Quaternion
	 **
	 *************************************************************************/
	Quaternion(const Vector3F &aa);

	/************************************************************************/
	/** \brief Quaternion Destructor
	 **
	 **  \par Description
	 **       This function destructs the object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 *************************************************************************/
	~Quaternion();

	/************************************************************************/
	/** \brief Rotation Matrix
	 **
	 **  \par Description
	 **       This function returns a rotation matrix based off the current
	 **       values.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \returns
	 **  An rotation matrix.
	 **  \endreturns
	 **
	 *************************************************************************/
	Matrix3F3 RotationMatrix(void) const;

	/************************************************************************/
	/** \brief Imaginary
	 **
	 **  \par Description
	 **       This function returns the imaginary part of the Quaternion.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \returns
	 **  #Vector3F of imaginary values
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector3F Imaginary(void);

	/************************************************************************/
	/** \brief Convert To Euler
	 **
	 **  \par Description
	 **       This function computes Euler values of this matrix.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \returns
	 **  #Vector3F of Euler values
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector3F ToEuler(void) const;

	/************************************************************************/
	/** \brief Copy Data
	 **
	 **  \par Description
	 **       This function copies the quaternion data to another data structure.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in/out]   dst      A structure to copy data into
	 **
	 *************************************************************************/
	void copyTo(float (&dst)[4]);

	/************************************************************************/
	/** \brief Scalar Multiplication Operator Override
	 **
	 **  \par Description
	 **       This function overrides the scalar multiplication behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   scalar      A #float to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication
	 **  \endreturns
	 **
	 *************************************************************************/
	const Quaternion operator*(const float scalar) const;

	/************************************************************************/
	/** \brief Equal Operator Override
	 **
	 **  \par Description
	 **       This function overrides the set equals behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \returns
	 **  This object
	 **  \endreturns
	 **
	 *************************************************************************/
	const Quaternion & operator=(const Quaternion &q);

	/************************************************************************/
	/** \brief Addition Operator Override
	 **
	 **  \par Description
	 **       This function overrides the addition behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   vecIn      A #Quaternion to perform this operation on
	 **
	 **  \returns
	 **  Result of the addition
	 **  \endreturns
	 **
	 *************************************************************************/
	const Quaternion operator+(const Quaternion &vecIn) const;

	/************************************************************************/
	/** \brief Subtraction Operator Override
	 **
	 **  \par Description
	 **       This function overrides the subtraction behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   vecIn      A #Quaternion to perform this operation on
	 **
	 **  \returns
	 **  Result of the subtraction
	 **  \endreturns
	 **
	 *************************************************************************/
	const Quaternion operator-(const Quaternion &vecIn) const;

	/************************************************************************/
	/** \brief Quaternion Multiplication Operator Override
	 **
	 **  \par Description
	 **       This function overrides the quaternion multiplication behavior
	 **       when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   vecIn      A #Quaternion to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication
	 **  \endreturns
	 **
	 *************************************************************************/
	const Quaternion operator*(const Quaternion &q) const;

	/************************************************************************/
	/** \brief Quaternion Division Operator Override
	 **
	 **  \par Description
	 **       This function overrides the quaternion division behavior
	 **       when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   q      A #Quaternion to perform this operation on
	 **
	 **  \returns
	 **  Result of the division
	 **  \endreturns
	 **
	 *************************************************************************/
	Quaternion operator/(const Quaternion &q) const;

	/************************************************************************/
	/** \brief Quaternion Conjugate
	 **
	 **  \par Description
	 **       This function returns the conjugate of the quaternion.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   v      #Vector3F input
	 **
	 **  \returns
	 **  Conjugate
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector3F Conjugate(const Vector3F &v) const;

	/************************************************************************/
	/** \brief Quaternion Conjugate Inversed
	 **
	 **  \par Description
	 **       This function returns the inverse conjugate of the quaternion.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   v      #Vector3F input
	 **
	 **  \returns
	 **  Inversed conjugate
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector3F ConjugateInversed(const Vector3F &v) const;

	/************************************************************************/
	/** \brief From DCM
	 **
	 **  \par Description
	 **       This function initializes the quaternion from a DCM.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   dcm      #Matrix3F3 DCM to generate quaternion from
	 **
	 *************************************************************************/
	void FromDCM(const Matrix3F3 &dcm);

	/************************************************************************/
	/** \brief From Yaw
	 **
	 **  \par Description
	 **       This function initializes the quaternion from yaw.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   yaw      #float DCM to generate quaternion from
	 **
	 *************************************************************************/
	void FromYaw(float yaw);

	/************************************************************************/
	/** \brief Derivative
	 **
	 **  \par Description
	 **       This function calculates the derivative of the quaternion.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   w      #Vector3F input
	 **
	 *************************************************************************/
	const Quaternion Derivative(const Vector3F &w);

protected:

};

}

#endif
