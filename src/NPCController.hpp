#pragma once
#include <glm/glm.hpp>

struct NPCController
{
	float movementSpeed = 3.0f;
	float movementCooldown = 0.0f;
	int behaviour = 0;
};