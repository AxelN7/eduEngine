#pragma once
#include <glm/glm.hpp>
#include "AnimState.hpp"

struct AnimationComponent
{
	float idleTime = 0.0f;		//Animation time for Idle
	float walkTime = 0.0f;		//Animation time for Walk
	float blendTimer = 0.0f;	//Timer on blend between 0.0 and blendDuration (Idle and Walk)
	float blendFactor = 0.0f;	
	float blendDuration = 0.5f;	//Time to fully blend

	float jumpTimer = 0.0f;		//Timer on Jump animation
	float jumpDuration = 2.2f;	//Total animation time for Jump
	float jumpBlendTimer = 0.0f; //Timer on blend between 0.0 and jumpBlendDuration (Idle/Walk and Jump)
	float jumbBlendDuration = 0.5f;	//Time to fully blend
	float jumpBlendFactor = 0.0f;
	bool jumpBlendOut = false;	//Check if blending out

	float animationSpeed = 1.0f;

	AnimState currentState = AnimState::Idle;
	AnimState targetState = AnimState::Idle;
	AnimState previousState = AnimState::Idle;
};