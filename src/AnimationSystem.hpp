#pragma once
#include <glm/glm.hpp>
#include <entt/entt.hpp>
#include "AnimationComponent.hpp"
#include "PlayerControllerComponent.hpp"

void AnimationSystem(float dt, entt::registry& registry)
{
	auto view = registry.view<AnimationComponent, PlayerControllerComponent>();

	for (auto entity : view)
	{
		auto& animation = view.get<AnimationComponent>(entity);
		auto& playerController = view.get<PlayerControllerComponent>(entity);

		animation.time0 += dt;
		animation.time1 += dt;

		if (playerController.isMoving)
		{
			animation.blendTimer += dt;
		}
		else
		{
			animation.blendTimer -= dt;
		}

		animation.blendTimer = glm::clamp(animation.blendTimer, 0.0f, animation.blendDuration);
		animation.blendFactor = animation.blendTimer / animation.blendDuration;
	}
}