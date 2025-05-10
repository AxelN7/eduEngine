#pragma once
#include <glm/glm.hpp>
#include <entt/entt.hpp>
#include "AnimationComponent.hpp"
#include "PlayerControllerComponent.hpp"

void AnimationLogicSystem(float dt, entt::registry& registry)
{
	auto view = registry.view<AnimationComponent, PlayerControllerComponent>();

	for (auto entity : view)
	{
		auto& animation = view.get<AnimationComponent>(entity);
		auto& playerController = view.get<PlayerControllerComponent>(entity);

		animation.idleTime += animation.animationSpeed * dt;
		animation.walkTime += animation.animationSpeed * dt;

		if (playerController.isMoving)
		{
			animation.blendTimer += dt;		// Blend up
		}
		else
		{
			animation.blendTimer -= dt;		// Blend down
		}

		animation.blendTimer = glm::clamp(animation.blendTimer, 0.0f, animation.blendDuration);
		animation.blendFactor = animation.blendTimer / animation.blendDuration;
	}
}