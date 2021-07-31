#pragma once

namespace WallG::Math
{
	struct AABB
	{
		Vector3 center;
		Vector3 extend;

		constexpr Vector3 Min() const { return center - extend; }
		constexpr Vector3 Max() const { return center + extend; }

	};
}