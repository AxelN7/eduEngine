#pragma once
#include <entt/entt.hpp>
#include "Observer.hpp"

class Source
{
private:
	Observer* observers[256];
	int numberOfObservers = 0;
public:
	bool pendingQuestComplete = false;
	Source(entt::entity owner) : entity(owner) {}

	void AddObserver(Observer* observer)
	{
		observers[numberOfObservers++] = observer;
	}

	void RemoveObserver(Observer* observer)
	{
		for (int i = 0; i < numberOfObservers; ++i)
		{
			if (observers[i] == observer)
			{
				observers[i] = observers[--numberOfObservers];
			}
		}
	}
protected:
	entt::entity entity;											// Entity the source belongs to
	void Notify(Event event)
	{
		for (int i = 0; i < numberOfObservers; ++i)
		{
			observers[i]->OnNotify(entity, event);
		}
	}
};
