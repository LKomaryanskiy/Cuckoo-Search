/*
	Description:
		Have basics functions for testing global optima search.
*/

#ifndef TEST_FUNCTIONS
#define TEST_FUNCTIONS

#include "FunctionHelper.h"

ObjectiveFunction sphere_function = ObjectiveFunction(
	[](std::valarray<double> args)
{
	return (args* args).sum();
}, 50, { -100.0, 100.0 }, "Sphere function");

ObjectiveFunction michaelwicz_function = ObjectiveFunction(
	[](std::valarray<double> args)
{
	const double m = 10;
	const double first_arg = -std::sin(args[0]) * std::pow(std::sin(args[0] * args[0] / M_PI), 2.0 * m);
	const double second_arg = -std::sin(args[args.size() - 1]) * std::pow(std::sin(2.0 * args[args.size() - 1] * args[args.size() - 1] / M_PI), 2.0 * m);
	return first_arg + second_arg;
}, 2, { 0.0, 5.0 }, "Michaelwicz function");

ObjectiveFunction ackley_function = ObjectiveFunction(
	[](std::valarray<double> args)
{
	const double a = 20.0;
	const double b = 0.2;
	const double c = M_PI * 2.0;
	const double cubic_sum = (args * args).sum();
	const double first_arg = a * std::exp(- b * std::sqrt((1.0 / double(args.size())) * cubic_sum));
	const double cos_sum = std::cos(c * args).sum();
	const double second_arg = std::exp((1.0 / double(args.size())) * cos_sum);
	const double difference = M_E - second_arg; 
	const double result = difference + a - first_arg;
	return result;
}, 20, { -32.768, 32.768 }, "Ackley function");

ObjectiveFunction griewank_function = ObjectiveFunction(
	[](std::valarray<double> args)
{
	const double sum = (args * args).sum() / 4000.0;
	double product = 1.0;
	for (unsigned i = 0; i < args.size(); ++i)
	{
		product *= std::cos(args[i] / double(i + 1));
	}
	return sum - product + 1.0;
}, 50, { -600, 600 }, "Griewank function");

ObjectiveFunction rastrigin_function = ObjectiveFunction(
	[](std::valarray<double> args)
{
	return 10 * args.size() + (args * args - 10.0 * std::cos(2 * M_PI * args)).sum();
}, 30, { -5.12, 5.12 }, "Rastrigin function");

ObjectiveFunction rosenbrock_function(
	[](std::valarray<double> args)
{
	double sum = 0.0;
	for (unsigned int i = 0; i < args.size() - 1; ++i)
	{
		sum += 100 * (args[i + 1] - args[i] * args[i]) * (args[i + 1] - args[i] * args[i]) + 
			(args[i] - 1) * (args[i] - 1);
	}
	return sum;
}, 30, { -5.0, 10 }, "Rosenbrock function");

#endif // !TEST_FUNCTIONS

