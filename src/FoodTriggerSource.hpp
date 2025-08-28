#pragma once
#include "Source.hpp"
#include <entt/entt.hpp>

class FoodTriggerSource : public Source
{
public:
	FoodTriggerSource(entt::entity owner) : Source(owner) {}

	void PlayerEntered()
	{
		Notify(Event::FoundFood);
	}
};