#define _USE_MATH_DEFINES

#include "cumulative_distribution_function.h"
#include <cmath>




const double cumulative_normal_distribution_function(const double& y)						// Cumulative distribution function of the standard normal distibution

{
	return std::erfc(-y / std::sqrt(2)) / 2;
}

const double probability_normal_density_function(const double& y)							// Probability density function of the standard normal distibution

{
	return (1.0 / (pow(2 * M_PI, 0.5)))*exp(-0.5*y*y);
}