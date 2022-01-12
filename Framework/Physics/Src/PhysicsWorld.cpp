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
	if (mPause)
	{
		return;
	}
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
	for (auto& c: mConstraints)
	{
		c->DebugDraw();
	}
	for (auto& p : mParticles)
	{
		Graphics::SimpleDraw::AddSphere(p->position, p->radius,Colors::Orange, 3, 4);
	}
}

void PhysicsWorld::DebugUI()
{
	ImGui::CollapsingHeader("Physics", ImGuiTreeNodeFlags_DefaultOpen);
	{
		ImGui::DragFloat3("Gravity", &mSettings.gravity.x, 0.1f, 0.0f, 10.0f);
		ImGui::Checkbox("Time Step", &mPause);
	}
}

WallG::Physics::Particle* PhysicsWorld::AddParticle()
{
	return mParticles.emplace_back(std::make_unique<Particle>()).get();
}

void PhysicsWorld::Clear()
{
	mConstraints.clear();
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
	for (int n = 0; n < mSettings.iterations; ++n)
	{
		for (auto& c : mConstraints)
			c->Apply();
	}
}
