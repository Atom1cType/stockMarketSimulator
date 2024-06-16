#pragma once

#include "BaseController.h"

class BaseAsyncController : public BaseController
{
public:
	virtual void Invoke() override;

	virtual void Process() {};
};
