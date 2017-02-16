#include "Cuckoo.h"

double Cuckoo::MakeFlight(double lambda, std::valarray<double> alpha)
{
	SetLambda(lambda);
	SetAlpha(alpha);

	return MakeFlight();
};

double Cuckoo::MakeFlight(double lambda)
{
	SetLambda(lambda);

	return MakeFlight();
};

double Cuckoo::MakeFlight()
{
	m_solutions += m_alpha * LevyFlight::GetValue(m_lambda, m_solutions.size());
	m_fitness = m_function(m_solutions);
	return m_fitness;
};

void Cuckoo::SetLambda(double lambda)
{
	if ((lambda < 0.0) || (lambda > 3.0))
		throw std::exception("Lambda must be in range [0, 3]\n");
	m_lambda = lambda;
};

void Cuckoo::SetAlpha(std::valarray<double> alpha)
{
	if (std::find_if(std::begin(alpha), std::end(alpha), [](double elem) {return (elem > 0.0); }) != std::end(alpha))
		throw std::exception("Alpha must be more than 0\n");
	m_alpha = alpha;
};