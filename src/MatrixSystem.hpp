#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "TransformComponent.hpp"
#include "MatrixComponent.hpp"
#include "ForwardRenderer.hpp"
#include "RenderableMesh.hpp"
#include "ShapeRenderer.hpp"

void MatrixSystem(entt::registry& registry)
{
	auto view = registry.view<TransformComponent, MatrixComponent>();

	for (auto entity : view)
	{
		const auto& transform = view.get<TransformComponent>(entity);
		auto& matrix = view.get<MatrixComponent>(entity);

		matrix.worldMatrix = glm::translate(glm::mat4(1.0f), transform.position) *
						glm::toMat4(transform.rotation) *
						glm::scale(glm::mat4(1.0f), transform.scalingVector);
	}
}
