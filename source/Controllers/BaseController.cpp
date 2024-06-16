#include "BaseController.h"

BaseController::~BaseController()
{
	Reset();
}

void BaseController::Initialize()
{
	mIsInitialized = true;
}

void BaseController::Disable()
{
	mIsEnabled = false;
}

bool BaseController::Enable()
{
	if (!mIsInitialized)
	{
		return false;
	}

	mIsEnabled = true;

	return IsEnabled();
}

bool BaseController::Reset()
{
	if (!IsEnabled())
	{
		return false;
	}

	mIsInitialized = false;
	mIsEnabled = false;

	return true;
}

bool BaseController::IsEnabled() const
{
	return mIsEnabled;
}

BaseController::operator bool() const
{
	return IsEnabled();
}
