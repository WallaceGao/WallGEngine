#include "Precompiled.h"
#include "PhysicsWorld.h"

using namespace WallG;
using namespace WallG::Physics;

void PhysicsWorld::Initialize()
{
}

void PhysicsWorld::Update(float deltaTime)
{
	AccumulateForce();
	Integrate();
	SatisfyConstraints();
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

}

void WallG::Physics::PhysicsWorld::Integrate()
{
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
