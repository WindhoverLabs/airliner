#ifndef MATRIX2F2_HPP
#define MATRIX2F2_HPP

#include "cfe.h"
#include "Vector2F.hpp"

namespace math
{

class Matrix2F2
{
private:

	/** \brief Number of rows and columns */
	static const int SIZE = 2;

	/** \brief Vector of NaN row data */
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
	Matrix2F2();

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
	 **
	 *************************************************************************/
    Matrix2F2(Vector2F m0, Vector2F m1);

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
    ~Matrix2F2();

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
    Matrix2F2 Transpose(void);

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
    static Matrix2F2 Identity(void);

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
	 **  Result of the multiplication as #Vector2F
	 **  \endreturns
	 **
	 *************************************************************************/
    Vector2F operator*(const Vector2F &vecIn);

	/************************************************************************/
	/** \brief Multiplication Operator Override
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
	 **  Result of the multiplication
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix2F2 operator*(const Matrix2F2 &matIn);

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
    Matrix2F2 operator*(const float &scalar);

	/************************************************************************/
	/** \brief Addition Operator Override
	 **
	 **  \par Description
	 **       This function overrides the addition behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   matIn      A #Matrix2F2 to perform this operation on
	 **
	 **  \returns
	 **  Result of the addition
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix2F2 operator+(const Matrix2F2 &matIn) const;

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
    Matrix2F2 Inversed(void);

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
    /** \brief Vectors of row data */
	Vector2F data[SIZE];

};

}

#endif
