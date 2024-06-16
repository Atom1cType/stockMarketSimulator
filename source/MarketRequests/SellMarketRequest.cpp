#include "../DataProviders/StockSimulatorDataProvider.h"
#include "SellMarketRequest.h"
#include <iostream>

SellMarketRequest::SellMarketRequest(MarketRequestData& marketRequestData)
	: BaseMarketRequest(marketRequestData)
{

}

bool SellMarketRequest::TryApply(StockData& currentStockData)
{
	if (!BaseMarketRequest::TryApply(currentStockData))
	{
		return false;
	}

	StockData& requestStockData = mMarketRequestData.mStockData;

	if (currentStockData.mPrice >= requestStockData.mPrice)
	{
		mStockDataToConsume = currentStockData;
		mCanBeConsumed = true;
	}

	return true;
}

bool SellMarketRequest::Consume()
{
	if (!BaseMarketRequest::Consume())
	{
		return false;
	}

	std::string& portfolioName = mMarketRequestData.mPortfolioName;
	std::string& stockName = mMarketRequestData.mStockData.mName;
	int requestedAmount = mMarketRequestData.mAmount;

	StockSimulatorDataProvider::Instance().RemoveStocksFromPortfolio(portfolioName, stockName, requestedAmount);

	return true;
}

void SellMarketRequest::Validate()
{
	BaseMarketRequest::Validate();

	std::string& portfolioName = mMarketRequestData.mPortfolioName;
	std::string& stockName = mMarketRequestData.mStockData.mName;
	int requestedAmount = mMarketRequestData.mAmount;

	int stockAmountInPortfolio = StockSimulatorDataProvider::Instance().GetStockAmountForPortfolio(portfolioName, stockName);

	if (stockAmountInPortfolio < 0)
	{
		std::cout << "There is no stocks with name '" << stockName << "' in portfolio, request will be removed\n";
		mCanBeRemoved = true;

		return;
	}

	if (stockAmountInPortfolio < requestedAmount)
	{
		std::cout << "There is no enough stocks with name '" << stockName << "' in portfolio, request will be removed\n";
		mCanBeRemoved = true;

		return;
	}
}
