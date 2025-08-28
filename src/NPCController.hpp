#pragma once
#include <glm/glm.hpp>

struct NPCController
{
	float movementSpeed = 0.0f;
	float movementCooldown = 0.0f;
	int behaviour = 0;
	bool enabled = false;
};