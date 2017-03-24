#include "Statistics.h"

Statistics::Statistics(CuckooSearch& cs) :
	m_cs(cs) 
{
	m_info.function_name = m_cs.GetObjectiveFunction().GetName();
	m_info.cuckoo_info.iterations = m_cs.GetMaxGenerations();
	m_info.cuckoo_info.nests = m_cs.GetNumberOfNests();
	m_info.cuckoo_info.lambda = m_cs.GetLambda();
	m_info.cuckoo_info.step = m_cs.GetStep();
	m_info.cuckoo_info.probability = m_cs.GetAbandonProbability();
	m_basic_test = true;
};

void Statistics::RunTestMin(unsigned int number_of_tests)
{
	if (m_basic_test)
	{
		m_handler = []() {};
		m_cs.SetStatisticsHandler(&m_handler);
	}
	m_basic_test = true;
	std::string full_file_path = m_functions_tests_path + m_cs.GetObjectiveFunction().GetName();
	if (m_cs.IsLazyCuckoo())
	{
		full_file_path += "_mod.txt";
	}
	else
	{
		full_file_path += "_std.txt";
	}

	std::ofstream o_file(full_file_path);

	m_info.number_of_tests = number_of_tests;
	m_info.result_statistics.all_results = std::valarray<double>(number_of_tests);
	m_info.solutions = std::vector<Nest>(number_of_tests);

	PrintHeader(std::cout);
	PrintHeader(o_file);

	std::clock_t start_time = std::clock();
	o_file.close();

	for (m_curr_test = 0; m_curr_test < number_of_tests; ++m_curr_test)
	{
		std::cout << "Test #" << m_curr_test + 1 << " result: ";
		m_cs.FindMin();
		m_info.solutions[m_curr_test] = m_cs.GetCurrentBestNest();
		m_info.result_statistics.all_results[m_curr_test] = m_cs.GetCurrentBestValue();
		std::cout << m_info.result_statistics.all_results[m_curr_test] << "\n";

		std::ofstream o_file(full_file_path, std::ios_base::app);
		o_file << "Test #" << m_curr_test + 1 << " result: " << m_info.result_statistics.all_results[m_curr_test] << "\n";
		o_file.close();
	}

	m_info.test_time = std::clock() - start_time;
	CalculateResultStatistics();
	o_file = std::ofstream(full_file_path, std::ios_base::app);
	OutputTotalInfo(std::cout);
	OutputTotalInfo(o_file, true);
};

void Statistics::RunAnvancedTestMin(unsigned int number_of_tests)
{
	m_basic_test = false;
	m_info.number_of_tests = number_of_tests;
	CreateStructs();
	CreateHandler();
	m_cs.SetStatisticsHandler(&m_handler);
	Statistics::RunTestMin(number_of_tests);
	CalculateSolutionStatistics();
	std::cout << "Printing log...\n";
	PrintDynamics();
	PrintLog();
};

void Statistics::CreateFiles4Grapher(bool create, unsigned int points_4_function)
{
	m_grapher_files = create;
	m_points = points_4_function;
};

double Statistics::GetStdDeviation()
{
	if (m_info.result_statistics.all_results.size() == 1)
		return 0.0;
	return std::sqrt((1.0 / double(m_info.result_statistics.all_results.size() - 1.0)) *
		((m_info.result_statistics.all_results - m_info.result_statistics.average_result) 
			* (m_info.result_statistics.all_results - m_info.result_statistics.average_result)).sum());
};

void Statistics::CalculateResultStatistics()
{
	m_info.result_statistics.worst_result = *std::max_element(std::begin(m_info.result_statistics.all_results), std::end(m_info.result_statistics.all_results));
	m_info.result_statistics.best_result = *std::min_element(std::begin(m_info.result_statistics.all_results), std::end(m_info.result_statistics.all_results));
	m_info.result_statistics.average_result = m_info.result_statistics.all_results.sum() / double(m_info.result_statistics.all_results.size());
	m_info.result_statistics.std_dev = GetStdDeviation();
};

void Statistics::CalculateSolutionStatistics()
{
	unsigned int number_of_tests = m_info.result_statistics.all_results.size();
	m_info.solution_statistics.average_fitness_dynamics = m_info.solution_statistics.fitness_dynamics.sum() / double(number_of_tests);
	m_info.solution_statistics.average_solution_dynamics = m_info.solution_statistics.solution_dynamics.sum() /
		std::valarray<double>(double(number_of_tests), m_info.cuckoo_info.iterations);
};

void Statistics::CreateStructs()
{
	m_info.solution_statistics.average_fitness_dynamics = std::valarray<double>(0.0, m_points);
	m_info.solution_statistics.average_solution_dynamics = std::valarray<std::valarray<double>>(
		std::valarray<double>(0.0, m_cs.GetObjectiveFunction().GetNumberOfDimensions()), m_info.cuckoo_info.iterations);
	m_info.solution_statistics.fitness_dynamics = std::valarray<std::valarray<double>>(
		std::valarray<double>(0.0, m_info.cuckoo_info.iterations), m_info.number_of_tests);
	m_info.solution_statistics.solution_dynamics = std::valarray<std::valarray<Egg>>(std::valarray<Egg>(
		std::valarray<double>(0.0, m_cs.GetObjectiveFunction().GetNumberOfDimensions()), m_info.cuckoo_info.iterations), m_info.number_of_tests);
};

void Statistics::OutputTotalInfo(std::ostream& o_stream, bool print_solutions)
{
	o_stream << "\t" << "Test time: " << m_info.test_time / double(CLOCKS_PER_SEC) << " seconds\n";
	o_stream << "\t" << "Avarage time for each function: " << m_info.test_time / (CLOCKS_PER_SEC * double(m_info.result_statistics.all_results.size())) << "\n";
	o_stream << "\t" << "Worst result: " << m_info.result_statistics.worst_result << "\n";
	o_stream << "\t" << "Best result: " << m_info.result_statistics.best_result << "\n";
	o_stream << "\t" << "Average result: " << m_info.result_statistics.average_result << "\n";
	o_stream << "\t" << "Standard deviation: " << m_info.result_statistics.std_dev << "\n";
	o_stream << "\t\t" << "*********************\n\n";

	if (print_solutions)
	{
		o_stream << "\t*** SOLUTIONS ***" << "\n";
		for (int i = 0; i < m_info.solutions.size(); ++i)
		{
			o_stream << "\tTest#" << (i + 1) << ": ";
			o_stream << m_info.solutions[i] << "\n";
		}
	}
};

void Statistics::PrintHeader(std::ostream& o_stream)
{
	o_stream << "Run " << m_info.result_statistics.all_results.size() <<
		" tests for " << m_info.function_name <<
		" (" << m_cs.GetObjectiveFunction().GetNumberOfDimensions() << " dimensions)\n" <<
		"Number of nests: " << m_info.cuckoo_info.nests << "\n" <<
		"Step: [" << m_info.cuckoo_info.step.GetMinStep()[0] << ", " << m_info.cuckoo_info.step.GetMaxStep()[0] << "]\n" <<
		"Lambda: [" << m_info.cuckoo_info.lambda.GetMinLambda() << ", " << m_info.cuckoo_info.lambda.GetMaxLamda() << "]\n" <<
		"Abandon probability: " << m_info.cuckoo_info.probability << "\n" <<
		"Iterations: " << m_info.cuckoo_info.iterations << "\n";
};

void Statistics::PrintLog()
{
	if (m_log_files)
	{
		std::string file_path = m_solutions_log_path + m_info.function_name + "\\";
		bool is_lazy = m_cs.IsLazyCuckoo();

		for (int i = 0; i < m_info.number_of_tests; ++i)
		{
			std::string full_file_path = file_path + "#" + std::to_string(i + 1);
			if (is_lazy)
			{
				full_file_path += "ModTestLog.txt";
			}
			else
			{
				full_file_path += "TestLog.txt";
			}
			std::ofstream o_file(full_file_path);
			for (int j = 0; j < m_info.cuckoo_info.iterations; ++j)
			{
				o_file << std::fixed;
				o_file << "#" << (j + 1) << " : Function value: " << std::setprecision(14) << m_info.solution_statistics.fitness_dynamics[i][j];
				for (int k = 0; k < m_cs.GetObjectiveFunction().GetNumberOfDimensions(); ++k)
				{
					o_file << std::setprecision(5) << m_info.solution_statistics.solution_dynamics[i][j][k] << ", ";
				}
				o_file << m_info.solution_statistics.solution_dynamics[i][j][m_cs.GetObjectiveFunction().GetNumberOfDimensions() - 1] << "]\n";
			}
			o_file.close();
		}
		std::string full_file_path = file_path + "#AVERAGE_";
		if (is_lazy)
		{
			full_file_path += "ModTestLog.txt";
		}
		else
		{
			full_file_path += "TestLog.txt";
		}
		std::ofstream o_file(full_file_path);
		for (int i = 0; i < m_info.cuckoo_info.iterations; ++i)
		{
			o_file << std::fixed;
			o_file << "#" << (i + 1) << " : Function value: " << std::setprecision(14) << m_info.solution_statistics.average_fitness_dynamics[i];
			for (int j = 0; j < m_cs.GetObjectiveFunction().GetNumberOfDimensions(); ++j)
			{
				o_file << std::setprecision(5) << m_info.solution_statistics.average_solution_dynamics[i][j] << ", ";
			}
			o_file << m_info.solution_statistics.average_solution_dynamics[i][m_cs.GetObjectiveFunction().GetNumberOfDimensions() - 1] << "]\n";
		}
		o_file.close();
	}
};

void Statistics::PrintDynamics()
{
	if (m_grapher_files)
	{
		std::string file_path = m_grapher_files_path + m_info.function_name + "\\";
		bool is_lazy = m_cs.IsLazyCuckoo();
		for (int i = 0; i < m_info.number_of_tests; ++i)
		{
			std::string full_file_path = file_path + "#" + std::to_string(i + 1);
			if (is_lazy)
			{
				full_file_path += "ModTestGraphLog.txt";
			}
			else
			{
				full_file_path += "TestGraphLog.txt";
			}
			std::ofstream o_file(full_file_path);
			const unsigned int step = m_info.cuckoo_info.iterations / m_points;
			int j = 0;
			while (j * step < m_info.cuckoo_info.iterations)
			{
				o_file << (j * step) << " " << m_info.solution_statistics.fitness_dynamics[i][j * step] << "\n";
				++j;
			}
			o_file.close();
		}
		std::string full_file_path = file_path + "#AVERAGE_";
		if (is_lazy)
		{
			full_file_path += "ModTestGraphLog.txt";
		}
		else
		{
			full_file_path += "TestGraphLog.txt";
		}
		std::ofstream o_file(full_file_path);
		const unsigned int step = m_info.cuckoo_info.iterations / m_points;
		int i = 0;
		while (i * step < m_info.cuckoo_info.iterations)
		{
			o_file << (i * step) << " " << m_info.solution_statistics.average_fitness_dynamics[i * step] << "\n";
			++i;
		}
		o_file.close();
	}
};

void Statistics::CreateHandler()
{
	m_handler =
	[&]() 
	{
		const unsigned int curr_generation = m_cs.GetCurrentGeneration() - 1;
		m_info.solution_statistics.fitness_dynamics[m_curr_test][curr_generation] = m_cs.GetCurrentBestNest().GetFitness();
		m_info.solution_statistics.solution_dynamics[m_curr_test][curr_generation] = m_cs.GetCurrentBestNest().GetSolutions();
	};
};

void Compare::RunTestMin(unsigned int number_of_tests)
{
	m_cs.UseLazyCuckoo();
	Statistics::RunTestMin(number_of_tests);
	m_cs.SetMaxGenerations(unsigned int(m_cs.GetMaxGenerations() * m_multiplier));
	m_cs.UseStandartCuckoo();
	m_info.cuckoo_info.iterations = m_cs.GetMaxGenerations();
	Statistics::RunTestMin(number_of_tests);
};

void Compare::RunAnvancedTestMin(unsigned int number_of_tests)
{
	m_cs.UseLazyCuckoo();
	Statistics::RunAnvancedTestMin(number_of_tests);
	m_cs.SetMaxGenerations(unsigned int(m_cs.GetMaxGenerations() * m_multiplier));
	m_cs.UseStandartCuckoo();
	m_info.cuckoo_info.iterations = m_cs.GetMaxGenerations();
	Statistics::RunAnvancedTestMin(number_of_tests);
};