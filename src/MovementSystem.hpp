#pragma once
#include <glm/glm.hpp>
#include <entt/entt.hpp>
#include "TransformComponent.hpp"
#include "LinearVelocityComponent.hpp"

void MovementSystem(float dt, entt::registry& registry)
{
	auto view = registry.view<TransformComponent, LinearVelocityComponent>();

	for (auto entity : view)
	{
		auto& transform = view.get<TransformComponent>(entity);
		auto& linearVelocity = view.get<LinearVelocityComponent>(entity);

		transform.position += linearVelocity.velocity * dt;

		glm::vec3 velocityHorizontal = glm::vec3(linearVelocity.velocity.x, 0, linearVelocity.velocity.z);
		if (glm::length2(velocityHorizontal) > 0.0001f)
		{
			glm::vec3 forwardVec = glm::normalize(velocityHorizontal);
			glm::quat rotate = glm::rotation(glm::vec3(0, 0, 1), forwardVec);
			transform.rotation = glm::slerp(transform.rotation, rotate, 0.2f);
		}
	}
}