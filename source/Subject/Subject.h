#pragma once

#include "../Data/NotifyEventsData.h"
#include <shared_mutex>

class Observer;

class Subject
{
public:
	Subject();
	virtual ~Subject() {};

	void AddObserver(Observer* observer);
	void RemoveObserver(Observer* observer);

	void Notify(NotifyEventType event);

private:
	Observer* mHead = nullptr;

	mutable std::shared_mutex mSharedMutex;
};
