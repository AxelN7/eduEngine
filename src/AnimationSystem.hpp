#pragma once
#include <glm/glm.hpp>
#include <entt/entt.hpp>
#include "AnimationComponent.hpp"
#include "MeshComponent.hpp"

void AnimationSystem(entt::registry& registry)
{
    auto view = registry.view<AnimationComponent, MeshComponent>();
    for (auto entity : view)
    {
        auto& animation = view.get<AnimationComponent>(entity);
        const auto& mesh = view.get<MeshComponent>(entity);

        int idleAnimation = 1;      // Clip index for Idle
        int walkAnimation = 2;      // Clip index for Walk
        int jumpAnimation = 3;      // Clip index for Jump

        if (auto meshPointer = mesh.reference.lock())
        {
            if (animation.currentState == AnimState::Jump)          // Animation blending between Idle/Walk and Jump
            {
                int prevAnimation = animation.previousState == AnimState::Walk ? walkAnimation : idleAnimation;         // Set previous animation clip
                float prevTime = animation.previousState == AnimState::Walk ? animation.walkTime : animation.idleTime;  // Set previous animation time

                meshPointer->animateBlend(prevAnimation, jumpAnimation, prevTime, animation.jumpTimer, animation.jumpBlendFactor);
            }
            else                                                    // Animation blending between Idle and Walk
            {
                meshPointer->animateBlend(idleAnimation, walkAnimation, animation.idleTime, animation.walkTime, animation.blendFactor);
            }
        }
    }
}