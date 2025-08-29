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
#include "Sphere.hpp"
#include "BVH.hpp"

std::vector<Sphere*> FindPossibleCollisions(SphereNode* treeRoot, Sphere* sphere)
{
	std::vector<Sphere*> possibleCollisions;

	if (!sphere || !treeRoot) return possibleCollisions;

	if (!CollisionTest::TestSphereSphere(*treeRoot->collisionRepresentation, *sphere)) return possibleCollisions;

	if (!treeRoot->leftChild && !treeRoot->rightChild)
	{
		possibleCollisions.push_back(treeRoot->collisionRepresentation);
		return possibleCollisions;
	}

	auto collisions = FindPossibleCollisions(treeRoot->leftChild.get(), sphere);
	possibleCollisions.insert(possibleCollisions.end(), collisions.begin(), collisions.end());

	collisions = FindPossibleCollisions(treeRoot->rightChild.get(), sphere);
	possibleCollisions.insert(possibleCollisions.end(), collisions.begin(), collisions.end());

	return possibleCollisions;
}

void CollisionSystem(entt::registry& registry, EventQueue& eventQueue)
{
	auto view = registry.view<TransformComponent, CollisionComponent>();

	std::vector<Sphere*> spheres;
	std::unordered_map<Sphere*, entt::entity> sphereToEntity;

	for (auto entity : view)
	{
		auto& t = view.get<TransformComponent>(entity);
		auto& c = view.get<CollisionComponent>(entity);

		c.boundingSphere.center = t.position;
		c.boundingBox.center = t.position;

		spheres.push_back(&c.boundingSphere);
		sphereToEntity[&c.boundingSphere] = entity;
	}

	auto root = BuildBVHBottomUp(spheres, 100.0f);

	// Broad-phase
	for (auto entity1 : view)
	{
		auto& c1 = view.get<CollisionComponent>(entity1);
		auto candidates = FindPossibleCollisions(root.get(), &c1.boundingSphere);
		
		for (auto s : candidates)
		{
			entt::entity entity2 = sphereToEntity[s];
			if (entity1 == entity2) continue;

			auto& c2 = view.get<CollisionComponent>(entity2);

			// Narrow-phase
			if (!CollisionTest::TestAABBAABB(c1.boundingBox, c2.boundingBox)) continue;

			// Quest triggers
			if (c1.isTrigger || c2.isTrigger)
			{
				bool entity1IsPlayer = registry.all_of<PlayerControllerComponent>(entity1);
				bool entity2IsPlayer = registry.all_of<PlayerControllerComponent>(entity2);

				if (entity1IsPlayer && registry.all_of<FoodTriggerSource>(entity2)) registry.get<FoodTriggerSource>(entity2).PlayerEntered();

				if (entity1IsPlayer && registry.all_of<HorseTriggerSource>(entity2)) registry.get<HorseTriggerSource>(entity2).PlayerEntered();

				if (entity2IsPlayer && registry.all_of<FoodTriggerSource>(entity1)) registry.get<FoodTriggerSource>(entity1).PlayerEntered();

				if (entity2IsPlayer && registry.all_of<HorseTriggerSource>(entity1)) registry.get<HorseTriggerSource>(entity1).PlayerEntered();

				if (entity1IsPlayer && registry.all_of<FeedHorseSource>(entity2)) registry.get<FeedHorseSource>(entity2).PlayerEntered();
							
				else if (entity2IsPlayer && registry.all_of<FeedHorseSource>(entity1)) registry.get<FeedHorseSource>(entity1).PlayerEntered();
			}
			// Collision resolve
			else if (registry.all_of<RigidBodyComponent>(entity1) && registry.all_of<RigidBodyComponent>(entity2))
			{
				auto& rb1 = registry.get<RigidBodyComponent>(entity1);
				auto& rb2 = registry.get<RigidBodyComponent>(entity2);
				auto& t1 = registry.get<TransformComponent>(entity1);
				auto& t2 = registry.get<TransformComponent>(entity2);

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