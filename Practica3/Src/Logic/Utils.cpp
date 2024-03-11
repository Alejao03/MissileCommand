#include "Utils.h"
#include <random>
int Utils::randomInt(int min, int max)
{
	return  rand() % (max + 1 - min) + min;;
}
 float Utils::randomFloat(float min, float max)
{
	return (max - min) * ((((float)rand()) / (float)RAND_MAX)) + min;
}
