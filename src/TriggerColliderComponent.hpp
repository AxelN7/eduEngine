#pragma once
#include "Sphere.hpp"
#include <string>

struct TriggerColliderComponent
{
	Sphere sphere;
	bool triggerActivated = false;
	std::string eventName;
};