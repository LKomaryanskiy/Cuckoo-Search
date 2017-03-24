/*
	Description:
		Class for tests automatization and collects statistics 
		which is saved in file and printed in console.
*/

#ifndef STATISTICS
#define STATISTICS

#include "CuckooSearch.h"
#include "FunctionHelper.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <ctime>
#include <algorithm>


struct CuckooInfo
{
	unsigned int iterations;
	unsigned int nests;
	Lambda lambda;
	Step step;
	double probability;
};

struct ResultStatistics
{
	std::valarray<double> all_results;
	double best_result;
	double worst_result;
	double average_result;
	double std_dev;
};

struct SolutionStatistics
{
	std::valarray<std::valarray<double>> fitness_dynamics;
	std::valarray<double> average_fitness_dynamics;

	std::valarray<std::valarray<Egg>> solution_dynamics;
	std::valarray<Egg> average_solution_dynamics;
};

struct TestInfo
{
	std::string function_name;
	unsigned int number_of_tests;
	CuckooInfo	cuckoo_info;
	ResultStatistics result_statistics;
	std::vector<Nest> solutions;
	SolutionStatistics solution_statistics;
	std::clock_t test_time;
};

class Statistics
{
public:
	Statistics(CuckooSearch& cs);
	virtual ~Statistics() {};

	virtual void RunTestMin(unsigned int number_of_tests = 1);
	virtual void RunAnvancedTestMin(unsigned int number_of_tests = 1);
	void CreateFiles4Grapher(bool create, unsigned int points_4_function = 50);
	inline void CreateSolutionsLog(bool create) { m_log_files = create; };
protected:
	CuckooSearch m_cs;
	TestInfo m_info;
	StatisticsHandler m_handler;
	std::string m_functions_tests_path = "Function test\\";
	std::string m_solutions_log_path = "logs\\";
	std::string m_grapher_files_path = "4AdvancedGrapher\\";

	bool m_grapher_files;
	unsigned int m_points;
	bool m_log_files;
	unsigned int m_curr_test;
	bool m_basic_test;

	double GetStdDeviation();
	void CalculateResultStatistics();
	void CalculateSolutionStatistics();
	void CreateStructs();

	void OutputTotalInfo(std::ostream& o_stream, bool print_solutions = false);
	void PrintHeader(std::ostream& o_stream);
	void PrintLog();
	void PrintDynamics();

	void CreateHandler();
};

class Compare : public Statistics
{
public:
	Compare(CuckooSearch& cs, double iter_multiplier = 1.0) :
		Statistics(cs), m_multiplier(iter_multiplier) {};
	virtual ~Compare() {};
	virtual void RunTestMin(unsigned int number_of_tests = 1);
	virtual void RunAnvancedTestMin(unsigned int number_of_tests = 1);
protected:
	double m_multiplier;
};


#endif //STATISTICS 

