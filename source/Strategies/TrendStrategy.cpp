#include "TrendStrategy.h"
#include <algorithm>
#include <cmath>
#include <random>

namespace
{
	const float Amplitude = 5;
	const float Offset = 10;
	const float MinResultValue = 0.5f;
}

float TrendStrategy::Apply(float Price, float Time)
{
	float result = Amplitude * std::sin(Time - Offset) + Price;

	return std::max(result, MinResultValue);
}
