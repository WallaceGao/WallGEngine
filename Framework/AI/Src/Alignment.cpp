#include "Precompiled.h"
#include "Alignment.h"
#include "Agent.h"

using namespace WallG;
using namespace WallG::AI;
using namespace WallG::Math;

Math::Vector2 Alignment::Calculate(Agent& agent)
{
	Math::Vector2 avgHeading;
	for (auto& neighbor : agent.neighbors)
	{
		avgHeading += neighbor->heading;
	}
	avgHeading /= static_cast<float>(agent.neighbors.size());
	

	return avgHeading-agent.heading;
}
