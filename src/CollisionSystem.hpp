#pragma once
#include <entt/entt.hpp>
#include "AABBColliderComponent.hpp"
#include "SphereColliderComponent.hpp"
#include "CollisionMath.hpp"
#include "CollisionTest.hpp"
#include "EventQueue.hpp"
#include "TransformComponent.hpp"
#include "CollisionComponent.hpp"
#include "Source.hpp"
#include "FoodTriggerSource.hpp"
#include "HorseTriggerSource.hpp"
#include "FeedHorseSource.hpp"
#include "PlayerControllerComponent.hpp"

void CollisionSystem(entt::registry& registry, EventQueue& eventQueue)
{
	auto view = registry.view<TransformComponent, CollisionComponent>();

	for (auto entity1 : view)
	{
		auto& t1 = view.get<TransformComponent>(entity1);
		auto& c1 = view.get<CollisionComponent>(entity1);

		c1.boundingSphere.center = t1.position;
		c1.boundingBox.center = t1.position;

		for (auto entity2 : view)
		{
			if (entity1 == entity2) continue;

			auto& t2 = view.get<TransformComponent>(entity2);
			auto& c2 = view.get<CollisionComponent>(entity2);

			// Broad-phase collision handling
			if (!CollisionTest::TestSphereSphere(c1.boundingSphere, c2.boundingSphere)) continue;

			// Narrow-phase collision handling
			if (!CollisionTest::TestAABBAABB(c1.boundingBox, c2.boundingBox)) continue;

			if (c1.isTrigger || c2.isTrigger)	// Quest triggers
			{
				bool entity1IsPlayer = registry.all_of<PlayerControllerComponent>(entity1);
				bool entity2IsPlayer = registry.all_of<PlayerControllerComponent>(entity2);

				if (entity1IsPlayer && registry.all_of<FeedHorseSource>(entity2))
				{
					registry.get<FeedHorseSource>(entity2).PlayerEntered();
				}
				else if (entity2IsPlayer && registry.all_of<FeedHorseSource>(entity1))
				{
					registry.get<FeedHorseSource>(entity1).PlayerEntered();
				}

				if (entity1IsPlayer && registry.all_of<FoodTriggerSource>(entity2))
					registry.get<FoodTriggerSource>(entity2).PlayerEntered();

				if (entity1IsPlayer && registry.all_of<HorseTriggerSource>(entity2))
					registry.get<HorseTriggerSource>(entity2).PlayerEntered();

				if (entity2IsPlayer && registry.all_of<FoodTriggerSource>(entity1))
					registry.get<FoodTriggerSource>(entity1).PlayerEntered();

				if (entity2IsPlayer && registry.all_of<HorseTriggerSource>(entity1))
					registry.get<HorseTriggerSource>(entity1).PlayerEntered();
			}
			else if (registry.all_of<RigidBodyComponent>(entity1) && registry.all_of<RigidBodyComponent>(entity2))
			{
				auto& rb1 = registry.get<RigidBodyComponent>(entity1);
				auto& rb2 = registry.get<RigidBodyComponent>(entity2);

				glm::vec3 delta = t2.position - t1.position;
				float dist = glm::length(delta);
				if (dist < 0.0001f) continue;

				float penetration = (c1.boundingSphere.radius + c2.boundingSphere.radius) - dist;
				if (penetration > 0.0f)
				{
					glm::vec3 normal = glm::normalize(delta);

					float totalMass = rb1.mass + rb2.mass;
					glm::vec3 correction = normal * (penetration / totalMass);

					t1.position -= correction * rb2.mass;
					t2.position += correction * rb1.mass;
				}
			}
		}
	}
}