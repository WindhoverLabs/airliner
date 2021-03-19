#ifndef MATRIX10F6_HPP
#define MATRIX10F6_HPP

#include "cfe.h"
#include "Vector6F.hpp"
#include "Matrix6F6.hpp"
#include "Matrix6F10.hpp"
#include "Matrix10F10.hpp"
#include "Vector10F.hpp"


namespace math
{
    
class Matrix6F10;

class Matrix10F6
{
public:

private:
	/** \brief Number of rows */
	static const int ROWS = 10;

	/** \brief Number of columns */
	static const int COLS = 6;

	/** \brief Vectors of NaN row data */
	Vector6F nan;

	/** \brief Vectors of row data */
	Vector6F data[ROWS];

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
	Matrix10F6();

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
	 **  \param [in]   m6      A #Vector6F specifying a row
	 **  \param [in]   m7      A #Vector6F specifying a row
	 **  \param [in]   m8      A #Vector6F specifying a row
	 **  \param [in]   m9      A #Vector6F specifying a row
	 **
	 *************************************************************************/
	Matrix10F6(Vector6F m0, Vector6F m1, Vector6F m2, Vector6F m3, Vector6F m4, Vector6F m5, Vector6F m6, Vector6F m7, Vector6F m8, Vector6F m9);

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
	~Matrix10F6();

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
	 **  Row of the matrix as #Vector6F
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
	 **  \param [in]   i      A #uint32 specifying a row
	 **
	 **  \returns
	 **  Row of the matrix as #Vector6F
	 **  \endreturns
	 **
	 *************************************************************************/
	Vector6F operator [] (uint32 i) const;

	/************************************************************************/
	/** \brief Multiplication Operator Override Matrix6F6
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   matIn      A #Matrix6F6 to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication as #Matrix10F6
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix10F6 operator*(const Matrix6F6 &matIn);

	/************************************************************************/
	/** \brief Multiplication Operator Override Vector6F
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
	 **  Result of the multiplication as #Vector10F
	 **  \endreturns
	 **
	 *************************************************************************/
    Vector10F operator*(const Vector6F &vecIn);

	/************************************************************************/
	/** \brief Multiplication Operator Override Matrix6F10
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   matIn      A #Matrix6F10 to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication as #Matrix10F10
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix10F10 operator*(const Matrix6F10 &matIn);


	void Zero(void);

protected:

};

}

#endif
