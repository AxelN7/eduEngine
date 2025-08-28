#pragma once
#include "Source.hpp"
#include <entt/entt.hpp>

class HorseTriggerSource : public Source
{
public:
	HorseTriggerSource(entt::entity owner) : Source(owner) {}

	void PlayerEntered()
	{
		Notify(Event::FoundHorse);
	}
};