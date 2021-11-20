#include "Precompiled.h"
#include "WanderBehavior.h"
#include "Agent.h"

using namespace WallG;
using namespace WallG::AI;

Math::Vector2 WanderBehavior::Calculate(Agent& agent)
{
	wanderTarget += {Math::RandomFloat(-1.0f, 1.0f)* wanderJitter, Math::RandomFloat(-1.0f, 1.0f)* wanderJitter};
	wanderTarget = Math::Normalize(wanderTarget);
	wanderTarget *= wanderRadius;
	
	Math::Vector2 localTarget = wanderTarget + Math::Vector2{0.0f, wanderDistance};
	Math::Vector2 worldTarget = Math::TransformCoord(localTarget, agent.LocalToWorld());

	Math::Vector2 centerLocal = localTarget - wanderTarget;
	Math::Vector2 centerWorld = Math::TransformCoord(centerLocal, agent.LocalToWorld());

	//DrawScreenCircle(centerWorld, wanderRadius, Colors::Red);
	//DrawScreenCircle(worldTarget, 10.0f, Colors::Aqua);

	auto toTarget = worldTarget - agent.position;
	auto desiredVelocity = Math::Normalize(toTarget) * agent.maxSpeed;
	return desiredVelocity - agent.velocity;
}
