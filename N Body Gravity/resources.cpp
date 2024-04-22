#include "resources.hpp"
#include <random>
raylib::Window window(800, 800, "fisics");

double randomDouble(double min, double max)
{
	std::uniform_real_distribution<double> unif(min, max);
	static std::default_random_engine re;
	double a_random_double = unif(re);
	return a_random_double;
}
