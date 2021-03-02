#ifndef MATRIX2F10_HPP
#define MATRIX2F10_HPP

#include "cfe.h"
#include "Vector10F.hpp"
#include "Vector2F.hpp"
#include "Matrix2F2.hpp"
#include "Matrix10F2.hpp"
#include "Matrix10F10.hpp"

#define M_DEG_TO_RAD_F		(0.01745329251994329576f)

namespace math
{

/* Forward declares */
class Matrix10F2;
class Matrix10F10;

class Matrix2F10
{
public:


private:
	/** \brief Number of rows */
	static const int ROWS = 2;

	/** \brief Number of columns */
	static const int COLS = 10;

	/** \brief Vectors of NaN row data */
	Vector10F nan;

	/** \brief Vectors of row data */
	Vector10F data[ROWS];

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
	Matrix2F10();

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
	 **
	 *************************************************************************/
	Matrix2F10(Vector10F m0, Vector10F m1);

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
	~Matrix2F10();

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
	Matrix10F2 Transpose(void);

	/************************************************************************/
	/** \brief Multiplication Operator Override Vector10F
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object
	 **       and a #Vector2F.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **	 \param [in]   vecIn      A #Vector10F to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication as #Vector2F
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector2F operator*(const Vector10F &vecIn);

	/************************************************************************/
	/** \brief Multiplication Operator Override
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   matIn      A #Matrix10F2 to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication
	 **  \endreturns
	 **
	 *************************************************************************/
	Matrix2F2 operator*(const Matrix10F2 &matIn);
	
	/************************************************************************/
	/** \brief Multiplication Operator Override
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   matIn      A #Matrix10F10 to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication
	 **  \endreturns
	 **
	 *************************************************************************/	
	Matrix2F10 operator*(const Matrix10F10 &matIn);


	void Zero(void);

protected:

};

}

#endif
