#pragma once
#include "Source.hpp"
#include <entt/entt.hpp>
#include <string>
#include <iostream>

class FeedHorseSource : public Source
{
private:
	bool playerInside = false;
public:
	FeedHorseSource(entt::entity owner) : Source(owner) {}

	void PlayerEntered()
	{
		playerInside = true;
	}

	void PlayerExited()
	{
		playerInside = false;
	}

	void TryFeedHorse(entt::registry& registry, EventQueue& eventQueue)
	{
		if (!playerInside || pendingQuestComplete) return;

		pendingQuestComplete = true;

		std::string eventString = "Quest: Horse fed! Quest complete!";
		eventQueue.Enqueue(entity, eventString);
	}

	bool IsPlayerInside() const { return playerInside; }
};