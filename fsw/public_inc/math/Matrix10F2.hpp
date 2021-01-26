#ifndef MATRIX10F2_HPP
#define MATRIX10F2_HPP

#include "cfe.h"
#include "Vector2F.hpp"
#include "Vector10F.hpp"
#include "Matrix2F2.hpp"
#include "Matrix2F10.hpp"
#include "Matrix10F10.hpp"

namespace math
{

/* Forward declares */
class Matrix2F10;
class Matrix10F10;
class Vector10F;

class Matrix10F2
{
public:

private:
	/** \brief Number of rows */
	static const int ROWS = 10;

	/** \brief Number of columns */
	static const int COLS = 2;

	/** \brief Vectors of row data */
	Vector2F data[ROWS];

	/** \brief Vectors of NaN row data */
	Vector2F nan;

public:
	/************************************************************************/
	/** \brief Matrix Constructor
	 **
	 **  \par Description
	 **       This function initializes the object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 *************************************************************************/
	Matrix10F2();

	/************************************************************************/
	/** \brief Matrix Constructor
	 **
	 **  \par Description
	 **       This function initializes the object with passed values.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   m0      A #Vector2F specifying a row
	 **  \param [in]   m1      A #Vector2F specifying a row
	 **  \param [in]   m2      A #Vector2F specifying a row
	 **  \param [in]   m3      A #Vector2F specifying a row
	 **  \param [in]   m4      A #Vector2F specifying a row
	 **  \param [in]   m5      A #Vector2F specifying a row
	 **  \param [in]   m6      A #Vector2F specifying a row
	 **  \param [in]   m7      A #Vector2F specifying a row
	 **  \param [in]   m8      A #Vector2F specifying a row
	 **  \param [in]   m9      A #Vector2F specifying a row
	 **
	 *************************************************************************/
	Matrix10F2(Vector2F m0, Vector2F m1, Vector2F m2, Vector2F m3, Vector2F m4, Vector2F m5, Vector2F m6, Vector2F m7, Vector2F m8, Vector2F m9);

	/************************************************************************/
	/** \brief Matrix Destructor
	 **
	 **  \par Description
	 **       This function destructs the object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 *************************************************************************/
	~Matrix10F2();

	/************************************************************************/
	/** \brief Index Operator Override
	 **
	 **  \par Description
	 **       This function overrides the [] behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   i      A #uint32 specifying a row
	 **
	 **  \returns
	 **  Row of the matrix
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector2F& operator [] (uint32 i);

	/************************************************************************/
	/** \brief Index Operator Override
	 **
	 **  \par Description
	 **       This function overrides the [] behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   i      A #uint32 specifying a row
	 **
	 **  \returns
	 **  Row of the matrix
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector2F operator [] (uint32 i) const;

	/************************************************************************/
	/** \brief Transpose Matrix
	 **
	 **  \par Description
	 **       This function returns this matrix transposed.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \returns
	 **  Transposed matrix
	 **  \endreturns
	 **
	 *************************************************************************/
	Matrix2F10 Transpose(void);

	/************************************************************************/
	/** \brief Multiplication Operator Override Vector2F
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object
	 **       and a #Vector2F.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **	 \param [in]   vecIn      A #Vector2F to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication as #Vector10F
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector10F operator*(const Vector2F &vecIn);

	/************************************************************************/
	/** \brief Multiplication Operator Override Matrix2F2
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   matIn      A #Matrix2F2 to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication as #Matrix10F2
	 **  \endreturns
	 **
	 *************************************************************************/
	Matrix10F2 operator*(const Matrix2F2 &matIn);

	/************************************************************************/
	/** \brief Multiplication Operator Override Matrix2F10
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   matIn      A #Matrix2F10 to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication as #Matrix10F10
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix10F10 operator*(const Matrix2F10 &matIn);

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
	Matrix10F2 operator*(const float &scalar);

	/************************************************************************/
	/** \brief Addition Operator Override
	 **
	 **  \par Description
	 **       This function overrides the addition behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   matIn      A #Matrix10F2 to perform this operation on
	 **
	 **  \returns
	 **  Result of the addition
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix10F2 operator+(const Matrix10F2 &matIn) const;

	/************************************************************************/
	/** \brief Zero Matrix
	 **
	 **  \par Description
	 **       This function zeroes out all values of the matrix.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 *************************************************************************/
	void Zero(void);
protected:

};

}

#endif
