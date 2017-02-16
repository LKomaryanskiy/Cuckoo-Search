#include "Nest.h"

Nest::Nest(ObjectiveFunction function, std::valarray<double> host_nest, double abandon_probability):
	m_function(function), m_solution(host_nest), m_abandon_probability(abandon_probability)
{
	m_fitness = m_function(host_nest);
};

Nest& Nest::operator=(const Cuckoo& new_solution)
{
	m_fitness = new_solution.GetFitness();
	m_solution = new_solution.GetSolutions();

	return *this;
};

void Nest::SetAbandonProbability(double prob)
{
	if ((prob < 0.0) || (prob > 1.0))
		throw std::exception("Abandon probability must be in range [0, 1]\n");
	m_abandon_probability = prob;
};
