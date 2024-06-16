#include "BaseAsyncController.h"
#include <iostream>

void BaseAsyncController::Invoke()
{
	BaseController::Invoke();

	while (IsEnabled())
	{
		Process();
	}
}
