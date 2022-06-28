#include "ECL_Controller.hpp"


ECL_Controller::ECL_Controller(const char *name)
{
	init();
}


void ECL_Controller::init(void)
{
	_last_run = 0;
	_tc = 0.1f;
	_k_p = 0.0f;
	_k_i = 0.0f;
	_k_ff = 0.0f;
	_integrator_max = 0.0f;
	_max_rate = 0.0f;
	_last_output = 0.0f;
	_integrator = 0.0f;
	_rate_error = 0.0f;
	_rate_setpoint = 0.0f;
	_bodyrate_setpoint = 0.0f;
}

void ECL_Controller::reset_integrator()
{
	_integrator = 0.0f;
}

void ECL_Controller::set_time_constant(float time_constant)
{
	if (time_constant > 0.1f && time_constant < 3.0f) {
		_tc = time_constant;
	}
}

void ECL_Controller::set_k_p(float k_p)
{
	_k_p = k_p;
}

void ECL_Controller::set_k_i(float k_i)
{
	_k_i = k_i;
}

void ECL_Controller::set_k_ff(float k_ff)
{
	_k_ff = k_ff;
}

void ECL_Controller::set_integrator_max(float max)
{
	_integrator_max = max;
}

void ECL_Controller::set_max_rate(float max_rate)
{
	_max_rate = max_rate;
}

float ECL_Controller::get_rate_error()
{
	return _rate_error;
}

float ECL_Controller::get_desired_rate()
{
	return _rate_setpoint;
}

float ECL_Controller::get_desired_bodyrate()
{
	return _bodyrate_setpoint;
}

float ECL_Controller::constrain_airspeed(float airspeed, float minspeed, float maxspeed)
{
	float airspeed_result = airspeed;

	if (!isfinite(airspeed)) {
		/* airspeed is NaN, +- INF or not available, pick center of band */
		airspeed_result = 0.5f * (minspeed + maxspeed);

	} else if (airspeed < minspeed) {
		airspeed_result = minspeed;
	}

	return airspeed_result;
}
