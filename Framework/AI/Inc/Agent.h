#pragma once
#include "Entity.h"

namespace WallG::AI
{
	class Agent;

	using Agents = std::vector<Agent*>;

	class Agent: public Entity
	{
	public:
		Agent(AIWorld& world, uint16_t typeId) : Entity(world, typeId)
		{
		}

		Agents neighbors;
		WallG::Math::Vector2 velocity = WallG::Math::Vector2::Zero;
		WallG::Math::Vector2 destination = WallG::Math::Vector2::Zero;
		float maxSpeed = 1.0f;
		float mass = 1.0f;
	};
}
