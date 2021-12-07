#pragma once

namespace WallG::AI
{
	using property = std::variant<int, float, Math::Vector2>;

	struct MemoryRecord
	{
		int endtityId = 0;
		float lastRecordedTime = 0.0f;
		float importance = 0.0f;

	};
	using MemoryRecords = std::list<MemoryRecord>;
}
