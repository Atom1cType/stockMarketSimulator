#include "../Strategies/RandomStrategy.h"	
#include "../Strategies/TrendStrategy.h"	
#include "StrategyFacade.h"
#include <iostream>

StrategyFacade& StrategyFacade::Instance()
{
	static StrategyFacade* Instance = new StrategyFacade();

	return *Instance;
}

void StrategyFacade::SetStrategy(SimulatorStrategyType simulatorStrategyType)
{
	mCurrentStrategyType = simulatorStrategyType;
	UpdateStrategy();
}

float StrategyFacade::ApplyStrategy(float Price, float Time)
{
	if (!mCurrentStrategy)
	{
		return -1.0f;
	}

	return mCurrentStrategy->Apply(Price, Time);
}

StrategyFacade::StrategyFacade() 
{
	UpdateStrategy();
}

void StrategyFacade::UpdateStrategy()
{
	switch (mCurrentStrategyType)
	{
		case SimulatorStrategyType::random:
		case SimulatorStrategyType::undefined:
			mCurrentStrategy = std::make_shared<RandomStrategy>();
			std::cout << "Strategy updated to Random" << std::endl;
			break;
		case SimulatorStrategyType::trend:
			mCurrentStrategy = std::make_shared<TrendStrategy>();
			std::cout << "Strategy updated to Trend" << std::endl;
			break;

		default:
			std::cout << "Strategy type is invalid" << std::endl;
			break;
	}
}
