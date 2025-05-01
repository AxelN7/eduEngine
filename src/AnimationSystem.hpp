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

        int idleAnimation = 1;
        int walkAnimation = 2;
        int jumpAnimation = 3;

        if (auto meshPointer = mesh.reference.lock())
        {
            if (animation.currentState == AnimState::Jump)
            {
                int prevAnimation = animation.previousState == AnimState::Walk ? walkAnimation : idleAnimation;     //Set previous animation clip
                float prevTime = animation.previousState == AnimState::Walk ? animation.walkTime : animation.idleTime;  //Set previous animation time

                meshPointer->animateBlend(prevAnimation, jumpAnimation, prevTime, animation.jumpTimer, animation.jumpBlendFactor);    //Animation blending between Idle/Walk and Jump
            }
            else
            {
                meshPointer->animateBlend(idleAnimation, walkAnimation, animation.idleTime, animation.walkTime, animation.blendFactor);     //Animation blending between Idle and Walk
            }
        }
    }
}