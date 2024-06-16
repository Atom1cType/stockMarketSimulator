#pragma once

#include "BaseController.h"
#include <string>

class InputController : public BaseController
{
public: 
	virtual void Tick() override;

private:
	void HandleInput();

	bool TryToNotify(std::string& inputString);
};
