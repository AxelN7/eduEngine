#pragma once
#include <entt/entt.hpp>
#include "EventQueue.hpp"
#include <string>
#include <iostream>
#include <vector>
#include "TransformComponent.hpp"
#include "PlayerControllerComponent.hpp"
#include "NPCController.hpp"
#include "AnimationComponent.hpp"
#include "AnimState.hpp"
#include "MeshComponent.hpp"
#include <imgui.h>
#include "ShapeRenderer.hpp"
#include "CollisionComponent.hpp"


class GUISystem
{
private:
	entt::registry& registry;
	EventQueue& eventQueue;

	std::vector<std::string> debugMessages;
	bool showBoneGizmo = false;
	bool showColliders = true;

	void RenderPlayerControls()
	{
		auto view = registry.view<TransformComponent, PlayerControllerComponent>();
		for (auto entity : view)
		{
			auto& playerTransform = view.get<TransformComponent>(entity);
			auto& playerController = view.get<PlayerControllerComponent>(entity);

			float playerScale = playerTransform.scalingVector.x;
			if (ImGui::SliderFloat("Player scale", &playerScale, 0.01f, 1.0f))                              // Adjust player scale
			{
				playerTransform.scalingVector = glm::vec3(playerScale);
			}

			ImGui::SliderFloat("Player speed", &playerController.movementSpeed, 0.1f, 50.0f);               // Adjust player movement speed
			break;
		}
	}

	void RenderNPCControls()
	{
		auto view = registry.view<NPCController>();
		for (auto entity : view)
		{
			auto& npcController = view.get<NPCController>(entity);

			const char* behaviours[] = { "Stay idle", "Random", "Follow player"};
			ImGui::Combo("NPC behaviour", &npcController.behaviour, behaviours, IM_ARRAYSIZE(behaviours));  // Adjust npc behaviour
		}
	}

	void RenderAnimationControls()
	{
		auto view = registry.view<AnimationComponent>();
		for (auto entity : view)
		{
			auto& animation = view.get<AnimationComponent>(entity);

			ImGui::SliderFloat("Blend factor", &animation.blendFactor, 0.0f, 1.0f);                         // Control blend factor in animation blend
			ImGui::SliderFloat("Jump Blend factor", &animation.jumpBlendFactor, 0.0f, 1.0f);                // Control jump blend factor in animation blend

			const char* stateText[] = { "Current animation state: Idle", "Current animation state: Walk", "Current animation state: Jump" };
			if (animation.currentState == AnimState::Jump)
			{
				ImGui::Text(stateText[2]);                      // Show Jump state
			}
			if (animation.currentState == AnimState::Walk)
			{
				ImGui::Text(stateText[1]);                      // Show Walk state
			}
			if (animation.currentState == AnimState::Idle)
			{
				ImGui::Text(stateText[0]);                      // Show Idle state
			}
		}
	}

	void RenderDebugOverlays()
	{
		auto view = registry.view<MeshComponent, PlayerControllerComponent>();
		for (auto entity : view)
		{
			auto& mesh = view.get<MeshComponent>(entity);

			if (auto meshPointer = mesh.reference.lock())
			{
				if (ImGui::Button("Bone gizmo ON"))             // Toggle bone visualization
				{
					mesh.drawSkeleton = true;
				}
				if (ImGui::Button("Bone gizmo OFF"))
				{
					mesh.drawSkeleton = false;
				}
			}

			if (ImGui::Button("Show Colliders and Triggers ON"))
			{
				showColliders = true;
			}
			if (ImGui::Button("Show Colliders and Triggers OFF"))
			{
				showColliders = false;
			}
		}
	}

	void ProcessEventQueue()
	{
		const auto& events = eventQueue.getQueue();
		const auto numEvents = eventQueue.getNumberOfEventsInQueue();

		for (int i = 0; i < numEvents; ++i)
		{
			debugMessages.push_back(events[i].eventString);
		}

		eventQueue.BroadcastAllEvents();
	}

	void DrawAABB(ShapeRendererPtr shapeRenderer)
	{
		if (showColliders)
		{
			auto view = registry.view<CollisionComponent>();
			for (auto entity : view)
			{
				auto& collider = view.get<CollisionComponent>(entity);

				glm::vec3 min = collider.boundingBox.center - glm::make_vec3(collider.boundingBox.halfWidths);
				glm::vec3 max = collider.boundingBox.center + glm::make_vec3(collider.boundingBox.halfWidths);

				ShapeRendering::Color4u color = collider.isTrigger
					? ShapeRendering::Color4u{ 0xFF0000FF }			// Red color for triggers
				: ShapeRendering::Color4u{ 0xFF00FF00 };		// Green color for colliders

				shapeRenderer->push_states(color);
				shapeRenderer->push_AABB(min, max);
				shapeRenderer->pop_states<ShapeRendering::Color4u>();
			}
		}
	}

	void DrawDebugMessages()
	{
		ImGui::Begin("Quest messages");

		if (ImGui::Button("Clear")) debugMessages.clear();

		ImGui::BeginChild("ScrollingRegion", ImVec2(0, 200), true);
		ImGui::Text("Quest: Find the food!");
		for (const auto& message : debugMessages)
		{
			ImGui::TextWrapped("%s", message.c_str());
		}

		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) ImGui::SetScrollHereY(1.0f);

		ImGui::EndChild();
		ImGui::End();
	}

public:
	GUISystem(entt::registry& reg, EventQueue& queue) : registry(reg), eventQueue(queue) {}

	void Update()
	{
		ProcessEventQueue();
	}

	void Draw(ShapeRendererPtr shapeRenderer)
	{
		RenderPlayerControls();
		RenderNPCControls();
		RenderAnimationControls();
		RenderDebugOverlays();
		DrawAABB(shapeRenderer);

		// Show debug messages
		DrawDebugMessages();
	}
};
