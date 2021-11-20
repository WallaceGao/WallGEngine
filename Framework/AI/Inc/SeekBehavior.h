#pragma once
#include "SteeringBehavior.h"

namespace WallG::AI
{
	class SeekBehavior: public SteeringBehavior
	{
	public:
		Math::Vector2 Calculate(Agent& agent);
	private: 


	};
}