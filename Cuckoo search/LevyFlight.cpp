#include "LevyFlight.h"


std::valarray<double> LevyFlight::GetValue(double lambda, unsigned long int dimension)
{
	std::valarray<double> result(dimension);

	for (double& elem : result)
	{
		elem = MantegnaRandom(lambda);
	}

	return result;
};

double LevyFlight::MantegnaRandom(double lambda)
{

	const double divider = std::tgamma((lambda + 1.0) / 2.0) * lambda *
		std::pow(2.0, (lambda - 1.0) / 2.0);
	const double sigma_x = std::pow(std::tgamma(1.0 + lambda) * std::sin(M_PI / (2.0 * lambda)) / divider, 1.0 / lambda);
	const double sigma_y = 1.0;

	const double x = GetNormalDistribution(0.0, sigma_x);
	const double y = GetNormalDistribution(0.0, sigma_y);

	return x / std::pow(y, 1.0 / lambda);
};

double LevyFlight::GetNormalDistribution(double mue, double sigma)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::lognormal_distribution<double> normal_dis(mue, sigma);
	return normal_dis(gen);
};