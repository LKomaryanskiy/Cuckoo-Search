#ifndef CUCKOO_SEARCH
#define CUCKOO_SEARCH

#include <functional>
#include <vector>
#include <cstdarg>

#include "FunctionHelper.h"
#include "LevyFlight.h"
#include "Cuckoo.h"
#include "Nest.h"

using SetOfNests = std::vector<Nest>;

class CuckooSearch
{
public:
	CuckooSearch(ObjectiveFunction obj_func, StopCritearian stop_criterian, unsigned int amount_of_nests = 32, unsigned int max_generations = 10000);

	std::valarray<double> GetSolution();

	void SetNumberOfNests(unsigned int nests);

private:
	unsigned int			m_amount_of_nests;
	SetOfNests				m_nests;
	ObjectiveFunction		m_objective_function;
	StopCritearian			m_stop_criterian;
	unsigned int			m_max_generations;
	double					m_alpha;
	double					m_probability;

	void GenerateInitialPopulation();
	double BuildNewNest();
	void AbandonNests();
	void RankNests();
};


#endif // !CUCKOO_SEARCH

