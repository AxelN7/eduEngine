#pragma once
#include "Observer.hpp"
#include "EventQueue.hpp"
#include "Event.hpp"
#include "QuestComponent.hpp"
#include <entt/entt.hpp>
#include <string>

class QuestObserver : public Observer
{
private:
	entt::registry & registry;
	EventQueue& eventQueue;
public:
	QuestObserver(entt::registry& reg, EventQueue& queue) : registry(reg), eventQueue(queue) {}

	void OnNotify(entt::entity source, Event event) override
	{
		auto view = registry.view<QuestComponent>();

		for (auto player : view)
		{
			auto& quest = view.get<QuestComponent>(player);

			switch (event)
			{
			case Event::FoundFood:
				if (quest.currentStep == QuestStep::FindFood)
				{
					quest.foodCollected = true;
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
					quest.horseFed = true;
					quest.currentStep = QuestStep::QuestComplete;
					std::string fedHorseEvent = "Quest: Horse fed! Quest complete!";
					eventQueue.Enqueue(source, fedHorseEvent);
				}
			}
		}
	}
};
