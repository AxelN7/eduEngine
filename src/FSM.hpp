#pragma once
#include <glm/glm.hpp>
#include <glmcommon.hpp>
#include <entt/entt.hpp>
#include "AnimState.hpp"
#include "AnimationComponent.hpp"
#include "PlayerControllerComponent.hpp"
#include "InputManager.hpp"

void FSM(float dt, InputManagerPtr input, entt::registry& registry)
{
	using Key = eeng::InputManager::Key;

	auto view = registry.view<AnimationComponent, PlayerControllerComponent>();

	for (auto entity : view)
	{
		auto& animation = view.get<AnimationComponent>(entity);
		auto& playerController = view.get<PlayerControllerComponent>(entity);

		animation.idleTime += animation.animationSpeed * dt;
		animation.walkTime += animation.animationSpeed * dt;

		if (input->IsKeyPressed(Key::Space) && animation.currentState != AnimState::Jump)	//Check if jump
		{
			animation.previousState = animation.currentState;	//Save current state in previous state
			animation.currentState = AnimState::Jump;	//Update current state
			animation.jumpTimer = 0.0f;
			animation.jumpBlendTimer = 0.0f;
			animation.jumpBlendFactor = 0.0f;
			animation.jumpBlendOut = false;
		}

		if (animation.currentState == AnimState::Jump)
		{
			animation.jumpTimer += animation.animationSpeed * dt;

			if (!animation.jumpBlendOut)	//Blend up/in
			{
				animation.jumpBlendTimer += dt;
				animation.jumpBlendFactor = glm::clamp(animation.jumpBlendTimer / animation.blendDuration, 0.0f, 1.0f);

				if (animation.jumpTimer >= animation.jumpDuration * 0.5f)	//Start blend down/out halfway
				{
					animation.jumpBlendOut = true;
					animation.jumpBlendTimer = animation.blendDuration;
				}
			}
			else   //Blend down/out
			{
				animation.jumpBlendTimer -= dt;
				animation.jumpBlendFactor = glm::clamp(animation.jumpBlendTimer / animation.blendDuration, 0.0f, 1.0f);

				if (animation.jumpBlendFactor <= 0.0f)	//Go back to previous state
				{
					animation.currentState = animation.previousState;
				}
			}
		}
		else   //Handle blending between Idle and Walk
		{
			animation.targetState = playerController.isMoving ? AnimState::Walk : AnimState::Idle;

			if (animation.targetState == AnimState::Walk)	//Blend up when transitioning to walk
			{
				animation.blendTimer += dt;
			}
			else   //Blend down when transitioning to idle
			{
				animation.blendTimer -= dt;
			}

			animation.blendTimer = glm::clamp(animation.blendTimer, 0.0f, animation.blendDuration);
			animation.blendFactor = animation.blendTimer / animation.blendDuration;

			if (animation.blendFactor >= 1.0f - 0.01f || animation.blendFactor <= 0.01f)	//Update current state when target state reached
			{
				animation.currentState = animation.targetState;
			}
		}
	}
}