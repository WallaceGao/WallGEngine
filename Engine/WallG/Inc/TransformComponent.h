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
		void SetRotation(const Math::Vector3& rotation) { mRotation = rotation; }
		const Math::Vector3& GetRotation() const { return mRotation; }

	private:
		Math::Vector3 mPosition;
		Math::Vector3 mRotation;
	};
}

