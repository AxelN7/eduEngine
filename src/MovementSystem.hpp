#pragma once
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
	}
}