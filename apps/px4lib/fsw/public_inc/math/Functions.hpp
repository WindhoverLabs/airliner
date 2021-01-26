namespace math
{
    inline const float gradual(const float &value, const float &x_low, const float &x_high, const float &y_low, const float &y_high)
    {
	    if (value < x_low) {
		    return y_low;

	    } else if (value > x_high) {
		    return y_high;

	    } else {
		    /* linear function between the two points */
		    float a = (y_high - y_low) / (x_high - x_low);
		    float b = y_low - a * x_low;
		    return  a * value + b;
	    }
    }
}
