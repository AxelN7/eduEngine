#pragma once
#include <glm/glm.hpp>
#include "AnimState.hpp"

struct AnimationComponent
{
	float time0 = 0.0f;
	float time1 = 0.0f;
	float blendTimer = 0.0f;
	float blendFactor = 0.0f;
	float blendDuration = 0.5f;
	AnimState currentState = AnimState::Idle;
	AnimState targetState = AnimState::Idle;
};