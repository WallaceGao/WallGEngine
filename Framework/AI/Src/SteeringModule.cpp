#include "Precompiled.h"
#include "SteeringModule.h"

using namespace WallG;
using namespace WallG::AI;

SteeringModule::SteeringModule(Agent& agent)
    : mAgent(agent)
{}

Math::Vector2 SteeringModule::Calculate()
{
    Math::Vector2 totalForce;
    for (auto& behavior : mBehaviors)
    {
        if (behavior->IsActive())
        {
            totalForce += behavior->Calculate(mAgent);
        }
    }

    return totalForce;
}
