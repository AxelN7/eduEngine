#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "MatrixComponent.hpp"
#include "MeshComponent.hpp"
#include "ForwardRenderer.hpp"
#include "RenderableMesh.hpp"
#include "ShapeRenderer.hpp"

void RenderSystem(entt::registry& registry, eeng::ForwardRenderer& forwardRenderer, ShapeRendererPtr shapeRenderer)     //Refactored      SRP, move the debug visualization rendering to a seperate system so RenderSystem has only one reason to change
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