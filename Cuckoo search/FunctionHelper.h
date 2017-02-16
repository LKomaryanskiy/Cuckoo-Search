#ifndef FUNCTION_HELPER
#define FUNCTION_HELPER

#include <functional>

using ObjectiveFunction = std::function<double(std::valarray<double>)>;

using CompareFitness = std::function<bool(double, double)>;

using StopCritearian = std::function<bool()>;

#endif // !FUNCTION_HELPER

