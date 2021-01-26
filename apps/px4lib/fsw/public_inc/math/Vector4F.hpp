#ifndef VECTOR4F_HPP
#define VECTOR4F_HPP

#include "cfe.h"

namespace math
{

class Vector4F
{
private:
	/** \brief Size of vector */
	static const int SIZE = 4;

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
    Vector4F();

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
	 **
	 *************************************************************************/
	Vector4F(float A, float B, float C, float D);

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
	~Vector4F();

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
	/** \brief Vector Length Squared
	 **
	 **  \par Description
	 **       This function calculates the length of all the values of the
	 **       vector and squares it.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   i      A #uint32 specifying an index
	 **
	 **  \returns
	 **  Vector length squared
	 **  \endreturns
	 **
	 *************************************************************************/
    float LengthSquared(void) const;

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
	 **  \param [in]   vecIn      A #Vector4F to perform this operation on
	 **
	 **  \returns
	 **  Result of the addition
	 **  \endreturns
	 **
	 *************************************************************************/
    const Vector4F operator+(const Vector4F &vecIn) const;

	/************************************************************************/
	/** \brief Subtraction Operator Override
	 **
	 **  \par Description
	 **       This function overrides the subtraction behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   vecIn      A #Vector4F to perform this operation on
	 **
	 **  \returns
	 **  Result of the subtraction
	 **  \endreturns
	 **
	 *************************************************************************/
    const Vector4F operator-(const Vector4F &vecIn) const;

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
    const Vector4F operator*(const float scalar) const;

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
    const Vector4F operator/(const float scalar) const;

	/************************************************************************/
	/** \brief Vector Multiplication Operator Override
	 **
	 **  \par Description
	 **       This function overrides the vector multiplication behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   vecIn      A #Vector4F to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication
	 **  \endreturns
	 **
	 *************************************************************************/
    float operator*(const Vector4F &vecIn) const;

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
    const Vector4F operator-() const;

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
    const Vector4F & operator=(const Vector4F &v);

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
    Vector4F Normalized(void) const;

protected:
    /** \brief Data of vector */
	float data[SIZE];

};

}

#endif
