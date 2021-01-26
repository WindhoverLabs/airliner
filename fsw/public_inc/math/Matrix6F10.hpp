#ifndef MATRIX6F10_HPP
#define MATRIX6F10_HPP

#include "cfe.h"
#include "Vector6F.hpp"
#include "Vector10F.hpp"
#include "Matrix10F6.hpp"
#include "Matrix6F6.hpp"
#include "Matrix10F10.hpp"

namespace math
{

class Matrix10F6;
class Matrix10F10;

class Matrix6F10
{
private:
	/** \brief Number of rows */
	static const int ROWS = 6;

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
	Matrix6F10();

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
	 **  \param [in]   m3      A #Vector10F specifying a row
	 **  \param [in]   m4      A #Vector10F specifying a row
	 **  \param [in]   m5      A #Vector10F specifying a row
	 **
	 *************************************************************************/
	Matrix6F10(Vector10F m0, Vector10F m1, Vector10F m2, Vector10F m3,
               Vector10F m4, Vector10F m5);

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
	~Matrix6F10();

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
	 **  Row of the matrix
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector10F operator [] (uint32 i) const;

	/************************************************************************/
	/** \brief Multiplication Operator Override Vector10F
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   vecIn      A #Vector10F to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication as #Vector6F
	 **  \endreturns
	 **
	 *************************************************************************/
    Vector6F operator*(const Vector10F &vecIn);

	/************************************************************************/
	/** \brief Multiplication Operator Override Matrix10F6
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   matIn      A #Matrix10F6 to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication as #Matrix6F6
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix6F6 operator*(const Matrix10F6 &matIn);

	/************************************************************************/
	/** \brief Multiplication Operator Override Matrix10F10
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
	 **  Result of the multiplication as #Matrix6F10
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix6F10 operator*(const Matrix10F10 &matIn);

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
    Matrix10F6 Transpose(void);

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
