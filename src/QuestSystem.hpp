#pragma once
#include <entt/entt.hpp>
#include "EventQueue.hpp"
#include "QuestComponent.hpp"
#include "PlayerControllerComponent.hpp"

class QuestSystem
{
private:
	entt::registry& registry;
	EventQueue& eventQueue;
public:
	QuestSystem(entt::registry& reg, EventQueue& queue) : registry(reg), eventQueue(queue) {}

	void Update()
	{
		
	}
};
