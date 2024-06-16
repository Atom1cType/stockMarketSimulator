#pragma once

#include "../Data/StockSimulatorData.h"
#include <memory>

class BaseStrategy;

class StrategyFacade
{
public:
	static StrategyFacade& Instance();

	void SetStrategy(SimulatorStrategyType simulatorStrategy);
	
	float ApplyStrategy(float Price, float Time);

private:
	StrategyFacade();

	void UpdateStrategy();

	SimulatorStrategyType mCurrentStrategyType = SimulatorStrategyType::random;

	std::shared_ptr<BaseStrategy> mCurrentStrategy;
};
