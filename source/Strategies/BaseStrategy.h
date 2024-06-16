#pragma once

class BaseStrategy
{
public:
	virtual float Apply(float Price, float Time) = 0;
};
