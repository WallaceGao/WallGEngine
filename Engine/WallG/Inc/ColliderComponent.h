#pragma once

#include "Component.h"

namespace WallG
{
	class TransformComponent;

	class ColliderComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Collider);
		MEMPOOL_DECLARE;

		ColliderComponent() = default;

		void Initialize() override;
		void DebugUI() override;

		Math::AABB GetAABB() const;
		void SetCenter(const Math::Vector3& center) { mCenter = center; }
		void SetExtend(const Math::Vector3& extend) { mExtend = extend; }

	private:
		const TransformComponent* mTransformComponent;
		Math::Vector3 mCenter = Math::Vector3::Zero;
		Math::Vector3 mExtend = Math::Vector3::One;


	};
}
