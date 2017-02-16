#ifndef NEST
#define NEST

#include "Cuckoo.h"
#include "FunctionHelper.h"

#include <valarray>
#include <exception>

class Nest
{
public:
	Nest(ObjectiveFunction function, std::valarray<double> host_nest = { 0.0 }, double abandon_probability = 0.25);

	Nest& operator=(const Cuckoo& new_solution);

	double GetAbandonProbability() { return m_abandon_probability; };
	double GetFitness() { return m_fitness; };
	std::valarray<double> GetSolutions() { return m_solution; };

	void SetAbandonProbability(double prob);

private:
	ObjectiveFunction		m_function;
	std::valarray<double>	m_solution;
	double					m_fitness;
	double					m_abandon_probability;
};

#endif // !NEST
