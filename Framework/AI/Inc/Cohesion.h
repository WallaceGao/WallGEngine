#pragma once
#include "SteeringBehavior.h"

namespace WallG::AI
{
	class Cohesion : public SteeringBehavior
	{
	public:
		Math::Vector2 Calculate(Agent& agent);

	};
}
