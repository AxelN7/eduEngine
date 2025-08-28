#pragma once
#include <entt/entt.hpp>
#include "FeedHorseSource.hpp"
#include "InputManager.hpp"

void QuestInputSystem(entt::registry& registry, InputManagerPtr input, EventQueue& eventQueue)
{
	using Key = eeng::InputManager::Key;

	if (input->IsKeyPressed(Key::F))
	{
		auto view = registry.view<FeedHorseSource>();
		for (auto entity : view)
		{
			auto& source = view.get<FeedHorseSource>(entity);
			if (source.IsPlayerInside())
			{
				source.TryFeedHorse(registry, eventQueue);
			}
		}
	}
}