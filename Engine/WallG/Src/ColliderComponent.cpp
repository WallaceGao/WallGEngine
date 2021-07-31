#include "Precompiled.h"
#include "ColliderComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"

using namespace WallG;

MEMPOOL_DEFINE(ColliderComponent,1000);

void ColliderComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
}

void ColliderComponent::DebugUI()
{
	const auto aabb = GetAABB();
	Graphics::SimpleDraw::AddAABB(aabb.center, aabb.extend, Graphics::Colors::Aqua);
}

Math::AABB ColliderComponent::GetAABB() const
{
	Math::AABB aabb;
	aabb.center = mTransformComponent->GetPosition() + mCenter;
	aabb.extend = mExtend;
	return aabb;
}