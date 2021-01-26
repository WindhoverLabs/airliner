#ifndef MATRIX10F3_HPP
#define MATRIX10F3_HPP

#include "cfe.h"
#include "Vector3F.hpp"
#include "Vector10F.hpp"
#include "Matrix3F3.hpp"
#include "Matrix3F10.hpp"
#include "Matrix10F10.hpp"

#define M_DEG_TO_RAD_F		(0.01745329251994329576f)

namespace math
{

/* Forward declares */
class Matrix3F10;
class Matrix10F10;
class Vector10F;

class Matrix10F3
{
public:

private:
	/** \brief Number of rows */
	static const int ROWS = 10;

	/** \brief Number of columns */
	static const int COLS = 3;

	/** \brief Vectors of row data */
	Vector3F data[ROWS];

	/** \brief Vectors of NaN row data */
	Vector3F nan;

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
	Matrix10F3();

	/************************************************************************/
	/** \brief Matrix Constructor
	 **
	 **  \par Description
	 **       This function initializes the object with passed values.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   m0      A #Vector3F specifying a row
	 **  \param [in]   m1      A #Vector3F specifying a row
	 **  \param [in]   m2      A #Vector3F specifying a row
	 **  \param [in]   m3      A #Vector3F specifying a row
	 **  \param [in]   m4      A #Vector3F specifying a row
	 **  \param [in]   m5      A #Vector3F specifying a row
	 **  \param [in]   m6      A #Vector3F specifying a row
	 **  \param [in]   m7      A #Vector3F specifying a row
	 **  \param [in]   m8      A #Vector3F specifying a row
	 **  \param [in]   m9      A #Vector3F specifying a row
	 **
	 *************************************************************************/
	Matrix10F3(Vector3F m0, Vector3F m1, Vector3F m2, Vector3F m3, Vector3F m4, Vector3F m5, Vector3F m6, Vector3F m7, Vector3F m8, Vector3F m9);

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
	~Matrix10F3();

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
	Vector3F& operator [] (uint32 i);

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
	Vector3F operator [] (uint32 i) const;

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
	Matrix3F10 Transpose(void);

	/************************************************************************/
	/** \brief Multiplication Operator Override Vector3F
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object
	 **       and a #Vector3F.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **	 \param [in]   vecIn      A #Vector3F to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication as #Vector10F
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector10F operator*(const Vector3F &vecIn);

	/************************************************************************/
	/** \brief Multiplication Operator Override Matrix3F3
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   matIn      A #Matrix3F3 to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication as #Matrix10F3
	 **  \endreturns
	 **
	 *************************************************************************/
	Matrix10F3 operator*(const Matrix3F3 &matIn);

	/************************************************************************/
	/** \brief Multiplication Operator Override Matrix3F10
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   matIn      A #Matrix3F10 to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication as #Matrix10F10
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix10F10 operator*(const Matrix3F10 &matIn);

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
	Matrix10F3 operator*(const float &scalar);

	/************************************************************************/
	/** \brief Addition Operator Override
	 **
	 **  \par Description
	 **       This function overrides the addition behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   matIn      A #Matrix10F3 to perform this operation on
	 **
	 **  \returns
	 **  Result of the addition
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix10F3 operator+(const Matrix10F3 &matIn) const;

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
