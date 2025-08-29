#pragma once
#include "Observer.hpp"
#include "EventQueue.hpp"
#include "Event.hpp"
#include "QuestComponent.hpp"
#include <entt/entt.hpp>
#include <string>
#include "NPCController.hpp"

class QuestObserver : public Observer
{
private:
	entt::registry & registry;
	EventQueue& eventQueue;
public:
	QuestObserver(entt::registry& reg, EventQueue& queue) : registry(reg), eventQueue(queue) {}

	void QuestOnGoing(entt::entity source, Event event, QuestComponent& quest)			// Set new quest step and enqueue event
	{
		switch (event)
		{
		case Event::FoundFood:
			if (quest.currentStep == QuestStep::FindFood)
			{
				quest.currentStep = QuestStep::GoToHorse;
				std::string foundFoodEvent = "Quest: Food successfully collected! Go to the horse.";
				eventQueue.Enqueue(source, foundFoodEvent);
			}
			break;
		case Event::FoundHorse:
			if (quest.currentStep == QuestStep::GoToHorse)
			{
				quest.currentStep = QuestStep::FeedHorse;
				std::string foundHorseEvent = "Quest: Feed the horse by pressing F.";
				eventQueue.Enqueue(source, foundHorseEvent);
			}
			break;
		case Event::FedHorse:
			if (quest.currentStep == QuestStep::FeedHorse)
			{
				quest.currentStep = QuestStep::QuestComplete;
				std::string fedHorseEvent = "Quest: Horse fed! Quest complete!";
				eventQueue.Enqueue(source, fedHorseEvent);
			}
		}
	}

	void QuestComplete(QuestComponent& quest)											// Once the quest is completed make the horse react
	{
		switch (quest.currentStep)
		{
		case QuestStep::QuestComplete:
			if (quest.currentStep == QuestStep::QuestComplete)
			{
				auto horseView = registry.view<NPCController>();
				for (auto horse : horseView)
				{
					auto& horseNPC = horseView.get<NPCController>(horse);
					horseNPC.movementSpeed = 3.0f;
					horseNPC.enabled = true;
				}
			}
		}
	}

	void OnNotify(entt::entity source, Event event) override
	{
		auto view = registry.view<QuestComponent>();

		for (auto player : view)
		{
			auto& quest = view.get<QuestComponent>(player);

			QuestOnGoing(source, event, quest);
			QuestComplete(quest);
		}
	}
};
