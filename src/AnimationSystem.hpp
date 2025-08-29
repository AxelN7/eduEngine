#pragma once
#include <glm/glm.hpp>
#include <entt/entt.hpp>
#include "AnimationComponent.hpp"
#include "MeshComponent.hpp"
#include <string>

void AnimationSystem(entt::registry& registry)
{
    auto view = registry.view<AnimationComponent, MeshComponent>();
    for (auto entity : view)
    {
        auto& animation = view.get<AnimationComponent>(entity);
        const auto& mesh = view.get<MeshComponent>(entity);

        if (auto meshPointer = mesh.reference.lock())
        {
            if (animation.currentState == AnimState::Jump)                                                                                                              // Animation blending between Idle/Walk and Jump
            {
                int prevAnimation = animation.previousState == AnimState::Walk ? animation.animations[2].clipIndex : animation.animations[1].clipIndex;                 // Set previous animation clip
                float prevTime = animation.previousState == AnimState::Walk ? animation.animations[2].animTime : animation.animations[1].animTime;                      // Set previous animation time

                meshPointer->animateBlend(prevAnimation, animation.animations[3].clipIndex, prevTime, animation.animations[3].animTime, animation.jumpBlendFactor);
            }
            else                                                                                                                                                        // Animation blending between Idle and Walk
            {
                meshPointer->animateBlend(animation.animations[1].clipIndex, animation.animations[2].clipIndex, animation.animations[1].animTime, animation.animations[2].animTime, animation.blendFactor);
            }
        }
    }
}