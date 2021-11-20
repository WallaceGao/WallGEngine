#include "Precompiled.h"
#include "ObstacleAvoidance.h"
#include "AIWorld.h"
#include "Agent.h"

using namespace WallG;
using namespace WallG::AI;

Math::Vector2 ObstacleAvoidance::Calculate(Agent& agent)
{
	AIWorld::Obstacles worldObstcles = agent.world.GetObstacles();
	// what is main some thing proportional to velocity
	float avoidanceDistance = boxLength + Math::Magnitude(agent.velocity)/agent.maxSpeed*boxLength;

	AIWorld::Obstacles localObstcles;
	Math::Matrix3 toLocal = Math::Inverse(agent.LocalToWorld());
	for (auto wo : worldObstcles)
	{
		if (Math::Distance(agent.position, wo.center) - wo.radius > avoidanceDistance)
		//if (X::Math::DistanceSqr(agent.position, wo.center) - wo.radius * wo.radius > avoidanceDistance * avoidanceDistance)
			continue;
		Math::Vector2 localCenter = Math::TransformCoord(wo.center, toLocal);
		if (localCenter.y < 0)
		{
			continue;
		}
		localObstcles.push_back({localCenter, wo.radius + agent.radius});
	}
	Math::Matrix3 localToWorld = agent.LocalToWorld();
	//Debug
	Math::Vector2 boxExtendTLWorld = Math::TransformCoord({ agent.radius,0.0f }, localToWorld);
	Math::Vector2 boxExtendTRWorld = Math::TransformCoord({ agent.radius , avoidanceDistance }, localToWorld);
	Math::Vector2 boxExtendBLWorld = Math::TransformCoord({ -agent.radius,0.0f }, localToWorld);
	Math::Vector2 boxExtendBRWorld = Math::TransformCoord({ -agent.radius , avoidanceDistance }, localToWorld);
	//DrawScreenLine({ boxExtendTLWorld }, { boxExtendTRWorld }, Colors::AliceBlue);
	//DrawScreenLine({ boxExtendBLWorld }, { boxExtendBRWorld }, Colors::AliceBlue);
	//DrawScreenLine({ boxExtendTLWorld }, { boxExtendBLWorld }, Colors::AliceBlue);
	//DrawScreenLine({ boxExtendTRWorld }, { boxExtendBRWorld }, Colors::AliceBlue);

	//bigest float number
	float closestY  = FLT_MAX;
	Math::Circle closestObstacle;
	for (auto lo : localObstcles)
	{
		if (Math::Abs(lo.center.x) > lo.radius)
		{
			continue;
		}
		// Apply equation of a circle with x= 0
		//(x−h)^2+(y−k)^2=r^2
		// x= 0
		// Keep smallest position closetY
		float ip = -sqrt(lo.radius* lo.radius - lo.center.x* lo.center.x) + lo.center.y;
		if (ip <= 0.0f)
		{
			ip = sqrt(lo.radius * lo.radius - lo.center.x * lo.center.x) + lo.center.y;
		}
		if (ip < closestY )
		{
			closestY = ip;
			closestObstacle = lo;
		}
	}
	if (closestY == FLT_MAX)
	{
		return Math::Vector2::Zero;
	}
	float lateral = (closestObstacle.radius - closestObstacle.center.x) * lateralForce + (avoidanceDistance - closestObstacle.center.y)/avoidanceDistance;
	float breaking = (closestObstacle.radius  - closestObstacle.center.y) * brakingWeight;
	Math::Vector2 localAvoidanceForce = { lateral , breaking };
	return Math::TransformNormal(localAvoidanceForce, agent.LocalToWorld());
}
