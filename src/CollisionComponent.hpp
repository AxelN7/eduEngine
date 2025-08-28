#pragma once
#include "Sphere.hpp"
#include "AABB.hpp"
#include <glm/glm.hpp>

struct CollisionComponent
{
	Sphere boundingSphere;
	AABB boundingBox;
	bool isTrigger = false;

	CollisionComponent(const Sphere& sphere, const AABB& ab, bool trigger = false) 
		: boundingSphere(sphere), boundingBox(ab), isTrigger(trigger) {}
};

struct RigidBodyComponent
{
	float mass = 1.0f;
};
