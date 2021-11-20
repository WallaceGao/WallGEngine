#pragma once
#include "SteeringBehavior.h"

namespace WallG::AI
{
	class ObstacleAvoidance : public SteeringBehavior
	{
	public:
		Math::Vector2 Calculate(Agent& agent);
		float brakingWeight = 0.8f;
		float lateralForce = 1.0f;

	private:
		float boxLength = 100.0f;
	};
}
