#include "Precompiled.h"
#include "SeekBehavior.h"
#include "Agent.h"

using namespace WallG;
using namespace WallG::AI;

Math::Vector2 SeekBehavior::Calculate(Agent& agent)
{
	auto toTarget = agent.destination - agent.position;
	auto desiredVelocity = Math::Normalize(toTarget) * agent.maxSpeed;
	return desiredVelocity - agent.velocity;
}
