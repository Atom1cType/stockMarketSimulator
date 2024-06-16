#pragma once

#include <atomic>

class BaseController
{
public:
	virtual ~BaseController();

	virtual void Tick() {};
	virtual void Invoke() {};
	virtual void Initialize();
	virtual void Disable();

	virtual bool Enable();
	virtual bool Reset();

	virtual bool IsEnabled() const;

	virtual operator bool() const;

private:
	std::atomic_bool mIsInitialized = false;
	std::atomic_bool mIsEnabled = false;
};

