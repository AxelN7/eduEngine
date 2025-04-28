#pragma once
#include "PlayerControllerComponent.hpp"
#include "LinearVelocityComponent.hpp"
#include "InputManager.hpp"
#include <entt/entt.hpp>
#include <glm/glm.hpp>

void PlayerControllerSystem(InputManagerPtr input, entt::registry& registry, const glm::vec3& forwardVector, const glm::vec3& rightVector)
{
	using Key = eeng::InputManager::Key;
	bool W = input->IsKeyPressed(Key::W);
	bool A = input->IsKeyPressed(Key::A);
	bool S = input->IsKeyPressed(Key::S);
	bool D = input->IsKeyPressed(Key::D);

	glm::vec3 direction = forwardVector * ((W ? 1.0f : 0.0f) + (S ? -1.0f : 0.0f)) + rightVector * ((A ? -1.0f : 0.0f) + (D ? 1.0f : 0.0f));	//Player direction depending on keyboard input
	if (glm::length(direction) > 0.01f)
	{
		direction = glm::normalize(direction);	//Normalize direction for consistent movement speed
	}

	auto view = registry.view<PlayerControllerComponent, LinearVelocityComponent>();

	for (auto entity : view)
	{
		auto& playerController = view.get<PlayerControllerComponent>(entity);
		auto& linearVelocity = view.get<LinearVelocityComponent>(entity);

		playerController.isMoving = glm::length(direction) > 0.01f;	//Check if player is moving

		linearVelocity.velocity.x = direction.x * playerController.movementSpeed;
		linearVelocity.velocity.z = direction.z * playerController.movementSpeed;
	}
}