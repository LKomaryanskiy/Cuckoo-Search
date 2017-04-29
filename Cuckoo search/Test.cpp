/*
	Description:
		Entry point of program.
		Have functions for testing.
*/

#include "CuckooSearch.h"
#include "TestFunctions.h"
#include "Statistics.h"

#include <stdlib.h>

enum enum_functions
{
	sphere = 1,
	ackley = 2,
	griewank = 3,
	rosenbrock = 4,
	rastrigin = 5,
	all = 6
};

//Parameters for cuckoo search
const unsigned int AMOUNT_OF_NESTS = 200;
const double MIN_STEP = 1e-6;
const double MAX_STEP = 1e-2;
const double MIN_LAMBDA = 0.1;
const double MAX_LAMBDA = 1.99;
const double ABANDON_PROBABILITY = 0.25;
const unsigned int ITERATIONS = 1000;

//Parameters for tests
const enum_functions CUUR_FUNCTION = all;
const unsigned int NUMBER_OF_TESTS = 5;
//Modified Cuckoo
const bool USE_LAZY_CUCKOO = true;
//Compare algorithms with standard and modified Cuckoo flights
const bool COMPARE_METHODS = false;
//Iteration multiplier for second algorithm (modified Cuckoo needs 3 times more calling objective function).
//This parameter enable just if set Compare methods in true.
const double ITER_MULTIPLIER = 3.0;
//Advanced test create handler, which can doing algorithm more slower, 
//but It handle more information and can create logs
const bool ADVANCED_TEST = false;
//It's log show best ever Nest stat for each iteration
const bool CREATE_LOG = true;
//It's parameter create file for AdvancedGrapher with statistics which
//demonstrates dynamics of objective function value changes
const bool CREATE_4_GRAPHER = true;
const unsigned int POINTS = 50;

void run_tests(CuckooSearch& cs)
{
	Statistics* stat;
	if (COMPARE_METHODS)
	{
		stat = new Compare(cs, ITER_MULTIPLIER);
	}
	else
	{
		stat = new Statistics(cs);
	}
	stat->CreateSolutionsLog(CREATE_LOG);
	stat->CreateFiles4Grapher(CREATE_4_GRAPHER, POINTS);
	if (ADVANCED_TEST)
	{
		stat->RunAnvancedTestMin(NUMBER_OF_TESTS);
	}
	else
	{
		stat->RunTestMin(NUMBER_OF_TESTS);
	}

	delete stat;
};

void test_sphere_function()
{
	const Bounds bounds = { -100.0, 100.0 };
	const unsigned int dimensions = 50;

	sphere_function.SetBounds(bounds);
	sphere_function.SetDimensions(dimensions);

	CuckooSearch cs = CuckooSearch(sphere_function, AMOUNT_OF_NESTS, { MIN_STEP, MAX_STEP },
	{ MIN_LAMBDA, MAX_LAMBDA }, ABANDON_PROBABILITY, ITERATIONS);
	if (USE_LAZY_CUCKOO)
	{
		cs.UseLazyCuckoo();
	}
	run_tests(cs);
};

void test_ackley_function()
{
	const Bounds bounds = { -32.768, 32.768 };
	const unsigned int dimensions = 100;
	ackley_function.SetBounds(bounds);
	ackley_function.SetDimensions(dimensions);

	CuckooSearch cs = CuckooSearch(ackley_function, AMOUNT_OF_NESTS, { MIN_STEP, MAX_STEP },
	{ MIN_LAMBDA, MAX_LAMBDA }, ABANDON_PROBABILITY, ITERATIONS);
	if (USE_LAZY_CUCKOO)
	{
		cs.UseLazyCuckoo();
	}
	run_tests(cs);
};

void test_griewank_function()
{
	const Bounds bounds = { -600.0, 600.0 };
	const unsigned int dimensions = 50;

	griewank_function.SetBounds(bounds);
	griewank_function.SetDimensions(dimensions);

	CuckooSearch cs = CuckooSearch(griewank_function, AMOUNT_OF_NESTS, { MIN_STEP, MAX_STEP },
	{ MIN_LAMBDA, MAX_LAMBDA }, ABANDON_PROBABILITY, ITERATIONS);
	if (USE_LAZY_CUCKOO)
	{
		cs.UseLazyCuckoo();
	}
	run_tests(cs);
};

void test_rosenbrock_function()
{
	const Bounds bounds = { -5.0, 10 };
	const unsigned int dimensions = 30;

	rosenbrock_function.SetBounds(bounds);
	rosenbrock_function.SetDimensions(dimensions);

	CuckooSearch cs = CuckooSearch(rosenbrock_function, AMOUNT_OF_NESTS, { MIN_STEP, MAX_STEP },
	{ MIN_LAMBDA, MAX_LAMBDA }, ABANDON_PROBABILITY, ITERATIONS);
	if (USE_LAZY_CUCKOO)
	{
		cs.UseLazyCuckoo();
	}
	run_tests(cs);
};

void tets_rasrigin_function()
{
	const Bounds bounds = { -5.12, 5.12 };
	const unsigned int dimensions = 30;

	rastrigin_function.SetBounds(bounds);
	rastrigin_function.SetDimensions(dimensions);

	CuckooSearch cs = CuckooSearch(rastrigin_function, AMOUNT_OF_NESTS, { MIN_STEP, MAX_STEP },
	{ MIN_LAMBDA, MAX_LAMBDA }, ABANDON_PROBABILITY, ITERATIONS);
	if (USE_LAZY_CUCKOO)
	{
		cs.UseLazyCuckoo();
	}
	run_tests(cs);
};

void test_all_functions()
{
	test_sphere_function();
	test_ackley_function();
	test_griewank_function();
	test_rosenbrock_function();
	tets_rasrigin_function();
};

void test()
{
	switch (CUUR_FUNCTION)
	{
	case sphere:
		{
			test_sphere_function();
			break;
		}
	case ackley:
		{
			test_ackley_function();
			break;
		}
	case griewank:
		{
			test_griewank_function();
			break;
		}
	case rosenbrock:
		{
			test_rosenbrock_function();
			break;
		}
	case rastrigin:
		{
			tets_rasrigin_function();
			break;
		}
	case all:
		{
			test_all_functions();
			break;
		}
	}

	system("pause");
};


int main()
{
	//test();
	/*ackley_function.SetDimensions(20);
	Egg solution = std::valarray<double>(0.0, 20);
	std::cout << ackley_function(solution) << "\n";
	system("pause");*/
	
	return EXIT_SUCCESS;
}