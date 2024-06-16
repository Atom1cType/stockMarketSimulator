#pragma once

#include "../Observer/Observer.h"
#include "BaseController.h"
#include <list>
#include <memory>

class Subject;

class MainController : public BaseController, public Observer
{
public:
	static MainController& Instance();
	static void InvokeBackground(std::shared_ptr<BaseController> controllerPtr);

	~MainController();

	virtual void Tick() override;
	virtual void Initialize() override;
	virtual bool Enable() override;
	virtual bool Reset() override;

	virtual void OnNotify(NotifyEventType Event) override;
	virtual void Subscribe() override;
	virtual void Unsubscribe() override;

	void AddBackgroundControllerToProcess(std::shared_ptr<BaseController> controllerPtr);
	void AddControllerToProcess(std::shared_ptr<BaseController> controllerPtr);

	Subject& GetSubject();

private:
	MainController() {};

	void OnNotifyExit();

	void AddController(std::shared_ptr<BaseController> controllerPtr, std::list<std::shared_ptr<BaseController>>& controllersList);

	std::list<std::shared_ptr<BaseController>> mBackgroundControllers;
	std::list<std::shared_ptr<BaseController>> mControllers;
};
