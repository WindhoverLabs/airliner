#ifndef MATRIX1F6_HPP
#define MATRIX1F6_HPP


#include "cfe.h"
#include "Vector6F.hpp"
#include "Matrix1F1.hpp"
#include "Matrix6F1.hpp"

namespace math
{

/* Forward declare 6F*/
class Vector6F;

class Matrix1F6
{
private:
	/** \brief Number of rows */
    static const int ROWS = 1;

    /** \brief Number of columns */
    static const int COLS = 6;

    /** \brief Vectors of row data */
    Vector6F data[ROWS];

    /** \brief Vectors of NaN row data */
    Vector6F nan;

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
    Matrix1F6();

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
	 **
	 *************************************************************************/
    Matrix1F6(Vector6F m0);

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
    ~Matrix1F6();

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
	/** \brief Multiplication Operator Override
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   matIn      A #Matrix6F1 to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix1F1 operator*(const Matrix6F1 &matIn);

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
