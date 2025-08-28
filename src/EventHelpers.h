#pragma once
#include <string>
#include <stdexcept>
#include "QuestObserver.hpp"

inline Event StringToEvent(const std::string& string)
{
	if (string == "Quest: Food successfully collected! Go to the horse.") return Event::FoundFood;
	if (string == "Quest: Feed the horse by pressing F.") return Event::FoundFood;
	if (string == "Quest: Horse fed! Quest complete!") return Event::FedHorse;

	throw std::runtime_error("Unknown event string: " + string);
}
