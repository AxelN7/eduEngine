#pragma once
#include <glm/glm.hpp>
#include <entt/entt.hpp>
#include "AnimState.hpp"
#include "AnimationComponent.hpp"
#include "PlayerControllerComponent.hpp"

void BasicFSM(float dt, entt::registry& registry)
{
	auto view = registry.view<AnimationComponent, PlayerControllerComponent>();

	for (auto entity : view)
	{
		auto& animation = view.get<AnimationComponent>(entity);
		auto& playerController = view.get<PlayerControllerComponent>(entity);

		animation.time0 += dt;
		animation.time1 += dt;

		if (playerController.isMoving)	//Target state based on input
		{
			animation.targetState = AnimState::Walk;
		}
		else
		{
			animation.targetState = AnimState::Idle;
		}

		if (animation.currentState != animation.targetState)	//Blend if state change
		{
			animation.blendTimer += dt;

			if (animation.blendTimer >= animation.blendDuration)	//State transition when blending is complete
			{
				animation.blendTimer = 0.0f;
				animation.currentState = animation.targetState;
				animation.blendFactor = 0.0f;
			}
			else
			{
				animation.blendFactor = animation.blendTimer / animation.blendDuration;
			}
		}
		else
		{
			animation.blendFactor = 0.0f;	//No state change
		}
	}
}