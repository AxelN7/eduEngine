#pragma once
#include <glm/glm.hpp>

struct Plane
{
	glm::vec3 normal;
	float distance;

	Plane() : normal(0.0f, 1.0f, 0.0f), distance(0.0f) {}
	Plane(const glm::vec3& n, float dist) : normal(glm::normalize(n)), distance(dist) {}

	float DistanceToPoint(const glm::vec3& point) const
	{
		return glm::dot(normal, point) - distance;
	}
};