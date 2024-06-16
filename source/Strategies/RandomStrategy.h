#pragma once

#include "BaseStrategy.h"

class RandomStrategy : public BaseStrategy
{
public:
	virtual float Apply(float Price, float Time);
};
