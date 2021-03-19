#ifndef MATRIX6F1_HPP
#define MATRIX6F1_HPP


#include "cfe.h"
#include "Vector6F.hpp"
#include "Vector1F.hpp"


namespace math
{

class Matrix6F1
{
private:
	/** \brief Number of rows and columns */
    static const int SIZE = 6;

    /** \brief Vector of NaN row data */
    Vector1F nan[SIZE];

    /** \brief Rotation lookup enumeration */
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
    Matrix6F1();

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
	 **
	 *************************************************************************/
    Matrix6F1(Vector1F m0, Vector1F m1, Vector1F m2, Vector1F m3, Vector1F m4, Vector1F m5);

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
    ~Matrix6F1();

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
