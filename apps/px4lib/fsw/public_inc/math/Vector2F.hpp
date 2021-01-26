#ifndef VECTOR2F_HPP
#define VECTOR2F_HPP

#include "cfe.h"

namespace math
{

class Matrix1F2;

class Vector2F
{
private:
	/** \brief Size of vector */
	static const int SIZE = 2;

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
	Vector2F();

	/************************************************************************/
	/** \brief Vector Constructor
	 **
	 **  \par Description
	 **       This function initializes the object with passed values.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   X      A #float specifying a value
	 **  \param [in]   Y      A #float specifying a value
	 **
	 *************************************************************************/
	Vector2F(float X, float Y);

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
	~Vector2F();

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
    Vector2F pow(const float scalar) const;

	/************************************************************************/
	/** \brief Addition Operator Override
	 **
	 **  \par Description
	 **       This function overrides the addition behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   vecIn      A #Vector2F to perform this operation on
	 **
	 **  \returns
	 **  Result of the addition
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector2F operator+(const Vector2F &vecIn);

	/************************************************************************/
	/** \brief Subtraction Operator Override
	 **
	 **  \par Description
	 **       This function overrides the subtraction behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   vecIn      A #Vector2F to perform this operation on
	 **
	 **  \returns
	 **  Result of the subtraction
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector2F operator-(const Vector2F &vecIn);

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
	Vector2F operator*(const float scalar);

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
	Vector2F operator*(const float scalar) const;

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
	 **  Result of the division
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector2F operator/(const float scalar);

	/************************************************************************/
	/** \brief Vector Multiplication Operator Override
	 **
	 **  \par Description
	 **       This function overrides the vector multiplication behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   vecIn      A #Vector2F to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication
	 **  \endreturns
	 **
	 *************************************************************************/
	float operator*(const Vector2F &vecIn);

	/************************************************************************/
	/** \brief Vector Multiplication Operator Override
	 **
	 **  \par Description
	 **       This function overrides the vector multiplication behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   vecIn      A #Vector2F to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication
	 **  \endreturns
	 **
	 *************************************************************************/
	float operator*(const Vector2F &vecIn) const;

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
	Vector2F operator-() const;

	/************************************************************************/
	/** \brief Vector Multiplication
	 **
	 **  \par Description
	 **       This function multiplies this object with the passed vector.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   vecIn      A #Vector2F to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector2F EMult(const Vector2F &vecIn);

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
	Vector2F Normalized(void);
	
	/************************************************************************/
	/** \brief Transpose Vector
	 **
	 **  \par Description
	 **       This function returns this vector transposed.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \returns
	 **  Transposed vector as #Matrix1F2
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix1F2 Transpose(void);

protected:

};

}

#endif
