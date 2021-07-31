#pragma once

namespace WallG::Math
{
	struct Vector4
	{
		// union main x same as r
		union { float x; float r; };
		union { float y; float g; };
		union { float z; float b; };
		union { float w; float a; };

		constexpr Vector4() noexcept : Vector4{ 0.0f } {}
		constexpr Vector4(float f) noexcept : Vector4{ f, f, f, f } {}
		constexpr Vector4(const Vector3& v, float w) noexcept : Vector4{ v.x, v.y, v.z, w } {}
		constexpr Vector4(float x, float y, float z, float w) noexcept : x{ x }, y{ y }, z{ z }, w{ w } {}
	};
}
