#include "Precompiled.h"
#include "PhysicsWorld.h"

using namespace WallG;
using namespace WallG::Physics;

void PhysicsWorld::Initialize(Settings settings)
{
	mSettings = std::move(settings);
}

void PhysicsWorld::Update(float deltaTime)
{
	mTimer += deltaTime;
	while (mTimer >= mSettings.timeStep)
	{
		mTimer -= mSettings.timeStep;
		AccumulateForce();
		Integrate();
		SatisfyConstraints();
	}
}

void PhysicsWorld::DebugDraw() const
{
	for (auto& p : mParticles)
		Graphics::SimpleDraw::AddSphere(p->position, p->radius, Colors::Cyan, 3, 4);
}

Particle* PhysicsWorld::AddParticle()
{
	return mParticles.emplace_back(std::make_unique<Particle>()).get();
}

void PhysicsWorld::AccumulateForce()
{
	for (auto& p : mParticles)
		SimpleDraw::AddSphere(p->position, p->radius, Colors::Orange, 3, 4);
}

void PhysicsWorld::Integrate()
{
	// Math::Sqr should be deltatime
	const float timeStepSqr = Math::Sqr(1 / 60.0f);
	for (auto& p : mParticles)
	{
		const auto newPosition = (p->position * 2) - p->lastPosition + (p->acceleration * timeStepSqr);
		p->lastPosition = p->position;
		p->position = newPosition;
	}
}

void WallG::Physics::PhysicsWorld::SatisfyConstraints()
{
}
