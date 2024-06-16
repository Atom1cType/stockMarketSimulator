#pragma once

#include "../Data/StockSimulatorData.h"
#include "../Observer/Observer.h"
#include "BaseController.h"
#include <unordered_map>

class PortfolioController : public BaseController, public Observer
{
public:
	~PortfolioController();

	virtual void Initialize() override;
	virtual bool Reset() override;

	virtual void OnNotify(NotifyEventType event) override;
	virtual void Subscribe() override;
	virtual void Unsubscribe() override;

private:
	void OnNotifyNew();
	void OnNotifyAdd();
	void OnNotifyRemove();
	void OnNotifyShow();
	void OnNotifyReport();
	void OnNotifySetActive();
};
