#ifndef CUCKOO
#define CUCKOO

#include "LevyFlight.h"
#include "FunctionHelper.h"

#include <valarray>
#include <exception>
#include <algorithm>

class Cuckoo
{
public:
	Cuckoo(ObjectiveFunction func, double lambda = 0.0, std::valarray<double> start_location = { 0.0 }, std::valarray<double> alpha = { 1.0 }) :
		m_function(func), m_lambda(lambda), m_solutions(start_location), m_alpha(alpha) {};

	double MakeFlight(double lambda, std::valarray<double> alpha);
	double MakeFlight(double lambda);
	double MakeFlight();

	double GetFitness() const { return m_fitness; };
	std::valarray<double> GetSolutions() const { return m_solutions; };

	void SetLambda(double lambda);
	void SetAlpha(std::valarray<double> alpha);

private:
	std::valarray<double>	m_solutions;
	ObjectiveFunction		m_function;
	double					m_fitness;
	double					m_lambda;
	std::valarray<double>	m_alpha;
};

#endif // !CUCKOO

