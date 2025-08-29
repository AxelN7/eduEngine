#pragma once

enum class QuestStep
{
	FindFood, GoToHorse, FeedHorse, QuestComplete
};

struct QuestComponent
{
	QuestStep currentStep = QuestStep::FindFood;
};