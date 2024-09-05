#include "../DataProviders/StockSimulatorDataProvider.h"
#include "../Subject/Subject.h"
#include "InputController.h"
#include "MainController.h"
#include "PortfolioController.h"
#include "StockSimulatorController.h"
#include <iostream>
#include <thread>

MainController& MainController::Instance()
{
	static MainController* instance = new MainController();

	return *instance;
}

void MainController::InvokeBackground(std::shared_ptr<BaseController> ControllerPtr)
{
	ControllerPtr->Invoke();
}

MainController::~MainController()
{
	Reset();
}

void MainController::Tick()
{
	BaseController::Tick();

	for (std::shared_ptr<BaseController>& controller : mControllers)
	{
		if (!IsEnabled() || !controller)
		{
			return;
		}

		controller->Tick();
	}
}

void MainController::Initialize()
{
	BaseController::Initialize();

	Subscribe();

	StockSimulatorDataProvider::Instance().Initialize();

	AddBackgroundControllerToProcess(std::make_shared<StockSimulatorController>());

	AddControllerToProcess(std::make_shared<InputController>());
	AddControllerToProcess(std::make_shared<PortfolioController>());

	for (std::shared_ptr<BaseController>& backgroundControllerPtr : mBackgroundControllers)
	{
		backgroundControllerPtr->Initialize();
	}

	for (std::shared_ptr<BaseController>& controllerPtr : mControllers)
	{
		controllerPtr->Initialize();
	}
}

bool MainController::Enable()
{
	bool isEnabled = BaseController::Enable();
	
	for (std::shared_ptr<BaseController>& backgroundControllerPtr : mBackgroundControllers)
	{
		isEnabled &= backgroundControllerPtr->Enable();

		std::thread thread(InvokeBackground, backgroundControllerPtr);
		thread.detach();
	}

	for (std::shared_ptr<BaseController>& controllerPtr : mControllers)
	{
		isEnabled &= controllerPtr->Enable();
	}

	return isEnabled;
}

bool MainController::Reset()
{
	for (std::shared_ptr<BaseController>& backgroundControllerPtr : mBackgroundControllers)
	{
		backgroundControllerPtr->Disable();
	}

	mBackgroundControllers.clear();
	mControllers.clear();

	Unsubscribe();

	return BaseController::Reset();
}

void MainController::OnNotify(NotifyEventType Event)
{
	switch (Event)
	{
		case NotifyEventType::Exit:
			OnNotifyExit();
			break;
		default:
			break;
	}
}

void MainController::Subscribe()
{
	GetSubject().AddObserver(this);
}

void MainController::Unsubscribe()
{
	GetSubject().RemoveObserver(this);
}

void MainController::AddBackgroundControllerToProcess(std::shared_ptr<BaseController> ControllerPtr)
{
	AddController(ControllerPtr, mBackgroundControllers);
}
void MainController::AddControllerToProcess(std::shared_ptr<BaseController> ControllerPtr)
{
	AddController(ControllerPtr, mControllers);
}

Subject& MainController::GetSubject()
{
	static Subject* subjectInstance = new Subject();

	return *subjectInstance;
}

void MainController::OnNotifyExit()
{
	Disable();
}

void MainController::AddController(std::shared_ptr<BaseController> controllerPtr, std::list<std::shared_ptr<BaseController>>& controllersList)
{
	if (!controllerPtr)
	{
		return;
	}

	controllersList.push_back(controllerPtr);
}
