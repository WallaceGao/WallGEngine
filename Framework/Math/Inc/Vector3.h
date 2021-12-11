#pragma once

namespace WallG::Math
{
	struct  Vector3
	{
		float x, y, z;
		//constexpr - constant expression
		//expression means the code clause evaluates to a single value of a single type
		constexpr Vector3() noexcept : Vector3{ 0.0f } {}
		constexpr explicit Vector3(float f) noexcept : Vector3{ f, f, f } {}
		constexpr Vector3(float x, float y, float z) noexcept : x{ x }, y{ y }, z{ z } {}
	

		// Commonly used vectors
		const static Vector3 Zero;
		const static Vector3 One;
		const static Vector3 XAxis;
		const static Vector3 YAxis;
		const static Vector3 ZAxis;

		// Const operators, they return a new vector without changing itself
		constexpr Vector3 operator+() const { return { +x, +y, +z }; }
		constexpr Vector3 operator+(const Vector3& rhs) const { return { x + rhs.x, y + rhs.y, z + rhs.z}; }
		constexpr Vector3 operator-() const { return { -x, -y, -z }; }
		constexpr Vector3 operator-(const Vector3& rhs) const { return { x - rhs.x, y - rhs.y, z - rhs.z }; }
		constexpr Vector3 operator*(const float rhs) const { return { x* rhs, y* rhs, z* rhs }; }
		constexpr Vector3 operator/(const float rhs) const { return { x/ rhs, y/ rhs, z/ rhs }; }

		// Mutating operators, these changes itself and return itself out
		Vector3& operator+=(const Vector3& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; };
		Vector3& operator-=(const Vector3& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; };
		Vector3& operator*=(float rhs) { x *= rhs; y *= rhs; z *= rhs; return *this; };
		Vector3& operator/=(float rhs) { x /= rhs; y /= rhs; z /= rhs; return *this; };
	};
}
