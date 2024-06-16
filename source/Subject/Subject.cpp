#include "../Observer/Observer.h"
#include "Subject.h"

Subject::Subject()
	: mHead(nullptr)
{

}

void Subject::AddObserver(Observer* observer)
{
	std::lock_guard lockGuard(mSharedMutex);

	observer->mNext = mHead;
	mHead = observer;
}

void Subject::RemoveObserver(Observer* observer)
{
	std::lock_guard lockGuard(mSharedMutex);

	if (mHead == observer)
	{
		mHead = observer->mNext;
		observer->mNext = nullptr;
		return;
	}

	Observer* current = mHead;

	while (current != nullptr)
	{
		if (current->mNext == observer)
		{
			current->mNext = observer->mNext;
			observer->mNext = nullptr;
			return;
		}

		current = current->mNext;
	}
}

void Subject::Notify(NotifyEventType event)
{
	std::shared_lock lockGuard(mSharedMutex);

	Observer* observer = mHead;

	while (observer != nullptr)
	{
		observer->OnNotify(event);
		observer = observer->mNext;
	}
}
