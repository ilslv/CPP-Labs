#pragma once
#include <cmath>

inline double f1(const double x)
{
	return exp(x);
}

inline double f2(const double x)
{
	return x * exp(-1. * x);
}

inline double f3(const double x)
{
	return x / pow((3 * x * x - 1), 4);
}

inline double f4(const double x)
{
	return pow(x, 1. / 3.) - sqrt(x);
}
