#ifndef MATRIX1F3_HPP
#define MATRIX1F3_HPP

#include "cfe.h"
#include "Vector3F.hpp"

#define M_DEG_TO_RAD_F		(0.01745329251994329576f)

namespace math
{

class Matrix1F3
{
public:

private:
	/** \brief Number of rows */
    static const int ROWS = 1;

    /** \brief Number of columns */
    static const int COLS = 3;

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
	Matrix1F3();

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
	 **
	 *************************************************************************/
	Matrix1F3(Vector3F m0);

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
	~Matrix1F3();

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
	 **  Row of the matrix as #Vector3F
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
	 **  Row of the matrix as #Vector3F
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector3F operator [] (uint32 i) const;

	/************************************************************************/
	/** \brief Multiplication Operator Override
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   vecIn      A #Vector3F to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication
	 **  \endreturns
	 **
	 *************************************************************************/
	float operator*(const Vector3F &vecIn);

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
	/** \brief Vectors of row data */
	Vector3F data[ROWS];

};

}

#endif
