#ifndef LEVY_FLIGHT
#define LEVY_FLIGHT

#include <valarray>
#include <random>

#define _USE_MATH_DEFINES
#include <math.h>


class LevyFlight
{
public:
	static std::valarray<double> GetValue(double lambda, unsigned long int dimension = 1);
protected:
	static double MantegnaRandom(double lambda);
	static double GetNormalDistribution(double mue, double sigma);
private:

	LevyFlight() = delete;
	LevyFlight(LevyFlight&) = delete;
	LevyFlight& operator=(LevyFlight&) = delete;
};

#endif // !LEVY_FLIGHT

