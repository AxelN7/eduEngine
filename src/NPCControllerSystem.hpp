#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <cstdlib>
#include "NPCController.hpp"
#include "TransformComponent.hpp"
#include "LinearVelocityComponent.hpp"

void NPCControllerSystem(float dt, entt::registry& registry)
{
	const glm::vec3 directions[] = { {1, 0, 0}, {-1, 0, 0}, {0, 0, 1}, {0, 0, -1} };	//List of directions

	auto view = registry.view<NPCController, TransformComponent, LinearVelocityComponent>();

	for (auto entity : view)
	{
		auto& npcController = view.get<NPCController>(entity);
		auto& npcTransform = view.get<TransformComponent>(entity);
		auto& npcVelocity = view.get<LinearVelocityComponent>(entity);

		switch (npcController.behaviour)
		{
		case 0:		//NPC stays idle
		{
			npcController.movementSpeed = 0.0f;
			if (npcController.enabled) npcController.behaviour = 1;
		}
		case 1:		//NPC moves randomly
		{
			npcController.movementCooldown -= dt;

			if (npcController.movementCooldown <= 0.0f)
			{
				int randomIndex = std::rand() % 4;
				npcVelocity.velocity = directions[randomIndex] * npcController.movementSpeed;
				npcController.movementCooldown = 2.0f;
			}
		}
		break;
		case 2:		//NPC follows the player
		{
			auto playerView = registry.view<PlayerControllerComponent, TransformComponent>();

			for (auto playerEntity : playerView)
			{
				auto& playerController = playerView.get<PlayerControllerComponent>(playerEntity);
				auto& playerTransform = playerView.get<TransformComponent>(playerEntity);

				glm::vec3 playerDirection = playerTransform.position - npcTransform.position;	//Calculate the direction towards the player
				if (glm::length(playerDirection) > 0.1f)
				{
					playerDirection = glm::normalize(playerDirection);
					npcVelocity.velocity = playerDirection * npcController.movementSpeed;
				}
				else
				{
					npcVelocity.velocity = glm::vec3(0);	//Stop movement when close to the player
				}
				break;
			}
		}
		break;
		}
	}
}