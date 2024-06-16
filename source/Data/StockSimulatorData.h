#pragma once

#include "../../nlohman/json.hpp"
#include <string>
#include <unordered_map>
#include <vector>

enum class SimulatorStrategyType
{
	undefined = -1,
	random = 0,
	trend = 1,
};

NLOHMANN_JSON_SERIALIZE_ENUM(SimulatorStrategyType,
	{
		{SimulatorStrategyType::undefined, "undefined"},
		{SimulatorStrategyType::random, "random"},
		{SimulatorStrategyType::trend, "trend"},
	}
);

struct JsonData
{

};

struct StockData
{
	std::string mName;

	float mPrice = 0.0f;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(StockData, mName, mPrice);

struct StockSimulatorSettings
{
	float mScreenUpdateIntervalSec = 5.0;

	SimulatorStrategyType mDefaultStrategy = SimulatorStrategyType::random;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(StockSimulatorSettings, mScreenUpdateIntervalSec, mDefaultStrategy);

struct StockSimulatorData : public JsonData
{
	StockSimulatorSettings mStockSimulatorSettings;

	std::vector<StockData> mStockDataVector;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(StockSimulatorData, mStockDataVector, mStockSimulatorSettings);

struct PortfolioStockData
{
	float mPrice = 0.0f;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(PortfolioStockData, mPrice);

struct PortfolioData : public JsonData
{
	std::unordered_map<std::string, std::list<PortfolioStockData>> mPortfolio;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(PortfolioData, mPortfolio);
