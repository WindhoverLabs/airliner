#ifndef MATRIX10F1_HPP
#define MATRIX10F1_HPP


#include "cfe.h"
#include "Vector10F.hpp"
#include "Vector1F.hpp"
#include "Matrix1F10.hpp"
#include "Matrix1F1.hpp"
#include "Matrix10F10.hpp"

namespace math
{

/* Forward declare 1F10 */
class Matrix1F10;
/* Forward declare 10F10 */
class Matrix10F10;

class Matrix10F1
{
private:
	/** \brief Number of rows */
    static const int SIZE = 10;

    /** \brief Vectors of NaN row data */
    Vector1F nan[SIZE];

    /** \brief Vectors of row data */
    Vector1F data[SIZE];

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
    Matrix10F1();

	/************************************************************************/
	/** \brief Matrix Constructor
	 **
	 **  \par Description
	 **       This function initializes the object with passed values.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   m0      A #Vector1F specifying a row
	 **  \param [in]   m1      A #Vector1F specifying a row
	 **  \param [in]   m2      A #Vector1F specifying a row
	 **  \param [in]   m3      A #Vector1F specifying a row
	 **  \param [in]   m4      A #Vector1F specifying a row
	 **  \param [in]   m5      A #Vector1F specifying a row
	 **  \param [in]   m6      A #Vector1F specifying a row
	 **  \param [in]   m7      A #Vector1F specifying a row
	 **  \param [in]   m8      A #Vector1F specifying a row
	 **  \param [in]   m9      A #Vector1F specifying a row
	 **
	 *************************************************************************/
    Matrix10F1(Vector1F m0, Vector1F m1, Vector1F m2, Vector1F m3, Vector1F m4,
               Vector1F m5, Vector1F m6, Vector1F m7, Vector1F m8, Vector1F m9);

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
    ~Matrix10F1();

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
    Vector1F& operator [] (uint32 i);

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
    Vector1F operator [] (uint32 i) const;

	/************************************************************************/
	/** \brief Multiplication Operator Override Matrix1F1
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   matIn      A #Matrix1F1 to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication as #Matrix10F1
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix10F1 operator*(const Matrix1F1 & matIn);

	/************************************************************************/
	/** \brief Multiplication Operator Override Matrix1F10
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   matIn      A #Matrix1F10 to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication as #Matrix10F10
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix10F10 operator*(const Matrix1F10 & matIn);

	/************************************************************************/
	/** \brief Multiplication Operator Override Vector1F
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   vecIn      A #Vector1F to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication as #Matrix10F1
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix10F1 operator*(const Vector1F & vecIn);

	/************************************************************************/
	/** \brief Convert To Vector
	 **
	 **  \par Description
	 **       This function converts returns this object as a #Vector10F pbject
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \returns
	 **  A #Vector10F representation of the matrix
	 **  \endreturns
	 **
	 *************************************************************************/
    Vector10F ToVector(void);

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
    Matrix1F10 Transpose(void);

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
