#include "Precompiled.h"
#include "Cohesion.h"
#include "Agent.h"

using namespace WallG;
using namespace WallG::AI;
using namespace WallG::Math;

Math::Vector2 AI::Cohesion::Calculate(Agent& agent)
{
	if (agent.neighbors.size() == 1)
	{
	 	return Vector2::Zero;
	}
	Vector2 avgPosition;
	for (auto& neighbor : agent.neighbors)
	{
		avgPosition += neighbor->position;
	}
	if (ShowDebug())
	{
	}
	Vector2 center =  avgPosition / static_cast<float>(agent.neighbors.size());

	auto toTarget = center - agent.position;
	auto desiredVelocity = Normalize(toTarget) * agent.maxSpeed;
	return desiredVelocity - agent.velocity;
}
