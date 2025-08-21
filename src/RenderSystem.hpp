#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "TransformComponent.hpp"
#include "MeshComponent.hpp"
#include "ForwardRenderer.hpp"
#include "RenderableMesh.hpp"
#include "ShapeRenderer.hpp"

void RenderSystem(entt::registry& registry, eeng::ForwardRenderer& forwardRenderer, ShapeRendererPtr shapeRenderer)     //Refactor      SRP, move the debug visualization rendering to a seperate system so RenderSystem has only one reason to change
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

            float axisLen = 1.0f;

            // Draw bone visualization
            if (mesh.drawSkeleton)                                                                                      //Refactor      SRP, move to a new system handling debug visualization to adhere to SRP
            {
                for (int i = 0; i < meshPointer->boneMatrices.size(); ++i)
                {
                    auto IBinverse = glm::inverse(meshPointer->m_bones[i].inversebind_tfm);

                    glm::mat4 global = matrix * meshPointer->boneMatrices[i] * IBinverse;

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