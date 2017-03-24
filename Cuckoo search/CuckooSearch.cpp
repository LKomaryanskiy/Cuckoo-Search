#include "CuckooSearch.h"


CuckooSearch::CuckooSearch(ObjectiveFunction func, unsigned amount_of_nests, Step step, Lambda lambda, double prob,
	unsigned max_generations, StopCritearian stop_crierian, bool use_lazy_cuckoo) :
	m_objective_function(func), m_amount_of_nests(amount_of_nests), m_step(step), m_lambda(lambda), m_abandon_probability(prob),
	m_max_generations(max_generations), m_stop_criterian(stop_crierian), m_use_lazy_cuckoo(use_lazy_cuckoo)
{
	if (m_use_lazy_cuckoo)
	{
		m_cuckoo = new LazyCuckoo(m_objective_function);
	}
	else
	{
		m_cuckoo = new Cuckoo(m_objective_function);
	}
	if (m_step.GetMinStep().size() == 1 || m_step.GetMaxStep().size() == 1)
	{
		m_step.SetMinStep(std::valarray<double>(m_step.GetMinStep()[0], func.GetNumberOfDimensions()));
		m_step.SetMaxStep(std::valarray<double>(m_step.GetMaxStep()[0], func.GetNumberOfDimensions()));
	}
};

CuckooSearch::~CuckooSearch()
{
	//ERROR: Can't delete pointer
	//delete m_cuckoo;
};

std::valarray<double> CuckooSearch::FindMax()
{
	m_cmp_fitness = [](const Nest& ls, const Nest& rs) {return (ls > rs); };

	return GetSolution();
};

std::valarray<double> CuckooSearch::FindMin()
{
	m_cmp_fitness = [](const Nest& ls, const Nest& rs) {return (ls < rs); };

	return GetSolution();
};

std::valarray<double> CuckooSearch::FindMax(Lambda lambda, Step step, double prob, StopCritearian stop_criterian)
{
	SetLamda(lambda);
	SetStep(step);
	SetAbandonProbability(prob);
	m_stop_criterian = stop_criterian;
	return FindMax();
};

std::valarray<double> CuckooSearch::FindMin(Lambda lambda, Step step, double prob, StopCritearian stop_criterian)
{
	SetLamda(lambda);
	SetStep(step);
	SetAbandonProbability(prob);
	m_stop_criterian = stop_criterian;
	return FindMin();
};

void CuckooSearch::UseLazyCuckoo()
{
	delete m_cuckoo;
	m_cuckoo = new LazyCuckoo(m_objective_function);
	m_use_lazy_cuckoo = true;
};

void CuckooSearch::UseStandartCuckoo()
{
	delete m_cuckoo;
	m_cuckoo = new Cuckoo(m_objective_function);
	m_use_lazy_cuckoo = false;
};

std::valarray<double> CuckooSearch::GetSolution()
{
	m_delta_step = std::pow((m_step.GetMinStep() / m_step.GetMaxStep()), 1.0 / double(m_max_generations));
	GenerateInitialPopulation();
	m_best_ever = m_nests[0];
	
	m_current_generation = 1;
	#pragma omp parallel 
	{
		while ((m_current_generation <= m_max_generations) && m_stop_criterian())
		{
			#pragma omp single
			{
				srand(time(nullptr));
				m_statistics_handler();
			}
			#pragma omp sections
			{
				#pragma omp section
				RecalculateLambdas();
				#pragma omp section
				RecalculateStep();
			}

			#pragma omp for
			for (int i = 0; i < m_amount_of_nests; ++i)
			{
				Nest new_solution = m_cuckoo->MakeFlight(m_nests[i]);
				unsigned int random_index = rand() % m_amount_of_nests;
				if (m_cmp_fitness(new_solution, m_nests[random_index]))
				{
					#pragma omp critical
					m_nests[random_index] = new_solution;

					if (m_cmp_fitness(m_nests[0], m_best_ever))
					{
						m_best_ever = m_nests[0];
					}
				}
			}

			#pragma omp single
			{
				RankNests();
				AbandonNests();
				++m_current_generation;
			}

		}
	}
	return m_best_ever.GetSolutions();
};

void CuckooSearch::GenerateInitialPopulation()
{
	m_nests = std::vector<Nest>(m_amount_of_nests);
	#pragma omp parallel
	{
		#pragma omp for
		for (int i = 0; i < m_amount_of_nests; ++i)
		{
			m_nests[i] = Nest(m_objective_function);
		}
		RankNests();
		#pragma omp sections
		{
			#pragma omp section
			RecalculateLambdas();
			#pragma omp section
			RecalculateStep();
		}
	}
};

void CuckooSearch::AbandonNests()
{
	if (m_abandon_probability < 0 || m_abandon_probability > 1)
	{
		throw std::exception("Abandon probability must be in range [0, 1]\n");
	}

	int rnd_index(m_amount_of_nests - m_abandon_probability * (rand() / double(RAND_MAX + 1.0)) * m_amount_of_nests);
	#pragma omp parallel for
	for (int i = rnd_index; i < m_amount_of_nests; ++i)
	{
		m_nests[i] = Nest(m_objective_function);
	}
};

void CuckooSearch::RankNests()
{
	std::sort(m_nests.begin(), m_nests.end(), m_cmp_fitness);
};

void CuckooSearch::RecalculateStep()
{
	#pragma omp parallel for
	for (int i = 0; i < m_amount_of_nests; ++i)
	{
		std::valarray<double> new_alpha = m_step.GetMaxStep() * std::pow(m_delta_step, double(m_current_generation));
		m_nests[i].SetAlpha(new_alpha);
	}
};

void CuckooSearch::RecalculateLambdas()
{
	if (m_amount_of_nests == 1)
	{
		m_nests[0].SetLambda(m_lambda.GetMinLambda() + (m_lambda.GetMaxLamda() - m_lambda.GetMinLambda()) / 2.0);
		return;
	}
	const double delta_lambda = m_lambda.GetMaxLamda() - m_lambda.GetMinLambda();
	#pragma omp parallel for
	for (int i = 0; i < m_amount_of_nests; ++i)
	{
		double new_lambda = m_lambda.GetMaxLamda() - (double(i) * (delta_lambda)) / double(m_amount_of_nests - 1);
		m_nests[i].SetLambda(new_lambda);
	}
};
