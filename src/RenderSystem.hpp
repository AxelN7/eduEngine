#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "MatrixComponent.hpp"
#include "MeshComponent.hpp"
#include "ForwardRenderer.hpp"
#include "ShapeRenderer.hpp"

void RenderSystem(entt::registry& registry, eeng::ForwardRenderer& forwardRenderer, ShapeRendererPtr shapeRenderer)
{
	auto view = registry.view<MatrixComponent, MeshComponent>();

	for (auto entity : view)
	{
		const auto& matrix = view.get<MatrixComponent>(entity);
		const auto& mesh = view.get<MeshComponent>(entity);

		if (auto meshPointer = mesh.reference.lock())
		{
			forwardRenderer.renderMesh(meshPointer, matrix.worldMatrix);
		}
	}
}