#pragma once
#include "Component.h"

namespace WallG
{
	class TransformComponent final: public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Transform);
		MEMPOOL_DECLARE;
		TransformComponent() = default;

		void DebugUI();

		void SetPosition(const Math::Vector3& postion) { mPosition = postion;}
		const Math::Vector3& GetPosition() const { return mPosition; }
		void SetRotation(const Math::Quaternion& rotation) { mRotation = rotation; }
		const Math::Quaternion& GetRotation() const { return mRotation; }
		void SetScale(const Math::Vector3& scale) { mScale = scale; }
		const Math::Vector3& GetScale() const { return mScale; }

	private:
		Math::Vector3 mPosition = Math::Vector3::Zero;
		Math::Quaternion mRotation = Math::Quaternion::Identity;
		Math::Vector3 mScale = Math::Vector3::One;
	};
}

