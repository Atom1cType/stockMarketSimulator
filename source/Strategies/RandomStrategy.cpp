#include "RandomStrategy.h"
#include <algorithm>
#include <random>

namespace
{
	std::random_device RandomDevice;
	std::default_random_engine RandomEngine(RandomDevice());
	std::uniform_int_distribution<int> uniformDistribution(-10, 10);

	const float MinResultValue = 0.1f;
}

float RandomStrategy::Apply(float Price, float Time)
{
	float resultValue = static_cast<float>(uniformDistribution(RandomEngine));

	return std::max(Price + resultValue, MinResultValue);
}
