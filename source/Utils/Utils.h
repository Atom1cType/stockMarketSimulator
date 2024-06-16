#pragma once

#include "../Data/NotifyEventsData.h"
#include <string>

namespace Utils
{
	bool TryGetStringName(std::string& nameOut);

	bool TryGetIntAmount(int& amountOut);

	std::string StringToLower(std::string& string);

	NotifyEventType GetNotifyEventTypeByString(std::string& inputString);
}
