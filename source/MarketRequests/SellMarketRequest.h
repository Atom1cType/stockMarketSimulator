#pragma once

#include "BaseMarketRequest.h"

class SellMarketRequest : public BaseMarketRequest
{
public:
	SellMarketRequest(MarketRequestData& marketRequestData);
	virtual bool TryApply(StockData& currentStockData) override;
	virtual bool Consume() override;
	virtual void Validate() override;
};
