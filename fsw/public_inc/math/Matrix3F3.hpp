#ifndef MATRIX3F3_HPP
#define MATRIX3F3_HPP

#include "cfe.h"
#include "Vector3F.hpp"

#define M_DEG_TO_RAD_F		(0.01745329251994329576f)

namespace math
{

class Matrix3F3
{
public:
	/** \brief Rotation options enumeration */
	typedef enum {
		ROTATION_NONE                = 0,
		ROTATION_YAW_45              = 1,
		ROTATION_YAW_90              = 2,
		ROTATION_YAW_135             = 3,
		ROTATION_YAW_180             = 4,
		ROTATION_YAW_225             = 5,
		ROTATION_YAW_270             = 6,
		ROTATION_YAW_315             = 7,
		ROTATION_ROLL_180            = 8,
		ROTATION_ROLL_180_YAW_45     = 9,
		ROTATION_ROLL_180_YAW_90     = 10,
		ROTATION_ROLL_180_YAW_135    = 11,
		ROTATION_PITCH_180           = 12,
		ROTATION_ROLL_180_YAW_225    = 13,
		ROTATION_ROLL_180_YAW_270    = 14,
		ROTATION_ROLL_180_YAW_315    = 15,
		ROTATION_ROLL_90             = 16,
		ROTATION_ROLL_90_YAW_45      = 17,
		ROTATION_ROLL_90_YAW_90      = 18,
		ROTATION_ROLL_90_YAW_135     = 19,
		ROTATION_ROLL_270            = 20,
		ROTATION_ROLL_270_YAW_45     = 21,
		ROTATION_ROLL_270_YAW_90     = 22,
		ROTATION_ROLL_270_YAW_135    = 23,
		ROTATION_PITCH_90            = 24,
		ROTATION_PITCH_270           = 25,
		ROTATION_ROLL_270_YAW_270    = 26,
		ROTATION_ROLL_180_PITCH_270  = 27,
		ROTATION_PITCH_90_YAW_180    = 28,
		ROTATION_PITCH_90_ROLL_90	 = 29,
		ROTATION_YAW_293_PITCH_68_ROLL_90 = 30,
		ROTATION_PITCH_90_ROLL_270	 = 31,
		ROTATION_PITCH_9_YAW_180 = 32,
		ROTATION_MAX
	} Rotation_t;

	/** \brief Rotation lookup struct */
	typedef struct {
		uint16 roll;
		uint16 pitch;
		uint16 yaw;
	} RotLookup_t;

private:

	/** \brief Number of rows and columns */
	static const int SIZE = 3;

	/** \brief Vector of NaN row data */
	Vector3F nan;

	/** \brief Rotation lookup enumeration */
	static const RotLookup_t RotLookup[];

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
	Matrix3F3();

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
	 **  \param [in]   m1      A #Vector3F specifying a row
	 **  \param [in]   m2      A #Vector3F specifying a row
	 **
	 *************************************************************************/
    Matrix3F3(Vector3F m0, Vector3F m1, Vector3F m2);

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
    ~Matrix3F3();

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
	 **  Row of the matrix
	 **  \endreturns
	 **
	 *************************************************************************/
    Vector3F operator [] (uint32 i) const;

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
    Matrix3F3 Transpose(void);

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
    static Matrix3F3 Identity(void);

	/************************************************************************/
	/** \brief Multiplication Operator Override Vector3F
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object
	 **       and a #Vector3F.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **	 \param [in]   vecIn      A #Vector3F to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication as #Vector3F
	 **  \endreturns
	 **
	 *************************************************************************/
    Vector3F operator*(const Vector3F &vecIn);

	/************************************************************************/
	/** \brief Multiplication Operator Override
	 **
	 **  \par Description
	 **       This function overrides the multiplication behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   matIn      A #Matrix3F3 to perform this operation on
	 **
	 **  \returns
	 **  Result of the multiplication
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix3F3 operator*(const Matrix3F3 &matIn);

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
    Matrix3F3 operator*(const float &scalar);

	/************************************************************************/
	/** \brief Addition Operator Override
	 **
	 **  \par Description
	 **       This function overrides the addition behavior when used on this object.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   matIn      A #Matrix3F3 to perform this operation on
	 **
	 **  \returns
	 **  Result of the addition
	 **  \endreturns
	 **
	 *************************************************************************/
    Matrix3F3 operator+(const Matrix3F3 &matIn) const;

	/************************************************************************/
	/** \brief Rotation Matrix
	 **
	 **  \par Description
	 **       This function returns yaw, pitch, and roll values for the
	 **       specific rotation orientation of the vehicle.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   matIn      A #Matrix3F3 to perform this operation on
	 **
	 **  \returns
	 **  An Euler matrix with roll, pitch, and yaw values.
	 **  \endreturns
	 **
	 *************************************************************************/
    static Matrix3F3 RotationMatrix(Matrix3F3::Rotation_t boardRotation);

	/************************************************************************/
	/** \brief Generate Matrix From Euler Values
	 **
	 **  \par Description
	 **       This function returns a rotation matrix based on passed
	 **       Euler values.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \param [in]   roll       A #float specifying roll
	 **  \param [in]   pitch      A #float specifying pitch
	 **  \param [in]   yaw        A #float specifying yaw
	 **
	 **  \returns
	 **  Identity matrix
	 **  \endreturns
	 **
	 *************************************************************************/
    static Matrix3F3 FromEuler(float roll, float pitch, float yaw);

	/************************************************************************/
	/** \brief Convert To Euler
	 **
	 **  \par Description
	 **       This function computes Euler values of this matrix.
	 **
	 **  \par Assumptions, External Events, and Notes:
	 **       None
	 **
	 **  \returns
	 **  #Vector3F of Euler values
	 **  \endreturns
	 **
	 *************************************************************************/
    Vector3F ToEuler(void) const;

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
    Matrix3F3 Inversed(void);

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
	Vector3F data[SIZE];

};

}

#endif
