#pragma once
#include <glm/glm.hpp>
#include <glmcommon.hpp>
#include <entt/entt.hpp>
#include "AnimState.hpp"
#include "AnimationComponent.hpp"
#include "PlayerControllerComponent.hpp"
#include "InputManager.hpp"

void IdleWalkBlend(float dt, auto& animation, auto& playerController)
{
	animation.targetState = playerController.isMoving ? AnimState::Walk : AnimState::Idle;

	if (animation.targetState == AnimState::Walk)											// Blend up when transitioning to walk
	{
		animation.blendTimer += dt;
	}
	else																					// Blend down when transitioning to idle
	{
		animation.blendTimer -= dt;
	}

	animation.blendTimer = glm::clamp(animation.blendTimer, 0.0f, animation.blendDuration);
	animation.blendFactor = animation.blendTimer / animation.blendDuration;

	if (animation.blendFactor >= 1.0f - 0.01f || animation.blendFactor <= 0.01f)			// Update current state when target state reached
	{
		animation.currentState = animation.targetState;
	}
}

void JumpBlend(float dt, auto& animation)
{
	//animation.jumpTimer += animation.animationSpeed * dt;
	animation.animations[3].animTime += animation.animationSpeed * dt;

	if (!animation.jumpBlendOut)															// Blend up/in
	{
		animation.jumpBlendTimer += dt;
		animation.jumpBlendFactor = glm::clamp(animation.jumpBlendTimer / animation.blendDuration, 0.0f, 1.0f);

		if (animation.animations[3].animTime >= animation.jumpDuration * 0.5f)				// Start blend down/out halfway
		{
			animation.jumpBlendOut = true;
			animation.jumpBlendTimer = animation.blendDuration;
		}
	}
	else																					// Blend down/out
	{
		animation.jumpBlendTimer -= dt;
		animation.jumpBlendFactor = glm::clamp(animation.jumpBlendTimer / animation.blendDuration, 0.0f, 1.0f);

		if (animation.jumpBlendFactor <= 0.0f)												// Go back to previous state
		{
			animation.currentState = animation.previousState;
		}
	}
}

void SetJump(auto& animation)
{
	animation.previousState = animation.currentState;										// Save current state in previous state
	animation.currentState = AnimState::Jump;												// Update current state
	//animation.jumpTimer = 0.0f;
	animation.animations[3].animTime = 0.0f;
	animation.jumpBlendTimer = 0.0f;
	animation.jumpBlendFactor = 0.0f;
	animation.jumpBlendOut = false;
}

void FSM(float dt, InputManagerPtr input, entt::registry& registry)			//Refactor		Long method, shorten it by adding three submethods containing the IdleWalkBlend logic, JumpBlend logic and SetJump logic. Also uses the vector of animationclips instead of primitive variables
{
	using Key = eeng::InputManager::Key;

	auto view = registry.view<AnimationComponent, PlayerControllerComponent>();

	for (auto entity : view)
	{
		auto& animation = view.get<AnimationComponent>(entity);
		auto& playerController = view.get<PlayerControllerComponent>(entity);

		//animation.idleTime += animation.animationSpeed * dt;
		//animation.walkTime += animation.animationSpeed * dt;
		animation.animations[1].animTime += animation.animationSpeed * dt;
		animation.animations[2].animTime += animation.animationSpeed * dt;

		if (input->IsKeyPressed(Key::Space) && animation.currentState != AnimState::Jump)	// Check if jump
		{
			SetJump(animation);
		}

		if (animation.currentState == AnimState::Jump)										// Handle blending between Idle/Walk and Jump
		{
			JumpBlend(dt, animation);
		}
		else																				// Handle blending between Idle and Walk
		{
			IdleWalkBlend(dt, animation, playerController);
		}
	}
}