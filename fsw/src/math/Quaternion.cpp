#include <math/Quaternion.hpp>
#include <math.h>

using namespace math;

Quaternion::Quaternion(float a, float b, float c, float d) :
	Vector4F(a, b, c, d)
{

}


Quaternion::Quaternion(const Matrix3F3 &mat)
{
	float tr = mat[0][0] + mat[1][1] + mat[2][2];
	if (tr > 0.0f) {
		float s = sqrtf(tr + 1.0f);
		data[0] = s * 0.5f;
		s = 0.5f / s;
		data[1] = (mat[2][1] - mat[1][2]) * s;
		data[2] = (mat[0][2] - mat[2][0]) * s;
		data[3] = (mat[1][0] - mat[0][1]) * s;
	} else {
		/* Find maximum diagonal element in dcm
		* store index in dcm_i */
		int mat_i = 0;
		for (int i = 1; i < 3; i++) {
			if (mat[i][i] > mat[mat_i][mat_i]) {
				mat_i = i;
			}
		}
		int mat_j = (mat_i + 1) % 3;
		int mat_k = (mat_i + 2) % 3;
		float s = sqrtf((mat[mat_i][mat_i] - mat[mat_j][mat_j] -
				mat[mat_k][mat_k]) + 1.0f);
		data[mat_i + 1] = s * 0.5f;
		s = 0.5f / s;
		data[mat_j + 1] = (mat[mat_i][mat_j] + mat[mat_j][mat_i]) * s;
		data[mat_k + 1] = (mat[mat_k][mat_i] + mat[mat_i][mat_k]) * s;
		data[0] = (mat[mat_k][mat_j] - mat[mat_j][mat_k]) * s;
	}
}

Quaternion::Quaternion(const Vector3F &aa)
{
    /* NOTE: This is for an AxisAngle - Not a normal Vector3F */
    float angle = aa.Length();
    Vector3F axis(aa[0], aa[1], aa[2]);
    axis.Normalize();
    if (angle < 1e-10)
    {
        data[0] = 1.0;
        data[1] = data[2] = data[3] = 0;
    }
    else
    {
        float magnitude = sin(angle / 2.0f);
        data[0] = cos(angle / 2.0f);
        data[1] = axis[0] * magnitude;
        data[2] = axis[1] * magnitude;
        data[3] = axis[2] * magnitude;
    }
}


Quaternion::Quaternion()
{

}


Quaternion::~Quaternion()
{

}


/* Multiplication */
const Quaternion Quaternion::operator*(const Quaternion &q) const
{
    Quaternion result (
            data[0] * q[0] - data[1] * q[1] - data[2] * q[2] - data[3] * q[3],
            data[0] * q[1] + data[1] * q[0] + data[2] * q[3] - data[3] * q[2],
            data[0] * q[2] - data[1] * q[3] + data[2] * q[0] + data[3] * q[1],
            data[0] * q[3] + data[1] * q[2] - data[2] * q[1] + data[3] * q[0]
    );
    return result;
}


/* Division */
Quaternion Quaternion::operator/(const Quaternion &q) const
{
    float norm = q.LengthSquared();
    Quaternion result (
            (  data[0] * q[0] + data[1] * q[1] + data[2] * q[2] + data[3] * q[3]) / norm,
            (- data[0] * q[1] + data[1] * q[0] - data[2] * q[3] + data[3] * q[2]) / norm,
            (- data[0] * q[2] + data[1] * q[3] + data[2] * q[0] - data[3] * q[1]) / norm,
            (- data[0] * q[3] - data[1] * q[2] + data[2] * q[1] + data[3] * q[0]) / norm
    );
    return result;
}

/* Derivative */
const Quaternion Quaternion::Derivative(const Vector3F &w)
{
    Vector4F row1(data[0], -data[1], -data[2], -data[3]);
    Vector4F row2(data[1],  data[0], -data[3],  data[2]);
    Vector4F row3(data[2],  data[3],  data[0], -data[1]);
    Vector4F row4(data[3], -data[2],  data[1],  data[0]);
    /* TODO double check that these are rows and not columns */
    Matrix4F4 Q(row1, row2, row3, row4);
    Vector4F v(0.0f, w[0], w[1], w[2]);

    Vector4F result_v(Q * v * 0.5f);
    Quaternion result_q(result_v[0], result_v[1], result_v[2], result_v[3]);

    return result_q;
}



// overload + operator to provide a vector addition
const Quaternion Quaternion::operator+(const Quaternion &vecIn) const
{
    Quaternion vecOut;
    vecOut[0] = data[0] + vecIn[0];
    vecOut[1] = data[1] + vecIn[1];
    vecOut[2] = data[2] + vecIn[2];
    vecOut[3] = data[3] + vecIn[3];
    return vecOut;
}



// overload * operator to provide a vector scalar product
const Quaternion Quaternion::operator*(const float scalar) const
{
    Quaternion vecOut;
    vecOut[0] = data[0] * scalar;
    vecOut[1] = data[1] * scalar;
    vecOut[2] = data[2] * scalar;
    vecOut[3] = data[3] * scalar;
    return vecOut;
}


// overload - operator to provide a vector subtraction
const Quaternion Quaternion::operator-(const Quaternion &vecIn) const
{
    Quaternion vecOut;
    vecOut[0] = data[0] - vecIn[0];
    vecOut[1] = data[1] - vecIn[1];
    vecOut[2] = data[2] - vecIn[2];
    vecOut[3] = data[3] - vecIn[3];
    return vecOut;
}


const Quaternion& Quaternion::operator =(const Quaternion &q)
{

    data[0] = q[0];
    data[1] = q[1];
    data[2] = q[2];
    data[3] = q[3];

    return *this;
}


/* Conjugate */
Vector3F Quaternion::Conjugate(const Vector3F &v) const
{
    float q0q0 = data[0] * data[0];
    float q1q1 = data[1] * data[1];
    float q2q2 = data[2] * data[2];
    float q3q3 = data[3] * data[3];
    
    Vector3F result(v[0] * (
            q0q0 + q1q1 - q2q2 - q3q3) +
            v[1] * 2.0f * (data[1] * data[2] - data[0] * data[3]) +
            v[2] * 2.0f * (data[0] * data[2] + data[1] * data[3]),

            v[0] * 2.0f * (data[1] * data[2] + data[0] * data[3]) +
            v[1] * (q0q0 - q1q1 + q2q2 - q3q3) +
            v[2] * 2.0f * (data[2] * data[3] - data[0] * data[1]),

            v[0] * 2.0f * (data[1] * data[3] - data[0] * data[2]) +
            v[1] * 2.0f * (data[0] * data[1] + data[2] * data[3]) +
            v[2] * (q0q0 - q1q1 - q2q2 + q3q3)
    );

    return result;
}


/* Conjugate with inversed quaternion */
Vector3F Quaternion::ConjugateInversed(const Vector3F &v) const
{
    float q0q0 = data[0] * data[0];
    float q1q1 = data[1] * data[1];
    float q2q2 = data[2] * data[2];
    float q3q3 = data[3] * data[3];
    
    Vector3F result(v[0] * (
            q0q0 + q1q1 - q2q2 - q3q3) +
            v[1] * 2.0f * (data[1] * data[2] + data[0] * data[3]) +
            v[2] * 2.0f * (data[1] * data[3] - data[0] * data[2]),

            v[0] * 2.0f * (data[1] * data[2] - data[0] * data[3]) +
            v[1] * (q0q0 - q1q1 + q2q2 - q3q3) +
            v[2] * 2.0f * (data[2] * data[3] + data[0] * data[1]),

            v[0] * 2.0f * (data[1] * data[3] + data[0] * data[2]) +
            v[1] * 2.0f * (data[2] * data[3] - data[0] * data[1]) +
            v[2] * (q0q0 - q1q1 - q2q2 + q3q3)
    );

    return result;
}


Matrix3F3 Quaternion::RotationMatrix(void) const
{
	Matrix3F3 R;
	float aSq = data[0] * data[0];
	float bSq = data[1] * data[1];
	float cSq = data[2] * data[2];
	float dSq = data[3] * data[3];
	R[0][0] = aSq + bSq - cSq - dSq;
	R[0][1] = 2.0f * (data[1] * data[2] - data[0] * data[3]);
	R[0][2] = 2.0f * (data[0] * data[2] + data[1] * data[3]);
	R[1][0] = 2.0f * (data[1] * data[2] + data[0] * data[3]);
	R[1][1] = aSq - bSq + cSq - dSq;
	R[1][2] = 2.0f * (data[2] * data[3] - data[0] * data[1]);
	R[2][0] = 2.0f * (data[1] * data[3] - data[0] * data[2]);
	R[2][1] = 2.0f * (data[0] * data[1] + data[2] * data[3]);
	R[2][2] = aSq - bSq - cSq + dSq;
	return R;
}


Vector3F Quaternion::Imaginary(void)
{
    return Vector3F(data[1], data[2], data[3]);
}



Vector3F Quaternion::ToEuler(void) const
{
	Vector3F euler(
		atan2f(2.0f * (data[0] * data[1] + data[2] * data[3]), 1.0f - 2.0f * (data[1] * data[1] + data[2] * data[2])),
		asinf(2.0f * (data[0] * data[2] - data[3] * data[1])),
		atan2f(2.0f * (data[0] * data[3] + data[1] * data[2]), 1.0f - 2.0f * (data[2] * data[2] + data[3] * data[3]))
	);

	return euler;
}


/**
 * set quaternion to rotation by DCM
 * Reference: Shoemake, Quaternions, http://www.cs.ucr.edu/~vbz/resources/quatut.pdf
 */
void Quaternion::FromDCM(const Matrix3F3 &dcm)
{
    float tr = dcm[0][0] + dcm[1][1] + dcm[2][2];

    if (tr > 0.0f) 
    {
        float s = sqrtf(tr + 1.0f);
        data[0] = (s * 0.5f);
        s = 0.5f / s;
        data[1] = (dcm[2][1] - dcm[1][2]) * s;
        data[2] = (dcm[0][2] - dcm[2][0]) * s;
        data[3] = (dcm[1][0] - dcm[0][1]) * s;

    } else {
        /* Find maximum diagonal element in dcm
        * store index in dcm_i */
        int dcm_i = 0;

        for (int i = 1; i < 3; i++) 
        {
            if (dcm[i][i] > dcm[dcm_i][dcm_i]) 
            {
                dcm_i = i;
            }
        }

        int dcm_j = (dcm_i + 1) % 3;
        int dcm_k = (dcm_i + 2) % 3;
        float s = sqrtf((dcm[dcm_i][dcm_i] - dcm[dcm_j][dcm_j] -
                 dcm[dcm_k][dcm_k]) + 1.0f);
        data[dcm_i + 1] = s * 0.5f;
        s = 0.5f / s;
        data[dcm_j + 1] = (dcm[dcm_i][dcm_j] + dcm[dcm_j][dcm_i]) * s;
        data[dcm_k + 1] = (dcm[dcm_k][dcm_i] + dcm[dcm_i][dcm_k]) * s;
        data[0] = (dcm[dcm_k][dcm_j] - dcm[dcm_j][dcm_k]) * s;
    }
}


/* create quaternion representing rotation only by yaw */
void Quaternion::FromYaw(float yaw)
{
    data[0] = cosf(yaw / 2.0f);
    data[1] = 0.0f;
    data[2] = 0.0f;
    data[3] = sinf(yaw / 2.0f);
}


Quaternion::Quaternion(float roll, float pitch, float yaw)
{
	double cosPhi_2 = cos(double(roll) / 2.0);
	double sinPhi_2 = sin(double(roll) / 2.0);
	double cosTheta_2 = cos(double(pitch) / 2.0);
	double sinTheta_2 = sin(double(pitch) / 2.0);
	double cosPsi_2 = cos(double(yaw) / 2.0);
	double sinPsi_2 = sin(double(yaw) / 2.0);

	/* operations executed in double to avoid loss of precision through
	 * consecutive multiplications. Result stored as float.
	 */
	data[0] = static_cast<float>(cosPhi_2 * cosTheta_2 * cosPsi_2 + sinPhi_2 * sinTheta_2 * sinPsi_2);
	data[1] = static_cast<float>(sinPhi_2 * cosTheta_2 * cosPsi_2 - cosPhi_2 * sinTheta_2 * sinPsi_2);
	data[2] = static_cast<float>(cosPhi_2 * sinTheta_2 * cosPsi_2 + sinPhi_2 * cosTheta_2 * sinPsi_2);
	data[3] = static_cast<float>(cosPhi_2 * cosTheta_2 * sinPsi_2 - sinPhi_2 * sinTheta_2 * cosPsi_2);
}



void Quaternion::copyTo(float (&dst)[4])
{
    dst[0] = data[0];
    dst[1] = data[1];
    dst[2] = data[2];
    dst[3] = data[3];
}
