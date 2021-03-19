#ifndef VECTOR10F_HPP
#define VECTOR10F_HPP

#include "cfe.h"
#include "math/Vector3F.hpp"

namespace math
{

class Vector3F;

class Vector10F
{
private:

	/** \brief Size of vector */
	static const int SIZE = 10;

	/** \brief NaN data */
	float nan;

	/** \brief Data of vector */
	float data[SIZE];

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
	Vector10F();

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
	 **  \param [in]   B      A #float specifying a value
	 **  \param [in]   C      A #float specifying a value
	 **  \param [in]   D      A #float specifying a value
	 **  \param [in]   E      A #float specifying a value
	 **  \param [in]   F      A #float specifying a value
	 **  \param [in]   G      A #float specifying a value
	 **  \param [in]   H      A #float specifying a value
	 **  \param [in]   I      A #float specifying a value
	 **  \param [in]   J      A #float specifying a value
	 **
	 *************************************************************************/
	Vector10F(float A, float B, float C, float D, float E, float F, float G, float H, float I, float J);

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
	~Vector10F();

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
	 **  \param [in]   vecIn      A #Vector10F to perform this operation on
	 **
	 **  \returns
	 **  Result of the addition
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector10F operator+(const Vector10F &vecIn);

	/************************************************************************/
	/** \brief Addition Operator Override
	 **
	 **  \par Description
	 **       This function overrides the addition behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   vecIn      A #Vector10F to perform this operation on
	 **
	 **  \returns
	 **  Result of the addition
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector10F operator+(const Vector10F &vecIn) const;

	/************************************************************************/
	/** \brief Addition Equals Operator Override
	 **
	 **  \par Description
	 **       This function overrides the addition equals (+=) behavior when
	 **       used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   vecIn      A #Vector10F to perform this operation on
	 **
	 *************************************************************************/
	void operator+=(const Vector10F &vecIn);

	/************************************************************************/
	/** \brief Subtraction Operator Override
	 **
	 **  \par Description
	 **       This function overrides the subtraction behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   vecIn      A #Vector10F to perform this operation on
	 **
	 **  \returns
	 **  Result of the subtraction
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector10F operator-(const Vector10F &vecIn);

	/************************************************************************/
	/** \brief Subtraction Operator Override
	 **
	 **  \par Description
	 **       This function overrides the Subtraction behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   vecIn      A #Vector10F to perform this operation on
	 **
	 **  \returns
	 **  Result of the subtraction
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector10F operator-(const Vector10F &vecIn) const;

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
	Vector10F operator*(const float scalar);

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
	Vector10F operator/(const float scalar);

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
	Vector10F operator-() const;

	/************************************************************************/
	/** \brief Vector Multiplication
	 **
	 **  \par Description
	 **       This function multiplies this object with the passed vector.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   vecIn      A #Vector10F to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector10F EMult(const Vector10F &vecIn);

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
	 **  \param [in]   min    A #float index of value to constrain
	 **  \param [in]   max    A #float index of value to constrain
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
	Vector10F Normalized(void);

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
	 **  \param [in]   v      A #Vector10F to perform this operation on
	 **
	 **  \returns
	 **  Result of the division
	 **  \endreturns
	 **
	 *************************************************************************/
	const Vector10F EDivide(const Vector10F &v) const;

protected:

};

}

#endif
