#include "LevyFlight.h"


std::valarray<double> LevyFlight::GetValue(double lambda, unsigned long int dimension)
{
	std::valarray<double> result(dimension);

	const double divider = std::tgamma((lambda + 1.0) / 2.0) * lambda *
		std::pow(2.0, (lambda - 1.0) / 2.0);
	const double sigma_x = std::pow(((std::tgamma(1.0 + lambda) * std::sin((M_PI * lambda) / 2.0)) / divider), 1.0 / lambda);
	const double sigma_y = 1.0;

	#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < dimension; ++i)
	{
		const double x = GetNormalDistribution(0.0, sigma_x);
		const double y = GetNormalDistribution(0.0, sigma_y);

		result[i] = x / std::pow(std::abs(y), 1.0 / lambda);
	}

	return result;
};

double LevyFlight::GetNormalDistribution(double mue, double sigma)
{
	static std::map<double, StochasticBuffer> variables;
	auto value = variables.find(sigma);
	if (value != variables.end())
	{
		if (!value->second.use_second)
		{
			auto stochastic_pair = BoxMullerTransform(mue, sigma);
			value->second.second_number = mue + sigma * stochastic_pair.second;
			value->second.use_second = true;

			return mue + sigma * stochastic_pair.first;
		}

		value->second.use_second = false;
		return value->second.second_number;
	}
	auto stochastic_pair = BoxMullerTransform(mue, sigma);
	variables.insert(std::pair<double, StochasticBuffer>(sigma, StochasticBuffer(stochastic_pair.second, true)));

	return stochastic_pair.first;
	
};

std::pair<double, double> LevyFlight::BoxMullerTransform(double mue, double sigma)
{
	double x;
	double y;
	double s;
	do {
		x = 2.0 * rand() / (RAND_MAX + 1) - 1.0;
		y = 2.0 * rand() / (RAND_MAX + 1) - 1.0;
		s = x * x + y * y;
	} while (s > 1.0);
	const double z0 = x * std::sqrt((-2.0 * std::log(s) / s));
	const double z1 = y * std::sqrt((-2.0 * std::log(s) / s));

	return std::pair<double, double>(mue + sigma * z0, mue + sigma * z1);
};