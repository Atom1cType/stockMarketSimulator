#pragma once

#include "StockSimulatorData.h"

struct MarketRequestData
{
	int mAmount = -1;

	std::string mPortfolioName;

	StockData mStockData;
};
