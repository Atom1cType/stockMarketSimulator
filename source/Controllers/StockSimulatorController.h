#include "../Data/MarketRequestData.h"
#include "../Data/StockSimulatorData.h"
#include "../Observer/Observer.h"
#include "BaseAsyncController.h"
#include <chrono>
#include <optional>
#include <shared_mutex>

class BaseMarketRequest;
struct StockSimulatorData;

class StockSimulatorController : public BaseAsyncController, public Observer
{
public:
	~StockSimulatorController();

	virtual void Initialize() override;
	virtual bool Reset() override;

	virtual void Process() override;

	virtual void OnNotify(NotifyEventType event) override;
	virtual void Subscribe() override;
	virtual void Unsubscribe() override;

private:
	void TryUpdatePrices(std::chrono::system_clock::time_point now);
	void PrintStockData(std::chrono::system_clock::time_point now);
	void ProcessMarketRequests();
	std::optional<MarketRequestData> CreateMarketRequestData();

	void OnNotifySwitch();
	void OnNotifyBuy();
	void OnNotifySell();

	StockSimulatorSettings mStockSimulatorSettings;

	std::chrono::system_clock::time_point mLastStockPrintTimestamp;
	std::chrono::system_clock::time_point mInitTimestamp;
	std::chrono::system_clock::time_point mLastDataUpdateTimestamp;

	std::atomic_bool mCanPrintStockData = true;

	std::vector<std::shared_ptr<BaseMarketRequest>> mMarketRequests;

	mutable std::shared_mutex mMarketRequestsMutex;
};
