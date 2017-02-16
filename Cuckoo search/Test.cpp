#include "LevyFlight.h"
#include <stdlib.h>

int main()
{
	for (int i = 0; i < 10; ++i) {
		std::valarray<double> test = LevyFlight::GetValue(10);
	}
	return EXIT_SUCCESS;
}