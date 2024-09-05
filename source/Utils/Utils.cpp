#include "Utils.h"
#include <algorithm>
#include <iostream>

bool Utils::TryGetStringName(std::string& nameOut)
{
	std::getline(std::cin, nameOut);
	std::cin.clear();

	if (nameOut.empty())
	{
		std::cout << "Invalid name\n";
		return false;
	}

	return true;
}

bool Utils::TryGetIntAmount(int& amountOut)
{
	int amount = -1;
	std::cin >> amount;
	std::cin.clear();

	if (amount <= 0 || amount >= static_cast<int>(INT_MAX * 0.5f))
	{
		std::cout << "Invalid amount\n";
		return false;
	}

	amountOut = amount;
	return true;
}

std::string Utils::StringToLower(std::string& string)
{
	std::transform(string.begin(), string.end(), string.begin(),
		[](unsigned char c) { return std::tolower(c); }
	);

	return string;
}

NotifyEventType Utils::GetNotifyEventTypeByString(std::string& inputString)
{
	if (inputString == "exit")
	{
		return NotifyEventType::Exit;
	}
	else if (inputString == "esc")
	{
		return NotifyEventType::Esc;
	}
	else if (inputString == "switch")
	{
		return NotifyEventType::Switch;
	}
	else if (inputString == "new")
	{
		return NotifyEventType::New;
	}
	else if (inputString == "add")
	{
		return NotifyEventType::Add;
	}
	else if (inputString == "remove")
	{
		return NotifyEventType::Remove;
	}
	else if (inputString == "show")
	{
		return NotifyEventType::Show;
	}
	else if (inputString == "report")
	{
		return NotifyEventType::Report;
	}
	else if (inputString == "setactive")
	{
		return NotifyEventType::SetActive;
	}
	else if (inputString == "buy")
	{
		return NotifyEventType::Buy;
	}
	else if (inputString == "sell")
	{
		return NotifyEventType::Sell;
	}
	else
	{
		return NotifyEventType::Undefined;
	}
}
