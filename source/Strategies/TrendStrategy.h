#pragma once

#include "BaseStrategy.h"

class TrendStrategy : public BaseStrategy
{
public:
	virtual float Apply(float Price, float Time);
};

