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

		// Const operators, they return a new vector without changing itself
		constexpr Vector4 operator+() const { return { +x, +y, +z, +w }; }
		constexpr Vector4 operator+(const Vector4& rhs) const { return { x + rhs.x, y + rhs.y, z + rhs.z , w + rhs.w }; }
		constexpr Vector4 operator-() const { return { -x, -y, -z , +w }; }
		constexpr Vector4 operator-(const Vector4& rhs) const { return { x - rhs.x, y - rhs.y, z - rhs.z , w - rhs.w}; }
		constexpr Vector4 operator*(const float rhs) const { return { x * rhs, y * rhs, z * rhs , w * rhs }; }
		constexpr Vector4 operator/(const float rhs) const { return { x / rhs, y / rhs, z / rhs , w / rhs }; }

		// Mutating operators, these changes itself and return itself out
		Vector4& operator+=(const Vector4& rhs) { x += rhs.x; y += rhs.y; z += rhs.z ; w += rhs.w; return *this; };
		Vector4& operator-=(const Vector4& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z ; w -= rhs.w; return *this; };
		Vector4& operator*=(float rhs) { x *= rhs; y *= rhs; z *= rhs; w *= rhs; return *this; };
		Vector4& operator/=(float rhs) { x /= rhs; y /= rhs; z /= rhs; w /= rhs; return *this; };
	};
}
