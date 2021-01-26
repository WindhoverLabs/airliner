#ifndef MATRIX10F10_HPP
#define MATRIX10F10_HPP

#include "cfe.h"
#include "Vector10F.hpp"
#include "Matrix10F1.hpp"
#include "Matrix10F2.hpp"
#include "Matrix10F3.hpp"
#include "Matrix10F6.hpp"

#define M_DEG_TO_RAD_F		(0.01745329251994329576f)

namespace math
{

/* Forward declares */
class Matrix10F1;
class Matrix10F2;
class Matrix10F3;
class Matrix10F6;

class Matrix10F10
{
public:

private:
	/** \brief Number of rows and columns */
	static const int SIZE = 10;

	/** \brief Rotation lookup enumeration */
	Vector10F data[SIZE];

	/** \brief Vector of NaN row data */
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
	Matrix10F10();

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
	 **  \param [in]   m6      A #Vector10F specifying a row
	 **  \param [in]   m7      A #Vector10F specifying a row
	 **  \param [in]   m8      A #Vector10F specifying a row
	 **  \param [in]   m9      A #Vector10F specifying a row
	 **
	 *************************************************************************/
	Matrix10F10(Vector10F m0, Vector10F m1, Vector10F m2, Vector10F m3, Vector10F m4, Vector10F m5, Vector10F m6, Vector10F m7, Vector10F m8, Vector10F m9);

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
	~Matrix10F10();

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
	Matrix10F10 Transpose(void);

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
	static Matrix10F10 Identity(void);

	/************************************************************************/
	/** \brief Multiplication Operator Override Vector10F
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object
	 **       and a #Vector10F.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **	 \param [in]   vecIn      A #Vector10F to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication as #Vector10F
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector10F operator*(const Vector10F &vecIn);

	/************************************************************************/
	/** \brief Multiplication Operator Override Matrix10F1
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   matIn      A #Matrix10F1 to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication as #Matrix10F1
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix10F1 operator*(const Matrix10F1 &matIn);

	/************************************************************************/
	/** \brief Multiplication Operator Override Matrix10F3
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
	 **  Result of the multiplication as #Matrix10F3
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix10F3 operator*(const Matrix10F3 &matIn);

	/************************************************************************/
	/** \brief Multiplication Operator Override Matrix10F2
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
	 **  Result of the multiplication as #Matrix10F2
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix10F2 operator*(const Matrix10F2 &matIn);

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
	 **  Result of the multiplication as #Matrix10F6
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix10F6 operator*(const Matrix10F6 &matIn);

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
	 **  Result of the multiplication as #Matrix10F10
	 **  \endreturns
	 **
	 *************************************************************************/
	Matrix10F10 operator*(const Matrix10F10 &matIn);

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
	Matrix10F10 operator*(const float &scalar);

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
	/** \brief Addition Operator Override
	 **
	 **  \par Description
	 **       This function overrides the addition behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   matIn      A #Matrix10F10 to perform this operation on
	 **
	 **  \returns
	 **  Result of the addition
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix10F10 operator+(const Matrix10F10 &matIn) const;

	/************************************************************************/
	/** \brief Subtraction Operator Override
	 **
	 **  \par Description
	 **       This function overrides the addition behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   matIn      A #Matrix10F10 to perform this operation on
	 **
	 **  \returns
	 **  Result of the subtraction
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix10F10 operator-(const Matrix10F10 &matIn) const;


    void Print();


protected:

};

}

#endif
