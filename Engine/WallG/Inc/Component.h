#pragma once
#include "TypeIds.h"

namespace WallG
{
	class GameObject;


	class Component
	{
	public:
		Component() = default;
		virtual ~Component() = default;

		virtual uint32_t GetTypeId() const = 0;

		virtual void Initialize() {};
		virtual void Terminate() {};
		virtual void Update(float) {};
		virtual void DebugUI() {};

		GameObject& GetOwner() { return *mOwner; }
		const GameObject& GetOwner() const { return *mOwner; }

	private:
		friend class GameObject;
		GameObject* mOwner = nullptr;
	};
}
