#include "../DataProviders/StockSimulatorDataProvider.h"
#include "BuyMarketRequest.h"

BuyMarketRequest::BuyMarketRequest(MarketRequestData& marketRequestData)
	: BaseMarketRequest(marketRequestData)
{

}

bool BuyMarketRequest::TryApply(StockData& currentStockData)
{
	if (!BaseMarketRequest::TryApply(currentStockData))
	{
		return false;
	}

	StockData& requestStockData = mMarketRequestData.mStockData;

	if (currentStockData.mPrice <= requestStockData.mPrice)
	{
		mStockDataToConsume = currentStockData;
		mCanBeConsumed = true;
	}

	return true;
}

bool BuyMarketRequest::Consume()
{
	if (!BaseMarketRequest::Consume())
	{
		return false;
	}

	std::string& portfolioName = mMarketRequestData.mPortfolioName;
	std::string& stockName = mMarketRequestData.mStockData.mName;
	int requestedAmount = mMarketRequestData.mAmount;

	StockSimulatorDataProvider::Instance().AddStocksToPortfolio(portfolioName, stockName, requestedAmount, mStockDataToConsume.mPrice);

	return true;
}