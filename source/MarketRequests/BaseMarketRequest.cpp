#include "../DataProviders/StockSimulatorDataProvider.h"
#include "BaseMarketRequest.h"
#include <iostream>

BaseMarketRequest::BaseMarketRequest(MarketRequestData& marketRequestData)
	: mMarketRequestData(marketRequestData)
{

}

bool BaseMarketRequest::TryApply(StockData& currentStockData)
{
	if (mCanBeRemoved)
	{
		return false;
	}

	StockData& requestStockData = mMarketRequestData.mStockData;

	if (requestStockData.mName != currentStockData.mName)
	{
		return false;
	}

	return true;
}

bool BaseMarketRequest::Consume()
{
	if (!mCanBeRemoved && mCanBeConsumed)
	{
		mCanBeConsumed = false;
		mCanBeRemoved = true;

		return true;
	}

	return false;
}

bool BaseMarketRequest::CanBeRemoved()
{
	return mCanBeRemoved;
}

void BaseMarketRequest::Validate()
{
	StockData& requestStockData = mMarketRequestData.mStockData;

	if (requestStockData.mName.empty()
		|| !StockSimulatorDataProvider::Instance().IsStockExists(requestStockData.mName)
	)
	{
		std::cout << "Stock name '" << requestStockData.mName << "' is invalid, request will be removed\n";

		mCanBeRemoved = true;
	}
}
