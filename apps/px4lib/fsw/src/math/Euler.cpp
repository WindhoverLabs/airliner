#include <math/Euler.hpp>
#include <math.h>

using namespace math;

Euler::Euler(math::Matrix3F3 dcm)
{
    float phi_val = float(atan2(dcm[2][1], dcm[2][2]));
    float theta_val = float(asin(-dcm[2][0]));
    float psi_val = float(atan2(dcm[1][0], dcm[0][0]));
    float pi = float(M_PI);

    if (float(fabs(theta_val - pi / float(2))) < float(1.0e-3)) {
        phi_val = float(0.0);
        psi_val = float(atan2(dcm[1][2], dcm[0][2]));

    } else if (float(fabs(theta_val + pi / float(2))) < float(1.0e-3)) {
        phi_val = float(0.0);
        psi_val = float(atan2(-dcm[1][2], -dcm[0][2]));
    }

    data[0] = phi_val;
    data[1] = theta_val;
    data[2] = psi_val;
};


Euler::Euler() :
	Vector3F()
{
};

Euler::Euler(Quaternion q)
{
    *this = Euler(Dcm(q));
}

Euler::Euler(float phi_, float theta_, float psi_)
{
    data[0] = phi_;
    data[1] = theta_;
    data[2] = psi_;
}

Euler::~Euler()
{
};





