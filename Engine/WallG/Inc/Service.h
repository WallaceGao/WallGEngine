#pragma once
#include "TypeIds.h"

namespace WallG
{
	class GameWorld;

	class Service
	{
	public:
		Service() = default;
		virtual ~Service() = default;

		virtual uint32_t GetTypeId() const = 0;

		virtual void Initialize() {};
		virtual void Terminate() {};
		virtual void Update(float) {};
		virtual void Render() {};
		virtual void DebugUI() {};

		GameWorld& GetOwner() { return *mWorld; }
		const GameWorld& GetOwner() const { return *mWorld; }

	private:
		friend class GameWorld;
		GameWorld* mWorld = nullptr;
	};
}
