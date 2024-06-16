#include "../DataProviders/StockSimulatorDataProvider.h"
#include "../Facades/StrategyFacade.h"
#include "../MarketRequests/BuyMarketRequest.h"
#include "../MarketRequests/SellMarketRequest.h"
#include "../Subject/Subject.h"
#include "../Utils/Utils.h"
#include "MainController.h"
#include "StockSimulatorController.h"
#include <iostream>

namespace
{
	const float PriceUpdateIntervalSec = 2.0f;
}

StockSimulatorController::~StockSimulatorController()
{
	Reset();
}

void StockSimulatorController::Initialize()
{
	BaseAsyncController::Initialize();

	StockSimulatorDataProvider& stockSimulatorDataProvider = StockSimulatorDataProvider::Instance();
	mStockSimulatorSettings = stockSimulatorDataProvider.GetStockSimulatorData().mStockSimulatorSettings;

	mInitTimestamp = std::chrono::system_clock::now();
	
	Subscribe();
}

bool StockSimulatorController::Reset()
{
	Unsubscribe();

	return BaseAsyncController::Reset();
}

void StockSimulatorController::Process()
{
	BaseAsyncController::Process();

	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

	TryUpdatePrices(now);

	ProcessMarketRequests();

	PrintStockData(now);
}

void StockSimulatorController::OnNotify(NotifyEventType event)
{
	switch (event)
	{
		case NotifyEventType::EnterCommand:
			mCanPrintStockData = false;
			break;
		case NotifyEventType::Switch:
			OnNotifySwitch();
			break;
		case NotifyEventType::Buy:
			OnNotifyBuy();
			mCanPrintStockData = true;
			break;
		case NotifyEventType::Sell:
			OnNotifySell();
			mCanPrintStockData = true;
			break;
		default:
			mCanPrintStockData = true;
			break;
	}
}

void StockSimulatorController::Subscribe()
{
	Subject& subject = MainController::Instance().GetSubject();
	subject.AddObserver(this);
}

void StockSimulatorController::Unsubscribe()
{
	Subject& subject = MainController::Instance().GetSubject();
	subject.RemoveObserver(this);
}

void StockSimulatorController::TryUpdatePrices(std::chrono::system_clock::time_point now)
{
	std::chrono::duration<float> durationWithoutUpdate = now - mLastDataUpdateTimestamp;

	if (durationWithoutUpdate.count() < PriceUpdateIntervalSec)
	{
		return;
	}

	mLastDataUpdateTimestamp = now;

	std::chrono::duration<float> durationFromInit = now - mInitTimestamp;

	StrategyFacade& strategyFacade = StrategyFacade::Instance();

	auto ApplyStrategy = [&strategyFacade, time = durationFromInit.count()](StockData& stockData) -> void
		{
			stockData.mPrice = strategyFacade.ApplyStrategy(stockData.mPrice, time);
		};

	StockSimulatorDataProvider::Instance().VisitAllStockData(ApplyStrategy);
}

void StockSimulatorController::PrintStockData(std::chrono::system_clock::time_point now)
{
	if (!mCanPrintStockData)
	{
		return;
	}

	std::chrono::duration<float> durationWithoutUpdate = now - mLastStockPrintTimestamp;

	if (durationWithoutUpdate.count() < mStockSimulatorSettings.mScreenUpdateIntervalSec)
	{
		return;
	}

	mLastStockPrintTimestamp = now;

	auto PrintStockData = [](StockData& stockData) -> void
		{
			std::cout << "Stock Name: " << stockData.mName << " Stock Price: " << stockData.mPrice << "\t";
		};

	StockSimulatorDataProvider::Instance().VisitAllStockData(PrintStockData);

	std::cout << std::endl;
}

void StockSimulatorController::ProcessMarketRequests()
{
	{
		std::shared_lock lockGuard(mMarketRequestsMutex);

		for (std::shared_ptr<BaseMarketRequest>& marketRequest : mMarketRequests)
		{
			marketRequest->Validate();

			auto TryApplyRequest = [&marketRequest](StockData& stockData)
				{
					marketRequest->TryApply(stockData);
				};

			StockSimulatorDataProvider::Instance().VisitAllStockData(TryApplyRequest);

			marketRequest->Consume();
		}
	}

	{
		std::lock_guard lockGuard(mMarketRequestsMutex);

		std::erase_if(mMarketRequests, [](std::shared_ptr<BaseMarketRequest>& marketRequest)
			{
				return marketRequest->CanBeRemoved();
			}
		);
	}
}

std::optional<MarketRequestData> StockSimulatorController::CreateMarketRequestData()
{
	std::cout << "Enter stock name: ";
	std::string stockName;
	if (!Utils::TryGetStringName(stockName))
	{
		return std::nullopt;
	}

	std::cout << "Enter amount: ";
	int amount = -1;
	if (!Utils::TryGetIntAmount(amount))
	{
		return std::nullopt;
	}

	std::cout << "Enter price: ";
	float price = -1.0f;
	std::cin >> price;

	if (price <= 0.0f)
	{
		std::cout << "Price is invalid\n";
		return std::nullopt;
	}

	std::string portfolioName = StockSimulatorDataProvider::Instance().GetActivePortfolioName();

	MarketRequestData marketRequestData{
		.mAmount = amount
		, .mPortfolioName = portfolioName
		, .mStockData = {
			.mName = stockName
			, .mPrice = price
		}
	};

	return marketRequestData;
}

void StockSimulatorController::OnNotifySwitch()
{
	std::cout << "`switch` command called\n";
	std::cout << "Enter type 0 - random, 1 - trend: ";
	int type = -1;
	std::cin >> type;

	SimulatorStrategyType strategyType = static_cast<SimulatorStrategyType>(type);

	StrategyFacade::Instance().SetStrategy(strategyType);

	mCanPrintStockData = true;
}

void StockSimulatorController::OnNotifyBuy()
{
	std::cout << "`buy` command called\n";

	std::optional<MarketRequestData> marketRequestDataOpt = CreateMarketRequestData();

	if (!marketRequestDataOpt)
	{
		return;
	}

	{
		std::lock_guard lockGuard(mMarketRequestsMutex);

		mMarketRequests.push_back(std::make_shared<BuyMarketRequest>(marketRequestDataOpt.value()));
	}
}

void StockSimulatorController::OnNotifySell()
{
	std::cout << "`sell` command called\n";

	std::optional<MarketRequestData> marketRequestDataOpt = CreateMarketRequestData();

	if (!marketRequestDataOpt)
	{
		return;
	}

	{
		std::lock_guard lockGuard(mMarketRequestsMutex);

		mMarketRequests.push_back(std::make_shared<SellMarketRequest>(marketRequestDataOpt.value()));
	}
}
