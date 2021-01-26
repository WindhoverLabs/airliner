#include <math/Dcm.hpp>
#include <math.h>

using namespace math;


Dcm::Dcm() :
	Matrix3F3()
{
};

Dcm::Dcm(Quaternion q)
{
    float a = q[0];
    float b = q[1];
    float c = q[2];
    float d = q[3];
    float aSq = a * a;
    float bSq = b * b;
    float cSq = c * c;
    float dSq = d * d;
    data[0][0] = aSq + bSq - cSq - dSq;
    data[0][1] = 2 * (b * c - a * d);
    data[0][2] = 2 * (a * c + b * d);
    data[1][0] = 2 * (b * c + a * d);
    data[1][1] = aSq - bSq + cSq - dSq;
    data[1][2] = 2 * (c * d - a * b);
    data[2][0] = 2 * (b * d - a * c);
    data[2][1] = 2 * (a * b + c * d);
    data[2][2] = aSq - bSq - cSq + dSq;
}

Dcm::~Dcm()
{
};



