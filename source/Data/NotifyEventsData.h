#pragma once

#include <string>

enum class NotifyEventType
{
	Undefined = -1,
	EnterCommand = 0,
	Exit = 1,
	Esc = 2,
	Switch = 3,
	New = 4,
	Add = 5,
	Remove = 6,
	Show = 7,
	Report = 8,
	SetActive = 9,
	Buy = 10,
	Sell = 11,
};
