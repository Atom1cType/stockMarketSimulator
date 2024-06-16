#pragma once

#include "BaseMarketRequest.h"

class BuyMarketRequest : public BaseMarketRequest
{
public:
	BuyMarketRequest(MarketRequestData& marketRequestData);
	virtual bool TryApply(StockData& currentStockData) override;
	virtual bool Consume() override;
};
