#pragma once
#include "Sphere.hpp"
#include "AABB.hpp"
#include "Plane.hpp"
#include "SphereColliderComponent.hpp"
#include "AABBColliderComponent.hpp"

namespace CollisionTest
{
	bool TestSphereSphere(const Sphere& a, const Sphere& b)			// Check if the distance between the two centers is less than the sum of radius.
	{
		glm::vec3 diff = b.center - a.center;
		float radiusSum = a.radius + b.radius;
		return glm::dot(diff, diff) <= radiusSum * radiusSum;
	}

	bool TestSpherePlane(const Sphere& s, const Plane& p)
	{
		float dist = p.DistanceToPoint(s.center);
		return std::abs(dist) <= s.radius;
	}

	bool TestAABBAABB(const AABB& a, const AABB& b)					// Check similar test as with Sphere but along each axis
	{
		float centerDiff = std::abs(a.center[0] - b.center[0]);
		float compoundedWidth = a.halfWidths[0] + b.halfWidths[0];
		if (centerDiff > compoundedWidth) return false;

		centerDiff = std::abs(a.center[1] - b.center[1]);
		compoundedWidth = a.halfWidths[1] + b.halfWidths[1];
		if (centerDiff > compoundedWidth) return false;

		centerDiff = std::abs(a.center[2] - b.center[2]);
		compoundedWidth = a.halfWidths[2] + b.halfWidths[2];
		if (centerDiff > compoundedWidth) return false;

		return true;
	}

	bool TestAABBPlane(const AABB& ab, const Plane& p)
	{
		float r = ab.halfWidths[0] * std::abs(p.normal.x) +
				  ab.halfWidths[1] * std::abs(p.normal.y) +
				  ab.halfWidths[2] * std::abs(p.normal.z);
		
		float s = p.DistanceToPoint(ab.center);
		return std::abs(s) <= r;
	}
}