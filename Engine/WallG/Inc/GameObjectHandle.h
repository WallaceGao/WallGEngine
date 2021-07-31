#pragma once

#include "Common.h"

namespace WallG
{
	class GameObjectHandle
	{
	public:

		GameObjectHandle() = default;

	private:
		friend class GameWorld;
		uint32_t mID = -1;
		uint32_t mGeneration = -1;
	};
}