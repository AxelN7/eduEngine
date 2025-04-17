#pragma once
#include <glm/glm.hpp>
#include <entt/entt.hpp>
#include "AnimationComponent.hpp"
#include "MeshComponent.hpp"
#include "LinearVelocityComponent.hpp"
#include "PlayerControllerComponent.hpp"
#include "RenderableMesh.hpp"

void AnimationSystem(float dt, entt::registry& registry)
{
	auto view = registry.view<AnimationComponent, MeshComponent, LinearVelocityComponent, PlayerControllerComponent>();

	for (auto entity : view)
	{
		auto& animation = view.get<AnimationComponent>(entity);
		const auto& mesh = view.get<MeshComponent>(entity);
		auto& velocity = view.get<LinearVelocityComponent>(entity);
		auto& player = view.get<PlayerControllerComponent>(entity);

		bool isMoveing = glm::length(glm::vec2(velocity.velocity.x, velocity.velocity.z)) > 0.01f;

		animation.animIndex = isMoveing ? 2 : 1;

		if (auto meshPtr = mesh.reference.lock())
		{
			meshPtr->animate(animation.animIndex, dt * animation.animSpeed);
		}
	}
}