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
	{
		auto color = Math::Lerp(Colors::Red, Colors::Blue, Math::Clamp(p->radius / 0.25f, 0.0f, 1.0f));
		Graphics::SimpleDraw::AddSphere(p->position, p->radius, color, 3, 4);
	}
}

Particle* PhysicsWorld::AddParticle()
{
	return mParticles.emplace_back(std::make_unique<Particle>()).get();
}

void PhysicsWorld::Clear()
{
	mParticles.clear();
}

void PhysicsWorld::AccumulateForce()
{
	for (auto& p : mParticles)
		p->acceleration = mSettings.gravity;
}

void PhysicsWorld::Integrate()
{
	// Math::Sqr should be deltatime
	const float timeStepSqr = Math::Sqr(mSettings.timeStep);
	for (auto& p : mParticles)
	{
		const auto newPosition = (p->position * 2.0f) - p->lastPosition + (p->acceleration * timeStepSqr);
		p->lastPosition = p->position;
		p->position = newPosition;
	}
}

void WallG::Physics::PhysicsWorld::SatisfyConstraints()
{
}
