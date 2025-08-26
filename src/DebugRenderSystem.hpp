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

void DebugRenderSystem(entt::registry& registry, ShapeRendererPtr shapeRenderer)
{
	auto view = registry.view<MeshComponent, MatrixComponent>();

	for (auto entity : view)
	{
		const auto& mesh = view.get<MeshComponent>(entity);
        const auto& matrix = view.get<MatrixComponent>(entity);

		if (auto meshPointer = mesh.reference.lock())
		{
			float axisLen = 1.0f;

            // Draw bone visualization
            if (mesh.drawSkeleton)                                                                                      //Refactored      SRP, move to a new system handling debug visualization to adhere to SRP
            {
                for (int i = 0; i < meshPointer->boneMatrices.size(); ++i)
                {
                    auto IBinverse = glm::inverse(meshPointer->m_bones[i].inversebind_tfm);

                    glm::mat4 global = matrix.worldMatrix * meshPointer->boneMatrices[i] * IBinverse;

                    glm::vec3 pos = glm::vec3(global[3]);
                    glm::vec3 right = glm::normalize(glm::vec3(global[0])); //X
                    glm::vec3 up = glm::normalize(glm::vec3(global[1]));    //Y
                    glm::vec3 fwd = glm::normalize(glm::vec3(global[2]));   //Z

                    shapeRenderer->push_states(ShapeRendering::Color4u::Red);
                    shapeRenderer->push_line(pos, pos + axisLen * right);

                    shapeRenderer->push_states(ShapeRendering::Color4u::Green);
                    shapeRenderer->push_line(pos, pos + axisLen * up);

                    shapeRenderer->push_states(ShapeRendering::Color4u::Blue);
                    shapeRenderer->push_line(pos, pos + axisLen * fwd);

                    shapeRenderer->pop_states<ShapeRendering::Color4u>();
                    shapeRenderer->pop_states<ShapeRendering::Color4u>();
                    shapeRenderer->pop_states<ShapeRendering::Color4u>();
                }
            }
		}
	}
}