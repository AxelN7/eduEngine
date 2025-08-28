#pragma once
#include <glm/glm.hpp>
#include <entt/entt.hpp>
#include "AnimationComponent.hpp"
#include "MeshComponent.hpp"
#include "FeedHorseSource.hpp"
#include "EventQueue.hpp"
#include <string>

void AnimationSystem(entt::registry& registry, EventQueue& eventQueue, float dt)
{
    auto view = registry.view<AnimationComponent, MeshComponent>();
    for (auto entity : view)
    {
        auto& animation = view.get<AnimationComponent>(entity);
        const auto& mesh = view.get<MeshComponent>(entity);

        if (auto meshPointer = mesh.reference.lock())
        {
            switch (animation.currentState)
            {
            case AnimState::Feed:
                meshPointer->animate(animation.animations[4].clipIndex, animation.stateTimer);
                animation.stateTimer -= dt;

                if (animation.stateTimer <= 0.0f)
                {
                    animation.currentState = AnimState::Idle;

                    auto horseView = registry.view<FeedHorseSource>();
                    for (auto horse : horseView)
                    {
                        auto& source = registry.get<FeedHorseSource>(horse);
                        if (source.pendingQuestComplete)
                        {
                            animation.currentState = AnimState::Jump;

                            if (registry.all_of<NPCController>(horse))
                            {
                                registry.get<NPCController>(horse).enabled = true;
                            }
                            std::string eventString = "Quest: Horse fed! Quest complete!";
                            eventQueue.Enqueue(horse, eventString);
                        }
                    }
                }
            }

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