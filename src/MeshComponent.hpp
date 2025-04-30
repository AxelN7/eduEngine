#pragma once
#include "RenderableMesh.hpp"

struct MeshComponent
{
	std::weak_ptr<eeng::RenderableMesh> reference;
	bool drawSkeleton = false;
};