#pragma once

namespace WallG::Math
{
	struct  Vector2
	{
		float x, y;
		//constexpr - constant expression
		//expression means the code clause evaluates to a single value of a single type
		constexpr Vector2() noexcept : Vector2{ 0.0f } {}
		constexpr Vector2(float f) noexcept : Vector2{ f, f} {}
		constexpr Vector2(float x, float y) noexcept : x{ x }, y{ y } {}


		// Commonly used vectors
		const static Vector2 Zero;
		const static Vector2 One;
		const static Vector2 XAxis;
		const static Vector2 YAxis;

		// Const operators, they return a new vector without changing itself
		constexpr Vector2 operator+() const { return { +x, +y}; }
		constexpr Vector2 operator+(const float rhs) const { return { x + rhs, y + rhs }; }
		constexpr Vector2 operator+(const Vector2& rhs) const { return { x + rhs.x, y + rhs.y}; }
		constexpr Vector2 operator-() const { return { -x, -y }; }
		constexpr Vector2 operator-(const float rhs) const { return { x + rhs, y + rhs }; }
		constexpr Vector2 operator-(const Vector2& rhs) const { return { x - rhs.x, y - rhs.y }; }
		constexpr Vector2 operator*(const float rhs) const { return { x * rhs, y * rhs}; }
		constexpr Vector2 operator/(const float rhs) const { return { x / rhs, y / rhs}; }

		// Mutating operators, these changes itself and return itself out
		Vector2& operator+=(const Vector2& rhs) { x += rhs.x; y += rhs.y; return *this; };
		Vector2& operator+=(const float rhs) { x += rhs, y += rhs; return *this; };
		Vector2& operator-=(const Vector2& rhs) { x -= rhs.x; y -= rhs.y; return *this; };
		Vector2& operator-=(const float rhs) { x -= rhs, y -= rhs; return *this; };
		Vector2& operator*=(float rhs) { x *= rhs; y *= rhs; return *this; };
		Vector2& operator/=(float rhs) { x /= rhs; y /= rhs; return *this; };
	};
}
#pragma once
