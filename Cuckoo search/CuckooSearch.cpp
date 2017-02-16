#include "CuckooSearch.h"

CuckooSearch::CuckooSearch(ObjectiveFunction obj_func, StopCritearian stop_criterian, unsigned int amount_of_nests, unsigned int max_generations):
	m_objective_function(obj_func), m_stop_criterian(stop_criterian), m_amount_of_nests(amount_of_nests), m_max_generations(max_generations)
{
	m_nests = std::vector<Nest>(m_amount_of_nests, obj_func);
	m_alpha = 1.0;
};


std::valarray<double> CuckooSearch::GetSolution()
{
	GenerateInitialPopulation();
	
	unsigned int i = 0;
	while ((i < m_max_generations) || m_stop_criterian())
	{

	}

	return m_nests[0].GetSolutions();
};

void CuckooSearch::SetNumberOfNests(unsigned int nests)
{
};

void CuckooSearch::GenerateInitialPopulation()
{
};

double CuckooSearch::BuildNewNest()
{
	return 0.0;
};

void CuckooSearch::AbandonNests()
{
};

void CuckooSearch::RankNests()
{
};

