#ifndef MATRIX1F10_HPP
#define MATRIX1F10_HPP


#include "cfe.h"
#include "Vector10F.hpp"
#include "Matrix10F10.hpp"
#include "Matrix10F1.hpp"
#include "Matrix1F1.hpp"
#include "Vector1F.hpp"

namespace math
{

/* Forward declare 10F1 */
class Matrix10F1;
/* Forward declare 10F10 */
class Matrix10F10;

class Matrix1F10
{
private:

	/** \brief Number of rows */
    static const int ROWS = 1;

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
    Matrix1F10();

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
	 **
	 *************************************************************************/
    Matrix1F10(Vector10F m0);

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
    ~Matrix1F10();

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
	/** \brief Multiplication Operator Override Matrix10F10
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object
	 **       and a #Matrix10F10.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **	 \param [in]   vecIn      A #Matrix10F10 to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication as #Matrix1F10
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix1F10 operator*(const Matrix10F10 &matIn);

	/************************************************************************/
	/** \brief Multiplication Operator Override Matrix10F1
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object
	 **       and a #Matrix10F1.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **	 \param [in]   vecIn      A #Matrix10F1 to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication as #Matrix1F1
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix1F1 operator*(const Matrix10F1 & matIn);

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
	 **  Result of the multiplication as #Vector1F
	 **  \endreturns
	 **
	 *************************************************************************/
    Vector1F operator*(const Vector10F &vecIn);

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
    Matrix10F1 Transpose(void);

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
