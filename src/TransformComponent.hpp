#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct TransformComponent
{
	glm::vec3 position;
	glm::vec3 scalingVector;
	glm::quat rotation;
};