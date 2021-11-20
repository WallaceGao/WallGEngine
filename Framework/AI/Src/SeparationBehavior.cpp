#include "Precompiled.h"
#include "SeparationBehavior.h"
#include "Agent.h"

using namespace WallG;
using namespace WallG::AI;

Math::Vector2 SeparationBehavior::Calculate(Agent& agent)
{
	Math::Vector2 separation;
	for (auto& neighbor : agent.neighbors)
	{

		auto neighborToAgent = agent.position - neighbor->position;
		auto lengthSqr = Math::Magnitude(neighborToAgent);
		if (lengthSqr <= 0.0f)
		{
			continue;
		}
		auto scaledVector = neighborToAgent / lengthSqr;
		auto seperationForce = agent.maxSpeed * (1.0f - Math::Min(lengthSqr, 100.0f) / 100.0f);
		separation += scaledVector * seperationForce;
	}

	if (ShowDebug())
	{
	}
	return separation;
}
