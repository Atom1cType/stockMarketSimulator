#pragma once

#include "../Data/StockSimulatorData.h"
#include <concepts>
#include <filesystem>
#include <fstream>
#include <functional>
#include <shared_mutex>

class StockSimulatorDataProvider
{
public:
	static StockSimulatorDataProvider& Instance();

	void Initialize();
	void VisitAllStockData(const std::function<void(StockData&)>& func);
	void AddNewPortfolio(const std::string& portfolioName);
	void SetActivePortfolio(const std::string& portfolioName);
	void DumpActivePortfolioStatus();
	void DumpActivePortfolioReport();

	bool IsPortfolioExists(const std::string& portfolioName);
	bool IsStockExists(const std::string& stockName);
	bool AddStocksToPortfolio(const std::string& portfolioName, const std::string& stockName, int amount, float price = 0.0f);
	bool RemoveStocksFromPortfolio(const std::string& portfolioName, const std::string& stockName, int amount);

	int GetStockAmountForPortfolio(const std::string& portfolioName, const std::string& stockName);

	std::string GetActivePortfolioName();

	StockSimulatorData& GetStockSimulatorData();

	template<std::derived_from<JsonData> JsonType>
	bool TryParseConfigToJsonData(const std::string& configName, JsonType& data);

private:
	StockSimulatorDataProvider() {};

	void InitializeStockSimulatorData();

	bool TryAddNewStockToPortfolio(PortfolioData& portfolioData, const std::string& stockName);

	std::filesystem::path TryFindFile(std::filesystem::path& path, const std::string& fileName, int outSteps = 3);

	StockSimulatorData mStockSimulatorData;
	PortfolioData mDefaultPortfolioData;
	std::unordered_map<std::string, PortfolioData> mPortfoliosMapData;

	std::string mCurrentActivePortfolioName;

	mutable std::shared_mutex mSharedMutex;
};

template<std::derived_from<JsonData> JsonType>
bool StockSimulatorDataProvider::TryParseConfigToJsonData(const std::string& configName, JsonType& data)
{
	std::filesystem::path currentPath = std::filesystem::current_path();
	std::filesystem::path pathToConfig = TryFindFile(currentPath, configName);

	std::ifstream configFile(pathToConfig);

	if (!configFile.good())
	{
		return false;
	}

	if (configFile.is_open())
	{
		std::stringstream stream;
		stream << configFile.rdbuf();

		std::string streamStr = stream.str();

		if (nlohmann::json::accept(streamStr))
		{
			nlohmann::json configJson = nlohmann::json::parse(streamStr);
			configJson.get_to(data);

			return true;
		}
	}

	return false;
}

