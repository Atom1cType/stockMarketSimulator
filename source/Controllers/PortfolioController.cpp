#include "../DataProviders/StockSimulatorDataProvider.h"
#include "../Subject/Subject.h"
#include "../Utils/Utils.h"
#include "MainController.h"
#include "PortfolioController.h"
#include <iostream>

PortfolioController::~PortfolioController()
{
	Reset();
}

void PortfolioController::Initialize()
{
	BaseController::Initialize();

	Subscribe();
}

bool PortfolioController::Reset()
{
	Unsubscribe();

	return BaseController::Reset();
}

void PortfolioController::OnNotify(NotifyEventType event)
{
	switch (event)
	{
		case NotifyEventType::New:
			OnNotifyNew();
			break;
		case NotifyEventType::Add:
			OnNotifyAdd();
			break;
		case NotifyEventType::Remove:
			OnNotifyRemove();
			break;
		case NotifyEventType::Show:
			OnNotifyShow();
			break;
		case NotifyEventType::Report:
			OnNotifyReport();
			break;
		case NotifyEventType::SetActive:
			OnNotifySetActive();
			break;

		default:
			break;
	}
}

void PortfolioController::Subscribe()
{
	Subject& subject = MainController::Instance().GetSubject();
	subject.AddObserver(this);
}

void PortfolioController::Unsubscribe()
{
	Subject& subject = MainController::Instance().GetSubject();
	subject.RemoveObserver(this);
}

void PortfolioController::OnNotifyNew()
{
	std::cout << "`new` command called\n";
	std::cout << "Enter portfolio name to create: ";

	std::string inputString;

	if (!Utils::TryGetStringName(inputString))
	{
		return;
	}

	StockSimulatorDataProvider::Instance().AddNewPortfolio(inputString);
	StockSimulatorDataProvider::Instance().SetActivePortfolio(inputString);
}

void PortfolioController::OnNotifyAdd()
{
	std::cout << "`add` command called" << std::endl;

	std::string activePortfolioName = StockSimulatorDataProvider::Instance().GetActivePortfolioName();

	if (!StockSimulatorDataProvider::Instance().IsPortfolioExists(activePortfolioName))
	{
		return;
	}

	std::cout << "Enter stock name to add: ";

	std::string inputString;

	if (!Utils::TryGetStringName(inputString))
	{
		return;
	}

	std::cout << "Enter stock amount to add: ";

	int amount = -1;

	if (!Utils::TryGetIntAmount(amount))
	{
		return;
	}

	StockSimulatorDataProvider::Instance().AddStocksToPortfolio(activePortfolioName, inputString, amount);
}

void PortfolioController::OnNotifyRemove()
{
	std::cout << "`remove` command called" << std::endl;

	std::string activePortfolioName = StockSimulatorDataProvider::Instance().GetActivePortfolioName();

	if (!StockSimulatorDataProvider::Instance().IsPortfolioExists(activePortfolioName))
	{
		return;
	}

	std::cout << "Enter stock name to remove: ";

	std::string inputString;

	if (!Utils::TryGetStringName(inputString))
	{
		return;
	}

	std::cout << "Enter stock amount to remove: ";

	int amount = -1;

	if (!Utils::TryGetIntAmount(amount))
	{
		return;
	}

	StockSimulatorDataProvider::Instance().RemoveStocksFromPortfolio(activePortfolioName, inputString, amount);
}

void PortfolioController::OnNotifyShow()
{
	std::cout << "`show` command called\n" << std::endl;

	StockSimulatorDataProvider::Instance().DumpActivePortfolioStatus();
}

void PortfolioController::OnNotifyReport() 
{
	std::cout << "`report` command called\n";

	StockSimulatorDataProvider::Instance().DumpActivePortfolioReport();
}

void PortfolioController::OnNotifySetActive()
{
	std::cout << "`setActive` command called\n";
	std::cout << "Enter portfolio name to set active: ";

	std::string inputString;

	if (!Utils::TryGetStringName(inputString))
	{
		return;
	}

	if (StockSimulatorDataProvider::Instance().IsPortfolioExists(inputString))
	{
		StockSimulatorDataProvider::Instance().SetActivePortfolio(inputString);
	}
}
