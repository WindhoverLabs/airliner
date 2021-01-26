#ifndef MATRIX3F10_HPP
#define MATRIX3F10_HPP

#include "cfe.h"
#include "Vector10F.hpp"
#include "Vector3F.hpp"
#include "Matrix3F3.hpp"
#include "Matrix10F3.hpp"
#include "Matrix10F10.hpp"
#include "Matrix3F3.hpp"

#define M_DEG_TO_RAD_F		(0.01745329251994329576f)

namespace math
{

/* Forward declares */
class Matrix10F3;
class Matrix10F10;

class Matrix3F10
{
public:


private:
	/** \brief Number of rows */
	static const int ROWS = 3;

	/** \brief Number of columns */
	static const int COLS = 10;

	/** \brief Vectors of row data */
	Vector10F data[ROWS];

	/** \brief Vectors of NaN row data */
	Vector10F nan;

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
	Matrix3F10();

	/************************************************************************/
	/** \brief Matrix Constructor
	 **
	 **  \par Description
	 **       This function initializes the object with passed values.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   m0      A #Vector10F specifying a row
	 **  \param [in]   m1      A #Vector10F specifying a row
	 **  \param [in]   m2      A #Vector10F specifying a row
	 **
	 *************************************************************************/
	Matrix3F10(Vector10F m0, Vector10F m1, Vector10F m2);

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
	~Matrix3F10();

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
	 **  Row of the matrix as #Vector10F
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector10F& operator [] (uint32 i);

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
	 **  Row of the matrix as #Vector10F
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector10F operator [] (uint32 i) const;

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
	Matrix10F3 Transpose(void);

	/************************************************************************/
	/** \brief Multiplication Operator Override Vector10F
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object
	 **       and a #Vector3F.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **	 \param [in]   vecIn      A #Vector10F to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication as #Vector3F
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector3F operator*(const Vector10F &vecIn);

	/************************************************************************/
	/** \brief Multiplication Operator Override
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   matIn      A #Matrix10F3 to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication
	 **  \endreturns
	 **
	 *************************************************************************/
	Matrix3F3 operator*(const Matrix10F3 &matIn);


	void Zero(void);

protected:

};

}

#endif
