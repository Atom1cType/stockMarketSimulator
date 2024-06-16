#pragma once

#include "../Data/MarketRequestData.h"

class BaseMarketRequest
{
public:
	BaseMarketRequest(MarketRequestData& marketRequestData);
	virtual ~BaseMarketRequest() {};

	virtual bool TryApply(StockData& currentStockData);
	virtual bool Consume();

	virtual bool CanBeRemoved();

	virtual void Validate();

protected:
	MarketRequestData mMarketRequestData;
	StockData mStockDataToConsume;
	bool mCanBeRemoved = false;
	bool mCanBeConsumed = false;
};
