#pragma once
#include "Sphere.hpp"
#include <vector>
#include <glm/glm.hpp>

struct SphereNode
{
	std::unique_ptr<Sphere> ownedSphere;	// Used for internal nodes
	Sphere* collisionRepresentation;
	std::unique_ptr<SphereNode> leftChild;
	std::unique_ptr<SphereNode> rightChild;

	SphereNode(std::unique_ptr<Sphere> sphere, std::unique_ptr<SphereNode> left, std::unique_ptr<SphereNode> right) 
		: ownedSphere(std::move(sphere)), collisionRepresentation(ownedSphere.get()), leftChild(std::move(left)), rightChild(std::move(right)) {}

	SphereNode(Sphere* ecsSphere) : collisionRepresentation(ecsSphere) {}
};

float DistanceBetweenSpheres(Sphere* leftSphere, Sphere* rightSphere)
{
	float centerDist = (rightSphere->center - leftSphere->center).length();
	float surfaceDist = centerDist - (leftSphere->radius + rightSphere->radius);
	return std::max(0.0f, surfaceDist);
}

void FindMinMaxPoints(glm::vec3 leftCenter, glm::vec3 rightCenter, float leftRadius, float rightRadius, glm::vec3& minOut, glm::vec3& maxOut)
{
	minOut.x = std::min(leftCenter.x - leftRadius, rightCenter.x - rightRadius);
	maxOut.x = std::max(leftCenter.x + leftRadius, rightCenter.x + rightRadius);

	minOut.y = std::min(leftCenter.y - leftRadius, rightCenter.y - rightRadius);
	maxOut.y = std::max(leftCenter.y + leftRadius, rightCenter.y + rightRadius);

	minOut.z = std::min(leftCenter.z - leftRadius, rightCenter.z - rightRadius);
	maxOut.z = std::max(leftCenter.z + leftRadius, rightCenter.z + rightRadius);
}

std::unique_ptr<SphereNode> BuildNodeFromSingleSphere(Sphere* sphere)
{
	return std::make_unique<SphereNode>(sphere);
}

std::unique_ptr<SphereNode> BuildNodeFromSpheres(Sphere* leftSphere, Sphere* rightSphere)
{
	glm::vec3 centerDelta = rightSphere->center - leftSphere->center;
	float dist = glm::length(centerDelta);

	float radius = dist * 0.5f + std::max(leftSphere->radius, rightSphere->radius);
	glm::vec3 midPoint = leftSphere->center + centerDelta * 0.5f;

	auto newSphere = std::make_unique<Sphere>(midPoint, radius);
	return std::make_unique<SphereNode>(std::move(newSphere), nullptr, nullptr);
}

std::vector<std::pair<SphereNode*, SphereNode*>> FindPairs(std::vector<SphereNode*> openList, float maxDistance)
{
	std::vector<std::pair<SphereNode*, SphereNode*>> allPairs;
	std::vector<SphereNode*> availableSpheres = openList;

	while (!availableSpheres.empty())
	{
		SphereNode* current = availableSpheres.back();
		availableSpheres.pop_back();

		float closestDistance = maxDistance;
		SphereNode* bestMatch = nullptr;
		int bestIndex = -1;

		for (int i = 0; i < availableSpheres.size(); ++i)
		{
			float distance = DistanceBetweenSpheres(current->collisionRepresentation, availableSpheres[i]->collisionRepresentation);

			if (distance < closestDistance)
			{
				closestDistance = distance;
				bestMatch = availableSpheres[i];
				bestIndex = i;
			}
		}

		if (bestMatch)
		{
			availableSpheres.erase(availableSpheres.begin() + bestIndex);
		}
		allPairs.push_back({ current, bestMatch });
	}
	return allPairs;
}

std::unique_ptr<SphereNode> BuildBVHBottomUp(std::vector<Sphere*> spheres, float maxDistanceBetweenLeaves)
{
	std::vector<std::unique_ptr<SphereNode>> nodes;
	nodes.reserve(spheres.size());

	for (auto* s : spheres)
	{
		nodes.push_back(std::make_unique<SphereNode>(s));
	}

	while (nodes.size() > 1)
	{
		auto left = std::move(nodes.back()); 
		nodes.pop_back();
		auto right = std::move(nodes.back());
		nodes.pop_back();

		auto parent = BuildNodeFromSpheres(left->collisionRepresentation, right->collisionRepresentation);

		parent->leftChild = std::move(left);
		parent->rightChild = std::move(right);

		nodes.push_back(std::move(parent));
	}
	return std::move(nodes.front());
}
