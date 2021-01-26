#ifndef VECTOR1F_HPP
#define VECTOR1F_HPP

#include "cfe.h"

namespace math
{

class Vector1F
{
private:

	/** \brief Size of vector */
	static const int SIZE = 1;

	/** \brief Data of vector */
	float data[SIZE];

	/** \brief NaN data */
	float nan;

public:
	/************************************************************************/
	/** \brief Vector Constructor
	 **
	 **  \par Description
	 **       This function initializes the object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 *************************************************************************/
	Vector1F();

	/************************************************************************/
	/** \brief Vector Constructor
	 **
	 **  \par Description
	 **       This function initializes the object with passed values.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   A      A #float specifying a value
	 **
	 *************************************************************************/
	Vector1F(float A);

	/************************************************************************/
	/** \brief Vector Destructor
	 **
	 **  \par Description
	 **       This function destructs the object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 *************************************************************************/
	~Vector1F();

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
	float& operator [] (uint32 i);

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

	/************************************************************************/
	/** \brief Vector Length
	 **
	 **  \par Description
	 **       This function calculates the length of all the values of the vector.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   i      A #uint32 specifying an index
	 **
	 **  \returns
	 **  Vector length
	 **  \endreturns
	 **
	 *************************************************************************/
	float Length(void) const;

	/************************************************************************/
	/** \brief Zero Vector
	 **
	 **  \par Description
	 **       This function zeroes out all values of the vector.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 *************************************************************************/
	void Zero(void);

	/************************************************************************/
	/** \brief Addition Operator Override
	 **
	 **  \par Description
	 **       This function overrides the addition behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   vecIn      A #Vector1F to perform this operation on
	 **
	 **  \returns
	 **  Result of the addition
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector1F operator+(const Vector1F &vecIn);

	/************************************************************************/
	/** \brief Subtraction Operator Override
	 **
	 **  \par Description
	 **       This function overrides the subtraction behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   vecIn      A #Vector1F to perform this operation on
	 **
	 **  \returns
	 **  Result of the subtraction
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector1F operator-(const Vector1F &vecIn);

	/************************************************************************/
	/** \brief Subtraction Operator Override
	 **
	 **  \par Description
	 **       This function overrides the subtraction behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   vecIn      A #Vector1F to perform this operation on
	 **
	 **  \returns
	 **  Result of the subtraction
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector1F operator-(const Vector1F &vecIn) const;

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
	Vector1F operator*(const float scalar);

	/************************************************************************/
	/** \brief Scalar Division Operator Override
	 **
	 **  \par Description
	 **       This function overrides the scalar division behavior when used on this object.
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
	Vector1F operator/(const float scalar);

	/************************************************************************/
	/** \brief Vector Power
	 **
	 **  \par Description
	 **       This function raises the vector to the passed power.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   scalar      A #float to perform this operation on
	 **
	 **  \returns
	 **  Result of the power
	 **  \endreturnsv
	 **
	 *************************************************************************/
	Vector1F pow(const float scalar) const;

	/************************************************************************/
	/** \brief Vector Multiplication Operator Override
	 **
	 **  \par Description
	 **       This function overrides the vector multiplication behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   scalar      A #Vector1F to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication
	 **  \endreturns
	 **
	 *************************************************************************/
	float operator*(const Vector1F &vecIn);

	/************************************************************************/
	/** \brief Negative Operator Override
	 **
	 **  \par Description
	 **       This function overrides the negative behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \returns
	 **  Negative of the vector
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector1F operator-() const;

	/************************************************************************/
	/** \brief Vector Multiplication
	 **
	 **  \par Description
	 **       This function multiplies this object with the passed vector.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   vecIn      A #Vector1F to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector1F EMult(const Vector1F &vecIn);

	/************************************************************************/
	/** \brief Vector Constrain
	 **
	 **  \par Description
	 **       This function constrains a vector value between a passed range.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   i      A #uint32 index of value to constrain
	 **  \param [in]   min    A #float of min value
	 **  \param [in]   max    A #float of max value
	 **
	 *************************************************************************/
	void Constrain(uint32 i, float min, float max);

	/************************************************************************/
	/** \brief Normalized Vector
	 **
	 **  \par Description
	 **       This function normalizes this vector and returns it.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \returns
	 **  This vector normalized
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector1F Normalized(void);

	/************************************************************************/
	/** \brief Normalize Vector
	 **
	 **  \par Description
	 **       This function normalizes this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 *************************************************************************/
	void Normalize(void);

	/************************************************************************/
	/** \brief Vector Division
	 **
	 **  \par Description
	 **       This function divides this object with the passed vector.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   v      A #Vector1F to perform this operation on
	 **
	 **  \returns
	 **  Result of the division
	 **  \endreturns
	 **
	 *************************************************************************/
	const Vector1F EDivide(const Vector1F &v) const;

protected:

};

}

#endif
