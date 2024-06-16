#pragma once

#include "../Data/NotifyEventsData.h"
#include <memory>

class Observer
{
	friend class Subject;

public:
	Observer();
	virtual ~Observer() {};
	virtual void OnNotify(NotifyEventType event) = 0;

	virtual void Subscribe() = 0;
	virtual void Unsubscribe() = 0;

private:
	Observer* mNext;
};
