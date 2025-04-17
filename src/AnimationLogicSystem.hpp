#pragma once
#include <glm/glm.hpp>
#include <entt/entt.hpp>
#include "AnimationComponent.hpp"
#include "PlayerControllerComponent.hpp"
#include "LinearVelocityComponent.hpp"

void AnimationLogicSystem(entt::registry& registry)
{
	auto view = registry.view<AnimationComponent, PlayerControllerComponent, LinearVelocityComponent>();

	for (auto entity : view)
	{
		auto& animation = view.get<AnimationComponent>(entity);
		auto& player = view.get<PlayerControllerComponent>(entity);
		auto& velocity = view.get<LinearVelocityComponent>(entity);

		
	}
}
