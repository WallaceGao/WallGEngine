#pragma once
#include "SteeringBehavior.h"

namespace WallG::AI
{
	class WanderBehavior: public SteeringBehavior
	{
	public:
		Math::Vector2 Calculate(Agent& agent);
		float wanderRadius = 50.0f;
		float wanderDistance = 50.0f;
		float wanderJitter = 30.0f;
	
	private: 
		Math::Vector2 wanderTarget;
	};
}
