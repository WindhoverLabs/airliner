#ifndef MATRIX4F4_HPP
#define MATRIX4F4_HPP

#include "cfe.h"
#include "Matrix4F4.hpp"
#include "Vector4F.hpp"

namespace math
{

class Matrix4F4
{
private:
	/** \brief Number of rows and columns */
	static const int SIZE = 4;

	/** \brief Vector of NaN row data */
	Vector4F nan;

	/** \brief Rotation lookup enumeration */
	Vector4F data[SIZE];

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
	Matrix4F4();

	/************************************************************************/
	/** \brief Matrix Constructor
	 **
	 **  \par Description
	 **       This function initializes the object with passed values.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   m0      A #Vector4F specifying a row
	 **  \param [in]   m1      A #Vector4F specifying a row
	 **  \param [in]   m2      A #Vector4F specifying a row
	 **  \param [in]   m3      A #Vector4F specifying a row
	 **
	 *************************************************************************/
	Matrix4F4(Vector4F m0, Vector4F m1, Vector4F m2, Vector4F m3);

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
	~Matrix4F4();

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
	Vector4F& operator [] (uint32 i);

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
	Vector4F operator [] (uint32 i) const;

	/************************************************************************/
	/** \brief Multiplication Operator Override
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object
	 **       and a #Vector4F.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **	 \param [in]   vecIn      A #Vector4F to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication as #Vector4F
	 **  \endreturns
	 **
	 *************************************************************************/
    Vector4F operator*(const Vector4F &v) const;


protected:

};

}

#endif
