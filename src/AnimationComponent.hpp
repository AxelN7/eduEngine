#pragma once
#include <glm/glm.hpp>
#include "AnimState.hpp"
#include <vector>

struct AnimationClip
{
	int clipIndex;									// Index representing an animation clip
	float animTime;									// Time for an animation clip
};

struct AnimationComponent
{
	float blendTimer = 0.0f;						// Progress timer for the blend
	float blendFactor = 0.0f;						// Current blend factor
	float blendDuration = 0.5f;						// Duration for a full blend

	float jumpDuration = 2.2f;						// Total duration of the Jump clip
	float jumpBlendTimer = 0.0f;					// Progress of Jump blend
	float jumpBlendFactor = 0.0f;					// Current Jump blend factor
	bool jumpBlendOut = false;						// Indicates if Jump is blending out

	float animationSpeed = 1.0f;					// Speed multiplier for animation

	AnimState currentState = AnimState::Idle;		// Current animation state
	AnimState targetState = AnimState::Idle;		// State to transition to
	AnimState previousState = AnimState::Idle;		// Previous animation state

public:
	std::vector<AnimationClip> animations;			// Collection of animation clips
};