#pragma once
#include "SteeringBehavior.h"

namespace WallG::AI
{
	class Alignment : public SteeringBehavior
	{
	public:
		Math::Vector2 Calculate(Agent& agent);

	};
}
