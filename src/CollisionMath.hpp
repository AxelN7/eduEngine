#pragma once
#include "Sphere.hpp"
#include "AABB.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <cmath>

namespace CollisionMath
{
	std::vector<glm::ivec2> FindMinMaxValues(const glm::vec3 points[], int numPoints)		// Find min/max values along each axis
	{
		glm::ivec2 minMaxX{ 0,0 };
		glm::ivec2 minMaxY{ 0,0 };
		glm::ivec2 minMaxZ{ 0,0 };

		for (int i = 0; i != numPoints; ++i)
		{
			glm::vec3 pt = points[i];

			if (pt.x < points[minMaxX.x].x) minMaxX.x = i;
			if (pt.x > points[minMaxX.y].x) minMaxX.y = i;

			if (pt.y < points[minMaxY.x].y) minMaxY.x = i;
			if (pt.y > points[minMaxY.y].y) minMaxY.y = i;

			if (pt.z < points[minMaxZ.x].z) minMaxZ.x = i;
			if (pt.z > points[minMaxZ.y].z) minMaxZ.y = i;
		}
		return std::vector<glm::ivec2>{ minMaxX, minMaxY, minMaxZ };
	}

	glm::ivec2 FindMostDistantPoints(const std::vector<glm::ivec2>& minMaxPoints, const glm::vec3 points[])
	{
		glm::vec3 xVec = points[minMaxPoints[0].y] - points[minMaxPoints[0].x];
		float xDist = glm::length(xVec);

		glm::vec3 yVec = points[minMaxPoints[1].y] - points[minMaxPoints[1].x];
		float yDist = glm::length(yVec);

		glm::vec3 zVec = points[minMaxPoints[2].y] - points[minMaxPoints[2].x];
		float zDist = glm::length(zVec);

		glm::ivec2 maxDist = minMaxPoints[0];
		
		if (yDist > xDist && yDist > zDist) maxDist = minMaxPoints[1];
		if (zDist > xDist && zDist > yDist) maxDist = minMaxPoints[2];

		return maxDist;
	}

	Sphere BuildSphereFromPoints(const glm::vec3 points[], int numPoints)
	{
		auto minMaxVectors = FindMinMaxValues(points, numPoints);
		auto mostDistantPoints = FindMostDistantPoints(minMaxVectors, points);

		Sphere s;
		s.center = (points[mostDistantPoints.x] + points[mostDistantPoints.y]) * 0.5f;
		s.radius = glm::length(points[mostDistantPoints.y] - s.center);
		return s;
	}

	AABB BuildAABBFromPoints(const glm::vec3 points[], int numPoints)
	{
		auto minMaxVectors = FindMinMaxValues(points, numPoints);

		glm::vec3 minPoint(points[minMaxVectors[0].x].x,
						   points[minMaxVectors[1].x].y,
						   points[minMaxVectors[2].x].z);
		glm::vec3 maxPoint(points[minMaxVectors[0].y].x,
			               points[minMaxVectors[1].y].y,
			               points[minMaxVectors[2].y].z);

		AABB ab;
		ab.center = (minPoint + maxPoint) * 0.5f;
		for (int i = 0; i < 3; ++i)
		{
			ab.halfWidths[i] = (maxPoint[i] - minPoint[i]) * 0.5f;
		}
		return ab;
	}
}