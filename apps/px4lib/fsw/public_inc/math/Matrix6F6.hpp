#ifndef MATRIX6F6_HPP
#define MATRIX6F6_HPP

#include "cfe.h"
#include "Vector6F.hpp"
#include "Matrix6F1.hpp"

namespace math
{

class Matrix6F6
{
private:
	/** \brief Number of rows and columns */
	static const int SIZE = 6;

	/** \brief Vectors of NaN row data */
	Vector6F nan[SIZE];

	/** \brief Vectors of row data */
	Vector6F data[SIZE];

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
	Matrix6F6();

	/************************************************************************/
	/** \brief Matrix Constructor
	 **
	 **  \par Description
	 **       This function initializes the object with passed values.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   m0      A #Vector6F specifying a row
	 **  \param [in]   m1      A #Vector6F specifying a row
	 **  \param [in]   m2      A #Vector6F specifying a row
	 **  \param [in]   m3      A #Vector6F specifying a row
	 **  \param [in]   m4      A #Vector6F specifying a row
	 **  \param [in]   m5      A #Vector6F specifying a row
	 **
	 *************************************************************************/
	Matrix6F6(Vector6F m0, Vector6F m1, Vector6F m2, Vector6F m3,
               Vector6F m4, Vector6F m5);

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
	~Matrix6F6();

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
	Vector6F& operator [] (uint32 i);

	/************************************************************************/
	/** \brief Index Operator Override
	 **
	 **  \par Description
	 **       This function overrides the [] behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **	 \param [in]   i      A #uint32 specifying a row
	 **
	 **  \returns
	 **  Row of the matrix
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector6F operator [] (uint32 i) const;

	/************************************************************************/
	/** \brief Addition Operator Override
	 **
	 **  \par Description
	 **       This function overrides the addition behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   matIn      A #Matrix6F6 to perform this operation on
	 **
	 **  \returns
	 **  Result of the addition
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix6F6 operator+(const Matrix6F6 &matIn) const;

	/************************************************************************/
	/** \brief Multiplication Operator Override
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   vecIn      A #Vector6F to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix6F1 operator*(const Vector6F &vecIn);

	/************************************************************************/
	/** \brief Inverse Matrix
	 **
	 **  \par Description
	 **       This function returns this matrix inversed.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \returns
	 **  Inversed Matrix
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix6F6 Inversed(void);

	/************************************************************************/
	/** \brief Identity Matrix
	 **
	 **  \par Description
	 **       This function returns an identity matrix of this size.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \returns
	 **  Identity matrix
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix6F6 Identity(void);

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

	/************************************************************************/
	/** \brief Swap Matrix Rows
	 **
	 **  \par Description
	 **       This function swaps the passed rows with each other.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
     **  \param [in]   a      A #uint8 specifying a row to swap
     **  \param [in]   b      A #uint8 specifying the row to swap row a with
	 **
	 *************************************************************************/
    void SwapRows(uint8 a, uint8 b);

	/************************************************************************/
	/** \brief Swap Matrix Columns
	 **
	 **  \par Description
	 **       This function swaps the passed columns with each other.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
     **  \param [in]   a      A #uint8 specifying a col to swap
     **  \param [in]   b      A #uint8 specifying the col to swap row a with
	 **
	 *************************************************************************/
    void SwapCols(uint8 a, uint8 b);


protected:

};

}

#endif
