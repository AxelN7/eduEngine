#pragma once
#include <glm/glm.hpp>
#include <glmcommon.hpp>
#include <entt/entt.hpp>
#include "AnimState.hpp"
#include "AnimationComponent.hpp"
#include "PlayerControllerComponent.hpp"
#include "InputManager.hpp"

void BasicFSM(float dt, InputManagerPtr input, entt::registry& registry)
{
	using Key = eeng::InputManager::Key;
	bool isMoving = input->IsKeyPressed(Key::W) || input->IsKeyPressed(Key::S) || input->IsKeyPressed(Key::A) || input->IsKeyPressed(Key::D);

	auto view = registry.view<AnimationComponent, PlayerControllerComponent>();

	for (auto entity : view)
	{
		auto& animation = view.get<AnimationComponent>(entity);
		auto& playerController = view.get<PlayerControllerComponent>(entity);

		animation.time0 += dt;
		animation.time1 += dt;

		if (input->IsKeyPressed(Key::Space) && animation.currentState != AnimState::Jump)	//Check if jump
		{
			animation.previousState = animation.currentState;	//Save current state
			animation.currentState = AnimState::Jump;
			animation.jumpTime = 0.0f;
		}

		if (animation.currentState == AnimState::Jump)
		{
			animation.jumpTime += dt;

			if (animation.jumpTime >= animation.jumpDuration)
			{
				animation.currentState = animation.previousState;
			}
		}
		else   //Handle blending between Idle and Walk
		{
			animation.targetState = isMoving ? AnimState::Walk : AnimState::Idle;

			if (animation.targetState == AnimState::Walk)
			{
				animation.blendTimer += dt;
			}
			else
			{
				animation.blendTimer -= dt;
			}

			animation.blendTimer = glm::clamp(animation.blendTimer, 0.0f, animation.blendDuration);
			animation.blendFactor = animation.blendTimer / animation.blendDuration;

			if (animation.blendFactor >= 1.0f - 0.01f)
			{
				animation.currentState = animation.targetState;
			}
			else if (animation.blendFactor <= 0.01f)
			{
				animation.currentState = animation.targetState;
			}
		}
	}
}