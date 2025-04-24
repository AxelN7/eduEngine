#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "TransformComponent.hpp"
#include "MeshComponent.hpp"
#include "ForwardRenderer.hpp"
#include "RenderableMesh.hpp"

void RenderSystem(entt::registry& registry, eeng::ForwardRenderer& forwardRenderer)
{
	auto view = registry.view<TransformComponent, MeshComponent>();

	for (auto entity : view)
	{
		const auto& transform = view.get<TransformComponent>(entity);
		const auto& mesh = view.get<MeshComponent>(entity);

		if (auto meshPointer = mesh.reference.lock())
		{
			glm::mat4 matrix =
				glm::translate(glm::mat4(1.0f), transform.position) *
				glm::toMat4(transform.rotation) *
				glm::scale(glm::mat4(1.0f), transform.scalingVector);
			forwardRenderer.renderMesh(meshPointer, matrix);
		}
	}
}