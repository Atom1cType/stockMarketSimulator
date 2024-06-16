#include "StockSimulatorDataProvider.h"
#include <iostream>
#include <iterator>
#include <list>
#include <string>

namespace
{
	const std::string StockSimulatorConfigName = "StockSimulatorConfig.json";
	const std::string DefaultPortfolioConfigName = "DefaultPortfolioConfig.json";
	const std::string DefaultPortfolioName = "DefaultPortfolio";
}

StockSimulatorDataProvider& StockSimulatorDataProvider::Instance()
{
	static StockSimulatorDataProvider* Instance = new StockSimulatorDataProvider();

	return *Instance;
}

void StockSimulatorDataProvider::Initialize()
{
	InitializeStockSimulatorData();
}

void StockSimulatorDataProvider::VisitAllStockData(const std::function<void(StockData&)>& func)
{
	std::lock_guard lockGuard(mSharedMutex);

	for (StockData& stockData : mStockSimulatorData.mStockDataVector)
	{
		func(stockData);
	}
}

void StockSimulatorDataProvider::AddNewPortfolio(const std::string& portfolioName)
{
	std::lock_guard lockGuard(mSharedMutex);

	mPortfoliosMapData.insert({ portfolioName, mDefaultPortfolioData });
}

void StockSimulatorDataProvider::SetActivePortfolio(const std::string& portfolioName)
{
	std::lock_guard lockGuard(mSharedMutex);

	if (portfolioName.empty())
	{
		return;
	}

	mCurrentActivePortfolioName = portfolioName;

	std::cout << "New active portfolio name: " << mCurrentActivePortfolioName << std::endl;
}

void StockSimulatorDataProvider::DumpActivePortfolioStatus()
{
	std::shared_lock lockGuard(mSharedMutex);

	auto portfolioResult = mPortfoliosMapData.find(mCurrentActivePortfolioName);

	if (portfolioResult == mPortfoliosMapData.end())
	{
		std::cout << "Failed to find portfolio with name: " << mCurrentActivePortfolioName << std::endl;
		return;
	}

	std::cout << "Current status for portfolio: " << mCurrentActivePortfolioName << std::endl;

	PortfolioData& portfolioData = portfolioResult->second;

	for (const auto& [key, value] : portfolioData.mPortfolio)
	{
		float currentStockMarketPrice = 0.0f;

		for (StockData& simulatorStockData : mStockSimulatorData.mStockDataVector)
		{
			if (simulatorStockData.mName == key)
			{
				currentStockMarketPrice = simulatorStockData.mPrice;
				break;
			}
		}

		std::cout << "Stock name '" << key << "' amount: " << value.size() << " current stock market price: " << currentStockMarketPrice << std::endl;
	}
}

void StockSimulatorDataProvider::DumpActivePortfolioReport()
{
	std::shared_lock lockGuard(mSharedMutex);

	auto portfolioResult = mPortfoliosMapData.find(mCurrentActivePortfolioName);

	if (portfolioResult == mPortfoliosMapData.end())
	{
		std::cout << "Failed to find portfolio with name: " << mCurrentActivePortfolioName << std::endl;
		return;
	}

	PortfolioData& portfolioData = portfolioResult->second;

	float totalPortfolioMarketPrice = 0.0;
	float totalPortfolionSpent = 0.0;

	for (const auto& [key, value] : portfolioData.mPortfolio)
	{
		float currentStockMarketPrice = 0.0f;

		for (StockData& simulatorStockData : mStockSimulatorData.mStockDataVector)
		{
			if (simulatorStockData.mName == key)
			{
				currentStockMarketPrice = simulatorStockData.mPrice;
				break;
			}
		}

		float totalStocksMarketPrice = currentStockMarketPrice * value.size();
		float totalStocksSpent = 0.0;

		for (const PortfolioStockData& portfolioStockData : value)
		{
			totalStocksSpent += portfolioStockData.mPrice;
		}

		totalPortfolioMarketPrice += totalStocksMarketPrice;
		totalPortfolionSpent += totalStocksSpent;
		float incomeFromStock = totalPortfolioMarketPrice - totalPortfolionSpent;

		std::string message = std::format("Stock name '{0}' amount: {1}\nTotal market price: {2}, total spent: {3}, total income: {4}"
			, key, value.size(), totalStocksMarketPrice, totalStocksSpent, incomeFromStock);

		std::cout << message << std::endl;
	}

	float totalIncome = totalPortfolioMarketPrice - totalPortfolionSpent;

	std::string message = std::format("Portfolio '{0}' total market price: {1} total spent: {2}\nTotal income: {3}"
		, mCurrentActivePortfolioName, totalPortfolioMarketPrice, totalPortfolionSpent, totalIncome);

	std::cout << message << std::endl;
}

bool StockSimulatorDataProvider::IsPortfolioExists(const std::string& portfolioName)
{
	std::shared_lock lockGuard(mSharedMutex);

	if (auto result = mPortfoliosMapData.find(portfolioName); result != mPortfoliosMapData.end())
	{
		return true;
	}
	else
	{
		std::cout << "Failed to find portfolio with name: " << portfolioName << std::endl;
		return false;
	}
}

bool StockSimulatorDataProvider::IsStockExists(const std::string& stockName)
{
	std::shared_lock lockGuard(mSharedMutex);

	for (StockData& stockData : mStockSimulatorData.mStockDataVector)
	{
		if (stockData.mName == stockName)
		{
			return true;
		}
	}

	return false;
}

bool StockSimulatorDataProvider::AddStocksToPortfolio(const std::string& portfolioName, const std::string& stockName, int amount, float price)
{
	std::lock_guard lockGuard(mSharedMutex);

	auto portfolioResult = mPortfoliosMapData.find(portfolioName);

	if (portfolioResult == mPortfoliosMapData.end())
	{
		std::cout << "Failed to find portfolio with name: " << portfolioName << std::endl;
		return false;
	}

	PortfolioData& portfolioData = portfolioResult->second;

	auto stockResult = portfolioData.mPortfolio.find(stockName);

	if (stockResult == portfolioData.mPortfolio.end())
	{
		std::cout << "Failed to find stock with name: " << stockName << "adding...\n";
		
		if (!TryAddNewStockToPortfolio(portfolioData, stockName))
		{
			return false;
		}

		stockResult = portfolioData.mPortfolio.find(stockName);
	}

	std::list<PortfolioStockData> stocksToAdd;
	stocksToAdd.assign(amount, { price });

	stockResult->second.splice(stockResult->second.end(), stocksToAdd);

	std::string message = std::format("Stocks `{0}` successfully added to portfolio `{1}`, with amount: {2} and price: {3}", stockName, portfolioName, amount, price);

	std::cout << message << std::endl;
	return true;
}

bool StockSimulatorDataProvider::RemoveStocksFromPortfolio(const std::string& portfolioName, const std::string& stockName, int amount)
{
	std::lock_guard lockGuard(mSharedMutex);

	auto portfolioResult = mPortfoliosMapData.find(portfolioName);

	if (portfolioResult == mPortfoliosMapData.end())
	{
		std::cout << "Failed to find portfolio with name: " << portfolioName << std::endl;
		return false;
	}

	PortfolioData& portfolioData = portfolioResult->second;

	auto stockResult = portfolioData.mPortfolio.find(stockName);

	if (stockResult == portfolioData.mPortfolio.end())
	{
		std::cout << "Failed to find stock with name: " << stockName << std::endl;

		return false;
	}

	std::list<PortfolioStockData>& portfolioDataList = stockResult->second;

	if (portfolioDataList.empty())
	{
		return false;
	}

	auto RemoveStockNameFromPortfolio = [&]() -> void
		{
			portfolioData.mPortfolio.erase(stockResult);
			std::cout << "Stocks `" << stockName << "` fully cleared from portfolio `" << portfolioName << "'\n";
		};

	if (amount >= portfolioDataList.size())
	{
		portfolioDataList.clear();
		RemoveStockNameFromPortfolio();
		return true;
	}

	auto iterator = portfolioDataList.begin();

	std::advance(iterator, amount);

	portfolioDataList.erase(portfolioDataList.begin(), iterator);

	if (portfolioDataList.empty())
	{
		RemoveStockNameFromPortfolio();
	}

	std::cout << "Stocks `" << stockName << "` successfully removed to portfolio `" << portfolioName << "` with amount: " << amount << std::endl;
	return true;
}

int StockSimulatorDataProvider::GetStockAmountForPortfolio(const std::string& portfolioName, const std::string& stockName)
{
	std::shared_lock lockGuard(mSharedMutex);

	auto portfolioResult = mPortfoliosMapData.find(portfolioName);

	if (portfolioResult == mPortfoliosMapData.end())
	{
		std::cout << "Failed to find portfolio with name: " << portfolioName << std::endl;
		return -1;
	}

	PortfolioData& portfolioData = portfolioResult->second;

	auto stockResult = portfolioData.mPortfolio.find(stockName);

	if (stockResult == portfolioData.mPortfolio.end())
	{
		std::cout << "Failed to find stock with name: " << stockName << std::endl;

		return -1;
	}

	return static_cast<int>(stockResult->second.size());
}

std::string StockSimulatorDataProvider::GetActivePortfolioName()
{
	std::shared_lock lockGuard(mSharedMutex);

	return mCurrentActivePortfolioName;
}

StockSimulatorData& StockSimulatorDataProvider::GetStockSimulatorData()
{
	return mStockSimulatorData;
}

void StockSimulatorDataProvider::InitializeStockSimulatorData()
{
	std::lock_guard lockGuard(mSharedMutex);

	TryParseConfigToJsonData<StockSimulatorData>(StockSimulatorConfigName, mStockSimulatorData);

	TryParseConfigToJsonData<PortfolioData>(DefaultPortfolioConfigName, mDefaultPortfolioData);
	mPortfoliosMapData.insert({ DefaultPortfolioName, mDefaultPortfolioData });

	mCurrentActivePortfolioName = DefaultPortfolioName;
}

bool StockSimulatorDataProvider::TryAddNewStockToPortfolio(PortfolioData& portfolioData, const std::string& stockName)
{
	for (StockData& simulatorStockData : mStockSimulatorData.mStockDataVector)
	{
		if (simulatorStockData.mName == stockName)
		{
			portfolioData.mPortfolio.insert({ stockName, {} });

			return true;
		}
	}

	std::cout << "Failed to add new stock with name: " << stockName << " doesn't present in configuration\n";

	return false;
}

std::filesystem::path StockSimulatorDataProvider::TryFindFile(std::filesystem::path& path, const std::string& fileName, int outSteps)
{
	if (!std::filesystem::is_directory(path))
	{
		return {};
	}

	for (const auto& directoryEntry : std::filesystem::recursive_directory_iterator(path))
	{
		if (!directoryEntry.exists())
		{
			continue;
		}

		if (directoryEntry.is_regular_file() && directoryEntry.path().filename() == fileName)
		{
			return directoryEntry.path();
		}
	}

	if (--outSteps > 0)
	{
		path /= "..";
		return TryFindFile(path, fileName, outSteps);
	}

	return {};
}
